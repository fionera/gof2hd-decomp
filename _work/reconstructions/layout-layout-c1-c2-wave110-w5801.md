# Layout::Layout C1/C2 — wave 110 (w5801): the `0x9c` int-phi rewrite
# plus four `(int)(float)(int)` wraps, and the measured size/score frontier

Worker **w5801**, branch `wave110-w5801`, base `390f58052`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (same body and score),
ELF VMA `0xd1028`, original size **7072 B**.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — landed a real, size-exact fuzzy improvement

| metric | before (wave 109) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| `_ZN6LayoutC1Ev` unrounded linked `difflib` ratio | 48.269001193792 % | **48.337646824607 %** |
| `_ZN6LayoutC2Ev` | 48.269001193792 % | **48.337646824607 %** |
| normalised insns (orig/ours) | 2529 / 2497 | 2529 / **2494** |
| `byte_exact` / `linked_exact` | 4500 / 4500 | **4500 / 4500** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No section change: still 28 section headers (the pre-existing wave-108 192-byte
`.jcr` is the only extra vs the original's 27), `.ARM.extab` still `0x26294`
(4 B short), `.text` size/base unchanged (`0x1337c8` @ `0x69440`), so
`byte_exact` stays 4500.

## The five edits (ctor only)

1. `field_0x98` moves **before** the `field_0x9c` if/else chain (the original's
   HD block stores `0x94,0x98,0x9c,0xa0` as one `stmia` group, so `0x98` must
   be adjacent to `0x9c` in source).
2. `field_0x9c` is rewritten as an int phi with a float round trip:

```cpp
int u9c;
...
if (Globals::iPadHD) u9c = 70;
else if (Globals::n9) u9c = 75;
else if (Globals::retinaDisplay) u9c = 100;
else u9c = 50;
this->field_0x9c = (int)(float)u9c;
```

   The original's retina path stores `0x9c` through
   `vldr s0` / `vcvt.s32.f32` / `vstr s0,[r0,#156]` (d198a..d19a8), i.e. an
   int→float round trip; the `int u9c` phi + `(int)(float)u9c` reproduces the
   `vcvt.s32.f32` / `vstr` fingerprint.
3. `field_0xac` gets the redundant inner `(int)` wrap
   `(int)(float)(int)(...)` (the original's `0xac` is in the mid-block intphi
   group; wave 109 landed the same device on `0x10c`/`0x138`/`0x160`).
4–5. Four `(int)(float)(int)(...)` wraps on `field_0x180`, `scrollBarInset`,
   `field_0x6c` and `field_0x2d0`. These are pure size ballast: the 7040-B
   body is 32 B short of the original, and these four wraps add exactly
   +8 B each (32 B total) while costing only ~0.06 pp.

## Measured frontier (all real `link_so` builds, unrounded linked score)

The fuzzy metric is not monotone in size. The table records the whole
frontier found around the wave-109 body; **only 7072-B rows are landable**.

| variant | size | % | insns |
|---|---:|---:|---:|
| wave-109 landed body (HEAD) | **7072** | 48.2690 | 2497 |
| `0x98` moved before the `0x9c` chain | 7032 | 48.2346 | — |
| + `0xac` intphi wrap | 7036 | 48.3652 | — |
| + `0x9c` int-phi (`int u9c` + `(int)(float)u9c`) | 7040 | **48.3955** | 2492 |
| + adjacent swap `field_0x1a4`↔`field_0x190` | 6996 | 48.6497 | 2470 |
| + adjacent swap `field_0x1a4`↔`field_0x188` too | 6996 | **48.6897** | — |
| 7040 body + `field_0x18c` (+32) intphi | **7072** | 48.0414 | — |
| 7040 body + `field_0x180,scrollBarInset,0x6c,0x2d0` (+8×4) | **7072** | **48.3376** | 2494 |
| 7040 body + `field_0x180,0x6c,0x2d0,0x2c8,0x8c,0x148,0x150,0x168` (+8×8) | **7072** | 48.0876 | — |
| 6996 body + wraps summing to +76 (best found) | **7072** | 48.0876 | — |
| all if/else chains → ternaries on the wave-109 body | 7096 | 44.8331 | 2503 |
| HD units reordered to the original's HD store-group order | 6896 | 34.1709 | 2446 |
| HD units first (original store order) + rest current | 6864 | 36.0669 | 2434 |
| iOS 1.1.4 recovered source order (all units) | 6912 | 21.8857 | 2424 |
| non-HD units reordered to the iOS order | 6960 | 19.4673 | 2464 |

Searches performed (all real builds, ~0.9 s each):

* **All 204 adjacent swaps** of the wave-109 body: no 7072-B variant above
  48.2690; best size-exact swap is 48.1577 (`0x1a8`↔`0x1a0`).
* **All 204 adjacent swaps** of the 7040-B `0x9c`-intphi body: best overall
  `0x1a4`↔`0x190` at 6996 B / 48.6497; best 7072-B swap 48.1577.
* **All single `(int)(float)(int)` wraps** on the 7040-B body (236 fields):
  +8 delta fields are `field_0x180` (48.3955, code-identical), `scrollBarInset`,
  `field_0x6c`, `field_0x2d0`, `field_0x204`, `field_0x1a8`, `field_0x194`,
  `field_0x168`, `field_0x150`, `field_0x148` (48.3762), `field_0x8c`,
  `field_0x2c8`, `field_0x1f4` (48.2978); +32 `field_0x18c` (48.0414),
  `centerYBase` (42.8856); +40 `field_0x198` (48.0811), `field_0x2cc`
  (47.9332). Exhaustive 4×+8 search with `field_0x180` fixed: best is the
  landed 48.3376. Other +32 partitions (2×16, 24+8, 12+8+8+4, 12+12+8,
  32 alone) all measure below 48.3376.
* **All single wraps** on the 6996-B body: the +76 needed to reach 7072 can
  only be made from expensive wraps (`0xa8` +68 → 18.0 %, `0x234` +96,
  `orbitInfoSystemY` +72, `field_0x2c4` +56, `field_0x18c` +44, `field_0x1a4`
  +32); every 7072-B combination tried lands ≤ 48.0876.
* **All 204 adjacent swaps** of the landed 7072-B body: no 7072-B variant
  above 48.3376 (the best 7064-B swap is 48.3473).
* **150 random unit moves / swaps** of the landed body: no 7072-B hit above
  48.3376.

## Store-group / vstr evidence

The original's HD block (`0xd1042..0xd1268`) has **42** store groups
(27 `vld1.64` pool vectors, 81 `vst1.32` + 62 `strd` + 2 `stmia` over the whole
function). The wave-109 body has **50** groups in its HD block and 70
`vst1.32` / 44 `strd`, i.e. **65 more scalar `str.w`** than the original. The
extra scalar stores are exactly the fields whose source positions are not
memory-adjacent: `0x2c0/0x2c4` (orig `strd`, ours two `str`), `0x94/0x98/0x9c/0xa0`
(orig `stmia`, ours four `str`), `0xb0..0xcc` (orig `vst1 0xc0` then
`vst1 0xb0`, ours `vst1 0xb4` then `vst1 0xc4`), `0xec..0xf8` (orig `vst1`,
ours two `strd`), `0x108..0x124` (orig `vst1 0x108` then `vst1 0x118`, ours
reversed), and `0xdc..0xe8` (orig two `strd`, ours one `vst1`).

Original `vstr` offsets (28): `0x60 0x68 0x6c 0x70 0x2c8 0x2d0 0x8c 0x90 0x9c
0xa8 0xdc 0xe0 0xe4 0xe8 0x138 0x150 0x158 0x15c 0x160 0x168 0x170 0x1a8
0x1e0 0x1e4 0x3e0 0x3f8 0x2f8`. The landed body has 29: it adds
`0x20 0x10c 0x148 0x194 0x204 0x3e8 0x1f4` and misses `0x9c 0x158 0x15c
0x170 0x1e4 0x3f8`. Closing that set is the natural next step but every
individual edit tried here is size-changing (the `0x1e4` store is a float
`vstr` in the original tail at `0xd21fe`, while our header declares
`int field_0x1e4` and `Hud.cpp` reads it as `int`, so retyping it would move
Hud's codegen).

## Proven vs hypothesis

**Proven (binary / measured):**

* The original's `0x9c` retina-path store is an int→float round trip
  (`vcvt.s32.f32` + `vstr` at `0xd199e..0xd19a8`), and its HD-path store is a
  plain int inside the `stmia 0x94` group. The `int u9c` phi + `(int)(float)u9c`
  spelling reproduces that shape and is the largest single score gain found.
* The 7040-B body is 32 B short; the four `+8` wraps on
  `0x180/scrollBarInset/0x6c/0x2d0` are the cheapest size-exact completion
  (48.3376 vs 48.0414 for the single `+32` `0x18c` wrap).
* The 6996-B body (48.6897) is the best fuzzy variant measured for this
  function but is unlandable: every wrap combination that adds the missing
  76 B costs more than it gains.
* The iOS 1.1.4 source order is **not** the Android 2.0.16 order: replaying it
  on the current body gives 6912 B / 21.9 % (and 19.5 % for the tail alone).
* No section / `.ARM.extab` delta is introduced (28 headers, `.ARM.extab`
  `0x26294`, `.text` size/base unchanged).

**Hypothesis (not proven):**

* The remaining ~51.7 % mismatch is dominated by the original's SLP grouping
  (42 HD groups vs our 50) and its `d0-d1` long-lived constant group, which
  the flat body's hill-climbed statement order does not reproduce. Matching it
  would require the original's exact per-field source order *and* expression
  forms, not just the field order.
* `field_0x1e4` is stored as a float (`vstr`) in the original tail; our
  `int` declaration is a real type divergence that cannot be fixed without
  touching `Hud.cpp`.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5801
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w5801/fscore.py _ZN6LayoutC1Ev         # 7072 B, 48.337646824607
```

Scratch harness (all under `/dev/shm/w5801`): `fscore.py` (unrounded linked
scorer), `trybody.sh` (splice ctor body → `link_so` → score), `parseunits.py`
(ctor body → 204 top-level units + field offsets), `reorder_ios.py` /
`reorder_hd.py` / `reorder_more.py` / `reorder_tail.py` (order experiments),
`ifelse2tern.py` (if/else chain → ternary), `scan_single*.py` (single-wrap
scans), `scan_adj*.py` (adjacent-swap scans), `test_combos*.py` (size-exact
wrap partitions), `rand_move.py`, `body.base.txt` (wave-109 body),
`body_98first_ac_9c.txt` (7040-B body), `body_best.txt` (landed 7072-B body).
