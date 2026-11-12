# LevelScript::process — wave 185 / w12907: alias/hoist sweep on the current body

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12907`, branch
`wave185-w12907`, base master `f390b0e68` (wave-184 harvest 10, raw **52,132**).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12907`.

The wave-183/184 alias/hoist sweeps were re-run on the **current** body (which
already contains the wave-183 `g5A`/`g1A`/`tvA`/`eiA`/`e0A`/`canvasA`/`rndA`
edits and the wave-184 `wcamera`/`camera10`/`v_dirVecA`/`geo` unaliases).  A
fresh generator was built from the block tree and 2,218 candidates were
measured across the requested classes: `Globals::*` reads, `m_pLevel->...`
chains, `Array` element pointers, and `Vector` temporaries.  One size-exact,
`.rel.plt`-identical raw winner was found.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,132           52,129     (-3)
process aligned insns    20,658           20,655
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,725  20,830 / 20,724
.text diffbytes          74,591           74,588     (-3)
.ARM.extab diffbytes     647              647        (unchanged; size/base exact)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,258           75,255     (-3)
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

## Landed edit (1, block-scope hoist of an existing local)

File line numbers are from the new file (block at source offsets
108522..109663, inside the cutscene-exit `else if` of the mission-0 intro
state machine, around line 1902):

```c++
                } else if (((RadioMessage *) ((*messages)[2]))->isOver() && __builtin_expect(m_nState == 1, 1)) { // lint: arm_builtin
-                   m_pRadar->enabled = 1;
                    Level *self = m_pLevel;
+                   m_pRadar->enabled = 1;
                    ...
-                   self->objectivesA = new Objective(3, 180000, m_pLevel);
+                   self->objectivesA = new Objective(3, 180000, self);
```

The block already had `Level *self = m_pLevel;` a few statements in.  Hoisting
that declaration to the top of the block (a pure pointer copy of `m_pLevel`,
which is never assigned in the block) and reusing `self` for the trailing
`Objective` argument is semantics-preserving and removes one `m_pLevel`
materialisation.  The `// lint: arm_builtin` waiver stays on the
`__builtin_expect` line.

An equivalent alias form (`Level *q1A = m_pLevel;` at the block head, rewrite
both `m_pLevel` uses) measures the same 52,129 / 57,232; the hoist form was
chosen because it introduces no extra local.  The two forms are
interchangeable for codegen purposes.

## Method

`gen.py` masks comments/string literals, matches every real statement `{}`
pair inside `process` (715 blocks; braced initialisers such as `Vector{...}`
are excluded, and switch bodies whose first token is `case`/`default` are
skipped for declaration insertion).  It generates:

* **alias** — insert `Type *name = expr;` at the block head and rewrite every
  use in the block, for `Globals::{status,Canvas,sound,layout,galaxy,
  appManager,rnd,gameText}`, `m_pLevel`/`m_pCamera`/`m_pHud`/`m_pRadar`/
  `m_pGeometry0..6`/`m_pExplosion`/`m_pParticleGeom0..2`, the function-scope
  locals `enemies`/`player`/`messages`/`status`, `player->geometry`/
  `player->player`, `m_pLevel->{getEnemies(),getPlayer(),getMessages(),
  getLandmarks(),getStarSystem(),getPlayerRoute(),lodManager,objectivesA/B}`,
  `(*enemies)[i|j|k|0..14|enemies->count-10]` and their `->geometry`/
  `->player` chains, `(*messages)[0..22|i]`, `(*wingmen)[i]`,
  `(*stationList)[i]`, `(*planets)[0]`, `(*landmarks)[3]`, and `Vector &`
  aliases of `m_tempVec`/`m_dirVec`/`m_camVec`.
* **value-ref** — `int &`/`uint8_t &`/`unsigned char &`/`KIPlayer *&` aliases
  of `enemies->count`, `m_pRadar->{enabled,lockedEnemy,dockNavPtr}`,
  `m_pHud->visible`, `(*enemies)[i]->{shipGroup,shipGroupFlag}`,
  `player->freeze`.
* **sub** — `m_pLevel->getEnemies()/getPlayer()/getMessages()` and
  `Globals::status` replaced by the function-scope locals.
* **unalias** — every pointer/reference `VarDecl` with a simple lvalue
  initialiser and >=2 uses in its enclosing block; the declaration is removed
  and every use names the initialiser (reassignments and non-simple
  initialisers are rejected).

`worker.py` (3 workers, `ulimit -v 12000000`) compiles each candidate with the
exact `compile_commands.json` command and runs the exact gold link (stopping
before strip; `.text`/`.rel.plt` bytes are identical in the unstripped
output), then measures the raw byte diff with a small ELF reader
(`measure.py`).  1,285 of the first 1,403 candidates and 404 of 605
local-alias candidates compile+link; 499 candidates are size-exact.  The
remaining size-exact candidates are neutral (52,132) or worse; only the hoist
above is below the base.  A cross-product of the winner with all 498 other
size-exact candidates (387 measured pairs) is uniformly 52,129 — no further
composition.  A same-block `-d`/`+d` size-compensating pair search over the
88 cancel pairs (|d| <= 16) finds no pair below 52,134.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12907
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52129 aligned= 20655 size= 57232/57232 insn= 20724/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52129 size=57232/57232 text=74588 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The harnesses (`gen.py`, `worker.py`, `worker_sel.py`, `combo.py`, `pair.py`,
`measure.py`, `one.py`) and result tables (`cands.json`, `all_res.json`,
`chain_res.json`, `val_res.json`, `loc_res.json`, `combo_res.json`,
`pair_res.json`) live in `/home/debian/GalaxyOnFire2/scratch/w12907/`; they
are scratch, not tracked.
