# Layout::Layout C1/C2 — wave 119 / worker w6912
## SLP-group enumeration + the config-major arm structure of the original

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6912`, base wave-119 master `a22737824` (after w6831).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — note only, no source change landed

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.923444976 % | **48.923444976 %** |
| `report.json` pct (1 dp) | 48.9 | 48.9 |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No source change landed, so the 27 section headers, the file size
(2,187,328 B), `.text`, `.ARM.exidx`, `.ARM.extab` and `.rodata` are
unchanged.  `tools/verify_baseline.json` was **not** touched.

The best raw score found is still **7024 B / 49.040000 %** (`v178`, the
`field_0x178` retina/n9 arm swap), 48 B short of the mandatory 7072 B and
therefore unlandable.  Every 7072-B variant measured this pass scores
**≤ 48.923444976 %**.

## 1. The 8 missing end-pool SLP groups, enumerated from the disassembly

Method (`/dev/shm/w6912/enum8.py`): parse the whole function disassembly,
track `add rX,pc,#imm` / `addw rX,pc,#imm` (pool address = `((a+4)&~3)+imm`)
and `add rX,r0,#imm`, then for every `vld1.64 {dA-dB}, [rX]` with a
pc-relative pool address classify the *next* consumer of `{dA-dB}` as
`vst1.32` (with its `[r0,#off]` destination) or `vbsl` (vector-select
input).  Registers are snapshotted per instruction, so the destination is
exact.

| | original | base (HEAD) |
|---|---:|---:|
| total `vld1.64` groups | **84** | **75** |
| end-pool groups (`pool ≥ 0xd2800`) | **50** | **42** |
| island groups (`.text` literal pools) | 34 | 33 |

The end-pool difference is exactly **8 groups = 32 pool words**, matching
the w6720 census.  The full `(destination, 4 words)` multiset diff is:

**Original-only end-pool groups (33):**

```
dst=0xec  [60,120,300,600]        dst=0x140 [19,19,19,243]
dst=0xb0  [24,220,200,210]        dst=0x2a0 [80,160,400,800]
dst=0x16c [262,248,84,120]        dst=0x180 [30,346,30,446]
dst=0x204 [12,5,7,5]              dst=0x214 [1,7,8,7]
dst=0x224 [28,49,2,1]             dst=0x23c [21,7,4,239]
dst=0x24c [-2,1,5,2]              dst=0x25c [22,1,421,11]
dst=0x26c [92,98,136,421]         dst=0x204 [16,7,7,6]
dst=0x214 [-2,7,10,7]             dst=0x224 [30,52,4,2]
dst=0x23c [27,9,5,306]            dst=0x24c [-1,2,7,4]
dst=0x25c [28,0,450,12]           dst=0x26c [99,105,145,450]
dst=?     [6,29,36,16]            dst=?     [3,15,18,8]
dst=?     [-2,2,1,-1]             dst=?     [0,1,2,1]
dst=?     [40,80,200,400]         dst=?     [12,110,100,105]
dst=?     [34,22,26,4]            dst=?     [17,11,13,2]
dst=?     [10,24,8,20]            dst=?     [5,12,4,10]
dst=?     [30,10,6,340]           dst=?     [15,5,3,170]
```

**Base-only end-pool groups (25):**

```
dst=0xdc  [0x41700000,0x3fc00000,0x41400000,0x3fc00000]   (float group)
dst=0x16c [131,124,42,60]         dst=0x16c [115,106,45,27]
dst=0x208 [5,7,5,1]               dst=0x218 [7,8,7,28]
dst=0x228 [49,2,1,1]              dst=0x240 [7,4,239,-2]
dst=0x250 [1,5,2,22]              dst=0x260 [1,421,11,92]
dst=0x270 [98,136,421,28]         dst=0x208 [7,7,6,-2]
dst=0x218 [7,10,7,30]             dst=0x228 [52,4,2,2]
dst=0x240 [9,5,306,-1]            dst=0x250 [2,7,4,28]
dst=0x260 [0,450,12,99]           dst=0x270 [105,145,450,30]
dst=?     [400,168,24,24]         dst=?     [200,84,12,12]
dst=?     [262,248,84,120]        dst=?     [138,128,60,44]
dst=?     [69,64,30,22]           dst=?     [10,6,340,-2]
dst=?     [5,3,170,0]
```

The dominant, *reproducible* pattern is the **16-byte window shift**:

| original | base |
|---|---|
| `0x204,0x214,0x224` | `0x208,0x218,0x228` |
| `0x23c,0x24c,0x25c,0x26c` | `0x240,0x250,0x260,0x270` |

i.e. the original's 4-int `vst1.32` groups start at offsets ≡ 4 (mod 16)
while ours start at offsets ≡ 8 (mod 16).  The *values* are the same; only
the window origin differs.  The remaining net +8 is made of the `0xec`
(2), `0x140` (1), `0xb0` (1), `0x2a0` (1), `0x180` (1) groups against the
base-only `0xdc` float group (1) and two extra `0x16c` groups (2).

