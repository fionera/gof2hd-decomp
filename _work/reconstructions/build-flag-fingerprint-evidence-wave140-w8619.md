# Build-flag fingerprint: evidence-first re-derivation (wave 140, w8619)

Status: **note-only research commit**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w8619`, base `e18e0c2a4` ("Wave 140 docs:
.text 86,472, .ARM.extab 1,411, semantic wins"). Scratch: `/dev/shm/w8619`.
No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or doc change is
landed. This note is the only commit.

The previous three waves (w8119/w8319/w8419) swept ~750 flag cells and found
no flag above the baseline. This pass changes the method: **fingerprint each
codegen-affecting flag from the shipped binary first** (canary sites, guard
variables, RTTI tables, EH call sites, frame-pointer prologues, NEON
instructions, libgcc helper calls), then A/B only the flags whose binary
evidence is still ambiguous, plus per-TU cells that had not been run.

Gate at the base (fresh `cmake --build cmake-build-match --target check`):

```text
verify: byte 4507/4524  linked 4507/4524  size 4524/4524  avg 99.93
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
RATCHET PASS
```

Section deltas (fresh measurement of the canonical build):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | **86,472** |
| `.ARM.extab` | 156,312 | **1,411** |
| `.note.gnu.build-id` | 36 | **20** |
| `.ARM.exidx`, `.rodata`, `.data.rel.ro`, `.data`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.rel.plt` | exact | **0** |

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
2,187,328 B, `sha256 d4c17be6…bed9483`. Our canonical `.so`
`sha256 848c1cd5…0bcc8e` (differs only in the build-id and the residual
`.text`/`.ARM.extab` bytes).

---

## 0. Bottom line

1. **Every codegen-affecting flag is now pinned by direct binary evidence,
   not only by A/B.** The shipped `.so` has exactly the same 678 canary
   functions / 787 `__stack_chk_fail` call sites, the same static-init guard
   calls, the same 176 `_ZTI` / 176 `_ZTS` / 99 `_ZTV` RTTI symbols, the same
   EH call-site census, the same 2,473 `add r7, sp` + 220 `mov r7, sp`
   frame-pointer prologues, and the same `__aeabi_idiv`/`__aeabi_uidiv`/
   `memcpy`/`memset` helper call counts as our build. The canonical flag set
   is the original's.
2. **No flag raises `byte_exact`.** This pass adds 217 scored cells (57 whole-build
   + 160 per-TU) to the ~750 of w8119/w8319/w8419. Every neutral cell
   reproduces the baseline raw residual total **92,242** exactly (i.e. it
   changes no byte of any of the 4,524 functions); every non-neutral cell is
   worse. **No cell has `rawdiff < 92,242`.**
3. **New per-TU results:** `-g0` is neutral only on `Layout.cpp` and
   `Gun.cpp` (previously the only two tested) and breaks 1–2,023 exact
   functions on every other residual TU, so `-g` is on globally.
   `-fno-threadsafe-statics`, `-fno-vectorize` and `-fno-tree-vectorize` are
   **neutral on all 13 residual TUs** (unobservable there);
   `-fno-rtti` is neutral on 11 and breaks `MGame.cpp`/`ModStation.cpp`;
   `-fno-stack-protector` breaks every residual TU.
4. **New whole-build flags:** every individual `-fno-builtin-<libc>`
   (strlen/abs/fabs/sinf/cosf/powf/memmove/memcmp/strcpy/strcmp/strcat/
   calloc/free) is neutral; `-fno-builtin-malloc` (1,488) and
   `-fno-builtin-sqrtf` (1,426) are negative; `-flto` cannot be linked by the
   original's gold (bitcode not an object), so the original is not LTO;
   `-mfloat-abi=soft` and `-mfpu=vfpv3-d16` fail the link; `+strict-align`
   is 1,365 and `-no-movt` is neutral.
5. **CGP branch-opt ratio:** the clang 7 default is
   `-cgp-freq-ratio-to-skip-merge=2`; `=1` is 3,027, `=2..1000` are all
   neutral. `-disable-cgp-branch-opts` remains the only 4,506 near-miss
   (w8419), and it breaks one exact function rather than fixing a residual.

