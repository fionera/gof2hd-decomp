# Layout::Layout — wave 115 (w6302): per-field pool-word audit and the 7072-B wall

Worker **w6302**, branch `wave115-w6302`, base `b847d5d46` (wave-114 harvest).
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (same body and score),
ELF VMA `0xd1028`, original size **7072 B**.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — no landable improvement; note-only commit

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC2Ev` size | 7072 | **7072** (unchanged) |
| `_ZN6LayoutC2Ev` unrounded `difflib` ratio | 48.545237146273 % | **48.545237146273 %** |
| `byte_exact` / `linked_exact` | 4500 / 4500 | **4500 / 4500** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No code change was landed, so the 27 section headers, file size 2,187,328 B and
`.ARM.extab 0x26298` are unchanged.

## Why the size is hard-pinned to exactly 7072

`byte_exact` is *content*-based (`ob == ub`), but a size change moves every later
function and the relocated literal-pool words inside them.  Measured on this
branch with the full `check` target:

| ctor size | `byte_exact` | verdict |
|---:|---:|---|
| 7072 | 4500 | PASS |
| 7076 (+4) | **2454** | RATCHET FAIL |
| 7064 (−8) | **3221** | RATCHET FAIL |

So even a 4-byte delta collapses the gate.  Every variant below was therefore
required to land on **exactly 7072 B**.

## The 46 missing pool words, decoded per field

The ctor ends in `pop {r4,r5,r6,r7,pc}` followed by one ADR-addressed literal
pool (words are 4-aligned; the 2 bytes after the `pop` are padding):

| body | code | pool start | pool bytes | pool words |
|---|---:|---:|---:|---:|
| **original** | 6106 (`0xd17da`) | `0xd2804` | **966** | **241** |
| HEAD | 6290 (`0xd1892`) | `0xd28bc` | 782 | 195 |

Instruction-class counts (whole function, pool words included):

| class | original | HEAD |
|---|---:|---:|
| `vld1.64` | 86 | 77 |
| `vst1.32` | 81 | 70 |
| `strd` | 62 | 49 |
| `stmia` | 8 | 4 |
| `str.w` | **103** | **166** |
| `movw` | 15 | 18 |
| `mov.w` | 99 | 91 |

The HD path is already vectorised to the *same* 26 four-word groups in both
builds, and every HD constant matches the original exactly (decoded from the
pool and cross-checked against the Ghidra `this+off = const` list).  The groups
differ only in **window alignment**:

| original group | values | HEAD group | values |
|---|---|---|---|
| `0xb0` | 4,20,25,11 | `0xb4` | 20,25,11,9 |
| `0xc0` | 9,11,5,253 | `0xc4` | 11,5,253,21 |
| `0xec` | 56,112,281,562 | *(scalar)* | — |
| `0x140` | 15,18,2,189 | `0x148` | 2,189,5,70 |
| `0x150` | 5,70,70,206 | `0x158` | 70,206,68,0 |
| `0x160` | 68,0,35,184 | `0x168` | 35,184,174,84 |
| `0x170` | 174,84,84,217 | *(scalar)* | — |
| `0x180` | 23,163,23,244 | `0x180` | 23,163,23,244 ✓ |
| `0x190` | −6,8,16,90 | `0x190` | −6,8,16,90 ✓ |
| `0x1a0` | 18,22,22,243 | `0x1a0` | 18,22,22,243 ✓ |
| `0x1b0` | 18,42,46,1 | `0x1b0` | 18,42,46,1 ✓ |
| `0x1c0` | 92,26,40,50 | `0x1c0` | 92,26,40,50 ✓ |
| `0x1d0` | 7,17,5,14 | `0x1d0` | 7,17,5,14 ✓ |

So the HD block contributes **zero** net pool-word deficit; the 46-word gap is
in the *non-HD* branches, where the original SLP-folds many more four-field
runs (e.g. `0x5c`, `0x7c`, `0xb0`, `0xc0`, `0xec`, `0x294`, `0x2a4`, `0x108`,
`0x118`, `0x128`, `0x140`, `0x13c`, `0x16c`, `0x180`, `0x1b0/0x1c0/0x1d0`,
`0x204`, `0x214`, `0x224`, `0x23c`, `0x24c`, `0x25c`, `0x26c`, …) than we do.
The original's `vbsl qN, qA, qB` groups at `0x1b0/0x1c0/0x1d0` are the
vectorised non-HD ternaries for that run.

### "distinct vs shared" audit — no wrong constant found

Decoded every pool word that feeds a vector store in both binaries and compared
the value of each destination field.  **All values agree.**  The only
"distinct in original / shared in ours" cases are *encoding* choices, not
values:

* `0x9c`: original materialises `100.0f`/`50.0f` from the ADR pool and does
  `vcvt.s32.f32`; HEAD keeps `int u9c` and does
  `vmov s0,rN; vcvt.f32.s32; vcvt.s32.f32`.  Same value, different pool use.
  The faithful float form is the `u9c` edit below.
* `0xac`, `0x194`, `0x3e8`: original stores a **plain int** (`str.w` of an
  integer register); HEAD wraps the expression in `(int)(float)(int)` /
  `(int)(float)`, forcing a float round trip.  These are real type bugs in our
  source (edits `cac`, `i194`, `i3e8` below).
* `0xdc..0xe8` HD: original uses two `strd` (0xdc/0xe0, 0xe4/0xe8); HEAD folds
  them into one `vst1.32 {14.0625,1.40625,28.125,1.40625}`.  Values identical.
* `0x144`/`0x148` HD: original stores plain ints 18 / 2; HEAD writes
  `13*1.40625f` / `2*1.40625f` (constant-folded to 18 / 2).  Code-identical
  when rewritten as `18` / `2` (measured: `i144` is size- and score-neutral).

## The faithful typing set lands on 7076, not 7072

Four edits are demonstrably faithful (each verified against the original's
store instruction):

| edit | change | size | % |
|---|---|---:|---:|
| `u9c` | `int u9c` if/else → `field_0x9c = HD ? 70.0f : (n9 ? 75.0f : (ret ? 100.0f : 50.0f))` | 7096 | 48.2992 |
| `i194` | drop `(int)(float)` on `field_0x194` | 7064 | 48.5549 |
| `i3e8` | drop the float cast on `scrollBarInset` | 7064 | 48.5646 |
| `cac` | drop `(int)(float)(int)` on `field_0xac` | 7072 | 48.3955 |
| **all four** | | **7076** | **48.6853** |
| all four + `nheight` (ternary `rewardBoxHeight`) | | 7068 | **48.7047** |
| all four + `i150` (drop cast on `field_0x150`) | | 7060 | 48.6939 |
| all four + `novol` (drop `volatile` on the `retinaDisplay` pointer) | | 7068 | 48.6346 |

So the faithful body is **+0.14 pp better** than HEAD but 4 bytes too long.
No 4-byte reduction was found (see below), and 7076 fails the gate (byte 2454).

## Searches that found nothing better at 7072 B

All measured with real `link_so` builds and the unrounded `verify.py` ratio:

* **1024 subsets** of the 9 wave-89 revert edits (`u9c`, `a8`, `d2d4`, `e184`,
  `f190`, `oy2`, `l260`, `nheight`, `p130`) plus `i194`: 54 land on 7072, best
  **48.5452** (ties: `()`, `(p130,)`).
* **210 pairs** over the 21 structural/typing edits (`t1cc`, `t20c`, `tox`,
  `t218`, `toy`, `t280`, `i144`, `i148`, `i150`, `i10c`, `i3e8`, …): 30 land on
  7072, best **48.5452**.
* **520 random subsets** (k ≤ 6) over 36 edits: 98 land on 7072, best
  **48.5452**.
* **700 random subsets** (k = 2..7) over 66 edits: 73 land on 7072, best
  **48.5452**.
* **250 adjacent statement swaps** of the 7076 faithful body: 14 land on 7072,
  best **48.4957**.
* **500 random single-statement moves** of the 7068 faithful body: 5 land on
  7072, best **48.2566**.
* **28 pairs of `mod 8 ≡ 4` edits** on the 7076 body: 2 land on 7072, best
  **40.66**.
* Moving `field_0x140`/`field_0x144` in front of `field_0x14c` (which makes the
  HD `0x140..0x17c` run SLP-fold into the original's four aligned groups):
  6944 B / 41.96 %.  Padding it back to 7072 with `l260+oy2+u9c`: 7072 B /
  39.54 %.  The vector-group fix is real but destroys the difflib alignment.
* Full ctor reorder to the original HD store order (wave 114, w6201):
  6896 B / 28.99 %.

## Proven vs hypothesis

**Proven (measured):**

* The original's code/pool split is 6106/966; ours is 6290/782.  The deficit is
  46 literal-pool words, i.e. 63 excess scalar `str.w` that SLP could not fold.
* Every HD-path constant and every HD vector-group value matches the original.
  The HD block has the same 26 groups in both builds; only the window alignment
  of `0xb0/0xc0/0xec/0x140..0x170` differs.
* `field_0x9c`, `field_0xac`, `field_0x194`, `field_0x3e8` are the four real
  typing mismatches; fixing all four gives 7076 B / 48.6853 % (better score,
  wrong size).
* The 7072-B body is a very strong local optimum: ~2500 built variants across
  every search above, none beat 48.5452 % at 7072 B.

**Hypothesis (not proven):**

* Landing a better 7072-B body needs a source shape whose *non-HD* branch
  stores are field-adjacent enough for SLP to fold the same four-word runs as
  the original, while the total code+pool sum stays 7072.  The faithful typing
  set is 4 B over; a single additional −4 B faithful change (not found here)
  would land 48.6853 % at 7072 B.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6302
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch harness (not committed): `/dev/shm/w6302/{score,decode,mapstore2,
poolcount,edits,edits2..edits6,trybuild,sweep4..sweep15,final_search,
swapsearch,movesearch}.py`, `Layout.cpp.base`, `orig.asm`, `ours.asm`.
