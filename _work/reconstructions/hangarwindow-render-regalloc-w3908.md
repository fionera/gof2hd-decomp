# `HangarWindow::render` — pass 3: 64-cell source grid, Ghidra expression order, LSDA isolation (w3908)

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave89-w3908`, base master `36c65db7`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra
`android_2.0.16_libgof2hdaa.so`, image base 0x10000). This pass is source-shape
only; no code change landed. Scratch is under `/dev/shm/w3908`.

## TL;DR

No source shape beats the current tree. Baseline is **80.6 % / 9700 B**
(raw difflib `0.8064427312775331`, 3626 vs 3638 normalized insns), gate
byte 4498 / linked 4498 / size 4524 / data 843 / imports 207, lint CLEAN,
drift 0.

New results in this pass:

1. **A Linux-host build does not reproduce the canonical gate** unless the
   darwin r18b `libc++abi.a` / `libandroid_support.a` are used. The Linux
   r18b packages carry `/buildbot/...` path strings where the original carries
   `/Volumes/Android/buildbot/...`; `.rodata` is 16 B shorter, every
   `.rodata`-relative literal shifts, and `byte_exact` falls 4498 -> 4344.
   Copying the two darwin archives into `.cache` restores 4498/4498 exactly.
2. A 64-cell grid over the six known source knobs confirms the current
   combination is the unique best cell (next best 0.79146).
3. The Ghidra expression order for the item-loop `y` and free-credits `y` /
   `baseY` was tested exactly; every faithful order is neutral or worse.
4. The residual is now localised to **two register ties inside the item
   loop** (not the prologue): the `iconSpace`/`textX` stack-slot pair is
   swapped, and the blueprint branch reloads `this`/`&Globals::layout` with
   the `ldrd` register order reversed. Both are consequences of the same
   `this` vs `&Globals::layout` tie.
5. The original's real `.ARM.extab` record for `render` is 948 B / 156 rows;
   ours is 928 B / 153 rows. The missing rows are the isolated 4-byte
   `getAmount()` call sites (original has six; ours merges them). Converting
   the second amount block from ternaries to `if/else` does **not** restore
   them (still 928 B) and costs 4 B / 1.8 points.

## 0. Reproducing the canonical gate on this Linux host

Fresh `cmake --preset default` on Linux downloads `android-ndk-r18b-linux-x86_64`.
Its `sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a` contains
`/buildbot/src/android/ndk-release-r18/...`, while the original `.rodata`
contains the darwin package's `/Volumes/Android/buildbot/...` (8 extra bytes
per path string; two strings -> 16 B). Result: `.rodata 0x41b57` instead of
`0x41b67`, and `byte_exact 4344` instead of 4498. The main repo's NDK cache
(`/home/debian/GalaxyOnFire2/gof2-decomp/.cache/ndk/android-ndk-r18b/`) has the
darwin package extracted. Copying just the two linked archives restores the
canonical build:

```sh
SRC=/home/debian/GalaxyOnFire2/gof2-decomp/.cache/ndk/android-ndk-r18b/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a
DST=.cache/ndk/android-ndk-r18b/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a
cp -p $SRC/libc++abi.a $SRC/libandroid_support.a $DST/
touch $DST/libc++abi.a $DST/libandroid_support.a
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8   # byte 4498 linked 4498
```

`.rodata` then reports `0x41b67` and the gate matches the committed baseline.
This is an environment fact, not a source change; `.cache/` is gitignored.

## 1. Baseline (canonical, this tree)

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
.text 0x1337c8 @ 0x69440, .ARM.extab 0x2628c @ 0x1a3b78, .rodata 0x41b67 @ 0x1c9e10
render: 9700/9700 B, pct 80.6, raw 0.8064427312775331
```

## 2. 64-cell grid

Six binary knobs, all measured with the official verifier
(`tools/verify/verify.py`, `autojunk=False`):

