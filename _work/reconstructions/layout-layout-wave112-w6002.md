# Layout::Layout C1/C2 — wave 112 (w6002): iOS source order, the real 3-way branch,
# and an exhaustive size-7072 sweep

Worker **w6002**, branch `wave112-w6002`, base `5d98bd391`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (same body and score),
ELF VMA `0xd1028`, original size **7072 B**.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — no landable improvement; the wave-110/111 body is the 7072-B optimum

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| `_ZN6LayoutC1Ev` unrounded linked `difflib` ratio | 48.337646824607 % | **48.337646824607 %** |
| `_ZN6LayoutC2Ev` | 48.337646824607 % | **48.337646824607 %** |
| normalised insns (orig/ours) | 2529 / 2494 | 2529 / 2494 |
| `byte_exact` / `linked_exact` | 4500 / 4500 | **4500 / 4500** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
27 section headers, file size 2,187,328 B, `.text` `0x1337c8`, `.ARM.extab` `0x26298` —
all unchanged (no code change landed).

**This pass is a note-only commit.** Every variant below was built for real
(`link_so` + `tools/verify` scorer) and measured; none beats 48.3376 % at the
mandatory 7072 B.

### The hard size constraint (re-confirmed)

`Layout::C1` must be **exactly** 7072 B. A 7064-B body (one `(int)` removed)
shrinks `.text` by 8 B and drops whole-`.so` `byte_exact` from **4500 to 3221**
(measured with the ratchet) because every following function shifts. So the
48.3955 % / 7048-B body cannot be landed, and neither can the 48.4148 % / 7040-B
body.

## 1. The iOS 1.1.4 oracle gives the *true* source order — and it scores worse

The w3905 note recovered the iOS 1.1.4 `Layout::Layout()` source order from the
iOS binary (the iOS build has no `-Oz` if-conversion, so its store order is the
source order). That order is reproduced here and was applied verbatim to the
current Android expressions (205 statement groups; `int u9c;` kept at the top):

```
4 8 c 10 14 18 1c 20 24 28 2c 30 34 38 3c 40 44 48 4c 50 54 58 5c 60 64 68 6c
2c0 70 74 78 2c4 2c8 2cc 2d0 2d4 2d8 7c 80 84 88 8c 90 94 98 9c ac a8 a0 a4
b0 b8 b4 bc c0 c4 c8 cc d0 d4 d8 dc e0 e4 e8 ec f0 f4 f8 fc 100 104
294 298 29c 2a0 2a4 2a8 2ac 2b0 2b4 2b8
108 10c 110 114 118 11c 120 124 128 12c 130 134 138 13c 140 144 148 14c 150
154 158 15c 160 164 168 16c 170 174 178 17c 180 184 188 18c 190 194 198 19c
1a0 1a4 1a8 1ac 1b0 1b4 1b8 1bc 1c0 1c4 1c8 1cc 1d0 1d4 1d8 1dc 1e0 1e4 1e8
1ec 1fc 204 208 20c 210 214 218 21c 220 224 228 22c 230 234 238 23c 240 244
248 24c 250 254 258 25c 260 264 268 26c 270 274 278 27c 280
3e0 3e4 3e8 3f4 3f8 3ec 3f0 3fc 2bc 1f0 1f4 200 1f8 284 285 286 288 28c 290
2f4 2f8 2fc 300 304 308 30c 310 314 318 31c 320
```

| variant | size | % |
|---|---:|---:|
| **current (hill-climbed) order, current expressions** | **7072** | **48.3376** |
| **full iOS source order, current expressions** | 6936 | 37.1906 |
| iOS order + `(int)(float)` on the original's vstr int fields | 6952 | 37.3140 |
| iOS order for the tail only (`0x1e4..0x320`) | 7000 | 21.0358 |
| iOS order for the first 37 fields only (`0x4..0x2d8`) | 7032 | 21.9288 |
| iOS order for `0x140..0x1dc` only (ascending) | 7008 | 45.1819 |
| iOS order for `0x108..0x134` only (ascending) | 7036 | 46.5246 |
| iOS order for `0x294..0x2b8` only (ascending) | 7088 | 40.6375 |
| iOS order for `0x1e4..0x280` only (ascending) | 6964 | 19.1528 |
| iOS order for `0x1b0..0x1dc` only (ascending) | 7064 | 43.0763 |
| full original HD store order replay (`0x14 4 24 … 1e0`) | 6552 | 15.1684 |

