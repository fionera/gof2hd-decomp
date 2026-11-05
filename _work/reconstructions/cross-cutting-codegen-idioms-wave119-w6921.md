# Cross-cutting codegen idioms in the 19 non-exact functions — wave 119 / w6921

Status: **research note, no source change landed, 2026-11-05**. Worktree
`/home/debian/GalaxyOnFire2/wt/w6921`, branch `wave119-w6921`, base master
`24243db02` (wave-119 harvest). Target `_ZN11LevelScript7processEi`
(`src/game/world/LevelScript.cpp`), original `0x137d18`, **57232 B (0xdf90)**,
20169 original instructions.

The target and the whole tree are unchanged: `LevelScript::process` stays at
**59.489705 % / 57232 B / 20043 ours insns**.  Gate on entry and exit:
`RATCHET PASS`, `byte_exact 4505`, `linked_exact 4505`, size-exact 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.  27 sections, file size
2,187,328 B, `.ARM.extab` 0x26290 (original 0x26298, 4 B short),
`.rodata` 0x41c27 (original 0x41b67, known `Status::missionCompleted` rank
table).  No section, extab, import or data byte changed.

This note answers the wave-119 cross-cutting question: classify the residual
of all 19 non-exact symbols by cause, then test the generic source idioms
(loop induction type, `bool` vs `int`, `const` locals, explicit comparisons,
early-return shape, predicate probability) on the cheapest functions and see
whether any idiom generalises.  **Answer: none of the syntactic idioms
generalises as a win.**  `const`, loop-induction spelling, explicit
comparisons and early-return shape are canonicalised by clang 7.0.2 and
produce byte-identical code; `bool`→`int` is neutral or worse; predicate
probability is the only cross-cutting mechanism that has ever landed a win
(`MGame::OnTouchEnd`, `PlayerEgo::calcCollision`), but it is function-specific
and destructive on `LevelScript::process`.

## 1. Exact commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w6921
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505

# single-function score (verify.py's own disassemble/reconcile/difflib)
python3 /dev/shm/w6921/score.py
# taxonomy of all 19 non-exact symbols
python3 /dev/shm/w6921/tax.py
# one source variant: compile the TU, gold-link, score, restore
python3 /dev/shm/w6921/harness.py src/game/world/LevelScript.cpp OLD NEW
```

The score is the unrounded
`difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked
`.so`; `verify`'s one-decimal display rounds `LevelScript` to 59.5 %.  The
variant harness compiles one TU with the exact `compile_commands.json` flags,
gold-links against the checked-in objects, scores the target symbol, then
restores the source (`/dev/shm/w6921/harness.py`; ~4 s/variant).  Every
variant below was a full `link_so` build, not an object-only estimate.

## 2. Taxonomy method

For each of the 19 non-exact symbols:

1. Disassemble both bodies with `verify.disassemble` and reconcile the streams
   exactly as the official scorer does.
2. Compute the LCS (`difflib.SequenceMatcher(..., autojunk=False)`).
3. LCS-matched instructions whose bytes differ are **raw-only** (literal-pool
   words and PC-relative addresses that differ between two independent links;
   the fuzzy metric masks them).
4. Pair the remaining unmatched instructions greedily, in this order:
   * exact normalised text → **scheduling** if the partner is ≤20 instructions
     away, else **block order**;
   * same shape after canonicalising GPR/VFP register names → **register
     allocation**;
   * same mnemonic → **imm/operand** (includes stack-slot `[sp,#N]` changes and
     literal-pool offsets);
   * remaining original-only → **structural**; remaining ours-only → **insert**.
5. Assign each paired instruction's bytes on both sides to its category.  The
   category sums therefore approximate the instruction-stream diff size (they
   differ from the raw byte-offset diff by the raw-only double count).

Script: `/dev/shm/w6921/tax.py` (imports `tools/verify/verify.py`; not tracked).
The method is the wave-111 `residual-taxonomy-wave111-w5920.md` §3 method,
re-run on the current master.

