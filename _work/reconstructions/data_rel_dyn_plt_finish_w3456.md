# `.data` / `.rel.dyn` / `.rel.plt` finish audit (w3456)

Status: research note (2026-11-01). Worktree `wt/w3456`, branch `wave87-w3456`,
based on master `7aab8bdc`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2,
`-Oz`, image base 0x10000). Every number below is from a command run in this tree or
from the original binary.

## TL;DR

At the current baseline **all ELF section sizes and bases are exact**, including
`.data` (0x296c @ 0x214000), `.data.rel.ro` (0x2fe8 @ 0x20cbb0), `.rel.dyn`
(0x7048 @ 0x504d4), `.rel.plt` (0x72d0 @ 0x5751c), `.got`, `.bss` and `.dynamic`.
The remaining differences are **content only**:

| section | differing bytes | runs | what it is |
|---|---:|---:|---|
| `.data` | **3** | 2 | 2 libc++abi `.text` pointers (`demangling_terminate_handler`, `demangling_unexpected_handler`), both `-0x24` |
| `.data.rel.ro` | **667** | 611 | 599 libc++abi `.text` pointers; 9 `Level::createScene` switch-table `.rodata` pointers (`+0x3cc`); 3 `.bss` pointers |
| `.rel.dyn` | **70** | 70 | 64 entries: 57 same-name/different-symbol-index, 7 reordered; the `(offset,type,name)` multiset is **exactly equal** |
| `.rel.plt` | **189** | 163 | 163 entries: 134 same-name/different-symbol-index, **29 name-order entries in exactly two groups (8 + 21)**; all offsets and types equal |

The `.rel.dyn` and `.rel.plt` symbol-index bytes are downstream of `.dynsym`
order. The `.rel.plt` name-order bytes are downstream of the object emission order
of `Level.cpp` and `LevelScript.cpp`. `.data` / `.data.rel.ro` are downstream of
the vendored libc++abi layout plus the known `.rodata`/`.bss` order campaigns.

No strict improvement was found. One controlled experiment (moving
`Level::createStaticObject` after `Level::createStaticObjects`) fixes the
`Level.cpp` PLT group but regresses two other functions, net **byte 4263 -> 4262**.
It is documented below and **not committed**.

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4263  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint` is `CLEAN ✓` and `drift` is `0 field(s) across 0 class(es)`. The
co-ordinator's stated baseline (byte 4182 / linked 4495) is stale for this
branch; the measured baseline is **4263 / 4496**.

Section table (`readelf -SW` on both files):

```
.rel.dyn     orig 0x504d4/0x7048   ours 0x504d4/0x7048   delta 0
.rel.plt     orig 0x5751c/0x72d0   ours 0x5751c/0x72d0   delta 0
.data.rel.ro orig 0x20cbb0/0x2fe8  ours 0x20cbb0/0x2fe8  delta 0
.dynamic     orig 0x20fba4/0x148   ours 0x20fba4/0x148   delta 0
.got         orig 0x20fcf4/0x430c  ours 0x20fcf4/0x430c  delta 0
.data        orig 0x214000/0x296c  ours 0x214000/0x296c  delta 0
.bss         orig 0x216970/0x69ac  ours 0x216970/0x69ac  delta 0
```

`readelf -dW` shows identical relocation metadata:

```
DT_PLTGOT   0x21068c      DT_PLTRELSZ 29392 (0x72d0)
DT_JMPREL   0x5751c       DT_REL      0x504d4
DT_RELSZ    28744 (0x7048) DT_RELCOUNT 1162
```

## 2. `.data` — 3 bytes, 2 libc++abi pointers

Raw byte comparison (`/tmp/final_audit.py`, §6):

```
.data  diff bytes 3  runs 2  first 0x216960
  0x216960  orig 0x00193f25  ours 0x00193f01   R_ARM_RELATIVE
  0x216964  orig 0x00194015  ours 0x00193ff1   R_ARM_RELATIVE
