# LevelScript::process — wave 176 / w12001: whole-condition expect re-sweep

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w12001`, branch
`wave176-w12001`, base master `b21088985` (wave-175 merge).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12001`.

Follow-up to `levelscript-process-expect-wave175-w11901.md` (the five hints
landed by w11901) and `levelscript-process-alias-additive-wave175-w11904.md`
(the additive block-scope aliases).  The wave-175 merge moved the base from
53,673 to **53,593**, so the whole-condition sweep was re-run from scratch:
cells that were neutral or size-changing on the wave-174 base can win now.

## Result (code win landed)

```
                         base (HEAD)      this wave
process raw diffbytes    53,593           53,564     (-29)
process aligned insns    18,476           18,332     (-144)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,727  20,830 / 20,727
.text diffbytes          80,166           80,137     (-29)
.ARM.extab diffbytes     746              745        (-1, size/base exact)
.rel.plt diffbytes       0                0          (byte-identical)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
stub_zero_size           0                0
missing / wrong_type     0 / 0            0 / 0
extra                    0                0
RATCHET                  PASS             PASS
```

`raw` is the same-address byte diff over the function body (literal pools
included), from `tools/fn_diff.py`.  Raw is the ground truth; the aligned
count is secondary.

## The change

Five `__builtin_expect` hints in `LevelScript::process`, four new wrappers and
one existing-expect flip:

1. `L1034` `if (__builtin_expect((m_nState == 3), 1))` — mission-1 state-3
   hot path (individual raw 53,588, -5).
2. `L1117` `if (__builtin_expect((m_nScriptTimerA >= 1100 - delta), 0))` —
   mission-0x4e state-6 escort drift (individual raw 53,591, -2).  The
   `// lint: arm_builtin` waiver was already present on this line.
3. `L2130` `if (__builtin_expect((player->isInWormhole()), 0))` — mission
   wormhole transition (individual raw 53,583, -10).
4. `L2507` `if (__builtin_expect((m_nState == 1), 0))` — state-1 conclude
   (individual raw 53,583, -10).
5. `L4902` existing `__builtin_expect(Globals::isRunningHDonWeakDevice, 1)`
   flipped to `0` — weak-device destroy-count branch (individual raw 53,591,
   -2).

All five are semantics-preserving (the hint only changes branch layout) and
carry the same-line `// lint: arm_builtin` waiver.  The individual deltas are
exactly additive: -5 -2 -10 -10 -2 = **-29**, and the combined build is
size-exact and `.rel.plt`-identical.

## Method

### Sweep A — whole-condition expect (643 conditions x 2 = 1,286 cells)

The wave-174 parser (`levelscript-process-expect-sweep-w11901.py`) was re-run
unchanged on the post-wave-175 source.  It finds all 643 `if`/`for` conditions
in `process` and the tail functions (the historical 644-condition set also
counts one `if` inside a `//` comment, which cannot affect codegen).  Each
condition is wrapped as `__builtin_expect((cond), e)` for `e = 0` and `e = 1`,
the real link is rebuilt (`ninja -C cmake-build-match libgof2hdaa.so -j2`),
and the candidate ELF is measured directly (`.dynsym` address/size, `.text`
same-address byte diff, `.rel.plt` md5).

| scope | cells | size-exact | neutral (53,593) | worse | better | size-changing | compile-fail |
|---|---:|---:|---:|---:|---:|---:|---:|
| `process` (620 conds) | 1,240 | 539 | 383 | 146 | **10** | 701 | 0 |
| tail functions (23 conds) | 46 | 34 | 31 | 0 | 3 | 0 | 12 |
| **total** | **1,286** | 573 | 414 | 146 | **13** | 713 | 12 |

The 12 compile failures are the pointer-only tail conditions
`if (m_pGeometryN)` in `render3D` (`__builtin_expect` needs an integral
argument); they are all after `process`.  The three tail "better" cells are
`L5348` (`level->getPlayer() != 0`, both variants, raw 53,592) and `L5407`
(`i != 8`, raw 53,592); they live in `resetCamera`/`skipCutscene` and were not
landed.

The ten `process` cells below the 53,593 baseline:

| line | kind | var | raw | condition |
|---:|---|---:|---:|---|
| 2130 | if | 0 | 53,583 | `player->isInWormhole()` |
| 2507 | if | 0 | 53,583 | `m_nState == 1` |
| 2337 | if | 1 | 53,585 | `(*messages)[4]->isOver()` |
| 1034 | if | 1 | 53,588 | `m_nState == 3` |
| 1342 | if | 0 | 53,588 | existing `expect(!isFading(), 1)` (wrapped again) |
| 4096 | for | 0 | 53,589 | `i < enemies->count` |
| 1444 | if | 0 | 53,590 | existing `expect(m_nState == 2 && timerA >= 2000, 1)` |
| 1117 | if | 0 | 53,591 | `m_nScriptTimerA >= 1100 - delta` |
| 2642 | if | 1 | 53,591 | `m_nScriptTimerA >= 3001` |
| 4902 | if | 0 | 53,591 | existing `expect(isRunningHDonWeakDevice, 1)` |

