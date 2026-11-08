# Link-time provenance: ICF / LTO / gc-sections / sort-section / -z now / build-id

Status: research note, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w10118`,
base `f4e0ea40b` (wave 154). Scratch: `/dev/shm/w10118`. **No `src/`, `CMakeLists.txt`,
baseline or allowlist change is landed**; this note plus the harness scripts are the
only commit. The canonical `cmake-build-match` was rebuilt and `check` re-run in this
worktree before and after the experiments; the gate is unchanged.

## 0. Bottom line

The original is a **plain, non-LTO, non-ICF, non-gc-sections gold 1.12 link** with
`-z now` and `--build-id`, exactly the canonical link. Every remaining link-time
hypothesis is disproven by a controlled relink of the already-built objects:

| hypothesis | evidence | gate effect (byte/linked of 4524) |
|---|---|---|
| gold `--icf=all` | original has 88 groups of byte-identical bodies at **distinct** addresses (427 functions, 1,126 foldable app bytes); ICF would fold them | **1347 / 4506** (was 4507/4507); `.text` -15,916 B, `.ARM.exidx` -6,160 B |
| gold `--icf=safe` | same 88 groups; safe folds them too | **1347 / 4507**; `.text` -11,364 B, `.ARM.exidx` -5,584 B |
| LTO (`-flto`) | original `.text`/`.rodata` are byte-identical to the non-LTO build; a full `-flto` build removes 355 exported symbols and changes `.rodata` by -5,872 B | **~1101 byte-exact of 4,168 common** (fast counter), size-exact 3,092; 355 symbols absent; `.text` -25,540 B, `.rodata` -5,872 B |
| `--gc-sections` | original keeps unreferenced local functions (284 more `.ARM.exidx` entries than a gc-sections link) | **2177 / 4507**; `.text` -52 B, `.ARM.exidx` -2,272 B, `.rodata` -16 B, `.bss` -16 B |
| `--sort-section=name` | original `.text` is object input order, not name order | **1342 / 4505**; `.text` +68 B, `.ARM.exidx` -1,728 B |
| no `-z now` | original `.got` (0x20fcf4) is **before** `.data` (0x214000), which gold only emits with `-z now` | **2932 / 4507**; `.got` moves after `.data` |
| no `--build-id` | original has a 36-byte `.note.gnu.build-id` at 0x1cc before `.text` | **1750 / 4507**; every following section shifts -36 B |
| `.note.gnu.gold-version` / `.comment` / `.ARM.attributes` | byte-identical in the canonical link | 0 diff bytes |

The canonical link is therefore already the original's effective link. No link-flag
change is landed because none raises `byte_exact`; the harness and this note are the
deliverable.

## 1. Baseline and harness fidelity

Canonical gate in this worktree (before and after all experiments):

```text
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Original `sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 B. The harness (`relink_variant.sh`) extracts the canonical gold command
verbatim from `cmake-build-match/build.ninja`, changes only the output path and inserts
the test flags after `--build-id`, then applies the same GNU `strip --strip-all` and
`tools/reorder_shstrtab.py`. The zero-flag control (`base`) reproduces the canonical
section fingerprint exactly:

| section | original | base relink | diff bytes |
|---|---:|---:|---:|
| `.text` | 1,259,464 | 1,259,464 | 86,347 |
| `.ARM.exidx` | 28,528 | 28,528 | 0 |
| `.ARM.extab` | 156,312 | 156,312 | 1,255 |
| `.rodata` | 269,159 | 269,159 | 0 |
| `.rel.plt` | 29,392 | 29,392 | 0 |
| `.note.gnu.build-id` | 36 | 36 | 20 |
| `.comment` / `.note.gnu.gold-version` / `.ARM.attributes` | 303 / 28 / 56 | same | **0 / 0 / 0** |
| `.dynsym` / `.dynstr` / `.hash` / `.rel.dyn` / `.got` / `.data` / `.bss` | sizes exact | sizes exact | **0** |

File size 2,187,328 B in both. The harness is faithful, so every variant below differs
from the original only by the named link flag.

## 2. ICF: disproven

### 2.1 Direct evidence in the original

`icf_probe.py` parses the original `.dynsym` and hashes each defined function's bytes:

* 4,524 `FUNC` symbols, 4,162 distinct addresses.
* 354 multi-symbol addresses are the compiler-emitted C1/C2 and D1/D2 aliases (same
  address, same body) — e.g. `_ZN3NFCC1Ev`/`_ZN3NFCC2Ev`. These are not ICF; the
  canonical non-ICF build has them at the same addresses.
* **88 groups of byte-identical bodies at distinct addresses**, covering 427 functions
  and 1,126 foldable app bytes. Examples: `MiningGame::up` / `MiningGame::down` (18 B),
  `PlayerEgo::GetUpVector` / `PlayerEgo::GetDirVector` (12 B), `ListItem::isShip` /
  `Route::waypointDefined` / `Radio::isShowingMessage` (10 B).
* The original's `.ARM.exidx` has 3,566 entries (28,528 B); an ICF link has 2,796
  (22,368 B). The original keeps every function's own exidx entry.

If the original had used `--icf=all`, those 88 app groups (and many archive groups)
would share one address and one exidx entry. They do not.

### 2.2 Controlled relink

| variant | `.text` | `.ARM.exidx` | byte/linked | note |
|---|---:|---:|---|---|
| base (no ICF) | 1,259,464 | 28,528 | **4507 / 4507** | exact |
| `--icf=all` | 1,243,548 (-15,916) | 22,368 (-6,160) | **1347 / 4506** | folds app + archive bodies |
| `--icf=safe` | 1,248,100 (-11,364) | 22,944 (-5,584) | **1347 / 4507** | same app fold count |

`--icf=all` also changes `.plt`/`.got`/`.rel.plt` slot order through the merged
functions, which is why `linked_exact` dips by one even though the instruction
sequences of the surviving bodies are unchanged.

## 3. LTO: disproven

A full `-flto` build was made in `/dev/shm/w10118/lto`:

```sh
cmake -S "$REPO" -B /dev/shm/w10118/lto -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$REPO/cmake/toolchain.cmake" \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT='-Oz -flto'
cmake --build /dev/shm/w10118/lto --target gof2 -j6
BUILD_DIR=/dev/shm/w10118/lto relink_variant.sh /dev/shm/w10118/variants lto \
  -plugin "$REPO/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/bfd-plugins/LLVMgold.so"
