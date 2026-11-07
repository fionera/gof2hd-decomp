# `Layout::Layout` C1/C2 — wave 135 / worker w8107
## Outer device-tree rebuild: tested (no win); landed 6 statement swaps, −26 diff bytes

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-135 master `2e51d765d` (7072 B, unrounded `difflib` **44.8 %**,
target-function differing bytes **5,783**).

## Result — landed: 6 statement swaps (size-exact, −26 differing bytes)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| unrounded `difflib` ratio | 44.8 % | 43.6 % |
| target-function differing bytes | 5,783 | **5,757** (−26) |
| `.text` differing bytes | 84,960 | **84,934** (−26) |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
The fuzzy ratio dips (43.6 %) because the reorder changes the normalized
instruction sequence, but the primary raw-byte metric improves.

Landed swaps (each keeps 7072 B; found by adjacent-statement hill climbing):

1. `field_0x1c` after `field_0x10_rightMargin`
2. `buttonInsetX` after `field_0x2c_rowHeight`
3. `field_0x114_strip5c` after `field_0x108`
4. `field_0x1dc` after the `field_0x1cc` if/else chain
5. `rewardBoxX` after `rewardBoxHeight`
6. `field_0x2f4` after `field_0x2f8` (descending, unlike the ascending guess)

Adjacent-swap hill climbing from HEAD: `5783 -> 5771 (0x2f4/0x2f8) -> 5761
(0x28/0x2c) -> 5760 (0x1c/0x10) -> 5759 (0x114/0x108) -> 5758 (0x1dc/0x1cc)
-> 5757 (rewardBoxX/rewardBoxHeight)`. Each round is a full `link_so` build +
raw-byte score; no swap lowered the size below 7072.

## 1. Ground truth: the outer split is `iPadHD` / `n9` / default

Resolving the GOT loads in the original prologue (literal-pool words read from
the `.so`, `add rX,pc` with PC = insn+4, no alignment):

* `d1030 ldr r1,[pc,#716]` (lit `d1300` = `0x0013f356`), `add r1,pc` at
  `d1032` (PC `d1036`) → `0x21038c` = GOT slot of `Globals::iPadHD`.
* `d126c ldr r1,[pc,#620]` (lit `d14dc` = `0x0013f176`), `add r1,pc` at
  `d126e` (PC `d1272`) → `0x2103e8` = GOT slot of `Globals::n9`.
* `d164e ldr r2,[pc,#648]` (lit `d18d8`), `add r2,pc` at `d1664` (PC `d1668`)
  → `0x2103f0` = GOT slot of `Globals::iPadLarge` (used only inside the
  default arm's `0x5c..0x78` chain).
* `d1576 ldr r2,[pc,#860]` (lit `d18d4`), `add r2,pc` at `d157e` (PC `d1582`)
  → `0x2103e4` = GOT slot of `Globals::retinaDisplay`.
* `d1870 ldr r1,[pc,#128]` → `0x21032c` = `Globals::iPad`; `d1872` →
  `0x2103e4` = `Globals::retinaDisplay` (common-arm conditions).

So the source shape is `if (iPadHD) {HD} else { if (n9) {n9} else {default}
/* non-HD common */ } /* all-device tail */`.  `iPadLarge` is an inner
condition, not the top-level second arm.

## 2. Device-tree rebuild experiments (all real `link_so` builds)

Hypothesis: the original source is device-major and its per-arm field order is
strictly ascending, so reordering the flat ternary body ascending should make
the SLP groups match.  The original HD arm's groups are `0x4,0x14,...,0x64`,
`0x7c`, `0x94`, `0xa4`, `0xb0`, `0xc0`, `0xec`, `0x108`, `0x118`, `0x128`,
`0x140..0x1d0`, `0x294`, `0x2a4`, `0x2c8` — consistent with ascending source
order (float runs `0xdc..0xe8` break the int groups, so no group at `0xd0`).

| # | source shape | size | ratio % | diffbytes |
|---|---|---:|---:|---:|
| 0 | HEAD flat field-major body | **7072** | **44.8** | **5,783** |
| 1 | full ascending sort of every statement | 6888 | 20.3 | 6,190 |
| 2 | ascending `0x94..0x1e0` | 6856 | 38.1 | 5,906 |
| 3 | ascending `0x94..0x108` | 6912 | 39.9 | 5,932 |
| 4 | ascending `0x94..0x110` | 6896 | 40.9 | 5,829 |
| 5 | ascending `0x94..0x140` | 6936 | 39.9 | 5,884 |
| 6 | ascending `0x108..0x1e0` | 6968 | 37.5 | 5,982 |
| 7 | ascending `0x140..0x1e0` | 7008 | 38.6 | 5,997 |
| 8 | ascending `0x1e0..0x400` | 6772 | 13.5 | 6,292 |
| 9 | ascending `0x1e4..0x2f4` | 6896 | 15.2 | 6,213 |
| 10 | first block `0x4..0x78` + `0x2c0..0x2d8` ascending | 7028 | 37.4 | 5,958 |

Every ascending variant is **smaller** than 7072 and scores worse on raw bytes.
The original is *not* simply ascending: the SLP vectorizer creates its constant
groups in the order `0xc0, 0x128, 0x140..0x1a0, 0x14, 0x4, 0x24..0x64, 0x2c8,
0x7c, 0x1b0, 0xb0, 0xec, 0x294, 0x2a4, 0x108, 0x118, 0x1c0, 0x1d0` (literal-pool
order), which is not ascending and is not the store order either.

## 3. Why the device tree does not reproduce the size

Region sizes (original vs HEAD):

| region | original | HEAD |
|---|---:|---:|
| prologue + HD arm | 580 | 562 |
| n9 arm | 128 | 132 |
| default arm | 752 | 778 |
| non-HD common | 2434 | 2602 |
| all-device tail | 2534 | 2348 |

HEAD's HD arm is 170 insns / 27 `vst1` groups vs the original's 164 / 27 — the
HD arm is already essentially reproduced.  The real residuals are the common
arm (HEAD 168 B too big) and the tail (HEAD 186 B too small): the original's
tail carries its own `if (iPadHD) / if (n9) / else` split, while HEAD's flat
tail lets the compiler merge it into the common arm.  Device-major rebuilds
(wave 133/134) shrink the body by 100–1600 B because the compiler does not
duplicate the common code the way the original tree does.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8107
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w8107/score.py                         # 7072 / 43.6 / 5757
```

Scratch harnesses (not committed, `/dev/shm/w8107/`): `score.py` (single-function
unrounded ratio + raw same-address byte diff), `offsets.json` (DWARF name→offset
map), `mkasc.py` (full ascending sort), `mkpart.py` (range sort), `mkunit.py`
(move one statement), `swap.py` / `hill.py` (adjacent-swap hill climbing),
`Layout.head.cpp` (pristine HEAD source).

## 5. Open questions

* The common arm's `if (n9) { vector store } else { retina ? ... }` split for
  `0x7c..0x8c` is still unmodelled; it is the first divergence in the common
  region (original starts at `0x2d0`, HEAD at `0x2c8`).
* The tail's device split (wave-132 variant D) is 20 B over 7072 on the w7683
  body; applying it to HEAD and then trimming the common arm by the same 186 B
  is the remaining structural lead.
* Adjacent-swap hill climbing plateaued at 5757; non-adjacent moves of the
  reward-box stragglers (`0x94`, `0x144`, `0x1f0`, `0x1f4`) all regressed
  (5,806–5,974), so the flat body is a strong local optimum.
