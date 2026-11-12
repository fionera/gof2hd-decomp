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
process raw diffbytes    51,968           51,961     (-7)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,274           74,267     (-7)
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
All four compose: the landed combination is **-7** raw bytes.  The remaining
candidates (2056..5370) were not swept in this pass.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13104
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51961 size=57232/57232 text=74267 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Harnesses (`sweepall.py`, `trymulti.py`, `tryvar.py`, `tryfile.py`, `vcombo3.py`,
`orig.dis`, `ours.dis`) live in `/home/debian/GalaxyOnFire2/scratch/w13104/`;
they are scratch, not tracked.
