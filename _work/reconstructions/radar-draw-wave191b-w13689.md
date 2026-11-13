# Radar::draw wave 191b (w13689) — enemy range-check order / landmark offset polarity /
# planet-loop guard: three new faithful divergences, no size-exact raw win

**Status:** complete, 2026-11-13. Worktree `wt/w13689`, branch `wave191b-w13689`,
base `master` `b3fd7e6a2`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13669.md` (notTurretMode/notOtherLock landed),
`-w13665.md` (gas-cloud store order landed), `-w13660.md`, `-w13656.md`,
`-w13647.md`, `-w13644.md`.

**No source change landed**: `src/` is untouched; every measured cell is
size-exact (9,908) and leaves `.ARM.extab` at 577.  Three new genuine
source-order divergences were reconstructed from the Thumb disassembly; none
is a raw win on the current base (6,013 / 1,955 / 9,908).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,955 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,270 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |
| zero / `missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. New evidence: three genuine source-order divergences

### 1.1 Enemy range check: `> 24000` before `< -24000` (`ER`)

Source (`Radar.cpp:714-716`) tests the lower bound first:

```cpp
if (!(this->egoPos.x - this->targetPos.x < -24000.0f) && !(this->egoPos.x - this->targetPos.x > 24000.0f) &&
    !(this->egoPos.y - this->targetPos.y < -24000.0f) && !(this->egoPos.y - this->targetPos.y > 24000.0f) &&
    !(this->egoPos.z - this->targetPos.z < -24000.0f) && !(this->egoPos.z - this->targetPos.z > 24000.0f) &&
    enemy->getType() != 16917) {
```

The original (`0x12ee3a`-`0x12ee4c`) tests the upper bound first:

```
12ee3a vcmpe.f32 s0, s18     ; s18 = +24000
12ee3e vmrs   APSR_nzcv, fpscr
12ee42 bgt.n  out            ; diff >  24000
12ee44 vcmpe.f32 s0, s16     ; s16 = -24000
12ee48 vmrs   APSR_nzcv, fpscr
12ee4c bmi.n  out            ; diff < -24000
```

Fix: swap each pair to `!(... > 24000.0f) && !(... < -24000.0f)`.

This is a real source-order divergence and makes the branch layout identical
to the original.  It also flips the `.text` literal pool at `0x12ec0c`
(ours `0x12ebd4`) from `[-24000, 60000, 24000, 100]` to
`[24000, 60000, -24000, 100]`, which is then **byte-identical** to the
original.  However the RA responds by assigning `+24000` to `s16` instead of
`s18` (and `-24000` to `s18` instead of `s16`), so the four `vldr` register
fields swap and the raw byte count is a wash.

Measured: raw **6,013** (tie), aligned 1,968 (+13), size 9,908,
`.text` 71,270 (tie), `.ARM.extab` 577.

### 1.2 Landmark `offset` ternary polarity (`OF`)

Source (`Radar.cpp:456`):

```cpp
int offset = i != 0 ? this->originY : this->originX;
```

The original (`0x12e922`-`0x12e93e`) defaults to `originY` (offset `0x230`)
and overrides `originX` (offset `0x22c`) on `i == 0`:

```
12e924 mov.w r9, #560        ; originY
12e92c cmp.w sl, #0
12e93c it    eq
12e93e moveq.w r9, #556      ; originX
```

Fix: `int offset = i == 0 ? this->originX : this->originY;`

Measured: raw 6,014 (+1), aligned 1,955 (tie), size 9,908,
`.text` 71,271, `.ARM.extab` 577.

### 1.3 Planet loop `onScreen` guard first (`os`)

Source (`Radar.cpp:546-550`):

```cpp
this->update(this->planetTargets->data()[i]);
int stationId = stations->data()[i];
int currentStation = Globals::status->getStation()->getIndex();
if (!this->onScreen) {
    continue;
}
```

The original (`0x12e3ee`-`0x12e404`) checks `onScreen` immediately after
`update()` and only then loads `stationId` / `currentStation`:

```
12e3ea blx   Radar::update
12e3ee ldrb.w r0, [r4, #284]   ; onScreen
12e3f2 cmp   r0, #0
12e3f4 beq.w continue
12e3f8 ldr.w r0, [sl, #4]      ; stations->data
12e3fc ldr.w r8, [r0, fp, lsl #2]  ; stationId
12e402 blx   Status::getStation
...
```

Fix: move the `if (!this->onScreen) continue;` guard before the two locals.

Measured: raw 6,014 (+1), aligned **1,951** (−4), size 9,908,
`.text` 71,271, `.ARM.extab` 577.

