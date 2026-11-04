# Layout::Layout C1/C2 — wave 107 (w5501): full store-set decode, the
# 6800-B all-ternary body, and the 4-byte size quantization

Worker **w5501**, branch `wave107-w5501`, base `f857b5171`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (same body and score),
ELF VMA `0xd1028`, original size **7072 B**.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result

**No landable improvement found.** The ctor stays at **7072 B /
47.84860557768924 %** (official unrounded linked `fscore.py`,
`autojunk=False`). `src/game/ui/Layout.cpp` is byte-identical to the base.
Gate before == after: `byte_exact` 4500/4524, `linked_exact` 4500/4524,
`size_exact` 4524/4524, `data_exact` 843/843, imports 207, lint CLEAN,
drift 0, `RATCHET PASS`.

This pass contributes five measured facts:

1. The original's **store set is exactly our 201 fields** (decoded all
   `str`/`strd`/`vstr`/`vst1.32`/`stmia` stores, 593 store words in 293
   instructions). No field is missing or extra. §1.
2. The original's block tree is **HD 137 fields / n9+retina 34 / mid 103 /
   tail 66**, i.e. group A (34) is duplicated three ways, group B (103) is
   HD-vs-mid, group C (66) is shared. §1.
3. The **4-byte size quantization is real**: a 4 B size change drops
   `byte_exact` 4500 -> 2454 (all later `.text` VMAs shift). Any landable
   variant must compile to exactly 7072 B. §2.
4. The **wave-89 all-ternary body is 6800 B / 50.7885 %** — the best fuzzy
   shape measured for this function — but it is 272 B (118 instructions)
   short. Its tail region matches 48.2 % vs the landed body's 34.6 %. §3.
5. The best size-exact variant remains the landed body. The best
   near-miss is **7076 B / 47.9299 %** (`0x1a4` cast strip + `0x310`
   if/else), 4 B over; the best 7028 B variant is 48.03 %. §4.

## 1. Full store-set decode (new, proven)

A complete store decoder over `_ZN6LayoutC1Ev` (tracking `add rX,r0,#imm`,
`str`, `strd`, `vstr`, `vst1.32 {d-d}` and `stmia`) yields **201 unique
offsets**, identical to the 201 fields our source assigns. Per block:

| block | VMA range | store words | unique offsets |
|---|---|---:|---:|
| HD (`if iPadHD`) | `d1042..d1268` | 137 | 137 |
| n9 | `d126c..d12fc` | 34 | 34 |
| retina/default | `d1570..d185f` | 47 | 34 |
| mid (non-HD) | `d1860..d21e1` | 221 | 103 |
| tail (all devices) | `d21e2..d2827` | 154 | 66 |

So the source shape is exactly

```
if (iPadHD) { <group A: 34> <group B: 103> }
else {
    if (n9) { <group A: 34> } else { <group A: 34> }
    <group B: 103>
}
<group C: 66>
```

Group A = `0x4..0x1e0` minus group B; group B = the 103 mid fields; group C =
`0x1f0..0x320`-ish tail. `hd_block.txt`/`order_rest.txt` in
`_work/reconstructions/layout_ctor/` already encode these.

The original's 27 `vstr` fields are
`0x60 0x68 0x6c 0x70 0x8c 0x90 0x9c 0xa8 0xdc 0xe0 0xe4 0xe8 0x138 0x150
0x158 0x15c 0x160 0x168 0x170 0x1a8 0x1e0 0x1e4 0x2c8 0x2d0 0x2f8 0x3e0
0x3f8`. The landed body has 28: it **misses** `0x9c 0x138 0x158 0x15c
0x170 0x1e4 0x3f8` and **adds** `0x20 0x10c 0x148 0x194 0x1a4 0x1f4
0x204 0x3e8`. The extra ones are all float-typed ternaries assigned to int
fields (`N * 1.40625f`, or the `(int)(float)` casts on `0x194`/`0x1a4`).

The `0x3f8` (rewardBoxY2) round trip is the only one whose source value is
**non-constant** (`(rewardBoxY + 17) << retinaDisplay`); `int v = <3-arm
ternary>; rewardBoxY2 = (int)(float)v;` reproduces the original's
`vmov/vcvt.f32.s32/vcvt.s32.f32/vstr` and compiles to 7088 B / 46.745 %
(16 B over, score below the landed body). A single-use `(int)(float)`
folds to an int select unless the temp is shared with another store.

## 2. The 4-byte size quantization (new, proven)

`check` on the 7076 B variant (see §4):

```
verify: byte 2454  linked 4500  stubs 0  missing 0  extra 0
FAIL  byte_exact 4500 -> 2454
RATCHET FAIL
```

