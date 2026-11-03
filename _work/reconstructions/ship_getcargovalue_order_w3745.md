# Ship::getCargoValue placement — w3745

Worker: w3745. Branch: `wave87-w3745`, based on master `e49e5c5b`.
Original: `../_work/bins/android_2.0.16_libgof2hdaa.so` (stripped, armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`).

This note explains why `Ship::getCargoValue()` was emitted at `0x1749a4` in the
plain no-order link (the address of the original `Ship::~Ship()`) and records the
source-order change that puts it back at its original address `0x175270`.

## 1. Baseline

`cmake-build-match` was configured with `cmake --preset default`. Gate
(`cmake --build cmake-build-match --target check -j8`):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

`.text` is `0x1337c8 @ 0x69440`. The brief's `4496` baseline is stale; this HEAD
is **4498 / 4498**.

Plain no-order link (the exact gold line minus `--section-ordering-file`):

```text
byte_exact 3012  linked_exact 4498
exact addresses 3890 / 4524   exact-address prefix 3885
first mismatch: Ship::~Ship()  original 0x1749a4  ours 0x1749d8
```

Commands:

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_ship/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_ship/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_ship/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_ship/base_noorder.so
```

## 2. Original Ship TU order (binary evidence)

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN4Ship' | c++filt
```

The original Ship object is one contiguous `.text` block, `0x17457c..0x1757a0`
(the `.ARM.exidx` CANTUNWIND sentinels at `0x17457c` and `0x1757a0` are the
object boundaries). Its order is:

```text
0x17457c Ship::Ship(...)
0x174608 Ship::refreshValue()
0x1749a4 Ship::~Ship()
0x174a02 Ship::setRace(int)
0x174a06 Ship::getRace()
0x174a0a Ship::getIndex()
...
0x175204 Ship::setEquipment(Item*, int)
0x175270 Ship::getCargoValue()
0x1752a4 Ship::getEquipmentValue()
...
```

`getCargoValue` is after `setEquipment(Item*, int)`, not after `refreshValue`.

## 3. Why clang put it early

`refreshValue()` ends with `this->value += getCargoValue();` and is defined
before `getCargoValue()` in our `Ship.cpp`. Clang 7 emits a callee at first
reference when the callee's definition has not been parsed yet. Therefore the
object section order is:

```text
Ship::Ship(...)
Ship::refreshValue()
Ship::getCargoValue()     <-- first reference from refreshValue
Ship::~Ship()
...
```

The ctor calls `refreshValue`, so `refreshValue` is emitted immediately after
the ctor (deferred). `refreshValue` then references `getCargoValue`, which is
emitted immediately after `refreshValue`. The original object instead has
`~Ship` immediately after `refreshValue` and `getCargoValue` at its definition
position after `setEquipment(Item*, int)`.

Controlled tiny compile (clang 7.0.2, same flags) reproduces the rule:

```sh
# refresh() defined before getCargo() and calls it:
#   ctor, refresh, getCargo, ~S
# getCargo() defined before refresh(), refresh() still calls it:
#   ctor, refresh, ~S, ..., getCargo   (getCargo emitted at its definition)
```

So the original source order must define `refreshValue` **after**
`getCargoValue`; the ctor's call still emits `refreshValue` early, but
`getCargoValue` is then emitted at its own definition.

## 4. Landed source change

Only definition order in `src/game/ship/Ship.cpp` changed (no body edited):

1. `Ship::refreshValue()` moved from directly after the ctor to directly after
   `Ship::getCargoValue()`.
2. Four one-line accessors that were pulled before `changeLoad` were moved to
   their original positions after `getCargoValue`:
   `getMaxPassengers` (after `getEquipmentValue`), `setPrice` (after
   `adjustPrice`), `getNumAddedDeviceSlots` and `getUnmoddedHandling` (after
   `equals`).

The resulting object section order is now:

```text
Ship::Ship(...)
Ship::refreshValue()
Ship::~Ship()
...
Ship::setEquipment(Item*, int)
Ship::getCargoValue()
Ship::getEquipmentValue()
...
```

and the plain no-order link puts `getCargoValue` at **0x175270**, exactly the
original address.

## 5. Measured results

| state | plain byte | exact addresses | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---|
| baseline | 3012 | 3890 / 4524 | 3885 | `Ship::~Ship()` `0x1749a4` vs `0x1749d8` |
| refreshValue move only | 3013 | 3892 / 4524 | 3887 | `Ship::setRace(int)` `0x174a02` vs `0x174a0a` |
| + four accessor moves (landed) | **3031** | **3925 / 4524** | **3887** | `Ship::setRace(int)` `0x174a02` vs `0x174a0a` |

Gate is unchanged by the landed change:

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

Gate `.so` A/B (stash the source, relink, restore, relink):

```text
baseline sha256  ee2ce25a7b7f6d401984122c6ede3f200fad08c4163f5b42a0648c27918a7e94
landed   sha256  ae3e2ec7ccb30d105dd301470f7559b017067256c30f749357c8f7462617dbf0
cmp -l | wc -l  193
```

The 193 differing bytes are only `.note.gnu.build-id` (20), `.ARM.exidx` (25)
and `.ARM.extab` (148). `.text`, `.rodata`, `.data.rel.ro`, `.data` and every
other section are byte-identical; every ELF section keeps its original size and
base.

## 6. Proven vs hypothesis

**Proven by measurement**

* The baseline plain no-order frontier is `Ship::~Ship()`; `getCargoValue` is
  emitted at `0x1749a4` because `refreshValue` (defined before it) references it.
* Moving `refreshValue` after `getCargoValue` makes clang emit `getCargoValue`
  at its definition position; the plain link then places it at `0x175270`.
* The four accessor moves remove a `+0x10` cumulative shift at `changeLoad`
  (`getMaxPassengers`/`setPrice`/`getNumAddedDeviceSlots`/`getUnmoddedHandling`
  were emitted before `changeLoad` instead of after `getCargoValue`).
* The landed change is gate-neutral (4498 / 4498, lint clean, drift 0) and a
  strict plain-link improvement: **+19 byte_exact, +35 exact addresses, +2
  prefix** over baseline.

**Hypothesis / next frontier**

* The next plain mismatch is `Ship::setRace(int)` at `0x174a02`; our source
  order is `getIndex, getRace, setRace`, the original is `setRace, getRace,
  getIndex`. Reordering those three is the next Ship TU step.
* `Ship::clone()` calls `addMod`, so clang still emits `addMod` immediately
  after `clone` (ours `0x1754c8`) instead of after `setPrice` (original
  `0x1755bc`). Defining `addMod` before `clone` in the source (so `clone` is the
  deferred callee of `makeShip`) should move it to `0x1755bc`; not tested here.
* The remaining Ship address deltas are source-order, not link flags. The
  section-ordering file is still required until the whole TU order converges.
