# Small tails — wave 189 / w13366: Generator::createMission raw 40 -> 3

Status: **size-exact win landed.** Worktree `wt/w13366`, branch
`wave189-w13366`, base master `30caae150`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

## What changed

`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` (1996 B,
744 insn) went from **raw 40 / aligned 21** to **raw 3 / aligned 3** with
the function size unchanged.

Two independent source changes, both semantically faithful:

1. **Bonus block restored to the original shape.**  The landed body used a
   dead `volatile int bv = rawBonus;` anchor plus `volatile int bonus` to
   coax the r6/r5 split; that forced frame 104 and a different bonus block.
   The original is `int bonus`, `volatile int rem`, `int remCopy = rem` and
   `b = rawBonus - remCopy`, frame 96.  On its own this shape is 2000 B
   (one extra cold `b.n` landing-pad stub, LSDA row 17 split from row 20).

2. **Hoist `int costRem;` to the top of the function.**  This is the lever
   that removes the extra landing pad.  clang pushes a cleanup scope for a
   local declaration (`int costRem = costs % 50;`) between the
   `getRandomName` temporary and `mission->setCosts(...)`.  That scope has a
   null cached landing pad, so `getInvokeDestImpl()` builds a *new* pad for
   the `setCosts` invoke and overwrites the enclosing `agentName` cleanup
   scope's cache (clang 7 `CGException.cpp`).  The original has one pad
   shared by LSDA rows 17 and 20; declaring `costRem` with the other ints
   and assigning it later restores the single pad, the frame-96 layout and
   the original cold-section order.  The body is instruction-identical.

The two changes together are 1996 B / 744 insn and leave only 3 differing
bytes.

## Remaining 3 bytes

| offset | original | ours |
|---|---|---|
| `0x1a2` (`bhi.n`) | target `0x97388` (race<4 && type==13 test) | target `0x973fa` (folded away) |
| `0x556`/`0x560` | `str/ldr r1, [sp, #32]` | `str/ldr r1, [sp, #44]` |

The `bhi` fold is `type > 14 => type != 13`; the original keeps the
redundant `type == 13` test.  The `sp+32` vs `sp+44` is the `volatile int
rem` alloca slot; hoisting `rem` to the top (renamed `remV`) moves it to
`sp+32` but re-colours the whole body (raw 85).  Both are independent of
the landing-pad fix.

## Gate

| metric | before | after |
|---|---:|---:|
| `createMission` raw | 40 | **3** |
| `.text` diff | 73,554 | **73,517** |
| `.ARM.extab` diff | 610 | **603** |
| build-id | 20 | 20 |
| total (text+extab+build-id) | 74,184 | **74,140** |

`check` RATCHET PASS: byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical, `.rodata` exact, `.ARM.exidx` identical, file
size 2,187,328 exact.  The other four tails are unchanged (Hud 32,
ModStation 11/11, MenuTouchWindow 28, MGame::OnUpdate 10).

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
# raw=3 aligned=3 size=1996/1996 insn=744/744
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
