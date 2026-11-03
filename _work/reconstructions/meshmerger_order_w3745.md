# MeshMerger section-order pass — w3745

Worker: w3745. Branch: `wave87-w3745`, based on master `385534e0`.
Original: `../_work/bins/android_2.0.16_libgof2hdaa.so` (stripped, armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`).

This note records the plain-no-order frontier at this HEAD and the source
reorder that moves the whole `MeshMerger` block to its original addresses.
Every number below was produced in this worktree with the commands in §8.

## 1. Baseline

`cmake-build-match` was configured with `cmake --preset default`. Baseline
(`cmake --build cmake-build-match --target check -j8`, then `lint`, `drift`):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

The brief's 4496 baseline is stale; this HEAD is **4498 / 4498**.

Gate ELF layout (`readelf -SW cmake-build-match/libgof2hdaa.so`):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
```

Plain no-order link (the exact gold line minus `--section-ordering-file`):

```text
byte_exact 3007  linked_exact 4498
exact addresses 3880 / 4524   exact-address prefix 3856
first mismatch: MeshMerger::setMesh(int, signed char, unsigned short)
                original 0x173bf8  ours 0x173ce0
```

## 2. Original order (binary evidence)

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN10MeshMerger' | c++filt
```

The original `.text` order of the `MeshMerger` TU is:

```text
0x1737a0  MeshMerger::MeshMerger(Array<uint16_t> const&, Array<Matrix>, PaintCanvas*, uint16_t)
0x173a60  MeshMerger::MeshMerger(int, int, PaintCanvas*, uint16_t)
0x173bf8  MeshMerger::setMesh(int, signed char, uint16_t)
0x173c4c  MeshMerger::setLod(int, signed char)
0x173c74  MeshMerger::setMatrix(int, Matrix const&)
0x173c84  MeshMerger::setEnabled(int, bool)
0x173ca6  MeshMerger::init()
0x173d90  MeshMerger::transformMesh(Mesh*, Matrix const&)
0x173f2c  MeshMerger::update()
0x17406c  MeshMerger::render()
0x174078  MeshMerger::~MeshMerger()
```

The baseline object section order (`readelf -SW` on
`cmake-build-match/CMakeFiles/gof2.dir/src/engine/render/MeshMerger.cpp.obj`)
was different:

```text
ctor(Array<uint16_t>)
ctor(int,int)
Matrix::Matrix()                 # weak, deduped by the linker
~MeshMerger
setMatrix
setLod
setMesh
setEnabled
render
update
init
transformMesh
```

So the baseline put the destructor and the four setters before `init`, and
`render`/`update` before `init`/`transformMesh`. The first plain mismatch was
exactly `setMesh` (`0x173bf8` original vs `0x173ce0` ours).

## 3. Landed source change

Only the top-level definition order in
`src/engine/render/MeshMerger.cpp` changed (no body edited; the sorted line
multiset is identical, 89 insertions / 89 deletions because the blank-line
separators moved with the blocks). The landed order is:

```text
MeshMerger::MeshMerger(Array<uint16_t> const&, Array<Matrix>, PaintCanvas*, uint16_t)
MeshMerger::MeshMerger(int, int, PaintCanvas*, uint16_t)
MeshMerger::setMesh
MeshMerger::setLod
MeshMerger::setMatrix
MeshMerger::setEnabled
MeshMerger::init
MeshMerger::transformMesh
MeshMerger::update
MeshMerger::render
MeshMerger::~MeshMerger
```

The resulting object section order is exactly the original order above
(modulo the linker-deduped weak `Matrix::Matrix()`):

```text
ctor(Array<uint16_t>)
ctor(int,int)
Matrix::Matrix()                 # weak
setMesh
setLod
setMatrix
setEnabled
init
transformMesh
update
render
~MeshMerger
```

Why this is the right order under `-ffunction-sections`: the section order is
first-reference order, not definition order.  `init()`'s body first calls
`transformMesh(...)` inside its loop and then `this->update()` at the end, so
defining `init` before `transformMesh`/`update` creates their sections in the
original `init, transformMesh, update` order.  `render` and the destructor are
defined last and are not referenced earlier, so they land last.  The setters
are not called from the constructors, so their definition order is their
section order.

## 4. Measured before/after

Every row is a controlled rebuild of this tree.  `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4498 / 4498** | `0291e00c…` | **3007** | 3880 / 4524 | 3856 | `MeshMerger::setMesh` `0x173bf8` vs `0x173ce0` |
| landed | **4498 / 4498** | `ee2ce25a…` | **3012** | **3890 / 4524** | **3885** | `Ship::~Ship()` `0x1749a4` vs `0x1749d8` |

Delta: **+5 plain byte-exact, +10 functions at their original address, +29
exact-address prefix**, gate counts unchanged.

The plain `report.json` changes exactly five rows, all `byte False -> True`:

```text
_ZN10MeshMerger13transformMeshEPN11AbyssEngine4MeshERKNS0_6AEMath6MatrixE
_ZN10MeshMerger6updateEv
_ZN10MeshMerger7setMeshEiat
_ZN10MeshMergerD1Ev
_ZN10MeshMergerD2Ev
```

The whole `MeshMerger` block is now address-exact in the plain no-order link:

```text
0x1737a0 ctor(Array<uint16_t>)  0x173a60 ctor(int,int)
0x173bf8 setMesh                0x173c4c setLod
0x173c74 setMatrix              0x173c84 setEnabled
0x173ca6 init                   0x173d90 transformMesh
0x173f2c update                 0x17406c render
0x174078 ~MeshMerger
```

## 5. Gate `.so` A/B

Baseline and landed gate `.so` were built by stashing the two source files and
relinking (`git stash push -- ...; cmake --build ... --target link_so;
git stash pop; cmake --build ... --target link_so`).

```text
baseline sha256 0291e00c6edcf19162b1dea80470ea069608199ad1ec2033ee10c92f96bed197
landed   sha256 ee2ce25a7b7f6d401984122c6ede3f200fad08c4163f5b42a0648c27918a7e94
cmp -l | wc -l  33
```

`.text` (1259464 B), `.rodata` (269159 B), `.data.rel.ro` (12264 B) and
`.data` (10604 B) are byte-identical between the two gate `.so` files.  The 33
differing bytes are exactly:

| range | bytes | what |
|---|---:|---|
| build-id | 20 | `.note.gnu.build-id` |
| `.ARM.exidx` | 6 | extab-pointer words for the reordered functions |
| `.ARM.extab` | 7 | extab record order for the reordered functions |

`.ARM.exidx` differing bytes against the original are unchanged at 1722 for
both `.so` files.  The `.ARM.extab` size is 156300 in both, versus 156312 in
the original (the pre-existing 12-byte extab shortfall, not caused here).

## 6. `AMeshMerger` (landed, metric-neutral)

`src/engine/render/AMeshMerger.cpp` had the destructor before `render`; the
original is `render` then `~AMeshMerger`:

```text
0x1811e4  AMeshMerger::render()
0x1811f0  AMeshMerger::~AMeshMerger()
```

The source was reordered to `render`, `~AMeshMerger`.  The object section order
is now `render`, `~AMeshMerger`, `Array<Mesh*>::~Array()` (weak, deduped).
This is gate-neutral and plain-metric-neutral (0 report rows change), but it
puts the TU in the original order.  It is included in this pass because it is
in the same `Classes/Game/MeshMerger/` family and is a prerequisite for the
eventual ordering-file removal.

## 7. `LodMeshMerger` (measured, not landed)

`src/engine/render/LodMeshMerger.cpp` is also out of order.  The original is:

```text
0x181200  LodMeshMerger::LodMeshMerger(int, int, PaintCanvas*, uint16_t)
0x1813ac  LodMeshMerger::setMesh
0x181400  LodMeshMerger::setLod
0x181428  LodMeshMerger::setMatrix
0x181438  LodMeshMerger::setEnabled
0x18145a  LodMeshMerger::init
0x181550  LodMeshMerger::transformMesh
0x18174c  LodMeshMerger::update
0x181914  LodMeshMerger::~LodMeshMerger
```

The baseline object order is `setEnabled, setLod, setMatrix, setMesh, update,
ctor, init, transformMesh, dtor`.  Reordering the definitions to `ctor,
setMesh, setLod, setMatrix, setEnabled, init, transformMesh, update, dtor`
produces the original object section order (with the weak `ArraySetLength` /
`Matrix::Matrix()` copies deduped), but it is **not** a plain-metric
improvement at this HEAD:

| variant | plain byte | exact addresses | prefix |
|---|---:|---:|---:|
| MeshMerger only (landed) | **3012** | 3890 | 3885 |
| + AMeshMerger | 3012 | 3890 | 3885 |
| + LodMeshMerger | 3008 | 3890 | 3885 |
| + both | 3008 | 3890 | 3885 |

The `-4` is a literal-pool artifact, not a `LodMeshMerger` body change: the
reorder shifts the vendored zlib block by `-8` in the plain link
(`get_crc_table` `0x1898ac -> 0x1898a4`; original `0x18989c`), and four zlib
functions (`crc32`, `get_crc_table`, `inflate_fast`, `zlibVersion`) whose
literal pools contain absolute addresses flip `byte True -> False`.  The
`LodMeshMerger` functions themselves do not gain or lose a byte row.  It is
therefore left unlanded; the desired definition order above is the next
`MeshMerger`-family change once the plain link reaches that block.

## 8. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order baseline
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_mm/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_mm/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_mm/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_mm/base_noorder.so

# source reorder (scratch script, not committed)
python3 /tmp/w3745_mm/do_reorder.py

# rebuild + plain no-order after
cmake --build cmake-build-match --target check -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_mm/final_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_mm/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_mm/final_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_mm/final_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/engine/render/MeshMerger.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN10MeshMerger' | c++filt

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_mm/gate_final.so
git stash push -- src/engine/render/MeshMerger.cpp src/engine/render/AMeshMerger.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_mm/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745_mm/gate_base.so /tmp/w3745_mm/gate_final.so
cmp -l /tmp/w3745_mm/gate_base.so /tmp/w3745_mm/gate_final.so | wc -l   # 33
```

## 9. Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4498 / linked 4498`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3007 / 4498`, prefix 3856,
  3880 / 4524, first mismatch `MeshMerger::setMesh` at `0x173bf8` vs
  `0x173ce0`.
* The original `MeshMerger` TU `.text` order is `ctor(Array), ctor(int,int),
  setMesh, setLod, setMatrix, setEnabled, init, transformMesh, update, render,
  ~MeshMerger` (`nm -nSD` on the original).
* Reordering the `MeshMerger.cpp` definitions to that order makes the object
  section order exactly the original order (weak `Matrix::Matrix()` aside,
  which the linker dedups).
* The landed reorder is gate-neutral (`4498 / 4498`, lint `CLEAN ✓`, drift 0;
  every ELF section size/base unchanged) and the gate `.text`, `.rodata`,
  `.data.rel.ro` and `.data` are byte-identical; only build-id (20 B),
  `.ARM.exidx` (6 B) and `.ARM.extab` (7 B) differ.
* The plain no-order build now has the whole `MeshMerger` block at its
  original addresses: exact addresses 3880 -> 3890, exact-address prefix
  3856 -> 3885, plain `byte_exact` 3007 -> 3012.  Exactly five
  `MeshMerger` rows flip `byte False -> True`.
* `AMeshMerger`'s original order is `render` then `~AMeshMerger`; the reorder
  is gate-neutral and changes 0 plain report rows.
* Reordering `LodMeshMerger` lowers the plain `byte_exact` from 3012 to 3008
  because the plain link's zlib block shifts by `-8` and four zlib literal
  pools change; it is not landed.

**Hypothesis / open**

* The next plain no-order frontier is `Ship::~Ship()` (`0x1749a4` original,
  `0x1749d8` ours).  The cause is `Ship::getCargoValue()` being emitted at
  `0x1749a4` in our plain link instead of `0x175270` (original); the `Ship`
  TU is the next per-TU source-order experiment.
* The ordering file still cannot be removed: the plain no-order build is
  `3012`, not `4498`.
* The `LodMeshMerger` reorder is expected to become a gain once the preceding
  `Ship`/`MGame` blocks are address-exact and the zlib shift disappears; that
  has not been demonstrated.
