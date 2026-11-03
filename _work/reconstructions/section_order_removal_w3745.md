# Section-order removal convergence — w3745

Worker: w3745. Branch: `wave87-w3745`, based on master `3fab830e`.
Original: `../_work/bins/android_2.0.16_libgof2hdaa.so` (stripped, armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`).

This note measures what `--section-ordering-file` is still doing at this HEAD and
whether the plain (no-order) link can converge on it. All numbers below were
produced in this worktree with the commands shown. Scratch output is under
`/tmp/w3745/`.

## 1. Baseline

`cmake-build-match` was configured with `cmake --preset default`. Baseline
(`cmake --build cmake-build-match --target check -j8`, then `lint`, `drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (`readelf -S -W cmake-build-match/libgof2hdaa.so`):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.shstrtab        0x000117 @ 0x215af0
```

The brief's 4496 baseline is stale; this HEAD is 4497/4497.

## 2. Landed source change: `Level::getGasClouds` placement

Only change: move `Level::getGasClouds()` in `src/game/world/Level.cpp` from
before `Level::createSpace()` to immediately before `Level::createAsteroids()`
(i.e. after `createSpace()` in the .cpp).

```diff
@@ after Level::getStarSystem()
-Array<KIPlayer *> *Level::getGasClouds() {
-    return gasClouds;
-}
-
-
 void Level::createSpace() {
@@ after Level::createSpace()
+Array<KIPlayer *> *Level::getGasClouds() {
+    return gasClouds;
+}
+
+
 void Level::createAsteroids() {
```

The move is gate-neutral (`byte 4497 / linked 4497`, `RATCHET PASS`, lint
`CLEAN`, drift 0), but it is a strict improvement in the plain no-order metric.

### A/B measurement (plain link, no `--section-ordering-file`)

```sh
# before: git show HEAD:src/game/world/Level.cpp > src/game/world/Level.cpp
# after:  restore the move above
cmake --build cmake-build-match --target link_so -j8
python3 _work/reconstructions/w3696/relink_noorder.py OUT.so
python3 tools/verify/verify.py --build-dir VERIFY \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so --ours OUT.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so OUT.so
```

| plain no-order link | byte_exact | linked_exact | funcs at orig addr | exact-address prefix | first mismatch |
|---|---:|---:|---:|---:|---|
| before (`getGasClouds` before `createSpace`) | 2723 | 4497 | 3170 / 4524 | 2292 | `Array<KIPlayer*>::Array()` orig `0xbde4e` ours `0xbde54` |
| after (`getGasClouds` before `createAsteroids`) | **2981** | 4497 | **3173 / 4524** | **2296** | `Array<AbstractGun*>::Array()` orig `0xbe680` ours `0xc01d4` |

Delta: `+258` byte_exact, `+3` exact addresses, `+4` exact-address prefix.

### Why the move helps (Level.o evidence)

`addr_match.py` reads the original's exported dynsym. Around the change the
original order is:

```text
0xbde48  Level::getStarSystem()
0xbde4e  Array<KIPlayer*>::Array()
0xbde6a  ArraySetLength<KIPlayer*>(...)
0xbde9e  Level::getGasClouds()
0xbdea4  Level::createGun(...)
```

The two objects were compiled directly with the build's exact command
(`compile_commands.json`) and their `.text.*` header order compared:

```text
before: getStarSystem, getGasClouds, Array<KIPlayer*>::Array(), ArraySetLength<KIPlayer*>, createGun, ...
after:  getStarSystem, Array<KIPlayer*>::Array(), ArraySetLength<KIPlayer*>, getGasClouds, createGun, ...
```

The after order matches the original's address order for this block. The
`Array<KIPlayer*>` template instantiations are lazily emitted by clang at a
point tied to the .cpp definition position of their first user
(`createSpace`, which does `new Array<KIPlayer *>()`). Moving `getGasClouds`
across `createSpace` in the .cpp moves the instantiation block relative to
`getGasClouds`.

## 3. Reordered-all no-order link

Experiment: permute every object's own `.text.*` section headers into
`cmake-build-match/section_order.txt` order, then relink with the same gold line
minus `--section-ordering-file`. The object link order (`link_order.rsp`) is
left unchanged.

```sh
mkdir -p /tmp/w3745/confirm/reordered2
python3 - <<'PY'
import os, subprocess
src = 'cmake-build-match/exidx_markers/link_order.rsp'
lines = [l.strip() for l in open(src) if l.strip()]
out = []
for i, p in enumerate(lines):
    dst = f'/tmp/w3745/confirm/reordered2/{i:03d}_{os.path.basename(p)}'
    subprocess.run(['python3',
                    '_work/reconstructions/w3696/reorder_elf_sections.py',
                    p, dst, 'cmake-build-match/section_order.txt'], check=True)
    out.append(dst)
open('/tmp/w3745/confirm/reordered2/link_order.rsp', 'w').write('\n'.join(out) + '\n')
PY
python3 _work/reconstructions/w3696/relink_noorder.py \
  /tmp/w3745/confirm/reordered2/noorder.so \
  /tmp/w3745/confirm/reordered2/link_order.rsp
# verify.py + addr_match.py as above
```

Result:

```text
byte_exact 4497  linked_exact 4497  fuzzy_perfect 4497
4524/4524 same; prefix 4524; first None
```

So, **given the existing object link order**, per-object `.text.*` section
permutation alone reproduces the ordering-file gate exactly. The ordering
file's residual contribution at this HEAD is per-object section order.

Content diff between the gate `.so` and this reordered no-order `.so`
(section sizes/addresses are identical; only these byte ranges differ):

| range | bytes | what |
|---|---:|---|
| `0x1dd..0x1f0` | 20 | `.note.gnu.build-id` |
| `0x19ee65..0x19eea8` | 32 | `.ARM.exidx` content, around `pConstToNonConst` (`0x947f0`, NdkAutosave.cpp) |
| `0x215af3..0x215c06` | 259 | `.shstrtab` (same string multiset, different order) |
| `0x215c31..0x216019` | ~40×1–2 | section-header `sh_name` indices |

The 32-byte `.ARM.exidx` difference is the known `NdkHooks`/`NdkAutosave`
marker/interleaving issue: the CANTUNWIND entry is for `pConstToNonConst`
(`0x947f0`); the reordered no-order link has an extra
`EXIDX_CANTUNWIND` entry at `0x947f0` relative to the gate. Permuting
`.ARM.exidx.text.*` sections as well gave the same 4497/4497 and the same
32-byte diff, so exidx section order is not the cause. Root cause not proven
(see §6).

## 4. Object link order still matters

The reordered experiment keeps `link_order.rsp` unchanged. Reversing the object
order is *not* neutral:

| variant | ordering file | object order | byte_exact | linked_exact | prefix |
|---|---|---:|---:|---:|---:|
| gate | yes | normal | 4497 | 4497 | 4524 |
| reordered objects | no | normal | 4497 | 4497 | 4524 |
| reordered objects | no | reversed | 1384 | 4496 | 0 |
| unreordered objects | no | reversed | 1358 | 4496 | 0 |
| unreordered objects | yes | reversed | 1358 | 4496 | 226 |

So `--section-ordering-file` does not fully override object link order; the
build's `link_order.rsp` (original source order) is still required for
`.rodata`/literal-pool placement. The correct statement is: *with the existing
link order*, the ordering file's remaining job is per-object section order.

## 5. Convergence gap

Using `section_order.txt` (generated by `tools/gen_section_order.py` from the
original dynsym address order, with local functions slotted by object source
order) as the target:

- `192 / 193` objects need at least one `.text.*` section moved.
- `837` out-of-order adjacent pairs across `4931` `.text.*` sections.
- Restricting to sections whose symbol is in the original dynsym:
  `613` adjacent inversions across `4690` exported `.text.*` sections, in
  `140 / 193` objects.

Worst objects by exported-section inversions:

```text
32/56  Array.cpp
28/98  Ship.cpp
28/49  Agent.cpp
20/41  TargetFollowCamera.cpp
17/58  Hud.cpp
17/36  TouchButton.cpp
15/129 Level.cpp
15/37  PlayerGasCloud.cpp
14/29  ArrayInstantiations.cpp
14/29  BluePrint.cpp
```

Most of the inversions are lazy template instantiations interleaved with the
strong functions, not the strong-function order itself. In `Level.o` the strong
`Level::*` functions are already in the original address order; the inversions
are the `Array<...>` instantiation blocks.

## 6. Proven vs hypothesis

**Proven by measurement in this worktree**

- Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN`, drift 0.
- Moving `Level::getGasClouds()` from before `createSpace()` to before
  `createAsteroids()` is gate-neutral and improves the plain no-order link from
  `2723` to `2981` byte-exact (`+258`), exact addresses `3170 -> 3173`,
  prefix `2292 -> 2296`.
- The move makes the `Array<KIPlayer*>` instantiation block precede
  `getGasClouds` in `Level.o`, matching the original address order
  (`0xbde4e`, `0xbde6a`, `0xbde9e`).
- With the existing object link order, per-object `.text.*` permutation alone
  (no `--section-ordering-file`) gives `4497/4497` and `4524/4524` exact
  addresses.
- Object link order still matters: reversing it drops to `1358/4496` even with
  the ordering file.
- The next plain no-order blocker is `Array<AbstractGun*>::Array()`:
  original `0xbe680`, ours `0xc01d4`. In `Level.o` the original wants
  `Array<AbstractGun*>::Array()`/`ArrayAdd<AbstractGun*>` before
  `createStaticObject` (`0xbe680`, `0xbe69c`, `0xbe6c0`), while clang emits
  them after `createStaticObject` and before `createShip`.

**Still hypothesis / open**

- The exact clang rule that decides where a lazy implicit instantiation is
  emitted relative to the strong functions is not fully characterized. It is
  clearly sensitive to the .cpp definition position of the first user
  (`getGasClouds` move proves that), but a simple "after the first user" rule
  does not explain every block in `Level.o`.
- The 32-byte `.ARM.exidx` `pConstToNonConst` (NdkAutosave.cpp) CANTUNWIND difference in
  the reordered no-order link is not explained; permuting exidx sections did
  not change it.
- No claim is made that the ordering file can be deleted today. It cannot:
  the plain link is still `2981`, and 192/193 objects have out-of-order
  sections.

## 7. Lazy-instantiation placement: `Array<AbstractGun*>` (2026-11-02)

This section isolates the next plain no-order blocker from §6: the lazy template
instantiation `Array<AbstractGun*>::Array()` and `ArrayAdd<AbstractGun*>`.
All numbers are from this worktree at the §1 baseline (`byte 4497 / linked 4497`,
`.text 0x1337c8 @ 0x69440`; lint `CLEAN ✓`, drift `0`).

### 7.1 Target order vs the current `Level.o` order

`cmake-build-match/section_order.txt` is the original-address witness.  The
relevant `Level` subsequence is:

```text
createGun
Array<AbstractGun*>::Array()
ArrayAdd<AbstractGun*>
createStaticObject
ArrayAdd<KIPlayer*>
createShip
createRoute
setPlayerRoute
createRadioMessages
ArraySetLength<AbstractGun*>
ArraySetLength<Player*>
getBoundingVolume
Array<RadioMessage*>::Array()
ArraySetLength<RadioMessage*>
getPlayer
```

The current `Level.cpp.obj` section-header order is different:

```text
createGun
ArrayReleaseClasses<AEMath::Vector*>
Array<AEMath::Vector*>::~Array()
ArrayReleaseClasses<Array<AEMath::Vector*>*>
Array<Array<AEMath::Vector*>*>::~Array()
createStaticObject
ArrayAdd<KIPlayer*>
Array<AbstractGun*>::Array()
ArraySetLength<AbstractGun*>
ArrayAdd<AbstractGun*>
createShip
...
```

Reproduce the object order:

```sh
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/Level.cpp.obj \
  | awk '$2 ~ /^\.text\./ {print $2}' | sed 's/^\.text\.//'
sed -n '2400,2420p' cmake-build-match/section_order.txt
```

So the first plain no-order mismatch is exactly the first displaced lazy copy:
`Array<AbstractGun*>::Array()` at original `0xbe680`, ours `0xc01d4`
(plain no-order `byte 2981`, `linked 4497`, prefix `2296`).

### 7.2 Controlled source experiments

Each row is a full rebuild of this tree followed by the same plain no-order
relink.  `gate` is `cmake --build cmake-build-match --target check -j8`
(the ordering-file build); `plain` is `relink_noorder.py` + `verify.py` +
`addr_match.py`.

| experiment | gate byte/linked | plain byte | prefix | first plain mismatch |
|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | **2981** | **2296** | `Array<AbstractGun*>::Array()` `0xbe680` vs `0xc01d4` |
| A: move `createStaticObjects()` definition after `createGun()` | 4491 / 4497 | 2977 | 2297 | `ArrayAdd<AbstractGun*>` `0xbe69c` vs `0xbe6d0` |
| B: A + move `createStaticObject()` before `createGun()` + suppress the `Level.o` `ArraySetLength<AbstractGun*>` copy with `extern template` (definition in `ArrayInstantiations.cpp`) + move `createShip()` after `createRadioMessages()` | 4491 / 4497 | 2542 | 2299 | `ArrayAdd<KIPlayer*>` `0xc01f0` vs `0xc0fbc` |

Commands (same for every row):

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target link_so -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/lazy/noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/lazy/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/lazy/noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/lazy/noorder.so
```

The source moves were scratch edits only; the tree was restored to the baseline
before committing this note.  Experiment A is gate-regressive by 6
(`4497 -> 4491`), so it is not landed.  Experiment B uses an `extern template`
declaration in `Level.cpp` plus an explicit instantiation in
`ArrayInstantiations.cpp`; that is a TU-membership experiment, not a proposed
landing.

### 7.3 What the object order shows

The baseline `Level.o` order is not "emit each template at its first textual
use".  `createGun()` calls `Array<AbstractGun*>::Array()` and
`ArrayAdd<AbstractGun*>`, yet both land *after* `createStaticObject()` and
`ArrayAdd<KIPlayer*>`.  The latter two are first referenced by
`createStaticObjects()`, which is defined before `createGun()`.

Moving `createStaticObjects()` after `createGun()` (experiment A) removes
`createStaticObject`/`ArrayAdd<KIPlayer*>` from the queue that is flushed after
`createGun()`.  The ctor and `ArrayAdd<AbstractGun*>` then land immediately
after `createGun()` at their original addresses.  But the same move also emits
`ArraySetLength<AbstractGun*>` too early (between the ctor and `ArrayAdd`) and
pushes `createStaticObject` after `createShip`, so the plain no-order byte count
falls.

Suppressing the `Level.o` `ArraySetLength<AbstractGun*>` copy (experiment B)
removes the insertion and makes both `Array<AbstractGun*>::Array()` and
`ArrayAdd<AbstractGun*>` address-exact.  The next mismatch is
`ArrayAdd<KIPlayer*>`, which is still emitted after `createShip` instead of
before it.  Moving `createShip()` later in the source did not change that
placement.

### 7.4 Proven vs hypothesis

**Proven by measurement**

- The baseline plain no-order build is `byte 2981 / linked 4497`, prefix `2296`,
  first mismatch `Array<AbstractGun*>::Array()` at `0xbe680` vs `0xc01d4`.
- The target `Level` order places the ctor and `ArrayAdd<AbstractGun*>`
  immediately after `createGun()`, and places `ArraySetLength<AbstractGun*>`
  after `createRadioMessages()`.
- The current `Level.o` places the ctor/setlength/`ArrayAdd<AbstractGun*>`
  after `createStaticObject()` and `ArrayAdd<KIPlayer*>`.
- Moving `createStaticObjects()` after `createGun()` makes the ctor and
  `ArrayAdd<AbstractGun*>` address-exact in the plain link, but is
  gate-regressive (`4497 -> 4491`) and lowers plain `byte_exact` (`2981 ->
  2977`).
- Suppressing the `Level.o` setlength copy makes the ctor and
  `ArrayAdd<AbstractGun*>` exact, but `ArrayAdd<KIPlayer*>` remains after
  `createShip`; the plain link is `2542` and the gate is still `4491`.

**Hypothesis / open**

- The ctor/`ArrayAdd<AbstractGun*>` placement is controlled by clang's deferred
  instantiation queue: a strong function first referenced before `createGun`
  (`createStaticObject`) is emitted before the queued template copies.  The
  exact rule that decides when the queue is flushed is not fully characterized;
  moving `createShip()` did not move `ArrayAdd<KIPlayer*>`.
- If `ArrayAdd<KIPlayer*>` can be made to emit before `createShip` while
  `ArraySetLength<AbstractGun*>` stays out of the early queue (or is emitted at
  its original `0xc3dde` position), the plain no-order frontier should move past
  `0xc01f0`.  That has not been demonstrated.

## 8. `PlayerStatic::update` placement (2026-11-02)

Status: landed in this pass. This is the next frontier recorded in
`_work/reconstructions/item_extractItems_placement_w3745.md` §6. The section-ordering-file
gate stays at **byte 4497 / linked 4497** and the gate `.so` is **byte-identical** before and
after; the plain no-order link improves from **3019 / 4497** to **3020 / 4497**.

### 8.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`, `drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

The original's `PlayerStatic` block, from the binary's own `.dynsym`:

```text
0xe08cc  PlayerStatic::PlayerStatic(int, AEGeometry*, float, float, float)
0xe096c  PlayerStatic::~PlayerStatic()
0xe0970  PlayerStatic::~PlayerStatic()          # deleting destructor
0xe0980  PlayerStatic::update(int)              # 2 bytes: bx lr
0xe0982  PlayerStatic::getPosition()            # 54 bytes
0xe09b8  PlayerStatic::render()                 # 6 bytes
0xe09be  PlayerStatic::translate(Vector const&)
0xe09c0  PlayerStatic::collide(float, float, float)
0xe09c4  PlayerStatic::outerCollide(float, float, float)
```

This is independently witnessed by the HD source-layout dump
(`_work/original_layout/original_layout.json`: `Classes/Game/PlayerStatic.cpp` lists
`PlayerStatic`, `~PlayerStatic`, `update`, `getPosition`, `render`, `translate`, `collide`,
`outerCollide` in that order). The disassembly confirms `update` is the empty `bx lr` body:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xe096c --stop-address=0xe09cc \
  ../_work/bins/android_2.0.16_libgof2hdaa.so
```

The baseline `PlayerStatic.cpp.obj` section-header order was:

```text
PlayerStatic::PlayerStatic
PlayerStatic::~PlayerStatic()   # D2
PlayerStatic::~PlayerStatic()   # D0
PlayerStatic::render
PlayerStatic::getPosition
PlayerStatic::update
PlayerStatic::translate
PlayerStatic::collide
PlayerStatic::outerCollide
```

So the plain no-order link put `render` at `0xe0980` and `update` at `0xe09bc`. The first
plain no-order mismatch was exactly:

```text
original 0xe0980 PlayerStatic::update(int)
ours     0xe09bc PlayerStatic::update(int)
```

### 8.2 Compile-only placement variants

The exact `compile_commands.json` command for `PlayerStatic.cpp` was used with only the input
and output paths replaced. The top-level definitions were extracted by brace matching and
re-emitted in the requested order. `readelf -SW` then gives the object's `.text.*` order.

| source order | `PlayerStatic` object order around the block |
|---|---|
| baseline: `render, getPosition, update` | `render, getPosition, update` |
| A: `update, render, getPosition` | `update, render, getPosition` |
| B: `update, getPosition, render` | `update, getPosition, render` |
| C: `render, update, getPosition` | `render, update, getPosition` |

`PlayerStatic.cpp` has no lazy template instantiations or cross-references between these
methods, so the object's section order follows the source definition order directly. Variant B
is the original address order.

### 8.3 Landed source change

Move `update()` before `getPosition()` and move `render()` after `getPosition()`:

```diff
@@ after ~PlayerStatic()
-void PlayerStatic::render() {
-    geometry->render();
+void PlayerStatic::update(int dt) {
 }

 Vector PlayerStatic::getPosition() {
@@
-void PlayerStatic::update(int dt) {
+void PlayerStatic::render() {
+    geometry->render();
 }
```

The resulting object order is the target:

```text
PlayerStatic::PlayerStatic
PlayerStatic::~PlayerStatic()   # D2
PlayerStatic::~PlayerStatic()   # D0
PlayerStatic::update
PlayerStatic::getPosition
PlayerStatic::render
PlayerStatic::translate
PlayerStatic::collide
PlayerStatic::outerCollide
```

### 8.4 Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`) followed by
`verify.py` and `addr_match.py`.

| state | gate byte/linked | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---:|---|
| baseline | **4497 / 4497** | **3019** | 3223 / 4524 | 2657 | `PlayerStatic::update` `0xe0980` vs `0xe09bc` |
| A: move `update` before `render` only | not separately gated | 3019 | 3224 | 2658 | `PlayerStatic::getPosition` `0xe0982` vs `0xe0988` |
| B: landed (`update, getPosition, render`) | **4497 / 4497** | **3020** | **3226 / 4524** | **2778** | `SystemPathFinder::constructPath` `0x11ba96` vs `0x11bada` |

Variant A proves the move is causal but incomplete: it fixes `update` and immediately exposes
`getPosition`. Variant B fixes the whole `PlayerStatic` block. The landed delta is
**+1 plain byte_exact, +3 functions at their original address, +121 exact-address prefix**,
with the gate unchanged.

The gate `.so` is byte-identical before and after:

```sh
cmp -s /tmp/w3745/playerstatic/gate_baseline.so \
       /tmp/w3745/playerstatic/gate_after.so
echo $?   # 0
```

Section fingerprint after the change (all unchanged):

```text
.text        0x1337c8 @ 0x69440
.ARM.exidx   0x006f70 @ 0x19cc08
.ARM.extab   0x02628c @ 0x1a3b78   (original 0x26298; known -12)
.rodata      0x041b67 @ 0x1c9e10
.data.rel.ro 0x002fe8 @ 0x20cbb0
.data        0x00296c @ 0x214000
.bss         0x0069ac @ 0x216970
```

Because the ordering file already places every `PlayerStatic` section at its original address,
the source reorder changes only the object's internal section order, not the linked gate
output. This is the ideal removal-campaign move: gate-neutral, plain-link-positive.

### 8.5 Next frontier

The first plain no-order mismatch is now:

```text
original 0x11ba96 SystemPathFinder::constructPath(Node*)
ours     0x11bada SystemPathFinder::constructPath(Node*)
```

This is a different TU and was not changed here. (`PlayerStaticFar` is also visibly out of
order later: the plain link emits its destructors before its constructor, while the original
starts with the constructor at `0x11c254`.)

### 8.6 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/playerstatic/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/playerstatic/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/playerstatic/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/playerstatic/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ship/PlayerStatic.cpp.obj \
  | awk '$3 ~ /^\.text\._ZN12PlayerStatic/ {print $3}' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep PlayerStatic
```

The compile-only variants in §8.2 used the exact command from
`cmake-build-match/compile_commands.json` for `PlayerStatic.cpp`, replacing only `-o` and the
input path. The full A/B in §8.4 used the normal build and the committed relink helper.

### 8.7 Proven vs hypothesis

**Proven by measurement**

* The original `PlayerStatic` order is `ctor, dtor, update, getPosition, render, translate,
  collide, outerCollide`; both the original `.dynsym` addresses and the HD source-layout dump
  agree. `update` is the 2-byte empty `bx lr` body at `0xe0980`.
* The baseline object emits `render, getPosition, update`; the plain no-order link therefore
  puts `update` at `0xe09bc` instead of `0xe0980`.
* Moving only `update` before `render` (variant A) fixes `update` but leaves `getPosition`
  displaced (`0xe0988` vs `0xe0982`); plain byte_exact stays 3019.
* Moving `update` and `getPosition` before `render` (variant B) reproduces the whole original
  block in the plain link. Plain byte_exact rises 3019 -> 3020, exact addresses 3223 -> 3226,
  prefix 2657 -> 2778.
* Variant B is gate-neutral: byte 4497 / linked 4497, lint `CLEAN ✓`, drift 0, `.text`
  `0x1337c8`, and the gate `.so` is byte-identical to the baseline (`cmp` status 0).

**Hypothesis / open**

* The next plain no-order frontier is `SystemPathFinder::constructPath` (`0x11ba96` original,
  `0x11bada` ours). Its object order was not investigated in this pass.
* The ordering file still cannot be removed: the plain no-order build is `3020`, not `4497`.

## 9. `SystemPathFinder::constructPath` placement (2026-11-02)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0. The plain no-order link rises from **3020 / 4497** (prefix 2778,
3226 / 4524 exported functions at their original address) to **3022 / 4497**
(prefix 2810, 3228 / 4524). The first plain no-order mismatch moves from
`SystemPathFinder::constructPath` to `PlayerStaticFar::PlayerStaticFar`.

This is the next frontier recorded in §8.5.

### 9.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

The original's `SystemPathFinder` block from the binary's own `.dynsym`:

```text
0x11b710  Node::Node(int)
0x11b73c  Array<Node*>::Array()
0x11b758  Node::~Node()
0x11b768  void ArrayRelease<Node*>(Array<Node*>&)
0x11b77c  SystemPathFinder::SystemPathFinder()
0x11b77e  SystemPathFinder::~SystemPathFinder()
0x11b780  SystemPathFinder::getJumpDistance(Array<SolarSystem*>*, int, int)
0x11b7a8  SystemPathFinder::getSystemPath(Array<SolarSystem*>*, int, int)
0x11b910  void ArraySetLength<Node*>(unsigned int, Array<Node*>&)
0x11b944  void ArrayAdd<Node*>(Node*, Array<Node*>&)
0x11b968  SystemPathFinder::search(Node*, Node*)
0x11ba30  void ArrayReleaseClasses<Node*>(Array<Node*>&)
0x11ba80  Array<Node*>::~Array()
0x11ba96  SystemPathFinder::constructPath(Node*)
0x11bb10  void ArrayRemove<Node*>(Node*, Array<Node*>&)
0x11bb54  SystemPathFinder::contains(Array<Node*>*, Node*)
```

The HD source-layout dump (`_work/original_layout/original_layout.json`,
`Classes/Game/SystemPathFinder.cpp`) independently lists the same relative
order for the strong functions and the two weak copies it records:
`ctor, dtor, getJumpDistance, getSystemPath, ArraySetLength<Node*>, search,
ArrayReleaseClasses<Node*>, constructPath, ArrayRemove<Node*>`. In particular
`constructPath` precedes `ArrayRemove<Node*>`.

The baseline plain no-order link already reproduced every symbol in this block
except the last two:

```text
original 0x11ba96  constructPath(Node*)
ours     0x11bada  constructPath(Node*)
original 0x11bb10  ArrayRemove<Node*>
ours     0x11ba96  ArrayRemove<Node*>
```

`contains` was already at `0x11bb54` in both. The whole block is therefore one
pair of swapped sections.

### 9.2 Object order and the source fix

The baseline `SystemPathFinder.cpp.obj` section-header order was:

```text
Node::Node
Array<Node*>::Array()
Node::~Node()
ArrayRelease<Node*>
SystemPathFinder ctor
SystemPathFinder dtor
getJumpDistance
getSystemPath
ArrayRelease<int>
Array<int>::~Array()
ArraySetLength<Node*>
ArrayAdd<Node*>
search
Array<int>::Array()
ArraySetLength<int>
ArrayReleaseClasses<Node*>
Array<Node*>::~Array()
ArrayRemove<Node*>
constructPath
contains
```

The `Array<int>` copies are deduplicated away by the linker (their first copy
is in an earlier TU), so the linked order is the target order up to
`Array<Node*>::~Array()` and then `ArrayRemove` / `constructPath` are swapped.

The fix is a pure source move: define `constructPath` **before** `search`
instead of after it.

```diff
@@ after SystemPathFinder::getSystemPath()
+Array<Node *> *SystemPathFinder::constructPath(Node *node) {
+    ...
+}
+
 Array<Node *> *SystemPathFinder::search(Node *start, Node *goal) {
@@
-Array<Node *> *SystemPathFinder::constructPath(Node *node) {
-    ...
-}
-
 int SystemPathFinder::contains(Array<Node *> *nodes, Node *node) {
```

The resulting object order is exactly the target:

```text
...
Array<Node*>::~Array()
constructPath
ArrayRemove<Node*>
contains
```

The mechanism is clang's first-use emission: `constructPath` is first used by
`search`'s `return constructPath(goal);`, so it is emitted at that call site
(after `search`), while its body is scanned at its own definition (before
`search`). The deferred `ArrayRemove<Node*>` instantiation is then flushed after
`constructPath`. Defining `constructPath` after `search` makes it emit at its
definition point, which is after the `ArrayRemove` flush. This is the same
first-use rule documented in `section_order_removal_w3696.md` §8.3, with the
additional observation that the *call site* wins over the definition point when
the definition precedes the caller.

Compile-only variants prove the source position controls the object order:

| source order | object order around the block |
|---|---|
| baseline: `search, constructPath, contains` | `ArrayRemove, constructPath, contains` |
| **landed: `constructPath, search, contains`** | **`constructPath, ArrayRemove, contains`** |
| `constructPath` before `getSystemPath` | `constructPath` before `ArraySetLength`/`ArrayAdd`/`search` |
| `constructPath` at end (after `contains`) | `ArrayRemove, constructPath, contains` (same as baseline) |

All 20 `.text.*` section byte sequences are identical between the baseline and
the landed object; only their order changed.

### 9.3 Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---:|---|
| baseline | **4497 / 4497** | **3020** | 3226 / 4524 | 2778 | `SystemPathFinder::constructPath` `0x11ba96` vs `0x11bada` |
| landed | **4497 / 4497** | **3022** | **3228 / 4524** | **2810** | `PlayerStaticFar::PlayerStaticFar` `0x11c254` vs `0x11c268` |

The entire `SystemPathFinder` block is now at its original address in the plain
link, and `constructPath` is byte-exact there (`verify.py` row
`_ZN16SystemPathFinder13constructPathEP4Node`: `byte True`, `linked True`,
`pct 100.0`). The landed delta is **+2 plain byte_exact, +2 functions at their
original address, +32 exact-address prefix**, with the gate unchanged.

The gate `.so` is not byte-identical to the baseline (unlike the `PlayerStatic`
move in §8.4). The only differences are:

| range | bytes | section |
|---|---:|---|
| `0x1dc..0x1f0` | 20 | `.note.gnu.build-id` |
| `0x1a0a7c..0x1a0a85` | 2 | `.ARM.exidx` |
| `0x1c042c..0x1c045a` | 44 | `.ARM.extab` |

`.text`, `.rodata`, `.data.rel.ro`, `.data`, `.plt` and `.got` are
byte-identical to the baseline. Against the original, `.ARM.exidx` differing
bytes are unchanged at 1744 and `.ARM.extab` differing bytes improve from
113823 to 113821. The plain no-order `.text` differing bytes improve from
387685 to 387508. The exidx/extab changes are the reordered per-function
records, not a body change.

### 9.4 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/spf/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/spf/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/spf/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/spf/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/SystemPathFinder.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | grep -E 'SystemPathFinder|_ZN4Node|ArrayIP4Node'
```

The compile-only variants used the exact `compile_commands.json` command for
`SystemPathFinder.cpp` with only `-o` and the input path replaced; no flag was
changed.

### 9.5 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3020 / 4497`, prefix 2778,
  3226 / 4524, first mismatch `constructPath` `0x11ba96` vs `0x11bada`.
* The baseline object emits `ArrayRemove` before `constructPath`; the original
  emits `constructPath` before `ArrayRemove` (both `.dynsym` and the HD
  source-layout dump agree).
* Moving the `constructPath` definition before `search` makes clang emit
  `constructPath` at `search`'s call site and `ArrayRemove` after it, exactly
  the original order. All 20 `.text.*` section bytes are unchanged.
* The landed move is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0) and
  improves the plain no-order build to `3022 / 4497`, prefix 2810,
  3228 / 4524. Every `SystemPathFinder` symbol is now at its original address
  in the plain link.
* The gate `.so` differs from the baseline only in the build-id, 2 bytes of
  `.ARM.exidx` and 44 bytes of `.ARM.extab`; `.text` is byte-identical.

**Hypothesis / open**

* The exact clang 7 deferred-emission rule is characterised empirically here
  (call-site wins over definition point when the definition precedes the
  caller; the body is scanned at the definition), not from the compiler source.
* The next plain no-order frontier is `PlayerStaticFar::PlayerStaticFar`
  (`0x11c254` original, `0x11c268` ours). Its object emits the destructors
  before the constructor; that is a separate TU and was not changed in this
  pass.
* The ordering file still cannot be removed: the plain no-order build is
  `3022`, not `4497`.

## 10. `PlayerStaticFar` constructor placement (2026-11-02)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0, and the gate `.so` is **byte-identical** to the baseline (`cmp` status 0).
The plain no-order link rises from **3022 / 4497** (prefix 2810, 3228 / 4524
exported functions at their original address) to **3024 / 4497** (prefix 2864,
3233 / 4524). The first plain no-order mismatch moves from
`PlayerStaticFar::PlayerStaticFar` to `LensFlare::update`.

This is the next frontier recorded in §9.5. The baseline value printed there
(`0x11c278`) was a transcription error; the measured baseline constructor
address is `0x11c268` (corrected in §9.3/§9.5).

### 10.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

The original's `PlayerStaticFar` block from the binary's own `.dynsym`:

```text
0x11c254  PlayerStaticFar::PlayerStaticFar(int, AEGeometry*, float, float, float)
0x11c2c4  PlayerStaticFar::~PlayerStaticFar()   # D1/D2
0x11c2c8  PlayerStaticFar::~PlayerStaticFar()   # D0
0x11c2d8  PlayerStaticFar::setYRotation(int)
0x11c2da  PlayerStaticFar::render()
0x11c2e0  PlayerStaticFar::update(int)
0x11c460  PlayerStaticFar::collide(float, float, float)
0x11c4ee  PlayerStaticFar::getInitPosition(AbyssEngine::AEMath::Vector)
0x11c4fc  PlayerStaticFar::outerCollide(float, float, float)
0x11c506  PlayerStaticFar::outerCollide(AbyssEngine::AEMath::Vector)
0x11c510  PlayerStaticFar::getProjectionVector(AbyssEngine::AEMath::Vector const&)
0x11c52e  PlayerStaticFar::projectCollisionOnSurface(AbyssEngine::AEMath::Vector const&)
```

The HD source-layout dump independently lists the constructor first, followed by
the three destructor symbols (`_work/original_layout/original_layout.json`,
`Classes/Game/PlayerStaticFar.cpp`), so the original source order had the
constructor before the destructor.

The baseline `PlayerStaticFar.cpp.obj` emitted the destructors before the
constructor:

```text
PlayerStaticFar::~PlayerStaticFar()   # D2
PlayerStaticFar::~PlayerStaticFar()   # D0
PlayerStaticFar::PlayerStaticFar
PlayerStaticFar::setYRotation
PlayerStaticFar::render
PlayerStaticFar::update
...
```

That made the plain no-order link place the constructor at `0x11c268` instead
of `0x11c254`. The first plain no-order mismatch was exactly:

```text
original 0x11c254 PlayerStaticFar::PlayerStaticFar
ours     0x11c268 PlayerStaticFar::PlayerStaticFar
```

### 10.2 Compile-only placement variants

The exact `compile_commands.json` command for `PlayerStaticFar.cpp` was used
with only the input and output paths replaced. The only source edit was moving
`PlayerStaticFar::~PlayerStaticFar() = default;` after the constructor body.

| source order | object `.text.*` order around the block |
|---|---|
| baseline: destructor, constructor | `D2, D0, ctor, setYRotation, render, update, ...` |
| **landed: constructor, destructor** | **`ctor, D2, D0, setYRotation, render, update, ...`** |

The two objects have the same 18 `.text.*` section names, and every section has
the same size and SHA-256 by name; only their order changed. This is a pure
emission-order fix, not a body change.

### 10.3 Landed source change

```diff
@@ after the includes
-PlayerStaticFar::~PlayerStaticFar() = default;
-
 PlayerStaticFar::PlayerStaticFar(int playerId, AEGeometry *geometry, float x, float y, float z)
     : PlayerStatic(playerId, geometry, x, y, z), viewDirection() {
@@
     this->boundingVolumes = nullptr;
 }

+PlayerStaticFar::~PlayerStaticFar() = default;
+
 void PlayerStaticFar::setYRotation(int /*yRotation*/) {
```

### 10.4 Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---:|---|
| baseline | **4497 / 4497** | **3022** | 3228 / 4524 | 2810 | `PlayerStaticFar::PlayerStaticFar` `0x11c254` vs `0x11c268` |
| landed | **4497 / 4497** | **3024** | **3233 / 4524** | **2864** | `LensFlare::update` `0x11d082` vs `0x11d6a4` |

The whole `PlayerStaticFar` block is now at its original address in the plain
link, and both constructor symbols are byte-exact there:

```text
_ZN15PlayerStaticFarC1EiP10AEGeometryfff  pct 100.0  byte True  linked True  size 112
_ZN15PlayerStaticFarC2EiP10AEGeometryfff  pct 100.0  byte True  linked True  size 112
```

The landed delta is **+2 plain byte_exact, +5 functions at their original
address, +54 exact-address prefix**, with the gate unchanged. The gate `.so` is
byte-identical to the baseline:

```sh
cmp -s /tmp/w3745/psf/gate_baseline.so /tmp/w3745/psf/gate_final.so
echo $?   # 0
```

Section fingerprint after the change (all unchanged):

```text
.text        0x1337c8 @ 0x69440
.ARM.exidx   0x006f70 @ 0x19cc08
.ARM.extab   0x02628c @ 0x1a3b78   (original 0x26298; known -12)
.rodata      0x041b67 @ 0x1c9e10
.data.rel.ro 0x002fe8 @ 0x20cbb0
.data        0x00296c @ 0x214000
.bss         0x0069ac @ 0x216970
```

### 10.5 Next frontier

The first plain no-order mismatch is now:

```text
original 0x11d082 LensFlare::update(int)
ours     0x11d6a4 LensFlare::update(int)
```

`LensFlare.cpp.obj` emits `render2D` before `update`; the original `.dynsym`
and the HD source-layout dump both put `update` before `render2D`. This is a
different TU and was not changed here.

### 10.6 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/psf/final_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/psf/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/psf/final_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/psf/final_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ship/PlayerStaticFar.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep PlayerStaticFar
```

The compile-only variants used the exact `compile_commands.json` command for
`PlayerStaticFar.cpp` with only `-o` and the input path replaced; no flag was
changed.

### 10.7 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3022 / 4497`, prefix 2810,
  3228 / 4524, first mismatch `PlayerStaticFar::PlayerStaticFar`
  `0x11c254` vs `0x11c268`.
* The original emits the constructor before the destructors: the original
  `.dynsym` gives `0x11c254` for the constructor and `0x11c2c4` for D1/D2, and
  the HD source-layout dump lists the constructor first.
* Moving the destructor definition after the constructor makes the object order
  `ctor, D2, D0, setYRotation, render, update, ...`; all 18 `.text.*` section
  bytes are unchanged by name.
* The landed move is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0) and
  the gate `.so` is byte-identical to the baseline.
* The plain no-order build improves to `3024 / 4497`, prefix 2864,
  3233 / 4524. Both constructor symbols are byte-exact at `0x11c254`.
* The next plain no-order frontier is `LensFlare::update` (`0x11d082` original,
  `0x11d6a4` ours); `LensFlare.cpp.obj` emits `render2D` before `update`, while
  the original `.dynsym` and HD dump put `update` first.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `3024`, not `4497`.
* The `LensFlare` move is the obvious next per-TU source-order experiment, but
  it was not tested or landed in this pass.

## 11. `LensFlare::update` placement (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0, and the gate `.so` is **byte-identical** to the baseline. The plain
no-order link rises from **3024 / 4497** (prefix 2864, 3233 / 4524 exported
functions at their original address) to **3076 / 4497** (prefix 2870,
3312 / 4524). The first plain no-order mismatch moves from
`LensFlare::update` to `Standing::setStanding`.

This is the next frontier recorded in §10.5.

### 11.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

The original's `LensFlare` block from the binary's own `.dynsym`:

```text
0x11d00c  LensFlare::LensFlare(AbyssEngine::PaintCanvas*)   # C1/C2
0x11d06c  LensFlare::~LensFlare()                           # D1/D2
0x11d082  LensFlare::update(int)                            # 6 bytes
0x11d088  LensFlare::render2D(float, float, float, int)     # 1568 bytes
```

The HD source-layout dump independently lists the same order
(`_work/original_layout/original_layout.json`, `Classes/Game/LensFlare.cpp`):
`LensFlare`, `~LensFlare`, `update`, `render2D`.

The baseline `LensFlare.cpp.obj` emitted `render2D` before `update`:

```text
LensFlare::LensFlare(AbyssEngine::PaintCanvas*)
LensFlare::~LensFlare()
LensFlare::render2D(float, float, float, int)
LensFlare::update(int)
```

That made the plain no-order link place `update` at `0x11d6a4` instead of
`0x11d082`. The first plain no-order mismatch was exactly:

```text
original 0x11d082 LensFlare::update(int)
ours     0x11d6a4 LensFlare::update(int)
```

### 11.2 Compile-only placement variants

The exact `compile_commands.json` command for `LensFlare.cpp` was used with only
the input and output paths replaced. The only source edit was moving the
`update` definition before `render2D`.

| source order | object `.text.*` order around the block |
|---|---|
| baseline: `render2D, update` | `ctor, dtor, render2D, update` |
| **landed: `update, render2D`** | **`ctor, dtor, update, render2D`** |

The two objects have the same four `.text.*` section names, and every section
has the same size and SHA-256 by name; only their order changed:

```text
same section name set: True
same (size,sha) by name: True
n sections 4
```

This is a pure emission-order fix, not a body change.

### 11.3 Landed source change

```diff
@@ after LensFlare::~LensFlare()
+void LensFlare::update(int) {
+    this->intensity = 0.0f;
+}
+
 void LensFlare::render2D(float x, float y, float depth, int colorIndex) {
@@
-void LensFlare::update(int) {
-    this->intensity = 0.0f;
-}
```

### 11.4 Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---:|---|
| baseline | **4497 / 4497** | **3024** | 3233 / 4524 | 2864 | `LensFlare::update` `0x11d082` vs `0x11d6a4` |
| landed | **4497 / 4497** | **3076** | **3312 / 4524** | **2870** | `Standing::setStanding` `0x11d6de` vs `0x11d8e8` |

The entire `LensFlare` block is now at its original address in the plain
no-order link:

```text
0011d00c T _ZN9LensFlareC1EPN11AbyssEngine11PaintCanvasE
0011d00c T _ZN9LensFlareC2EPN11AbyssEngine11PaintCanvasE
0011d06c T _ZN9LensFlareD1Ev
0011d06c T _ZN9LensFlareD2Ev
0011d082 T _ZN9LensFlare6updateEi
0011d088 T _ZN9LensFlare8render2DEfffi
```

`update` is byte-identical at `0x11d082` (`00 21 01 60 70 47`, the empty
`bx lr` body). `render2D` is disassembly-identical (`pct 100.0`, `linked
True`) but not byte-exact in the plain no-order link (`byte False`), because
its 1568-byte body contains literal-pool words that encode addresses of data
that moved in the no-order layout; in the gate build it is byte-exact.

The landed delta is **+52 plain byte_exact, +79 functions at their original
address, +6 exact-address prefix**, with the gate unchanged. The gate `.so` is
byte-identical to the baseline:

```sh
cmp -s /tmp/w3745/lf/gate_base.so /tmp/w3745/lf/gate_after.so
echo $?   # 0
sha256sum /tmp/w3745/lf/gate_base.so /tmp/w3745/lf/gate_after.so
# 995e151c06f84cf343689d2464796000e39b561a06d7623a516d9729df766c01  (both)
```

Section fingerprint after the change (all unchanged):

```text
.text        0x1337c8 @ 0x69440
.ARM.exidx   0x006f70 @ 0x19cc08
.rodata      0x041b67 @ 0x1c9e10
```

### 11.5 Next frontier

The first plain no-order mismatch is now:

```text
original 0x11d6de Standing::setStanding(int, int)
ours     0x11d8e8 Standing::setStanding(int, int)
```

`Standing.cpp.obj` emits `getStandings, setStandings, applyPoints,
getStandingRate, getStanding, isEnemyWithAnyone, getEnemyRace, isNeutral,
isEnemy, isFriend, applyMissionCompleted, applyDelict, setStanding, ...`,
while the original `.dynsym` (and the HD source-layout dump) put
`setStanding` immediately after the destructor, followed by `setStandings`,
`getStandings`, `getStanding`, `getStandingRate`, `setPlayerSignatureRace`,
`rehabilitate`, `isEnemyWithAnyone`, `isEnemy`, `isFriend`, `isNeutral`,
`getEnemyRace`, `applyPoints`, `applyKill`, `applyDelict`, `applyStealCargo`,
`applyMissionCompleted`, `applyDisable`, `getMissionBonus`. This is a broad
`Standing` TU reorder and was not changed here.

### 11.6 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/lf/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/lf/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/lf/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/lf/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/engine/render/LensFlare.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep LensFlare
```

The compile-only variants used the exact `compile_commands.json` command for
`LensFlare.cpp` with only `-o` and the input path replaced; no flag was
changed.

### 11.7 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3024 / 4497`, prefix 2864,
  3233 / 4524, first mismatch `LensFlare::update` `0x11d082` vs `0x11d6a4`.
* The original emits `update` before `render2D`: the original `.dynsym` gives
  `0x11d082` for `update` and `0x11d088` for `render2D`, and the HD
  source-layout dump lists `update` first.
* Moving the `update` definition before `render2D` makes the object order
  `ctor, dtor, update, render2D`; all four `.text.*` section bytes are
  unchanged by name.
* The landed move is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0) and
  the gate `.so` is byte-identical to the baseline (same SHA-256).
* The plain no-order build improves to `3076 / 4497`, prefix 2870,
  3312 / 4524. Every `LensFlare` symbol is at its original address there, and
  `update` is byte-exact at `0x11d082`.
* The next plain no-order frontier is `Standing::setStanding`
  (`0x11d6de` original, `0x11d8e8` ours); the original `.dynsym` and HD dump
  put `setStanding` immediately after the destructor.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `3076`, not `4497`.
* The `Standing` TU reorder is the obvious next per-TU source-order
  experiment, but it was not tested or landed in this pass.

## 12. `Standing::setStanding` reorder (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0. The plain no-order link rises from **3076 / 4497** (prefix 2870,
3312 / 4524 exported functions at their original address) to **3085 / 4497**
(prefix 2994, 3342 / 4524). The first plain no-order mismatch moves from
`Standing::setStanding` to `Wanted::Wanted`.

This is the next frontier recorded in §11.5.

### 12.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

The original's `Standing` block from the binary's own `.dynsym`:

```text
0x11d6a8  Standing::Standing()
0x11d6c8  Standing::~Standing()
0x11d6de  Standing::setStanding(int, int)
0x11d6e6  Standing::setStandings(int*)
0x11d6ea  Standing::getStandings()
0x11d6ee  Standing::getStanding(int)
0x11d728  Standing::getStandingRate(int)
0x11d74c  Standing::setPlayerSignatureRace(int)
0x11d750  Standing::rehabilitate(int)
0x11d780  Standing::isEnemyWithAnyone()
0x11d7a2  Standing::isEnemy(int)
0x11d808  Standing::isFriend(int)
0x11d86e  Standing::isNeutral(int)
0x11d890  Standing::getEnemyRace(int)
0x11d8a8  Standing::applyPoints(int, int)
0x11d8d0  Standing::applyKill(int)
0x11d93c  Standing::applyDelict(int, int)
0x11d98c  Standing::applyStealCargo(int)
0x11d992  Standing::applyMissionCompleted(int)
0x11d99a  Standing::applyDisable(int)
0x11d9a0  Standing::getMissionBonus(int)
```

The HD source-layout dump (`_work/original_layout/original_layout.json`,
`Classes/Game/Standing.cpp`) independently lists the same order for the
functions it contains; `isNeutral` and `applyPoints` are Android-2.0.x
additions and are witnessed only by the Android `.dynsym`. The Android
`.dynsym` is the authority used here.

The baseline `Standing.cpp.obj` order was:

```text
ctor, dtor, getStandings, setStandings, applyPoints, getStandingRate,
getStanding, isEnemyWithAnyone, getEnemyRace, isNeutral, isEnemy, isFriend,
applyMissionCompleted, applyDelict, setStanding, setPlayerSignatureRace,
applyStealCargo, applyKill, getMissionBonus, applyDisable, rehabilitate
```

so the plain no-order link put `setStanding` at `0x11d8e8` instead of
`0x11d6de`. The first plain no-order mismatch was exactly:

```text
original 0x11d6de Standing::setStanding(int, int)
ours     0x11d8e8 Standing::setStanding(int, int)
```

### 12.2 Landed source change

The `Standing.cpp` definitions were re-emitted in the original `.dynsym`
order. The two namespace-scope tables (`Standing_enemyRaceTable` and
`g_apk_raceTable`) were kept immediately before their first users, so their
`.rodata` emission order is unchanged. No body was edited.

The reordered object order now matches the original address order exactly:

```text
Standing::Standing()
Standing::~Standing()
Standing::setStanding(int, int)
Standing::setStandings(int*)
Standing::getStandings()
Standing::getStanding(int)
Standing::getStandingRate(int)
Standing::setPlayerSignatureRace(int)
Standing::rehabilitate(int)
Standing::isEnemyWithAnyone()
Standing::isEnemy(int)
Standing::isFriend(int)
Standing::isNeutral(int)
Standing::getEnemyRace(int)
Standing::applyPoints(int, int)
Standing::applyKill(int)
Standing::applyDelict(int, int)
Standing::applyStealCargo(int)
Standing::applyMissionCompleted(int)
Standing::applyDisable(int)
Standing::getMissionBonus(int)
```

The gate build (with the ordering file) is unchanged in `.text`,
`.ARM.exidx` and `.ARM.extab` content for the `Standing` range; the only
gate `.so` change is the 20-byte build-id plus 2 bytes of `.ARM.exidx`
extab-offset data (the `.ARM.extab` record order follows object section
order, which the ordering file does not reorder).

### 12.3 The `.ARM.exidx` confound and the enabling `BoundingVolume` reorder

The `Standing` reorder alone is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`,
drift 0) and moves the plain-link exact-address prefix 2870 -> 2934
(3312 -> 3331 functions at their original address). But the raw plain
`byte_exact` **drops** 3076 -> 2800. The cause is measured, not guessed:

* the plain link's `.ARM.exidx` shrinks from `0x6f50` to `0x6f48` (one
  fewer merged `CANTUNWIND` entry);
* that shifts `.ARM.extab` and `.rodata` from `0x1c9e10` to `0x1c9e00`;
* every PC-relative literal-pool word that points into `.rodata` changes,
  so 278 functions that were `linked_exact` but literal-pool-exact lose
  `byte_exact` (the instruction streams are unchanged: `linked_exact`
  stays 4497).

The `Standing` block itself is now *more* correct: the original has 11
`.ARM.exidx` entries in the `Standing` range (the `setPlayerSignatureRace`
`CANTUNWIND` entry covers `rehabilitate`/`isEnemyWithAnyone`/`isEnemy`/
`isFriend` up to `isNeutral`), and the reordered object reproduces exactly
that 11-entry pattern. The baseline's 12-entry pattern had been
accidentally compensating for a missing entry elsewhere in the plain link.

To keep the plain-link `.ARM.exidx` size at the baseline `0x6f50` and make
the pass a strict improvement, the next frontier TU was reordered in the
same pass: `BoundingVolume.cpp`. Its original `.dynsym` order is

```text
0x11f6dc  BoundingVolume::BoundingVolume(float, float, float, float, float, float)
0x11f718  BoundingVolume::~BoundingVolume()
0x11f74c  BoundingVolume::setVolumes(Array<BoundingVolume*>*)
0x11f750  BoundingVolume::setVolume(BoundingVolume*)
0x11f788  BoundingVolume::getProjectionVector(AEMath::Vector const&)
0x11f7d8  BoundingVolume::getCollisionNormal(AEMath::Vector const&)
0x11f7e4  BoundingVolume::update(float, float, float)
0x11f870  BoundingVolume::collide(float, float, float)
0x11f8ac  BoundingVolume::outerCollide(float, float, float)
0x11f8b0  BoundingVolume::staticProjectCollisionOnSurface(AEMath::Vector const&, Array<BoundingVolume*>*)
```

and the reordered object reproduces that order. This adds back the one
`.ARM.exidx` entry (plain `.ARM.exidx` back to `0x6f50`, `.rodata` back to
`0x1c9e10`) and makes the whole `BoundingVolume` block address-exact in the
plain link. It also reduces the gate `.ARM.extab` differing-byte count
against the original from 113,809 to 113,807; `.text` (112,124 differing
bytes) and `.ARM.exidx` (1,744) are unchanged.

The `BoundingVolume` HD source-layout dump lists the same ctor/dtor/
`getProjectionVector`/`getCollisionNormal`/`update`/`collide`/
`outerCollide`/`staticProjectCollisionOnSurface` order; `setVolumes` and
`setVolume` are witnessed only by the Android `.dynsym`.

### 12.4 Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---:|---|
| baseline | **4497 / 4497** | **3076** | 3312 / 4524 | 2870 | `Standing::setStanding` `0x11d6de` vs `0x11d8e8` |
| `Standing` only, full reorder | 4497 / 4497 | 2800 | 3331 / 4524 | 2934 | `BoundingVolume::BoundingVolume` `0x11f6dc` vs `0x11f798` |
| `Standing` + `BoundingVolume` (landed) | **4497 / 4497** | **3085** | **3342 / 4524** | **2994** | `Wanted::Wanted` `0x122890` vs `0x12290c` |

The landed delta is **+9 plain byte_exact, +30 functions at their original
address, +124 exact-address prefix**, with the gate unchanged. The gate
`.so` is not byte-identical to the baseline (build-id plus `.ARM.exidx`/
`.ARM.extab` record order), but the gate counts are unchanged and the
`.ARM.extab` differing-byte count against the original improves by 2.

Section fingerprint after the change (unchanged):

```text
.text        0x1337c8 @ 0x69440
.ARM.exidx   0x006f70 @ 0x19cc08
.ARM.extab   0x02628c @ 0x1a3b78   (original 0x26298; known -12)
.rodata      0x041b67 @ 0x1c9e10
```

### 12.5 Next frontier

The first plain no-order mismatch is now:

```text
original 0x122890 Wanted::Wanted(int, AEMath::String, int, int, bool, int, int, int, int, int, int, int, int, int)
ours     0x12290c Wanted::Wanted(...)
```

`Wanted.cpp.obj` emits the accessors (`getIndex`, `getBoard`, `getRace`,
`isMale`, `getShip`, ...) before the constructor; the original `.dynsym`
puts the constructor and destructor first. This is a separate TU and was
not changed in this pass.

### 12.6 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/st/final_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/st/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/st/final_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/st/final_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/Standing.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/engine/math/BoundingVolume.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E 'Standing|BoundingVolume'
```

### 12.7 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3076 / 4497`, prefix 2870,
  3312 / 4524, first mismatch `Standing::setStanding` `0x11d6de` vs
  `0x11d8e8`.
* The original emits `Standing` in the `.dynsym` order listed in §12.1;
  the reordered object reproduces that order exactly.
* The `Standing` reorder alone is gate-neutral but drops raw plain
  `byte_exact` to 2800 because the plain `.ARM.exidx` shrinks by one
  `CANTUNWIND` entry and shifts `.rodata` by `-0x10`; `linked_exact`
  stays 4497, so no instruction stream changed.
* The reordered `Standing` block reproduces the original's 11-entry
  `.ARM.exidx` pattern exactly.
* `BoundingVolume.cpp` reordered to its original `.dynsym` order adds back
  the missing `.ARM.exidx` entry (plain `.ARM.exidx` `0x6f50`, `.rodata`
  `0x1c9e10`) and makes the whole `BoundingVolume` block address-exact in
  the plain link.
* The landed combined change is gate-neutral (`4497 / 4497`, lint
  `CLEAN ✓`, drift 0) and improves the plain no-order build to
  `3085 / 4497`, prefix 2994, 3342 / 4524. The gate `.ARM.extab`
  differing-byte count against the original improves 113,809 -> 113,807.
* The next plain no-order frontier is `Wanted::Wanted` (`0x122890`
  original, `0x12290c` ours); `Wanted.cpp.obj` emits its accessors before
  the constructor, while the original `.dynsym` puts the constructor and
  destructor first.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `3085`, not `4497`.
* The plain no-order raw `byte_exact` is sensitive to the global
  `.ARM.exidx` entry count (each entry is 8 bytes and the `.rodata`
  alignment turns an 8-byte change into a 16-byte base shift). Until the
  whole layout converges, a locally correct reorder can lower raw
  `byte_exact` while raising the exact-address prefix; the prefix and
  `linked_exact` are the more stable convergence measures.
* The `Wanted` reorder is the obvious next per-TU source-order experiment,
  but it was not tested or landed in this pass.

## 13. `Wanted` ctor placement / full TU reorder (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0, and the gate `.so` is **byte-identical** before and after
(`sha256 d61445d5e77792199a94c360745efea80e88c37c7277f05eaefd09e590153d00`).
The plain no-order link rises from **3085 / 4497** (prefix 2994,
3342 / 4524 exported functions at their original address) to **3088 / 4497**
(prefix 3032, 3373 / 4524). The first plain no-order mismatch moves from
`Wanted::Wanted` to `MenuTouchWindow::~MenuTouchWindow`.

This is the next frontier recorded in §12.5.

### 13.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

The brief's 4496 baseline is stale; this HEAD is 4497/4497.

The original's `Wanted` block from the binary's own address-sorted `.dynsym`
(`nm -nD --defined-only`):

```text
0x122890  Wanted::Wanted(int, AEMath::String, int, int, bool, int, int, int, int, int, int, int, int, int)  # C1/C2
0x122904  Wanted::~Wanted()                                                                                # D1/D2
0x122920  Wanted::getIndex()
0x122924  Wanted::getName()
0x122930  Wanted::getNumWingmen()
0x122934  Wanted::getBoard()
0x122938  Wanted::getRace()
0x12293c  Wanted::isMale()
0x122940  Wanted::getShip()
0x122944  Wanted::getWeapon()
0x122948  Wanted::getHitpoints()
0x12294c  Wanted::getLoot()
0x122950  Wanted::getLootAmount()
0x122954  Wanted::getReward()
0x122958  Wanted::getRequiredBounties()
0x12295c  Wanted::getRequiredMission()
0x122960  Wanted::setRequiredMission(int)
0x122964  Wanted::setImageParts(int*)
0x122968  Wanted::getImageParts()
0x12296c  Wanted::setTerminated(bool)
0x122972  Wanted::setActive(bool)
0x122978  Wanted::setTravelsTo(int)
0x12297c  Wanted::setCurrentLocation(int)
0x122980  Wanted::setLastSeen(int)
0x122984  Wanted::isActive()
0x12298a  Wanted::isTerminated()
0x122990  Wanted::getLastSeen()
0x122994  Wanted::getTravelsTo()
0x122998  Wanted::getCurrentLocation()
```

The HD source-layout dump (`_work/original_layout/original_layout.json`,
`gof2hd_1.1.19_ios` -> `Classes/Game/Wanted.cpp`) lists the same order for
every symbol it contains; `setRequiredMission` is an Android-2.0.x addition
witnessed only by the Android `.dynsym`. The Android address order is the
authority used here.

The baseline `Wanted.cpp.obj` order was the .cpp definition order:

```text
getIndex, getBoard, getRace, isMale, getShip, getWeapon, getHitpoints,
getLoot, getLootAmount, getReward, getRequiredBounties, getRequiredMission,
getNumWingmen, getCurrentLocation, getTravelsTo, getLastSeen, getImageParts,
setImageParts, setRequiredMission, setCurrentLocation, setTravelsTo,
setLastSeen, isTerminated, setTerminated, isActive, setActive, getName,
ctor, dtor
```

so the plain no-order link put the constructor at `0x12290c` instead of
`0x122890`. The first plain no-order mismatch was exactly:

```text
original 0x122890 Wanted::Wanted(...)
ours     0x12290c Wanted::Wanted(...)
```

### 13.2 Landed source change

The `Wanted.cpp` definitions were re-emitted in the original address order
above. No body was edited; there are no namespace-scope statics in this TU, so
there is no `.rodata`/`.data` emission-order side effect. The reordered object
order now matches the original address order exactly:

```text
ctor, dtor, getIndex, getName, getNumWingmen, getBoard, getRace, isMale,
getShip, getWeapon, getHitpoints, getLoot, getLootAmount, getReward,
getRequiredBounties, getRequiredMission, setRequiredMission, setImageParts,
getImageParts, setTerminated, setActive, setTravelsTo, setCurrentLocation,
setLastSeen, isActive, isTerminated, getLastSeen, getTravelsTo,
getCurrentLocation
```

The gate build (with the ordering file) is **byte-identical** to the baseline,
including the 20-byte build-id: `cmp` reports 0 differing bytes. The ordering
file already placed the `Wanted` output block correctly; this pass only removes
a source/object-order divergence that the plain input-order link exposed.

### 13.3 Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `d61445d5…` | **3085** | 3342 / 4524 | 2994 | `Wanted::Wanted` `0x122890` vs `0x12290c` |
| `Wanted` full reorder (landed) | **4497 / 4497** | `d61445d5…` (identical) | **3088** | **3373 / 4524** | **3032** | `MenuTouchWindow::~MenuTouchWindow` `0x125170` vs `0x125186` |

The landed delta is **+3 plain byte_exact, +31 functions at their original
address, +38 exact-address prefix**, with the gate unchanged and the gate `.so`
byte-identical.

The whole `Wanted` block is now address-exact in the plain no-order link, and
29 of its 31 exported function rows are byte-exact. The two remaining rows are
`Wanted::~Wanted()` D1/D2, which are disassembly-identical but tail-call
`operator delete[]` at `0x79584` (original) vs `0x795a4` (ours); the `b.w`
immediate is a local-address difference, not a `Wanted` placement problem:

```text
orig 28B @0x122904  |  ours 28B @0x122904
  ...
  b.w 0x79584        ~  b.w 0x795a4
```

The plain no-order `.ARM.exidx` grows by one 8-byte entry
(`0x6f50 -> 0x6f58`; original `0x6f70`), which moves `.ARM.extab` from
`0x1a3b78` to `0x1a3b80` (same size `0x2628c`). `.rodata` stays at
`0x1c9e10` because the 16-byte alignment absorbs the 8-byte shift, so the
literal-pool addresses are unchanged and the plain `byte_exact` rises rather
than falls. The plain `.text` size is `0x1337e8` before and after (the gate
`.text` is `0x1337c8`; the plain link's extra `0x20` is the known marker
effect).

### 13.4 Next frontier

The first plain no-order mismatch is now:

```text
original 0x125170 MenuTouchWindow::~MenuTouchWindow()   # D1/D2
ours     0x125186 MenuTouchWindow::~MenuTouchWindow()   # D1/D2
```

The `MenuTouchWindow` constructor (`0x12299c`), `loadPreviewRecords`
(`0x124f1c`), `addButton` (`0x124fd8`), `ArrayAdd<TouchButton*>`
(`0x12510c`), `Array<TouchSlider*>::Array()` (`0x125130`) and
`ArrayAdd<TouchSlider*>` (`0x12514c`) are all already address-exact. The
16-byte displacement is one extra weak copy emitted by our object at the
original dtor address:

```text
original 0x125170  MenuTouchWindow::~MenuTouchWindow()
ours     0x125170  Array<GameRecord*>::~Array()          # 16 bytes
ours     0x125186  MenuTouchWindow::~MenuTouchWindow()
```

`MenuTouchWindow.cpp.obj` emits `Array<GameRecord*>::~Array()` between the
`ArrayAdd<TouchSlider*>` weak copy and the destructor; the original does not
place it there. This is the same weak-copy TU-membership/emission class as
§4 and §12.3, and it was not changed in this pass.

### 13.5 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/wanted/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/wanted/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/wanted/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/wanted/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/Wanted.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E 'Wanted|MenuTouchWindow'

# gate .so A/B (baseline vs landed source)
sha256sum /tmp/w3745/wanted/gate_base.so /tmp/w3745/wanted/gate_after.so
cmp -l /tmp/w3745/wanted/gate_base.so /tmp/w3745/wanted/gate_after.so | wc -l   # 0
```

### 13.6 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3085 / 4497`, prefix 2994,
  3342 / 4524, first mismatch `Wanted::Wanted` `0x122890` vs `0x12290c`.
* The original emits `Wanted` in the address order listed in §13.1; the HD
  source-layout dump independently lists the same order for every symbol it
  contains.
* Re-emitting the `Wanted.cpp` definitions in that order makes the object's
  `.text.*` order match the original address order exactly and is
  gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0).
