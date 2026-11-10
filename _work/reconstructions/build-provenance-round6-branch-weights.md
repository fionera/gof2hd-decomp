# Build provenance round 6 (w11120): no source-only shape reaches the
# Status 4/3/2 switch plateau; clang 7.0.2 has exactly one switch-weight
# producer and it is two-level

Status: **research note only**, 2026-11-10. Worktree `wt/w11120`, base
master `85c1f88df` (wave 166). Scratch `/dev/shm/w11120`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` **83,147** differing bytes,
`.ARM.extab` **659**, `.rel.plt` **0** (byte-identical).

Targets:

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `Status::missionCompleted` | `_ZN6Status16missionCompletedEbbx` | 722 | 320 | 1,432 |
| `LevelScript::process` | `_ZN11LevelScript7processEi` | 54,247 | 17,532 | 57,232 |

## 0. Bottom line

1. **The original's first jump-table body order is exactly the flat weight
   order `G1=4, G2=3, G3=2` (default 1), and the source order already equals
   that body order.**  The original's 150..189 `tbh` bodies are laid out
   `167/174`, then `150,151,152,153,154,155,158,162,166,170,184,189`, then
   `156,157,160,163,164,165,168,169,171,172`, then the loop latch.  Our
   unprofiled build lays the same bodies out ascending by case value; the
   prologue and both dispatch blocks are byte-identical up to the 7th
   halfword of the first `tbh` (case 156).  This is the whole 722-byte
   residual.
2. **The plateau is narrow in ratio, not in absolute weight.**  Measured by
   IR `!prof` injection: `[1,4,3,2]`, `[1,5,4,3]`, `[1,6,4,3]`,
   `[1,8,6,4]` -> **558/47/1432**; `[1,7,4,3]` -> 1028/394/1432;
   `[1,7,5,3]` -> 1055/191/1444; `[1,4,4,2]` -> 1252.  The requirement is
   `G1/G2 <= 1.5` **and** `G2/G3 <= 1.5` with `G3 >= 2`; a geometric vector
   (`[1,256,64,16,4,1]` -> 1297) is far outside.
3. **clang 7.0.2 has exactly one source-level switch-weight producer:
   `__builtin_expect` on the switch value, and it is two-level.**
   `LowerExpectIntrinsic::handleSwitchExpect` (LLVM 7.0.2) fills every
   successor with `UnlikelyBranchWeight` and sets exactly one case (or the
   default) to `LikelyBranchWeight`.  With the defaults that is
   `[1,2000,1,1,...]`; with `-mllvm -likely-branch-weight=4 -mllvm
   -unlikely-branch-weight=3` it is `[3,4,3,3,...]`.  It can never produce
   `4 > 3 > 2`.
4. **`[[likely]]`/`[[unlikely]]` do not exist in this clang.**
   `-std=c++11/14/17/2a` all print `warning: unknown attribute 'likely'
   ignored`; no `branch_weights` metadata is emitted.  (The build is
   `-std=c++11` anyway.)
5. **`__builtin_expect_with_probability` does not exist in this clang**
   (`error: use of undeclared identifier`).  Re-confirmed on this base.
6. **Nested `switch` lowering is multiplicative, not flat.**  A 2-level or
   4-level nested switch on the same value is folded by
   `SimplifyCFG::FoldValueComparisonIntoPredecessors`; outer cases are
   scaled by the inner switch's *total* weight (`T_inner`), inner cases by
   the outer default weight.  Real Status shapes measure 1291-1337 raw
   (sizes 1392-1460), not the 558 plateau.  A small synthetic fold with
   `-likely=4 -unlikely=3` emitted `[27, 5440, 4080, 408, 306..., 36, 27...]`
   -- three levels, but ratios ~13 and ~8.5.
7. **`__builtin_unreachable` in the default is worse and not size-exact**
   (1195/384/1420), and it is a soft lint criterion (`arm_builtin`) needing
   a same-line waiver.
8. **Loop rotation / loop-condition weights do not move the switch bodies.**
   `__builtin_expect(i < missions->size(), 1)` on the `for` condition is
   exactly neutral (722/320/1432).
9. **The MBP no-profile threshold is not the lever either.**
   `-mllvm -static-likely-prob=50/60/70/90` changes the layout
   (1276/429/1440, 1268/417/1444, 1268/378/1432, 1260/401/1424) but never
   reaches the grouped order.
10. **Verdict: the original's switch weights are profile-shaped.**  Every
    source-only producer in clang 7.0.2 gives either two levels
    (`__builtin_expect`) or a steep multiplicative sequence (nested
    switches); neither lands in the `G1/G2 <= 1.5, G2/G3 <= 1.5` plateau.
    The flat `4/3/2` vector is reachable only by `!prof` injection or a real
    profile.  No `src/`, `CMakeLists.txt`, baseline or allowlist change is
    landed.

## 1. The original body order

`prof_reconstruct_w10820.py extract` still works on this base (it is the
LevelScript mission-switch extractor):

```sh
W10820_SCRATCH=/dev/shm/w11120/run \
  python3 _work/reconstructions/prof_reconstruct_w10820.py extract
