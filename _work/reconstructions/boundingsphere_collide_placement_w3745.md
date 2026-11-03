# Section-order removal: `BoundingSphere::collide` placement (w3745)

Worker: w3745. Branch: `wave87-w3745`, based on master
`400eb82eccb98713ad842bb0d51aad2aa9c3190e` ("docs: lock RadioMessage
emission order"). Original: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`). All numbers below were produced in
this worktree with the commands shown. Scratch output is under `/tmp/w3745_bs/`.

This note closes the next open item after
`_work/reconstructions/radiomessage_ctor_placement_w3745.md`: the first plain
(no `--section-ordering-file`) mismatch at this HEAD is
`BoundingSphere::collide(float,float,float)` at `0x151d68` (ours `0x151ee8`).
Reordering the top-level definitions in `src/engine/math/BoundingSphere.cpp`
to the original emitted order makes the whole `BoundingSphere` block
address-exact in the plain link. The source change is gate-neutral.

## 0. Result

| build | gate byte/linked | plain byte_exact | exact addresses | exact-address prefix | first plain mismatch |
|---|---:|---:|---:|---:|---|
| before (HEAD `400eb82e`) | **4497 / 4497** | **2852** | **3456 / 4524** | **3305** | `BoundingSphere::collide(float,float,float)` `0x151d68` vs `0x151ee8` |
| after (this pass) | **4497 / 4497** | **2855** | **3461 / 4524** | **3320** | `Gun::setOffset(AbyssEngine::AEMath::Vector*)` `0x1522e4` vs `0x15236c` |

Gate unchanged (`4497/4497`, `RATCHET PASS`), `.text` `0x1337c8 @ 0x69440`,
`.rodata` 0 differing bytes, lint `CLEAN ✓`, drift 0.

The plain `byte_exact` gain is +3, not +5: the five functions that moved
(`collide`, `outerCollide`, `getCollisionNormal`, `update`,
`projectCollisionOnSurface`) all become address-exact, but `collide` and
`update` still differ in their `blx`/`b.w` PLT-slot operand in the plain link.
The three functions whose only remaining difference was their own address
(`outerCollide`, `getCollisionNormal`, `projectCollisionOnSurface`) flip to
`byte=True`. There are **no losses** (3 gained, 0 lost in the plain verify
report).

The entire `BoundingSphere` block is now at the original addresses in the
plain link:

| symbol | original | before | after |
|---|---:|---:|---:|
| `BoundingSphere::BoundingSphere(...)` (`C1`/`C2`) | `0x151d28` | `0x151d28` | `0x151d28` |
| `BoundingSphere::~BoundingSphere()` (`D1`/`D2`) | `0x151d64` | `0x151d64` | `0x151d64` |
| `BoundingSphere::collide(float,float,float)` | `0x151d68` | `0x151d68` | **`0x151d68`** |
| `BoundingSphere::outerCollide(float,float,float)` | `0x151da0` | `0x151e18` | **`0x151da0`** |
| `BoundingSphere::getCollisionNormal(Vector const&)` | `0x151e10` | `0x151e88` | **`0x151e10`** |
| `BoundingSphere::update(float,float,float)` | `0x151e70` | `0x151d68` | **`0x151e70`** |
| `BoundingSphere::projectCollisionOnSurface(Vector const&)` | `0x151e74` | `0x151d6c` | **`0x151e74`** |

## 1. Baseline

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
.ARM.extab       0x02628c @ 0x1a3b78   (original 0x26298, known -0xc)
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

Plain no-order baseline:

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_bs/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_bs/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_bs/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_bs/base_noorder.so
```

```text
byte_exact 2852  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3456/4524 same; prefix 3305;
            first (0x151d68, '_ZN14BoundingSphere7collideEfff', 0x151ee8)
```

## 2. Original order (binary witness)

`nm -nSD --defined-only` on the original gives the emitted `BoundingSphere`
order:

```text
00151d28 0000003c T _ZN14BoundingSphereC1Efffffff
00151d28 0000003c T _ZN14BoundingSphereC2Efffffff
00151d64 00000004 T _ZN14BoundingSphereD1Ev
00151d64 00000004 T _ZN14BoundingSphereD2Ev
00151d68 00000036 T _ZN14BoundingSphere7collideEfff
00151da0 00000070 T _ZN14BoundingSphere12outerCollideEfff
00151e10 00000060 T _ZN14BoundingSphere18getCollisionNormalERKN11AbyssEngine6AEMath6VectorE
00151e70 00000004 T _ZN14BoundingSphere6updateEfff
00151e74 000000ac T _ZN14BoundingSphere25projectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorE
```

The HD source-layout dump independently witnesses the same order
(`_work/original_layout/original_layout.json`,
`tu_symbols.gof2hd_1.1.19_ios["Classes/Game/BoundingSphere.cpp"]`):

```text
BoundingSphere(...), ~BoundingSphere(), ~BoundingSphere(),
collide, outerCollide, getCollisionNormal, update, projectCollisionOnSurface,
_typeinfo name, _vtable, _typeinfo
```

The older SD dump (`gof2sd_1.0.1_ios`) has a different order
(`outerCollide, update, getCollisionNormal, collide, ctor, ctor`), so the HD
dump is the relevant witness for the Android 2.0.16 build.

## 3. Baseline object order

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/engine/math/BoundingSphere.cpp.obj`
(section-header order = emission order; this TU has no weak template copies):

```text
BoundingSphere::BoundingSphere(...)
BoundingSphere::~BoundingSphere()
BoundingSphere::update(float, float, float)
BoundingSphere::projectCollisionOnSurface(Vector const&)
BoundingSphere::outerCollide(float, float, float)
BoundingSphere::getCollisionNormal(Vector const&)
BoundingSphere::collide(float, float, float)
```

The source file defined the functions in that same order. The original order
is a different permutation of the tail: `collide`, `outerCollide`,
`getCollisionNormal`, `update`, `projectCollisionOnSurface`.

## 4. Landed source change

Only the order of the top-level definitions in
`src/engine/math/BoundingSphere.cpp` changed; no body, declaration, or header
was edited (the vtable/declaration order in `BoundingSphere.h` is untouched).

```text
before (source / object order):
  ctor, dtor, update, projectCollisionOnSurface, outerCollide,
  getCollisionNormal, collide

after:
  ctor, dtor, collide, outerCollide, getCollisionNormal, update,
  projectCollisionOnSurface
```

The object order after the edit is exactly the original address order:

```text
BoundingSphere::BoundingSphere(...)
BoundingSphere::~BoundingSphere()
BoundingSphere::collide(float, float, float)
BoundingSphere::outerCollide(float, float, float)
BoundingSphere::getCollisionNormal(Vector const&)
BoundingSphere::update(float, float, float)
BoundingSphere::projectCollisionOnSurface(Vector const&)
```

## 5. Controlled A/B

Each row is a full `check` rebuild plus the plain no-order relink.

| # | source | gate byte/linked | plain byte | exact addrs | prefix | first plain mismatch |
|---|---|---:|---:|---:|---:|---|
| 0 | baseline | **4497 / 4497** | **2852** | 3456 | 3305 | `BoundingSphere::collide` `0x151d68` vs `0x151ee8` |
| 1 | full `BoundingSphere` reorder (landed) | **4497 / 4497** | **2855** | **3461** | **3320** | `Gun::setOffset(Vector*)` `0x1522e4` vs `0x15236c` |

Commands (same for every row):

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_bs/final_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_bs/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_bs/final_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_bs/final_noorder.so
```

Final gate:

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

Final plain no-order:

```text
byte_exact 2855  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3461/4524 same; prefix 3320;
            first (0x1522e4, '_ZN3Gun9setOffsetEPN11AbyssEngine6AEMath6VectorE', 0x15236c)
```

### 5.1 Gate `.so` A/B

The gate `.so` is byte-identical in `.text` and `.rodata`; only the build-id
and the `.ARM.exidx`/`.ARM.extab` record order for the reordered functions
change:

| section | before | after | differing bytes |
|---|---:|---:|---:|
| `.text` | `0x1337c8 @ 0x69440` | `0x1337c8 @ 0x69440` | 0 |
| `.rodata` | `0x41b67 @ 0x1c9e10` | `0x41b67 @ 0x1c9e10` | 0 |
| `.ARM.exidx` | `0x6f70 @ 0x19cc08` | `0x6f70 @ 0x19cc08` | 4 |
| `.ARM.extab` | `0x2628c @ 0x1a3b78` | `0x2628c @ 0x1a3b78` | 45 |
| `.note.gnu.build-id` | — | — | 20 |

`cmp -l` gives 69 differing bytes total (20 build-id + 4 + 45). All other
sections (including `.rel.plt`, `.plt`, `.got`, `.dynsym`, `.data.rel.ro`,
`.data`) are byte-identical, so the reorder does not perturb the gate PLT
order.

Against the original, the per-section differing-byte counts are unchanged by
this pass: `.text` 107941, `.ARM.exidx` 1731, `.rel.plt` 21, `.dynsym` 324,
and `.rodata` / `.plt` / `.got` / `.data.rel.ro` / `.data` 0. The reordered
EH records already differed by their absolute offsets, so the gate's
`byte_exact` count is unchanged.

Reproduce the A/B:

```sh
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_bs/gate_after.so
git stash push -- src/engine/math/BoundingSphere.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_bs/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745_bs/gate_base.so /tmp/w3745_bs/gate_after.so
cmp -l /tmp/w3745_bs/gate_base.so /tmp/w3745_bs/gate_after.so | wc -l   # 69
```

Measured hashes:

```text
base  073fcfdeb3e6151bcce559d4da02c5365e86411e8bad7967b844a86b9614e599
after eee00d579d9391e9da717ebdf87447533732af0ee43557dcc94f0ebfdf8bb9a6
```

### 5.2 Plain-link layout and why the gain is +3, not +5

Plain-link section layout:

| section | original | before | after |
|---|---:|---:|---:|
| `.text` | `0x1337c8 @ 0x69440` | `0x1337e8 @ 0x69440` | `0x1337e8 @ 0x69440` |
| `.ARM.exidx` | `0x6f70 @ 0x19cc08` | `0x6f60 @ 0x19cc28` | `0x6f68 @ 0x19cc28` |
| `.ARM.extab` | `0x26298 @ 0x1a3b78` | `0x2628c @ 0x1a3b88` | `0x2628c @ 0x1a3b90` |
| `.rodata` | `0x41b67 @ 0x1c9e10` | `0x41b67 @ 0x1c9e20` | `0x41b67 @ 0x1c9e20` |

The reorder gives `update` its own `.ARM.exidx` entry: in the baseline object
order the 4-byte `update` sits immediately after the 4-byte `dtor`, and the
plain link merges them into one CANTUNWIND entry; after the reorder `update`
is separated from `dtor` by `collide`/`outerCollide`/`getCollisionNormal`, so
it gets its own entry (`0x151e70`, `w1=0x80b0b0b0`, exactly as in the
original). The plain `.ARM.exidx` therefore grows by 8 bytes
(`0x6f60 -> 0x6f68`). The `.rodata` base
does **not** move further: both before and after it is `0x1c9e20` (the plain
`.text` is already `+0x20` vs the original, the known empty-`.text`/marker
artifact). Therefore there is no global literal-pool shift and no function
loses `byte_exact`.

The per-function diff of the two plain verify reports is exactly:

```text
GAIN 3
  BoundingSphere::outerCollide
  BoundingSphere::getCollisionNormal
  BoundingSphere::projectCollisionOnSurface
LOSS 0
```

`BoundingSphere::collide` and `BoundingSphere::update` are now at their
original addresses but are still not `byte=True` in the plain link because
their only remaining difference is a PLT-slot operand:

```text
orig  151d64: f049 be08  b.w 19b978
ours  151d64: f049 be18  b.w 19b998   # BoundingSphere::~BoundingSphere tail call
orig  151d9a: f049 bdf5  b.w 19b988
ours  151d9a: f049 be05  b.w 19b9a8   # BoundingSphere::collide tail call
```

This is the same plain-link PLT-order artifact documented in the earlier
section-order notes; `linked_exact` is true for both.

## 6. Next frontier

The first plain no-order mismatch is now the `Gun.cpp` overload order:

```text
original 0x1522e4  Gun::setOffset(AbyssEngine::AEMath::Vector*)
ours     0x1522e4  Gun::setOffset(int, int)
original 0x152328  Gun::setOffset(int, int)
ours     0x15236c  Gun::setOffset(AbyssEngine::AEMath::Vector*)
```

The original emits the `Vector*` overload before the `int,int` overload; our
object emits them in the opposite order. This is a separate TU
(`src/game/ship/Gun.cpp`) and was not changed in this pass.

## 7. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  `.rodata` 0 differing bytes, lint `CLEAN ✓`, drift 0.
* Baseline plain no-order is `byte 2852 / linked 4497`, `3456/4524`
  addresses, prefix `3305`; first mismatch
  `BoundingSphere::collide(float,float,float)` at `0x151d68` (ours
  `0x151ee8`).
* The original emits the `BoundingSphere` functions in the order listed in
  §2; the HD source-layout dump independently witnesses the same order. The
  baseline object emits them in source order, which differs.
* Reordering the definitions is gate-neutral (`4497/4497`, `.text` and
  `.rodata` byte-identical, `.rel.plt`/`.plt`/`.got`/`.dynsym`/`.data*`
  byte-identical, lint `CLEAN`, drift 0) and makes every `BoundingSphere`
  symbol address-exact in the plain link.
* The plain link improves `2852 -> 2855` byte-exact, `3456 -> 3461` exact
  addresses, `3305 -> 3320` exact-address prefix; the per-function diff is
  3 gains / 0 losses.
* The plain link's next mismatch is
  `Gun::setOffset(AbyssEngine::AEMath::Vector*)` at `0x1522e4` (ours
  `Gun::setOffset(int,int)` at the same address).
* The plain `.ARM.exidx` grows by 8 bytes because `collide` gets its own
  entry; the `.rodata` base is unchanged at `0x1c9e20`, so there is no
  global literal-pool shift.

**Hypothesis / still open**

* The plain `.text` `+0x20` and `.ARM.extab` `-0xc` are the known plain-link
  artifacts; until they are fixed, any TU reorder that adds an exidx entry
  can move `.ARM.exidx`/`.ARM.extab` bases. Here the `.rodata` 16-byte
  alignment absorbs it.
* The ordering file is still required: the plain link is `2855`, not `4497`.
* The next TU (`Gun.cpp`) needs its own `setOffset` overload-order pass under
  the full gate.
