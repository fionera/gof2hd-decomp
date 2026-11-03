# Section-order removal: `Waypoint` constructor placement (w3745)

Status: research note + landed source reorder, 2026-11-03. Worktree `wt/w3745`,
branch `wave87-w3745`, based on master `c05123d3` (`docs: lock GameRecord
emission order`). Every number below was produced by a command run in this
worktree against `../_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's
NDK r18b clang 7.0.2 / gold 1.12.

This is the next frontier recorded in
`_work/reconstructions/gamerecord_ctor_placement_w3745.md` §6: the plain
(no `--section-ordering-file`) link placed `Waypoint::Waypoint(int,int,int,
Route*)` at `0x157c9c` instead of the original `0x157c54`.

## TL;DR

* Baseline at this HEAD: **byte_exact 4497 / linked_exact 4497**, `.text`
  `0x1337c8 @ 0x69440`, lint `CLEAN ✓`, drift `0`. The brief's `4496` baseline
  is stale.
* Plain no-order baseline: **byte_exact 2864 / linked 4497**, **3499 / 4524**
  exported functions at their original address, exact-address prefix **3479**.
  First mismatch: `Waypoint::Waypoint(int,int,int,Route*)` original `0x157c54`,
  ours `0x157c9c`.
* Cause: `src/game/world/Waypoint.cpp` defined the accessors first and the
  constructor/destructor last. Both the Android 2.0.16 binary's address order
  and the iOS 1.1.19 source-layout dump place the constructor first, then the
  destructors, then `getPosition`, `setActive`, `reached`, `reset`.
* Fix: reorder the `.cpp` definitions to exactly that order. It is a pure
  section-order change: all 40 `SHF_ALLOC` content sections of the object are
  byte-identical, and the gate `.so` is **byte-identical**.
* Gate after the move: **byte 4497 / linked 4497**, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0.
* Plain no-order after: **byte_exact 2866 / linked 4497**, **3508 / 4524**
  addresses, prefix **3493**. All nine `Waypoint` `.dynsym` rows are now
  address- and size-exact in the plain link.
* The next plain no-order mismatch is `StatusWindow::getRelativeScrollStartPos()`
  (`0x1581d0` original, `0x158220` ours).

## 1. Baseline and commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Plain no-order baseline (exact gold line minus `--section-ordering-file`):

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/waypoint/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/waypoint/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/waypoint/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/waypoint/base_noorder.so
```

```text
compared 4524  avg 99.9%  byte_exact 2864  linked_exact 4497  fuzzy_perfect 4497
/tmp/w3745/waypoint/base_noorder.so: 3499/4524 same; prefix 3479;
  first (0x157c54, '_ZN8WaypointC1EiiiP5Route', 0x157c9c)
```

## 2. Ground truth: the original `Waypoint` order

Original binary (`nm -nSD --defined-only`):

```text
00157c54 000000d0 T _ZN8WaypointC1EiiiP5Route
00157c54 000000d0 T _ZN8WaypointC2EiiiP5Route
00157d24 00000004 T _ZN8WaypointD1Ev
00157d24 00000004 T _ZN8WaypointD2Ev
00157d28 00000010 T _ZN8WaypointD0Ev
00157d38 00000026 T _ZN8Waypoint11getPositionEv
00157d5e 00000006 T _ZN8Waypoint9setActiveEb
00157d64 0000000a T _ZN8Waypoint7reachedEv
00157d6e 00000010 T _ZN8Waypoint5resetEv
```

The iOS 1.1.19 source-layout dump
(`_work/original_layout/original_layout.json`, `Classes/Game/Waypoint.cpp`)
lists the same order: constructor, constructor, destructor, destructor,
`getPosition`, `setActive`, `reached`, `reset`.

Baseline source order in this tree was:

```text
setActive, reached, getPosition, reset, ctor, dtor
```

and the baseline object emitted exactly that (plus the implicit `D0` after
`D2`):

```text
setActive, reached, getPosition, reset, C2, D2, D0, KIPlayer weak copies
```

## 3. The change

Only the definition order in `src/game/world/Waypoint.cpp` changed:

```diff
-void Waypoint::setActive(bool active) { ... }
-void Waypoint::reached() { ... }
-Vector Waypoint::getPosition() { ... }
-void Waypoint::reset() { ... }
-Waypoint::Waypoint(...) { ... }
-Waypoint::~Waypoint() { }
+Waypoint::Waypoint(...) { ... }
+Waypoint::~Waypoint() { }
+Vector Waypoint::getPosition() { ... }
+void Waypoint::setActive(bool active) { ... }
+void Waypoint::reached() { ... }
+void Waypoint::reset() { ... }
```

