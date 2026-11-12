# LevelScript::process — wave 186 / w13007: alias/hoist sweep, size-compensating pair

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13007`, branch
`wave186-w13007`, base master `2d02567f6` (wave-185 harvest 9, raw **52,075**).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13007`.

The wave-183/184/185 alias/hoist sweeps were re-run on the **current** body
(the wave-183 `g5A`/`g1A`/`tvA`/`eiA`/`e0A`/`canvasA`/`rndA` edits, the
wave-184 `wcamera`/`camera10`/`v_dirVecA`/`geo` unaliases and the wave-185
`self` hoist are all present).  A fresh generator was built from the block
tree and 1,615 candidates were measured across the requested classes:
`Globals::*` reads, `m_pLevel->...` chains, `Array` element pointers, and
`Vector` temporaries.  Every single edit is neutral or worse on this body, so
the **size-compensating pair** space was searched.  One pair is size-exact,
`.rel.plt`-identical and raw-lowering; it is landed below.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,075           52,067     (-8)
process aligned insns    20,766           20,762
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          74,402           74,394     (-8)
.ARM.extab diffbytes     647              647        (unchanged; size/base exact)
.ARM.exidx diffbytes     0                0
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,069           75,061     (-8)
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

## Landed edits (2, a size-compensating pair)

File line numbers are from the new file.  The two blocks are disjoint, so the
combined result is exactly the measured pair.

| # | new line | kind | edit | alone raw / size |
|---:|---:|---|---|---:|
| 1 | 2776 | alias | `AbyssEngine::PaintCanvas *canvas = Globals::Canvas;` at the head of the `m_nScriptTimerA >= 8001` block (state 13), 2 uses of `Globals::Canvas` -> `canvas` | 52,372 / 57,236 (+4) |
| 2 | 4653 | unalias | remove `Level *lvl = m_pLevel;` in the `m_nScriptTimerA >= 1001` block, 2 uses -> `m_pLevel` | 52,324 / 57,228 (-4) |
| both | | | | **52,067 / 57,232** |

Both edits are pure pointer copies of members/statics that are never assigned
between the declaration and its uses (`m_pLevel`, `Globals::Canvas`), so they
are semantics-preserving.  Neither half is a win alone: the alias grows the
body by 4 B and the unalias shrinks it by 4 B; together they restore the exact
size and lower the raw diff by 8.

## Method

`gen2.py` masks comments/string literals, matches every statement `{}` pair
inside `process` (642 blocks; braced initialisers such as `Vector{...}` are
excluded, and switch bodies whose first token is `case`/`default` are skipped
for declaration insertion).  It generates:

* **alias** — insert `auto *name = expr;` at the block head and rewrite every
  use in the block, for `Globals::{status,Canvas,sound,layout,galaxy,
  appManager,rnd,gameText}`, `m_pLevel`/`m_pCamera`/`m_pHud`/`m_pRadar`/
  `m_pGeometry0..6`/`m_pExplosion`/`m_pParticleGeom0..2`, the function-scope
  locals `enemies`/`player`/`messages`/`status`, `player->geometry`/
  `player->player`, `m_pLevel->{getEnemies(),getPlayer(),getMessages(),
  getLandmarks(),getStarSystem(),getPlayerRoute(),getDockingTarget(),
  getFriendRoute(),getGasClouds(),lodManager,objectivesA/B,
  particleRenderBoolPtr,particleEmitBoolPtr,field_*}`,
  every textual `(*enemies)[...]`, `(*messages)[...]`, `(*wingmen)[...]`,
  `(*stationList)[...]`, `(*landmarks)[...]`, `(*planets)[...]` index form
  found in the body, and `(*enemies)[i|0]->geometry/player` chains.
* **aliasref** — `auto &name = expr;` for `m_tempVec`/`m_dirVec`/`m_camVec`
  and value members (`m_nState`, `enemies->count`, `m_pLevel->getEnemies()->
  count`, `m_pRadar->{enabled,lockedEnemy,dockNavPtr}`, `m_pHud->visible`,
  `player->{freeze,enemyFlagsLo}`, `m_pLevel->field_54/58/5c`).
