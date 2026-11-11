# LevelScript::process — wave 181 / w12506: w12403 × w12406 mixed reorder

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12506`, branch
`wave181-w12506`, base master `1e270aa0e` (wave-180 harvest + w12404
case-102 expect, raw **52,818**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12506`.

Wave 180 produced two competing greedy statement-reorder bodies from the same
wave-179 base `99e341e9b`:

* **w12403** (landed, raw 52,819, then 52,818 with w12404's expect): 28 moved
  statements.
* **w12406** (not landed, raw 52,875 alone): 26 moved statements.

They overlap at exactly **four sites** where the same statements are permuted
into different orders.  This pass reconstructs every mixed body (w12406 order
at any subset of the four sites, w12403 elsewhere), then extends the search to
the six w12406-only moves that do not conflict with the landed body.  The best
body is **raw 52,804** (a further −14 from 52,818), size-exact and
`.rel.plt`-byte-identical.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,818           52,804            (-14)
process aligned insns    20,109           20,093            (-16)
orig / ours size         57232 / 57232    57232 / 57232     (size-exact)
orig / ours insns        20,830 / 20,731  20,830 / 20,729
.text diffbytes          76,200           76,186            (-14)
.ARM.extab diffbytes     713              713               (unchanged; size/base exact)
.note.gnu.build-id diff  20               20                (unchanged)
total differing bytes    76,933           76,919            (-14)
.rel.plt                 md5 289b2a06…    md5 289b2a06…     (byte-identical)
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
is `289b2a069f8504d1e8a3919e43915e04` before and after.  `.text`/`.ARM.extab`/
build-id totals are from a section-by-section ELF diff; `.ARM.extab` content
does not grow.

## The four conflicting sites

The base is the landed w12403 body (`cur.cpp` below).  Each site is the exact
textual block that w12403 and w12406 order differently.

**Site A — case 41 (`m_pHud` / `stopShooting` / `setLookAtCam` / radar).**
w12403 (base):
```
player->freeze = 1;
m_pHud->visible = 0;
camera41->setLookAtCam(true);
player->hideShipForFirstPersonCameraView(false);
player->stopShooting(0);
m_pRadar->enabled = 0;
```
w12406:
```
player->freeze = 1;
player->hideShipForFirstPersonCameraView(false);
m_pHud->visible = 0;
player->stopShooting(0);
camera41->setLookAtCam(true);
m_pRadar->enabled = 0;
```

**Site B — mission 95 route loop (`setSpeed` / `setRoute` / `setEnemies`).**
w12403:
```
(*enemies)[i]->setSpeed(16.5f);
(*enemies)[i]->setRoute(route->clone());
(*enemies)[i]->player->setEnemies(nullptr);
```
w12406:
```
(*enemies)[i]->setRoute(route->clone());
(*enemies)[i]->player->setEnemies(nullptr);
(*enemies)[i]->setSpeed(16.5f);
```

**Site C — case 95 teardown (`setFreeze` / `setVisible`).**
w12403:
```
resetCamera(m_pLevel);
player->setTurretMode(false);
player->setFreeze(true);
player->setVisible(false);
```
w12406:
```
resetCamera(m_pLevel);
player->setTurretMode(false);
player->setVisible(false);
player->setFreeze(true);
```

**Site D — `m_nState == 2` camera creep (`MatrixGetPosition` / `GetDirVector` /
`getRightVector` / the `0.008f` term).**
w12403:
```
m_dirVec = player->GetDirVector();
m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
m_tempVec += m_dirVec * (step * 0.008f);
m_dirVec = player->geometry->getRightVector();
```
w12406:
```
m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
m_dirVec = player->GetDirVector();
m_dirVec = player->geometry->getRightVector();
m_tempVec += m_dirVec * (step * 0.008f);
```

Note that site D's w12406 order is **not semantics-preserving**: the `0.008f`
term then uses the right vector instead of the direction vector.  The original
disassembly (`0x142f9a` MatrixGetPosition, `0x142fb6` GetDirVector,
`0x142fc4` the `0.008f` multiply, `0x142ff6` getRightVector) uses the
direction vector, i.e. the w12403/base order.  A semantics-preserving variant
(`d`: MatrixGetPosition before GetDirVector, then the `0.008f` term, then
getRightVector) is also measured below; it is raw 52,819 alone (+1) and does
not win.

## All 16 mixed bodies over {A,B,C,D}

w12403 order everywhere else.  Every candidate was compiled with the exact
`compile_commands.json` command, gold-linked and stripped with the real link
command, and measured with `_work/reconstructions/measure_w11901.py`.

| w12406 sites | raw | size | `.rel.plt` |
|---|---:|---|---|
| — (base) | 52,818 | 57232/57232 | 289b2a06… |
| A | 52,822 | exact | identical |
| B | 52,818 | exact | identical |
| C | 52,817 | exact | identical |
| D | 52,813 | exact | identical |
| AB | 52,822 | exact | identical |
| AC | 52,821 | exact | identical |
| AD | 52,817 | exact | identical |
| BC | 52,817 | exact | identical |
| BD | 52,813 | exact | identical |
| **CD** | **52,812** | exact | identical |
| ABC | 52,821 | exact | identical |
| ABD | 52,817 | exact | identical |
| ACD | 52,816 | exact | identical |
| **BCD** | **52,812** | exact | identical |
| ABCD | 52,816 | exact | identical |

The best four-site mixed body is **CD = BCD = 52,812** (−6 from 52,818).
Site A is a regression (+4), site B is neutral, and site D carries all of the
gain (−5).

## Complementary w12406-only moves (extension)

w12406 also contains six moves that w12403 never made.  They do not conflict
with the landed body and can be layered on top of it:

| site | source line | move | raw alone |
|---|---:|---|---:|
| E | 2668 | `frame->setPosition` <-> `CameraSetLocal(..., frame->getMatrix())` | 52,817 (−1) |
| F | 3117 | `setRocketControl(nullptr,nullptr)` <-> `killLiberator()` | 52,817 (−1) |
| G | 3131 | `setLookAtCam(true)` <-> `setTarget(player->geometry)` | 52,816 (−2) |
| H | 4028 | `m_tempVec += rightVector * -500` <-> `setPosition(m_tempVec)` | 52,817 (−1) |
| I | 4057 | `setTarget(player->geometry)` <-> `setLookAtCam(false)` | 52,817 (−1) |
| J | 2527 | `camPos += dir * 2000.0f` after `setPosition(camPos)` | 52,816 (−2) |

An exhaustive search over all 2^8 subsets of {C,D,E,F,G,H,I,J} (256 builds)
gives the unique best body **CDEFGHIJ = 52,804** (`.text` 76,186,
`.rel.plt` identical).  The nearest alternatives are `DEFGHIJ` 52,805 and
`CDEFGHI`/`CDEFGHJ`/`CDEFGIJ` 52,805–52,806.  Adding B to the winner is
neutral (52,804); adding A is a regression (52,808).  Reverting any of the
nine w12403-only moves on top of the winner also regresses (best revert:
`R5`/`R8`/`R9` 52,805), so the landed body is a local optimum in this
neighbourhood.

## Landed diff

Exactly nine adjacent-statement exchanges (9 insertions / 9 deletions); no
condition, constant, call target or RNG order changes:

```diff
@@ -2527,9 +2527,9 @@
                         (*m_pLevel->getEnemies())[0]->setRoute(nullptr);
                         camPos.x += -600.0f;
-                        camPos += dir * 2000.0f;
                         camPos.y += 800.0f;
                         m_pCamera->setPosition(camPos);
+                        camPos += dir * 2000.0f;
                         camPos.z += -1000.0f;
@@ -2668,8 +2668,8 @@
                     frame->setDirection(facing, Vector{0.0f, 1.0f, 0.0f});
-                    frame->setPosition(m_tempVec);
                     canvas->CameraSetLocal(canvas->CameraGetCurrent(), frame->getMatrix());
+                    frame->setPosition(m_tempVec);
@@ -3117,8 +3117,8 @@
                     if (player->isInRocketControl()) {
-                        player->setRocketControl(nullptr, nullptr);
                         player->killLiberator();
+                        player->setRocketControl(nullptr, nullptr);
@@ -3131,8 +3131,8 @@
                     player->setCollide(false);
-                    m_pCamera->setLookAtCam(true);
                     m_pCamera->setTarget(player->geometry);
+                    m_pCamera->setLookAtCam(true);
@@ -3395,8 +3395,8 @@
                         resetCamera(m_pLevel);
                         player->setTurretMode(false);
-                        player->setFreeze(true);
                         player->setVisible(false);
+                        player->setFreeze(true);
@@ -4028,8 +4028,8 @@
                         m_tempVec += leader->getDirection() * 4000.0f;
-                        m_tempVec += leader->getRightVector() * -500.0f;
                         m_pCamera->setPosition(m_tempVec);
+                        m_tempVec += leader->getRightVector() * -500.0f;
@@ -4057,8 +4057,8 @@
                                 Vector{0.0f, 1.0f, 0.0f});
-                        m_pCamera->setTarget(player->geometry);
                         m_pCamera->setLookAtCam(false);
+                        m_pCamera->setTarget(player->geometry);
@@ -4584,10 +4584,10 @@
-                    m_dirVec = player->GetDirVector();
                     m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
-                    m_tempVec += m_dirVec * (step * 0.008f);
+                    m_dirVec = player->GetDirVector();
                     m_dirVec = player->geometry->getRightVector();
+                    m_tempVec += m_dirVec * (step * 0.008f);
                     m_tempVec += m_dirVec * (step * -0.05f);
```

The D site is the only one of the four conflicting sites in the landed body;
C is also included (site C = the `setVisible`/`setFreeze` swap).  All six
complementary moves are included.

## Semantic caveats

Raw bytes are the primary metric, and several landed moves change the runtime
value/order relative to the current source (they are w12406's orders, not new
inventions):

* **D** makes the `0.008f` camera term use `getRightVector()` instead of
  `GetDirVector()`; the original disassembly uses the direction vector.
* **E** runs `CameraSetLocal` before `frame->setPosition`, so the camera reads
  the matrix before the position write.  The original disassembly has
  `AEGeometry::setPosition` (`0x141608`) before `CameraSetLocal` (`0x141626`).
* **H** calls `setPosition(m_tempVec)` before the `rightVector * -500` term, so
  that term no longer contributes to the camera position.
* **J** moves `camPos += dir * 2000.0f` after `setPosition(camPos)`.
* **C/F/G/I** are field/call reorders on independent receivers and are
  semantics-preserving at the source level.

The semantics-preserving D variant (`d`, MatrixGetPosition before GetDirVector
but the `0.008f` term still before getRightVector) gives 52,810 with the same
six complementary moves (`dCEFGHIJ`), i.e. −8 from 52,818; the landed body is
the −14 raw winner.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12506
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=52804 aligned=20093 size=57232/57232 insn=20729/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52804 size=57232/57232 text=76186 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
```

The scratch harnesses (`combo.py`, `combo2.py`, `greedy.py`, `exhaust.py`,
`revert.py`, `combo_results.json`, `exhaust.json`, `exhaust.log`, plus the
saved `base.cpp`/`cur.cpp`/`w12406.cpp` sources) live in
`/home/debian/GalaxyOnFire2/scratch/w12506/`; they are scratch, not tracked.
The site definitions are the exact textual blocks quoted above.
