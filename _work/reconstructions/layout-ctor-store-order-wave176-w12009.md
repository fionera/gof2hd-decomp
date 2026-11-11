# Layout::Layout store-order pass (wave 176, w12009)

`Layout::Layout()` (`_ZN6LayoutC1Ev` / `C2Ev`) is a long straight-line sequence of
member stores. The original store order is not the declaration order. Reordering
the 204 parsed statement units with a width-5 window permutation search found:

- base (wave 175): raw 5,620, size 7,072/7,072
- best: raw 5,611, size 7,072/7,072 (aligned 2,334)

The landed order is `scratch/w12009/best2_1_5611.json` (unit permutation of
`Layout.base.cpp`). Net source diff: six moved assignment statements plus one
removed blank line before the closing brace.

Key moved stores: `windowTopInset` before `field_0xc_leftMargin`; `field_0x20_top`
before `field_0x40`; `field_0x44` before `field_0x30_rowHeight`;
`field_0x2cc` before `field_0x2d0`; `field_0x2c8` after `field_0x2d8`.

Verified on the worker tree with `tools/fn_diff.py`; re-measure on the combined
wave-176 tree before merge.
