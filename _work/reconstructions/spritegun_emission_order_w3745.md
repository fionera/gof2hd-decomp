# SpriteGun emission order (w3745)

Status: research note + landed pass. Worktree `wt/w3745`, branch `wave87-w3745`,
based on master `4a7a7a3f` (the TouchButton pass). Every number below was
measured in this worktree against `../_work/bins/android_2.0.16_libgof2hdaa.so`
with the tree's NDK r18b clang 7.0.2 / gold 1.12.

This is the next TU after the TouchButton pass (`b6069f46`). It reorders the
`SpriteGun.cpp` definitions so the object's `.text.*` sections are emitted in
the original's address order. The enforced gate is exactly neutral (the gate
`.so` is byte-identical); the plain no-order link gains four exact function
addresses and thirteen exact-address prefix entries.

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

The brief's 4496/4496 baseline is stale; this HEAD is **4498 / 4498**.

Gate ELF layout (`readelf -SW cmake-build-match/libgof2hdaa.so`):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78   (original: 0x26298, the known -12)
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

Plain no-order baseline (`relink_noorder.py` + `verify.py` + `addr_match.py`):

```text
compared 4524  avg 99.9%  byte_exact 3001  linked_exact 4498  fuzzy_perfect 4498
3858/4524 same; prefix 3817;
first (0x169abc, SpriteGun::render(), 0x169ace)
```

## 2. Original SpriteGun address order

From the original's `.dynsym`:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN9SpriteGun'
```

```text
0x169aa4  0x14  SpriteGun::SpriteGun(Gun*, int)
0x169ab8  0x02  SpriteGun::~SpriteGun()      # D1/D2 alias
0x169aba  0x02  SpriteGun::~SpriteGun()      # D0
0x169abc  0x02  SpriteGun::render()
0x169abe  0x06  SpriteGun::update(int)
0x169ac4  0x06  SpriteGun::setEnemies(Array<Player*>*)
0x169aca  0x06  SpriteGun::setEnemy(Player*)
```

The disassembly confirms the bodies and the order:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x169aa4 --stop-address=0x169ad0 \
  ../_work/bins/android_2.0.16_libgof2hdaa.so
```

```text
169aa4  SpriteGun::SpriteGun(Gun*, int)    20 B (vptr + field_0x4 = 0)
169ab8  SpriteGun::~SpriteGun()            2 B  bx lr
169aba  SpriteGun::~SpriteGun() (D0)       2 B  udf #254
169abc  SpriteGun::render()                2 B  bx lr
169abe  SpriteGun::update(int)             6 B  ldr r0,[r0,#8]; b.w <veneer>
169ac4  SpriteGun::setEnemies(...)         6 B  ldr r0,[r0,#8]; b.w <veneer>
169aca  SpriteGun::setEnemy(...)           6 B  ldr r0,[r0,#8]; b.w <veneer>
```

`SpriteGun.cpp` has no dump witness (`_work/original_layout/tu_mapping.md`:
`game/weapons/SpriteGun.cpp | no-witness(2.0.x)`), so the address order above is
the binary evidence.

## 3. Baseline object order

Before the pass, `SpriteGun.cpp.obj` emitted:

```text
SpriteGun::SpriteGun(Gun*, int)
SpriteGun::~SpriteGun()          # D2
SpriteGun::~SpriteGun()          # D0
SpriteGun::setEnemies(Array<Player*>*)
SpriteGun::update(int)
SpriteGun::setEnemy(Player*)
SpriteGun::render()
AbstractGun::isRocketGun()
AbstractGun::isBombGun()
AbstractGun::isMineGun()
AbstractGun::isAutoTurret()
```

This is exactly the source definition order (no forward-reference pull: the
ctor body only initialises `field_0x4`, and the four methods are independent).
The plain no-order link therefore put `render` at `0x169ace` instead of
`0x169abc`.

## 4. Source reorder

Only the four method definitions were permuted; no body, signature, comment or
table was edited:

```diff
@@
-void SpriteGun::setEnemies(Array<Player *> *enemies) {
-    this->gun->setEnemies(enemies);
+void SpriteGun::render() {
 }
 
 void SpriteGun::update(int elapsed) {
     this->gun->update(elapsed);
 }
 
-void SpriteGun::setEnemy(Player *enemy) {
-    this->gun->setEnemy(enemy);
+void SpriteGun::setEnemies(Array<Player *> *enemies) {
+    this->gun->setEnemies(enemies);
 }
 
-void SpriteGun::render() {
+void SpriteGun::setEnemy(Player *enemy) {
+    this->gun->setEnemy(enemy);
 }
```

