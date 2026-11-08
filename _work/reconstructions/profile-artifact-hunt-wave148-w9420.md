# Profile-artifact hunt (wave 148, w9420): no `.profraw`/`.profdata`/PGO artifact is shipped anywhere; a sample-PGO replay is falsified

Status: **note-only research commit**, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w9420`, base `cbe3b5f7f` (wave 147). Scratch:
`/dev/shm/w9420`. No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist
or doc change is landed. This note plus the reproduction script are the only
commit.

Gate at the base (fresh `cmake --build cmake-build-match --target check -j6`):

```text
verify: byte 4507/4524  linked 4507/4524  size 4524/4524  avg 99.93
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
lint CLEAN   drift 0
RATCHET PASS
```

Mission: find the original build's branch-weight **profile data** (a shipped
`.profraw`/`.profdata`/LLVM asset, or a profile-derived section/string in the
binary). If a profile is recoverable, replay it and measure `byte_exact`.

## 0. Bottom line

1. **No profile artifact is shipped.** Every APK named in the brief
   (`/dev/shm/gof2flags`, `/tmp/gof2re2/stock`, `/tmp/gof2mod`, plus the r18b
   `Original APK`) contains no `.profraw`, `.profdata`, `.proftext`, `.bc`,
   `.ll`, `.o`, `.a`, `.d`, `compile_commands.json`, `CMakeLists.txt`,
   `Android.mk`, `Application.mk`, `*.ninja`, `*.gradle` or build log. The
   only "build" file is the r18b APK's `assets/crashlytics-build.properties`
   (a Crashlytics UUID, not a profile).
2. **The original `.so` carries no profile-derived section or string.** It has
   exactly the 27 original sections and **no** `.llvm_*`/`.llvm_prf_*`
   section. `strings` finds zero occurrences of `__llvm_prf`, `__profc`,
   `__profd`, `__profn`, `profraw`, `profdata`, `fprofile` or `PGO`.
   `.rodata` is byte-identical to our no-profile build, so there is no
   `__llvm_prf_names` blob either.
3. **The iOS / Windows sibling builds carry no PGO artifact.** The two iOS
   Mach-O binaries (`ios_1.1.4_HD`, `ios_1.0.5_armv7`) have no `__DWARF` and
   no `__llvm_prf` section, and no profile strings. The Windows PE
   (`win_GOF2.exe`) has no `.pgd`/`.pgc`/`__PGO`/`LTCG` string; its only build
   path is `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb` (the PDB is not
   shipped). The source-layout dumps (`source file list dump/`) contain no
   build flags or profile references. The four sibling repos contain no
   profile artifact.
4. **The profile is not recoverable as a file.** The only recoverable form is
   the *block layout* (the branch weights' observable), which wave 147 already
   decoded for the 16 switch tables. The weight vector itself is not in the
   binary.
5. **New falsification: sample PGO (`-fprofile-sample-use`) is not the
   mechanism.** A synthetic sample profile that gives the original's
   `Status::missionCompleted` case bodies samples in source order *does*
   annotate the switch (LLVM 7 `SampleProfileLoader` handles `SwitchInst`) and
   reproduces a graded branch-weight vector, but it marks the function
   `hot` (`.text.hot`), shrinks it to **1372 B** (original 1432 B), moves it to
   the front of `.text`, and worsens the raw diff **722 -> 1326** and the
   aligned instruction count **320 -> 741**. `.text` shrinks by 32 B, so it
   cannot land.

**Conclusion.** The original's branch weights are real but their profile file
was not shipped. The provenance is "a profile was used", not "this profile
file". The only way to replay it is to reconstruct the weight vector from the
binary's block layout (wave 129/147), not to find a `.profdata`.

---

## 1. Artifact inventory (exhaustive)

### 1.1 APKs

| APK | entries | profile/build artifact |
|---|---:|---|
| `/dev/shm/gof2flags/GOF2_2.0.16_Original_Price.apk` | 16 | none (r14 build) |
| `/tmp/gof2re2/stock/net.fishlabs.gof2hdallandroid2012.apk` | 16 | none (r14 build) |
| `/tmp/gof2mod/apk/GOF2FHD_Original.apk` | 16 | none (r14 build) |
| `/tmp/gof2mod/apk/GOF2FHD_MOD.apk` | 16 | none (r14 mod) |
| `Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk` | 143 | only `assets/crashlytics-build.properties` (build UUID, not a profile) |

The r18b APK's 143 entries are Java resources, `.lang` files, `classes.dex`,
four `lib/armeabi-v7a/*.so`, and the Crashlytics/Firebase `.properties`
files. There is no `assets/` profile, no OBB, no `*.profraw`/`*.profdata`.
The two 2026 repacks and the 2024 repack are the old 2,469,656-byte r14
build (`libgof2hdaa.so`); the r18b ground truth is 2,187,328 B.

`unzip -l` scan for `prof|llvm|pgo|\.bc|\.o$|\.a$|\.d$|build|ninja|cmake|make`:

```text
/dev/shm/gof2flags/GOF2_2.0.16_Original_Price.apk        (none)
/tmp/gof2re2/stock/net.fishlabs.gof2hdallandroid2012.apk (none)
/tmp/gof2mod/apk/GOF2FHD_MOD.apk                         (none)
/tmp/gof2mod/apk/GOF2FHD_Original.apk                    (none)
Original APK/...APKPure.apk   assets/crashlytics-build.properties
```

`classes.dex` in all four repacks has no `profdata`, `profraw`, `fprofile`,
`pgo` or `llvm` string.

### 1.2 Original `.so`

```text
27 sections (same list as docs/VALIDATION.md); no .llvm_* / .llvm_prf_*
strings: __llvm_prf 0, __profc 0, __profd 0, __profn 0,
         profraw 0, profdata 0, fprofile 0, PGO 0
```

The only `llvm`/`clang` strings are in `.comment` (the clang 7.0.2 version
string) and `.note.gnu.gold-version` (`gold 1.12`). The `pGO` byte sequences
in `.text` are `70 47` (`bx lr`) followed by the next instruction — not a
string.

`.rodata` is byte-identical to the canonical build (0 differing bytes), so an
instrumented build's `__llvm_prf_names`/`__llvm_prf_data` blobs are absent.

### 1.3 iOS / Windows sibling builds

| binary | format | `__DWARF` | `__llvm_prf` | profile strings |
|---|---|---|---|---|
| `_work/bins/ios_1.1.4_HD` | Mach-O armv7 | no | no | 0 |
| `_work/bins/ios_1.0.5_armv7` | Mach-O armv7 | no | no | 0 |
| `_work/bins/win_GOF2.exe` | PE32 i386 | n/a | n/a | 0 (`.pgd`/`.pgc`/`__PGO`/`LTCG`) |

The iOS binaries are stripped (only a 1,528-entry external `LC_SYMTAB`; no game
code symbols). The Windows PE's only build path is the PDB reference
`C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb`.

### 1.4 Source-layout dumps and sibling repos

`source file list dump/source file lists dump.txt` is a plain list of source
paths per platform (windows / mac fhd / iOS 1.1.19 / iOS 1.0.1 etc.); it has
no compiler flags, no build system, and no `prof`/`pgo`/`llvm` token.
`_work/original_layout/*` is the parsed TU map (no build metadata).
`gof2-vita`, `KaamoClubModApi`, `gof2edit`, `FMOD-GOF2-Recreated` contain no
`.profdata`/`.profraw`/`.pgd`/`.pgc`.

### 1.5 Git history

`git log --all --diff-filter=D/A --name-only | grep -iE 'prof|pgo'` finds only
the five prior PGO notes and `pgo_flat_w9319.py`; no profile file was ever
committed.

---

## 2. Why the artifact hunt is decisive (and what it does not prove)

The absence of a shipped profile does **not** prove the original was not
PGO-built: `-fprofile-instr-use` / `-fprofile-sample-use` consume the profile
at compile time and leave no trace in the object (the `!prof` metadata is
lowered away; only the block layout survives). The absence of `__llvm_prf_*`
sections only proves the shipped build was not **instrumented**
(`-fprofile-instr-generate`), which is expected for a release build.

So the provenance question is: *can the consumed profile be reconstructed from
its only observable, the block layout?* For the 16 switch tables, wave 147
already did this (the physical case-block order is source order, not numeric).
For the 17 residual functions, the weight vector is not uniquely recoverable
from layout alone; only the *ordering* is.

---

## 3. New experiment: sample-PGO replay

LLVM 7's `-fprofile-sample-use` is a PGO-like mechanism that (unlike
`-fprofile-instr-use`) leaves functions absent from the profile alone. It is
therefore a plausible explanation for "1,957/1,973 branch functions match the
heuristic" (w8119) plus one deviating switch. I tested it on
`Status::missionCompleted`.

### 3.1 The profile

The sample-profile text format is `name:total:head` followed by
`line-offset: samples` lines, where the offset is relative to the function's
`DISubprogram` line (1314). A profile with samples on every line offset
0..259:

```text
_ZN6Status16missionCompletedEbbx:100000:100000
 0: 1000
 1: 1000
 ...
 259: 1000
```

### 3.2 It does annotate the switch

Compiling `Status.cpp` with `-fprofile-sample-use=all.sample` produces, in the
optimized IR, a switch `!prof`:

```text
!10265 = !{!"branch_weights", i32 1, i32 1001, i32 1001, i32 1001,
          i32 1, i32 1, i32 1, i32 1001, ...}
```

i.e. `SampleProfileLoader` handles `SwitchInst` and sets graded weights. (The
weights are uniform here because every line was sampled; the mechanism is
real.)

### 3.3 Measured result (full build, `Status.cpp` only)

| build | byte_exact | linked_exact | `missionCompleted` raw | aligned | size | `.text` size |
|---|---:|---:|---:|---:|---:|---:|
| canonical | **4507** | **4507** | **722** | **320** | 1432 | 1,259,464 |
| sample-PGO replay | 4507 | 4507 | **1326** | **741** | **1372** | **1,259,432 (−32)** |

The sample loader marks the function `hot`, so the object puts it in
`.text.hot._ZN6Status16missionCompletedEbbx`; gold places that section at the
front of `.text`, moving the function from `0xabf38` to `0x69534` and shrinking
`.text` by 32 B. The function itself shrinks to 1372 B. The gate's
`byte_exact`/`linked_exact` stay at 4507 only because `missionCompleted` was
already one of the 17 residuals; the section size regresses, so this cannot
land.

**Verdict:** sample PGO is falsified as the original mechanism (or at least as
a way to hit 1432 B). The original's `.text` is a single section with the
function at its TU-order address, not a `.text.hot`-first layout.

---

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9420
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

python3 _work/reconstructions/profile_artifact_hunt_w9420.py scan
python3 _work/reconstructions/profile_artifact_hunt_w9420.py sample-replay
```

The `sample-replay` mode writes `/dev/shm/w9420/all.sample`, configures a
launcher build in `/dev/shm/w9420/build_sample`, links the whole `.so`, and
prints the `missionCompleted` raw/aligned/size numbers. It does not modify the
canonical `cmake-build-match` build.

Scratch (`/dev/shm/w9420`): `all.sample`, `launch.sh`, `build_sample/`,
`status_all.obj`, `status_all.ll`, `scan_*.txt`.

---

## 5. Open items

1. The per-TU `-fprofile-instr-use` replay with a graded counter map is being
   pursued in parallel (w9415/w9416); it can reproduce the source-order body
   sequence but has not yet hit 1432 B (wave 129's `!prof` vector gives
   1444 B). This note's sample-PGO replay is an independent, different
   mechanism.
2. The original's exact weight vector is not uniquely recoverable from the
   binary; only the case-block *ordering* is. A future pass should search for a
   weight vector that keeps the low-group tail merge at 1432 B, not for a
   shipped `.profdata`.
