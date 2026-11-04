# HangarWindow::render — wave 108 (w5613): LSDA sink relocation to the ship branch

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w5613`, branch `wave108-w5613`, base master
`8f98ec4d6`.

## Result

| metric | before (HEAD 8f98ec4d6) | landed |
|---|---:|---:|
| `render` fuzzy | 86.253444 | **88.254756** |
| `render` size | 9700 | 9700 |
| `render` LSDA record | 948 B / 156 rows | **948 B / 156 rows** |
| `.ARM.extab` | `0x26298` @ `0x1a3b78` | **`0x26298` @ `0x1a3b78`** |
| `.text` | `0x1337c8` @ `0x69440` | same |
| `.ARM.exidx` | `0x6f70` @ `0x19cc08` | same |
| `.rodata` | `0x41b67` @ `0x1c9e10` | same |
| ELF section count | 27 | 27 |
| byte_exact / linked_exact | 4500 / 4500 | **4500 / 4500** |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

No section size, base or count changed. The 156-row / 948-byte record is kept.

## Landed diff (4 hunks, all inside `render`)

```diff
@@ -793,9 +793,6 @@ void HangarWindow::render() {
                     } else if (item->isMoveToCargoButton()) {
                         (*this->buttons)[6]->setPosition(this->hintOffsetX + Globals::layout->buttonInsetX, y, 17);
-                        // LSDA sink: splits the item-branch cleanup region so the
-                        // 156-row / 948-byte .ARM.extab record is preserved.
-                        volatile int *S = 0;
                         (*this->buttons)[6]->setVisible(true);
@@ -816,6 +813,9 @@ void HangarWindow::render() {
                     if (item == this->selectedItem) {
                         if (item->isShip()) {
+                            // LSDA sink: splits the ship-branch cleanup region so the
+                            // 156-row / 948-byte .ARM.extab record is preserved.
+                            volatile int *padX = 0;
                             if (tab != 0) {
@@ -957,10 +957,7 @@ void HangarWindow::render() {
-                {
-                    volatile ChoiceWindow *p = this->dialog;
-                    (*this->buttons)[12 + count]->setSplitText(price);
-                }
+                (*this->buttons)[12 + count]->setSplitText(price);
@@ -1046,8 +1043,6 @@ void HangarWindow::render() {
             if (i == 0 || (Globals::options[73] && i == 1) || ...) {
-                // Sink: pins the free-credits block's register allocation.
-                volatile int S2 = 0;
                 visible = false;
```

`pad3` (the `volatile int pad3 = y;` anchor in the `if (left > 0)` block) is
kept. The three removed sinks were wave-106/107 LSDA levers; the new
`volatile int *padX` in the `isShip()` branch replaces all three and scores
higher at the same 948-byte record.

## Measured table (all 9700 B unless noted)

| variant | fuzzy | size | record |
|---|---:|---:|---:|
| HEAD (pad3 + p-block + `S` + `S2`) | 86.253444 | 9700 | 948 |
| no `pad3` | 87.988981 | 9700 | 936 |
| no `pad3` + no p-block | 88.209366 | 9700 | 940 |
| no p-block + no `S` + no `S2` | 86.111877 | **9696** | 936 |
| no p-block + no `S` + no `S2` + `padX` before `buttons[9]->draw()` | 88.236915 | 9700 | 948 |
| **no p-block + no `S` + no `S2` + `padX` before `if (tab != 0)` (LANDED)** | **88.254756** | **9700** | **948** |
| no p-block + no `S2` (`S` kept) | 88.236915 | 9700 | 940 |
| no p-block + no `S2` + `padX` before `buttons[8]->setPosition()` | 86.556474 | 9700 | 948 |
| no `pad3` + `padX` before `buttons[0]->setVisible()` | 86.611570 | 9700 | 948 |
| no `pad3` + `padN` before `replaceTextKeepSize` + `padX` before `buttons[0]->setVisible()` | 87.272727 | 9700 | 948 |
| landed, `padX` typed `volatile char *` / `unsigned *` / `TouchButton *` | 88.254756 | 9700 | 948 |
| landed, `padX` typed `volatile int` (non-pointer) | 82.751965 | 9688 | 948 |

Sweeps behind the table: every source line in the list body, the buy-credits
block and the free-credits block was tried as a `volatile int *padX = 0;`
insertion site (about 450 lines per sweep), on four different bases
(`HEAD`, `no p-block`, `no p-block + no S`, `no pad3`), plus a 1000-sample
random search over 8 base toggles x 0-4 extra sinks drawn from a 23-site pool.
Only the sites listed above reach 948/9700 at >= 88.2.

## What is proven

1. **The 948-byte / 156-row record is preserved at exactly 9700 B with the
   ship-branch sink**, and the fuzzy score rises 86.253444 -> 88.254756
   (+2.00 points) with no change to any ELF section size, base or count.
2. **The three old sinks (`S`, `S2`, and the `volatile ChoiceWindow *p`
   block) are not needed.** Removing all three and adding one pointer sink in
   the `isShip()` branch is strictly better than keeping any of them.
3. **The sink's pointee type is irrelevant** (`int*`, `char*`, `unsigned*`,
   `TouchButton*`, `int**` all give 88.254756); only the pointer-vs-scalar
   distinction matters (a scalar sink costs 12 B of body and 6 points).
4. **The full gate stays green**: `check` RATCHET PASS, byte 4500 /
   linked 4500 / size 4524 / data 843 / imports 207, lint CLEAN, drift 0.

## Hypothesis (not proven)

The remaining ~12 % gap is register allocation and block placement, not LSDA
scoping. The best variant still differs at, e.g., `0x149650`
(`ldrd r5,r6,[sp,#132]` vs the original's separate `ldr r6,[sp,#136]` /
`ldr r5,[sp,#132]`) and at `0x149b24` (`cbz r0,0xabe` vs
`cmp r0,#0; beq.w`), i.e. the same `sl`/`r9` and near/far-branch shape
described in `hangarwindow-render-wave106-w5407.md`. The original's split of
the `getStationAmount` / `getAmount` call sites into two landing pads is still
not reproduced by any plain source shape tried (see the wave-106 note).

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target lint  -j6      # CLEAN
cmake --build cmake-build-match --target drift -j6      # 0 fields

# per-function fuzzy score (same normalisation as the gate)
python3 /dev/shm/w5613/score.py _ZN12HangarWindow6renderEv
# LSDA record size
python3 /dev/shm/w5613/exrec.py cmake-build-match/libgof2hdaa.so
```

Scratch helpers (`score.py`, `exrec.py`, `gen2.py`, `randsearch.py`,
`sweep2*.py`) live under `/dev/shm/w5613/`; they are not part of the commit.
