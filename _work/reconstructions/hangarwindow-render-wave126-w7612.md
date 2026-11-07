# `HangarWindow::render` — wave 126 (w7612): ship-branch SimplifyCFG fold

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7612`,
base master `bf365a964` (`render` 94.388529 %).

This pass: **94.388529 -> 94.388529** (raw difflib over the reconciled
instruction stream). No source change landed; this is a measurement/evidence
record for the ship-branch residual.

## Result

| metric | before (HEAD bf365a964) | landed |
|---|---:|---:|
| `render` fuzzy (raw difflib) | 94.388529 | **94.388529** |
| `render` size | 9700 | 9700 |
| normalized insns | 3626 / 3627 | 3626 / 3627 |
| diff ops / matched | 248 / 0.943885 | 248 / 0.943885 |
| byte_exact / linked_exact | 4506 / 4506 | 4506 / 4506 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

`check`: **RATCHET PASS** — `verify: byte 4506 linked 4506 stubs 0 missing 0
extra 0 avg 99.94`; `data: exact 843 of 843`; `sodiff: imports 207`.

## The residual (re-derived)

Original `0x149b1e`..`0x149b98`:

```
149b1e: ldr  r0, [sp, #108]        ; tab
149b20: ldrd r5, r4, [sp, #132]    ; r5 = &Globals::layout, r4 = this
149b24: cbz  r0, 149b96            ; tab == 0 -> the tab==1 test
149b26: cmp  r0, #3
149b28: bne.n 149b44               ; tab != 3 -> body
...
149b3a: cmp  r4, r0                ; ship == getShip()?
149b3c: ldrd r5, r4, [sp, #132]    ; extra reload of layout/this
149b40: beq.w 14a6d0
149b44: <body>                     ; button 1 setPosition/setVisible/draw/getWidth
149b94: ldr  r0, [sp, #108]        ; tab reload after the body calls
149b96: cmp  r0, #1
149b98: bne.w 14a6d0
```

Ours (same addresses):

```
149b1e: ldr  r0, [sp, #108]
149b20: ldrd r6, r5, [sp, #132]    ; r6 = &Globals::layout, r5 = this
149b24: cmp  r0, #0
149b26: beq.w 14a6d0               ; tab == 0 -> the join, threaded past the test
149b2a: cmp  r0, #3
149b2c: bne.n 149b44
...
149b3e: cmp  r4, r0
149b40: beq.w 14a6d0               ; no reload
149b44: <body>
149b94: ldr  r0, [sp, #108]
149b96: cmp  r0, #1
149b98: bne.w 14a6d0
```

Two independent differences:
1. the `tab == 0` edge is folded straight to the join (`beq.w`, 4 B) instead of
   the narrow `cbz` to the `tab == 1` test (2 B); the original's extra
   `ldrd r5,r4` reload at `0x149b3c` (4 B) makes the two forms the same size;
2. the physical roles are shifted by one: original `layout=r5, this=r4`, ours
   `layout=r6, this=r5`.

## Root cause: SimplifyCFG folds the switch case-0 edge

`clang -S -emit-llvm` on the current source lowers the ship branch to

```llvm
%1719:
  switch i32 %93, label %1728 [   ; %93 = tab
    i32 0, label %1959            ; <-- join, NOT the upgrade test
    i32 3, label %1720
  ]
...
%1765:                            ; the upgrade test
  %1770 = load i8, i8* %204       ; this->upgradeMode
  %1771 = icmp ne i8 %1770, 0
  %1772 = and i1 %203, %1771      ; %203 = icmp eq i32 %93, 1
  br i1 %1772, label %1773, label %1959
```

`-mllvm -print-after-all` shows the switch is *formed* with the original's
target and only later folded:

| pass | ship switch |
|---|---|
| raw IR (`Instrument ... pre inlining`) | no switch: `icmp ne i32 %tab,0` / `icmp eq i32 %tab,3` branches |
| first `Simplify the CFG` | `default=%1742, case 0 ->%1778, case 3 ->%1734`; `%1778` is the `upgradeMode && tab==1` test block (has the `buttonOffset` phi) |
| `Remove unused exception handling info` | unchanged (`case 0 ->%1778`) |
| `Function Integration/Inlining` | `case 0 ->%1832`; `%1832` is the `buttonOffset` **join phi** block |

So the fold happens in the inliner's CFG cleanup, not in `Jump Threading`
(`Jump Threading` already sees `case 0 ->%2024`, the join). The raw IR has
`%2333 = icmp ne i32 %tab, 0; br %2333, %2334, %2394` with `%2394` the upgrade
test, so the front-end shape is right; the switch formation also keeps it right;
only the post-inlining SimplifyCFG re-targets the edge.

## `-jump-threading-threshold=4` probe (reverted)

Adding a per-file `-mllvm;-jump-threading-threshold=4` to `HangarWindow.cpp`
(CMakeLists `set_source_files_properties`, reverted) keeps `case 0` pointing at
the upgrade test in the IR and the emitted code becomes the original's narrow
`cbz r0, <tab==1 test>`:

```
149b10: cbz r0, 149b7e
...
149b28: ldr r0, [sp, #108]
149b2a: beq.n 149b7e
```

but the whole function re-allocates and shrinks to **9672 B** (3620 insns),
dropping the raw score to **86.9 %** (`diff 531`). The flag is not a win and is
not kept.

## Variants measured this wave (all reverted)

Base = 94.388529 %, 9700 B, 3626/3627.

| variant | pct | size |
|---|---:|---:|
| base `if (tab != 0) { if (!(tab==3 && ship==getShip())) {...} } if (upgradeMode && tab == 1)` | **94.388529** | 9700 |
| `if (tab == 1 && this->upgradeMode)` | 94.333379 | 9700 |
| `if (this->upgradeMode && tab - 1 == 0)` | 94.388529 | 9700 |
| `if (this->upgradeMode && !(tab != 1))` | 94.388529 | 9700 |
| `if (this->upgradeMode && (unsigned) tab == 1u)` | 94.388529 | 9700 |
| `if (this->upgradeMode && tab >= 1 && tab <= 1)` | 94.388529 | 9700 |
| `if (this->upgradeMode > 0 && tab == 1)` | 94.388529 | 9700 |
| `if (this->upgradeMode == 1 && tab == 1)` | 94.388529 | 9700 |
| `if (this->upgradeMode && (tab == 1) && true)` | 94.388529 | 9700 |
| `if ((this->upgradeMode) && (tab == 1))` | 94.388529 | 9700 |
| inner `if (tab != 3 || ship != getShip())` | 94.388529 | 9700 |
| compound `if (tab != 0 && !(tab==3 && ...))` | 94.388529 | 9700 |
| `if (!(tab == 0))` | 94.250655 | 9700 |
| `if (tab == 0) {} else { if (...) {...} }` | 94.250655 | 9700 |
| `buttonOffset = 0;` before `if (tab != 0)` | 86.770589 | 9684 |
| `bool up = this->upgradeMode; if (up && tab == 1)` | 86.620690 | 9684 |

Every form that keeps 9700 B leaves the SimplifyCFG fold intact; every form
that blocks the fold (the `-mllvm` probe, the extra `buttonOffset` store, the
`bool` local) re-allocates the frame and loses far more than the 12 diff ops
the ship branch is worth.

## Conclusion

The ship-branch `cbz` vs `cmp/beq.w` difference is not reachable from the
source expression shape: it is a post-inlining `SimplifyCFG` edge fold through
a `buttonOffset` phi. Reproducing the original's un-folded edge requires either
a different original phi/CFG shape (unknown) or a per-file pass flag, and the
only flag that works (`-jump-threading-threshold=4`) re-allocates the whole
function to 9672 B / 86.9 %.

The four independent Greedy/CFG ties from waves 124–125 remain (blueprint
`ldrd` pair order, this ship-branch fold, buy-credits `&Globals::layout` spill,
free-credits `sl`/`r9` role swap). The function stays at 94.388529 %, 9700 B.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j8      # RATCHET PASS
python3 tools/verify/verify.py --show _ZN12HangarWindow6renderEv
# IR trace (scratch):
#   clang++ <cc.sh flags> -S -mllvm -print-after-all -o hw.s HangarWindow.cpp
```

Scratch (not committed): `/dev/shm/w7612/{score.py,buildscore.sh,shipbatch.py,
HangarWindow.base.cpp,orig.asm,ours.asm,ir/pa.txt}`.