```

Both are Thumb function pointers into libc++abi's `.text`:

* original `0x193f25` -> `0x193f24`, ours `0x193f01` -> `0x193f00`
  (`demangling_terminate_handler`);
* original `0x194015` -> `0x194014`, ours `0x193ff1` -> `0x193ff0`
  (`demangling_unexpected_handler`).

The unstripped relink (`/tmp/unstripped_w3456.so`) names our targets:

```
00193f00 000000f0 t _ZL28demangling_terminate_handlerv
00193ff0 0000001c t _ZL29demangling_unexpected_handlerv
0019400c 00000238 T __cxa_demangle
```

The original's `__cxa_demangle` is at `0x194030` (nm -D), i.e. **0x24 later**.
So the whole libc++abi `.text` prefix is 0x24 bytes shorter in our archive. This
is a vendored-archive/layout difference, not app `.data` content.

## 3. `.data.rel.ro` — 667 bytes, 611 words

All 611 differing words are `R_ARM_RELATIVE`. Classified by target section and
word delta:

| words | original target | ours target | delta |
|---:|---|---|---:|
| 536 | `.text` | `.text` | `-0x24` |
| 63 | `.text` | `.text` | `-0x20` |
| 9 | `.rodata` | `.rodata` | `+0x3cc` |
| 1 | `.bss` | `.bss` | `-0x10` |
| 1 | `.bss` | `.bss` | `+0x4` |
| 1 | `.bss` | `.bss` | `-0xc` |

The 599 `.text` words are the same libc++abi layout delta as `.data` (two
different shifts because the archive's `.text` is not uniformly short).

The 9 `.rodata` words are `Level::createScene`'s switch table at
`0x20d9c0..0x20d9e1`:

| slot | original | ours | symbol |
|---|---:|---:|---|
| `0x20d9c0` | `0x1fd740` | `0x1fdb0c` | `kShipHangarHeights` |
| `0x20d9c4` | `0x1fd758` | `0x1fdb24` | `kHangarMeshes` |
| `0x20d9c8` | `0x1fd740` | `0x1fdb0c` | `kShipHangarHeights` |
| `0x20d9cc` | `0x1fd7c4` | `0x1fdb90` | (Level table) |
| `0x20d9d0` | `0x1fd740` | `0x1fdb0c` | `kShipHangarHeights` |
| `0x20d9d4` | `0x1fd740` | `0x1fdb0c` | `kShipHangarHeights` |
| `0x20d9d8` | `0x1fd740` | `0x1fdb0c` | `kShipHangarHeights` |
| `0x20d9dc` | `0x1fd740` | `0x1fdb0c` | `kShipHangarHeights` |
| `0x20d9e0` | `0x1fd7e8` | `0x1fdbd4` | (Level table) |

This is the known `Level.cpp` `.rodata` block shift (`+0x3cc`), not a `.data`
problem.

The 3 `.bss` words are the known `.bss` internal-order residue:

| slot | original | ours | symbol | delta |
|---|---:|---:|---|---:|
| `0x20da34` | `0x217bf8` | `0x217be8` | `gPirateGreetingFaceB` | `-0x10` |
| `0x20df04` | `0x218388` | `0x21838c` | `CHAR_BRENT` | `+0x4` |
| `0x20e29c` | `0x2183c0` | `0x2183b4` | `g_dw_playerFace` | `-0xc` |

`nm -nS /tmp/unstripped_w3456.so` confirms the three local symbols. `.bss` is
size-exact; only the internal order differs (see `bss_audit.md`).

## 4. `.rel.dyn` — a pure permutation, not a content difference

Type counts are exactly equal:

```
1817 R_ARM_ABS32
 614 R_ARM_GLOB_DAT
