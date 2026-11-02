# Section-order removal: `ArraySetLength` placement (w3745)

Worker: w3745. Branch: `wave87-w3745`, based on master `66aafce4`
("docs: lock MGame::OnUpdate 99.88%"). Original:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b
clang 7.0.2 `-Oz`). All numbers below were produced in this worktree with the
commands shown. Scratch output is under `/tmp/w3745/aset/`.

This note closes the open item in
`_work/reconstructions/deferred_instantiation_flush_w3745.md` §6: the first
remaining plain (no `--section-ordering-file`) mismatch was
`ArraySetLength<AbstractGun*>`. The cause is a source-order divergence between
`assignGuns()` and `connectPlayers()`; moving `assignGuns()` before
`connectPlayers()` is gate-neutral and makes the whole `ArraySetLength` /
`getBoundingVolume` block address-exact in the plain link.

## 0. Result

| build | gate byte/linked | plain byte_exact | exact addresses | exact-address prefix | first plain mismatch |
|---|---:|---:|---:|---:|---|
| before (HEAD `66aafce4`) | **4497 / 4497** | **2986** | **3179 / 4524** | **2304** | `ArraySetLength<AbstractGun*>` `0xc3dde` vs `0xc3efc` |
| after (this pass) | **4497 / 4497** | **2989** | **3182 / 4524** | **2342** | `Level::updateAlienAttackers` `0xc5ca4` vs `0xc5cc8` |

Delta: `+3` plain byte_exact, `+3` exact addresses, `+38` exact-address prefix.
Gate unchanged (`4497/4497`, `RATCHET PASS`), `.text` `0x1337c8 @ 0x69440`,
lint `CLEAN ✓`, drift `0`.

The entire block from `createRadioMessages` through `getPlayer` is now at the
original addresses:

| symbol | original | before | after |
|---|---:|---:|---:|
| `Level::createRadioMessages(int)` | `0xc0de4` | `0xc0de4` | `0xc0de4` |
| `ArraySetLength<AbstractGun*>` | `0xc3dde` | `0xc402c` | **`0xc3dde`** |
| `ArraySetLength<Player*>` | `0xc3e12` | `0xc3dde` | **`0xc3e12`** |
| `Level::getBoundingVolume(int, AEGeometry*)` | `0xc3e48` | `0xc3e14` | **`0xc3e48`** |
| `Array<RadioMessage*>::Array()` | `0xc4060` | `0xc4060` | `0xc4060` |
| `ArraySetLength<RadioMessage*>` | `0xc407c` | `0xc407c` | `0xc407c` |
| `Level::getPlayer()` | `0xc40b0` | `0xc40b0` | `0xc40b0` |

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

Plain no-order baseline:

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/aset/noorder_base.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/aset/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/aset/noorder_base.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/aset/noorder_base.so
```

```text
byte_exact 2986  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3179/4524 same; prefix 2304;
            first (0xc3dde, '_Z14ArraySetLengthIP11AbstractGunEvjR5ArrayIT_E', 0xc3efc)
```

## 2. Source-layout evidence

The original source-layout dump is authoritative for TU membership and symbol
order. `_work/original_layout/original_layout.json`, iOS 1.1.19
`Classes/Game/Level.cpp`, has this symbol order (indices 20-41):

```text
20 assignGuns()
21 connectPlayers()
...
25 getStarSystem()
26 ArraySetLength<KIPlayer*>
27 getGasClouds()
28 ArraySetLength<Array<Gun*>*>
29 ArraySetLength<Gun*>
30 createGun(...)
31 createStaticObject(...)
32 ArrayAdd<KIPlayer*>
33 createShip(...)
34 createRoute(int)
35 setPlayerRoute(Route*)
36 createRadioMessages(int)
37 ArraySetLength<AbstractGun*>
38 ArraySetLength<Player*>
39 getBoundingVolume(int, AEGeometry*)
40 ArraySetLength<RadioMessage*>
41 getPlayer()
```

Two facts follow directly:

* `assignGuns` precedes `connectPlayers` in the original TU.
* The original deferred order is `ArraySetLength<AbstractGun*>`, then
  `ArraySetLength<Player*>`, then `ArraySetLength<RadioMessage*>`.

The same dump also shows that `ArraySetLength<BoundingVolume*>` belongs to
`Classes/Game/Globals.cpp`, not `Level.cpp`:

```sh
python3 - <<'PY'
import json
j = json.load(open('_work/original_layout/original_layout.json'))
for ver, d in j['tu_symbols'].items():
    for k, v in d.items():
        for s in v['symbols']:
            if 'ArraySetLength<BoundingVolume' in s:
                print(ver, k, '::', s)