So a 4 B size change collapses `byte_exact` by 2046 (every function after
`Layout::Layout` in `.text` shifts). This is why every variant in §4 that is
not exactly 7072 B is unlandable, regardless of its fuzzy score.

## 3. The 6800-B all-ternary body (new measurement)

`/dev/shm/w5501/Layout.6800.cpp` (the wave-89 all-ternary body, one ternary
per field, hill-climbed order) compiles to **6800 B / 50.7885 %** with 2449
normalized instructions vs the original's 2567 (118 fewer, 272 B fewer).
Region equality: HD 23.3 %, n9 54.5 %, pool1 88.2 %, retina 55.4 %,
mid 29.3 %, **tail 48.2 %**, pool2 70.6 %. The landed 7072 B body's tail is
only 34.6 %, so the landed body's extra 272 B (the if/else chains and the
5-arm rewardBoxY2) *hurt* the tail match. The all-ternary body is the best
fuzzy shape but is 272 B short.

Reordering the 6800 body to the iOS source order gives 6848 B / 34.71 %
(the iOS order is structurally right but loses on this metric, as recorded
in `layout_ctor/ios_source_order.md`).

## 4. 7072-B searches (all real `link_so` builds, official scorer)

| variant | size | raw % |
|---|---:|---:|
| landed body | **7072** | **47.8486** |
| 6800 all-ternary body | 6800 | 50.7885 |
| `0x1a4` cast strip | 7056 | 47.8772 |
| `0x1a4` strip + `0x310` if/else | **7076** | **47.9299** |
| `0x1a4` strip + `0x310` if/else + `0x194` strip | 7068 | 47.8997 |
| `0x1a4` strip + `0x98` if/else + `0x310` if/else | 7028 | 48.0287 |
| `0x1a4` strip + `0x310` if/else + `0x1f4` int-arm | 7068 | 47.8598 |
| `0x1a4` strip + `0x310` hybrid (2-arm if/else) | 7056 | 47.8772 |
| `0x150` `(double)` strip (size-neutral) | 7072 | 47.7992 |
| `0x194` cast strip | 7064 | 47.8183 |
| `0x204` int-arm | 7072 | 47.6304 |
| 3-arm rewardBoxY2 + `(int)(float)` round trip | 7088 | 46.7450 |
| `0x30c`/`0x310` paired if/else (true strd source) | 7080 | 47.5413 |
| paired `0x30c`/`0x310` + `0x1a4` strip | 7064 | 47.5697 |

Searches performed (all negative for a 7072 B body above 47.8486):

* all 199 single ternary->if/else conversions on the landed body
  (`moves.txt`): the only score improvement is `0x310` at 7100/47.8632;
* all 199 single conversions on the 6800 body (`scan6800.log`);
* all 24 arm permutations of the `0x310` if/else chain;
* all cast-form combinations of `0x150` x `0x194` (42 builds);
* all subsets of the six extra-`vstr` int-arm conversions on the
  `0x1a4`-strip + `0x310` body;
* 800 random 2-5 edit mixes (if/else + cast strip + int-arm) on the landed
  body (`randsearch2.py`), 400 with `0x310` forced (`rand310.py`);
* 203 adjacent swaps of the 7076 B variant (only one 7072 B hit, 46.01 %).

The landed body is a genuine local optimum of the fuzzy metric at exactly
7072 B. The 4 B gap between the 7068 and 7076 codegen states could not be
closed by any source edit tried; it looks like a branch-encoding / pool
alignment quantum rather than a missing statement.

## 5. Proven vs hypothesis

* **Proven**: the 201-field store set; the HD/n9/retina/mid/tail block
  partition and field counts; the 27-field original `vstr` set; the 4 B
  size quantization and its `byte_exact` collapse; the 6800 B body's size
  and score; every size/score in the §4 table (real `link_so` builds).
* **Hypothesis**: the original source is the flat per-field ternary/if-else
  sequence in the iOS order (per `ios_source_order.md`), which clang `-Oz`
  then if-converts and SLP-vectorizes into the observed block layout. The
  `-Oz` block layout is what the fuzzy metric rewards, which is why the
  hill-climbed flat body beats the structurally-true iOS order and the
  reconstructed block tree.

## Commands

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w5501/fscore.py                        # 47.84860557768924
```

Scratch harness (all under `/dev/shm/w5501`): `trybody.sh` (swap ctor body,
`link_so`, score), `fscore.py` (official unrounded linked scorer),
`regions.py` (per-region equality), `ifelse_lib.py` (`convert_one`),
`combo_edit.py` (cast/if edit combos), `scan6800.py`, `randsearch2.py`,
`rand310.py`, `mk7076.py`, `hd_block.txt`, `order_rest.txt`, `ios_order.txt`.
