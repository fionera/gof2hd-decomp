# Layout::Layout C1/C2 — complete store-order table, HD-block field-set diff,
# and a landable 7072 B improvement (w4502, wave 95)

Worker **w4502**, branch `wave95-w4502`, base `4a88bd6e`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B** (`.text` `0x69440..0x1cb808`; the ctor's literal pools are inside
`.text`). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Ghidra image base `0x10000`.

Continues `layout-layout-c1-c2-hd-block-order-w4221.md`,
`layout-layout-c1-c2-vstr-shapes-w4401.md`,
`layout-layout-c1-c2-vstr-opacity-w4274.md` and
`layout-layout-c1-c2-hd-vector-w4101.md`.

## Result

Landed a **size-neutral** three-part source change on top of restoring the
pre-pass-2 statement order. Raw verifier ratio (unrounded, `autojunk=False`):

| body | `usize` | raw % | report pct |
|---|---:|---:|---:|
| HEAD `4a88bd6e` (pass-2 order `0x120` before `0x130`) | 7072 | **47.3161033797** | 47.3 |
| pass-9 order (`0x130` before `0x120`) | 7072 | 47.6644802226 | 47.7 |
| **landed: pass-9 order + `0x168` float branches + `0x124`→`0x10c` swap + `0x188`→`0x184` swap** | **7072** | **47.7326968974** | **47.7** |

Gate (before == after; the function size is unchanged, so no downstream
function moves):

| metric | before | after |
|---|---:|---:|
| `byte_exact` | 4500 / 4524 | **4500 / 4524** |
| `linked_exact` | 4500 / 4524 | **4500 / 4524** |
| `size_exact` | 4524 / 4524 | **4524 / 4524** |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 | **0** |
| `data_exact` | 843 / 843 | **843 / 843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

`RATCHET PASS`.

The landed diff is four lines in `src/game/ui/Layout.cpp`:

```diff
-    this->field_0x10c_rowBaseY = ...;
     this->field_0x124 = ...;
+    this->field_0x10c_rowBaseY = ...;
...
-    this->field_0x120 = ...;
     this->field_0x130 = ...;
+    this->field_0x120 = ...;
...
-    this->field_0x168 = Globals::iPadHD ? 25 * 1.40625f : (Globals::n9 ? 37 : (Globals::retinaDisplay ? 50 : 25));
+    this->field_0x168 = Globals::iPadHD ? 25 * 1.40625f : (Globals::n9 ? 37.0f : (Globals::retinaDisplay ? 50.0f : 25.0f));
...
+    this->field_0x188 = ...;
     if (Globals::iPadHD)
         this->field_0x184 = 163;
...
-    this->field_0x188 = ...;
```

(The `0x120`/`0x130` pair is the pass-2 commit `a0f2dcc2`; see §1.)

## 1. The pass-2 `0x120`/`0x130` swap is a regression in the current tree

`a0f2dcc2` ("move `0x120` before `0x130`") claims 47.655 -> 47.664. Measured on
the current tree, the two sources are identical except for that one pair of
lines, and repeated clean rebuilds are stable:

| source order | `usize` | raw % |
|---|---:|---:|
| `0x130` then `0x120` (pass 9, `30752cad`) | 7072 | **47.6644802226** |
| `0x120` then `0x130` (HEAD `a0f2dcc2`) | 7072 | **47.3161033797** |

So restoring the pass-9 order is itself a strict +0.348 pp improvement, and it
is the first component of the landed change. (The pass-2 measurement was not
reproducible; the likely cause is a stale `libgof2hdaa.so` in that pass's
scorer.)

## 2. Complete store-order table (offset, width, source register, order)

Method: `stores.py` walks `objdump -d` output, tracks `r0`-relative base
registers through `add/adds/add.w/addw` and `mov`, and expands `vst1.32`
(`8 * d-regs`), `strd` (8), `stmia` (`4 * regs`), `vstr` (4), `strh` (2),
`strb` (1), `str` (4). It is exact for both binaries: the original HD block
yields the same 137-field list as the w4101 note.

### 2.1 Original HD block (`d1042..d1268`, 42 store groups, 137 fields)

