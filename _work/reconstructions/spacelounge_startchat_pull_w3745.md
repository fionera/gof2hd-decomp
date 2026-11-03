# SpaceLounge `startChat` pull (w3745)

Status: research note + landed pass. Worktree `wt/w3745`, branch
`wave87-w3745`, based on master `8c93c9a8`. Every number below was measured
in this worktree against `../_work/bins/android_2.0.16_libgof2hdaa.so` with
the tree's NDK r18b clang 7.0.2 / gold 1.12.

This is the next plain no-order frontier recorded in
`_work/reconstructions/spacelounge_weak_copy_order_w3745.md` §7: after the
weak-copy pass the first plain no-order mismatch was
`SpaceLounge::refresh()` (`0x16e2ee` original, `0x17076c` ours), because
`startChat()`'s body pulled `getSoundId`, `getSpecificSoundForRace` and
`onKeyPress` ahead of `refresh`. The fix is a pure definition move: put
`startChat()` **after** `getSpecificSoundForRace()` in the .cpp. Its section
is still created early by `init()`'s body (so its linked address is
unchanged), but its body is emitted late, after all three callees, so it no
longer pulls them.

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
.ARM.extab       0x02628c @ 0x1a3b78   (original: 0x26298, the known -12)
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

Plain no-order baseline (`relink_noorder.py` + `verify.py` + `addr_match.py`):

```text
compared 4524  avg 99.9%  byte_exact 3003  linked_exact 4498  fuzzy_perfect 4498
3865/4524 same; prefix 3834;
first (0x16e2ee, '_ZN11SpaceLounge7refreshEv', 0x17076c)
```

## 2. The target order and the pull

Original `.text` order of the `SpaceLounge` block (`nm -nSD --defined-only`):

```text
0x16aa74  SpaceLounge::setHangarUpdate(bool)
0x16aa7c  SpaceLounge::startChat()
0x16e2ee  SpaceLounge::refresh()
0x16e2f0  SpaceLounge::onKeyPress(int)
0x170478  SpaceLounge::updateScreenPositions()
0x170738  SpaceLounge::update(int)
0x170bec  SpaceLounge::draw()
0x170d38  SpaceLounge::drawLounge()
0x1714d4  SpaceLounge::draw3DShip()
0x1714e0  SpaceLounge::checkLocationMode()
0x1714e4  SpaceLounge::mapMode()
0x1714ea  SpaceLounge::listMode()
0x1714f8  SpaceLounge::getSoundId(Agent*)
0x17177c  SpaceLounge::getSpecificSoundForRace(int, int, bool)
0x1717f0  SpaceLounge::hangarNeedsUpdate()
0x1717f8  SpaceLounge::OnTouchBegin(int, int)
...
```

The baseline `SpaceLounge.cpp.obj` `.text.*` order around the block was:

```text
...
Array<ImagePart*>::Array()
SpaceLounge::setHangarUpdate(bool)
Array<TouchButton*>::Array()
void ArraySetLength<TouchButton*>(unsigned int, Array<TouchButton*>&)
SpaceLounge::startChat()
SpaceLounge::getSoundId(Agent*)                 <-- pulled
AbyssEngine::String::String(int)
SpaceLounge::getSpecificSoundForRace(int, int, bool)  <-- pulled
SpaceLounge::onKeyPress(int)                    <-- pulled
SpaceLounge::refresh()
SpaceLounge::updateScreenPositions()
...
SpaceLounge::listMode()
SpaceLounge::hangarNeedsUpdate()
```

`startChat`'s body calls `getSoundId` (16 calls), `getSpecificSoundForRace`
(2 calls) and `onKeyPress` (1 call); the original disassembly confirms the
calls and that `refresh` (`0x16e2ee`, 2 bytes `bx lr`) immediately follows
`startChat`:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x16aa7c --stop-address=0x16e2ee \
  ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E 'getSoundId|getSpecificSoundForRace|onKeyPress'
$OD -d --start-address=0x16e2e8 --stop-address=0x16e2f4 \
  ../_work/bins/android_2.0.16_libgof2hdaa.so