* The gate `.so` is byte-identical before and after (same sha256, `cmp` 0
  differing bytes), so the ordering file already reproduced the `Wanted`
  output block.
* The plain no-order build improves to `3088 / 4497`, prefix 3032,
  3373 / 4524. The entire `Wanted` block is address-exact there; 29/31 rows
  are byte-exact, and the two remaining rows (D1/D2) differ only in the
  `b.w` target `operator delete[]` (`0x79584` vs `0x795a4`).
* The plain `.ARM.exidx` grows by one entry (`0x6f50 -> 0x6f58`, original
  `0x6f70`); `.ARM.extab` shifts `+8` in base but keeps its size, and
  `.rodata` stays at `0x1c9e10`.
* The next plain no-order frontier is `MenuTouchWindow::~MenuTouchWindow`
  (`0x125170` original, `0x125186` ours), displaced by an extra
  `Array<GameRecord*>::~Array()` weak copy at `0x125170`.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `3088`, not `4497`.
* The `MenuTouchWindow` weak-copy placement is the next per-TU emission-order
  experiment, but it was not tested or landed in this pass.

## 14. `Radar::getPlanetDockIndex` placement (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0. The plain no-order link rises from **3097 / 4497** (prefix 3081,
3382 / 4524 exported functions at their original address) to **3099 / 4497**
(prefix 3096, 3384 / 4524). The first plain no-order mismatch moves from
`Radar::getPlanetDockIndex` to `AutoPilotList::fire`.

