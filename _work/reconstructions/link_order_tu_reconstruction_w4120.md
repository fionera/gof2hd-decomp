# Link-order / TU reconstruction: can the section-ordering file be removed? (w4120)

Status: research note + one build-system correction, 2026-11-03. Worktree `wt/w4120`,
branch `wave91-w4120`, base `a4557300`. The canonical build was not changed: it still
uses the generated section-ordering file and still measures **byte_exact 4498 /
linked_exact 4498**. Scratch work is under `/dev/shm/w4120`.

## 0. Bottom line

* The original `libgof2hdaa.so` **was linked without a `--section-ordering-file`**:
  its `.text` is grouped per original TU (105 TUs, 104 transitions, 0 interleaving)
  and its `.ARM.exidx` contains the 193 empty-`.text` `CANTUNWIND` sentinels that
  only a plain `-ffunction-sections` link produces. The current tree reproduces those
  193 sentinels address-for-address.
* The current tree **cannot yet drop the ordering file**. With the same objects and
  the reconstructed link order, a plain input-order link measures
  **byte_exact 3036 / linked_exact 4498** (canonical: 4498 / 4498). The loss is
  placement-only: `linked_exact` is unchanged, and every function body is still the
  same code.
* The gap is **not** object link order. The reconstructed link order is already the
  original TU order; only **1 of 193** objects has a strong (`T`) function interleaved
  with another object (`NdkHooks.cpp` / `NdkAutosave.cpp`). The gap is
  **within-TU section order**:
  * 7 of 193 objects define their exported functions in a different order from the
    original (Ship, Agent, BluePrint, LodMeshMerger, PlayerGasCloud, ParticleSettings,
    ParticleSystemManager);
  * clang 7's deferred emission hoists a callee that is referenced before its
    definition, so source reordering alone cannot reproduce the original within-TU
    order (concrete case: `Ship::getCargoValue` is emitted immediately after
    `Ship::refreshValue` in our build, but after `Ship::~Ship` in the original);
  * local (`static`) functions and weak inline copies are not in `.dynsym`, so their
    original positions must be recovered from the binary, not from the source dumps.
* `--unresolved-symbols=ignore-all` is **already absent** from the current gold link
  line, and `tools/sodiff_allow.txt` is **empty**. Neither is needed for the current
  gate.
* Landed change: `CMakeLists.txt` now always compiles with
  `-ffunction-sections -funwind-tables`; `GOF2_MATCH_LAYOUT` only controls whether the
  generated ordering file is used. This makes `GOF2_MATCH_LAYOUT=OFF` the faithful
  plain-input-order POC (previously it also dropped `-ffunction-sections`, which the
  original demonstrably used). Canonical gate unchanged.

## 1. Baseline

```
cmake -S . -B cmake-build-match -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Section fingerprint (canonical, ordering file ON):

```
.text       0x1337c8 @ 0x69440
.ARM.exidx  0x006f70 @ 0x19cc08   (3566 entries, 272 CANT, 194 duplicate targets,
                                   193 CANT+real pairs)
.ARM.extab  0x026290 @ 0x1a3b78   (original 0x26298, known -8)
.rodata     0x041b67 @ 0x1c9e10
```

## 2. What the ordering file is doing

`tools/gen_section_order.py` reads the original's exported function addresses
(`nm -nD`) and emits one `.text.<mangled>` name per original address. With
`-ffunction-sections`, every function is its own input section, so gold places our
functions in exactly the original sequence. Local functions (not in `.dynsym`) are
slotted next to their object's neighbouring sections in source order; empty-`.text`
TU sentinels are renamed and placed by `tools/gen_exidx_markers.py`.

`tools/gen_link_order.py` separately orders the **objects** by the lowest original
address of a strong (`T`) function they define. That is the original TU order. The
current `cmake-build-match/link_order.rsp` is exactly that min-strong-address order
(193 objects). It contains two explicit fixups: `GameText.cpp` immediately before
`Engine.cpp`, and `NdkAutosave.cpp` immediately after `NdkHooks.cpp`.

## 3. Evidence that the original was linked without an ordering file

This was established in `_work/reconstructions/ffunction_sections_and_order_file.md`
and re-verified here:

```
path                                             entries  CANT  dup  CANT+real
original android_2.0.16_libgof2hdaa.so              3566   272  194        193
cmake-build-match/libgof2hdaa.so (canonical)        3566   272  194        193
cmake-build-off/libgof2hdaa.so (no ordering file)   3566   273  193        192
```

The 193 `CANT+real` pairs are gold's `fix_exidx_coverage()` sentinels for the empty
plain `.text` section every `-ffunction-sections` object carries. A link with an
ordering file collapses those same-named `.text` sections to one index; a plain
input-order link keeps one per TU. The original has one per TU, and its `.text` is
perfectly grouped by original TU (105 TUs, 104 transitions, 0 interleaving).

## 4. Current no-ordering-file POC

The faithful POC is:

```
cmake -S . -B cmake-build-off -G Ninja \
  -DGOF2_MATCH_LAYOUT=OFF \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz
cmake --build cmake-build-off --target link_so -j8
python3 tools/verify/verify.py --build-dir /dev/shm/w4120/verify_off_base \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-off/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Measured (current source, `-ffunction-sections`, plain input-order link, reconstructed
`link_order.rsp`):

| build | byte_exact | linked_exact | .text | .ARM.exidx | .ARM.extab | .rodata base | exported funcs at original address |
|---|---:|---:|---:|---:|---:|---:|---:|
| canonical (ordering file) | **4498** | **4498** | `0x1337c8` | `0x6f70` | `0x26290` | `0x1c9e10` | 4524 / 4524 |
| `GOF2_MATCH_LAYOUT=OFF` | **3036** | **4498** | `0x1337e8` | `0x6f70` | `0x26290` | `0x1c9e30` | 3925 / 4524 |

The `.text` is 0x20 bytes larger and the later sections shift by 0x20. The first
address divergence is inside `Ship.cpp` (see §6), not at an object boundary.

For comparison, relinking the canonical (marker-renamed) objects with only the
`--section-ordering-file` argument removed gives byte_exact 3031, `.text 0x1337e8`,
`.ARM.exidx 0x6f68`, `.rodata 0x1c9e20`. The plain-object OFF build is closer
(`.ARM.exidx` exact) because it keeps one empty `.text` per object.

## 5. Link order is already reconstructed; the gap is within-TU order

`_work/reconstructions/tu_membership_interleaving_w3613.md` measured the object
contiguity:

| metric | objects | interleaved |
|---|---:|---:|
| all exported functions (incl. weak `W`) | 195 | 144 |
| strong (`STB_GLOBAL`, `T`) functions only | 193 | **1** |

The single strong interleaving is `NdkHooks.cpp` containing `NdkAutosave.cpp`'s
`ndk_autosave` at `0x94770`. Everything else is contiguous. The 144 figure is
dominated by weak inline copies (`Array`, `String`, `Matrix` ctors) that every TU
emits; the surviving copy is decided by link order, which is already the original
TU order.

So the remaining placement gap is the order of sections **inside** each object.
The object's `.text.*` section order is clang's emission order. Comparing that order
to the original address order for the 193 app objects:

```
objects                                          193
objects whose exported functions are out of order   7
exported functions in the simulation             4282
same relative position (vs original order)       4140
adjacent-correct pairs                           4155 / 4281
longest increasing subsequence (actual OFF .so)  4430 / 4524
adjacent-correct pairs (actual OFF .so)          4397 / 4523
```

The seven out-of-order objects, with inversion counts (pairs out of original order):

```
660 inversions / 48 funcs   Agent.cpp
310 inversions / 93 funcs   Ship.cpp
138 inversions / 27 funcs   BluePrint.cpp
 17 inversions / 11 funcs   LodMeshMerger.cpp
 11 inversions / 17 funcs   PlayerGasCloud.cpp
  6 inversions /  6 funcs   ParticleSettings.cpp
  4 inversions / 40 funcs   ParticleSystemManager.cpp
```

All other 186 objects already emit their exported functions in original-address
order.

## 6. Source-reorder experiment and the clang callee-hoist blocker

A tool-assisted reorder of the seven files (move each top-level function definition
block to the slot of the original-address order, leaving non-function chunks and
local functions in place) was tested in a scratch copy under `/dev/shm/w4120/tree1`.
It fixes the exported-function order almost completely, but it does **not** remove the
ordering file:

| variant | byte_exact | linked_exact | .text | .ARM.exidx | .rodata base |
|---|---:|---:|---:|---:|---:|
| canonical, before reorder | **4498** | **4498** | `0x1337c8` | `0x6f70` | `0x1c9e10` |
| canonical, 7 TUs reordered | 4495 | 4498 | `0x1337c8` | `0x6f70` | `0x1c9e10` |
| OFF, before reorder | 3036 | 4498 | `0x1337e8` | `0x6f70` | `0x1c9e30` |
| OFF, 7 TUs reordered | 3018 | 4498 | `0x1337e8` | `0x6f70` | `0x1c9e30` |

The canonical build loses 3 byte-exact functions (the reorder changes local-static/
`.rodata` emission order); that is not a strict improvement, so the reorder is not
landed.

