# LevelScript::process — wave 183 / w12707: alias/hoist sweep on the current body

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12707`, branch
`wave183-w12707`, base master `9555474d8` (wave-182 harvest 5, raw **52,412**).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12707`.

The wave-182 w12607 alias/partial-receiver edits are **not** present in the
current body (no `g5A`/`g1A`/`camA`/`eiA`/`e0A`/`tvA` in the file), so the
block-scope alias sweep was re-run from scratch on the current tree.  It finds
eight size-exact, `.rel.plt`-identical raw winners covering the requested
classes: `m_pLevel->...` chains, `Array` element pointers, `Globals::*`
substitution, and `Vector` temporaries.  All eight compose additively.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,412           52,365     (-47)
process aligned insns    22,050           22,228
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,727
.text diffbytes          75,015           74,968     (-47)
.ARM.extab diffbytes     646              646        (unchanged; size/base exact)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,681           75,634     (-47)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
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
`289b2a069f8504d1e8a3919e43915e04` before and after (byte-identical to the
original).

## Landed edits (8, all block-scope receiver aliases / substitutions)

Line numbers are from the base (`9555474d8`).  "alias" adds a block local;
"sub" replaces a getter with the already-available function-scope local.

| # | base line | kind | edit | alone raw |
|---:|---:|---|---|---:|
| 1 | 3665 | alias | `KIPlayer *eiA = (*enemies)[i];` in the case-5 two-ship loop (2 uses) | 52,388 |
| 2 | 2650 | alias | `AEGeometry *g1A = m_pGeometry1;` (`m_nState == 2`, 2 uses) | 52,403 |
| 3 | 1004 | alias | `AEGeometry *g5A = m_pGeometry5;` (case 1, 2 uses) | 52,408 |
| 4 | 2762 | alias | `TargetFollowCamera *camA = m_pCamera;` (`m_nState == 12`, 2 uses) | 52,408 |
| 5 | 1594 | sub | `m_pLevel->getEnemies()->count` -> `enemies->count` in the mission-0 spawn loop | 52,410 |
| 6 | 4275 | alias | `KIPlayer *e0A = (*enemies)[0];` (case 142, 3 uses) | 52,410 |
| 7 | 908 | alias | `Vector &tvA = m_tempVec;` (case 14, 3 uses) | 52,411 |
| 8 | 4610 | alias | `Vector &tvA = m_tempVec;` (`m_nState == 3`, 3 uses) | 52,411 |
| all 8 | | | | **52,365** |

Every edit is a pure pointer copy of a value that cannot change inside its
block (`m_pGeometry1/5`, `m_pCamera`, `(*enemies)[i]`, `(*enemies)[0]`), a
substitution of the function-scope `enemies` local (assigned once at function
entry), or a reference to the `m_tempVec` member (a value member, so the
reference reads/writes the same storage).  All eight are semantics-preserving
and their block ranges are disjoint, so the combined result is exactly the sum
of the individual deltas (52,412 - 47 = 52,365).

## Method

`gen.py` masks comments/string literals, matches every `{}` pair inside
`process` (lines 605-5356): **862 blocks**.  Switch bodies whose first token is
`case`/`default` are skipped for declaration insertion (a declaration before
the first case label is crossed by the jump).

Expression families (1,233 candidates):

* **Globals::* reads** — `Globals::{status,Canvas,sound,layout,galaxy,
  appManager,rnd,gameText}`.
* **m_pLevel->... chains** — `m_pLevel->{lodManager,objectivesA,objectivesB,
  getEnemies(),getPlayer(),getMessages(),getLandmarks(),getStarSystem()}`.
* **Array element pointers** — `(*enemies)[i|0|1|2|3|10|stationIndex|
  fighterIndex|k|j|leader|chaserIndex|target|14|13|enemies->count-10]`,
  `(*messages)[0..22]`, `(*wingmen)[i]`, `(*stationList)[i]`,
  `(*planets)[0]`, `(*landmarks)[3]`, `(*m_pLevel->getEnemies())[0|i]`,
  `(*self->getEnemies())[0]`.
* **Vector temporaries** — reference aliases `Vector &x = m_tempVec|m_dirVec|
  m_camVec`.
* **member/chains** — `m_pHud`, `m_pRadar`, `m_pCamera`, `m_pGeometry0..6`,
  `m_pExplosion`, `m_pParticleGeom0..2`, `player->geometry`, `player->player`,
  `(*enemies)[i]->geometry/player`, `(*enemies)[0]->geometry/player`,
  `m_pCamera->getPosition()`, `Globals::status->getMission()`.
