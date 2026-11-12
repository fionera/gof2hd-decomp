# LevelScript::process — wave 188 / w13205 pass 5: K=4..8 region re-sweep, fifth win

Continuation of
`_work/reconstructions/levelscript-process-d48-up5098-wave188-w13205.md` on the
same worktree/branch.  Base for this pass is the committed 51,917 tree
(`72818563d`).  Target `_ZN11LevelScript7processEi`
(`src/game/world/LevelScript.cpp`, original `0x137d18`, **57,232 B = 0xdf90**,
size-exact).

The remaining winners from the completed d4..8 sweep were re-evaluated on the
current body in the three regions touched by the landed moves (lines
3335-3355, 4600-4700, 5090-5110; 190 candidates).  One further size-exact,
`.rel.plt`-identical raw winner is landed here.

## Result (code win landed)

```
                         base (51,917)    this pass
process raw diffbytes    51,917           51,914     (-3)
process aligned insns    20,927           20,924     (-3)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,210           74,207     (-3)
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

## Landed move

Line numbers are from the 51,917 base (`72818563d`, 1-based); harness
statement ids are 0-based.

| # | base line | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 5106 | up | `m_pGeometry3->setVisible(true);` | `fighter->setVisible(false);` (5102) … `m_nScriptTimerB = 0;` (5105) |

The moved call writes only `m_pGeometry3`'s visibility.  The crossed statements
write the fighter and player visibility and the two script timers; none touches
`m_pGeometry3`.  The move is worth **-3 raw bytes**.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13205
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51914 aligned= 20924 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses and result tables (`stmts.py`, `model.py`, `gen.py`, `eval.py`,
`sweep.py`, `sweep_region.py`, `elfread.py`, `res_regions.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w13205/`; they are scratch, not tracked.
