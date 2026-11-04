# Layout::Layout C1/C2 — Ghidra block decomposition, literal-pool placement,
# and a fresh size-7072 order search (w4801, wave 98)

Worker **w4801**, branch `wave98-w4801`, base `fe4f5e7cf`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(Ghidra image base `0x10000`; Ghidra address = ELF VMA + `0x10000`, so the ctor
is `0xe1028` in the Ghidra project).

Continues `layout-layout-c1-c2-hd-store-table-w4701.md` (wave 97, the landed
47.733 state), `layout-layout-c1-c2-store-order-w4502.md`,
`layout-layout-c1-c2-hd-vector-w4101.md` and
`_work/reconstructions/layout_ctor/ios_source_order.md`.

## Result

**No landable improvement found.** The tree stays at **7072 B /
47.7326968973747 %** (unrounded `fscore.py`, `autojunk=False`). This pass
contributes:

1. a **full block decomposition** of the original ctor from the Ghidra
   decompilation and the disassembly (§1), which shows the source is a
   tail-merged four-way flag tree plus a common mid block and tail;
2. the **literal-pool placement result** (§2): the original's end pool starts
   with `0.325, 0.65, 100, 50` at `0xd2854`; ours puts `0.325/0.65` in a
   nearby island at `0xd1920` and starts the end pool at `0xd2908` with
   integers. This is the dominant remaining byte-level difference;
3. a fresh, exact-source **size-7072 order search** (§3): all 201 adjacent
   swaps, 300 random block moves of length 1–4, the iOS order, an in-place HD
   reorder, and several targeted group moves. None beats 47.733 at 7072 B.

Gate is unchanged and green (`check` on the pristine tree):

| metric | value |
|---|---:|
| `byte_exact` | 4500 / 4524 |
| `linked_exact` | 4500 / 4524 |
| `size_exact` | 4524 / 4524 |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 |
| `data_exact` | 843 / 843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |

`RATCHET PASS`.

## 1. Full original block decomposition (new)

The original is a tail-merged branch tree. Using the store parser
(`stores.py`, capstone, `skipdata=True`; it tracks `r0`-relative bases through
`add/adds/add.w/addw` and expands `vst1.32`, `strd`, `stmia`, `vstr`, `strb`)
over the original function gives these disjoint regions:

| region | original range | stores (fields) |
|---|---|---:|
| HD (`iPadHD`) | `d1042..d1268`, `b.w d21e2` | 137 |
| n9 | `d127e..d12fc`, `b.n d1860` | 34 |
| retina/default | `d1570..d185e`, fall-through | 34 |
| mid | `d1860..d215c`, `b.n d21e2` | 105 |
| tail | `d21e2..d2bd0` | 71 |

The union is exactly the 204 `Layout` fields. The field sets are **not**
disjoint:

* **HD-only (33)**: `0x4..0x78` (30) + `0x2c4,0x2c8,0x2cc`.
* **HD∩mid (104)**: `0x7c..0x1e0` (90) + `0x294..0x2b8` (10) +
  `0x2c0,0x2d0,0x2d4,0x2d8` (4).
* **tail-only (66)**: `0x1e4..0x290` (44) + `0x2f4..0x320` (12) +
  `0x3e0..0x3fc` (8) + `0x2bc`.
* **tail∩HD/mid (5)**: `0x130,0x134,0x298,0x29c,0x2bc`.

This is exactly the iOS source order cut at the 137/34/105/71 boundaries:
the HD branch assigns the first 137 iOS-order fields, the n9/retina/default
branches assign the next 34, the common mid block assigns the same 104
non-HD values as the HD branch's second half, and the tail assigns the last
66. The current flat-ternary source compiles to the same tree (w4101 §2), so
the source form is not the residual.

The Ghidra decompilation (`ghidra_decomp_addr_w3742.py 0xe1028 0x1ba0`) is
consistent with this: the top-level test is `if (!iPadHD)`, the second is
`if (!n9)`, and the non-HD path contains the per-field conditionals. The
decompiler's store order is the original scheduler order, not source order.

## 2. Literal-pool placement (new, and the dominant residual)

The original's end-of-function literal pool starts at **`0xd2854`**:

```
0xd2854  3ea66666 3f266666 42c80000 42480000   ; 0.325, 0.65, 100, 50
0xd2864  0013df2e 0013de3c 0013ddb4 0013dd96   ; addresses
0xd2874  438ca000 43c80000 0013dc9a 43480000   ; 281.25, 400, addr, 200
...
```

Our end pool starts at **`0xd2908`** with integers; the `0.325/0.65` pair is
instead in a nearby island at **`0xd1920`**:

```
ours 0xd1920  3ea66666 3f266666 ...
ours 0xd2908  00000004 00000016 00000019 ...
```

The original loads the pair from the end pool with
`addw r1, pc, #0xf08` at `d1948`; ours loads it from the nearby island with
`subw r5, pc, #32` at `d1936`. The `100/50` pair is likewise in the end pool
in the original and elsewhere in ours. Because the verifier compares the
literal-load instructions and their immediates, this shifts a large run of
tail/mid instructions even when the surrounding code is otherwise identical.
It is a **pool-island placement** difference, not a missing statement or a
wrong value.

