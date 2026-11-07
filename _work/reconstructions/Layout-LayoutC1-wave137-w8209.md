# `Layout::Layout` C1/C2 — wave 137 / worker w8209
## Fuzzy-first adjacent-statement hill climb: 44.8 % -> 45.9 % fuzzy at 7,072 B

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-136 master `655e3bcc5` (7072 B; the committed `report.json` was
stale at 45.0 %, a fresh build of HEAD scores **44.8 %** fuzzy / **5,783** raw
same-address diff bytes).

## Result — landed: 8 statement swaps (size-exact, fuzzy 44.8 -> 45.9)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| fuzzy `difflib` ratio (verify.py) | 44.8 % | **45.9 %** |
| target-function raw diff bytes | 5,783 | 5,812 |
| `.text` differing bytes | 84,709 | 84,764 |
| `byte_exact` / `linked_exact` | 4507 / 4507 | **4507 / 4507** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.

## 1. Why the fuzzy metric, not raw bytes

Wave 135 (w8107) hill-climbed the *raw same-address byte diff* and landed six
swaps (5,783 -> 5,757 bytes).  Wave 136 (w8108) then found an ascending
`0xb8..0x118` reorder at 5,754 raw bytes but only 41.7 % fuzzy, so the raw-byte
metric and the instruction-sequence metric disagree.  This pass re-scores every
adjacent swap with `verify.py`'s fuzzy ratio (difflib over the normalized
instruction stream, `autojunk=False`) and keeps the best ratio at exactly
7,072 B.

The committed `report.json` at HEAD is stale (it predates wave 135 and says
45.0 %); a fresh build of HEAD is 44.8 %.  The mission's "44.8 %" baseline is
therefore the real HEAD value.

## 2. Method

`/dev/shm/w8209/layout_units.py` splits the constructor body into 205
top-level units (statements, local declarations, and whole `if/else` chains;
`else` continuations are merged into the preceding unit).  `/dev/shm/w8209/climb3.py`
does best-improvement adjacent-swap hill climbing:

1. For each adjacent pair, skip swaps that break a data dependency
   (`this->field` read/write sets and local declarations are checked both
   ways, so a field read in one unit is never moved across its writer).
2. Render the reordered body, `ninja -C cmake-build-match libgof2hdaa.so`
   (0.5 s), and score with the same `verify.py` disassembly/normalization code
   used by the gate.
3. Accept the highest fuzzy ratio that keeps size 7072 B; tie-break on lower
   raw bytes.  Repeat.

Each full 204-swap round costs ~200 s.  The climb started from the pre-wave-135
body (44.8 %) because the w8107 swaps are net-negative on fuzzy (HEAD 43.6 %).

## 3. Landed reorders

Reverted (the six w8107 raw-byte swaps, which lower fuzzy to 43.6 %):

1. `field_0x1c` back before `field_0x10_rightMargin`
2. `buttonInsetX` back before `field_0x2c_rowHeight`
3. `field_0x114_strip5c` back before `field_0x108`
4. `field_0x1dc` back before the `field_0x1cc` if/else chain
5. `rewardBoxX` back before `rewardBoxHeight`
6. `field_0x2f4` back before `field_0x2f8`

New fuzzy-first swaps (both keep 7072 B):

7. `field_0x214` before `field_0x1ec` (round 0: 44.8 -> 45.5 %)
8. `field_0x27c` before `field_0x274` (round 1: 45.5 -> 45.9 %)

Both new swaps are pure member-store reorders; no expression changed and no
field is read before it is written.

## 4. Device-tree / group-move checks (no win on this body)

The wave-133 w7752 lever (make the `0x294, 0x298, 0x29c, 0x2a0` group
contiguous ascending, optionally with `0x170` or `0x26c`) was re-tested on the
current body:

| variant | size | fuzzy | raw |
|---|---:|---:|---:|
| base (after climb) | 7072 | 45.9 % | 5,812 |
| `0x294` contiguous | 7072 | 45.9 % | 5,812 (no codegen change) |
| `0x294` + `0x170` | 7080 | 45.0 % | 5,839 |
| `0x294` + `0x26c` | 7072 | 44.6 % | 5,759 |
| `0x170` alone | 7080 | 45.0 % | 5,839 |
| `0x26c` alone | 7072 | 44.6 % | 5,759 |

The w7752 48 % variant does not transfer: the wave-135/136 body changes the
surrounding register pressure, and the `0x294` group is already scheduled
adjacently by the backend.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8209
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8209/score.py                         # 7072 / 45.9 / 5812
```

Scratch harnesses (not committed, `/dev/shm/w8209/`): `layout_units.py`
(statement splitter/merger), `climb3.py` (dependency-checked adjacent-swap hill
climb), `variants.py` (SLP group-move tests), `score.py` (single-function fuzzy
+ raw score), `state.json` (final unit order), `Layout.head.cpp` /
`Layout.pre135.cpp` (pristine bodies).

## 6. Open questions

* The climb is a strong local optimum at 45.9 %.  Non-adjacent moves
  (e.g. hoisting the `0x140..0x1d0` tail groups) were not exhausted; the
  `0xdc..0xe8` scalar-vs-`vst1` window is still unmatched.
* The original's prologue pushes `r6` (six registers); ours pushes five.  The
  w7752 `0x294` group was the only known lever for that and it no longer fires
  on this body.
* If raw bytes ever outrank fuzzy again, the six w8107 swaps are the revert
  point (5,812 -> 5,757 with fuzzy 45.9 -> 43.6).
