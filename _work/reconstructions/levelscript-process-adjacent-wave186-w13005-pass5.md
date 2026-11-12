# LevelScript::process — wave 186 / w13005 pass 5: adjacent-winner combination

Continuation of
`_work/reconstructions/levelscript-process-adjacent-wave186-w13005-pass4.md`
on the same worktree/branch.  Base for this pass is the committed 52,056 tree
(`e0442b083`).  Target `_ZN11LevelScript7processEi`
(`src/game/world/LevelScript.cpp`, original `0x137d18`, **57,232 B = 0xdf90**,
size-exact).

The remaining valid -1 adjacent winners from the pass-4 sweep were combined
greedily on the 52,056 body; two compose and lower raw to **52,054** (-2).
The third (`1741/1742`) does not improve the combined tree.

## Result (code win landed)

```
                         base (52,056)    this pass
process raw diffbytes    52,056           52,054     (-2)
process aligned insns    20,787           20,789     (+2)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,383           74,381     (-2)
.ARM.extab diffbytes     647              647        (unchanged; size/base exact @0x1a3b78, 156,312 B)
.ARM.exidx diffbytes     0                0
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

## Landed moves

Line numbers are from the 52,056 base (`e0442b083`); statement ids are the
harness's 0-based ids.

| # | base line (1-based) | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 3569 | down | `player->player->setVulnerable(true);` | `m_pHud->visible = 1;` (3570) |
| 2 | 4749 | down | `m_pLevel->getPlayer()->resetGunDelay();` | `player->setFreeze(false);` (4750) |

Both pairs are independent: player vulnerability vs HUD visibility, and the
gun-delay reset vs the player freeze flag.  The combination is worth -2 raw
bytes.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13005
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52054 aligned= 20789 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses and result tables (`cands_d1b.json`, `res_d1b.jsonl`, `stmts.py`,
`gen.py`, `eval.py`, `apply.py`, `greedy.py`) live in
`/home/debian/GalaxyOnFire2/scratch/w13005/`; they are scratch, not tracked.
