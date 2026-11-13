# Radar::draw wave 191b (w13678) — enemy on-screen `field_0x76` store order landed (raw +1, aligned −8)

**Status:** complete, 2026-11-13. Worktree `wt/w13678`, branch `wave191b-w13678`,
base `master` `b221b3826`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13669.md` (notOtherLock/notTurretMode order),
`-w13660.md` (gas-blip store order), `-w13656.md`, `-w13647.md` (target
brackets), `-w13644.md` (blip/store order), `-w13597.md` (r10/r11 cascade).

**Landed:** reorder the enemy on-screen block so `enemy->field_0x76 = 1;`
precedes `this->egoPos = player->getPosition();`, matching the original's
store at `0x12ee00` before the `Player::getPosition` call at `0x12ee0a`.
`raw` 6,013 → 6,014 (+1), `aligned` 1,955 → 1,947 (−8), size stays
9,908/9,908, `.text` differing bytes 71,247 → 71,248, `.ARM.extab` 577
(unchanged), `.ARM.exidx` / `.rodata` 0 / 0.  Full `check` gate is green
(byte_exact 4508, linked_exact 4508, size-exact 4524, data 843/843, imports
207, lint CLEAN, parity 5370/5370, drift 0).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,955 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,247 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |
| zero / `missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. Landed change: enemy `field_0x76 = 1` before `egoPos`

Source (`Radar.cpp:708-712`) was:

```cpp
this->egoPos = player->getPosition();
enemy->field_0x76 = 1;
this->targetPos = enemy->player->getPosition();
```

The original (`0x12edf8`-`0x12ee2a`) is:

```
12edf8 str  r5, [sp, #116]
12edfa add  r5, sp, #232
12edfc ldr  r1, [sp, #136]      ; player
12edfe movs r0, #1
12ee00 strb.w r0, [r8, #118]    ; enemy->field_0x76 = 1
12ee04 mov  r0, r5
12ee06 str  r6, [sp, #104]
12ee08 mov  sl, r9
12ee0a blx  Player::getPosition
...
12ee14 blx  Vector::operator=   ; egoPos = player->getPosition()
12ee18 ldr.w r1, [r8, #4]       ; enemy->player
12ee1e blx  Player::getPosition
12ee2a blx  Vector::operator=   ; targetPos = enemy->player->getPosition()
```

So the three statements are `field_0x76 = 1`, `egoPos`, `targetPos`.  Moving
the store first reproduces the original's `strb.w` before the first
`getPosition` call and keeps the two vector assignments in order.  Size is
unchanged (all three forms are the same instruction count); the only metric
movement is the local hunk.

## 2. Fresh angles measured this pass

All cells are size 9,908 unless noted.  Base = 6,013 / 1,955 / 9,908.

### 2.1 Gas-spark in-sight geometry (`GF`) — faithful, aligned −25, raw +16

The source computed `float top` and the X test as `left > screenX - size*2`:

```cpp
int size = this->turretScopeHalfWidth;
int left = (int) (PlayerEgo::crosshairPos.x - (float) size);
float top = PlayerEgo::crosshairPos.y - (float) size;
inSight = this->screenX > left && left > this->screenX - size * 2 && this->screenY > (int) top &&
          (int) top > this->screenY - size * 2;
```

The original (`0x12f4a8`-`0x12f4f8`) is X-first with an `int top`:

```
12f4b4 vmov s0, r0
12f4ba vcvt.f32.s32 s0, s0
12f4be vldr s2, [r2]        ; cx
12f4c2 vldr s4, [r2, #4]    ; cy
12f4c6 vsub.f32 s4, s4, s0  ; top_f
12f4ca vsub.f32 s2, s2, s0  ; left_f
12f4ce vcvt.s32.f32 s0, s4  ; top
12f4d2 vcvt.s32.f32 s2, s2  ; left
12f4da cmp  r1, r2          ; screenX > left
12f4de add.w r2, r2, r0, lsl #1   ; left + size*2
12f4e2 cmp  r1, r2          ; screenX < left + size*2
12f4ee cmp  r2, r1          ; screenY > top
12f4f2 lsls r0, r0, #1
12f4f4 add  r0, r1          ; top + size*2
12f4f6 cmp  r2, r0          ; screenY < top + size*2
```

The faithful form is:

```cpp
int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
inSight = this->screenX > left && this->screenX < left + size * 2 && this->screenY > top &&
          this->screenY < top + size * 2;
```

With `GF` the gas-spark block is instruction-for-instruction identical to the
original.  Raw **6,029** (+16), aligned **1,930** (−25), `.text` 71,263,
`.ARM.extab` 577.  The raw rise is the shifted-region artefact (the block is
inside the 4-byte `campaign`-spill shift); the local instruction stream is
exact.

