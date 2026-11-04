# `Generator::createMission` — wave 103 (wf304)

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(orig `0x971b8`, 1996 B, `.ARM.extab` LSDA record 168 B / 25 call-site entries).

## Result

| variant | fuzzy | body B | LSDA record | gate |
|---|---|---|---|---|
| wave-102 HEAD (`volatile double padA`, `for` loop, `short pick`, `0.5000001f`) | 64.4 | 1996 | 168 | green |
| `while` + `int pick` + `0.5f` + hoisted `costs/itemId/amount` | 69.4 | 1996 | 168 | green |
| `volatile int amount` (else as above) | 73.5 | 1992 | 168 | short 4 B |
| `volatile int amount` + oracle reward order (9,7,8,3\|\|5,11) | 74.1 | 1996 | 168 | green |
| **landed**: `volatile amount` + `int count = nextInt(15)` + HEAD reward order | **77.5** | 1996 | 168 | **green** |
| landed, `__builtin_expect` removed | 52.3 | 1988 | 168 | green |
| landed, `amount` non-volatile | 67.1 | 1996 | 168 | green |
| wf205 hoisted-decl shape | 76.6 | 1996 | 180 | **breaks** (`.ARM.extab` +12, `byte_exact` 4500→4040) |
| `volatile amount` + local `int tec` | 75.6 | 1996 | 176 | breaks |

Full gate after landing: byte 4500, linked 4500, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `.ARM.extab` 0x26298, **RATCHET PASS**.

Landed source deltas vs wave-102 HEAD (all inside `createMission`):

* loop: `while (!selected) { selected = guard < 1; int pick = ...; --guard; ... }`
  instead of `for (; !selected; --guard) { ... short pick ...; type = pick; }`;
* `itemId` / `amount` hoisted to the top of the body (`amount` is `volatile`);
* `type == 8` arm: `int count = rnd->nextInt(15); difficulty = getTecLevel(); amount = count + 5;`
  (one store of `amount`, `difficulty` left in a register — matches the original schedule);
* `costs` stays a use-site `int costs = ...`;
* `costs * 0.5f` (the `0.5000001f` +1-ulp anchor is no longer needed).

## What the oracle established (logic, not bytes)

`game/Generator.c` `createMission` @`0x000a2a7c` (2044 B) confirms the body shape:

* reward chain order is `9` (×1.2), `7` (×0.7), `8`, `3||5` (×2.0), `11` (`*0.6` +
  `+= reward*0.6/5.0 * amount`);
* the `type == 8` arm is `amount = nextInt(15); difficulty = getTecLevel(); amount += 5;`
  (one store of `amount`, `difficulty` in a register);
* `pick == 12` is `type = 12; if (getIndex() != 25) type = pick;` (not a goto);
* `Mission` ctor argument order matches the current source.

Note: the *source* order of the reward chain does not survive codegen — clang reorders the
`else if` chain.  With this body the wave-102 order (7,9,8,11,3,5) emits the original's
9-first test chain and scores 77.5 %, while the oracle's textual order emits a 7-first
chain and scores 74.1 %.  The binary is the authority here.

## Why `volatile int amount`

The original spills `amount` to its stack slot **immediately** in every arm
(`str r0,[sp,#28]` at `0x974a8` / `0x974b4` / `0x9753e`; reloaded at `0x97830` for
`setProductionGoods`).  With a plain `int amount` clang -Oz keeps it in `r2` across the
whole switch and stores it only at the join, which also changes the stack-slot colouring
(8 int slots instead of the original's 7, frame 104 instead of 96).  `volatile` forces the
same "store at the definition" schedule.  It is value-transparent (no observable change;
the variable is a plain local, never aliased).

## Proven vs hypothesis

* **Proven** (measured with `tools/verify/verify.py` + `tools/sodiff`):
  * `.ARM.extab` size is driven by the LSDA call-site entry count; a record of 176/180 B
    shifts `.rodata` and collapses `byte_exact` to 4040.  Only `usize == 1996 &&
    record == 168` is gate-compatible.
  * `loop_int + anchor_off + hoist_on` is worth +5.0 points over HEAD at the same
    1996/168.
  * `volatile int amount` is worth +4.1 (69.4 → 73.5) and is worth +10.4 in the final
    shape (67.1 → 77.5).
  * the `int count` temporary in the `type == 8` arm is worth +0.3 over
    `amount = nextInt(15) + 5`.
  * `__builtin_expect(pick == 10, 1)` is a branch-weight anchor: without it the body
    shrinks to 1988 B and the match drops to 52.3 % (the loop / `pick == 10` block order
    flips).
* **Hypothesis / still open**: the remaining ~22 % is register allocation and block
  placement, not source logic:
  * the original keeps `difficulty` in `r6` and `galaxy` in `[sp,16]`; ours keeps
    `difficulty` in `r5` and `galaxy` in `r6`;
  * the original places the `getRoutes() == nullptr` block out of line at `0x97586`
    (`cmp r0,#0; beq.w`) and the `type=8`/`type=13` join at `0x973f8`; ours places both
    inline (`cbz`) and the join at `0x97362`;
  * the `tbb` table entries differ because the case blocks sit at different relative
    offsets (12 table bytes + ~20 decoded lines).
  Every declaration-order / `uint32_t` / `short` / `do-while` / branch-swap / `!ptr` /
  local-`sys` variant tried canonicalises to the same IR; only `volatile`, the `count`
  temporary and the reward-chain order moved the allocator.

## Commands

```
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
```
