# Section-order removal: `ArrayAdd<RadioMessage*>` placement (w3745)

Worker: w3745. Branch: `wave87-w3745`, based on master `638836cb`
("docs: lock ArraySetLength placement"). Original:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b
clang 7.0.2 `-Oz`). All numbers below were produced in this worktree with the
commands shown. Scratch output is under `/tmp/w3745/radio/`.

This note closes the open item in
`_work/reconstructions/arraysetlength_placement_w3745.md` §5: the first
remaining plain (no `--section-ordering-file`) mismatch was
`Level::updateAlienAttackers`, displaced by the lazy `ArrayAdd<RadioMessage*>`
copy. Moving the `Level::createRadioMessage` definition after
`Level::checkGameOver` is gate-neutral and makes the whole
`createRadioMessage`/`updateAlienAttackers`/…/`checkGameOver`/
`ArrayAdd<RadioMessage*>`/`pirateStationAction` block address-exact in the
plain link.

## 0. Result

| build | gate byte/linked | plain byte_exact | exact addresses | exact-address prefix | first plain mismatch |
|---|---:|---:|---:|---:|---|
| before (HEAD `638836cb`) | **4497 / 4497** | **2989** | **3182 / 4524** | **2342** | `Level::updateAlienAttackers` `0xc5ca4` vs `0xc5cc8` |
| after (this pass) | **4497 / 4497** | **2992** | **3193 / 4524** | **2377** | `StarMap::~StarMap` `0xc749c` vs `0xc7cbc` |

Delta: `+3` plain byte_exact, `+11` exact addresses, `+35` exact-address
prefix. Gate unchanged (`4497/4497`, `RATCHET PASS`), `.text` `0x1337c8 @
0x69440`, `.rodata` 0 differing bytes, lint `CLEAN ✓`, drift `0`.

The entire block is now at the original addresses in the plain link:

| symbol | original | before | after |
|---|---:|---:|---:|
| `Level::createRadioMessage(int,int)` | `0xc566c` | `0xc566c` | `0xc566c` |
| `Level::updateAlienAttackers(int)` | `0xc5ca4` | `0xc5cc8` | **`0xc5ca4`** |
| `Level::checkGameOver(int)` | `0xc627e` | `0xc62a2` | **`0xc627e`** |
| `ArrayAdd<RadioMessage*>` | `0xc628a` | `0xc5ca4` | **`0xc628a`** |
| `Level::pirateStationAction(bool)` | `0xc62b0` | `0xc62b0` | `0xc62b0` |

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

The brief's `4496 / 4496` baseline is stale; this HEAD is `4497 / 4497`
(the `Level::createMission` pass is already on `master`).

Plain no-order baseline:

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/radio/noorder_base.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/radio/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/radio/noorder_base.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/radio/noorder_base.so
```

```text
byte_exact 2989  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3182/4524 same; prefix 2342;
            first (0xc5ca4, '_ZN5Level20updateAlienAttackersEi', 0xc5cc8)
