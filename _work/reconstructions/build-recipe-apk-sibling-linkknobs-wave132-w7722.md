# Build recipe: APK/OBB/sibling-port mining + untested gold link knobs (wave 132, w7722)

Status: research note, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7722`,
branch `wave132-w7722`, base `70e451ffe` ("PlayerJumpgate ctor extab wave 131").
Scratch: `/dev/shm/w7722`. **No `src/`, `tools/`, `CMakeLists.txt`, baseline or
allowlist change is landed.** This note is the only commit.

Gate at the base and after this pass (fresh `check`):

```text
verify: byte 4506/4524  linked 4506/4524  size 4524/4524  avg 99.94
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
```

Section deltas vs the original (2,187,328 B):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | 85,379 |
| `.ARM.extab` | 156,312 | 1,420 |
| `.note.gnu.build-id` | 36 | 20 |
| every other section | exact | **0** |

---

## 0. Bottom line

1. **No observable artifact contradicts the current recipe.** The APK, its
   OBB, the four bundled `.so` files, the second official 2.0.16 build and the
   four sibling ports all agree with the established NDK r18b / clang 7.0.2 /
   gold 1.12 / CMake / `-Oz -g` / static-libc++ recipe.
2. **The requested gold link knobs are all either neutral or disproven.**
   `--sort-section=none`, `-O1`, `--no-keep-memory`, `--sort-common` (all
   directions) are byte-identical to the canonical link. `--sort-section=name`
   (1,342 byte-exact) and any `--section-ordering-file` (1,345–1,543) reorder
   `.text` and are disproven. **`--incremental` is impossible**: gold rejects it
   with `-z relro`, and the original is RELRO/BIND_NOW.
3. **New artifact use: the second official 2.0.16 build independently confirms
   the source/TU order.** The clang-3.8/NDK-r14 `libgof2hdaa.so` from the
   KiritoJPK archive has the same app-function order as the r18b ground truth:
   **4,220 of 4,244 adjacent common-function pairs preserved**, and all 8
   remaining app breaks are weak/template (`Array<T>`, `Transform`, `GameText`)
   or `glError`/`Engine` instantiations, not TU-order changes. This is the
   first independent confirmation that `tools/source_order.txt` is a property
   of the source tree, not an artifact of the r18b link.
4. **The sibling ports contain no native Android ELF**, so there is no
   `.comment`/`.note.android.ident`/`.ARM.attributes` to compare. `gof2-vita`
   is a Vita *loader* for the SD 1.0.4 `lib/armeabi/libgof2.so` (sha1
   `66F317C81795FDF4C8D40D9E6E5C3BF85D602904`), `KaamoClubModApi` is a Windows
   `d3d9.dll` proxy (Rust), `gof2edit` is a Rust save editor, and
   `FMOD-GOF2-Recreated` is an FMOD project. The useful ELF comparison is
   therefore against the APK's own `libcrashlytics.so` / FMOD prebuilts and the
   old r14 build, all of which are consistent with the recipe.
5. **No gate improvement.** The remaining 18 functions are source-shape
   residuals; no link knob can change compiler-generated code or `.ARM.extab`.

---

## 1. APK / OBB / native-library inventory

APK: `/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk`
(143 ZIP entries, sha256 `9cfe4484...`). It contains exactly four
`lib/armeabi-v7a/*.so` files and **no** `Android.mk`, `Application.mk`,
`CMakeLists.txt`, `*.cmake`, `*.gradle`, `*.ninja`, `compile_commands.json`,
`Makefile`, `*.o`, `*.a`, `*.d`, or build log. All ZIP timestamps are
`1980-00-00 00:00` (zipalign/repack), so the archive itself carries no native
link timestamp.

| APK member | sha256 | build identity |
|---|---|---|
| `lib/armeabi-v7a/libgof2hdaa.so` | `6d38fd76...` | r18b / clang 7.0.2 / gold 1.12; bfd `.shstrtab` order (ground truth is the same file after `tools/reorder_shstrtab.py`) |
| `lib/armeabi-v7a/libcrashlytics.so` | `afbe76e6...` | GCC 4.9.x 20150123, gold 1.11, `libstdc++.so` NEEDED, **no** `.note.android.ident`; NDK r13b/r14-era prebuilt |
| `lib/armeabi-v7a/libfmodex.so` | `60ec4de0...` | GCC 4.4.3/4.6, ARMv5TE/Thumb-1/VFPv1, no build-id/gold note; FMOD prebuilt |
| `lib/armeabi-v7a/libfmodevent.so` | `b92cd6ff...` | GCC 4.4.3, ARMv5TE/Thumb-1/VFPv2; FMOD prebuilt |

The APK's Java-side metadata (`META-INF/MANIFEST.MF` `Created-By: Android
Gradle 3.1.2`; `assets/crashlytics-build.properties` `#Mon Oct 22 10:18:17
CEST 2018`; `fabric/*.properties`, `firebase-*.properties`,
`play-services-*.properties`) pins the 2018 Gradle/AGP/Fabric/Firebase era but
says nothing about the native compile or link line. It is already documented in
`build-provenance-external-wave128-w7640.md` §1–2 and is not repeated here.

**OBB.** `/home/debian/GalaxyOnFire2/KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android/GOF2FHD_OBB.7z`
contains one 1,625,421,767-byte `main.47947006...obb`. It is a ZIP with **2,460
files**, all under `47947_GOF2CONTENT_ETC/assets/data/...`; no `.so`, `.mk`,
`CMakeLists`, `*.properties`, `*.txt`, `*.json`, `*.xml`, `*.ini`, `*.cfg`,
or build log. The OBB is game content only.

**Sibling ports.** A full file scan of `../gof2-vita`, `../KaamoClubModApi`,
`../gof2edit`, `../FMOD-GOF2-Recreated` found **no native ARM ELF, `.a`,
`.o`, `.dll`, `.exe`, `.lib`, or `.dylib`** (the only >100 kB non-text files
are two `.aei` assets in `KaamoClubModApi`). Consequently there is nothing to
compare on `.comment`/`.note.android.ident`/`.ARM.attributes`:

| repo | what it is | native build evidence |
|---|---|---|
| `gof2-vita` | Vita loader for the **SD 1.0.4** `lib/armeabi/libgof2.so` (sha1 `66F317C8...`), ARMv6, `ndk23_*` JNI names | only its own `CMakeLists.txt` (VitaSDK, `-O3 -g -ffast-math`); not the HD lib |
| `KaamoClubModApi` | Windows `d3d9.dll` proxy + mod API (Rust `cdylib`, `/DEF:d3d9.def`) | `Cargo.toml`/`build.rs`; no Android toolchain |
| `gof2edit` | Rust CLI save/`.bin` editor | `Cargo.toml`; no native game build |
| `FMOD-GOF2-Recreated` | FMOD Designer project (`.fdp`/`.fdt`) and FSB tools | no compiled library |

The only sibling that names a game `.so` is `gof2-vita`, and it is the
**non-HD** ARMv6 `libgof2.so`, not `libgof2hdaa.so`; its `ndk23_*` symbols are
the r14-era JNI names, consistent with the old 2.0.16 build below.

---

## 2. ELF metadata comparison

### 2.1 Ground truth vs our build

`readelf -A` is byte-for-byte identical between the original and the canonical
build:

```text
Tag_CPU_name: "ARM v7"
Tag_CPU_arch: v7
Tag_THUMB_ISA_use: Thumb-2
Tag_FP_arch: VFPv3
Tag_ABI_align_needed: 8-byte
Tag_ABI_enum_size: int
Tag_CPU_unaligned_access: v6
```

`.comment` is also identical (clang 7.0.2 string at offset 1, GCC 4.9.x string
at offset 108), and `.note.android.ident` is the r18b API-16 note. Nothing in
the attributes contradicts `-march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp`.

### 2.2 APK's other libraries

`libcrashlytics.so` is the only other r7-era ARMv7 ELF: `Tag_CPU_name "ARM v7"`,
Thumb-2, **VFPv3-D16** (no NEON), `Tag_ABI_optimization_goals: Aggressive
Speed`, gold 1.11, `libstdc++.so` NEEDED, no `.note.android.ident`. It is a
Crashlytics NDK 2.0.5 prebuilt, not a Fishlabs build. The FMOD libraries are
ARMv5TE/Thumb-1/VFPv1–2 prebuilts. Neither is evidence for the game lib's
flags beyond the era.

### 2.3 Second official 2.0.16 build (clang 3.8 / NDK r14)

`KiritoJPK_.../GOF2FHD_APK.7z` → `Experimental/New system/GOF2_2.0.16_Original_Price.apk`
→ `lib/armeabi-v7a/libgof2hdaa.so`, 2,469,656 B, sha256
`9183a68b3a76ce83880df2c044b694f21969c9fb5df190f2b1abd26e5b9c3187`:

```text
.comment: GCC 4.9.x 20150123 + Android clang version 3.8.275480
          (based on LLVM 3.8.275480)
gold 1.11, no .note.android.ident, libstdc++.so NEEDED
.text 0x1682c8 @ 0x61940, .rodata 0x427d4, .data 0x2944, .bss 0x6854
ARM: v7, Thumb-2, VFPv3, Tag_ABI_optimization_goals: Aggressive Speed
```

The `Aggressive Speed` tag comes from the GCC-built gnustl runtime, not from
`-O2`; our own `-O2` build (clang/libc++) has no such tag. This is an STL
discriminator, not an optimization-level one.

**New result: the app-function order matches the r18b ground truth.** Sorting
both `.dynsym` tables by address and comparing the 4,245 common functions:

```text
adjacent common-function pairs preserved: 4,220 / 4,244
app-function breaks: 8, all weak/template or helper instantiations:
  Transform::Transform() -> Transform::Transform(Transform*)
  Array<unsigned>::~Array() -> Array<Mesh*>::~Array()
  Array<Mesh*>::~Array() -> Array<SpriteSystem*>::~Array()
  Array<Camera*>::~Array() -> Array<Transform*>::~Array()
  Array<Transform*>::~Array() -> Array<Image2D*>::~Array()
  GameText::GameText() -> GameText::~GameText()
  glError() -> Array<int>::~Array()
  Array<int>::~Array() -> Engine::Engine()
```

Every other break is in libc++abi/libstdc++/libgcc code (`_ZNSt*`, `__cxa*`,
`_ZSt*`, `sha256*`, `_Znwj`, `_ZdaPv*`), whose order depends on the runtime
archive, not the app TU list. The non-template app functions are in exactly the
same order in both official builds. This is an independent confirmation of the
TU membership and `tools/source_order.txt`.

The old build is **not** useful for the 18 non-exact functions: it is a
different compiler (`clang 3.8`) and a different optimization level (its
`.text` is 17% larger, consistent with `-O2`), so its instruction selection is
not comparable byte-for-byte.

---

## 3. Untested gold link knobs

### 3.1 Method

The canonical link command is the `libgof2hdaa.so` custom command in
`cmake-build-match/build.ninja`. It was extracted with

```sh
cd cmake-build-match
ninja -t commands libgof2hdaa.so | tail -1
```

and replayed with an extra flag inserted immediately after `ld.gold`, then the
same GNU `strip --strip-all` and `tools/reorder_shstrtab.py`. The helper used
(`/dev/shm/w7722/relink.py`) is:

```python
import subprocess, sys
WT='/home/debian/GalaxyOnFire2/wt/w7722'; BD=WT+'/cmake-build-match'
cmd=open('/dev/shm/w7722/link_cmd.txt').read().strip().splitlines()[-1]
parts=[p.strip() for p in cmd.split(' && ')]
gold,strip,reorder=parts[1],parts[2],parts[3]
extra=sys.argv[1]; out=sys.argv[2]
gold=gold.replace('ld.gold ','ld.gold '+extra+' ',1)
gold=gold.replace(BD+'/libgof2hdaa.unstripped.so',out+'.unstripped')
strip=strip.replace(BD+'/libgof2hdaa.so',out).replace(BD+'/libgof2hdaa.unstripped.so',out+'.unstripped')
reorder=reorder.replace(BD+'/libgof2hdaa.unstripped.so',out+'.unstripped').replace(BD+'/libgof2hdaa.so',out)
for c in (gold,strip,reorder):
    r=subprocess.run(c,shell=True,cwd=BD,capture_output=True,text=True)
    if r.returncode: print(c); print(r.stderr); sys.exit(1)
```

`byte_exact`/`linked_exact` were measured with `tools/verify/verify.py` against
the original. The canonical baseline is `.so` sha256
`60fe2dda7f2fd1a21240d3ad4f4b693b796d91782fd0f485ba590db83be3c6b7`,
byte 4506 / linked 4506.

### 3.2 Results

| extra gold flag | `.so` sha256 | byte_exact | linked_exact | verdict |
|---|---|---:|---:|---|
| *(none, canonical)* | `60fe2dda...` | 4506 | 4506 | baseline |
| `--sort-section=none` | `60fe2dda...` | 4506 | 4506 | **neutral** (identical) |
| `-O1` | `60fe2dda...` | 4506 | 4506 | **neutral** (identical) |
| `-O1 --sort-section=none` | `60fe2dda...` | 4506 | 4506 | neutral |
| `-O1 --no-keep-memory` | `60fe2dda...` | 4506 | 4506 | neutral |
| `--no-keep-memory` | `60fe2dda...` | 4506 | 4506 | **neutral** (identical) |
| `--sort-common` | `60fe2dda...` | 4506 | 4506 | neutral |
| `--sort-common=ascending` | `60fe2dda...` | 4506 | 4506 | neutral |
| `--sort-common=descending` | `60fe2dda...` | 4506 | 4506 | neutral |
| `--sort-section=name` | `b7937fd2...` | **1342** | 4504 | disproven |
| `--sort-section=name --no-text-reorder` | `b7937fd2...` | 1342 | 4504 | disproven |
| `-O2` | `831915b1...` | **1720** | 4506 | disproven (wrong layout) |
| `--no-text-reorder` | `331eec6f...` | 1543* | 4506* | disproven |
| `--section-ordering-file=<empty>` | `331eec6f...` | 1543* | 4506* | disproven |
| `--section-ordering-file=<.text>` | `a120b974...` | 1345 | 4506 | disproven |
| `--section-ordering-file=<one .text._ZN3NFCC2Ev>` | `679ad744...` | — | — | disproven (layout changes) |
| `--incremental` | — | — | — | **link error**: `incremental linking is not compatible with -z relro` |

`*` `--no-text-reorder` and an empty section-ordering file produce the same
`.so`; `byte_exact` 1543 was measured on the empty-file variant.

### 3.3 Interpretation

* The original is consistent with gold's default `--sort-section=none`; the
  explicit flag is byte-identical, so the recipe's "no `--sort-section`" claim
  is confirmed (the default and the explicit spelling are indistinguishable).
* `-O1` is a no-op for this link. `-O2` changes the section layout and drops
  `byte_exact` to 1,720, so the original was not linked with `-O2`.
* `--no-keep-memory` and `--sort-common` are pure implementation knobs and are
  byte-neutral.
* Any section-ordering file reorders `.text` away from the original's plain
  object input order; the empty-file result (1,543) is the same as name-sorting.
* `--incremental` cannot have been used: gold's incremental mode is
  incompatible with `-z relro`, and the original has `PT_GNU_RELRO`,
  `DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`. This is a **hard exclusion**, not just
  a neutral result.

No link knob changed `.ARM.extab` or `.ARM.exidx`; those are compiler-emitted
and remain the source-shape residual (1,420 differing `.ARM.extab` bytes).

---

## 4. What is confirmed / contradicted

**Confirmed:**

* NDK r18b / clang 7.0.2 / gold 1.12 / API 16 / armeabi-v7a / Thumb-2 / NEON /
  softfp / static libc++ (`-Oz -g`).
* No `-fdata-sections`, no `-fvisibility=hidden`, no section-ordering file,
  no `--sort-section`, no `-O2`, no `--incremental`.
* The link order `crtbegin → app objects (source_order.txt) → whole zlib/libzip
  → log/GLES/EGL/android → FMOD → libcrypto → m → libc++abi →
  libandroid_support → libgcc → dl → c → crtend`, then GNU strip + `.shstrtab`
  reorder.
* The source/TU order is stable across the two official 2.0.16 builds (r18b
  CMake and r14 ndk-build/gnustl).

**Not contradicted by any artifact:**

* CMake (rather than ndk-build) for the r18b build: no `-fdata-sections` and
  the STL archives after the user libraries remain the two discriminators.
* The exact CMake version, generator, build directory and host macOS version
  remain unobservable (the shipped `.so` is stripped).

**No residual change.** This pass lands only this note. The 18 non-byte-exact
functions are source-shape residuals; the build recipe is not the cause.

---

## 5. Reproduction commands

```sh
# baseline
cd /home/debian/GalaxyOnFire2/wt/w7722
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# APK / OBB inventory
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -Z1 "$APK" | grep -Ei '\.so$|\.obb$|AndroidManifest|\.dex$|\.arsc$|\.mk$|CMake|gradle'
unzip -l "$APK" | grep -E 'lib/|assets/'

# native-library metadata
for l in libcrashlytics libfmodex libfmodevent libgof2hdaa; do
  unzip -p "$APK" lib/armeabi-v7a/$l.so > /dev/shm/w7722/apk_$l.so
  readelf -p .comment /dev/shm/w7722/apk_$l.so
  readelf -n /dev/shm/w7722/apk_$l.so
  readelf -A /dev/shm/w7722/apk_$l.so
done

# OBB
7z x -y /home/debian/GalaxyOnFire2/KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android/GOF2FHD_OBB.7z \
  -o/dev/shm/w7722/obb
unzip -l /dev/shm/w7722/obb/Android/obb/*/*.obb | grep -Ei '\.so$|\.mk$|CMake|gradle|\.properties$'