```

```text
16ae40: blx 67d4c <_ZN11SpaceLounge10getSoundIdEP5Agent@plt>
16b1ea: blx 67d58 <_ZN11SpaceLounge23getSpecificSoundForRaceEiib@plt>
16c8ae: blx 67d64 <_ZN11SpaceLounge10onKeyPressEi@plt>
...
16e2ea: blx 5eb54 <_Unwind_Resume@plt>
16e2ee: 4770  bx lr                 <_ZN11SpaceLounge7refreshEv>
16e2f0: b5f0  push {r4, r5, r6, r7, lr}  <_ZN11SpaceLounge10onKeyPressEi>
```

So the original's `startChat` body did **not** create the
`getSoundId`/`getSpecificSoundForRace`/`onKeyPress` sections ahead of
`refresh`, even though it calls them.

## 3. The clang mechanism: section creation vs body emission

With `-ffunction-sections`, clang creates a function's section at its first
**reference** (a call from an earlier-emitted body), but emits the function's
**body** at its out-of-line definition. The body emission is what creates the
sections of its callees. A function that is first referenced early but
defined late therefore lands early, while its callees are created late.

Controlled synthetic (`/tmp`, not committed), compiled with the build's exact
flags:

```cpp
extern void ext(int); extern int g;
struct A { int v; void ctor(); void intro(); void dtor(); void init();
           void set(bool); void startChat(); void refresh(); void onKeyPress();
           void updateScreen(); void getSoundId(); void getSpecific(); };
void A::ctor() { init(); ext(1); g += 1; }
void A::intro() { ext(2); g += 2; }
void A::dtor() { ext(3); g += 3; }
void A::init() { set(0); startChat(); ext(4); g += 4; }
void A::set(bool x) { v = x; }
void A::refresh() { ext(6); g += 6; }
void A::onKeyPress() { ext(7); g += 7; }
void A::updateScreen() { ext(8); g += 8; }
void A::getSoundId() { ext(9); g += 9; }
void A::getSpecific() { ext(10); g += 10; }
void A::startChat() { getSoundId(); getSpecific(); onKeyPress(); ext(5); g += 5; }
```

`readelf -SW` gives:

```text
A::ctor()
A::init()
A::intro()
A::dtor()
A::set(bool)
A::startChat()      <-- section created by init()'s body
A::refresh()
A::onKeyPress()
A::updateScreen()
A::getSoundId()
A::getSpecific()
```

`startChat` is created early (by `init`'s body, after `set`) but its body is
emitted last, so its callees are already emitted and are not pulled. This is
exactly the target shape. The same synthetic with `startChat` defined before
`refresh` emits `startChat, getSoundId, getSpecific, onKeyPress, refresh` —
the baseline pull.

## 4. Landed source change

Pure permutation of top-level definitions in `src/game/menu/SpaceLounge.cpp`
(2377 lines before and after; `diff <(sort old) <(sort new)` is empty). The
`startChat` definition (414 lines) moves from immediately after
`setHangarUpdate` to immediately after `getSpecificSoundForRace`, before
`hangarNeedsUpdate`:

```text
before: ... setHangarUpdate, startChat, refresh, onKeyPress, ..., listMode,
              getSoundId, getSpecificSoundForRace, hangarNeedsUpdate, ...
after:  ... setHangarUpdate, refresh, onKeyPress, ..., listMode,
              getSoundId, getSpecificSoundForRace, startChat, hangarNeedsUpdate, ...