The resulting source order is `ctor, dtor, render, update, setEnemies, setEnemy`.

## 5. Object order after

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/weapons/SpriteGun.cpp.obj`
now lists:

```text
SpriteGun::SpriteGun(Gun*, int)
SpriteGun::~SpriteGun()
SpriteGun::~SpriteGun()
SpriteGun::render()
SpriteGun::update(int)
SpriteGun::setEnemies(Array<Player*>*)
SpriteGun::setEnemy(Player*)
AbstractGun::isRocketGun()
AbstractGun::isBombGun()
AbstractGun::isMineGun()
AbstractGun::isAutoTurret()
```

The seven strong sections are in exactly the original address order.

## 6. Gate A/B

Both builds are the full `check` gate. The reorder is exactly gate-neutral:
the stripped gate `.so` is **byte-identical** before and after.

| build | byte_exact | linked_exact | .text | .rodata | lint | drift |
|---|---:|---:|---:|---:|---|---|
| baseline | 4498 | 4498 | 0x1337c8 @ 0x69440 | 0x41b67 @ 0x1c9e10 | CLEAN ✓ | 0 |
| SpriteGun reorder | **4498** | **4498** | 0x1337c8 @ 0x69440 | 0x41b67 @ 0x1c9e10 | CLEAN ✓ | 0 |

```sh
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_sg/gate_base.so
# apply the source reorder, rebuild
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_sg/gate_after.so
sha256sum /tmp/w3745_sg/gate_base.so /tmp/w3745_sg/gate_after.so
# b52269e10725c2561f0226625abc924943031be86f06a6eab30c4f3151983032  (both)
cmp -l /tmp/w3745_sg/gate_base.so /tmp/w3745_sg/gate_after.so | wc -l   # 0
```

The ordering file fixes the output address of every section, and the reorder
changes only the *input* section order, so the gate `.text`, `.ARM.exidx`,
`.ARM.extab` and `.rodata` are all unchanged. This is a stronger result than
the previous TU reorders, where exidx/extab bytes moved.

## 7. Plain no-order A/B

Same `relink_noorder.py` + `verify.py` + `addr_match.py` commands:

| plain no-order link | byte_exact | linked_exact | funcs at orig addr | exact-address prefix | first mismatch |
|---|---:|---:|---:|---:|---|
| baseline | 3001 | 4498 | 3858 / 4524 | 3817 | `SpriteGun::render()` `0x169abc` vs `0x169ace` |
| SpriteGun reorder | **3001** | 4498 | **3862 / 4524** | **3830** | `Array<Array<ImagePart*>*>::Array()` `0x16aa24` vs `0x16aa2a` |

The whole `SpriteGun` block is now address-exact in the plain no-order link:

```text
orig / ours
0x169aa4  SpriteGun::SpriteGun(Gun*, int)
0x169ab8  SpriteGun::~SpriteGun()   (D1/D2)
0x169aba  SpriteGun::~SpriteGun()   (D0)
0x169abc  SpriteGun::render()
0x169abe  SpriteGun::update(int)
0x169ac4  SpriteGun::setEnemies(Array<Player*>*)
0x169aca  SpriteGun::setEnemy(Player*)
```

The plain byte count does **not** rise because the three tail-call functions
(`update`, `setEnemies`, `setEnemy`) are still byte-false for a different
reason: their `b.w` targets are gold long-branch veneers in the `.text` tail,
and the whole tail is shifted `+0x20` in the no-order link.

| symbol | original `b.w` target | no-order `b.w` target | delta |
|---|---:|---:|---:|
| `SpriteGun::update` | `0x19c678` | `0x19c698` | `+0x20` |
| `SpriteGun::setEnemies` | `0x19c628` | `0x19c648` | `+0x20` |
| `SpriteGun::setEnemy` | `0x19c688` | `0x19c6a8` | `+0x20` |

The shift is pre-existing and not caused by this pass: the no-order link's
`.text` is `0x1337e8` (gate/original `0x1337c8`), and the last exported text
symbol `__cxa_demangle` is at `0x194030` in the original/gate but `0x194050`
(`+0x20`) in the no-order link. The veneer block sits after it, so every
veneer target inherits the `+0x20`. This is the same no-order `.text` size
delta recorded in `section_order_removal_w3745.md` §27.4.

Measured section content against the original (differing bytes):

| section | baseline no-order | reordered no-order |
|---|---:|---:|
| `.text` | 270,582 | **270,565** |
| `.ARM.exidx` | 8,507 | 8,507 |
| `.ARM.extab` | 128,303 | 128,303 |
| `.rodata` | 0 | 0 |

The 17-byte `.text` improvement is the smaller `b.w` immediates: the three
tail-call functions now sit at their original addresses, so the branch delta
to the shifted veneer block is `+0x20` instead of a larger, position-dependent
delta. No function crosses from `byte=false` to `byte=true`, hence the flat
`byte_exact` count.

## 8. Next frontier

The first plain no-order mismatch is now a weak-copy emission-order issue in
the next TU, `src/game/menu/SpaceLounge.cpp`:

```text
original 0x16a9be  ArrayReleaseClasses<Array<ImagePart*>*>
original 0x16aa0e  Array<Array<ImagePart*>*>::~Array()
original 0x16aa24  Array<Array<ImagePart*>*>::Array()
original 0x16aa40  ArraySetLength<Array<ImagePart*>*>(...)
original 0x16aa74  SpaceLounge::setHangarUpdate(bool)
original 0x16aa7c  SpaceLounge::startChat()

