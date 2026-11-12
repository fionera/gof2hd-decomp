# LevelScript::process — iOS-structure + data-flow pass (wave 189, w13309)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13309`, branch
`wave189-w13309`, base master `247466fde` (wave-188 harvest, raw **51,917**,
`.text` 74,047, `.ARM.extab` 639).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13309`.

Continuation of the wave-183..188 iOS-structure passes (w12704..w13204) and the
wave-182..188 data-flow passes.  This pass re-diffs the iOS 1.1.4 Mach-O and the
original source-layout dumps, checks the operator-overload call census against
the original, and runs a fresh **comparison-operand swap sweep** over the whole
`process` body (a data-flow family not swept before: only one compare site had
ever been measured).

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,917           51,899     (-18)
process aligned insns    20,935           20,935
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          74,047           74,029     (-18)
.ARM.extab diffbytes     639              639        (size/base exact @0x1a3b78, 156,312 B)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
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

## Landed changes

### A. Three comparison-operand swaps (-3)

| # | source line | before | after |
|---:|---:|---|---|
| 1 | 1325 | `for (unsigned int i = 3; i < enemies->count; i++)` | `for (unsigned int i = 3; enemies->count > i; i++)` |
| 2 | 2581 | `getEmpPoints() < getMaxEmpPoints()` | `getMaxEmpPoints() > getEmpPoints()` |
| 3 | 4881 | `for (unsigned int i = 0; i < enemies->count; i++)` | `for (unsigned int i = 0; enemies->count > i; i++)` |

All are strict comparisons of pure integer operands; swapping the operands and
the operator is semantics-preserving (no side effects, no NaN).  Measured on
the wave-188 body: each alone is raw **51,917 -> 51,916** (-1); the three
compose to **51,914** (-3), size 57,232/57,232 exact, `.rel.plt` unchanged.

### B. `forceUpdate(30,false)` / `m_nState = 12` swap (-4)

Source lines 1240-1241 (`case 12` entry): the wave-188 literal-pool pass had
landed `forceUpdate(30,false); m_nState = 12;`.  On the wave-189 body the
opposite order (`m_nState = 12; forceUpdate(30,false);`) measures raw
**51,914 -> 51,910** (-4), size-exact.  The two statements are independent
(`forceUpdate` reads no `m_nState`), so the reorder is safe.

### C. Five independent statement-order reverts (-9)

Re-testing the wave-188 landed statement swaps on the current body showed
five of them are now raw regressions; reverting each is a win.  All five are
reorders of statements with disjoint receivers:

| site | reverted order (now) | raw effect alone |
|---|---|---:|
| 2954 | `enemies[1]->setPosition(...)` before `enemies[0]->setPosition(...)` (reads `enemies[3]`) | -3 |
| 4631 | `m_pCamera->setTarget(player->geometry)` before `tvA += Vector{...}` | -2 |
| 4633 | `m_pLevel->lodManager->forceUpdate(delta,false)` before `m_pCamera->setFixed(false)` | -2 |
| 4677 | `m_nState++` before `player->setSpeed(0.02f)` | -2 |
| 4679 | `m_pLevel->lodManager->forceUpdate(delta,false)` before `camera->setPosition(tv)` | -1 |

Combined with A+B this gives raw **51,901** (-16 from 51,917), size
57,232/57,232 exact, `.rel.plt` unchanged, `.text` 74,031.

### D. Fresh distance-1 adjacent-statement sweep (-2 more)

A fresh d1 sweep over all 1,722 adjacent single-line statement pairs on the
wave-188 body (`scratch/w13309/sweep_d1.py`, ~1.5 s/candidate) found 9 raw
wins.  Seven were the already-landed A/B sites or semantically unsafe
(`camPos`/matrix read-after-write) pairs.  Two more are safe reorders of
statements with disjoint receivers and are landed:

| site | reverted order (now) | raw effect alone |
|---|---|---:|
| 5105 | `m_pGeometry2->setVisible(false)` before `m_pGeometry3->setVisible(true)` | -1 |
| 5312 | `(*enemies)[0]->field_0x140 = !…` before `m_nScriptTimerB = 0` | -1 |

Both compose with A+B+C: final raw **51,899** (-18 from 51,917), size
57,232/57,232 exact, `.rel.plt` unchanged, `.text` 74,029.

## Final section state (harvested on master `06aa50e0d`)

```
.text diffbytes          74,009
.ARM.exidx               0
.ARM.extab               639   (size/base exact @0x1a3b78, 156,312 B)
.note.gnu.build-id       20
total                    74,668
file size                2,187,328 / 2,187,328
.rodata/.data/.bss       0 / 0 / 0
.rel.dyn/.dynsym/.dynstr/.hash  0 / 0 / 0 / 0
.rel.plt                 md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
byte_exact               4507
linked_exact             4507
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN
drift                    0
symbol parity            5370/5370
```

Two further wave-188 reverts were measured but not landed because they are
semantically unsafe or neutral/worse:
* line 2674 `frame->setPosition(m_tempVec)` / `CameraSetLocal(..., frame->getMatrix())`
  — reverting is -1 but `CameraSetLocal` reads the matrix that `setPosition`
  writes, so the current order is the faithful one;
* line 5303 `turnEnemy()` position permutations — +1 / neutral.

## Operator-overload call census (new structural evidence)

Direct `bl`/`blx` targets in the process body are directly comparable between
the original and ours (both link every function at its original address and
the `.plt` is byte-identical).  The census shows the two scalar/vector
multiply overloads are the only *structural* call-count mismatch:

| callee | original | ours |
|---|---:|---:|
| `AEMath::operator*(const Vector&, float)` (`0x5ec74`) | 29 | 26 |
| `AEMath::operator*(float, const Vector&)` (`0x62658`) | 1 | 4 |

The original's single `float * Vector` call is at `0x13c2f6`, immediately
after `AEMath::VectorNormalize(player->getPosition())` and before the
`m_tempVec = …` store — i.e. source line 2153
(`m_tempVec = 5000.0f * VectorNormalize(...)`).  The other three
`float * Vector` sites in our source should be `Vector * float`:

* line 2504 `camPos += 3000.0f * dir;` — original `0x139a66` calls `0x5ec74`
  (`Vector,float`) with the scalar in `r2`, i.e. `dir * 3000.0f`;
* line 3821 `geo->translate(-30000.0f * right);` — original `0x142b86` calls
  `0x5ec74` with `r2 = -30000.0f`, i.e. `right * -30000.0f`;
* line 3181 `… - 2400.0f * player->GetDirVector()` — original `0x143b90`
  calls `0x5ec74` with `r2 = 2400.0f`, i.e. `player->GetDirVector() * 2400.0f`.

All 8 combinations of these three corrections were measured: **raw is exactly
51,917 in every case** (size-exact, `.rel.plt` identical).  The call-target
bytes are already mismatched at the shifted addresses, so the correction is
structurally right but metric-neutral; it is recorded here rather than landed
alone.  (The wave-187 w13103 change at line 2504 is therefore the wrong
overload; it is kept only because reverting it is also metric-neutral.)

## Comparison-operand swap sweep (method)

`scratch/w13309/cmp_swap.py` parses every top-level `a < b`, `a > b`,
`a <= b`, `a >= b`, `a == b`, `a != b` in the `process` body (balanced
operand scan, casts and `->` chains handled), rewrites it to
`b <op'> a` (`<`↔`>`, `<=`↔`>=`, `==`/`!=` unchanged), and measures it with
the exact NDK r18b `-Oz` compile + gold link (`scratch/w13309/fast.py`,
~1.5 s/candidate).  All 595 sites were measured; the three landed sites are the only wins.
Results are in `scratch/w13309/cmp_res.json`.

