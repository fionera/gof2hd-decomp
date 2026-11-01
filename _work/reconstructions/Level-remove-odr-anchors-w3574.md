# Removing the `(void)` odr-use anchors in `Level.cpp` (w3574)

Status: research note + landed source change (2026-11-01). Branch `wave87-w3574`,
based on master `54e36581`. All numbers below were measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the repo's NDK r18b clang 7.0.2
(`-Oz`, armeabi-v7a).

## TL;DR

The eight `(void) kStationTurrets; ...` statements at the end of `Level::createGun`
are load-bearing: they are the first odr-use of eight namespace-scope tables and
therefore pin their `.rodata` emission between `createGun`'s `g_cg_soundTable` and
`createMission`'s first local static (the `case 183` `Coords6`).  Removing them
alone moves the tables later and costs **1 byte_exact** (`createCampaignMission`).

They can be removed without any gate change by moving the eight table definitions
to just after `Level::createGun` and marking them `[[gnu::used]]`.  `[[gnu::used]]`
makes clang emit an internal-linkage static at its *definition* point instead of
its first-use point, so the tables land at the same `.rodata` addresses.  The
stripped `.so` is byte-identical to the anchor version except for the 20-byte
`.note.gnu.build-id` (a pre-strip content hash; local symbol order changed).

Gate before = gate after:

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 1. Baseline

`cmake-build-match` was absent:

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes/bases (stripped link output):

```
.text        0x1337c8 @ 0x69440
.ARM.exidx   0x6f70   @ 0x19cc08
.ARM.extab   0x26290  @ 0x1a3b78   (known -8 residue)
.rodata      0x41b67  @ 0x1c9e10   (0 differing bytes vs original)
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

`lint` -> `CLEAN ✓`; `drift` -> `0 field(s) across 0 class(es)`.

The eight anchors were at `src/game/world/Level.cpp:1777-1786`:

```cpp
    // Emission-order references: the original Level TU emits the station/blueprint
    // tables here, before createMission's local coordinate tables.
    (void) kStationTurrets;
    (void) kAlienStationTurrets;
    (void) kAlienOutpostTurrets;
    (void) kBaseTurrets;
    (void) kPirateStationCoords;
    (void) kBlueprintRaces;
    (void) kBlueprintPositions;
    (void) kBlueprintSpacePoints;
```

They are the only `(void)` anchors in `Level.cpp`; the analogous anchors in
`StarMap.cpp` and `MenuTouchWindow.cpp` are out of scope for this pass.

## 2. Why the anchors matter

Original `.rodata` order (from `_work/reconstructions/rodata_content_map_round3.py`
and re-confirmed here by direct byte comparison):

```
0x1fc54c  g_cg_meshTable          (first used in Level::assignGuns)
0x1fc8f0  g_cg_soundTable         (first used in Level::createGun)
0x1fcc94  kStationTurrets
0x1fcd74  kAlienStationTurrets
0x1fce34  kAlienOutpostTurrets
0x1fcedc  kBaseTurrets
0x1fcf54  kPirateStationCoords
0x1fcf84  kBlueprintRaces
0x1fcf98  kBlueprintPositions
0x1fcfd4  kBlueprintSpacePoints
0x1fcfe8  Level::createMission case-183 Coords6
```

The eight tables are namespace-scope `static const` arrays used only in
`createMission` (and `kBaseTurrets` also in `createCampaignMission`).  clang emits
such a table at its **first odr-use in TU codegen order**, not at its declaration
point.  The first real uses are inside `createMission`, *after* the `case 183`
local static in source order (the `siegedStation` block is after the big
`switch`), so without the anchors the tables are emitted after `0x1fcfe8`.

Controlled A/B (anchors deleted, nothing else changed):

```
verify: byte 4495  linked 4496  ...   FAIL byte_exact 4496 -> 4495
```

* `.rodata` differing bytes: **758** (77 runs), first diff `0x1fcc98`.
* The one function that flips is `_ZN5Level21createCampaignMissionEv`
  (`pct == 100.0`, `byte == false`): its pool deltas to `kBaseTurrets`
  (`0x1fcedc` -> moved address) are the only byte-visible effect.
* `.text`, `.ARM.exidx`, `.ARM.extab`, `.data`, `.bss`, `.got` sizes are
  unchanged; `createGun` itself stays byte-exact (the `(void)` statements emit
  no code at `-Oz`).

## 3. What clang 7 actually does (controlled micro-tests)

All tests compiled with the repo's NDK r18b clang++ and the match flags
(`-target armv7-none-linux-androideabi21 -march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g
-stdlib=libc++ -ffunction-sections`).  Sources under `/tmp/ordtest*.cpp`.

| shape | emission point | evidence |
|---|---|---|
| namespace-scope `static const` array | first odr-use | `ordtest.cpp`: source `a,b,c`, uses `b,a,c` -> `.rodata` `b,a,c` |
| namespace-scope static used before a function-local static in the same function | first use, before the local static | `ordtest20.cpp` -> `nsa, loc` |
| internal-linkage static data member (`namespace { struct S { static const int b[5]; }; }`) | **first use**, not definition point | `ordtest17.cpp`: `a` (use), `c` (use), `b` (defined between) -> `a,c,b` |
| external-linkage static data member | definition point | `ordtest16.cpp`: `a` (use), `b` (defined), `c` (use) -> `a,b,c` |
| internal-linkage `static const` with `[[gnu::used]]` | definition point | `ordtest23.cpp`: `a` (use), `b` (used, defined), `c` (use) -> `a,b,c` |

The external-linkage static-data-member route is not usable here: the members
would become exported `.dynsym` objects (the original exports none of these
tables; `data: exact 843 of 843` would gain 8 extras).  `[[gnu::used]]` is the
attribute-free-of-`__attribute__` spelling already used in this tree
(`PlayerCreature.cpp`, `MissionsWindow.cpp`).

## 4. Experiments

Every row rebuilt with `cmake --build cmake-build-match --target check -j8`.

| # | change | byte | linked | `.rodata` diff | first diff | verdict |
|---:|---|---:|---:|---:|---:|---|
| 0 | baseline (anchors) | 4496 | 4496 | 0 | — | — |
| 1 | delete the eight `(void)` anchors | 4495 | 4496 | 758 | `0x1fcc98` | -1 byte |
| 2 | move definitions after `createGun`; make them static data members of an anonymous-namespace struct | 4495 | 4496 | 758 | `0x1fcc98` | -1 byte (internal-linkage members emit at first use) |
| 3 | move definitions after `createGun`; add `[[gnu::used]]`; delete anchors | **4496** | **4496** | **0** | — | **equal gate, anchors gone** |

Experiment 2 is the useful negative result: it proves the fix is not "move the
definitions" per se, but "force definition-point emission".

Final `check` for experiment 3:

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint` -> `CLEAN ✓`; `drift` -> `0 field(s) across 0 class(es)`;
`diff` -> `original: 5370  ours: 5370  extra: 0  missing: 0`.

