# HangarWindow LSDA exact + a −4 elsewhere (wave 119, w6942)

Target: land w6933's `HangarWindow::render` LSDA fix (91.3 → 91.7 %, 9700 B) **without**
growing `.ARM.extab` past the 16-byte `.rodata` alignment gap, i.e. keep `.rodata` at
`0x1c9e10` and `byte_exact` at 4505.

Worktree `/home/debian/GalaxyOnFire2/wt/w6942`, branch `wave119-w6942`, base master
`7a031742d`. Builds on `hangarwindow-render-wave119-w6933.md` (the ternary/pad3 change),
`generator-createmission-wave119-w6924.md` and `-w6780.md`.

```
before (HEAD 7a031742d):  HangarWindow 91.3 %  944 B record   .ARM.extab 0x26298  .rodata 0x1c9e10
after:                    HangarWindow 91.7 %  948 B record   .ARM.extab 0x26294  .rodata 0x1c9e10
                          MGame::OnTouchEnd 81.3 -> 71.7 %   280 -> 272 B record
gate: byte 4505  linked 4505  data 843/843  imports 207  lint CLEAN  drift 0  27 sections
      avg 99.94 -> 99.93   file size 2,187,328 (exact)   RATCHET PASS
```

**Result: the −4 is taken from `MGame::OnTouchEnd` (one `__builtin_expect`), not from
`Generator::createMission`.** The net `.ARM.extab` is 4 B short of the original
(`0x26294` vs `0x26298`) but the shortfall is absorbed by the 16-byte `.rodata`
alignment, so every section address (including `.rodata` `0x1c9e10`) is unchanged and
`byte_exact`/`linked_exact` stay at 4505/4505.

---

## 1. The two source edits

### 1.1 `src/game/ui/HangarWindow.cpp` — the w6933 change (landed verbatim)

Replace the selected-item `right`/`left` `if/else` with two conditional operators and
delete the `volatile int pad3 = left;` codegen anchor. Per the w6933 note this gives a
distinct landing pad per `?:` arm (two extra cleanup rows) while the removed `pad3`
lifetime marker merges the `buttons[8]` block back into one long row; the two effects
cancel in `.text` (9700 B unchanged) and the LSDA record becomes exactly the original's
948 B.

```diff
-                                    int right, left;
-                                    if (tab == 1) {
-                                        right = item->item->getStationAmount();
-                                        left = item->item->getAmount();
-                                    } else {
-                                        right = item->item->getAmount();
-                                        left = this->bluePrint->getCurrentAmount(item->item->getIndex());
-                                    }
+                                    int right = tab == 1 ? item->item->getStationAmount() : item->item->getAmount();
+                                    int left = tab == 1 ? item->item->getAmount() : this->bluePrint->getCurrentAmount(item->item->getIndex());
                                     if (left > 0) {
-                                        volatile int pad3 = left;
```

### 1.2 `src/game/menu/MGame.cpp` — the −4 (`MGame::OnTouchEnd`)

```diff
                         int count = -amount;
-                        if (count > 0) {
+                        if (__builtin_expect(count > 0, 0)) {  // lint: arm_builtin anchor: MGame::OnTouchEnd; cold ore-report branch
                             report += String("\n") + String(count) + String("t ") +
                                       *Globals::gameText->getText(1274 + refined);
                         }
```

`MGame::OnTouchEnd` is already non-byte-exact (81.3 %), so changing its block layout
costs no `byte_exact`. The hint keeps the body at exactly **8080 B** (no `.text`
shift) and removes one 5-byte gap row from its LSDA: 44 rows / `cslen` 260 →
43 rows / `cslen` 255, i.e. record **280 → 272 (−8)**.

Why −8 is enough: after w6933 the total `.ARM.extab` is `0x2629c`. `.rodata` is
16-byte aligned and `.ARM.extab` starts at `0x1a3b78` (8 mod 16), so any section size
in `0x26289..0x26298` lands `.rodata` back on `0x1c9e10`. `0x2629c − 8 = 0x26294` is
inside that window. (A −4, −8 or −12 all work; −16 would move `.rodata` to `0x1c9e00`.)

## 2. Why not `Generator::createMission`?

The task title suggested "Generator −4", but the current `createMission` record is
**176 B, not 172 B** — the w6924 note's "restores 172 B" claim is stale (the docs
commit `98f531de9` already records `Generator +4`). Measured at HEAD:

| | original | HEAD (w6924 shape) |
|---|---:|---:|
| record | 172 | **176** |
| `cslen` | 151 | **152** |
| rows | 26 | 26 |
| `.text` | 1996 | 1996 |

The +4 is one extra 6-byte row in place of the original's 5-byte pad=0 null-block gap
row (ours places the `routes == nullptr` block at `+0x240`, immediately before
`getName`; the original places it at `+0x3ce`, between the `type == 15` and `type == 0`
arms of the amount switch). To get 172 the `cslen` must drop to 148..151 while `.text`
stays 1996; every source spelling tried this pass still gives 176/152/26:

| variant | text | record/cslen/rows |
|---|---:|---|
| HEAD | 1996 | 176/152/26 |
| `__builtin_expect(nextInt(2)==0, 0/1)` (inner) | 1996 | 176/152/26 |
| `__builtin_expect(race<4, 0/1)` | 1996 | 176/152/26 |
| `__builtin_expect(type==13, 1)` | 1996 | 176/152/26 |
| `if (type == 13 && race < 4)` (condition swap) | 1996 | 176/152/26 |
| `type = nextInt(2) == 0 ? 1 : 4` | 1996 | 176/152/26 |
| `int t = 4; if (...) t = 1; type = t` | 1996 | 176/152/26 |
| `if (getOffer() != 5) {else-body} else {...}` (arms swapped) | 1996 | 176/152/26 |
| `if (!getRoutes())` / `bool noRoutes` | 1996 | 176/152/26 |
| `volatile int padB` in the null block | 2000 | 176/152/26 |
| `__builtin_expect(getOffer()==5, 1)` | 2032 | 168/147/25 |
| `__builtin_expect(getOffer()==5, 0)` | 1960 | 176/152/26 |
| `__builtin_expect(race<4 && type==13, 0)` | 2044 | 168/147/25 |
| `__builtin_expect(routes==nullptr, 0)` | 1988 | 176/152/26 |
| `__builtin_expect(mask!=0, 0)` | 2008 | 176/152/26 |
| `__builtin_expect(pick==10, 0)` | 1996 | 176/152/26 |
| `__builtin_expect(pick==12, 1)` | 1996 | 176/152/26 |
| drop `volatile double padA` (+ its store) | 1988 | 176/152/26 |

The 168 B variants all change `.text` (1960..2048), which shifts every later function
and therefore `.rodata`; they cannot be used without a compensating `.text` change.
`Level::createShip`, `Level::createScene`, `Hud::draw`, `Level::createMission`,
`LevelScript::process`, `MGame::OnUpdate`, `Radar::draw`, `SpaceLounge::drawLounge`
and `ModStation::ModStation()` were also swept with `__builtin_expect(cond, 0/1)` on
every `if` in the function body; none produced `Δtext = 0` with a negative record
delta. `MGame::OnTouchEnd`'s `count > 0` was the only hit.

## 3. The −4 census method (reusable)

`tools/exidx_table.py` / `_work/reconstructions/exidx_table.py` gives each function's
`.ARM.extab` record length. A record is `personality+unwind (pre bytes) + ff ff 01 +
uleb(cslen) + cslen + pad-to-4`; therefore

```
need = (pre + 3 + uleb_len(cslen) + cslen) % 4   (0 -> 4)
```

is the minimum number of content bytes that must be *removed* to shrink the padded
record by 4. A `cslen` reduction of `need .. need+3` bytes gives exactly −4; a row
removal (5–6 bytes) gives −4 for `need` 2/3/4 and −8 for `need` 1.

Census at HEAD (`/dev/shm/w6942/census_head.txt`, 517 functions) lists every function
whose record matches the original but has `need = 1`; the most useful are the ones
that are *already non-byte-exact*, because their code may change for free:

| function | pct | record | cslen | rows | need |
|---|---:|---:|---:|---:|---:|
| `MGame::OnTouchEnd` | 81.3 | 280 | 260 | 44 | 1 |
| `ModStation::ModStation()` | 99.2 | 112 | 89 | 15 | 1 |
| `SpaceLounge::drawLounge` | 95.7 | 168 | 145 | 25 | 2 |
| `Level::createScene` | 92.8 | 228 | 206 | 37 | 3 |
| `Level::createShip` | 70.2 | 336 | 315 | 57 | 4 |
| `Radar::draw` | 74.4 | 132 | 112 | 19 | 4 |

## 4. Section / gate impact