```

Result (`lto.so`, 2,089,008 B = -98,320 B):

| section | original | LTO build | delta |
|---|---:|---:|---:|
| `.text` | 1,259,464 | 1,233,924 | **-25,540** |
| `.ARM.exidx` | 28,528 | 25,416 | -3,112 |
| `.ARM.extab` | 156,312 | 142,852 | -13,460 |
| `.rodata` | 269,159 | 263,287 | **-5,872** |
| `.rel.plt` | 29,392 | 19,752 | -9,640 |
| `.dynsym` | 89,248 | 83,568 | -5,680 |
| `.data` / `.bss` | 10,604 / 27,052 | 10,588 / 27,036 | -16 / -16 |

Symbol parity fails: `sodiff` reports **355 absent** defined symbols (5,370 original vs
5,015 LTO), e.g. `AEPakFile::Write`, `AbstractGun::isAutoTurret`, `AbstractGun::isBombGun`
— LTO internalized/inlined them away. The official verifier was stopped after ~11 min
because LTO's cross-TU inlining creates very large functions; `fast_exact.py` (same
byte rule: same name, size > 0 both, bytes at the symbol's own address) gives
**1,101 byte-identical of 4,168 common functions**, 3,092 size-exact.

The decisive evidence is simpler: the original `.rodata` is **byte-identical** to the
non-LTO canonical build (0/269,159). LTO performs cross-TU constant merging and
internalization, which changes `.rodata` by -5,872 B here. The original also has all
5,370 defined symbols and the non-LTO `.text` order; LTO has neither.

## 4. `--gc-sections`: disproven

| variant | `.text` | `.ARM.exidx` | `.rodata` | `.bss` | byte/linked |
|---|---:|---:|---:|---:|---|
| base | 1,259,464 | 28,528 | 269,159 | 27,052 | **4507 / 4507** |
| `--gc-sections` | 1,259,412 (-52) | 26,256 (-2,272) | 269,143 (-16) | 27,036 (-16) | **2177 / 4507** |

Gold drops 284 `.ARM.exidx` entries (unreferenced local functions) and 52 text bytes.
The original keeps them, so its link did not use `--gc-sections`.

## 5. `--sort-section=name`: disproven

| variant | `.text` | `.ARM.exidx` | `.rodata` | `.bss` | byte/linked |
|---|---:|---:|---:|---:|---|
| base | 1,259,464 | 28,528 | 269,159 | 27,052 | **4507 / 4507** |
| `--sort-section=name` | 1,259,532 (+68) | 26,800 (-1,728) | 269,136 (-23) | 27,060 (+8) | **1342 / 4505** |

Gold 1.12 only accepts `none`/`name` (`--help`: `--sort-section [none,name]`); the
original `.text` is plain object input order, which is already reproduced by
`tools/source_order.txt`. Sorting by name scrambles that order.

## 6. `-z now` and `--build-id`: proven present

`-z now` is load-bearing for the **section order**, not just `DT_FLAGS`:

| file | `.data.rel.ro` | `.dynamic` | `.got` | `.data` |
|---|---:|---:|---:|---:|
| original | 0x20cbb0 | 0x20fba4 | **0x20fcf4** | 0x214000 |
| base (with `-z now`) | 0x20cbb0 | 0x20fba4 | **0x20fcf4** | 0x214000 |
| no `-z now` | 0x20ced0 | 0x20fec4 | 0x21296c | 0x210000 |

Without `-z now`, gold puts `.got` after `.data`; the gate drops to **2932 / 4507**.
The original's dynamic tags (`FLAGS BIND_NOW`, `FLAGS_1 NOW`) and `PT_GNU_RELRO`
containing `.got` agree.

`--build-id` is proven by the 36-byte `.note.gnu.build-id` at 0x1cc. Removing it
(no `--build-id`) shifts every following section by -36 B and drops the same-address
byte count to **1750 / 4507** (`linked_exact` stays 4507 because addresses are
normalized). The original's build-id value itself is not reproducible from the
stripped file (it hashes the unstripped link output, including the build directory);
see `toolchain-runtime-provenance-wave120-w7007.md` §6.

## 7. Note/comment/attribute strings: byte-identical

The canonical link already reproduces these exactly (0 differing bytes):

```text
.comment                303 B  "Android (4751641 based on r328903) clang version 7.0.2 ..."
                                "GCC: (GNU) 4.9.x 20150123 (prerelease)"
