# Section-order removal: `Gun::setOffset` overload order (w3745)

Worker: w3745. Branch: `wave87-w3745`, based on master
`477d6a94` ("docs: lock BoundingSphere emission order"). Original:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b
clang 7.0.2 `-Oz`). All numbers below were produced in this worktree with the
commands shown. Scratch output is under `/tmp/w3745/gun/`.

This note closes the next open item recorded in
`_work/reconstructions/boundingsphere_collide_placement_w3745.md` §6: the
first plain (no `--section-ordering-file`) mismatch at this HEAD is the
`Gun::setOffset` overload pair. The original emits
`setOffset(AbyssEngine::AEMath::Vector*)` **before** `setOffset(int,int)`; our
`Gun.cpp` defined them in the opposite order. Swapping the two top-level
definitions makes both overloads address-exact in the plain link. The source
change is gate-neutral.

## 0. Result

| build | gate byte/linked | plain byte_exact | exact addresses | exact-address prefix | first plain mismatch |
|---|---:|---:|---:|---:|---|
| before (HEAD `477d6a94`) | **4497 / 4497** | **2855** | **3461 / 4524** | **3320** | `Gun::setOffset(AbyssEngine::AEMath::Vector*)` `0x1522e4` vs `0x15236c` |
| after (this pass) | **4497 / 4497** | **2856** | **3463 / 4524** | **3373** | `Radio::setMessages(Array<RadioMessage*>*)` `0x15501c` vs `0x155036` |

Gate unchanged (`4497/4497`, `RATCHET PASS`), `.text` `0x1337c8 @ 0x69440`,
`.rodata` 0 differing bytes, lint `CLEAN ✓`, drift `0`.

The plain `byte_exact` gain is **+1**, not +2: both overloads move onto their
original addresses (`+2` exact addresses), but only the `Vector*` overload
becomes `byte=True`. The `int,int` overload's instruction stream is already
identical (`pct == 100.0`); its only remaining difference is the PIC literal
pool word for `gSO_table`, which is `+0x10` because the plain link's
`.rodata` base is `0x1c9e20` instead of the original `0x1c9e10` (the known
plain-link artifact, see §5.2).

The two overloads in the plain link:

| symbol | original | before | after |
|---|---:|---:|---:|
| `Gun::setOffset(AbyssEngine::AEMath::Vector*)` | `0x1522e4` (0x44 B) | `0x15236c` | **`0x1522e4`** |
| `Gun::setOffset(int, int)` | `0x152328` (0x88 B) | `0x1522e4` | **`0x152328`** |

No other exported function address changes: the two functions are a fixed-size
swap (`0x44 + 0x88` either way), so the block total and every following
function are unchanged.

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
.ARM.extab       0x02628c @ 0x1a3b78   (original 0x26298, known -0xc)
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

Plain no-order baseline:

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/gun/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/gun/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/gun/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/gun/base_noorder.so
```

```text
byte_exact 2855  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3461/4524 same; prefix 3320;
            first (0x1522e4, '_ZN3Gun9setOffsetEPN11AbyssEngine6AEMath6VectorE', 0x15236c)
```

## 2. Original order (binary witness)

`nm -nSD --defined-only` on the original gives the emitted `Gun` order around
the overloads:

```text
001522dc 00000006 T _ZN3Gun17setLevelCollisionEb
001522e4 00000044 T _ZN3Gun9setOffsetEPN11AbyssEngine6AEMath6VectorE
00152328 00000088 T _ZN3Gun9setOffsetEii
001523b0 00000006 T _ZN3Gun9setImpactEP6Sparks
001523b6 00000006 T _ZN3Gun10setEnemiesEP5ArrayIP6PlayerE
```

The HD source-layout dump
(`_work/original_layout/original_layout.json`,
`tu_symbols.gof2hd_1.1.19_ios["Classes/Game/Gun.cpp"]`) lists
`Gun::setOffset(AbyssEngine::AEMath::Vector*)` after `getMagnitude` and before
`setEnemies`, consistent with the Android order. The HD build does not list a
`setOffset(int,int)` overload at all (it is Android-2.0.x-era), so the Android
`.dynsym` is the authority for that overload's position.

`Gun.h` already declared the overloads in the original order
(`setOffset(Vector *v);` then `setOffset(int a, int b);`); only the definition
order in `Gun.cpp` was reversed.

## 3. Baseline object order

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/weapons/Gun.cpp.obj`
(section-header order = emission order; this TU has no weak template copies
between the two overloads):