```
 #  addr   off    w  instruction
 1  d10a4  0x014 16  vst1.32 {d2-d3}, [r1]        ; 0x14,18,1c,20
 2  d10b0  0x004 16  vst1.32 {d2-d3}, [r1]        ; 0x04,08,0c,10
 3  d10be  0x024 16  vst1.32 {d2-d3}, [r1]        ; 0x24..30
 4  d10cc  0x034 16  vst1.32 {d2-d3}, [r1]        ; 0x34..40
 5  d10da  0x044 16  vst1.32 {d2-d3}, [r1]        ; 0x44..50
 6  d10e8  0x054 16  vst1.32 {d2-d3}, [r1]        ; 0x54..60
 7  d10f6  0x064 16  vst1.32 {d2-d3}, [r1]        ; 0x64..70
 8  d1104  0x2c8 16  vst1.32 {d2-d3}, [r1]        ; 0x2c8..2d4
 9  d1112  0x07c 16  vst1.32 {d2-d3}, [r1]        ; 0x7c..88
10  d1118  0x074  8  strd r6, r5, [r0, #116]      ; 0x74,78
11  d1122  0x2c0  8  strd r3, r6, [r0, #704]      ; 0x2c0,2c4
12  d1128  0x2d8  4  str.w r3, [r0, #728]         ; 0x2d8
13  d1132  0x0ac  4  str.w r3, [r0, #172]         ; 0xac
14  d1136  0x08c  8  strd r6, r4, [r0, #140]      ; 0x8c,90
15  d113e  0x094 16  stmia.w r4, {r5,r8,ip,lr}    ; 0x94,98,9c,a0
16  d1148  0x0a4  8  strd r9, r3, [r0, #164]      ; 0xa4,a8
17  d1158  0x0c0 16  vst1.32 {d0-d1}, [r2]        ; 0xc0..cc
18  d1166  0x0b0 16  vst1.32 {d0-d1}, [r2]        ; 0xb0..bc
19  d1174  0x0ec 16  vst1.32 {d0-d1}, [r2]        ; 0xec..f8
20  d1182  0x294 16  vst1.32 {d0-d1}, [r2]        ; 0x294..2a0
21  d1190  0x2a4 16  vst1.32 {d0-d1}, [r2]        ; 0x2a4..2b0
22  d119e  0x108 16  vst1.32 {d0-d1}, [r2]        ; 0x108..114
23  d11ac  0x118 16  vst1.32 {d0-d1}, [r2]        ; 0x118..124
24  d11c0  0x0d0 12  stmia.w fp, {r3,r6,r8}       ; 0xd0,d4,d8
25  d11c8  0x0dc  8  strd r2, r4, [r0, #220]      ; 0xdc,e0
26  d11ce  0x0e4  8  strd r1, r4, [r0, #228]      ; 0xe4,e8
27  d11d4  0x0fc  8  strd r9, r1, [r0, #252]      ; 0xfc,100
28  d11da  0x104  4  str.w r2, [r0, #260]         ; 0x104
29  d11de  0x2b4  8  strd lr, r1, [r0, #692]      ; 0x2b4,2b8
30  d11e8  0x128 16  vst1.32 {d30-d31}, [r1]      ; 0x128..134
31  d11f6  0x140 16  vst1.32 {d16-d17}, [r1]      ; 0x140..14c
32  d11fe  0x150 16  vst1.32 {d18-d19}, [r1]      ; 0x150..15c
33  d1206  0x160 16  vst1.32 {d20-d21}, [r1]      ; 0x160..16c
34  d120e  0x170 16  vst1.32 {d22-d23}, [r1]      ; 0x170..17c
35  d1216  0x180 16  vst1.32 {d24-d25}, [r1]      ; 0x180..18c
36  d121e  0x190 16  vst1.32 {d26-d27}, [r1]      ; 0x190..19c
37  d1226  0x1a0 16  vst1.32 {d28-d29}, [r1]      ; 0x1a0..1ac
38  d122e  0x1b0 16  vst1.32 {d2-d3}, [r1]        ; 0x1b0..1bc
39  d1236  0x1c0 16  vst1.32 {d4-d5}, [r1]        ; 0x1c0..1cc
40  d123e  0x1d0 16  vst1.32 {d30-d31}, [r1]      ; 0x1d0..1dc
41  d1244  0x138  8  strd r1, r3, [r0, #312]      ; 0x138,13c
42  d1250  0x1e0  4  str.w r4, [r0, #480]         ; 0x1e0
```

### 2.2 Our HD block (`d1042..d1274`, 50 store groups, 137 fields)

