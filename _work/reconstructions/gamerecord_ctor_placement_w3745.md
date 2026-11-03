# Section-order removal: `GameRecord` ctor placement (w3745)

Status: research note + landed source move, 2026-11-03. Worktree `wt/w3745`,
branch `wave87-w3745`, based on master `e770f3c6` (`docs: lock createMission
90.69% and SolarSystem order`). Every number below was produced by a command
run in this worktree against `../_work/bins/android_2.0.16_libgof2hdaa.so`
with the tree's NDK r18b clang 7.0.2 / gold 1.12.

This is the next frontier recorded in
`_work/reconstructions/solarsystem_emission_order_w3745.md` §7: the plain
(no `--section-ordering-file`) link placed `GameRecord::GameRecord()` at
`0x15591a` instead of the original `0x155900`.

## TL;DR

* Baseline at this HEAD: **byte_exact 4497 / linked_exact 4497**, `.text`
  `0x1337c8 @ 0x69440`, lint `CLEAN ✓`, drift `0`. The brief's `4496` baseline
  is stale.
* Plain no-order baseline: **byte_exact 2860 / linked 4497**, **3495 / 4524**
  exported functions at their original address, exact-address prefix **3410**.
  First mismatch: `GameRecord::GameRecord()` original `0x155900`, ours
  `0x15591a`.
* Cause: `src/game/mission/GameRecord.cpp` defined the destructor before the
  constructor, so `GameRecord.cpp.obj` emitted `~GameRecord()` first. The
  original binary and the iOS 1.1.19 source-layout dump both place the
  constructor first.
* Fix: move the `GameRecord::GameRecord()` definition before
  `GameRecord::~GameRecord()`. This is a pure move; all 16 content sections in
  the object are byte-identical, and only the `.text.*` section-header order
  changes.
* Gate after the move: **byte 4497 / linked 4497**, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0. Plain no-order after: **byte_exact 2864 /
  linked 4497**, **3499 / 4524** addresses, prefix **3479**.
* The next plain no-order mismatch is `Waypoint::Waypoint(int, int, int,
  Route*)` (`0x157c54` original, `0x157c9c` ours).

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

Plain no-order baseline (the exact gold line minus
`--section-ordering-file`):

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/gamerecord/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/gamerecord/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/gamerecord/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/gamerecord/base_noorder.so
```

```text
compared 4524  avg 99.9%  byte_exact 2860  linked_exact 4497  fuzzy_perfect 4497
/tmp/w3745/gamerecord/base_noorder.so: 3495/4524 same; prefix 3410;
  first (1399040, '_ZN10GameRecordC1Ev', 1399066)
```

`1399040 = 0x155900`, `1399066 = 0x15591a`.

## 2. Original order

`nm -nSD --defined-only` on the original, restricted to `GameRecord`:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | grep '_ZN10GameRecord'
```

```text
00155900 000000d4 T _ZN10GameRecordC1Ev
00155900 000000d4 T _ZN10GameRecordC2Ev
001559d4 0000001a T _ZN10GameRecordD1Ev
001559d4 0000001a T _ZN10GameRecordD2Ev
001559f0 00000a88 T _ZN10GameRecord4loadEv
```

The HD 1.1.19 source-layout dump independently lists the same order
(`_work/original_layout/original_layout.json`,
`tu_symbols.gof2hd_1.1.19_ios["Classes/Game/GameRecord.cpp"]`):

```text
_GameRecord::GameRecord()
_GameRecord::GameRecord()
_GameRecord::~GameRecord()
_GameRecord::~GameRecord()
_GameRecord::load()
```

## 3. Baseline object order

`readelf -SW` on `GameRecord.cpp.obj` (section-header order = emission order):

```text
GameRecord::~GameRecord()
GameRecord::GameRecord()
GameRecord::load()
Array<bool>::~Array()
Array<int>::~Array()
```

The destructor was first because the `.cpp` defined it first. The ordering
file already puts the constructor at `0x155900` in the gate link, but the
plain no-order link follows the object order and therefore starts the block
with the destructor at `0x155900` and puts the constructor at `0x15591a`.

## 4. Landed source change

Move the constructor definition above the destructor. No body was edited.

```diff
@@
-GameRecord::~GameRecord() {
-}
-
 GameRecord::GameRecord() {
     this->data = new uint8_t[0x87];
@@
     this->field_0x1b4 = 0;
 }
 
+GameRecord::~GameRecord() {
+}
+
 typedef uint32_t uint;
```

The resulting object order is the original address order:

```text
GameRecord::GameRecord()
GameRecord::~GameRecord()
GameRecord::load()
Array<bool>::~Array()
Array<int>::~Array()
```

### Purity check

Compile the pre-move source (`git show HEAD:src/game/mission/GameRecord.cpp`)
and the post-move source with the exact `compile_commands.json` command,
replacing only the input and output paths. Comparing every content section
whose name starts with `.text.`, `.ARM.`, `.rodata`, `.data` or `.bss`:

```text
content sections compared 16 differing 0
text sections 5 5 order same? False
first order diff 0 .text._ZN10GameRecordD2Ev .text._ZN10GameRecordC2Ev
base text order  [.text._ZN10GameRecordD2Ev, .text._ZN10GameRecordC2Ev, .text._ZN10GameRecord4loadEv, ...]
after text order [.text._ZN10GameRecordC2Ev, .text._ZN10GameRecordD2Ev, .text._ZN10GameRecord4loadEv, ...]
```

So the move changes only `.text.*` section-header order (and section-index
operands in relocation sections); no function body, literal, table or
exception-table bytes change.

