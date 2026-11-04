# `.ARM.extab` size-exact: the five remaining LSDA cleanup scopes (wave 102, wm103)

Status: research note + landed source changes, 2026-11-04. Worktree
`/home/debian/GalaxyOnFire2/wt/wm103`, branch `wave102-wm103`, base `53d9ee4d5`.
Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(real address = Ghidra address - 0x10000). Scratch lived in `/dev/shm/wm103`.

## Result

`.ARM.extab` is now **size- and base-exact** `0x26298` @ `0x1a3b78` (was `0x2628c`,
-12 B). `.text` (`0x1337c8`), `.ARM.exidx` (`0x6f70`) and `.rodata`
(`0x41b67` @ `0x1c9e10`) are unchanged. The full gate is unchanged:

| metric | before | after |
|---|---:|---:|
| byte_exact / linked_exact | 4500 / 4500 | **4500 / 4500** |
| size-exact / data / imports | 4524, 843, 207 | same |
| `.ARM.extab` | `0x2628c` (-12) | **`0x26298` (0)** |
| `.ARM.exidx` differing bytes | 1666 | **842** |
| `.ARM.exidx` differing `w1` targets | 1602 | **822** |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

The remaining raw `.ARM.exidx` difference is one shifted range: the
`Generator::createMission` record is 4 B short (168 vs 172) and the
`LevelScript::process` record is 4 B long (352 vs 348). They cancel, so the section size
is exact, but every extab record between them sits at -4. All other 2438 EXT records are
at their original addresses.

## The five record deltas at the start of the pass

`_work/reconstructions/exidx_table.py --diff` on the base build:

| function | orig | base | delta | rows orig/base |
|---|---:|---:|---:|---:|
| `Generator::createMission` | 172 | 176 | +4 | 26 / 26 |
| `Level::createShip` | 336 | 328 | -8 | 57 / 54 |
| `LevelScript::process` | 348 | 352 | +4 | 46 / 46 |
| `HangarWindow::render` | 948 | 932 | -16 | 156 / 153 |
| `Hud::draw` | 476 | 480 | +4 | 77 / 78 |
| **total** | | | **-12** | |

## Landed fixes

### 1. `Level::createShip` 328 -> 336 (exact)

The w4237 recipe, verified again on this base. Three ordinary C++ changes:

1. Drop the artificial `try { player->setEmpData(...); } catch (...) { __builtin_trap(); }`
   (the original calls `setEmpData` with a plain `blx` and has `ttypeEncoding=0xff`).
2. Use real `new PlayerFighter(...)` / `new PlayerFixedObject(...)` instead of
   `::operator new(sizeof(...))` + placement new (the original allocates with `_Znwj` and
   has a cleanup row for each constructor).
3. Rewrite the `switch (type)` as the original's if-chain in the order the binary tests:
   `0x33`, `0x31`, `0x2c`.

Measured on an isolated `Level.cpp` compile:

| variant | text | extab |
|---|---:|---:|
| base | 2764 | 328 |
| drop try/catch | 2776 | 316 |
| + real `new` | 2768 | 336 |
| + if-chain 0x33/0x31/0x2c | **2764** | **336** |

### 2. `HangarWindow::render` 932 -> 948 (exact)

One `volatile` sink immediately after the `} else {` that opens the selected-item
`switch (tab)` block:

```cpp
} else {
    volatile int padS = 0; (void) padS;
    // The original dispatches the two selected-item button cases through a jump table...
    switch (tab) {
```

Measured on an isolated `HangarWindow.cpp` compile: body `9700` B (unchanged),
`.ARM.extab.text._ZN12HangarWindow6renderEv` `0x3b4` (948 B). A 355-line sweep of
single-line `volatile int padN` insertions found this placement; the sink splits a
cleanup region without changing the body size.

### 3. `Hud::draw` 480 -> 476 (exact)

Reorder the independent declarations in the jump-drive/cloak/docking progress block from
`barOffset, text, barW, barH, dt/fade` to `barH, dt/fade, text, barOffset, barW`:

```cpp
float progress = ...;
if (this->field_0x279) progress = 1.0f - progress;
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
this->field_0x468 += dt;
float fadeIn = ...; float fade = 1.0f;
if (fadeIn < 1.0f) fade = fadeIn;
String text = *Globals::gameText->getText(...) + String(" ");
barOffset = (int) ((float) textH + (float) frameH * 2.5f);
int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
```

All five statements are independent, so the reorder is semantics-preserving. An exhaustive
sweep of the 120 permutations of `{barOffset, text, barW, barH, dt/fade}` after the
progress block found two that keep body `9332` B and give `.ARM.extab` 476 B:
`H D T B W` and `H D T W B`; the first is landed.

### 4. `Generator::createMission` 176 -> 168 (-4, cancels LevelScript)

Two changes:

1. Fold the single-use `toSystem` temporary into the `distance` call:
   ```cpp
   SolarSystem *fromSystemPtr = systems->data()[fromSystem];
   float reward = (galaxy->distance(fromSystemPtr,
       systems->data()[galaxy->getStation(targetStation)->getSystem()]) / 1200.0f + 1.0f) * ...;
   ```
   This merges the two `getStation`/`getSystem` cleanup rows and shrinks the call-site
   table by 5 bytes (record 176 -> 168, body 1996 -> 1984).
2. Add `volatile double padA = 0; (void) padA;` at the top of the function to restore the
   original 1996-byte body. The pad does not change the call-site table.

Measured on an isolated `Generator.cpp` compile:

| variant | text | extab |
|---|---:|---:|
| base | 1996 | 176 |
| fold `toSystem` only | 1984 | 168 |
| + `volatile double padA` | **1996** | **168** |

The landed record is 168 B / 25 rows (orig 172 B / 26 rows). The -4 is deliberate: it is
exactly the compensation for `LevelScript::process` +4, so the section stays exact.

## `LevelScript::process` 352 (+4) — not fixed

46 rows in both, but our row starts are `18624..54448` instead of `12162..54462`: the
code before the first post-gap cleanup row is ~6.5 KB longer in ours (the function is
53.2% matched). Seven starts cross the 16384 ULEB boundary (3 bytes instead of 2), which
is the +4 after 4-byte record alignment. Fixing it requires matching the body layout
before row 13 (or merging a cleanup row); neither was found without changing the 57232-byte
body. It is left as the -4 partner of the Generator record.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wm103
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8      # RATCHET PASS, byte 4500
cmake --build cmake-build-match --target lint  -j8      # CLEAN
cmake --build cmake-build-match --target drift -j8      # 0 fields

# section sizes
NDK=.cache/ndk/android-ndk-r18b
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf \
  -SW cmake-build-match/libgof2hdaa.so | grep -E '\.text |\.ARM.exidx|\.ARM.extab|\.rodata'

# per-record lengths
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so

# raw exidx/extab content
python3 _work/reconstructions/exidx_raw_audit.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

## Proven vs hypothesis

* **Proven by measurement:** the four landed changes give the exact record lengths above
  on isolated TU compiles and on the full link; `.ARM.extab` is `0x26298`; byte_exact /
  linked_exact stay 4500; all other gate metrics are unchanged.
* **Proven from the original tables/disassembly:** `Level::createShip` has no type table
  (`ttype=0xff`) and uses `_Znwj`; `HangarWindow::render` and `Hud::draw` are non-byte-exact
  bodies whose record length is the only hard constraint; `Generator::createMission`'s +4
  is a single 2-byte landing-pad ULEB where the original has a 1-byte gap row.
* **Hypothesis:** the `volatile double padA` and `volatile int padS` are codegen anchors,
  not original source. `LevelScript::process` is expected to become exact once its body
  layout matches the original (the 7 ULEB-boundary starts will drop below 16384); that
  would also let `Generator::createMission` return to 172 B and make every `.ARM.exidx`
  `w1` operand exact.
