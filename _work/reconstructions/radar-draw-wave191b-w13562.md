# Radar::draw wave 191b (w13562) — campaign/`sl` root cause pinned, no size-exact flip

**Status:** complete, 2026-11-13. Worktree `wt/w13562`, branch `wave191b-w13562`,
base `master` `4a865a92e`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191b-w13544.md` (guard ABCDE, base 6,013) and
`radar-draw-campaign-sl-wave167-w11206.md`. **No source change landed**: every
candidate measured on this base is raw-neutral (6,013) or worse with size exact.
`src/` is untouched.

## 0. Baseline (verified)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,965 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.rel.plt` | byte-identical |

## 1. New evidence: the interval sizes and the greedy-RA priority

The full LLVM 7.0.1 source is on the box (`/tmp/llvm-7.0.1.src`). The greedy
allocator is **not** weight-ordered; `RAGreedy::enqueue`
(`lib/CodeGen/RegAllocGreedy.cpp:681`) is:

```cpp
const unsigned Size = LI->getSize();
...
  Prio = (1u << 29) + Size;          // global ranges: long -> short
...
Prio |= (1u << 31);                  // global/local above RS_Split
if (VRM->hasKnownPreference(Reg))    // a COPY hint to a physreg
  Prio |= (1u << 30);                // hinted ranges go first
```

So the queue is **(has a physreg COPY hint) → (live-range size) → (vreg no.)**.
Spill weight only matters later, for eviction.

Measured live intervals in our build (recomputed from the pre-RA MIR dump at
`mim.txt:395122`, 16 slot units per machine instruction):

| vreg | what | def slot | last use | span |
|---|---|---:|---:|---:|
| `%2487` | shared zero / `missionBlocksPlanets` | 240 | 36,640 | **36,400** |
| `%422` | `campaign` | 2,128 | 24,448 | **22,320** |
| `%420` | `alienOrbit` | 2,032 | 24,560 | 22,528 |

`%2487` is `rgpr` (rematerializable) with six defs (`t2MOVi 0` ×5 at slots
240/880/1088/1296/1504 plus `t2SUBri`+`t2MOVCCi` at 1664/1696) and is used for
the two `strb` zero stores (`plasmaInRange`/`lockConfirmed`) **and**
`missionBlocksPlanets`.  `%422` is a plain `COPY $r0` from the
`getCurrentCampaignMission()` call, i.e. it *is* hinted (bit 30) while `%2487`
is not.

Post-RA (VirtRegRewriter dump) `%2487` → `$r10` (sl) and `%422` → `%stack.45`
(sp+0x90).  The original instead has `%2487` in **fp** (`mov.w fp,#0` at
`0x12e07a`, re-materialised at `0x12e0d8`, `subs.w fp,r0,#172` at `0x12ebe0`)
and `%422` in **sl** (`mov sl,r0` at `0x12e0f8`, `cmp.w sl,#2` at `0x12e38a`).

Additional original-only facts recovered this pass:

* `missionBlocksPlanets` is **spilled at `0x12e3ba`** (`str.w fp,[sp,#112]`,
  i.e. sp+0x70) right before the planet block and **reloaded into r5**: the
  planet-block test is `cmp r5,#0` at `0x12ffae`, not `cmp fp,#0`.  In ours it
  stays in sl and is spilled to `%stack.42` / reloaded into `$r10`.
* The original's `sl` is reused `campaign` → `stations`
  (`mov sl,r0` at `0x12e3c6`, spilled to sp+0x74), which is only possible
  because campaign's live range ends at `0x12e38a` and stations starts at
  `0x12e3c6`.
* `mode` is at sp+0x64 in the original and sp+0x68 in ours (the very first
  differing byte, `+0x0e`); `hud` (sp+0x54) and `player` (sp+0x88) match.
* Every byte before `+0x33a` differs only in branch-target immediates, i.e. the
  whole residual is the single campaign/zero register+stack-slot split.

The contradiction that blocks a source fix: `%422` is hinted and therefore
dequeued *before* `%2487`, yet it is the one that gets spilled.  For the
original's colouring to be reproduced, either `%2487` must gain a hint (so it
is dequeued first and takes sl, pushing campaign into fp — but the original has
the opposite assignment) or one of the >36,000-slot hinted globals must stop
taking sl.  `-mllvm -split-spill-mode=size` does put `campaign` in `sl`
(`mov sl,r0` at `0x12e0f6`) but is size-fatal (9,892 B) and re-lays the frame
(304 bytes), i.e. it is a different allocation, not this one.

