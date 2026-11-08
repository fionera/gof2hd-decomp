# Build provenance at wave 158 (w10520): per-TU opt levels, `-mllvm` matrix,
# unwind/stack-protector cells, and the w10313 Status profile

Status: **research note only**, 2026-11-08. Worktree `wt/w10520`, base master
`cfde1f5f2` (wave 158). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w10520`.
No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or doc change is
landed; this note is the only commit.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` **85,372** differing bytes (section size
1,259,464 B), `.ARM.extab` **1,284** (size 156,312 B), `.rel.plt` 0,
`.note.gnu.build-id` 20. All 27 section sizes exact; file size 2,187,328 B.

## 0. Bottom line

1. **No per-TU optimization level beats `-Oz` at the wave-158 tree.** The full
   13-TU x 5-level matrix reproduces the wave-154/155/156/157 conclusion.
   `-Oz` is the minimum of both raw and aligned diff for every one of the 17
   residual names. The closest non-`-Oz` cell is `Layout::Layout` at `-Os`
   (raw 5,742 vs 5,739, aligned 2,121 vs 1,978).
2. **No `-mllvm` knob improves the gate.** 27 measured cells; 19 are exactly
   neutral (`byte 4507 / linked 4507 / size 4524`, raw 91,154, aligned 25,657).
   Every non-neutral variant is worse on both metrics. One new cell,
   `-mllvm -disable-cgp-branch-opts`, is a **1-byte regression** (`byte 4506`):
   it breaks `AbyssEngine::Engine::SetUVMatrix(Matrix const&)`, so the
   original build had CGP branch opts enabled.
