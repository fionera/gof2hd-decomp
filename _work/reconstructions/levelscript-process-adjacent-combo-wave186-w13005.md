# LevelScript::process — wave 186 / w13005 pass 3: adjacent-winner combination

Continuation of
`_work/reconstructions/levelscript-process-adjacent-wave186-w13005.md` on the
same worktree/branch.  Base for this pass is the committed 52,067 tree
(`64f8b2b04`).  Target `_ZN11LevelScript7processEi`
(`src/game/world/LevelScript.cpp`, original `0x137d18`, **57,232 B = 0xdf90**,
size-exact).

The d1 sweep found nine distinct adjacent winners.  The best
(`m_bCinematicBreak = 1;` / `m_pCamera->enableFirstPersonCam(false);`, -7) was
landed in pass 2.  The remaining eight were combined greedily on the 52,067
tree (each re-checked size-exact and `.rel.plt`-identical); four compose and
lower raw to **52,060** (-7).  The other four (`1740/1741`, `1741/1742`,
`1824/1825`, `2399/2400`) do not improve the combined tree.

## Result (code win landed)

```
                         base (52,067)    this pass
process raw diffbytes    52,067           52,060     (-7)
process aligned insns    20,760           20,787     (+27)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,394           74,387     (-7)
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
stub_zero_size         0                0
missing / wrong_type     0 / 0            0 / 0
extra                    0                0
RATCHET                  PASS             PASS
```

## Landed moves

Line numbers are from the 52,067 base (`64f8b2b04`); statement ids are the
harness's 0-based ids.  All four are adjacent swaps (`down` = moved statement
i now follows j).

| # | base line (1-based) | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 692 | down | `m_pCamera->enableFirstPersonCam(false);` | `resetCamera(self);` (693) |
| 2 | 3154 | down | `m_pCamera->setTarget((*enemies)[3]->geometry);` | `m_tempVec += v_dirVecA * 10000.0f;` (3155) |
| 3 | 5247 | down | `m_nScriptTimerB = 0;` | `((PlayerFighter *) (*enemies)[0])->setExhaustVisible(true);` (5248) |
| 4 | 1909 | down | `m_pCamera->setLookAtCam(false);` | `player->setComputerControlled(false);` (1910) |

Each pair is independent under the prefix-aware model: camera-mode/target vs
the player reset / `m_tempVec` arithmetic / enemy exhaust flag, and the
script-timer store vs the enemy exhaust call / player computer-control flag.
The combination is worth -7 raw bytes.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13005
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52060 aligned= 20787 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses and result tables (`cands_d1.json`, `res_d1.jsonl`, `stmts.py`,
`gen.py`, `eval.py`, `apply.py`, `greedy.py`) live in
`/home/debian/GalaxyOnFire2/scratch/w13005/`; they are scratch, not tracked.