This is the next frontier recorded in §13.4 of this note (the `Wanted` pass),
after the intervening `MenuTouchWindow` and `PendingProduct` placement passes.

### 14.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate section fingerprint: `.text 0x1337c8 @ 0x69440`,
`.ARM.exidx 0x6f70 @ 0x19cc08`, `.ARM.extab 0x2628c @ 0x1a3b78`,
`.rodata 0x41b67 @ 0x1c9e10`.

The original's `Radar` block from the binary's own `.dynsym`
(`nm -nSD --defined-only`):

```text
0x130704  0x270  Radar::calcDistance(float, float, float, float, float, float)
0x130974  0x024  Radar::getPlanetDockIndex()
0x130998  0x690  Radar::drawCurrentLock(Hud*)
0x131028  0x004  Radar::getLockedGasCloud()
0x13102c  0x004  Radar::getLockedAsteroid()
0x131030  0x006  Radar::unlockAsteroid()
0x131036  0x004  Radar::getLockedEnemy()
0x13103a  0x014  Radar::stationLocked()
0x13104e  0x008  Radar::getTurretScopeWidth()
0x131056  0x006  Radar::isPlasmaInRange()
0x13105c  0x006  Radar::hasScanner()
```

The HD source-layout dump (`_work/original_layout/original_layout.json`,
`gof2hd_1.1.19_ios` -> `Classes/Game/Radar.cpp`) lists the same order for the
symbols it contains: `calcDistance`, `getPlanetDockIndex`, `drawCurrentLock`,
`getLockedAsteroid`, `unlockAsteroid`, `getLockedEnemy`, `stationLocked`,
`isPlasmaInRange`, `hasScanner`. `getLockedGasCloud` and
`getTurretScopeWidth` are Android-2.0.x additions witnessed only by the
Android `.dynsym`; `update(KIPlayer*)` is also an Android addition.

