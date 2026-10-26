# Status::missionCompleted — residual block-order artifact

Status: analysis only (no source change landed). `Status::missionCompleted(bool,bool,long long)`
is at **60.2%** (orig 1432 B @ `0xabf38`, ours 1428 B). The body is block-for-block identical;
the residual is entirely jump-table MBB successor ordering.

## Evidence

- Original high jump table: TBH @ `0xabfe2`, 40 entries @ `0xabfe6`.
- Sorting the original targets gives the case-body layout:
  `a7/ae, 96, 97, 98, 99, 9a, 9b, 9e, a2, a6, aa, b8, bd, 9c, 9d, a0, a3, a4, a5, a8, a9, ab, ac, default`
  — exactly the order the case labels appear in `src/game/mission/Status.cpp`.
- The low table (`8, b, d, e`) also matches source order.
- Our layout is ascending by case value
  (`... 9b, 9c, 9d, 9e, a0, a2, a3, a4, a5, a6, a8, a9, aa, ab, ac, default, b8, bd`).
- The layout is determined by `bb.104`'s successor list. Rewriting that list in the MIR to
  source order reproduces the original layout exactly (size stays 1428); reversing it reverses
  the layout. No missing code, no struct/layout bug.

## Why source cannot fix it

`buildJumpTable` (same compiler revision `1d739ffb…`) adds successors by iterating `Table`,
which is built from `sortAndRangeify`-sorted clusters, i.e. ascending by case value. Empirically,
compiling with cases reversed, ascending, `0x9e`-last, `0x9c/0x9d`-last, or with an explicit
`default: break;` all produce the identical ascending layout and size 1428.

Alternatives tested, all worse:

| Variant | Result |
|---|---|
| if/else-if chain | size 1436, **22.6%**, `byte_exact` drops to 1342 |
| if-chain calling `m->getType()` per arm | size 1540, no jump table |
| `-Os` | same ascending layout, size 1456 |
| `-O1` / `-O2` | no jump table |
| `-mllvm -disable-block-placement` | different layout, size 1436 |

## Remaining concrete deltas

- block order and its short/wide branch-encoding consequences,
- one `cbnz` vs `cmp/bne` in `case 0x8`,
- one scheduling swap in `case 0x9e`.

Conclusion: this is a `MachineBlockPlacement` ordering artifact; the original's source-order
layout requires a source-order successor list that `buildJumpTable` cannot produce from any
natural source shape at the project's fixed `-Oz` flags.