Full stripped-`.so` comparison against the anchor baseline build:

```
$ cmp /tmp/new.so /tmp/base.so
differ: byte 477, line 1
$ python3 - <<'PY'
# count differing bytes
...
PY
new vs baseline diff bytes 20 ['0x1dc', ..., '0x1ef']
```

`0x1dc..0x1ef` is exactly the 20-byte SHA-1 payload of `.note.gnu.build-id`
(section starts at `0x1cc`).  Every other byte of the 2,187,380-byte stripped
`.so` is identical.  The build-id differs because it hashes the pre-strip file,
including local `.symtab` order; the eight tables moved in the source and their
local symbol indices changed.  The build-id is already not the original's, and
will converge only with whole-file identity.

## 5. Landed source change

`src/game/world/Level.cpp`:

* the eight table definitions moved from just after `createGasClouds` to just
  after `Level::createGun` (i.e. between `createGun` and `createMission`);
* each definition gained `[[gnu::used]]`;
* the eight `(void)` statements and their comment were deleted.

The table contents, order, names, linkage and section (plain `.rodata`) are
unchanged.  No linker flag, no `__attribute__`, no inline asm, no padding, no
manual data.

## 6. Proven vs hypothesis

**Proven (commands above):**
- The anchors are the first odr-use of the eight tables and pin their `.rodata`
  emission between `g_cg_soundTable` and `createMission`'s `case 183` local.
- Deleting them alone moves the tables later: 758 `.rodata` bytes differ, first
  at `0x1fcc98`, and `createCampaignMission` loses byte-exactness (4496 -> 4495).
- `[[gnu::used]]` on the moved internal-linkage definitions reproduces the exact
  original `.rodata` order: 0 differing bytes, byte/linked 4496/4496, all section
  sizes/bases unchanged, lint clean, drift 0, symbol parity 5370/5370.
- The resulting stripped `.so` differs from the anchor baseline only in the
  20-byte `.note.gnu.build-id`.

**Hypothesis (not proven from source):**
- The original source probably used a different natural mechanism to emit these
  tables before `createMission` (e.g. a helper function's local statics, or a
  source order where the station/blueprint blocks precede the `case 183` switch).
  `[[gnu::used]]` is a reconstruction aid, not evidence about the original text.
- The exact clang 7 static-emission worklist is characterised empirically above;
  the rule "internal-linkage static data member = first use, `[[gnu::used]]` =
  definition point" is measured, not documented.
