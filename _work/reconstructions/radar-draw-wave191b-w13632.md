# Radar::draw wave 191b (w13632) — four confirmed structural divergences, no size-exact raw win

**Status:** complete, 2026-11-13. Worktree `wt/w13632`, branch `wave191b-w13632`,
base `master` `4abbb1be8`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13614.md` (RA flip family), `-w13606.md`
(pre-RA IR stability), `-w13593.md` (post-RA map), `-w13587.md` (weights).
**No source change landed**: `src/` is untouched; every measured cell is
raw-neutral (6,013) or worse and size-exact unless noted.

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,965 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,197 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |
| zero/`missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. New evidence: four real structural divergences, all raw-worse on the base

This pass reconstructed the affected blocks directly from the Thumb disassembly
and confirmed **four** places where the current source's evaluation order differs
from the original.  Each fix is individually size-exact (except where noted) and
each is a genuine correctness improvement, but **every one raises the raw byte
count** because the base's wrong RA colouring happens to align better.

### 1.1 Planet-loop preamble: `onScreen` first (known, re-confirmed)

Original `0x12e3f2`–`0x12e40c`:
```
update();  ldrb onScreen; cmp; beq;  stations->data()[i];  getStation()->getIndex();  ...
```
Current source evaluates `stationId`/`currentStation` before the `!onScreen`
guard.  Moving the guard first:

```cpp
this->update(this->planetTargets->data()[i]);
if (!this->onScreen) {
    continue;
}
int stationId = stations->data()[i];
int currentStation = Globals::status->getStation()->getIndex();
```

### 1.2 Music tail: original evaluates the mask **before** `alwaysEnemySeen`

Original `0x130280`–`0x1302a2`:
```
sub.w r3,r1,#136        ; A = current-136
cmp r3,#15 ; bhi stop   ; A
lsls r2,r3 ; tst 0xe071 ; beq stop   ; B = ((1<<A) & 0xe071)==0
cmp r1,#151 ; beq skip  ; C = (current != 151 && alwaysEnemySeen)
ldr alwaysEnemySeen ; eors #1 ; bne skip
```
So the source order is `A || B || C`, not the current `A || C || B`:

```cpp
if ((uint32_t) (current - 136) > 15 ||
    ((1 << (current - 136)) & 0xe071) == 0 ||
    __builtin_expect((current != 151 && alwaysEnemySeen), 0)) {  // lint: arm_builtin
```

### 1.3 Sound ladder: original tests `inAlienOrbit` first

Original `0x1302aa`–`0x1302c6`: `getStation()->isAttackedByAliens()` is *second*,
`inAlienOrbit()` first:
```
inAlienOrbit ; cbnz -> play 136
getStation()->isAttackedByAliens ; cbnz -> play 136
getCurrentCampaignMission == 16
```
Current source tests `isAttackedByAliens`, then `== 16`, then `inAlienOrbit`.
Correct order:

```cpp
if (Globals::status->inAlienOrbit() ||
    Globals::status->getStation()->isAttackedByAliens() ||
    Globals::status->getCurrentCampaignMission() == 16) {
```

### 1.4 Asteroid timer: `= 0` in the candidate arm, `+= mode` in the else arm (new)

This is the "asteroid candidate arm `= 0` + `+= mode` in else" fix.  Original
`0x12fd86`–`0x12fd94`:
```
cmp field_0x10, asteroids->data()[best]
itt ne ; movne r1,#0 ; strne r1,[r4,#412]   ; asteroidLockTimer = 0
str asteroids->data()[best],[r4,#16]         ; field_0x10 = ...
```
and original `0x12fe34`–`0x12fe42` (the `else` of
`if (!asteroidHit || isDockingToAsteroid())`):
```
ldr field_0x1b8 ; ldr asteroidLockTimer ; ldr mode
subs r1,#200 ; add r0,r2 ; str r0,[r4,#412] ; cmp r0,r1
```
Current source has `asteroidLockTimer += mode` in the `if (best > -1)` block and
no `+= mode` in the else arm.  Correct pair:

```cpp
if (best > -1) {
    if (this->field_0x10 != this->asteroids->data()[best]) {
        this->asteroidLockTimer = 0;          // was += mode
    }
    this->field_0x10 = static_cast<PlayerAsteroid *>(this->asteroids->data()[best]);
    asteroidHit = true;
}
...
} else {                                       // !asteroidHit || isDockingToAsteroid() == false
    this->lockedAsteroid = nullptr;
    auto hasScanner = this->hasScanner19;
    this->asteroidLockTimer += mode;           // added
    if (this->asteroidLockTimer > this->field_0x1b8 - 200) {
```

`f1` (`= 0`) alone is size-fatal (9,888, −20 B); `f2` (`+= mode` in else) alone
is size-fatal (9,916, +8 B); **`f1+f2` together are exactly size-exact**
(9,908) — they are a matched pair.

A fifth, smaller divergence: the true arm of the `candidateWreck ? ... :
lockedAsteroid == nullptr` test stores `asteroidLockTimer = 0` **before**
`field_0x10 = 0` in the original (`0x12fe22`–`0x12fe30`), while the source has
the reverse order.  Swapping them (`f3b`) is size-fatal (+4 B, 9,912) and
raw-worse (6,923), so it is not landable.

## 2. Cells measured this pass (all reverted; base 6,013 / 1,965 / 9,908)

### 2.1 The four structural fixes

| cell | raw | aligned | size | insn | `.text` diff | `.ARM.extab` diff |
|---|---:|---:|---:|---:|---:|---:|
| base | **6,013** | **1,965** | 9,908 | 3541 | 71,197 | 577 |
| `onScreen`-first (`os`) | 6,014 | 1,961 | 9,908 | 3541 | 71,198 | 577 |
| music `A,B,C` (`mus`) | 6,036 | 1,963 | 9,908 | 3542 | 71,220 | 577 |
| sound `inAlienOrbit`-first (`snd`) | 6,028 | 1,960 | 9,908 | 3541 | 71,212 | 577 |
| asteroid `f1+f2` (`f`) | 6,190 | 1,960 | 9,908 | 3541 | 71,374 | 577 |
| `mus+snd` | 6,036 | 1,958 | 9,908 | 3542 | 71,220 | 577 |
| `f+mus+snd+os` | 6,214 | **1,949** | 9,908 | 3542 | 71,398 | 577 |
| `f3b` (arm order) | 6,923 | 2,162 | **9,912** | 3541 | 84,481 | 574 |

`f+mus+snd+os` is the structurally most-correct size-exact cell and has the best
alignment seen (1,949 vs 1,965), but the worst raw of the size-exact set.

### 2.2 RA-flip family (`targets` local, from w13614) re-measured with the fixes

| cell | raw | aligned | size | insn | `.text` diff | `.ARM.extab` diff |
|---|---:|---:|---:|---:|---:|---:|
| `T` (`targets` local for `update`) | 6,697 | 2,079 | 9,908 | 3526 | – | – |
| `T+os` | 6,692 | 2,079 | 9,908 | 3526 | – | – |
| `T+snd` | 6,712 | 2,074 | 9,908 | 3526 | – | – |
| `T+mus+snd` | 6,720 | 2,068 | 9,908 | 3527 | – | – |
| `T+mus+snd+os` | 6,715 | 2,068 | 9,908 | 3527 | – | – |
| `T+f` | 6,877 | 2,079 | 9,908 | 3526 | – | – |
| `T+f+mus+snd+os` | 6,895 | 2,068 | 9,908 | 3527 | 72,079 | **529** |
| `T+f1+f2+f3b+mus+snd+os` | 7,622 | 2,194 | **9,912** | 3527 | 85,180 | **520** |

