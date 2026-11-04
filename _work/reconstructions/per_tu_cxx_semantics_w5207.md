# Per-TU C++ semantic flags on all app TUs (w5207)

Status: research note (2026-11-04). Worktree `/home/debian/GalaxyOnFire2/wt/w5207`,
branch `wave104-w5207`, base commit `8c5fd3646` ("Hud::draw pass 2: dock posY top*2 fix
+ extab-exact BHTWPD order"). No `src/`, `CMakeLists.txt`, tool, baseline or allowlist
change landed: this commit contains this note only. The canonical tree is unchanged and
still green.

## 0. Bottom line

* 192 app TUs x 21 C++ semantic flags = **4,032 per-TU compilations**.
* 721 of those (flag, TU) pairs produce a byte-different object; every one was linked
  into a complete `libgof2hdaa.so` and measured with the verifier's own raw-byte rule.
* **No flag on any TU raises `byte_exact` above the 4500 baseline.** 15 pairs are
  gate-neutral (full `verify` confirms `byte_exact 4500`, `linked_exact 4500`); the
  other 706 pairs are strictly worse.
* Applying each flag globally: **11 flags produce a byte-identical `.so`** (they are
  no-ops on this source), **9 are worse**, and **`-fno-common` fails to link**
  (`multiple definition of 'g_android_back_button_pressed'`).
* **No strict improvement exists** in the tested set. The current global flag set is
  confirmed; no build/source change is warranted.

## 1. Baseline

Configured and built in this worktree (the nested-worktree original/FMOD paths are
required, as documented in `AGENTS.md`):

```sh
mkdir -p /dev/shm/w5207/tmp
cd /home/debian/GalaxyOnFire2/wt/w5207
TMPDIR=/dev/shm/w5207/tmp cmake --preset default
TMPDIR=/dev/shm/w5207/tmp cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5207/tmp cmake --build cmake-build-match --target check -j6
```

`check` output:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`report.json`: `compared 4524`, `size_exact 4524/4524`, `byte_exact 4500`,
`linked_exact 4500`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`.
Lint is `CLEAN`; drift is `0`.

Artifacts:

| artifact | sha256 |
|---|---|
| `cmake-build-match/libgof2hdaa.so` | `5711617bd54ca23932eaec084683e6ab530283c51dfa501b908569a212bdc022` |
| ground truth `android_2.0.16_libgof2hdaa.so` | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` |

Baseline section sizes (all equal to the original; `.data.rel.ro` included):

```text
.text=0x1337c8  .rodata=0x41b67  .data=0x296c  .data.rel.ro=0x2fe8  .bss=0x69ac
.init_array=0xc  .fini_array=0x8  .dynsym=0x15ca0  .dynstr=0x2e318  .hash=0x973c
.rel.dyn=0x7048  .rel.plt=0x72d0  .got=0x430c  .ARM.exidx=0x6f70  .ARM.extab=0x26298
```

The baseline's `.init_array`, `.fini_array`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`,
`.rel.plt`, `.data.rel.ro`, `.rodata` and `.data` are byte-identical to the original
(section hashes below). This is the static-init/typeinfo/vtable reference state.

## 2. Method

The 192 app TUs are exactly the `src/` entries of
`cmake-build-match/compile_commands.json` (the `gof2` object library; the vendored
libcrypto/zlib/libzip/libc++abi/libgcc objects are not app TUs and were not varied).

The 21 flags tested:

```text
-fno-threadsafe-statics -fno-use-cxa-atexit -fno-sized-deallocation
-faligned-allocation -fno-aligned-allocation -fno-common -fcommon
-fno-strict-aliasing -fwrapv -fno-strict-overflow
-fno-delete-null-pointer-checks -fno-assume-sane-operator-new
-fno-operator-names -fno-access-control -fno-gnu-inline-asm
-fno-delayed-template-parsing -fno-elide-constructors
-fvisibility=default -fvisibility=hidden -fvisibility=protected
-fvisibility-inlines-hidden
```

Three measurements were made.

1. **Object screen (all 192 TUs x 21 flags).** Each TU's exact compile command from
   `compile_commands.json` was re-run with the candidate flag inserted immediately after
   the compiler executable, writing to `/dev/shm/w5207/objs/<flag>/<tu>.obj`. The object
   was compared to the canonical object section-by-section (`.text*`, `.rodata*`,
   `.data*`, `.bss*`, `.symtab`, `.rel*`, `.ARM.exidx`, `.ARM.extab`, debug sections).
   A byte-identical object proves the flag is a no-op for that TU (the linked `.so`
   cannot change).
2. **Per-TU link matrix.** For every object-differing (flag, TU) pair, the canonical gold
   link command was taken from `ninja -C cmake-build-match -t commands link_so` and the
   one baseline object was replaced by the variant object; the output was written to
   `/dev/shm/w5207/so/<flag>/<tu>.so`. `byte_exact` was then computed with the verifier's
   own rule (`tools/verify/elf.py` + raw `bytes_at(value, size)` equality, with the same
   size-0 exclusions as `verify.py`). For every pair that came out at `byte_exact 4500`,
   the full official `tools/verify/verify.py` was run to obtain `linked_exact`.
3. **Global matrix.** For each flag, *all* app objects were replaced by that flag's
   objects and the whole `.so` was relinked; `byte_exact` and the `.so` sha256 were
   measured. A sha equal to the baseline sha proves the flag is globally a no-op.

Per-TU counts for the 16 TUs that contain the 24 non-byte-exact functions were also
computed with `verify.py`'s `disassemble`/`reconcile` on just that TU's functions, so a
local improvement cannot hide behind a global regression.

The full harness is reproduced in the appendix.

## 3. Raw results

### 3.1 Object screen: TUs whose object changes

| flag | object-differing TUs (of 192) |
|---|---:|
| `-fno-threadsafe-statics` | 1 (`engine/core/GameText.cpp`) |
| `-fno-use-cxa-atexit` | 3 (`engine/core/GameText.cpp`, `engine/render/Engine.cpp`, `engine/render/ParticleSettings.cpp`) |
| `-fno-sized-deallocation` | 0 |
| `-faligned-allocation` | 0 |
| `-fno-aligned-allocation` | 0 |
| `-fno-common` | 4 (`platform/android/BackButton.c`, `engine/file/BlowfishSubkeyFile.c`, `platform/android/GOF2HD2012.c`, `platform/android/main.c`) |
| `-fcommon` | 0 |
| `-fno-strict-aliasing` | 65 |
| `-fwrapv` | 29 |
| `-fno-strict-overflow` | 29 |
| `-fno-delete-null-pointer-checks` | 0 |
| `-fno-assume-sane-operator-new` | 0 |
| `-fno-operator-names` | 0 |
| `-fno-access-control` | 0 |
| `-fno-gnu-inline-asm` | 0 |
| `-fno-delayed-template-parsing` | 0 |
| `-fno-elide-constructors` | 94 |
| `-fvisibility=default` | 0 |
| `-fvisibility=hidden` | 192 |
| `-fvisibility=protected` | 192 |
| `-fvisibility-inlines-hidden` | 112 |

Total object-differing pairs: **721**. All 721 linked successfully.

### 3.2 Per-TU and global gate results

`global byte` is the whole-`.so` `byte_exact` when the flag is applied to every app TU.
`per-TU best` is the best `byte_exact` over the one-TU-at-a-time variants (4500 when the
flag is a no-op for every TU, because the baseline is then unchanged). `==4500` / `<4500`
/ `>4500` count the one-TU variants.

| flag | obj-diff TUs | global byte | global `.so` == baseline | per-TU pairs | per-TU best | best TU | ==4500 | <4500 | >4500 |
|---|---:|---:|---|---:|---:|---|---:|---:|---:|
| `-fno-threadsafe-statics` | 1 | 1368 / 4521 | no | 1 | 1368 | `engine/core/GameText.cpp` | 0 | 1 | 0 |
| `-fno-use-cxa-atexit` | 3 | 1356 / 4524 | no | 3 | 1579 | `engine/core/GameText.cpp` | 0 | 3 | 0 |
| `-fno-sized-deallocation` | 0 | 4500 / 4524 | yes | 0 | 4500 | — | 0 | 0 | 0 |
| `-faligned-allocation` | 0 | 4500 / 4524 | yes | 0 | 4500 | — | 0 | 0 | 0 |
| `-fno-aligned-allocation` | 0 | 4500 / 4524 | yes | 0 | 4500 | — | 0 | 0 | 0 |
| `-fno-common` | 4 | **LINKFAIL** | — | 4 | 4487 | `platform/android/BackButton.c` | 0 | 4 | 0 |
| `-fcommon` | 0 | 4500 / 4524 | yes | 0 | 4500 | — | 0 | 0 | 0 |
| `-fno-strict-aliasing` | 65 | 1371 / 4524 | no | 65 | 4500 | `engine/render/SpriteSystem.cpp` | 3 | 62 | 0 |
| `-fwrapv` | 29 | 1460 / 4524 | no | 29 | 4500 | `game/ui/StatusWindow.cpp` | 1 | 28 | 0 |
| `-fno-strict-overflow` | 29 | 1460 / 4524 | no | 29 | 4500 | `game/ui/StatusWindow.cpp` | 1 | 28 | 0 |
| `-fno-delete-null-pointer-checks` | 0 | 4500 / 4524 | yes | 0 | 4500 | — | 0 | 0 | 0 |
| `-fno-assume-sane-operator-new` | 0 | 4500 / 4524 | yes | 0 | 4500 | — | 0 | 0 | 0 |
| `-fno-operator-names` | 0 | 4500 / 4524 | yes | 0 | 4500 | — | 0 | 0 | 0 |
| `-fno-access-control` | 0 | 4500 / 4524 | yes | 0 | 4500 | — | 0 | 0 | 0 |
| `-fno-gnu-inline-asm` | 0 | 4500 / 4524 | yes | 0 | 4500 | — | 0 | 0 | 0 |
| `-fno-delayed-template-parsing` | 0 | 4500 / 4524 | yes | 0 | 4500 | — | 0 | 0 | 0 |
| `-fno-elide-constructors` | 94 | 1370 / 4524 | no | 94 | 4500 | `engine/math/Vector.cpp` | 9 | 85 | 0 |
| `-fvisibility=default` | 0 | 4500 / 4524 | yes | 0 | 4500 | — | 0 | 0 | 0 |
| `-fvisibility=hidden` | 192 | 57 / 244 | no | 192 | 2490 | `engine/render/MovingStars.cpp` | 0 | 192 | 0 |
| `-fvisibility=protected` | 192 | 1345 / 4524 | no | 192 | 4500 | `engine/render/Curve.cpp` | 1 | 191 | 0 |
| `-fvisibility-inlines-hidden` | 112 | 1278 / 4383 | no | 112 | 2845 | `engine/render/IParticleSystem.cpp` | 0 | 112 | 0 |

The `-fno-common` global link failure is real and diagnostic:

```text
ld.gold: error: .../fnocommon/platform__android__GOF2HD2012.c.obj:
  multiple definition of 'g_android_back_button_pressed'
ld.gold: .../fnocommon/platform__android__BackButton.c.obj: previous definition here
```

Both C TUs have a tentative definition of the 4-byte object. Under clang 7's default
`-fcommon` they merge; `-fno-common` makes both strong `.bss` definitions.

### 3.3 Full `verify` on the 15 gate-neutral one-TU variants

All 15 returned exactly the baseline gate:

```text
byte 4500  linked 4500  avg 99.9  (compared 4524)
```

| flag | TU |
|---|---|
| `-fno-strict-aliasing` | `engine/render/SpriteSystem.cpp` |
| `-fno-strict-aliasing` | `engine/core/Array.cpp` |
| `-fno-strict-aliasing` | `engine/core/ArrayInstantiations.cpp` |
| `-fwrapv` | `game/ui/StatusWindow.cpp` |
| `-fno-strict-overflow` | `game/ui/StatusWindow.cpp` |
| `-fno-elide-constructors` | `engine/math/Vector.cpp` |
| `-fno-elide-constructors` | `engine/render/SpriteSystem.cpp` |
| `-fno-elide-constructors` | `engine/math/Quaternion.cpp` |
| `-fno-elide-constructors` | `platform/android/NdkHooks.cpp` |
| `-fno-elide-constructors` | `game/ship/PlayerWormHole.cpp` |
| `-fno-elide-constructors` | `game/ship/PlayerJumpgate.cpp` |
| `-fno-elide-constructors` | `game/ship/PlayerCreature.cpp` |
| `-fno-elide-constructors` | `game/world/Waypoint.cpp` |
| `-fno-elide-constructors` | `game/ship/PlayerJunk.cpp` |
| `-fvisibility=protected` | `engine/render/Curve.cpp` |

These are neutral for three distinct reasons, all confirmed by section diffs:

* **Weak-definition masking.** `Array.cpp`'s `ArrayRemove<Mesh/String/Transform>` and
  `ArrayInstantiations.cpp`'s `ArrayRemove<Node/Agent>` change in the object, but the
  same weak template instantiations are also emitted by `PaintCanvas.cpp` (and other
  TUs), which appears earlier in `tools/source_order.txt`; gold keeps the first
  definition, so the linked bytes do not move. `SpriteSystem.cpp`'s
  `ArrayAddCached<Mesh>` is likewise masked.
* **Debug/relocation-only changes.** `StatusWindow.cpp` (`-fwrapv`,
  `-fno-strict-overflow`), `Quaternion.cpp`, `SpriteSystem.cpp`, `NdkHooks.cpp`,
  `PlayerJunk.cpp` (`-fno-elide-constructors`) change only `.debug_*`; `Vector.cpp`,
  `PlayerWormHole.cpp`, `PlayerJumpgate.cpp`, `PlayerCreature.cpp`, `Waypoint.cpp`
  change only `.rel.text`/`.rel.ARM.exidx`/`.rel.data.rel.ro`, not the exported
  function bytes.
* **Symbol-table-only change.** `Curve.cpp` (`-fvisibility=protected`) changes only
  `.symtab`; its exported code and data are unchanged.

### 3.4 Per-TU counts for the 16 TUs containing non-byte-exact functions

Baseline (from `report.json`; the same 24 functions are the current non-byte set):

| TU | compared | byte | linked | non-byte functions |
|---|---:|---:|---:|---|
| `game/core/Globals.cpp` | 41 | 39 | 39 | `Globals::Globals` C1/C2 (80.5%) |
| `game/menu/MGame.cpp` | 45 | 43 | 43 | `MGame::OnTouchEnd` (74.8%), `MGame::OnUpdate` (99.9%) |
| `game/menu/ModStation.cpp` | 40 | 38 | 38 | `ModStation::ModStation` C1/C2 (98.4%) |
| `game/menu/SpaceLounge.cpp` | 34 | 33 | 33 | `SpaceLounge::drawLounge` (95.5%) |
| `game/mission/Generator.cpp` | 15 | 14 | 14 | `Generator::createMission` (64.4%) |
| `game/mission/Status.cpp` | 131 | 130 | 130 | `Status::missionCompleted` (60.4%) |
| `game/ship/PlayerEgo.cpp` | 174 | 173 | 173 | `PlayerEgo::calcCollision` (87.6%) |
| `game/ui/HangarWindow.cpp` | 32 | 31 | 31 | `HangarWindow::render` (80.2%) |
| `game/ui/Hud.cpp` | 54 | 53 | 53 | `Hud::draw` (92.1%) |
| `game/ui/Layout.cpp` | 50 | 48 | 48 | `Layout::Layout` C1/C2 (47.8%) |
| `game/ui/MenuTouchWindow.cpp` | 41 | 38 | 38 | `MenuTouchWindow::MenuTouchWindow` C1/C2 (95.1%), `OnTouchBegin` (97.7%) |
| `game/weapons/Gun.cpp` | 28 | 27 | 27 | `Gun::calcCharacterCollision` (90.0%) |
| `game/weapons/Radar.cpp` | 19 | 18 | 18 | `Radar::draw` (73.9%) |
| `game/weapons/RocketGun.cpp` | 13 | 12 | 12 | `RocketGun::update` (86.9%) |
| `game/world/Level.cpp` | 107 | 104 | 104 | `Level::createShip` (66.0%), `createScene` (88.3%), `createMission` (92.0%) |
| `game/world/LevelScript.cpp` | 17 | 16 | 16 | `LevelScript::process` (53.4%) |

For every one of these 16 TUs, no tested flag improved either its per-TU `byte` or
`linked` count. The only neutral target-TU result was `Generator.cpp` under
`-fno-strict-aliasing` (14/15 byte, 14/15 linked, unchanged); every other target-TU
variant lost at least one function. There were **0 IMPROVE rows** in the per-TU table.

### 3.5 Static initialisers

The original `.init_array` is 12 bytes, three entries:

```text
original: 41 94 06 00  c1 94 06 00  dd 94 06 00   ->  0x69441, 0x694c1, 0x694dd
baseline: 41 94 06 00  c1 94 06 00  dd 94 06 00   ->  identical
sha256(.init_array) = 79ad50ff2cbe176c...  (original and baseline)
```

The three init functions are byte-identical to the original at those addresses
(`init1` at 0x69440, touch buffer at 0x694c0, ParticleSettings at 0x694dc). The third
init calls `__cxa_atexit@plt` at `0x69500`; the original imports `__cxa_atexit` (not
`atexit`).

`-fno-use-cxa-atexit` changes the init sequence and sizes:

| variant | `.init_array` entries | `.text` | `.ARM.exidx` | `.rel.dyn` | `.got` |
|---|---|---|---:|---:|---:|---:|
| baseline | `0x69441, 0x694c1, 0x694dd` | `0x1337c8` | `0x6f70` | `0x7048` | `0x430c` |
| `-fno-use-cxa-atexit` Engine.cpp | `0x69471, 0x694cd, 0x694e9` | `0x1337e8` | `0x6f78` | `0x7040` | `0x4308` |
| `-fno-use-cxa-atexit` ParticleSettings.cpp | `0x69461, 0x694e1, 0x6951d` | `0x1337f8` | `0x6f78` | `0x7040` | `0x430c` |

The variant changes the init sequence (tail-call target/arguments) and all three
`.init_array` entries. The original's third init calls `__cxa_atexit@plt` directly at
`0x69500`, and the original imports `__cxa_atexit`; the baseline matches. The original
therefore uses `__cxa_atexit` (baseline).

### 3.6 Typeinfo / vtables / visibility

Baseline and original are byte-identical in every relevant section:

| section | original sha256 (first 12) | baseline sha256 (first 12) |
|---|---|---|
| `.dynsym` | `5a3b59e8436c` | `5a3b59e8436c` |
| `.dynstr` | `a42e036792f6` | `a42e036792f6` |
| `.hash` | `fdc6fe8d74f1` | `fdc6fe8d74f1` |
| `.rel.dyn` | `4f55867c1547` | `4f55867c1547` |
| `.rel.plt` | `43bf7c4eb170` | `43bf7c4eb170` |
| `.data.rel.ro` | `34ad551db3f4` | `34ad551db3f4` |
| `.rodata` | `73c737319958` | `73c737319958` |
| `.data` | `bf3a87118411` | `bf3a87118411` |
| `.init_array` | `79ad50ff2cbe` | `79ad50ff2cbe` |

The original exports **451** `_ZTI*`/`_ZTV*`/`_ZTS*` symbols, all with
`st_other` visibility **DEFAULT (0)**; the baseline matches exactly. The per-TU
visibility variants show what the original does *not* do:

| variant (Level.cpp) | `.dynsym` | `.dynstr` | `.data.rel.ro` | effect on Level's symbols |
|---|---:|---:|---|---|
| baseline | `0x15ca0` | `0x2e318` | `34ad551db3f4` | 451 typeinfo/vtable symbols, all DEFAULT |
| `-fvisibility=hidden` | `0x15430` | `0x2d202` | `2e2a83231fd4` | 135 symbols disappear from `.dynsym` |
| `-fvisibility=protected` | `0x15ca0` | `0x2e318` | `177f5bb5fca9` | 135 symbols change DEFAULT -> PROTECTED (3) |
| `-fvisibility-inlines-hidden` | `0x15c90` | `0x2e2f5` | `62bef8f9572b` | 1 symbol disappears |

`-fvisibility=default` is object-identical for all 192 TUs and the global `.so` sha is
the baseline sha, so the original's default visibility is confirmed.

### 3.7 Thread-safe statics

`-fno-threadsafe-statics` changes only `engine/core/GameText.cpp`. The original has the
`__cxa_guard_acquire`/`release`/`abort` PLT entries and the four calls at
`0x81844`, `0x81882`, `0x818b8`, `0x81904`; the baseline is identical. The variant
removes the guard PLT entries and changes `.text` `0x1337c8 -> 0x133684` (-0x144),
`.rodata` `0x41b67 -> 0x419b7` (-0x1b0), `.dynsym` `0x15ca0 -> 0x15c50`, and
`.dynstr` `0x2e318 -> 0x2e2b5`. The original therefore uses thread-safe statics.

## 4. Proven / disproven / unobservable

### Positively proven (hard evidence)

1. **The original uses default symbol visibility.** `-fvisibility=default` is a global
   no-op (`.so` sha identical); `hidden` drops to 57 byte-exact and deletes 135 Level
   symbols from `.dynsym`; `protected` changes 135 visibility bits and drops to 1345;
   `inlines-hidden` drops to 1278 and deletes 1 symbol.
2. **The original uses thread-safe statics.** The four `__cxa_guard_*` calls and their
   PLT entries are present in the original and baseline; `-fno-threadsafe-statics`
   removes them and drops `byte_exact` to 1368.
3. **The original uses `__cxa_atexit`.** The original `.init_array` is exactly the
   baseline's three entries, and the third init calls `__cxa_atexit@plt`; the original
   imports `__cxa_atexit`. `-fno-use-cxa-atexit` changes all three entries and the init
   code and drops `byte_exact` to 1356.
4. **The original is compiled with `-fcommon` (or clang 7's default, which is
   `-fcommon`).** `-fcommon` is a global no-op (`.so` sha identical); `-fno-common`
   makes `g_android_back_button_pressed` a duplicate strong definition in two C TUs and
   the link fails. Per-TU `-fno-common` drops `byte_exact` to 4482-4487.
5. **The original uses strict aliasing.** `-fno-strict-aliasing` changes 65 TUs and
   drops global `byte_exact` to 1371.
6. **The original does not use `-fwrapv` or `-fno-strict-overflow`.** Both change 29 TUs
   and drop global `byte_exact` to 1460 (they produce the same `.so` sha on this source).
7. **The original elides constructors.** `-fno-elide-constructors` changes 94 TUs and
   drops global `byte_exact` to 1370.
8. **No per-TU flag improves a target TU.** All 16 TUs containing the 24 non-byte
   functions were measured function-by-function; no flag improved any of them.
9. **The baseline static-init, typeinfo/vtable and visibility state is exact.** The
   relevant sections (`.init_array`, `.data.rel.ro`, `.dynsym`, `.dynstr`, `.hash`,
   `.rel.dyn`, `.rel.plt`, `.rodata`, `.data`) are byte-identical to the original.

### Disproven

* Every one of the 21 flags as a per-TU improvement (0/721 pairs raised `byte_exact`
  above 4500; 15 were neutral and 706 were worse).
* Every one of the 21 flags as a global improvement (11 no-ops, 9 worse, 1 link
  failure).
* Any claim that a per-TU `-fno-strict-aliasing`, `-fwrapv`, `-fno-strict-overflow`,
  `-fno-elide-constructors`, `-fvisibility=*` or `-fno-threadsafe-statics` is needed
  for a currently-failing function. None moves a target TU.

### Unobservable / still unknown

* **`-fno-sized-deallocation`, `-faligned-allocation`, `-fno-aligned-allocation`,
  `-fno-delete-null-pointer-checks`, `-fno-assume-sane-operator-new`,
  `-fno-operator-names`, `-fno-access-control`, `-fno-gnu-inline-asm`,
  `-fno-delayed-template-parsing`, `-fvisibility=default`** are no-ops on this source:
  all 192 objects are byte-identical and the global `.so` sha equals the baseline. The
  original may or may not have passed them; the binary cannot distinguish.
* **`-fcommon` vs the clang 7 default.** The baseline is `-fcommon`; whether the
  original build script passed it explicitly or relied on the compiler default is
  unobservable.
* **Flags outside the tested set, flag combinations, and per-TU flags on the vendored
  archives** (libcrypto, zlib, libzip, libc++abi, libgcc) were not varied. In
  particular, the vendored libraries are built by `third_party/match_libs.cmake` with
  their own flags and are outside the app-TU question.
* The original build script is not in the APK; all claims are codegen inferences.

## 5. Confidence table

| claim | confidence | hard evidence |
|---|---|---|
| No tested per-TU flag strictly improves the gate | **very high** | 4,032 compilations; 721 object-diff pairs all linked; 15 neutral pairs full-verified at 4500/4500; 706 pairs byte < 4500; 0 pairs > 4500 |
| Original uses default visibility | **very high** | `-fvisibility=default` global `.so` sha == baseline; hidden 57/244, protected 1345/4524, inlines-hidden 1278/4383; `.dynsym`/`.data.rel.ro` byte-identical |
| Original uses thread-safe statics | **very high** | original and baseline have the same four `__cxa_guard_*` calls and PLT entries; `-fno-threadsafe-statics` removes them and drops to 1368 |
| Original uses `__cxa_atexit` | **very high** | `.init_array` bytes and all three init functions byte-identical; third init calls `__cxa_atexit@plt`; variant changes all three entries |
| Original uses `-fcommon` | **very high** | `-fcommon` global `.so` sha == baseline; `-fno-common` global link fails on duplicate `g_android_back_button_pressed`; per-TU variants drop to 4482-4487 |
| Original uses strict aliasing | **high** | `-fno-strict-aliasing` changes 65 TUs and drops to 1371 |
| Original does not use `-fwrapv` / `-fno-strict-overflow` | **high** | both change 29 TUs and drop to 1460; they are identical on this source |
| Original elides constructors | **high** | `-fno-elide-constructors` changes 94 TUs and drops to 1370 |
| The 10 no-op flags are unobservable | **high** | all 192 objects byte-identical; global `.so` sha identical |
| Vendored-archive flags are outside this study | **certain** | only `src/` TUs varied; archives built by `third_party/match_libs.cmake` |

## 6. Strict improvement?

**No.** No tested per-TU flag and no tested global flag raises `byte_exact` above
**4500** or improves `linked_exact` without a `byte_exact` loss. The minimal change is
therefore **no change**: the canonical `-Oz` flag set, default visibility, thread-safe
statics, `__cxa_atexit`, `-fcommon`, strict aliasing, constructor elision and default
signed-overflow semantics are all confirmed. All experiments were reverted; the only
committed artifact is this note. `git status` is clean.

## Appendix: reproduction harness

All scripts below are self-contained and were run from the repo root with
`TMPDIR=/dev/shm/w5207/tmp`. They write only under `/dev/shm/w5207`.

### A.1 Object screen (`screen.py`)

```python
import json, shlex, subprocess, hashlib, os, struct, concurrent.futures, time

REPO='/home/debian/GalaxyOnFire2/wt/w5207'
OUT='/dev/shm/w5207/objs'
FLAGS=[
 '-fno-threadsafe-statics','-fno-use-cxa-atexit','-fno-sized-deallocation',
 '-faligned-allocation','-fno-aligned-allocation','-fno-common','-fcommon',
 '-fno-strict-aliasing','-fwrapv','-fno-strict-overflow',
 '-fno-delete-null-pointer-checks','-fno-assume-sane-operator-new',
 '-fno-operator-names','-fno-access-control','-fno-gnu-inline-asm',
 '-fno-delayed-template-parsing','-fno-elide-constructors',
 '-fvisibility=default','-fvisibility=hidden','-fvisibility=protected',
 '-fvisibility-inlines-hidden',
]

def parse_sections(path):
    d=open(path,'rb').read()
    e_shoff=struct.unpack_from('<I',d,0x20)[0]
    e_shentsize=struct.unpack_from('<H',d,0x2e)[0]
    e_shnum=struct.unpack_from('<H',d,0x30)[0]
    e_shstrndx=struct.unpack_from('<H',d,0x32)[0]
    secs=[]
    for i in range(e_shnum):
        off=e_shoff+i*e_shentsize
        name,typ,flags,addr,offset,size,link,info,align,entsize=struct.unpack_from('<IIIIIIIIII',d,off)
        secs.append(dict(nameoff=name,typ=typ,offset=offset,size=size))
    strtab=secs[e_shstrndx]
    sd=d[strtab['offset']:strtab['offset']+strtab['size']]
    out={}
    for s in secs:
        n=sd[s['nameoff']:].split(b'\0',1)[0].decode()
        out[n]=b'' if s['typ']==8 else d[s['offset']:s['offset']+s['size']]
    return out

cc=json.load(open(f'{REPO}/cmake-build-match/compile_commands.json'))
cc=[e for e in cc if '/src/' in e['file']]
base_paths={e['file']:os.path.join(e['directory'],shlex.split(e['command'])[shlex.split(e['command']).index('-o')+1]) for e in cc}
base_secs={e['file']:parse_sections(base_paths[e['file']]) for e in cc}

def compile_one(flag,e):
    args=shlex.split(e['command'])
    i=args.index('-o')
    tag=flag.replace('=','_').replace('-','')
    d=os.path.join(OUT,tag); os.makedirs(d,exist_ok=True)
    rel=e['file'].split('/src/',1)[1].replace('/','__')
    outp=os.path.join(d,rel+'.obj')
    args[i+1]=outp
    args.insert(1,flag)
    r=subprocess.run(args,cwd=e['directory'],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    if r.returncode!=0: return (flag,e['file'],'COMPILE_FAIL',r.stderr.decode(errors='replace')[-500:])
    secs=parse_sections(outp); b=base_secs[e['file']]
    changed=[n for n in sorted(set(b)|set(secs)) if b.get(n)!=secs.get(n)]
    hb=hashlib.sha256(open(base_paths[e['file']],'rb').read()).hexdigest()
    ht=hashlib.sha256(open(outp,'rb').read()).hexdigest()
    return (flag,e['file'],'IDENTICAL' if ht==hb else 'DIFF',changed)

results=[]
with concurrent.futures.ThreadPoolExecutor(max_workers=6) as ex:
    futs=[ex.submit(compile_one,flag,e) for flag in FLAGS for e in cc]
    for f in concurrent.futures.as_completed(futs): results.append(f.result())
json.dump(results,open('/dev/shm/w5207/screen_results.json','w'))
```

### A.2 One-TU link matrix (`link_variant.py` + `matrix.py`)

`link_variant.py` takes the canonical gold command from
`ninja -C cmake-build-match -t commands link_so`, replaces the one baseline object with
the variant object, and rewrites the output paths:

```python
import os, subprocess
REPO='/home/debian/GalaxyOnFire2/wt/w5207'
BUILD=os.path.join(REPO,'cmake-build-match')
OUT='/dev/shm/w5207/so'

def link_cmd():
    out=subprocess.check_output(['ninja','-C',BUILD,'-t','commands','link_so'],text=True)
    return next(ln for ln in out.splitlines() if 'ld.gold -shared' in ln)

def make_so(flag,tufile,so_path,unstripped_path):
    tag=flag.replace('=','_').replace('-','')
    rel=tufile.split('/src/',1)[1] if '/src/' in tufile else tufile
    obj=os.path.join('/dev/shm/w5207/objs',tag,rel.replace('/','__')+'.obj')
    base_obj=os.path.join(BUILD,'CMakeFiles/gof2.dir/./src/'+rel+'.obj')
    cmd=link_cmd().replace(base_obj,obj)
    cmd=cmd.replace(os.path.join(BUILD,'libgof2hdaa.unstripped.so'),unstripped_path)
    cmd=cmd.replace(os.path.join(BUILD,'libgof2hdaa.so'),so_path)
    r=subprocess.run(cmd,shell=True,cwd=REPO,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    return r.returncode==0, r.stderr.decode(errors='replace')[-2000:]
```

`matrix.py` then links every `DIFF` pair from `screen_results.json` and computes
`byte_exact` with the verifier's ELF reader:

```python
import sys, json, os, hashlib
sys.path.insert(0,'/home/debian/GalaxyOnFire2/wt/w5207/tools/verify')
from elf import Elf32
O=Elf32('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
ofunc=O.functions()

def fast_be(path):
    U=Elf32(path); u=U.functions(); common=set(ofunc)&set(u); be=0
    for n in common:
        a,b=ofunc[n],u[n]
        if a['size']>0 and b['size']==0: continue
        if a['size']==0 and b['size']>0: continue
        if O.bytes_at(a['value'],a['size'])==U.bytes_at(b['value'],b['size']): be+=1
    return be,len(common)
```

For every pair with `fast_be == 4500`, the official verifier was run:

```sh
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w5207/verify_eq/<flag>__<tu> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5207/so/<flag>/<tu>.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

### A.3 Global matrix (`global_matrix.py`)

Same link-command substitution, but every `CMakeFiles/gof2.dir/./src/*.obj` is replaced
by the corresponding `/dev/shm/w5207/objs/<flag>/*.obj`; the resulting `.so` is compared
to the baseline sha and measured with `fast_be`.

### A.4 Per-TU counts (`tu_counts.py`)

`verify.py`'s `Resolver`, `disassemble` and `reconcile` are applied only to the
functions of each of the 16 TUs containing non-byte functions (symbol -> TU map from
the NDK `nm` on the canonical objects), giving per-TU `byte` and `linked` counts
independent of the global count.
