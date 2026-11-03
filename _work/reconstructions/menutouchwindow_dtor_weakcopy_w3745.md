# Section-order removal: `MenuTouchWindow` dtor weak copy (w3745)

Status: research note + landed pass, 2026-11-03. Worktree `wt/w3745`, branch
`wave87-w3745`, based on master `dbee2a82`. Every number below was produced by a
command run in this worktree against
`../_work/bins/android_2.0.16_libgof2hdaa.so` (NDK r18b clang 7.0.2 / gold 1.12).

This is the next frontier recorded in
`_work/reconstructions/section_order_removal_w3745.md` §13.4: the plain
(no `--section-ordering-file`) link placed an extra
`Array<GameRecord*>::~Array()` weak copy at the original
`MenuTouchWindow::~MenuTouchWindow()` address.

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

Gate `.so` sha256: `d61445d5e77792199a94c360745efea80e88c37c7277f05eaefd09e590153d00`.

Plain no-order baseline (`relink_noorder.py` + `verify.py` + `addr_match.py`,
commands in §5):

```text
compared 4524  avg 99.9%  byte_exact 3088  linked_exact 4497  fuzzy_perfect 4497
3373/4524 same; prefix 3032;
first (0x125170, '_ZN15MenuTouchWindowD1Ev', 0x125186)
```

## 2. Original object order vs baseline object order

The original's own `.dynsym` address order for the block is the authority:

```text
0x12299c  MenuTouchWindow::MenuTouchWindow(int)                  # C1/C2
0x124f1c  MenuTouchWindow::loadPreviewRecords()
0x124fd8  MenuTouchWindow::addButton(...)
0x12510c  ArrayAdd<TouchButton*>
0x125130  Array<TouchSlider*>::Array()
0x12514c  ArrayAdd<TouchSlider*>
0x125170  MenuTouchWindow::~MenuTouchWindow()                    # D1/D2
0x12538a  ArrayReleaseClasses<TouchSlider*>
0x1253da  Array<TouchSlider*>::~Array()
0x1253f0  Array<GameRecord*>::~Array()
0x125406  MenuTouchWindow::isInMissionScreen()
```

The baseline `MenuTouchWindow.cpp.obj` `.text.*` order was:

```text
MenuTouchWindow::MenuTouchWindow(int)
Array<TouchButton*>::Array()          # discarded: defined at 0xd7004
MenuTouchWindow::loadPreviewRecords()
MenuTouchWindow::addButton(...)
ArrayAdd<TouchButton*>                # 0x12510c
ArraySetLength<TouchButton*>          # discarded
Array<TouchSlider*>::Array()          # 0x125130
ArrayAdd<TouchSlider*>                # 0x12514c
Array<GameRecord*>::~Array()          # <-- emitted here, 0x125170 in plain link
MenuTouchWindow::~MenuTouchWindow()   # 0x125186 in plain link
ArrayReleaseClasses<TouchButton*>     # discarded: defined at 0xd923c
__clang_call_terminate
Array<TouchButton*>::~Array()         # discarded: defined at 0xd928c
ArrayReleaseClasses<TouchSlider*>
Array<TouchSlider*>::~Array()
ArrayReleaseClasses<AbyssEngine::String*>  # discarded
Array<AbyssEngine::String*>::~Array()      # discarded
MenuTouchWindow::isInMissionScreen()
...
```

`Array<GameRecord*>::~Array()` is the only weak copy in this block that is
defined by `MenuTouchWindow.cpp.obj` (checked over every object in
`link_order.rsp`), so it is the only one whose placement can move the dtor.
The original emits it after the dtor and after `Array<TouchSlider*>::~Array()`;
the baseline emits it immediately before the dtor.

## 3. Landed source change

Move the `MenuTouchWindow::~MenuTouchWindow()` definition from after
`addButton()` to before `loadPreviewRecords()` in
`src/game/ui/MenuTouchWindow.cpp`. No body was edited.

```diff
@@ before loadPreviewRecords()
+MenuTouchWindow::~MenuTouchWindow() {
+    ...  (unchanged body)
+}
+
 void MenuTouchWindow::loadPreviewRecords() {
 ...
 void MenuTouchWindow::addButton(...) {
 ...
-MenuTouchWindow::~MenuTouchWindow() {
-    ...  (unchanged body)
-}
-
 bool MenuTouchWindow::isInMissionScreen() {
```

The constructor forward-references `loadPreviewRecords()` and `addButton()`, so
clang still emits those two immediately after the constructor. The destructor
now sits at its source position after them, and clang flushes the deferred
`Array<GameRecord*>::~Array()` instantiation after the destructor's own weak
copies. The resulting object order is:

```text
MenuTouchWindow::MenuTouchWindow(int)
Array<TouchButton*>::Array()          # discarded
MenuTouchWindow::loadPreviewRecords()
MenuTouchWindow::addButton(...)
ArrayAdd<TouchButton*>                # 0x12510c
ArraySetLength<TouchButton*>          # discarded
Array<TouchSlider*>::Array()          # 0x125130
ArrayAdd<TouchSlider*>                # 0x12514c
MenuTouchWindow::~MenuTouchWindow()   # 0x125170
ArrayReleaseClasses<TouchButton*>     # discarded
__clang_call_terminate
Array<TouchButton*>::~Array()         # discarded
ArrayReleaseClasses<TouchSlider*>     # 0x12538a
Array<TouchSlider*>::~Array()         # 0x1253da
ArrayReleaseClasses<AbyssEngine::String*>  # discarded
Array<AbyssEngine::String*>::~Array()      # discarded
Array<GameRecord*>::~Array()          # 0x1253f0
MenuTouchWindow::isInMissionScreen()  # 0x125406
...
```

The surviving order now matches the original address order exactly. The
discarded weak copies are the same ones the baseline already discarded (gold
keeps the first definition in link order).

## 4. Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `d61445d5…` | **3088** | 3373 / 4524 | 3032 | `MenuTouchWindow::~MenuTouchWindow` `0x125170` vs `0x125186` |
| dtor moved (landed) | **4497 / 4497** | `ca6ac0da…` | **3093** | **3378 / 4524** | **3066** | `PendingProduct::PendingProduct(BluePrint*)` `0x12d3c8` vs `0x12d400` |

Landed delta: **+5 plain byte_exact, +34 exact-address prefix, +5 exported
functions at their original address**, gate unchanged (`4497 / 4497`, lint
`CLEAN ✓`, drift 0).

The whole `MenuTouchWindow` block is now address-exact in the plain no-order
link:

```text
orig 0x125170  MenuTouchWindow::~MenuTouchWindow()   == ours
orig 0x12538a  ArrayReleaseClasses<TouchSlider*>     == ours
orig 0x1253da  Array<TouchSlider*>::~Array()         == ours
orig 0x1253f0  Array<GameRecord*>::~Array()          == ours
orig 0x125406  MenuTouchWindow::isInMissionScreen()  == ours
```

### Gate `.so` side effects

Section sizes and bases are unchanged, but the gate `.so` is not byte-identical
to the baseline: 66 differing bytes.

| range | bytes | what |
|---|---:|---|
| `0x1dc..0x1ef` | 20 | `.note.gnu.build-id` |
| `0x1a0f8c`, `0x1a0f94`, `0x1a0f9c`, `0x1a0fa4` | 4 | `.ARM.exidx` words (MenuTouchWindow records) |
| `0x1c17f8..0x1c182b` | 42 | `.ARM.extab` record bytes |

The `.ARM.extab` differing-byte count against the original moves
`113807 -> 113810` (+3; not gate-tracked). `.text` (111719), `.ARM.exidx`
(1744), `.rel.plt` (21), `.dynsym` (324), `.dynstr` (1268), `.hash` (133),
`.rel.dyn` (74) and the build-id (20) differing-byte counts are unchanged.
The +3 is `.ARM.extab` record ordering/content for the moved weak copy, not a
gate regression.

## 5. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/mtw/final_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/mtw/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/mtw/final_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/mtw/final_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ui/MenuTouchWindow.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | awk '$1 >= "0012299c" && $1 <= "0012d0cc"'

# gate .so A/B (baseline vs landed source)
sha256sum /tmp/w3745/gate_base.so /tmp/w3745/mtw/gate_after.so
cmp -l /tmp/w3745/gate_base.so /tmp/w3745/mtw/gate_after.so | wc -l   # 66
```

## 6. Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3088 / 4497`, prefix 3032,
  3373 / 4524, first mismatch `MenuTouchWindow::~MenuTouchWindow`
  `0x125170` vs `0x125186`.
* `MenuTouchWindow.cpp.obj` is the only object in `link_order.rsp` that defines
  `Array<GameRecord*>::~Array()`; in the baseline it emits that weak copy
  before the dtor, while the original emits it at `0x1253f0`, after the dtor
  and after `Array<TouchSlider*>::~Array()`.
* Moving the dtor definition before `loadPreviewRecords()` makes clang emit the
  weak copy after the dtor, reproducing the original object order for every
  surviving section in the block.
* The move is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0; every ELF
  section size/base unchanged) and improves the plain no-order build to
  `3093 / 4497`, prefix 3066, 3378 / 4524.
* The whole `MenuTouchWindow` block is address-exact in the plain no-order
  link (`0x125170`, `0x12538a`, `0x1253da`, `0x1253f0`, `0x125406`).
* The next plain no-order frontier is
  `PendingProduct::PendingProduct(BluePrint*)` (`0x12d3c8` original,
  `0x12d400` ours).

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `3093`, not `4497`.
* The `.ARM.extab` +3 differing-byte side effect is attributable to the moved
  weak copy's LSDA record order; it is not gate-tracked and the section size is
  unchanged. A different source shape that emits the same `.text` order with a
  different extab order was not tested.