Follow-up operand-order sweeps on the same body (all size-exact, base 51,917):

* **logical `&&`/`||` operand swap** (112 sites, `logical_swap.py`): one raw
  -1 candidate at line 4291 (`player->getRoute() != 0 && …getWaypoint(0)->state`),
  but it is **not landed**: the swap reverses the short-circuit order so the
  null check no longer guards the dereference.  The original (`0x1395ba`
  `getRoute` / `cmp 0` / `0x1395c6` `getRoute` / `getWaypoint`) has the safe
  order, so the -1 is a branch-displacement artefact, not a faithful change.
* **arithmetic `+`/`*` operand swap** (85 unambiguous sites, `arith_swap.py`):
  no wins (all 51,917).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13309
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51915 aligned= 20935 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

## Harvest on wave-189 master (2026-11-12)

The branch was rebased onto master `06aa50e0d` (after w13201/w13202 and the
w13309 tvA swap). Each candidate was re-measured individually on that body:

| candidate | raw alone |
|---|---:|
| `enemies[1]` before `enemies[0]` `setPosition` | -3 |
| `m_nState++` before `player->setSpeed(0.02f)` | -1 |
| `m_pGeometry2` before `m_pGeometry3` `setVisible` | -1 |
| `field_0x140` toggle before `m_nScriptTimerB = 0` | -1 |
| `m_nState = 12` after `m_tempVec.z` | +1 (rejected) |
| `camera->setPosition(tv)` before `forceUpdate` | +1 (rejected) |

The `m_pCamera->setTarget` before `tvA +=` swap and the
`forceUpdate`/`setFixed(false)` and `forceUpdate`/`camera->setPosition`
orders were already present on master.  Combined landed result: raw
**51,890 -> 51,884** (-6), aligned 21,007 -> 21,008, `.text`
74,015 -> 74,009, total 74,674 -> 74,668.
