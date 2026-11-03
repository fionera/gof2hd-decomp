# Section-order removal: `StatusWindow` accessor order (w3745)

Status: research note + landed source reorder, 2026-11-03. Worktree `wt/w3745`,
branch `wave87-w3745`, based on master `e3b11cfb` (`docs: lock Generator 72.9%,
createScene 88.3%, Waypoint order`). Every number below was produced by a
command run in this worktree against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's NDK r18b
clang 7.0.2 / gold 1.12.

This is the next frontier recorded in
`_work/reconstructions/waypoint_ctor_placement_w3745.md` §7: the plain
(no `--section-ordering-file`) link placed
`StatusWindow::getRelativeScrollStartPos()` at `0x158220` instead of the
original `0x1581d0`.

## TL;DR

* Baseline at this HEAD: **byte_exact 4497 / linked_exact 4497**, `.text`
  `0x1337c8 @ 0x69440`, `.ARM.exidx 0x6f70 @ 0x19cc08`, `.rodata
  0x41b67 @ 0x1c9e10`, lint `CLEAN ✓`, drift `0`. The brief's `4496` baseline
  is stale (the Waypoint pass already landed at `b4a91f8a`).
* Plain no-order baseline: **byte_exact 2866 / linked 4497**, **3508 / 4524**
  exported functions at their original address, exact-address prefix **3493**.
  First mismatch: `StatusWindow::getRelativeScrollStartPos()` original
  `0x1581d0`, ours `0x158220` (exactly `+0x50`, the size of
  `getRelativeScrollHeight`).
* Cause: `src/game/ui/StatusWindow.cpp` defined
  `getRelativeScrollHeight()` before `getRelativeScrollStartPos()`. The
  Android 2.0.16 binary's address order is the reverse, and the
  `gof2sd_1.0.1_ios` source-layout dump lists `getRelativeScrollStartPos`
  before `getRelativeScrollHeight` too.
* Fix: swap the two accessor definitions in the `.cpp`. It is a pure
  section-order change: all **56** `SHF_ALLOC` content sections of the object
  are byte-identical, and the gate `.so` is **byte-identical**.
* Gate after the move: **byte 4497 / linked 4497**, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0 (unchanged; no baseline update needed).
* Plain no-order after: **byte_exact 2866 / linked 4497**, **3510 / 4524**
  addresses, prefix **3504**. All 13 `StatusWindow` `.dynsym` rows are now
  address-exact in the plain link.
* The next plain no-order mismatch is `TargetFollowCamera::~TargetFollowCamera()`
  (`0x15a998` original, `0x15aa56` ours). That is a different TU and was not
  investigated in this pass.

## 1. Baseline and commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Plain no-order baseline (exact gold line minus `--section-ordering-file`):

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/status/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/status/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/status/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/status/base_noorder.so
```

```text
compared 4524  avg 99.91%  byte_exact 2866  linked_exact 4497  fuzzy_perfect 4497
/tmp/w3745/status/base_noorder.so: 3508/4524 same; prefix 3493;
  first (0x1581d0, '_ZN12StatusWindow25getRelativeScrollStartPosEv', 0x158220)
