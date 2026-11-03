# SpaceLounge weak-copy order (w3745)

Status: research note + landed pass. Worktree `wt/w3745`, branch
`wave87-w3745`, based on master `de63a206`. Every number below was measured
in this worktree against `../_work/bins/android_2.0.16_libgof2hdaa.so` with
the tree's NDK r18b clang 7.0.2 / gold 1.12.

This is the next plain no-order frontier recorded in
`_work/reconstructions/spritegun_emission_order_w3745.md` §8: the
`Array<Array<ImagePart*>*>` weak copies in `SpaceLounge.cpp` were emitted
after `SpaceLounge::setHangarUpdate(bool)` instead of before it. The fix is
gate-neutral (the gate `.so` is byte-identical) and moves the plain no-order
frontier past the whole `setHangarUpdate`/`startChat` block.

## 1. Baseline

`cmake --preset default`, then `cmake --build cmake-build-match --target check -j8`
(plus `lint` and `drift`):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout:

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78
.rodata          0x041b67 @ 0x1c9e10
.data            0x00296c @ 0x214000
```

Plain no-order baseline (`relink_noorder.py` + `verify.py` + `addr_match.py`):

```text
compared 4524  avg 99.9%  byte_exact 3001  linked_exact 4498  fuzzy_perfect 4498
3862/4524 same; prefix 3830;
first (0x16aa24, '_ZN5ArrayIPS_IP9ImagePartEEC2Ev', 0x16aa2a)
```

## 2. Original order

Original `.dynsym` (`nm -nSD --defined-only`):

```text
0016a894 00000006 T SpaceLounge::introFinished()
0016a89a 00000124 T SpaceLounge::~SpaceLounge()
0016a9be 00000050 W void ArrayReleaseClasses<Array<ImagePart*>*>(Array<Array<ImagePart*>*>&)
0016aa0e 00000016 W Array<Array<ImagePart*>*>::~Array()
0016aa24 0000001c W Array<Array<ImagePart*>*>::Array()
0016aa40 00000034 W void ArraySetLength<Array<ImagePart*>*>(unsigned int, Array<Array<ImagePart*>*>&)
0016aa74 00000006 T SpaceLounge::setHangarUpdate(bool)
0016aa7c 00003872 T SpaceLounge::startChat()
```

The HD source-layout dump (`_work/original_layout/original_layout.json`,
`gof2hd_1.1.19_ios`, `Classes/Game/SpaceLounge.cpp`) independently lists the
same relative order for the symbols it contains:

```text
SpaceLounge::SpaceLounge()
SpaceLounge::init()
void ArrayRemove<Agent*>(Agent*, Array<Agent*>&)
SpaceLounge::SpaceLounge()
SpaceLounge::introFinished()
SpaceLounge::~SpaceLounge()
void ArrayReleaseClasses<Array<ImagePart*>*>(Array<Array<ImagePart*>*>&)
SpaceLounge::~SpaceLounge()
void ArraySetLength<Array<ImagePart*>*>(unsigned int, Array<Array<ImagePart*>*>&)
SpaceLounge::setHangarUpdate(bool)
SpaceLounge::refresh()
SpaceLounge::onKeyPress(int)
SpaceLounge::startChat()
...
```

So `setHangarUpdate` is after the `Array<Array<ImagePart*>*>` weak copies and
before `refresh`/`onKeyPress`/`startChat`.

## 3. Why the baseline object order was wrong

`-ffunction-sections` creates a function's section at its first reference:
the definition, or an earlier call site in a body that is emitted first. In
the baseline `SpaceLounge.cpp.obj` the relevant order was:

```text
...
void ArrayReleaseClasses<Array<ImagePart*>*>(Array<Array<ImagePart*>*>&)
Array<Array<ImagePart*>*>::~Array()
void ArrayReleaseClasses<AbyssEngine::AEMath::Vector*>(...)
Array<AbyssEngine::AEMath::Vector*>::~Array()
AbyssEngine::EaseInOutMatrix::~EaseInOutMatrix()
__clang_call_terminate
SpaceLounge::setHangarUpdate(bool)          <-- emitted at its definition
Array<Array<ImagePart*>*>::Array()          <-- emitted by init()'s body
void ArraySetLength<Array<ImagePart*>*>(...)
...
SpaceLounge::startChat()                    <-- emitted by init()'s call site
```

`setHangarUpdate` was defined before `init()` in the source, so its section
was created before `init()`'s body emitted the `Array<Array<ImagePart*>*>`
ctor and `ArraySetLength`. `startChat`'s section is created by the
`this->startChat()` call inside `init()`'s body, so it lands after the
templates.

## 4. Landed source change

Two edits in `src/game/menu/SpaceLounge.cpp`, no body changed:

1. Move the `SpaceLounge::setHangarUpdate` definition from before `init()` to
   after `init()` (immediately before `startChat()`).
2. In `init()`, replace the direct store

   ```cpp
   this->hangarUpdate = 0;
   ```

   with the call

   ```cpp
   this->setHangarUpdate(0);
   ```

The setter is trivial and is inlined to exactly the same instruction as the
direct store. The odr-use nevertheless creates the `setHangarUpdate` section
at the call site, i.e. between `ArraySetLength<Array<ImagePart*>*>` and
`startChat`. The generated `init()` code is unchanged; the original
disassembly at the same point is the same store:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x16a0b0 --stop-address=0x16a0d0 \
  ../_work/bins/android_2.0.16_libgof2hdaa.so
```