1162 R_ARM_RELATIVE
```

The multiset of `(r_offset, r_type, symbol_name)` is **exactly equal** between
the two files. 64 of 3593 entries differ:

* **57** entries have the same offset, type and symbol name but a different
  `r_info` symbol index;
* **7** entries are reordered; their offsets are a permutation of the same
  offsets (the `(offset,type,name)` multiset is still equal).

The 7 reordered entries are two combreloc blocks:

```
idx 2476..2479  curTouchSize / maxTouchSize / touches / Globals::Canvas
idx 3297..3299  _zip_err_type / _zip_nerr_str / _zip_err_str
```

Both blocks are GOT-slot-order permutations: the same four/three GOT slots are
assigned to the same symbols, but the combreloc sort order differs because the
symbol indices differ.

The 57 same-name/different-index entries are the `Globals` `.data` block
(`logoIsShown`, `isInMainMenu`, `cItemList*`, `sound`, `status`, `options`, ...)
and a handful of shader/`PaintCanvas`/`Transform` symbols. They are all
downstream of `.dynsym` order.

`.dynsym` order at this baseline: **5330 / 5578 entries at the same index**
(248 symbols differ). The largest single shift is
`RecordHandler::recordStoreWrite`/`recordStoreWritePreview`, which are at
dynsym indices 2047/2048 in ours but 1985/1986 in the original; that +62 shift
moves the whole `Globals` block by -2. The mechanism is gold's
`std::unordered_map` iteration order keyed by namepool offsets (see
`dynsym_relcount.md`), not a link knob.

## 5. `.rel.plt` — 134 index bytes + 29 name-order bytes in two groups

All 3674 entries are `R_ARM_JUMP_SLOT` at **identical `r_offset`s**. The
`(offset,type)` multiset is exactly equal; the `(offset,type,name)` multiset
differs in exactly two contiguous groups:

### Group A — 8 slots, `Level.cpp`

```
idx 1950  orig Player::setEmpData            ours FileRead::loadStaticCollision
idx 1951  orig PlayerFighter::PlayerFighter  ours FileRead::loadStationCollision
idx 1952  orig Array<RadioMessage>::Array    ours Player::setEmpData
idx 1953  orig ArraySetLength<RadioMessage>  ours PlayerFighter::PlayerFighter
idx 1954  orig RadioMessage::RadioMessage    ours Array<RadioMessage>::Array
idx 1955  orig RadioMessage::RadioMessage    ours ArraySetLength<RadioMessage>
idx 1956  orig FileRead::loadStaticCollision ours RadioMessage::RadioMessage
idx 1957  orig FileRead::loadStationCollision ours RadioMessage::RadioMessage
```

The original first-reference order inside `Level.cpp.obj` is
`createShip` -> `createRadioMessages` -> `getBoundingVolume`; ours is
`getBoundingVolume` -> `createShip` -> `createRadioMessages`.

`Level::getBoundingVolume` is the only caller of
`FileRead::loadStaticCollision`/`loadStationCollision` in `Level.cpp`, and it is
emitted immediately after `Level::createStaticObject` because
`createStaticObject` calls it before its definition. In the original,
`createStaticObject` is itself emitted at first use, so its callee
`getBoundingVolume` is deferred to its definition point (after
`createRadioMessages`).

### Group B — 21 slots, `LevelScript.cpp`

```
idx 2816..2836  a 21-slot rotation:
  orig: Standing::setStanding, TargetFollowCamera::setRotationAroundTarget,
        Player::setShootingEnabled, Mission::setType, AEGeometry::isVisible,
        PlayerEgo::autoTurretIsEnabled, PlayerEgo::setAutoTurret,
        PlayerWormHole::open, Level::flashScreen,
        StarSystem::scaleSunDuringSupernovaIntro,
        StarSystem::updateSupernova, PlayerEgo::isInWormhole,
        PlayerWormHole::freeMissionLock, StarSystem::switchSunForSupernovaIntro,
        PlayerEgo::setDockingState, StarSystem::switchPlanetForIntro,
        Level::switchSkyboxForIntro, ParticleSystemManager::reset,
        PlayerEgo::startSmokeEmission, PlayerStation::setVisible,
        PlayerEgo::deleteHackingGame
  ours: TargetFollowCamera::setRotationAroundTarget,
        Player::setShootingEnabled, Mission::setType, AEGeometry::isVisible,
        PlayerWormHole::open, Level::flashScreen,
        StarSystem::scaleSunDuringSupernovaIntro,
        StarSystem::updateSupernova, PlayerEgo::isInWormhole,
        PlayerWormHole::freeMissionLock, Standing::setStanding,
        PlayerEgo::autoTurretIsEnabled, PlayerEgo::setAutoTurret,
        PlayerEgo::setDockingState, PlayerEgo::deleteHackingGame,
        StarSystem::switchSunForSupernovaIntro,
        StarSystem::switchPlanetForIntro, Level::switchSkyboxForIntro,
        ParticleSystemManager::reset, PlayerEgo::startSmokeEmission,
        PlayerStation::setVisible