.note.gnu.gold-version   28 B  "GNU" / "gold 1.12"
.note.android.ident     152 B  "Android" / "r18b" / "5063045" (API 16)
.ARM.attributes          56 B  ARM v7, Thumb-2, VFPv3, NEONv1, softfp
```

There is no `.note.gnu.gold-version`-style string that distinguishes an LTO link
(gold still emits it), so these sections confirm the toolchain but not the LTO/ICF
question; the code/data sections above decide that.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10118
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6

# ICF evidence in the original
python3 _work/reconstructions/w10118/icf_probe.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# controlled relinks (each prints the section sizes and the verify summary)
_work/reconstructions/w10118/run_variant.sh icfall  --icf=all
_work/reconstructions/w10118/run_variant.sh icfsafe --icf=safe
_work/reconstructions/w10118/run_variant.sh gcs     --gc-sections
_work/reconstructions/w10118/run_variant.sh sortname --sort-section=name
DROP='-z now'    _work/reconstructions/w10118/run_variant.sh noznow
DROP='--build-id' _work/reconstructions/w10118/run_variant.sh nobuildid

# full LTO (see §3); then verify with fast_exact.py
python3 _work/reconstructions/w10118/fast_exact.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  /dev/shm/w10118/variants/lto.so
```

Harness scripts committed alongside this note:

* `_work/reconstructions/w10118/relink_variant.sh` — verbatim canonical gold command
  from `build.ninja` + extra flags, then strip + `reorder_shstrtab.py`.
* `_work/reconstructions/w10118/run_variant.sh` — relink + `verify.py` + `sodiff.py`.
* `_work/reconstructions/w10118/icf_probe.py` — duplicate-address / identical-body
  probe for the original.
* `_work/reconstructions/w10118/fast_exact.py` — fast byte/size counter for variants
  whose full fuzzy verify is too slow (LTO).

## 9. Conclusion

The canonical single plain gold link is the original's effective link. The remaining
17 non-byte-exact functions and the 86,347 `.text` / 1,255 `.ARM.extab` residual bytes
are source-shape residuals, not link-flag or LTO/ICF residuals. No link flag is landed;
the full gate stays **4507/4524 byte, 4507/4524 linked, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370**.

Measured on the canonical build after the experiments (unchanged):

* raw same-address `.text` diff **86,347 B** (unique per-function sum; the 17 residual
  symbols sum to 92,239 because the `Layout` C1/C2 and `ModStation` C1/C2 aliases are
  counted twice).
* instruction-index aligned diff **25,959** over all 17 symbols, **23,941** over the
  15 unique functions (the wave-154 figure).
* file size **2,187,328 B** (exact), `.ARM.extab` 1,255 differing bytes (size-exact).