```

## 2. Ground truth: the original `StatusWindow` order

Original binary (`nm -nSD --defined-only`):

```text
00157d80 000002c4 T _ZN12StatusWindowC1Ev
00157d80 000002c4 T _ZN12StatusWindowC2Ev
00158044 00000120 T _ZN12StatusWindow6reInitEv
00158164 0000006a T _ZN12StatusWindowD1Ev
00158164 0000006a T _ZN12StatusWindowD2Ev
001581d0 00000030 T _ZN12StatusWindow25getRelativeScrollStartPosEv
00158200 00000050 T _ZN12StatusWindow23getRelativeScrollHeightEv
00158250 000000ca T _ZN12StatusWindow6updateEi
0015831c 00001888 T _ZN12StatusWindow4drawEv
00159ba4 0000046c T _ZN12StatusWindow16getMedalHintTextEi
0015a010 000000ac T _ZN12StatusWindow12OnTouchBeginEii
0015a0bc 00000108 T _ZN12StatusWindow11OnTouchMoveEii
0015a1c4 00000300 T _ZN12StatusWindow10OnTouchEndEii
```

The `gof2sd_1.0.1_ios` source-layout dump
(`_work/original_layout/original_layout.json`,
`Classes/Game/StatusWindow.cpp`) lists the accessors in the same order:

```text
_StatusWindow::getRelativeScrollStartPos()
_StatusWindow::getRelativeScrollHeight()
_StatusWindow::getMedalHintText(int)
...
```

(The newer `gof2hd_1.1.19_ios` dump does not contain
`getRelativeScrollStartPos` at all, so it is not evidence against the swap.)

The baseline object emitted `getRelativeScrollHeight` before
`getRelativeScrollStartPos`, exactly matching the `.cpp` definition order:

```text
StatusWindow::StatusWindow()
Array<TouchButton*>::Array()
void ArraySetLength<TouchButton*>(unsigned int, Array<TouchButton*>&)
StatusWindow::reInit()
StatusWindow::~StatusWindow()
...
StatusWindow::getRelativeScrollHeight()
StatusWindow::getRelativeScrollStartPos()
StatusWindow::update(int)
...
```

`reInit` is already emitted between the constructor and the destructor because
clang emits the forward-referenced function after its caller; only the two
accessors were out of order.

## 3. The change

Only the definition order in `src/game/ui/StatusWindow.cpp` changed:

```diff
-float StatusWindow::getRelativeScrollHeight() { ... }
-
 float StatusWindow::getRelativeScrollStartPos() {
     int range = this->scrollOffset;
     if (range > 0) return 0.0f;
     return -(float) range / (float) this->contentHeight;
 }
+
+float StatusWindow::getRelativeScrollHeight() { ... }
```

No body text changed. The object's `.text.*` header order becomes:

```text
StatusWindow::StatusWindow()
StatusWindow::reInit()
StatusWindow::~StatusWindow()
StatusWindow::getRelativeScrollStartPos()
StatusWindow::getRelativeScrollHeight()
StatusWindow::update(int)
StatusWindow::draw()
StatusWindow::getMedalHintText(int)
StatusWindow::OnTouchBegin(int, int)
StatusWindow::OnTouchMove(int, int)
StatusWindow::OnTouchEnd(int, int)
```

which matches the original address order.

## 4. Measured A/B

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `c2f65daf…` | **2866** | 3508 / 4524 | 3493 | `StatusWindow::getRelativeScrollStartPos()` `0x1581d0` vs `0x158220` |
| accessor swap (landed) | **4497 / 4497** | `c2f65daf…` | **2866** | **3510 / 4524** | **3504** | `TargetFollowCamera::~TargetFollowCamera()` `0x15a998` vs `0x15aa56` |

Landed delta: **+2 plain exported functions at their original address, +11
exact-address prefix**, gate counts unchanged (`4497 / 4497`, lint
`CLEAN ✓`, drift 0). The two plain-link gains are exactly
`getRelativeScrollStartPos` and `getRelativeScrollHeight`; there are no losses.
`byte_exact` does not move because both accessors were already byte-exact
(they contain no self-relative references) before the swap.

All 13 `StatusWindow` rows in the plain link now match the original address:

```text
00157d80 00157d80 OK _ZN12StatusWindowC1Ev
00157d80 00157d80 OK _ZN12StatusWindowC2Ev
00158044 00158044 OK _ZN12StatusWindow6reInitEv
00158164 00158164 OK _ZN12StatusWindowD1Ev
00158164 00158164 OK _ZN12StatusWindowD2Ev
001581d0 001581d0 OK _ZN12StatusWindow25getRelativeScrollStartPosEv
00158200 00158200 OK _ZN12StatusWindow23getRelativeScrollHeightEv
00158250 00158250 OK _ZN12StatusWindow6updateEi
0015831c 0015831c OK _ZN12StatusWindow4drawEv
00159ba4 00159ba4 OK _ZN12StatusWindow16getMedalHintTextEi
0015a010 0015a010 OK _ZN12StatusWindow12OnTouchBeginEii
0015a0bc 0015a0bc OK _ZN12StatusWindow11OnTouchMoveEii
0015a1c4 0015a1c4 OK _ZN12StatusWindow10OnTouchEndEii
```

## 5. Why the gate does not move

The reorder is a pure object section-header permutation:

* All **56** `SHF_ALLOC` content sections of `StatusWindow.cpp.obj` are
  byte-identical between the baseline and the reordered source (`.text.*`,
  `.ARM.exidx.*`, `.ARM.extab.*`, `.rodata.*`). Only section-header order and
  the section-index operands in relocation sections differ.
* The object has no `.rodata` content (`readelf -SW` size 0), and the gate
  `.so` is **byte-identical**: sha256
  `c2f65daf1792e05c7f31bf3119e151acae41f2836082df8b1d77b1cce194c102`
  before and after. Every ELF section size and base is unchanged
  (`.text 0x1337c8 @ 0x69440`, `.ARM.exidx 0x6f70 @ 0x19cc08`,
  `.rodata 0x41b67 @ 0x1c9e10`).
* The plain no-order link differs only in `.text` content, `.dynsym` and the
  build-id; `.rodata`, `.data`, `.data.rel.ro`, `.got` and `.plt` are
  byte-identical before and after, and the plain `.text` size is
  `0x1337e8` in both.

Because the ordering file already lists the two accessor sections in original
address order, the linked output cannot depend on the object's internal
section-header order. This is the same mechanism as the `GameRecord` and
`Waypoint` passes.

## 6. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements (before and after the source move)
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/status/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/status/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/status/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/status/base_noorder.so

python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/status/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/status/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/status/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/status/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ui/StatusWindow.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN12StatusWindow'

# gate .so A/B (baseline vs landed source)
sha256sum /tmp/w3745/status/gate_base.so cmake-build-match/libgof2hdaa.so
# both c2f65daf1792e05c7f31bf3119e151acae41f2836082df8b1d77b1cce194c102
```