The original's first end-pool reference is the mid-block `field_0x8c/0x90`
code; the current build's first end-pool reference is later (`d1a30`), because
clang chose a local island for `0.325/0.65`. Forcing the constants into the
end pool is not reachable by source-order edits alone: moving `field_0x90`
and the `field_0x9c` if/else block to the front of the ctor gives 6836 B /
34.84 %, and moving `field_0xc0` to the front gives 7008 B / 39.43 %.

## 3. Exact-source size-7072 order search (new)

The previous wave's searches used a merged statement list that is not
source-equivalent (joining it gives 7000 B, not 7072). This pass re-splits the
ctor into its **202 top-level source units** (an `if/else` chain is one unit;
joining the units reproduces the pristine source exactly and measures 7072 B /
47.7326968974 %). All searches below use that exact decomposition.

| variant | `usize` | raw % |
|---|---:|---:|
| pristine tree (baseline) | 7072 | **47.7326968974** |
| all 201 adjacent swaps | — | only the 6 size-7072 swaps are score-neutral; every other swap changes size |
| 300 random block moves (length 1–4, size-7072 filter) | 7072 | no improvement |
| full iOS source order (current expressions) | 6784 | 35.4760 |
| HD fields reordered in place to iOS order | 6876 | 35.0424 |
| `0x2c4,0x2c8,0x2cc,0x2d0,0x2d4,0x2d8` → iOS order | 7072 | 47.5831 |
| `0x1e0` moved before `0x1e4` | 6984 | 40.5449 |
| `0xc0` moved before `0x4` | 7008 | 39.4332 |
| first 34 fields as explicit `if/else` (current order) | 6792 | 27.5147 |
| all ternaries → explicit `if/else`, iOS order | 6896 | 18.0625 |

The best size-changing variant remains the known `0xac` before `0x98`
adjacent swap: **6976 B / 48.1022772673 %**. The best 7072 B variant is still
the pristine tree.

The `rewardBoxY2` 7024-byte body (w4701 §6) still measures **7024 B /
47.9233226837 %**. The +48 B needed to land it cannot be supplied by the
11-edit set: `0x150` all-float is +8 B, `0x158`/`0x15c` reorders are +16 B,
`0x160` all-float is +24 B, but the deltas are not additive
(`A+B+J` = 7064 B / 47.1691, `A+C+J` = 7064 B / 47.1292).

## 4. Irreducible residue

The remaining 52.27 % is **not reachable by local source edits on this tree**:

1. **Register allocation.** The original prologue saves six registers
   (`r6,r7,r8,r9,sl,fp`); ours saves seven (`+r5`). Our scheduler keeps
   `r5 = r0 + 0x1c0` live across the HD block; the original recomputes
   `add.w r1, r0, #448` at `d1232`. This shifts every register assignment
   from the first HD store onward.
2. **Literal-pool islands.** The original's `0.325/0.65/100/50` are in the
   end pool at `0xd2854`; ours has `0.325/0.65` in a local island at
   `0xd1920` and starts the end pool at `0xd2908`. The load immediates and
   pool contents therefore differ for a long run of mid/tail instructions.
3. **SLP phase.** The original groups `0x140..0x17c` as four aligned
   four-word vectors; ours is shifted two words because `0x140/0x144` are not
   source-adjacent to `0x148/0x14c` in the current order. Every individual
   move tried loses more than it gains (w4502 §5, w4701 §5).

A from-scratch branch-tree body that reproduces the original's exact
`if/else` nesting and the original's constant-use order is the only remaining
route; every automated variant tried in this pass is smaller than 7072 B and
scores worse.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
python3 /dev/shm/w4801/fscore.py                            # 47.7326968974, 7072 B
python3 /dev/shm/w4801/stores.py <so> _ZN6LayoutC1Ev 0xd1042 0xd1268
python3 /dev/shm/w4801/split_units.py                       # 202 exact units
python3 /dev/shm/w4801/adj2.py                              # adjacent swaps
python3 /dev/shm/w4801/rand2.py 300                         # random block moves
```

Scratch (`/dev/shm/w4801`): `fscore.py` (official unrounded ratio, no link),
`harness.py` (copy source → `link_so` → score), `stores.py` (capstone store
parser with `skipdata`), `split_units.py` (exact 202-unit splitter),
`adj2.py`, `rand2.py`, `ios_allif.py`, `reorder_hd_inplace.py`,
`reorder_group.py`, `move_unit.py`, `Layout.base.cpp`,
`Layout.7024.cpp`, `Layout.ios.cpp`.

The Ghidra decompilation is at `/dev/shm/w4801/layout_decomp.c`
(`ghidra_decomp_addr_w3742.py /home/debian/GalaxyOnFire2 GalaxyOnFire2 /
android_2.0.16_libgof2hdaa.so 0xe1028 0x1ba0 300`).
