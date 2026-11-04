# Layout::Layout C1/C2 — per-block store-order agreement, store-count
# divergence, and the confirmed 7072-byte local optimum (w5001, wave 100)

Worker **w5001**, branch `wave100-w5001`, base `44225192c`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original
size **7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(Ghidra image base `0x10000`, so the ctor is `0xe1028` in the Ghidra project).

Continues `layout-layout-c1-c2-ghidra-order-w4801.md` (wave 98, the landed
47.733 state), `layout-layout-c1-c2-hd-store-table-w4701.md`,
`layout-layout-c1-c2-store-order-w4502.md` and
`_work/reconstructions/layout_ctor/ios_source_order.md`.

## Result

**No landable improvement found.** The tree stays at **7072 B /
47.7326968973747 %** (unrounded `fscore2.py`, `autojunk=False`). This pass
adds three measurement results that were not in the earlier notes and that
close out the remaining "store-order" hypothesis:

1. **Per-flag-config store-order agreement** (§1): the runtime store *order*
   is already 82 % (HD), 81 % (n9) and 53 % (retina/default) identical to the
   original. The retina/default path is the worst, but reordering the source
   to the original store order makes the score *worse*, not better.
2. **Store-count divergence** (§2): ours emits **363** store instructions on
   the default path where the original emits **435** (same 807 bytes of
   fields). Ours SLP-vectorises more aggressively; the original keeps many
   scalar `str`/`strd` stores separated by per-field flag branches.
3. **Exhaustive negative searches** (§3–§5) around the two remaining
   mechanisms (per-field `if/else` shape, `(int)(float)` round-trip type
   shape, and the 7024-byte `rewardBoxY2` body). Nothing at exactly 7072 B
   beats the landed tree.

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

## 1. Per-config store-order agreement (new)

Using the Thumb/VFP emulator (`emu2.py`, the w4301 `emu.py` parameterised by
`EMU_SO`) to trace every store to `this` for six flag configurations, the
sequence of stored field offsets was compared with
`difflib.SequenceMatcher(autojunk=False)`:

| config | orig stores | ours stores | store-order ratio |
|---|---:|---:|---:|
| `iPadHD=1` | 615 | 603 | 0.824 |
| `n9=1` | 573 | 561 | 0.813 |
| `retinaDisplay=1` | 435 | 363 | 0.534 |
| default | 435 | 363 | 0.531 |
| `iPad=1` | 435 | 363 | 0.531 |
| `iPadLarge=1` | 441 | 369 | 0.556 |

**All 807 stored bytes agree in value in all six configurations** (a full
byte-for-byte `this` image diff between the original and the build is empty),
so no value is wrong — the residue is purely instruction selection,
scheduling and register allocation.

The HD and n9 store *orders* are already ~82 % identical. The
retina/default path is the outlier, and it is exactly the path where the
store **count** differs by 72 instructions (§2).

## 2. Store-count divergence (new)

On the default path the original performs **435** store instructions
(`str`/`strd`/`stmia`/`vst1`/`vstr`) to write the 807 field bytes; ours
performs **363**. Both cover the same byte set. Ours merges more adjacent
field writes into `vst1.32`/`strd` pairs.

Concretely, the original's default block starts

```
d1596  ldrb.w r3,[r8]        ; reload the retinaDisplay flag
d159a  cmp    r3,#0
d159c  it     eq
d159e  moveq  r4,#30
d15a0  it     eq
d15a2  moveq  r1,#15
d15a4  cmp    r3,#0
d15a6  it     eq
d15a8  moveq  r2,#29
...
d15da  strd   r1,r4,[r0,#4]      ; 0x4, 0x8
d15de  add.w  r1,r0,#12
d15e2  stmia  r1!,{r2,r5,r6}     ; 0xc, 0x10, 0x14
d15ea  strd   r3,lr,[r0,#24]     ; 0x18, 0x1c
d15ee  strd   ip,ip,[r0,#32]     ; 0x20, 0x24
d15f2  str.w  ip,[r0,#40]        ; 0x28
```

whereas ours emits the same values with a different grouping and a different
register assignment (`str r5,[r0,#36]`, `strd r1,lr,[r0,#4]`,
`strd r6,r2,[r0,#12]`, `strd sl,r3,[r0,#20]`, `str r4,[r0,#28]`, ...). The
instruction *text* therefore differs even where the store order agrees, which
is why the fuzzy metric is only 53 % on this path while the store order is
82–100 % on the field sequence.

