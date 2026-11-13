# Generator::createMission — wave 191b / w13557: raw 3 -> 1 (rem slot fixed)

Status: **size-exact win landed.**  Worktree `wt/w13557`, branch
`wave191b-w13557`, base master `4c5d0b229`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` (1996 B,
744 insn) went from **raw 3 / aligned 3** to **raw 1 / aligned 1** with
the function size unchanged (1996/1996, 744/744 insn).

## What changed

Two coupled source edits, both semantically faithful:

1. `Generator.cpp:828` — mark the `type == 12` station override as
   likely:
   ```cpp
   if (__builtin_expect(type == 12, 1)) {  // lint: arm_builtin
       targetStation = agent->getStation();
   }
   ```
2. `Generator.cpp:951` — restore the bonus block to the plain-int
   shape the original binary actually has:
   ```cpp
   int rem = rawBonus % 50;
   int b = rawBonus + rem;
   if (b % 50 != 0) {
       b = rawBonus - rem;
   }
   bonus = b;
   ```
   (replacing the previous `int remVal ... volatile int rem = remVal;
   int remCopy = rem;` shape).

The `__builtin_expect` raises the estimated frequency of the block
containing the bonus computation just enough that `RAGreedy` no longer
evicts `agent` from `r9` for `rem`; `rem` is instead spilled and
machine-level `StackSlotColoring` colours its spill slot onto the
`status`/`mission` slot at **`sp+32`** — exactly the original.  The
whole bonus block becomes instruction-identical:

```
9770c: adds r5, r1, r6
9770e: str  r1, [sp, #32]
97710: movs r1, #50
97712: mov  r0, r5
97714: blx  __aeabi_idivmod
97718: ldr  r0, [sp, #32]
9771a: cmp  r1, #0
9771c: it   ne
9771e: subne r5, r6, r0
97720: str  r5, [sp, #16]
```

Frame stays 96, alloca layout unchanged (agentName `sp+80`, Strings
`sp+68/56/44`, canary `sp+92`).

## Remaining 1 byte

| offset | original | ours |
|---|---|---|
| `0x9735a` (`bhi.n`) | `d815` -> `0x97388` (race-check block) | `d84e` -> `0x973fa` (merge block) |

On the `type >= 15` edge LVI knows `type != 13`, so JumpThreading drops
the redundant `type == 13` test.  See
`generator-createmission-wave191b-w13557.md` §2 for the measured
attempts (every mask form inserts a 2-byte instruction).

## Gate

| metric | before | after |
|---|---:|---:|
| `createMission` raw | 3 | **1** |
| `.text` differing bytes | 71,241 | **71,239** |
| `.ARM.extab` differing bytes | 576 | 576 |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rodata` size delta | 485 | 485 |

`check` RATCHET PASS: byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370
(0 extra / 0 missing).  `.rel.plt` byte-identical.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
# raw=1 aligned=1 size=1996/1996 insn=744/744
```