* **sub** — `m_pLevel->getEnemies()/getPlayer()/getMessages()` and
  `Globals::status` replaced by the function-scope locals.

Indexed expressions are only aliased when no nested block redeclares a free
variable of the expression (`i`, `j`, `k`, `stationIndex`, ...).

`sweep.py`-equivalent (`worker.py`, 4 workers, `ulimit -v 12000000`) compiles
each candidate with the exact `compile_commands.json` command and runs the
exact `build.ninja` gold link (stopping before strip; `.text`/`.rel.plt` bytes
are identical in the unstripped output).  It finds exactly **8** size-exact,
`.rel.plt`-identical raw winners (table above); the other 1,184 successful
candidates are neutral or worse, and 921 are size-changing (rejected).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12707
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52365 aligned= 22228 size= 57232/57232 insn= 20727/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52365 size=57232/57232 text=74968 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The harnesses (`gen.py`, `apply.py`, `eval.py`, `worker.py`, `combo.py`) and
result tables (`cands.json`, `results3.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w12707/`; they are scratch, not tracked.

## Second sweep on the improved tree (52,365): size-compensating pair

Re-ran the same generator (1,246 candidates, now also `Globals::switch_to_target_setting`,
`Globals::isRunningHDonWeakDevice`, and `m_pLevel->field_54/58/5c/74/80/8c` as
`int &`/`unsigned char &` reference aliases) on the 52,365 tree.  It finds
**no** further single size-exact winner (65 neutral, 213 worse, 929
size-changing).  As in wave 175 / w12607 pass 4, a size-compensating pair can
still win when neither half does; the full `-4` x `+4` cross-product
(49 x 31 = 1,519 pairs) was measured and exactly one pair is size-exact and
raw-lowering: **57,228 + 57,236 -> 52,361** (-4 from the 52,365 tree).

| base line | kind | edit | alone raw / size |
|---:|---|---|---:|
| 5140 | alias | `AEGeometry *g1A = m_pGeometry1;` (2 uses) | 52,408 / 57,228 |
| 5140 | alias | `AbyssEngine::PaintCanvas *canvasA = Globals::Canvas;` (2 uses) | 52,458 / 57,236 |
| both | | | **52,361** / 57,232 |

```
                         after pass 1     after pass 2
process raw diffbytes    52,365           52,361     (-4)
process aligned insns    22,228           22,227
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,727  20,830 / 20,726
.text diffbytes          74,968           74,964     (-4)
.ARM.extab diffbytes     646              646        (unchanged)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,634           75,630     (-4)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

Both are pure pointer copies of members/statics that are never assigned in
`process`; the pair is semantics-preserving.  The `-12` x `+12` cross-product
(27 x 12 = 324 pairs) was also measured: best size-exact pair is 52,457, no
win.  The `-8` x `+8` and `-16` x `+16` spaces were not exhausted.

Cumulative: raw **52,412 -> 52,361** (-51), `.text` **75,015 -> 74,964**,
`.ARM.extab` unchanged at 646, `.rel.plt` byte-identical, size 57,232 exact.

## Further pair probes on the 52,361 tree (no further win)

The `-16` x `+16` cross-product (65 x 25 = 1,625 pairs; 1,556 measured, 65
compile-error pairs from same-block duplicate alias names) has no size-exact
pair below 52,361 (best 52,398).  The top-11 `-8` candidates x top-5 `+8`
candidates (55 pairs) also have no win (best 52,517).  The full `-8` x `+8`
space (11,781 pairs) was not exhausted; the best `+8` candidate alone is
52,482, so the remaining space is unlikely to beat 52,361.

## Final state (wave 183 / w12707)

```
process raw diffbytes    52,412 -> 52,361   (-51)
process aligned insns    22,050 -> 22,227
orig / ours size         57232 / 57232      (size-exact)
orig / ours insns        20,830 / 20,726
.text diffbytes          75,015 -> 74,964   (-51)
.ARM.extab diffbytes     646 -> 646         (size/base exact)
.note.gnu.build-id       20 -> 20
total differing bytes    75,681 -> 75,630   (-51)
.rel.plt                 md5 289b2a06…      (byte-identical)
byte_exact               4507                4507
linked_exact             4507                4507
size-exact               4524/4524           4524/4524
data_exact               843/843             843/843
imports                  207                 207
lint                     CLEAN ✓             CLEAN ✓
drift                    0                   0
symbol parity            5370/5370           5370/5370
stub_zero_size           0                   0
missing / wrong_type     0 / 0               0 / 0
extra                    0                   0
RATCHET                  PASS                PASS
```
