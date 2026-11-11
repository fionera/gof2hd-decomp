# LevelScript::process — wave 182 / w12607: fresh alias/hoist/partial-receiver sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12607`, branch
`wave182-w12607`, base master `57e085059` (wave-181 harvest 5, raw **52,523**).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12607`.

This is the wave-181 task's alias/hoist/partial-receiver sweep re-run on the
**current** body.  The wave-179 alias set (w12302) and the wave-181 unalias set
(w12504) were measured on older bodies; neither composed with the w12503
statement-reorder body.  Re-enumerating the block tree of the current body from
scratch finds eight size-exact, `.rel.plt`-identical raw winners, and all eight
compose.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,523           52,494     (-29)
process aligned insns    22,148           20,510     (-1,638)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,730  20,830 / 20,729
.text diffbytes          75,365           75,336     (-29)
.ARM.extab diffbytes     655              655        (unchanged; size/base exact at 0x1a3b78)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    76,040           76,011     (-29)
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
included), from `tools/fn_diff.py`; it is the primary metric.  `.rel.plt` md5
is `289b2a069f8504d1e8a3919e43915e04` before and after (measured with
`_work/reconstructions/measure_w11901.py`).  The 1,638-instruction aligned
improvement is a register-allocation side effect; size and section content are
unchanged except `.text`.

## Landed edits (8, all block-scope receiver aliases)

Line numbers are from the wave-181 base (`57e085059`).  "alias" adds a block
local; "unalias" removes one and names the expression at every use; "sub"
replaces a member/getter expression with an already-available function-scope
local.

| # | base line | kind | edit | alone raw |
|---:|---:|---|---|---:|
| 1 | 805 | unalias | `TargetFollowCamera *wcamera = m_pCamera;` (case 8, 3 uses) | 52,519 |
| 2 | 1004 | alias | `AEGeometry *g5A = m_pGeometry5;` (case 1, 2 uses) | 52,519 |
| 3 | 1594 | sub | `m_pLevel->getEnemies()` -> `enemies` in the mission-0 spawn loop | 52,521 |
| 4 | 2409 | unalias | `KIPlayer *pirate = (*enemies)[i];` (2 uses) | 52,522 |
| 5 | 2650 | alias | `AEGeometry *g1A = m_pGeometry1;` (`m_nState == 2`, 2 uses) | 52,514 |
| 6 | 2762 | alias | `TargetFollowCamera *camA = m_pCamera;` (`m_nState == 12`, 2 uses) | 52,519 |
| 7 | 3908 | unalias | `AEGeometry *geo = player->geometry;` (3 uses) | 52,521 |
| 8 | 5130 | unalias | `TargetFollowCamera *wcamera = m_pCamera;` (`m_nState == 11`, 3 uses) | 52,520 |
| all 8 | | | | **52,494** |

Every edit is a pure pointer copy of a value that cannot change inside its
block (`m_pCamera`, `m_pGeometry1/5`, `m_pLevel`, `player->geometry`,
`(*enemies)[i]`), or a substitution of a function-scope local (`enemies`) that
is assigned once at function entry and never reassigned.  All eight are
semantics-preserving.  Edits 1, 7 and 8 are the same three w12504 unalias wins
(-4, -2, -3); they do compose here (w12504's fourth unalias, `Level *self =
m_pLevel` in case 1, does not win on this body).

## Method

`gen.py` masks comments/string literals, matches every `{}` pair, and keeps the
compound-statement blocks inside `process` (lines 605-5356): **707 blocks**.
Braced initialisers (`Vector{...}`) are matched but not classified as blocks.
Switch bodies (first token `case`/`default`) are skipped for declaration
insertion (a declaration before the first `case` is crossed by the jump).

`gen2.py` then generates three candidate families:

* **alias** (719): insert `Type *name = expr;` at the block head and rewrite
  every use in the block for 25 expressions: `m_pLevel`, `m_pCamera`,
  `m_pHud`, `m_pRadar`, `m_pGeometry0..6`, `Globals::{Canvas,status,sound,
  layout,galaxy,rnd,appManager}`, `m_pLevel->{getEnemies,getPlayer,
  getMessages,getLandmarks,getStarSystem}`, `player->{player,geometry}`.
  Requires >=2 uses and a collision-free fresh name.
* **unalias** (126): every pointer `VarDecl` with a simple (non-`new`,
  non-cast, non-`&`) initialiser, >=2 uses in its enclosing block, no nested
  redeclaration.  The declaration is deleted and every use names the
  initialiser.
* **sub** (153): replace `m_pLevel->getPlayer()`/`getEnemies()`/
  `getMessages()`/`Globals::status` with the function-scope `player`/
  `enemies`/`messages`/`status` local inside a block (>=1 use).

`sweep.py` evaluates all 998 candidates with the exact
`compile_commands.json` command and the exact gold link (stopping before
strip), 4 workers, `ulimit -v 12000000`: 375 s.  It finds exactly **8**
size-exact, `.rel.plt`-identical raw winners (table above).  `combo.py` then
measures all 2^8 subsets with concrete descending-offset edits (all eight
block ranges are disjoint): the full set is the unique best body at
**52,494**.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12607
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52494 aligned= 20510 size= 57232/57232 insn= 20729/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52494 size=57232/57232 text=75336 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The harnesses (`gen.py`, `gen2.py`, `apply.py`, `sweep.py`, `combo.py`,
`secdiff.py`, `harness.py`) and result tables (`cands_all.json`,
`sweep_results.jsonl`, `combo_results.json`) live in
`/home/debian/GalaxyOnFire2/scratch/w12607/`; they are scratch, not tracked.

## Second sweep on the improved tree (landed)

Re-ran the same three families on the 52,494 tree (734 alias + 127 unalias +
156 sub = 1,017 candidates, 381 s).  Exactly **one** size-exact,
`.rel.plt`-identical winner remains:

| base line | kind | edit | alone raw |
|---:|---|---|---:|
| 2403 | alias | `AbyssEngine::AERandom *rndA = Globals::rnd;` (5 uses in the mission-0 pirate-spawn loop) | 52,493 |

```
                         after pass 1     after pass 2
