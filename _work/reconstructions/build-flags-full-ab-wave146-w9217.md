# Build flags: independent full-build A/B re-verification (wave 146, w9217)

Status: **note-only research commit**, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w9217`, base `31297fadc` ("Wave 145 docs:
LevelScript 54,895, Radar 8,373, MGame 4,180, .text 85,651"). Scratch:
`/dev/shm/w9217`. No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or
doc change is landed. This note plus the harness script are the only commit.

The mission: the user has "no clue whether the flags are right". This pass
re-verifies the compile-flag set **independently**, from a clean full-build A/B,
rather than re-reading the prior notes. Every cell is a fresh build directory,
a full 192-TU compile, a full gold link, and the official per-function
verifier. The answer is unchanged but now independently measured on the
wave-145 tree: **the canonical flag set is the original's, and no single flag
raises `byte_exact`.**

Gate at the base (fresh `cmake --build cmake-build-match --target check -j6`):

```text
verify: byte 4507/4524  linked 4507/4524  size 4524/4524  avg 99.93
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
RATCHET PASS
```

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
2,187,328 B, `sha256 d4c17be6…bed9483`. Canonical build (this worktree):
`cmake-build-match/libgof2hdaa.so`, `sha256 3393253b…9c6a72`.

Section deltas of the canonical build (the control cell below reproduces them
exactly):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | **85,651** |
| `.ARM.extab` | 156,312 | **1,370** |
| `.note.gnu.build-id` | 36 | **20** (build-dir dependent; see §4) |
| `.rel.plt`, `.ARM.exidx`, `.rodata`, `.data`, `.bss`, all others | exact | **0** |

---

## 0. Bottom line

1. **21 single-flag full builds (plus the empty control) were run.** The
   canonical `-Oz -g` build and the six unobservable variants are the only cells
   that reach `byte_exact 4507`; no flag exceeds it. Every flag that changes
   codegen lowers `byte_exact`; every flag that keeps `byte_exact 4507` is
   byte-identical to the canonical build except for the build-id (a
   build-directory artifact).
2. **Six flags are genuinely unobservable** on this tree: `-g3`,
   `-fno-omit-frame-pointer`, `-mtune=cortex-a8`, `-mno-movt`,
   `-fno-delete-null-pointer-checks`, `-fno-vectorize`. They change no
   allocatable byte at all.
3. **One flag is codegen-neutral but artifact-falsified:**
   `-mfpu=neon-vfpv4` leaves `.text`/`.ARM.extab`/`byte_exact` unchanged but
   grows `.ARM.attributes` from 56 to 58 bytes (`Tag_FP_arch: VFPv4`,
   `Tag_Advanced_SIMD_arch: NEONv1 with Fused-MAC`). The original is
   `VFPv3`/`NEONv1`, i.e. `-mfpu=neon` is the faithful spelling.
4. **The other 14 tested flags are falsified** (they change `.text` and/or
   `.ARM.extab` and drop `byte_exact`). `-fno-common` does not even link
   (`multiple definition of g_android_back_button_pressed`), which proves the
   original used the clang-7 default `-fcommon`.
5. **Independent cross-check:** this full-build matrix reproduces wave 123's
   numbers exactly for the overlapping cells (`-O1` 1033, `-O2` 1168, `-O3`
   1167, `-Os` 1263, `-g0` 2071). That is strong evidence the harness and the
   scoring are sound, and that the residual 17 functions are source shape, not
   build configuration.

---

## 1. Method

The harness is `_work/reconstructions/build_flag_ab_w9217.py` (committed with
this note). It drives the in-tree `tools/flag_fingerprint.py` variant matrix:

* one **fresh build directory per cell** (`/dev/shm/w9217/flag-ab/build-<name>`);
* a toolchain wrapper (`toolchain_extra.cmake`) that includes the canonical
  `cmake/toolchain.cmake` and appends exactly one flag to `CMAKE_CXX_FLAGS` /
  `CMAKE_C_FLAGS` **after** the canonical `-Oz -g` line, so the cell flag wins
  for every last-wins option (`-O*`, `-g*`, `-f(no-)rtti`, `-f(no-)exceptions`,
  `-f(no-)stack-protector`, `-march`, `-mfpu`, …);
* a full `link_so` (all 192 app TUs + the three vendored archives + gold link +
  GNU strip + `.shstrtab` reorder);
* the official `tools/verify/verify.py` in fast mode. Fast mode only
  short-circuits the difflib fuzzy percentage; `byte_exact` is the exact
  `ob == ub` comparison and is unaffected. This was validated against a full
  official verify on two cells: `-g0` → 2071 and `-fno-rtti` → 1701, exactly
  the fast-mode numbers.

**Serial only (`--jobs 1`).** The vendored zlib/libzip/libcrypto archives live
in the source tree (`third_party/*/lib*_gof2.a`), so concurrent variant builds
race on the shared archive build. A first 8-way parallel run corrupted the
OpenSSL ARM assembly build (`sha256-armv4.S:1874: error: unterminated #if`) and
falsely reported `-O1` as a build failure. The serial run is clean; the harness
defaults to `--jobs 1` and documents why.

`rawdiff` in this note is the **same-address differing-byte count** of the
section (the mission's primary metric). For a cell whose section size differs
from the original, a same-address diff is not defined; the table reports the
size delta instead (`n/a (±D)`).

---

## 2. The 21-flag matrix (full build, serial)

Original `.text` = 1,259,464 B; original `.ARM.extab` = 156,312 B. `byte_exact`
is out of 4,524. `control` is the empty flag string.

| flag | byte_exact | `.text` size (Δ) | `.text` diff | `.ARM.extab` size (Δ) | extab diff | verdict |
|---|---:|---:|---:|---:|---:|---|
| `(none)` control | **4507** | 1,259,464 (+0) | 85,651 | 156,312 (+0) | 1,370 | canonical |
| `-O1` | 1033 | 1,326,856 (+67,392) | n/a (+67,392) | 156,288 (−24) | n/a (−24) | falsified |
| `-O2` | 1168 | 1,455,400 (+195,936) | n/a (+195,936) | 151,696 (−4,616) | n/a (−4,616) | falsified |
| `-O3` | 1167 | 1,513,560 (+254,096) | n/a (+254,096) | 152,648 (−3,664) | n/a (−3,664) | falsified |
| `-Os` | 1263 | 1,359,496 (+100,032) | n/a (+100,032) | 149,356 (−6,956) | n/a (−6,956) | falsified |
| `-g0` | 2071 | 1,259,464 (+0) | 804,437 | 156,312 (+0) | 1,683 | falsified |
| `-g3` | **4507** | 1,259,464 (+0) | 85,651 | 156,312 (+0) | 1,370 | **UNOBSERVABLE** |
| `-fno-omit-frame-pointer` | **4507** | 1,259,464 (+0) | 85,651 | 156,312 (+0) | 1,370 | **UNOBSERVABLE** |
| `-fomit-frame-pointer` | 1298 | 1,236,584 (−22,880) | n/a (−22,880) | 136,428 (−19,884) | n/a (−19,884) | falsified |
| `-fno-stack-protector` | 1369 | 1,237,224 (−22,240) | n/a (−22,240) | 155,772 (−540) | n/a (−540) | falsified |
| `-fstack-protector-all` | 83 | 1,374,056 (+114,592) | n/a (+114,592) | 159,932 (+3,620) | n/a (+3,620) | falsified |
| `-fno-rtti` | 1701 | 1,259,464 (+0) | 177,478 | 156,312 (+0) | 3,342 | falsified |
| `-fno-exceptions` | 1351 | 1,168,036 (−91,428) | n/a (−91,428) | 30,732 (−125,580) | n/a (−125,580) | falsified |
| `-fno-threadsafe-statics` | 1368 | 1,259,140 (−324) | n/a (−324) | 156,252 (−60) | n/a (−60) | falsified |
| `-march=armv7ve` | 1391 | 1,259,016 (−448) | n/a (−448) | 156,092 (−220) | n/a (−220) | falsified |
| `-mfpu=neon-vfpv4` | **4507** | 1,259,464 (+0) | 85,651 | 156,312 (+0) | 1,370 | codegen-neutral, **artifact-falsified** (`.ARM.attributes` 56→58) |
| `-mtune=cortex-a8` | **4507** | 1,259,464 (+0) | 85,651 | 156,312 (+0) | 1,370 | **UNOBSERVABLE** |
| `-fno-strict-aliasing` | 1371 | 1,260,744 (+1,280) | n/a (+1,280) | 156,316 (+4) | n/a (+4) | falsified |
| `-fwrapv` | 1388 | 1,259,688 (+224) | n/a (+224) | 156,312 (+0) | 3,816 | falsified |
| `-fno-delete-null-pointer-checks` | **4507** | 1,259,464 (+0) | 85,651 | 156,312 (+0) | 1,370 | **UNOBSERVABLE** |
| `-mno-movt` | **4507** | 1,259,464 (+0) | 85,651 | 156,312 (+0) | 1,370 | **UNOBSERVABLE** |
| `-fno-vectorize` | **4507** | 1,259,464 (+0) | 85,651 | 156,312 (+0) | 1,370 | **UNOBSERVABLE** |

No cell has `byte_exact > 4507`. No cell keeps `byte_exact 4507` while
changing `.text` or `.ARM.extab`.

### 2.1 Extra flags (new hypotheses, same full-build method)

| flag | byte_exact | `.text` size (Δ) | `.text` diff | `.ARM.extab` size (Δ) | extab diff | verdict |
|---|---:|---:|---:|---:|---:|---|
| `-marm` | 57 | 1,717,944 (+458,480) | n/a (+458,480) | 154,400 (−1,912) | n/a (−1,912) | falsified (Thumb proven) |
| `-fno-inline-functions` | 1219 | 1,294,216 (+34,752) | n/a (+34,752) | 158,108 (+1,796) | n/a (+1,796) | falsified |
| `-fno-jump-tables` | 1363 | 1,263,848 (+4,384) | n/a (+4,384) | 156,348 (+36) | n/a (+36) | falsified |
| `-fno-slp-vectorize` | 1484 | 1,257,128 (−2,336) | n/a (−2,336) | 156,296 (−16) | n/a (−16) | falsified (SLP on) |
| `-fvisibility=hidden` | 57 | 1,249,080 (−10,384) | n/a (−10,384) | 156,288 (−24) | n/a (−24) | falsified (default visibility) |
| `-fno-common` | **link fail** | — | — | — | — | falsified (`-fcommon` proven) |
| `-mno-unaligned-access` | 1367 | 1,258,728 (−736) | n/a (−736) | 156,316 (+4) | n/a (+4) | falsified (unaligned access on) |

`-fno-common` fails with
`ld.gold: error: …/GOF2HD2012.c.obj: multiple definition of 'g_android_back_button_pressed'`.
That is a direct proof that the original was compiled with clang 7's default
`-fcommon` (the tentative definition is merged, not duplicated).

---

## 3. Classification

**UNOBSERVABLE — no allocatable byte changes (only the build-id, §4):**

* `-g3` (clang 7's `-g` is already `-g2`; `-g3` adds macro info that does not
  reach codegen)
* `-fno-omit-frame-pointer` (ARM `-Oz` already keeps frame pointers)
* `-mtune=cortex-a8` (scheduling model does not change this instruction mix)
* `-mno-movt` (no `movt` choice changes on this tree)
* `-fno-delete-null-pointer-checks` (no null-check deletion is load-bearing)
* `-fno-vectorize` (the loop vectorizer is already off at `-Oz`)

**Codegen-neutral but artifact-falsified:**

* `-mfpu=neon-vfpv4`: `.text`, `.ARM.extab`, `byte_exact` all identical, but
  `.ARM.attributes` is 58 B (VFPv4 / NEONv1+Fused-MAC) versus the original's
  56 B (VFPv3 / NEONv1). The original's witnessed FPU is `neon`.

**FALSIFIED — changes codegen and lowers `byte_exact`:**

* optimization: `-O1`, `-O2`, `-O3`, `-Os` (and `-g0`)
* frame pointer: `-fomit-frame-pointer`
* stack protector: `-fno-stack-protector`, `-fstack-protector-all`
* C++ runtime: `-fno-rtti`, `-fno-exceptions`, `-fno-threadsafe-statics`
* ISA: `-march=armv7ve`, `-marm`, `-mno-unaligned-access`
* aliasing/overflow: `-fno-strict-aliasing`, `-fwrapv`
* inlining/switches/vectorization/visibility: `-fno-inline-functions`,
  `-fno-jump-tables`, `-fno-slp-vectorize`, `-fvisibility=hidden`
* common symbols: `-fno-common` (link failure)

**CONFIRMED canonical:** `-Oz -g -std=c++11 -fPIC -frtti -fexceptions
-fstack-protector-strong -funwind-tables -ffunction-sections`, with
`-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp`, default
`-fvisibility=default`, default strict aliasing, `-fwrapv` off, threadsafe
statics on, `-fcommon`, and the loop vectorizer off / SLP on at `-Oz`.

---

## 4. Why the build-id diff is not a flag signal

The original's `.note.gnu.build-id` is 20 differing bytes against the canonical
build. That note is the SHA-1 of the **unstripped** link output (the link is
gold *without* `-s`, then GNU strip), and the unstripped output contains
`.debug_*` sections whose `DW_AT_comp_dir` is the build directory. Two scratch
build directories with different name lengths therefore produce different
build-ids even when every allocatable section is identical. Example: the
`-mtune=cortex-a8` and `-mno-movt` cells have 19 differing build-id bytes
instead of 20, while their `.text` and `.ARM.extab` are byte-identical to the
control. Only compare build-id counts between builds made in the **same**
directory name.

---

## 5. Cross-check against the prior notes

This pass was run independently, but the overlapping cells agree exactly with
the earlier waves where the source has not moved:

| cell | this pass (wave 145 tree) | prior note |
|---|---:|---|
| `-O1` | 1033 | w7319: 1033 |
| `-O2` | 1168 | w7319: 1168 |
| `-O3` | 1167 | w7319: 1167 |
| `-Os` | 1263 | w7319: 1263 |
| `-g0` | 2071 | w7319: 2071 |
| `-g3` | 4507 (neutral) | w7319: `-g2/-g3` neutral |
| `-fno-vectorize` | 4507 (neutral) | w8619: neutral |
| `-mno-movt` | 4507 (neutral) | w8619: `-no-movt` neutral |
| `-fno-common` | link fail | w8619: link error |

The small differences elsewhere (e.g. `-fno-rtti` 1701 here vs 1699 in w8619,
`-fno-exceptions` 1351 vs 1342) are source drift between wave 140 and wave 145,
not harness disagreement: the canonical `byte_exact` itself moved from 4507 to
4507 with a different residual set over those waves.

The wave-142 per-TU matrix (w8719) found no per-TU cell that keeps
`byte_exact 4507` and lowers the raw diff; this whole-build pass is consistent
with that (no whole-build cell does either).

---

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9217
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# the whole matrix (serial; ~25-60 s/cell)
python3 _work/reconstructions/build_flag_ab_w9217.py \
    --baseline-build cmake-build-match \
    --workdir /dev/shm/w9217/flag-ab
```

The harness writes `<workdir>/results.json` (raw `flag_fingerprint` data,
including every differing section's size and same-address diff bytes) and
prints the compact table of §2.

Scratch (not committed): `/dev/shm/w9217/` — `variants.json`, `extras.json`,
`ff2/` (21-flag serial run), `ff3/` (extra flags), `serial.log`,
`extras.log`, `build_flag_ab_w9217.py` (copied into the repo).

---

## 7. What would falsify this note

* A full-build single-flag cell with `byte_exact > 4507` and no section-size
  regression. None of the 29 cells in this pass does.
* A cell that keeps `byte_exact 4507` but changes `.text` or `.ARM.extab`.
  None does (the six "unobservable" cells are byte-identical).
* A binary observable that contradicts the canonical set: a different canary
  set / guard-call count / RTTI table count / EH call count / frame-pointer
  count / libgcc helper count. The wave-140 census already showed all of these
  identical, and this pass's `-fno-*` cells reproduce the expected direction of
  every one.
