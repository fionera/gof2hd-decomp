# `Status::missionCompleted` — recovering the 4-byte `.text` shortfall (w3472)

Status: landed pass. Branch `wave87-w3472`, based on master `8efda908`.
Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2 `r328903`, `-Oz`, image base 0x10000; real VMA = Ghidra - 0x10000).

## TL;DR

* Baseline in this worktree is **byte_exact 4414 / linked_exact 4496**, not the
  task brief's stale `4182/4495`.
* The original `Status::missionCompleted(bool,bool,long long)` is **1432 B @
  `0xabf38`** and switches directly on `m->getType()`; it has two TBH tables
  (40 entries for types 150..189, 7 entries for types 8..14).
* A direct `switch (m->getType())` compiles to **1428 B** in our tree: clang 7
  lays the case bodies out in ascending case-value order, while the original
  lays them out in source order. The 4-byte `.text` shrink shifts every later
  function and costs 1377 byte-exact functions.
* `switch ((short) m->getType())` compiles to **1432 B** with no padding anchor.
  Used alone it is gate-neutral (byte 4414 / linked 4496); the `missionCompleted`
  body itself drops from 86.7% to 57.7%.
* With the size fixed, the w3469 Level `.rodata` corrections become landable.
  The combined pass is a strict improvement: **byte_exact 4414 -> 4441 (+27),
  linked 4496 unchanged**, every ELF section size/base exact, lint clean,
  drift 0. `.rodata` differing bytes drop 13,121 -> 8,826 (first still
  `0x1fb7f8`).

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4414  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`report.json` row for the target: `osize 1432`, `usize 1432`, `pct 86.7`,
`linked false`. The current source uses the invented `kMissionCase[190]`
type->rank lookup table in `Level.cpp`; that table is what the Level `.rodata`
pass needs to remove.