process raw diffbytes    52,494           52,493     (-1)
process aligned insns    20,510           20,510
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          75,336           75,335     (-1)
.ARM.extab diffbytes     655              655        (unchanged)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    76,011           76,010     (-1)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

`Globals::rnd` is a static pointer that is never assigned in `process`, so the
block-scope copy is a pure pointer copy.  Cumulative: raw **52,523 -> 52,493**
(-30), `.text` **75,365 -> 75,335**, `.ARM.extab` unchanged at 655, `.rel.plt`
byte-identical, size 57,232 exact.

## Third sweep: extended expression families (landed)

Added indexed-enemy and remaining member/global expressions to the alias family
(`(*enemies)[0..3]`, `(*enemies)[i]`, `(*m_pLevel->getEnemies())[0/i]`,
`m_pExplosion`, `m_pParticleGeom0..2`, `Globals::gameText`) and a
**scope-extend** family (use an existing block alias for every remaining
occurrence of its initialiser in the same block, 35 candidates) and a
**hoist** family (move a uniquely-named block alias to function scope, 33
candidates).  356 candidates, 96 s.

Two size-exact, `.rel.plt`-identical winners:

| base line | kind | edit | alone raw |
|---:|---|---|---:|
| 3668 | alias | `KIPlayer *eiA = (*enemies)[i];` in the mission-0 case-5 two-ship loop (2 uses) | 52,469 |
| 4277 | alias | `KIPlayer *e0A = (*enemies)[0];` in case 142 (3 uses) | 52,491 |
| both | | | **52,467** |

```
                         after pass 2     after pass 3
process raw diffbytes    52,493           52,467     (-26)
process aligned insns    20,510           20,538
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,729  20,830 / 20,727
.text diffbytes          75,335           75,309     (-26)
.ARM.extab diffbytes     655              655        (unchanged)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    76,010           75,984     (-26)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

Both blocks are disjoint, both elements are read-only in their block (neither
`(*enemies)[i]` nor `(*enemies)[0]` is assigned), so the aliases are pure
pointer copies.  No extend/hoist candidate won (all neutral at 52,493 or
size-changing).

Cumulative: raw **52,523 -> 52,467** (-56), `.text` **75,365 -> 75,309**,
`.ARM.extab` unchanged at 655, `.rel.plt` byte-identical, size 57,232 exact.