| metric | before (HEAD) | after | original |
|---|---:|---:|---:|
| `.text` size / base | `0x1337c8` / `0x69440` | same | same |
| `.ARM.exidx` size / base | `0x6f70` / `0x19cc08` | same | same |
| `.ARM.extab` size / base | `0x26298` / `0x1a3b78` | **`0x26294`** / `0x1a3b78` | `0x26298` |
| `.rodata` base | `0x1c9e10` | **`0x1c9e10`** | `0x1c9e10` |
| `.rodata` size | `0x41c27` | `0x41c27` | `0x41b67` (+192 rank table) |
| file size | 2,187,328 | 2,187,328 | 2,187,328 |
| sections | 27 | 27 | 27 |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** | — |
| `data_exact` / imports | 843 / 207 | 843 / 207 | — |
| avg fuzzy | 99.94 | 99.93 | — |
| lint / drift | CLEAN / 0 | CLEAN / 0 | — |

`exidx_table.py --diff` after the change:

```
functions with differing extab record length:
  _ZN5MGame10OnTouchEndEiiPv: orig 280 ours 272 delta -8
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE: orig 172 ours 176 delta +4
  total delta -4 (-0x4)
```

`HangarWindow::render` is now **exact** (948 B). Raw-byte deltas (HEAD → after):
`.text` 88,119 → **89,562** (the `MGame::OnTouchEnd` block reorder makes that one body
differ more from the original even though it stays non-byte-exact), `.ARM.exidx`
861 → **1,657** (the extab record shift), `.ARM.extab` 109,453 → **135,594** (the
`.ARM.extab` records after the shifted one), `.rodata` 0 → 0.

**Fuzzy cost.** `HangarWindow::render` 91.3 → **91.7** (+0.4);
`MGame::OnTouchEnd` 81.3 → **71.7** (−9.6 on that one function = **0.0021** of the
4524-function average). Average 99.94 → 99.93, well inside the 0.3-point budget.

## 5. Proven vs hypothesis

**Proven**

* w6933's ternary + `pad3` removal gives `HangarWindow::render` a 948 B record at
  9700 B `.text` (re-measured on this base).
* `MGame::OnTouchEnd`'s `if (count > 0)` with `__builtin_expect(..., 0)` keeps the
  body at exactly 8080 B and shrinks its LSDA 280 → 272 B (44→43 rows, cslen 260→255).
* With both edits the `.ARM.extab` is `0x26294`, `.rodata` stays at `0x1c9e10`, all 27
  section headers are unchanged, the file size is exact, and the ratchet passes at
  byte 4505 / linked 4505 / data 843 / imports 207 / lint CLEAN / drift 0.

**Hypothesis / open**

* The original `createMission` 172 B record needs the cold `routes == nullptr` block
  placed between the `type == 15` and `type == 0` switch arms (a pad=0 gap row). Every
  source spelling tried reproduces ours, not the original; this is a
  `MachineBlockPlacement` decision (the shared `type` store block is chained with the
  offer arm here, with the else arm in the original). See
  `generator-createmission-wave119-w6780.md` for the IR/block-placement evidence.
* The −9.6-point `MGame::OnTouchEnd` drop is the cost of the hint reordering that
  function's blocks; it is a legitimate branch hint (ore-refine reports are rare), but
  a cheaper `Δtext=0` variant may exist in a function not swept here.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
cmake --build cmake-build-match --target drift -j6        # 0 fields
cmake --build cmake-build-match --target lint -j6         # CLEAN
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch (not committed): `/dev/shm/w6942/` — `measure.py` (compile one TU with the exact
`compile_commands.json` flags and read `.ARM.extab.text.<mangled>`), `vtest.py`
(string-edit + compile + text size/hash + LSDA), `rows.py` / `rowsobj.py` (LSDA row
dumps from a `.so` / `.o`), `census_head.txt`, `sweep/expect2.py` (per-function
`__builtin_expect` sweep), `mg_before.txt` / `mg_after.txt`.

## 7. Doc follow-up (not committed here)

The task asked for the note + source only. `docs/VALIDATION.md` §"Current state — ELF
sections" and `docs/IDENTICALITY.md` still say `.ARM.extab` is size-exact `0x26298`
with a `HangarWindow -4 / Generator +4` cancelling pair. The new truth is
`.ARM.extab` `0x26294` (4 B short), `HangarWindow::render` exact at 948 B,
`MGame::OnTouchEnd` 272 vs 280 (−8) and `Generator::createMission` 176 vs 172 (+4).
Those two sentences should be updated in the docs commit that follows.
