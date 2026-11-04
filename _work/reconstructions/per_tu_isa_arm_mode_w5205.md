# Per-TU ISA / ARM-mode flag sweep (w5205)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5205`,
branch `wave104-w5205`, base `8c5fd3646`. Scratch is `/dev/shm/w5205`.
**No source or build-file change is landed.** The canonical gate is unchanged:
byte_exact **4500/4524**, linked_exact **4500/4524**, size-exact **4524/4524**,
data_exact **843/843**, imports **207**, lint **CLEAN**, drift **0**,
`RATCHET PASS`.

## 0. Bottom line

* **Every one of the 192 app TUs is Thumb.** The original's `.dynsym` has
  4,501 Thumb-valued (odd) and 23 ARM-valued (even) defined `FUNC` symbols;
  every app TU has at least one exported function and **no app TU owns an
  even-valued exported function**. The 23 ARM functions are exactly the
  vendored libcrypto subset (SHA-224/256, `armv4cpuid.S`, the NEON SHA
  routine), and all 23 are already byte-exact.
* **The vendored C TUs are already in the right mode.** All 121
  zlib+libzip functions are Thumb and byte-exact; all 23 libcrypto functions
  are ARM and byte-exact. Flipping any of them is a large regression
  (zlib `-marm` 2215, libzip `-marm` 1346, libcrypto `sha256.c -mthumb` 3214).
* **No per-TU ISA/ARM-mode variant is a strict improvement.** The 16 TUs that
  contain the 24 non-byte-exact functions were swept with all 22 requested
  variants (352 compiles, 121 codegen-changing combinations, each relinked).
  The best result is **4499** (`Layout.cpp -mcpu=cortex-a8`), i.e. one below
  baseline. A sweep of all 192 app TUs with the eight variants that actually
  change code (1,536 compiles, 862 size-changing TUs, each relinked) also
  never exceeds **4500**.
* The remaining 14 variants (`-mtune=cortex-a8/-a9`, `-mno-movt`,
  `-mno-neg-immediates`, `-mno-madd4`, `-mcrc`, `-mnocrc`,
  `-mstack-alignment=4/8/16`, `-mno-restrict-it`, `-march=armv7-a`,
  `-mthumb`, `-mfpu=neon`) produce **identical `.text` in all 192 app TUs**
  (2,688 compiles, 0 changed sections).

## 1. Baseline and commands

```sh
export TMPDIR=/dev/shm/w5205/tmp
cd /home/debian/GalaxyOnFire2/wt/w5205
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Baseline (`check`, reproduced before and after the sweep):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes (ours, from `readelf -SW`):

```text
.text            0x1337c8 @ 0x069440
.rodata          0x041b67 @ 0x1c9e10
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
.ARM.attributes  0x000038 @ 0
```

Hashes:

```text
original .so  d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483
ours   .so    04a3d86ad8089129700d5ed978d05a310d993480a55c962f4fad5c742f75f684
.ARM.attributes (both) b03afea6aa56c9ca191d5385abb498e64c7ecdf8e249a80e68704050256b1bc2
```

The `.so` hash is link-dependent through `.note.gnu.build-id` (a hash of the
unstripped link output); the section contents other than that note are stable.
Two relinks in this pass produced `04a3d86a...` and `88c308ea...` with identical
section contents and the same 4500/4500 gate.
`.ARM.attributes` is byte-identical (56/56 bytes) between the original and
ours.

## 2. ARM/Thumb classification of the original

The original is stripped of `.symtab`, but every exported function is in
`.dynsym` with the ARM/Thumb interworking bit in `st_value`. Parsing
`readelf --dyn-syms -W`:

```text
defined FUNC symbols: 4524
  odd  (Thumb):        4501   (includes the two size-0 exports)
  even (ARM):            23
  size 0:                 2   (AbyssEngine::getAppVersion, __cxa_end_cleanup)