**Verdict (unchanged, now evidence-backed): the original is a plain
`-Oz -g2` clang 7.0.2 build with `-fstack-protector-strong`, default
visibility, default strict aliasing, `-fwrapv` off, `-fno-math-errno` off,
RTTI and exceptions on, threadsafe statics on, frame pointers on, unaligned
access on, SLP vectorization on and the loop vectorizer off at `-Oz`. The 17
residual functions are source shape, not build configuration.**

---

## 1. Method

### 1.1 Fast scorer

Scratch builds use a compiler launcher that appends `$GOF2_EXTRA_FLAGS`
(whole-build cells) or `$GOF2_TU_FLAGS` when the source basename equals
`$GOF2_TU` (per-TU cells):

```sh
#!/bin/sh
exec "$@" $GOF2_EXTRA_FLAGS          # launch.sh
# launch_tu.sh checks basename($arg) == $GOF2_TU before appending
```

Each whole-build cell deletes every `CMakeFiles/gof2.dir/*.obj`, relinks with
`link_so -j64`, and scores with a byte-only scorer that reproduces
`verify.py`'s `byte_exact` modulo the two zero-size exports
(`AbyssEngine::getAppVersion`, `__cxa_end_cleanup`): the scorer prints
**4,505/4,522**, which is exactly the official **4,507/4,524** minus those
two zero-length symbols. `rawdiff` is the sum of same-address differing
bytes over the 17 non-exact functions (92,242 at baseline). The official
`.text` diff is 86,472; the per-function sum is larger because it double
counts the C1/C2 aliases and the literal-pool bytes inside those functions.

The vendored `libcrypto_gof2.a`/zlib/libzip archives are built once per build
dir (their custom commands do not see the launcher) and then reused; the
per-TU cells delete only the app objects.

### 1.2 Binary-evidence census

`objdump -d` of both `.so`s is split per exported function and every
`bl`/`blx` target is counted. The canary set is compared **per function and
per call count**, not just as a total. The RTTI census is over `.dynsym`
(`_ZTI*`, `_ZTS*`, `_ZTV*`). The frame-pointer census counts the
`add r7, sp, #N` / `mov r7, sp` prologues.

---

## 2. Binary evidence table (original vs canonical build)

| flag | binary observable | original | ours | verdict |
|---|---|---:|---:|---|
| `-fstack-protector-strong` | functions with `__stack_chk_fail` | **678** | **678** | strong |
| ` ` | `__stack_chk_fail` call sites | **787** | **787** | " |
| ` ` | per-function call-count vector | — | — | **identical** |
| `-fstack-protector` (basic) | canary functions (A/B) | 29 | — | no |
| `-fstack-protector-all` | canary functions (A/B) | 3,960 | — | no |
| `-fno-stack-protector` | canary functions (A/B) | 25 | — | no |
| `-fno-threadsafe-statics` | `__cxa_guard_acquire` calls/funcs | **2 / 1** | **2 / 1** | on |
| ` ` | `__cxa_guard_release` calls/funcs | **1 / 1** | **1 / 1** | " |
| ` ` | `__cxa_guard_abort` calls/funcs | **1 / 1** | **1 / 1** | " |
| `-fvisibility=hidden` | defined `.dynsym` exports | 5,370 | 5,370 | default |
| `-fno-rtti` | `_ZTI` / `_ZTS` / `_ZTV` symbols | **176 / 176 / 99** | **176 / 176 / 99** | RTTI on |
| `-fno-exceptions` | `_Unwind_Resume` calls/funcs | **540 / 525** | **540 / 525** | EH on |
| ` ` | `__cxa_throw` calls/funcs | **3 / 3** | **3 / 3** | " |
| ` ` | `__cxa_begin_catch` calls/funcs | **12 / 10** | **12 / 10** | " |
| ` ` | `__cxa_end_catch` calls/funcs | **10 / 6** | **10 / 6** | " |
| ` ` | `__cxa_rethrow` calls/funcs | **1 / 1** | **1 / 1** | " |
| `-fomit-frame-pointer` | `add r7, sp, #N` prologues | **2,473** | **2,473** | frame ptr on |
| ` ` | `mov r7, sp` prologues | **220** | **220** | " |
| ` ` | `push {…, r7, …}` prologues | **2,706** | **2,706** | " |
| `-fno-slp-vectorize` | `vpush` / `vpop` | **232 / 83** | **232 / 83** | SLP on |
| ` ` | `vmul` / `vdiv` / `vcmpe` | **1,319 / 496 / 959** | same | " |
| `-fno-vectorize` | (loop vectorizer off at `-Oz`) | — | — | neutral |
| `-fno-unaligned-access` | `+strict-align` A/B | 1,365 | — | unaligned on |
| `-fno-builtin` | `memcpy` / `memset` calls | **3 / 5** | **3 / 5** | builtins on |
| ` ` | `__aeabi_idiv` calls/funcs | **68 / 46** | **68 / 46** | " |
| ` ` | `__aeabi_uidiv` calls/funcs | **29 / 19** | **29 / 19** | " |
| `-fno-common` | duplicate `g_android_back_button_pressed` | link error | — | `-fcommon` |
| `-flto` | gold cannot read bitcode | link error | — | no LTO |
| `-mfloat-abi=soft` | ABI mismatch | link error | — | softfp |
| `-mfpu=vfpv3-d16` | ABI mismatch | link error | — | NEON |
| `-mno-movt` | `movt` count | 25 | 25 | movt on |

