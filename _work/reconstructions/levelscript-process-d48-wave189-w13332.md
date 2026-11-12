# LevelScript::process — wave 189 / w13332: K=4..8 re-sweep, two-move win

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`,
original `0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`wt/w13332`, branch `wave189-w13332`, base master `75e111e90` (raw **51,874**,
aligned 21,005).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13332`.

The w13205 K=4..8 (d4..8) harness was rebuilt from the w13205 note with both
documented harness bugs fixed (see below), and the full d4..8 sweep was re-run
on the current 51,874 body.  Two size-exact, `.rel.plt`-identical raw winners
were found; they are independent (3,000+ lines apart) and compose additively.

## Result (code win landed)

```
                         base (51,874)    this pass
process raw diffbytes    51,874           51,870     (-4)
process aligned insns    21,005           21,002     (-3)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          73,999           73,995     (-4)
.ARM.extab diffbytes     639              639        (unchanged; size/base exact)
.ARM.exidx diffbytes     0                0
.rodata / .data          exact            exact
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
file size                2,187,328        2,187,328  (exact)
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
included), from `tools/fn_diff.py`.  `.rel.plt` md5 is
`289b2a069f8504d1e8a3919e43915e04` before and after.

## Landed moves

Line numbers are from the base (`75e111e90`, 1-based); harness statement ids
are 0-based.

| # | base line | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 3343 | down | `m_pLevel->getPlayer()->resetGunDelay();` | `resetCamera(m_pLevel);` (3344) … `camera->setTarget(player->geometry);` (3349) |
| 2 | 5106 | up | `m_pGeometry3->setVisible(true);` | `fighter->setVisible(false);` (5102) … `m_nScriptTimerB = 0;` (5105) |

Move 1 (`-1 raw` alone): `resetGunDelay` writes only the player's gun-delay
state.  The crossed statements reposition/reset the camera and set player
flags (`setFreeze`, `setVulnerable`, `setVisible`); none touches the gun
delay.

Move 2 (`-3 raw` alone): writes only `m_pGeometry3`'s visibility.  The
crossed statements write the fighter/player visibility and the two script
timers; none touches `m_pGeometry3`.

Combined the two moves are worth **-4 raw bytes** (51,870), exactly additive.

## Harness bugs fixed (from the w13205 note)

1. The link command spells the LevelScript object as
   `CMakeFiles/gof2.dir/./src/game/world/LevelScript.cpp.obj` (note the `./`);
   the replacement string must match that exact path, otherwise every
   candidate links the baseline object.  The harness now greps the object
   path out of `linkonly.txt`.
2. The access model must keep call-argument parentheses (the paren-stripping
   regex now requires the `(` not to be preceded by an identifier character)
   and normalise `(*x)` to `x` before chain extraction.

With both fixes the d4..8 sweep produced 4,233 candidates (2,817 size-exact);
the two winners above are the only ones below the 51,874 baseline.  The
remaining 2,712 size-exact candidates are neutral or worse.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13332
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51870 aligned= 21002 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses and result tables (`stmts.py`, `model.py`, `gen.py`, `eval.py`,
`sweep.py`, `elfread.py`, `res_d48.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w13332/`; they are scratch, not tracked.

## Follow-up sweeps on the landed 51,870 body

* **d13..40** (809 candidates, 414 size-exact): no candidate below 51,870.
  Five neutral moves at exactly 51,870 (`up 1803,1816`, `up 1865,1879`,
  `up 1866,1879`, `down 4950,4963`, `down 4950,4964`).
* **d9..12** (1,343 candidates, 790 size-exact): no candidate below 51,870
  (best 51,870).
* **2-opt**: all 5,090 disjoint pairs of the 104 neutral moves (99 re-measured
  neutral d4..8 moves + 5 d13..40 moves) were measured; every one is exactly
  51,870 and size-exact.
* **3-opt**: 1,000 random disjoint triples of the same neutral set; every one
  is exactly 51,870 and size-exact.

The neutral moves are codegen-neutral (some change bytes but not the diff
count); no pair/triple combination improves on the baseline.  The d4..8
two-move win above is the only improvement found in this pass.
