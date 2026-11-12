# LevelScript::process — wave 187 / w13108: `__builtin_expect` add/flip/remove re-sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13108`, branch
`wave187-w13108`, base master `8b5d4281c` (wave-186 harvest 6, process raw
**51,968**, aligned **20,856**, `.text` diff **74,274**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13108`.

This pass re-runs the whole `__builtin_expect` sweep on the current wave-186
body (the body changed by 107 raw bytes since w13008's 52,075 re-sweep, and by
164 since w12908's 52,132 re-sweep), covering whole-condition adds,
top-level `&&`/`||` leaf adds, recursively-nested leaf adds, flip/remove of
every existing hint (including the nested hint at line 4338), ternary-condition
adds, switch-subject adds, operands inside existing `__builtin_expect(...)`
arguments, and cancelling-size pair searches.  A candidate is acceptable only
if it is **size-exact (57,232 B) and `.rel.plt`-identical** (md5
`289b2a069f8504d1e8a3919e43915e04`).

## Result: no acceptable win; `src/` left at base

```
                         base (HEAD)      this pass
process raw diffbytes    51,968           51,968     (unchanged)
process aligned insns    20,856           20,856
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,727  20,830 / 20,727
.text diffbytes          74,274           74,274
.ARM.extab diffbytes     647              647        (size/base exact, 156,312 B @ 0x1a3b78)
.ARM.exidx               0                0
.note.gnu.build-id       20               20
total differing bytes    74,941           74,941
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
file size                2,187,328        2,187,328      (exact)
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
included) from `tools/fn_diff.py`; it is the primary metric.  `check` was run
on the clean base tree and reported `RATCHET PASS`.  `lint` reported
`CLEAN ✓` and `drift` reported `0 field(s) across 0 class(es)`.

## Sweeps run (all on the current 51,968 body, process lines 605–5371)

| sweep | cells | size-exact | size-exact + rel.plt-identical | best raw |
|---|---:|---:|---:|---:|
| whole-condition add (`if`/`for`, 620 conditions x {0,1}) | 1,240 | 542 | 535 | 51,968 |
| top-level `&&`/`||` leaf add | 446 | 187 | 177 | 51,968 |
| recursive (parenthesised) leaf add | 8 | 2 | 2 | 51,968 |
| flip of every existing hint (incl. nested 4338) | 65 | 25 | 25 | 51,968 |
| remove of every existing hint (incl. nested 4338) | 66 | 35 | 35 | 51,968 |
| ternary-condition add | 42 | 39 | 39 | 51,968 |
| switch-subject add | 20 | 4 | 4 | 51,968 |
| operand add *inside* an existing `__builtin_expect(...)` | 30 | 30 | 30 | 51,968 |
| **all single cells** | **1,917** | **847** | **847** | **51,968** |

**Every size-exact, `.rel.plt`-identical single edit is byte-identical to the
base function** (raw stays 51,968).  No single add/flip/remove is below
51,968.  The closest non-neutral single edits are +1 B (e.g. whole add at
lines 1229, 2194, 2951, 3752, 3873, 4077, 4284, 5145, 5155; leaf add at 4078;
remove at 3467/3752) and are therefore not landed.

The four wave-183 harvest-4 cells were re-evaluated on this body:

* line 1448 `__builtin_expect((m_nState == 2 && m_nScriptTimerA >= 2000), 0)`
  — flip 52,065 / 57,228; remove 51,970 / 57,232.
* line 3153 `(m_nScriptTimerA >= 8001)` (hint removed in wave 183) — re-adding
  hint 1 is byte-identical (51,968); hint 0 is 54,599 / 57,224.
* line 4735 `__builtin_expect((ship->shipGroup == 9), 1)` — flip 52,697 /
  57,208; remove 52,167 / 57,232.
* line 5184 `__builtin_expect((m_nScriptTimerA >= 5500 && ...), 0)` — flip
  52,107 / 57,248; leaf add 1 is byte-identical.

## Pairs searched

| search | pairs | size-exact + rel.plt-identical | best raw |
|---|---:|---:|---:|
| whole-add cancelling pairs (top-100 estimate + near-region top-100) | 174 | 114 | 51,977 |
| mixed add/flip/remove/leaf cancelling pairs (near-region, top-120 estimate) | 120 | 84 | 52,005 |

