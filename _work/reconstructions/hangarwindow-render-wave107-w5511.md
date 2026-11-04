# HangarWindow::render — wave 107 (w5511): LSDA sink relocation

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w5511`, branch `wave107-w5511`, base master
`f857b5171`.

## Result

| metric | before (HEAD f857b5171) | landed |
|---|---:|---:|
| `render` fuzzy | 85.096419 | **86.253444** |
| `render` size | 9700 | 9700 |
| `render` LSDA record | 948 B | **948 B** |
| `.text` size | 0x1337c8 | 0x1337c8 |
| `.ARM.exidx` size | 0x6f70 | 0x6f70 |
| `.ARM.extab` size | 0x26294 | **0x26294** |
| `.rodata` base | 0x1c9e10 | 0x1c9e10 |
| ELF section count | 28 | 28 |
| byte_exact | 4500 | 4500 |
| linked_exact | 4500 | 4500 |
| size_exact | 4524/4524 | 4524/4524 |
| data_exact | 843/843 | 843/843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

The `.ARM.extab` section remains **4 B short of the original** (0x26294 vs
0x26298) and still has **28 sections** (original: 27) — both deltas are
pre-existing and unchanged by this pass. The `render` record itself is
untouched at 948 B (156 rows).

## Landed diff (3 hunks, all in `render`)

```diff
                                     if (left > 0) {
-                                        // Codegen anchors: the dead stores keep the
+                                        // Codegen anchor: the dead store keeps the
                                         // list-body register roles (this -> r6) and
                                         // the 9700-byte body of the original.
                                         volatile int pad3 = y;
-                                        volatile int pad3b = y;
                                         (*this->buttons)[8]->setPosition(
                                             this->hintOffsetX + Globals::layout->buttonInsetX,
                                             this->field_0x114 + y, 17);
-                                        // LSDA sink: keeps the original 156-row
-                                        // .ARM.extab record (no sink -> 155 rows).
-                                        volatile int PADX = 0;
                                         (*this->buttons)[8]->setVisible(true);
                                         (*this->buttons)[8]->draw();
                                     }
@@ -797,6 +793,9 @@ void HangarWindow::render() {
                     } else if (item->isMoveToCargoButton()) {
                         (*this->buttons)[6]->setPosition(this->hintOffsetX + Globals::layout->buttonInsetX, y, 17);
+                        // LSDA sink: splits the item-branch cleanup region so the
+                        // 156-row / 948-byte .ARM.extab record is preserved.
+                        volatile int *S = 0;
                         (*this->buttons)[6]->setVisible(true);
                         (*this->buttons)[6]->draw();
                         continue;
@@ -1047,6 +1046,8 @@ void HangarWindow::render() {
             if (i == 0 || (Globals::options[73] && i == 1) || (Globals::options[74] && i == 2) ||
                 (Globals::options[75] && i == 3)) {
+                // Sink: pins the free-credits block's register allocation.
+                volatile int S2 = 0;
                 visible = false;
             } else {
```

All three sinks are plain `volatile` locals in real C++ source; no asm,
attributes, linker tricks, namespace-scope globals, or data anchors. The
pointer sink (`volatile int *S = 0;`) is used because an `int` sink at the same
site costs 4 B (9704 B body); `int*` keeps the body at 9700 B.

## Measured table (all 9700 B unless noted)

Baseline HEAD: 85.096419, 9700 B, extab record 948 B, section 0x26294.

| variant | raw | extab rec | size |
|---|---:|---:|---:|
| HEAD (pad3+pad3b+PADX before b8 setVisible) | 85.0964 | 948 | 9700 |
| plain (no pad3/pad3b, no PADX) | 85.5883 | 940 | 9696 |
| no PADX only | 86.8320 | 940 | 9700 |
| no PADX + ptr sink before b8 `setVisible` | 85.5608 | 948 | 9696 |
| no PADX + ptr sink before b8 + `S2` before `visible=false` | 86.1433 | 948 | 9700 |
| no PADX + `pad3` + `S2` + **ptr sink at buttons[6] (LANDED)** | **86.2534** | **948** | **9700** |
| no `pad3` at all + `S2` + b6 ptr sink | 87.9890 | 936 | 9700 |
| landed shape with `pad3b` instead of `pad3` | 86.2534 | 948 | 9700 |

Other observations from the sweeps (all 156-row / 948 B unless noted):

- `int` (non-pointer) sinks at the buttons[6] site cost 4 B (9704 B).
- Moving the free-credits sink to any other line in that block scores
  86.0882–86.2259; `visible = false;` is the best slot.
- A free-credits `getWidth`-first ordering costs ~12 fuzzy points (74.x) — the
  `getHeight`-first source shape is kept.
- The `pad3`/`pad3b` pair is redundant for the extab record; one of them is
  enough to hold the 9700-byte body and the list-body register roles.

## What is proven

1. **The 948 B / 156-row record is preserved with the sinks at buttons[6] and
   the free-credits `visible = false;` branch**, at exactly 9700 B, with no
   change to `.ARM.extab` (0x26294), `.ARM.exidx`, `.text`, `.rodata` base, or
   section count.
2. **A single `volatile int *` sink at the buttons[6] `setVisible` boundary is
   the cheapest way to keep the 156th LSDA row** while improving the fuzzy
   score: it splits the item-branch cleanup region without perturbing the
   surrounding register allocation as much as the previous buttons[8] sink.
3. **Removing `pad3b` and the old `PADX` sink is neutral-to-positive** for the
   record and the score; the remaining `pad3` still holds the body size and the
   `this -> r6` list-body register role.
4. **The full gate stays green**: `check` reports byte 4500 / linked 4500 /
   RATCHET PASS, lint CLEAN, drift 0 field(s).

## Hypothesis (not proven)

The original's two landing pads
(`149656 +4 lp=14b5a2` `Item::getStationAmount`, `149660 +4 lp=14b64c`
`Item::getAmount`) are a clang 7.0.2 artifact of two separate cleanup scopes
around the two calls. Our source still merges the remaining cleanup region into
one 18-byte record (`149658 +18`), so a cold-path sink is still required to
reach 156 rows. The remaining fuzzy gap is dominated by register allocation
(`sl`/`r9` swap for `this`/`Globals::layout` at `0x1496da`) and the
free-credits block shape, not by the LSDA sinks.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields
cmake --build cmake-build-match --target lint -j6       # CLEAN

# per-function fuzzy score (verify.py machinery, same normalisation as the gate)
python3 /dev/shm/w5511/score.py _ZN12HangarWindow6renderEv
# LSDA record size for one function
python3 /dev/shm/w5511/exrec.py cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
```

Scratch helpers used for the sweeps live under `/dev/shm/w5511/`
(`score.py`, `measure.sh`, `measure2.py`, `exrec.py`, `sweep*.out`); they are
not part of the commit.
