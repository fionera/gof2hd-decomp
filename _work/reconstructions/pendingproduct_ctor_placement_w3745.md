# Section-order removal: `PendingProduct` ctor placement (w3745)

Status: research note + landed pass, 2026-11-03. Worktree `wt/w3745`, branch
`wave87-w3745`, based on master `4a25b7a2`. Every number below was produced by
a command run in this worktree against
`../_work/bins/android_2.0.16_libgof2hdaa.so` (NDK r18b clang 7.0.2 / gold 1.12).

This is the next frontier recorded in
`_work/reconstructions/menutouchwindow_dtor_weakcopy_w3745.md` §6: the plain
(no `--section-ordering-file`) link placed
`PendingProduct::PendingProduct(BluePrint*)` at `0x12d400` instead of the
original `0x12d3c8`.

## 1. Baseline

The brief's `4496` baseline is stale; this HEAD is `4497 / 4497`.

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
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (`readelf -SW cmake-build-match/libgof2hdaa.so`):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78   (original 0x26298; known -12)
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

Gate `.so` sha256: `ca6ac0da5846cef5d57f36028987c2b327d14437a4f062c306524b35b2e91a9e`.

Plain no-order baseline (`relink_noorder.py` + `verify.py` + `addr_match.py`,
commands in §5):

```text
compared 4524  avg 99.9%  byte_exact 3093  linked_exact 4497  fuzzy_perfect 4497
3378/4524 same; prefix 3066;
first (0x12d3c8, '_ZN14PendingProductC1EP9BluePrint', 0x12d400)
```

## 2. Original object order vs baseline object order

The original's own `.dynsym` address order for the block is the authority:

```text
0x12d3c8  PendingProduct::PendingProduct(BluePrint*)        # C1/C2, 128 B
0x12d448  PendingProduct::PendingProduct(int, String, int, int)  # C1/C2, 56 B
0x12d480  PendingProduct::~PendingProduct()                 # D1/D2, 4 B
0x12d484  Radar::Radar(Level*)                              # next TU/function
```

The HD source-layout dump independently lists the same order
(`_work/original_layout/original_layout.json`,
`gof2hd_1.1.19_ios` -> `Classes/Game/PendingProduct.cpp`):

```text
PendingProduct::PendingProduct(BluePrint*)
PendingProduct::PendingProduct(BluePrint*)
PendingProduct::PendingProduct(int, AbyssEngine::String, int, int)
PendingProduct::PendingProduct(int, AbyssEngine::String, int, int)
PendingProduct::~PendingProduct()
```

The baseline `PendingProduct.cpp.obj` `.text.*` order was the .cpp definition
order, i.e. the reverse of the constructor pair:

```text
PendingProduct::PendingProduct(int, AbyssEngine::String, int, int)  # 56 B
PendingProduct::PendingProduct(BluePrint*)                          # 128 B
PendingProduct::~PendingProduct()                                   # 4 B
```

so the plain no-order link put the `BluePrint*` ctor at `0x12d400` (after the
56-byte int ctor) instead of `0x12d3c8`. The first plain no-order mismatch was
exactly:

```text
original 0x12d3c8 PendingProduct::PendingProduct(BluePrint*)
ours     0x12d400 PendingProduct::PendingProduct(BluePrint*)
```

The header already declared the `BluePrint*` ctor first
(`src/game/mission/PendingProduct.h`); only the .cpp definition order was
inverted.

## 3. Landed source change

Swap the two constructor definitions in
`src/game/mission/PendingProduct.cpp`. No body was edited.

```diff
@@
+PendingProduct::PendingProduct(BluePrint *bp) {
+    this->blueprintIndex = bp->getIndex();
+    this->stationName = bp->getStationName();
+    this->stationIndex = bp->getStationIndex();
+    this->quantity = bp->getQuantity();
+}
+
 PendingProduct::PendingProduct(int blueprintIndex, String stationName,
                                int stationIndex, int quantity) {
     this->blueprintIndex = blueprintIndex;
@@
     this->quantity = quantity;
 }
 
-PendingProduct::PendingProduct(BluePrint *bp) {
-    this->blueprintIndex = bp->getIndex();
-    this->stationName = bp->getStationName();
-    this->stationIndex = bp->getStationIndex();
-    this->quantity = bp->getQuantity();
-}
-
 PendingProduct::~PendingProduct() {
 }
```

The resulting object order now matches the original address order exactly:

```text
PendingProduct::PendingProduct(BluePrint*)                          # 0x12d3c8
PendingProduct::PendingProduct(int, AbyssEngine::String, int, int)  # 0x12d448
PendingProduct::~PendingProduct()                                   # 0x12d480
```

`readelf -SW` on the object confirms the `.text.*` section order:

```sh
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/mission/PendingProduct.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt
```

## 4. Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `ca6ac0da…` | **3093** | 3378 / 4524 | 3066 | `PendingProduct::PendingProduct(BluePrint*)` `0x12d3c8` vs `0x12d400` |
| ctor swap (landed) | **4497 / 4497** | `0f27c0e1…` | **3097** | **3382 / 4524** | **3081** | `Radar::getPlanetDockIndex()` `0x130974` vs `0x131004` |

Landed delta: **+4 plain byte_exact, +15 exact-address prefix, +4 exported
functions at their original address**, gate unchanged (`4497 / 4497`, lint
`CLEAN ✓`, drift 0).

The whole `PendingProduct` block is now address-exact in the plain no-order
link:

```text
orig 0x12d3c8  PendingProduct::PendingProduct(BluePrint*)        == ours
orig 0x12d448  PendingProduct::PendingProduct(int, String, int, int) == ours
orig 0x12d480  PendingProduct::~PendingProduct()                 == ours
```

Both constructor pairs (`C1`/`C2`) also became `byte_exact` in the plain link
(they were `linked_exact` but not `byte_exact` before, because the 128-byte
`BluePrint*` ctor was at the wrong address). The destructor remains
`linked_exact` but not `byte_exact`: it is the same 4-byte tail call in both
binaries, but the local interworking-veneer target differs by `0x20`
(`0x19be88` original vs `0x19bea8` ours):

```text
orig 12d480: f06e bd02  b.w 0x19be88
ours 12d480: f06e bd12  b.w 0x19bea8
```

`Status::addPendingProduct` is likewise `linked_exact` but not `byte_exact`;
its only difference is the same class of local tail-call target
(`0x19bef8` original vs `0x19bf18` ours). Neither is a `PendingProduct`
placement problem.

### Gate `.so` side effects

Section sizes and bases are unchanged, but the gate `.so` is not byte-identical
to the baseline: 66 differing bytes.

| range | bytes | what |
|---|---:|---|
| `0x1dc..0x1ef` | 20 | `.note.gnu.build-id` |
| `0x1a1074`, `0x1a107c` | 1 each | `.ARM.exidx` words (PendingProduct records) |
| `0x1c1ec7..0x1c1efd` | 44 | `.ARM.extab` record bytes |

`.text` (0 differing bytes), `.rodata` (0), `.data` (0), `.data.rel.ro` (0)
are unchanged. The `.ARM.extab` differing-byte count against the original is
unchanged at `113810`; `.text` against the original is unchanged at `111719`
and `.ARM.exidx` at `1744`.

## 5. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/pp/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/pp/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/pp/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/pp/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/mission/PendingProduct.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -i PendingProduct

# gate .so A/B (baseline vs landed source)
sha256sum /tmp/w3745/pp/gate_base.so /tmp/w3745/pp/gate_after.so
cmp -l /tmp/w3745/pp/gate_base.so /tmp/w3745/pp/gate_after.so | wc -l   # 66
```

## 6. Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3093 / 4497`, prefix 3066,
  3378 / 4524, first mismatch `PendingProduct::PendingProduct(BluePrint*)`
  `0x12d3c8` vs `0x12d400`.
* The original emits the `BluePrint*` ctor at `0x12d3c8`, the
  `(int, String, int, int)` ctor at `0x12d448`, and the destructor at
  `0x12d480`; the HD source-layout dump independently lists the same order.
* The baseline `PendingProduct.cpp.obj` emitted the two constructors in the
  opposite order because the .cpp definitions were in the opposite order from
  the header and the original TU.
* Swapping the two definitions makes the object's `.text.*` order match the
  original address order exactly.
* The swap is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0; every ELF
  section size/base unchanged; `.text` and `.rodata` byte-identical to the
  baseline gate) and improves the plain no-order build to `3097 / 4497`,
  prefix 3081, 3382 / 4524.
* Both `PendingProduct` constructor pairs (`C1`/`C2`) are now `byte_exact` in
  the plain no-order link; the whole block is address-exact.
* The remaining `PendingProduct::~PendingProduct` and
  `Status::addPendingProduct` `byte_exact` misses are local tail-call veneer
  address differences (`0x19be88` vs `0x19bea8`, `0x19bef8` vs `0x19bf18`),
  not placement differences.
* The next plain no-order frontier is `Radar::getPlanetDockIndex()`
  (`0x130974` original, `0x131004` ours); `Radar.cpp.obj` emits
  `drawCurrentLock` before `getPlanetDockIndex`, while the original emits
  `getPlanetDockIndex` first.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `3097`, not `4497`.
* The `Radar` reorder is the obvious next per-TU source-order experiment, but
  it was not tested or landed in this pass.
