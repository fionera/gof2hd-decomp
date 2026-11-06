# Radar::draw pass (wave119, w6967) — literal-pool root cause and 50 more variants, no source win

**Status:** research note, 2026-11-06. Worktree `wt/w6967`, branch
`wave119-w6967`, based on current `master` (`d512ec24e`, after
`radar-draw-wave119-w6957.md`). **No source change landed**; this commit
contains this note only. The target remains the pristine w6957 baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.541284 %** (official `verify.py` display 74.5). 2599 instructions match.

Gate at start and end: `RATCHET PASS`, byte 4505, linked 4505, data 843/843,
imports 207, lint CLEAN, drift 0. No section/extab change (note-only commit).

## 1. Exact commands used

```sh
cd /home/debian/GalaxyOnFire2/wt/w6967
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS

NDK=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$NDK -d --start-address=0x12e050 --stop-address=0x130740 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6967/orig.asm
$NDK -d --start-address=0x12e050 --stop-address=0x130740 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6967/ours.asm
```

Per-variant loop (≈1 s each): edit `Radar.cpp`, `cmake --build
cmake-build-match --target link_so -j6`, then score the single symbol with a
scratch `difflib` harness that mirrors `tools/verify/verify.py`'s
`disassemble`/`reconcile` (identical 74.541284 % on the baseline). The harness
is `/dev/shm/w6967/score.py`; the variant drivers are `/dev/shm/w6967/v*.py`
and `sweep.py`; `/dev/shm/w6967/dumpn.py` prints the reconciled instruction
stream with indices, `/dev/shm/w6967/vp.py` prints the `vpush` mask.

## 2. New finding: the first literal pool is misplaced because of the `status` load

The largest single residual block is `orig[1077:1077] ours[1047:1080]`
(33 instructions). It is **not** a missing statement — it is the first
literal pool. Both pools hold the same five words
(`0x000e1c9a 0x000e2166 0x000e20bc 0x000e2224 0x000e1fda`) but at different
addresses:

| | pool address | pool contents |
|---|---|---|
| original | `12e340` (after the 109/110 guard skip block + `nop`) | 5 words |
| ours (baseline) | `12e2e0` (right after the waypoint `b.n`) | 5 words |

Root cause, from the raw Thumb:

* original `12e2de b.n 12e2fc` — the waypoint block branches to the
  **otherLock #1** chain; `12e2fc` is `mov r1, r4`;
* ours `12e2de b.n 12e310` — the waypoint block branches to
  `12e310 ldr r5, [sp,#156]`, the `Status *status` pointer load that the
  w6957 pass added.

Because ours has a live `status` load at `12e310`, `12e310` is a branch
target and the constant-island pass cannot put the pool there; it goes at
`12e2e0`. The original loads `status` only at `12e31a` (`ldr r6,[sp,#156]`,
after the `landmarks != nullptr` test), so its first branch target is the
otherLock chain and the pool is deferred to `12e340`.

This is the real reason the w6957 `Status *status` local is a *tuned* shape:
it buys the guard's register shape but moves the pool 0x60 bytes earlier.

Moving the declaration later does move the pool, but splits the otherLock
chain across the pool and loses more than it gains:

| variant | usize | raw % | pool |
|---|---:|---:|---|
| baseline (w6957) | 9908 | **74.541284** | `12e2e0` |
| `Status *status` declared just before the guard | 9920 | 73.284630 | after otherLock skip |
| guard uses `Globals::status`, no local | 9912 | 72.266017 | `12e2c0` |
| `Status *status` after `int otherLock;` | 9920 | 73.284630 | after otherLock skip |

All three defer the pool but split the contiguous `cbnz/cbnz/cmp+beq;
movs r5,#1` chain that the original keeps intact.

## 3. `vpush {d8-d13}` is only reachable through distance-first `inSight`

The VFP register census is otherwise **exactly** the original's
(`s16`×16, `s18`×16, `s20`×5, `s22`×6, `s25`×13, `s29`×1, `s32`×70 in both).
The only difference is that the original additionally uses `s24`/`s26`
(`d12`/`d13`) for the enemy `inSight` crosshair values, which are live across
the `operator-` and `VectorLength` calls. That forces `vpush {d8-d13}` and
shifts every stack slot (`mode` `[sp,#100]` vs `[sp,#112]`,
`turretMode`/`planetHit` at 148/152, `hud` at 84/88, …).

Every bounds-first spelling leaves the crosshair values dead before the calls,
so clang uses caller-saved `s0`/`s2` and saves only `d8-d11`. Only a
distance-first condition with **float** locals loaded before the `if` restores
`d8-d13`:

```cpp
float crossX = PlayerEgo::crosshairPos.x;
float crossY = PlayerEgo::crosshairPos.y;
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
    this->screenX > (int) crossX - this->field_0x124 &&
    this->screenX < (int) crossX + this->field_0x124 &&
    this->screenY > (int) crossY - this->field_0x124 &&
    this->screenY < (int) crossY + this->field_0x124) {
    inSight = true;
}
```

This is almost certainly the original source shape (the Thumb does
`vldr s26,[r0]` / `vldr s24,[r0,#4]` before `operator-`, then
`vcvt.s32.f32 s24,s24` / `vcvt.s32.f32 s26,s26` before `VectorLength`).
It emits `vpush {d8-d13}` and is 12 B shorter (9896 B) but re-rolls the whole
function: **67.995983 %**. Declaration order (`crossY` first), interleaved
`x/y` bounds, and inline `(int)PlayerEgo::crosshairPos.x` all give the same
~68 % or worse.