## 2. Negative cells measured this pass (all reverted; base 6,013 / 9,908)

### 2.1 `__builtin_expect` flip sweep (all 30 `expect` sites in `draw`)

Flipping `0`↔`1` at each site individually.  None is a raw win:

| site | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| 0 `!mission->isEmpty()` 1→0 | 9,556 | 9,888 |
| 1 landmark guard 0→1 | 9,476 | 9,896 |
| 2 `landmarks->data()[i]==nullptr` | 9,265 | 9,920 |
| 3 `onScreen` | 9,505 | 9,872 |
| 4 `screenX > centerX-...` | 9,568 | 9,868 |
| 5 `(i&0x7fffffff)==3` | 8,458 | 9,904 |
| 6 campaignMission guard | 9,300 | 9,964 |
| 7 `cur != 120` 1→0 | 6,027 | 9,908 |
| 8 `getStatusValue()&...` | 8,468 | 9,912 |
| 9 `!lockConfirmed` 0→1 | 6,178 | 9,904 |
| 10 `field_0x18 != 0` | 6,185 | 9,912 |
| 11 outer `expect(isDockingToPlanet)` 0→1 | 6,015 | 9,908 |
| 12 inner `isDockingToPlanet` 1→0 | 6,013 | 9,908 |
| 13 `enemyFlagsLo` (enemy loop) | 8,680 | 9,888 |
| 14 `screenX < cx+...` | 6,095 | 9,908 |
| 15 `enemyFlagsLo` 1→0 | 6,013 | 9,908 |
| 16 `enemyFlagsLo` 0→1 | 8,485 | 9,900 |
| 17 `carriesFriendCargoFlag` 0→1 | 6,013 | 9,908 |
| 18 `field_0x24` 0→1 | 8,416 | 9,904 |
| 19 `!isDockedToDockingPoint` 1→0 | 8,744 | 9,936 |
| 20 `enemyLockTimer > ...` | 8,568 | 9,940 |
| 21 `dockNavPtr != current` | 8,534 | 9,932 |
| 22 `cloud == locked` | 7,115 | 9,908 |
| 23 `field_0x1bc >= 1` | 6,177 | 9,912 |
| 24 `(cur != 151 && alwaysEnemySeen)` | 7,211 | 9,912 |
| 25 `field_0x1bc < 3` | 6,158 | 9,912 |
| 26 `field_0x1bc < 5` | 7,182 | 9,900 |
| 27 `alwaysEnemySeen` (ladder) | 7,231 | 9,912 |
| 28 `bossSeen` | 6,157 | 9,908 |
| 29 `alwaysEnemySeen` (ladder 2) | 7,231 | 9,912 |

### 2.2 `campaign` declaration placement / spelling

| cell | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| after `planetTargets` assignment | 6,090 | 9,908 |
| after `asteroids`/`gasClouds`/`enemies`/`route`/`landmarks` | 9,180 | 9,904 |
| after `MatrixGetInverse` | 9,130 | 9,912 |
| after `cameraZ` | 6,137 | 9,908 |
| direct `getCurrentCampaignMission()` in guard (no local) | 9,236 | 9,904 |
| `const int` | 6,013 | 9,908 |
| `long` / `register int` / `(int)(...)` / `+0` / `\|0` / `*1` | 6,013 | 9,908 |
| two-statement `int campaign; campaign = ...;` | 6,013 | 9,908 |
| `int campaign = 0; campaign = ...;` | 6,013 | 9,908 |
| `unsigned int` | 6,014 | 9,908 |
| `volatile int` | 9,208 | 9,892 |
| `bool campaignReady = campaign >= 2;` (early or at guard) | 6,013 | 9,908 |
| `int campaignReady = campaign - 2;` | 6,013 | 9,908 |
| `int campaignBlock = campaign >= 2;` | 6,013 | 9,908 |
| `bool canDock = ...` hoisted | 9,615 | 9,920 |

### 2.3 `campaign` guard operand order / spelling

| cell | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| `campaign > 1` / `2 <= campaign` / `!(campaign < 2)` / `>= 0x2` | 6,013 | 9,908 |
| `campaign >= 2U` / `(unsigned) campaign >= 2` | 6,014 | 9,908 |
| `planetTargets != 0` / bare / `!= NULL` | 6,013 | 9,908 |
| `isDockingToPlanet() == 0` / `== false` | 6,013 | 9,908 |
| `alienOrbit == 0` | 8,515 | 9,920 |
| `!alienOrbit != 0` | 6,022 | 9,908 |
| planetTargets first | 8,791 | 9,924 |
| alienOrbit first | 8,824 | 9,920 |
| docking first | 8,644 | 9,920 |
| nested `if (campaign >= 2) { if (...) {` | 6,013 | 9,908 |

