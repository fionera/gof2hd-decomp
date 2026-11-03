# Section-order removal: `Radio::setMessages` placement (w3745)

Worker: w3745. Branch: `wave87-w3745`, based on master `82d08d75`
("docs: lock Status 86.7%, createMission 90.6%, Gun overload order").
Original: `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2 `-Oz`). All numbers below were produced in this worktree
with the commands shown. Scratch output is under `/tmp/w3745_radio/`.

This note closes the next open item in
`_work/reconstructions/gun_setoffset_overload_order_w3745.md` §6: the first
plain (no `--section-ordering-file`) mismatch at this HEAD is
`Radio::setMessages(Array<RadioMessage*>*)` at `0x15501c` (ours `0x155036`).
Reordering the top-level definitions in `src/game/core/Radio.cpp` to the
original emitted order makes the whole `Radio` block address-exact in the
plain link. The source change is gate-neutral and the gate `.so` is
byte-identical before and after.

## 0. Result

| build | gate byte/linked | plain byte_exact | exact addresses | exact-address prefix | first plain mismatch |
|---|---:|---:|---:|---:|---|
| before (HEAD `82d08d75`) | **4497 / 4497** | **2856** | **3463 / 4524** | **3373** | `Radio::setMessages` `0x15501c` vs `0x155036` |
| after (this pass) | **4497 / 4497** | **2857** | **3470 / 4524** | **3385** | `SolarSystem::getName()` `0x1556f0` vs `0x15571e` |

Gate unchanged (`4497/4497`, `RATCHET PASS`), `.text` `0x1337c8 @ 0x69440`,
`.rodata` 0 differing bytes, lint `CLEAN ✓`, drift `0`. The gate `.so` sha256
is **identical** before and after:

```text
1ecc20f3fbd49abce61ac8426fee08743f908610d5d0a6dbb69c1e4d6e4c050b
```

Delta: `+1` plain byte_exact, `+7` functions at their original address,
`+12` exact-address prefix, **0 losses** in the plain verify report.

The entire `Radio` block is now at the original addresses in the plain link:

| symbol | original | before | after |
|---|---:|---:|---:|
| `Radio::Radio()` (`C1`/`C2`) | `0x154f7c` | `0x154f7c` | `0x154f7c` |
| `Radio::~Radio()` (`D1`/`D2`) | `0x154fd4` | `0x154fd4` | `0x154fd4` |
| `Radio::setMessages(Array<RadioMessage*>*)` | `0x15501c` | `0x155036` | **`0x15501c`** |
| `Radio::setCurrentMessage(RadioMessage*)` | `0x15504c` | `0x155066` | **`0x15504c`** |
| `Radio::getMessage(int)` | `0x155050` | `0x15502c` | **`0x155050`** |
| `Radio::lastMessageShown()` | `0x15505a` | `0x155026` | **`0x15505a`** |
| `Radio::update(long, PlayerEgo*, LevelScript*)` | `0x155060` | `0x15506c` | **`0x155060`** |
| `Radio::draw(long long, PlayerEgo*, LevelScript*)` | `0x155378` | `0x155384` | **`0x155378`** |
| `Radio::isShowingMessage()` | `0x1555d0` | `0x15501c` | **`0x1555d0`** |

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

Gate ELF layout (`readelf -SW cmake-build-match/libgof2hdaa.so`):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

Plain no-order baseline:

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_radio/noorder_base.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_radio/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_radio/noorder_base.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_radio/noorder_base.so
```

```text
byte_exact 2856  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3463/4524 same; prefix 3373;
            first (0x15501c, '_ZN5Radio11setMessagesEP5ArrayIP12RadioMessageE', 0x155036)
```

## 2. Original order (binary witness)

`nm -nSD --defined-only` on the original gives the emitted `Radio` order:

```text
00154f7c  Radio::Radio()                                  0x58
00154fd4  Radio::~Radio()                                 0x48
0015501c  Radio::setMessages(Array<RadioMessage*>*)       0x30
0015504c  Radio::setCurrentMessage(RadioMessage*)         0x04
00155050  Radio::getMessage(int)                          0x0a
0015505a  Radio::lastMessageShown()                       0x06
00155060  Radio::update(long, PlayerEgo*, LevelScript*)   0x318
00155378  Radio::draw(long long, PlayerEgo*, LevelScript*) 0x258
001555d0  Radio::isShowingMessage()                       0x0a
```

The HD source-layout dump
(`_work/original_layout/original_layout.json`, `gof2hd_1.1.19_ios`,
`Classes/Game/Radio.cpp`) independently witnesses the same order:

```text
_Radio::Radio()
_Radio::~Radio()
_Radio::setMessages(Array<RadioMessage*>*)
_Radio::setCurrentMessage(RadioMessage*)
_Radio::getMessage(int)
_Radio::lastMessageShown()
_Radio::update(long, PlayerEgo*, LevelScript*)
_Radio::draw(long long, PlayerEgo*, LevelScript*)
_Radio::isShowingMessage()
```

`_work/original_layout/tu_mapping.md` line 122 maps our
`game/core/Radio.cpp` to the original `Classes/Game/Radio.cpp` (`exact(HD)`),
so the TU membership is not in question.

## 3. Baseline object order

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/core/Radio.cpp.obj`
(section-header order = emission order) before the change:

```text
Radio::Radio()
Radio::~Radio()
__clang_call_terminate
Array<ImagePart*>::~Array()
Array<AbyssEngine::String*>::~Array()
Radio::isShowingMessage()
Radio::lastMessageShown()
Radio::getMessage(int)
Radio::setMessages(Array<RadioMessage*>*)
Radio::setCurrentMessage(RadioMessage*)
Radio::update(long, PlayerEgo*, LevelScript*)
Array<AbyssEngine::String*>::Array()
Radio::draw(long long, PlayerEgo*, LevelScript*)
```

This is exactly the source order. The weak template copies are emitted at the
parse point of their first user (`~Radio` uses the two `Array` destructors;
`update` does `new Array<String *>()`), and are discarded at link time in
favour of the canonical copies (original `Array<String*>::Array()` at
`0x732f1`, `Array<ImagePart*>::~Array()` at `0xe0ff9`). They therefore do not
occupy addresses in the final link.

## 4. Source change

Only change: reorder the top-level definitions in
`src/game/core/Radio.cpp` to the original address order:

```text
before (source / object order):
  ctor, dtor, isShowingMessage, lastMessageShown, getMessage,
  setMessages, setCurrentMessage, update, draw

after:
  ctor, dtor, setMessages, setCurrentMessage, getMessage,
  lastMessageShown, update, draw, isShowingMessage
```

The bodies are unchanged. The move is a pure permutation: the sorted line
multiset of the file is identical before and after (`diff <(git show
HEAD:src/game/core/Radio.cpp | sort) <(sort src/game/core/Radio.cpp)` is
empty). `git diff --stat` is `12 insertions(+), 12 deletions(-)`.

After the move the object order is the original strong-function order:

```text
Radio::Radio()
Radio::~Radio()
__clang_call_terminate
Array<ImagePart*>::~Array()
Array<AbyssEngine::String*>::~Array()
Radio::setMessages(Array<RadioMessage*>*)
Radio::setCurrentMessage(RadioMessage*)
Radio::getMessage(int)
Radio::lastMessageShown()
Radio::update(long, PlayerEgo*, LevelScript*)
Array<AbyssEngine::String*>::Array()
Radio::draw(long long, PlayerEgo*, LevelScript*)
Radio::isShowingMessage()
```

## 5. Controlled A/B

Each row is a full `check` rebuild plus the plain no-order relink.

| # | source | gate byte/linked | gate `.so` sha256 | plain byte | exact addrs | prefix | first plain mismatch |
|---|---|---:|---|---:|---:|---:|---|
| 0 | baseline | **4497 / 4497** | `1ecc20f3…` | **2856** | 3463 | 3373 | `Radio::setMessages` `0x15501c` vs `0x155036` |
| 1 | full `Radio` reorder (landed) | **4497 / 4497** | `1ecc20f3…` | **2857** | **3470** | **3385** | `SolarSystem::getName()` `0x1556f0` vs `0x15571e` |

The gate `.so` is byte-identical (`cmp -l` = 0 bytes, same sha256), so
`.text`, `.rodata`, `.ARM.exidx`, `.ARM.extab`, `.plt`, `.got`, `.dynsym` and
the build-id are all unchanged by the source move. The ordering file already
encoded the original `Radio` strong order; the plain link was the only place
where the source order leaked through.

Commands (same for every row):

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_radio/noorder_final.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_radio/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_radio/noorder_final.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_radio/noorder_final.so
```

Final gate:

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

Final plain no-order:

```text
byte_exact 2857  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3470/4524 same; prefix 3385;
            first (0x1556f0, '_ZN11SolarSystem7getNameEv', 0x15571e)
```

### 5.1 Per-function byte delta

The plain verify report gains exactly one `byte=True` row and loses none:

```text
+ _ZN5Radio11setMessagesEP5ArrayIP12RadioMessageE
```

`setCurrentMessage`, `getMessage`, `lastMessageShown`, `isShowingMessage`,
the constructors and the destructor were already byte-exact in the baseline
(their bodies contain no address-dependent operand). `update` and `draw`
remain `byte=False` in the plain link for the known layout reasons, not
because of this TU's order.

### 5.2 Why `update` / `draw` still are not byte-exact in the plain link

Word-level comparison at the same addresses (original vs plain final) shows
exactly one differing 4-byte word in each function:

```text
update  0x155374  orig 0x000751c6  ours 0x000751d6  delta +0x10
draw    0x155490  orig 0xf047b862  ours 0xf047b872  delta +0x10 (imm)
```

* `0x155374` is the last literal-pool word of `update` (object offset
  `0x314`), the `R_ARM_REL32` relocation against `.L.str`. The plain link's
  `.rodata` base is `0x1c9e20` vs the original `0x1c9e10` (the known plain
  `.text +0x20` / `.ARM.exidx`/`.ARM.extab` artifact), so the REL32 value is
  `+0x10`.
* `0x155490` is a Thumb-2 `beq.w` whose target is local code at the end of
  `.text`: original `0x19c558`, plain `0x19c578` (`+0x20`, the plain `.text`
  size `0x1337e8` vs `0x1337c8`). The instruction encoding differs only in
  that branch immediate.

Both are global plain-link artifacts already documented in
`_work/reconstructions/radiomessage_ctor_placement_w3745.md` §5.2 and
`gun_setoffset_overload_order_w3745.md` §5; neither is caused by the `Radio`
ordering.

Plain-link section layout is unchanged by the pass:

```text
.text       0x1337e8 @ 0x69440
.ARM.exidx  0x006f68 @ 0x19cc28
.ARM.extab  0x02628c @ 0x1a3b90
.rodata     0x041b67 @ 0x1c9e20
```

## 6. Next frontier

The first plain no-order mismatch is now:

```text
original 0x1556f0  SolarSystem::getName()
ours     0x15571e  SolarSystem::getName()
```

This is a separate TU (`src/game/world/SolarSystem.cpp`) and was not changed
in this pass. Our plain `SolarSystem` order is
`ctor, dtor, getIndex, getRace, getSecurityLevel, getTextureIndex, getX, getY,
getZ, getWarpGateIndex, getStations, getRoutes, getForbiddenGoods, isVisible,
getName, getAttackRace, ...`; the original starts
`ctor, dtor, getIndex, getName, getSecurityLevel, getRace, getAttackRace,
hasPirateBase, ...`.

## 7. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  `.rodata` 0 differing bytes, lint `CLEAN ✓`, drift 0; gate `.so` sha256
  `1ecc20f3fbd49abce61ac8426fee08743f908610d5d0a6dbb69c1e4d6e4c050b`.
* Baseline plain no-order is `byte 2856 / linked 4497`, `3463/4524`
  addresses, prefix `3373`; first mismatch `Radio::setMessages` at `0x15501c`
  (ours `0x155036`).
* The original emits the `Radio` functions in the order listed in §2; the HD
  1.1.19 source-layout dump independently witnesses the same order, and the
  TU mapping is `exact(HD)`.
* The baseline object emits the functions in source order, which differs.
* Reordering the definitions is gate-neutral (`4497/4497`, gate `.so`
  **byte-identical**, lint `CLEAN`, drift 0) and makes all nine `Radio`
  symbols address-exact in the plain link.
* The plain link improves `2856 -> 2857` byte-exact, `3463 -> 3470` exact
  addresses, `3373 -> 3385` exact-address prefix; the per-function diff is
  1 gain / 0 losses (`setMessages`).
* `update` and `draw` remain non-byte-exact in the plain link solely because
  of the plain `.rodata +0x10` and `.text +0x20` layout artifacts; each has
  exactly one differing word, both accounted for.
* The plain link's next mismatch is `SolarSystem::getName` at `0x1556f0`
  (ours `0x15571e`).

**Hypothesis / still open**

* The plain `.text +0x20`, `.ARM.exidx -8` and `.ARM.extab -0xc` are the
  known plain-link artifacts; until the `.rodata` base is exact, any function
  with a `.rodata` REL32 literal or a local `.text` branch target will stay
  non-byte-exact even when its instruction stream and address are correct.
* The ordering file is still required: the plain link is `2857`, not `4497`.
* The next TU (`SolarSystem.cpp`) needs its own emission-order pass under the
  full gate.