```

No body, signature, comment or table is edited. `init()` still calls
`startChat()` at the same source point, so `startChat`'s section is still
created by `init`'s body at the same object position.

### 4.1 Object order after

```text
...
Array<ImagePart*>::Array()
SpaceLounge::setHangarUpdate(bool)
Array<TouchButton*>::Array()
void ArraySetLength<TouchButton*>(unsigned int, Array<TouchButton*>&)
SpaceLounge::startChat()
SpaceLounge::refresh()
SpaceLounge::onKeyPress(int)
AbyssEngine::String::String(int)          <-- weak copy, deduplicated in the link
SpaceLounge::updateScreenPositions()
SpaceLounge::update(int)
SpaceLounge::draw()
SpaceLounge::drawLounge()
SpaceLounge::draw3DShip()
SpaceLounge::checkLocationMode()
SpaceLounge::mapMode()
SpaceLounge::listMode()
SpaceLounge::getSoundId(Agent*)
SpaceLounge::getSpecificSoundForRace(int, int, bool)
SpaceLounge::hangarNeedsUpdate()
SpaceLounge::OnTouchBegin(int, int)
...
```

The strong-function order is now exactly the original `.text` order; the
remaining `Array<TouchButton*>::Array` / `ArraySetLength<TouchButton*>` /
`String::String(int)` entries are weak copies that the linker deduplicates to
earlier TUs, so they do not appear in the linked `.text`.

## 5. Gate A/B

Both builds are the full `check` gate. The gate counts are exactly neutral;
the gate `.so` differs only in the build-id, 8 `.ARM.exidx` bytes and 1601
`.ARM.extab` bytes (the LSDA record order follows the body emission order).
`.text`, `.rodata`, `.data`, `.data.rel.ro`, `.dynsym`, `.dynstr`, `.hash`,
`.rel.dyn` and `.rel.plt` are byte-identical.

| build | byte_exact | linked_exact | .text | .rodata | lint | drift |
|---|---:|---:|---:|---:|---|---|
| baseline | 4498 | 4498 | 0x1337c8 @ 0x69440 | 0x41b67 @ 0x1c9e10 | CLEAN ✓ | 0 |
| landed | **4498** | **4498** | 0x1337c8 @ 0x69440 | 0x41b67 @ 0x1c9e10 | CLEAN ✓ | 0 |

```sh
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_sl/gate_base.so   # 65d514df...
# apply the source move, rebuild
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_sl/gate_expA.so   # 0291e00c...
```

Content against the original (differing bytes over the common section):

| section | baseline | landed |
|---|---:|---:|
| `.text` | 107,936 | 107,936 |
| `.ARM.exidx` | 1,724 | **1,722** |
| `.ARM.extab` | 128,117 | **127,819** |
| `.rodata` | 0 | 0 |

## 6. Plain no-order A/B

Same `relink_noorder.py` + `verify.py` + `addr_match.py` commands:

| plain no-order link | byte_exact | linked_exact | funcs at orig addr | exact-address prefix | first mismatch |
|---|---:|---:|---:|---:|---|
| baseline | 3003 | 4498 | 3865 / 4524 | 3834 | `SpaceLounge::refresh()` `0x16e2ee` vs `0x17076c` |
| landed | **3007** | 4498 | **3880 / 4524** | **3856** | `MeshMerger::setMesh(int, signed char, unsigned short)` `0x173bf8` vs `0x173c20` |

The whole `SpaceLounge` block is now address-exact in the plain no-order
link, including `getSoundId`/`getSpecificSoundForRace` after `listMode`:

```text
orig / ours (plain no-order)
0x16aa74  SpaceLounge::setHangarUpdate(bool)
0x16aa7c  SpaceLounge::startChat()
0x16e2ee  SpaceLounge::refresh()
0x16e2f0  SpaceLounge::onKeyPress(int)
0x170478  SpaceLounge::updateScreenPositions()
0x170738  SpaceLounge::update(int)
0x170bec  SpaceLounge::draw()
0x170d38  SpaceLounge::drawLounge()
0x1714d4  SpaceLounge::draw3DShip()
0x1714e0  SpaceLounge::checkLocationMode()
0x1714e4  SpaceLounge::mapMode()
0x1714ea  SpaceLounge::listMode()
0x1714f8  SpaceLounge::getSoundId(Agent*)
0x17177c  SpaceLounge::getSpecificSoundForRace(int, int, bool)
0x1717f0  SpaceLounge::hangarNeedsUpdate()
0x1717f8  SpaceLounge::OnTouchBegin(int, int)
0x171928  SpaceLounge::OnTouchMove(int, int)
0x171a48  SpaceLounge::OnTouchEnd(int, int)
0x173760  SpaceLounge::OnRender3D()
0x173792  SpaceLounge::OnRenderBG()
```

### 6.1 Control: `startChat` after `onKeyPress` only

To prove the mechanism is the *body* emission point, a second variant moved
`startChat` to just after `onKeyPress` (before `updateScreenPositions`). Its
body is then emitted after `onKeyPress` but before `getSoundId` /
`getSpecificSoundForRace`, so those two are still pulled:

```text
object: startChat, refresh, onKeyPress, String::String(int),
        getSoundId, getSpecificSoundForRace, updateScreenPositions, ...
