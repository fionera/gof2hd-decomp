# LevelScript::process — wave 186 / w13005: K=4..8 + d13..40 re-sweep and 2-opt

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13005`, branch
`wave186-w13005`, base master `2d02567f6` (wave-185 harvest, raw **52,075**,
aligned 20,766).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13005`.

The w12905 harness was not preserved in scratch, so it was rebuilt from the
w12905 note (`stmts.py`, `gen.py`, `eval.py`, `apply.py`, `greedy.py`,
`pairs.py`).  The sweep was re-run on the current 52,075 body and the
distance-13..40 range was re-checked; the 2-opt pass over the neutral single
moves found the only win.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,075           52,074     (-1)
process aligned insns    20,766           20,766     (0)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,402           74,401     (-1)
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

`raw` is the same-address byte diff over the function body (literal pools
included), from `tools/fn_diff.py`.  `.rel.plt` md5 is
`289b2a069f8504d1e8a3919e43915e04` before and after.

## Method

* `stmts.py` parses the `process` body (lines 606-5364), tracks brace depth and
  keeps single-line statements (`;`-terminated, brace-free, balanced, not
  `case`/`default`/`break`/`continue`/`return`/control header).
  **2,727 statements** in the current body.
* `gen.py` keeps a pair `(i,j)` only when every statement between them has the
  same brace depth, every non-blank/non-comment source line between them is
  itself a recognized statement line, and the moved statement is prefix-aware
  independent of every crossed statement including the far endpoint.  Both
  directions are generated.  Access chains are parsed with `(*x)[i]->y`
  normalised to `x[i]->y`; prefix aliasing (`m_tempVec` vs `m_tempVec.x`)
  blocks a move, and calls are treated as read+write of their receiver chain
  (two calls on the same receiver base keep their order).
* `eval.py` compiles one candidate with the exact `compile_commands.json`
  command, runs the exact gold link command (stopping before strip), and
  reports the same-address raw diff, function size and `.rel.plt` md5 with a
  minimal ELF32 reader.  Four workers, `ulimit -v 12000000`.
* `apply.py` applies a sequence of base-statement moves to the line array
  (remove the moved statement line, insert before/after the target line), then
  measures the combined tree.  `greedy.py` adds neutral moves one at a time;
  `pairs.py` evaluates all near-range neutral pairs in both orders.

Candidate counts: **2,736** for K=4..8 and **392** for d13..40.  2,156 are
size-exact and `.rel.plt`-identical.

### Single-move results

With the corrected independence model the K=4..8 / d13..40 space has **no**
single-move winner: the best is 52,075 (neutral).  88 moves are neutral
(size-exact, `.rel.plt`-identical, raw == 52,075).

### 2-opt result

All 272 neutral pairs whose statement ranges overlap or are within 8
statements were evaluated in both orders (544 candidates).  One combination
reaches raw **52,074** (-1); every other pair is >= 52,075.  Greedily adding a
third neutral move to the winning pair does not improve it (all 88 additions
stay at 52,074).

## Landed moves

Line numbers are from the base (`2d02567f6`); statement ids are the harness's
0-based ids.  "up"/"down" is relative to the base position.

| # | base line (1-based) | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 1875 | up | `m_pRadar->enabled = 0;` | `m_bCinematicBreak = 1;` (1869) … `m_pHud->visible = 0;` (1874) |
| 2 | 1871 | down | `resetCamera(m_pLevel);` | `m_pCamera->enableFirstPersonCam(false);` (1872) … `player->stopShooting(0);` (1876) |

Both moves are independent of every crossed statement: the radar flag is a
pure field store on `m_pRadar`, while the crossed statements store
`m_bCinematicBreak`, the camera mode, the player vulnerability flag and the HUD
visibility; `resetCamera` only writes the camera target/offsets (and reads
`m_pLevel`), and the crossed statements are camera-mode/player/HUD/radar
stores.  Neither move alone changes the raw diff (both are neutral); the pair
is worth -1.

## A parser bug found and fixed mid-pass

The first sweep used a statement-access parser whose compound-assignment
handling dropped the LHS (`camPos += ...` produced an empty write set), so it
reported a spurious -1 move (`camPos += dir * 10000.0f;` past
`m_pCamera->setPosition(camPos)`), which is semantically invalid because
`setPosition` reads `camPos`.  The parser now strips the compound operator
before locating the LHS chain and normalises `(*x)` receiver chains; the
re-sweep with the corrected model is the one reported above.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13005
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52074 aligned= 20766 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2         # 0 fields
```

The harnesses (`stmts.py`, `gen.py`, `eval.py`, `apply.py`, `greedy.py`,
`pairs.py`) and result tables (`cands.json`, `res_all2.jsonl`,
`neutral2.json`, `pairs_res2.json`) live in
`/home/debian/GalaxyOnFire2/scratch/w13005/`; they are scratch, not tracked.