### 2.2 Gas on-screen block order (`GO`) — new source-order divergence

The original (`0x12f618`-`0x12f6cc`) orders the on-screen gas-cloud block as
`field_0x76 = 1`, `egoPos`, `targetPos`, the 24000 test, `DrawImage2D`, then
the unconditional `field_0x73 = 0`.  The source has `field_0x73 = 0` at the
top, `DrawImage2D` before `targetPos`, and no explicit sink.  Reconstructing
the original order (`GO`) gives raw **6,105** (+92), aligned **1,947** (−8).
The raw jump is the whole gas block moving; the aligned win is real but the
raw cost is large, so it was left unlanded.

### 2.3 Landmark `egoPos` before `targetPos` (`Lego`) — new source-order divergence

The original (`0x12e8d0`-`0x12e900`) assigns `egoPos = player->getPosition()`
first, then `targetPos = landmarks[i]->player->getPosition()`.  The source has
`targetPos` first.  `Lego` is a faithful fix: raw **6,022** (+9), aligned
**1,940** (−15).  Left unlanded because of the raw cost.

### 2.4 Target brackets (re-measured on this base)

| cell | raw | aligned | size | insn | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| base | **6,013** | **1,955** | 9,908 | 3541 | 71,247 | 577 |
| planet bracket X-first, `top+size > screenY` (`P'`) | 6,014 | 1,944 | 9,908 | 3541 | 71,248 | 577 |
| planet bracket X-first, `screenY < top+size` (`P`, exact) | 6,019 | 1,994 | 9,908 | 3540 | 71,253 | 577 |
| landmark bracket X-first (`L`) | 6,020 | 1,988 | 9,908 | 3540 | 71,254 | 577 |
| asteroid `slotCount`/`field_0x1b0` order (`A`) | 6,029 | 1,960 | 9,908 | 3541 | 71,263 | 577 |

`P` is the literal original spelling and its block is instruction-for-
instruction identical, but the one-instruction length change disturbs the
downstream gas-block alignment and the global aligned metric rises to 1,994.
`P'` is semantically identical but emits a predicated form, and gives the
best aligned of the two.

### 2.5 Other blip-geometry / store-order cells

| cell | raw | aligned | size | `.ARM.extab` |
|---|---:|---:|---:|---:|
| enemy offscreen `field_0x73` before `field_0x76` (`off73`) | **6,013** | 1,975 | 9,908 | 577 |
| enemy on-screen `field_0x76` first (landed, `E76`) | **6,014** | **1,947** | 9,908 | 577 |
| gas-spark X-half only (`gasx`) | 6,014 | 1,951 | 9,908 | 577 |
| asteroid `tractorMode2 && field_0x1c == nullptr && wreck` (`tm2`) | 6,015 | 1,945 | 9,908 | 577 |
| `E76` + `P'` | 6,015 | 1,936 | 9,908 | 577 |
| `Lego` + `P'` | 6,023 | 1,929 | 9,908 | 577 |
| `GF` + `E76` | 6,030 | 1,922 | 9,908 | 577 |
| `GF` + `P'` | 6,030 | 1,919 | 9,908 | 577 |
| `GF` + `E76` + `Lego` | 6,039 | 1,907 | 9,908 | 577 |

No size-exact cell reaches raw ≤ 6,013 with a better aligned value; `off73`
is the only raw tie and it is aligned-worse (1,975).  Every aligned win pays
raw because the whole planet/landmark/enemy/gas region is shifted by the
single greedy-RA cascade (`campaign` spilled in ours, `sl` in the original).

## 3. Why no raw win: the RA cascade is unchanged

The first byte difference is still the `mode` spill slot (`sp+0x68` vs
`sp+0x64`); the first length difference is still `cmp.w sl,#2` vs
`ldr r0,[sp,#0x90]; cmp r0,#2` at `0x12e38a`.  The whole
planet/landmark/enemy/gas loop region is shifted by 4 bytes from
`0x12e3d4`, so block-local reorderings change bytes but cannot recover the
shift.  The landed `E76` is the smallest raw-cost faithful fix found.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6014 aligned= 1947 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13678/`: `cell.py` (compile+link+
strip+`fn_diff`+section-diff cell harness), `batch.py` / `sweep.py`,
`adiff.py` / `groups.py` / `callblocks.py` / `buckets.py` (address- and
call-block-aligned diffs), `Radar.base.cpp` (pristine source), and the
`c*.json` / `swaps.json` / `rots.json` / `combos.json` measured cells.
