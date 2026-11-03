# Static link order: removing the last binary-derived build input (w4217)

Status: landed build-system change, 2026-11-03. Worktree `wt/w4217`, branch `wave92-w4217`,
base `b379d00d`. Scratch work is under `/dev/shm/w4217`. This note extends
`link_order_tu_reconstruction_w4210.md` (which removed the section-ordering file and proved
that a plain input-order link reproduces all 4,524 addresses) and
`link_order_tu_reconstruction_w4120.md` (which proved the object order is already the original
TU order).

## 0. Bottom line

The build no longer reads the original `libgof2hdaa.so` to decide link order. The object order
is a checked-in, human-readable **plain source list**:

```
tools/source_order.txt     192 source paths in the original TU order
```

`CMakeLists.txt` reads that list, builds `add_library(gof2 OBJECT ...)` from it, and links
`$<TARGET_OBJECTS:gof2>` directly. `tools/gen_link_order.py` (the old `.dynsym`-derived
generator) is now an analysis tool only; `tools/gen_section_order.py` and
`tools/gen_exidx_markers.py` are deleted.

The resulting link is **byte-identical** to the previous generated-order link:

```
before: sha256 2e6b0412909809ee031786184bf5ee32a922d5f002d7d7b802fc33510ecfc777
after:  sha256 2e6b0412909809ee031786184bf5ee32a922d5f002d7d7b802fc33510ecfc777
```

All 192 objects are on the link line in the same order, all 4,524 exported functions are at the
same addresses, and every gate count is unchanged.

## 1. Before / after gate

```
                                   before      after
verify byte_exact                  4498        4498
verify linked_exact                4498        4498
size-exact                         4524/4524   4524/4524
stub_zero_size                     0           0
missing / wrong_type / extra       0 / 0 / 0   0 / 0 / 0
data_exact                         843/843     843/843
imports                            207         207
.text                              0x1337c8    0x1337c8
.rodata                            0x41b67     0x41b67
.ARM.exidx                         0x6f70      0x6f70
.ARM.extab                         0x26290     0x26290  (original 0x26298, known -8)
lint                               CLEAN       CLEAN
drift                              0 fields    0 fields
```

Section table after the change (identical to before):

```
.text       0x1337c8 @ 0x69440
.ARM.exidx  0x006f70 @ 0x19cc08
.ARM.extab  0x026290 @ 0x1a3b78
.rodata     0x041b67 @ 0x1c9e10
.data.rel.ro 0x002fe8 @ 0x20cbb0
.got        0x00430c @ 0x20fcf4
.data       0x00296c @ 0x214000
.bss        0x0069ac @ 0x216970
```

## 2. What was generated before

`CMakeLists.txt` had a custom command that ran `tools/gen_link_order.py` on every link:

```
python3 tools/gen_link_order.py <original.so> <nm> <build>/link_order.rsp $<TARGET_OBJECTS:gof2>
```

The generator read the original `.dynsym`, took each object's lowest strong (`T`) function
address, sorted the objects by that address, and applied two witnessed fixups
(`GameText.cpp` before `Engine.cpp`; `NdkAutosave.cpp` immediately after `NdkHooks.cpp`).
The result was a 192-line response file of object paths. That response file was the last
build input derived from the original binary.

The order itself is stable and is the original TU order (w4120: 1/193 strong interleavings,
and w4210 removed the last section-ordering file). It does not depend on our source contents,
only on the original's TU boundaries, so freezing it as a source list is faithful.

## 3. The checked-in list

`tools/source_order.txt` contains the 192 TUs in the exact order of the old
`cmake-build-match/link_order.rsp`, with a short header explaining the invariant. It is a plain
source list (one `src/...` path per line), not a symbol/address map. The first and last entries:

```
src/platform/android/BackButton.c
src/engine/core/NFC.cpp
src/engine/input/VirtualInput.cpp
...
src/engine/core/Array.cpp
src/engine/core/ArrayInstantiations.cpp
```

Derivation (one-time, reproduced here so the list is not a transcript-only artifact):

```sh
# with a configured build:
python3 - <<'PY'
import os
bd = 'cmake-build-match'
pre = os.path.abspath(bd) + '/CMakeFiles/gof2.dir/./'
for line in open(bd + '/link_order.rsp'):
    obj = line.strip()
    if not obj:
        continue
    assert obj.startswith(pre) and obj.endswith('.obj')
    rel = obj[len(pre):-4]
    assert os.path.exists(rel)
    print(rel)
PY
```