```

This is not a simple rotation: the original puts
`autoTurretIsEnabled`/`setAutoTurret` immediately after `AEGeometry::isVisible`,
while ours puts them after `Standing::setStanding`; the original puts
`deleteHackingGame` last, while ours puts it after `setDockingState`.

All 21 first references are relocation entries in
`LevelScript::process`. Their order is the order of the call sites in the
compiled body, so this group is downstream of the `LevelScript::process`
source-shape reconstruction (currently 54.6% fuzzy), not of a link flag.

## 6. Controlled experiment: move `Level::createStaticObject`

The group A diagnosis predicts that deferring `getBoundingVolume` should fix
the 8-slot group. I moved the whole `Level::createStaticObject` definition
(21,848 bytes, source lines 1275..1677) to five different anchors in
`src/game/world/Level.cpp`, rebuilt and ran the full gate each time:

| anchor | byte_exact | linked_exact | result |
|---|---:|---:|---|
| baseline (no move) | **4263** | 4496 | PASS |
| before `createSentryGuns` (after `createStaticObjects`) | 4262 | 4496 | FAIL -1 |
| before `createShip` | 4262 | 4496 | FAIL -1 |
| before `createRadioMessages` | 4262 | 4496 | FAIL -1 |
| before `getBoundingVolume` | 4262 | 4496 | FAIL -1 |
| before `getPlayer` | 4262 | 4496 | FAIL -1 |

All five positions are equivalent because clang emits `createStaticObject` at
its first use (`createStaticObjects` calls it), independent of where the
definition sits. The move does fix group A: `Level::createRadioMessages`
flips `byte False -> True`. But it also flips:

```
Level::createStaticObject        byte True  -> False
Globals::getWreckCollision       byte True  -> False
```

net **-1**. The 8-slot group is therefore real, but the collateral PLT
reordering of `createStaticObject`'s own first references outweighs the gain.
Not committed.

Reproduction of the experiment:

```sh
cp src/game/world/Level.cpp /tmp/Level.cpp.orig
python3 - <<'PY'
p='src/game/world/Level.cpp'
s=open('/tmp/Level.cpp.orig').read()
start=s.index('KIPlayer *Level::createStaticObject(')
end=s.index('void Level::createMission() {')
block=s[start:end]
s2=s[:start]+s[end:]
ins=s2.index('void Level::createSentryGuns() {')
s2=s2[:ins]+block+s2[ins:]
open(p,'w').write(s2)
PY
cmake --build cmake-build-match --target check -j8      # byte 4262
cp /tmp/Level.cpp.orig src/game/world/Level.cpp
cmake --build cmake-build-match --target check -j8      # byte 4263
```

## 7. What is proven vs still hypothesis

**Proven (commands in §1-§6):**

1. Baseline is byte 4263 / linked 4496; all ELF section sizes and bases are
   exact, including `.data`, `.data.rel.ro`, `.rel.dyn`, `.rel.plt`, `.got`,
   `.bss` and `.dynamic`.
2. `.data` differs by exactly 3 bytes: two `R_ARM_RELATIVE` libc++abi
   `demangling_*_handler` pointers, both `-0x24`.
3. `.data.rel.ro` differs by 667 bytes / 611 words: 599 libc++abi `.text`
   pointers (`-0x24`/`-0x20`), 9 `Level::createScene` `.rodata` pointers
   (`+0x3cc`), 3 `.bss` pointers (`gPirateGreetingFaceB`, `CHAR_BRENT`,
   `g_dw_playerFace`).
4. `.rel.dyn` is a pure permutation: the `(offset,type,name)` multiset is
   exactly equal; 57 entries differ only in symbol index, 7 are reordered.
5. `.rel.plt` has identical offsets and types; 134 entries differ only in
   symbol index; exactly 29 entries differ in name order, in two contiguous
   groups of 8 (`Level.cpp`) and 21 (`LevelScript.cpp`).
6. Moving `Level::createStaticObject` after `createStaticObjects` fixes the
   8-slot group (`createRadioMessages` becomes byte-exact) but regresses
   `createStaticObject` and `Globals::getWreckCollision`, net -1.

**Hypothesis / not yet proven:**

1. `.rel.dyn` becomes byte-identical only when `.dynsym` insertion order
   matches. The 248-symbol `.dynsym` delta is gold's hash-map iteration order
   keyed by namepool offsets; the mechanism is proven, the exact insertion
   order is not reconstructed.
2. `.rel.plt` group B is fixable only by reordering the call sites in
   `LevelScript::process` to the original code order (the function is 54.6%
   fuzzy and is a separate reconstruction target).
3. `.data` / `.data.rel.ro`'s 599 libc++abi pointers require an exact
   libc++abi `.text` layout (the original's archive is 0x24 bytes longer in
   the prefix), not app source.
4. The 9 `.rodata` and 3 `.bss` `.data.rel.ro` pointers fall out of the
   existing `.rodata` Level-block and `.bss` internal-order campaigns.

## 8. Reproduction commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w3456

cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# section sizes / bases
readelf -SW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E '\.data|\.rel\.|\.got|\.bss|\.dynamic'
readelf -SW cmake-build-match/libgof2hdaa.so            | grep -E '\.data|\.rel\.|\.got|\.bss|\.dynamic'

# dynamic metadata
readelf -dW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E 'REL|PLT|JMP'
readelf -dW cmake-build-match/libgof2hdaa.so            | grep -E 'REL|PLT|JMP'

# raw section byte diffs / relocation multisets / word classification
python3 /tmp/final_audit.py

# .rel.plt two-group extraction
python3 /tmp/groups.py

# .rel.dyn differing entries
python3 /tmp/reldyn_diff.py

# unstripped relink for local .bss names
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' | tail -1 \
  > /tmp/link_cmd.txt
# edit -o to /tmp/unstripped_w3456.so and drop the trailing strip, then:
bash /tmp/relink_unstripped.sh
arm-linux-androideabi-nm -nS /tmp/unstripped_w3456.so | grep -E 'gPirateGreetingFaceB|CHAR_BRENT|g_dw_playerFace'
```

The scratch scripts `/tmp/final_audit.py`, `/tmp/groups.py`,
`/tmp/reldyn_diff.py` and `/tmp/relink_unstripped.sh` are reproduced inline
above; they only read the two `.so` files and the build directory.
