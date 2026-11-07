# `Layout::Layout` C1/C2 — wave 134 / worker w7771
## Whole-constructor outer device-tree rebuild: tested, no source win

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-134 master `bbffef482` (7072 B, unrounded `difflib` **44.958150657633 %**,
target-function differing bytes **5,784**).

## Result — no landable source win (note only)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 44.958150657633 % | **44.958150657633 %** |
| target-function differing bytes | 5,784 | **5,784** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note.

## 1. Ground truth: the top-level split is `iPadHD` / `n9` / default

The prompt's `iPadHD / iPadLarge / else` is not the top-level split.  Resolving the
GOT loads in the original prologue:

* `d1030 ldr r1,[pc,#716]` → literal `d1300 = 0x0013f356`; `add r1,pc`
  (`pc = d1034`) → **`0x21038c` = `Globals::iPadHD`**.
* `d126c ldr r1,[pc,#620]` → literal `d14dc = 0x0013f176`; `add r1,pc`
  (`pc = d1272`) → **`0x2103e8` = `Globals::n9`**.

So the original control flow is:

```
d1042..d1268  if (iPadHD) { HD arm }            b d21e2
d127e..d12fc  else if (n9) { n9 arm }           b d1860
d1570..d185e  else { default arm }              fallthrough d1860
d1860..d21e0  non-HD common (n9/retina/iPadLarge/iPad tree)
d21e2..d2bc8  all-device common tail
```

`iPadLarge` appears only inside the default/non-HD branch tree.  This confirms the
w7751 decode; the coordinator's "iPadLarge" label is the inner condition.

## 2. Variants built and scored (all real `link_so` builds)

Every variant was produced from the current `Layout.cpp` statements (same expressions,
same `* 1.40625f` / `(int)(float)` forms) and reordered into the requested shape.  The
scorer is the same unrounded `difflib` + same-address byte diff used by `tools/verify`.

| # | source shape | size | ratio % | diffbytes |
|---|---|---:|---:|---:|
| 0 | HEAD flat field-major body | **7072** | **44.9582** | **5,784** |
| 1 | flat body in iOS source order (204 stores) | 6888 | 36.0926 | 6,041 |
| 2 | device-major, HD store order + default store order | 6612 | 25.4553 | 6,241 |
| 3 | device-major, all regions in iOS source order | 6904 | 37.7990 | 5,983 |
| 4 | device-major, HD order / iOS n9 / default order | 6844 | 32.6580 | 5,983 |
| 5 | #3 + explicit `if/else` for 0x5c..0x78 | 6904 | 37.0208 | 6,012 |
| 6 | #3 + hand-transcribed Ghidra default arm (locals + ifs) | 6968 | 36.7169 | 5,920 |
| 7 | HEAD + 0x194..0x1dc ascending | 6936 | 38.8978 | 5,913 |
| 8 | HEAD + 0x140..0x1dc ascending | 6952 | 36.6118 | 5,892 |
| 9 | HEAD + `0x170` before `0x16c` | 7072 | 44.9582 | 5,784 |

No variant beats 5,784 differing bytes at 7072 B.  The device-major bodies are
**104–460 B short** of the original size, and their raw byte diff is **136–457 bytes
worse** than HEAD.

## 3. Why the outer tree does not win

1. **The default arm is the deficit.**  Original default arm `d1570..d185e` is
   750 B; the device-major rebuild's default arm is 486–550 B.  The original arm is
   scalar if-converted (`ldrb`/`cmp`/`it eq`/`moveq` + `str`) for 0x4..0x58 and
   branchy for 0x5c..0x78.  Clang 7.0.2 lowers the current nested ternaries to
   `vbsl`/`vst1.32` groups instead, which is shorter.  Variant 6 transcribes the
   Ghidra default arm with local `int` variables and explicit `if`s and recovers
   only 64 B of the 264 B gap; the compiler still re-vectorises several stores.
2. **The n9 arm matches but the common/tail split does not.**  The n9 arm is
   126 B original vs 128 B ours, but the non-HD common and tail shift: original
   `0x980` + `0x9e6`; ours `0x960` + `0x9f2` (variant 3).  The 180 B tail surplus
   partly offsets the default-arm deficit.
3. **Register allocation / literal-pool placement.**  The original keeps `iPadHD`
   in `sl`; every device-major source shape that moves the tail changes the
   prologue and the `add rX,pc` literal-pool immediates.  This is why the raw byte
   diff stays worse even when the fuzzy instruction ratio improves (variant 7:
   38.9 % vs 45.0 %).
4. **HEAD is a strong local optimum.**  The compiler already produced the outer
   `iPadHD` / `n9` / default split from the flat ternaries (HEAD's own branch
   targets: `d103e beq d1278`, `d1286 beq d1568`, `d1274 b d229c`).  Writing the
   split explicitly changes the schedule without adding the original's scalar
   default-arm code.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7771
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7771/score.py                         # 7072 / 44.958150657633 / 5784
```

Scratch harnesses (not committed, `/dev/shm/w7771/`): `score.py` (single-function
unrounded ratio + same-address raw-byte diff), `reorder_ios.py` (flat iOS order),
`build_device.py` / `build_device_ios2.py` / `build_device_mix.py` (device-major
region emitters), `default_arm.txt` (hand-transcribed Ghidra default arm),
`mkvariant.py` (replace the constructor body), `Layout.orig.cpp` (pristine HEAD
source), `Layout.asc194.cpp` / `Layout.asc140b.cpp` / `Layout.swap2.cpp` (reorder
variants).

## 5. Open questions

* The original default arm's scalar `moveq` schedule needs the source to keep the
  per-field values in **local variables** (not inline ternaries) and to store them
  in the original's `order_default.txt` order.  Variant 6 is the first step; the
  missing 200 B is the 0x5c..0x78 branch tree, which the compiler still vectorises.
* The original non-HD common arm's `vbsl` retina selection is already reproduced
  by HEAD; the remaining tail difference is the reward-box / `0x3e0..0x3fc` block.
* A hybrid that keeps HEAD's flat body for the tail but uses the Ghidra default arm
  for 0x4..0x78/0x2c0..0x2cc is the next lead; it must pay for the default arm's
  extra bytes without changing the literal-pool layout.