```text
16a0b8: blx 64584 <ChoiceWindow::ChoiceWindow()>
16a0bc: movs r4, #0
16a0be: strb.w r4, [r9, #53]        ; hangarUpdate = 0
16a0c2: ldr.w r0, [r9, #68]
```

There is no `bl setHangarUpdate` in the original `init()`; the call is
inlined. The same instruction is produced by the landed source.

### 4.1 Controlled synthetic confirmation

A minimal TU compiled with the build's exact flags reproduces the mechanism
(`/tmp`, not committed):

```cpp
#include "engine/core/Array.h"
extern void ext();
struct A {
    A();
    ~A();
    void init();
    void set(bool);
    void start();
    Array<Array<int *> *> *p;
    bool v;
};
A::A() { init(); }
A::~A() { delete p; }
void A::init() { p = new Array<Array<int *> *>(); ArraySetLength(3, *p); set(0); start(); }
void A::set(bool x) { this->v = x; }
void A::start() { ext(); }
```

`readelf -SW` gives exactly the target relative order:

```text
A::A()
A::init()
A::~A()
Array<Array<int*>*>::~Array()
Array<Array<int*>*>::Array()
void ArraySetLength<Array<int*>*>(unsigned int, Array<Array<int*>*>&)
A::set(bool)
A::start()
```

and the `set(0)` call is inlined to a direct `strb`. Without the call (plain
`this->v = 0;`), the same TU emits `A::set` before the `Array` ctor, i.e. the
old wrong order.

## 5. Gate A/B

Both builds are the full `check` gate. The reorder is exactly gate-neutral:
the stripped gate `.so` is **byte-identical** before and after.

| build | byte_exact | linked_exact | .text | .rodata | lint | drift |
|---|---:|---:|---:|---:|---|---|
| baseline | 4498 | 4498 | 0x1337c8 @ 0x69440 | 0x41b67 @ 0x1c9e10 | CLEAN ✓ | 0 |
| landed | **4498** | **4498** | 0x1337c8 @ 0x69440 | 0x41b67 @ 0x1c9e10 | CLEAN ✓ | 0 |

```sh
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_sl/gate_base.so
# apply the source change, rebuild
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_sl/gate_after.so
sha256sum /tmp/w3745_sl/gate_base.so /tmp/w3745_sl/gate_after.so
# c72226e83e182640ea683b40d9bc7bc254ba5dc533a41ce5ee7292c2ffd913dc  (both)
cmp -l /tmp/w3745_sl/gate_base.so /tmp/w3745_sl/gate_after.so | wc -l   # 0
```

The ordering file fixes every output address, so the gate `.text`,
`.ARM.exidx`, `.ARM.extab` and `.rodata` are all unchanged.

## 6. Plain no-order A/B

Same `relink_noorder.py` + `verify.py` + `addr_match.py` commands:

| plain no-order link | byte_exact | linked_exact | funcs at orig addr | exact-address prefix | first mismatch |
|---|---:|---:|---:|---:|---|
| baseline | 3001 | 4498 | 3862 / 4524 | 3830 | `Array<Array<ImagePart*>*>::Array()` `0x16aa24` vs `0x16aa2a` |
| landed | **3003** | 4498 | **3865 / 4524** | **3834** | `SpaceLounge::refresh()` `0x16e2ee` vs `0x17076c` |

The whole `setHangarUpdate`/`startChat` block is now address-exact in the
plain no-order link:

```text
orig / ours
0x16a9be  ArrayReleaseClasses<Array<ImagePart*>*>
0x16aa0e  Array<Array<ImagePart*>*>::~Array()
0x16aa24  Array<Array<ImagePart*>*>::Array()
0x16aa40  ArraySetLength<Array<ImagePart*>*>(...)
0x16aa74  SpaceLounge::setHangarUpdate(bool)
0x16aa7c  SpaceLounge::startChat()
```