The original also spills in this block (`strd fp,sl,[sp]` at `d1570`); ours
does not. That is a register-pressure difference, not a source statement
difference.

## 3. Instruction-match map by original block (re-measured)

`cov2.py` maps every normalised original instruction back to its address and
counts how many are inside an `equal` opcode of the `difflib` alignment:

| original block | range | matched / total | % |
|---|---|---:|---:|
| HD | `d1042..d126c` | 42 / 164 | 25.6 |
| n9 | `d127e..d12fc` | 22 / 40 | 55.0 |
| retina/default | `d1570..d185e` | 167 / 278 | 60.1 |
| mid | `d1860..d21e0` | 221 / 768 | 28.8 |
| tail | `d21e2..d2bd0` | 463 / 942 | 49.2 |
| **total** | | **1200 / 2529** | **47.4** |

The mid block (768 instructions, 28.8 %) is the single largest loss. It is the
non-HD arm of the first 137 source fields plus the per-flag chains for
`0x7c..0x1dc`; its store order is already correct (§1), so the loss is
instruction selection/RA, not statement order.

## 4. Negative searches run this pass

All sizes are real `link_so` sizes; percentages are the official unrounded
`fscore2.py` unless marked *fast* (`fast.py`, ~0.4 s, under-reports by
≈0.08 pp but ranks correctly).

### 4.1 Reorder to the original store order

Built the 204-unit source order by walking the original's runtime store order
per block (HD → n9 → retina → mid → tail, first occurrence wins) and
reordering only the units that belong to each block:

| variant | `usize` | raw % |
|---|---:|---:|
| pristine (baseline) | 7072 | **47.7326968974** |
| mid units → original mid store order | 6808 | 33.8866 |
| tail units → original tail store order | 7012 | 20.1479 |
| HD units → original HD store order | 6944 | 32.1743 |
| full HD+n9+ret+mid+tail store order | 6956 | 33.5476 |
| first 9 default-path stores as source order | 7060 | 43.1076* |
| first 17 / 34 / 60 / 103 default stores | 6972 / 7032 / 7016 / 7036 | 21.3 / 18.9 / 20.6 / 18.9* |
| first 34 units → iOS order | 6980 | 34.7217* |
| mid subset → iOS order | 6928 | 41.7635* |
| units `0x138..0x17c` → iOS order | 7112 | 45.5249* |

Several small iOS sub-reorders (`0x13c..0x148`, `0x144..0x148`,
`0x150..0x160`, `0x158..0x170`, `0x160..0x17c`) are **code-identical** to the
pristine tree (7072 B, 2499 instructions) — the scheduler is insensitive to
them. Every reorder that actually changes the code at 7072 B scores lower.

### 4.2 Per-field `if/else` instead of ternaries

A recursive ternary→`if/else` converter (`tern.py`) rewrites 189 of the 204
units into explicit per-field branch chains, which is the shape the iOS
source has and the shape the original's per-field `ldrb`/`cmp`/`it` runs
suggest:

| variant | `usize` | raw % |
|---|---:|---:|
| all 189 units → `if/else`, current order | 7032 | 20.3696* |
| all 189 units → `if/else`, iOS order | 6776 | 28.5601* |
| units `0..34` → `if/else` | 7012 | 24.1820* |
| units `0..137` → `if/else` | 6780 | 31.5937* |
| units `34..137` → `if/else` | 6844 | 33.8449* |
| units `34..204` → `if/else` | 7088 | 20.6355* |
| units `137..204` → `if/else` | 7176 | 18.9951* |

The `if/else` shape reproduces the original's branch structure (the build
does reload the flag byte once per field, like the original), but it loses
badly on the instruction-sequence metric because the compiler then schedules
the moves differently from the ternary/select form the landed tree uses. The
landed ternary tree is the better *metric* optimum even though the original
source was almost certainly written as `if/else`.

### 4.3 `(int)(float)` round-trip type edits

For every unit whose HD arm is `N * 1.40625f`, converted it to the folded int
literal (`hdint`); for every unit whose HD arm is an int literal, converted
it to `N.0f` (`hdfloat`). 174 candidate edits, all measured:

* 54 `hdint` edits change nothing (code-identical); the ones that do change
  code are all ≤ pristine. The only size-7072 edits are
  `0x204` (2501 insns, 47.5149*), `0x148` is 7056 B / 47.6608*.
* 54 `hdfloat` edits change nothing; `0x188` (`unit 115`) is 7088 B /
  47.8140* (the only variant that beats pristine on the *fast* metric, but it
  is 16 B too large), `0x194`/`0x1a4` are 7080 B / 47.8027*.
* 400 random subsets of 1–4 of those 174 edits: the best size-7072 variant is
  still the pristine tree (fast 47.8123). `0x148 hdint + 0x188 hdfloat`
  lands exactly 7072 B but at fast 47.6228.

### 4.4 The 7024-byte `rewardBoxY2` body, re-exhausted

Deleting the two redundant `iPadLarge`/`retinaDisplay` arms of
`rewardBoxY2` (semantically identical: `(110+17)<<1 = 254`,
`(76+17)<<1 = 186`, `(55+17)<<1 = 144`) gives **7024 B / 47.9633 % (fast)**.
All `2^r`, `r ≤ 4`, subsets of the seven known size-changing edits
(`150f, 160f, 158r, 15cr, 170r, 1e4f, 3f8f, 9cf, 204i, 148i, 1f4i`) were
measured on that body. Exactly **10** subsets land at 7072 B; the best is
`{160f, 158r, 3f8f}` at fast **46.5524** — worse than pristine. So the
+48 B needed to land the higher-ratio body cannot be supplied by these
edits.

## 5. Irreducible residue (updated statement)

The remaining 52.27 % is **register allocation + SLP grouping**, not source
statement order:

1. **Store order is already right.** HD 82 %, n9 81 %; every field value is
   byte-identical in all six flag configurations. Reordering to the original
   store order always lowers the score (§4.1).
2. **Store grouping is wrong.** Ours emits 363 store instructions on the
   default path vs the original's 435; the original keeps scalar
   `str`/`strd` runs between per-field flag branches, ours merges them into
   `vst1` vectors and `strd` pairs. Reproducing the original's grouping
   requires the compiler to schedule the *same* source the same way, i.e. the
   exact original statement order + expression types, which every measured
   variant fails to hit at 7072 B.
3. **Register allocation is wrong from the first HD store.** The original
   prologue saves six registers (`r6,r7,r8,r9,sl,fp`); ours saves seven
   (`+r5`) and keeps `r5 = r0 + 0x1c0` live across the HD block where the
   original recomputes `add.w r1, r0, #448` at `d1232`. The original also
   spills (`strd fp,sl,[sp]`) in the retina/default block; ours does not.
4. **Per-field `if/else` is the true source shape but not the metric
   optimum.** §4.2 shows the branch shape is reproducible but scores 20–34 %
   at sizes 6776–7176 B; the landed ternary tree is a better fuzzy match.
5. **The 7024-byte `rewardBoxY2` body** is a +0.23 pp win at −48 B, but no
   combination of the known size edits lands it at exactly 7072 B (§4.4).

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
python3 /dev/shm/w5001/fscore2.py                           # 47.7326968974, 7072 B
python3 /dev/shm/w5001/cov2.py                              # per-block match map
EMU_SO=.../libgof2hdaa.so python3 /dev/shm/w5001/emu2.py    # store trace
python3 /dev/shm/w5001/tern.py                              # ternary -> if/else
python3 /dev/shm/w5001/scan_edits.py hdint                  # per-unit type edits
python3 /dev/shm/w5001/b7024.py                             # 7024-byte body edits
```

Scratch (`/dev/shm/w5001`): `fscore2.py` (official unrounded ratio, no
link), `harness.py` (rewrite ctor unit order → `link_so` → score),
`fast.py` (0.4 s approximate scorer), `units.py` (204-unit splitter),
`mkorder.py` (unit → field offsets), `emu2.py` (emulator over an arbitrary
`.so`), `cov2.py` (per-block instruction match map), `tern.py`
(ternary → `if/else`), `scan_edits.py` / `combo.py` / `randedit.py`
(type-edit sweeps), `b7024.py` (7024-byte body), `reord.py` / `sub.py` /
`ifrange.py` (order sweeps), `Layout.base.cpp` (pristine ctor backup).
