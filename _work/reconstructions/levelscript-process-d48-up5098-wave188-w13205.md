# LevelScript::process — wave 188 / w13205 pass 4: K=4..8 re-sweep, fourth win

Continuation of
`_work/reconstructions/levelscript-process-d48-up4627-wave188-w13205.md` on the
same worktree/branch.  Base for this pass is the committed 51,920 tree
(`c5ac0cef9`).  Target `_ZN11LevelScript7processEi`
(`src/game/world/LevelScript.cpp`, original `0x137d18`, **57,232 B = 0xdf90**,
size-exact).

The completed corrected-model K=4..8 sweep (`res_d48c.jsonl`) left one more
size-exact, `.rel.plt`-identical winner far from the landed moves; it is
landed here.

## Result (code win landed)

```
                         base (51,920)    this pass
process raw diffbytes    51,920           51,917     (-3)
process aligned insns    20,932           20,927     (-5)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,213           74,210     (-3)
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

Line numbers are from the 51,920 base (`c5ac0cef9`, 1-based); harness
statement ids are 0-based.

| # | base line | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 5106 | up | `m_pGeometry2->setVisible(false);` | `m_pGeometry3->setPosition(...)` (5099) … `m_pGeometry3->setVisible(true);` (5105) |

The moved call writes only `m_pGeometry2`'s visibility.  The crossed
statements write `m_pGeometry3`'s transform/visibility, the fighter and player
visibility, and the two script timers; none touches `m_pGeometry2`.  The move
is worth **-3 raw bytes**.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13205
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51917 aligned= 20927 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses and result tables (`stmts.py`, `model.py`, `gen.py`, `eval.py`,
`sweep.py`, `elfread.py`, `res_d48c.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w13205/`; they are scratch, not tracked.