The baseline plain no-order link put `drawCurrentLock` at the original
`getPlanetDockIndex` address and `getPlanetDockIndex` after it:

```text
original 0x130974 Radar::getPlanetDockIndex()   (0x24 bytes)
         0x130998 Radar::drawCurrentLock(Hud*)  (0x690 bytes)
ours     0x130974 Radar::drawCurrentLock(Hud*)
         0x131004 Radar::getPlanetDockIndex()
```

This was the first plain no-order mismatch:
`Radar::getPlanetDockIndex` `0x130974` vs `0x131004`.

### 14.2 Baseline object order

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/weapons/Radar.cpp.obj`
(section-header order = emission order):

```text
...
Radar::draw(Player*, Hud*, int)
Radar::calcDistance(float, float, float, float, float, float)
AbyssEngine::String::String(int)          # weak copy
Radar::drawCurrentLock(Hud*)
Radar::getPlanetDockIndex()
Radar::getLockedGasCloud()
...
```

`calcDistance` is emitted before `drawCurrentLock` even though it is defined
later in the .cpp: its section is created at its first reference inside
`Radar::draw` (line 395), which precedes the `drawCurrentLock` definition. The
`String::String(int)` weak copy is deduplicated by the linker to `0x737b4` and
does not occupy a slot in the `Radar` block.

### 14.3 Landed source change

Only change: move `Radar::getPlanetDockIndex()` in
`src/game/weapons/Radar.cpp` from after `Radar::calcDistance()` to immediately
before `Radar::drawCurrentLock()`. No body was edited.

```diff
@@ after Radar::draw()
+int Radar::getPlanetDockIndex() {
+    SolarSystem *sys = reinterpret_cast<SolarSystem *>(
+        static_cast<intptr_t>(Globals::status->getSystem()));
+    Array<int> *stations = reinterpret_cast<Array<int> *>(sys->getStations());
+    return (*stations)[this->planetDockIndex];
+}
 void Radar::drawCurrentLock(Hud *) {
@@ after Radar::calcDistance()
-int Radar::getPlanetDockIndex() { ... }
 KIPlayer *Radar::getLockedGasCloud() {
```

The object order becomes:

```text
Radar::calcDistance(...)
AbyssEngine::String::String(int)
Radar::getPlanetDockIndex()
Radar::drawCurrentLock(Hud*)
Radar::getLockedGasCloud()
...
```

which matches the original address order. The gate `.so` is unchanged in
`.text` and `.rodata`; it differs only in the 20-byte build-id, two
`.ARM.exidx` extab-pointer words, and the corresponding `.ARM.extab` record
order for the two swapped functions. The `.ARM.exidx` differing-byte count
against the original improves 1744 -> 1743.

### 14.4 Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `0f27c0e1…` | **3097** | 3382 / 4524 | 3081 | `Radar::getPlanetDockIndex` `0x130974` vs `0x131004` |
| `getPlanetDockIndex` before `drawCurrentLock` (landed) | **4497 / 4497** | `63b008d9…` | **3099** | **3384 / 4524** | **3096** | `AutoPilotList::fire` `0x131398` vs `0x13140c` |

The landed delta is **+2 plain byte_exact, +2 functions at their original
address, +15 exact-address prefix**, with the gate counts unchanged.

The whole `Radar` block is now address-exact in the plain no-order link, and
every `Radar` function except the known-incomplete `Radar::draw` is
byte-exact there (checked with `verify.py --show` for each exported `Radar`
symbol). `Radar::getPlanetDockIndex` itself is 36 bytes at `0x130974` in both
the original and the plain link.

Gate `.so` A/B: `cmp -l` reports 180 differing bytes, all outside `.text` and
`.rodata`:

| range | bytes | what |
|---|---:|---|
| `0x1dc..0x1ef` | 20 | `.note.gnu.build-id` |
| `0x1a10cc`..`0x1a10d4` | 3 | `.ARM.exidx` extab-pointer words for `0x130974` / `0x130998` |
| `0x1c213c..0x1c220d` | 157 | `.ARM.extab` record order for the two swapped functions |

The original's `.ARM.exidx` extab pointers for `0x130974` / `0x130998` are
`0x1c2144` / `0x1c2150` (ascending). The baseline gate had them reversed
(`0x1c2204` / `0x1c213c`); the landed gate has them ascending again
(`0x1c213c` / `0x1c2148`), matching the original's relative order. The
absolute offset differs by the known `.ARM.extab` size delta (ours `0x2628c`,
original `0x26298`). Differing-byte counts against the original:

| section | baseline gate | landed gate |
|---|---:|---:|
| `.text` | 111719 | 111719 |
| `.ARM.exidx` | 1744 | **1743** |
| `.ARM.extab` | 113810 | 113810 |
| `.rodata` | 0 | 0 |
| `.data.rel.ro` | 6277 | 6277 |

Plain no-order section layout is unchanged by the pass:
`.text 0x1337e8 @ 0x69440`, `.ARM.exidx 0x6f58 @ 0x19cc28`,
`.ARM.extab 0x2628c @ 0x1a3b80`, `.rodata 0x41b67 @ 0x1c9e10`.

### 14.5 Negative control: full HD tail reorder

Re-emitting the whole `calcDistance` / `getPlanetDockIndex` /
`drawCurrentLock` tail in the HD dump order (`calcDistance`, then
`getPlanetDockIndex`, then `drawCurrentLock`) produces the *same* object order
as §14.3, but is **gate-regressive**:

| variant | gate byte/linked | plain byte | funcs at orig addr | prefix |
|---|---:|---:|---:|---:|
| landed minimal move (A) | **4497 / 4497** | **3099** | **3384 / 4524** | **3096** |
| full tail reorder (B) | 4494 / 4497 | 3097 | 3384 / 4524 | 3096 |

So the minimal move is the correct landing; the extra `calcDistance` source
move costs 3 gate byte_exact and 2 plain byte_exact without changing any
address. The cause was not investigated further (it is a local
`.text`/`.ARM.extab` side effect of the source move, not a placement change).

### 14.6 Next frontier

The first plain no-order mismatch is now:

```text
original 0x131398 AutoPilotList::fire()
ours     0x13140c AutoPilotList::fire()
```

The original `AutoPilotList` order is `ctor`, `down`, `dtor`, `fire`,
`getTargetString`, `up`, `draw`, `touch`. Our `AutoPilotList.cpp.obj` order is
`ctor`, `Array<String*>::Array()`, `ArraySetLength<String*>`, `down`, `dtor`,
`ArrayReleaseClasses<String*>`, `Array<String*>::~Array()`, `up`, `touch`,
`fire`, `getTargetString`, `draw`. This is a separate TU and was not changed
in this pass.

### 14.7 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/radar/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/radar/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/radar/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/radar/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/weapons/Radar.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN5Radar'

# gate .so A/B (baseline vs landed source)
sha256sum /tmp/w3745/radar/gate_base.so cmake-build-match/libgof2hdaa.so
cmp -l /tmp/w3745/radar/gate_base.so cmake-build-match/libgof2hdaa.so | wc -l   # 180
```

### 14.8 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3097 / 4497`, prefix 3081,
  3382 / 4524, first mismatch `Radar::getPlanetDockIndex` `0x130974` vs
  `0x131004`.
* The original emits `Radar` in the `.dynsym` address order listed in §14.1;
  the HD source-layout dump independently lists the same order for every
  symbol it contains.
* The baseline `Radar.cpp.obj` emits `drawCurrentLock` before
  `getPlanetDockIndex`; `calcDistance` is emitted before both because its
  section is created at its first reference in `Radar::draw`.
* Moving `getPlanetDockIndex` before `drawCurrentLock` in the .cpp swaps the
  two object sections and makes the object order match the original address
  order. The move is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0).
* The gate `.so` differs only in the build-id, two `.ARM.exidx` extab
  pointers, and the corresponding `.ARM.extab` record order; `.text` and
  `.rodata` are byte-identical. The `.ARM.exidx` differing-byte count against
  the original improves 1744 -> 1743, and the extab records are now in the
  original's relative order.
* The plain no-order build improves to `3099 / 4497`, prefix 3096,
  3384 / 4524. The whole `Radar` block is address-exact there and every
  `Radar` function except `Radar::draw` is byte-exact.
* The full HD tail reorder (experiment B) is gate-regressive
  (`4494 / 4497`) and plain-regressive (`3097`), so the minimal move is the
  correct landing.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `3099`, not `4497`.
* The next plain no-order frontier is `AutoPilotList::fire` (`0x131398`
  original, `0x13140c` ours); `AutoPilotList.cpp` needs its own emission-order
  pass.

## 15. `AutoPilotList::fire` placement (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0. The plain no-order link rises from **3099 / 4497** (prefix 3096,
3384 / 4524 exported functions at their original address) to **3102 / 4497**
(prefix 3101, 3389 / 4524). The first plain no-order mismatch moves from
`AutoPilotList::fire` to `ListItemWindow::ListItemWindow()`.

This is the next frontier recorded in §14.6 (the `Radar::getPlanetDockIndex`
pass).

### 15.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate section fingerprint (unchanged by this pass): `.text 0x1337c8 @ 0x69440`,
`.ARM.exidx 0x6f70 @ 0x19cc08`, `.ARM.extab 0x2628c @ 0x1a3b78`,
`.rodata 0x41b67 @ 0x1c9e10`.

The original's `AutoPilotList` block from the binary's own `.dynsym`
(`nm -nSD --defined-only`):

```text
0x131064  0x2f8  AutoPilotList::AutoPilotList(Level*)   (C1 == C2)
0x13135c  0x01c  AutoPilotList::down()
0x131378  0x020  AutoPilotList::~AutoPilotList()        (D1 == D2)
0x131398  0x004  AutoPilotList::fire()
0x13139c  0x030  AutoPilotList::getTargetString()
0x1313cc  0x01c  AutoPilotList::up()
0x1313e8  0x0e8  AutoPilotList::draw()
0x1314d0  0x058  AutoPilotList::touch(int, int)
0x131528  0x08c  ListItemWindow::ListItemWindow()       (next TU)
```

The HD source-layout dump (`_work/original_layout/original_layout.json`,
`tu_symbols/gof2hd_1.1.19_ios/Classes/Game/AutoPilotList.cpp`) lists only the
two template symbols (`ArraySetLength<String*>`,
`ArrayReleaseClasses<String*>`), so it does not witness the function order;
the Android `.dynsym` is the authority for the emitted order. TU membership is
witnessed: `_work/original_layout/tu_mapping.md` line 109 maps
`game/core/AutoPilotList.cpp` to `Classes/Game/AutoPilotList.cpp` exactly.

The baseline plain no-order link put `fire` after `touch`:

```text
original 0x131398 AutoPilotList::fire()          (0x4 bytes)
ours     0x13140c AutoPilotList::fire()
```

This was the first plain no-order mismatch (`addr_match.py`).

### 15.2 Baseline object order

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/core/AutoPilotList.cpp.obj`
(section-header order = emission order):

```text
AutoPilotList::AutoPilotList(Level*)
Array<AbyssEngine::String*>::Array()                 # weak copy
void ArraySetLength<AbyssEngine::String*>(...)
AutoPilotList::down()
AutoPilotList::~AutoPilotList()
void ArrayReleaseClasses<AbyssEngine::String*>(...)
Array<AbyssEngine::String*>::~Array()                # weak copy
AutoPilotList::up()
AutoPilotList::touch(int, int)
AutoPilotList::fire()
AutoPilotList::getTargetString()
AutoPilotList::draw()
```

The weak template copies are deduplicated by the linker and do not occupy slots
in the `AutoPilotList` block, so the effective strong order was
`ctor`, `down`, `dtor`, `up`, `touch`, `fire`, `getTargetString`, `draw`.

`down` is emitted before the out-of-line destructor even though its definition
follows the destructor in the .cpp: its section is created at its **first
reference**, the `while ((*this->entries)[this->selected] == nullptr) this->down();`
loop at the end of the constructor. This is the same first-reference emission
rule already recorded for `Radar::calcDistance` in §14.2.

### 15.3 Landed source change

Only change: reorder the definitions in `src/game/core/AutoPilotList.cpp` so
the emitted strong order is the original's. `fire` and `getTargetString` move
before `up`; `touch` moves after `draw`. No body was edited.

```diff
@@ after AutoPilotList::~AutoPilotList()
 void AutoPilotList::down() { ... }
 
+int AutoPilotList::fire() {
+    return this->selected;
+}
+
+String AutoPilotList::getTargetString() {
+    int idx = this->selected;
+    if (idx >= 0 && (uint32_t) idx < this->entries->size())
+        return String(*(*this->entries)[idx], false);
+    return String("", false);
+}
+
 void AutoPilotList::up() { ... }
 
-void AutoPilotList::touch(int px, int py) { ... }
-
-int AutoPilotList::fire() { ... }
-
-String AutoPilotList::getTargetString() { ... }
-
 void AutoPilotList::draw() { ... }
+
+int AutoPilotList::touch(int px, int py) { ... }
```

The object order becomes:

```text
AutoPilotList::AutoPilotList(Level*)
Array<AbyssEngine::String*>::Array()
void ArraySetLength<AbyssEngine::String*>(...)
AutoPilotList::down()
AutoPilotList::~AutoPilotList()
void ArrayReleaseClasses<AbyssEngine::String*>(...)
Array<AbyssEngine::String*>::~Array()
AutoPilotList::fire()
AutoPilotList::getTargetString()
AutoPilotList::up()
AutoPilotList::draw()
AutoPilotList::touch(int, int)
```

which matches the original address order exactly.

### 15.4 Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `0c8c704c…` | **3099** | 3384 / 4524 | 3096 | `AutoPilotList::fire` `0x131398` vs `0x13140c` |
| `fire`/`getTargetString` before `up`, `touch` after `draw` (landed) | **4497 / 4497** | `e753425b…` | **3102** | **3389 / 4524** | **3101** | `ListItemWindow::ListItemWindow()` `0x131528` vs `0x133ad8` |

The landed delta is **+3 plain byte_exact, +5 functions at their original
address, +5 exact-address prefix**, with the gate counts unchanged.

The whole `AutoPilotList` block is now address-exact in the plain no-order
link, and every `AutoPilotList` row in the plain verify report is
`byte=True` (10 rows: ctor C1/C2, down, dtor D1/D2, fire, getTargetString,
up, draw, touch; all sizes exact).

Gate `.so` A/B (`cmp -l` = 59 differing bytes, all outside `.text` and
`.rodata`):

| range | bytes | what |
|---|---:|---|
| `0x1dc..0x1ef` | 20 | `.note.gnu.build-id` |
| `0x1a111c`, `0x1a1124` | 2 | `.ARM.exidx` extab-pointer words for `up` / `touch` |
| `0x1c2298..0x1c22bf` | 37 | `.ARM.extab` record order for the two swapped functions |

The original's `.ARM.exidx` extab pointers for `up` (`0x1313ec`) / `touch`
(`0x1314d4`) are `0x1c22a0` / `0x1c22c0` (ascending). The baseline gate had
them reversed (`0x1c22a4` / `0x1c2298`); the landed gate has them ascending
again (`0x1c2298` / `0x1c22b8`), matching the original's relative order. The
absolute offsets differ by the known `.ARM.extab` size delta (ours `0x2628c`,
original `0x26298`). Differing-byte counts against the original:

| section | baseline gate | landed gate |
|---|---:|---:|
| `.text` | 111241 | 111241 |
| `.ARM.exidx` | 1743 | 1743 |
| `.ARM.extab` (common prefix) | 113810 | 113811 |
| `.rodata` | 0 | 0 |
| `.data.rel.ro` | 0 | 0 |
| `.data` | 0 | 0 |
| `.dynsym` / `.dynstr` / `.hash` | 324 / 1268 / 133 | 324 / 1268 / 133 |
| `.rel.dyn` / `.rel.plt` | 74 / 21 | 74 / 21 |
| `.plt` / `.got` / `.dynamic` / `.gnu.version` | 0 | 0 |

The `+1` `.ARM.extab` common-prefix byte is the absolute-offset shift of the
two records; the relative record order is now the original's.

Plain no-order section layout is unchanged by the pass:
`.text 0x1337e8 @ 0x69440`, `.ARM.exidx 0x6f58 @ 0x19cc28`,
`.ARM.extab 0x2628c @ 0x1a3b80`, `.rodata 0x41b67 @ 0x1c9e10`.

### 15.5 Control: `down`'s source position is irrelevant

Moving the `down` definition to the end of the .cpp (source order `ctor`,
`dtor`, `fire`, `getTargetString`, `up`, `draw`, `touch`, `down`) produces the
**same object section order** and a **byte-identical plain no-order `.so`**
(`cmp` 0 differing bytes, same sha256 `032c4fa4…`). This confirms that `down`
is emitted at its first reference in the constructor, not at its definition;
the minimal landed move is sufficient.

### 15.6 Next frontier

The first plain no-order mismatch is now:

```text
original 0x131528 ListItemWindow::ListItemWindow()   (C1/C2, 0x8c bytes)
ours     0x133ad8 ListItemWindow::ListItemWindow()
```

Our plain `ListItemWindow` order is
`OnTouchBegin`, `shows3DShip`, `OnTouchMove`, `OnTouchEnd`, `render`, `set`,
`dtor`, `draw`, `update`, `ctor`; the original is
`ctor`, `dtor`, `set`, `shows3DShip`, `draw`, `update`, `render`,
`OnTouchBegin`, `OnTouchMove`, `OnTouchEnd`. This is a separate TU
(`src/game/ui/ListItemWindow.cpp`) and was not changed in this pass.

### 15.7 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/autopilot/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/autopilot/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/autopilot/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/autopilot/base_noorder.so

# after the source move
cmake --build cmake-build-match --target check -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/autopilot/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/autopilot/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/autopilot/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/autopilot/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/core/AutoPilotList.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN13AutoPilotList'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/autopilot/gate_after.so
git stash push -- src/game/core/AutoPilotList.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/autopilot/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745/autopilot/gate_base.so /tmp/w3745/autopilot/gate_after.so
cmp -l /tmp/w3745/autopilot/gate_base.so /tmp/w3745/autopilot/gate_after.so | wc -l   # 59
```

### 15.8 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3099 / 4497`, prefix 3096,
  3384 / 4524, first mismatch `AutoPilotList::fire` `0x131398` vs `0x13140c`.
* The original emits `AutoPilotList` in the `.dynsym` address order listed in
  §15.1; the HD dump independently witnesses the TU membership but not the
  function order.
* The baseline object emits `up`, `touch`, `fire`, `getTargetString`, `draw`
  after the destructor; `down` is emitted immediately after the constructor
  because the constructor first-references it.
* Moving `fire`/`getTargetString` before `up` and `touch` after `draw` makes
  the object's strong `.text.*` order match the original address order. The
  move is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0).
* The gate `.so` differs only in the build-id, two `.ARM.exidx` extab
  pointers, and the corresponding `.ARM.extab` record order; `.text` and
  `.rodata` are byte-identical. The `.ARM.exidx` extab records for `up` /
  `touch` are now in the original's ascending relative order.
* The plain no-order build improves to `3102 / 4497`, prefix 3101,
  3389 / 4524. The whole `AutoPilotList` block is address-exact there and all
  10 exported rows are byte-exact.
* The control experiment (moving `down` to the end of the .cpp) produces a
  byte-identical plain no-order `.so`, proving `down`'s source position is
  irrelevant because of the constructor's first reference.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `3102`, not `4497`.
* The next plain no-order frontier is `ListItemWindow::ListItemWindow()`
  (`0x131528` original, `0x133ad8` ours); `ListItemWindow.cpp` needs its own
  emission-order pass.

## 16. `ListItemWindow` emission order (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0. The plain no-order link rises from **3102 / 4497** (prefix 3101,
3389 / 4524 exported functions at their original address) to **3112 / 4497**
(prefix 3172, 3401 / 4524). The first plain no-order mismatch moves from
`ListItemWindow::ListItemWindow()` to `MovingStars::MovingStars()`.

This is the next frontier recorded in §15.6 (the `AutoPilotList::fire` pass).

### 16.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

The original's `ListItemWindow` block from the binary's own `.dynsym`
(`nm -nSD --defined-only`):

```text
0x131528  0x08c  ListItemWindow::ListItemWindow()   (C1 == C2)
0x1315b4  0x0a2  ListItemWindow::~ListItemWindow()  (D1 == D2)
0x131658  0x1aa4 ListItemWindow::set(ListItem*, unsigned, unsigned, unsigned, unsigned, bool)
0x1330fc  0x006  ListItemWindow::shows3DShip()
0x133104  0x764  ListItemWindow::draw()
0x133868  0x160  ListItemWindow::update(int)
0x1339c8  0x0ac  ListItemWindow::render()
0x133a74  0x060  ListItemWindow::OnTouchBegin(int, int)
0x133ad4  0x030  ListItemWindow::OnTouchMove(int, int)
0x133b04  0x060  ListItemWindow::OnTouchEnd(int, int)
```

The HD source-layout dump witnesses the TU
(`_work/original_layout/tu_mapping.md` line 169:
`game/ui/ListItemWindow.cpp` -> `Classes/Game/ListItemWindow.cpp`), but not the
function order; the Android `.dynsym` is the authority for the emitted order.

The baseline plain no-order link put the ctor last:

```text
original 0x131528 ListItemWindow::ListItemWindow()   (0x8c bytes)
ours     0x133ad8 ListItemWindow::ListItemWindow()
```

