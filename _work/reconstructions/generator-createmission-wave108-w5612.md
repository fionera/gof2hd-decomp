# `Generator::createMission` — wave 108 (w5612)

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record 168 B).

Status: **research note, no source change landed.** The landed wave-106 shape
(`volatile double padA` before the reward computation, `__builtin_expect(pick == 10, 1)`,
`__builtin_expect(pick == 12, 0)`, shared `goto assignType`) stays at
**82.1 % / 1996 B**. Nothing measured this session beats it, so `Generator.cpp` is
unchanged and the gate is untouched.

Gate at the start and end of this pass (full `check`):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
RATCHET PASS
```

## 1. What is already exact

The prologue, the whole pick loop (`0x97248..0x972d8`) except the `pick == 12`
arm placement, the campaign-mission switch, the `type == 8` item loop, the
`switch (type)` jump table, the distance/reward block from `0x9760a` on, the
`Mission` ctor, the costs rounding, the target-system loop and the landing pads
are instruction-for-instruction identical modulo register names and stack
offsets. The remaining residual is the same one described in
`generator-createmission-wave106-w5409.md`:

* frame 112 vs original 96 (`padA` at `[sp,#80]`, `amount` at `[sp,#104]`,
  `agentName` at `[sp,#92]`; original `agentName` at `[sp,#80]`, canary at
  `[sp,#92]`, `amount` reusing `targetStation`'s `[sp,#28]`);
* `agent`/`targetStation` stack slots swapped (`ldrd r4,r9` vs `ldrd r9,r4`);
* `difficulty` in `r5` instead of the original's `r6`;
* `galaxy`/`fromSystemPtr` swapped (`galaxy` in `r6`, `fromSystemPtr` at
  `[sp,#16]`; original keeps `fromSystemPtr` in `r6` and spills `galaxy`);
* `pick == 12` arm is out of line (`cmp r6,#12; beq` into it) instead of the
  original's fall-through (`cmp r6,#12; bne` over it);
* `getRoutes() == nullptr` block is inline (`cbz`) instead of the original's
  out-of-line cold block at `0x97586` (`cmp; beq.w`);
* the `amount` stores are sunk to one shared join store (`[sp,#104]`) instead
  of the original's per-case stores (`[sp,#28]` in each arm).

## 2. The `amount` store sink (the 12-byte size anchor)

The original has five `amount` stores: `0x974a8` (type 8), `0x974b4` (initial
0), `0x9753e` (case 15), `0x975ca` (case 0) and `0x97604` (cases 2/11). Ours
has two: the initial 0 store and one shared store at the switch join. The
original's extra stores are exactly the 12 bytes that our `volatile double
padA` is standing in for. If the store sink could be prevented, the pad could
be dropped and the frame could shrink toward 96.

Direct evidence that a runtime-dependent store address prevents the sink:
`volatile int amount[2]; amount[(unsigned) type >> 31] = ...` emits the
type-8 store inside the arm (`str r0,[sp,#100]`) and difficulty in `r2`
instead of `r5`, but the extra address arithmetic costs 24 bytes
(`usize 2020`, 75.1 %). No zero-cost spelling was found.

## 3. Measured variants (official verifier, one `link_so` per row)

All rows start from the landed wave-106 shape and change only the listed
source line(s). `pct` is the `createMission` row of `report.json`.

| variant | pct | usize | notes |
|---|---:|---:|---|
| **landed wave-106 shape** | **82.1** | **1996** | baseline |
| `amount = count + 5;` before `difficulty = getTecLevel()` | 81.8 | 1996 | store moves into the arm, difficulty still `r5` |
| `amount = rnd->nextInt(15) + 5;` then `difficulty = getTecLevel()` | 81.8 | 1996 | same schedule, no `count` local |
| `int tec = getTecLevel(); amount = count+5; difficulty = tec;` | 79.8 | 2000 | CVP folds `tec`, frame grows |
| `int tec` with `amount` before `difficulty` | 79.5 | 2000 | same |
| `volatile int tec` | 76.2 | 2004 | forces extra memory ops |
| `volatile int amount[1]` | 82.1 | 1996 | identical code |
| `volatile int &amountRef = amount` | 82.1 | 1996 | identical code |
| `volatile int *amountPtr = &amount` | 82.1 | 1996 | pointer folded away |
| `volatile int amount[2]` + `amount[(unsigned)type>>31]` | 75.1 | 2020 | prevents sink, costs address arithmetic |
| `int amount` (non-volatile) | 69.8 | 1992 | amount stays in `r2`, no per-case stores |
| `volatile int amount` declared at first use | 82.1 | 1996 | slot unchanged (`[sp,#104]`) |
| `int level` at first use | 80.1 | 2000 | frame grows |
| `int level = 0;` at top | 82.1 | 1996 | identical code |
| `unsigned guard` | 55.9 | 1984 | loop polarity changes |
| `short pick` | 65.3 | 2004 | bad |
| `unsigned pick` | 82.1 | 1996 | identical code |
| `int selected` instead of `bool` | 81.9 | 1996 | slightly worse |
| `int i` in the used-sum loop | 81.8 | 1996 | slightly worse |
| `switch` case 15 moved to the front | 82.1 | 1996 | compiler re-sorts |
| `default: amount = 0;` inside the switch | 79.5 | 1996 | store still sunk |
| `if/else if` chain instead of `switch` | 75.7 | 1992 | loses the `tbb` shape |
| `volatile int padA` (4-byte pad) | 77.9 | 1992 | 4 B short |
| `volatile float padA` | 78.2 | 1992 | 4 B short |
| two `volatile int` pads | 70.8 | 1996 | right size, wrong code |
| `{ volatile double padA; }` scoped at reward | 75.4 | 1996 | frame 104, pad at `[sp,#48]` |
| `{ volatile double padA; }` scoped at function top | 77.4 | 1996 | frame 104, `agentName` at `[sp,#80]` |
| pad before `Galaxy *galaxy` / `if (difficulty >= 10)` | 82.1 | 1996 | ties with landed position |
| pad before `int fromSystem` | 81.2 | 1996 | |
| pad before `SolarSystem *fromSystemPtr` | 81.5 | 1996 | |
| pad before `int itemId = 0;` | 79.7 | 1996 | |
| pad before `AbyssEngine::String agentName` | 72.3 | 1996 | |
| pad inside the `type == 8` arm | 72.3 | 1996 | |
| pad before `Mission *mission` | 80.6 | 1996 | |
| pad before `int costs` | 74.1 | 1996 | |
| pad at end before `return mission` | 72.6 | 1996 | |
| `getRoutes()==nullptr` expect 0 | 61.9 | 2000 | out-of-line block, global realloc |
| `getRoutes()!=nullptr` expect 1 (inverted if) | 61.9 | 2000 | same |
| explicit `goto nullRoutes` block | 82.1 | 1996 | compiler canonicalises back to `cbz` |
| p12 plain (no cold expect) | 79.3 | 1996 | p12 `bne` right, loop test `bne` wrong |
| p12 plain + `while (__builtin_expect(!selected,1))` | 42.9 | 2032 | bad |
| `if (pick == 8) { type=pick; continue; }` | 64.7 | 1972 | CVP fold |
| duplicated `type = pick` in both p12 arms | 50.7 | 1988 | CVP fold |
| duplicated `type = pick + 0` | 50.7 | 1988 | CVP fold |
| `used->data[pick]` expect 0 | 57.7 | 2004 | bad |
| `used != nullptr` expect 1 | 54.0 | 1992 | bad |
| `if (race > 3) continue; type = 10;` (swapped) | 55.0 | 2000 | bad |
| `int difficulty` hoisted to the top | 82.1 | 1996 | identical code |
| `int difficulty = 0;` | 82.1 | 1996 | identical code |
| `volatile int rem` for the bonus remainder | 78.1 | 1992 | bad |
| `int bonusRem` hoisted to the top | 82.1 | 1996 | rem still in `r9` |
| `register int targetStation` | 82.1 | 1996 | identical code |
| `volatile int targetStation` | 49.3 | 2000 | bad |
| `Agent *agentAlias = agent` used everywhere | 82.1 | 1996 | alias folded away |

## 4. Proven vs hypothesis

Proven from the binary and the measurements above:

* The original's `amount` is a volatile memory local: the initial 0 store is
  live on the default paths, and every case writes it separately. The current
  shared join store is a `MachineSink` artefact of our CFG, not a different
  source semantic.
* The 12-byte `padA` is exactly the size of the three missing per-case stores.
  A 4-byte pad leaves the function 4 bytes short (1992); a double pad is the
  smallest spelling that reaches 1996 without changing the surrounding code.
* The `p10` expect is required for the loop rotation: removing it collapses
  the score to ~52-60 % even though it gives the original frame-96 slot map.
* `getRoutes()==nullptr` expect 0 does produce the original's out-of-line
  `cmp; beq.w` block, but the global register allocation changes and the score
  falls to 61.9 %.
* The `pick == 12` fall-through is coupled to the loop rotation through MBP's
  frequency-based chain formation: every spelling that makes p12 fall through
  also inverts the loop test (`bne` exit instead of `beq` body).

Hypothesis (not closed): a source shape that keeps the p10 arm hot, makes the
`amount` stores per-case (no sink) and keeps the loop test at the bottom would
let the pad be dropped and the frame collapse to 96. No such spelling was
found in this pass; the store-sink and the p12 placement both appear to be
downstream of the same `difficulty`/`amount` schedule in the `type == 8` arm
(`mov r6,r1; adds r0,r6,#5; str r0,[sp,#28]` in the original vs
`mov r5,r0; adds r0,r6,#5; b <shared store>` in ours).

## 5. Commands

```
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Per-variant scoring used one `link_so` plus:

```
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --top 0
```

Side-by-side disassembly:

```
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
```
