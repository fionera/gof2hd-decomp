# LevelScript::process — wave 185 / w12908: expect add/flip/remove re-sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12908`, branch
`wave185-w12908`, base master `f390b0e68` (wave-184 harvest 10, raw **52,132**).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12908`.

This pass re-runs the whole `__builtin_expect` sweep on the current 52,132 body:
whole-condition adds, top-level `&&`/`||` leaf adds, recursive (any-depth)
`&&`/`||` leaf adds, flip/remove of every existing hint, ternary-condition adds,
and cancelling-size pair/triple searches.  A candidate is only acceptable if it
is **size-exact (57,232 B) and `.rel.plt`-identical** (md5
`289b2a069f8504d1e8a3919e43915e04`).

## Result: no acceptable win; `src/` left at base

```
                         base (HEAD)      this pass
process raw diffbytes    52,132           52,132     (unchanged)
process aligned insns    20,658           20,658
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
ours insns               20,725            20,725
.text diffbytes          74,591           74,591
.ARM.extab diffbytes     647              647        (size/base exact, 156,312 B @ 0x1a3b78)
.note.gnu.build-id       20               20
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
included) from `tools/fn_diff.py`; it is the primary metric.  `check` was run
on the clean base tree and reported `RATCHET PASS`.

## Sweeps run (all on the current body)

| sweep | cells | size-exact | size-exact + rel.plt-identical | best raw |
|---|---:|---:|---:|---:|
| whole-condition add (`if`/`for`, 617 in process + 26 tail) | 1,274 | 584 | 576 | 52,132 |
| top-level `&&`/`||` operand add | 462 | 219 | 209 | 52,132 |
| recursive (any-depth) `&&`/`||` operand add | 470 | 219 | 209 | 52,132 |
| flip/remove of every existing hint | 134 | 59 | 59 | 52,132 |
| ternary-condition add (`?:` conditions) | 38 | 37 | 37 | 52,132 |
| add+add cancelling-size pairs | 64 | 45 | 39 | 52,144 |
| strip/flip + add cancelling-size pairs | 136 | 85 | 85 | 52,169 |
| strip/flip + strip/flip size-exact pairs | 321 | 317 | 317 | 52,134 |
| strip/flip + leaf-add cancelling-size pairs | 83 | 72 | 72 | 52,194 |
| two-strip + add triples | 30 | 24 | 18 | 52,196 |

**No single cell and no size-exact pair/triple improves the current 52,132 body
while keeping `.rel.plt` byte-identical.**  The best rel.plt-identical results
are neutral (52,132) or worse; the closest non-neutral single edit is the
`m_nState == 2 && m_nScriptTimerA >= 2000` strip at line 1447 (52,134, size
exact, `.rel.plt` identical) — 2 bytes worse than base, so not landed.

Every size-exact add cell (584 whole-condition, 219 leaf, 219 deep-leaf, 37
ternary) is **byte-identical** to the base function: adding those hints does not
change codegen at all.  The wave-183 harvest-4 cells (1447/3147/4734/5185) are
still present in the body; flipping or removing them is neutral or worse.

## The near-miss: a raw win that changes `.rel.plt`

The only configuration found with `raw < 52,132` and function size exactly
57,232 B is a two-edit combination:

1. **Strip** the existing hint on the 5700-frame station-departure transition
   (line 5185): `if (__builtin_expect((m_nScriptTimerA >= 5700 && ...), 0))`
   -> `if ((m_nScriptTimerA >= 5700 && ...))`.
2. **Add** hint 0 to the supernova-reversal guard (line 5176):
   `if (m_nScriptTimerA >= 5000 && m_nScriptTimerA - delta < 5000)`
   -> `if (__builtin_expect((m_nScriptTimerA >= 5000 && m_nScriptTimerA - delta < 5000), 0))`.

```
raw 52,132 -> 52,127   (-5), size 57,232/57,232 exact
.text diff 74,591 -> 74,588
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
at source line 5177 and `setGammaHP` at line 5211.  In the original the
switchSkybox call site is at `0x1459a4` and the setGammaHP call site at
`0x145bcc` (source order), so gold assigns switchSkybox the lower PLT slot.
The hint at 5176 makes the switchSkybox block cold/out-of-line; the two call
sites become `0x1456ee` (setGammaHP) and `0x1457fe` (switchSkybox), so gold
assigns setGammaHP the lower slot and the two `.rel.plt` entries swap.  A
third hint on the enclosing `m_nScriptTimerA >= 6500` block (line 5202, hint 0
or 1) does not restore the order; the triple measures raw 52,130/52,196 with
the same swapped `.rel.plt`.  Because `.rel.plt` must stay byte-identical, this
near-miss is **not landed**.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12908
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52132 aligned= 20658 size= 57232/57232 insn= 20725/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

The sweep drivers used here are the tracked whole-condition driver
`_work/reconstructions/levelscript-process-expect-sweep-w11901.py` (plus its
`measure_w11901.py`), and scratch variants for leaf/deep-leaf/ternary/pair
searches (`sweep_proc.py`, `leaves.py`, `deepleaves.py`, `flipremove.py`,
`ternary.py`, `pairsearch.py`, `search2.py`, `search3.py`, `search4.py`,
`combo.py`) in `/home/debian/GalaxyOnFire2/scratch/w12908/` (scratch, not
tracked).  The result tables are `cells.tsv`, `leaves.tsv`, `deepleaves.tsv`,
`cellsB.tsv`, `ternary.tsv`, `pairs.tsv`, `search2.tsv`, `search3.tsv`,
`search4.tsv`, `triples.txt` there.