This was the first plain no-order mismatch (`addr_match.py`).

### 16.2 Baseline object order

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ui/ListItemWindow.cpp.obj`
(section-header order = emission order, weak copies included):

```text
ListItemWindow::OnTouchBegin(int, int)
ListItemWindow::shows3DShip()
ListItemWindow::OnTouchMove(int, int)
ListItemWindow::OnTouchEnd(int, int)
ListItemWindow::render()
ListItemWindow::set(...)
Array<AbyssEngine::String*>::~Array()
Array<AbyssEngine::String*>::Array()
Array<int>::Array()
AbyssEngine::String::String(int)
void ArrayAdd<AbyssEngine::String*>(...)
void ArrayAdd<int>(...)
ListItemWindow::~ListItemWindow()
void ArrayRelease<int>(Array<int>&)
Array<int>::~Array()
__clang_call_terminate
ListItemWindow::draw()
ListItemWindow::update(int)
ListItemWindow::ListItemWindow()
AbyssEngine::AEMath::Matrix::Matrix()
```

The weak template copies are deduplicated by the linker and placed by the
ordering file, so the effective strong order was
`OnTouchBegin`, `shows3DShip`, `OnTouchMove`, `OnTouchEnd`, `render`, `set`,
`dtor`, `draw`, `update`, `ctor` — the reverse of the original block in several
places.

### 16.3 Controlled experiments

Each row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| experiment | gate byte/linked | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---:|---|
| baseline | **4497 / 4497** | **3102** | 3389 / 4524 | 3101 | `ListItemWindow::ListItemWindow()` `0x131528` vs `0x133ad8` |
| A: move only the ctor definition before `OnTouchBegin` | **4497 / 4497** | **3104** | 3391 | 3103 | `ListItemWindow::~ListItemWindow()` `0x1315b4` vs `0x13319c` |
| B: reorder all strong definitions to the original address order (landed) | **4497 / 4497** | **3112** | **3401** | **3172** | `MovingStars::MovingStars()` `0x146050` vs `0x146088` |

Experiment A is gate-neutral and proves the ctor move itself is causal, but it
only recovers the ctor (`C1`/`C2`); the dtor is still displaced. Experiment B
is the strict improvement and is landed.

### 16.4 Landed source change

Only change: reorder the top-level definitions in
`src/game/ui/ListItemWindow.cpp` so the emitted strong order is the original's.
No body was edited. The static const arrays stay immediately before their
current user (`set`, `draw`, `update`) and keep their relative order, so the
object's `.rodata` content order is unchanged.

```text
before (source / object strong order):
  OnTouchBegin, shows3DShip, OnTouchMove, OnTouchEnd, render, set, dtor,
  draw, update, ctor

after:
  ctor, dtor, set, shows3DShip, draw, update, render, OnTouchBegin,
  OnTouchMove, OnTouchEnd
```

The object order becomes (weak copies still interleaved, but all in the
ordering file):

```text
ListItemWindow::ListItemWindow()
AbyssEngine::AEMath::Matrix::Matrix()
ListItemWindow::~ListItemWindow()
Array<AbyssEngine::String*>::~Array()
void ArrayRelease<int>(Array<int>&)
Array<int>::~Array()
__clang_call_terminate
ListItemWindow::set(...)
Array<AbyssEngine::String*>::Array()
Array<int>::Array()
AbyssEngine::String::String(int)
void ArrayAdd<AbyssEngine::String*>(...)
void ArrayAdd<int>(...)
ListItemWindow::shows3DShip()
ListItemWindow::draw()
ListItemWindow::update(int)
ListItemWindow::render()
ListItemWindow::OnTouchBegin(int, int)
ListItemWindow::OnTouchMove(int, int)
ListItemWindow::OnTouchEnd(int, int)
```

which matches the original address order exactly.

### 16.5 Measured before/after

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `423730f2…` | **3102** | 3389 / 4524 | 3101 | `ListItemWindow::ListItemWindow()` `0x131528` vs `0x133ad8` |
| full reorder (landed) | **4497 / 4497** | `1ff6423a…` | **3112** | **3401 / 4524** | **3172** | `MovingStars::MovingStars()` `0x146050` vs `0x146088` |

The landed delta is **+10 plain byte_exact, +12 functions at their original
address, +71 exact-address prefix**, with the gate counts unchanged.

The whole `ListItemWindow` block is now address-exact in the plain no-order
link (all 12 exported rows, `C1`/`C2`, `D1`/`D2`, `set`, `shows3DShip`, `draw`,
`update`, `render`, `OnTouchBegin`, `OnTouchMove`, `OnTouchEnd`). Every row is
`byte=True` in the plain verify report except `render` (the known
`ListItemWindow.h` layout blocker, `x`/`y`/`width` offsets; size is exact).

Gate `.so` A/B (`cmp -l` = 993 differing bytes, all outside `.text` and
`.rodata`):

| range | bytes | what |
|---|---:|---|
| `0x1dc..0x1ef` | 19 | `.note.gnu.build-id` |
| `.ARM.exidx` | 15 | extab-pointer words for the reordered functions |
| `.ARM.extab` | 959 | extab record order for the reordered functions |

`.text` and `.rodata` are byte-identical between the baseline and landed gate
`.so`. The `.ARM.exidx` differing-byte count against the original improves
1743 -> 1737; the `.ARM.extab` common-prefix count moves 113810 -> 113811
(the known `-8` absolute size delta). Parsing the `.ARM.exidx` extab pointers
for the `ListItemWindow` range shows the landed gate now has the original's
ascending relative record order with a constant `-8` offset:

```text
orig  0x131528 CANTUNWIND, 0x131528 -> 0x1c22c8, 0x1315b4 -> 0x1c22e8,
      0x131658 -> 0x1c2304, 0x133104 -> 0x1c26c0, 0x133868 -> 0x1c273c,
      0x1339c8 -> 0x1c274c, 0x133a74 -> 0x1c2758, 0x133ad4 -> 0x1c2764,
      0x133b04 -> 0x1c2770
after 0x131528 CANTUNWIND, 0x131528 -> 0x1c22c0, 0x1315b4 -> 0x1c22e0,
      0x131658 -> 0x1c22fc, 0x133104 -> 0x1c26b8, 0x133868 -> 0x1c2734,
      0x1339c8 -> 0x1c2744, 0x133a74 -> 0x1c2750, 0x133ad4 -> 0x1c275c,
      0x133b04 -> 0x1c2768
```

Plain no-order section layout is unchanged by the pass:
`.text 0x1337e8 @ 0x69440`, `.ARM.exidx 0x6f58 @ 0x19cc28`,
`.ARM.extab 0x2628c @ 0x1a3b80`, `.rodata 0x41b67 @ 0x1c9e10`.

### 16.6 Next frontier

The first plain no-order mismatch is now:

```text
original 0x146050 MovingStars::MovingStars()   (C1/C2, 0x140 bytes)
ours     0x146088 MovingStars::MovingStars()
```

Our plain `MovingStars` order starts with the destructor
(`0x146050 MovingStars::~MovingStars()`), then the ctor at `0x146088`; the
original starts with the ctor. This is a separate TU
(`src/game/world/MovingStars.cpp`) and was not changed in this pass.

### 16.7 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/liw/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/liw/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/liw/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/liw/base_noorder.so

# after the source move
cmake --build cmake-build-match --target check -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/liw/final_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/liw/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/liw/final_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/liw/final_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ui/ListItemWindow.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN14ListItemWindow'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/liw/gate_after.so
git stash push -- src/game/ui/ListItemWindow.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/liw/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745/liw/gate_base.so /tmp/w3745/liw/gate_after.so
cmp -l /tmp/w3745/liw/gate_base.so /tmp/w3745/liw/gate_after.so | wc -l   # 993
```

### 16.8 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3102 / 4497`, prefix 3101,
  3389 / 4524, first mismatch `ListItemWindow::ListItemWindow()` `0x131528`
  vs `0x133ad8`.
* The original emits `ListItemWindow` in the `.dynsym` address order listed in
  §16.1; the HD dump independently witnesses the TU membership.
* The baseline object emits the ctor last, after `update`; the strong order is
  nearly the reverse of the original.
* Moving only the ctor definition before `OnTouchBegin` is gate-neutral and
  makes the ctor address-exact (`plain 3102 -> 3104`), proving the ctor move
  itself is causal; the dtor then becomes the first mismatch.
* Reordering all strong definitions to the original address order is
  gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0) and makes the whole
  `ListItemWindow` block address-exact in the plain no-order link.
* The gate `.so` differs only in the build-id, 15 `.ARM.exidx` bytes and 959
  `.ARM.extab` bytes; `.text` and `.rodata` are byte-identical. The
  `.ARM.exidx` extab records for `ListItemWindow` are now in the original's
  ascending relative order (constant `-8` absolute offset).
* The plain no-order build improves to `3112 / 4497`, prefix 3172,
  3401 / 4524. All `ListItemWindow` rows are byte-exact there except the known
  `render` layout blocker.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `3112`, not `4497`.
* The next plain no-order frontier is `MovingStars::MovingStars()`
  (`0x146050` original, `0x146088` ours); `MovingStars.cpp` needs its own
  emission-order pass. Its plain order currently starts with the destructor,
  so the ctor is not simply last as in `ListItemWindow`.

## 17. `MovingStars` emission order (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0. The plain no-order link rises from **3112 / 4497** (prefix 3172,
3401 / 4524) to **3118 / 4497** (prefix 3179, 3407 / 4524). The first plain
no-order mismatch moves from `MovingStars::MovingStars()` to
`ChoiceWindow::ChoiceWindow()`.

This is the next frontier recorded in §16.6 (the `ListItemWindow` pass).

### 17.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

The original's `MovingStars` block from the binary's own `.dynsym`
(`nm -nSD --defined-only`):

```text
0x146050  0x140  MovingStars::MovingStars()   (C1 == C2)
0x146190  0x036  MovingStars::~MovingStars()  (D1 == D2)
0x1461c8  0x54c  MovingStars::update(int, AbyssEngine::AEMath::Matrix, bool, float)
0x146714  0x048  MovingStars::render()
0x14675c  0x090  MovingStars::translate(AbyssEngine::AEMath::Vector const&)
```

The HD source-layout dump witnesses the TU
(`_work/original_layout/tu_mapping.md` line 60:
`engine/render/MovingStars.cpp` -> `Classes/Game/MovingStars.cpp`), but not the
function order. The Android `.dynsym` is the authority for the emitted order.

The baseline plain no-order link put the destructor first:

```text
original 0x146050 MovingStars::MovingStars()   (0x140 bytes)
ours     0x146088 MovingStars::MovingStars()
```

### 17.2 Baseline object order

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/engine/render/MovingStars.cpp.obj`
(section-header order = emission order; this TU has no weak template copies):

```text
MovingStars::~MovingStars()
MovingStars::MovingStars()
MovingStars::update(int, AbyssEngine::AEMath::Matrix, bool, float)
MovingStars::translate(AbyssEngine::AEMath::Vector const&)
MovingStars::render()
```

The source file `src/engine/render/MovingStars.cpp` defined the destructor
before the constructor, and `translate` before `render`. The original's
address order is the reverse in both pairs.

### 17.3 Controlled experiments

Each row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| experiment | gate byte/linked | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---:|---|
| baseline | **4497 / 4497** | **3112** | 3401 / 4524 | 3172 | `MovingStars::MovingStars()` `0x146050` vs `0x146088` |
| A: move only the ctor before the dtor | **4497 / 4497** | **3116** | 3405 | 3177 | `MovingStars::render()` `0x146714` vs `0x146764` |
| B: move ctor before dtor **and** render before translate (landed) | **4497 / 4497** | **3118** | **3407** | **3179** | `ChoiceWindow::ChoiceWindow()` `0x1467ec` vs `0x14719c` |

Row A flips exactly the four ctor/dtor rows (`C1`, `C2`, `D1`, `D2`) to
byte-exact. Row B additionally flips `render` and `translate`; the six-row
gain is the whole plain `byte_exact` delta (`3112 -> 3118`) and there are no
losses. `MovingStars::update` was already byte-exact in the baseline because
the ctor and dtor sizes are unchanged by their swap (`0x140 + 0x36`), so its
address (`0x1461c8`) did not move.

### 17.4 Landed source change

Only change: reorder the top-level definitions in
`src/engine/render/MovingStars.cpp` so the emitted order is the original's.
No body was edited; the `namespace AbyssEngine { ... }` forward declaration
block stays between the ctor and the dtor, where it was relative to the ctor.

```text
before (source / object order):
  dtor, ctor, update, translate, render

after:
  ctor, dtor, update, render, translate
```

The object order becomes exactly the original address order:

```text
MovingStars::MovingStars()
MovingStars::~MovingStars()
MovingStars::update(int, AbyssEngine::AEMath::Matrix, bool, float)
MovingStars::render()
MovingStars::translate(AbyssEngine::AEMath::Vector const&)
```

### 17.5 Measured before/after

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `ebf35659…` | **3112** | 3401 / 4524 | 3172 | `MovingStars::MovingStars()` `0x146050` vs `0x146088` |
| full reorder (landed) | **4497 / 4497** | `a6e134f6…` | **3118** | **3407 / 4524** | **3179** | `ChoiceWindow::ChoiceWindow()` `0x1467ec` vs `0x14719c` |

The landed delta is **+6 plain byte_exact, +6 functions at their original
address, +7 exact-address prefix**, with the gate counts unchanged.

The whole `MovingStars` block is now address-exact in the plain no-order link
(all 8 exported rows including `Level::enableMovingStars` are at the original
address). Every `MovingStars` row is `byte=True` in the plain verify report
except `Level::enableMovingStars` (its call target is unchanged; its byte
difference is the PLT slot, not this TU's order).

Gate `.so` A/B (`cmp -l` = 36 differing bytes, all outside `.text` and
`.rodata`):

| range | bytes | what |
|---|---:|---|
| `0x1dc..0x1ef` | 20 | `.note.gnu.build-id` |
| `.ARM.exidx` | 4 | extab-pointer words for `MovingStars` |
| `.ARM.extab` | 12 | extab record order for `MovingStars` |

`.text`, `.rodata`, `.data.rel.ro`, `.data`, `.plt`, `.got`, `.dynamic` and
`.rel.plt` are byte-identical between the baseline and landed gate `.so`.
The differing-byte counts against the original are unchanged by the pass
(`.text` 107941, `.ARM.exidx` 1737, `.ARM.extab` 113818, `.rodata` 0,
`.rel.plt` 21, `.plt`/`.got`/`.dynamic` 0), because the reordered records
already differed by their absolute offsets.

Parsing the `.ARM.exidx` extab pointers for the `MovingStars` range shows the
landed gate now has the original's ascending relative record order with a
constant `-4` absolute offset (the known `.ARM.extab` size delta):

```text
orig  0x146050 -> 0x1c2c98, 0x146190 -> 0x1c2ca4, 0x1461c8 -> 0x1c2cb0,
      0x146714 -> 0x1c2cc0, 0x14675c -> 0x1c2ccc
base  0x146050 -> 0x1c2ca0, 0x146190 -> 0x1c2c94, 0x1461c8 -> 0x1c2cac,
      0x146714 -> 0x1c2cc8, 0x14675c -> 0x1c2cbc   # ctor/dtor and render/translate reversed
after 0x146050 -> 0x1c2c94, 0x146190 -> 0x1c2ca0, 0x1461c8 -> 0x1c2cac,
      0x146714 -> 0x1c2cbc, 0x14675c -> 0x1c2cc8
```

Plain no-order section layout is unchanged by the pass:
`.text 0x1337e8 @ 0x69440`, `.ARM.exidx 0x6f58 @ 0x19cc28`,
`.ARM.extab 0x2628c @ 0x1a3b80`, `.rodata 0x41b67 @ 0x1c9e10`.

### 17.6 Next frontier

The first plain no-order mismatch is now:

```text
original 0x1467ec ChoiceWindow::ChoiceWindow()   (C1/C2, 0x84 bytes)
ours     0x14719c ChoiceWindow::ChoiceWindow()
```

Our plain `ChoiceWindow` order is
`OnTouchMove, hasChoice, setWidth, set(String,String), set(6-arg), set(String,bool),
set(String,String,bool), OnTouchEnd, set(String), removeButtons, OnTouchBegin,
dtor, setHeight, setMedal, ctor, setMiscButton, update, draw`; the original is
`ctor, dtor, set(String), set(String,String,bool), set(String,String),
set(6-arg), set(String,bool), removeButtons, hasChoice, setWidth, setHeight,
setMedal, setMiscButton, update, draw, OnTouchBegin, OnTouchMove, OnTouchEnd`.
This is a separate TU (`src/game/ui/ChoiceWindow.cpp`) and was not changed in
this pass.

### 17.7 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/ms/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/ms/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/ms/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/ms/base_noorder.so

# after the source move
cmake --build cmake-build-match --target check -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/ms/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/ms/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/ms/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/ms/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/engine/render/MovingStars.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN11MovingStars'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/ms/gate_after.so
git stash push -- src/engine/render/MovingStars.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/ms/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745/ms/gate_base.so /tmp/w3745/ms/gate_after.so
cmp -l /tmp/w3745/ms/gate_base.so /tmp/w3745/ms/gate_after.so | wc -l   # 36
```

### 17.8 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3112 / 4497`, prefix 3172,
  3401 / 4524, first mismatch `MovingStars::MovingStars()` `0x146050` vs
  `0x146088`.
* The original emits `MovingStars` in the `.dynsym` address order listed in
  §17.1; the HD dump independently witnesses the TU membership.
* The baseline object emits the dtor before the ctor and `translate` before
  `render`, the reverse of the original address order.
* Moving only the ctor before the dtor is gate-neutral and flips exactly the
  four ctor/dtor rows to byte-exact (`plain 3112 -> 3116`), proving the ctor
  move itself is causal; the next mismatch is `render`.
* Moving ctor+dtor and render+translate to the original address order is
  gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0) and makes the whole
  `MovingStars` block address-exact in the plain no-order link.
* The gate `.so` differs only in the build-id, 4 `.ARM.exidx` bytes and 12
  `.ARM.extab` bytes; `.text` and `.rodata` are byte-identical. The
  `.ARM.exidx` extab records for `MovingStars` are now in the original's
  ascending relative order (constant `-4` absolute offset).
* The plain no-order build improves to `3118 / 4497`, prefix 3179,
  3407 / 4524. All `MovingStars` rows are byte-exact there except
  `Level::enableMovingStars` (PLT-slot difference, not this TU's order).

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `3118`, not `4497`.
* The next plain no-order frontier is `ChoiceWindow::ChoiceWindow()`
  (`0x1467ec` original, `0x14719c` ours); `ChoiceWindow.cpp` needs its own
  emission-order pass.

## 18. `TargetFollowCamera` emission order (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0. The plain no-order link rises from **2866 / 4497** (prefix 3504,
3510 / 4524) to **2954 / 4497** (prefix 3557, 3709 / 4524). The first plain
no-order mismatch moves from `TargetFollowCamera::~TargetFollowCamera()` to
`Mission::setFailed(bool)`.

This is the next frontier recorded in
`_work/reconstructions/statuswindow_accessor_order_w3745.md` §6.

### 18.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.91
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
.data.rel.ro     0x002fe8 @ 0x20cbb0
```

The original's `TargetFollowCamera` block from the binary's own `.dynsym`
(`nm -nSD --defined-only`):

```text
0x15a4c8  TargetFollowCamera::TargetFollowCamera(unsigned, AEGeometry*, Vector, Vector)  (C1 == C2)
0x15a688  TargetFollowCamera::aproximateCooefficientsForAproximationOfDampingFunktion(...)
0x15a998  TargetFollowCamera::~TargetFollowCamera()                                     (D1 == D2)
0x15a99a  TargetFollowCamera::setActive(bool)
0x15a9a0  TargetFollowCamera::setTarget(AEGeometry*)
0x15a9a4  TargetFollowCamera::getTarget()
0x15a9a8  TargetFollowCamera::setTargetOffset(Vector const&)
0x15a9ae  TargetFollowCamera::getTargetOffset()
0x15a9b2  TargetFollowCamera::setCamOffset(Vector const&)
0x15a9cc  TargetFollowCamera::getCamOffset()
0x15a9d0  TargetFollowCamera::setPosition(float, float, float)
0x15a9d6  TargetFollowCamera::setPosition(Vector const&)
0x15a9e4  TargetFollowCamera::getPosition()
0x15a9e8  TargetFollowCamera::translate(float, float, float)
0x15aa48  TargetFollowCamera::update(int)
0x15b280  TargetFollowCamera::translateNoUpdate(float, float, float)
0x15b2d8  TargetFollowCamera::enableFirstPersonCam(bool)
0x15b32c  TargetFollowCamera::hideShipForFirstPersonCam()
0x15b332  TargetFollowCamera::setFirstPersonMatrix(Matrix&)
0x15b338  TargetFollowCamera::setBoostPercentage(float, int)
0x15b34a  TargetFollowCamera::setRumblePercentage(float, int)
0x15b350  TargetFollowCamera::setFixed(bool)
0x15b358  TargetFollowCamera::setLocal(Matrix)
0x15b3d4  TargetFollowCamera::getLocal()
0x15b3e4  TargetFollowCamera::setLocked(bool)
0x15b464  TargetFollowCamera::setRotationAroundTarget(bool)
0x15b46c  TargetFollowCamera::rotateAroundTarget(float, float, float)
0x15b498  TargetFollowCamera::zoomTarget(float)
0x15b49e  TargetFollowCamera::setLookAtCam(bool)
0x15b4a4  TargetFollowCamera::calculateCoefficents(float)
0x15b50a  TargetFollowCamera::setFastForwardMode(bool)
0x15b574  TargetFollowCamera::setShipHandling(float)
0x15b5c8  TargetFollowCamera::isInFastForwardMode()
0x15b5ce  TargetFollowCamera::isInLookAtMode()
0x15b5d4  TargetFollowCamera::resetShipHandling()
0x15b5ec  TargetFollowCamera::hit()
0x15b60e  TargetFollowCamera::hitSmall()
0x15b62c  TargetFollowCamera::useTargetsUpVector(bool)
0x15b632  TargetFollowCamera::roll(float)
0x15b644  TargetFollowCamera::setRoll(float)
```

The HD source-layout dump independently witnesses this order
(`_work/original_layout/original_layout.json`,
`gof2hd_1.1.19_ios` -> `Classes/Game/TargetFollowCamera.cpp`): constructor,
`aproximateCooefficients...`, constructor, destructor, `setActive`, `setTarget`,
`getTarget`, `setTargetOffset`, `getTargetOffset`, `setCamOffset`,
`getCamOffset`, `setPosition(float,float,float)`, `setPosition(Vector const&)`,
`getPosition`, `translate`, `update`, `translateNoUpdate`,
`enableFirstPersonCam`, `hideShipForFirstPersonCam`, `setFirstPersonMatrix`,
`setBoostPercentage`, `setRumblePercentage`, `setFixed`, `setLocal`,
`setRotationAroundTarget`, `rotateAroundTarget`, `zoomTarget`, `setLookAtCam`,
`calculateCoefficents`, `setFastForwardMode`, `setShipHandling`,
`isInFastForwardMode`, `isInLookAtMode`, `resetShipHandling`, `hit`,
`hitSmall`, `useTargetsUpVector`. The Android 2.0.16 binary additionally has
`getLocal`, `setLocked`, `roll` and `setRoll`; their binary addresses place
them exactly where the landed source now defines them.

### 18.2 Baseline object order

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ship/TargetFollowCamera.cpp.obj`
(section-header order = emission order):