The deeper blocker is clang 7's **deferred emission of callees**. A function that is
referenced before its definition is emitted immediately after the caller, not at its
definition point. Minimal reproducer:

```cpp
int g();
int f() { return g(); }
int h() { return 7; }
int g() { return 3; }
int i() { return 9; }
```

`readelf -SW` on the object gives `.text._Z1fv`, `.text._Z1gv`, `.text._Z1hv`,
`.text._Z1iv` — `g` is hoisted ahead of `h`, exactly as in the source order
`f, g, h, i`.

The same thing happens in `Ship.cpp`:

```
original Ship TU order (addresses):
  0x17457c  Ship::Ship
  0x174608  Ship::refreshValue        (calls Ship::getCargoValue at 0x174978)
  0x1749a4  Ship::~Ship
  0x174a02  Ship::setRace
  ...
  0x175270  Ship::getCargoValue
  0x1752a4  Ship::getEquipmentValue

our reordered Ship.cpp object order:
  0x17457c  Ship::Ship
  0x174608  Ship::refreshValue
  0x175270  Ship::getCargoValue        <-- hoisted by clang
  0x1749a4  Ship::~Ship
  0x174a02  Ship::setRace
  ...
```

The original's `refreshValue` really does call `getCargoValue` (PLT call at
`0x174978`), so this is not a decomp error. The original compiler emitted
`getCargoValue` at its source position; our clang 7.0.2 (linux NDK) hoists it. The
original was built on macOS with the darwin NDK, so a host-specific codegen-order
difference is possible, but it is not yet proven. Either way, source reordering alone
cannot reproduce the original within-TU order for this TU.

## 7. TU membership divergences still to converge

From `_work/original_layout/tu_mapping.md` and `original_layout_notes.md`:

* `NdkHooks.cpp` / `NdkAutosave.cpp`: the only strong interleaving. The original
  link order is `NdkHooks` then `NdkAutosave`; merging them (or moving
  `ndk_autosave`) is a known candidate, but the earlier merge measurement cost 519
  byte_exact because it changed the object's section/PLT order.
* `GameResource.cpp`: our tree has the original TU again (w3624), but the original
  link order override (`LowResourceTable` before `ResourceMaterial`) is load-bearing
  for `.rodata`.
* `PaintCanvas.cpp`: the original engine kitchen-sink TU also defines `Camera`,
  `Material`, `Image2D`, `ResourceMesh/Texture/Transform`, `AELoadedTexture`. Our
  tree splits these into separate objects. They are contiguous sub-ranges today, but
  the split changes local-section and `.rodata` emission order.
* `Vector.cpp` / `BSphere.cpp`: the SD dump attributes `BSphere::Merge(BSphere)` and
  `operator=` to `Vector.cpp`; merging them cost 456 byte_exact at the w3613 HEAD.
* `AbstractGun` -> `BeamGun.cpp`, `KeyFrame` -> `Transform.cpp`,
  `Node` -> `SystemPathFinder.cpp`, `AESound`/`KeyCode`/`Resource*` empty TUs: the
  empty ones are already removed; the rest are still split.
* The 2.0.x-only TUs (`PlayerCreature`, `SpriteGun`, GLES2 shader zoo, `NFC`,
  `VirtualInput`) have no dump witness and must be placed from binary evidence.

## 8. Concrete removal plan

1. **Keep the canonical build green.** Leave `GOF2_MATCH_LAYOUT=ON` as the default
   and the ordering file as the byte-matching aid. Use
   `GOF2_MATCH_LAYOUT=OFF` as the convergence meter.
2. **Converge object link order.** The current `gen_link_order.py` min-strong-address
   order is already the original TU order (1/193 strong interleaving). Fix the
   `NdkHooks`/`NdkAutosave` interleaving first; re-measure the OFF gate.
3. **Converge TU membership.** Merge/split the known divergences in §7, one at a
   time, under the full canonical gate. `.rodata`/`.data`/local-section order is the
   sensitive part; a merge that only fixes `.text` order can still lose byte_exact.
4. **Reorder exported function definitions** in the seven TUs of §5 to original
   address order. This is mechanical and tool-assisted; it fixes the exported
   sequence but not the callee-hoist cases.
5. **Handle callee hoisting.** For each hoisted callee (start with
   `Ship::getCargoValue`), either (a) prove the darwin/linux clang emission order
   differs and find a source shape that reproduces the original, or (b) split the
   callee into a separate TU at the original boundary (changes TU membership), or
   (c) accept a small residual and keep the ordering file for that TU.