# [131, 64, 135, 145, 142, 144, 139, 69, 73, 67, 70, 29, 41, 80, 56,
#  125, 92, 157, 40, 102, 154, 24, 14, 21, 42, 91, 94, 105, 114, 16]
```

For `Status::missionCompleted` the first `tbh` is at `0xabfe2` (table base
`0xabfe6`, cases 150..189).  Decoding the original halfwords gives the
physical body order

```
167/174 (0xac036)
150 151 152 153 154 155 158 162 166 170 184 189   (0xac110 .. 0xac2ee)
156 157 160 163 164 165 168 169 171 172           (0xac32a .. 0xac41a)
latch/default (0xac45a)
```

The second `tbh` (8..14, base `0xac052`) is byte-identical in both builds
(`8, 0/11, 13, 14`).  The 4/3/2 classes are therefore
`G1={167,174}`, `G2={150,151,152,153,154,155,158,162,166,170,184,189}`,
`G3={156,157,160,163,164,165,168,169,171,172}`, default/latch 1.

## 2. Plateau constraints (IR `!prof` injection)

`_work/reconstructions/pgo_switchweight_w9920.py` is stale (`inject_weights`
looks for `], !dbg !9298`; the current IR uses `!9297`).  A worktree-local
copy with a bracket-based insertion reproduces the wave-152 numbers:

| vector `[default,G1,G2,G3]` | raw | aligned | size |
|---|---:|---:|---:|
| `[1,4,3,2]` | **558** | **47** | **1432** |
| `[1,5,4,3]` | 558 | 47 | 1432 |
| `[1,6,4,3]` | 558 | 47 | 1432 |
| `[1,8,6,4]` | 558 | 47 | 1432 |
| `[3,4,3,2]` | 558 | 47 | 1432 |
| `[1,7,4,3]` | 1028 | 394 | 1432 |
| `[1,7,5,3]` | 1055 | 191 | 1444 |
| `[1,4,4,2]` | 1252 | 332 | 1432 |
| `[1,256,64,16,4,1]` (wave 152) | 1297 | 465 | 1432 |
| `[1,2,2,1]` (wave 163) | 1372 | — | 1372 |

The default weight is irrelevant (`[3,4,3,2]` is identical to `[1,4,3,2]`);
what matters is `G1/G2 <= 1.5`, `G2/G3 <= 1.5`, `G3 >= 2`.

## 3. Source-level mechanisms in clang 7.0.2

### 3.1 `__builtin_expect` on the switch value is two-level

LLVM 7.0.2 `LowerExpectIntrinsic.cpp:57-85`:

```cpp
  SmallVector<uint32_t, 16> Weights(n + 1, UnlikelyBranchWeight);
  if (Case == *SI.case_default())
    Weights[0] = LikelyBranchWeight;
  else
    Weights[Case.getCaseIndex() + 1] = LikelyBranchWeight;
```

So `switch (__builtin_expect(t, K))` is `[U, ..., L at K, ..., U]` and
`switch (__builtin_expect(t, not-a-case))` is `[L, U, U, ...]`.  No other
switch metadata producer exists in clang 7.0.2 (`opt` is not shipped in the
NDK, but the source and the emitted IR agree).

Measured on the real Status source (full TU compile + gold link, this
worktree):

| source shape | flags | raw | aligned | size |
|---|---|---:|---:|---:|
| baseline | — | **722** | **320** | **1432** |
| `switch (__builtin_expect(m->getType(), 167))` | defaults | 1317 | 632 | 1392 |
| same | `-likely=4 -unlikely=3` | 887 | 349 | 1428 |
| same | `-likely=8 -unlikely=1` | 1008 | 339 | 1432 |
| `switch (__builtin_expect(m->getType(), 150))` | `-likely=4 -unlikely=3` | 957 | 356 | 1428 |

The `-likely=4 -unlikely=3` rows also re-weight the four
`__builtin_expect`s already in the case bodies (166, 184, 157, 163), which
is why the size moves; the pure switch vector `[3,4,3,3,...]` is
722/320/1432 (IR injection) -- i.e. it does not move the layout at all.

### 3.2 Nested switches are multiplicative

`SimplifyCFG::FoldValueComparisonIntoPredecessors` (LLVM 7.0.2) for the
"outer default goes to the inner switch" case does:

```cpp
  Weights.push_back(Weights[0] * SuccWeights[i + 1]);   // inner cases
  ...
  for (unsigned i = 1; i < CasesFromPred; ++i)
    Weights[i] *= ValidTotalSuccWeight;                 // outer cases * T_inner
  Weights[0] *= SuccWeights[0];                         // default