```

## 2. Source-layout evidence

`_work/original_layout/original_layout.json`, iOS 1.1.19
`Classes/Game/Level.cpp`, has this symbol order (indices 68-79):

```text
68 Level::createRadioMessage(int, int)
69 Level::updateAlienAttackers(int)
70 Level::update(long long, bool)
71 Level::checkObjective(int)
72 Level::stealFriendCargo()
73 Level::friendCargoWasStolen()
74 Level::removeObjectives()
75 Level::getPlayerGuns()
76 Level::getEnemyGuns()
77 Level::checkGameOver(int)
78 void ArrayAdd<RadioMessage*>(RadioMessage*, Array<RadioMessage*>&)
79 Level::pirateStationAction(bool)
```

Two facts follow directly:

* The strong functions are in the original address order; our plain link already
  matches them for this block.
* The original puts the `ArrayAdd<RadioMessage*>` copy **after** `checkGameOver`
  (`0xc627e` → `0xc628a`), not immediately after its first user
  `createRadioMessage` (`0xc566c`).

## 3. Mechanism

The `Level.cpp.obj` `.text.*` section order is the order in which clang
materialises the LLVM `Function` objects (see
`_work/reconstructions/deferred_instantiation_flush_w3745.md` §2):

* a **strong** function is materialised at its first *reference* (usually a call
  in an earlier body), which is why `createRadioMessage` stays at `0xc566c`
  even when its definition is moved;
* an **implicit template copy** such as `ArrayAdd<RadioMessage*>` is
  materialised when the body of its first user is *parsed*, i.e. at the
  definition position of `createRadioMessage` in the `.cpp`, not at
  `createRadioMessage`'s own first-use position.

Baseline `Level.cpp.obj` order (relevant region):

```text
createRadioMessage
ArrayAdd<RadioMessage*>
updateAlienAttackers
...
checkGameOver
pirateStationAction
```

After the move:

```text
createRadioMessage
updateAlienAttackers
...
checkGameOver
ArrayAdd<RadioMessage*>
pirateStationAction
```

`createRadioMessage` is the only user of `ArrayAdd<RadioMessage*>` in
`Level.cpp` (all `ArrayAdd(new RadioMessage(...), *this->messages)` calls are in
its body). `Array<RadioMessage*>::Array()` and
`ArraySetLength<RadioMessage*>` are materialised earlier by
`createRadioMessages` (plural, `0xc0de4`), so they do not move.

### 3.1 Controlled proof: definition at end of file

A scratch variant moved the whole `createRadioMessage` definition to the end of
`Level.cpp` (before the static member definitions) and compiled it with the
exact `compile_commands.json` command. The strong function stayed at object
position 98, but `ArrayAdd<RadioMessage*>` moved from position 99 to position
128 (the end of the object). This proves the copy follows the definition parse
point, not the first-use point.

```sh
RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RE -SW /tmp/w3745/radio/Level_end.o \
  | awk '$2 ~ /^\.text\./ {print $2}' | sed 's/^\.text\.//' | grep -n "" \
  | grep -E 'createRadioMessage|ArrayAddIP12|updateAlien|checkGameOver|pirateStation'
