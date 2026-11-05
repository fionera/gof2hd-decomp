# Layout::Layout — wave 119 / worker w6784
## Exhaustive 7072-B search: no landable source win

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), ELF VMA `0xd1028`,
original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6784`, base = current wave-119 master.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — no source change landed

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio (linked) | 48.655110580 % | **48.655110580 %** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No baseline update.  File size stays exactly **2,187,328 B**, 27 section
headers unchanged.

## The 7072-B wall

`byte_exact` compares each function's raw bytes, and every literal-pool word
in a GOT-relative `ldr/add` pair holds `GOT_slot - pool_address`.  Any change
to `Layout::Layout`'s size shifts `.text` and rewrites those words in every
following function: a 7076-B ctor drops `byte_exact` 4505 → 2454.  The ctor
therefore **must** stay exactly 7072 B, and every candidate below was scored
with that constraint.

## Best measured variants (exact linked score, real builds)

| variant | size | ratio % |
|---|---:|---:|
| HEAD | **7072** | **48.655110580** |
| `0x1a0`/`0x1a8` swap (#147) | 7076 | 48.675562637 |
| #147 + `0x2d4` arm swap (#37) | 7068 | **48.704663212** |
| #147 + `0x110`/`0x2b8` swap (#83) | 7068 | 48.694959155 |
| #147 + `0x120`/`0x128` swap (#91) | 7068 | 48.694959155 |
| #147 + #37 + #83 | 7060 | **48.724082935** |
| #147 + `0x28c`/`rewardBoxY2=132` swap (#231) | 7060 | **48.763955343** |
| #147 + #37 + #83 + move `0x28c` | 7044 | **48.822355289** |
| #37 alone (`0x2d4` arms) | 7064 | 48.684210526 |

Every 7072-B variant found is **≤ 48.655110580 %**.  The best source shape
(#147+#37, 7068 B) is 4 B short; padding it with any real statement costs more
alignment than the 0.0496 pp it gains.

## Searches run (all exact linked metric, 7072 B required)

| search | builds | best 7072 result |
|---|---:|---:|
| exhaustive adjacent swaps on HEAD | 606 | 48.655110580 (no win) |
| exhaustive adjacent swaps on #147 (7076) | 606 | 48.655110580 (no win) |
| exhaustive adjacent swaps on #147+#37 (7068) | 606 | 48.635729934 (no win) |
| all pairs of the 120 "interesting" swaps | 7,078 | 48.575413429 (#215+#234) |
| single-statement moves ±8 on #147 | 3,232 | 48.655110580 (no win) |
| single-statement moves ±8 on #147+#37 | 3,232 | 48.446215139 (no win) |
| single-statement moves ±8 on #147+#37+#83 | 3,232 | 48.484848485 (no win) |
| int→float literal conversion, every assignment | 172 | 48.385811080 (`0x1c8`) |
| top-level ternary inversion `(!c)?b:a`, every assignment | 178 | no 7072 hit |
| `(int)(float)` / `(float)` / `(int)` wrap, every assignment | 552 | no 7072 hit |
| SLP-run adjacency (`0xb0`,`0xec`,`0x13c`,`0x140`,`0x16c`,`0x180`,`0x2a0`,`0x204`,`0x208`) | 9 | `0xb0`/`0x16c`/`0x2a0` are no-ops; rest 7016–7108 |
| full address-order reorder of the ctor | 1 | 6940 B, 20.072115 % |
| partial address reorders (`≤0x78`,`≤0x6c`,`≤0x1e0`,`≤0x2d8`) | 4 | 7068/24.760383, 6880/31.549296, 7020/19.553963 |
| block-A order prefix reorders (8/16/24/30 fields) | 4 | 7048/41.585342, 7056/46.202910 |
| `0x2c0` moved next to `0x2c4` | 2 | 7028/45.238571 (7020/45.305633 on #147+#37) |
| `0x28c` moved next to `0x288` (original uses `strd`) | 2 | 7048/48.573139 (7044/48.493315 on #147+#37) |
| `field_0x9c` float-local / int-temp / int-cast forms | 3 | 7056/48.454636, 7064/48.564593 |
| `field_0xac` float forms | 3 | 7072/48.655110580 (no change) |
| `field_0x2c8` no-temp / plain / int-temp / float-temp | 4 | 7072/48.655110580, 7072/46.432842 |
| `field_0x2d8` split-ternary form | 1 | 7096/42.266402 |
| `field_0x2d0` cast forms | 2 | 7072/48.655110580, 7064/48.226385 |
| duplicate-store append (`0x285`,`0x286`,`0x284`,`0x4`,`0x28c`) | 5 | 7072/48.296473 (`0x285`) |

## Structural findings (for the next pass)

The original is **not** a flat ternary chain.  Its control flow is

```
if (Globals::iPadHD) { block A } else if (Globals::n9) { block B } else { block C }
<common tail>
```

with block A at `0xd1042..0xd1268` (jumps to the tail at `0xd21e2`), block B at
`0xd127e..0xd14c6`, block C at `0xd1570..0xd21e2`, tail at `0xd21e2..0xd2800`.
Block C's stores are in **address order**; block A's stores are grouped by the
SLP vectorizer (destinations `0x14,0x4,0x24,0x34,0x44,0x54,0x64,0x2c8,0x7c,
0x74,0x2c0,0x2d8,0xac,0x8c,0xa4,0xc0,0xb0,0xec,0x294,0x2a4,0x108,0x118,
0xdc,0xe4,0xfc,0x104,0x2b4,0x128,0x140,0x150,0x160,0x170,0x180,0x190,
0x1a0,0x1b0,0x1c0,0x1d0,0x138,0x1e0`).  The current HEAD source already
reproduces the merged `iPadHD` branch (clang hoists the shared test), so the
remaining gap is store pairing and scheduling, not a missing top-level branch:

* `strd` count: original **62**, HEAD **47**; `str` count: original **121**,
  HEAD **188**.  The original pairs 15 more stores than HEAD.  Pairing is
  worth ~0.75 pp but costs 4 B per pair (two `str.w` = 8 B, one `strd` = 4 B),
  so a pairing pass must be paired with a compensating +4 B source change.
* `vst1.32` count: original **81**, HEAD **70** (the 8 missing SLP groups).
* Original block C stores `0x28c` paired with `0x288` (`strd r2,r3,[r0,#648]`);
  HEAD stores `0x28c` separately in three branches.
* Original stores `0x2d4` **only once**, in block C, as
  `n9 ? 160 : (retina ? 100 : 60)` (`strd r4,r6,[r0,#724]`); block A/B never
  write it.  HEAD writes `112` for `iPadHD` and uses a 4-arm `if/else` chain.
* Original stores `rewardBoxWidth` (`0x3e8`) once per branch (4 `str.w`); HEAD
  stores it once from a ternary.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6784
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
```

Scratch harnesses (not committed) in `/dev/shm/w6784`:
`fworker.py` (PCH-accelerated minimal-TU scorer, ~0.25 s/variant on 6 cores),
`unitparse.py`, `windowmove*.py`, `pairswap.py`, `cast37.py`, `floats.py`,
`invert.py`, `slpruns.py`, `partialsort.py`, `reorder_ctor.py`,
`Layout.base.cpp`, `Layout.7037.cpp`, `Layout.7060.cpp`, `pre.h.pch`.
`fworker.score_src` is byte-for-byte equivalent to `tools/verify/verify.py`
on the baseline (48.655110580 %).