```text
TargetFollowCamera::TargetFollowCamera()
AEMath::Matrix::Matrix()
TargetFollowCamera::aproximateCooefficientsForAproximationOfDampingFunktion(...)
TargetFollowCamera::getTarget()
TargetFollowCamera::getPosition()
TargetFollowCamera::getTargetOffset()
TargetFollowCamera::getCamOffset()
TargetFollowCamera::getLocal()
TargetFollowCamera::setLocal(Matrix)
TargetFollowCamera::setTarget(AEGeometry*)
TargetFollowCamera::zoomTarget(float)
TargetFollowCamera::setRoll(float)
TargetFollowCamera::roll(float)
TargetFollowCamera::~TargetFollowCamera()
TargetFollowCamera::setLookAtCam(bool)
TargetFollowCamera::setActive(bool)
TargetFollowCamera::setFixed(bool)
TargetFollowCamera::setRotationAroundTarget(bool)
TargetFollowCamera::useTargetsUpVector(bool)
TargetFollowCamera::isInLookAtMode()
TargetFollowCamera::isInFastForwardMode()
TargetFollowCamera::hideShipForFirstPersonCam()
TargetFollowCamera::setFirstPersonMatrix(Matrix&)
TargetFollowCamera::setCamOffset(Vector const&)
TargetFollowCamera::setTargetOffset(Vector const&)
TargetFollowCamera::setPosition(Vector const&)
TargetFollowCamera::setPosition(float, float, float)
TargetFollowCamera::setRumblePercentage(float, int)
TargetFollowCamera::setBoostPercentage(float, int)
TargetFollowCamera::enableFirstPersonCam(bool)
TargetFollowCamera::rotateAroundTarget(float, float, float)
TargetFollowCamera::hit()
TargetFollowCamera::hitSmall()
TargetFollowCamera::setLocked(bool)
TargetFollowCamera::update(int)
TargetFollowCamera::translateNoUpdate(float, float, float)
TargetFollowCamera::translate(float, float, float)
TargetFollowCamera::setShipHandling(float)
TargetFollowCamera::calculateCoefficents(float)
TargetFollowCamera::resetShipHandling()
TargetFollowCamera::setFastForwardMode(bool)
```

The first plain no-order mismatch was exactly the destructor:

```text
original 0x15a998 TargetFollowCamera::~TargetFollowCamera()
ours     0x15aa56 TargetFollowCamera::~TargetFollowCamera()
```

### 18.3 The change

Only the top-level definition order in `src/game/ship/TargetFollowCamera.cpp`
changed. No body text changed: extracting the 40 definition blocks from the
before and after files gives the same 40 blocks byte-for-byte, only permuted.
The new order is the original `.dynsym` address order above, with
`aproximateCooefficients...` placed immediately after the constructor (the
constructor calls it, so clang emits it there anyway) and the destructor next.
The emission order after the reorder is:

```text
TargetFollowCamera::TargetFollowCamera()
AEMath::Matrix::Matrix()
TargetFollowCamera::aproximateCooefficientsForAproximationOfDampingFunktion(...)
TargetFollowCamera::~TargetFollowCamera()
TargetFollowCamera::setActive(bool)
TargetFollowCamera::setTarget(AEGeometry*)
TargetFollowCamera::getTarget()
TargetFollowCamera::setTargetOffset(Vector const&)
TargetFollowCamera::getTargetOffset()
TargetFollowCamera::setCamOffset(Vector const&)
TargetFollowCamera::getCamOffset()
TargetFollowCamera::setPosition(float, float, float)
TargetFollowCamera::setPosition(Vector const&)
TargetFollowCamera::getPosition()
TargetFollowCamera::translate(float, float, float)
TargetFollowCamera::update(int)
TargetFollowCamera::translateNoUpdate(float, float, float)
TargetFollowCamera::enableFirstPersonCam(bool)
TargetFollowCamera::hideShipForFirstPersonCam()
TargetFollowCamera::setFirstPersonMatrix(Matrix&)
TargetFollowCamera::setBoostPercentage(float, int)
TargetFollowCamera::setRumblePercentage(float, int)
TargetFollowCamera::setFixed(bool)
TargetFollowCamera::setLocal(Matrix)
TargetFollowCamera::getLocal()
TargetFollowCamera::setLocked(bool)
TargetFollowCamera::setRotationAroundTarget(bool)
TargetFollowCamera::rotateAroundTarget(float, float, float)
TargetFollowCamera::zoomTarget(float)
TargetFollowCamera::setLookAtCam(bool)
TargetFollowCamera::calculateCoefficents(float)
TargetFollowCamera::setFastForwardMode(bool)
TargetFollowCamera::setShipHandling(float)
TargetFollowCamera::isInFastForwardMode()
TargetFollowCamera::isInLookAtMode()
TargetFollowCamera::resetShipHandling()
TargetFollowCamera::hit()
TargetFollowCamera::hitSmall()
TargetFollowCamera::useTargetsUpVector(bool)
TargetFollowCamera::roll(float)
TargetFollowCamera::setRoll(float)
```

This matches the original address order exactly. The two forward-reference
pulls are visible and match the binary: `setTargetOffset` pulls
`getTargetOffset` after it, and `setCamOffset` pulls `getCamOffset` after it;
`translate` pulls `update` after it; `setFastForwardMode` pulls
`setShipHandling` after it.

### 18.4 Measured A/B

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `c2f65daf…` | **2866** | 3510 / 4524 | 3504 | `TargetFollowCamera::~TargetFollowCamera()` `0x15a998` vs `0x15aa56` |
| full reorder (landed) | **4497 / 4497** | `9f5e224a…` | **2954** | **3709 / 4524** | **3557** | `Mission::setFailed(bool)` `0x15bac0` vs `0x15bac4` |

The landed delta is **+88 plain byte_exact, +199 functions at their original
address, +53 exact-address prefix**, with the gate counts unchanged.

All **47** exported rows that mention `TargetFollowCamera` are now at their
original address in the plain no-order link (42 `TargetFollowCamera` dynsym
rows from the 40 definitions, with `C1`/`C2` and `D1`/`D2` duplicated, plus
the 2 `PlayerEgo`, 1 `Explosion` and 2 `LevelScript` users). The
plain no-order `byte_exact` count for those 47 rows rises from **32 to 41**;
the six remaining non-byte rows are `setShipHandling`, `setTargetOffset`,
`resetShipHandling`, `setFirstPersonMatrix`, `translate` and
`PlayerEgo::setTargetFollowCamera`, all `pct 100.0` / `linked True` with only
PLT or literal-pool differences.

Gate `.so` A/B (`cmp -l` = 85 differing bytes, all outside `.text` and
`.rodata`):

| range | bytes | what |
|---|---:|---|
| `.note.gnu.build-id` | 19 | build-id |
| `.ARM.exidx` | 6 | exidx record order for the reordered functions |
| `.ARM.extab` | 60 | extab record order for the reordered functions |

`.text`, `.rodata` and `.data.rel.ro` are byte-identical between the baseline
and landed gate `.so`; every ELF section size and base is unchanged
(`.text 0x1337c8 @ 0x69440`, `.ARM.exidx 0x6f70 @ 0x19cc08`,
`.ARM.extab 0x2628c @ 0x1a3b78`, `.rodata 0x41b67 @ 0x1c9e10`). The
differing-byte counts against the original are unchanged for `.text`
(108023) and `.ARM.exidx` (1731), and improve for `.ARM.extab`
(113823 -> 113820).

Plain no-order `.rodata` content is byte-identical before and after; its base
shifts `0x1c9e20 -> 0x1c9e30` because the plain `.ARM.exidx` grows
`0x6f68 -> 0x6f70` and `.ARM.extab` shifts `+8`. The plain `.text` size is
`0x1337e8` in both.

### 18.5 Next frontier

The first plain no-order mismatch is now:

```text
original 0x15bac0 Mission::setFailed(bool)   (4 bytes)
ours     0x15bac4 Mission::setFailed(bool)
```

`src/game/mission/Mission.cpp` defines `hasFailed()` before `setFailed()` and
`hasWon()` before `setWon()`; the original address order is `setFailed`,
`hasFailed`, `setWon`, `hasWon`. This is a separate TU and was not changed in
this pass.

### 18.6 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements (before and after the source reorder)
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/tfc/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/tfc/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/tfc/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/tfc/base_noorder.so

python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/tfc/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/tfc/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/tfc/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/tfc/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ship/TargetFollowCamera.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN18TargetFollowCamera'

# gate .so A/B (baseline vs landed source)
sha256sum /tmp/w3745/tfc/gate_base.so cmake-build-match/libgof2hdaa.so
cmp -l /tmp/w3745/tfc/gate_base.so cmake-build-match/libgof2hdaa.so | wc -l   # 85
```

### 18.7 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `2866 / 4497`, prefix 3504,
  3510 / 4524, first mismatch `TargetFollowCamera::~TargetFollowCamera()`
  `0x15a998` vs `0x15aa56`.
* The original emits `TargetFollowCamera` in the `.dynsym` address order
  listed in §18.1; the `gof2hd_1.1.19_ios` source-layout dump independently
  witnesses the same order for the functions it contains.
* The baseline object emitted the accessor block before the destructor and
  `update`/`translate` in the wrong order; the reordered source emits exactly
  the original address order.
* The reorder is a pure definition permutation: the 40 definition blocks are
  byte-identical before and after.
* The reorder is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0, every
  ELF section size/base unchanged) and improves the plain no-order build to
  `2954 / 4497`, 3709 / 4524 addresses, prefix 3557.
* All 47 exported rows that mention `TargetFollowCamera` are address-exact in
  the plain no-order link; the 47-row plain `byte_exact` count rises from 32
  to 41.
* The gate `.so` differs only in the build-id, 6 `.ARM.exidx` bytes and 60
  `.ARM.extab` bytes; `.text`, `.rodata` and `.data.rel.ro` are byte-identical.
  `.ARM.extab` differing bytes against the original improve 113823 -> 113820.
* The next plain no-order frontier is `Mission::setFailed(bool)`
  (`0x15bac0` original, `0x15bac4` ours).

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `2954`, not `4497`.
* The remaining six non-byte `TargetFollowCamera` rows are PLT/literal-pool
  differences, not emission-order differences; they are expected to clear
  only when the PLT/GOT first-reference order converges.

## 19. `Mission::setFailed` emission order (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0, and the gate `.so` is **byte-identical** to the baseline
(sha256 `9f5e224a…`, `cmp` status 0). The plain no-order link keeps
`byte_exact 2954 / linked_exact 4497`, but its exact-address count rises from
**3709 / 4524** to **3713 / 4524** and its exact-address prefix from **3557**
to **3615**. The first plain no-order mismatch moves from
`Mission::setFailed(bool)` to `SentryGun::render()`.

This is the next frontier recorded in §18.5 (the `TargetFollowCamera` pass).

### 19.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (unchanged by this pass):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

The original's `Mission` block from the binary's own `.dynsym`
(`nm -nSD --defined-only`):

```text
0x15bac0  0x04  Mission::setFailed(bool)
0x15bac4  0x04  Mission::hasFailed()
0x15bac8  0x04  Mission::setWon(bool)
0x15bacc  0x04  Mission::hasWon()
```

The HD source-layout dump independently witnesses the same order
(`_work/original_layout/original_layout.json`,
`gof2hd_1.1.19_ios` -> `Classes/Game/Mission.cpp`; the `gof2sd_1.0.1_ios`
dump agrees):

```text
_Mission::setFailed(bool)
_Mission::hasFailed()
_Mission::setWon(bool)
_Mission::hasWon()
```

The baseline `Mission.cpp.obj` emitted the getter before the setter in both
pairs:

```text
Mission::hasFailed()
Mission::setFailed(bool)
Mission::hasWon()
Mission::setWon(bool)
```

That made the plain no-order link put `setFailed` at `0x15bac4` instead of
`0x15bac0`. The first plain no-order mismatch was exactly:

```text
original 0x15bac0 Mission::setFailed(bool)   (4 bytes)
ours     0x15bac4 Mission::setFailed(bool)
```

### 19.2 Landed source change

Only change: reorder the four definitions in `src/game/mission/Mission.cpp`
to the original address order. No body was edited.

```diff
@@ after Mission::isVisible()
-unsigned char Mission::hasFailed() { return this->failed; }
-
 void Mission::setFailed(bool failed) {
     this->failed = failed ? 1 : 0;
 }
 
-unsigned char Mission::hasWon() { return this->won; }
+unsigned char Mission::hasFailed() { return this->failed; }
 
 void Mission::setWon(bool won) {
     this->won = won ? 1 : 0;
 }
 
+unsigned char Mission::hasWon() { return this->won; }
+
 void Mission::setProductionGoods(int a, int b) {
```

The resulting object order is exactly the original address order:

```text
Mission::setFailed(bool)
Mission::hasFailed()
Mission::setWon(bool)
Mission::hasWon()
```

The four `.text.*` section bodies are unchanged (same 4-byte sizes); only
their order changed.

### 19.3 Measured before/after

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `9f5e224a…` | **2954** | 3709 / 4524 | 3557 | `Mission::setFailed` `0x15bac0` vs `0x15bac4` |
| landed | **4497 / 4497** | `9f5e224a…` | **2954** | **3713 / 4524** | **3615** | `SentryGun::render` `0x15c3d0` vs `0x15c464` |

The landed delta is **+4 functions at their original address, +58
exact-address prefix**, with the gate counts unchanged and the gate `.so`
byte-identical.

The plain `byte_exact` count is unchanged because all four functions were
already byte-exact at their old addresses: each is a 4-byte body with no
PC-relative literal or branch, so its bytes do not depend on its address.
The improvement is placement-only, which is exactly what the ordering-file
removal campaign measures. The plain link now has the whole `Mission` block
at its original address:

```text
$NM -nD --defined-only /tmp/w3745/mission/after_noorder.so | grep -E '_ZN7Mission(9setFailed|9hasFailed|6setWon|6hasWon)'
0015bac0 T _ZN7Mission9setFailedEb
0015bac4 T _ZN7Mission9hasFailedEv
0015bac8 T _ZN7Mission6setWonEb
0015bacc T _ZN7Mission6hasWonEv
```

All four rows are `byte=True`, `linked=True`, `pct 100.0` in both the
baseline and landed plain verify reports; no other row changed its
`byte`/`linked`/`pct` status (checked by diffing the two `report.json`
files).

The gate `.so` is byte-identical before and after:

```sh
sha256sum /tmp/w3745/mission/gate_base.so /tmp/w3745/mission/gate_after.so
# 9f5e224a149be0c841c4c004ab9b929b45ecbbe64e0a95e7211e5b2a67acefd4  (both)
cmp -l /tmp/w3745/mission/gate_base.so /tmp/w3745/mission/gate_after.so | wc -l
# 0
```

Plain no-order section layout is unchanged by the pass:
`.text 0x1337e8 @ 0x69440`, `.ARM.exidx 0x6f70 @ 0x19cc28`,
`.ARM.extab 0x2628c @ 0x1a3b98`, `.rodata 0x41b67 @ 0x1c9e30`.

### 19.4 Next frontier

The first plain no-order mismatch is now:

```text
original 0x15c3d0 SentryGun::render()   (2 bytes)
ours     0x15c464 SentryGun::render()
```

The original `SentryGun` order is `ctor`, `D1/D2`, `D0`, `render`, `update`
(`0x15c380`, `0x15c3bc`, `0x15c3c0`, `0x15c3d0`, `0x15c3d2`). Our
`SentryGun.cpp.obj` emits `update` before `render`, so the plain link puts
`render` after `update`'s 0x94-byte body. This is a separate TU
(`src/game/weapons/SentryGun.cpp`) and was not changed in this pass.

### 19.5 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements (before and after the source reorder)
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/mission/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/mission/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/mission/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/mission/base_noorder.so

cmake --build cmake-build-match --target check -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/mission/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/mission/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/mission/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/mission/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/mission/Mission.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN7Mission'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/mission/gate_after.so
git stash push -- src/game/mission/Mission.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/mission/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745/mission/gate_base.so /tmp/w3745/mission/gate_after.so
cmp -l /tmp/w3745/mission/gate_base.so /tmp/w3745/mission/gate_after.so | wc -l   # 0
```

### 19.6 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `2954 / 4497`, prefix 3557,
  3709 / 4524, first mismatch `Mission::setFailed` `0x15bac0` vs `0x15bac4`.
* The original emits `Mission` in the `.dynsym` address order
  `setFailed, hasFailed, setWon, hasWon`; the `gof2hd_1.1.19_ios` and
  `gof2sd_1.0.1_ios` source-layout dumps independently witness the same
  order.
* The baseline object emitted `hasFailed, setFailed, hasWon, setWon`; moving
  the four definitions to the original order makes the object's `.text.*`
  order match the original address order. The four section bodies are
  unchanged (4 bytes each).
* The landed move is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0,
  every ELF section size/base unchanged) and the gate `.so` is byte-identical
  to the baseline (same SHA-256, `cmp` status 0).
* The plain no-order build now has all four `Mission` functions at their
  original addresses: exact addresses 3709 -> 3713, exact-address prefix
  3557 -> 3615. Plain `byte_exact` is unchanged at 2954 because the four
  functions were already byte-exact at the wrong addresses (their 4-byte
  bodies contain no address-dependent words).
* The next plain no-order frontier is `SentryGun::render`
  (`0x15c3d0` original, `0x15c464` ours); the original `SentryGun` order is
  `ctor, D1/D2, D0, render, update`, while our object emits `update` before
  `render`.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `2954`, not `4497`.
* The `SentryGun` reorder is the obvious next per-TU source-order experiment,
  but it is a separate TU and was not tested or landed in this pass.

## 20. `SentryGun::render` emission order (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0, and the gate `.so` is **byte-identical** to the baseline
(sha256 `b79f0185…`, `cmp` status 0). The plain no-order link keeps
`byte_exact 2954 / linked_exact 4497`, but its exact-address count rises from
**3713 / 4524** to **3715 / 4524** and its exact-address prefix from **3615**
to **3625**. The first plain no-order mismatch moves from
`SentryGun::render()` to `NewsTicker::update(int)`.

This is the next frontier recorded in §19.4 (the `Mission` pass).

### 20.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`) at branch `wave87-w3745`, HEAD `7dc0bf4c`:

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (unchanged by this pass):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
```

The original's `SentryGun` block from the binary's own `.dynsym`
(`nm -nSD --defined-only`):

```text
0x15c380  0x3c  SentryGun::SentryGun(Gun*, int, int, int, Level*)
0x15c380  0x3c  SentryGun::SentryGun(Gun*, int, int, int, Level*)   # C1/C2 alias
0x15c3bc  0x04  SentryGun::~SentryGun()                            # D1/D2 alias
0x15c3c0  0x10  SentryGun::~SentryGun()                            # D0
0x15c3d0  0x02  SentryGun::render()
0x15c3d2  0x94  SentryGun::update(int)
```

The HD source-layout dump independently witnesses the same order
(`_work/original_layout/original_layout.json` ->
`Classes/Game/SentryGun.cpp`, object `SentryGun.o`):

```text
_SentryGun::SentryGun(Gun*, int, int, int, Level*)
_SentryGun::~SentryGun()
_SentryGun::~SentryGun()
_SentryGun::render()
_SentryGun::update(int)
```

The baseline `SentryGun.cpp.obj` emitted `update` before `render`:

```text
SentryGun::SentryGun(Gun*, int, int, int, Level*)
SentryGun::~SentryGun()
SentryGun::~SentryGun()
SentryGun::update(int)
SentryGun::render()
AbstractGun::isRocketGun()
AbstractGun::isBombGun()
AbstractGun::isMineGun()
AbstractGun::isAutoTurret()
```

That made the plain no-order link put `render` after `update`'s 0x94-byte
body. The first plain no-order mismatch was exactly:

```text
original 0x15c3d0 SentryGun::render()   (2 bytes)
ours     0x15c464 SentryGun::render()
```

### 20.2 Landed source change

Only change: move the `SentryGun::render()` definition in
`src/game/weapons/SentryGun.cpp` from after `SentryGun::update()` to before it.
No body was edited.

```diff
@@ after SentryGun::~SentryGun()
+void SentryGun::render() {
+}
+
 void SentryGun::update(int dt) {
     ...
 }