## 3. Per-function taxonomy (current master)

`diff B` is the category sum (both sides).  `%` is the unrounded verifier
ratio.  C1/C2 aliases have identical bodies and are listed separately because
the report counts them separately.

| function | size | % | diff B | raw-only | RA | block | sched | imm/op | struct | ins |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `Layout::Layout` C1 | 7072 | 48.96 | 7894 | 66 | 2596 | 1352 | 228 | 2256 | 702 | 694 |
| `Layout::Layout` C2 | 7072 | 48.96 | 7894 | 66 | 2596 | 1352 | 228 | 2256 | 702 | 694 |
| `LevelScript::process` | 57232 | 59.49 | 54598 | 7906 | 7616 | 26540 | 404 | 10366 | 890 | 876 |
| `Level::createShip` | 2764 | 70.28 | 1754 | 206 | 776 | 92 | 100 | 372 | 104 | 104 |
| `Radar::draw` | 9908 | 74.43 | 5732 | 1144 | 940 | 600 | 248 | 2400 | 200 | 200 |
| `MGame::OnTouchEnd` | 8080 | 81.28 | 4330 | 1470 | 36 | 1108 | 76 | 1580 | 30 | 30 |
| `Generator::createMission` | 1996 | 83.61 | 696 | 144 | 104 | 68 | 72 | 256 | 26 | 26 |
| `HangarWindow::render` | 9700 | 91.34 | 2342 | 738 | 644 | 244 | 88 | 464 | 82 | 82 |
| `Level::createMission` | 17172 | 92.29 | 3404 | 852 | 1212 | 276 | 116 | 756 | 96 | 96 |
| `Status::missionCompleted` | 1432 | 92.45 | 340 | 156 | 40 | 12 | 28 | 80 | 12 | 12 |
| `Level::createScene` | 2252 | 92.76 | 308 | 20 | 144 | 16 | 12 | 32 | 42 | 42 |
| `Gun::calcCharacterCollision` | 1984 | 93.38 | 262 | 46 | 80 | 32 | 0 | 80 | 12 | 12 |
| `PlayerEgo::calcCollision` | 968 | 95.24 | 186 | 114 | 8 | 0 | 0 | 28 | 18 | 18 |
| `SpaceLounge::drawLounge` | 1948 | 95.68 | 198 | 30 | 84 | 0 | 16 | 40 | 14 | 14 |
| `Hud::draw` | 9332 | 95.99 | 934 | 218 | 276 | 52 | 84 | 192 | 56 | 56 |
| `MenuTouchWindow::OnTouchBegin` | 1896 | 98.09 | 72 | 0 | 72 | 0 | 0 | 0 | 0 | 0 |
| `ModStation` C1 | 1008 | 99.18 | 20 | 0 | 12 | 0 | 8 | 0 | 0 | 0 |
| `ModStation` C2 | 1008 | 99.18 | 20 | 0 | 12 | 0 | 8 | 0 | 0 | 0 |
| `MGame::OnUpdate` | 11440 | 99.94 | 22 | 6 | 8 | 0 | 0 | 0 | 4 | 4 |
| **total (19 symbols)** | | | **91006** | **13182** | **17256** | **31744** | **1716** | **21158** | **2990** | **2960** |

Aggregate shares: **block order 34.9 %**, **imm/operand 23.2 %**,
**register allocation 19.0 %**, **literal pool 14.5 %**, structural 3.3 %,
insert 3.3 %, scheduling 1.9 %.

Instruction-mix evidence (original/ours, from the same disassembly):