Best `.rel.plt`-identical pair: `remove(5190)` + `whole add0(5145)` = raw
**52,005** / size 57,232 (worse than base).  Best whole-add pair:
`add0(2756)` + `add1(2802)` = 51,977.

### The near-miss: a raw win that changes `.rel.plt`

The only configuration found with `raw < 51,968` and function size exactly
57,232 B is a two-edit combination (the same shape w12908/w13008 found on the
older bodies):

1. **Strip** the existing hint on the 5700-frame station-departure transition
   (line 5190):
   `if (__builtin_expect((m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700), 0))`
   -> `if ((m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700))`.
2. **Add** hint 0 to the supernova-reversal guard (line 5181):
   `if (m_nScriptTimerA >= 5000 && m_nScriptTimerA - delta < 5000)`
   -> `if (__builtin_expect((m_nScriptTimerA >= 5000 && m_nScriptTimerA - delta < 5000), 0))`.

```
raw 51,968 -> 51,963   (-5), size 57,232/57,232 exact
.text diff 74,274 -> 74,271
.rel.plt md5 289b2a06… -> 07752cde942efd50fdec71192614b8e0   (NOT identical)
```

The `.rel.plt` change is a two-entry swap of adjacent PLT slots
(`.rel.plt` indices 2842/2843, GOT `0x213300`/`0x213304`):

```
original: 0x213300 -> Level::switchSkyboxForSupernovaReversal   (sym 3379)
          0x213304 -> Player::setGammaHP(int)                   (sym 2731)
combo   : 0x213300 -> Player::setGammaHP(int)
          0x213304 -> Level::switchSkyboxForSupernovaReversal
```

Both symbols are first referenced inside `process`: `switchSkyboxForSupernovaReversal`
at source line 5182 and `setGammaHP` at line 5216.  In the original the
switchSkybox call site is at `0x1459a4` and the setGammaHP call site at
`0x145bcc` (source order), so gold assigns switchSkybox the lower PLT slot.
The hint at 5181 makes the switchSkybox block cold/out-of-line; the two call
sites become `0x1456ee` (setGammaHP) and `0x1457fe` (switchSkybox), so gold
assigns setGammaHP the lower slot and the two `.rel.plt` entries swap.

Attempts to restore the PLT order on the current body all failed (each keeps
the swapped `.rel.plt` or worsens the raw):

* add hint 0/1 on the enclosing `if (m_nScriptTimerA >= 6500)` (line 5207):
  hint 0 is neutral (51,968, same swap); hint 1 gives raw 53,393 / size 57,228;
* add hint 0 on `if (m_nScriptTimerA > 5000 && m_nScriptTimerA < 6000)` (line
  5199): raw 52,038, same swap;
* add hint 0 on `if (m_nScriptTimerA >= 5500)` (line 5196) or on the
  `m_nScriptTimerA < 6000` guard (line 5176): neutral, same swap;
* remove/flip the 5176 hint: neutral, same swap;
* remove(5190)+add0(5181)+add1(5207)+{add0(5173|5084|5116), add1(5332),
  add0(5347)}: raw 53,396–53,427, same swap.

Because `.rel.plt` must stay byte-identical this near-miss is **not landed**.
(The task's total diff budget also treats `.rel.plt` as byte-identical; the
swap would add 8 differing bytes while saving only 5 in `.text`.)

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13108
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51968 aligned= 20856 size= 57232/57232 insn= 20727/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN ✓
cmake --build cmake-build-match --target drift -- -j2   # 0 drifted fields
```

The whole-condition driver is the tracked
`_work/reconstructions/levelscript-process-expect-sweep-w11901.py` (with
`measure_w11901.py`).  Scratch drivers used here (not tracked, in
`/home/debian/GalaxyOnFire2/scratch/w13108/`): `sweep.py` (nested-aware
`__builtin_expect` finder, condition/leaf/ternary/switch/inner-leaf/flip/remove
modes), `combo.py` (multi-edit applier), `pairsearch.py`, `pairsearch3.py`.
Result tables: `cells.tsv`, `results.tsv`, `pair_results.tsv`,
`pair3_results.tsv`.