-
-void SentryGun::render() {
-}
```

The resulting object order is exactly the original address order:

```text
SentryGun::SentryGun(Gun*, int, int, int, Level*)
SentryGun::~SentryGun()
SentryGun::~SentryGun()
SentryGun::render()
SentryGun::update(int)
AbstractGun::isRocketGun()
AbstractGun::isBombGun()
AbstractGun::isMineGun()
AbstractGun::isAutoTurret()
```

The `.text.*` section bodies are unchanged (2-byte `render`, 0x94-byte
`update`); only their order changed.

### 20.3 Measured before/after

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `b79f0185…` | **2954** | 3713 / 4524 | 3615 | `SentryGun::render` `0x15c3d0` vs `0x15c464` |
| landed | **4497 / 4497** | `b79f0185…` | **2954** | **3715 / 4524** | **3625** | `NewsTicker::update` `0x15e0e4` vs `0x15e1e4` |

The landed delta is **+2 functions at their original address, +10
exact-address prefix**, with the gate counts unchanged and the gate `.so`
byte-identical.

The plain `byte_exact` count is unchanged because both functions were already
byte-exact at their old addresses: `render` is a 2-byte `bx lr`, and
`update`'s 148-byte body was already byte-exact in both links (its PC-relative
words resolve to the same data). The improvement is placement-only, which is
exactly what the ordering-file removal campaign measures. No report row
changed its `byte`/`linked`/`pct` status (diff of the two `report.json`
files: 0 changed rows).

The plain no-order link now has the whole `SentryGun` block at its original
address:

```text
$NM -nD --defined-only /tmp/w3745/sentry/after_noorder.so | grep '_ZN9SentryGun'
0015c380 T _ZN9SentryGunC1EP3GuniiiP5Level
0015c380 T _ZN9SentryGunC2EP3GuniiiP5Level
0015c3bc T _ZN9SentryGunD1Ev
0015c3bc T _ZN9SentryGunD2Ev
0015c3c0 T _ZN9SentryGunD0Ev
0015c3d0 T _ZN9SentryGun6renderEv
0015c3d2 T _ZN9SentryGun6updateEi
```

The gate `.so` is byte-identical before and after:

```sh
sha256sum /tmp/w3745/sentry/gate_base.so /tmp/w3745/sentry/gate_after.so
# b79f01852a7ed886af6def6e17b54a3f5424d01bd491750980f62fbc67bd9745  (both)
cmp -l /tmp/w3745/sentry/gate_base.so /tmp/w3745/sentry/gate_after.so | wc -l
# 0
```

Plain no-order section layout is unchanged by the pass:
`.text 0x1337e8 @ 0x69440`, `.ARM.exidx 0x6f70 @ 0x19cc28`,
`.ARM.extab 0x2628c @ 0x1a3b98`, `.rodata 0x41b67 @ 0x1c9e30`.

### 20.4 Next frontier

The first plain no-order mismatch is now:

```text
original 0x15e0e4 NewsTicker::update(int)   (0x90 bytes)
ours     0x15e1e4 NewsTicker::update(int)
```

The original `NewsTicker` order is `ctor`, `replaceTokens`, `D1/D2`, `update`,
`draw`, `getHeight`, `OnTouchBegin`, `OnTouchMove`, `OnTouchEnd`
(`0x15c468`, `0x15c858`, `0x15e0d2`, `0x15e0e4`, `0x15e174`, `0x15e2b4`,
`0x15e2d8`, `0x15e328`, `0x15e374`). Our
`src/game/world/NewsTicker.cpp.obj` emits `draw` before `update`:

```text
NewsTicker::~NewsTicker()
NewsTicker::draw()
NewsTicker::update(int)
NewsTicker::getHeight()
...
```

so the plain link puts `draw` at `0x15e0e4` and `update` at `0x15e224`.
Moving the `update` definition before `draw` is the obvious next per-TU
source-order experiment; it is a separate TU and was not tested or landed in
this pass.

### 20.5 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements (before and after the source reorder)
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/sentry/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/sentry/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/sentry/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/sentry/base_noorder.so

cmake --build cmake-build-match --target check -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/sentry/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/sentry/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/sentry/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/sentry/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/weapons/SentryGun.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN9SentryGun'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/sentry/gate_after.so
git stash push -- src/game/weapons/SentryGun.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/sentry/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745/sentry/gate_base.so /tmp/w3745/sentry/gate_after.so
cmp -l /tmp/w3745/sentry/gate_base.so /tmp/w3745/sentry/gate_after.so | wc -l   # 0
```

### 20.6 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `2954 / 4497`, prefix 3615,
  3713 / 4524, first mismatch `SentryGun::render` `0x15c3d0` vs `0x15c464`.
* The original emits `SentryGun` in the `.dynsym` address order
  `ctor, D1/D2, D0, render, update`; the `gof2hd_1.1.19_ios` source-layout
  dump independently witnesses `ctor, ~SentryGun, ~SentryGun, render, update`.
* The baseline object emitted `update` before `render`; moving the `render`
  definition before `update` makes the object's `.text.*` order match the
  original address order. The section bodies are unchanged (2 and 0x94 bytes).
* The landed move is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0,
  every ELF section size/base unchanged) and the gate `.so` is byte-identical
  to the baseline (same SHA-256, `cmp` status 0).
* The plain no-order build now has all seven `SentryGun` dynsym rows at their
  original addresses: exact addresses 3713 -> 3715, exact-address prefix
  3615 -> 3625. Plain `byte_exact` is unchanged at 2954 because both functions
  were already byte-exact at the wrong addresses.
* The next plain no-order frontier is `NewsTicker::update`
  (`0x15e0e4` original, `0x15e1e4` ours); the original `NewsTicker` order puts
  `update` before `draw`, while our object emits `draw` before `update`.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `2954`, not `4497`.
* The `NewsTicker` reorder is the obvious next per-TU source-order experiment,
  but it is a separate TU and was not tested or landed in this pass.

## 21. `NewsTicker::update` emission order (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0. The gate `.so` is **not** byte-identical to the baseline: it differs
in 32 bytes (20 `.note.gnu.build-id`, 2 `.ARM.exidx`, 10 `.ARM.extab`); every
other section, including `.text`, `.rodata`, `.dynsym`, `.dynstr`, `.plt` and
`.rel.plt`, is byte-identical. The EH-table change is the expected consequence
of correcting the source order: the original's `.ARM.extab` records are emitted
in function order, and the baseline had `draw`'s record before `update`'s.
The plain no-order link improves from **byte 2954 / 3715 of 4524 exact
addresses / prefix 3625** to **byte 2955 / 3717 of 4524 / prefix 3634**; the
first plain no-order mismatch moves from `NewsTicker::update` to
`Sparks::~Sparks`.

This is the next frontier recorded in §20.4 (the `SentryGun` pass).

### 21.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`) at branch `wave87-w3745`, HEAD `647b324d`:

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (unchanged by this pass):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
```

The original's `NewsTicker` block from the binary's own `.dynsym`
(`nm -nSD --defined-only`):

```text
0x15c468  0x3cc  NewsTicker::NewsTicker(int, int, int, int, int)   # C1/C2 alias
0x15c858  0x1814 NewsTicker::replaceTokens(AbyssEngine::String)
0x15e0d2  0x10   NewsTicker::~NewsTicker()                         # D1/D2 alias
0x15e0e4  0x90   NewsTicker::update(int)
0x15e174  0x140  NewsTicker::draw()
0x15e2b4  0x24   NewsTicker::getHeight()
0x15e2d8  0x50   NewsTicker::OnTouchBegin(int, int)
0x15e328  0x4c   NewsTicker::OnTouchMove(int, int)
0x15e374  0x16   NewsTicker::OnTouchEnd(int, int)
```

The HD source-layout dump independently witnesses `update` before `draw`
(`_work/original_layout/original_layout.json` ->
`gof2hd_1.1.19_ios` -> `Classes/Game/NewsTicker.cpp`):

```text
_NewsTicker::NewsTicker(int, int, int, int, int)
_NewsTicker::replaceTokens(AbyssEngine::String)
_void ArrayReleaseClasses<NewsItem*>(Array<NewsItem*>&)
_NewsTicker::NewsTicker(int, int, int, int, int)
_NewsTicker::~NewsTicker()
_NewsTicker::update(int)
_NewsTicker::draw()
_NewsTicker::OnTouchBegin(int, int)
_NewsTicker::OnTouchMove(int, int)
_NewsTicker::OnTouchEnd(int, int)
```

(The dump's duplicate ctor and `ArrayReleaseClasses` are weak/template
emissions; the binary `.dynsym` is the authority for the strong-function
order and also places `getHeight` between `draw` and `OnTouchBegin`.)

The baseline `NewsTicker.cpp.obj` emitted `draw` before `update`:

```text
NewsTicker::NewsTicker(int, int, int, int, int)
Array<NewsItem*>::Array()
void ArrayAdd<NewsItem*>(NewsItem*, Array<NewsItem*>&)
NewsTicker::replaceTokens(AbyssEngine::String)
void ArrayReleaseClasses<NewsItem*>(Array<NewsItem*>&)
Array<NewsItem*>::~Array()
AbyssEngine::String::String(int)
Array<Item*>::Array()
void ArrayAdd<Item*>(Item*, Array<Item*>&)
Array<Item*>::~Array()
NewsTicker::~NewsTicker()
NewsTicker::draw()
NewsTicker::update(int)
NewsTicker::getHeight()
NewsTicker::OnTouchBegin(int, int)
NewsTicker::OnTouchMove(int, int)
NewsTicker::OnTouchEnd(int, int)
```

That made the plain no-order link put `draw` at `0x15e0e4` and `update` at
`0x15e224`. The first plain no-order mismatch was exactly:

```text
original 0x15e0e4 NewsTicker::update(int)   (0x90 bytes)
ours     0x15e1e4 NewsTicker::update(int)
```

### 21.2 Landed source change

Only change: move the `NewsTicker::update()` definition in
`src/game/world/NewsTicker.cpp` from after `NewsTicker::draw()` to before it.
No body was edited.

```diff
@@ after NewsTicker::~NewsTicker()
+void NewsTicker::update(int dt) {
+    ...
+}
 void NewsTicker::draw() {
     ...
 }
-void NewsTicker::update(int dt) {
-    ...
-}
```

The resulting object order is exactly the original strong-function order:

```text
NewsTicker::NewsTicker(int, int, int, int, int)
...
NewsTicker::~NewsTicker()
NewsTicker::update(int)
NewsTicker::draw()
NewsTicker::getHeight()
NewsTicker::OnTouchBegin(int, int)
NewsTicker::OnTouchMove(int, int)
NewsTicker::OnTouchEnd(int, int)
```

The `.text.*` section bodies are unchanged (`update` 0x90 bytes, `draw`
0x140 bytes); only their order changed.

### 21.3 Measured before/after

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `b79f0185…` | **2954** | 3715 / 4524 | 3625 | `NewsTicker::update` `0x15e0e4` vs `0x15e1e4` |
| landed | **4497 / 4497** | `418fc2d5…` | **2955** | **3717 / 4524** | **3634** | `Sparks::~Sparks` `0x15e474` vs `0x15e708` |

The landed delta is **+1 plain byte-exact** (`NewsTicker::update` itself),
**+2 functions at their original address** (`update` and `draw`), and
**+9 exact-address prefix**, with the gate counts unchanged.

The plain `report.json` changes exactly one row:

```text
_ZN10NewsTicker6updateEi  byte False -> True  (pct 100.0, linked True, 144 B)
```

`update` was `pct == 100.0` but not byte-exact before the move because its
`blx GameText::getLanguage@plt` immediate is PC-relative: the baseline body at
`0x15e224` encoded `f701 e97a`, the original at `0x15e0e4` encodes
`f701 ea1a`. At the original address the encoding matches. The gate
`report.json` is byte-identical before/after (all 4524 rows unchanged).

The plain no-order link now has the whole `NewsTicker` block at its original
address:

```text
$NM -nD --defined-only /tmp/w3745/newsticker/after_noorder.so | grep '_ZN10NewsTicker'
0015c468 T _ZN10NewsTickerC1Eiiiii
0015c468 T _ZN10NewsTickerC2Eiiiii
0015c858 T _ZN10NewsTicker13replaceTokensEN11AbyssEngine6StringE
0015e0d2 T _ZN10NewsTickerD1Ev
0015e0d2 T _ZN10NewsTickerD2Ev
0015e0e4 T _ZN10NewsTicker6updateEi
0015e174 T _ZN10NewsTicker4drawEv
0015e2b4 T _ZN10NewsTicker9getHeightEv
0015e2d8 T _ZN10NewsTicker12OnTouchBeginEii
0015e328 T _ZN10NewsTicker11OnTouchMoveEii
0015e374 T _ZN10NewsTicker10OnTouchEndEii
```

Plain no-order section layout is unchanged by the pass:
`.text 0x1337e8 @ 0x69440`, `.ARM.exidx 0x6f70 @ 0x19cc28`,
`.ARM.extab 0x2628c @ 0x1a3b98`, `.rodata 0x41b67 @ 0x1c9e30`.

### 21.4 Why the gate `.so` changes (and why that is correct)

The section-ordering file orders `.text.*` only; `.ARM.exidx` and
`.ARM.extab` follow the object's section order. The original emits the
`update` LSDA record before the `draw` record; the baseline emitted them in
the opposite order. Correcting the source order therefore necessarily
reorders those two records. The gate `.so` differs from the baseline in
exactly 32 bytes:

| section | differing bytes | cause |
|---|---:|---|
| `.note.gnu.build-id` | 20 | gold hashes the changed EH content |
| `.ARM.exidx` | 2 | the two `w1` prel31 extab pointers for `update`/`draw` swap |
| `.ARM.extab` | 10 | the two 12-byte LSDA records swap positions |

`.text`, `.rodata`, `.dynsym`, `.dynstr`, `.plt` and `.rel.plt` are
byte-identical (0 differing bytes each). The `.ARM.exidx`/`.ARM.extab`
section sizes and bases are unchanged.

The record contents are identical in all three binaries; only their order
changes. For the `NewsTicker` block the original sequence is
`X, Y, Y, A(update), B(draw), Y, B`; the baseline was
`X, Y, Y, B(draw), A(update), Y, B`; the landed tree is
`X, Y, Y, A(update), B(draw), Y, B`, i.e. exactly the original sequence.
The landed block is uniformly `-0x18` in `.ARM.extab` offsets from the
original (`0x1c4ef4` vs `0x1c4f0c` for the first record), which is the
pre-existing cumulative record-size delta from preceding functions, not a
regression from this pass.

### 21.5 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order baseline
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/newsticker/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/newsticker/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/newsticker/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/newsticker/base_noorder.so

# source reorder: move NewsTicker::update() before NewsTicker::draw()
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order after
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/newsticker/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/newsticker/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/newsticker/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/newsticker/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/NewsTicker.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN10NewsTicker'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/newsticker/gate_after.so
git stash push -- src/game/world/NewsTicker.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/newsticker/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745/newsticker/gate_base.so /tmp/w3745/newsticker/gate_after.so
cmp -l /tmp/w3745/newsticker/gate_base.so /tmp/w3745/newsticker/gate_after.so | wc -l   # 32
```

### 21.6 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `2954 / 4497`, prefix 3625,
  3715 / 4524, first mismatch `NewsTicker::update` `0x15e0e4` vs `0x15e1e4`.
* The original emits `NewsTicker` in the `.dynsym` address order
  `ctor, replaceTokens, D1/D2, update, draw, getHeight, OnTouchBegin,
  OnTouchMove, OnTouchEnd`; the `gof2hd_1.1.19_ios` source-layout dump
  independently witnesses `update` before `draw`.
* The baseline object emitted `draw` before `update`; moving the `update`
  definition before `draw` makes the object's `.text.*` order match the
  original address order. The section bodies are unchanged (0x90 and 0x140
  bytes).
* The landed move is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0,
  every ELF section size/base unchanged) and the gate `report.json` is
  byte-identical (all 4524 rows unchanged). The gate `.so` differs only in
  the build-id and the two swapped EH records (32 bytes total), which is the
  original's own record order.
* The plain no-order build now has all eleven `NewsTicker` dynsym rows at
  their original addresses: exact addresses 3715 -> 3717, exact-address
  prefix 3625 -> 3634, plain `byte_exact` 2954 -> 2955
  (`NewsTicker::update` flips to byte-exact because its PC-relative PLT
  branch immediate now matches).
* The next plain no-order frontier is `Sparks::~Sparks`
  (`0x15e474` original, `0x15e708` ours); the original `Sparks` order is
  `ctor, isRocket, D1/D2, explode, explode(Vector), render, update,
  translate`, while our object emits `D1/D2` after `translate`.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `2955`, not `4497`.
* The `-0x18` `.ARM.extab` block offset is a pre-existing cumulative
  record-size delta from preceding functions (the known seven-record
  backlog), not caused by this pass.
* The `Sparks` destructor reorder is the obvious next per-TU source-order
  experiment, but it is a separate TU and was not tested or landed in this
  pass.

## 22. `Sparks` destructor emission order (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0. The gate `report.json` is **byte-identical** (0 changed rows); the
gate `.so` differs from the baseline in 51 bytes (20 `.note.gnu.build-id`,
5 `.ARM.exidx`, 26 `.ARM.extab`), all from the destructor's EH-record move.
Every other section, including `.text`, `.rodata`, `.dynsym`, `.dynstr`,
`.plt` and `.rel.plt`, is byte-identical. The plain no-order link improves
from **byte 2955 / 3717 of 4524 exact addresses / prefix 3634** to
**byte 2961 / 3724 of 4524 / prefix 3647**; the first plain no-order mismatch
moves from `Sparks::~Sparks` to `PlayerJunk::update`.

This is the next frontier recorded in §21.6.

### 22.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`) at branch `wave87-w3745`, HEAD `95be20ac`:

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (unchanged by this pass):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78
.rodata          0x041b67 @ 0x1c9e10
.data            0x00296c @ 0x214000
```

The original's `Sparks` block from the binary's own `.dynsym`
(`nm -nSD --defined-only`):

```text
0x15e38c  0xdc  Sparks::Sparks(int)                    # C1/C2 alias
0x15e468  0x0c  Sparks::isRocket()
0x15e474  0x16  Sparks::~Sparks()                      # D1/D2 alias
0x15e48c  0x108 Sparks::explode(int, int, int)
0x15e594  0x28  Sparks::explode(AEMath::Vector const&)
0x15e5c0  0xa0  Sparks::render()
0x15e660  0x74  Sparks::update(int)
0x15e6d4  0x4c  Sparks::translate(AEMath::Vector const&)
```

The baseline `Sparks.cpp.obj` emitted the destructor last:

```text
Sparks::Sparks(int)
Sparks::isRocket()
Sparks::explode(int, int, int)
Sparks::explode(AbyssEngine::AEMath::Vector const&)
Sparks::render()
Sparks::update(int)
Sparks::translate(AbyssEngine::AEMath::Vector const&)
Sparks::~Sparks()
```

That made the plain no-order link put `~Sparks` at `0x15e708` instead of
`0x15e474`; the first plain no-order mismatch was exactly:

```text
original 0x15e474 Sparks::~Sparks()   (0x16 bytes)
ours     0x15e708 Sparks::~Sparks()
```

### 22.2 Landed source change

Only change: move the `Sparks::~Sparks()` definition in
`src/engine/render/Sparks.cpp` from the end of the file to immediately after
`Sparks::isRocket()`. No body was edited.

```diff
@@ after Sparks::isRocket()
+Sparks::~Sparks() {
+    delete[] this->lifetimeThresholds;
+    this->lifetimeThresholds = nullptr;
+}
+
 void Sparks::explode(int x, int y, int z) {
@@ end of file
-Sparks::~Sparks() {
-    delete[] this->lifetimeThresholds;
-    this->lifetimeThresholds = nullptr;
-}
```

The resulting object order is exactly the original address order:

```text
Sparks::Sparks(int)
Sparks::isRocket()
Sparks::~Sparks()
Sparks::explode(int, int, int)
Sparks::explode(AbyssEngine::AEMath::Vector const&)
Sparks::render()
Sparks::update(int)
Sparks::translate(AbyssEngine::AEMath::Vector const&)
```

The `.text.*` section bodies are unchanged (destructor 0x16 bytes, `explode`
0x108, `render` 0xa0, `update` 0x74, `translate` 0x4c); only their order
changed. The whole Sparks block now lands at the original addresses in the
plain no-order link:

```text
$NM -nD --defined-only /tmp/w3745/sparks/after_noorder.so | grep '_ZN6Sparks'
0015e38c T _ZN6SparksC1Ei
0015e38c T _ZN6SparksC2Ei
0015e468 T _ZN6Sparks8isRocketEv
0015e474 T _ZN6SparksD1Ev
0015e474 T _ZN6SparksD2Ev
0015e48c T _ZN6Sparks7explodeEiii
0015e594 T _ZN6Sparks7explodeERKN11AbyssEngine6AEMath6VectorE
0015e5c0 T _ZN6Sparks6renderEv
0015e660 T _ZN6Sparks6updateEi
0015e6d4 T _ZN6Sparks9translateERKN11AbyssEngine6AEMath6VectorE
```

### 22.3 Measured before/after

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `527d4920…` | **2955** | 3717 / 4524 | 3634 | `Sparks::~Sparks` `0x15e474` vs `0x15e708` |
| landed | **4497 / 4497** | `5ebe1e38…` | **2961** | **3724 / 4524** | **3647** | `PlayerJunk::update` `0x15e798` vs `0x15e7c4` |

The landed delta is **+6 plain byte-exact** (`render`, `update`,
`explode(int,int,int)`, `translate`, `D1`, `D2`), **+7 functions at their
original address** (those six plus `explode(Vector)`), and **+13
exact-address prefix**, with the gate counts unchanged. The plain
`report.json` changes exactly six rows, all `byte False -> True`:

```text
_ZN6Sparks6renderEv
_ZN6Sparks6updateEi
_ZN6Sparks7explodeEiii
_ZN6Sparks9translateERKN11AbyssEngine6AEMath6VectorE
_ZN6SparksD1Ev
_ZN6SparksD2Ev
```

`Sparks::explode(Vector)` is at its original address but is still not
byte-exact in the plain no-order link: it tail-calls
`Sparks::explode(int,int,int)` through the PLT, and the Thumb→ARM
interworking veneer it branches to is at `0x19c5f8` in the original but
`0x19c618` in the plain link. The gate link has it byte-exact. This is a
link/veneer-placement artifact, not a source-order issue.

### 22.4 Why the gate `.so` changes (and why that is correct)

The section-ordering file orders `.text.*` only; `.ARM.exidx` and
`.ARM.extab` follow the object's section order. The original emits the
`~Sparks` LSDA record immediately after `isRocket` and before `explode(int)`;
the baseline emitted it after `translate`. Correcting the source order
therefore necessarily reorders those records. The gate `.so` differs from
the baseline in exactly 51 bytes:

| section | differing bytes | cause |
|---|---:|---|
| `.note.gnu.build-id` | 20 | gold hashes the changed EH content |
| `.ARM.exidx` | 5 | the `~Sparks`/`explode(int)` extab pointers move |
| `.ARM.extab` | 26 | the two LSDA records move to their original relative order |

`.text`, `.rodata`, `.dynsym`, `.dynstr`, `.plt` and `.rel.plt` are
byte-identical (0 differing bytes each). The `.ARM.exidx`/`.ARM.extab`
section sizes and bases are unchanged.

`readelf --unwind` shows the original relative order
`ctor, (isRocket inline), ~Sparks, explode(int), (explode(Vector) inline),
render, update, translate`; the landed tree reproduces exactly that sequence.
The absolute `.ARM.extab` offsets are uniformly `-0x18` from the original
(`0x1c4f3c` vs `0x1c4f54` for the ctor record), which is the pre-existing
cumulative record-size delta from preceding functions, not caused by this
pass.

### 22.5 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order baseline
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/sparks/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/sparks/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/sparks/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/sparks/base_noorder.so

# source reorder: move Sparks::~Sparks() before Sparks::explode(int,int,int)
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order after
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/sparks/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/sparks/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/sparks/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/sparks/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/engine/render/Sparks.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN6Sparks'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/sparks/gate_after.so
git stash push -- src/engine/render/Sparks.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/sparks/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745/sparks/gate_base.so /tmp/w3745/sparks/gate_after.so
cmp -l /tmp/w3745/sparks/gate_base.so /tmp/w3745/sparks/gate_after.so | wc -l   # 51
```

### 22.6 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `2955 / 4497`, prefix 3634,
  3717 / 4524, first mismatch `Sparks::~Sparks` `0x15e474` vs `0x15e708`.
* The original emits `Sparks` in the `.dynsym` address order
  `ctor, isRocket, D1/D2, explode(int), explode(Vector), render, update,
  translate`; the baseline object emitted `~Sparks` last.
* Moving the `~Sparks` definition immediately after `Sparks::isRocket()`
  makes the object's `.text.*` order match the original address order. The
  section bodies are unchanged.
* The landed move is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0,
  every ELF section size/base unchanged) and the gate `report.json` is
  byte-identical (all 4524 rows unchanged). The gate `.so` differs only in
  the build-id and the moved EH records (51 bytes total), which is the
  original's own record order.
