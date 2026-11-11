# Wave 167 merge tests (20 workers, 2026-11-11)

Base: master `d00057e6d` (waves 165-166 docs).  Every worker ran in its own
worktree `/home/debian/GalaxyOnFire2/wt/w112NN` with scratch `/dev/shm/w112NN`,
`-j2` builds and a 12 GB per-agent `ulimit -v` (the wave-165 OOM mitigation).
Gate on entry: `byte_exact 4507/4524`, linked 4507, size 4524/4524, data
843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370, `.text` 82,898,
`.ARM.extab` 660, `.rel.plt` 0, file size exact.

## Landed

| worker | target | change | raw | aligned | size |
|---|---|---|---:|---:|---:|
| w11201 | `LevelScript::process` | 11 more `__builtin_expect` cells (498-cell operand + 772-cell full-if + 486-cell re-sweep) | 54,095 -> **54,031** (-64) | 17,621 -> **17,764** (+143) | 57,232 exact |
| w11212 | `MGame::OnTouchEnd` | 22<->74 and 43<->65 reward-chain swaps | 4,082 -> **3,775** (-307) | 1,018 -> **1,454** (+436) | 8,080 exact |
| w11218 | `Level.cpp` | per-TU `-mllvm -enable-earlycse-memssa=false` | 5,436 -> **5,435** (-1) | 767 | 17,172 exact |

Combined `.text` **82,898 -> 82,526** (-372), `.ARM.extab` **660 -> 670**
(+10), `.rel.plt` byte-identical, all section sizes exact.  `byte_exact`
stayed 4507.  `check` green.

## Measured, not landed

| worker | target | best shape | raw | aligned | size | why not landed |
|---|---|---|---:|---:|---:|---|
| w11202 | LevelScript | call-shape note: 3 `setPosition` overload bugs pinned (4604 fff->Vector is -4 B, needs +4 compensation) | 54,095 | 17,621 | exact | needs a size companion |
| w11203 | LevelScript | mission-0 switch order + state-16 block | 54,095 | 17,621 | exact | no landed win |
| w11204 | LevelScript | size-exact `m_pLevel` alias shapes | 54,095 | 17,621 | exact | no raw win |
| w11205 | LevelScript | mission-78 dead-tail reconstruction (A/B: +20/+140 B) | 54,095 | 17,621 | exact | no size-neutral shape |
| w11206 | Radar | campaign/sl next-wave list | 7,487 | 1,454 | exact | no source win |
| w11207 | Radar | residual branch-layout analysis | 7,487 | 1,454 | exact | no source win |
| w11208 | Layout | first-half store-map re-measure | 5,658 | 2,280 | exact | no size-exact raw win |
| w11209 | Layout | multi-way device-dispatch tail (explicit blocks) | 5,863 | 2,652 | 7,008 | head is 198 B long |
| w11210 | createMission | guard `icmp` de-sinking sweep | 5,436 | 767 | exact | no source win |
| w11211 | createMission | spill-slot decode + scope/lifetime sweep | 5,436 | 767 | exact | no source win |
| w11213 | HangarWindow | cleanup-scope construct sweep | 2,245 | 620 | exact | no win |
| w11214 | createShip | faithful jitter `X,Y,Z` order | 1,560 | 455 -> 460 | exact | raw unchanged, aligned worse |
| w11215 | Hud | dock/show/damage RA sweep | 762 | 222 | exact | no raw win |
| w11216 | Status | gap labels, `(type,flag)` pair, rank if-chain | 722 | 320 | 1,432 | all fail size/raw |
| w11217 | small tail | five-residual re-sweep | unchanged | unchanged | exact | no source win |
| w11219 | build | reconstruct CMakeLists/Android.mk; refute hidden visibility + version script | - | - | - | note only |
| w11220 | build | branch-layout dump for LevelScript/Radar; weights are profile-shaped | - | - | - | note only |

## Merge lessons

* w11201's docs commit had already rewritten the wave-165/166 section; the
  coordinator rewrote it again with the combined numbers.  Always re-measure
  after cherry-picking a docs commit.
* w11212's MGame::OnTouchEnd swap is the biggest single-function raw win of
  the wave (-307) but costs +436 aligned and +12 `.ARM.extab`; raw is the
  byte-identicality ground truth.
* w11218's Level.cpp flag is independent of the source changes and combines
  cleanly.
* The aligned total rose to 25,680 (+579) while raw fell by 372: the
  MGame/LevelScript raw wins trade aligned instructions for byte identity.
