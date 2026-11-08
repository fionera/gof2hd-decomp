# `Layout::Layout` C1/C2 — wave 144 / worker w8909
## The `clz` mask is a *broadcast scalar i1*; a shared `retina` local reproduces it but shrinks the ctor by 120–192 B. No size-neutral clz shape found (raw diff stays 5,704 @ 7,072 B).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Baseline = wave-143 master `f5093474f`
(target-function raw same-address diff **5,704**, `.text` differing bytes 85,851).

## 1. Ground truth: the two mask idioms

Original (default-arm/common-tail, `d1ac6..`):
```
clz   r1, r8            ; r8 = Globals::retinaDisplay value (32-bit zero-extended)
cmp.w r8, #0            ; same r8 reused by every scalar select in the tail
mov.w r1, r1, lsr #5    ; r1 = (r8 == 0)
vdup.16 d24, r1
vmovl.u16 q12, d24
vshl.s32 q12, q12, #31
vneg.s32 q13, q11       ; q13 = -31
vshl.s32 q13, q12, q13  ; 0xFFFFFFFF where r8 == 0
```
Ours (wave-143 baseline):
```
vdup.16 d16, ip         ; ip = retina value
vmov.i32 q10, #24
vbic.i16 d16, #0xff00
vceq.i16 d16, d16, #0  ; 0xFFFF where ip == 0
vmovl.u16 q11, d16
vshl.i32 q11, q11, #24
vneg.s32 q10, q10       ; q10 = -24
vshl.s32 q10, q11, q10
```
Both broadcast a 16-bit value; the difference is **where the `== 0` happens**: the original
does it *scalar* on the i32 register (`clz`), ours does it *in-vector* (`vceq.i16`).

## 2. NEW finding: the idiom is decided by IR shape, not by the C++ type

Isolated clang-7.0.2 experiments (`-Oz -g`, same flags):

| source | IR | asm |
|---|---|---|
| `rd ? A : B` ×4, `rd` a global `unsigned char` | one `icmp eq i8`, `insertelement <4 x i1>` broadcast | **clz** |
| `unsigned char v = rd; v ? A : B` ×4 | one `icmp ne i8`, broadcast | `cmp/movne` (not clz) |
| `unsigned char v = rd; (v==0) ? A : B` ×4 | one `icmp eq i8`, broadcast | **clz** |
| `bool v = rd; v ? A : B` ×4 | `zext i1`, broadcast | `cmp/movne` |
| `int v = rd; v ? A : B` ×4 | `icmp ne i32`, broadcast | `cmp/movne` |

So **clz == a single shared `icmp eq i8` broadcast to the 4 select lanes**.  The original's
`clz r1, r8` is reused by all seven `vbsl` groups, i.e. the original has ONE shared i8 compare
for the whole common tail.

Our baseline IR (`Layout.ll`) instead has **47 distinct `icmp eq i8 %235, 0`** in one basic
block (`%235` = the `volatile` load from `*rd`).  The SLP group members are non-adjacent in
source (`0xb0`=L71, `0xb4`=L77, `0xb8`=L76, `0xbc`=L81), so each select carries a different
icmp; the vectorizer therefore emits `insertelement <4 x i8>` + `icmp eq <4 x i8>` → `vceq.i16`.
The icmp sharing in our IR is in runs of ~3 consecutive source statements, never 4.

## 3. Reproducing clz — measured (real `link_so`)

Introduce `unsigned char retina = Globals::retinaDisplay;` and use `retina` for the group
(lines 71..115):

| variant | size | raw diff | mask |
|---|---:|---:|---|
| HEAD (wave-143) | **7072** | **5704** | 3×vceq |
| local `unsigned char retina`, first 4 stmts | 6952 | **5662** | **0×vceq, 5×clz** |
| local `unsigned char retina`, 8 stmts | 6900 | 5809 | clz |
| local `unsigned char retina`, 16 stmts | 6980 | 5754 | clz |
| local `unsigned char retina`, 45 stmts (whole group) | 6880 | 5769 | clz |
| local `int retina`, 4 stmts | 6952 | 5662 | clz |
| local `bool retina`, 45 stmts | 7084 | 5972 | cmp/movne |
| `Globals::retinaDisplay == 0 ?`, 45 stmts | 7056 | 5887 | still 3×vceq |
| `!Globals::retinaDisplay ?`, 45 stmts | 7056 | 5887 | still 3×vceq |
| `3*1.40625f`→`4` on L71 (diagnostic) | 7084 | 6216 | clz |

The **first-4-statements local is the best mask fix**: it removes all `vceq` and gives
`diff 5662` (−42 vs 5,704), but the ctor shrinks to **6952 B**.

## 4. Why it cannot land yet: the 120–192 B hole

The original is 7072 B *with* the clz mask, so it carries ~120–192 B more code than any
clz-producing shape we can build.  Random move-to-position climbs over the group with the
local `retina` in place never reached 7072 (140+140 builds; range 6776..7004).  A size change
of even 120 B shifts every function after `Layout` and drops `byte_exact` 4507 → 2455
(linked_exact stays 4507), so it cannot be committed without the compensating tail.

The missing code is the "device-major shared tail" already flagged in wave 143: the original
has **10 `vbsl`** groups and 4 `clz` masks in `d1028..d2bc8`, ours has 6 `vbsl` and 5 `clz`.
Reconstructing the extra `vbsl` groups (original `d20b4`, `d22f8` clz sites) is the
prerequisite for landing the clz mask at 7,072 B.

## 5. Not landed

No source change (baseline `5704 @ 7072` kept, gate green).  The one mask-faithful variant is
`unsigned char retina` for the first four statements (`diff 5662`, `0×vceq`, `5×clz`,
`6952 B`) — **size-negative**, so it is recorded here rather than committed.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8909
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 fdiff.py _ZN6LayoutC1Ev          # 7072/7072, diff 5704
```
Scratch (`/dev/shm/w8909/`): `Layout.base.cpp`, `tryv.py` (build+score), `search*.py`
(size-7072 / clz searches), `regdiff.py` (per-128-byte diff), `Layout.ll` / `Layout.noslp2.ll`
(IR evidence), `base2.so` / `local2.so` (baseline vs clz-local disassembly).

## 7. Open questions

* Reconstruct the extra `vbsl` groups at original `d20b4` / `d22f8` so the ~120–192 B saved by
  the shared `retina` compare is compensated and the ctor stays 7,072 B.
* Why does clang-7 not CSE the 47 identical `icmp eq i8 %235, 0` in one block?  A minimal
  47-select repro (`td.cpp`) *does* CSE to one icmp; the Layout block has `or i8 %235` uses in
  other blocks and a `volatile` load, which is the only structural difference.  If the icmps
  were CSE'd the baseline would already emit clz at 7,072 B.
