# `Layout::Layout` C1/C2 — wave 160 / worker w10606
## The `0x140` 16-byte group: full store-order diff; every contiguous
## placement that creates it costs 28–120 B (size 6,952–7,044), so it cannot
## be landed under the size-exact gate.  Source unchanged; baseline re-measured
## raw **5,739**, aligned **1,978**, 7,072 B.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-159 HEAD `08af4633b`: raw same-address diff **5,739**,
aligned instruction diff **1,978**, size 7,072.
Exit: raw **5,739**, aligned **1,978**, size 7,072 (no source change).
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.

Ghidra provider was unavailable (`rea decompile` -> `provider_unavailable`,
`GHIDRA_INSTALL_DIR` incomplete); the analysis below is from authoritative
objdump disassembly of the original `.so` plus the checked-in `Layout.h`.

## 1. The constructor is branch-specialised by the display globals

The original loads `Globals::iPadHD` once at `d1036`, then the whole body is
duplicated per display class: iPadHD block `d1042..d1268` -> `b.w d21e2`;
n9 block `d127e..`; then iPadLarge/retina/iPad/default.  The original's
`iPadHD` block is a clean run of 16-byte NEON `vst1.32` groups separated by
`strd`/`str`/`stmia` singles.  Our ternary-based source also gets
branch-specialised, but only partially: the compiler only duplicates the
stores that are cheap/near the branch, and leaves the rest in the common
tail.

## 2. Full `iPadHD`-block store order (offset/width/kind)

```
ORIG (42 stores):
14/16vst1 4/16vst1 24/16vst1 34/16vst1 44/16vst1 54/16vst1 64/16vst1
2c8/16vst1 7c/16vst1 74/8strd 2c0/8strd 2d8/4str ac/4str 8c/8strd
94/16stmia a4/8strd c0/16vst1 b0/16vst1 ec/16vst1 294/16vst1 2a4/16vst1
108/16vst1 118/16vst1 d0/12stmia dc/8strd e4/8strd fc/8strd 104/4str
2b4/8strd 128/16vst1 140/16vst1 150/16vst1 160/16vst1 170/16vst1
180/16vst1 190/16vst1 1a0/16vst1 1b0/16vst1 1c0/16vst1 1d0/16vst1
138/8strd 1e0/4str

OURS (51 stores):
14/16vst1 4/16vst1 24/16vst1 34/16vst1 44/16vst1 54/16vst1 64/16vst1
2c8/16vst1 7c/16vst1 74/8strd 2c4/4str 98/4str 2d8/4str 8c/8strd
1e0/4str 9c/4str ac/4str b0/16vst1 c0/16vst1 294/16vst1 2a4/16vst1
dc/16vst1 a0/4str a8/4str a4/4str d0/8strd d8/4str ec/8strd f4/8strd
fc/8strd 104/4str 2b4/4str 118/16vst1 108/16vst1 128/16vst1
150/16vst1 160/16vst1 170/16vst1 180/16vst1 190/16vst1 1a0/16vst1
1b0/16vst1 2b8/4str 23c/4str 138/8strd 148/8strd 204/4str 2bc/4str
2c0/4str 1c0/16vst1 1d0/16vst1
```

The `vst1` group-base sequence (the metric w10508 tracked) is:

```
ORIG  14 4 24 34 44 54 64 2c8 7c c0 b0 ec 294 2a4 108 118 128 140 150 160 170 180 190 1a0 1b0 1c0 1d0
OURS  14 4 24 34 44 54 64 2c8 7c b0 c0 294 2a4 dc 118 108 128 150 160 170 180 190 1a0 1b0 1c0 1d0
```

Four independent deltas: `b0/c0` order, `dc` vs `ec`, `118/108` order, and
the missing `140` group.  The `150..1d0` run is already exact.

## 3. Where `0x140`..`0x14c` are stored in our baseline