## 5. Measured A/B

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `7a8b34c8…` | **2860** | 3495 / 4524 | 3410 | `GameRecord::GameRecord()` `0x155900` vs `0x15591a` |
| ctor before dtor (landed) | **4497 / 4497** | `b237d806…` | **2864** | **3499 / 4524** | **3479** | `Waypoint::Waypoint(int, int, int, Route*)` `0x157c54` vs `0x157c9c` |

Landed delta: **+4 plain byte_exact, +4 exported functions at their original
address, +69 exact-address prefix**, gate counts unchanged (`4497 / 4497`,
lint `CLEAN ✓`, drift 0). The four plain-link gains are exactly the two
constructor aliases and the two destructor aliases:

```text
GameRecord::GameRecord()   C1  False -> True
GameRecord::GameRecord()   C2  False -> True
GameRecord::~GameRecord()  D1  False -> True
GameRecord::~GameRecord()  D2  False -> True
```

No function lost `byte_exact` or `linked_exact`. All five `GameRecord`
`.dynsym` rows are now at the original address and size in the plain no-order
link:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | grep '_ZN10GameRecord' | awk '{print $1, $2, $4}' > /tmp/w3745/gamerecord/orig_syms.txt
$NM -nSD --defined-only /tmp/w3745/gamerecord/after_noorder.so \
  | grep '_ZN10GameRecord' | awk '{print $1, $2, $4}' > /tmp/w3745/gamerecord/after_syms.txt
diff -u /tmp/w3745/gamerecord/orig_syms.txt /tmp/w3745/gamerecord/after_syms.txt
# no output
```

The plain-link `.rodata` is byte-identical before and after:

```sh
OBJCOPY=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objcopy
$OBJCOPY -O binary --only-section=.rodata /tmp/w3745/gamerecord/base_noorder.so  /tmp/w3745/gamerecord/base.rodata
$OBJCOPY -O binary --only-section=.rodata /tmp/w3745/gamerecord/after_noorder.so /tmp/w3745/gamerecord/after.rodata
cmp /tmp/w3745/gamerecord/base.rodata /tmp/w3745/gamerecord/after.rodata   # 0
```

### Gate `.so` side effects

Every ELF section size and base is unchanged, and `section_order.txt` is
unchanged. The gate `.so` is not byte-identical: **67 differing bytes**.

| range | bytes | what |
|---|---:|---|
| `.note.gnu.build-id` | 20 | build-id |
| `.ARM.exidx` | 2 | `GameRecord` ctor/dtor records |
| `.ARM.extab` | 45 | `GameRecord` ctor/dtor LSDA records |

`.text`, `.rodata`, `.data`, `.data.rel.ro` and `.bss` are byte-identical
between the two gate links. The two changed exidx entries target the original
addresses `0x155900` (`GameRecord::GameRecord()`) and `0x1559d4`
(`GameRecord::~GameRecord()`); the extab bytes are their records at
`0x1c3fac`/`0x1c3fd0`.

## 6. Next frontier

The first plain no-order mismatch is now:

```text
original 0x157c54  Waypoint::Waypoint(int, int, int, Route*)
ours     0x157c9c  Waypoint::Waypoint(int, int, int, Route*)
```

The original `Waypoint` address order is constructor first, then destructors,
then `getPosition`, `setActive`, `reached`, `reset`. Our
`Waypoint.cpp.obj` currently emits the accessors first and the constructor
last, so this is the next per-TU source-order candidate. It was not changed
in this pass.

## 7. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements (before and after the source move)
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/gamerecord/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/gamerecord/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/gamerecord/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/gamerecord/base_noorder.so

python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/gamerecord/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/gamerecord/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/gamerecord/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/gamerecord/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/mission/GameRecord.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN10GameRecord'

# gate .so A/B (baseline vs landed source)
sha256sum /tmp/w3745/gamerecord/gate_base.so /tmp/w3745/gamerecord/gate_after.so
cmp -l /tmp/w3745/gamerecord/gate_base.so /tmp/w3745/gamerecord/gate_after.so | wc -l   # 67
```

## 8. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `2860 / 4497`, prefix 3410,
  3495 / 4524, first mismatch `GameRecord::GameRecord()` `0x155900` vs
  `0x15591a`.
* The original binary and the iOS 1.1.19 source-layout dump both place
  `GameRecord::GameRecord()` at `0x155900`, `GameRecord::~GameRecord()` at
  `0x1559d4`, and `GameRecord::load()` at `0x1559f0`.
* The baseline object emitted the destructor before the constructor because
  the `.cpp` definitions were in that order.
* Moving the constructor definition above the destructor is a pure
  section-order change: all 16 object content sections are byte-identical;
  only `.text.*` header order differs.
* The move is gate-neutral in counts (`4497 / 4497`, lint `CLEAN ✓`, drift 0;
  every ELF section size/base unchanged) and improves the plain no-order build
  to `2864 / 4497`, 3499 / 4524 addresses, prefix 3479.
* The four plain-link gains are exactly the `GameRecord` C1/C2/D1/D2 rows;
  there are no losses. All five `GameRecord` `.dynsym` rows are now
  address- and size-exact in the plain link.
* The plain-link `.rodata` is byte-identical before and after.
* The next plain no-order frontier is `Waypoint::Waypoint(int, int, int,
  Route*)` (`0x157c54` original, `0x157c9c` ours).

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `2864`, not `4497`.
* The `Waypoint` reorder is the obvious next per-TU source-order experiment,
  but it was not tested or landed in this pass.