The output was checked against `git ls-files 'src/*.cpp' 'src/*.c'` (same 192 files, no
duplicates, no omissions) and committed as `tools/source_order.txt`.

## 4. Build-system change

`CMakeLists.txt`:

* replaced the `file(GLOB_RECURSE ...)` source discovery with
  `file(STRINGS tools/source_order.txt _srcs)` + comment/blank filtering;
* `add_library(gof2 OBJECT ${_srcs})` now receives the explicit ordered list;
* added `CMAKE_CONFIGURE_DEPENDS` on `tools/source_order.txt`, so editing the list re-runs
  CMake and re-orders the objects;
* added a configure-time guard that the list covers exactly the `src/*.cpp` + `src/*.c` set
  (a new TU cannot be silently omitted from the link);
* removed the `gen_link_order.py` custom command and the `link_order.rsp` dependency;
* the link now uses `set(_link_objs "$<TARGET_OBJECTS:gof2>")` directly and depends on
  `tools/source_order.txt`.

`tools/ratchet.py`:

* `newest_build_input_mtime()` now watches `tools/source_order.txt`, `CMakeLists.txt` and
  `cmake/toolchain.cmake` (the deleted generator paths and the old build-dir rsp are gone), so
  a hand edit to the link order makes the `.so` stale before the ratchet runs.

Deleted (off the build path; confirmed by `grep -rn` before deletion):

```
tools/gen_section_order.py
tools/gen_exidx_markers.py
```

`tools/gen_link_order.py` is kept as an analysis tool; nothing on the build path invokes it.

## 5. Verification that the order is unchanged

The link command was extracted from Ninja and the object paths mapped back to sources:

```sh
ninja -C cmake-build-match -t commands link_so | tail -1
```

Result: **192 objects, in exactly the same order as the old `link_order.rsp`** (element-wise
comparison `srcs == expected`: `True`). The old response file is not read by the build any
more; it was only used as the comparison oracle for this pass.

The whole `.so` hash is unchanged, and an `nm -nD --defined-only` dump is identical before and
after (`5370` symbols, `diff` empty). A direct ELF comparison of the 4,524 common function
symbols gives **0 address mismatches**:

```
common funcs 4524   addr mismatches 0
```

**The link no longer needs the original `.so` at all.** A fresh build directory configured with
`-DGOF2_ORIGINAL_SO=/dev/shm/w4217/does_not_exist.so` (and an explicit
`-DGOF2_FMOD_LIBDIR=.../apk_libs`) built `link_so` successfully and produced the same hash
`2e6b0412...c777`. Only the `verify`/`diff` targets still take the original as their oracle;
nothing on the link path reads it.

## 6. How to verify / maintain

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

To re-check the frozen order against the current objects without changing the build:

```sh
ninja -C cmake-build-match -t commands link_so | tail -1 |
  grep -o '[^ ]*\.obj' |
  sed 's#^.*/CMakeFiles/gof2.dir/\./##; s/\.obj$//' > /dev/shm/order_now.txt
diff -u <(grep -v '^#' tools/source_order.txt | grep -v '^$') /dev/shm/order_now.txt
```

When a new TU is added, insert it at its original TU position (the source-layout dumps and the
original address order are the authority) and update `tools/source_order.txt` in the same
commit; the configure-time guard will fail until the set matches.

## 7. Proven vs remaining

**Proven by measurement:**

* The frozen source list is exactly the old generated object order (192/192, element-wise).
* The frozen order produces the same `.so` hash, the same section sizes/bases, the same
  `nm -nD` dump, and 4,524/4,524 exported functions at their original addresses.
* The build no longer reads the original `.so` for link order (only the `verify`/`diff`
  validation targets still need it, as the oracle).
* The gate is unchanged: byte 4498 / linked 4498 / data 843 / imports 207 / lint CLEAN /
  drift 0.

**Remaining (unchanged by this pass):**

* The 26 non-byte-exact function bodies and the resulting `.ARM.extab` -8.
* `.dynsym`/`.dynstr`/`.hash` order, which follows gold's symbol-hash iteration over the
  complete insertion sequence and converges with the remaining body/TU contents.
