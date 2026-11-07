# w8311 (wave-138): `Level::createMission` — case-183 convoy offset block
# reconstructed from Thumb; default-first switch lands the ascending 0,1,2
# compare chain (fuzzy 92.34 -> 92.49 %, `.text` 85,547 -> 85,538)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8311`, based on master
`96b069792`. Continues `level-createmission-wave137-w8213.md`,
`…-w8214.md`, `…-wave136-w8111.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; `_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` =
17,172 B). Ghidra MCP was unavailable this session (`rea doctor`: the extracted
Ghidra at `/home/debian/ghidra-extract/ghidra_12.1.2_PUBLIC` is missing
`application.properties`/`analyzeHeadless`); manual NDK `objdump` disassembly of
the original is the authority and is what `tools/verify/verify.py` scores.

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   fuzzy 92.3432 %
                             6055 orig / 6065 ours insns
.text diff 85,547   .ARM.extab 1,396   .rel.plt 0   .note.gnu.build-id 20
.rodata/.data/.bss/.ARM.exidx exact
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

(The prompt quoted `.text` 85,535 / `.ARM.extab` 1,395 from a different
worktree; the numbers above are this worktree's measured baseline.)

## 1. The original case-183 block, from Thumb

Original `b1404`–`b1456` (objdump of the original `.so`):

```text
b1404 ldr.w r0,[r8,#248]
b1408 bic.w r1,r5,#0x80000000
b140c cmp   r1,#2
b140e ldr   r0,[r0,#4]
b1410 ldr.w r0,[r0,r6,lsl #2]
b1414 strh.w sl,[r0,#317]          ; field_0x13d = 0
b1418 beq.n b1438                  ; case 2
b141a cmp   r1,#1
b141c beq.n b142c                  ; case 1
b141e cbnz  r1,b1444               ; default
b1420 case0: ldr -3000 -> [sp,#376]; ldr 20000 -> [sp,#372]; ldr -50000; b.n b144e
b142c case1: ldr -7000  -> [sp,#376]; ldr 10000 -> [sp,#372]; ldr -30000; b.n b144e
b1438 case2: ldr 25000; str sl(0) -> [sp,#376]; str 25000 -> [sp,#372]; ldr -36000; b.n b144e
b1444 default: ldr 6000 -> [sp,#376]; ldr 12000 -> [sp,#372]; ldr -45000
b144e join:  str r1,[sp,#380]
b1450 ldr r1,[r0,#0]; ldr r2,[r1,#68]; mov r1,r9; blx r2   ; setPosition(Vector)
```

So:

* **compare order** is descending: `cmp #2` → case2, `cmp #1` → case1,
  `cbnz` → default, fall-through case0;
* **block layout** is ascending: case0 (`b1420`), case1 (`b142c`),
  case2 (`b1438`), default (`b1444`), join (`b144e`);
* every case block stores **y then x then z** (`[sp,#376]`, `[sp,#372]`,
  `[sp,#380]`), i.e. the original source assigns the components in the same
  textual order for all four blocks;
* the **default block falls through into the join** (no branch), so the shared
  `str [sp,#380]` is the tail of the default path as well.

Literal pool (`b15bc`…, verified with `objdump -s`): `-3000, 20000, -50000,
-7000, 10000, -30000, 25000, -36000, 6000, 12000, -45000` — i.e. the pool
follows the layout order case0, case1, case2, default.

The checked-in source (wave 132) is the ascending if/else chain
`if (k==0) … else if (k==1) … else if (k==2) … else …`; its emitted compare
order already matches the original (`cmp #2`, `cmp #1`, `cbnz`), but the
MachineBlockPlacement layout is case0, case2, case1, join, default.

## 2. Source-permutation sweep (24 switches + 6 if/else chains)

Each variant rebuilt with `ninja -C cmake-build-match link_so` and scored with
the official `verify.py` difflib ratio; the emitted block order was read from
the `-mllvm -print-machineinstrs -mllvm
-filter-print-funcs=_ZN5Level13createMissionEv` MIR (post-`Branch Probability
Basic Block Placement`) and cross-checked against the literal pool values.

Only **three** distinct layouts are reachable, selected by the *first case in
the source order*:

| source order starts with | emitted compare chain | layout after MBP |
|---|---|---|
| `case 0`  | `cmp #2`, `cmp #1`, `cbnz` | case0, case2, case1, join, default |
| `case 1`  | `cmp #0`, `cmp #2`, `cmp #1` | case1, case0, case2, join, default |
| `case 2` or `default` | `cmp #0`, `cmp #1`, `cmp #2` | case2, case0, case1, join, default |

All 24 switch permutations and all 6 if/else permutations fall into one of
these three buckets. **No source permutation produces the original's
case0, case1, case2, default, join layout.**

### Why (LLVM 7.0.2 `SelectionDAGBuilder` + `MachineBlockPlacement`)

* `visitSwitch` → `lowerWorkItem` (`SelectionDAGBuilder.cpp`): at `-Oz`
  (`optForMinSize`) the case clusters are *not* split into a binary tree; they
  are sorted by probability (equal → by case value, ascending) and then the
  "rearrange the case blocks so that the last one falls through" loop swaps
  the cluster whose MBB is `NextBlock(SwitchMBB)` to the end. Since the IR
  block order is the source order, a source starting with `case 0` moves
  case0 to the end, giving the emission order `case2, case1, case0` (hence
  `cmp #2` first).
* `visitSwitchCase` adds the successors as `[TrueBB(case), FalseBB(next
  compare)]` unless `CB.TrueBB == NextBlock(SwitchBB)`, in which case it
  swaps them. For the first compare block that swap can only fire when the
  first case body is `case 2`, but that same source order suppresses the
  rearrange swap above, so the two conditions are mutually exclusive for this
  three-case switch. The successor order is therefore always
  `[case2, cmp1]`, `[case1, cmp0]`, `[case0, default]`.
* `MachineBlockPlacement::buildChain` walks the hot path
  `loopcond → cmp2 → cmp1 → cmp0 → case0` (75/67/50 % edges), then fills the
  remaining loop blocks from `BlockWorkList` in insertion order
  (`markBlockSuccessors` follows `MBB->successors()`); the three case blocks
  all have equal frequency (0.2422), so `selectBestCandidateBlock` takes the
  first work-list entry, i.e. case2, then case1, then default. The join
  (frequency 0.7266) is then picked ahead of the default because it is
  reached from all three cases.
* The original's layout would need the work list to be
  `[case1, case2, default]`, i.e. the cmp#2 block's successors to be
  `[cmp1, case2]` — unreachable from a plain `switch`/if-chain with this
  compiler.

### Measured variants

| variant | size | insns | fuzzy |
|---|---:|---:|---:|
| baseline if/else 0,1,2 (`else` body y,z,x) | 17172 | 6055/6065 | 92.3432 |
| `switch` 0,1,2 + `default` last | 17172 | 6055/6065 | 92.3432 |
| **`switch` `default` first, then 0,1,2 (landed)** | **17172** | **6055/6065** | **92.4917** |
| `switch` 2,1,0 + `default` last | 17172 | 6055/6065 | 92.4917 |
| `switch` 2,0,1 + `default` last | 17172 | 6055/6065 | 92.4917 |
| `switch` 1,2,0 / 1,0,2 | 17172 | 6055/6065 | 92.3597 |
| `switch` 0,2,1 / 0,1,default,2 / 0,default,1,2 | 17172 | 6055/6065 | 92.3432 |
| if/else 1,2,0 / 1,0,2 | 17172 | 6055/6065 | 92.3597 |
| if/else 2,1,0 / 2,0,1 | 17172 | 6055/6065 | 92.4917 |
| default body y,x,z (any case order) | 17188 | 6055/6065 | 90.0 (vectorised `vmov`/`vstr`) |
| default body x,y,z (any case order) | 17164 | 6055/6059 | 89.4 |
| `if (k==3)` / `if (k>=3)` default-first chain | 17172 | 6055/6066 | 91.5 |
| `Vector offset` local (default ctor) | 17176 | — | 90.0 (w8214) |
| `__builtin_expect` chain | 17172 | — | 90.0 (w8214) |

The default block's source order matters for size: `y,z,x` (checked-in) and
`z,x,y` keep 17,172 B; `y,x,z` grows to 17,188 B because clang folds the three
stores into `vmov.f32`/`vstr`; `x,y,z` shrinks to 17,164 B.

## 3. Landed change

```cpp
switch (k) {
default:
    offset[1] = 6000.0f;
    offset[2] = -45000.0f;
    offset[0] = 12000.0f;
    break;
case 0:
    offset[0] = 20000.0f;
    offset[1] = -3000.0f;
    offset[2] = -50000.0f;
    break;
case 1:
    offset[0] = 10000.0f;
    offset[1] = -7000.0f;
    offset[2] = -30000.0f;
    break;
case 2:
    offset[0] = 25000.0f;
    offset[1] = 0.0f;
    offset[2] = -36000.0f;
    break;
}
```

The cases are in the corrected ascending 0,1,2 order; the emitted compare
chain is `cmp #0`, `cmp #1`, `cmp #2` (the ascending form) and the layout is
case2, case0, case1, join, default. This is the best reachable shape: it keeps
the function at exactly 17,172 B, keeps `.rel.plt` byte-identical, and lifts
the official fuzzy ratio by **+0.1485 pp**. The original's exact
case0,case1,case2 layout remains unreachable (see §2); the residual is a
MachineBlockPlacement tie-break, not a source-level difference.

## Gate counts (before -> after)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      85,547 -> 85,538   (-9)
.ARM.extab      1,396  -> 1,396
.rel.plt        0      -> 0
.note.gnu.build-id 20  -> 20
_ZN5Level13createMissionEv  92.3432 -> 92.4917 %
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ninja -C cmake-build-match link_so
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w8311/scorex.py          # exact verify.py ratio for one symbol
python3 /dev/shm/w8311/sig.py <variant>   # per-block store/constant signature
python3 /dev/shm/w8311/layout.py <variant># MIR post-MBP block order
```

Scratch: `/dev/shm/w8311/{Level.base.cpp,Level.gen_d012.cpp,scorex.py,score.py,
gen.py,sig.py,layout.py,mir.py,sweep.py,sweep2.py,dumpvar.py,mbp.cpp,
sdisel.cpp,sdbuilder.cpp}`.