```text
Gun::setLevelCollision(bool)
Gun::setOffset(int, int)
Gun::setOffset(AbyssEngine::AEMath::Vector*)
Gun::setImpact(Sparks*)
```

This is exactly the source order. The original address order is the reverse
pair: `setOffset(Vector*)` then `setOffset(int,int)`.

## 4. Landed source change

Only the order of the two top-level definitions in
`src/game/weapons/Gun.cpp` changed; no body, declaration or header was edited.

```text
before (source / object order):
  setLevelCollision, setOffset(int,int), setOffset(Vector*), setImpact

after:
  setLevelCollision, setOffset(Vector*), setOffset(int,int), setImpact
```

The object order after the edit is exactly the original address order:

```text
Gun::setLevelCollision(bool)
Gun::setOffset(AbyssEngine::AEMath::Vector*)
Gun::setOffset(int, int)
Gun::setImpact(Sparks*)
```

## 5. Controlled A/B

Each row is a full `check` rebuild plus the plain no-order relink.

| # | source | gate byte/linked | plain byte | exact addrs | prefix | first plain mismatch |
|---|---|---:|---:|---:|---:|---|
| 0 | baseline | **4497 / 4497** | **2855** | 3461 | 3320 | `Gun::setOffset(Vector*)` `0x1522e4` vs `0x15236c` |
| 1 | swap the two `setOffset` definitions (landed) | **4497 / 4497** | **2856** | **3463** | **3373** | `Radio::setMessages` `0x15501c` vs `0x155036` |

Commands (same for every row):

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/gun/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/gun/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/gun/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/gun/after_noorder.so
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
byte_exact 2856  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3463/4524 same; prefix 3373;
            first (0x15501c, '_ZN5Radio11setMessagesEP5ArrayIP12RadioMessageE', 0x155036)
```

The per-function diff of the two plain verify reports is exactly:

```text
GAIN 1
  Gun::setOffset(AbyssEngine::AEMath::Vector*)