## 2. Why the window is shifted: the original is **config-major**

This is the new structural finding of this pass.  The original's `.text`
is not a flat list of per-field ternaries.  It is a three-arm
`if (iPadHD) / else if (n9) / else` chain, and each of the first two arms
is a **straight-line, ascending-offset block of stores** that clang can
SLP-vectorise:

```
0xd103e  cmp.w sl,#0 ; beq.w 0xd126c        ; if (Globals::iPadHD == 0) -> else
0xd1042  ... HD arm: 0x4 .. 0x1d0 (straight line, 16 SLP groups)
0xd1268  b.w 0xd21e2                        ; -> HD arm, part 2
0xd126c  ... ldrb fp,[retina?] ; cmp #0
0xd127a  beq.w 0xd1570                      ; if (n9 == 0) -> else
0xd127e  ... n9 arm: 0x4 .. 0x74 + 0x2c0..0x2cc (7 SLP groups)
0xd12fc  b.n 0xd1860                        ; -> n9 arm, part 2
0xd1570  ... else arm: per-field `retina ? A : B` selects (`it eq; moveq`)
0xd21e2  ... HD arm part 2: 0x1e4,0x1e8,0x1ec,0x1fc, then
0xd2204    7 vst1 groups at 0x204,0x214,0x224,0x23c,0x24c,0x25c,0x26c
0xd240e  ... n9 arm part 3: the same 7 groups with n9 constants
```

Concrete evidence for the HD arm (all values are the `iPadHD` arm of the
current source):

```
0xd220a vld1.64 {d16-d17},[pool 0xd2a90] -> 0xd227e vst1.32 [r0,#0x204]
        words [12,5,7,5] = 0x204,0x208,0x20c,0x210 HD values
0xd2216 -> [r0,#0x214]  [1,7,8,7]   = 0x214,0x218,0x21c,0x220
0xd221e -> [r0,#0x224]  [28,49,2,1] = 0x224,0x228,0x22c,0x230
0xd2226 -> [r0,#0x23c]  [21,7,4,239]
0xd222e -> [r0,#0x24c]  [-2,1,5,2]
0xd2236 -> [r0,#0x25c]  [22,1,421,11]
0xd223e -> [r0,#0x26c]  [92,98,136,421]
```

The n9 arm repeats the same seven windows with n9 constants
(`[16,7,7,6]`, `[-2,7,10,7]`, `[30,52,4,2]`, `[27,9,5,306]`,
`[-1,2,7,4]`, `[28,0,450,12]`, `[99,105,145,450]`).

`0x1fc` and `0x200` are **not** part of those groups in the original:
`0x1fc` is a scalar `str.w` (`0xd2246`) and `0x200` is stored much later
in the common tail (`0xd2608`).  In the base they are adjacent to
`0x204`, so clang starts the window at `0x1fc`/`0x208` instead of `0x204`.

The `else` arm is *not* straight-line: it keeps the per-field
`Globals::retinaDisplay ? A : B` selects (`it eq; moveq`, `vbsl`), which is
exactly why the original has only two fully vectorised arms.

REA/Ghidra confirms the same shape at the top of the decompiled body:
`if (Globals::iPadHD == 0) { if (Globals::n9 == 0) { ...retina/default... }
else { ...n9... } } else { ...HD... }`.

## 3. Experiments (all real single-TU link+strip builds, exact scorer)

Scorer `/dev/shm/w6912/fast.sh <src.cpp>`: compile `Layout.cpp` with the
exact NDK r18b clang 7.0.2 flags, gold-link the one object, strip with the
same `arm-linux-androideabi-strip --strip-all`, then run
`tools/verify/verify.py` `Resolver` + `reconcile` + `difflib(autojunk=False)`
on `_ZN6LayoutC1Ev`.  It reproduces the full-link number bit-for-bit
(7072 / 48.923444976 on the base).  ~0.4 s/variant.

| variant | source change | size | ratio % |
|---|---|---:|---:|
| base | — | **7072** | **48.923445** |
| `v178` | `field_0x178`: swap retina/n9 arms | 7024 | **49.040000** (best raw) |
| `v178_16c` | `v178` + `field_0x16c` retina/n9 swap | 7040 | 48.671329 |
| `v178_234c` | `v178` + move `field_0x234` before `field_0x120` | 6924 | 43.350151 |
| `vcfg` | 0x204..0x280 block rewritten config-major (HD/n9/else) | 7016 | 41.129355 |
| `vcfg3` | same, 0x1ec/0x1fc/0x200 left outside the block | 7016 | 41.129355 |
| `vcfg2` | *whole ctor* rewritten config-major (173 statements) | 6400 | 12.415541 |
| `vm1` | move `0x1fc`,`0x200` after the 0x280 chain | 7032 | 45.590882 |
| `vm2` | `vm1` + move `0x204` before `0x208` | 6984 | 45.310303 |
| `vsh9` | sort first 9 statements (0x4..0x28) by offset | 7040 | 41.380686 |
| `vsh12` | sort first 12 statements by offset | 7060 | 24.720447 |
| `vsh15` | sort first 15 statements by offset | 7068 | 24.775225 |

