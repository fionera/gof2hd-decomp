# LevelScript::process — wave 119 / w6808 (structural block-placement sweep)

Status: **analysis note, no code change landed, 2026-11-05**. Branch
`wave119-w6808`, base master `319bd4d30` (wave-119 harvest). Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 instructions.

## Result

The master body is a hard local optimum for statement/line/block reordering.
No size-exact variant beat the baseline:

```
                         before = after (master)
raw ratio (difflib)      59.380284
orig / ours insns        20169 / 20043
orig / ours size         57232 / 57232   (size-exact)
byte_exact               4505
linked_exact             4505
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN
drift                    0
RATCHET                  PASS
sections                 27
file size                2187328
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6808/score3.py`, same definition as the wave-118/119 notes).

The interesting finding is structural: the remaining gap is **basic-block
placement**, not missing code or wrong constants.  The original and our build
emit the same campaign-switch case *order* (verified from the dispatch chain
targets), but clang places the per-case state-machine blocks in different
places.  That is why 21383 of 21526 aligned instructions differ while the
function size is exact.

## Region analysis (address-aligned)

`orig` offsets are from `0x137d18`; `ours` offsets are the corresponding
regions in our link (the cumulative shift is shown by the case deltas below).

| region | score | orig insns | ours insns |
|---|---:|---:|---:|
| prologue (`0x0..0xd72`) | 74.26% | 1171 | 1148 |
| range 131–145 block (`0xd72..0xefe`) | 79.74% | 149 | 162 |
| range 64–73 block (`0xefe..0x1f8a`) | 58.38% | 1473 | 1278 |
| main case bodies (`0x1f8a..0x3800`) | 44.63% | 2284 | 3277 |
| shared state machine (`0x3800..0xd11e`) | 54.85% | 13810 | 12888 |
| post-switch tail (`0xd11e..0xdf90`) | 72.01% | 1282 | 1290 |

The main-case region is ~1000 instructions larger in ours and the shared
state-machine region ~900 instructions smaller: the same code is distributed
differently by block placement.

## Per-case body scores

Case start addresses were read from both dispatch chains (they are in the same
order in both binaries; only the cumulative byte shift differs).  Body end =
next case start in address order; the last main case ends at the shared block
(`orig 0x3800`, ours `0x41d0`).

```
case 125:  10.7%  o=  37 u= 375     case 102:  21.7%  o= 105 u=  33
case 154:  24.9%  o= 169 u=  80     case  40:  26.1%  o= 135 u=  41
case  16:  40.3%  o= 451 u=1428     case  91:  41.5%  o=  39 u=  14
case  24:  69.3%  o=  52 u=  49     case  56:  71.4%  o=  44 u=  26
case  29:  71.6%  o=  65 u=  69     case  41:  71.8%  o=  82 u=  88
case  69:  76.2%  o=1460 u=1278     case  42:  76.6%  o=  59 u=  69
case  92:  79.4%  o= 301 u= 281     case 105:  82.7%  o= 112 u=  96
case  80:  84.4%  o=  77 u=  77     case 157:  90.6%  o= 167 u= 164
case 114:  93.0%  o=  22 u=  21     case  14:  93.3%  o= 132 u= 121
case  21:  94.3%  o=  53 u=  53     case  94:  96.3%  o= 182 u= 192
case 135:  98.8%  o= 162 u= 162
```

Cumulative case-entry deltas (`orig_offset - ours_offset`, bytes): case 29
+610, case 40 -280, case 41 +584, case 42 +446, case 56 +544, case 80 +556,
case 91 +406, case 92 -332, case 94 +470, case 102 -8, case 105 +428,
case 114 +484, case 125 +610, case 154 +182, case 157 -282.

## Structural finding: case 125's state machine is out-of-line in the original

Original case 125 (`0x2284..0x22e6`, 0x62 B) is only the state-0 route setup and
then `b.w 0x3800` (state-2 check).  The state-1 handler is at `0x3726..0x3800`
(after case 16's body) and the state-2 handler starts at `0x3800`; the chain
continues to `0x5f9e` (state 3) etc.  In our build the whole state machine is
inline at `0x2022..0x2432` (case 125's body), so case 125 is 375 instructions
instead of 37 and the shared region loses those blocks.

Ghidra's decompilation (`rea decompile ... --procedure
_ZN11LevelScript7processEi`) confirms the original source shape is
`if (state == 0) {...} else { if (state == 1 && ...) {...} }` followed by
separate `if (state == 2)`, `if (state == 3)`, `else if (state-4U < 3 && ...)`.
Our source uses two separate `if`s for states 0 and 1.  Making it `else if`
is semantically more faithful but changes the block layout: clang then emits a
`tbh` dispatch and the body grows by 24 B (see below), so it cannot be landed
without a compensating 24-byte reduction elsewhere.

The opposite happens in case 91: the original emits the state-0 docking-name
code inline in the case body (`0x2f2a..0x2f94`), while our build places it in
the shared region (`0x13c802..0x13c856`) and the case body is only 14
instructions.  Case 16 is the same story in the other direction: ours is 1428
instructions vs 451 in the original because its state handlers are inline.

## Variants measured (all size-exact unless stated)

| variant | size | score |
|---|---:|---:|
| master base | 57232 | **59.380284** |
| case 125 state 0/1 `else if` | 57256 | 57.652237 |
| case 125 state 0/1 `else if` + `state==1 && getRoute()!=0` + separate state 2/3 | 57284 | 57.819882 |
| case 125 state 2/3 split only | 57260 | 57.515852 |
| case 125 `goto checkState2` (skip state 1 after state 0) | 57256 | 57.652237 |
| swap case 14 and case 16 in source | 57232 | 59.380284 (identical codegen) |
| swap case 21 and case 24 in source | 57232 | 59.380284 (identical codegen) |
| swap case 125 state-0 and state-1 blocks | 57232 | 59.072821 |
| swap case 125 state-1 and state-2 blocks | 57300 | 57.187624 |

Case order in the source is irrelevant: clang's block placement produces the
same `.text` for `case 14`/`case 16` and `case 21`/`case 24` swaps.

## Search coverage (no wins)

All candidates were compiled with the exact `compile_commands.json` flags,
gold-linked with the checked-in object list, and scored with the verify
disassembler.  Only `usize == 57232` variants were accepted.

- 9000 random single-line / multi-line block swaps at equal brace depth
  (block lengths 1,2,3,4,6): no improvement.
- 4800 random balanced-brace block swaps (whole statements/if-blocks, equal
  depth): no improvement.
- 1772 systematic adjacent simple-line swaps: no improvement.
- 12k random balanced-block moves to a random equal-depth position: no
  improvement.
- 2-line block swaps (systematic, all positions): no improvement.

The master body already contains the wave-119 w6704/w6753 adjacent-swap,
move-2 and random-pair wins; those neighbourhoods are exhausted.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6808/score3.py cmake-build-match/libgof2hdaa.so   # 59.380284
```

**Build gotcha:** the build is timestamp-driven; `touch
src/game/world/LevelScript.cpp` before `link_so` after any `git checkout`.

## Next step for a future pass

The only remaining lever is to make clang place case 125's state machine
out-of-line like the original (and case 91's state-0 block inline).  The
source-level `else if` is the faithful shape but costs 24 B; a pass that finds
a real 24-byte reduction elsewhere (or otherwise reproduces the original block
order) could unlock the ~1000-instruction main-case/shared-region redistribution
and is worth far more than further statement reordering.
