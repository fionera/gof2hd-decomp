# `Generator::createMission` — wave 110 (w5809)

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record 168 B).

Status: **source change landed. 82.1 % -> 82.6 %**, size still exact at
1996 B.  The change is a pure declaration-hoist: `padA` is declared next to
`amount` at the top of the function and assigned at its old site, instead of
being declared-and-initialised in one statement just before the reward
computation.

No ELF section changed.  `.ARM.extab` stays **0x26294** (still 4 B short of
the original 0x26298), `.jcr` stays 0xc0, 28 sections, file size +44 B.

## 1. The landed diff

```diff
     uint32_t race = agent->getRace();
     int level;
+    volatile double padA;
     volatile int amount;
@@
     Galaxy *galaxy = Globals::galaxy;
     int fromSystem = Globals::status->getStation()->getSystem();
     SolarSystem *fromSystemPtr = systems->data()[fromSystem];
-    volatile double padA = 0; (void) padA;
+    padA = 0; (void) padA;
     float reward = ...
```

That is the whole change.  It is a real source-shape change (a declaration
moved to the top of its scope, matching ordinary C++ style), not a padding
anchor or a compiler hint.

Why it helps: the stack-slot allocator sorts the frame objects by live-range
weight; moving the `padA` alloca changes the tie order of the remaining
objects.  Measured effect on the frame:

| object | base (82.1) | landed (82.6) | original |
|---|---|---|---|
| `agentName` | `[sp,#92]` | **`[sp,#80]`** | `[sp,#80]` |
| `amount` | `[sp,#104]` | `[sp,#92]` | `[sp,#28]` |
| `padA` | `[sp,#80]` | `[sp,#96]` | (none) |
| canary | `[sp,#108]` | `[sp,#108]` | `[sp,#92]` |
| frame | 112 | 112 | 96 |

`agentName` moving to `[sp,#80]` makes the three `add rX, sp, #80` sites
(`orig[533]`, `orig[648]`, `orig[713]`) exact; difflib replace blocks drop
from 86 to 83.

## 2. Measured table

All rows are the official scorer (`tools/verify` normalisation +
`difflib.SequenceMatcher` ratio) after one `link_so`, starting from the
landed wave-108/109 shape unless noted.  `pct / usize`.

### Declaration-position sweep (the productive area)

| variant | pct | usize |
|---|---:|---:|
| **`padA` declared before `amount`, assigned at old site (LANDED)** | **82.6** | **1996** |
| same, `padA` declared before `int level` | 82.6 | 1996 |
| same, `padA` declared before `race` | 82.6 | 1996 |
| `padA` declared after `amount` | 75.4 | 1996 |
| `padA` declared + assigned at top (`pad_decl_early`) | 75.4 | 1996 |
| `padA` declared at top, initialised at declaration | 75.2 | 2004 |
| `padA` declared at top with `(void)` at declaration | 75.2 | 2000 |
| `padA` declared + initialised before `galaxy` | 82.1 | 1996 |
| `padA` after `galaxy` | 81.2 | 1996 |
| `padA` after `fromSystem` | 81.5 | 1996 |
| `padA` after `reward` | 80.4 | 2000 |
| `padA` after `agentName` / `difficulty` / `itemId` | 79.7 | 1996 |
| `padA` and `amount` declarations swapped | 82.1 | 1996 |
| two `volatile double` pads | 78.7 | 2004 |

### Pad type sweep (all with the landed hoist)

| pad type | pct | usize |
|---|---:|---:|
| `volatile double` (LANDED) | **82.6** | **1996** |
| `volatile long long` | 78.5 | 1996 |
| `volatile float` | 78.8 | 1992 |
| `volatile int` | 78.4 | 1992 |
| non-volatile `double` | 71.5 | 1984 |

### `amount` sweep

| variant | pct | usize |
|---|---:|---:|
| `volatile int amount` (LANDED) | **82.6** | **1996** |
| non-volatile `int amount` | 70.5 | 1992 |
| `amount = count + 5;` before `getTecLevel()` | 82.3 | 1996 |
| `int tec = getTecLevel(); amount = count + 5; difficulty = tec;` | 80.3 | 2000 |
| `amount = count; amount += 5;` | 81.5 | 1996 |

### p12 / loop / getRoutes re-checks on top of the landed shape

| variant | pct | usize |
|---|---:|---:|
| landed (`p10 exp1`, `p12 exp0`) | **82.6** | **1996** |
| p12 plain (no `__builtin_expect`) | 79.9 | 1996 |
| p12 inverted `if (pick != 12) ... else ...` | 79.9 | 1996 |
| p12 `else { selected = true; }` form | 82.6 | 1996 |
| p12 `!__builtin_expect(pick != 12, 1)` | 82.6 | 1996 |
| p12 `selected` before `getIndex` check | 76.2 | 1992 |
| `expect(getIndex()==25, 0)` | 69.9 | 1992 |
| `expect(getRoutes()==nullptr, 0)` | 62.4 | 2000 |
| `expect(getRoutes()==nullptr, 1)` | 48.2 | 2040 |
| inverted `getRoutes()!=nullptr` (plain) | 82.6 | 1996 |
| `getRoutes` temp `bool noRoutes` | 82.6 | 1996 |
| `getRoutes` temp + `expect(noRoutes, 0)` | 82.6 | 1996 |
| `type = nextInt(2)==0 ? 1 : 4` | 82.6 | 1996 |
| `if/else` assigning `type = 1/4` | 82.6 | 1996 |
| `if (type == 13 && race < 4)` (swapped `&&`) | 82.6 | 1996 |

Nothing beat 82.6 / 1996.

## 3. Proven vs hypothesis

Proven (measured this pass):

* Hoisting the `padA` declaration next to `amount` is worth +0.5 pt and keeps
  the function size-exact at 1996 B.  It moves `agentName` from `[sp,#92]` to
  `[sp,#80]`, matching the original.
* All other pad declaration positions either tie (before `level` / before
  `race`) or lose.  Initialising at the declaration loses 7 points.
* `volatile int amount` is required: non-volatile `amount` drops to 70.5 %
  and 1992 B.
* `volatile double` is the only pad type that keeps both 1996 B and the
  score; `long long` / `float` / `int` pads all lose 4+ points.
* The `p12`/loop-cond coupling is unchanged by the hoist: p12 plain gives
  the original `cmp r6,#12; bne` successor order but moves the loop cond to
  the top (79.9 %).
* `expect(getRoutes()==nullptr, 0)` does produce the original out-of-line
  cold block (`cmp; beq.w`) but costs 4 B (2000 B, not size-exact) and drops
  the score to 62.4 %.

Hypothesis (not closed):

* The remaining 17.4 % is the pick/type arm block layout: the swapped
  `p12` successors, the inlined `getRoutes()==nullptr` block, and the
  `amount` store sink (ours 4 stores to `[sp,#92]`, original 5 stores to
  `[sp,#28]`), plus the `agent`/`targetStation` stack-slot swap and the
  `difficulty` register (`r5` vs original `r6`).  These are all downstream of
  the frame/RA layout; no source spelling tried this pass flips them.
* The original frame is 96 with no pad object; our 8-byte `padA` forces the
  frame to 112.  A real source construct that generates the pad's 12 code
  bytes without an 8-byte frame object would be needed to reach frame 96.

## 4. Commands

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Gate at the end of this pass:

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Single-function score:

```
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
```

Scratch files from this pass live under `/dev/shm/w5809/` (`batch2.py`,
`score1.py`, `variants_*.json`, `mi_win.txt`, `diff_win.txt`); they are not
part of the repo.