* **fnalias / fnaliasref** — the same expressions hoisted once to function
  scope (after the four entry locals) and rewritten over the whole body.
* **sub** — `m_pLevel->getEnemies()/getPlayer()/getMessages()` and
  `Globals::status` replaced by the function-scope locals.
* **unalias** — every pointer/reference `VarDecl` with a pure (call-free)
  initialiser and >=2 uses in its enclosing block; the declaration is removed
  and every use names the initialiser.
* **hoist / reuse / hoistreuse** — move an existing pointer `VarDecl` to the
  block head and/or reuse it for the other occurrences of its initialiser
  (the wave-185 `self` hoist shape).

Indexed expressions are only rewritten when no nested block (including a
`for`/`if` header declaration) shadows a free variable of the expression.

`worker.py` (4 workers, `ulimit -v 12000000`) compiles each candidate with the
exact `compile_commands.json` command and runs the exact gold link (stopping
before strip; `.text`/`.rel.plt` bytes are identical in the unstripped output),
then measures the raw byte diff with a small ELF reader.  738 candidates are
size-exact and `.rel.plt`-identical; **none** is below the 52,075 base (505
are exactly neutral).  The size-changing candidates were then paired:
`pair.py` combined the top 40 per side of the `-4/+4`, `-8/+8`, `-12/+12` and
`-16/+16` spaces (3,880 pairs, 17 overlap-skips).  Exactly five pairs are
size-exact, `.rel.plt`-identical and below the base; the best is the landed
pair at 52,067.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13007
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52067 aligned= 20762 size= 57232/57232 insn= 20726/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52067 size=57232/57232 text=74394 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The harnesses (`gen.py`, `gen2.py`, `worker.py`, `pair.py`, `goldtmpl.txt`)
and result tables (`cands2.json`, `res_0_1615.jsonl`, `pair_res.jsonl`) live
in `/home/debian/GalaxyOnFire2/scratch/w13007/`; they are scratch, not
tracked.

## Pass 2 (same wave): cast-expression alias, 52,067 -> 52,064

After the pair landed, the generator was re-run on the new body with an extra
candidate family: **castalias** — `auto *name = <cast expression>;` at the
block head, rewriting every use of the full cast expression in the block.
C-style `((Type *) expr)` and `reinterpret_cast<T>(expr)` forms were extracted
from the body (91 candidates).  One is a size-exact, `.rel.plt`-identical raw
winner.

```
                         pass 1           pass 2
process raw diffbytes    52,067           52,064     (-3)
process aligned insns    20,762           20,760
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,727
.text diffbytes          74,394           74,391     (-3)
.ARM.extab diffbytes     647              647        (unchanged)
.note.gnu.build-id       20               20
total differing bytes    75,061           75,058     (-3)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

Landed edit (new line 1367, in the `if (((RadioMessage *) ((*messages)[2]))
->isOver())` block of the `m_nState == 1` state):

```c++
+               PlayerFighter *fighter = (PlayerFighter *) (*enemies)[0];
                m_nState = 2;
-               ((PlayerFighter *) (*enemies)[0])->setCloakingPossible(false);
+               fighter->setCloakingPossible(false);
                (*enemies)[0]->awake();
                (*enemies)[0]->setVisible(true);
-               ((PlayerFighter *) (*enemies)[0])->setAIDisabled(true);
+               fighter->setAIDisabled(true);
```

The two cast uses read the same array element in one block; caching the
downcast pointer is semantics-preserving (the intervening calls do not
reallocate `enemies[0]`).  Alone it measures 52,064 / 57,232.

Cumulative wave-186 result: raw **52,075 -> 52,064** (-11), `.text`
**74,402 -> 74,391**, `.ARM.extab` unchanged at 647, `.rel.plt` byte-identical,
size 57,232 exact.