```

The 23 even-valued functions are exactly the libcrypto subset:

```text
0x18f730 SHA224_Init            0x18f7b4 SHA256_Init
0x18f838 SHA256_Update          0x18f948 SHA224_Update
0x18f94c SHA256_Transform       0x18f954 SHA256_Final
0x18fd6c SHA224                 0x18fdd0 SHA256
0x18fe38 SHA224_Final           0x18fe40 OPENSSL_atomic_add
0x18fe5c OPENSSL_cleanse        0x18feb0 CRYPTO_memcmp
0x18ff00 _armv7_neon_probe      0x18ff08 _armv7_tick
0x18ff10 _armv8_aes_probe       0x18ff18 _armv8_sha1_probe
0x18ff20 _armv8_sha256_probe    0x18ff28 _armv8_pmull_probe
0x18ff30 OPENSSL_wipe_cpu       0x18ff88 OPENSSL_instrument_bus
0x18ff90 OPENSSL_instrument_bus2 0x1900e0 sha256_block_data_order
0x190ef0 sha256_block_data_order_neon
```

Direct disassembly confirms the mode, e.g. `SHA256_Update` at `0x18f838`:
`e92d40f8  push {r3, r4, r5, r6, r7, lr}` (ARM), while
`InitializeBlowfish` at `0x6b3b0` is Thumb.

Mapping every object's defined function symbols back to the original
`.dynsym` shows:

```text
app TUs: 192 / 192 have at least one exported function
app TUs with an even-valued (ARM) exported function: 0 / 192
```

Because a TU compiled `-marm` emits *all* of its functions in ARM mode, this
is decisive: no app TU was compiled ARM.

## 3. Per-TU variant sweep

### 3.1 Method

For each TU, the exact command from
`cmake-build-match/compile_commands.json` was used, with the variant inserted
immediately before `-o` (so it overrides the earlier `-mthumb`/`-mfpu=neon`).
The resulting object's `.text.*` sections were compared with the baseline
object's. Only combinations that changed code were relinked.

Relinking had to bypass Ninja's deps log: copying a variant object over the
build object makes Ninja think the stored deps mtime is stale and recompile
the TU with the baseline flags. The harness therefore sets the copied
object's mtime to the exact `deps mtime` reported by
`ninja -t deps <obj>`, moves `libgof2hdaa.so` aside, and runs
`ninja -C cmake-build-match link_so`. The resulting `.so` was scored with a
raw-byte checker that reproduces the official `byte_exact` count exactly
(4,500 on baseline; zero-size exports counted exact).

Variants tested:

```text
-marm -mthumb -mfpu=vfpv3-d16 -mfpu=neon -march=armv7-a
-mtune=cortex-a8 -mtune=cortex-a9 -mcpu=cortex-a8 -mcpu=cortex-a9
-mno-unaligned-access -mrestrict-it -mno-restrict-it -mno-movt
-mno-neg-immediates -mno-madd4 -mcrc -mnocrc -mexecute-only
-mlong-calls -mstack-alignment=4 -mstack-alignment=8 -mstack-alignment=16
```

### 3.1.1 Exact command forms

Variant compile (the command is the literal `compile_commands.json` command
with the variant inserted before `-o`):

```sh
cd /home/debian/GalaxyOnFire2/wt/w5205/cmake-build-match
# <cmd> is the compile_commands.json command for the TU
<cmd-with-"-o <build-obj>"-replaced-by-"-o /dev/shm/w5205/obj/<tag>.o">   -c /home/debian/GalaxyOnFire2/wt/w5205/<tu>
```

Relink one variant object without letting Ninja recompile it:

```sh
cd /home/debian/GalaxyOnFire2/wt/w5205/cmake-build-match
ninja -t deps CMakeFiles/gof2.dir/<tu>.obj | head -1   # gives "deps mtime <ns>"
cp /dev/shm/w5205/obj/<variant>.o CMakeFiles/gof2.dir/<tu>.obj
touch -d @<deps-mtime> CMakeFiles/gof2.dir/<tu>.obj
mv libgof2hdaa.so libgof2hdaa.so.bak
ninja link_so
python3 /dev/shm/w5205/fastbyte.py libgof2hdaa.so
```

`fastbyte.py` reads both ELFs' `.dynsym` (`tools/verify/elf.py`), extracts
each common function's bytes at `st_value & ~1` for `st_size` bytes, and
counts exact matches (zero-size exports counted exact). It reproduces the
official baseline exactly: `4500 4524 24`.

### 3.2 The 16 TUs containing the 24 non-exact functions

The 24 non-byte-exact functions live in 16 TUs:

```text
Layout.cpp LevelScript.cpp Status.cpp Generator.cpp Level.cpp Radar.cpp
MGame.cpp HangarWindow.cpp Globals.cpp RocketGun.cpp PlayerEgo.cpp Gun.cpp
Hud.cpp MenuTouchWindow.cpp SpaceLounge.cpp ModStation.cpp
```

All 22 variants on those 16 TUs: 352 compiles, **121 combinations changed
`.text`**, each relinked. Best byte_exact per TU:

| TU | best byte_exact | best variant | baseline |
|---|---:|---|---:|
| `src/game/ui/Layout.cpp` | 4499 | `-mcpu=cortex-a8` | 4500 |
| `src/game/weapons/RocketGun.cpp` | 4487 | `-mcpu=cortex-a9` | 4500 |
| `src/game/weapons/Radar.cpp` | 4472 | `-mno-unaligned-access` | 4500 |
| `src/game/core/Globals.cpp` | 4425 | `-mcpu=cortex-a8` | 4500 |
| `src/game/ship/PlayerEgo.cpp` | 4288 | `-mcpu=cortex-a9` | 4500 |
| `src/game/menu/MGame.cpp` | 4270 | `-mrestrict-it` | 4500 |
| `src/game/ui/Hud.cpp` | 4012 | `-mfpu=vfpv3-d16` | 4500 |
| `src/game/weapons/Gun.cpp` | 3793 | `-mrestrict-it` | 4500 |
| `src/game/ui/HangarWindow.cpp` | 3728 | `-mfpu=vfpv3-d16` | 4500 |
| `src/game/world/LevelScript.cpp` | 3683 | `-mrestrict-it` | 4500 |
| `src/game/menu/ModStation.cpp` | 3264 | `-mrestrict-it` | 4500 |
| `src/game/menu/SpaceLounge.cpp` | 3027 | `-mcpu=cortex-a8` | 4500 |
| `src/game/mission/Generator.cpp` | 2599 | `-mrestrict-it` | 4500 |
| `src/game/world/Level.cpp` | 2499 | `-mcpu=cortex-a8` | 4500 |
| `src/game/mission/Status.cpp` | 2337 | `-mno-unaligned-access` | 4500 |
| `src/game/ui/MenuTouchWindow.cpp` | 2765 | `-mrestrict-it` | 4500 |

No combination exceeds 4500. The only 4499 is `Layout.cpp -mcpu=cortex-a8`,
which changes `Layout::setWindowDimensions` (a previously exact function)
and does not fix `Layout::Layout`.

### 3.3 All 192 app TUs, codegen-changing variants

The eight variants that actually changed code were swept over all 192 TUs
(1,536 compiles). Rows with a non-zero `.text` size delta were relinked
individually (862 relinks):

| variant | TUs changed | TUs with size change | relinked | max byte | min byte |
|---|---:|---:|---:|---:|---:|
| `-marm` | 192 | 192 | 192 | 4500 | 1284 |
| `-mcpu=cortex-a8` | 94 | 66 | 66 | 4497 | 1408 |
| `-mcpu=cortex-a9` | 184 | 118 | 118 | 4500 | 1413 |
| `-mexecute-only` | 115 | 106 | 106 | 3382 | 1357 |
| `-mfpu=vfpv3-d16` | 88 | 82 | 82 | 4445 | 1587 |
| `-mlong-calls` | 189 | 189 | 189 | 4500 | 1345 |
| `-mno-unaligned-access` | 25 | 23 | 23 | 4472 | 1378 |
| `-mrestrict-it` | 175 | 86 | 86 | 4498 | 1433 |

No result exceeds 4500. The six 4500 rows are `Array.cpp` and
`ArrayInstantiations.cpp`; those TUs' weak/duplicate exported symbols are
resolved from `PaintCanvas.cpp` first, so changing them is gate-neutral.

`-mfpu=vfpv3-d16` fails to compile three TUs that include `arm_neon.h`
(`Engine.cpp`, `DrawFBOShader.cpp`, `Globals.cpp`); this is a known
NEON-intrinsics limitation, not a codegen result.

### 3.4 The other 14 variants

All 192 TUs were compiled with the remaining 14 variants (2,688 compiles).
**Every `.text.*` section was byte-identical to baseline** (0 changed
sections): `-mtune=cortex-a8`, `-mtune=cortex-a9`, `-mno-movt`,
`-mno-neg-immediates`, `-mno-madd4`, `-mcrc`, `-mnocrc`,
`-mstack-alignment=4/8/16`, `-mno-restrict-it`, `-march=armv7-a`,
`-mthumb`, `-mfpu=neon`.

## 4. Vendored C TUs

The vendored libraries are built outside the app TU list. Current modes and
byte-exactness:

| library | current mode | functions | byte-exact |
|---|---|---:|---:|
| zlib 1.2.3 | `-mthumb` | 121 (zlib+libzip together) | 121 / 121 |
| libzip 0.9.3 | `-mthumb` | (above) | 121 / 121 |
| libcrypto 1.1.0l (GCC 4.9) | ARM (`-march=armv7-a`, no `-mthumb`) | 23 | 23 / 23 |

Flipping the mode:

| experiment | byte_exact |
|---|---:|
| baseline | **4500** |
| zlib rebuilt `-marm` | 2215 |
| libzip rebuilt `-marm` | 1346 |
| libcrypto `sha256.c` rebuilt `-mthumb` (asm left ARM) | 3214 |

All are large regressions. The current per-library ISA modes are therefore
positively confirmed.

## 5. `.ARM.attributes`

The final `.so` `.ARM.attributes` is byte-identical to the original
(56 bytes, sha256 `b03afea6...`). A representative app object
(`Layout.cpp`) carries:

```text
Tag_CPU_arch: v7
Tag_CPU_arch_profile: Application
Tag_ARM_ISA_use: Yes
Tag_THUMB_ISA_use: Thumb-2
Tag_FP_arch: VFPv3
Tag_Advanced_SIMD_arch: NEONv1
Tag_CPU_unaligned_access: v6
```

Per-variant object attributes:

* `-marm` leaves `Tag_THUMB_ISA_use: Thumb-2` in the object (the target still
  supports Thumb-2); **attributes alone cannot prove the mode**, the dynsym
  value does.
* `-mfpu=vfpv3-d16` changes `Tag_FP_arch` to `VFPv3-D16` and drops
  `Tag_Advanced_SIMD_arch`.
* `-mno-unaligned-access` changes `Tag_CPU_unaligned_access` to `None`.

Because the final section is a merge, a single TU compiled with a lower FPU
or unaligned-access attribute would be invisible in the linked `.so`. The
per-TU sweeps above are what rule those choices out on codegen grounds.

## 6. Proven / disproven / unobservable

### Positively proven

1. All 192 app TUs are Thumb; no app TU is ARM.
2. The only ARM code in the original is the 23-function libcrypto subset,
   and it is already byte-exact.
3. zlib and libzip are Thumb and byte-exact; libcrypto is ARM and byte-exact.
4. No per-TU `-marm`, `-mfpu=vfpv3-d16`, `-mcpu=cortex-a8/a9`,
   `-mno-unaligned-access`, `-mrestrict-it`, `-mexecute-only`, or
   `-mlong-calls` improves the gate. The best observed is 4499.
5. `-mtune=cortex-a8/a9`, `-mno-movt`, `-mno-neg-immediates`, `-mno-madd4`,
   `-mcrc`, `-mnocrc`, `-mstack-alignment=4/8/16`, `-mno-restrict-it`,
   `-march=armv7-a`, `-mthumb`, and `-mfpu=neon` are codegen-identical to
   baseline on every app TU.
6. The final `.ARM.attributes` is byte-identical to the original.

### Disproven

1. Any app TU compiled `-marm` (all exported functions are Thumb).
2. zlib/libzip compiled `-marm` (2215 / 1346 vs 4500).
3. libcrypto `sha256.c` compiled `-mthumb` (3214 vs 4500).
4. Any of the tested per-TU variants being a strict improvement.

### Unobservable / unknown

1. The original's exact per-TU command lines are not recoverable from the
   stripped `.so`; only the merged `.ARM.attributes` survives.
2. A per-TU flag that is codegen-neutral on this source (e.g. `-mtune`,
   `-mno-movt`) could have been used without leaving a trace. The sweep
   proves it would not change the binary, not that it was absent.
3. Per-TU FPU/unaligned-access attributes below the merge maximum are
   invisible in the final `.so`; only their codegen effect is testable, and
   no tested value improves the gate.
4. Local (non-exported) functions have no symbols in the original. However,
   all 4,524 compared functions are size- and address-exact, so any local
   function with a different ISA/size would have shifted the exported
   addresses; no such shift exists.

## 7. Strict improvement

**None.** No tested per-TU ISA/ARM-mode variant raises `byte_exact` above
4500. The minimal change is therefore the note only; no source, build flag,
baseline, or allowlist change is made.

## 8. Confidence table

| Claim | Confidence | Hard evidence |
|---|---|---|
| All 192 app TUs Thumb | **proven** | 4,501 odd / 23 even defined `FUNC` symbols; 0/192 app TUs own an even-valued export |
| The 23 ARM functions are libcrypto | **proven** | exact address list; ARM disassembly (`e92d40f8 push`) |
| libcrypto is already byte-exact | **proven** | 23/23 raw-byte equal to original |
| zlib+libzip are Thumb and byte-exact | **proven** | 121/121 raw-byte equal; odd dynsym values |
| No per-TU `-marm` improvement | **proven** | 192/192 TUs swept; max 4500 |
| No per-TU variant improvement | **proven** | 16 failing TUs × 22 variants; 192 TUs × 8 codegen-changing variants; max 4500 |
| 14 variants are codegen-neutral | **proven** | 2,688 compiles, 0 differing `.text.*` sections |
| Final `.ARM.attributes` exact | **proven** | 56/56 bytes, sha256 `b03afea6...` |
| Exact original per-TU command lines | **unobservable** | stripped `.so`; only merged attributes survive |
| Codegen-neutral per-TU flags were absent | **unobservable** | no binary trace by definition |
