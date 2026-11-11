# LevelScript::process — wave 177 / w12104: sub-expression expects on the current base

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w12104`, branch
`wave177-w12104`, base master `9445ac5fb` (wave-176 merge).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12104/`.

Follow-up to the wave-176 notes `levelscript-process-expect-wave176-w12001.md`
(whole-condition sweep), `levelscript-process-expect-subs-wave176-w12004.md`
(sub-expression sweep) and `levelscript-process-branchlayout-wave176-w12003.md`
(branch layout).  The wave-176 merge left `process` at raw **53,568** /
aligned **18,498**.  This pass re-runs the sub-expression sweep on that base
(cells that were neutral or size-changing on the wave-175 base can win now)
and adds a targeted whole-condition sweep over the rare `m_nState` /
`getCurrentCampaignMission` guards.

## Result (code win landed)

```
                         base (9445ac5fb)   this pass
process raw diffbytes    53,568             53,566     (-2)
process aligned insns    18,498             18,498     (0)
orig / ours size         57232 / 57232      57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726    20,830 / 20,728
.text diffbytes          79,591             79,589     (-2)
.ARM.extab diffbytes     1,321              1,321      (size/base exact)
.rel.plt                 md5 289b2a06…      md5 289b2a06…   (byte-identical)
byte_exact               4507               4507
linked_exact             4507               4507
size-exact               4524/4524          4524/4524
data_exact               843/843            843/843
imports                  207                207
lint                     CLEAN ✓            CLEAN ✓
drift                    0                  0
symbol parity            5370/5370          5370/5370
stub_zero_size           0                  0
missing / wrong_type     0 / 0              0 / 0
extra                    0                  0
RATCHET                  PASS               PASS
```

`raw` is the same-address byte diff over the function body (literal pools
included), from `tools/fn_diff.py`; it is the primary metric.  The +2
instruction count is a block-placement side effect and does not change the
aligned count.

## The change

Two `__builtin_expect` hints, both semantics-preserving, both with the
same-line `// lint: arm_builtin` waiver:

1. **Line 1688 — mission-42 `case 16`, state-1 transition (mission-specific
   rare guard).**  The radio-message completion test is cold: the original
   lays out the fall-through as the not-over path.

   ```cpp
   // before
   if (((RadioMessage *) ((*messages)[1]))->isOver()) {
   // after
   if (__builtin_expect(((RadioMessage *) ((*messages)[1]))->isOver(), 0)) { // lint: arm_builtin
   ```

   Individual raw 53,567 (-1), size-exact, `.rel.plt` identical.

2. **Line 5049 — mission-157 state-5 explosion flash countdown (mission-specific
   rare guard).**  The countdown body is the hot path.

   ```cpp
   // before
   if (m_nFlashCountdown > 0) {
   // after
   if (__builtin_expect(m_nFlashCountdown > 0, 1)) { // lint: arm_builtin
   ```

   Individual raw 53,567 (-1), size-exact, `.rel.plt` identical.

The two hints are additive: the combined build is raw **53,566** (-2),
size-exact, `.rel.plt` byte-identical.

The adjacent **line 5051** `if (m_nFlashCountdown <= 0)` with `expect(..., 1)`
is also -1 alone (raw 53,567) but **neutral** when added to the pair (the two
hints share the same block layout); it is documented but not landed.

## Sweeps

All cells were built with the real gold link (`ninja -C cmake-build-match
libgof2hdaa.so -j2`) and measured from the candidate ELF: raw same-address
diff, function size, whole-`.text` diff and `.rel.plt` md5.

| sweep | cells | variants | size-exact | neutral | worse | better | size-changing | compile-fail |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| `andor` (&&/|| operands, all conditions) | 747 | 1,494 | 654 | 476 | 175 | **3** | 841 | 0 |
| `inner` (expr inside existing expect) | 65 | 130 | — | all | 0 | 0 | — | 0 |
| `bang` (inner of `!(...)`) | 22 | 44 | — | all | 0 | 0 | — | 0 |
| `flip` (existing expect 0<->1) | 65 | 65 | — | some | rest | 0 | — | 0 |
| `strip` (remove existing expect) | 65 | 65 | — | some | rest | 0 | — | 0 |
| `whole` rare guards (`m_nState` / mission) | 199 | 398 | — | best 53,568 | rest | 0 | — | 0 |

The `andor` sweep is the wave-176 `levelscript-process-subs-sweep-w12004.py`
cell generator run unchanged on the new base.  The three size-exact winners:

| raw | site | change | condition |
|---:|---|---|---|
| 53,567 | line 1688 | wrap, expect 0 | `((RadioMessage *) ((*messages)[1]))->isOver()` |
| 53,567 | line 5049 | wrap, expect 1 | `m_nFlashCountdown > 0` |
| 53,567 | line 5051 | wrap, expect 1 | `m_nFlashCountdown <= 0` |

The full `andor` table is committed as
`levelscript-process-cells-wave177-w12104.tsv` (columns: kind, line, idx,
variant, raw, size, text, relplt, cond).  The targeted rare-guard whole table
is `levelscript-process-rare-whole-wave177-w12104.tsv`.

### Rejected: line 5156 (size-changing, `.rel.plt` flip)

`if (m_nScriptTimerA >= 5000 && m_nScriptTimerA - delta < 5000)` with
`expect(..., 0)` on the first operand is the best raw cell in the whole sweep
— raw **53,558** (-10) — but the function grows to **57,236 B** (+4) and
`.rel.plt` changes (md5 `07752cde…`).  The change is a swap of entries
2842/2843: `Level::switchSkyboxForSupernovaReversal` and
`Player::setGammaHP` trade PLT order, because the hinted `>= 5000` block is
moved to the cold path past the `>= 6500` block that calls `setGammaHP`.
Compensating the +4 with a -4 cell (line 5093 `m_nScriptTimerB > 2000`
expect 1, or line 5201 `m_nState == 1` expect 0) restores the size but not
the `.rel.plt` order and is worse raw (53,649 / 54,672), so it was not
landed.

## Gate

`check` is green: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, symbol parity **5370/5370** (0 extra / 0 missing),
stub_zero_size **0**, missing **0**, wrong_type **0**, extra **0**,
`RATCHET PASS`.  `process` is raw **53,566** / aligned **18,498** / size
**57,232**; `.text` **79,589**; `.ARM.extab` **1,321** (size- and base-exact);
`.rel.plt` byte-identical (md5 `289b2a069f8504d1e8a3919e43915e04`).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12104
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53566 aligned= 18498 size= 57232/57232 insn= 20728/20830
cmake --build cmake-build-match --target check -- -j2    # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2    # CLEAN
cmake --build cmake-build-match --target drift -- -j2    # 0 fields
```

The sweep driver is `/home/debian/GalaxyOnFire2/scratch/w12104/sweep.py`
(scratch, not tracked); it is the wave-176 `subs` cell generator plus the
`whole`/`inner`/`bang`/`flip`/`strip` modes, and rebuilds the real gold link
for every cell.
