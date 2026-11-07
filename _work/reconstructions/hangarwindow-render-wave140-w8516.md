# `HangarWindow::render` — wave 140 (w8516): swap-weight probes on the ship
# branch (no pair move); first credit-block source win (2062 -> 2059)

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w8516`,
base `d4857f174` (wave-139 docs; `render` 94.7 %, gate byte 4507 / linked 4507).

This pass: **2062 -> 2059 raw diff bytes, size 9700 B** (landed). `check` is green
(byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN, drift 0).
The win is a two-line declaration swap in the credit block, not the ship branch.

Metric used below: **diffbytes** = differing bytes in the 9700-byte function body
(`/dev/shm/w8516/fdiff.py`). Landed baseline: **2062**. Base-order control: **2082**.

## Finding 1 (re-confirmed): base order still gives the wrong pair

`this->selectedItem->ship == Globals::status->getShip()` (base order) is 2082
diffbytes, 9700 B, and at `0x149b20` is

```
149b20: e9dd 6521  ldrd r6, r5, [sp, #132]   ; layout=r6, this=r5
...
149b30: 6ea9       ldr  r1, [r5, #104]       ; this->selectedItem
149b36: 68cc       ldr  r4, [r1, #12]        ; ship -> r4  (BEFORE the call)
149b3a: blx getShip
149b3e: 4284       cmp  r4, r0
149b40: beq.w join
```

Original is the same instruction sequence but `ldrd r5, r4` (layout=r5, this=r4)
plus a `ldrd r5,r4,[sp,#132]` reload at `0x149b3c` and a 2-byte `cbz r0,0x149b96`
to the upgrade test instead of our 6-byte `cmp/beq.w join`.

## Finding 2 (NEW): machine-IR live ranges of `this`/`layout`/`ship`

`clang -mllvm -print-after-all` gives the greedy-regalloc machine IR (the NDK
clang is a release build, so `-debug-only=regalloc` is unavailable, but
`-print-after-all` works). For the base-order build the ship branch is:

```
bb.381: %3400 = t2LDRi12 %stack.80,0   ; tab
        %3343 = t2LDRi12 %stack.76,0   ; this
        %3366 = t2LDRi12 %stack.79,0   ; layout
        t2CMPri %3400,0 ; bcc bb.382
bb.383: %2055 = t2LDRi12 %3343,104     ; this->selectedItem
        %403  = t2LDRi12 %2055,12      ; ship
        tBL getShip
bb.384: t2CMPrr %403,%2058 ; bcc bb.436
bb.385: ... uses %3343 (this), %3366 (layout) ...
```

Slot indices (from `-print-after-all`): `%3343` (this) `[63464,65408]`,
`%3366` (layout) `[63468,65424]`, `%403` (ship) `[63664,63840]`. All three are
global (multi-MBB). `ship` is nevertheless allocated `r4` first, so `this`/`layout`
get `r5`/`r6`. The original has `this=r4`, `layout=r5` and reuses `r4` for `ship`
with a reload, i.e. its `this` piece was placed before `ship`.

`-mllvm -regalloc-csr-first-time-cost=N` (0,1,2,6,12,30) does **not** move the
pair in the single-TU `-S` output, so the tie is not the CSR first-use cost.

## Finding 3 (NEW): the credit-block `r6`/`sl` swap is a declaration-order tie

At `0x14a912` the original does `add r6, sp, #312` (2 B) for `&description`
and keeps `Globals::layout` in `sl`; ours does `add.w sl, sp, #312` (4 B) and
spills layout to `sp+128` (`strd sl,r9,[sp,#128]` at `0x14a8ec`). This single
register choice is what produces the long ~1473-byte diff run from `0x14a90a`.

**Source lever found:** swapping the two `String` declarations in the credit
loop

```cpp
String price("");        // was: String description("");
String description("");  // was: String price("");
```

moves the `sp+312` pointer from `sl` to `r5`:

```
14a912: ad4e       add  r5, sp, #312    ; description
14a916: f50d 7a96  add.w sl, sp, #300   ; price
```

and drops the raw diff from **2062 to 2059** at the same 9700 B. The original
uses `r6` for `sp+312` and `r5` for `sp+300` (both 2-byte); ours now uses `r5`
for `sp+312` and `sl` for `sp+300` (one 4-byte). The stack slots are unchanged
(`description`=sp+312, `price`=sp+300), so semantics are identical. Swapping the
assignment order in the `switch` as well is byte-identical to the declaration
swap alone (2059).

## Experiments this wave (all reverted except the declaration swap)

Base-order control 2082; landed control 2062. Every variant was built with the
full link and scored with `fdiff.py`.

| variant | diffbytes | size | pair |
|---|---:|---:|---|
| landed (control) | 2062 | 9700 | r5,r4 |
| base order (control) | 2082 | 9700 | r6,r5 |
| base + `this->selectedItem != nullptr` in `if (tab != 0)` | 2349 | 9700 | r6,r5 (this=r5) |
| base + `this->upgradeMode != 2` in `if (tab != 0)` | 2082 | 9700 | r6,r5 |
| base + `int lw = listWidth; (void)lw;` | 2082 | 9700 | r6,r5 |
| base + `padX = &this->field_0x114` | 2082 | 9700 | r6,r5 |
| base + `padX = (volatile int*)this` / `...layout` | 2082 | 9700 | r6,r5 |
| base + `ListItem *sel` used in cond | 2082 | 9700 | r6,r5 |
| base + `Ship *ship` hoist (re-test) | 2082 | 9700 | r6,r5 |
| base + `tab == 1 && upgradeMode` (reordered) | 8722 | 9628 | – |
| base + `(tab==1) ? upgradeMode : false` | 8722 | 9628 | – |
| base + `this != nullptr` in upgrade test | 8571 | 9672 | – |
| base + `padX` moved before `if (item == selectedItem)` | 6930 | 9704 | this reloaded from stack |
| base + volatile read of `this`/`layout` in ship block | 7500/7526 | 9712/9716 | – |
| landed + `String price(""); String description("");` | **2059** | **9700** | r5,r4 |
| base + same declaration swap | 2079 | 9700 | r6,r5 |

## Conclusion

* The ship-branch pair (`this=r4, layout=r5`) is still not reachable from the
  base-order source: `ship` is a global interval and wins `r4` before the split
  `this`, exactly as wave 139 concluded. Every new source probe that keeps 9700 B
  leaves the pair at `r6,r5`.
* NEW: the credit-block `r6`/`sl` tie is sensitive to the *declaration order* of
  the two `String` locals. Swapping them lands a 3-byte raw improvement
  (2062 -> 2059) at 9700 B with the gate green. This is the first source win in
  this function in four waves.

## Open questions / next levers

* The ship branch needs `this` allocated before `ship`. The machine IR shows
  `%3343` (this) and `%3366` (layout) are both global and larger than `%403`
  (ship), yet ship still takes `r4`; the missing piece is a source shape that
  makes the `this` reload a non-split global that survives `shouldEvict`.
* The credit block would fully match if `&description` took `r6` (2-byte) and
  layout stayed in `sl`. After the declaration swap ours is `r5`/`sl`; a shape
  that pushes the `sp+312` pointer to `r6` while keeping `sp+300` at `r5` would
  close the ~1470-byte run.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w8516/fdiff.py _ZN12HangarWindow6renderEv   # 2059
# machine IR (release clang has no -debug-only=regalloc):
#   /dev/shm/w8516/cc.sh -S -mllvm -print-after-all -o /tmp/hw.s
```

Scratch (not committed): `/dev/shm/w8516/{cc.sh,try.sh,batch.py,batch2.py,
batch3.py,batch4.py,batch5.py,fdiff.py,paa.txt,hw.ll,HangarWindow.landed.cpp}`.