LOSS 0
```

The exported-address diff is exactly the two overloads:

```text
Gun::setOffset(int, int)                          0x1522e4 -> 0x152328
Gun::setOffset(AbyssEngine::AEMath::Vector*)      0x15236c -> 0x1522e4
```

### 5.1 Gate `.so` A/B

The gate `.so` is byte-identical in `.text` and `.rodata`; only the build-id
and the `.ARM.extab` record order for the two swapped functions change:

| section | before | after | differing bytes |
|---|---:|---:|---:|
| `.text` | `0x1337c8 @ 0x69440` | `0x1337c8 @ 0x69440` | 0 |
| `.rodata` | `0x41b67 @ 0x1c9e10` | `0x41b67 @ 0x1c9e10` | 0 |
| `.ARM.exidx` | `0x6f70 @ 0x19cc08` | `0x6f70 @ 0x19cc08` | 0 |
| `.ARM.extab` | `0x2628c @ 0x1a3b78` | `0x2628c @ 0x1a3b78` | 4 |
| `.note.gnu.build-id` | — | — | 20 |

`cmp -l` gives 24 differing bytes total (20 build-id + 4 `.ARM.extab`), at
file offsets `0x1dc..0x1ef` and `0x1a164d`, `0x1a1655`, `0x1c3cbf`, `0x1c3ccb`.
All other sections (including `.rel.plt`, `.plt`, `.got`, `.dynsym`,
`.data.rel.ro`, `.data`) are byte-identical, so the swap does not perturb the
gate PLT order.

Reproduce the A/B:

```sh
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/gun/gate_after.so
git stash push -- src/game/weapons/Gun.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/gun/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745/gun/gate_base.so /tmp/w3745/gun/gate_after.so
cmp -l /tmp/w3745/gun/gate_base.so /tmp/w3745/gun/gate_after.so | wc -l   # 24
```

Measured hashes:

```text
base  eee00d579d9391e9da717ebdf87447533732af0ee43557dcc94f0ebfdf8bb9a6
after b2cf789cec9248b74a44ba226e29a71f6d589a9b6d6780821d1aba22eaab1d71
```

### 5.2 Plain-link layout and why the gain is +1, not +2

Plain-link section layout (identical before and after; the swap does not
change any section size or base):

| section | original | plain (before/after) |
|---|---:|---:|
| `.text` | `0x1337c8 @ 0x69440` | `0x1337e8 @ 0x69440` |
| `.ARM.exidx` | `0x6f70 @ 0x19cc08` | `0x6f68 @ 0x19cc28` |
| `.ARM.extab` | `0x26298 @ 0x1a3b78` | `0x2628c @ 0x1a3b90` |
| `.rodata` | `0x41b67 @ 0x1c9e10` | `0x41b67 @ 0x1c9e20` |

The `Vector*` overload is now fully byte-identical. Its disassembly matches
the original instruction-for-instruction and both literal-pool words match
(the stack-guard PIC delta and the `100.0f` `kZOffset` word).

The `int,int` overload is at the original address and its instruction stream
is identical (`pct == 100.0`), but its `gSO_table` PIC literal-pool word is
`+0x10`:

```text
orig  1523ac: 6a0b0000   # 0x15233e + 0x000b0b6a = 0x202ea8  (gSO_table)
ours  1523ac: 7a0b0000   # 0x15233e + 0x000b0b7a = 0x202eb8  (gSO_table + 0x10)
```

`0x202ea8` is the original `gSO_table` (row 0 is all zeros; row 2 at
`0x202f20` is `{320,0,100},{0,0,100},{-320,0,100}`, matching the source
table). The `+0x10` is exactly the plain link's `.rodata` base shift
(`0x1c9e10 -> 0x1c9e20`), caused by the plain `.text +0x20` /
`.ARM.exidx`/`.ARM.extab` layout, not by this TU. The same `+0x10` shift
already suppresses hundreds of other otherwise-exact functions in the plain
link.

## 6. Next frontier

The first plain no-order mismatch is now:

```text
original 0x15501c  Radio::setMessages(Array<RadioMessage*>*)
ours     0x155036  Radio::setMessages(Array<RadioMessage*>*)
```

This is a separate TU (`src/game/core/Radio.cpp`) and was not changed in this
pass.

## 7. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  `.rodata` 0 differing bytes, lint `CLEAN ✓`, drift 0.
* Baseline plain no-order is `byte 2855 / linked 4497`, `3461/4524`
  addresses, prefix `3320`; first mismatch
  `Gun::setOffset(Vector*)` at `0x1522e4` (ours `0x15236c`).
* The original emits `setOffset(Vector*)` at `0x1522e4` (0x44 B) then
  `setOffset(int,int)` at `0x152328` (0x88 B); the baseline object emits the
  reverse pair. `Gun.h` already declared the original order.
* Swapping the two definitions is gate-neutral (`4497/4497`, `.text` and
  `.rodata` byte-identical, `.rel.plt`/`.plt`/`.got`/`.dynsym`/`.data*`
  byte-identical, lint `CLEAN`, drift 0) and makes both overloads
  address-exact in the plain link.
* The plain link improves `2855 -> 2856` byte-exact, `3461 -> 3463` exact
  addresses, `3320 -> 3373` exact-address prefix; the per-function diff is
  1 gain / 0 losses. Only the two overload addresses change.
* The `Vector*` overload is fully byte-identical in the plain link; the
  `int,int` overload differs only by the `gSO_table` PIC pool word, which is
  `+0x10` because the plain `.rodata` base is `0x1c9e20` vs the original
  `0x1c9e10`.
* The plain link's next mismatch is `Radio::setMessages` at `0x15501c`
  (ours `0x155036`).

**Hypothesis / still open**

* The plain `.text +0x20`, `.ARM.exidx -8` and `.ARM.extab -0xc` are the
  known plain-link artifacts; until the `.rodata` base is exact, any function
  with a `.rodata` PIC literal will stay non-byte-exact even when its
  instruction stream and address are correct.
* The ordering file is still required: the plain link is `2856`, not `4497`.
* The next TU (`Radio.cpp`) needs its own emission-order pass under the full
  gate.