`T` does flip the colouring to the original's (`mov.w fp,#0` at the zero store,
`mov sl,r0` after `getCurrentCampaignMission`), but it hoists
`this->planetTargets` out of the planet loop (the original reloads
`[r4,#320]` at the header every iteration), so it is structurally wrong and
raw-worse.  Note `T+f+mus+snd+os` is the only family that *improves* `.ARM.extab`
(577 → 529/520), but it costs 800–1,600 raw bytes.

### 2.3 Fresh hint/loop-form probes (all size-exact, raw-worse)

| cell | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,013** | **1,965** | 9,908 |
| `__builtin_expect(!onScreen,0)` in planet loop | 6,029 | 2,047 | 9,908 |
| `__builtin_expect(data[i]==nullptr,0)` in planet loop | 6,029 | 2,047 | 9,908 |
| `__builtin_expect(campaign>=2,1)` on planet guard | 8,644 | 2,269 | **9,920** |
| `__builtin_expect(campaign>=2,0)` on planet guard | 9,088 | 3,809 | **9,936** |
| `__builtin_expect(!onScreen,1)` in planet loop | 9,431 | 2,866 | **9,900** |
| `int campaign` before `turretMode` (`cf`) | 8,559 | 1,956 | **9,900** |
| `cf + os` | 8,560 | 1,952 | **9,900** |
| `targets` local for `update`, header ldr kept | 6,697 | 2,079 | 9,908 |
| `targets` local for null-check (`t_null`) | 9,227 | 2,217 | **9,952** |
| `targets` local for size (`t_cond`) | 9,182 | 2,219 | **9,948** |
| `targets` local for lock block | 8,404 | 2,143 | **9,912** |
| `targets` local for planet guard | 8,791 | 2,238 | **9,924** |
| inlined `targets` expression (no local) | 9,350 | 2,281 | **9,868** |
| inlined `system` expression (no local) | 6,013 | 1,965 | 9,908 |
| landmark guard `expect(...,1)` | 9,476 | 2,306 | **9,896** |

## 3. Why the base is a local optimum

The raw metric is dominated by instruction-address drift, not by the RA colour
swap alone.  The per-256-byte diff profile of the base is nearly flat
(~220–250 differing bytes per 256-byte block) from `0x12e150` to `0x12f650`:
once the instruction stream diverges, every following instruction's bytes
differ.  The base has 3,541 instructions vs the original's 3,532 (9 extra,
mostly spill reloads); `T` has 3,526 (6 fewer).  A variant only wins if it
matches the original's instruction *stream*, and every IR-changing source
spelling tried (this pass and the ~1,000 cells in w13562–w13614) either leaves
the pre-RA IR byte-identical (raw stays exactly 6,013) or changes it in a way
that increases the drift.

The structurally correct cells (`f+mus+snd+os`, 1,949 aligned) are the best
*alignment* match but the worst *raw* match, because the correct fixes change
the instruction count and therefore the address alignment.  The base's wrong
RA colouring is a raw local minimum.

## 4. Conclusion

Four genuine structural divergences are confirmed and documented above
(planet-loop `onScreen` order, music `A||B||C` order, sound ladder
`inAlienOrbit`-first, asteroid `= 0`/`+= mode` pair).  All are individually
size-exact (the asteroid pair is exact only as a pair) and all are raw-worse on
the current base.  No size-exact raw win was found; `src/` is untouched and the
worktree is clean.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13632/`: `cell.py` (1.2 s
compile+link+strip+`fn_diff` cell), `cellf.py` (cell with extra `-mllvm` flags),
`flagcell.py` (per-TU flag injection through `build.ninja`), `secdiff.py`
(section byte diff), `cellext.py` (cell + `.text`/`.ARM.extab`/`.ARM.exidx`
diffs), `Radar.base.cpp` (pristine source), `orig.dis` / `ours.dis`,
`prera.mir` / `greedy.mir` / `vrr.mir` (`-print-machineinstrs` sections),
`adiff.py` (address-annotated aligned diff).
