# LevelScript::process — wave 186 / w13005 pass 2: adjacent (d1) swaps

Continuation of
`_work/reconstructions/levelscript-process-k48-d1340-2opt-wave186-w13005.md`
on the same worktree/branch.  Base for this pass is the committed 52,074 tree
(`d3d3d7c0d`).  Target `_ZN11LevelScript7processEi`
(`src/game/world/LevelScript.cpp`, original `0x137d18`, **57,232 B = 0xdf90**,
size-exact).

The K=4..8 / d13..40 / 2-opt passes were exhausted at 52,074, so the
distance-1 (adjacent) window was re-run on the current body: 2,490 candidates
(both directions; adjacent swaps in the two directions are the same
permutation), 1,990 size-exact and `.rel.plt`-identical.

## Result (code win landed)

```
                         base (52,074)    this pass
process raw diffbytes    52,074           52,067     (-7)
process aligned insns    20,766           20,760     (-6)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,401           74,394     (-7)
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

Line numbers are from the 52,074 base (`d3d3d7c0d`); statement ids are the
harness's 0-based ids.

| # | base line (1-based) | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 3716 | down | `m_bCinematicBreak = 1;` | `m_pCamera->enableFirstPersonCam(false);` (3717) |

The two statements are independent: `m_bCinematicBreak` is a pure flag store
(`m_nFlags` high byte) and `enableFirstPersonCam` only writes camera state.
The swap is worth **-7 raw bytes** (52,074 -> 52,067).  The other adjacent
winners found in the same sweep are smaller (`36/37`, `1458/1459`,
`2677/2678` each -2; `662/663`, `1740/1741`, `1741/1742`, `1824/1825`,
`2399/2400` each -1) and are not landed here.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13005
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52067 aligned= 20760 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses and result tables (`cands_d1.json`, `res_d1.jsonl`, `stmts.py`,
`gen.py`, `eval.py`, `apply.py`) live in
`/home/debian/GalaxyOnFire2/scratch/w13005/`; they are scratch, not tracked.