The full table is committed as
`levelscript-process-cells-wave176-w12001.tsv` (columns: line, kind, variant,
raw, size, text, relplt, cond).

### Sweep B — existing-expect flip / strip (64 sites x 2 = 128 cells)

All 64 `__builtin_expect` sites in `process` were flipped (0<->1) and stripped.
Result: 65 size-exact, 63 size-changing, 7 better, 0 compile failures.  The
seven better cells:

| line | variant | raw | condition |
|---:|---|---:|---|
| 1444 | strip | 53,586 | `m_nState == 2 && m_nScriptTimerA >= 2000` |
| 1342 | flip | 53,588 | `!Globals::layout->isFading()` |
| 1342 | strip | 53,588 | `!Globals::layout->isFading()` |
| 2840 | strip | 53,588 | `player->isDockedToDockingPoint()` |
| 1444 | flip | 53,590 | `m_nState == 2 && m_nScriptTimerA >= 2000` |
| 4902 | flip | 53,591 | `Globals::isRunningHDonWeakDevice` |
| 4909 | strip | 53,591 | `i < 22` |

The table is committed as
`levelscript-process-expects-wave176-w12001.tsv` (columns: line, variant, raw,
size, text, relplt, arg0).

### Combination search

The ten Sweep-A process wins and the seven Sweep-B wins were combined with the
same harness (edits applied right-to-left, one real link per candidate):

* Exhaustive over the ten Sweep-A wins (1,023 non-empty subsets): 383
  size-exact.  Best **53,564** with
  `{2130:0, 2507:0, 1034:1, 1117:0, 4902:0}` (three equivalent variants:
  `1034:1` and `1342:0` are interchangeable, and adding both is neutral).
  The full ten-cell set is size +4 (57,236) / raw 53,857.
* Adding each of 183 nearby size-exact cells (raw <= 53,620, within 150 lines
  of a winner) to the landed set: **no improvement**; every candidate stays at
  53,564 or worse.
* Hill-climb over the 12 best sites (add/remove/swap, size-exact + `.rel.plt`
  identical): local optimum at **53,564**.
* Adding the Sweep-B wins to the landed set is size-changing or neutral:
  `1444 strip` is -4 B (57,228, raw 54,020), `2840 strip` is -8 B (57,224,
  raw 54,024), `4909 strip` is neutral (53,564), `1342 flip` is neutral
  (53,564).  Compensating `1444/2840` with `2337:1`/`4096:0` (+8 B) gives
  53,797-53,820, worse than 53,564.

## Rejected / not landed

* `2337:1` alone is -8 but adding it to the landed set is size +8 (57,240,
  raw 54,733).
* `4096:0` alone is -4 but adding it is size +8 (57,240, raw 54,737).
* `2642:1` is -2 but adding it is size -8 (57,224, raw 54,022).
* `1444 strip` is -7 alone but size -4 in the landed set (57,228, raw 54,020).
* `2840 strip` (the w11901-era escort hint at `L2840`) is -5 alone but size
  -8 in the landed set (57,224, raw 54,024).
* The three tail cells (`L5348`, `L5407`) are -1 each but change tail
  functions, not `process`; they were not landed.

## Gate

`check` is green: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, symbol parity **5370/5370** (0 extra / 0 missing),
stub_zero_size **0**, missing **0**, wrong_type **0**, extra **0**,
`RATCHET PASS`.  `process` is raw **53,564** / aligned **18,332** / size
**57,232**, `.text` **80,137**, `.ARM.extab` **745** (size- and base-exact),
`.rel.plt` byte-identical (md5 `289b2a069f8504d1e8a3919e43915e04`).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12001
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53564 aligned= 18332 size= 57232/57232 insn= 20727/20830
cmake --build cmake-build-match --target check -- -j2    # RATCHET PASS
cmake --build cmake-build-match --target drift -- -j2    # 0 fields
cmake --build cmake-build-match --target lint -- -j2     # CLEAN
```

The sweep drivers are the wave-174 scripts
`levelscript-process-expect-sweep-w11901.py`,
`levelscript-process-expects-w11901.py`, `measure_w11901.py` (run unchanged);
the combination harness lives in
`/home/debian/GalaxyOnFire2/scratch/w12001/apply_edits.py` (scratch, not
tracked).
