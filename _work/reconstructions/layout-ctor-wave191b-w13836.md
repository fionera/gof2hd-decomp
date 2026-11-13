# `Layout::Layout` C1/C2 — wave 191b / worker w13836

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13836`, base master `da998189d` (raw 5,572 / aligned 2,012 /
size 7,072).

**Result: one size-exact aligned+raw win landed; size unchanged.**

| | before | after |
|---|---:|---:|
| raw | 5,572 | **5,571** |
| aligned | 2,012 | **2,011** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,516/2,552 | 2,516/2,552 |
| `.text` diff | 76,052 | **76,051** |
| `.ARM.extab` diff | 596 | 596 |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical (29,392 B); all ELF section sizes unchanged.

## Landed step

| step | swap | raw | aligned | size | insn | `.text` | extab |
|---|---|---:|---:|---:|---:|---:|---:|
| 1 | `s(141,196)` | 5,571 | **2,011** | 7,072 | 2,516 | 76,051 | 596 |

`s(141,196)` swaps the independent, side-effect-free stores
`this->field_0x94` (`Globals::iPadHD ? 14 : (n9 ? 15 : (retina ? 20 : 10))`,
source line 208) and `this->field_0x1ec`
(`Globals::iPadHD ? 26 : (n9 ? 34 : (retina ? 38 : 19))`, source line 303).
Both write distinct scalar fields and read only `Globals` flags, so the store
image is unchanged for every flag combination.  This is the same pair the task
flagged as `field_0x1ec`/`field_0x94`; the earlier w13808 `s(141,197)` win was
on a different base and no longer improves after w13816.

## Search (from the current 5,572/2,012 point)

* Targeted single-swap sweep, 2,166 candidates: all pairs inside the
  `orbitInfoTextX`/`field_0x20c`/`field_0x200` block (units 140..152), all pairs
  inside units 193..200, every cross pair for units
  `{141,147,148,150,196,197}`, and every single-unit move of those six units
  across units 130..205.  148 size-exact; only `s(141,196)` improves aligned
  (2,011/5,571).  Next best size-exact: `m(150,154)`/`m(150,155)`/`mb(150,153)`
  at raw 5,571 but aligned 2,012 (raw-only, not landed).
* Second-generation combinations on top of `s(141,196)` (147 candidates: the
  swap plus each size-exact candidate from the first sweep, plus the four
  raw-5,571 moves): best is 2,012/5,572 — no double beats the single.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/scratch/w13836
python3 applyops.py cand.cpp s:141,196
python3 harness.py cand.cpp          # 5571 2011 7072
cd /home/debian/GalaxyOnFire2/wt/w13836
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Scratch artifacts: `base.cpp` (pre-session), `base2.cpp` (landed),
`harness.py`, `applyops.py`, `combo.py`, `sweep.py`, `storeoffs.py`,
`out_target.json`, `out_combo2.json`.

## Second pass (same worker): three independent-store moves

| | before | after |
|---|---:|---:|
| raw | 5,571 | **5,585** |
| aligned | 2,011 | **2,005** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,516/2,552 | 2,516/2,552 |
| `.text` diff | 76,051 | **76,065** |
| `.ARM.extab` diff | 596 | 596 |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS (byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370);
`.rel.plt` byte-identical (29,392 B).  This is an aligned win with a raw
regression (raw is dominated by the global byte shift), the same trade the
`LevelScript`/`HangarWindow` passes accepted.

### Landed steps (from the 5,571/2,011 point)

| step | op | raw | aligned | size |
|---|---|---:|---:|---:|
| 1 | `m(89,84)` — `field_0x114_strip5c` before `field_0x118` | 5,576 | 2,007 | 7,072 |
| 2 | `+ m(196,189)` — `field_0x94` before `field_0x238_blk16.a` | 5,586 | 2,005 | 7,072 |
| 3 | `+ mb(150,153)` — `field_0x200` after `field_0x1fc` | **5,585** | **2,005** | 7,072 |

All three units write distinct scalar fields and read only `Globals` flags (no
`this` read), so the store image is unchanged for every flag combination.

### Search (from the landed 5,571/2,011 point)

* **Distance 13..50 single swaps** (`out_d13_50_b2.json`): 6,631 candidates,
  157 size-exact, best aligned 2,012 (`s(142,189)`); no win.  This space was
  not covered by the earlier distance-1..12 sweep.
* **Targeted 0x94 group** (`out_035.json`): 52 moves/swaps of the
  `0x94`/`0x98`/`0x9c`/`0xa0`/`0xa4`/`0xa8` units and of `0x1ec`/`0x300`;
  only 4 size-exact, best 2,011 (`m(50,51)`, `mb(197,196)`), no win.
  Making the `0x94..0xa0` stores contiguous is still much worse (size).
* **Single-unit moves, units 62..127 radius 8** (`out_mv_62_127.json`):
  2,112 candidates, 391 size-exact; **`m(89,84)` = 2,007/5,576** is the win.
* **Single-unit moves, units 148..205 radius 8** (`out_mv_148_205.json`):
  1,784 candidates, 580 size-exact; **`m(196,189)` = 2,009/5,581** and
  `mb(150,153)`/`m(150,154)` = 2,011/5,570 (raw-only).
* **Combination sweep** (`out_win2.json`, 83 candidates): the three landed
  moves together are 2,005/5,585; `m(89,84)+m(196,189)` alone is 2,005/5,586.
* **Re-sweep from the new 2,005 point**: distance-1..12 swaps
  (`out_d1_12_b3.json`, 2,394) and moves radius 6 over units 80..95 and
  145..200 (`out_mv_b3.json`, 1,726) — 331 / 616 size-exact, none below 2,005.
  The 2,005 point is a local optimum for these neighbourhoods.

### Reproduction

```sh
cd /home/debian/GalaxyOnFire2/scratch/w13836
BASE=base2.cpp python3 applyops.py base3.cpp m:89,84 m:196,189 mb:150,153
python3 harness.py base3.cpp          # 5585 2005 7072
```

Scratch artifacts: `base2.cpp` (first-pass landed), `base3.cpp` (this pass),
`jobs_*.txt` / `out_*.json` for every sweep above.
