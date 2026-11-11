# LevelScript::process — wave 176 / w12004: sub-expression expects + rare-guard flips

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`,
original `0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`wt/w12004`, branch `wave176-w12004`, base master `b21088985` (wave 175).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12004/`.

Second independent `__builtin_expect` pass after
`levelscript-process-expect-wave175-w11901.md`.  w11901 swept the *whole*
condition of every `if`; this pass covers the cells it did not:

* operands of every top-level `&&` / `||` (including nested parenthesised
  operands), 544 cells;
* operands inside an existing `__builtin_expect(EXPR, e)` call, 48 cells;
* the inner expression of every `!(EXPR)`, 44 cells;
* flip / strip of every existing `__builtin_expect` site, 128 cells.

The focus was the rare guards (states >= 1, mission-specific intro blocks,
deactivation/shared tail); the winning cells are all in mission-specific or
state-specific arms.

## Result (code win landed)

```
                         base (b21088985)   this pass
process raw diffbytes    53,593             53,580     (-13)
process aligned insns    18,476             18,483     (+7)
orig / ours size         57232 / 57232      57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,727    20,830 / 20,728
.text diffbytes          80,166             80,153     (-13)
.rel.plt                 md5 289b2a06…       md5 289b2a06…   (byte-identical)
byte_exact               4507               4507
linked_exact             4507               4507
size-exact               4524/4524          4524/4524
data_exact               843/843            843/843
imports                  207                207
lint                     CLEAN ✓            CLEAN ✓
drift                    0                  0
symbol parity            5370/5370          5370/5370
RATCHET                  PASS               PASS
```

`raw` is the same-address byte diff over the function body (literal pools
included), from `tools/fn_diff.py`; it is the primary metric.  The +7 aligned
is the usual block-placement side effect and is secondary.

## The three edits

All three are semantics-preserving; `__builtin_expect` only changes branch
weights.  Line numbers are the base `src/game/world/LevelScript.cpp` lines.

1. **Line 1342 — `case 5` of the mission-intro state switch (rare state
   guard).**  Flip the existing hint from `1` to `0`:

   ```cpp
   // before
   if (__builtin_expect(!Globals::layout->isFading(), 1)) { // lint: arm_builtin
   // after
   if (__builtin_expect(!Globals::layout->isFading(), 0)) { // lint: arm_builtin
   ```

   This is the state-5 arm (`m_nState == 5`) of the per-mission intro state
   machine; the original lays the fall-through out as the not-fading path
   being the cold one.

2. **Line 1444 — mission-95/99/109/119/126/133/160/161 "Chain A" short
   station-departure intro (mission-specific shared block).**  Strip the
   existing hint entirely:

   ```cpp
   // before
   if (__builtin_expect(m_nState == 2 && m_nScriptTimerA >= 2000, 1)) { // lint: arm_builtin
   // after
   if (m_nState == 2 && m_nScriptTimerA >= 2000) {
   ```

   The original emits the plain compare/branch pair here; the hint made clang
   place the two-condition test as a hot fall-through, which is the wrong
   shape for this once-per-mission block.

3. **Line 4320 — mission-specific `shipGroupFlag == 44` cloaking block (rare
   guard), inner `&&` sub-expression.**  Add a hint on the *first* operand of
   the existing `&&` (the whole condition already carries `expect(..., 0)`):

   ```cpp
   // before
   if (__builtin_expect(!((PlayerFighter *) (*enemies)[i])->field_0x13c &&
                        Globals::rnd->nextInt(100) <= 9, 0)) { // lint: arm_builtin
   // after
   if (__builtin_expect(__builtin_expect(!((PlayerFighter *) (*enemies)[i])->field_0x13c, 0) &&
                        Globals::rnd->nextInt(100) <= 9, 0)) { // lint: arm_builtin
   ```

   The inner `expect(..., 0)` is the only single sub-expression cell that is a
   size-exact raw win on this base (raw 53,592 alone; it contributes the last
   -1 in the final triple).  The one-line waiver covers both builtins (lint
   waivers are per line).

## Cell tables

Committed alongside this note:

* `levelscript-process-subs-wave176-w12004.tsv` — 544 sub-expression cells
  (`set` = `andor` / `inner-expect` / `bang-inner`).  247 size-exact
  (190 neutral at 53,593, 56 worse, **1 better**); 273 size-changing;
  24 compile-fail (pointer-only operands such as `*enemies`).
* `levelscript-process-expect-flips-wave176-w12004.tsv` — 128 flip/strip cells
  for the 64 existing `__builtin_expect` sites.  65 size-exact
  (9 neutral, 49 worse, **7 better**); 63 size-changing.

Best size-exact single cells:

| raw | site | change | condition |
|---:|---|---|---|
| 53,586 | line 1444 | strip | `m_nState == 2 && m_nScriptTimerA >= 2000` |
| 53,588 | line 1342 | flip 1→0 | `!Globals::layout->isFading()` |
| 53,588 | line 1342 | strip | `!Globals::layout->isFading()` |
| 53,588 | line 2840 | strip | `player->isDockedToDockingPoint()` |
| 53,590 | line 1444 | flip 1→0 | `m_nState == 2 && m_nScriptTimerA >= 2000` |
| 53,591 | line 4902 | flip 1→0 | `Globals::isRunningHDonWeakDevice` |
| 53,591 | line 4909 | strip | `i < 22` |
| 53,592 | line 4320 | wrap first `&&` operand, expect 0 | `!((PlayerFighter *) (*enemies)[i])->field_0x13c` |

The 24 compile failures are `__builtin_expect(pointer, e)` on bare pointer
operands (`*enemies`, `(RadioMessage *) ((*messages)[2])`, …); they cannot
affect the process body.

## Combination search

The landed shape is the greedy hill-climb optimum over the 301 size-exact
cells (all three sub-expression sets plus the flip/strip set):

| step | shape | raw | size |
|---:|---|---:|---:|
| 0 | base | 53,593 | 57,232 |
| 1 | line 1444 strip | 53,586 | 57,232 |
| 2 | + line 1342 flip | 53,581 | 57,232 |
| 3 | + line 4320 inner wrap | **53,580** | **57,232** |
| 4 | + any other size-exact cell | no improvement | 57,232 |

A full pairwise search over the 17 best size-exact cells confirms the best
pair is (line 1444 strip, line 1342 flip) at 53,581; no pair beats it, and no
third cell added to it beats 53,580.

Rejected near-misses:

* line 3136 `m_nState == 8` strip is **53,582 at 57,228 B** (size-changing).
  Adding a +4 B compensating cell was measured against every size-57236 cell;
  the best compensation is 53,758, so it does not beat the triple.
* The best size-changing sub-expression cell is line 5093
  `m_nScriptTimerB > 2000` expect 1 at 53,650 / 57,228 B; no +4 B compensation
  tested better than the triple.
* w11901's whole-condition cells were not re-tested; this pass deliberately
  uses only sub-expression operands and existing-expect flips/strips.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53580 aligned= 18483 size= 57232/57232 insn= 20728/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=53580 size=57232/57232 text=80153 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -j2   # 0 fields
```

The sweep harness (generic, repo-relative) is committed as
`levelscript-process-subs-sweep-w12004.py`; it rebuilds the real gold link for
each cell and records `raw/size/.text/.rel.plt` exactly as above.
