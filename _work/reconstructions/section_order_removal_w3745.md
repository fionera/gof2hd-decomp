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