### 2.4 Zero / `missionBlocksPlanets` spellings

| cell | raw | size |
|---|---:|---:|
| `plasmaInRange/lockConfirmed = false` / `(uint8_t)0` / `'\0'` | 6,013 | 9,908 |
| `uint8_t zeroByte = 0;` used for both | 6,013 | 9,908 |
| `x = (uint8_t)(x & 0)` for both | 6,013 | 9,908 |
| store order swapped (`lockConfirmed` first) | 6,015 | 9,908 |
| move both stores after `SetColor` / after `getMission` | 9,260 / 6,055 | 9,916 / 9,908 |
| `missionBlocksPlanets` decl moved after guard/`alwaysEnemySeen`/`alwaysEnemy`/if | 6,013 | 9,908 |
| `int missionBlocksPlanets = (int) false;` | 6,013 | 9,908 |
| `bool missionBlocksZero = false; int mbp = missionBlocksZero;` | 6,013 | 9,908 |
| `int mbp = (mission->getType() == 0) ? 0 : 0;` | 9,183 | 9,912 |

### 2.5 Other early locals

| cell | raw | size |
|---|---:|---:|
| `bool turretMode` → `int`/`char`/`uint8_t`/`unsigned char` | 9,226 | 9,860 |
| `int alienOrbit` → `unsigned`/`long`/`char`/`unsigned char` | 6,013 | 9,908 |
| `int alienOrbit` → `bool` | 6,022 | 9,908 |
| `int notTurretMode = turretMode ? 0 : 1;` | 6,013 | 9,908 |
| `int notOtherLock = otherLock ? 0 : 1;` | 6,013 | 9,908 |
| `int otherLock = (int) otherLockFlag;` | 6,013 | 9,908 |
| `bool onPlanetCourse` → `int` | 6,013 | 9,908 |
| `bool landmarkHit`/`planetHit`/`marked`/`bossSeen`/`enemyHit` → `int` | 8,730 / 8,460 / 8,464 / 9,186 / 8,615 | 9,936 / 9,912 / 9,912 / 9,924 / 9,940 |
| `bool otherLockFlag` → `int` | 9,447 | 9,896 |

### 2.6 Music tail (all 5 non-base orders of the three predicates)

`A = (uint32_t)(current-136) > 15`, `B = expect(current != 151 && alwaysEnemySeen, 0)`,
`C = ((1 << (current-136)) & 0xe071) == 0`.

| order | raw | size |
|---|---:|---:|
| **ABC (base)** | **6,013** | 9,908 |
| ACB | 6,036 | 9,908 |
| BAC | 6,017 | 9,908 |
| BCA | 6,166 | 9,904 |
| CAB | 6,166 | 9,904 |
| CBA | 6,163 | 9,896 |

Sound-ladder orders (`isAttackedByAliens`, `getCurrentCampaignMission()==16`,
`inAlienOrbit`): base 6,013; the other five are 6,018–6,029, all size exact.

### 2.7 Adjacent statement swaps, lines 560–950 (previously unswept)

127 same-indent adjacent swaps; every one is 6,013 or worse with size 9,908
(best = 6,013, i.e. neutral).  Combined with w13544 §5.4 (950–1360) and the
earlier 337–560 sweep, the whole function's adjacent-swap space is now covered.

## 3. Conclusion

The residual is one register-allocation decision (`%2487` zero/mbp → sl vs fp,
`%422` campaign → spill vs sl) plus the resulting frame-slot permutation.  It is
robust against every source spelling tried across waves 162–191b and this pass
(≈120 cells here): the IR the front end produces is byte-stable under all of
them, and the greedy allocator's `(hint, size)` priority is not reachable from
the source without changing a live range enough to change the size.  The one
knob that reaches `campaign → sl` is `-mllvm -split-spill-mode=size`, which is
size-fatal (9,892 B) and produces a different frame.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13562/`: `try.py` (1 s
compile+link+fn_diff cell, absolute source path required), `cell.py`
(`measure([(old,new),...], tag)`), `sbs.py`, `rawdiff.py`, `swaps.py`,
`livecalc.py` (recomputes per-vreg live spans from the MIR dump), `mim.txt`
(`-print-machineinstrs`, Greedy RA for `Radar::draw` at line 404141, pre-RA at
395122), `mi2.txt`, `mi.txt`.  LLVM 7.0.1 source tree for reference:
`/tmp/llvm-7.0.1.src`.
