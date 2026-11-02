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
