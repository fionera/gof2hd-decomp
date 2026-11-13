# Radar::draw wave 191b (w13720) — landmark lock `int left`, gas-spark in-sight geometry, music play() constants (raw +20, aligned −35)

**Status:** complete, 2026-11-13. Worktree `wt/w13720`, branch `wave191b-w13720`,
base `master` `cd8eee9c3`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13714.md` (planet-loop `onScreen` guard + enemy
update order), `-w13699.md` (enemy lock-blip `setFrame` order), `-w13689.md`,
`-w13678.md`, `-w13669.md`, `-w13665.md`, `-w13660.md`.

**Landed:** three independent, faithful reconstructions:

1. **`L2`** — the landmark lock bracket converts `left` to `int` up front, exactly
   like `top`.  The original (`0x12e86a`-`0x12e8b2`) emits *both* `vsub`+`vcvt.s32`
   pairs before the first compare (`vldr s2,[r3]` crosshair.x, `vldr s4,[r3,#4]`
   crosshair.y, `vsub s4`/`vsub s2`, `vcvt.s32 s0`/`vcvt.s32 s2`); ours only
   converted `top` up front because `left` was a `float`.  Source now declares
   `int left = (int) (crosshairPos.x - (float) size)`.
2. **`GF`** — the gas-spark in-sight test is written in the original's arithmetic
   form.  The original (`0x12f4a8`-`0x12f4f8`) computes `left + size*2`
   (`add.w r2, r2, r0, lsl #1`) and `top + size*2` and compares `screenX`/`screenY`
   against them; it also keeps `top` as an `int` (`vcvt.s32` up front).  Source
   is now `screenX > left && screenX < left + size*2 && screenY > top &&
   screenY < top + size*2` with `int top`.
3. **`C`** — the music `play()` constants in the `field_0x1bc` ladder were
   transposed.  The original (`0x130348`-`0x13038c`, `0x130442`) plays
   `140` in the `field_0x1bc <= 2` else-branch and `151` for `alwaysEnemySeen`
   in the `field_0x1bc <= 4` branch; our source had `151` and `140`
   respectively (a real behavioural bug, not just a byte mismatch).