```

| variant | gate byte/linked | plain byte | funcs at orig addr | prefix | first mismatch |
|---|---:|---:|---:|---:|---|
| baseline (`startChat` before `refresh`) | 4498 / 4498 | 3003 | 3865 | 3834 | `refresh` |
| control (`startChat` after `onKeyPress`) | 4498 / 4498 | 3003 | 3867 | 3836 | `updateScreenPositions` |
| landed (`startChat` after `getSpecificSoundForRace`) | 4498 / 4498 | **3007** | **3880** | **3856** | `MeshMerger::setMesh` |

The control is only marginally better than baseline and clearly worse than
the landed move, which confirms that the body must be emitted after *all*
three callees, not just `onKeyPress`.

Note that the HD iOS source-layout dump
(`_work/original_layout/original_layout.json`, `gof2hd_1.1.19_ios`,
`Classes/Game/SpaceLounge.cpp`) lists `setHangarUpdate, refresh, onKeyPress,
startChat, updateScreenPositions, ..., getSoundId, getSpecificSoundForRace,
hangarNeedsUpdate` — i.e. the *control* placement, not the landed one. The
iOS build's startChat body evidently did not pull `getSoundId` /
`getSpecificSoundForRace` either, so the dump is not an order oracle for the
Android 2.0.16 binary here; the Android `.text` addresses are.

## 7. Next frontier

The first plain no-order mismatch is now the next TU, `MeshMerger`:

```text
original 0x173bf8  MeshMerger::setMesh(int, signed char, unsigned short)
ours     0x173c20  MeshMerger::setMesh(int, signed char, unsigned short)
```

Original `MeshMerger` order: ctor(Array), ctor(int), `setMesh`, `setLod`,
`setMatrix`, `setEnabled`, `init`, `transformMesh`, `update`, `render`,
`~MeshMerger`. Our plain object order is ctor, ctor, `~MeshMerger`,
`setMatrix`, `setLod`, `setMesh`, `setEnabled`, `render`, `update`, `init`,
`transformMesh`. This is a separate TU (`src/game/mesh/MeshMerger.cpp`) and
was not changed in this pass.

## 8. Proven vs hypothesis

**Proven by measurement**

- Baseline gate is `4498 / 4498`, `.text 0x1337c8 @ 0x69440`, `.rodata
  0x41b67 @ 0x1c9e10`, lint `CLEAN ✓`, drift 0; plain no-order is
  `3003 / 4498`, 3865 / 4524 at original addresses, prefix 3834, first
  mismatch `SpaceLounge::refresh()` `0x16e2ee` vs `0x17076c`.
- The original `SpaceLounge` order is `setHangarUpdate, startChat, refresh,
  onKeyPress, updateScreenPositions, ..., listMode, getSoundId,
  getSpecificSoundForRace, hangarNeedsUpdate, ...`; `refresh` immediately
  follows `startChat` (`0x16e2ee`).
- The baseline object order is `setHangarUpdate, startChat, getSoundId,
  String::String(int), getSpecificSoundForRace, onKeyPress, refresh, ...`;
  `startChat`'s body pulls its three callees.
- Moving the `startChat` definition after `getSpecificSoundForRace` makes the
  object order `startChat, refresh, onKeyPress, ..., listMode, getSoundId,
  getSpecificSoundForRace, hangarNeedsUpdate`; the gate is unchanged
  (`4498 / 4498`, every section size/base exact, `.text`/`.rodata`
  byte-identical) and the gate `.so` differs only in build-id,
  8 `.ARM.exidx` bytes and 1601 `.ARM.extab` bytes.
- The plain no-order link improves to `3007 / 4498`, 3880 / 4524 at original
  addresses, prefix 3856; the whole `SpaceLounge` block is address-exact and
  the first mismatch moves to `MeshMerger::setMesh`.
- The control (`startChat` after `onKeyPress` only) leaves
  `getSoundId`/`getSpecificSoundForRace` pulled and gives only
  `3003 / 4498`, 3867 / 4524, prefix 3836, first mismatch
  `updateScreenPositions`.
- The `.ARM.exidx`/`.ARM.extab` content against the original also improves
  (1724 -> 1722 and 128,117 -> 127,819 differing bytes).

**Hypothesis / open**

- The original source almost certainly defined `startChat` after
  `getSpecificSoundForRace` (or otherwise arranged for its body to be emitted
  after all three callees). The `.text` order alone cannot distinguish a
  plain input-order link from an ordering-file link, but the deferred-body
  rule reproduces the original block exactly.
- The ordering file still cannot be removed: the plain no-order build is
  3007, not 4498.
- `MeshMerger` is the next per-TU source-order experiment; it was not tested
  or landed here.

## 9. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order baseline
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

# apply the source move (startChat after getSpecificSoundForRace), then:
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_sl/expA_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_sl/verify_expA \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_sl/expA_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_sl/expA_noorder.so

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
# 65d514df... (baseline) vs 0291e00c... (landed)
```
