# LevelScript::process — wave 188 / w13205 pass 2: K=4..8 re-sweep, second win

Continuation of
`_work/reconstructions/levelscript-process-d48-up1236-wave188-w13205.md` on the
same worktree/branch.  Base for this pass is the committed 51,925 tree
(`f92ec097c`).  Target `_ZN11LevelScript7processEi`
(`src/game/world/LevelScript.cpp`, original `0x137d18`, **57,232 B = 0xdf90**,
size-exact).

After fixing two access-model bugs (below), the K=4..8 (d4..8) sweep was
re-run on the 51,925 body.  One further size-exact, `.rel.plt`-identical raw
winner was found and is landed here.

## Result (code win landed)

```
                         base (51,925)    this pass
process raw diffbytes    51,925           51,924     (-1)
process aligned insns    20,928           20,928     (0)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,218           74,217     (-1)
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

Line numbers are from the 51,925 base (`f92ec097c`, 1-based); harness
statement ids are 0-based.

| # | base line | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 3348 | up | `resetCamera(m_pLevel);` | `player->setFreeze(false);` (3344) … `camera->setLookAtCam(false);` (3347) |

`resetCamera` reads the player geometry and writes only the camera target and
target/cam offsets; every crossed statement writes an unrelated player flag
(`setFreeze`, `setVisible`, `setVulnerable`) or the camera's `lookAtCam` bool
(`setLookAtCam`).  The move is worth **-1 raw byte**.

## Access-model bugs fixed

The first sweep used a read/write model that was too permissive in two ways:

1. **Call-argument parentheses were stripped as if they were receiver
   parentheses.**  `m_pCamera->setPosition(m_tempVec)` normalised to
   `m_pCamera->setPositionm_tempVec`, so the argument read was lost and the
   call was not even recognised.  The paren-stripping regex now requires the
   `(` not to be preceded by an identifier character.
2. **`(*x)` was not normalised to `x`.**  `(*enemies)[0]->setPosition(...)`
   lost its receiver chain; `(*x)` is now rewritten first.

With the corrected model the d4..8 candidate set shrinks from 6,204 to 3,625
(the old model generated invalid moves such as moving
`m_pCamera->setPosition(m_tempVec)` above the `m_tempVec = ...` definition).
The two landed moves were re-checked by hand against the corrected model.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13205
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51924 aligned= 20928 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses and result tables (`stmts.py`, `model.py`, `gen.py`, `eval.py`,
`sweep.py`, `elfread.py`, `res_d48c.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w13205/`; they are scratch, not tracked.