The two functions that move from plain `byte=False` to `byte=True` are:

```text
_Z14ArraySetLengthIP5ArrayIP9ImagePartEEvjRS0_IT_E
_ZN5ArrayIPS_IP9ImagePartEEC2Ev
```

Measured `.text` content against the original (differing bytes over the
common `0x1337c8` prefix):

| section | baseline no-order | landed no-order |
|---|---:|---:|
| `.text` | 270,531 | **270,449** |
| `.ARM.exidx` | 8,507 | 8,507 |
| `.ARM.extab` | 128,303 | 128,303 |
| `.rodata` | 0 | 0 |

Plain no-order section layout is unchanged by the pass
(`.text 0x1337e8 @ 0x69440`, `.ARM.exidx 0x6f68 @ 0x19cc28`,
`.ARM.extab 0x2628c @ 0x1a3b90`, `.rodata 0x41b67 @ 0x1c9e20`).

## 7. Next frontier

The first plain no-order mismatch is now `SpaceLounge::refresh()`
(`0x16e2ee` original, `0x17076c` ours). This is a different mechanism:
`startChat()`'s body calls `getSoundId`, `getSpecificSoundForRace` and
`onKeyPress`, so their sections are created during `startChat`'s body, before
`refresh`'s definition. The original `.text` order instead places them after
`listMode` (`getSoundId` `0x1714f8`, `getSpecificSoundForRace` `0x17177c`),
which the original's section-ordering file can do but a plain object-order
link cannot without moving the source definitions (which would move
`startChat` too). This is a separate TU-order frontier and was not changed in
this pass.

## 8. Proven vs hypothesis

**Proven by measurement**

- Baseline gate is `4498 / 4498`, `.text 0x1337c8 @ 0x69440`, lint
  `CLEAN ✓`, drift 0; plain no-order is `3001 / 4498`, 3862 / 4524 at
  original addresses, prefix 3830, first mismatch
  `Array<Array<ImagePart*>*>::Array()` `0x16aa24` vs `0x16aa2a`.
- The original `SpaceLounge` block order is `ArrayReleaseClasses`,
  `~Array`, `Array` ctor, `ArraySetLength`, `setHangarUpdate`, `startChat`
  (dynsym and the iOS source-layout dump agree on the relative order).
- The baseline object order is `setHangarUpdate` before the `Array` ctor /
  `ArraySetLength` / `startChat`.
- Moving the `setHangarUpdate` definition after `init()` and replacing
  `this->hangarUpdate = 0;` with `this->setHangarUpdate(0);` makes the
  object order `Array` ctor, `ArraySetLength`, `setHangarUpdate`,
  `startChat`; the setter is inlined to the same `strb.w` instruction, and
  the gate `.so` is byte-identical (`sha256 c72226e8…`, `cmp` = 0).
- The plain no-order link improves to `3003 / 4498`, 3865 / 4524 at
  original addresses, prefix 3834; the whole `setHangarUpdate`/`startChat`
  block is address-exact and the two `Array<Array<ImagePart*>*>` copies
  become byte-exact. `.text` differing bytes drop 270,531 -> 270,449.
- The next plain no-order mismatch is `SpaceLounge::refresh()`
  (`0x16e2ee` original, `0x17076c` ours).

**Hypothesis / open**

- The original source almost certainly called `setHangarUpdate(0)` in
  `init()` rather than assigning the field directly. The inlined store is
  identical either way; the call-site section creation is the observable
  difference. The iOS dump's symbol order (setter before `refresh`) is
  consistent with the landed placement.
- The `refresh`/`onKeyPress`/`getSoundId` order is a call-site-pull problem
  in `startChat`; it is not fixed by this pass and may not be reachable in a
  plain object-order link without a section ordering file.
- The ordering file still cannot be removed: the plain no-order build is
  3003, not 4498.

## 9. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + verify
mkdir -p /tmp/w3745_sl
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_sl/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_sl/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_sl/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_sl/base_noorder.so

# apply the source change (move setHangarUpdate after init; call it from init)
# then:
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_sl/after2_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_sl/verify_after2 \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_sl/after2_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_sl/after2_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/menu/SpaceLounge.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN11SpaceLounge'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_sl/gate_after.so
git stash push -- src/game/menu/SpaceLounge.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_sl/gate_base2.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745_sl/gate_base2.so /tmp/w3745_sl/gate_after.so
cmp -l /tmp/w3745_sl/gate_base2.so /tmp/w3745_sl/gate_after.so | wc -l   # 0
```