## 2. Original dispatch (binary evidence)

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -sW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep missionCompleted
```

The original body at `0xabfda` is:

```
sub.w   r1, r0, #150
cmp     r1, #39
bhi.n   <low/default path>
tbh     [pc, r1, lsl #1]        @ 0xabfe2, 40 halfwords at 0xabfe6
...
sub.w   r1, r0, #8
cmp     r1, #6
bhi.n   <default>
tbh     [pc, r1, lsl #1]        @ 0xac04e, 7 halfwords at 0xac052
```

Decoding the high table gives the original body layout in source order:

```
167/174, 150, 151, 152, 153, 154, 155, 158, 162, 166, 170, 184, 189,
156, 157, 160, 163, 164, 165, 168, 169, 171, 172, default
```

and the low table gives `8, 11, 13, 14` (types 0 and 11 share a source body but
the compiler splits type 0 out of the 8..14 range). There is no byte-table load
in the original.

## 3. The 4-byte shortfall

Replacing the lookup with a direct switch on the actual mission types (the
current source's case bodies converted back to type labels) gives:

```sh
# edit Status.cpp: switch (kMissionCase[m->getType()]) -> switch (m->getType())
cmake --build cmake-build-match --target link_so -j8
readelf -sW cmake-build-match/libgof2hdaa.so | grep missionCompleted
# 000abf39  1428 FUNC ...
```

`report.json`: `osize 1432`, `usize 1428`, `pct 60.2`, and the whole gate drops
to **byte 3037 / linked 4496** because `.text` is 4 B shorter and every later
function shifts by -4.

The direct switch's TBH table is still 40 entries, and its bodies are
block-for-block the same code, but clang 7 emits them in ascending case-value
order:

```
167/174, 150, 151, 152, 153, 154, 155, 156, 157, 158, 160, 162, 163,
164, 165, 166, 168, 169, 170, 171, 172, default, 184, 189
```

The different body positions change `b.n`/`b.w` and `cbz`/`cmp+bne` encodings;
the net is -4 B. Reordering the case blocks in the source (ascending, reverse,
or the original order) does not change the emitted ascending layout.

## 4. The 4-byte fix: signed-short switch lowering

Changing only the switch expression:

```cpp
switch ((short) m->getType()) {
```

gives **1432 B** and keeps `.text` at `0x1337c8`. The signed-narrow lowering
places the 184/189 bodies after the default block, which restores the size
without any padding anchor. It is semantically safe here: mission types are
small non-negative values (0..189), and the original switch only has cases in
0..14 and 150..189.

Cast sweep (same source, probe compile of `Status.cpp` only):

| switch expression | function size |
|---|---:|
| `m->getType()` | 1428 |
| `(short) m->getType()` | **1432** |
| `(unsigned char) m->getType()` | 1436 |
| `(unsigned short) m->getType()` | 1436 |
| `(m->getType() & 0xff)` | 1436 |
| `(short)(m->getType() + 0x100)` | 1444 |
| `(long long) m->getType()` | 1500 |
| `(signed char) m->getType()` | 380 |

The short form is stable across source case order (original, ascending and
reverse all give 1432; descending gives 1364). It is a codegen-shape change,
not a data/padding change.

Used **alone**, the short form is gate-neutral:

| tree | byte | linked | `.text` | `missionCompleted` pct |
|---|---:|---:|---:|---:|
| baseline (`kMissionCase` lookup) | 4414 | 4496 | `0x1337c8` | 86.7 |
| direct `int` switch | 3037 | 4496 | `0x1337c4` | 60.2 |
| direct `short` switch | 4414 | 4496 | `0x1337c8` | 57.7 |

## 5. Full pass (landed): w3469 Level data + short switch

With the 4-byte size fixed, the w3469 Level `.rodata` corrections can land
without shifting `.text`:

* `src/game/world/Level.cpp`: `g_ips_shipTypeMap[64]` (second 32 values
  `8,0,0,2,0,0,0,1,0,1,1,2,1,3,3,3,3,1,1,0,8,1,1,0,3,2,0,0,8,1,3,1`),
  `jumpgateMeshes[16]` (drop the duplicated `kAgentMeshes` half), and remove the
  invented `kMissionCase[190]`.
* `src/engine/render/ImageFactory.cpp`: add the two missing
  `gCreateChar2Table` rows `{0,0,0,2}` and `{1,1,1,1}`.
* `src/engine/render/LensFlare.cpp`: `alignas(16)` on `kFlareRed`,
  `kFlareGreen`, `kFlareBlue`.
* `src/game/ui/Hud.cpp`: `g_Hud_raceBadge[10] =
  {1190,1187,1189,1188,-1,-1,-1,-1,-1,-1}` (drop `alignas(16)`).

Measured with the full `check` target:

```
verify: byte 4441  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes/bases (all exact):

```
.text        0x1337c8 @ 0x69440
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

`.rodata` differing bytes (ours vs original, same size/base): **8,826**
(was 13,121), first differing byte still `0x1fb7f8`. `.data` and `.got` remain
byte-identical; `.data.rel.ro` remains 21 differing bytes.

The gain is +27 byte-exact functions from the corrected `.rodata` data; the
`missionCompleted` body itself is still a 57.7% near-miss (the direct `int`
form was 60.2%). The 4-byte fix is what prevents the -4 `.text` shift that
previously cost 1377 byte-exact functions.

## 6. Reproduction commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8

# direct int switch A/B
#   switch (m->getType()) { ... }  (type labels)
cmake --build cmake-build-match --target link_so -j8
readelf -sW cmake-build-match/libgof2hdaa.so | grep missionCompleted   # 1428
cmake --build cmake-build-match --target verify -j8

# short switch
#   switch ((short) m->getType()) { ... }
cmake --build cmake-build-match --target link_so -j8
readelf -sW cmake-build-match/libgof2hdaa.so | grep missionCompleted   # 1432
cmake --build cmake-build-match --target verify -j8

# full pass
python3 tools/ratchet.py --build-dir cmake-build-match --update-baseline
cmake --build cmake-build-match --target check -j8
```

## 7. Proven vs hypothesis

**Proven**

* Original `missionCompleted` is 1432 B and dispatches directly on
  `m->getType()` with two TBH tables (40 + 7 entries); there is no lookup-table
  load.
* A direct `int` switch emits 1428 B in this tree because clang 7 places the
  bodies in ascending case-value order; the -4 B shifts `.text` and costs 1377
  byte-exact functions.
* `switch ((short) m->getType())` emits 1432 B with no padding anchor and keeps
  every section size/base exact.
* With the short switch, the w3469 Level/ImageFactory/LensFlare/Hud `.rodata`
  corrections are a strict gate improvement: byte 4414 -> 4441, linked 4496
  unchanged, lint clean, drift 0.

**Hypothesis / next pass**

* The original source almost certainly wrote `switch (m->getType())` (int), not
  a `short` cast. The cast is a codegen-shaping workaround for clang 7's
  ascending block placement, not recovered source. If a later source/TU change
  makes the direct `int` switch emit source-order bodies, the cast should be
  removed and the function re-tested.
* The remaining `.rodata` content order (8,826 differing bytes) is still the
  next value lever; the Level block content order is the main known cluster.