| function | tbh | blx/bl | ldr | str | vld1/vst1 | vcvt | ldrd/strd |
|---|---:|---:|---:|---:|---:|---:|---:|
| `Layout` | 0/0 | 0/0 | 114/105 | 185/247 | **167/147** | 37/42 | 65/51 |
| `LevelScript` | 15/15 | 3045/3057 | 5889/5778 | 1208/1207 | 10/10 | 202/199 | 545/537 |
| `createShip` | 1/1 | 89/86 | 208/205 | 149/154 | 0/0 | 11/9 | 34/31 |
| `Radar` | 0/0 | 392/402 | 893/875 | 251/246 | 0/0 | 70/70 | 51/50 |
| `OnTouchEnd` | 0/0 | 484/483 | 794/795 | 202/202 | 0/0 | 0/0 | 17/17 |
| `createMission` | 1/1 | 838/838 | 1600/1610 | 477/477 | 18/18 | 114/114 | 67/68 |
| `ModStation` | 0/0 | 25/25 | 76/76 | 78/78 | 10/10 | 6/6 | 13/13 |
| `OnUpdate` | 0/0 | 623/623 | 954/953 | 247/247 | 2/2 | 16/16 | 24/24 |

`Layout` is the only function with a real SLP deficit (20 extra `vld1/vst1`
groups in ours, 167 vs 147, plus 62 extra `str`).  Every other function has
the same SLP group count as the original; its `imm/op` bytes are stack-slot
and literal-pool offsets, not SLP.

## 4. Cross-cutting cause matrix