```

A synthetic 4-level chain with `-likely=4 -unlikely=3` emits
`[27, 5440, 4080, 408, 306 x11, 36, 27 x9]` -- three levels, but the
ratios are `T_inner` (~13) and `T_inner2` (~8.5), far outside the plateau.
On the real Status source:

| shape | flags | raw | aligned | size |
|---|---|---:|---:|---:|
| 4-level chain (G1 / 8..14 / G2 / G3) | defaults | 1295 | 617 | 1416 |
| same | `-likely=4 -unlikely=3` | 1337 | 702 | 1460 |
| same | `-likely=4 -unlikely=1` | 1324 | 550 | 1416 |
| same | `-likely=8 -unlikely=1` | 1322 | 660 | 1420 |
| 2-level tree (G1+8..14+G2 / G3) | defaults | 1291 | 662 | 1408 |
| same | `-likely=4 -unlikely=3` | 1298 | 412 | 1404 |
| same | `-likely=4 -unlikely=1` | 1301 | 453 | 1392 |

None is size-exact at 1432 and none is a raw win.

### 3.3 `[[likely]]` / `[[unlikely]]`

```sh
clang++ -target armv7-none-linux-androideabi16 -std=c++2a -Oz -S -emit-llvm
# warning: unknown attribute 'likely' ignored [-Wunknown-attributes]
```

Same for `c++11`, `c++14`, `c++17`; no `branch_weights` node is emitted.
The build is C++11, so this is doubly unavailable.

### 3.4 `__builtin_expect_with_probability`

```sh
clang++ ... -std=c++11 -c t.cpp
# error: use of undeclared identifier '__builtin_expect_with_probability'
```

### 3.5 `__builtin_unreachable` in the default

`default: __builtin_unreachable();` (soft lint criterion `arm_builtin`,
needs a same-line `// lint: arm_builtin` waiver) measures
**1195 / 384 / 1420** -- worse raw and 12 B short.  It removes the default
edge, so the remaining case edges are uniform and the layout stays
ascending.

### 3.6 Loop rotation / loop-condition weights

`for (unsigned i = 0; __builtin_expect(i < missions->size(), 1); i++)`
is exactly neutral: **722 / 320 / 1432**.  The loop header's branch weights
do not enter the dispatch block's successor order.

### 3.7 MBP's no-profile threshold

`-mllvm -static-likely-prob=N` (the `hasBetterLayoutPredecessor` threshold
when `!hasProfileData()`):

| N | raw | aligned | size |
|---:|---:|---:|---:|
| 50 | 1276 | 429 | 1440 |
| 60 | 1268 | 417 | 1444 |
| 70 | 1268 | 378 | 1432 |
| 90 | 1260 | 401 | 1424 |

It moves the layout but never into the grouped order.

## 4. LevelScript

`LevelScript::process` baseline on this base is **54,247 / 17,532 /
57,232**.  The harness still extracts the original 30-case mission order
`[131, 64, 135, 145, 142, 144, 139, 69, 73, 67, 70, 29, 41, 80, 56, 125,
92, 157, 40, 102, 154, 24, 14, 21, 42, 91, 94, 105, 114, 16]`, which is
likewise not ascending.  The same two-level limit applies: the only
source-level switch-weight producer is `handleSwitchExpect`, so a source
shape can at best put one case first and leave the rest ascending.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11120
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx

# plateau control (worktree-local bracket-based injector, see scratch)
python3 /dev/shm/w11120/inj.py groups 4 4 4 3 2      # 558/47/1432
python3 /dev/shm/w11120/inj.py twolevel 4 3 167      # 722/320/1432
python3 /dev/shm/w11120/inj.py groupsdef 1 7 7 7 4 3   # 1028/394/1432
```

Scratch (`/dev/shm/w11120/`): `inj.py` (bracket-safe `!prof` injector +
`groups`/`twolevel`/`groupsdef` modes), `try.sh` / `try2.py` (source-shape
compile+link+measure), `v_expect167.cpp`, `v_expect150.cpp`,
`v_nest4.cpp`, `v_nest2.cpp`, `v_unreach.cpp`, `v_loop.cpp`,
`Status.orig.cpp`, `t7..t11.cpp`.

## 6. Files changed

* `_work/reconstructions/build-provenance-round6-branch-weights.md` -- this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
