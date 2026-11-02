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
