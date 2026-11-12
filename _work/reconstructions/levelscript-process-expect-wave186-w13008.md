# LevelScript::process — wave 186 / w13008: `__builtin_expect` add/flip/remove re-sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13008`, branch
`wave186-w13008`, base master `2d02567f6` (wave-185 harvest 9, raw **52,075**,
aligned **20,766**, `.text` diff **74,402**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13008`.

This pass re-runs the whole `__builtin_expect` sweep on the current wave-185 body
(the body changed by 57 diff-bytes since w12908's 52,132 re-sweep), covering:
whole-condition adds, top-level `&&`/`||` leaf adds, every existing hint
flip/remove, ternary-condition adds, operands *inside* existing
`__builtin_expect(...)` wrappers, the nested inner hint at line 4336, switch
subjects, and cancelling-size pair/triple searches.  A candidate is acceptable
only if it is **size-exact (57,232 B) and `.rel.plt`-identical** (md5
`289b2a069f8504d1e8a3919e43915e04`).

## Result: no acceptable win; `src/` left at base

```
                         base (HEAD)      this pass
process raw diffbytes    52,075           52,075     (unchanged)
process aligned insns    20,766           20,766
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
ours insns               20,726            20,726
.text diffbytes          74,402           74,402
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
included) from `tools/fn_diff.py`; `check` was run on the clean base tree and
reported `RATCHET PASS`.  `lint` reported `CLEAN ✓` and `drift` reported
`0 field(s) across 0 class(es)`.

## Sweeps run (all on the current 52,075 body)

| sweep | cells | size-exact | size-exact + rel.plt-identical | best raw |
|---|---:|---:|---:|---:|
| whole-condition add (`if`/`for`, 643 conditions x {0,1}) | 1,286 | 578 | 569 | 52,075 |
| top-level `&&`/`||` operand add | 446 | 200 | 190 | 52,075 |
| deep (recursively split) operand add | 446 | 200 | 190 | 52,075 |
| flip/remove of every existing hint (67 x {flip,remove}) | 134 | 57 | 57 | 52,075 |
| ternary-condition add (21 `?:`) | 42 | 27 | 27 | 52,075 |
| operand add *inside* an existing `__builtin_expect(...)` arg | 30 | 30 | 30 | 52,075 |
| switch-subject add (10 `switch (...)`) | 20 | 5 | 4 | 52,075 |
| cancelling-size region pairs/triples (lines 5050–5220) | 208 | 41 | 31 | 52,070* |
| cancelling-size pairs, all regions (87 pairs) | 87 | 62 | 55 | 52,070* |

\* the 52,070 result is the near-miss below, which changes `.rel.plt`.

**Every size-exact add cell is byte-identical to the base function** (raw stays
52,075): adding a hint at those sites does not change codegen at all.  No single
flip/remove is below 52,075 either; the size-exact neutral flips/removes are
also byte-identical.

### Near-miss: a raw win that changes `.rel.plt`

The only configuration found with `raw < 52,075` and function size exactly
57,232 B is the same two-edit combination w12908 found on the older body:

1. **Strip** the existing hint on the 5700-frame station-departure transition
   (line 5185):
   `if (__builtin_expect((m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700), 0))`
   -> `if ((m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700))`.
2. **Add** hint 0 to the supernova-reversal guard (line 5176):
   `if (m_nScriptTimerA >= 5000 && m_nScriptTimerA - delta < 5000)`
   -> `if (__builtin_expect((m_nScriptTimerA >= 5000 && m_nScriptTimerA - delta < 5000), 0))`.

```
raw 52,075 -> 52,070   (-5), size 57,232/57,232 exact
.text diff 74,402 -> 74,399
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
at source line 5177 and `setGammaHP` at line 5211.  The hint at 5176 makes the
switchSkybox block cold/out-of-line, so its call site is emitted after the hot
`m_nScriptTimerA >= 6500` block that contains `setGammaHP`; gold then assigns
`setGammaHP` the lower PLT slot.  Measured call sites:

```
base     switchSkybox 0x1454f0   setGammaHP 0x1456b2   rel.plt 289b2a06…
combo    setGammaHP  0x1456ee   switchSkybox 0x1457fe  rel.plt 07752cde…
```

Attempts to restore the PLT order on the current body all failed (each remains
size-exact or nearly so but keeps the swapped `.rel.plt`):

* hint 0/1 on the enclosing `if (m_nScriptTimerA >= 6500)` (line 5202):
  raw 52,070 / 52,070, same swap;
* hint 0 on `if (m_nScriptTimerA > 5000 && m_nScriptTimerA < 6000)` (line 5194):
  raw 52,142, same swap;
* hint 0 on `if (m_nScriptTimerA >= 5500)` (line 5191): raw 52,070, same swap;
* merge the 6500 block into `else if` after the planet block (semantically
  equivalent: the two conditions are mutually exclusive), with or without
  `__builtin_expect(..., 0/1)`: raw 52,137, size 57,228, same swap;
* move the switchSkybox block after the 6500 block: raw 52,218, size 57,252,
  same swap; move it before `setRumblePercentage`: raw 52,188, size 57,236,
  same swap.

Because `.rel.plt` must stay byte-identical this near-miss is **not landed**.
(The task's total diff budget also treats `.rel.plt` as byte-identical; the
swap would add 8 differing bytes while saving only 5 in `.text`.)

### Other near misses (rel.plt-identical but worse than base)

| edit(s) | raw | size | note |
|---|---:|---:|---|
| remove inner hint at line 4336 | 52,076 | 57,232 | +1 |
| flip inner hint at line 4336 | 52,080 | 57,232 | +5 |
| `else if` merge of the 6500 block (M1) | 52,087 | 57,232 | +12 |
| move switchSkybox block after 6500 block | 52,108 | 57,232 | +33 |
| `if (2775) add0` + `if (2800) add1` | 52,090 | 57,232 | best rel.plt-identical pair |
| `remove(5191)` + `add0(5140)` | 52,136 | 57,232 | best region pair |

The nested hint at line 4336
(`__builtin_expect(__builtin_expect(!...field_0x13c, 0) && Globals::rnd->nextInt(100) <= 9, 0)`)
was not covered by the earlier w12908 sweep (its hint scanner skipped nested
calls); flipping or removing the inner hint is size-exact and
`.rel.plt`-identical but does not improve the raw diff.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13008
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52075 aligned= 20766 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN ✓
cmake --build cmake-build-match --target drift -- -j2   # 0 drifted fields
```

The whole-condition driver is the tracked
`_work/reconstructions/levelscript-process-expect-sweep-w11901.py` (with
`measure_w11901.py`).  Scratch drivers used here (not tracked, in
`/home/debian/GalaxyOnFire2/scratch/w13008/`): `hints.py` (nested-aware
`__builtin_expect` finder), `conds.py` (condition finder + top-level
`&&`/`||` splitter), `edit.py` (remove/flip/add edit primitives),
`fr.py` (flip/remove sweep), `leaf.py top|deep` (operand-add sweeps),
`ternary.py`, `innerleaf.py`, `switch.py`, `pair_region.py`, `pair_all.py`,
and the structural-combo drivers `combo*.py`.  Result tables: `fr.tsv`,
`cells.tsv`, `leaf_top.tsv`, `leaf_deep.tsv`, `ternary.tsv`, `innerleaf.tsv`,
`switch.tsv`, `pair_region.tsv`, `pair_all.tsv`, `combo*.tsv`.