`vm2` is the important measurement: it **reproduces the original's window
exactly** — groups at `0x204 [12,5,7,5]`, `0x214 [1,7,8,7]`,
`0x224 [28,49,2,1]`, `0x24c [-2,1,5,2]`, `0x25c [22,1,421,11]`,
`0x26c [92,98,136,421]` and the n9 counterparts — yet it is 88 B short
(6984 B) and scores **45.31**, i.e. *worse* than the base.  Matching the
SLP windows is therefore **not** sufficient (and not even necessary) for a
better `difflib` score: the score is dominated by the hot-body instruction
alignment, and every reorder that fixes the windows also shifts the hot
body.

### Searches run this pass (all real builds)

| search | variants | best 7072-B result |
|---|---:|---:|
| exhaustive single-statement move on `v178` (all frm→to) | 42,642 | **48.138563** (stmt 185 → 201) |
| cast-wrap combinations on `v178` (12 edits, 1–3 at a time) | 298 | no 7072 hit; best sizes 7024–7056 |
| single-statement move on `vm2` (statements 105/142–175/147/150/152) | 7,828 | **46.261216** (stmt 172 → 104) |

The `v178` exhaustive move sweep confirms the handoff's finding: no
single-statement relocation of the 7024-B shape reaches 7072 B with a
score above the base.  The cast-wrap combinations are all codegen no-ops
on `v178` (sizes stay 7024/7032/7040/7048/7056), so the 48 B cannot be
recovered with `(int)(float)` round-trips.

## 4. Proven vs hypothesis

**Proven (measured):**

* The end-pool SLP deficit is exactly **50 − 42 = 8 groups = 32 words**;
  the full `(dst, words)` multiset diff is enumerated above (§1).
* The dominant difference is a **16-byte window shift**: the original
  groups `0x204,0x214,0x224 / 0x23c,0x24c,0x25c,0x26c`, the base groups
  `0x208,0x218,0x228 / 0x240,0x250,0x260,0x270`.
* The original's `.text` is **config-major**: a top-level
  `if (iPadHD) / else if (n9) / else` with straight-line HD and n9 arms
  (confirmed by the branch structure at `0xd103e`/`0xd127a`, the seven
  `vst1` windows per arm, and the REA decompilation).
* Reproducing the exact windows (`vm2`) gives 6984 B / 45.31 % — the
  windows are not what the fuzzy metric rewards.
* The best raw shape remains **`v178`, 7024 B / 49.040 %**; it is 48 B
  short and no 7072-B variant found this pass beats 48.923445 %.

**Hypothesis (not proven):**

* The original source is a *hybrid*: `if (iPadHD) { all HD constants }
  else if (n9) { all n9 constants } else { the field-major
  retina/iPad/default ternaries }`.  The `else` arm keeps per-field
  selects (`it eq; moveq`, `vbsl`), which is why only the HD/n9 arms are
  fully vectorised.  A full rewrite of the ctor in that shape (`vcfg2`)
  compiles to 6400 B, so the split point between the arms is not the
  whole ctor; the original's `0x4..0x1d0` and `0x1e4..0x280` halves are
  laid out separately (`0xd1268 b.w 0xd21e2`, `0xd12fc b.n 0xd1860`).
* The remaining 48 B of `v178` is the original's four outlined cold
  `iPad ? A : B` tails (`0xd2802` 38 B, `0xd2828`/`0xd2836`/`0xd2844`
  14 B each = 80 B) minus the base's single 28-B tail.  Adding them
  requires a source shape that marks the `iPad` test cold; every explicit
  `if/else` conversion tried so far *shrinks* the body instead.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6912
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6912/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6912/ours.dis

python3 /dev/shm/w6912/enum8.py orig.dis \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  ours.dis cmake-build-match/libgof2hdaa.so
bash /dev/shm/w6912/fast.sh /dev/shm/w6912/v178.cpp   # -> 7024 49.040000000
```

Scratch harnesses (not committed, `/dev/shm/w6912/`): `enum8.py`
(SLP-group enumerator), `groups2.py`, `fast.sh`, `score.py`,
`scorelib.py`, `lib.py`, `mkcfg.py`, `mkcfg2.py`, `mkmove2.py`,
`sorthead.py`, `sweep178.py`, `combo178.py`, `sweepvm2.py`, `sweepvm2b.py`, plus the
variant sources `Layout.base.cpp`, `v178.cpp`, `v178_16c.cpp`,
`v178_234c.cpp`, `vcfg.cpp`, `vcfg2.cpp`, `vcfg3.cpp`, `vm1.cpp`,
`vm2.cpp`, `vsh9/12/15.cpp`, `orig.dis`, `ours.dis`, `enum8.txt`.
