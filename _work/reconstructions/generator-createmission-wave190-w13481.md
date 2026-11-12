# Generator::createMission — wave 190 / w13481: 3-byte residual, no win landed

Status: **no code change landed** (HEAD shape retained).  Worktree
`wt/w13481`, branch `wave190-w13481`, base master `d5e2c1b55`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` (1996 B,
744 insn) is still **raw 3 / aligned 3 / size 1996/1996**:

| offset | original | ours |
|---|---|---|
| `0x1a2` (`bhi.n`) | target `0x97388` (race<4 && type==13 test) | target `0x973fa` (folded away) |
| `0x556`/`0x560` | `str/ldr r1,[sp,#32]` (rem) | `str/ldr r1,[sp,#44]` |

Both are independent; fixing either alone would be a raw-1/raw-2 win.
Nothing below beat the HEAD shape at 1996 B, so the tree is left clean.

## 1. The `bhi` fold is a *second*-pass JumpThreading decision

Pass dumps (`-mllvm -print-after-all -filter-print-funcs=<mangled>`):

* The front-end emits `select i1 (race<4), i1 (type==13), i1 false`
  (`%157` in the first SROA dump), then InstCombine turns it into
  `and i1` and later `or i1 (race>3), (type!=13)`.
* The **first** JumpThreading pass (early pipeline) already retargets the
  `type<15` false edge from the race-check block to the merge block,
  because on that edge LVI knows `type u>= 15`, hence `type != 13`.
* The **second** JumpThreading pass (main pipeline) does it again after
  the `or` form appears: `or` with one operand known true on the edge
  (`type != 13`) is folded to the merge block.

The original must therefore have a race-check block whose condition is
**not** resolvable by LVI on the `type>=15` edge.  LVI cannot see through
a *load in the race-check block* (its `CmpInst` path is skipped when the
compared value is an instruction in that block), i.e. the original's
`type` alloca was probably not promoted by SROA.  Our `type` is fully
promoted (phi of constants `{0,4,7,11,12}` plus the loop value), so LVI
resolves it.

### Guard forms measured (all 1996 B / raw 3 unless noted)

`type<=14`, `!(type>14)`, `type<15u`, `(int)type<15` (raw 4),
`type-15<0` (1912 B), `(type^13)==0`, `(type|0)==13`, `(type+0)==13`,
`(type-0)==13`, `13==type`, `(uint32_t)type==13`, `type==0xd`,
`(race<4)&(type==13)`, `(int)(race<4)&(int)(type==13)`, ternary,
`race<4 && __builtin_expect(type==13,1)`,
`__builtin_expect(race<4,1) && __builtin_expect(type==13,1)`,
`int ok = 0; if (race<4) ok = type==13; if (__builtin_expect(ok!=0,1))`,
`bool ok` phi, `int ok` ternary, `uint32_t typeCopy = type;`,
`int typeInt = type;`, `uint32_t *typePtr = &type;`, `*(uint32_t*)&type`,
`volatile uint32_t *tp = &type` (2012 B), `uint32_t *volatile tp`
(1984/2012 B), `volatile uint32_t type` (2000 B, **branch fixed** but
+4 B and 9 extra insns), `(type & 0x1f) == 13` (1964 B, **branch fixed**
but RA keeps `type` in `r5` instead of sp+36), `(type & 0xf)`,
`(type % 16)`, `(type & 0xffff)` (all 1964 B), `(type << 19) == 13<<19`
(1968 B), `switch (type) { case 0: case 11: case 14: ... }` (1944 B,
different switch lowering), mask-check nested if (raw 3),
`__builtin_expect` moved to the mask check (raw 3).

Nested `if (race<4) { if (type==13) {...} }` is **1984 B** (the
`getRoutes()==nullptr` block moves inline instead of the cold section);
adding `__builtin_expect` to both branches merges the two ifs back into
the `and` form and reproduces the HEAD bytes exactly (raw 3).

## 2. `rem` slot: IR-level StackColoring merge, not SSC

`rem` is the `volatile int rem = remVal;` alloca.  The MIR shows:

* after `Merge disjoint stack slots` (IR-level StackColoring) the
  `volatile int rem` alloca (fi#2, 4 B) is **dead**, merged into fi#5,
  the 12-byte `AbyssEngine::String` temporary for
  `mission->setTargetSystemName(...)` (its `LIFETIME_START` is in the
  block after the `Mission` constructor, i.e. after `rem`'s
  `LIFETIME_END` — disjoint, so merged).
* the machine-level `StackSlotColoring` therefore sees `rem` as part of
  a String slot and cannot move it to the `status`/`mission` spill slot
  at sp+32.

The original's `rem` is at sp+32, i.e. a *spill slot* shared with
`status` (0x97228) and `mission` (0x5a6), so in the original the
volatile alloca (or a plain-int RA spill) must not be merged with a
String at the IR level.

Measured (all 1996 B):

| variant | raw | note |
|---|---:|---|
| HEAD (`volatile int rem`, `remCopy`) | **3** | rem merged into String slot sp+44 |
| `int rem` plain (no volatile) | 97 | RA keeps rem in `r9`, spills `agent`; frame 104 |
| `volatile int rem` hoisted to function top | 85 | rem at sp+96, frame 104 |
| `volatile int rem` before `if ((type|4)==12)` | 85 | frame 104 |
| `volatile int rem` at top of bonus else | 3 | same as HEAD |
| `alignas(8) volatile int rem` | 85 | frame 104, rem sp+48 |
| `volatile int rem[2..4]` | 3 | merged with a String anyway |
| `volatile int remCopy` | 755 | 2004 B |
| `(void)rem` after bonus / after ctor | 593/519 | 2000 B |

Hoisting the alloca to the top does move `rem` out of the String region
but its live range then spans the whole function, so machine SSC cannot
share it with any spill slot and the frame grows to 104 (all sp offsets
+4).  The plain-int form grows the frame to 104 too (extra spill slot)
and shifts every sp offset.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
# raw=3 aligned=3 size=1996/1996 insn=744/744
```

Scratch harness (not in the repo): `/home/debian/GalaxyOnFire2/scratch/w13481/`
— `Generator.base.cpp` (pristine HEAD copy), `try.sh` (build+score),
`rawdiff.py`, `spmap.py`, `subv.py`/`nested.py`/`maskv.py` (variant
sweeps), `miC.txt`/`ir.txt` (`-print-machineinstrs` /
`-print-after-all` dumps for the HEAD shape), plus LLVM 7.0.1 sources
(`JumpThreading.cpp`, `StackSlotColoring.cpp`, `StackColoring.cpp`,
`LazyValueInfo.cpp`).

## 4. Suggested next angles

* Make the `type` alloca survive SROA (a load in the race-check block
  defeats LVI) **without** changing the codegen — a cheap opaque escape
  of `&type` that costs no instruction has not been found yet.
* Prevent the IR-level StackColoring merge of the volatile `rem` alloca
  with the `setTargetSystemName` String while keeping its live range
  short enough that machine SSC can share the sp+32 spill slot.
