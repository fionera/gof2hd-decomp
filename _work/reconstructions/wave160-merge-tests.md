# Wave 160 merge tests (coordinator)

Base: `08af4633b` (wave-159 docs), gate green (byte_exact 4507/4524,
`.text` 85,722, `.ARM.extab` 1,256, `.rel.plt` byte-identical).

Baseline on the base tree:

| fn | raw | aligned | size |
|---|---:|---:|---:|
| `Layout::Layout` | 5,739 | 1,978 | 7,072 |
| `LevelScript::process` | 55,055 | 17,059 | 57,232 |
| `Radar::draw` | 8,124 | 1,320 | 9,908 |
| `MGame::OnTouchEnd` | 4,387 | 901 | 8,080 |
| `Level::createMission` | 5,652 | 709 | 17,172 |
| `HangarWindow::render` | 1,965 | 291 | 9,700 |

## Landed

* **w10507** (Layout: three adjacent field swaps): raw **5,739 -> 5,733**
  (-6), aligned 1,978 (0), 7,072 B exact.  An exhaustive 204-pair
  adjacent-swap sweep found exactly three size-exact strict wins; the
  strongest non-landable lead is `field_0x160` before `field_0x158`
  (5,702/1,957 at 7,024 B, gate-fatal).
* **w10605** (LevelScript: reload `Globals::status->field_90` in the
  mission-fail loop bound instead of the cached `stationList->count`):
  raw **55,055 -> 55,047** (-8), aligned **17,059 -> 17,050** (-9), 57,232 B.
* **w10604** (LevelScript: drop the hacking-guard `__builtin_expect`):
  raw **55,047 -> 55,056** (+9), aligned **17,050 -> 17,031** (-19).
* **w10602** (LevelScript: case-16 state-0 `m_nState++` instead of `= 1`):
  raw **55,056 -> 55,048** (-8), aligned **17,031 -> 17,049** (+18).
  Net LevelScript: **55,048 / 17,049** (was 55,055 / 17,059).
* **w10612** (createMission: hoist the alien-loop `end` bound and recompute
  the tail; the landed form is `(i < end) & (i < friendCount + alienGuns +
  field_164 + field_168)`): raw **5,652 -> 5,454** (-198), aligned
  **709 -> 786** (+77), 17,172 B exact.  `.text` diff **85,722 -> 85,524**.
* **w10613** (createMission: case-183 freighter default store order):
  raw **5,454 -> 5,461** (+7), aligned **786 -> 781** (-5).
  Net createMission: **5,461 / 781** (was 5,652 / 709).

Totals after the landings: `.text` **85,518** (-204), `.ARM.extab`
**1,258** (+2: w10602 and w10604 each add one LSDA byte), `.rel.plt` 0,
aligned total **23,546** (+62).  The raw `.text` win is ~200 bytes; the
aligned total rises because createMission's recomputed tail re-orders blocks
that the greedy instruction alignment penalises.  `check` is green,
byte_exact 4507/4524.

## Measured, not landed

| candidate | raw | aligned | size | why not |
|---|---:|---:|---:|---|
| w10611 (MGame::OnTouchEnd: cold starMap return hint + AND-form won guard) | 4,375 | 956 | 8,080 | **`.rel.plt` 7 bytes differ** (a PLT slot swaps) — breaks the byte-identical `.rel.plt` invariant |
| w10608 (Radar: duplicate `stationLockTimer` store) | 8,536 | 1,314 | 9,908 | +412 raw for -6 aligned; `.text` +412 |
| w10616 (HangarWindow: row-39 ULEB length) | 2,514 | 485 | 9,700 | +549 text / +194 aligned for extab 1,256 -> 731; net total bytes +24 |
| w10610 (MGame::OnTouchEnd: arm order 0x800/0x200/0x400) | — | — | 8,080 | **byte_exact 4507 -> 4505** (PLT slot swap); not green |
| w10603 (LevelScript literal-pool audit) | 55,055 | 17,059 | 57,232 | no src change; found the unreachable `translate(-delta*0.8f)` block (moving it out is -20 B) and the missing mission-89 case-4 `if (m_nScriptTimerA >= 8000)` (+8 B) |
| w10607 (Layout float literal audit) | 5,739 | 1,978 | 7,072 | four real fixes sum to -64 B; the only size-exact combination regresses to 5,845/2,551 |
| w10617 (Hud::draw sweep) | 1,325 | 249 | 9,332 | no win; `int barH` is the original type but gives 9,324 B |
| w10619 (small tail full-stream diff) | unchanged | unchanged | exact | no win; ModStation's literal-pool order is a real lever but the register pair still swaps |
| w10620 (build provenance) | — | — | — | research note: every non-code ELF byte is identical; `.ARM.extab` 1,256 decomposes into 12 functions' LSBAs; 40+ per-TU flag cells tested, none wins |