* `0x140` and `0x144` are **not** in the iPadHD block: they are common-tail
  stores at `0xd25be` (`0x144`) and `0xd26da` (`0x140`), because the source
  statements are at lines 284/295, far from the `0x130/0x134/0x138` chain.
* `0x148`/`0x14c` are a `strd` at `0xd1248` (end of the iPadHD block).
* The original groups all four as one `vst1.32 {d16-d17}` at `0xd11f6`,
  values `{15,18,2,189}` (literal pool `d1330`), placed between the `0x128`
  group and the `0x150` group.

## 4. Sweeps: the group is reachable, but never at 7,072 B

Single-TU compile (`clang 7.0.2`, exact flags) + `.text._ZN6LayoutC2Ev`
disassembly; 703 contiguous insertion points and 192 permutation/anchor
combinations tested.

* **All 24 permutations of `0x140/0x144/0x148/0x14c`**, inserted contiguously
  before `0x150`/`0x154`/`0x158`/`0x15c`, with `0x13c` kept or moved after
  `0x15c`/`0x1d0`/`0x1dc`/`0x2b4`: **96 of 192 variants create a `0x140` group**
  (contrary to w10508's "never 0x140"), but every one gives
  `... 128 140 150 160 17c 18c 19c 1d0 1c0` — the `0x170/0x180/0x190/0x1a0/0x1b0`
  groups collapse to `0x17c/0x18c/0x19c` (the scheduler shifts the 16-byte
  boundaries by 12 B).  Sizes **6,952–7,044**; none is 7,072.
* **Natural order `140,144,148,14c` at every line boundary** of the
  constructor (703 contiguous insertion points x `0x13c` positions): no
  contiguous placement yields 7,072; the best `0x140` variant is 6,728 B.
* **Move only `0x140/0x144`** next to the existing `0x148` (after `0x154`):
  compiler groups `0x13c,0x140,0x144,0x148` (base `0x13c`), size 7,032.
* **Put all four in the common tail** next to the current `0x140/0x144`:
  no `0x140` group; groups `150 160 170 180 1a8 1cc`, size 6,852.
* **Per-branch plain constants** (add `0x140..0x14c` to all five arms of the
  `0x130/0x134/0x138` chain, delete the ternaries): size 6,972, groups
  `128 144 154 17c 19c 18c 1d0 1c0` — `0x140` still not in the iPadHD block
  (the compiler leaves it for the n9/retina arms).

## 5. Conclusion / open question

The `0x140` group is not blocked by an `stmia` in the current baseline (the
`0x138/0x13c` pair is already an exact `strd`); it is blocked by the fact
that `0x140/0x144` live in the common tail while `0x148/0x14c` live in the
iPadHD block.  Making all four contiguous does create the `vst1` at `0x140`,
but the extra 16-byte group changes register pressure/scheduling and shifts
the downstream `0x170..0x1b0` group boundaries by 12 B, costing 28–120 B.
The baseline's exact 7,072 B is tied to the *scattered* placement.

A source shape that both creates `0x140` and preserves the `0x150..0x1d0`
run at exactly 7,072 B was not found.  The likely next step is the full
per-display-class if/else restructure (duplicate the whole body per branch,
as the original compiler did), which would also fix the `b0/c0`, `dc/ec`
and `108/118` deltas; that is a much larger change than a statement reorder.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10606
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10606/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5739
python3 /dev/shm/w10606/align_diff.py _ZN6LayoutC1Ev   # 1978
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10606/`: `compile.sh` (single-TU compile),
`groups_obj.py`/`size_obj.py` (group-base + size extractor),
`sweep140.py`/`sweep140b.py`/`sweep_pos.py` (permutation/position sweeps),
`try_specific.py`/`try_common.py`/`try_mix.py` (targeted variants),
`Layout_base.cpp` (baseline source), `orig_layout.s`/`ours_layout.s`
(disassembly), `orig_stores.txt`/`ours_stores.txt` (full store order).