# old r14 build
7z x -y /home/debian/GalaxyOnFire2/KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android/GOF2FHD_APK.7z \
  'Experimental/New system/GOF2_2.0.16_Original_Price.apk' -o/dev/shm/w7722/oldapk
unzip -p '/dev/shm/w7722/oldapk/Experimental/New system/GOF2_2.0.16_Original_Price.apk' \
  lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/w7722/old_r14.so
readelf -p .comment /dev/shm/w7722/old_r14.so
readelf -A /dev/shm/w7722/old_r14.so

# link-knob A/B
cd cmake-build-match && ninja -t commands libgof2hdaa.so | tail -1 > /dev/shm/w7722/link_cmd.txt
python3 /dev/shm/w7722/relink.py --sort-section=none /dev/shm/w7722/sortnone.so
python3 /dev/shm/w7722/relink.py -O1 /dev/shm/w7722/O1.so
python3 /dev/shm/w7722/relink.py -O2 /dev/shm/w7722/O2.so
python3 /dev/shm/w7722/relink.py --no-keep-memory /dev/shm/w7722/nokeep.so
python3 /dev/shm/w7722/relink.py --sort-section=name /dev/shm/w7722/sortname.so
python3 /dev/shm/w7722/relink.py --incremental /dev/shm/w7722/incr.so   # fails: -z relro
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w7722/verify_x \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w7722/O1.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --top 0
```

---

## 6. Files touched

* `_work/reconstructions/build-recipe-apk-sibling-linkknobs-wave132-w7722.md`
  (this note).
* Scratch only: `/dev/shm/w7722/` (`relink.py`, `vfy.sh`, `symorder*.py`,
  `base.so`, `sortnone.so`, `sortname.so`, `O1.so`, `O2.so`, `nokeep.so`,
  `sortcommon*.so`, `emptyorder.so`, `ordertext.so`, `orderone.so`,
  `old_r14.so`, `apk_*.so`, `obb_list.txt`, `link_cmd.txt`).
