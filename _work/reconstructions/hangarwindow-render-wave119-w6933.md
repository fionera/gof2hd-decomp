# HangarWindow::render — wave 119 (w6933): the amount block is a pair of ternaries

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6933`, branch `wave119-w6933`, base `59184fd11`.

Builds on `level-createship-wave119-w6922.md` (LSDA census; HangarWindow 944 vs
948, "row encoding, not a missing pad"; the `NoopGuard` experiment) and
`hangarwindow-render-wave119-w6839.md` / `-w6915.md` (91.337281 landed).

**Result: the `NoopGuard` is not needed.** Replacing the selected-item
`right`/`left` `if/else` with two conditional operators — and deleting the
`volatile int pad3 = left;` codegen anchor — makes clang 7 emit the original
948-byte `.ARM.extab` record exactly, keeps the body at 9700 B, and raises the
official fuzzy score.

| metric | before (HEAD `59184fd11`) | after |
|---|---:|---:|
| `render` fuzzy (official `verify.py`, rounded) | 91.3 | **91.7** |
| `render` size | 9700 | **9700** |
| `render` `.ARM.extab` record | 944 | **948 (exact)** |
| `render` rows / row bytes | 156 / 923 | 156 / 924 |
| `render` rows with `len>=0x80` | 14 | **15** |
| `.ARM.extab` size | `0x26290` (156,304; -8) | **`0x26294` (156,308; -4)** |
| `.text` size / base | `0x1337c8` / `0x69440` | same (exact) |
| `.rodata` base | `0x1c9e10` | same (exact) |
| file size | 2,187,328 | same (exact) |
| byte_exact / linked_exact | 4505 / 4505 | **4505 / 4505** |
| size-exact / data / imports | 4524 / 843 / 207 | same |
| lint / drift / sections | CLEAN / 0 / 27 | same |

`cmake --build cmake-build-match --target check -j6` -> **RATCHET PASS**.
`exidx_table.py --diff` now reports **one** differing record:
`Generator::createMission` 168 vs 172 (-4). `HangarWindow::render` is exact.

---

## 1. The change

```diff
--- a/src/game/ui/HangarWindow.cpp
+++ b/src/game/ui/HangarWindow.cpp
@@ -567,20 +567,13 @@ void HangarWindow::render() {
-                                    int right, left;
-                                    if (tab == 1) {
-                                        right = item->item->getStationAmount();
-                                        left = item->item->getAmount();
-                                    } else {
-                                        right = item->item->getAmount();
-                                        left = this->bluePrint->getCurrentAmount(item->item->getIndex());
-                                    }
+                                    // Ternaries (rather than an if/else) keep the
+                                    // original's per-arm landing pads: clang emits a
+                                    // distinct cleanup row for each ?: arm even when
+                                    // the arm code is identical, which is what the
+                                    // 948-byte .ARM.extab record encodes.
+                                    int right = tab == 1 ? item->item->getStationAmount() : item->item->getAmount();
+                                    int left = tab == 1 ? item->item->getAmount() : this->bluePrint->getCurrentAmount(item->item->getIndex());
                                     if (left > 0) {
-                                        // Codegen anchor: the dead store keeps the
-                                        // list-body register roles (this -> r6) and
-                                        // the 9700-byte body of the original. Storing
-                                        // `left` (already in r0) avoids a reload of y.
-                                        volatile int pad3 = left;
                                         (*this->buttons)[8]->setPosition(
```

Both edits are load-bearing and they cancel in `.text` size:

* the two ternaries add **+4 B** of body (two `?:` arms instead of one
  `if/else` join) and, more importantly, give the two calls in each arm their
  own landing pad;
* deleting `pad3` removes exactly **-4 B** (its `volatile` store) and removes
  the extra `llvm.lifetime.end` cleanup scope that was splitting the
  `buttons[8]` block.

Net `.text` = 9700 B, unchanged.  The ternary alone is 9704 B / 956 B record;
`pad3` removal alone is 9696 B / 936 B record; only the pair is 9700 / 948.

## 2. Why a ternary changes the call-site table

This is the w3572 mechanism, re-confirmed here: clang 7 emits a distinct
landing pad for each arm of a `?:` even when the two arms emit byte-identical
code (`CodeGenFunction::getLandingPad` caches on the innermost `EHScope`;
`EmitConditionalOperator` evaluates each arm in its own conditional
evaluation).  An `if/else` whose two arms are simple assignments to the same
local merges into one call-site row.

IR evidence (our object, `-S -emit-llvm`):

```
; if/else form (before)
  %413 = invoke i32 @_ZN4Item16getStationAmountEv(...) unwind label %421
  %416 = invoke i32 @_ZN4Item9getAmountEv(...)         unwind label %421   ; same LP -> one row
...
; ternary form (after)
  %... = invoke i32 @_ZN4Item16getStationAmountEv(...) unwind label %A
  %... = invoke i32 @_ZN4Item9getAmountEv(...)         unwind label %B     ; distinct LPs -> two rows
```

The same happens in the `else` arm (`getAmount` vs `getCurrentAmount`), so the
fix restores **two** original row splits, not one.

## 3. LSDA row census (exact)

Commands (scratch parser `/dev/shm/w6933/lsda.py`, same layout as
`exidx_table.parse_lsda`; LSDA starts at extab target + 16):

```
python3 /dev/shm/w6933/lsda.py <so> _ZN12HangarWindow6renderEv
```

| | original | before | after |
|---|---:|---:|---:|
| record bytes | 948 | 944 | **948** |
| header (`lps/tt/cs`) | `ff/ff/01` | `ff/ff/01` | `ff/ff/01` |
| `cslen` | 927 | 923 | 924 |
| rows | 156 | 156 | 156 |
| row-byte histogram | 20x5,125x6,11x7 | 21x5,127x6,8x7 | 21x5,126x6,9x7 |
| rows with `len>=0x80` | 18 | 14 | 15 |

The after record is `16 + 5 + 924 = 945` content bytes, padded by the section
aligner to 948.  The original is `16 + 5 + 927 = 948` with no padding.  So the
**section size now matches**, but the last 3 bytes of the record are padding
rather than real rows — the remaining gap is exactly the 3 missing long rows
below.

### The amount block now matches the original

| row | original | before | after |
|---|---|---|---|
| tab==1 `getStationAmount` | `(0x57e, 4, lp)` | merged `(0x580, 0x12)` | **`(0x57e, 4)`** |
| tab==1 `getAmount` | `(0x588, 4, lp)` | (same row) | **`(0x588, 4)`** |
| else `getAmount` | `(0xbb0, 4, lp)` | merged `(0xbb0, 0x1e)` | **`(0xbb0, 4)`** |
| else `getIndex`+`getCurrentAmount` | `(0xbc0, ...)` | (same row) | **`(0xbc0, 0x10)`** |

Only the landing-pad *addresses* differ (by 4, from the 4-byte code shift);
start/length now match the original exactly.

### The 3 rows still missing (all code-layout, not EH-shape)

| original long row | ours (after) | source region |
|---|---|---|
| `[39] (0x9cc, 0x96)` | `[38] (0x9cc, 0x76)` short | ship branch: `getShip()` is in the *next* row |
| `[40] (0xa88, 0x8c)` | merged into `[39] (0xa62, 0xb2)` | ship branch: `buttons[1]`+upgrade block |
| `[135] (0x1d8e, 0x9c)` | `[135] (0x1de8, 0x6a)` short | buy-credits non-listMode gap row |

These account for the remaining `cslen` delta (`927 - 924 = 3`).  They are
**not** reachable by the amount-block change; see §5.

## 4. What the `pad3` anchor was doing

`volatile int pad3 = left;` is a scalar local; clang emits
`EHStack.pushCleanup<CallLifetimeEnd>(NormalEHLifetimeMarker, ...)` for it
(`CGDecl.cpp:1228`), and `NormalEHLifetimeMarker = LifetimeMarker |
NormalAndEHCleanup` is an **EH** cleanup (`EHScopeStack.h:95`).  That extra
cleanup scope is the innermost EH scope for the `buttons[8]` calls, so their
landing pad differs from the `buttons[9]` calls and the row splits in the
wrong place (3 rows instead of the original's 1).  Removing it merges the
`buttons[8]`/`buttons[9]`/`getWidth` calls into the original single long row
`(0xbc0, 0x9a)`.

The same mechanism explains why `padX` (`volatile int *padX = 0;` in the ship
branch) is still load-bearing: it creates the extra landing pad that splits
the ship branch.  Its split point is wrong (§5), but deleting it merges the
whole branch into one 362-byte row (`extab` 936).

## 5. Dead ends measured this pass (all with the ternary + `pad3` gone)

| variant | extab | text | rows | long | note |
|---|---:|---:|---:|---:|---|
| **landed** | **948** | **9700** | 156 | 15 | |
| ternary, keep `pad3` | 956 | 9704 | 158 | 14 | |
| `if/else`, no `pad3` | 936 | 9696 | 154 | 15 | |
| ternary, `left` declared first | 948 | 9696 | 156 | 15 | body -4 |
| ternary + free-credits `getWidth`/`getHeight` swap | 948 | 9700 | 156 | 15 | identical rows |
| ship `if/else` (no `padX`) | 936 | 9700 | 154 | 15 | branch merged |
| ship `padX` moved inside `if (!(...))` | 948 | 9676 | 156 | 15 | getShip joins outer LP, but body -24 |
| ship inner `int padY = 0;` (keep `padX`) | 956 | 9692 | 158 | 14 | getShip still separate |
| ship inner `int bx/by` locals | 956 | 9692 | 158 | 14 | |

The ship-branch split needs `getShip()` in the **outer** landing pad and
`buttons[1]`+upgrade block in an **inner** one.  Moving `padX` inside the
inner `if` achieves exactly that row grouping (`getShip` -> outer LP), but the
cleanup dispatch table shrinks by 24 B, so the body is 9676 B.  A local
declared after `getShip` but scoped over both the `buttons[1]` and
`upgradeMode` blocks would reproduce the original grouping at the original
size; no such declaration shape has been found yet.  The free-credits gap row
is a pure body-layout difference (the original's gap is 0x9c, ours 0x6a) and
is not fixed by the `getWidth`/`getHeight` swap.

## 6. Section / gate impact

| section | before | after | original |
|---|---:|---:|---:|
| `.text` differing bytes | 88,125 | **88,063** | 0 |
| `.ARM.exidx` differing bytes | 1,674 | **1,663** | 0 |
| `.ARM.extab` size | 156,304 (-8) | **156,308 (-4)** | 156,312 |
| `.ARM.extab` differing bytes | 135,823 | **135,588** | 0 |
| `.rodata` base / size | `0x1c9e10` / 269,351 | same | `0x1c9e10` / 269,159 |
| file size | 2,187,328 | 2,187,328 | 2,187,328 |

No function address moves: the +4 grows `.ARM.extab` inside the 16-byte
`.rodata` alignment gap, so `.rodata` stays at `0x1c9e10`.  `.text`,
`.ARM.exidx`, `.rodata` and the file size are unchanged.  The only remaining
`.ARM.extab` shortfall is `Generator::createMission` (-4).

`docs/VALIDATION.md` and `docs/IDENTICALITY.md` inline numbers are updated in
the same commit (`.ARM.extab` 8 B short -> 4 B short; `.text` 88,125 ->
88,063; `.ARM.exidx` 1,674 -> 1,663; `.ARM.extab` 135,831 -> 135,588).

## 7. Proven vs hypothesis

**Proven**

* The selected-item `right`/`left` block is a pair of conditional operators in
  the original: converting it to two `?:` initializers reproduces the
  original's two 4-byte cleanup rows in *both* arms (tab==1 and else).
* `pad3` was a codegen anchor whose `volatile` store (and its
  `NormalEHLifetimeMarker` cleanup) split the `buttons[8]` block; deleting it
  restores the original single long row and exactly cancels the ternary's
  +4 B, keeping the body at 9700 B.
* With both edits the `render` `.ARM.extab` record is exactly 948 B and the
  section grows `0x26290 -> 0x26294`; the gate is unchanged
  (`byte 4505 / linked 4505`, data 843/843, imports 207, lint CLEAN, drift 0,
  27 sections, file size exact).
* The fuzzy score improves 91.3 -> 91.7 (official rounded metric); the
  differing `.text` bytes fall 88,125 -> 88,063.

**Hypothesis / open**

* The remaining 3 bytes of `cslen` (and the 3 missing `len>=0x80` rows) are
  the ship-branch split (`getShip` must join the outer landing pad while the
  `buttons[1]`+upgrade block keeps an inner one) and the buy-credits
  non-listMode gap row.  Both are body-layout consequences; the `padX` anchor
  currently splits the ship branch at the wrong call.
* A source shape that puts `getShip` in the outer landing pad without
  shrinking the cleanup dispatch table by 24 B was not found (see §5).

## 8. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint -j6       # CLEAN
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
python3 tools/verify/verify.py --show _ZN12HangarWindow6renderEv
```

Scratch helpers (not committed): `/dev/shm/w6933/` — `measure.py` (compile one
TU, read `.ARM.extab.text._ZN12HangarWindow6renderEv`, parse the LSDA),
`variant.py` / `v2.py` / `v3.py` (apply string replacements and measure),
`lsda.py` / `rowsobj.py` (row dumps), `rowcalls.py` (row -> call list),
`dis.py` (side-by-side Thumb disassembly).
