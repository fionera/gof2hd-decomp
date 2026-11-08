# Wave 161 merge tests (coordinator)

Base: `01c7f92d6` (wave-160 docs), gate green (byte_exact 4507/4524,
`.text` 85,518, `.ARM.extab` 1,258, `.rel.plt` byte-identical, aligned
total 23,546).

Baseline on the base tree:

| fn | raw | aligned | size |
|---|---:|---:|---:|
| `Layout::Layout` | 5,733 | 1,978 | 7,072 |
| `LevelScript::process` | 55,048 | 17,049 | 57,232 |
| `MGame::OnTouchEnd` | 4,387 | 901 | 8,080 |
| `MGame::OnUpdate` | 318 | 19 | 11,440 |
| `HangarWindow::render` | 1,965 | 291 | 9,700 |

## Landed

* **w10702** (LevelScript: mission-89 case-4 double `getStarSystem()` +
  the missing `m_nScriptTimerA >= 8000 -> m_nState = 5` tail, compensated by
  dropping the non-original `if (!enemies->data_) continue;` in the
  mission-0 loop): raw **55,048 -> 55,013** (-35), aligned
  **17,049 -> 17,308** (+259), 57,232 B exact.
* **w10707** (Layout: `field_0x34` before `field_0x30_rowHeight` and
  `field_0x19c` before `field_0x18c`): raw **5,733 -> 5,722** (-11), aligned
  **1,978 -> 2,015** (+37), 7,072 B exact.
* **w10710** (MGame::OnTouchEnd: drop the stale
  `__builtin_expect(isDockingToStream(), 1)` hint): raw
  **4,387 -> 4,125** (-262), aligned **901 -> 905** (+4), `.rel.plt` 0,
  8,080 B exact.
* **w10711** (MGame::OnTouchEnd: mining-refine check `count > 0` instead of
  `count != 0`): raw **4,125 -> 4,127** (+2), aligned **905 -> 906** (+1);
  the only semantically-correct spelling that stays 8,080 B and keeps
  `.rel.plt` byte-identical.
* **w10718** (MGame::OnUpdate: m122 `radioBase` via the live `st`): raw
  **318 -> 37** (-281), aligned **19 -> 109** (+90), 11,440 B exact.  The
  aligned rise is the short m122 `mov`-induced block shift; the raw metric
  is the byte-identicality ground truth.
* **w10716** (HangarWindow: row-39 `getCredits()` + `volatile` pad shapes):
  raw **1,965 -> 2,514** (+549), aligned **291 -> 485** (+194), but the
  `.ARM.extab` diff falls **1,258 -> 725** (-533) with the 948 B / 156-row
  record intact.  Net differing bytes -16.

Totals after the landings: `.text` **85,480** (-38), `.ARM.extab` **725**
(-533), `.rel.plt` 0, `.note.gnu.build-id` 19, total differing bytes
**86,224** (was 86,796, -572).  Aligned total **24,131** (+585): the rise is
w10702 (+259), w10716 (+194), w10718 (+90), w10707 (+37) and the small
MGame::OnTouchEnd pair.  `check` is green, byte_exact 4507/4524.

## Measured, not landed

| candidate | raw | aligned | size | why not |
|---|---:|---:|---:|---|
| w10701 (LevelScript `Status *&status` + reorder) | 55,061 | 17,051 | 57,232 | +13 raw / +2 aligned vs base; the plain reorder is 54,929/16,920 but +60 B |
| w10703 (LevelScript state-3 else-if chain) | slightly worse | slightly worse | 57,232 | structural variant only |
| w10704 (LevelScript extra 13-entry `tbh`) | 55,013 | 17,308 | 57,232 | analysis: the extra table is the mission-0 dispatch |
| w10708 (Radar faithful tail + bool `alwaysEnemy`) | 8,367 | 1,333 | 9,908 | worse on both metrics |
| w10712 (createMission alien register map) | 5,461 | 781 | 17,172 | no source win |
| w10713 (createMission case-183 default 2,0,1) | 5,454 | 786 | 17,172 | raw -7 but aligned +5 |
| w10714 (createShip type-switch) | 1,560 | 460 | 2,764 | raw -3 but aligned +11 (same as w10517) |
| w10715 (Status nested-default / fold-overflow) | 722 | 320 | 1,432 | analysis only; the 1,432-B weight cell accepts a 4/3/2 arithmetic progression |
| w10719 (MenuTouchWindow late `field_0x20_top` reload) | 28 | 16 | 1,896 | raw -1 but aligned +1 |
| w10720 (build provenance round 2) | — | — | — | research note: `-g`/`-g2`/`-g3=limited`, flag order, response file and debug flags are all neutral |

## Key open questions for wave 162

1. **LevelScript -60 B companion**: the plain `messages,status,enemies,player`
   reorder is 54,929/16,920 at +60 B; the +60 decomposes into +10 prologue
   (`fp` canary spill/reload), +44 block placement/pool after the
   mission-95/99 `setStatusValue` call, +4 one extra `startFade` call
   (the original tail-merges its two `startFade(true,-1,500)` sites).
2. **Radar 4 B tail**: the faithful tail is 9,904 B; the `alwaysEnemySeen`
   `eors.w` lowering and a 2-byte cleanup are the missing bytes.
3. **MGame::OnTouchEnd PLT order**: `arm200 + truthy won` reproduces the
   original body order and the late join but is 8,060 B; `arm800 + truthy`
   is 8,080 B but swaps the `dockToStream`/`useCloak` PLT slots.
4. **HangarWindow `cbz`**: forcing the `tab == 0` edge to emit `cbz` needs a
   shape that is size-exact at 9,700 B with the credits call.
5. **Build provenance**: every non-code ELF byte is identical; the remaining
   levers are not per-TU flags.