`raw` 6,021 → 6,041 (+20), `aligned` 1,917 → 1,882 (−35), size stays
9,908/9,908, `.text` differing bytes 72,153 → 72,173 (+20), `.ARM.extab` 564
(unchanged), `.ARM.exidx` / `.rodata` 0 / 0.  Full `check` gate is green
(byte_exact 4508, linked_exact 4508, size-exact 4524, data 843/843, imports
207, lint CLEAN, parity 5370/5370, drift 0).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,021 |
| instruction-index alignment | 1,917 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 72,153 |
| `.ARM.extab` differing bytes | 564 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |
| zero / `missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. Landed change `L2`: landmark lock bracket `int left`

Source (`Radar.cpp:438-444`) was:

```cpp
int size = this->field_0x124;
float left = PlayerEgo::crosshairPos.x - (float) size;
int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
bool topOk = top < this->screenY;
if (this->screenY < top + size + size && topOk && (int) left + size + size > this->screenX &&
    (int) left < this->screenX) {
```

Original (`0x12e86a`-`0x12e8b2`):

```
12e86a ldr  r3,[pc]          ; crosshairPos
12e86c ldr  r2,[r4,#292]     ; size (field_0x124)
12e872 vmov s0,r2
12e878 vcvt.f32.s32 s0,s0
12e87c vldr s2,[r3]          ; crosshair.x
12e880 vldr s4,[r3,#4]       ; crosshair.y
12e884 vsub.f32 s4,s4,s0     ; top
12e888 vsub.f32 s2,s2,s0     ; left
12e88c vcvt.s32.f32 s0,s4    ; (int)top
12e890 vcvt.s32.f32 s2,s2    ; (int)left
12e894 vmov r3,s2
12e898 cmp  r0,r3            ; screenX > left
```

Both `left` and `top` are converted to `int` before the first compare.
Declaring `left` as `int` reproduces the up-front `vcvt` (and the early
`crosshair.x` load).

## 2. Landed change `GF`: gas-spark in-sight geometry

Source (`Radar.cpp:965-970`) was:

```cpp
int size = this->turretScopeHalfWidth;
int left = (int) (PlayerEgo::crosshairPos.x - (float) size);
float top = PlayerEgo::crosshairPos.y - (float) size;
inSight = this->screenX > left && left > this->screenX - size * 2 && this->screenY > (int) top &&
          (int) top > this->screenY - size * 2;
```

Original (`0x12f4a8`-`0x12f4f8`):

```
12f4aa ldr.w r0,[r4,#300]    ; size
12f4b4 vmov  s0,r0
12f4ba vcvt.f32.s32 s0,s0
12f4be vldr  s2,[r2]         ; crosshair.x
12f4c2 vldr  s4,[r2,#4]      ; crosshair.y
12f4c6 vsub.f32 s4,s4,s0     ; top
12f4ca vsub.f32 s2,s2,s0     ; left
12f4ce vcvt.s32.f32 s0,s4    ; (int)top
12f4d2 vcvt.s32.f32 s2,s2    ; (int)left
12f4d6 vmov  r2,s2
12f4da cmp   r1,r2           ; screenX > left
12f4de add.w r2,r2,r0,lsl #1 ; left + size*2
12f4e2 cmp   r1,r2           ; screenX < left + size*2
12f4ea ldr   r2,[r4,#256]    ; screenY
12f4ee cmp   r2,r1           ; screenY > top
12f4f2 lsls  r0,r0,#1
12f4f4 add   r0,r1           ; top + size*2
12f4f6 cmp   r2,r0           ; screenY < top + size*2
```

`top` is an `int` and the upper bounds are formed as `left + size*2` /
`top + size*2`, not as `screenX - size*2`.

## 3. Landed change `C`: music `play()` constants

Source (`Radar.cpp:1285-1297`) had:

```cpp
} else if (field_0x1bc < 3) {
    if (alwaysEnemySeen) play(151);
    else if (bossSeen)   play(149);
    else                 play(151);     // wrong
} else if (field_0x1bc < 5) {
    if (alwaysEnemySeen) play(140);     // wrong
    else if (bossSeen)   play(149);
    else                 play(141);
}
```

Original:

```
130348 ldr  r1,[r4,#444]     ; field_0x1bc
13034c cmp  r1,#2
13034e bgt  130364           ; n > 2
130356 cbnz r1,130386        ; alwaysEnemySeen -> play(151)
13035a cbnz r1,130378        ; bossSeen        -> play(149)
130360 movs r1,#140          ; else            -> play(140)
130364 cmp  r1,#4
13036a bgt  130380           ; n > 4
13036e cbnz r1,130386        ; alwaysEnemySeen -> play(151)
130374 beq  13053c           ; !bossSeen       -> play(141)
130378 movs r1,#149          ; bossSeen        -> play(149)
130380 ...
130442 moveq r1,#142 / movne r1,#150
```

So the ladder is `alwaysEnemySeen→151`, `bossSeen→149/149/150`,
`else→140/141/142` for `n<=2 / n<=4 / n>4`.

## 4. Fresh angles measured this pass (all size 9,908 unless noted)

Base = 6,021 / 1,917 / 9,908, `.text` 72,153, `.ARM.extab` 564.
`raw`/`aligned` are `tools/fn_diff.py`; `.text`/`.ARM.extab` are whole-section
differing bytes.

| cell | raw | aligned | `.text` | `.ARM.extab` | notes |
|---|---:|---:|---:|---:|---|
| base | **6,021** | **1,917** | 72,153 | 564 | |
| **`L2` landmark `int left` (landed)** | 6,024 | 1,907 | 72,156 | 564 | |
| **`GF` gas-spark geometry (landed)** | 6,037 | 1,892 | 72,169 | 564 | |
| **`C` music constants (landed)** | 6,022 | 1,917 | 72,154 | 564 | semantic fix |
| **`L2`+`GF`+`C` (landed)** | **6,041** | **1,882** | 72,173 | 564 | |
| `GF_inttop` (`int top` only) | 6,037 | 1,895 | 72,169 | 564 | |
| `GF_xf` (reorder only, `float top`) | 6,025 | 1,913 | 72,157 | 564 | |
| `L_intleft` (`int left` + X-first order) | 6,027 | 1,950 | 72,159 | 564 | worse |
| `L2`+`C` | 6,025 | 1,907 | 72,157 | 564 | |
| `GF`+`C` | 6,038 | 1,892 | 72,170 | 564 | |
| `GF`+`L2`+`C`+`O` (`O` below) | 6,056 | 1,877 | 72,188 | 564 | |
| `O` music `inAlienOrbit` first | 6,036 | 1,912 | 72,168 | 564 | faithful, not landed |
| `R` music clause reorder | 6,044 | 1,915 | 72,176 | 564 | faithful, not landed |
| `G3` gas on-screen block order | 6,113 | 1,909 | 72,245 | 564 | faithful, raw cost |
| `GF`+`L2`+`C`+`G3` | 6,133 | 1,874 | 72,265 | 564 | best aligned seen |
| `sblip` station blip source reorder | 6,027 | 1,912 | 72,159 | 564 | emitted order already correct |
| `P_exact` planet bracket X-first | 6,027 | 1,956 | 72,159 | 564 | worse |
| `camp_ordA` (`planetTargets` first) | 8,789 | 2,185 | 435,588 | 1,247 | size 9,924 (fatal) |
| `camp_ordB` (`!alienOrbit` second) | 8,437 | 2,071 | 430,870 | 1,245 | size 9,916 (fatal) |
| `c_plain` (drop campaign `__builtin_expect`) | 9,202 | 2,211 | 441,287 | 1,324 | size 9,968 (fatal) |
| `c_exp1` (expect 1) | 9,292 | 2,351 | 441,205 | 1,326 | size 9,956 (fatal) |
| `boss_if` (`if (shipGroup==10) bossSeen=true`) | 9,221 | 2,324 | 443,001 | 1,395 | size 9,988 (fatal) |
| `boss_true` | 8,536 | 2,079 | 383,230 | 564 | size 9,900 (fatal) |
| `RM` drop redundant wreck `field_0x76=1` | 8,352 | 2,085 | 383,046 | 564 | size 9,900 (fatal) |

## 5. Still-open faithful cells (measured, not landed)

* **`O`** — the `field_0x1bc >= 1` stop branch evaluates `inAlienOrbit()` first,
  then `getStation()->isAttackedByAliens()`, then
  `getCurrentCampaignMission() == 16` (original `0x1302a4`-`0x1302c6`).  Our
  source has `isAttackedByAliens() || campaign==16 || inAlienOrbit()`.  `O`
  alone is raw +15 / aligned −5; `GF`+`L2`+`C`+`O` is 6,056 / 1,877.
* **`R`** — the first music condition's clause order.  Original evaluates
  `(current-136) > 15`, then the `0xe071` bitmask, then
  `current != 151 && alwaysEnemySeen` (`0x130280`-`0x1302a2`); our source has
  the `alwaysEnemySeen` clause second.  Raw +23 / aligned −2.
* **`G3`** — the gas-cloud on-screen block order.  Original is
  `field_0x76=1`, `egoPos`, `targetPos`, 24000-test, `DrawImage2D`,
  unconditional `field_0x73=0` (`0x12f61a`-`0x12f6cc`); ours emits
  `field_0x73=0` before `egoPos` and `DrawImage2D` before `targetPos`.
  Raw +92 / aligned −8.
* **`RM`** — the wreck branch stores `enemy->field_0x76 = 1` a second time;
  the original stores it exactly four times (`0x12ee00`, `0x12eed6`,
  `0x12f61a`, `0x12f774`) and not in the wreck branch.  Removing the store
  shrinks the function to 9,900 B, so it needs an 8-byte compensation
  elsewhere before it can land.

## 6. RA cascade (unchanged, still the dominant raw cost)

`campaign` is still spilled to `sp+0x90` in ours while `missionBlocksPlanets`
lives in `sl`; the original keeps `campaign` in `sl` and
`missionBlocksPlanets` in `fp` (spilled to `sp+0x70` around the planet loop).
The first divergence is `0x12e38a`:

```
orig: cmp.w sl,#2                          (4 B)
ours: str r0,[sp,#0x98]; ldr r0,[sp,#0x90]; cmp r0,#2   (8 B)
```

This +4 B shift propagates through most of the function and dominates `raw`.
Greedy-RA evidence (`-print-machineinstrs`, `radar.ll`): `campaign` is
`%422:gpr`, spilled at its def (`t2STRi12 $r0, %stack.45`) and reloaded in
four predecessor blocks; `missionBlocksPlanets` is `%2730:rgpr` in `$r10`
with partial spills to `%stack.42`.  `$r11` (fp) is unused over the whole
`campaign` live range, so the spill is a greedy-RA tie-break, not a
pressure shortage.  All source spellings tried (`int`/`long`/`unsigned`
campaign, `int`/`char`/`short`/`unsigned` `missionBlocksPlanets`, declaration
orders, `campaign > 1`, `2 <= campaign`, `campaignOk`, `const`, `&&`
reorders) are either byte-identical or size-fatal, so the IR is invariant.

Other confirmed RA colouring differences (not source-order bugs):
`field_0x73` boolean sits in `r6` in the original but `fp` in ours
(`0x12ef8e`); `bossSeen = (shipGroup == 10)` is `it eq; moveq r6,#1` in the
original but `subs #10; clz; lsr` in ours (`0x12ed52`); the asteroid
`else` lock-blip emits `setFrame, draw, setRefPixelPosition` in ours vs
`setFrame, setRefPixelPosition, draw` in the original (`0x1304a4`).
