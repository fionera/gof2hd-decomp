# Wave 163 merge tests (20 workers, 2026-11-08)

Base: master `e51db2fc7` (wave-162 final).  Every worker ran in its own
worktree `/home/debian/GalaxyOnFire2/wt/w109NN` with scratch
`/dev/shm/w109NN` and the standard `codex exec -p dsv41` command line.
Gate on entry: `byte_exact 4507/4524`, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.text` 85,042, `.ARM.extab` 725, `.rel.plt` 0, file size exact.

## Landed (3 source commits)

| worker | target | change | raw | aligned | size |
|---|---|---|---:|---:|---:|
| w10901 | `LevelScript::process` | +24 B companion for `L1364:0`/`L1389:1` (`expect(m_nState==2,0)`, `expect(m_nState==5,1)`, `m_bCinematicBreak=0`, setTarget-first) | 54,698 -> **54,541** (-157) | 17,456 -> **17,427** (-29) | 57,232 exact |
| w10917 | `Hud::draw` | volatile `Globals::status` reload in the damage block; drop the block-4 null test | 1,227 -> **1,015** (-212) | 248 -> **229** (-19) | 9,332 exact |
| w10919 | `MGame::OnUpdate` | `Level *level = this->level` local forces the GOT base into r9 | 37 -> **16** (-21) | 109 -> **6** (-103) | 11,440 exact |

Combined `.text` **85,042 -> 84,652** (-390), exactly the sum of the three
per-function raw wins.  `.rel.plt` stayed byte-identical (0), `.ARM.extab`
stayed 725, all section sizes exact.  `byte_exact` stayed 4507 (no function
reached 0 diff).  `check` green.

Note: w10901's own note reports raw "54,698 (0)"; the measured post-merge
value is 54,541.  The per-worker claim was stale/wrong; the commit is a
genuine -157.

## Measured, not landed

| worker | target | best shape | raw | aligned | size | why not landed |
|---|---|---|---:|---:|---:|---|
| w10902 | LevelScript | mission-0 `4,3,5` swap + 10-entry `tbh` | 54,977 | 17,608 | exact | raw +279 / aligned +181 regression; structure matches (16 `tbh` -> 15) but layout worse |
| w10903 | LevelScript | second `0.8f` load hoisted PLT-safe | 54,698 | 17,456 | **57,264 (+32)** | +32 B block; needs a -32 B companion.  Note `39c034629` was committed in the worker tree, not on master |
| w10906 | Layout C1/C2 | explicit `iPadHD/n9/else` chain for first two fields | 5,695 claimed / **5,705 measured** | 2,083 | 7,072 exact | claimed -10 raw did not reproduce on master; commit `3ccd90ce3` reverted |
| w10907 | Layout C1/C2 | `0x8c..0xa8` contiguous run | 5,705 | 2,082 | 7,072 exact | all shapes raw-neutral or worse |
| w10908 | Radar::draw | outer-scope `alwaysEnemy` + `expect(field_0x1bc>=1,1)` | 7,584 claimed / **8,124 measured** | 1,420 claimed | 9,908 exact | claimed -540 raw did not reproduce; commit `aab4acfad` reverted |
| w10909 | Radar::draw | `campaign` -> `sl` via hot-guard expects | 9,611 | worse | 9,908 exact | structural regression (`.text` 86,529, extab 754) |
| w10910 | MGame::OnTouchEnd | cold `initHudMenu` block | 4,575 | 1,373 | 8,080 exact | aligned +467 vs base 906; raw -1,241 but layout worse |
| w10911 | MGame::OnTouchEnd | `bool Mission::hasWon` kept | 5,742 | 1,109 | 8,080 exact | raw +1,615 / aligned +203; commit `17ab74903` reverted |
| w10912 | createMission | `Reassociate` `and`-operand sweep | 5,454 | 786 | 17,172 exact | no size-exact flip; best candidate 5,684/789 |
| w10913 | createMission | 4-byte stack-slot permutation | 5,454 | 786 | 17,172 exact | frame map analysed; no size-exact flip |
| w10914 | createShip | original case order | **1,560** (-3) | 460 (+11) | 2,764 exact | raw win but aligned +11; held for a combined pass |
| w10915 | Status | 4/3/2 source progression | 722 | 320 | 1,432 exact | source weights cannot reach the plateau (see w10920); `expect(getType(),167)` gives 1,277/632/1,392 |
| w10916 | HangarWindow::render | size-exact `tab==0` `cbz` | 2,514 | 485 | 9,700 exact | `.ARM.extab` 725 -> 1,250; commit `002f2c997` reverted |
| w10918 | small tail | new shape sweep | unchanged | unchanged | exact | no win |
| w10920 | build provenance | profile harness round 4 | n/a | n/a | n/a | sample PGO cannot weight the empty-`br` Status switch successors; instrumented PGO moves non-switch layout away |

## Measurement anomaly resolved

The handoff flagged `MGame::OnTouchEnd` as measuring 5,742 on every tree
instead of the documented 4,127.  Cause: the check used the wrong mangled
name (`_ZN5MGame11OnTouchEndEv`, the 0-arg overload).  The real target is
`_ZN5MGame10OnTouchEndEiiPv`; it measures **4,127** at HEAD, at
`29248c1d0` and at `e51db2fc7`.  No doc correction was needed.

## Merge lesson

Several per-worker claims did not reproduce on master (w10906, w10908,
w10916's extab cost, w10901's raw).  Always re-measure a cherry-pick in
both orders before landing; a worker's "landed" note is a claim, not a
measurement.