No body text changed. The object's `.text.*` header order becomes:

```text
Waypoint::Waypoint(int, int, int, Route*)
Waypoint::~Waypoint()          # D2
Waypoint::~Waypoint()          # D0
Waypoint::getPosition()
Waypoint::setActive(bool)
Waypoint::reached()
Waypoint::reset()
KIPlayer::initPush(...)        # weak copies, discarded by the link
...
```

which matches the original address order.

## 4. Measured A/B

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `b237d806…` | **2864** | 3499 / 4524 | 3479 | `Waypoint::Waypoint` `0x157c54` vs `0x157c9c` |
| minimal: ctor first, rest unchanged | **4497 / 4497** | `b237d806…` | **2866** | 3501 / 4524 | 3481 | `Waypoint::~Waypoint()` `0x157d24` vs `0x157d6a` |
| full reorder (landed) | **4497 / 4497** | `b237d806…` | **2866** | **3508 / 4524** | **3493** | `StatusWindow::getRelativeScrollStartPos()` `0x1581d0` vs `0x158220` |

Landed delta: **+2 plain byte_exact, +9 exported functions at their original
address, +14 exact-address prefix**, gate counts unchanged (`4497 / 4497`,
lint `CLEAN ✓`, drift 0). The nine plain-link gains are exactly the nine
`Waypoint` rows (C1, C2, D1, D2, D0, `getPosition`, `setActive`, `reached`,
`reset`); there are no losses.

The minimal row shows that moving only the constructor to the top is enough
to make the two constructor aliases exact (and is worth the same +2
`byte_exact`), but the full reorder is strictly better in the address metric
because it also makes the destructors and accessors exact.

## 5. Why the gate does not move

The reorder is a pure object section-header permutation:

* All **40** `SHF_ALLOC` content sections of `Waypoint.cpp.obj` are
  byte-identical between the baseline and the reordered source (`.text.*`,
  `.ARM.exidx.*`, `.ARM.extab.*`, `.rodata.*`). Only section-header order and
  the section-index operands in relocation sections differ.
* The gate `.so` is **byte-identical**: sha256
  `b237d80674aa9d27b4c5e9aa29f6a20fb1a50bdb8bc064a4648ad7b790166e37`
  before and after. Every ELF section size and base is unchanged.
* The plain no-order `.rodata` is byte-identical before and after
  (`objcopy --only-section=.rodata` + `cmp`).

Because the ordering file already lists the `Waypoint` sections in original
address order, the linked output cannot depend on the object's internal
section-header order. This is the same mechanism as the `GameRecord` pass.

## 6. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements (before and after the source move)
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/waypoint/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/waypoint/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/waypoint/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/waypoint/base_noorder.so

python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/waypoint/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/waypoint/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/waypoint/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/waypoint/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/Waypoint.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN8Waypoint'

# gate .so A/B (baseline vs landed source)
sha256sum /tmp/w3745/waypoint/gate_base.so /tmp/w3745/waypoint/gate_after.so
# both b237d80674aa9d27b4c5e9aa29f6a20fb1a50bdb8bc064a4648ad7b790166e37
```

## 7. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `2864 / 4497`, prefix 3479,
  3499 / 4524, first mismatch `Waypoint::Waypoint` `0x157c54` vs `0x157c9c`.
* The original binary's `Waypoint` block is constructor, destructors,
  `getPosition`, `setActive`, `reached`, `reset`; the iOS 1.1.19 dump lists
  the same order.
* The baseline `.cpp` definition order was accessors first, constructor and
  destructor last, and the object emitted that order.
* Reordering the `.cpp` definitions to the original order is a pure
  section-order change: all 40 `SHF_ALLOC` content sections are byte-identical,
  and the gate `.so` is byte-identical (same sha256 before and after).
* The reorder is gate-neutral in counts (`4497 / 4497`, lint `CLEAN ✓`,
  drift 0; every ELF section size/base unchanged) and improves the plain
  no-order build to `2866 / 4497`, 3508 / 4524 addresses, prefix 3493.
* The nine plain-link gains are exactly the nine `Waypoint` rows; no function
  lost `byte_exact` or `linked_exact`.
* The plain-link `.rodata` is byte-identical before and after.
* The next plain no-order frontier is `StatusWindow::getRelativeScrollStartPos()`
  (`0x1581d0` original, `0x158220` ours). `StatusWindow.cpp.obj` currently
  emits `getRelativeScrollHeight` before `getRelativeScrollStartPos`; the
  original address order is the reverse.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `2866`, not `4497`.
* The `StatusWindow` accessor swap is the obvious next per-TU source-order
  experiment, but it was not tested or landed in this pass.