6. **Recover local/static function order.** Local functions are absent from
   `.dynsym`; their original positions come from `.ARM.exidx`/disassembly. The
   ordering file currently places them from our source order, so this is the last
   large unknown.
7. **Remove the file only when the OFF gate is 4498/4498** (or when the residual is
   documented as irreducible). Then delete the `GOF2_MATCH_LAYOUT` branch, the
   marker renaming, and `tools/gen_section_order.py`/`gen_exidx_markers.py` from the
   canonical path.

## 9. `--unresolved-symbols=ignore-all` and `tools/sodiff_allow.txt`

* The current gold link line contains **no** `--unresolved-symbols=ignore-all`:

  ```
  ld.gold -shared -soname libgof2hdaa.so --build-id -s -z now \
    --exclude-libs=libgcc.a [--section-ordering-file=...] -o libgof2hdaa.so \
    crtbegin_so.o @link_order.rsp --whole-archive libz libzip --no-whole-archive \
    -l log -l GLESv2 -l GLESv1_CM -l EGL -l android libfmodex.so libfmodevent.so \
    libcrypto.a -l m libc++abi.a libandroid_support.a libgcc.a -l dl -l c crtend_so.o
  ```

  A shared-library link allows undefined imports by default, which is what the
  original does. The flag is not needed and is not present.
* `tools/sodiff_allow.txt` is **empty** (only comments). The current `sodiff` reports
  `extra-beyond-allow 0`, `missing 0`, `allowed 0`. No allowlist entries are needed.

## 10. Landed change

`CMakeLists.txt`:

```diff
 option(GOF2_MATCH_LAYOUT "Reproduce the original .text function order via section ordering" ON)
-if(GOF2_MATCH_LAYOUT)
-    target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)
-endif()
+# The original was compiled with -ffunction-sections (proven by the 193 empty-.text
+# CANTUNWIND sentinels in its .ARM.exidx).  Keep it on in both modes; the option only
+# controls whether the generated section-ordering file is used to place the sections.
+target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)
```

Canonical gate before/after: **byte 4498 / linked 4498 -> byte 4498 / linked 4498**,
`.text 0x1337c8`, `.rodata 0x41b67 @ 0x1c9e10`, `.ARM.exidx 0x6f70`,
`.ARM.extab 0x26290`, data 843/843, imports 207, lint clean, drift 0.
`GOF2_MATCH_LAYOUT=OFF` now builds the faithful plain-input-order POC:
**byte 3036 / linked 4498**, `.text 0x1337e8`, `.ARM.exidx 0x6f70`,
`.rodata 0x41b67 @ 0x1c9e30`.

## 11. Proven vs hypothesis

**Proven by measurement:**

* The original was linked without a section-ordering file (193 empty-`.text`
  CANTUNWIND sentinels; per-TU `.text` grouping).
* The current ordering file is worth **1462 byte_exact** (4498 -> 3036) with
  `linked_exact` unchanged.
* The object link order is already the original TU order; only 1/193 objects has a
  strong-function interleaving.
* 7/193 objects emit their exported functions in a different order from the original;
  reordering them fixes the exported sequence but costs 3 canonical byte-exact and
  does not fix the no-order gate because of clang callee hoisting.
* `Ship::getCargoValue` is hoisted immediately after `Ship::refreshValue` by our
  clang 7.0.2, while the original places it after `Ship::~Ship`; the original really
  does call it from `refreshValue`.
* `--unresolved-symbols=ignore-all` is absent and `sodiff_allow.txt` is empty.

**Hypothesis (not proven):**

* The original's callee emission order may be a darwin-vs-linux clang 7.0.2
  difference, or a source-order difference we have not reconstructed.
* With exact TU membership, exported-function order, callee emission order, and
  local-function order, a plain input-order link will reproduce the original `.text`
  and the ordering file can be deleted. The current evidence is consistent with this
  but the OFF gate is still 1462 byte_exact short.

## 12. Reproduction

```sh
# canonical
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# faithful no-ordering-file POC
cmake -S . -B cmake-build-off -G Ninja -DGOF2_MATCH_LAYOUT=OFF \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs -DGOF2_MATCH_OPT=-Oz
cmake --build cmake-build-off --target link_so -j8
python3 tools/verify/verify.py --build-dir /dev/shm/w4120/verify_off_base \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-off/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

The reorder tool and probes are committed alongside this note in
`_work/reconstructions/w4120/` (`parse_funcs.py`, `reorder.py`, `order_probe.py`,
`addr_cmp.py`, `delta.py`, `linkorder_cmp.py`). The reorder experiment tree was
`/dev/shm/w4120/tree1`.