The `-fstack-protector`/`-all`/`-fno-stack-protector` canary-count rows are the w5218 measurements on the same artifact; the current-build `byte_exact` for those cells is in §3. The canary set is not merely the same size: the per-function
`__stack_chk_fail` call-count vector is identical (`ca == cb` over all 678
functions). The A/B alternatives in the table are the measured
`byte_exact` counts from §3.

---

## 3. Whole-build A/B matrix (new cells)

Canonical: `byte_exact 4,505/4,522`, `rawdiff 92,242`. "Neutral" means
`4,505` **and** `rawdiff 92,242` (no byte of any function changes).

### 3.1 The mission's flag list

| flag | byte_exact | rawdiff | note |
|---|---:|---:|---|
| `-fstack-protector-strong` | **4,505** | 92,242 | canonical (neutral) |
| `-fstack-protector` (basic) | 1,375 | 882,336 | 29 canary funcs |
| `-fstack-protector-all` | 89 | 530,285 | 3,960 canary funcs |
| `-fno-stack-protector` | 1,375 | 883,198 | 25 canary funcs |
| `-fno-threadsafe-statics` | 1,366 | 172,307 | guards removed |
| `-fvisibility=hidden` | 56 | 2,129 | exports collapse |
| `-fvisibility=default` | **4,505** | 92,242 | neutral |
| `-fno-strict-aliasing` | 1,369 | 378,596 | strict aliasing on |
| `-fwrapv` | 1,386 | 214,012 | wrapv off |
| `-fno-strict-overflow` | 1,386 | 214,012 | same codegen as `-fwrapv` |
| `-fno-math-errno` | 1,354 | 208,448 | math-errno on |
| `-ffast-math` | 1,333 | 376,535 | no fast-math |
| `-fno-rtti` | 1,699 | 157,377 | RTTI on |
| `-fno-exceptions` | 1,342 | 753,267 | EH on |
| `-mno-unaligned-access` | 1,365 | 391,465 | unaligned on |
| `-fno-vectorize` | **4,505** | 92,242 | loop vectorizer already off |
| `-fno-slp-vectorize` | 1,482 | 348,374 | SLP on |
| `-fomit-frame-pointer` | 1,310 | 1,124,298 | frame ptr on |
| `-fno-inline-functions` | 1,217 | 889,259 | inlining on |

### 3.2 New whole-build flags