ours     0x16a9be  ArrayReleaseClasses<Array<ImagePart*>*>
ours     0x16aa0e  Array<Array<ImagePart*>*>::~Array()
ours     0x16aa24  SpaceLounge::setHangarUpdate(bool)
ours     0x16aa2a  Array<Array<ImagePart*>*>::Array()
ours     0x16aa46  ArraySetLength<Array<ImagePart*>*>(...)
ours     0x16aa7c  SpaceLounge::startChat()
```

The strong `SpaceLounge` functions (`ctor`, `init`, `introFinished`, `dtor`,
`setHangarUpdate`, `startChat`, ...) are already at their original addresses;
only the `Array<Array<ImagePart*>*>` weak copy is emitted after
`setHangarUpdate` instead of before it. This is a separate TU and was not
changed in this pass.

## 9. Proven vs hypothesis

**Proven by measurement**

- Baseline gate is **4498 / 4498**, `.text 0x1337c8 @ 0x69440`, `.rodata
  0x41b67 @ 0x1c9e10`, lint `CLEAN ✓`, drift 0.
- Baseline plain no-order is **3001 / 4498**, 3858 / 4524 at original
  addresses, prefix 3817, first mismatch `SpriteGun::render()` `0x169abc` vs
  `0x169ace`.
- The original `SpriteGun` address order is `ctor, ~SpriteGun (D1/D2),
  ~SpriteGun (D0), render, update, setEnemies, setEnemy` (§2).
- The reordered source makes the object's seven strong `.text.*` sections
  exactly that order (§5).
- The reorder is exactly gate-neutral: **4498 / 4498**, every ELF section
  size/base unchanged, and the stripped gate `.so` is byte-identical
  (`sha256 b52269e1…`, `cmp` = 0).
- The plain no-order link improves to **3862 / 4524** at original addresses
  and prefix **3830**; the whole `SpriteGun` block is address-exact. Plain
  `byte_exact` stays **3001** because the three tail-call functions still
  branch to the `+0x20`-shifted veneer block; `.text` differing bytes drop
  270,582 -> 270,565.
- The `+0x20` veneer shift is the pre-existing no-order `.text` size delta
  (`0x1337e8` vs `0x1337c8`), witnessed by `__cxa_demangle` at `0x194050`
  instead of `0x194030`.

**Hypothesis / open**

- The ordering file still cannot be removed: the plain no-order build is
  3001, not 4498.
- The next per-TU frontier is the `Array<Array<ImagePart*>*>` weak copy in
  `SpaceLounge.cpp` (emitted after `setHangarUpdate` instead of before it).
- The `+0x20` no-order `.text` size delta is a separate archive/local-code
  issue; it must be closed before the three `SpriteGun` tail-call functions
  can become byte-exact in the plain link.

## 10. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + verify
mkdir -p /tmp/w3745_sg
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_sg/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_sg/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_sg/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_sg/base_noorder.so

# source reorder (ctor, dtor, render, update, setEnemies, setEnemy)
# then:
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_sg/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_sg/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_sg/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_sg/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/weapons/SpriteGun.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN9SpriteGun'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_sg/gate_base.so
git stash push -- src/game/weapons/SpriteGun.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_sg/gate_base2.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_sg/gate_after.so
sha256sum /tmp/w3745_sg/gate_base2.so /tmp/w3745_sg/gate_after.so
cmp -l /tmp/w3745_sg/gate_base2.so /tmp/w3745_sg/gate_after.so | wc -l   # 0
```
