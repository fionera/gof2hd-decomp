# LevelScript::process — wave 188 / w13208: `__builtin_expect` re-sweep on the wave-187 body

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13208`, branch
`wave188-w13208`, base master `ce16f45f1` (wave-187 harvest 5, raw **51,930**,
`.text` **74,223**, `.ARM.extab` diff 639, total 74,882).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13208`.

This pass re-runs the whole `__builtin_expect` add/flip/remove sweep on the
current wave-187 body.  The body changed by 145 diff-bytes since the last
re-sweep (`levelscript-process-expect-wave186-w13008.md`, raw 52,075), so every
cell was re-measured from scratch.  A candidate is acceptable only if it is
**size-exact (57,232 B) and `.rel.plt`-identical** (md5
`289b2a069f8504d1e8a3919e43915e04`).

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,930           51,927     (-3)
process aligned insns    20,928           20,928     (0)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,223           74,220     (-3)
.ARM.extab diffbytes     639              639        (size/base exact, 156,312 B @ 0x1a3b78)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    74,882           74,879     (-3)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
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
included) from `tools/fn_diff.py` / `measure_w11901.py`; it is the primary
metric.

## The change

One sub-expression `__builtin_expect` add in `LevelScript::process`, in the
mission-specific `messages[7]` trigger block (base line 2376):

```cpp
// before
if (((RadioMessage *) ((*messages)[7]))->isTriggered() && m_nState == 4) {
// after
if (__builtin_expect(((RadioMessage *) ((*messages)[7]))->isTriggered(), 1) && m_nState == 4) { // lint: arm_builtin
```

This is the first operand of the existing top-level `&&` (an `andor` cell of
the sub-expression sweep).  It is semantics-preserving (branch-weight hint
only), size-exact, and `.rel.plt`-identical: raw 51,930 -> **51,927** (-3),
`.text` 74,223 -> **74,220**.

## Sweeps run (all on the current 51,930 body)

| sweep | cells | size-exact cells | size-exact better | best raw |
|---|---:|---:|---:|---:|
| whole-condition add (643 conditions x {0,1}) | 1,286 | 287 | 2 (tail only) | 51,929* |
| sub-expression add (`&&`/`||` operands, inner-expect, `!(...)`) | 842 | 416 | **1** | **51,927** |
| flip/remove of every existing hint (68 x {flip,strip}) | 136 | 62 | 0 | 51,930 |
| ternary-condition add (19 `?:`) | 19 | 8 | 0 | 51,930 |

\* the three whole-condition size-exact better rows (two distinct cells) are in tail functions,
not `process`: `resetCamera` line 5375 `level->getPlayer() != 0` (both
variants) and `skipCutscene` line 5434 `i != 8`, each raw 51,929 (-1).  They
were not landed (same policy as wave 176 / w12001: the target is `process`).

Every size-exact whole-condition add is byte-identical to the base function
(raw stays 51,930); all size-exact flip/remove and ternary cells are also
neutral.  The single sub-expression winner is the `messages[7]` `andor` cell
above.

### Size-changing cells and pair search

The whole sweep's best size-changing cells are all worse than the base:
`Level::doInstantJump == 0` hint 0 (raw 52,002, size +8), `m_nState == 0 &&
messages[2]->isTriggered()` hint 0 (52,003, +8), `messages[14]->isOver()` hint
0 (52,004, -4), `m_nState == 11` hint 0 (52,014, -4), `m_nState == 9/10` hint
0/1 (52,017, -4), `m_nScriptTimerA >= 5500` hint 1 (52,005, -8).

A cancelling-size pair search was run over the size-changing cells, pairing
top cells of each `-d` group with the matching `+d` group:

* region-focused (lines 4800-5373, 686 pairs): no size-exact,
  `.rel.plt`-identical pair below the base.  The near-miss is the same one
  w13008 found: strip the 5700-frame station-departure hint (line 5191) +
  add hint 0 to the supernova-reversal guard (line 5182) gives raw **51,925**
  at 57,232 B, but swaps `.rel.plt` entries 2842/2843
  (`switchSkyboxForSupernovaReversal` / `setGammaHP`), md5
  `07752cde942efd50fdec71192614b8e0`.  Not landable.
* all-region top-8-per-delta (473 pairs): best `.rel.plt`-identical pair is
  strip the existing `messages[14]->isOver()` hint (line 5096, -8) + add hint
  0 to `m_nScriptTimerA >= 6001` (line 5146, +8) at raw **51,970**; next is
  `m_nScriptTimerA >= 12001 && messages[12]->isOver()` hint 1 (line 5032,
  -12) + `messages[10]->isTriggered()` hint 0 (line 3508, +12) at raw
  **51,981**.  Both are worse than the base.

Triple combinations around the near-miss (adding hints to the 5500/6500
guards, both hint polarities, and several `+4` compensators) were also
measured; none both beats 51,927 and keeps `.rel.plt` byte-identical.

## Cell tables

Committed alongside this note:

* `levelscript-process-whole-wave188-w13208.tsv` — 1,286 whole-condition
  cells (columns: line, kind, variant, raw, size, text, relplt, cond).
* `levelscript-process-subs-wave188-w13208.tsv` — 1,684 sub-expression cells
  (`set` = `andor` / `inner-expect` / `bang-inner`).
* `levelscript-process-flips-wave188-w13208.tsv` — 272 flip/strip rows for the
  68 existing `__builtin_expect` sites (each cell appears for variant 0 and
  1; the replacement is variant-independent).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13208
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51927 size=57232/57232 text=74220 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 drifted fields
```

The sweep drivers are the tracked
`_work/reconstructions/levelscript-process-expect-sweep-w11901.py`,
`levelscript-process-subs-sweep-w12004.py` and `measure_w11901.py`.  The
pair-search and ternary harnesses used here are scratch
(`/home/debian/GalaxyOnFire2/scratch/w13208/`), not tracked.
