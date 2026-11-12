# LevelScript::process — wave 188 / w13202: branch-layout pass (branch map + adjacent-statement harvest)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13202`, branch `wave188-w13202`, base master
`ce16f45f1` (wave-187 harvest 5, raw **51,930**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13202`.

Continuation of the wave-187 w13102 branch-layout work.  This pass rebuilds the
branch/`tbh` map of both bodies, maps every branch whose target/offset differs
to its source line, and re-sweeps every adjacent pair of independent
statements on the current body.  Four size-exact, `.rel.plt`-identical raw
wins compose.

**Wave-189 rebase (master 630c6962a, 2026-11-12).** The ten reverse
distance-2 rotations from pass 3 apply on top of the wave-189 body (which
already contains the w13201 reorders). Measured on current master: raw
**51,900 -> 51,892 (-8)**, aligned **20,963 -> 21,005**, size 57,232/57,232,
`.rel.plt` byte-identical, `.ARM.extab` content unchanged 639. `.text`
differing bytes **74,032 -> 74,017 (-15)**; total differing bytes
**74,691 -> 74,676**. The numbers below are the original wave-188
measurement (51,930 -> 51,889); the rebase is the landed form.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,930           51,919     (-11)
process aligned insns    20,928           20,951     (+23)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          74,223           74,212     (-11)
.ARM.extab diffbytes     639              639        (size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx diffbytes     0                0          (byte-identical)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.rodata / .data / .bss   0 / 0 / 0        0 / 0 / 0
file size                2,187,328        2,187,328      (exact)
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

## 1. Branch census and target map (both bodies)

`objdump -d` over `0x137d18..0x145ca8`.  The mnemonic multiset is unchanged
from wave 187 (`b` 369/369, `tbh` 15/15, `bx` 8/8; the `cbz`/`cbnz` vs
`beq`/`bne`, `bcs` vs `bcc`, `bge` vs `blt` deltas are the known
comparison-polarity differences spread over the per-mission switch).

An opcode-normalised full-stream alignment (registers erased, branch targets
replaced by target instruction index) still reports **exactly 2** matched
branch pairs with a different relative target offset:

```
0x137f66 bne.n rel -17  |  0x137f60 bne.n rel -16   (intro-ship for-loop, i<3)
0x137f9c cbz   rel +18  |  0x137f9a cbz   rel +19   (getWingmen() != 0 guard)
```

Both are in the shared "active tail" and are the same loop rotated
differently (original condition at the bottom, ours at the top); the body
length difference is one `ldr` saved by reusing r4 for the enemy pointer.
Every other branch target is placed identically, so the fall-through
structure is already correct — the residual is instruction selection /
register allocation, not missing or extra blocks.

The 15 `tbh` tables match in count; the only range difference remains the
mission-0 `switch (m_nState)` (original 10 entries 6..15, ours 13 entries
3..15 because clang 7 SimplifyCFG merges the preceding
`else if (m_nState == 3)` arm), as documented by w12702/w12802.

## 2. Register-allocation observation (not landed)

The original keeps `this` in `sl` and the `__stack_chk_guard` address in
`fp`; ours keeps `this` in `fp` and the guard address in `sl`.  The original
also keeps `&Globals::status` in `r4` and reloads the pointer at every use,
while the baseline body kept a `Status *status` local in `r4` *and*
`&Globals::status` in `r9`.  Removing the local (using `Globals::status->`
at its two remaining uses) and using a `Status *&status` reference were both
measured: raw 51,931 (+1) and 51,931 (+1) respectively, size-exact and
`.rel.plt`-identical.  The swap is a greedy-allocator ordering consequence,
not a source-level difference, so both variants were reverted.

## 3. Landed change — adjacent independent-statement harvest

A full sweep of every adjacent pair of independent single-line statements in
`process` (1,197 measured candidates, one compile+gold-link+strip per
candidate, `ulimit -v 12000000`, `-j2`) found 10 raw wins.  Six were rejected:
three move a compound-assignment target across a reader
(`camPos += …` / `camPos.y += …` / `*camPos += …` before `setPosition`) and
one moves `delete frame` across `frame->setPosition`, i.e. they are not
semantics-preserving.  The four independent, semantics-preserving wins
compose additively:

| source lines (base) | move | raw |
|---|---|---:|
| 1240-1241 | `m_nState = 12;` before `m_pLevel->lodManager->forceUpdate(30, false);` | -4 |
| 1276-1278 | `m_pGeometry6->setDirection(...)` before `(*m_pLevel->getEnemies())[0]->geometry->moveForward(-10000.0f);` | -3 |
| 1912-1913 | `m_bCinematicBreak = 0;` before `player->setComputerControlled(false);` | -1 |
| 2954-2955 | `(*enemies)[1]->setPosition(...)` before `(*enemies)[0]->setPosition(ref + ...);` | -3 |

Composed: raw **51,930 -> 51,919** (-11), `.text` 74,223 -> 74,212,
size-exact 57,232, `.rel.plt` byte-identical, `.ARM.extab` unchanged (639,
156,312 B).  Each pair touches disjoint state (a flag store vs a LOD update;
two different enemy geometries; a flags byte vs a player control call; two
different enemies), so the reorders are semantics-preserving.

The overlapping `player->setFreeze(false) / setVisible(true) /
player->player->setVulnerable(true)` run (source 3344-3346) gives -1 in
isolation but does not compose with the four moves above: every permutation
that moves the first two statements grows the function to 57,236 B
(gate-fatal), and the only size-exact permutation (`setVulnerable` first)
measures 51,922, i.e. +3 worse than the landed body.  It was dropped.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13202
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51919 aligned= 20951 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`bmap.py`, `opalign.py`, `stmts.py`, `dep.py`, `sweep.py`,
`measure.py`, `apply2.py`, `apply3.py`, `matchruns.py`, `winlines2.py`) live
in `/home/debian/GalaxyOnFire2/scratch/w13202/`; they are scratch, not tracked.

## Pass 2: distance-2 rotation harvest (same branch map)

Re-ran the statement sweep on the improved 51,919 body at distance 2
(`[A,B,C] -> [B,C,A]`, 938 measured candidates; the dependency model was
tightened to treat compound assignments (`+=`, `-=`, …) and `delete X` as
writes of their target, and a move is admissible only if the moved statement
is independent of *every* statement it crosses).  Eight independent,
semantics-preserving rotations compose:

| source lines (51,919 body) | rotation | raw |
|---|---|---:|
| 760-762 | `Vector zero{}` moved after `Vector pos = …; m_nScriptTimerA += delta;` | -3 |
| 2281-2283 | `setLookAtCam(false)` moved after `setTarget` + `setComputerControlled(false)` | -1 |
| 2296-2298 | `(*enemies)[0]->field_0x24 = 0;` moved after `routePoints`/`new Route` | -1 |
| 2337-2339 | `m_bCinematicBreak = 1;` moved after `setTarget` + `setPosition` | -3 |
| 3188-3190 | `(*enemies)[3]->setActive(false);` moved after `setVisible` + `setVulnerable` | -1 |
| 3345-3347 | `player->setVisible(true);` moved after `setVulnerable` + `setLookAtCam(false)` | -1 |
| 3540-3542 | `wcamera->setTarget(...)` moved after `m_nScriptTimerA = 0` + `setPosition(...)` | -3 |
| 3552-3554 | `m_nScriptTimerA += delta;` moved after `Vector zero{}` + `Vector pos = …` | -1 |

Composed:

```
                         after pass 1     after pass 2
process raw diffbytes    51,919           51,905     (-14)
process aligned insns    20,951           20,988     (+37)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,212           74,198     (-14)
.ARM.extab diffbytes     639              639        (size/base exact, 156,312 B)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

Rejected distance-2 cells (all measured, all semantics-changing):
`m_tempVec += …` moved past `setPosition(m_tempVec)` (51,918),
`updateReferenceMatrix` moved past `translate` (51,916; the original emits
`updateReferenceMatrix` *before* `translate` at `0x13c610`/`0x13c634`, so the
move contradicts the binary even though it lowers the raw count), and the
`player->setFreeze/setVisible/setVulnerable` run (all size-exact rotations
except one measure ≥ +3 on the composed body).

Cumulative for the whole pass: raw **51,930 -> 51,905** (-25), `.text`
74,223 -> 74,198, size-exact 57,232, `.rel.plt` byte-identical, `.ARM.extab`
unchanged (639, 156,312 B).

## Pass 3: reverse distance-2 rotations (same branch map)

The distance-2 sweep above only tested `[A,B,C] -> [B,C,A]` (move A after C).
The opposite rotation `[A,B,C] -> [C,A,B]` (move C before A) was then swept
separately on the 51,905 body (792 measured candidates; a move is admissible
only if the moved statement is independent of every statement it crosses).
Ten independent, semantics-preserving rotations compose:

| source lines (51,905 body) | rotation | raw |
|---|---|---:|
| 719-721 | `sound->stop(player->field_0x1c)` before `Vector zero158{}` | -3 |
| 827-829 | `player->setPosition(0,0,0)` before `switchPlanetForIntro()` | -1 |
| 889-891 | `m_nScriptTimerA = 0` before `m_tempVec += …` | -1 |
| 1238-1240 | `m_nState = 12` before `m_tempVec.y += 500.0f` | -1 |
| 1615-1617 | `m_pCamera->translate(1000,700,1000)` before `setTarget` | -3 |
| 1962-1964 | `setComputerControlled(false)` before `m_pHud->visible = 1` | -2 |
| 2075-2077 | `m_pCamera->translate(3000,1000,2000)` before `setPosition` | -1 |
| 2969-2971 | `player->killLiberator()` before `setRocketControl(nullptr,nullptr)` | -1 |
| 3088-3090 | `player->setFreeLookMode(true)` before `setRotationAroundTarget(true)` | -1 |
| 3346-3348 | `player->setVisible(true)` before `camera->setLookAtCam(false)` | -1 |

Composed:

```
                         after pass 2     after pass 3
process raw diffbytes    51,905           51,889     (-16)
process aligned insns    20,988           21,020     (+32)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,198           74,182     (-16)
.ARM.extab diffbytes     639              639        (size/base exact, 156,312 B)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

Cumulative for the whole pass: raw **51,930 -> 51,889** (-41), `.text`
74,223 -> 74,182, size-exact 57,232, `.rel.plt` byte-identical, `.ARM.extab`
unchanged (639, 156,312 B).
