# `Layout::Layout` — wave 173 (w11812): store-order reorder

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (`src/game/ui/Layout.cpp`, 7,072 B,
base raw 5,640 / aligned 2,282).  The landed change is a pure source-order
reorder of four neighbouring store groups:

* `field_0x174` after `field_0x170`/`field_0x17c` instead of before them.
* `field_0x284_sliderSlot5Enabled` before `field_0x2fc`/`field_0x300`.
* `choiceWindowTitleHeight` after `field_0x300` instead of after `field_0x304`.

No values, types, or control flow change.  Measured on the wave-172 base:

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 5,640 | **5,633** |
| aligned insns | 2,282 | **2,290** |
| size | 7,072 | 7,072 |
| `.text` differing bytes | 81,503 | **81,496** |

The full `check` gate is green (`byte_exact 4507/4524`, linked 4507, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical).  The change is additive with the other wave-173
wins; the combined tree is recorded in the wave-173 `docs/IDENTICALITY.md`
section.