PY
```

```text
gof2hd_1.1.19_ios Classes/Game/Globals.cpp :: _void ArraySetLength<BoundingVolume*>(...)
gof2sd_1.0.1_ios Classes/Game/PlayerJumpgate.cpp :: _void ArraySetLength<BoundingVolume*>(...)
```

Our `Level.cpp` emits a duplicate weak copy; the linker keeps the earlier
`Globals.cpp` copy (`0xa50c8`), so the duplicate does not occupy output space.
It is not the cause of the address delta.

## 3. Mechanism

clang 7.0.2 emits implicit template copies in **first-textual-use order**.
`ArraySetLength<Player*>` is first used in `connectPlayers()`
(`ArraySetLength(enemies->size(), *targets)`); `ArraySetLength<AbstractGun*>`
is first used in `assignGuns()` (`ArraySetLength(slots, *enemyGuns)`). In the
baseline source `connectPlayers()` was defined before `assignGuns()`, so the
`Player*` copy was emitted first:

```text
baseline Level.cpp.obj (relevant order)
  createRadioMessages
  ArraySetLength<Player*>
  getBoundingVolume
  ArraySetLength<AbstractGun*>
  Array<RadioMessage*>::Array()
  ArraySetLength<RadioMessage*>
  getPlayer
```

Moving the `assignGuns()` definition before `connectPlayers()` makes its body
parsed first, so the `AbstractGun*` copy is emitted first:

```text
after Level.cpp.obj (relevant order)
  createRadioMessages
  ArraySetLength<AbstractGun*>
  Array<Player*>::Array()        (weak duplicate, discarded by the linker)
  ArraySetLength<Player*>
  Array<Player*>::~Array()       (weak duplicate, discarded by the linker)
  getBoundingVolume
  Array<BoundingVolume*>::Array() (weak duplicate, discarded by the linker)
  ArraySetLength<BoundingVolume*> (weak duplicate, discarded by the linker)
  Array<RadioMessage*>::Array()
  ArraySetLength<RadioMessage*>
  getPlayer
```

The discarded weak duplicates do not take output addresses, so the kept
symbols land exactly at the original addresses. The move is a pure source-order
change; no flag, pragma, anchor, or linker change is involved.

## 4. Controlled A/B

The only source change is the move of the `Level::assignGuns()` definition from
after `connectPlayers()` (and after `createShip`/`createRoute`/
`setPlayerRoute`) to immediately before `connectPlayers()`. The function body is
unchanged.

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/aset/noorder_final.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/aset/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/aset/noorder_final.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/aset/noorder_final.so
```

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)

plain no-order: compared 4524  byte_exact 2989  linked_exact 4497
                3182/4524 same; prefix 2342;
                first (0xc5ca4, '_ZN5Level20updateAlienAttackersEi', 0xc5cc8)
```

The address check with `nm`:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nD --defined-only /tmp/w3745/aset/noorder_final.so | \
  awk '$1>="000c3d00" && $1<="000c4100"'
```

```text
000c3dde W _Z14ArraySetLengthIP11AbstractGunEvjR5ArrayIT_E
000c3e12 W _Z14ArraySetLengthIP6PlayerEvjR5ArrayIT_E
000c3e48 T _ZN5Level17getBoundingVolumeEiP10AEGeometry
000c4060 W _ZN5ArrayIP12RadioMessageEC2Ev
000c407c W _Z14ArraySetLengthIP12RadioMessageEvjR5ArrayIT_E
000c40b0 T _ZN5Level9getPlayerEv
```

These are the original addresses (same `nm` on the original).

## 5. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN`, drift 0.
* Baseline plain no-order is `byte 2986 / linked 4497`, 3179/4524 addresses,
  prefix 2304; first mismatch `ArraySetLength<AbstractGun*>` at `0xc3dde`
  (ours `0xc3efc`).
* Moving `Level::assignGuns()` before `Level::connectPlayers()` is
  gate-neutral (`4497/4497`, `.text` unchanged, lint `CLEAN`, drift 0) and
  improves the plain no-order link to `2989 / 4497`, 3182/4524 addresses,
  prefix 2342.
* The whole `createRadioMessages`/`ArraySetLength<AbstractGun*>`/
  `ArraySetLength<Player*>`/`getBoundingVolume`/`ArraySetLength<RadioMessage*>`/
  `getPlayer` block is now at the original addresses.
* The original iOS 1.1.19 `Level.cpp` symbol order has `assignGuns` before
  `connectPlayers` and `ArraySetLength<AbstractGun*>` before
  `ArraySetLength<Player*>`.
* The original `ArraySetLength<BoundingVolume*>` belongs to `Globals.cpp`;
  our `Level.cpp` duplicate is a discarded weak copy.

**Hypothesis / still open**

* The next plain no-order mismatch is `Level::updateAlienAttackers`
  (`0xc5ca4` original, `0xc5cc8` ours). It is caused by
  `ArrayAdd<RadioMessage*>`: the original emits it at `0xc628a` (after
  `checkGameOver`), ours at `0xc5ca4` (immediately after `createRadioMessage`).
  This is a separate deferred-template placement issue and was not fixed here.
* The extra weak copies in our `Level.cpp` (`Array<Player*>::Array()`,
  `Array<Player*>::~Array()`, `Array<BoundingVolume*>::Array()`,
  `ArraySetLength<BoundingVolume*>`, `Array<RadioMessage*>::Array()`) are not
  in the original `Level.cpp` symbol list. They are discarded by the linker
  today, so they do not affect addresses; removing them (via `extern template`
  / original-owner TU placement) is a TU-membership cleanup, not needed for
  this fix.
* The ordering file is still required: the plain link is `2989`, not `4497`.
