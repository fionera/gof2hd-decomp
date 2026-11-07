# `Layout::Layout` C1/C2 — wave 138 / worker w8301
## Stale-report discrepancy resolved (43.6 % -> 45.9 %) + one new swap (raw 5,812 -> 5,775)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`, image base `0x10000`.

Baseline = wave-137 master `96b069792` (w8107 body, fresh build **43.6 %** fuzzy, raw
same-address diff **5,757** B).  The mission's premise — "w8209 measured 45.9 % in its own
worktree but lands at 43.6 % on the merged base" — is a **measurement artifact, not a codegen
difference**.

## 1. Root cause: the tracked `report.json` was stale

`cmake --build ... --target check` runs `tools/ratchet.py`, which invokes `verify.py` with
`--build-dir cmake-build-match/verify`.  That writes
`cmake-build-match/verify/report.json`, **not** the tracked repo-root `report.json`.  Only the
separate `verify` target copies the build-dir snapshot to the repo root (CMakeLists.txt:217).
The wave-137 revert read the stale root file, which still held the pre-w8209 body's 43.6 %.

Fresh-build evidence on the same merged base (`96b069792` + each body):

| body | build-dir `report.json` | raw same-address diff |
|---|---:|---:|
| HEAD (w8107 six swaps) | **43.6 %** | **5,757** |
| w8209 (six reverts + `0x214`/`0x1ec`, `0x27c`/`0x274`) | **45.9 %** | 5,812 |

The w8209 source was therefore re-landed unchanged; it is a real 43.6 -> 45.9 improvement on
the merged base.  `Layout.cpp` is byte-identical between the w8209 worktree base
(`655e3bcc5`) and `7369aec01^` (verified with `git show ...:src/game/ui/Layout.cpp | md5sum`),
and the function's vaddr is fixed at `0xd1028` in both links, so no address-dependent verifier
effect is involved — the only difference was which `report.json` was read.

## 2. New hypothesis: continue the fuzzy-first climb on the merged base

The w8209 note's open question was whether its 45.9 % was a local optimum.  The body was split
into 231 top-level units (statements, declarations, whole `if/else` chains) and every safe
adjacent pair was re-scored with the exact `verify.py` disassembly/normalization/`reconcile`
code (`/dev/shm/w8301/score.py`), keeping size 7072 B.  Dependency-checked adjacent swaps
(`this->` read/write sets and local declarations, both directions) were tested.

One swap improved the combined objective (same official pct, lower raw bytes):

```
this->field_0x11c = ...;      ->      this->field_0x12c = ...;
this->field_0x12c = ...;              this->field_0x11c = ...;
```

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| official fuzzy (build-dir report) | 45.9 % | **45.9 %** |
| target-function raw diff bytes | 5,812 | **5,775** |
| `.text` differing bytes | 85,535 | **85,553** |
| `byte_exact` / `linked_exact` | 4507 / 4507 | **4507 / 4507** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` -> `RATCHET PASS`.  A second climb round
found no strictly better adjacent swap, so the body is again a local optimum at 45.9 %.

## 3. Region-sort sweep (no win)

Whole-region ascending sorts (the w8108 lever) were re-tested on this body.  Every variant
loses size and/or fuzzy:

| range | size | fuzzy | raw |
|---|---:|---:|---:|
| base | 7072 | **45.9** | **5,775** |
| `0xb8..0x118` | 6984 | 37.5 | 5,909 |
| `0x94..0x1e0` | 6960 | 37.7 | 5,818 |
| `0x4..0x78` | 7076 | 24.6 | 5,964 |
| `0x94..0x108` | 7056 | 41.4 | 5,964 |
| `0x108..0x1e0` | 6968 | 37.5 | 5,869 |
| `0x140..0x1e0` | 7008 | 37.3 | 5,925 |
| `0xb0..0x110` | 7040 | 43.3 | 5,937 |
| `0xbc..0x110` | 7048 | 43.5 | 5,918 |

The w8209 group-move results (`0x294` no-op; `0x294+0x170` 7080/45.0; `0x294+0x26c`
7072/44.6) still hold, so the `0x294` lever does not fire on this body.

## 4. Remaining structural gaps (unchanged)

* Prologue: original `stmdb sp!, {r6,r7,r8,r9,sl,fp}` (6 saved value regs); ours
  `stmdb sp!, {r7,r8,r9,sl,fp}` (5).  The original carries one extra live value register.
* `0xdc..0xe8`: original emits four scalar `vstr`; ours emits one `vst1` (SLP merge).
* First literal-pool group order: original loads `d0-d1` first and `d2-d3` last; ours is
  rotated (`d2-d3` first, `d0-d1` last).
* The `0x140..0x1d0` tail still groups at `0x148/0x158/0x168` (ours) vs
  `0x140/0x150/0x160/0x170` (original).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8301
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
cmake --build cmake-build-match --target verify -j6     # syncs repo-root report.json
python3 /dev/shm/w8301/score.py                          # 7072 / 45.9 / 5775
```

Scratch harnesses (not committed, `/dev/shm/w8301/`): `score.py` (exact verify.py per-function
fuzzy + raw same-address diff), `units.py` (231-unit splitter + dependency check),
`climb.py` (adjacent-swap hill climb, state in `state.json`), `variants.py` (region sorts),
`Layout.head.cpp` / `Layout.w8209.cpp` (pristine bodies).
