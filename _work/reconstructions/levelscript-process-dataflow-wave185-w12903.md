# LevelScript::process — DATA-FLOW pass (wave 185, w12903)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12903`, branch
`wave185-w12903`, base master `f390b0e68` (wave-184 harvest 10, raw **52,132**,
`.text` 74,591).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12903`.

Continuation of the wave-182/183 data-flow passes (w12604, w12703): bool vs
int, `float` vs `int` casts, `Vector` construction/assignment forms, `Array`
indexing, multiply-vs-branch.  One size-exact, `.rel.plt`-identical raw win
landed.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,132           52,122     (-10)
process aligned insns    20,658           20,678     (+20)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,725 / 20,830  20,724 / 20,830
.text diffbytes          74,591           74,581     (-10)
.ARM.extab diffbytes     646              646        (size/base exact, 156,312 B @ 0x1a3b78)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,257           75,247     (-10)
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
included) from `tools/fn_diff.py`; `.rel.plt` md5 is
`289b2a069f8504d1e8a3919e43915e04` before and after (byte-identical to the
original).

## Landed change (source line 1277)

Mission-0 `case 1` block, immediately before `return 1;`:

```cpp
// before
m_pGeometry6->setDirection(-(*m_pLevel->getEnemies())[0]->geometry->getDirection(),
                           Vector{0.0f, 1.0f, 0.0f});
// after
m_pGeometry6->setDirection(-(*enemies)[0]->geometry->getDirection(),
                           Vector{0.0f, 1.0f, 0.0f});
```

`enemies` is the function-scope cached `Array<KIPlayer *> *` from
`m_pLevel->getEnemies()` (line 613).  `Level::getEnemies()` is a pure getter
(`return m_pEnemies;`), so naming the cached pointer is behaviour-neutral, but
it removes one reload of `m_pLevel` + one getter call at this site and moves
the emitted sequence closer to the original.  Measured alone on the wave-184
body: raw **52,132 -> 52,122** (-10 B), size 57,232/57,232 exact, `.text`
74,591 -> 74,581, `.rel.plt` unchanged.

## Method

`scratch/w12903/harness.py` / `batch.py` compile one candidate with the exact
`compile_commands.json` command, run the exact `build.ninja` gold link (stop
before strip; `.text`/`.rel.plt` bytes are identical in the unstripped output),
and measure with `_work/reconstructions/measure_w11901.py`.  ~1.5 s/candidate.
`linemap.py`/`align.py` map our `-g` line info onto the original disassembly
for inspection.

### `Array`-indexing / cached-local sweep (the win)

All **62** individual `(*m_pLevel->getEnemies())` -> `(*enemies)` sites inside
`process` were measured one at a time (size-exact, `.rel.plt`-identical unless
noted).  Only line 1277 improves; the neighbours are strongly negative:

| line | raw | delta |
|---:|---:|---:|
| 1277 `setDirection(-(*enemies)[0]->geometry->getDirection(), …)` | 52,122 | **-10** |
| 1270 `(*enemies)[0]->setPosition(m_tempVec)` | 52,531 | +399 |
| 1272 `(*enemies)[0]->geometry->moveForward(-10000.0f)` | 52,528 | +396 |
| 1279 `m_pGeometry6->setPosition((*enemies)[0]->geometry->getPosition())` | 52,529 | +397 |

The wholesale substitution (all 68 occurrences in `process`) is size-short
(56,844) and raw 55,321, so the sites must be chosen individually.

### Data-flow families probed and rejected on the current body

All measured against 52,132, size-exact and `.rel.plt`-identical unless noted.

* **`(float)(N*delta)` -> `(float) delta * N.0f`** (34 sites: `-2,-4,-3,-12,
  -18,-20,2,3,4,5,6,10,12,13,35,200 * delta`).  Best is neutral
  (`(-18*delta)`, `(-20*delta)`); everything else is +45..+3016, and many are
  size-changing (57,224/57,240/57,256) — gate-fatal.  The landed
  `-delta * k` / `delta * -k` cells are already the optimal spelling.
* **Escort route arrays** (lines 3610-3617): converting the remaining
  product forms back to differences is worse — r0x `(r.x-d.x)*9e5` +8,
  r0z `(r.z-d.z)*9e5` +12, r1y `(d.y-r.y)*-9e5` +8, r1z
  `d.z*9e4 - r.z*9e5` neutral, r1x product form +2, r0y product form +8.
  The wave-182/183 differences are the local optimum.
* **`float` vs `int` casts**: `(int)VectorLength(...) <= 49999` ->
  float `< 50000.0f` +44, int `< 50000` neutral; `(int)((float)delta*0.5f)`
  -> `delta/2` size-changing (+1457/+1084); `(int)(transport.z - 200000.0f -
  (float)delta)` -> integer form size-changing (-108); `(int)(timer/6500*30)`
  -> integer multiply size-changing (-4).
* **`long long` comparison factoring**: `m_nScriptTime >= m_nScriptTimerA +
  4000` -> subtract form size-changing (-12); `timerA <= 7000 && timerA +
  delta > 7000` -> `timerA > 7000-delta` size-changing (-24);
  `m_nScriptTime - delta <= 38000` -> `<= 38000+delta` +6;
  `timerA >= 1100-delta` -> `timerA+delta >= 1100` size-changing (+8).
* **`Vector` construction/assignment forms**: combining
  `m_tempVec = X; m_tempVec += V;` into `m_tempVec = X + V;` at all 15 sites
  is worse (+15..+2677, most size-changing); `Vector zero{0,0,0}` vs
  `Vector zero;` is neutral; operand-swapping `X + Vector{…}` at 11 sites is
  neutral or worse (+1..+25); adding `Vector &` aliases in mission-0 cases
  12/13 is neutral, removing the existing `tvA`/`v_dirVecA` aliases is +1/+2.
* **multiply-vs-branch**: `speed = (i==0)?…` -> `if/else` is neutral
  (identical code); the `nextInt(2)==0 ? 1 : -1` sign ternaries -> `if`
  are size-changing (+1641/+1598); `(float)-delta` vs `-(float)delta` at
  lines 1080/1137 is +364..+2075; `1.0f - x/K` vs `x/-K + 1.0f` is neutral
  (compiler canonicalises); `(float)delta * 0.2f * depth` (left-assoc,
  the literal original order) is +3.
* **bool vs int**: `if (m_bRenderParticles)` / `if (field_0xaa/ab)` already
  load bytes and compare; no alternative spelling moves the code.
* **Cached-local substitutions**: `Globals::status->` -> `status->` (196
  sites) is size-exact but +3229; `m_pLevel->getPlayer()` -> `player`
  size-changing (53,488); wholesale `(*m_pLevel->getEnemies())` size-changing
  (56,844).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12903
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52122 aligned= 20678 size= 57232/57232 insn= 20724/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52122 size=57232/57232 text=74581 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Harnesses (`harness.py`, `batch.py`, `combo.py`, `combo2.py`, `combo3.py`,
`align.py`, `align2.py`, `linemap.py`, `vcmap.py`, `dis.py`, `rundiff.py`) and
result tables (`res_*.jsonl`, `cands_*.json`) live in
`/home/debian/GalaxyOnFire2/scratch/w12903/`; they are scratch, not tracked.
