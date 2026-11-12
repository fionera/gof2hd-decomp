# LevelScript::process — wave 187 / w13107: alias/hoist sweep on the wave-186 body

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13107`, branch
`wave187-w13107`, base master `8b5d4281c` (wave-186 harvest 6, raw **51,968**,
`.text` 74,274).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13107`.

The wave-183..186 alias/hoist sweeps were re-run on the **current** body (which
already contains the w13007 alias/castalias/Vector-ref pairs and the w13005
statement reorders).  A fresh generator was built from the block tree and
**1,576** compiling candidates were measured across the requested classes:
`Globals::*` reads, `m_pLevel->...` chains, `Array` element pointers, and
`Vector` temporaries.  Exactly one is a size-exact, `.rel.plt`-identical raw
winner; it is landed below.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,968           51,965     (-3)
process aligned insns    20,856           20,859
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,727  20,830 / 20,727
.text diffbytes          74,274           74,271     (-3)
.ARM.extab diffbytes     647              647        (unchanged; size/base exact)
.ARM.exidx diffbytes     0                0
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    74,941           74,938     (-3)
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

## Landed edit (1, block-scope Array-element alias)

File line numbers are from the new file; the block is the mission-0
`m_nScriptTimerA > 25000` visibility gate (around line 5240):

```c++
                    if (m_nScriptTimerA > 25000) {
                        if (!(*enemies)[0]->isVisible()) {
-                           (*enemies)[0]->setVisible(true);
-                           (*enemies)[0]->setActive(true);
+                           KIPlayer *enemy = (*enemies)[0];
+                           enemy->setVisible(true);
+                           enemy->setActive(true);
                        }
                    }
```

`enemies` is the function-scope cached `Array<KIPlayer *> *` from
`m_pLevel->getEnemies()` (line 613).  The two calls read the same array
element in one block and neither reallocates the array, so caching the element
pointer is semantics-preserving.  Measured alone on the wave-186 body:
raw **51,968 -> 51,965** (-3 B), size 57,232/57,232 exact, `.text`
74,274 -> 74,271, `.rel.plt` unchanged.

## Method

`gen.py` masks comments/string literals, matches every statement `{}` pair
inside `process` (statement blocks only; braced initialisers such as
`Vector{...}` are excluded, and switch bodies whose first token is
`case`/`default` are skipped for declaration insertion).  It generates:

* **alias** — insert `auto *name = expr;` at the block head and rewrite every
  use in the block, for `Globals::{status,Canvas,sound,layout,galaxy,
  appManager,rnd,gameText}`, `m_pLevel`/`m_pCamera`/`m_pHud`/`m_pRadar`/
  `m_pGeometry0..6`/`m_pExplosion`/`m_pParticleGeom0..2`, the function-scope
  locals `enemies`/`player`/`messages`/`status`, `player->geometry`/
  `player->player`, `m_pLevel->{getEnemies(),getPlayer(),getMessages(),
  getLandmarks(),getStarSystem(),getPlayerRoute(),getFriendRoute(),
  getGasClouds(),getDockingTarget(0/1),lodManager,objectivesA/B}`, every
  textual `(*enemies|messages|wingmen|landmarks|planets|stationList)[...]`
  index form found in the body, and `(*enemies)[i|j|k|0]->geometry/player`.
* **aliasref** — `auto &name = expr;` for `m_tempVec`/`m_dirVec`/`m_camVec`.
* **unalias** — every pointer/reference `VarDecl` with a pure (call-free or
  pure-getter) initialiser and >=2 uses in its enclosing block; the
  declaration is removed and every use names the initialiser.
* **hoist** — move an existing pure pointer/ref `VarDecl` to the block head.

Indexed expressions are only rewritten when no nested block shadows a free
variable of the expression, and bare lvalues are skipped when the block
assigns them.  `harness.py` compiles each candidate with the exact
`compile_commands.json` command, runs the exact gold link (stopping before
strip; `.text`/`.rel.plt` bytes are identical in the unstripped output), and
measures with `_work/reconstructions/measure_w11901.py`.  ~0.4 s/candidate
with 4 workers under `ulimit -v 12000000`.

The 1,576 valid candidates are all size-exact or size-changing as tabulated;
**none** other than the landed edit is below the 51,968 base (752 are exactly
neutral at 51,968).  A size-compensating pair search is the natural next step
on this body; the single-edit space is exhausted.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13107
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51965 aligned= 20859 size= 57232/57232 insn= 20727/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51965 size=57232/57232 text=74271 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

The harnesses (`gen.py`, `gen2.py`, `harness.py`, `worker.py`, `adiff.py`) and
result tables (`cands.json`, `res.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w13107/`; they are scratch, not tracked.

## Pass 2 (same wave): size-compensating alias pair, 51,965 -> 51,962

The single-edit sweep was re-run on the pass-1 body (1,584 candidates): no
single edit wins.  The size-compensating pair search was then run over the
`-4/+4`, `-8/+8`, `-12/+12`, `-16/+16`, `-20/+20`, `-24/+24` spaces (991 pairs,
individual raw <= base+1000).  One pair is size-exact, `.rel.plt`-identical and
raw-lowering.

```
                         pass 1           pass 2
process raw diffbytes    51,965           51,962     (-3)
process aligned insns    20,859           20,857
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,727  20,830 / 20,725
.text diffbytes          74,271           74,268     (-3)
.ARM.extab diffbytes     647              647        (unchanged)
.note.gnu.build-id       20               20
total differing bytes    74,938           74,935     (-3)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

Landed edits (disjoint blocks; neither is a win alone):

| # | new line | kind | edit |
|---:|---:|---|---|
| 1 | 3342 | alias | `TargetFollowCamera *camera = m_pCamera;` at the head of the `m_nScriptTimerA >= 2001` block; 3 uses of `m_pCamera` -> `camera` |
| 2 | 5239 | hoist | hoist the existing `KIPlayer *enemy = (*enemies)[0];` above the `if (!(*enemies)[0]->isVisible())` test and use `enemy` in the condition |

Both are pure pointer copies of a member / array element that are not
reassigned in their blocks; the pair restores the exact size and lowers the
raw diff by 3.

Cumulative wave-187 result: raw **51,968 -> 51,962** (-6), `.text`
**74,274 -> 74,268**, `.ARM.extab` unchanged at 647, `.rel.plt` byte-identical,
size 57,232 exact.