| knob | 0 | 1 |
|---|---|---|
| top `Layout *layout` local | absent (faithful lazy) | present (baseline) |
| `hintLayout` local | absent (faithful hint loop) | present (baseline) |
| free-credits order | height-first (baseline) | width-first (`getWidth` first) |
| three `description = description;` | absent | present (baseline) |
| `pad3` | absent | present (baseline) |
| `volatile float textYf` | absent | present (baseline) |

Baseline = `(1,1,0,1,1,1)`. Best cells:

| cell | size | raw |
|---|---:|---:|
| **(1,1,0,1,1,1) baseline** | **9700** | **0.8064427313** |
| (1,1,0,1,0,1) | 9692 | 0.7914600551 |
| (1,1,0,0,0,1) | 9672 | 0.7811939887 |
| (1,1,0,0,1,1) | 9680 | 0.7804878049 |
| (1,0,0,1,0,1) | 9712 | 0.7730213352 |
| (1,0,0,1,1,1) | 9716 | 0.7706068529 |
| (1,0,1,1,1,1) | 9692 | 0.6929003853 |
| (0,0,0,1,1,1) | 9712 | 0.6258073382 |
| (0,1,0,1,1,1) | 9724 | 0.6161241588 |
| (1,1,1,1,1,1) | 9744 | 0.6198540548 |

All 64 cells are worse than baseline. The grid reproduces the w3828 numbers
and shows the current anchors are jointly load-bearing.

## 3. Ghidra expression order

Ghidra `decompile_function` gives the item-loop `y` as one expression:

```c
iVar10 = (*(int *)(iVar10 + 0x70) + *(int *)(param_1 + 0x10c)) * uVar24 +
         *(int *)(param_1 + 0xb4) + *(int *)(iVar10 + 0x20) + *(int *)(iVar10 + 0xc);
```

i.e. `(layout->0x70 + this->0x100.d) * i + this->scrollOffset +
layout->0x20 + layout->0xc`. The current source keeps a `baseY` local and
adds it last. Rewriting `y` to the Ghidra association (no `baseY`) gives
**0.8056167401 / 9700** (same as the w3828 "baseY operand reordered" cell).
All other associations of `baseY` (`a`..`f` in the sweep) are <= baseline;
`baseY = (0xc + 0x20) + scrollOffset` is codegen-identical to baseline.

Free-credits `y` in Ghidra:

```c
iVar22 = (*(int *)(*piVar43 + 0x34) + *(int *)(*piVar43 + 0x30)) * iVar36 +
         iVar9 + iVar49 + iVar48 * 2;
```

Reordering the `0x34 + 0x30` pair or the `baseY` terms is codegen-neutral
(0.8064427313). Removing the `baseY` local and writing the full expression
costs 28 B and drops to 0.6233301198.

## 4. The two remaining register ties (new evidence)

### 4.1 `iconSpace` / `textX` stack slots are swapped

Original (`0x1491f4`..`0x1491fc`):

```
1491f4: adds r0, r2, r6
1491f8: str  r0, [sp, #92]   ; textX
1491fa: str  r6, [sp, #96]   ; iconSpace
```

Ours (`0x1491f6`..`0x1491fc`):

```
1491f6: adds r0, r2, r6
1491fa: str  r0, [sp, #96]   ; textX
1491fc: str  r6, [sp, #92]   ; iconSpace
```

The two locals are swapped. Declaration order (`int textX; int iconSpace;`)
and splitting `textX` into `textX = 0x2cc + 0x4c; ... textX += iconSpace;`
are either neutral or much worse (0.6702346645 / 9688 for the split form).
The slot assignment follows the live ranges, not the declaration order.

### 4.2 Blueprint branch: `ldrd` register order reversed

Original (`0x1496da`):

```
1496da: ldrd sl, r9, [sp, #132]   ; sl = &Globals::layout, r9 = this
1496e0: ldr.w r0, [sl]            ; layout
1496e6: ldr.w r6, [r9, #244]      ; this->progressBarWidth
```

Ours (`0x1496da`):