```
 #  addr   off    w  instruction
 1  d109e  0x014 16  vst1.32 {d0-d1}, [r1]
 2  d10aa  0x004 16  vst1.32 {d0-d1}, [r1]
 3  d10b8  0x024 16  vst1.32 {d0-d1}, [r1]
 4  d10c6  0x034 16  vst1.32 {d0-d1}, [r1]
 5  d10d4  0x044 16  vst1.32 {d0-d1}, [r1]
 6  d10e2  0x054 16  vst1.32 {d0-d1}, [r1]
 7  d10f0  0x064 16  vst1.32 {d0-d1}, [r1]
 8  d10fe  0x2c8 16  vst1.32 {d0-d1}, [r1]
 9  d110c  0x07c 16  vst1.32 {d0-d1}, [r1]
10  d1112  0x074  8  strd r6, r5, [r0, #116]
11  d1120  0x2c4  4  str.w r6, [r0, #708]
12  d1126  0x1e0  4  str.w r3, [r0, #480]
13  d112c  0x2d8  4  str.w r3, [r0, #728]
14  d1132  0x08c  8  strd r3, r1, [r0, #140]
15  d1138  0x09c  4  str.w r1, [r0, #156]
16  d1142  0x0ac  4  str.w r1, [r0, #172]
17  d1146  0x098  4  str.w r3, [r0, #152]
18  d114e  0x0b4 16  vst1.32 {d2-d3}, [r3]        ; 0xb4,b8,bc,c0
19  d115c  0x0c4 16  vst1.32 {d2-d3}, [r3]        ; 0xc4,c8,cc,d0
20  d116a  0x294 16  vst1.32 {d2-d3}, [r3]
21  d1178  0x2a4 16  vst1.32 {d2-d3}, [r3]
22  d1186  0x0dc 16  vst1.32 {d2-d3}, [r3]        ; 0xdc,e0,e4,e8
23  d1194  0x0b0  4  str.w r3, [r0, #176]
24  d119a  0x0a8  4  str.w r1, [r0, #168]
25  d11a0  0x0a0  4  str.w r6, [r0, #160]
26  d11a6  0x0d4  8  strd r1, r3, [r0, #212]
27  d11b0  0x0a4  4  str.w r5, [r0, #164]
28  d11b4  0x0ec  8  strd r2, r3, [r0, #236]
29  d11ba  0x0f4  8  strd r1, r8, [r0, #244]
30  d11c2  0x0fc 12  stmia.w r1, {r5,ip,lr}      ; 0xfc,100,104
31  d11ce  0x2b4  4  str.w r6, [r0, #692]
32  d11d2  0x118 16  vst1.32 {d30-d31}, [r1]
33  d11e0  0x108 16  vst1.32 {d30-d31}, [r1]
34  d11ee  0x128 16  vst1.32 {d16-d17}, [r1]
35  d11f6  0x148 16  vst1.32 {d18-d19}, [r1]      ; 0x148..154
36  d11fe  0x158 16  vst1.32 {d20-d21}, [r1]      ; 0x158..164
37  d1206  0x168 16  vst1.32 {d22-d23}, [r1]      ; 0x168..174
38  d120e  0x180 16  vst1.32 {d24-d25}, [r1]
39  d1216  0x190 16  vst1.32 {d26-d27}, [r1]
40  d121e  0x1a0 16  vst1.32 {d28-d29}, [r1]
41  d1226  0x1b0 16  vst1.32 {d0-d1}, [r1]
42  d122c  0x23c  4  str.w r1, [r0, #572]
43  d1232  0x2b8  4  str.w r1, [r0, #696]
44  d1238  0x138  8  strd r1, r3, [r0, #312]
45  d123e  0x204  4  str.w r1, [r0, #516]
46  d1246  0x2bc  4  str.w ip, [r0, #700]
47  d124a  0x178  8  strd r1, r3, [r0, #376]
48  d1252  0x2c0  4  str.w r1, [r0, #704]
49  d1260  0x1c0 16  vst1.32 {d2-d3}, [r5]
50  d1264  0x1d0 16  vst1.32 {d30-d31}, [r1]
```

### 2.3 The three concrete HD-block differences

**(a) Field-set difference.** Both blocks store 137 fields, but the sets differ:

| | offsets |
|---|---|
| original-only | `0x94`, `0x140`, `0x144` |
| ours-only | `0x204`, `0x23c`, `0x2bc` |

The values are identical (emulator table, §4); only the **block placement**
differs. The original's source has `0x94`/`0x140`/`0x144` early enough that
clang sinks them into the `iPadHD` block; ours has them at source lines 178,
182, 186 (the reward-box area), so clang leaves them in the common path.
Conversely `0x204`, `field_0x238_blk16.b` (`0x23c`) and `0x2bc` sit in our
HD-specific source region and get sunk into the HD block, while the original
stores them in the tail. Moving them individually was measured (§5) and always
loses more than it gains; the current order is a local optimum.