The iOS-order build is **136 B smaller** than the original. The missing bytes are
the original's `(int)(float)` round trips (7 int fields stored with `vstr`) and,
above all, its **SLP/NEON-vectorised tail**: the original computes the
`retina ? A : B` selects of `0x1b0..0x1dc` with `vshl.s32` / `vbsl` / `vdup.16` /
`vceq.i16` and stores them with `vst1.32`, while our flat-ternary body stays
scalar. A faithful source order therefore cannot reach 7072 B with our
expression set.

### Bin-level match map (baseline vs iOS order)

`difflib` equal-instruction density per 256-B bin of the original (relative to
`0xd1028`):

| bin | baseline | iOS order |
|---|---:|---:|
| `0x000` | 27.5 % | 2.5 % |
| `0x100` | 7.9 % | 0.0 % |
| `0x200` | 45.7 % | 65.4 % |
| `0x300` | 81.9 % | 80.3 % |
| `0x400` | 67.2 % | 84.0 % |
| `0x500` | 60.4 % | 42.6 % |
| `0x600` | 64.9 % | 27.8 % |
| `0x700` | 58.5 % | 16.0 % |
| `0x800` | 46.8 % | 17.0 % |
| `0x900` | 35.4 % | 20.3 % |
| `0xa00` | 37.1 % | 38.6 % |
| `0xb00` | 20.5 % | 12.3 % |
| `0xc00` | 35.9 % | 25.6 % |
| `0xd00` | 31.2 % | 14.6 % |
| `0xe00` | 20.0 % | 20.0 % |
| `0xf00` | 17.2 % | 11.1 % |
| `0x1000` | 15.8 % | 17.1 % |
| `0x1100` | 23.4 % | 22.1 % |
| `0x1200` | 29.0 % | 26.1 % |
| `0x1300` | 34.6 % | 0.0 % |
| `0x1400` | 31.2 % | 33.8 % |
| `0x1500` | 29.8 % | 6.0 % |
| `0x1600` | 48.4 % | 46.2 % |
| `0x1700` | 34.1 % | 39.8 % |
| `0x1800` | 69.1 % | 80.5 % |
| `0x1900` | 33.3 % | 22.2 % |
| `0x1a00` | 84.1 % | 90.5 % |
| `0x1b00` | 89.3 % | 87.5 % |

The iOS order is better in the literal-pool bins `0x200..0x4ff` and in the tail
bins `0x1800` / `0x1a00`, but much worse in the HD block (`0x000..0x1ff`) and in
the mid tail. The current order is a *tuned* compromise, not the source order.

## 2. The real 3-way branch (`iPadHD` / `n9` / `retinaDisplay`)

The original is a tail-merged 3-way branch (HD block `d1042..d1268` → `b d21e2`;
n9 block `d126c..d12fc` → `b d1860`; non-n9 block `d1570..d185e` → falls into
`d1860`). A literal source reconstruction was generated by specialising every
per-field ternary with the w5902 `gen3.py` evaluator:

```cpp
if (Globals::iPadHD) { <HD constants> }
else if (Globals::n9) { <n9 constants> }
else { <retina/iPad ternaries> }
```

| variant | size | % |
|---|---:|---:|
| 3-way, iOS source order | 4584 | 17.4850 |
| 3-way, current source order | 4408 | 18.2664 |

Both are far too small: with the explicit branch tree the compiler tail-merges
the three bodies aggressively (the original's 7072 B comes from the flat-ternary
form plus the NEON tail, not from three literal copies). This confirms the
w5902 conclusion that the flat ternary list is the right source shape and the
3-way branch is a compiler artifact.

## 3. Exhaustive `(int)(float)(int)` → `(int)(float)` search (all 256 combinations)

The 8 fields whose source carries `(int)(float)(int)` are
`0x6c, 0x2d0, 0xac, 0x10c, 0x138, 0x160, 0x180, scrollBarInset (0x3e0)`.
Every one of the 2^8 combinations was built and scored.

