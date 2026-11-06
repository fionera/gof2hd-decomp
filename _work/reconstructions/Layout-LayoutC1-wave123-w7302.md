# `Layout::Layout` C1/C2 — wave 123 / worker w7302

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).

## Result — no source win

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.704663212435 % | **48.704663212435 %** |
| `report.json` pct (1 dp) | 48.7 | 48.7 |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note.

## 1. What the original actually is

The original is **not** a flat field-major ternary chain, but it is also not
the wave-121 iOS-order config-major body.  The exact shape recovered from the
executed store trace and the CFG is:

```
if (iPadHD) {                       // 0xd1042..0xd1268, 137 stores
    /* HD constants in the HD trace order */
} else {
    if (n9) { /* first group, n9 values */ }
    else    { /* first group, retina/iPad values */ }
    /* common1: a tree of if/else groups, 0xd1860..0xd21e0 */
}
/* common2: field-major ternaries, 0xd21e2..0xd2800 */
```

The HD block is a run of 4-word `vst1.32 {dN-dN+1}` stores.  The first
store is at `0x14` and writes `0x14..0x20`; the second at `0x4` writes
`0x4..0x10`; etc.  The exact HD trace order (first write to each word) is:

```
14 18 1c 20 4 8 c 10 24 28 2c 30 34 38 3c 40 44 48 4c 50 54 58 5c 60 64 68
6c 2c0 70 74 78 2c4 2c8 2cc 2d0 2d4 2d8 7c 80 84 88 8c 90 94 98 9c ac a8 a0
a4 b0 b4 b8 bc c0 c4 c8 cc d0 d4 d8 dc e0 e4 e8 ec f0 f4 f8 fc 100 104 294
298 29c 2a0 2a4 2a8 2ac 2b0 108 10c 110 114 118 11c 120 124 d0 d4 d8 dc e0
e4 e8 fc 100 104 2b4 2b8 128 12c 130 134 140 144 148 14c 150 154 158 15c
160 164 168 16c 170 174 178 17c 180 184 188 18c 190 194 198 19c 1a0 1a4
1a8 1ac 1b0 1b4 1b8 1bc 1c0 1c4 1c8 1cc 1d0 1d4 1d8 1dc 138 13c 1e0
```

(duplicates in the list above are the same word reached through different
`vst1` groups; the unique-word order is in
`/dev/shm/w7302/order_hd_trace.txt` during the pass.)

The common1 block is a **tree of groups**, not a per-field ternary chain.
Examples from the CFG (default config, `emu3.py` visited-set + store trace):

| block | condition | fields stored in the block |
|---|---|---|
| `0xd1860` | `n9 == 0` → `0xd1904` | `2d0 2d4 2d8` |
| `0xd18bc` | n9 path | `7c 80 84 88` |
| `0xd1904` | non-n9 path | `7c 80 84 88` |
| `0xd1ac6` | `retina == 0` → `0xd1ca6` | `a0 a4 b0 b4 b8 bc c0 c4 c8 cc d0 d4 d8 dc e0 e4 e8 ec f0 f4 f8 fc 100 104 108 10c 110 114 118 11c 120 124 128 12c 294 298 29c 2a0 2a4 2a8 2ac 2b0 2b4 2b8` |
| `0xd1cba` | retina path | `138 13c 140 144 148 14c` |
| `0xd20b4` | default tail | `1b0 1b4 1b8 1bc 1c0 1c4 1c8 1cc 1d0 1d4 1d8 1dc 1e0` |

This grouping is what makes the original common1 2434 B long.  A per-field
ternary or per-field `if/else` chain compiles much smaller.

## 2. Variants measured (real `link_so` builds, exact unrounded ratio)

All variants below are semantically exact for all 32 flag combinations
(verified with `emu2.py`/`cmp32.py`: 201 words, 0 differences).

| variant | size | unrounded % |
|---|---:|---:|
| **current field-major baseline** | **7072** | **48.704663** |
| current + `field_0x124` HD arm `(int)(1*1.40625f)` | 7064 | **48.724083** |
| current + `field_0x128` HD arm `(int)(1*1.40625f)` | 7064 | 48.724083 |
| config-major, HD trace order, final ints | 6580 | 21.3 |
| config-major, HD trace order, current expressions | 6368 | 24.5 |
| config-major, iOS order, final ints (wave-121) | 6672 | 27.7 |
| config-major, iOS order, current expressions | 6788 | 13.7 |
| config-major, per-field `if/else` (iOS order) | 6664 | 28.8 |
| config-major, per-field `if/else` (trace order) | 6416 | 15.3 |
| config-major, full 4-flag decision tree | 11856 | 12.3 |
| current order, 9 SLP groups moved contiguous | 7004 | 42.1 |
| current + `0xb0..0xbc` moved contiguous | 7096 | 44.1 |

The 7064-B body is the only variant that beats the baseline on the unrounded
metric, but it shifts `.text` and drops `byte_exact` from 4506 to 3222.
The 7072-B wall is mandatory.

## 3. Why the config-major body is short

The original common1 tree duplicates stores in each branch.  A source that
writes each field once (ternary or `if/else` chain) produces ~400 B less code
than the original.  Reproducing the exact tree requires emitting the CFG
groups, which is straightforward but the resulting body is 11.8 KB if every
leaf is expanded.  The original sits between the two extremes because clang
outlines/merges some leaves.  Reconstructing the exact outline decisions is
the remaining work.

## 4. Searches that failed to beat the baseline at 7072 B

* all 201 adjacent unit swaps (`swap_search.py`): no 7072-B hit above
  48.704663.
* 23 single-field round-trip edits that restore 7072 B from the 7064-B body
  (`(int)(float)`, `(int)(float)(int)`, `(float)`, `(int)` wraps): best is
  48.704663 (neutral); most are below.
* moving each of the 9 original-only SLP groups contiguous: all change the
  size (7004-7096) and score below 48.7.
* config-major HD/n9/else/common1/common2 bodies in the HD trace order, the
  iOS source order, and the default trace order: all 6.3-6.8 KB.

## 5. Remaining root cause

The residual is clang 7.0.2's SLP vectorisation and register allocation on
the common1 tree.  The current 7072-B body is a hill-climbed field-major
order that happens to hit the exact size and matches ~48.7 % of the linear
instruction stream.  The true config-major source needs the original's
grouped-store tree (and its outline decisions) to reach 7072 B; a readable
per-field reconstruction is ~400 B short.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7302
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7302/score_one.py cmake-build-match/libgof2hdaa.so
```

Scratch harnesses (not committed): `/dev/shm/w7302/emu2.py` (store emulator),
`emu3.py` (adds visited-pc set), `cfg.py`/`cfg3.txt` (common1 CFG),
`gen_trace.py`, `gen_trace_expr.py`, `gen_if.py`, `gen_tree.py`,
`fastsize2.py`, `fastscore.py`, `swap_search.py`, `search_size2.py`.