**(b) Vector-group phase shift.** The original groups the `0x140..0x17c` run as
four 4-word vectors:

```
0x140,144,148,14c | 0x150,154,158,15c | 0x160,164,168,16c | 0x170,174,178,17c
```

Ours groups it shifted by two words, because `0x140`/`0x144` are not adjacent
to `0x148`/`0x14c` in our source:

```
0x148,14c,150,154 | 0x158,15c,160,164 | 0x168,16c,170,174 | strd 0x178,17c
```

This is the single largest store-group difference in the HD block (four groups,
16 fields). It is a direct consequence of (a): the original's source has the
whole `0x140..0x17c` run contiguous; ours interleaves `0x204`/`0x2bc` and puts
`0x140`/`0x144` in the reward-box section.

**(c) Register allocation / load order.** The first ten store groups are
identical, but the original's first *load* is the `0xc0..0xcc` group
(`vld1.64 {d0-d1}` at `d1046`, stored at `d1158`), while ours loads the
`0x14..0x20` group first (`d1042`) and uses `d0-d1` for every group. The
original uses `d2-d3` for the early groups and `d0-d1` only for the `0xb0..0x124`
run. The original prologue saves six registers (`r6,r7,r8,r9,sl,fp`); ours saves
seven (`+r5`), because our scheduler keeps `r5 = r0 + 0x1c0` live from `d11ca`
to `d1260`, while the original recomputes `add.w r1, r0, #448` at `d1232` and
stores four bytes later. This is the residual RA/scheduling difference the
earlier notes identified; it is not reachable by expression edits.

## 3. The 27 `vstr` stores (final build)

Original 27: `96 104 108 112 140 144 156 168 220 224 228 232 312 336 344 348
352 360 368 424 480 484 712 720 760 992 1016`.

Final build 25: `32 96 104 108 112 140 144 168 220 224 228 232 268 328 352
360 424 480 500 516 712 720 760 992 1000`.

* **missing (7)**: `0x9c 0x150 0x158 0x15c 0x170 0x1e4 0x3f8`
* **extra (6)**: `0x20 0x10c 0x148 0x1f4 0x204 0x3e8`

The landed `0x168` change fixes a **shape** mismatch that the w4401 note
reported: the original stores `0x168` with one `vcvt.s32.f32` + `vstr`
(`d1e56/d1e5e`), i.e. the source ternary is float-typed. Ours used to emit
`vmov s0,rN; vcvt.f32.s32; vcvt.s32.f32; vstr` (an int round-trip). Writing the
non-HD branches as `37.0f/50.0f/25.0f` makes the whole conditional float-typed
and reproduces the original's single-`fptosi` shape (verified in the final
disassembly at `d1f28/d1f30`).

The `0x124`→`0x10c` source swap moves the (still incorrect) extra `vstr` from
`0x124` to `0x10c`; both are plain `int` in the original (`0x118` vector group).
The swap is a scheduling win, not a shape fix.

Measured single-field forms on the pass-9 body (official verifier, unrounded):

| variant | `usize` | raw % |
|---|---:|---:|
| `0x168` float branches | 7088 | 47.7553 |
| `0x168` if/else | 7064 | 47.5944 |
| `0x150` float branches | 7080 | 47.2178 |
| `0x1e4` `4*1.40625f` mixed | 7024 | 46.1875 |
| `0x1e4` all-float | 7072 | 41.0175 |
| `0x3f8` `(int)(float)((rewardBoxY+17)<<retina)` | 7088 | 47.5774 |
| `0x9c` float ternary / `(int)(float)` / float local | 7072 | 47.5358 |
| `0x20` HD branch → `7` | 6968 | 20.8878 |
| `0x124` HD branch → `1` | 7008 | 42.5617 |
| `0x148` HD branch → `2` | 7064 | 47.4354 |
| `0x1f4` HD branch → `22` | 7064 | 47.5944 |
| `0x204` HD branch → `12` | 7072 | 47.3674 |

Only `0x168` float branches is a strict improvement on its own, and it is
`+16 B`. Pairing it with the `0x188`→`0x184` swap (`-16 B`) gives exactly
7072 B and 47.7024; the subsequent `0x124`→`0x10c` swap raises it to 47.7327.
Both swaps were found by the exhaustive adjacent-swap hill-climb described in
§5.

## 4. Emulator values (why the field sets are equivalent)

`emu2.py` over the original for the four flag configurations (int values):