| flag | byte_exact | note |
|---|---:|---|
| `-fno-builtin-strlen` | **4,505** | neutral |
| `-fno-builtin-abs` / `-fabs` / `-sinf` / `-cosf` / `-powf` | **4,505** | neutral |
| `-fno-builtin-memmove` / `-memcmp` / `-strcpy` / `-strcmp` / `-strcat` | **4,505** | neutral |
| `-fno-builtin-calloc` / `-free` | **4,505** | neutral |
| `-fno-builtin-malloc` | 1,488 | malloc builtin used |
| `-fno-builtin-sqrtf` | 1,426 | sqrtf builtin used |
| `-fno-tree-vectorize` | **4,505** | neutral (clang alias) |
| `-fno-peel-loops` / `-fno-unswitch-loops` | **4,505** | clang 7 ignores (warning) |
| `-flto` | build fail | gold cannot link LLVM bitcode |
| `-mfloat-abi=soft` | build fail | ABI mismatch |
| `-mfpu=vfpv3-d16` | build fail | ABI mismatch |
| `-Xclang -target-feature -Xclang +strict-align` | 1,365 | same as `-mno-unaligned-access` |
| `-Xclang -target-feature -Xclang -no-movt` | **4,505** | neutral |

### 3.3 CGP branch-opt ratio (new)

`-mllvm -cgp-freq-ratio-to-skip-merge` default is **2**.

| value | byte_exact |
|---|---:|
| 1 | 3,027 |
| 2 / 3 / 4 / 5 / 10 / 20 / 50 / 100 / 1000 | **4,505** (neutral) |

`-disable-cgp-branch-opts` remains 4,506 at the wave-140 baseline (it breaks
`AbyssEngine::Engine::SetUVMatrix`), i.e. CGP branch opts are on; no ratio
value fixes a residual.

---

## 4. Per-TU matrix (new cells)

All cells recompile only the named TU; baseline is `4,505` / `92,242`.

### 4.1 `-g0` on all 13 residual TUs

(`-g` is codegen-relevant at clang 7 `-Oz` because DWARF `DBG_VALUE` users
pin PostRA machine sinking.)

| TU | byte_exact | rawdiff | note |
|---|---:|---:|---|
| `Layout.cpp` | **4,505** | 92,242 | neutral |
| `Gun.cpp` | **4,505** | 92,242 | neutral |
| `LevelScript.cpp` | 4,504 | 92,248 | 1 exact fn breaks |
| `Radar.cpp` | 4,504 | 92,312 | 1 |
| `Hud.cpp` | 4,504 | 92,266 | 1 |
| `Status.cpp` | 4,500 | 92,299 | 5 |
| `SpaceLounge.cpp` | 4,503 | 92,434 | 2 |
| `Generator.cpp` | 4,502 | 92,257 | 3 |
| `Level.cpp` | 4,486 | 93,965 | 19 |
| `MGame.cpp` | 4,471 | 95,341 | 34 |
| `HangarWindow.cpp` | 4,372 | 96,054 | 133 |
| `MenuTouchWindow.cpp` | 3,616 | 114,966 | 889 |
| `ModStation.cpp` | 2,482 | 139,431 | 2,023 |

`-g` is therefore on for every residual TU. `Layout.cpp`/`Gun.cpp` are
unobservable; their residual functions are unchanged.

### 4.2 Per-TU source flags (all 13 residual TUs)

| flag | result |
|---|---|
| `-fno-threadsafe-statics` | **neutral on all 13** (no function-local statics in these TUs) |
| `-fno-vectorize` | **neutral on all 13** |
| `-fno-tree-vectorize` | **neutral on all 13** |
| `-fno-rtti` | neutral on 11; `MGame.cpp` 2,927, `ModStation.cpp` 1,740 (both use RTTI) |
| `-fno-stack-protector` | negative on all 13 (1,740–3,095; canaries removed) |

### 4.3 Per-TU scheduler flags (all 13 residual TUs)

| flag | result |
|---|---|
| `-mllvm -enable-misched` | negative on all 13 (2,033–3,795) |
| `-mllvm -misched-postra` | negative on all 13 (4,395–4,502) |
| `-mllvm -pre-RA-sched=source` | negative on all 13 (2,035–4,492) |
| `-mllvm -disable-postra-machine-sink` | `Layout`/`LevelScript` neutral; `MGame` 4,505 but raw **92,266** (+24); `Generator` 4,505 but raw **92,244** (+2); all others negative |

### 4.4 Per-TU negative-global flags on `Layout.cpp` / `Gun.cpp`

