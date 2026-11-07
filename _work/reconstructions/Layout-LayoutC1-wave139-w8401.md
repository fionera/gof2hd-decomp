# Layout::Layout — wave 139 / w8401 (fuzzy-first reorder, raw 5,775 -> 5,765)

Status: **landed by the coordinator from w8401's uncommitted sweep result**
(the agent overran its 45-minute budget; its worktree diff was evaluated in
the main tree).

The reorder keeps the wave-138 w8301 ascending `0xb8..0x118` region and adds
three adjacent-statement moves found by a fuzzy>=45.9 sweep:

* `field_0x298_buttonYBias` before `field_0x2a0_listRowGap`
* `field_0x1c` after `field_0x10_rightMargin`
* `buttonInsetX` after `field_0x2c_rowHeight`
* `field_0x114_strip5c` after `field_0x108`
* `rewardBoxX` after `rewardBoxHeight`

Measured on the wave-139 master:

| metric | before | after |
|---|---:|---:|
| C1 raw same-address diff | 5,775 | **5,765** |
| official fuzzy | 45.9 % | 45.6 % |
| size | 7,072 B | 7,072 B |

The raw byte diff is the deterministic metric (the official fuzzy pct varies
with `PYTHONHASHSEED`), so the -10 raw bytes are kept.
