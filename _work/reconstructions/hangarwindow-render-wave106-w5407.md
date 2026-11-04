# HangarWindow::render — wave 106 (w5407): LSDA sink relocation

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w5407`,
branch `wave106-w5407`, base master `486f8a119`.

## Result

| metric | before (HEAD) | landed |
|---|---:|---:|
| `render` fuzzy | 84.7107 | **85.0964** |
| `render` size | 9700 | 9700 |
| `.ARM.extab` | 0x26298 | **0x26298** |
| `render` LSDA record | 948 B / 156 rows | **948 B / 156 rows** |
| byte_exact | 4500 | 4500 |
| linked_exact | 4500 | 4500 |
| size_exact | 4524/4524 | 4524/4524 |
| data_exact | 843/843 | 843/843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

Landed diff (3 insertions, 2 deletions, all in `render`):

```diff
@@ -584,11 +584,13 @@ void HangarWindow::render() {
                                         (*this->buttons)[8]->setPosition(
                                             this->hintOffsetX + Globals::layout->buttonInsetX,
                                             this->field_0x114 + y, 17);
+                                        // LSDA sink: keeps the original 156-row
+                                        // .ARM.extab record (no sink -> 155 rows).
+                                        volatile int PADX = 0;
                                         (*this->buttons)[8]->setVisible(true);
                                         (*this->buttons)[8]->draw();
                                     }
                                     if (right > 0) {
-                                        volatile int padR = 0;
                                         (*this->buttons)[9]->setPosition(
@@ -816,7 +818,7 @@ void HangarWindow::render() {
                     if (item == this->selectedItem) {
                         if (item->isShip()) {
                             if (tab != 0) {
-                                if (!(tab == 3 && this->selectedItem->ship == Globals::status->getShip())) {
+                                if (!(tab == 3 && item->ship == Globals::status->getShip())) {
@@ -899,6 +901,7 @@ void HangarWindow::render() {
         this->render();
         this->viewMode = 1;
         this->listItemWindow->draw();
+        volatile int pV = 0;
     }
```

`item->ship` is semantically identical here: the enclosing condition is
`item == this->selectedItem`.

## What is proven

1. **The `padR` anchor is not required for the ratchet in this tree.** Removing
   `volatile int padR = 0;` from the `if (right > 0)` block gives fuzzy
   **86.832**, 9700 B, `.ARM.extab 0x26290` (940 B / 155 rows), and a full
   `check` still reports `byte 4500 / linked 4500 / RATCHET PASS`. The reason
   `.rodata` does not move: extab base `0x1a3b78` + `0x26290` = `0x1c9e08`, and
   `.rodata` is 16-byte aligned at `0x1c9e10`, so the 8-byte shortfall is
   absorbed by alignment padding. The w104 note's "byte 4500 -> 4040" claim is
   stale for this tree. (The section is nevertheless no longer *size*-exact,
   which is why the landed shape keeps 156 rows.)
2. **The missing 156th row is the `getStationAmount` / `getAmount` split.**
   Original LSDA:
   ```
   149656 +4 lp=14b5a2   Item::getStationAmount
   149660 +4 lp=14b64c   Item::getAmount
   ```
   ours (no sink):
   ```
   149658 +18 lp=14b648  both calls in one cleanup region
   ```
   The two original rows have distinct landing pads (both are the same 2-insn
   `b.n` epilogue stub, emitted twice), i.e. clang created two separate cleanup
   scopes. Our source shape merges them into one 18-byte region.
3. **A single `volatile int` sink before `(*this->buttons)[8]->setVisible(true);`
   restores the 156th row** at 9700 B and `.ARM.extab 0x26298`, scoring
   **85.0964**. This is the best 156-row / 9700 B / exact-extab placement found
   by a full sweep of every source line in the function.
4. The `pV` sink after `this->listItemWindow->draw();` is worth +0.165 on the
   156-row shape; `item->ship` is worth +0.055.

## Measured table (all 9700 B unless noted)

Baseline HEAD: 84.7107, extab 0x26298, 156 rows.

| variant | raw | extab | rows | size |
|---|---:|---|---:|---:|
| HEAD | 84.7107 | 026298 | 156 | 9700 |
| HEAD + `item->ship` | 84.7658 | 026298 | 156 | 9700 |
| HEAD + `pV` | 84.876 | 026298 | 156 | 9700 |
| HEAD + `item->ship` + `pV` | 84.9311 | 026298 | 156 | 9700 |
| padR removed (baseX) | **86.832** | 026290 | 155 | 9700 |
| baseX + `pV` removed | 84.9311 | 026298 | 156 | 9700 |
| baseX + `item->ship` reverted | 85.0413 | 026298 | 156 | 9700 |
| baseX + `pad3` removed | 85.0138 | 026298 | 156 | 9700 |
| **baseX + PADX before buttons[8] setVisible (landed)** | **85.0964** | **026298** | **156** | **9700** |
| baseX + PADX before buttons[8] draw | 85.0964 | 026298 | 156 | 9700 |
| baseX + PADX after `if (left>0)` | 85.073 | 026298 | 156 | 9700 |
| baseX + PADX before `if (right>0)` | 84.9311 | 026298 | 156 | 9700 |
| baseX + PADX before `} else {` | 85.073 | 026298 | 156 | 9700 |
| baseX + PADX before `buttonOffset = 0;` | 84.1019 | 026298 | 156 | 9700 |
| baseX + PADX at line 609 | 84.9587 | 026298 | 156 | 9700 |
| baseX + PADX at line 621 | 84.9587 | 026294 | 156 | 9700 |
| baseX + PADX at line 798 | 85.0454 | 026298 | 156 | 9704 |
| baseX + PADX at line 953 | 84.9353 | 026298 | 156 | 9704 |
| baseX + PADX + free-credits `getWidth` first | 74.663 | 026298 | 156 | 9700 |
| baseX + PADX + free-credits `SetColor` before `textYf` | 84.7976 | 026298 | 156 | 9704 |
| baseX + PADX + swap `left`/`right` blocks | 62.3969 | 026298 | 156 | 9704 |
| baseX + PADX + `this->selectedItem == item` | 85.0413 | 026298 | 156 | 9700 |
| baseX + PADX + buttons[9] operand order | 85.0413 | 026298 | 156 | 9700 |
| baseX + PADX + `b` ternary / `0 < x` comparisons | 85.0964 | 026298 | 156 | 9700 |

Natural (no-sink) attempts to split the cleanup region:

| variant | raw | extab | rows | size |
|---|---:|---|---:|---:|
| `int r2 = getStationAmount(); int l2 = getAmount(); right=r2; left=l2;` | 82.6512 | 026298 | 156 | 9684 |
| `int l2 = getAmount(); left = l2;` (first call direct) | 82.6027 | 026298 | 156 | 9680 |
| nested `{}` around each call | 86.832 | 026290 | 155 | 9700 |
| split into two `if (tab == 1)` statements | 86.832 | 026290 | 155 | 9700 |
| `String name;` moved before `SetColor` | 64.0747 | 026298 | 156 | 9700 |
| `String name;` moved to loop top | 72.83 | 02629c | 156 | 9700 |

## Hypothesis (not proven)

The original's two landing pads are a clang 7.0.2 artifact of two separate
cleanup scopes around the two calls. Our source (same statement block, same
`String name` lifetime) makes clang emit one 18-byte cleanup region. The
attempts above that do split it naturally also perturb register allocation
enough to cost 4+ fuzzy points, so the single cold-path sink is the cheapest
way to keep the original record length. The remaining diff is still dominated
by register allocation: `this`/`Globals::layout` are swapped in `sl`/`r9`
(`ldrd sl,r9,[sp,#132]` vs `ldrd r9,sl,[sp,#132]` at `0x1496da`), and the
free-credits block keeps the `getHeight`-first source shape because the
faithful `getWidth`-first order costs ~10 fuzzy points.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields
cmake --build cmake-build-match --target lint -j6       # CLEAN

# per-function fuzzy score (verify.py machinery, same normalisation as the gate)
python3 /dev/shm/w5407/score.py _ZN12HangarWindow6renderEv
# LSDA rows
python3 _work/reconstructions/gameresource_gen/lsda.py \
  cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
```

Scratch helpers used for the sweeps live under `/dev/shm/w5407/`
(`score.py`, `measure2.sh`, `sweep4.py`, `exp*.py`); they are not part of the
commit.
