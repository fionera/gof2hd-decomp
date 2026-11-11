# Layout::Layout — unit swap 72/77 (wave 177, w12110)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (7,072 B), base master `9445ac5fb`
(wave-176 merge, including w12009's 204-unit store order).

A pair-distance sweep over the 204 parsed straight-line statement units found a
single swap that improves the raw same-address diff:

- swap units 72 and 77 (`field_0x2a8_metricB` moved before
  `field_0x298_buttonYBias`; `field_0xf0` moved after `ringMidFarThreshold`)
- raw **5,611 -> 5,604** (-7)
- aligned 2,334 unchanged
- size **7,072 / 7,072** exact; `.rel.plt` unchanged

The unit list is produced by `scratch/w12110/harness.py` (`load()` /
`render()`); the winning pair is recorded in
`scratch/w12110/pairs_true_1_30.json` (`{"d": 5, "i": 72, "j": 77}`).
Re-measure on the combined wave-177 tree before merge.
