# `.ARM.extab` LSDA cleanup scopes: corrected decoder + three byte-exact-body records (w7681)

Status: research note, **no landed source change**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7681`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).

## Result

| metric | before | after |
|---|---:|---:|
| `.ARM.extab` differing bytes | 1427 | **1427** |
| `.ARM.extab` differing records | 13 | **13** |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.rel.plt` differing bytes | 0 | **0** |
| byte_exact / linked_exact | 4506 / 4506 | **4506 / 4506** |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

`.ARM.extab` is still size- and base-exact at `0x26298` @ `0x1a3b78`.

## 1. Correct LSDA decoding (new this wave)

The w7662/w7642 notes decoded the call-site table but the exact header offset was
not written down.  A generic-personality `.ARM.extab` record is:

```
[4-byte prel31 personality]
[unwind opcodes: (SIZE+1) words]
[LSDA: LPStart encoding, TType encoding, ULEB class-info offset,
       call-site encoding, ULEB call-site length, call-site rows]
```

The unwind-opcode block starts with `SIZE = (words-1)` but the bytes are stored
MSB-to-LSB per 32-bit word, so the SIZE byte is at **record offset 7**, not 4.
The LSDA therefore starts at `record + 4 + (SIZE+1)*4`.  For all GoF2 generic
records `SIZE == 1` (8-byte unwind block), so the LSDA starts at record offset
12.  The LSDA header is the normal `ff ff 01 <uleb len>` (LPStart omit, TType
omit, ULEB128 call sites).  The w7662 decoder's row values were correct because
it happened to skip the 8 bytes; this note records the rule explicitly.

The corrected decoder is committed as `_work/reconstructions/extab_lsda_decode.py`.
It prints every differing record side by side, grouped by the function at the
exidx entry (duplicate exidx entries for the same address are kept in order).

