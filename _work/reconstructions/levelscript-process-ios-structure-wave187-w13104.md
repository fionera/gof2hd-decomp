# LevelScript::process — iOS-structure pass 5 (wave 187, w13104)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13104`, branch
`wave187-w13104`, base master `8b5d4281c` (wave-186 harvest, raw **51,968**,
`.text` 74,274).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13104`.

Continuation of wave-183 w12704, wave-184 w12804, wave-185 w12904 and wave-186
w13004 (iOS 1.1.4 structural oracle).  This pass re-diffs the iOS Mach-O
implementation and the Android original, re-tests the remaining structural
divergences, and then runs a fresh distance-1 (adjacent) sweep over the current
51,968 body.  **A size-exact, `.rel.plt`-identical raw win (-7) was found and
landed.**

## Result (code win landed)

```
                         base (51,968)    this pass
process raw diffbytes    51,968           51,950     (-18)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,274           74,256     (-18)
.ARM.extab diffbytes     647              647        (unchanged; size/base exact @0x1a3b78, 156,312 B)
.ARM.exidx diffbytes     0                0
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
file size                2,187,328        2,187,328  (exact)
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

## Landed moves

All three are independent of each other and of the PLT first-reference order
(verified: `.rel.plt` md5 unchanged).

| # | base line | move | raw |
|---:|---:|---|---:|
| 1 | 1368 | `m_nState = 2;` before `PlayerFighter *fighter = …` (mission-106 state 1) | -2 |
| 2 | 1371 | `(*enemies)[0]->setVisible(true);` before `(*enemies)[0]->awake();` | -4 |
| 3 | 1729 | `int routePoints[3] = …;` before `m_pCamera->setPosition(camPos);` (mission-29 intro) | -1 |
| 4 | 2921 | `m_tempVec += m_dirVec * 700.0f;` before `… * 10000.0f;` (two additive terms) | -1 |
| 5 | 4575 | `e->setVisible(false);` before `e->setActive(false);` | -2 |
| 6 | 4579 | `tv = player->getPosition();` before `m_pCamera->setTarget(player->geometry);` | -3 |
| 7 | 4629 | `tvA += Vector{…};` before `m_pCamera->setTarget(player->geometry);` | -1 |
| 8 | 4632 | `forceUpdate(delta,false);` before `m_pCamera->setPosition(tvA);` | -1 |
| 9 | 4675 | `tv += Vector{…};` before `player->setSpeed(0.02f);` | -1 |

The `m_pGeometry6` candidate from the sweep (`m_pGeometry6->setPosition(...);`
before `m_pGeometry6 = intro;`, -1) is a semantic change (it calls setPosition
on the *old* pointer).  The landed form keeps the store first and calls
`intro->setPosition(...)` instead, which is what the original emits
(`13e330 str r5,[r6,#220]` then `13e33c mov r0,r5; setPosition`) and is worth
the same -1.

## iOS 1.1.4 oracle re-check (unchanged conclusions)

`_work/bins/ios_1.1.4_HD` is the stripped armv7 Mach-O.  Re-confirmed from the
wave-183..186 passes:

* The iOS source-layout dump lists exactly the same non-`process` members as
  the Android dynsym.  **No per-state helpers exist in either build.**
* Mission-0 and mission-1 bodies are statement-for-statement identical.
  **No enum value differs**; no state is split/collapsed in mission 0/1.
* The complete `tbh` census (15 in both) isolates exactly two structural
  divergences: mission-0's switch range (`6..15` original vs `3..15` ours —
  clang 7 merges our `else if (m_nState == 3)` into the switch) and mission-145
  (original splits states 0/1 from the `2..5` `tbh`; ours uses one `switch`).

## Measured structural variants (all rejected)

Every variant below was measured on the 51,968 base with the exact NDK r18b
`-Oz` command, gold link and GNU strip.

| variant | raw | size | `.rel.plt` |
|---|---:|---:|---|
| **base** | **51,968** | **57,232** | `289b2a06…` |
| mission-1: `forceUpdate(delta,true)` before `rotate` | 51,971 | 57,232 | identical |
| mission-1: `getPosition` before `setToSleep` | 51,969 | 57,232 | identical |
| mission-1: both reorders | 51,972 | 57,232 | identical |
| `-2400.0f * GetDirVector()` → `GetDirVector() * -2400.0f` | 51,969 | 57,232 | identical |
| `-30000.0f * right` → `right * -30000.0f` | 51,969 | 57,232 | identical |
| both float*Vector → Vector*float rewrites | 51,970 | 57,232 | identical |
| mission-154 state-9 exit: full original emitted order | 52,000 | 57,232 | identical |
| mission-154: move `setTarget` out of `if (field_0xaa)` | 51,987 | 57,232 | identical |
| mission-106 state-1: full original order (`m_nScriptTimerA=0`, `m_nState=2`, `awake`, `setVisible`, `setCloakingPossible`, `setAIDisabled`) | 52,003 | 57,232 | identical |
| mission-0 state-machine source order `3,4,5` | 55,144 | 57,248 (+16) | **differs** |
| mission-0 state-machine source order `4,3,5` (faithful) | 54,989 | 57,240 (+8) | **differs** |
| mission-145 faithful two-switch form (w13004 re-test) | 51,968 | 57,232 | identical |

The mission-0 order fixes are blocked by `.rel.plt`: moving the state-3 block
changes the first-reference order of the imported calls (`relplt` becomes
`a8d49cee…`), so they cannot be landed without a compensating PLT-order change.

## Fresh distance-1 sweep

The wave-186 w13005 d1 sweeps ran on the 52,056 body; the body has since moved
to 51,968 (w13001/w13002/w13007).  A fresh d1 sweep was run over lines
605..2056 of the current body (472 of 1,914 candidates, both directions are the
same permutation).  It found four size-exact, `.rel.plt`-identical winners:
`1368` (-2), `1371` (-4), `1729` (-1), `1893` (-1, semantic form above).
All four compose: the landed combination is **-7** raw bytes.

A second d1 sweep over lines 2056..3214 of the new body (593 of 1,442
candidates) found one more safe winner: `2921` (swap the two additive
`m_tempVec += m_dirVec * {700,10000}.0f;` terms, -1), landed.  Two further
-1 candidates were rejected as semantic changes: `2532` (`camPos.y += 800.0f;`
after `setPosition`, but the original adds all three offsets first,
`13c11e..13c152`) and `2674` (`CameraSetLocal` before `frame->setPosition`,
but the original calls `setPosition` first, `14160c` then `141626`).  The
remaining candidates (3214..5370) were not swept in this pass.

A third d1 sweep over lines 3214..4729 of the new body (849 candidates) found
five more safe winners: `4575` (-2), `4579` (-3), `4629` (-1), `4632` (-1),
`4675` (-1).  They compose to **-10** (better than the sum of the individual
deltas).  Candidate `3734` (`*camPos += *tmp * 400.0f;` after `setPosition`)
was rejected as a semantic change.  The remaining candidates (4729..5370)
were not swept in this pass.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13104
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51950 size=57232/57232 text=74256 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Harnesses (`sweepall.py`, `trymulti.py`, `tryvar.py`, `tryfile.py`, `vcombo3.py`,
`orig.dis`, `ours.dis`) live in `/home/debian/GalaxyOnFire2/scratch/w13104/`;
they are scratch, not tracked.
