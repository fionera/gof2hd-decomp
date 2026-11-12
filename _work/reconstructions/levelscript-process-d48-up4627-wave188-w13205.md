# LevelScript::process — wave 188 / w13205 pass 3: K=4..8 re-sweep, third win

Continuation of
`_work/reconstructions/levelscript-process-d48-up3343-wave188-w13205.md` on the
same worktree/branch.  Base for this pass is the committed 51,924 tree
(`8e1e5ab55`).  Target `_ZN11LevelScript7processEi`
(`src/game/world/LevelScript.cpp`, original `0x137d18`, **57,232 B = 0xdf90**,
size-exact).

The corrected-model K=4..8 (d4..8) sweep on the 51,925 body produced one
further size-exact, `.rel.plt`-identical raw winner, landed here (it is
independent of the pass-2 move at line 3343).

## Result (code win landed)

```
                         base (51,924)    this pass
process raw diffbytes    51,924           51,920     (-4)
process aligned insns    20,928           20,932     (+4)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,217           74,213     (-4)
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

Line numbers are from the 51,924 base (`8e1e5ab55`, 1-based); harness
statement ids are 0-based.

| # | base line | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 4632 | up | `m_pCamera->setTarget(player->geometry);` | `m_nState++;` (4628) … `tvA += Vector{-350.0f, 400.0f, -1500.0f};` (4631) |

The camera target call reads `player->geometry`; the crossed statements write
`m_nState`, the local `tvA`, and the player speed (`player->setSpeed(0.0f)`),
none of which changes the player geometry.  The move is worth **-4 raw
bytes**.  It is 3,000+ lines away from the pass-2 move, so the two compose
independently.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13205
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51920 aligned= 20932 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses and result tables (`stmts.py`, `model.py`, `gen.py`, `eval.py`,
`sweep.py`, `elfread.py`, `res_d48c.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w13205/`; they are scratch, not tracked.