| offset | HD | n9 | retina | default |
|---|---:|---:|---:|---:|
| `0x94`  | 14 | 15 | 20 | 10 |
| `0x140` | 15 | 19 | 22 | 11 |
| `0x144` | 18 | 19 | 26 | 13 |
| `0x204` | 12 | 16 | 18 | 9 |
| `0x23c` | 21 | 27 | 30 | 15 |
| `0x2bc` | 2  | 3  | 2  | 2 |
| `0x1e4` | 5  | 6  | 8  | 4 |
| `0x3f8` | 132 | 111 | 186 | 55 |
| `0x9c`  | 70 | 75 | 100 | 50 |
| `0x150` | 5  | 6  | 8  | 4 |
| `0x158` | 70 | 13 | 28 | 14 |
| `0x15c` | 206 | 136 | 158 | 79 |
| `0x170` | 174 | 106 | 128 | 64 |

All our source expressions produce these values; the differences are pure
codegen shape/placement.

## 5. Search method and measurements

* **Store table**: `stores.py` (parser described in §2) over
  `objdump -d --start-address=0xd1028 --stop-address=0xd2bc8`.
* **Scorer**: `fscore.py` builds `link_so` then reuses
  `tools/verify/verify.py`'s `disassemble`/`reconcile` with
  `SequenceMatcher(autojunk=False)` and prints the unrounded ratio.
* **Unit splitter**: `units.py` parses the 204 top-level statements/if-else
  chains of `Layout::Layout()` (same 204-unit decomposition as the w4274 note).
* **Adjacent-swap hill-climb**: 203 evals per pass. On the pass-9 body no
  size-7072 swap beats 47.6645 (all equal or worse); the best size-changing
  swap is `0xac` before `0x98` (`6984 B`, 47.9553).
* On the `0x168`-float body, the sweep found exactly one size-7072 improvement:
  swap `0x188`/`0x184` (47.7024). A second sweep on that body found swap
  `0x124`/`0x10c` (47.7327). A third sweep found nothing; 150 random
  1/2/3-unit moves from the 47.7327 body found nothing.
* Individual HD field-set moves (all size-changing, none landable):
  `0x94`→after `0x90` 6992/45.469; `0x140`→before `0x148` 6976/45.222;
  `0x144`→before `0x148` 6984/45.026; `0x148`→before `0x14c` 7016/46.897;
  `0x204`→after `0x17c` 7048/45.887; `0x2bc`→after `0x17c` 7032/46.531;
  `0x23c`→after `0x17c` 7024/41.155.
* Region breakdown of the landed body (original address buckets):
  `hd 42/163 = 25.8%`, `n9 21/38 = 55.3%`, `def 155/259 = 59.8%`,
  `mid 214/734 = 29.2%`, `tail 478/953 = 50.2%`, other 290/382 = 75.9%.

## 6. What remains

1. **HD-block field set / vector phase**: the original's `0x140,144,148,14c`
   grouping requires `0x140`/`0x144` to be adjacent to `0x148`/`0x14c` in the
   source *and* `0x94` to be in the HD-specific region, while `0x204`/`0x23c`/
   `0x2bc` must move out. Every individual move loses (the current order is a
   local optimum), so this needs the full original statement interleaving, not
   local edits.
2. **The seven missing `vstr`s**: `0x9c`, `0x150`, `0x158`, `0x15c`, `0x170`,
   `0x1e4`, `0x3f8`. `0x3f8` is reproducible (`(int)(float)((rewardBoxY+17)<<
   retina)`, +16 B / 47.577) and `0x150` is reproducible (float branches,
   +8 B / 47.218), but both are size-changing; `0x9c`/`0x158`/`0x15c`/`0x170`
   still fold to an int select in every source form tried.
3. **RA**: the original's six-register prologue and low-register `cbz` flag
   tests remain the dominant HD-block mismatch; matching them requires the
   exact Android 2.0.16 statement order *and* expression types.

## 7. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8     # RATCHET PASS
python3 /dev/shm/w4502/fscore.py                       # unrounded target score
python3 /dev/shm/w4502/stores.py /dev/shm/w4502/orig.dis
python3 /dev/shm/w4502/regions.py
```

Scratch (`/dev/shm/w4502`): `stores.py`, `fieldmap.py`, `seqdiff.py`,
`hddiff.py`, `rangediff.py`, `units.py`, `reorder.py`, `combo.py`,
`fscore.py`, `vstrset.py`, `regions.py`, `emu2.py` (copy of w4221's, path
fixed), `Layout_base.cpp` (pass-9 body), `Layout_BEST_ind.cpp` (landed body).