These are the two TUs where `-g0` is neutral, so they are the best candidates
for a flag that changes only the residual function.

| flag | Layout | Gun |
|---|---:|---:|
| `-fno-builtin-malloc` | **4,505** | **4,505** |
| `-fno-builtin-sqrtf` | **4,505** | **4,505** |
| `-fwrapv` | **4,505** | **4,505** |
| `-fno-strict-aliasing` | **4,505** | 2,809 |
| `-fno-math-errno` | **4,505** | **4,505** |
| `-fno-unroll-loops` | **4,505** | **4,505** |
| `-fno-jump-tables` | 2,468 | **4,505** |
| `-mno-unaligned-access` | 2,455 | **4,505** |
| `-fno-slp-vectorize` | 2,453 | **4,505** |
| `-fno-optimize-sibling-calls` | 2,439 | 3,920 |
| `-fomit-frame-pointer` | 2,444 | 2,869 |
| `-fno-inline-functions` | 1,367 | 1,443 |
| `-fno-inline` | 1,367 | 1,443 |
| `-fno-exceptions` | 1,367 | 1,367 |
| `-ffast-math` | 3,258 | 3,804 |

Every neutral cell is exactly `rawdiff 92,242`: it changes no residual byte.
**No cell anywhere in this pass has `rawdiff < 92,242`.**

---

## 5. What remains

The 17 residual functions are unchanged in kind from w8419:

* block/switch-body order (`Status::missionCompleted`, `LevelScript::process`,
  `Level::createMission`);
* receiver caching / register allocation (`MGame::OnUpdate`,
  `MGame::OnTouchEnd`, `MenuTouchWindow::OnTouchBegin`, `ModStation`);
* literal-pool rotation (`Level::createMission`, `Radar::draw`);
* C1/C2 alias placement (`Layout`, `ModStation`).

No build flag changes any of them toward the original while keeping the
other 4,507 functions exact. The evidence table in §2 now pins every
codegen-affecting flag from the artifact itself, so future flag work can be
skipped unless a *new* compiler option is discovered.

---

## 6. Reproduction

```sh
# canonical gate
cd /home/debian/GalaxyOnFire2/wt/w8619
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# scratch whole-build cell
cmake -S . -B /dev/shm/w8619/b1 -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz \
  -DCMAKE_C_COMPILER_LAUNCHER=/dev/shm/w8619/launch.sh \
  -DCMAKE_CXX_COMPILER_LAUNCHER=/dev/shm/w8619/launch.sh
find /dev/shm/w8619/b1/CMakeFiles/gof2.dir -name '*.obj' -delete
GOF2_EXTRA_FLAGS="-fno-strict-aliasing" \
  cmake --build /dev/shm/w8619/b1 --target link_so -j64
python3 /dev/shm/w8619/score.py /dev/shm/w8619/b1/libgof2hdaa.so

# per-TU cell
GOF2_TU=Layout.cpp GOF2_TU_FLAGS="-g0" \
  cmake --build /dev/shm/w8619/t1 --target link_so -j64

# evidence census (canaries / guards / RTTI / EH / frame pointers / helpers)
# see §1.2; the script is reproduced in the commit message of this note.
```

Scratch (not committed): `/dev/shm/w8619/` — `launch.sh`, `launch_tu.sh`,
`matrix.sh`, `seq.sh`, `matrix_tu.sh`, `score.py`, `sec.py`, `cells*.txt`,
`tu_*.txt`, `m*_results.txt`, `b1..b8/`, `t1..t8/`.

---

## 7. What would falsify this note

* A full-gate flag cell with `byte_exact >= 4,506` and no size regression.
  None of the 217 cells in this pass (plus the ~750 of w8119/w8319/w8419)
  does; the closest are `-disable-cgp-branch-opts` and
  `-arm-force-fast-isel` at 4,506, both of which break one exact function.
* A per-TU cell with `rawdiff < 92,242` while keeping `byte_exact 4,505` and
  every section exact. None does.
* A binary observable that contradicts §2 (e.g. a different canary set,
  guard call count, RTTI table count, EH call count, frame-pointer count or
  libgcc helper count).
