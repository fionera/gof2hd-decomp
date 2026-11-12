# LevelScript::process — wave 188 / w13207: alias/hoist sweep on the wave-187 body

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13207`, branch
`wave188-w13207`, base master `ce16f45f1` (wave-187 harvest 5, raw **51,930**,
`.text` 74,223).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13207`.

**Result: no code win.**  The alias/hoist space was re-run on the current
51,930 body; 6,164 candidates were measured, 1,691 are size-exact and
`.rel.plt`-identical, but **all 1,691 are neutral at raw 51,930**.  The
size-compensating pair space was then searched in four passes; the best pair
is raw **51,939**, nine bytes *worse* than the base.  `src/` is left at the
base and this note is the only change.

## Baseline measured in this worktree

```
process raw diffbytes    51,930
process aligned insns    20,928
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726
.text diffbytes          74,223
.rel.plt                 md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

## Candidate generator (scratch `gen.py`)

The generator masks comments/string literals, enumerates every statement `{}`
pair inside `process` (862 blocks), and emits block-scope edits for the
requested classes:

* **alias** — `auto *name = expr;` at the block head, rewriting every use of
  `expr` in the block: `Globals::{status,Canvas,sound,layout,galaxy,appManager,
  rnd,gameText}`, `m_pLevel`/`m_pCamera`/`m_pHud`/`m_pRadar`/
  `m_pGeometry0..6`/`m_pExplosion`/`m_pParticleGeom0..2`, the function-scope
  locals `enemies`/`player`/`messages`/`status`,
  `m_pLevel->{getEnemies(),getPlayer(),getMessages(),getLandmarks(),
  getStarSystem(),getPlayerRoute(),getFriendRoute(),getGasClouds(),
  getDockingTarget(0/1),lodManager,objectivesA/B,field_*}`,
  `(*enemies|messages|wingmen|landmarks|planets|stationList)[...]` and the
  `(*enemies)[...]->{geometry,player}` / `player->{geometry,player}` chains.
* **aliasref** — `auto &name = expr;` for `m_tempVec`/`m_dirVec`/`m_camVec`.
* **castalias** — `auto *name = ((Type *) expr);` for the C-style downcasts.
* **unalias** — remove a pure pointer/ref `VarDecl` with >=2 uses and name its
  initialiser at every use.
* **hoist** — move a pure pointer `VarDecl` to the block head.

Blocks whose first token is `case`/`default` are skipped for declaration
insertion; an expression is skipped when a nested block declares one of its
free identifiers.  The outermost function block is skipped (the four entry
locals already cover it) and plain member/global aliases are only generated
for blocks <= 1,500 chars, which leaves 6,164 candidates.

`harness.py` compiles the TU with the exact `compile_commands.json` command and
runs the exact gold link (stopping before strip; `.text`/`.rel.plt` are
identical in the unstripped output), then measures raw/size/`.rel.plt` with a
small ELF reader.  `worker.py` runs 4 workers under `ulimit -v 12000000`
(~0.33 s/candidate).

## Single-edit result

6,164 candidates; 4,783 compile+link with the original `.rel.plt` md5.
1,691 are size-exact (57,232) and **every one is raw 51,930** — the same code
as the base.  The size-changing candidates are all worse; the closest are:

| size | best raw | edit |
|---:|---:|---|
| 57,224 (-8) | 51,957 | alias `(*enemies)[3]` in the block at 183,437..184,073 |
| 57,220 (-12) | 51,967 | alias `(*enemies)[3]->geometry` in the same region |
| 57,228 (-4) | 51,986 | alias `(*enemies)[3]` at 184,177..185,824 |
| 57,236 (+4) | 51,979 | hoist `m_pCamera` at 225,550..227,456 |
| 57,240 (+8) | 51,968 | alias `m_pCamera` at 183,437..184,073 |

No single edit is below 51,930.

## Size-compensating pair result

Four searches, all requiring net size 57,232, disjoint blocks and the original
`.rel.plt` md5:

| search | pairs | best size-exact raw |
|---|---:|---:|
| `-4/+4`, top 40 per side (`pair.py 4 40`) | 1,600 | 51,942 |
| `-8/+8`, top 40 per side (`pair.py 8 40`) | 1,600 | 51,940 |
| same-block `-8/+8` pairs (`sbbatch.py`) | 217 | **51,939** |
| `-4` top 5 x *all* `+4` (`pairx.py 4 5`) | 510 | 51,949 |

The best pair is the same-block pair `(912, 933)`:
`auto *zzq11819 = (*enemies)[3];` and `auto *zzq11814 = m_pCamera;` at the head
of the block at 183,437..184,073 (raw 51,939, size 57,232).  Individually these
are 51,957 and 51,968; their interaction cancels 56 bytes but not the last 9.
The best cross-block pair is `(3098, 3297)` at 51,949.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13207
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51930 aligned= 20928 size= 57232/57232 insn= 20726/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51930 size=57232/57232 text=74223 relplt=289b2a069f8504d1e8a3919e43915e04
```

The harnesses (`gen.py`, `harness.py`, `worker.py`, `pair.py`, `pairx.py`,
`sbbatch.py`, `sameblock.py`) and result tables (`cands.json`, `cands_f.json`,
`res.jsonl`, `pair_res.jsonl`, `pairx_res.jsonl`, `sb_res.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w13207/`; they are scratch, not tracked.

## Conclusion

On the wave-187 body the whole block-scope alias/hoist single-edit space and
the size-compensating pair neighbourhood are exhausted without a win.  The
remaining 51,930 raw bytes are not reachable by aliasing `Globals::*` reads,
`m_pLevel->...` chains, `Array` element pointers or `Vector` temporaries; the
next angle should be regalloc/declaration-lifetime (w13201), branch layout
(w13202), data flow (w13203) or literal-pool placement (w13206).