* The plain no-order build now has all ten `Sparks` dynsym rows at their
  original addresses: exact addresses 3717 -> 3724, exact-address prefix
  3634 -> 3647, plain `byte_exact` 2955 -> 2961.
* The next plain no-order frontier is `PlayerJunk::update`
  (`0x15e798` original, `0x15e7c4` ours). The original `PlayerJunk` order is
  `ctor, D1/D2, D0, reset, update, render, collide, outerCollide`; our object
  emits `reset, collide, outerCollide, render, update`.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `2961`, not `4497`.
* `Sparks::explode(Vector)` is blocked in the plain link only by the
  Thumb→ARM veneer address (`0x19c5f8` original vs `0x19c618` plain); it is
  byte-exact in the gate link. The veneer placement rule is not characterized
  here.
* The `PlayerJunk` reorder is the obvious next per-TU source-order
  experiment, but it is a separate TU and was not tested or landed in this
  pass.

## 23. `PlayerJunk` emission order (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4497 / linked 4497**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0. The gate `report.json` is **byte-identical** (0 changed rows); the
gate `.so` differs from the baseline in 60 bytes (20 `.note.gnu.build-id`,
3 `.ARM.exidx`, 37 `.ARM.extab`), all from the moved EH records. Every other
section, including `.text`, `.rodata`, `.dynsym`, `.dynstr`, `.plt` and
`.rel.plt`, is byte-identical. The plain no-order link improves from
**byte 2961 / 3724 of 4524 exact addresses / prefix 3647** to
**byte 2962 / 3728 of 4524 / prefix 3666**; the first plain no-order mismatch
moves from `PlayerJunk::update` to `MarqueeImage::setPosition`.

This is the next frontier recorded in §22.6.

### 23.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`) at branch `wave87-w3745`, HEAD `d38831b6`:

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (unchanged by this pass):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78
.rodata          0x041b67 @ 0x1c9e10
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

The original's `PlayerJunk` block from the binary's own `.dynsym`
(`nm -nSD --defined-only`):

```text
0x15e720  0x48  PlayerJunk::PlayerJunk(int, Player*, AEGeometry*, float, float, float)  # C1/C2 alias
0x15e768  0x04  PlayerJunk::~PlayerJunk()                                              # D1/D2 alias
0x15e76c  0x10  PlayerJunk::~PlayerJunk()                                              # D0
0x15e77c  0x1c  PlayerJunk::reset()
0x15e798  0x134 PlayerJunk::update(int)
0x15e8cc  0x24  PlayerJunk::render()
0x15e8f0  0x04  PlayerJunk::collide(float, float, float)
0x15e8f4  0x04  PlayerJunk::outerCollide(float, float, float)
```

The `gof2hd_1.1.19_ios` source-layout dump (`_work/original_layout/original_layout.json`,
`tu_symbols.gof2hd_1.1.19_ios["Classes/Game/PlayerJunk.cpp"]`) independently
witnesses the same relative order for the four non-trivial functions:
`~PlayerJunk`, `update`, `render`, `collide`, `outerCollide` (the dump does not
list `reset`, which is present in the Android binary between `D0` and `update`).

The baseline `PlayerJunk.cpp.obj` emitted the definitions in this order:

```text
PlayerJunk::PlayerJunk(int, Player*, AEGeometry*, float, float, float)
PlayerJunk::~PlayerJunk()          # D1/D2
PlayerJunk::reset()
PlayerJunk::collide(float, float, float)
PlayerJunk::outerCollide(float, float, float)
PlayerJunk::render()
PlayerJunk::update(int)
```

That made the plain no-order link put `update` at `0x15e7c4` instead of
`0x15e798`; the first plain no-order mismatch was exactly:

```text
original 0x15e798 PlayerJunk::update(int)   (0x134 bytes)
ours     0x15e7c4 PlayerJunk::update(int)
```

### 23.2 Landed source change

Only change: move the `PlayerJunk::update()`, `PlayerJunk::render()`,
`PlayerJunk::collide()` and `PlayerJunk::outerCollide()` definitions in
`src/game/ship/PlayerJunk.cpp` so the definition order is
`ctor, dtor, reset, update, render, collide, outerCollide`. No body was edited.

```diff
@@ after PlayerJunk::reset()
-int PlayerJunk::collide(...) { return 0; }
-int PlayerJunk::outerCollide(...) { return 0; }
-void PlayerJunk::render() { ... }
 void PlayerJunk::update(int elapsed) { ... }
+
+void PlayerJunk::render() { ... }
+
+int PlayerJunk::collide(...) { return 0; }
+
+int PlayerJunk::outerCollide(...) { return 0; }
```

The resulting object order is exactly the original address order:

```text
PlayerJunk::PlayerJunk(int, Player*, AEGeometry*, float, float, float)
PlayerJunk::~PlayerJunk()
PlayerJunk::reset()
PlayerJunk::update(int)
PlayerJunk::render()
PlayerJunk::collide(float, float, float)
PlayerJunk::outerCollide(float, float, float)
```

The `.text.*` section bodies are unchanged (ctor 0x48, D1/D2 0x4, D0 0x10,
`reset` 0x1c, `update` 0x134, `render` 0x24, `collide`/`outerCollide` 0x4);
only their order changed. The whole `PlayerJunk` block now lands at the
original addresses in the plain no-order link:

```text
$NM -nD --defined-only /tmp/w3745/playerjunk/after_noorder.so | grep '_ZN10PlayerJunk'
0015e720 T _ZN10PlayerJunkC1EiP6PlayerP10AEGeometryfff
0015e720 T _ZN10PlayerJunkC2EiP6PlayerP10AEGeometryfff
0015e768 T _ZN10PlayerJunkD1Ev
0015e768 T _ZN10PlayerJunkD2Ev
0015e76c T _ZN10PlayerJunkD0Ev
0015e77c T _ZN10PlayerJunk5resetEv
0015e798 T _ZN10PlayerJunk6updateEi
0015e8cc T _ZN10PlayerJunk6renderEv
0015e8f0 T _ZN10PlayerJunk7collideEfff
0015e8f4 T _ZN10PlayerJunk12outerCollideEfff
```

### 23.3 Measured before/after

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `5ebe1e38…` | **2961** | 3724 / 4524 | 3647 | `PlayerJunk::update` `0x15e798` vs `0x15e7c4` |
| landed | **4497 / 4497** | `3ee988db…` | **2962** | **3728 / 4524** | **3666** | `MarqueeImage::setPosition` `0x15f5f2` vs `0x15f5f6` |

The landed delta is **+1 plain byte-exact** (`PlayerJunk::update` itself),
**+4 functions at their original address** (`update`, `render`, `collide`,
`outerCollide`), and **+19 exact-address prefix**, with the gate counts
unchanged.

The plain `report.json` changes exactly one row:

```text
_ZN10PlayerJunk6updateEi  byte False -> True  (pct 100.0, linked True, 0x134 B)
```

`update` was `pct == 100.0` but not byte-exact before the move because its
`blx` PLT immediate is PC-relative: the baseline body at `0x15e7c4` encoded
a different branch displacement from the original at `0x15e798`. At the
original address the encoding matches. `render`, `collide` and
`outerCollide` move to their original addresses but remain non-byte in the
plain link (`reset`, `D0`, `D1`, `D2` likewise): the plain link's `.dynsym`
still differs by 7 bytes, so their PLT slots are not the gate's slots. They
are byte-exact in the gate link.

The plain no-order section layout is unchanged by the pass:
`.text 0x1337e8 @ 0x69440`, `.ARM.exidx 0x6f70 @ 0x19cc28`,
`.ARM.extab 0x2628c @ 0x1a3b98`, `.rodata 0x41b67 @ 0x1c9e30`.

### 23.4 Why the gate `.so` changes (and why that is correct)

The section-ordering file orders `.text.*` only; `.ARM.exidx` and
`.ARM.extab` follow the object's section order. The original emits the
`update` LSDA record before the `render` record; the baseline emitted them
in the opposite order. Correcting the source order therefore necessarily
reorders those two records. The gate `.so` differs from the baseline in
exactly 60 bytes:

| section | differing bytes | cause |
|---|---:|---|
| `.note.gnu.build-id` | 20 | gold hashes the changed EH content |
| `.ARM.exidx` | 3 | the `update`/`render` extab pointers move |
| `.ARM.extab` | 37 | the two LSDA records swap to their original relative order |

`.text`, `.rodata`, `.dynsym`, `.dynstr`, `.plt` and `.rel.plt` are
byte-identical (0 differing bytes each). The `.ARM.exidx`/`.ARM.extab`
section sizes and bases are unchanged.

`readelf --unwind` shows the original relative order
`ctor, D1/D2, D0, reset, update, render, collide`; the landed tree reproduces
exactly that sequence. The absolute `.ARM.extab` offsets are uniformly
`-0x18` from the original (`0x1c4f88` vs `0x1c4fa0` for the `reset` record,
`0x1c4f94` vs `0x1c4fac` for `update`, `0x1c4fb4` vs `0x1c4fcc` for
`render`), which is the pre-existing cumulative record-size delta from
preceding functions, not caused by this pass. The baseline was non-uniform
(`reset` `-0x18`, `update` `-0xc`, `render` `-0x38`).

### 23.5 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order baseline
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/playerjunk/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/playerjunk/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/playerjunk/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/playerjunk/base_noorder.so

# source reorder: move update/render before collide/outerCollide
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order after
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/playerjunk/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/playerjunk/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/playerjunk/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/playerjunk/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ship/PlayerJunk.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN10PlayerJunk'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/playerjunk/gate_after.so
git stash push -- src/game/ship/PlayerJunk.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/playerjunk/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745/playerjunk/gate_base.so /tmp/w3745/playerjunk/gate_after.so
cmp -l /tmp/w3745/playerjunk/gate_base.so /tmp/w3745/playerjunk/gate_after.so | wc -l   # 60
```

### 23.6 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `2961 / 4497`, prefix 3647,
  3724 / 4524, first mismatch `PlayerJunk::update` `0x15e798` vs `0x15e7c4`.
* The original emits `PlayerJunk` in the `.dynsym` address order
  `ctor, D1/D2, D0, reset, update, render, collide, outerCollide`; the
  `gof2hd_1.1.19_ios` source-layout dump independently witnesses
  `update, render, collide, outerCollide` in that order.
* The baseline object emitted `collide, outerCollide, render, update`;
  moving the definitions to `update, render, collide, outerCollide` makes
  the object's `.text.*` order match the original address order. The section
  bodies are unchanged.
* The landed move is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0,
  every ELF section size/base unchanged) and the gate `report.json` is
  byte-identical (all 4524 rows unchanged). The gate `.so` differs only in
  the build-id and the moved EH records (60 bytes total), which is the
  original's own record order.
* The plain no-order build now has all ten `PlayerJunk` dynsym rows at their
  original addresses: exact addresses 3724 -> 3728, exact-address prefix
  3647 -> 3666, plain `byte_exact` 2961 -> 2962.
* The next plain no-order frontier is `MarqueeImage::setPosition`
  (`0x15f5f2` original, `0x15f5f6` ours). The original `MarqueeImage` order
  is `ctor, D1/D2, setPosition, setSpeed, update, draw(int,int), draw()`;
  our object emits `setSpeed` before `setPosition` and `draw()` before
  `draw(int,int)`.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `2962`, not `4497`.
* `render`, `collide`, `outerCollide`, `reset`, `D0`, `D1` and `D2` are at
  their original addresses after this pass but are not byte-exact in the
  plain link; the plain `.dynsym` still differs by 7 bytes, so their PLT
  slots differ. They are byte-exact in the gate link. The exact 7-byte
  `.dynsym` difference was not investigated.
* The `MarqueeImage` reorder is the obvious next per-TU source-order
  experiment, but it is a separate TU and was not tested or landed in this
  pass.

## 24. `MarqueeImage` emission order (2026-11-03)

Status: landed in this pass. The section-ordering-file gate stays at
**byte 4498 / linked 4498**, `.text` `0x1337c8 @ 0x69440`, lint `CLEAN ✓`,
drift 0. The gate `.so` is **byte-identical** before and after
(sha256 `e23253eb79646e94d5c4768ad1d8ded49937e74daa20c2244e324790324433e3`,
`cmp -l` = 0 bytes). The plain no-order link improves from
**byte 2962 / 3728 of 4524 exact addresses / prefix 3666** to
**byte 2963 / 3733 of 4524 / prefix 3685**; the first plain no-order mismatch
moves from `MarqueeImage::setPosition` to `SpacePoint::isFree`.

This is the next frontier recorded in §23.6.

### 24.1 Baseline and target

Baseline (`cmake --build cmake-build-match --target check -j8`, then `lint`,
`drift`) at branch `wave87-w3745`, HEAD `52e03527`:

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (unchanged by this pass):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78
.rodata          0x041b67 @ 0x1c9e10
.data            0x00296c @ 0x214000
```

The original's `MarqueeImage` block from the binary's own `.dynsym`
(`nm -nSD --defined-only`):

```text
0x15f59c  0x54  MarqueeImage::MarqueeImage(unsigned short, int, int, int, float)  # C1/C2 alias
0x15f5f0  0x02  MarqueeImage::~MarqueeImage()                                    # D1/D2 alias
0x15f5f2  0x06  MarqueeImage::setPosition(int, int)
0x15f5f8  0x04  MarqueeImage::setSpeed(float)
0x15f5fc  0x58  MarqueeImage::update(int)
0x15f654  0xa0  MarqueeImage::draw(int, int)
0x15f6f4  0x08  MarqueeImage::draw()
```

The `gof2hd_1.1.19_ios` source-layout dump
(`_work/original_layout/original_layout.json`,
`tu_symbols.gof2hd_1.1.19_ios["Classes/Game/MarqueeImage.cpp"]`) independently
witnesses the relative order `setSpeed, update, draw(int, int), draw()`; the
dump does not list `setPosition` or the destructor, which are present in the
Android binary between the ctor and `setSpeed`.

The baseline `MarqueeImage.cpp.obj` emitted the definitions in this order:

```text
MarqueeImage::MarqueeImage(unsigned short, int, int, int, float)
MarqueeImage::~MarqueeImage()
MarqueeImage::setSpeed(float)
MarqueeImage::setPosition(int, int)
MarqueeImage::draw()
MarqueeImage::draw(int, int)
MarqueeImage::update(int)
```

That made the plain no-order link put `setPosition` at `0x15f5f6` instead of
`0x15f5f2`; the first plain no-order mismatch was exactly:

```text
original 0x15f5f2 MarqueeImage::setPosition(int, int)
ours     0x15f5f6 MarqueeImage::setPosition(int, int)
```

### 24.2 Landed source change

Only change: move the `MarqueeImage` definitions in
`src/engine/render/MarqueeImage.cpp` so the definition order is
`ctor, dtor, setPosition, setSpeed, update, draw(int, int), draw()`. No body
was edited.

```diff
@@ after MarqueeImage::~MarqueeImage()
-void MarqueeImage::setSpeed(float speed) {
-    this->speed = speed;
-}
-
 void MarqueeImage::setPosition(int x, int y) {
     this->x = x;
     this->y = y;
 }
 
-void MarqueeImage::draw() {
-    return draw(this->x, this->y);
+void MarqueeImage::setSpeed(float speed) {
+    this->speed = speed;
 }
 
 void MarqueeImage::update(int dt) {
@@ ... (end of update)
+void MarqueeImage::draw() {
+    return draw(this->x, this->y);
+}
```

The resulting object order is exactly the original address order. The
`.text.*` section sizes are unchanged and equal the original's:

```text
MarqueeImage::MarqueeImage(unsigned short, int, int, int, float)  0x54
MarqueeImage::~MarqueeImage()                                     0x02
MarqueeImage::setPosition(int, int)                               0x06
MarqueeImage::setSpeed(float)                                     0x04
MarqueeImage::update(int)                                         0x58
MarqueeImage::draw(int, int)                                      0xa0
MarqueeImage::draw()                                              0x08
```

`draw()` stays 8 bytes: moving `draw(int, int)` before it does not cause
same-TU inlining at `-Oz`.

### 24.3 Measured before/after

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4498 / 4498** | `e23253eb…` | **2962** | 3728 / 4524 | 3666 | `MarqueeImage::setPosition` `0x15f5f2` vs `0x15f5f6` |
| landed | **4498 / 4498** | `e23253eb…` | **2963** | **3733 / 4524** | **3685** | `SpacePoint::isFree` `0x1603e6` vs `0x1603f4` |

The landed delta is **+1 plain byte-exact** (`MarqueeImage::draw(int, int)`),
**+5 functions at their original address** (`setPosition`, `setSpeed`,
`update`, `draw(int, int)`, `draw()`), and **+19 exact-address prefix**, with
the gate counts unchanged. The gate `.so` is byte-identical, so the gate
`report.json` is unchanged.

The plain `report.json` changes exactly one row:

```text
_ZN12MarqueeImage4drawEii  byte False -> True  (pct 100.0, linked True, 0xa0 B)
```

All nine `MarqueeImage` dynsym rows are now at their original addresses in the
plain link:

```text
$NM -nD --defined-only /tmp/w3745_marquee_after.so | grep MarqueeImage
0015f59c T _ZN12MarqueeImageC1Etiiif
0015f59c T _ZN12MarqueeImageC2Etiiif
0015f5f0 T _ZN12MarqueeImageD1Ev
0015f5f0 T _ZN12MarqueeImageD2Ev
0015f5f2 T _ZN12MarqueeImage11setPositionEii
0015f5f8 T _ZN12MarqueeImage8setSpeedEf
0015f5fc T _ZN12MarqueeImage6updateEi
0015f654 T _ZN12MarqueeImage4drawEii
0015f6f4 T _ZN12MarqueeImage4drawEv
```

The plain no-order section layout is unchanged by the pass
(`.text 0x1337e8 @ 0x69440`, `.ARM.exidx 0x6f70 @ 0x19cc28`,
`.ARM.extab 0x2628c @ 0x1a3b98`, `.rodata 0x41b67 @ 0x1c9e30`).

`MarqueeImage::draw()` is at its original address but is still not byte-exact
in the plain link: it tail-calls `draw(int, int)` through the Thumb→ARM
interworking veneer, which is at `0x19c618` in the original and `0x19c638` in
the plain link (`b.w` displacement `0x3cbf8e` vs `0x3cbf9e`). This is the same
veneer-placement artifact recorded for `Sparks::explode(Vector)` in §22.3; it
is byte-exact in the gate link.

### 24.4 Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order baseline
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_marquee_base.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_marquee_base.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_marquee_base.so

# source reorder: setPosition, setSpeed, update, draw(int,int), draw()
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order after
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_marquee_after.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_marquee_after.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_marquee_after.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/engine/render/MarqueeImage.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep MarqueeImage

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_marquee_gate_after.so
git stash push -- src/engine/render/MarqueeImage.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_marquee_gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745_marquee_gate_base.so /tmp/w3745_marquee_gate_after.so
cmp -l /tmp/w3745_marquee_gate_base.so /tmp/w3745_marquee_gate_after.so | wc -l   # 0
```

### 24.5 Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4498 / linked 4498`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `2962 / 4498`, prefix 3666,
  3728 / 4524, first mismatch `MarqueeImage::setPosition` `0x15f5f2` vs
  `0x15f5f6`.
* The original emits `MarqueeImage` in the `.dynsym` address order
  `ctor, D1/D2, setPosition, setSpeed, update, draw(int,int), draw()`; the
  `gof2hd_1.1.19_ios` source-layout dump independently witnesses
  `setSpeed, update, draw(int,int), draw()` in that order.
* The baseline object emitted `setSpeed, setPosition, draw(), draw(int,int),
  update`; moving the definitions to `setPosition, setSpeed, update,
  draw(int,int), draw()` makes the object's `.text.*` order match the original
  address order. The section bodies and sizes are unchanged.
* The landed move is gate-neutral (`4498 / 4498`, lint `CLEAN ✓`, drift 0,
  every ELF section size/base unchanged) and the gate `.so` is byte-identical
  (`cmp -l` = 0), so the gate `report.json` is unchanged.
* The plain no-order build now has all nine `MarqueeImage` dynsym rows at
  their original addresses: exact addresses 3728 -> 3733, exact-address
  prefix 3666 -> 3685, plain `byte_exact` 2962 -> 2963.
* The next plain no-order frontier is `SpacePoint::isFree`
  (`0x1603e6` original, `0x1603f4` ours). The original `SpacePoint` order is
  `ctor, isFree, take, giveFree, getIndex, D1/D2`; our object emits
  `ctor, giveFree, take, D1/D2, isFree, getIndex`.

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `2963`, not `4498`.
* `MarqueeImage::draw()` is at its original address but is not byte-exact in
  the plain link only because the Thumb→ARM veneer is at `0x19c638` instead
  of `0x19c618`. The veneer placement rule is not characterized here; it is
  byte-exact in the gate link.
* The `SpacePoint` reorder is the obvious next per-TU source-order
  experiment, but it is a separate TU and was not tested or landed in this
  pass.
