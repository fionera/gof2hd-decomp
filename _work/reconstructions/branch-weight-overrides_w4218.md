# Per-file branch-weight overrides: remove three, keep StarMap's 7:1 (w4218)

Status: code change landed, 2026-11-03. Worktree `wt/w4218`, branch `wave92-w4218`,
base master `b379d00d`. Every measurement below was made in this worktree against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's
NDK r18b clang 7.0.2 and the official `tools/verify/verify.py` (`autojunk=False`).
Scratch harnesses live in `/dev/shm/w4218` (`measure.py`, `weights.py`, `starvar*.py`,
`oracle.py`).

## TL;DR

`CMakeLists.txt` carried four per-file `-mllvm -likely-branch-weight` /
`-mllvm -unlikely-branch-weight` overrides. They are not original build flags; they
were matching knobs. This pass removes three of them:

| TU | old override | why it can go |
|---|---|---|
| `src/game/weapons/Radar.cpp` | `80:1` | default `2000:1` is gate-neutral; official pct unchanged |
| `src/game/menu/SpaceLounge.cpp` | `20:12` | default `2000:1` is gate-neutral; official pct unchanged |
| `src/game/ui/MenuTouchWindow.cpp` | `5:1` | default `2000:1` is gate-neutral; official pct unchanged |

`src/game/world/StarMap.cpp` keeps `7:1`. It is irreducible with clang 7.0.2: the
branch needs exactly a 7:1 branch weight, and the only source-level weight mechanism
(`__builtin_expect`) has a fixed default of 2000:1 (or 1:2000). Removing it drops
`byte_exact` 4498 -> 2404.