```sh
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

## 2. Current 13 differing records

| owner (function at the exidx entry) | body byte-exact? | diff bytes |
|---|---|---:|
| `Generator::createMission` | no (90.0%) | 24 |
| `PlayerJumpgate::PlayerJumpgate` | **yes** | 15 |
| `Level::createMission` | no (92.3%) | 139 |
| `Level::createShip` | no (70.5%) | 70 |
| `RecordHandler::recordStoreRead` | **yes** | 3 |
| `ModStation::ModStation` | no (99.2%) | 2 |
| `Radar::Radar` | **yes** | 7 |
| `Radar::draw` | no (75.3%) | 29 |
| `LevelScript::process` | no (60.2%) | 292 |
| `HangarWindow::render` | no (94.4%) | 762 |
| `Hud::draw` | no (96.6%) | 40 |
| `SpaceLounge::drawLounge` | no (95.7%) | 14 |
| `MGame::OnTouchEnd` | no (81.9%) | 30 |

The three byte-exact bodies are pure EH-shape deltas; the other ten will close
when their bodies become byte-exact.  `ModStation` is in the second group: its
row-3 start differs by 8 because the `MatrixSetRotation` argument schedule is
still 11 bytes / 4 runs off (see `modstation-ctor-wave125-w7660.md`).

## 3. The three byte-exact-body records

### `PlayerJumpgate::PlayerJumpgate` (`0xa4fa4`, 264 B)

Original rows (start, length, landing pad):

```
(0,48,0) (48,16,238) (66,4,224) (74,8,238) (90,22,236)
(126,12,234) (156,20,222) (202,8,238) (210,54,0)
```

Ours:

```
(0,48,0) (48,8,236) (58,6,234) (66,4,224) (74,8,234)
(90,48,238) (156,20,222) (202,8,236) (210,54,0)
```

The call sites are identical (setVisible 52, `operator new` 60, Array ctor 66,
ArraySetLength 78, inAlienOrbit 90, getSystem 104, getRace 108, setRadius 128,
`operator new` 134, BoundingSphere ctor 172, setRotation 210).  The original
groups setVisible + `operator new` + ArraySetLength + setRotation on one
cleanup block (238), and splits inAlienOrbit/getSystem/getRace (236) from
setRadius/`operator new` (234).  Ours merges inAlienOrbit..`operator new` on
238 and keeps setVisible/setRotation (236) separate from
`operator new`/ArraySetLength (234).

Source experiments tried (all preserve the 264-byte body unless noted):

* `volumes` declared before the `if` (with/without `= 0`): moves the
  trampoline assignment but does **not** merge setVisible with the if body.
* `int radius;` / `int radius = 0;` before the `if`: changes the body layout
  (offsets 60/68/76...), not usable.
* nested `{ }` around the radius if/else or around setRadius+BoundingSphere:
  optimised away, no LSDA change.
* `BoundingSphere *sphere = new ...; (*...)[0] = sphere;`: adds a 6-byte row at
  132, not usable.
* `setVisible` inside/after the `if`: changes the body layout.
* `for (int once = 0; once < 1; ++once)` around the radius if/else **does**
  produce the original LP_B/LP_C split (rows 4/5/6 match exactly), but the loop
  costs 4 bytes (total 268 B) and the function is no longer byte-exact.
  `while (1) { ...; break; }` is optimised back to the 264-byte v0 body and
  loses the split.  `for` with `once == 0`, `!once`, `<= 0`, `< 1u`, `break`,
  or a loop variable declared outside all keep the +4-byte loop.
* `alloc_before_if` / conditional allocation: the allocation moves before the
  `cbz`, body changes.

Conclusion: the split needs a real loop/scope boundary; the compiler at `-Oz`
will not emit the boundary for a plain block.  The only construct found that
produces it costs 4 bytes, which would drop `byte_exact` (the body is currently
byte-exact) and `size-exact`.

### `RecordHandler::recordStoreRead` (`0xcbd34`, 5416 B)

Only rows 161/162/164 differ; pads 5078 and 5100 are both `b.n 0xcd24e`
(destroy local `String path` at `sp+0x9c`):

```
orig  (4028,6,5078) (4040,6,5100) (4054,6,5100)
ours  (4028,6,5100) (4040,6,5078) (4054,6,5078)
```

The call sites are `AEFile::Read(equipmentCount)` at 4028, `operator new` at
4040 and `ArraySetLength` at 4054.  The original assigns the pre-`if` Read to
the lower trampoline and the `if` body to the higher one; ours is exactly
swapped.  Source experiments tried (all keep the 5416-byte body and the same
call-site offsets):

* `equipment` declared before the `if` (with `= 0`, `= nullptr`, no init).
* `new` before the `if` (compiler sinks it back), conditional allocation.
* `if (equipmentCount > 0)`, `if (equipmentCount)`, comma-operator condition,
  `goto skip`, `if/else {}` with empty else, `do { } while (0)`, nested `if (1)`.
* `bool hasEquipment = equipmentCount >= 1;` before the `if`.
* `AEFile::Read` declared `void` instead of `bool` (return value unused).
* `for`/`while`/`do-while` around the equipment block: changes the body
  offsets (3992/3976/...), not usable.

The swap is a pure permutation of two adjacent trampolines in the ARM backend's
landing-pad table; none of the source spellings tried changes the machine block
order of the two landing-pad blocks.

### `Radar::Radar` (`0x12d484`, 2488 B)

Rows 1/3/11/13/15 use pad 2462 in the original and 2464 in ours; rows 19/21
use 2464 in the original and 2462 in ours.  Pads 2462/2464 are both
`b.n 0x12de28` (destroy `String` at `sp+0x10`).  This is the same two-identical-
trampoline swap.  The early rows are the `labelStrings` block (Matrix ctor,
ArraySetLength, inAlienOrbit, the `new String` calls); the late rows are the
post-`labelStrings` code (`Level::getEnemyGuns`, `Image2DCreate`).

Source experiments tried (all keep the 2488-byte body):

* remove the explicit `{ }` around the `data()[0]` assignment.
* add a `{ }` around the `data()[1..3]` assignments.
* wrap either group in `while (1) { ...; break; }` (optimised away).
* replace the `data()[0]` expression with named `String name/suffix` locals:
  changes the body layout (offsets 554/660/...), not usable.

## 4. Negative / attempted this wave

* Corrected the LSDA decoder: the unwind-opcode SIZE byte is at record offset 7
  and the LSDA starts at `4 + (SIZE+1)*4`.  This confirms the w7662 row values
  and makes the remaining records reproducible from the committed script.
* All PlayerJumpgate / recordStoreRead / Radar source-scope variants above.
* `for`-loop boundary for PlayerJumpgate: correct split, +4 bytes.
* `AEFile::Read` return-type change: no LSDA change.
* `while (1) { ...; break; }` is consistently optimised to the plain body and
  does not create a cleanup boundary.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7681
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields

python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# extab differing bytes 1427, records differ 13, len-diff 0
```

Scratch variants and the raw decoder used this pass live in `/dev/shm/w7681/`
(`lsda.py`, `objlsda.py`, `gen*.py`).