```

```text
98:_ZN5Level18createRadioMessageEii
99:_ZN5Level20updateAlienAttackersEi
...
108:_ZN5Level13checkGameOverEi
109:_ZN5Level19pirateStationActionEb
128:_Z8ArrayAddIP12RadioMessageEvT_R5ArrayIS2_E
```

## 4. Source change

Only change: move the `Level::createRadioMessage(int, int)` definition from
immediately after `Level::createScene()` to immediately after
`Level::checkGameOver()` (before `Level::pirateStationAction()`). The body is
unchanged; a comment at the new definition records why.

```diff
@@ after Level::createScene()
-void Level::createRadioMessage(int type, int sub) {
-    ... 225 lines ...
-}
-
 void Level::updateAlienAttackers(int dt) {
 ...
 bool Level::checkGameOver(int param) {
 ...
 }
 
+// Defined after checkGameOver on purpose: clang materialises the implicit
+// ArrayAdd<RadioMessage*> copy when this body is parsed, and the original
+// Level.o places it after checkGameOver (0xc628a). ...
+void Level::createRadioMessage(int type, int sub) {
+    ... unchanged body ...
+}
+
 void Level::pirateStationAction(bool param) {
```

The move is gate-neutral. `cmake-build-match/section_order.txt` is unchanged
(the ordering file is generated from the original address order for exported
symbols), so the gate `.text` is byte-identical before/after; only the
`.note.gnu.build-id` and the `.ARM.exidx`/`.ARM.extab` record order for this
block differ.

## 5. Controlled A/B

Each row is a full `check` rebuild plus the plain no-order relink.

| # | source | gate byte/linked | plain byte | exact addrs | prefix | first plain mismatch |
|---|---|---:|---:|---:|---:|---|
| 0 | baseline | **4497 / 4497** | **2989** | 3182 | 2342 | `updateAlienAttackers` `0xc5ca4` vs `0xc5cc8` |
| 1 | `createRadioMessage` after `checkGameOver` | **4497 / 4497** | **2992** | **3193** | **2377** | `StarMap::~StarMap` `0xc749c` vs `0xc7cbc` |
| 2 | `createRadioMessage` at end of file (scratch) | not built | — | — | — | object order only: `ArrayAdd` at position 128 |

Commands (same for every row):

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/radio/noorder_final.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/radio/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/radio/noorder_final.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/radio/noorder_final.so
```

Final gate:

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Final plain no-order:

```text
byte_exact 2992  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3193/4524 same; prefix 2377;
            first (0xc749c, '_ZN7StarMapD1Ev', 0xc7cbc)
```

Final `nm` for the block:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nD --defined-only /tmp/w3745/radio/noorder_final.so | \
  awk '$1>="000c5600" && $1<="000c6400"'
```

```text
000c566c T _ZN5Level18createRadioMessageEii
000c5ca4 T _ZN5Level20updateAlienAttackersEi
000c5f54 T _ZN5Level21updateAsteroidClusterEv
000c5f58 T _ZN5Level6updateExb
000c6250 T _ZN5Level14checkObjectiveEi
000c625c T _ZN5Level16stealFriendCargoEv
000c6264 T _ZN5Level20friendCargoWasStolenEv
000c626a T _ZN5Level16removeObjectivesEv
000c6272 T _ZN5Level13getPlayerGunsEv
000c6278 T _ZN5Level12getEnemyGunsEv
000c627e T _ZN5Level13checkGameOverEi
000c628a W _Z8ArrayAddIP12RadioMessageEvT_R5ArrayIS2_E
000c62b0 T _ZN5Level19pirateStationActionEb
```

These are the original addresses (same `nm` on the original).

### 5.1 `.ARM.exidx` / `.ARM.extab` side effect

The move changes the `.ARM.exidx`/`.ARM.extab` record order for this block
(the linker emits those sections in input-section order, and the ordering file
only lists `.text.*`). The `.ARM.extab` LSDA addresses for the block become a
uniform `-4` from the original instead of the baseline's mixed order:

| function | original LSDA | baseline | after |
|---|---:|---:|---:|
| `updateAlienAttackers` | `0x1ac3bc` | `0x1ac3c4` | `0x1ac3b8` |
| `update` | `0x1ac3cc` | `0x1ac3d4` | `0x1ac3c8` |
| `ArrayAdd<RadioMessage*>` | `0x1ac3dc` | `0x1ac3b8` | `0x1ac3d8` |
| `pirateStationAction` | `0x1ac3e8` | `0x1ac3e4` | `0x1ac3e4` |

The relative order is now the original's; the whole block is 4 bytes early
(the known `.ARM.extab` `-0xc` residue is elsewhere). The raw `.ARM.extab`
byte-diff against the original rises by 4 (`113,767 → 113,771`) only because
the comparison alignment changed; `.text` is identical (`112,149` differing
bytes in both), `.ARM.exidx` is unchanged (`1,749`), and `.rodata` remains
`0` differing bytes.

## 6. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  `.rodata` 0 differing bytes, lint `CLEAN`, drift 0.
* Baseline plain no-order is `byte 2989 / linked 4497`, `3182/4524` addresses,
  prefix `2342`; first mismatch `Level::updateAlienAttackers` at `0xc5ca4`
  (ours `0xc5cc8`).
* The original iOS 1.1.19 `Level.cpp` symbol order places
  `ArrayAdd<RadioMessage*>` after `checkGameOver`, not after
  `createRadioMessage`.
* Moving the `createRadioMessage` definition after `checkGameOver` is
  gate-neutral (`4497/4497`, `.text` unchanged, `.rodata` unchanged, lint
  `CLEAN`, drift 0) and improves the plain no-order link to `2992 / 4497`,
  `3193/4524` addresses, prefix `2377`.
* The whole `createRadioMessage`/…/`checkGameOver`/`ArrayAdd<RadioMessage*>`/
  `pirateStationAction` block is now at the original addresses in the plain
  link.
* The template copy follows the definition parse point, not the strong
  function's first-use point: moving the definition to the end of the file
  leaves `createRadioMessage` at object position 98 but moves
  `ArrayAdd<RadioMessage*>` to position 128 (the end).
* The `.ARM.extab` LSDA order for the block becomes the original's relative
  order (uniform `-4` instead of the baseline's mixed order).

**Hypothesis / still open**

* The next plain no-order mismatch is `StarMap::~StarMap`
  (`0xc749c` original, `0xc7cbc` ours). It is a separate `StarMap.cpp`
  object-order issue: our `StarMap.o` emits `initStarSystem` before the
  destructor, while the original emits `StarMap::~StarMap` immediately after
  `StarMap::init` (`0xc749c`) and `initStarSystem` at `0xc7580`. The 4-byte
  gap after `StarMap::init` in our object is also unexplained. Not fixed here.
* The ordering file is still required: the plain link is `2992`, not `4497`.