The `__builtin_expect` calls themselves stay in all four files. They are source-level
hints (the original's default weight is 2000:1), not compiler-flag knobs; removing
them changes function sizes and would break the gate.

## Gate (before -> after)

| metric | before | after |
|---|---:|---:|
| byte_exact | 4498 / 4524 | **4498 / 4524** |
| linked_exact | 4498 / 4524 | **4498 / 4524** |
| size-exact | 4524 / 4524 | 4524 / 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 |
| data_exact | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| avg_match | 99.91 | 99.91 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

`report.json` is byte-identical before and after: the official per-function percentages
(rounded to 0.1) and the summary are unchanged. `.text`, `.rodata`, `.ARM.exidx` and
`.ARM.extab` are unchanged.

The earlier flag studies (`radar-draw-three-shape-9908-w4105.md`,
`spacelounge-drawlounge-w4118.md`, `menutouchwindow-c1-c2-pass4-two-value-selects-w4116.md`)
swept these ratios with the object-level oracle. Those sweeps show the flags move the
oracle score by a few tenths, but the official linked verifier rounds the same functions
to the same 0.1% with or without the flag, and the gate counts are identical.

## 1. The three removed overrides

All three TUs use `__builtin_expect`; the overrides only changed the weight that
`LowerExpectIntrinsic` writes into `!prof`. With the override removed the hint keeps
clang's default `2000:1`, which is what the original release used.

### 1.1 Radar.cpp (`80:1`)

Three hints in `Radar::draw`:

* line 409 `__builtin_expect(lockedAsteroid == nullptr && lockedGasCloud == nullptr && field_0x1c == nullptr, 0)`
* line 429 `__builtin_expect(this->onScreen, 1)`
* line 478 `__builtin_expect((i & 0x7fffffff) == 1 || (i & 0x7fffffff) == 3, 0)`

`Radar::draw` is 9908 B either way. The object-level oracle (which compares
object-relative normalized instruction streams) moves 74.0 -> 72.5, i.e. the flag
did change some branch encodings, but the official linked verifier still reports
**73.9%** and `byte_exact`/`linked_exact` are unchanged. The flag was a matching
knob, not an original build flag.

### 1.2 SpaceLounge.cpp (`20:12`)

One hint in `SpaceLounge::drawLounge`, line 1211:

```cpp
if (__builtin_expect(agent->isKnown(), 1)) {
```

`drawLounge` is 1948 B either way. Object-level oracle 95.5 -> 95.3; the official
verifier reports **95.6%** both ways and the gate is unchanged.

### 1.3 MenuTouchWindow.cpp (`5:1`)

Four hints in `MenuTouchWindow::OnTouchEnd`:

* lines 2885 and 2912 `__builtin_expect(button->OnTouchEnd(x, y), 1)`
* line 3018 `__builtin_expect(this->choiceWindow->OnTouchEnd(x, y) == 0, 1)`
* line 3020 `__builtin_expect(this->loadGame(this->selectedRow) == 0, 0)`

`OnTouchEnd` is 8132 B either way. Object-level oracle 97.9 -> 97.8; the official
verifier reports **95.2%** both ways. `OnTouchBegin` (1896 B, 97.7%) and the
constructors (9600 B, 93.9%) are unaffected by the flag (they contain no hint).

## 2. Why StarMap's 7:1 stays

### 2.1 The exact branch

`StarMap::drawOnScreenInfo` @ `0xc8354`, 3488 B (0xda0). The relevant source is the
campaign-mission 120 arm:

```cpp
} else if (Globals::status->getCurrentCampaignMission() == 120 &&
           (stationMode ? (*this->stations)[index]->getIndex() == 93
                        : (bool) __builtin_expect(
                              (int) (*this->systems)[index]->getStationEnumIndex(93) >= 0, 1))) {
    this->iconBuffer[1] = this->keyImageWanted;
}
```

With the 7:1 override the final IR is (this is the only branch-weight metadata in
the function):

```llvm
%186 = call i32 @_ZN11SolarSystem19getStationEnumIndexEi(...)
%187 = icmp sgt i32 %186, -1
br i1 %187, label %188, label %193, !prof !7595
!7595 = !{!"branch_weights", i32 7, i32 1}
```

Without the override the same branch is:

```llvm
!7595 = !{!"branch_weights", i32 2000, i32 1}
```

The IR shape is otherwise identical. The weight changes the block frequencies
(`-mllvm -print-machine-bfi` shows BB29..BB39 shift, e.g. BB29 0.020477 -> 0.022331),
which moves the mission-116 block in/out of the fall-through path and changes the
`campaign`/`freelance` register assignment.

### 2.2 Removing it is a hard gate regression

| build | `drawOnScreenInfo` | `StarMap::draw` @ | `.text` | byte_exact | linked_exact |
|---|---:|---:|---:|---:|---:|
| 7:1 (landed) | 0xda0 (3488) | 0xc90f4 | 0x1337c8 | 4498 | 4498 |
| no override | 0xdac (3500) | 0xc9100 | 0x1337e8 | **2404** | **4497** |

The 12-byte growth shifts every later function; every PC-relative reference then
differs, hence the 2094-function drop. This was re-measured in this worktree with
`cmake --build cmake-build-match --target check` after deleting the StarMap block.

### 2.3 No natural heuristic gives 7:1

LLVM 7's `BranchProbabilityInfo` heuristics are fixed ratios:

| heuristic | ratio |
|---|---|
| zero compare (`calcZeroHeuristics`) | 20:12 |
| pointer compare (`calcPointerHeuristics`) | 20:12 |
| floating compare (`calcFloatingPointHeuristics`) | 20:12 |
| loop backedge (`calcLoopBranchHeuristics`) | 124:4 or 62:4 |
| cold call (`calcColdCallHeuristics`) | 4:64 |
| invoke (`calcInvokeHeuristics`) | 1048575:1 |
| no heuristic | 1:1 |

None is 7:1. The branch is `icmp sgt i32 %x, -1` (InstCombine's canonical form of
`x >= 0`), so the natural zero-compare heuristic is 20:12. The only source-level
weight mechanism in clang 7.0.2 is `__builtin_expect`, whose `LowerExpectIntrinsic`
pass writes `(LikelyBranchWeight, UnlikelyBranchWeight)` = `(2000, 1)` by default
(or `(1, 2000)` for an expect-false). `__builtin_expect_with_probability` does not
exist in this clang:

```
$ clang++ ... -c t.cpp
t.cpp:2:19: error: use of undeclared identifier '__builtin_expect_with_probability'
```

### 2.4 Source shapes measured (object-level oracle, `autojunk=False`)

All rows are the real `StarMap.cpp` with the override removed, measured with the
object-level oracle (`measure.py`, the same normalized-instruction `difflib` ratio
as the official verifier but with object-relative branch targets). `usize` is the
function section size; `pct` is that ratio. The target is 3488 B / 100.0%
(the official verifier also reports 3488 B / 100.0% for the 7:1 control).

| variant | usize | pct |
|---|---:|---:|
| baseline with 7:1 (control) | **3488** | **100.0** |
| baseline, no override | 3500 | 87.3 |
| plain condition, no `__builtin_expect` | 3480 | — |
| expect around the whole `else if` condition | 3508 | 81.6 |
| expect around `getCurrentCampaignMission() == 120` | 3520 | 79.7 |
| expect around the ternary | 3496 | 85.6 |
| expect around `stationMode` | 3496 | 85.6 |
| nested `if (mission == 120) { if (...) }` | 3500 | — |
| nested `if` + `bool target` + expect | 3500 | — |
| split station/system paths | 3500 | — |
| `(bool) __builtin_expect((x >= 0) != 0, 1)` | 3500 | 87.3 |
| `(bool) __builtin_expect(!!(x >= 0), 1)` | 3500 | 87.3 |
| `__builtin_expect((x >= 0) != 0, 1)` (no `(bool)` cast) | 3488 | 93.8 |
| `__builtin_expect(!!(x >= 0), 1)` (no cast) | 3488 | 93.8 |
| `__builtin_expect(x >= 0, 0)` (no cast) | 3488 | 93.8 |
| `__builtin_expect(x >= 0, 1)` (no cast) | 3488 | 93.8 |
| expect on both mission and enum | 3508 / 3528 | 79.6–81.6 |
| expect on mission (0/1) + enum (0/1) grid | 3520–3532 | 77.9–79.7 |

The four 3488-byte variants are **not byte-exact**: dropping the `(bool)` cast makes
the ternary type `int`, so `&&` normalizes both arms through `!= 0`; the station arm
becomes `subs #93; clz; lsrs` instead of `cmp #93; bne`, and the allocator swaps
`campaign`/`freelance` (campaign `[sp,#24]`, freelance `r6`). The target keeps the
station arm as a direct comparison and campaign in `r6`.

A weight sweep over the exact same source shape confirms the narrow window:

| `-likely-branch-weight` (unlikely=1) | `drawOnScreenInfo` |
|---:|---:|
| 1..6 | 0xd98 (3480) |
| **7** | **0xda0 (3488)** |
| 8..2000 | 0xdac (3500) |

Only 7:1 reproduces the original block frequencies and allocation. The 7:1 override
is therefore the faithful encoding of the original's branch probability at this
site, not an arbitrary anchor.

## 3. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
# verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

Single-function oracle (compiles the real TU and compares the function's section):

```sh
python3 /dev/shm/w4218/measure.py --src src/game/world/StarMap.cpp \
    --fn _ZN7StarMap16drawOnScreenInfoEib            # with 7:1 -> 3488 / 100.0
python3 /dev/shm/w4218/measure.py --src src/game/world/StarMap.cpp \
    --fn _ZN7StarMap16drawOnScreenInfoEib --strip-bw # no override -> 3500 / 87.3
```

## 4. Evidence locations

Scratch (not committed): `/dev/shm/w4218/` — `measure.py` (object-level oracle),
`weights.py` (final-IR branch-weight dump), `starvar*.py` (source-shape batches),
`oracle.py` (patched w3692 oracle), `bfi_flag.txt` / `bfi_nostrip.txt`
(`-print-machine-bfi` block frequencies), `ours_flag.txt` / `ours_nostrip.txt`
(linked disassembly dumps), `CMakeLists.three_removed.txt`.
