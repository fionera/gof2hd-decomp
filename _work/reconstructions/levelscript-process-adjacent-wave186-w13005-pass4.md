# LevelScript::process — wave 186 / w13005 pass 4: second adjacent sweep

Continuation of
`_work/reconstructions/levelscript-process-adjacent-combo-wave186-w13005.md`
on the same worktree/branch.  Base for this pass is the committed 52,060 tree
(`ba9002180`).  Target `_ZN11LevelScript7processEi`
(`src/game/world/LevelScript.cpp`, original `0x137d18`, **57,232 B = 0xdf90**,
size-exact).

The d1 (adjacent) sweep was re-run on the 52,060 body (2,490 candidates, 1,988
size-exact and `.rel.plt`-identical).  It found a new best adjacent swap worth
**-4** and four further -1 swaps.

## Result (code win landed)

```
                         base (52,060)    this pass
process raw diffbytes    52,060           52,056     (-4)
process aligned insns    20,787           20,787     (0)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,387           74,383     (-4)
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

## Landed move

Line numbers are from the 52,060 base (`ba9002180`); statement ids are the
harness's 0-based ids.

| # | base line (1-based) | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 3717 | down | `m_bCinematicBreak = 1;` | `player->hideShipForFirstPersonCameraView(false);` (3718) |

The flag store and the player ship-visibility call are independent.  The swap
is worth -4 raw bytes (52,056).  The other adjacent winners on this body
(`1740/1741`, `1741/1742`, `2399/2400`, each -1) are not landed here; the
`1824/1825` candidate (`*camPos += ...` / `m_pCamera->setPosition(*camPos)`) is
**not** independent (the camera call reads the vector the add writes) and was
excluded.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13005
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52056 aligned= 20787 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses and result tables (`cands_d1b.json`, `res_d1b.jsonl`, `stmts.py`,
`gen.py`, `eval.py`, `apply.py`, `greedy.py`) live in
`/home/debian/GalaxyOnFire2/scratch/w13005/`; they are scratch, not tracked.