## 4. Variant sweep (all on the pristine w6957 baseline)

`usize` is emitted function size; `raw %` is the unrounded ratio.

| variant | usize | raw % |
|---|---:|---:|
| **baseline (w6957)** | 9908 | **74.541284** |
| `Status *status` before guard / after `int otherLock` | 9920 | 73.284630 |
| guard uses `Globals::status` (no local) | 9912 | 72.266017 |
| `int otherLock = 1; if(...)` / `otherLock` first / `unsigned`/`char` | 9908 | 74.541284 |
| `notOtherLock`/`notTurretMode` swapped / `unsigned` / `char` / `xor` | 9908 | 74.541284 |
| `notOtherLock`/`notTurretMode` moved inside the guard | 9900 | 72.536928 |
| negations inlined (`!turretMode && !otherLock`) | 9888 | 71.770609 |
| `bool notOtherLock`/`bool notTurretMode` | 9920 | 71.379409 |
| `int landmarkHit` | 9896 | 70.891884 |
| `bool alienOrbit`→`int` | 9908 | 74.530601 |
| `int gasHit` | 9908 | 74.512615 |
| `int topOk` | 9908 | 74.397936 |
| `int onPlanetCourse` | 9908 | 74.541284 |
| `bool missionBlocksPlanets`→`int` | 9900 | 71.684588 |
| mission type block nested `if`/`else` (faithful decompiler shape) | 9908 | 74.541284 |
| `SetColor` after the mission block | 9908 | 74.025229 |
| quad `DrawImage2D` calls reversed | 9908 | 73.881881 |
| `plasmaInRange`/`lockConfirmed` after `SetColor` | 9908 | 74.426606 |
| `lockConfirmed` before `plasmaInRange` | 9908 | 74.483945 |
| waypoint `&&` / ternary single-`if` form | 9908 | 74.541284 |
| `int campaign`→`uint32_t` | 9908 | 74.541284 |
| asteroid-slot init `!= 5`→`< 5` / `uint32_t` | 9908 | 74.541284 |
| otherLock `== nullptr`→`!ptr` | 9908 | 74.541284 |
| asteroid dead-check plain `if`/nested `if` | 9908 | 74.541284 |
| enemy dead-check plain `if`/nested `if` | 9908 | 74.541284 |
| enemy loop `while` form | 9900 | 68.577160 |
| enemy `i` `int` / declared before loop / enemy ptr hoisted | 9908 | 74.541284 |
| `egoPos`/`targetPos` assignment swapped | 9880 | 63.824289 |
| `this->onScreen`→`!= 0` / `(bool)` | 9908 | 74.541284 |
| `Sprite *blip` removed (enemy lock tail) | 9908 | 74.465950 |
| `Sprite *blip` removed (gas lock tail) | 9924 | 64.291870 |
| recompute: `getStarSystem()->getPlanetTargets()`, no `goingToPlanet`/`isDocking` | 9848 | 64.347326 |
| recompute: `this->planetTargets`, no `goingToPlanet`/`isDocking` | 9840 | 64.327822 |
| recompute: targets local, keep `goingToPlanet`/`isDocking` | 9928 | 72.711646 |
| recompute replaced by `this->field_0x1a8 = onPlanetCourse` | 9836 | 63.471503 |
| recompute deleted | 9832 | 63.451850 |
| **faithful distance-first `inSight`** (float locals) | 9896 | 67.995983 |
| distance-first, `crossY` declared first | 9896 | 67.995983 |
| distance-first, `y` bounds before `x` | 9896 | 67.938603 |
| distance-first, inline `(int)PlayerEgo::crosshairPos.x` | 9960 | 64.259100 |
| bounds-first, inline `(int)PlayerEgo::crosshairPos.x` | 9872 | 70.933410 |
| bounds-first, `x>,x<,y>,y<` order | 9936 | 71.352899 |
| bounds-first float locals | 9872 | 70.933410 |
| split bounds around `VectorLength` (`x>` before, rest after) | 9924 | 64.186579 |
| gas-cloud float crosshair locals before `getSparks` | 9888 | 69.019945 |
| enemy-loop float crosshair locals before `update` | 9892 | 65.365854 |
| enemy-loop float crosshair locals at loop top | 9872 | 59.715640 |
| `Status *status` + faithful distance-first + faithful recompute | 9892 | 56.593485 |

No variant beats **74.541284 % / 9908 B**.

## 5. Remaining residuals (unchanged)

- First literal pool at `12e2e0` vs `12e340` (§2).
- `vpush {d8-d11}` vs `{d8-d13}` (§3); `mode` `[sp,#112]` vs `[sp,#100]`.
- otherLock #1: ours `mov.w r9,#1` (32-bit), original `movs r5,#1` (16-bit);
  `status` in `r5` vs `r6`/`r5`.
- Enemy loop index in `r5` (ours) vs `fp` (original); ours hoists
  `add.w fp,sp,#232` (egoPos temp) where the original uses `r5`.
- Planet-lock tail slot (orig before the landmark loop, ours after) — the
  `radar-draw-wave119-w6950.md` finding, unchanged.
- Asteroid dead-check if-conversion (orig[2527:2537] vs ours[2527:2528]).
- Sound/music tail pre-indexed `ldr.w r0,[r5,#244]!` vs orig
  `ldr.w r5,[r4,#244]; mov r0,r5`.

The baseline is a tuned local optimum: the faithful source shapes
(distance-first `inSight`, no invented recompute) reproduce the original's
call multiset and `vpush` mask but are 100–170 B shorter and re-roll every
block after the planet loop, scoring 63–68 %.
