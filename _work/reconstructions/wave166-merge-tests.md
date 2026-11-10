# Waves 165-166 merge tests (20 workers each, 2026-11-08 / 2026-11-10)

Base for wave 166: master `85c1f88df` (wave 165's two landed wins).  Every
worker ran in its own worktree `/home/debian/GalaxyOnFire2/wt/w111NN` with
scratch `/dev/shm/w111NN` and the standard `codex exec -p dsv41` command line.
Wave 165 was interrupted twice by a host OOM (5 clang processes reached
27-306 GB RSS; see the dmesg note in the wave-166 docs commit).  Wave 166
re-ran the same 20 tasks with `-j2` builds and a 12 GB per-agent `ulimit -v`.

Gate on entry (wave 166): `byte_exact 4507/4524`, linked 4507, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370, `.text` 83,147, `.ARM.extab` 659, `.rel.plt` 0, file size exact.

## Landed

| worker | target | change | raw | aligned | size |
|---|---|---|---:|---:|---:|
| w11103 | `LevelScript::process` | 13 more `__builtin_expect` cells (hot/cold sweep, size-compensating pairs) | 54,247 -> **54,095** (-152) | 17,532 -> **17,621** (+89) | 57,232 exact |
| w11106 | `Radar::draw` | `expect(isDockingToPlanet(), 1)` on the planet-lock blip skip | 7,549 -> **7,546** (-3) | 1,446 -> **1,460** (+14) | 9,908 exact |
| w11107 | `Radar::draw` | sound-ladder sub-guard expects + `notTurretMode`/`notOtherLock` decl order | 7,546 -> **7,489** (-57) | 1,460 -> **1,445** (-15) | 9,908 exact |
| w11109 | `Layout::Layout` | `field_0x280` n9/else branch order | 5,688 -> **5,685** (-3) | 2,108 -> **2,120** (+12) | 7,072 exact |
| w11108 | `Layout::Layout` | `field_0x2c_rowHeight` before `0x34`; swap `0x34`/`0x44` stores | 5,685 -> **5,658** (-27) | 2,120 -> **2,280** (+160) | 7,072 exact |
| w11112 | `MGame::OnTouchEnd` | reward-chain 96/127 <-> 144 swap | 4,087 -> **4,082** (-5) | 917 -> **1,018** (+101) | 8,080 exact |
| w11118 | `Radar.cpp` | per-TU `-mllvm -disable-postra-machine-licm` | 7,489 -> **7,487** (-2) | 1,445 -> **1,454** (+9) | 9,908 exact |

Wave 165 also landed (already in the base):
`Radar::draw` 7,578 -> 7,549 (w11106) and `MGame::OnUpdate` 16 -> 15
(w11117, aligned 6 -> 111).

Combined `.text` **83,147 -> 82,898** (-249), `.ARM.extab` **659 -> 660**
(+1), `.rel.plt` byte-identical, all section sizes exact.  `byte_exact`
stayed 4507.  `check` green.

## Measured, not landed

| worker | target | best shape | raw | aligned | size | why not landed |
|---|---|---|---:|---:|---:|---|
| w11101 | LevelScript | PLT call-histogram census + 0.8f sizing | 54,247 | 17,532 | exact | no source win |
| w11102 | LevelScript | mission-switch physical order already matched; mission-0 4,3,5 re-measured | 54,247 | 17,532 | exact | no source win |
| w11104 | LevelScript | callee-saved map (`Status*&`, direct, alias probes) | 54,247 | 17,532 | exact | no raw win |
| w11105 | LevelScript | callee inlining histogram; mission-78 dead tail | 54,247 | 17,532 | exact | analysis only |
| w11110 | createMission | `SinkCmpExpression` is the `and`-operand gate | 5,436 | 767 | exact | no source win |
| w11111 | createMission | frame map + phi/block/decl-order sweep | 5,436 | 767 | exact | no source win |
| w11113 | HangarWindow | cleanup-scope construct sweep | 2,245 | 620 | exact | no raw win |
| w11114 | createShip | faithful jitter `X,Y,Z` order | 1,560 | 455 -> 460 (+5) | exact | raw unchanged, aligned worse |
| w11115 | Hud | dock/show order sweep | 762 | 222 | exact | no raw win |
| w11116 | Status | switch-weight producers | 722 | 320 | 1,432 | ratio < 2 unreachable from source |
| w11117 | small tail | five residual re-sweep | unchanged | unchanged | exact | no new win |
| w11118 | build | per-TU flag sweep (5,712 cells) | -8 | - | - | LevelScript knob dropped (size 57,228 with w11103 source) |
| w11119 | build | CMake r18b, uniform `-Oz`/`-g2`, per-TU link order | - | - | - | note only |
| w11120 | build | Status 4/3/2 plateau unreachable from source | - | - | - | note only |

## Merge lessons

* Per-worker flag claims must be re-measured after the source changes land:
  w11118's LevelScript `-machine-sink-split-probability-threshold=0` was
  size-neutral on the old source but **-4 B** on w11103's source (57,228),
  which is gate-fatal because `LevelScript::process` is its own `.text`
  section.  The flag was dropped; the Radar LICM knob survives.
* w11106 + w11107 combine better than either alone on Radar (7,546 then
  7,489).
* w11109 + w11108 combine exactly (5,685 - 27 = 5,658).
* The aligned total rose to 25,101 (+493) while raw fell by 1,819: several
  raw wins (Layout +172, MGame +101, LevelScript +89) trade aligned
  instructions for byte identity.  Raw/`.text` is the byte-identicality
  ground truth; aligned is indicative.