3. **`-fno-unwind-tables` is a regression** (launcher cell, applied after the
   target's `-funwind-tables`): `byte 2878`, raw 91,700, aligned 26,772
   (base 4507 / 91,154 / 25,657). `-funwind-tables` and
   `-fno-asynchronous-unwind-tables` are exactly neutral. The canonical
   `-funwind-tables` is confirmed.
4. **`-fstack-protector-strong` is the only viable stack-protector spelling.**
   `-fstack-protector` (basic) gives `byte 1377 / linked 3786 / size 3794`;
   `-fno-stack-protector` `1377 / 3783 / 3790`; `-fstack-protector-all`
   collapses to `83 / 969 / 981`. `-fstack-protector-explicit` is not
   accepted by clang 7.0.2 (unknown argument).
5. **The w10313 Status profile replays exactly at the wave-158 tree**:
   `Status::missionCompleted` raw 722 -> **1,299**, aligned 320 -> **583**,
   size 1,432 (exact), all 145 other functions in `Status.cpp` byte-identical,
   gate unchanged at 4507/4507/4524. It is still not byte-exact, and it is the
   only residual it touches.
6. **No flag or profile makes any of the 17 residuals byte-exact.** The
   residual set remains source-shape / TU-shape work. Nothing is landable: the
   canonical `-Oz` build is best on every measured axis.

## 1. Baseline

```text
verify: byte 4507  linked 4507  size 4524/4524  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
lint CLEAN   drift 0   symbol parity 5370/5370
RATCHET PASS
```

Residual raw / aligned / size at the wave-158 tree (17 names, 15 unique
bodies; `LayoutC1/C2` and `ModStationC1/C2` are aliases):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `Layout::Layout` (C1/C2) | 5,739 | 1,978 | 7,072 |
| `LevelScript::process` | 55,094 | 17,129 | 57,232 |
| `Level::createShip` | 1,563 | 449 | 2,764 |
| `Level::createMission` | 5,487 | 774 | 17,172 |
| `Status::missionCompleted` | 722 | 320 | 1,432 |
| `Radar::draw` | 8,181 | 1,278 | 9,908 |
| `MGame::OnTouchEnd` | 4,387 | 901 | 8,080 |
| `MGame::OnUpdate` | 37 | 109 | 11,440 |
| `Gun::calcCharacterCollision` | 463 | 57 | 1,984 |
| `HangarWindow::render` | 1,965 | 291 | 9,700 |
| `SpaceLounge::drawLounge` | 175 | 41 | 1,948 |
| `Hud::draw` | 1,325 | 249 | 9,332 |
| `Generator::createMission` | 162 | 64 | 1,996 |
| `MenuTouchWindow::OnTouchBegin` | 29 | 15 | 1,896 |
| `ModStation::ModStation` (C1/C2) | 43 | 12 | 1,008 |

Raw sum over the 17 names **91,154**; aligned sum **25,657**. Over the 15
unique bodies (excluding the two C2 aliases) the sums are **85,372** and
**23,667** — the unique raw sum is exactly the `.text` differing-byte count.

## 2. Per-TU `-Oz/-O1/-O2/-Os/-O3` sweep

Harness: `tools/per_tu_opt_sweep.py` with `tools/per_tu_flags.cmake` (each
cell is a full independent scratch build; `-Oz` reproduces the canonical gate
exactly). 13 TUs x 5 levels. Four `-Oz` cells hit the shared
`third_party/*/lib*_gof2.a` archive race in the parallel run and were
re-measured sequentially; their values below are the sequential retry.

### 2.1 Residual raw/aligned/size (lower is better)

| TU / function | -Oz | -O1 | -O2 | -Os | -O3 |
|---|---|---|---|---|---|
| `Layout.cpp` `LayoutC1` | **5739/1978/7072** | 7813/4767/7072 | 5994/2181/7072 | 5742/2121/7072 | 6381/3755/7072 |
| `LevelScript.cpp` `process` | **55094/17129/57232** | 59901/36997/57232 | 61605/33985/57232 | 55775/24322/57232 | 64600/34955/57232 |
| `Level.cpp` `createShip` | **1563/449/2764** | 2979/1616/2764 | 2951/1092/2764 | 2876/1063/2764 | 2968/1094/2764 |
| `Level.cpp` `createMission` | **5487/774/17172** | 18220/8827/17172 | 18778/10645/17172 | 17565/6032/17172 | 19763/7838/17172 |
| `Status.cpp` `missionCompleted` | **722/320/1432** | 1877/855/1432 | 1692/778/1432 | 1328/628/1432 | 1764/979/1432 |
| `Radar.cpp` `draw` | **8181/1278/9908** | 10199/4807/9908 | 10058/4438/9908 | 9967/4268/9908 | 10505/4207/9908 |
| `MGame.cpp` `OnTouchEnd` | **4387/901/8080** | 8080/3104/8080 | 9398/3766/8080 | 7824/2522/8080 | 9735/3861/8080 |
| `MGame.cpp` `OnUpdate` | **37/109/11440** | 11493/4707/11440 | 14022/5638/11440 | 10987/1884/11440 | 14058/5680/11440 |
| `Gun.cpp` `calcCharacterCollision` | **463/57/1984** | 1911/549/1984 | 1927/346/1984 | 1879/322/1984 | 1932/349/1984 |
| `HangarWindow.cpp` `render` | **1965/291/9700** | 9608/4925/9700 | 9569/2214/9700 | 9439/2124/9700 | 9871/5316/9700 |
| `SpaceLounge.cpp` `drawLounge` | **175/41/1948** | 1965/453/1948 | 1861/224/1948 | 1858/236/1948 | 1899/230/1948 |
| `Hud.cpp` `draw` | **1325/249/9332** | 8874/1943/9332 | 9475/1998/9332 | 9123/1666/9332 | 9659/2321/9332 |
| `Generator.cpp` `createMission` | **162/64/1996** | 2137/857/1996 | 2085/836/1996 | 1980/566/1996 | 2414/963/1996 |
| `MenuTouchWindow.cpp` `OnTouchBegin` | **29/15/1896** | 2084/689/1896 | 1876/737/1896 | 1873/736/1896 | 1939/743/1896 |
| `ModStation.cpp` `ModStationC1` | **43/12/1008** | 957/290/1008 | 946/44/1008 | 919/42/1008 | 1048/98/1008 |

(`LayoutC2` and `ModStationC2` are identical to their C1 rows.)

**Conclusion:** `-Oz` is optimal for every residual on both metrics. No TU was
compiled at another level.

## 3. Targeted `-mllvm` / unwind / stack-protector matrix

Harness: `/dev/shm/w10520/run_variant.py` (one build directory per variant, a
compiler-launcher cell so the extra flags are appended *after* the canonical
flags and after the target's `-ffunction-sections -funwind-tables`). `raw` /
`aligned` are the sums over the 17 residual names; `.text` is the linked
section size. Base = canonical `-Oz` build.

| variant | flags | byte | linked | size | `.text` | raw | aligned |
|---|---|---:|---:|---:|---:|---:|---:|
| base | — | **4507** | **4507** | **4524** | **1259464** | **91154** | **25657** |
| `slp0` | `-mllvm -slp-threshold=0` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `inline1` | `-mllvm -inline-threshold=1` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `unroll0` | `-mllvm -unroll-threshold=0` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `gvnhoist` | `-mllvm -enable-gvn-hoist=false` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `prefixoff` | `-mllvm -profile-guided-section-prefix=false` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `sink0` | `-mllvm -max-uses-for-sinking=0` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `sink0_prefixoff` | both of the above | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `ifcvt_old` | `-mllvm -old-thumb2-ifcvt` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `guards1` | `-mllvm -guards-predicate-pass-branch-weight=1` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `shrinkwrap_on` | `-mllvm -enable-shrink-wrap=true` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `postmisched_off` | `-mllvm -enable-post-misched=false` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `taildup_place0` | `-mllvm -tail-dup-placement-threshold=0` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `funwind_explicit` | `-funwind-tables` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `sp_strong` | `-fstack-protector-strong` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `nounwind_async` | `-fno-asynchronous-unwind-tables` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `slp_off` | `-mllvm -vectorize-slp=false` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `loopvec_off` | `-mllvm -vectorize-loops=false` | 4507 | 4507 | 4524 | 1259464 | 91154 | 25657 |
| `cgp_branch_off` | `-mllvm -disable-cgp-branch-opts` | 4506 | 4506 | 4524 | 1259464 | 91154 | 25657 |
| `slp1` | `-mllvm -slp-threshold=1` | 1637 | 4503 | 4521 | 1259560 | 94324 | 26379 |
| `inline0` | `-mllvm -inline-threshold=0` | 1356 | 4458 | 4498 | 1259112 | 96368 | 26883 |
| `unroll50` | `-mllvm -unroll-threshold=50` | 1554 | 4504 | 4521 | 1259432 | 94166 | 26166 |
| `loadpre` | `-mllvm -enable-load-pre=false` | 1402 | 4194 | 4332 | 1257800 | 123585 | 31769 |
| `earlytaildup` | `-mllvm -disable-early-taildup=true` | 2700 | 4493 | 4516 | 1259432 | 93446 | 26510 |
| `tailmerge0` | `-mllvm -tail-merge-threshold=0` | 1338 | 3855 | 3875 | 1357684 | 142307 | 34569 |
| `exitbias100` | `-mllvm -block-placement-exit-block-bias=100` | 1741 | 4443 | 4480 | 1259432 | 97291 | 26741 |
| `taildup_off` | `-mllvm -disable-tail-duplicate` | 1963 | 4450 | 4501 | 1259496 | 93942 | 26159 |
| `noplace` | `-mllvm -disable-block-placement` | 1254 | 3118 | 4121 | 1268168 | 143307 | 72516 |
| `nounwind` | `-fno-unwind-tables` | 2878 | 4507 | 4524 | 1259464 | 91700 | 26772 |
| `sp_basic` | `-fstack-protector` | 1377 | 3786 | 3794 | 1237416 | 138400 | 38426 |
| `sp_all` | `-fstack-protector-all` | 83 | 969 | 981 | 1374024 | 102657 | 28773 |
| `sp_none` | `-fno-stack-protector` | 1377 | 3783 | 3790 | 1237288 | 138396 | 38426 |
| `gvnsink` | `-mllvm -enable-gvn-sink` | 1349 | 4336 | 4385 | 1259896 | 130368 | 33695 |
| `machinesink_off` | `-mllvm -disable-machine-sink` | 1235 | 3737 | 4019 | 1263240 | 136194 | 40830 |
| `shrinkwrap_off` | `-mllvm -enable-shrink-wrap=false` | 1457 | 4470 | 4500 | 1259400 | 94361 | 26180 |
| `globalmerge_off` | `-mllvm -enable-global-merge=false` | 1397 | 4497 | 4517 | 1259816 | 96072 | 26228 |
| `no_rtti` | `-fno-rtti` | 1701 | 4507 | 4524 | 1259464 | 94319 | 26180 |
| `no_exceptions` | `-fno-exceptions` | 1344 | 3761 | 3772 | 1168804 | 138306 | 51429 |
| `tailmerge_false` | `-mllvm -enable-tail-merge=false` | 1338 | 3855 | 3875 | 1357684 | 142307 | 34569 |

Not accepted by clang 7.0.2 (`Unknown command line argument`):
`-mllvm -enable-ext-tsp-block-placement=false`,
`-mllvm -enable-early-ifcvt=false`, `-mllvm -arm-ldst-opt=false`,
`-mllvm -enable-merge-functions`, `-mllvm -enable-unsafe-fp-math`,
`-mllvm -enable-no-nans-fp-math`, `-mllvm -enable-cond-stores-opt=false`,
`-mllvm -enable-early-taildup=true`, `-mllvm -enable-cgp=true`,
`-mllvm -static-likely-branch-weight=64`.
`-mllvm -enable-machine-outliner` is accepted but crashes clang 7.0.2 on
`VirtualInput.cpp` (backend abort), so it is not usable.

### 3.1 `-fno-unwind-tables` launcher cell

The launcher appends the flag after the whole compiler argument list (the
target's `-funwind-tables` would otherwise win). The applied cell gives
`byte 2878 / linked 4507 / size 4524`, raw 91,700, aligned 26,772 — the
canonical `-funwind-tables` is confirmed. `-fno-asynchronous-unwind-tables`
is exactly neutral (it does not remove the C++ EH tables).

### 3.2 Stack-protector variants

`-fstack-protector` (basic) and `-fno-stack-protector` are indistinguishable
on the gate (`byte 1377`, `linked ~3785`, `size ~3790`) and both are far below
`strong` (4507). `-fstack-protector-all` collapses to `byte 83`.
**`-fstack-protector-strong` is the original's spelling.** Explicit
`-fstack-protector-strong` reproduces the canonical gate exactly.

### 3.3 New wave-158 knob results

* `-mllvm -enable-shrink-wrap=true` is exactly neutral; `=false` is worse
  (1457). Shrink-wrapping is on by default in the original build.
* `-mllvm -enable-post-misched=false` and
  `-mllvm -tail-dup-placement-threshold=0` are exactly neutral.
* `-mllvm -disable-cgp-branch-opts` is a **1-byte regression**: it breaks
  `AbyssEngine::Engine::SetUVMatrix(Matrix const&)` (raw +1, aligned
  unchanged). CGP branch opts are part of the original build.
* `-mllvm -vectorize-slp=false` and `-mllvm -vectorize-loops=false` are
  exactly neutral (nothing to vectorize at `-Oz`).
* `-fno-rtti` keeps size/addresses exact but is a large raw regression
  (94,319 / 26,180); `-fno-exceptions` changes `.text` size (1,168,804) and
  collapses the gate.
* `-mllvm -enable-global-merge=false` is worse (1397).

## 4. The w10313 Status profile

The committed `_work/reconstructions/status_instr_1432_relplt0_w10313.proftext`
(116 counters, func hash 2003737435339834133) was replayed on
`src/game/mission/Status.cpp` with `-fprofile-instr-use` +
`-mllvm -max-uses-for-sinking=0` +
`-mllvm -profile-guided-section-prefix=false`, then the whole `.so` was
relinked and all 17 residuals measured.

| function | baseline raw/aligned/size | w10313 profile |
|---|---|---|
| `Status::missionCompleted` | 722 / 320 / 1432 | **1299 / 583 / 1432** |
| all 16 other residuals | unchanged | unchanged |
| **17-name total** | **91154 / 25657** | **91731 / 25920** |

Full gate with the profile: `byte 4507 / linked 4507 / size 4524` (unchanged).
All 145 other functions in `Status.cpp` are byte-identical to the canonical
object. The profile reproduces the w10313 numbers exactly at the wave-158
tree, keeps the size exact, and touches no other function. It is still not
byte-exact (the original body is the unprofiled heuristic body; see the
w10313 note).

The `status_flat_w10117.proftext` variant (all counters 0) gives
`1352 / 637 / 1428` — not the 558/47 flat-`!prof` IR result; the flat vector
is a source/IR-shape experiment, not an instrumented profile.

## 5. Conclusion

* The original is a single `-Oz` build; no TU was compiled at
  `-O1/-O2/-Os/-O3`.
* The stock clang 7.0.2 `-Oz` `-mllvm` defaults are the original's; no knob
  improves the gate. The 19 neutral cells are consistent with defaults.
* `-funwind-tables` and `-fstack-protector-strong` are confirmed as the
  original's flags; the alternatives collapse the gate.
* The w10313 Status profile is a faithful, size-exact provenance artifact but
  is not a gate win.
* No flag or profile makes any of the 17 residuals byte-exact.
* The wave-158 gate is unchanged: **4507/4524 byte, 4507/4524 linked,
  4524/4524 size, `.text` 85,372 differing bytes, `.ARM.extab` 1,284, raw sum
  91,154 (85,372 over the 15 unique bodies), aligned sum 25,657 (23,667 over
  the 15 unique bodies)**.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10520
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j32

# per-TU opt-level matrix (13 TUs x 5 levels)
python3 tools/per_tu_opt_sweep.py --build-root /dev/shm/w10520/pertu --jobs 13 \
  --out /dev/shm/w10520/pertu_results.json

# targeted flag matrix (compiler-launcher cell)
python3 /dev/shm/w10520/run_variant.py /dev/shm/w10520/spec.json \
  /dev/shm/w10520/variants 4 /dev/shm/w10520/variant_results.json
python3 /dev/shm/w10520/run_variant.py /dev/shm/w10520/spec2.json \
  /dev/shm/w10520/variants2 3 /dev/shm/w10520/variant_results2.json

# w10313 Status profile replay
python3 /dev/shm/w10520/status_profile.py
```

### 6.1 Harness (self-contained)

`/dev/shm/w10520/run_variant.py` writes a per-variant launcher that appends
the extra flags after the whole compiler argument list, configures one build
directory per variant, builds `link_so`, runs `tools/verify/verify.py`, and
measures `tools/fn_diff.py` raw/aligned on the 17 residual names plus the
linked `.text` size:

```python
launcher = os.path.join(bd, 'launcher.sh')
open(launcher, 'w').write('#!/bin/sh\ncomp="$1"; shift\nexec "$comp" "$@" %s\n' % flags)
os.chmod(launcher, 0o755)
cfg = ('cmake -S %s -B %s -G Ninja '
       '-DCMAKE_CXX_COMPILER_LAUNCHER=%s -DCMAKE_C_COMPILER_LAUNCHER=%s '
       '-DGOF2_ORIGINAL_SO=%s -DGOF2_FMOD_LIBDIR=%s' %
       (ROOT, bd, launcher, launcher, ORIG, FMOD))
```

The Status replay reuses the committed `_work/reconstructions/profgen_w10314.py`
with `REPO`/`SCRATCH` pointed at this worktree; it instruments `Status.cpp`
with `-fprofile-instr-generate -emit-llvm -S`, writes the 116-counter text
profile, merges it with `llvm-profdata`, recompiles the TU with
`-fprofile-instr-use=<profdata>` and links the full `.so` with only that
object replaced.

**Note:** concurrent variant builds race on the shared source-tree archives
`third_party/{zlib,libzip,openssl}/lib*_gof2.a` (the vendored-library step
writes them in place). Four `-Oz` per-TU cells and no flag cell failed this
way; they were re-measured sequentially.

## 7. Files touched

* `_work/reconstructions/build-provenance-wave158-w10520.md` (this note).
* No source, tool, CMake, baseline, allowlist or doc change.