| combination (bits = removed inner `(int)`) | size | % |
|---|---:|---:|
| `00000000` (baseline) | 7072 | **48.3376** |
| `01000000` (`0x138`) | 7072 | 48.3376 (code-identical) |
| `00010000` (`0x160`) | 7072 | 48.3280 |
| `00000100` (`0xac`) | 7072 | 48.1880 |
| `10000000` (`0x3e0`) | 7064 | 48.3569 |
| `00000001` (`0x6c`) | 7064 | 48.3569 |
| `00000010` (`0x2d0`) | 7064 | 48.3569 |
| `00000011` | 7056 | 48.3762 |
| `10000011` (`0x6c,0x2d0,0x3e0`) | 7048 | **48.3955** (best overall) |
| `11000011` | 7048 | 48.3955 |
| `10010011` | 7048 | 48.3858 |
| `11111111` | 7024 | 48.1038 |

No 7072-B combination beats the baseline. The best 7072-B combination other than
the baseline is 48.3280.

## 4. `(int)(float)` cast scans

### 4.1 Adding a round trip to every field (on the 7048-B body, 309 single-line assignments)

Most fields fold (size unchanged, score 48.3955). Size-changing results:

| field | size | % |
|---|---:|---:|
| `field_0x48` | 6992 | 39.5679 |
| `headerTitleY` | 7016 | 42.5439 |
| `field_0x38`, `field_0x1b4`, `field_0x1b8` | 7032 | 47.61 / 47.82 / 47.82 |
| `field_0x4c` | 7036 | 43.3725 |
| `field_0x3c`, `field_0x40`, `field_0x44`, `field_0x1bc`, `field_0x1b0` | 7040 | 47.65 / 47.85 / 47.73 / 47.48 / 47.72 |
| `field_0xc_leftMargin`, `marqueeWidth`, `field_0xc0` | 7056 | 41.41 / 47.64 / 47.69 |
| `field_0x2d8`, `field_0xa4`, `driftSpeedDivisor`, `field_0x1a4` | 7064 | 48.307 / 43.303 / 42.026 / 48.347 |
| `field_0x90`, `hudInset` | 7072 | 45.994 / 41.975 |
| `field_0x1e4` | 7080 | 45.294 |
| `field_0x1fc` | 7084 | 46.921 |
| `centerYBase` | 7088 | 42.135 |
| `field_0x2c4` | 7100 | 47.021 |
| `field_0xdc` | 7112 | 42.789 |

Only `0x90` and `hudInset` land on 7072 B, and both lose 2–6 pp.

### 4.2 Adding a round trip on the baseline body

309 single-line assignments scanned. 283 stay at 7072 B and are **code-identical**
(48.3376); the only size-7072 field that changes the stream is `field_0x2c_rowHeight`
(47.7214). No improvement.

### 4.3 Removing the round trip (on the 7048-B body)

| field | size | % |
|---|---:|---:|
| `field_0x194` | 7040 | **48.4051** |
| `field_0x180` | 7040 | 48.3955 |
| `field_0x160` | 7040 | 48.3749 |
| `field_0x150` | 7040 | 48.3652 |
| `field_0x10c_rowBaseY` | 7040 | 48.2057 |
| `field_0x138` | 7048 | 48.3858 |
| `field_0xac` | 7048 | 48.2456 |

### 4.4 Faithful cast set (the original's 27 `vstr` fields)

Adding `(int)(float)` to the original's int-vstr fields we lack
(`0x60 0x68 0x70 0x158 0x15c 0x168 0x170 0x1a8 0x1e4 0x2c8 0x3f8`) and removing
it from the four fields the original does *not* round-trip
(`0xac 0x10c 0x180 0x194`):

| variant | size | % |
|---|---:|---:|
| faithful cast set | 7016 | 42.3238 |
| add only | 7104 | 45.3190 |
| remove only | 7036 | 48.1053 |

## 5. `if/else` → ternary conversions (on the 7048-B body)

