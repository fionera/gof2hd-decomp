# Wave 147 (w9415): `Status::missionCompleted` — per-TU `-fprofile-instr-use`
# with a graded switch profile does **not** reproduce the original 1432 B.
# Note + harness only; no `src/` change.

Status: **note-only** (no `src/`/`CMakeLists.txt` change).  Gate on the
unchanged tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.  Worktree
`wt/w9415`, base master `cbe3b5f7f` (wave 147).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**.
Baseline: raw same-address diff **722**, instruction-index aligned diff **320**.

## 0. New result

1. **The graded profile is real and does produce the original's source-order
   case layout — but only when the rest of the function is cold.**  With a
   synthetic `.profdata` whose 29 case counters encode the w9305 winning
   vector (`1,3,3,4,4,4,4,4,3x12,2x10`) and every non-case counter = 0, the
   TBH case blocks are laid out exactly in source order
   (`167,174,150..155,158,162,166,170,184,189,156,157,160,163,164,165,168,169,171,172`).
   But the function is **1476 B** (entry-only) / **1364 B** (whole case body),
   not 1432 B.
2. **Warming the non-case counters scrambles the order.**  With every
   non-case counter = 1 the switch `!prof` is still the graded vector, yet the
   case blocks are laid out
   `167,174,170,189,162,154,152,150,151,153,155,158,184,166,171,172,163,160,156,164,157,168,165,169`
   and the function is **1416 B**.  The extra branch weights on the early
   guards / loop / default block dominate the block placement.
3. **The emitted vector is not the requested one.**  LLVM 7's
   `PGOUseFunc::setBranchWeights` maps both switch edges of a shared successor
   through `GetSuccessorNumber`, so the second value of a shared block gets
   the *base* weight, not the first value's weight.  The requested
   `1,3,3,4,4,4,4,4,3x12,2x10` is emitted as
   `1,3,2,4,4,2,4,4,3x12,2x10` (case 174 -> 2, case 11 -> 2).
4. **The rest of the TU is byte-exact.**  The profile contains only
   `missionCompleted`; the other 145 `.text.*` function sections of
   `Status.cpp.obj` are byte-identical to the canonical object.  Only the
   target section is renamed `.text.hot.` (the profile marks the function hot)
   and the debug/relocation sections shift.
5. **Conclusion.**  A per-TU `-fprofile-instr-use` profile cannot reproduce
   the original 1432 B: it necessarily weights *every* branch, while the
   w9305 switch-only `!prof` injection (which gives 1432) weights only the
   switch.  The original was therefore not built with a plain full-TU PGO
   profile, or the profile's non-switch weights happened to be neutral
   (unrecoverable from the binary).

## 1. Measured variants

All numbers are the real TU + real checked-in gold link (unstripped `.so`,
`.text` bytes).  `base` = value of every non-case counter; `body` = the whole
case body gets the case weight (not just the case-entry block).

| variant | raw | size | source order | aligned |
|---|---:|---:|---|---:|
| master (no profile) | **722** | **1432** | no (numeric) | **320** |
| base=0, entry only | 1387 | 1476 | **yes** | 635 |
| base=0, whole body | 1323 | 1364 | **yes** | 546 |
| base=1, entry only | 1370 | 1416 | no | 694 |
| base=1, whole body | 1376 | 1416 | no | 677 |

The w9305 switch-only injection remains the only 1432 B reproduction
(raw 558 with the w9214 vector; 459 with the pgi-first body).

## 2. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9415
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# graded profile, both bases, entry-only:
python3 _work/reconstructions/pgo_graded_w9415.py
# whole case body:
python3 _work/reconstructions/pgo_graded_w9415.py --body
# one base:
python3 _work/reconstructions/pgo_graded_w9415.py --base 1
```

The harness derives the case -> profile-counter map from a fresh
`-fprofile-instr-generate -emit-llvm` build (no hard-coded counter numbers),
writes the `.proftext` with the real `FuncHash 2003737435339834133` and
`NumCounters 116`, merges it with the NDK `llvm-profdata`, recompiles
`Status.cpp` with `-fprofile-instr-use`, links with the checked-in gold
command, and reports raw/aligned diff, TBH order, and other-`.text`-section
identity.

## 3. Open items

1. The 1432 B layout needs switch-only `!prof`; a full profile is too broad.
   If a build-level profile is ever wanted, the non-switch branch weights must
   be recovered from the original's block layout too (a much larger search).
2. The emitted shared-successor weight quirk (`174 -> base`, `11 -> base`) is
   a property of LLVM 7's `GetSuccessorNumber`; any profile-based attempt must
   account for it.