## 7. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `2866 / 4497`, prefix 3493,
  3508 / 4524, first mismatch
  `StatusWindow::getRelativeScrollStartPos()` `0x1581d0` vs `0x158220`.
* The original binary's `StatusWindow` block is constructor, `reInit`,
  destructors, `getRelativeScrollStartPos`, `getRelativeScrollHeight`,
  `update`, `draw`, `getMedalHintText`, `OnTouchBegin`, `OnTouchMove`,
  `OnTouchEnd`; the `gof2sd_1.0.1_ios` dump lists the accessors in the same
  order.
* The baseline `.cpp` definition order was `getRelativeScrollHeight` before
  `getRelativeScrollStartPos`, and the object emitted that order.
* Swapping the `.cpp` definitions is a pure section-order change: all 56
  `SHF_ALLOC` content sections are byte-identical, and the gate `.so` is
  byte-identical (same sha256 before and after).
* The swap is gate-neutral in counts (`4497 / 4497`, lint `CLEAN ✓`,
  drift 0; every ELF section size/base unchanged) and improves the plain
  no-order build to `2866 / 4497`, 3510 / 4524 addresses, prefix 3504.
* The two plain-link gains are exactly the two accessor rows; no function
  lost `byte_exact` or `linked_exact`.
* The plain no-order `.rodata`, `.data`, `.data.rel.ro`, `.got` and `.plt`
  are byte-identical before and after.
* The next plain no-order frontier is
  `TargetFollowCamera::~TargetFollowCamera()` (`0x15a998` original,
  `0x15aa56` ours).

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `2866`, not `4497`.
* `TargetFollowCamera.cpp` is the next per-TU source-order experiment; its
  object currently emits the accessors before the destructor, while the
  original has the constructor, `aproximateCooefficients...`, destructor,
  then the accessors. Not tested or landed here.