```
1496da: ldrd r9, sl, [sp, #132]   ; r9 = &Globals::layout, sl = this
1496e0: ldr.w r0, [r9]            ; layout
1496e6: ldr.w r6, [sl, #244]      ; this->progressBarWidth
```

The stack slots are identical (`sp+132 = &Globals::layout`,
`sp+136 = this`); only the register assignment is swapped. The instruction
sequence is otherwise identical. `HangarWindow *self = this;` /
`HangarWindow &self = *this;` inside the branch, a branch-local
`Layout *lay = Globals::layout;`, reordering the `buttonInsetX +
hintOffsetX` addition (3 sites, individually and together), and splitting
the `DrawImage2D` x/y into locals all regress (0.7618..0.7985). The tie is
not source-order sensitive.

### 4.3 The hint loop is already structurally exact

With `(top=1, hint=0)` the hint loop is instruction-for-instruction the
original's, including `sl = rows`, `fp = r`, `r9 = r*imgH`,
`ldr r5,[sp,#132]` + `ldr r2,[r5,#0]` per iteration. It scores 0.7706 only
because the *rest* of the function then uses `this = sl` / `&layout = fp`.
This is direct evidence that the hint-loop source shape is right and the
remaining delta is the global `this`/`&layout` tie.

## 5. LSDA: the real missing rows are the isolated `getAmount()` calls

`_work/reconstructions/exidx_table.py --diff`:

```
_ZN12HangarWindow6renderEv: orig 948 ours 928 delta -20
```

`gameresource_gen/lsda.py` shows the original's isolated 4-byte rows:

```
149656 +4 lp=14b5a2   ; getStationAmount
149660 +4 lp=14b64c   ; getAmount
149c88 +4 lp=14b5a2   ; getAmount
149e26 +4 lp=14b600   ; getStationAmount
149e30 +4 lp=14b658   ; getAmount
149f14 +4 lp=14b600   ; getAmount
```

Ours merges them, e.g. `149656 +18` for the first pair. The note's w3572
analysis already identified this. New this pass: converting the *second*
amount block from two ternaries to an explicit `if/else` (the shape Ghidra
shows) produces separate statements but still one cleanup row
(`149656 +18`, `149c88 +30`, `149e2e +18`, `149f22 +40`), leaves the
record at 928 B, and costs 4 B / 1.8 points (0.7887634261 / 9696). The
isolation therefore comes from the EH cleanup state around each call, not
from the ternary/if-else form.

## 6. Proven vs hypothesis

Proven (measured in this tree):

* The canonical gate on Linux requires the darwin r18b `libc++abi.a` +
  `libandroid_support.a`; with the Linux archives `byte_exact` is 4344.
* Baseline 0.8064427312775331 / 9700 B; all 64 grid cells are worse.
* The hint loop with `hintLayout` removed matches the original instruction
  for instruction; the remaining delta is the global `this`/`&layout` tie.
* The blueprint branch's `ldrd` register order is reversed while the stack
  slots are identical.
* The original's `render` LSDA record is 948 B / 156 rows; ours 928 / 153.
  The six isolated `getAmount()` rows are the missing structural rows.
* The second amount block as `if/else` does not restore the LSDA rows.

Hypothesis / open:

* The original source has no `hintLayout` local and no self-assigns; the
  missing 18 B of item-loop text and the 20 B of LSDA are the same
  `this`/`&layout` pressure phenomenon. A shape that spills `&layout`
  (freeing `sl` for `rows`/`width`) while keeping `this = fp` would likely
  recover both.
* The `getAmount()` isolation is an EH cleanup-scope difference. It may
  require a temporary with a destructor live across the first call, or a
  different statement scope; no plain-C++ form found so far reproduces it.

## 7. Commands

```sh
cmake --build cmake-build-match --target check -j8   # RATCHET PASS byte 4498 linked 4498
cmake --build cmake-build-match --target lint  -j8   # CLEAN
cmake --build cmake-build-match --target drift -j8   # 0 fields
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
python3 _work/reconstructions/gameresource_gen/lsda.py \
  cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
```
