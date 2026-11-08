# Wave 164 merge tests (20 workers, 2026-11-08)

Base: master `b3d3a1f68` (wave-163 docs).  Every worker ran in its own
worktree `/home/debian/GalaxyOnFire2/wt/w110NN` with scratch
`/dev/shm/w110NN` and the standard `codex exec -p dsv41` command line.
Gate on entry: `byte_exact 4507/4524`, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.text` 84,652, `.ARM.extab` 725, `.rel.plt` 0, file size exact.

## Landed (10 source changes + 4 provenance notes)

| worker | target | change | raw | aligned | size |
|---|---|---|---:|---:|---:|
| w11002 | `LevelScript::process` | 13 PLT-safe `__builtin_expect` cells (whole-function re-sweep) | 54,541 -> **54,247** (-294) | 17,427 -> **17,532** (+105) | 57,232 exact |
| w11019 | `LevelScript.cpp` | per-TU `-mllvm -disable-postra-machine-sink` matching knob | 54,541 -> **54,539** (-2) | 17,427 | 57,232 exact |
| w11008 | `Radar::draw` | `expect` on the `field_0x1bc` sound-ladder sub-guards + faithful tail | 8,124 -> **7,578** (-546) | 1,320 -> **1,428** (+108) | 9,908 exact |
| w11005 | `Layout::Layout` | explicit `iPadHD/n9/else` chain for the first two fields | 5,705 -> **5,695** (-10) | 2,082 -> **2,083** (+1) | 7,072 exact |
| w11006 | `Layout::Layout` | five tail store-order relocations | 5,695 -> **5,688** (-7) | 2,083 -> **2,108** (+25) | 7,072 exact |
| w11010 | `Level::createMission` | `expect(comingFromAlienWorld,0)` flips the StackSlotColoring weight order | 5,454 -> **5,436** (-18) | 786 -> **767** (-19) | 17,172 exact |
| w11011 | `Mission::hasFailed` | bool field + bool return | 4,127 -> **4,125** (-2) | 906 -> **905** (-1) | 8,080 exact |
| w11012 | `MGame::OnTouchEnd` | swap the 100/110 reward-chain blocks | 4,125 -> **4,087** (-38) | 905 -> **917** (+12) | 8,080 exact |
| w11013 | `HangarWindow::render` | volatile `scrollOffset` LSDA sink | 2,514 -> **2,245** (-269) | 485 -> **620** (+135) | 9,700 exact |
| w11015 | `Hud::draw` | volatile status reload at the `hitFlags & 2/0x24` boundary; `barYOffset` after the show-bar `String` | 1,015 -> **762** (-253) | 229 -> **222** (-7) | 9,332 exact |
| w11014 | `Level::createShip` | original case order `0x31,0x2c,0x33` + jitter call order `Y,X,Z` | 1,563 -> **1,560** (-3) | 449 -> **455** (+6) | 2,764 exact |
| w11017 | `Generator::createMission` | `volatile int bonus` + local `b` | 162 -> **127** (-35) | 64 -> **107** (+43) | 1,996 exact |

Combined `.text` **84,652 -> 83,177** (-1,475), `.ARM.extab` **725 -> 659**
(-66), `.rel.plt` byte-identical, all section sizes exact.  `byte_exact`
stayed 4507 (no function reached 0 diff).  `check` green.

Provenance notes landed from w11018 (toolchain), w11019 (flag space),
w11020 (link recipe): NDK r18b clang 7.0.2, gold 1.12, binutils 2.27
`strip --strip-all`, API 16, C++11, `-Oz`, full `-g`, r18b CMake toolchain.
Every non-code ELF byte is identical; the ground truth is the APK copy with
`.shstrtab` normalised to gold Stringpool order.

## Measured, not landed

| worker | target | best shape | raw | aligned | size | why not landed |
|---|---|---|---:|---:|---:|---|
| w11001 | LevelScript | mission-0 case order + `tbh` tables | 54,541 | 17,427 | exact | analysis only; emitted-test-order/rel.plt addendum |
| w11003 | LevelScript | `0.8f` hoist + state-3 translate companion | 54,844 (+303) | 17,437 (+10) | 57,232 exact | structural (third `0.8f` pool word) but raw regression; the genuine -48 B is unclaimed |
| w11004 | LevelScript | callee-saved map + call-site audit | 54,541 | 17,427 | exact | analysis only |
| w11007 | Radar | campaign/sl block map + condition chains | 8,124 | 1,320 | exact | analysis only; wrong local optimum remains |
| w11009 | createMission | `FindMergedConditions` is the `and`-operand gate, not `Reassociate` | 5,454 | 786 | exact | analysis only |
| w11016 | Status | source-level switch-weight sweep | 722 | 320 | 1,432 | no source shape reaches the 4/3/2 plateau |
| w11018 | build | toolchain provenance | n/a | n/a | n/a | note only |
| w11020 | build | link recipe provenance | n/a | n/a | n/a | note only |

## Merge lessons

* Per-worker claims must be re-measured after cherry-pick.  w11005's
  first-two-field chain (claimed 5,695) did **not** reproduce in wave 163
  but **does** reproduce now; the two Layout changes are super-additive
  (5,705 -> 5,688, not 5,705 - 10 - 7 = 5,688 exactly, which happens to
  match here but was verified independently).
* w11011 + w11012 combine better than either alone: MGame::OnTouchEnd
  measures 4,087/917, not 4,089/918.
* w11013's HangarWindow change also improves `.ARM.extab` in combination
  (659, not the claimed 671).
* The aligned total rose to 24,608 (+408) while raw fell by 1,475: several
  raw wins (Radar +108, HangarWindow +135, Generator +43) trade aligned
  instructions for byte identity.  Raw/`.text` is the byte-identicality
  ground truth; aligned is indicative.