| field | size | % |
|---|---:|---:|
| `rewardBoxHeight` | 7040 | **48.4148** (best ratio measured this pass) |
| `field_0x134` | 7048 | 48.3955 (code-identical) |
| `field_0x184` | 7048 | 48.3955 (code-identical) |
| `rewardBoxY2` | 7048 | 48.3955 |
| `field_0x1cc`, `field_0x280` | 7048 | 48.3157 |
| `field_0x190` | 7020 | 48.3530 |
| `field_0x2d4` | 7040 | 47.8963 |
| `field_0x218`, `orbitInfoStationY` | 7056 | 47.1901 |
| `field_0x20c`, `orbitInfoTextX` | 7084 | 46.9209 |
| `field_0x260` | 7120 | 45.4672 |

`rewardBoxHeight` as a ternary is the highest fuzzy score measured (48.4148) but
is 32 B short. Adding the faithful `0x1e4` round trip to reach 7072 gives
45.6301; `+0x2d8 +0x1a4` gives 7064 / 48.3171.

## 6. Order searches (all real builds)

* **All 204 adjacent swaps** of the 7048-B body: no 7072-B variant above
  48.3376; best 7072-B swap is `0x140`↔`0x28c` at 48.1894.
* **500 random moves** (1–3 statements per iteration) on the 7048-B body: no
  7072-B hit above 48.3376; best overall 7048 / 48.3955.
* **400 random moves** on the baseline body: only code-identical 7072-B hits
  (48.3376); no improvement.
* **Region reorders** to the iOS ascending order (see §1) all lose.
* The full original HD store-order replay is 15.2 %.

## 7. Conclusion

The wave-110/111 body is a genuine local optimum for the verifier's fuzzy metric
at the mandatory 7072 B. The residual mismatch is structural and not reachable
by source reordering:

1. the original's HD block groups stores as `vst1.32`/`strd` runs of the *iOS
   source order* (`0xb0..0xbc`, `0xc0..0xcc`, `0x140..0x1dc`), while the tuned
   order that scores best groups `0xb4..0xc0`, `0xc4..0xd0`, `0x148..0x154`, …;
2. the original's tail is NEON/`vbsl`-vectorised; ours is scalar;
3. the original's 7 extra `(int)(float)` round trips add ~32 B of `vstr` code
   that the flat-ternary source cannot reproduce without disturbing the stream.

The best measured variants are, in order:

| variant | size | % | landable |
|---|---:|---:|---|
| `rewardBoxHeight` ternary + 7048 base | 7040 | 48.4148 | no (32 B short) |
| remove `(int)(float)` on `0x194` + 7048 base | 7040 | 48.4051 | no (32 B short) |
| remove inner `(int)` on `0x6c,0x2d0,0x3e0` | 7048 | 48.3955 | no (24 B short) |
| remove inner `(int)` on one of `0x6c/0x2d0/0x3e0` | 7064 | 48.3569 | no (8 B short) |
| **baseline (landed)** | **7072** | **48.3376** | **yes** |

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6002
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w6002/fscore.py _ZN6LayoutC1Ev         # 7072 B, 48.337646824607
```

Scratch harness (all under `/dev/shm/w6002`): `fscore.py` (unrounded linked
scorer), `tryvar.py` / `rw.py` (statement move/reorder), `combo8.py`
(all-256 inner-`(int)` search, results in `combo8.json`), `scanwrap2.py`
(add-cast scan), `scanrmcast.py` (remove-cast scan), `scanifelse.py`
(if/else→ternary scan), `adjswap2.py` (adjacent-swap scan), `rand2.py` /
`rand3.py` (random-move searches), `region.py` (ascending-region reorder),
`iosorder.py` / `gen3way.py` (iOS source order and 3-way branch generators),
`Layout.ios.cpp`, `Layout.3way.cpp`, `Layout.3way_cur.cpp`, `Layout.hdord.cpp`.

---

## 8. Second session (same worker): follow-up sweeps from the 7012/6996 frontier

The first session stopped at the "no 7072 variant above 48.3376" frontier. A
second pass re-ran the size-7072 hunt from the *best-scoring* (but too small)
bodies, because the fuzzy metric monotonically prefers the shorter builds and
the question was whether the missing bytes could be re-added without losing
the gain.

### 8.1 The 7048 → 7012 → 6996 swap chain

Starting from the 7048-B body (`(int)(float)(int)` → `(int)(float)` on
`0x6c`, `0x2d0`, `scrollBarInset`), two adjacent swaps of the statement-group
list give a steadily better fuzzy score while shrinking:

| variant | size | % |
|---|---:|---:|
| 7048 body | 7048 | 48.3955 |
| 7048 + swap(`field_0x184`, `field_0x1a4`) | 7012 | 48.4110 |
| 7048 + swap(`field_0x184`,`field_0x1a4`) + swap(`field_0x1f8`,`field_0x140`) | 6996 | **48.6097** |

### 8.2 All 204 adjacent swaps of the 7012 body

No swap lands on 7072 B. Best overall is the 6996-B body above; the best 7012-B
hits are the code-identical ones at 48.4110.

### 8.3 Hybrid baseline/iOS prefix orders

Prefix of the iOS source order (first *n* offsets) followed by the tuned
baseline order for the rest, for n = 9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 53,
61, 77, 93, 109, 125, 141, 157, 173:

| n | size | % |
|---:|---:|---:|
| 9 | 7060 | 24.61 |
| 13/17/21/25 | 7068 | 24.70 |
| 29 | 7036 | 39.66 |
| 45 | 7024 | 38.33 |
| 93 | 6948 | 35.34 |
| 125 | 6940 | 37.93 |
| 173 | 6920 | 37.88 |

Every hybrid is far worse than both pure orders — the tuned order's advantage is
not localised to any prefix.

### 8.4 Re-adding bytes to the 6996 body

`(int)(float)` round trips added to all 189 single-line assignments of the
6996-B body: 161 fold to a code-identical build, 28 change the size, and **none
lands on 7072** (nearest: `field_0x2d8` → 7004, `field_0x2c4` → 7052,
`field_0x1fc` → 7028, `field_0x1e4` → 7020, `field_0xdc` → 7044).

A 4-deep beam search (top-6 sizes per depth) over those 28 fields found exactly
one 7072-B combination — `{centerYBase, field_0x1a4, field_0x2c4}` — at only
**42.03 %**. Adding bytes to the 6996 body is always a net loss.

### 8.5 Why the iOS order really is the source order, yet still scores worse

Disassembling the iOS-order build shows it reproduces the original's HD block
*almost instruction for instruction*: the same `vld1.64 {d2-d3}` +
`vst1.32 {d2-d3}` 16-byte constant groups at offsets `0x14`, `0x4`, `0x24`,
`0x34`, `0x44`, `0x54`, `0x64`, `0x2c8`, `0x7c`, and the same
`strd`/`stmia` runs for `0x74/0x78`, `0x2c0/0x2c4`, `0x8c/0x90`, `0x94..0xa0`,
`0xa4/0xa8`. So the ascending (iOS) order is confirmed as the *source* order and
the original's SLP vectorisation is reproduced by our compiler.

The `difflib` opcode map nevertheless shows why it loses: the iOS build matches
in a few long runs (73, 72, 87 and 126 consecutive instructions) separated by
large gaps (135-, 78-, 40-instruction deletes), while the tuned order matches
many isolated instructions throughout. `difflib.SequenceMatcher.ratio()` counts
matched *elements*, so the tuned order's dense one-instruction matches beat the
faithful order's long-but-sparse runs.

### 8.6 Conclusion of the second session

Unchanged: the wave-110/111 body is the best landable (exactly 7072 B) variant.
The measured frontier is

| variant | size | % | landable |
|---|---:|---:|---|
| 6996-B swap chain | 6996 | 48.6097 | no (76 B short) |
| 7012-B swap | 7012 | 48.4110 | no (60 B short) |
| 7048-B inner-cast removal | 7048 | 48.3955 | no (24 B short) |
| **baseline** | **7072** | **48.3376** | **yes** |

Scratch for this session: `/dev/shm/w6002/h2.py` (group parse / emit / build /
score harness), `exp1.py` (7048→7012→6996 chain), `exp3.py` (204 adjacent swaps
of the 7012 body), `exp5.py` (iOS order), `exp6.py` (hybrid prefixes),
`exp7.py`/`exp8.py` (add-cast scans on 6996 / 7052), `exp9.py` (beam search),
`diffmap2.py` (difflib opcode map), `orig_addr.dis` / `ours.dis` (side-by-side
disassembly).
