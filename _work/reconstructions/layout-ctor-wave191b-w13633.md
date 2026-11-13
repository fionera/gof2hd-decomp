# `Layout::Layout` C1/C2 — wave 191b / worker w13633

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13633`, base master `4abbb1be8` (after w13607).

**Result: one size-exact *raw* win landed (raw −3, aligned +28).**

| | before | after |
|---|---:|---:|
| raw | 5,235 | **5,232** |
| aligned | 2,668 | 2,696 |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,517/2,552 | 2,516/2,552 |
| `.text` diff | 71,197 | **71,194** |
| `.ARM.extab` diff | 577 | 577 |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## The win

Three independent, semantically-null statement moves in the ctor:

1. Swap the `field_0x2cc` and `field_0x2d0` stores (source order `2cc` then
   `2d0`).  This restores the original's instruction classes for that pair:
   the original emits `str.w [r0,#716]` for 0x2cc and `vstr s0,[r0,#720]` for
   0x2d0; the w13607 order had them reversed.  (The dynamic store order is
   unchanged: the emulator trace is still `2c8 2cc 2d0 2d4 2d8`.)
2. Hoist the `volatile unsigned char *rd = &Globals::retinaDisplay;`
   declaration above that pair (temporary-lifetime change; `rd` is first
   *used* in the 0x2d4 chain, so this is legal and semantics-free).
3. Move the deferred `field_0x2c8 = (int)(float)t_field_0x2c8;` store after
   the `field_0x2d8` store (still after its local's declaration).

Plus one independent store swap:

4. `field_0x50` (0x50) and `field_0x74` (0x74) exchange source positions.

All four fields are independent; the store image was verified identical for
all 32 combinations of `iPadHD/n9/retinaDisplay/iPad/iPadLarge`
(`scratch/w13633/valall.py`, 0 configs differ).

The raw win comes from the interaction of (1)+(2): neither alone helps
(2cc/2d0 alone is 5,239/2,663; rd-hoist alone is 5,236/2,701), but together
they are 5,232/2,700.  Adding (4) gives 5,232/2,696.

## Measured candidates (all real `link_so` builds, size-exact unless noted)

Exhaustive pair swap of all 990 unit pairs in the first 45 ctor statements
(object-size prefilter, 59 size-exact survivors):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (w13607) | 5,235 | 2,668 | 7,072 |
| swap `field_0x50`/`field_0x74` (units 18,29) | 5,235 | 2,664 | 7,072 |
| swap `field_0x3c`/`field_0x2c_rowHeight` (13,15) | 5,239 | 2,639 | 7,072 |
| 18,29 + 13,15 | 5,239 | 2,636 | 7,072 |
| 18,29 + 13,15 + 2cc/2d0 | 5,243 | 2,631 | 7,072 |
| 2cc/2d0 + rd-hoist | 5,232 | 2,700 | 7,072 |
| **2cc/2d0 + rd-hoist + 18,29 (landed)** | **5,232** | **2,696** | 7,072 |

The raw 5,232 point is only reachable with (1)+(2); every raw-5,232 survivor
has aligned ≥ 2,696.  The aligned-favouring variants (13,15 etc.) pay a raw
cost.  `(18,29)` alone is the only strict Pareto improvement found
(raw unchanged, aligned −4) and is recorded here as the fallback.

## Negative / neutral results this session

* **Full adjacent swap of all 231 top-level ctor units** (230 builds):
  16 size-exact, best raw 5,235; best aligned 2,627 (swap 120, raw 5,294).
* **1,200 random single/double unit swaps** across the whole ctor (object-size
  prefilter): no raw < 5,235; best aligned 2,648 (raw 5,499).
* **Full ascending-by-offset sort of the 189 simple assignment statements**
  (`sortvar.py`): 6,230 / 3,588 / **6,836** (size-fatal).  The original source
  order is not ascending; the current arrangement is a narrow 7,072 ridge.
* **All 1,486 combinations of `{2cc/2d0, rd-hoist} + up to 2 pair swaps`**:
  1,164 size-exact; raw 5,232 only with aligned ≥ 2,696.
* **`const`-ifying `t_field_0x2c8`, `v`, or `rd`**: codegen-identical.
* **Adding `(int)(float)` / `(float)` to `field_0x2cc`**: codegen-identical
  (the compiler folds it); `(int)(float)(int)` is 5,392 / 7,080 (fatal).
* **Removing the three dead `footerTextInset` stores in the opening
  if/else chain**: 5,571 / 2,685 / 7,072 (raw worse).
* **Moving the `rd` declaration alone to any of units 15..37**:
  codegen-identical (5,235 / 2,668).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13633
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5232 aligned=2696 size=7072/7072 insn=2516/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13633/`: `base.cpp`, `units.py`,
`hdr.py`, `search.py`, `search2.py`, `search3.py`, `fastsearch.py`,
`fastrandom.py`, `combosearch.py`, `combosearch2.py`, `focused.py`,
`combo.py`, `scan_rd.py`, `sortvar.py`, `emu.py`, `valall.py`, `secdiff.py`,
`measure.sh`, `compile_cmd.txt`.
