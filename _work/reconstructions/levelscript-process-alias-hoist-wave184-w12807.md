# LevelScript::process — wave 184 / w12807: alias/hoist sweep on the current body

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12807`, branch
`wave184-w12807`, base master `4c06c2662` (wave-183 harvest, raw **52,281**).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12807`.

The wave-183 task's alias/hoist sweep was re-run on the **current** body.  The
wave-182/183 edits (`g5A`, `g1A`, `tvA`, `eiA`, `e0A`, `canvasA`, `rndA`) are
present; the generator was rebuilt from scratch (block tree + expression
families) and 1,701 candidates were measured.  Four distinct edits are
size-exact, `.rel.plt`-identical raw winners and compose additively.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,281           52,271     (-10)
process aligned insns    20,700           20,630
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,723
.text diffbytes          74,842           74,832     (-10)
.ARM.extab diffbytes     646              646        (unchanged; size/base exact)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,508           75,498     (-10)
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

## Landed edits (4, all block-scope receiver aliases / unaliases)

Line numbers are from the new file.  "unalias" removes a block local and names
the initialiser at every use; "alias" adds a block-scope reference to the
`m_dirVec` member (same storage).

| # | new line | kind | edit | alone raw |
|---:|---:|---|---|---:|
| 1 | 807 | unalias | remove `TargetFollowCamera *wcamera = m_pCamera;` in `case 8`, 2 uses -> `m_pCamera` | 52,277 |
| 2 | 1173 | unalias | remove `TargetFollowCamera *camera10 = m_pCamera;` in `case 10` (s10), 2 uses -> `m_pCamera` | 52,279 |
| 3 | 3148 | alias | `Vector &v_dirVecA = m_dirVec;` in the `m_nState == 8` block, 4 uses | 52,279 |
| 4 | 3914 | unalias | remove `AEGeometry *geo = player->geometry;`, 2 uses -> `player->geometry` | 52,279 |
| all 4 | | | | **52,271** |

Every edit is a pure pointer/reference copy of a member that is never assigned
between the declaration and its uses (`m_pCamera`, `player->geometry`), or a
reference to the `m_dirVec` value member (so the alias reads/writes the same
storage).  The four block ranges are disjoint, so the combined result is
exactly the sum of the individual deltas (52,281 - 10 = 52,271).

Note: `wcamera` in `case 8` was a wave-182 w12607 unalias win on that older
body; on the current (wave-183) body removing it is worth -4, i.e. the
direction has flipped, so it was re-measured rather than assumed.

## Method

`gen.py` masks comments/string literals, matches every statement `{}` pair
inside `process` (699 blocks; switch bodies whose first token is `case`/
`default` are skipped for declaration insertion), and generates three families:

* **alias** (1,037): insert `Type *name = expr;` at the block head and rewrite
  every use in the block, for `m_pLevel`/`m_pCamera`/`m_pHud`/`m_pRadar`/
  `m_pGeometry0..6`/`m_pExplosion`/`m_pParticleGeom0..2`, `Globals::{status,
  Canvas,sound,layout,galaxy,appManager,rnd,gameText}`, `m_pLevel->{getEnemies,
  getPlayer,getMessages,getLandmarks,getStarSystem,lodManager,objectivesA/B}`,
  `player->{geometry,player}`, 21 `(*enemies)[...]` index forms, and `Vector
  &` aliases of `m_tempVec`/`m_dirVec`/`m_camVec`.
* **unalias** (531): every pointer/reference `VarDecl` with a simple
  initialiser and >=2 uses in its enclosing block; the declaration is removed
  and every use names the initialiser.
* **sub** (133): `m_pLevel->getEnemies()/getPlayer()/getMessages()` and
  `Globals::status` replaced by the function-scope `enemies`/`player`/
  `messages`/`status` local.

`worker.py` (4 workers, `ulimit -v 12000000`) compiles each candidate with the
exact `compile_commands.json` command and runs the exact gold link (stopping
before strip; `.text`/`.rel.plt` bytes are identical in the unstripped output),
then measures the raw byte diff with a small ELF reader (`measure.py`).
695 candidates are size-exact; the best are the four above.  The remaining
size-exact candidates are neutral (52,281) or worse; 158 candidates fail to
compile (mostly aliasing inside cast expressions, e.g.
`(PlayerFighter *) (*enemies)[i]->field_...`), and 848 are size-changing.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12807
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52271 aligned= 20630 size= 57232/57232 insn= 20723/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52271 size=57232/57232 text=74832 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The harnesses (`gen.py`, `worker.py`, `measure.py`, `combo.py`) and result
tables (`cands.json`, `results_all.json`) live in
`/home/debian/GalaxyOnFire2/scratch/w12807/`; they are scratch, not tracked.