`●` = dominant cause (≥25 % of that function's taxonomy bytes), `○` =
measurable secondary cause, `-` = not a cause.  `pred` = predicate-probability
sensitivity measured in this or a previous wave (`L` = landed win, `D` =
destructive, `T` = tested/neutral).  `slot` = stack-slot colouring evidence
from the wave-119 notes.

| function | block | RA | lit-pool | imm/op | SLP | pred | slot | dominant evidence |
|---|:-:|:-:|:-:|:-:|:-:|:-:|:-:|---|
| `Layout` C1/C2 | ○ | ● | - | ● | ● | - | - | 42-vs-50 SLP groups; 167/147 `vld1` |
| `LevelScript::process` | ● | ○ | ○ | ○ | - | D | ○ | 48.6 % block; 15/15 `tbh`; expect collapses to 42-56 % |
| `createShip` | - | ● | ○ | ○ | - | T | ○ | Greedy-RA cascade; jitter XZY landed 70.2→70.3 |
| `Radar::draw` | ○ | ○ | ○ | ● | - | T | ● | 1.7 pts pure `[sp,#N]`; loop rotation/pool placement |
| `OnTouchEnd` | ● | - | ● | ● | - | **L** | - | `== true` landed 80.40→81.28; PLT-order trap |
| `Generator::createMission` | ○ | ○ | ○ | ● | - | D | ● | MBP cold-block; expect collapses; frame 112 vs 96 |
| `HangarWindow::render` | ○ | ● | ● | ○ | - | T | - | `&Globals::layout` r8↔sl; +4 B pool shift |
| `createMission` | ○ | ● | ○ | ○ | - | D | ● | RA 36 %; convoy k-chain; expect collapses to 62.8 % |
| `Status::missionCompleted` | - | ○ | ● | ○ | - | T | - | rank-table 192 B; `hasWon()==true` 92.07 (worse) |
| `createScene` | - | ● | - | - | - | T | - | rnd-local basin; 92.76 local optimum |
| `Gun::calcCharacterCollision` | - | ● | ○ | ● | - | T | ● | LICM hoist order; pp/delta slots |
| `PlayerEgo::calcCollision` | - | - | ● | ○ | - | **L** | - | expect(shr,1) 87.64→94.37; 2 redundant guards |
| `SpaceLounge::drawLounge` | - | ● | - | ○ | - | - | - | pad/x greedy tie-break; 576-variant sweep |
| `Hud::draw` | - | ● | ○ | ○ | - | T | - | r4/r5/fp rotation; Canvas in fp vs r5 |
| `OnTouchBegin` | - | ● | - | - | - | - | - | 18 diff lines, all register/rematerialisation |
| `ModStation` C1/C2 | - | ○ | - | - | - | - | - | 11 B post-RA schedule; pool order already exact |
| `OnUpdate` | - | ○ | ○ | - | - | T | ○ | `Status *st` anchor; 41 B one run |

## 5. Idiom experiments

All variants are ordinary C++ (no asm, attributes, globals or linker tricks).
`Δ` is the change in the target's unrounded ratio; `size` is ours.

### 5.1 `ModStation::ModStation` C1 (1008 B, baseline 99.175824 %)

The cheapest function; residual is 11 B / 4 runs, a post-RA scheduling
artifact (`str.w r8` / yaw load / `mov r4,r2`).  It has no loop, so loop
induction is not testable here.

| idiom | variant | size | % | Δ |
|---|---|---:|---:|---:|
| — | baseline | 1008 | **99.175824** | — |
| const | `const float *const pitchTable` | 1008 | 99.175824 | 0 |
| const | `const float *const yawTable` | 1008 | 99.175824 | 0 |
| const | `const int pitchBits/yawBits` | 1008 | 99.175824 | 0 |
| const | `const float yaw/pitch` | 1008 | 99.175824 | 0 |
| const | all four | 1008 | 99.175824 | 0 |
| explicit cmp | `Globals::iPad != 0 ?` (both sites) | 1008 | 99.175824 | 0 |
| explicit cmp | `101 == getIndex()`, `100 == getIndex()` | 1008 | 99.175824 | 0 |
| bool/int | `savedPostEffect = 0` | 1008 | 99.175824 | 0 |
| bool/int | `savedPostEffect = (bool)0` | 1008 | 99.175824 | 0 |
| type | `unsigned pitchBits/yawBits` | 1008 | 99.175824 | 0 |
| type | `(int)*(const volatile unsigned int*)` | 1008 | 99.175824 | 0 |
| type | `int race = 0;` | 1008 | 99.175824 | 0 |
| type | `unsigned race` | 1008 | 99.175824 | 0 |
| local | `int race3 = race * 3` | 1008 | 99.175824 | 0 |
| bool local | `bool isIPad = Globals::iPad != 0` | **968** | 79.382889 | −19.79 |
| int local | `int isIPad = Globals::iPad ? 1 : 0` | **968** | 79.382889 | −19.79 |
| early-exit/ternary | `race` if/else → nested ternary | **1000** | 82.369146 | −16.81 |

Every syntactic idiom that does not change the IR is byte-identical.  The
only changes that move the code are the ones that add a live local across the
`MatrixSetRotation` calls (`isIPad`) or fold the `race` branch chain into a
select (ternary), and both lose badly.  This confirms the wave-119 w6909
conclusion: the ctor is a hard local optimum.

### 5.2 `MGame::OnUpdate` (11440 B, baseline 99.935275 %)

Residual is 41 B / one run in the mission-23 idle-contact block; the
`Status *st` anchor is the known lever.  This function has loops, so it is
the cheapest place to test loop induction.

| idiom | variant | size | % | Δ |
|---|---|---:|---:|---:|
| — | baseline | 11440 | **99.935275** | — |
| loop | `for (int n = 3; i < n; ++i)` | 11440 | 99.935275 | 0 |
| loop | `for (int n = 3; i != n; i++)` | 11440 | 99.935275 | 0 |
| loop | `for (unsigned n = 3; i < (int)n; i++)` | 11440 | 99.935275 | 0 |
| loop | `for (unsigned i = 0; i < cargo->size(); ++i)` | 11440 | 99.935275 | 0 |
| loop | `for (int i = 0; i < 3; i++)` (drop `n`) | 11440 | 99.883495 | −0.052 |
| loop | `for (int i = 0; i < (int)cargo->size(); i++)` | 11440 | 99.883495 | −0.052 |
| loop | `for (unsigned i = 0; i != cargo->size(); i++)` | 11440 | 99.883495 | −0.052 |
| const | `Status *const st` | 11440 | 99.935275 | 0 |
| explicit cmp | `if (this->jumpDriveActive == true)` | 11440 | 99.883495 | −0.052 |
| explicit cmp | `if (cargo)` instead of `!= nullptr` | 11440 | 99.935275 | 0 |
| explicit cmp | `this->elapsedTime >= 5001` | 11440 | 99.935275 | 0 |
| explicit cmp | `if (this->dialogueWindow)` | 11440 | 99.935275 | 0 |
| explicit cmp | `pauseOpen == true`, `menuTouchOpen == true` | 11440 | 99.935275 | 0 |
| bool/int | `int hasOre = 0` instead of `bool` | 11440 | 99.262136 | −0.673 |

`bool`→`int` on a real local is the only *semantic* type idiom that changes
code, and it loses.  Loop spelling is canonicalised except when it changes
the induction type or the compare form, which also loses.

### 5.3 `LevelScript::process` (57232 B, baseline 59.489705 %)

The target function.  All variants below are size-57232 unless stated.

| idiom | variant | size | % | Δ |
|---|---|---:|---:|---:|
| — | baseline | 57232 | **59.489705** | — |
| predicate | `m_bStartSequence != 0` → `== true` | 57232 | 59.489705 | 0 |
| predicate | `m_bStartSequence != 0` → bare | 57232 | 59.489705 | 0 |
| predicate | `isOver()` → `isOver() == true` (61 sites) | 57232 | 59.489705 | 0 |
| predicate | `isTriggered() != 0` (32 sites) | 57232 | 59.489705 | 0 |
| predicate | `isWingMan() == true` | 57232 | 59.489705 | 0 |
| predicate | `m_bCinematicBreak = true/false` | 57232 | 59.489705 | 0 |
| predicate | `isTriggered() == true` (32 sites) | **57248** | 57.487442 | −2.00 |
| predicate | `m_bRenderParticles == true` | **57224** | 58.234270 | −1.26 |
| loop | `int i`→`unsigned i` in every `i < 3` loop | 57232 | 59.489705 | 0 |
| loop | postfix→prefix in every `i < 3` loop | 57232 | 59.489705 | 0 |
| loop | `unsigned int`→`unsigned` | 57232 | 59.489705 | 0 |
| loop | `i = 5; i != 9` → `i < 9` | 57232 | 59.489705 | 0 |
| loop | `i = 0; i != 2` → `i < 2` | 57232 | 59.489705 | 0 |
| loop | `i = 1; i <= 3` → `i < 4` | 57232 | 59.489705 | 0 |
| loop | `j = 1; j != 4` → `j < 4` | 57232 | 59.489705 | 0 |
| loop | `k = 2; k != 10` → `k < 10` | 57232 | 59.489705 | 0 |
| const | `Array<KIPlayer*> *const enemies` | 57232 | 59.489705 | 0 |
| const | `PlayerEgo *const player` | 57232 | 59.489705 | 0 |
| const | `Array<RadioMessage*> *const messages` | 57232 | 59.489705 | 0 |
| const | all three prologue locals const | 57232 | 59.489705 | 0 |
| explicit cmp | `m_nState == 0` → `!m_nState` | 57232 | 59.489705 | 0 |
| explicit cmp | `m_nState != 0` → bare | 57232 | 59.489705 | 0 |
| explicit cmp | `messages != nullptr` → bare | 57232 | 59.489705 | 0 |
| explicit cmp | `return m_bCinematicBreak != 0` → bare | 57232 | 59.489705 | 0 |
| explicit cmp | `enemies != nullptr` → bare | 57232 | 59.489705 | 0 |
| expect | `expect(m_bStartSequence != 0, 1)` | **57356** | 49.914130 | −9.58 |
| expect | `expect(m_bStartSequence != 0, 0)` | **57156** | 55.792880 | −3.70 |
| expect | `expect(introMsg->isOver() && m_nState == 0, 1)` | **57520** | 41.761851 | −17.73 |
| expect | `expect(introMsg->isOver() && m_nState == 0, 0)` | **57432** | 43.335904 | −16.15 |

The `isTriggered()` and `m_bRenderParticles` results are the only predicate
rewrites that change the IR: both predicates are `uint8_t`, so `== true` is a
real `== 1` compare and changes branch probability.  They move the body size
and lose.  `__builtin_expect` is a very strong block-placement lever here and
every tested site is destructive — the 59.49 % body is not a
branch-probability-metadata shape.

### 5.4 `PlayerEgo::calcCollision` (968 B, baseline 95.238095 %)

The early-return shape test; this is also the function where a predicate hint
previously landed 87.64→94.37.

| idiom | variant | size | % | Δ |
|---|---|---:|---:|---:|
| — | baseline | 968 | **95.238095** | — |
| early return | `if (!objects) return;` | 968 | 95.238095 | 0 |
| early return | merge the two prologue returns into one `||` | 968 | 95.238095 | 0 |
| explicit cmp | `if (!object) continue;` | 968 | 95.238095 | 0 |
| bool/int | `int playing` instead of `bool` | 968 | 95.238095 | 0 |
| predicate | `if (playing == true)` | 968 | 95.238095 | 0 |
| loop | `++i` | 968 | 95.238095 | 0 |
| loop | `int i` instead of `unsigned` | 968 | 94.949495 | −0.289 |
| loop | `i != objects->size()` | 968 | 94.949495 | −0.289 |

### 5.5 Spot checks on other functions

| function | variant | size | % | Δ |
|---|---|---:|---:|---:|
| `Status::missionCompleted` | baseline | 1432 | **92.454632** | — |
| `Status::missionCompleted` | `m->hasWon() == true` | 1432 | 92.072588 | −0.382 |
| `Status::missionCompleted` | `m->isCampaignMission() == true` | 1432 | 92.454632 | 0 |
| `Status::missionCompleted` | `if (!m)` | 1432 | 92.454632 | 0 |
| `Level::createShip` | baseline | 2764 | **70.278330** | — |
| `Level::createShip` | `int hard` instead of `bool` | 2764 | 70.278330 | 0 |
| `Level::createShip` | `raid/stationPirates/convoy/blueprint == true` | 2764 | 70.278330 | 0 |
| `Level::createShip` | `if (waypoint)` / `if (!Globals::status)` | 2764 | 70.278330 | 0 |

## 6. Findings

**Proven (measured on full `link_so` builds):**

1. **`const` locals are a non-idiom.**  12 measurements across
   `ModStation` (6), `LevelScript` (4), `OnUpdate` (1) and `PlayerEgo` (1):
   every `const`/`const *const` spelling is byte-identical.  clang 7.0.2
   discards top-level `const` before codegen.
2. **Loop-induction spelling is a non-idiom.**  20 measurements (7
   `OnUpdate`, 10 `LevelScript`, 3 `PlayerEgo`): prefix vs postfix,
   `unsigned` vs `unsigned int`, `i < 3` vs `i != 3`, `i <= 3` vs `i < 4`,
   `int` vs `unsigned` bound — 15 are byte-identical.  The 5 that change the
   induction *type* (`unsigned`→`int`) or the compare *form* (`<`→`!=` on an
   unsigned bound) all lose (`OnUpdate` −0.052, `PlayerEgo` −0.289).
3. **Explicit comparisons are a non-idiom.**  `p != nullptr` vs `p`,
   `m_nState == 0` vs `!m_nState`, `return x != 0` vs `return x`, `== true`
   on real `bool`s: 18 measurements, all byte-identical.
4. **Early-return shape is a non-idiom.**  Merging the two `PlayerEgo`
   prologue returns into one `||` test is byte-identical; `if (!objects)` is
   byte-identical.
5. **`bool`→`int` is neutral or worse.**  On a real local (`OnUpdate`
   `hasOre`) it changes code and loses 0.673 points; on a `bool` field or a
   `bool` local that is only tested it is neutral.
6. **Predicate probability is the only cross-cutting mechanism with landed
   wins.**  `MGame::OnTouchEnd` (`== true` on `uint8_t` predicates,
   80.3964→81.2827) and `PlayerEgo::calcCollision`
   (`__builtin_expect(...,1)`, 87.6437→94.3723) are the two proven cases.
   It requires a *non-bool* integer predicate (`unsigned char`/`uint8_t`) or
   an explicit `__builtin_expect`; on real `bool`s clang canonicalises
   `== true` away.  On `LevelScript` every tested site is destructive
   (41.8–58.2 %, body size 57156–57520).
7. **No idiom generalises to `LevelScript::process`.**  Every syntactic idiom
   is either byte-identical (Δ = 0) or loses.  The best measured variant is
   still the baseline **59.489705 % / 57232 B**.
8. **`Layout` is the only SLP-shaped residual.**  It has 167 vs 147
   `vld1/vst1` groups and 247 vs 185 `str`; every other function has the
   original's SLP group count.  `Layout`'s block+RA+imm/op bytes are the
   consequence of the missing SLP grouping, not of a source idiom.

**Hypothesis (not proven):**

* The remaining `LevelScript` gap is a single structural block-placement
  problem, not a source-idiom problem.  The two known structural levers are
  (a) making case 125's state machine out-of-line like the original
  (`else if` costs +24 B) and (b) the faithful prologue call order
  `messages, player, enemies` (costs +72 B).  Both need a size compensator
  before they can be landed.  A generic idiom will not supply one.
* The `imm/op` bucket (23.2 % of all taxonomy bytes) is mostly stack-slot
  colouring (`[sp,#N]`) and literal-pool offsets, not wrong constants.  The
  Radar note measured 1.7 points of pure stack-slot masking; the same
  mechanism is visible in `createShip`, `Gun`, `Generator` and `OnUpdate`.
  It is a Greedy-RA outcome, not a source spelling.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6921
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505

python3 /dev/shm/w6921/score.py                          # 59.489705 / 57232
python3 /dev/shm/w6921/tax.py                            # 19-row taxonomy
python3 /dev/shm/w6921/run_modstation.py                 # §5.1
python3 /dev/shm/w6921/run_onupdate.py                   # §5.2
python3 /dev/shm/w6921/run_levelscript_idioms.py         # §5.3 predicate
python3 /dev/shm/w6921/run_ls_loops.py                   # §5.3 loop
python3 /dev/shm/w6921/run_ls_const.py                   # §5.3 const
python3 /dev/shm/w6921/run_ls_cmp.py                     # §5.3 compare
python3 /dev/shm/w6921/run_ls_expect.py                  # §5.3 expect
python3 /dev/shm/w6921/run_pe.py                         # §5.4
python3 /dev/shm/w6921/run_status_idioms.py              # §5.5
python3 /dev/shm/w6921/run_cs_idioms.py                  # §5.5
```

Scratch helpers are `/dev/shm/w6921/{score,tax,harness,run_*}.py` (not
tracked).  The variant harness restores the source after each build and
touches it, so the final `check` rebuilds the baseline.

## 8. Gate

```
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint:   CLEAN
drift:  0 field(s)
```

27 sections, file size 2,187,328 B, `.text` 0x1337c8, `.ARM.exidx` 0x6f70,
`.ARM.extab` 0x26290 (original 0x26298, 4 B short), `.rodata` 0x41c27
(original 0x41b67), `.rel.plt` 0x72d0, `.rel.dyn` 0x7048, `.data` 0x296c,
`.bss` 0x69ac, `.got` 0x430c — all unchanged by this pass.