## 2. Measured table (all size 9,908 unless noted)

Base = 6,013 / 1,955 / 9,908, `.text` 71,270, `.ARM.extab` 577.

| cell | raw | aligned | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|
| base | **6,013** | **1,955** | 71,270 | 577 |
| `ER` enemy range `>24000` first | **6,013** | 1,968 | 71,270 | 577 |
| `OF` landmark offset polarity | 6,014 | 1,955 | 71,271 | 577 |
| `os` planet-loop guard first | 6,014 | **1,951** | 71,271 | 577 |
| `P` planet bracket X-first | 6,019 | 1,994 | 71,276 | 577 |
| `L` landmark bracket X-first | 6,020 | 1,988 | 71,277 | 577 |
| `A` asteroid `slotCount` before `field_0x1b0` | 6,029 | 1,960 | 71,286 | 577 |
| `eblip` enemy blip `setFrame` first | 6,017 | 1,935 | 71,274 | 577 |
| `sblip` station blip `setRefPixelPosition` before draw | 6,019 | 1,950 | 71,276 | 577 |
| `gasd` gas `isDead`-first | 6,014 | 1,955 | 71,271 | 577 |
| `sframe` station frame `(t/field)*(n-1)` | 6,014 | 1,956 | 71,271 | 577 |
| `pframe` planet frame `(t/field)*(n-1)` | 6,018 | 1,957 | 71,275 | 577 |
| `astA` asteroid tractorAuto store swap | **6,013** | 1,959 | 71,270 | 577 |
| `astM` asteroid tractorMode2 store swap | **6,013** | 1,955 | 71,270 | 577 |
| `ER`+`astM`+`split3` | **6,013** | 1,968 | 71,270 | 577 |
| `ER`+`eblip` | 6,017 | **1,948** | 71,274 | 577 |
| `os`+`eblip` | 6,018 | **1,931** | 71,275 | 577 |
| `pdi_inside` (unfaithful, see §3) | **6,011** | 1,956 | **71,268** | 577 |

The `ER` fix is the only new cell that is raw-tie; its aligned regression is
caused solely by the `s16`/`s18` constant-register swap (the literal pool
itself is byte-identical).  `os` is the best aligned cell among the faithful
fixes but costs +1 raw.

## 3. The only raw-negative cell is unfaithful

Moving `this->planetDockIndex = i;` inside the `field_0x18` `if` gives
raw **6,011** (−2), aligned 1,956, `.text` 71,268, extab 577, size 9,908.
But the original stores `planetDockIndex` **unconditionally** at `0x12e5ae`
(`str.w fp,[r4,#64]`), while the variant predicates it (`ittt ne`).  This is
the same cell w13669 §2.2 rejected; the faithful unconditional store is kept.

## 4. Exhaustive adjacent-statement / expect sweeps

* 133 adjacent simple-statement swaps across the whole `Radar::draw` body:
  every raw-tie cell is aligned 1,955 (no improvement); no cell reduces raw
  below 6,013.
* All 29 `__builtin_expect` sites flipped and removed (58 cells): no raw win.
  Raw-tie cells are `rm4`, `rm10`, `rm12`, `rm14`, `flip14` (all aligned
  1,955).  `rm8` (drop the freighter-mission expect) gives raw 6,020 /
  aligned 1,921; `rm19` (drop the enemy-lock-timer expect) gives raw 6,728 /
  aligned 1,891 — both raw-worse.

## 5. Why there is still no raw win

The head is byte-identical apart from the zero/campaign colouring: original
`mov.w fp,#0` at `0x12e07a`/`0x12e0d8` and `mov sl,r0` at `0x12e0f8`; ours
`mov.w sl,#0` and `str r0,[sp,#0x90]`.  Every block-level source fix above
leaves that greedy-RA cascade intact, so the `0x12e38a` seed
(`str r0,[sp,#0x98]; ldr r0,[sp,#0x90]; cmp r0,#2` vs `cmp.w sl,#2`) and the
address shift it causes survive.  The `ER` fix additionally proves the
literal-pool ordering is tied to the branch order but is exactly cancelled by
the `s16`/`s18` constant-register swap.

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1955 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13689/`: `cell.py`
(compile+link+strip+`fn_diff`+section-diff cell harness), `secdiff.py`,
`bytediff.py`, `seqdiff.py` / `shapediff.py` (instruction-sequence and
shape-only diffs), `sweep.py` (133 adjacent-swap candidates),
`gen_expect.py` (58 expect cells), `Radar.base.cpp`, `orig.dis` / `ours.dis`,
`mir.txt` (`-print-machineinstrs`), `draw_preRA.mir` / `draw_greedy.mir`.
