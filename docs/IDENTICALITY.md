# Binary identicality: gap analysis & roadmap

Goal: produce a `libgof2hdaa.so` byte-identical to `../_work/bins/android_2.0.16_libgof2hdaa.so`
(same sha256). This document measures the gap and tracks progress. Measured against the original
(2,187,328 bytes) with our stripped link output.

## Update 2026-11-07 (wave 132)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Wave-132 `Level::createShip` **73.2 -> 73.4** raw at the original 2764 B,
1007/1007 insns.  The only source change is the declaration order of the three
spawn-coordinate locals (`z`, `y`, `x` instead of `x`, `y`, `z`); the
expressions and types are unchanged.  The reordering changes the register
allocator's stack-slot colouring so the coordinate block's frame slots line up
with the original (`wpx=[sp,#44]`, `jitter1=[sp,#36]`, `wpy=[sp,#48]`).
Section deltas are unchanged from wave 131 (`.text` **85,167**, `.ARM.extab`
**156,304 B / 108,023 differing bytes**, 8 B short).  See
`_work/reconstructions/level-createship-wave132-w7693.md`.

## Update 2026-11-07 (wave 131)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Wave-131 `Level::createShip` **70.5 -> 73.2** raw (**73.187686**) at the original 2764 B,
1007/1007 insns.  The first ~100 instructions (prologue through the `Player` ctor) are now
byte-identical: the original spills `race` to `[sp,#60]`, keeps `type` in `fp`, `shipClass`
in `r5`+`sl`, `this` in `r9`, `status` in `r4`, `mission` in `r8`.  The shape that reproduces
this is the faithful two-call `getLevel() > 20 ? 20 : getLevel()` clamp (level and empPoints),
`::operator new` + placement `new` for `PlayerFighter`/`PlayerFixedObject`, the faithful
`x = waypoint->x + 1st`, `y = waypoint->y + 2nd`, `z = waypoint->z + 3rd` spawn, and a
`try { player->setEmpData(...); } catch (...) { __builtin_trap(); }` around `setEmpData`.
The `try` is an RA/scheduling barrier: it keeps the coordinate block above `setEmpData`
(without it the block sinks below the call and the prologue reverts to `str r2,[sp,#60]`,
`mov sl,r3`).  Cost: the `try`/`catch` record is **328 B** (54 rows, `ttype 0x00`) instead of
the original's 336 B cleanup-only record, so `.ARM.extab` is **156,304 B (8 B short)** and
108,023 content bytes differ.  Whole-`.text` differing bytes fall **85,379 -> 85,167**.
See `_work/reconstructions/level-createship-wave131-w7693.md`.

Section deltas: `.text` **85,167** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **108,023** (size **156,304 B**, 8 B short, `0x26290`); `.rodata` **0**
(byte-identical); `.rel.plt` **0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**.
File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 129)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Wave-129 re-lands the deferred w7645 `LevelScript::process` case-40/41/42 corrections at the
original 57232 B, plus the genuine case-42 state-6 fixes that pay for them: the bogus
`player->freeze = 0` (absent from the original) is removed, `player->setVisible(false)` moves to
its original slot after `setComputerControlled`, `freeMissionLock()` moves after the camera
`translate`, and the missing `m_nState = 7` state transition is restored.  The case-41 state-4
`timerA += delta` now precedes the camera translate as in the original.  `LevelScript::process`
raw fuzzy is 59.89 (was 60.10) because the corrected statements move blocks within the global
`MachineBlockPlacement` permutation; the function's own differing-byte count falls 54577 ->
54623 and the whole-`.text` count is 85,379 after the combined w7668 case-91 fix (85,487 on the w7666 base).  See
`_work/reconstructions/levelscript-process-wave129-w7666.md`.

Wave-129 gain: `Radar::draw` **75.268817 -> 75.297491** raw (display 75.4) at the original
9908 B, by computing the post-loop `getStationEnumIndex` before `getAutoPilotTarget` in the
reduced recompute.  The whole-`.text` differing-byte count fell **85,441 -> 85,440**.  The
block layout is otherwise unchanged; the faithful single-recompute shape (removing the
post-loop recompute) still re-rolls the planet-loop register split (`fp`/`sl`) and scores
72.93 %.  See `_work/reconstructions/radar-draw-wave129-w7674.md`.

Section deltas: `.text` **85,379** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,420** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-07 (wave 127)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.9**,
`LevelScript::process` **60.1**, `Level::createShip` **70.5**, `Status::missionCompleted` **71.0**, `Radar::draw` **75.4**, `MGame::OnTouchEnd` **81.9**,
`Level::createMission` **92.3**, `Gun::calcCharacterCollision` **93.4**,
`HangarWindow::render` **94.7**, `SpaceLounge::drawLounge` **95.7**, `Level::createScene` **96.5**,
`Hud::draw` **96.6**, `Generator::createMission` **97.2**, `MenuTouchWindow::OnTouchBegin` **98.1**,
`ModStation` C1/C2 **99.2**, `MGame::OnUpdate` **99.9**.

Wave-127 gain: `Generator::createMission` **90.0 -> 97.2** at the original size (1996 B) and
the original frame (96 B).  The clean shape (plain `int amount`, `Galaxy *galaxy` value,
`amount = 0; itemId = 0;` in the else arm) reproduces the original's exact seven-slot map;
the bonus `rem`/`agent` register-allocation fixed point is broken by writing the round-down
as the equivalent `bonus -= rem * 2` instead of `bonus = rawBonus - rem`, which lets
`rawBonus` die at the add so `rem` no longer needs `r9` and `agent` stays in `r9`.  The
`volatile short padA`, `volatile int *ap = &amount` and `Galaxy *&galaxy` hacks are removed.
The remaining 2.8 % is the bonus block's `r5`/`r6`/spill choice plus the `bhi` value-range
fold.  See `_work/reconstructions/generator-createmission-wave127-w7652.md`.

Section deltas: `.text` **85,438** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,427** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-07 (wave 126)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.9**,
`LevelScript::process` **60.1**, `Level::createShip` **70.5**, `Status::missionCompleted` **71.0**, `Radar::draw` **75.4**, `MGame::OnTouchEnd` **81.9**,
`Generator::createMission` **97.2**, `Level::createMission` **92.3**,
`Gun::calcCharacterCollision` **93.4**, `Level::createScene` **96.5**,
`HangarWindow::render` **94.7**, `SpaceLounge::drawLounge` **95.7**, `Hud::draw` **96.6**,
`MenuTouchWindow::OnTouchBegin` **98.1**, `ModStation` C1/C2 **99.2**,
`MGame::OnUpdate` **99.9**.

Wave-126 gains: `LevelScript::process` case-154 state 0/1 is now instruction-faithful
(`setComputerControlled` before `setVulnerable`, one `setSpeed(0.0f)`, `setTarget` moved from
state 1 to state 0, `m_nState++` at the block tail instead of `m_nState = 1` at the head, and the
state-1 `setSpeed(0.0f)` prologue restored). Size stays exactly 57232 B; `.rel.plt` and
`.ARM.exidx` remain byte-identical and `.rodata` exact. The function's fuzzy score is 60.2
(60.235 raw) but the whole-`.text` differing-byte count fell **85,552 -> 85,496** at that point (current **85,438** after the later wave-126/127 changes). See
`_work/reconstructions/levelscript-process-wave126-w7603.md`.

`Status::missionCompleted` **71.0 -> 71.1** — one `__builtin_expect(...,1)` on the case-162
`hasCargo` loop condition keeps the function at 1432 B and matches the original's loop branch
layout more closely. The remaining residual is the high-TBH body order: the original lays the
22 high case bodies out in source order, our identical clang in ascending case-value order,
which is a switch-edge-frequency (`!prof`/PGO) effect not expressible from C++11. See
`_work/reconstructions/status-missioncompleted-wave126-w7605.md`.

Section deltas: `.text` **85,438** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,427** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-06 (wave 125)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.9**,
`LevelScript::process` **60.2**, `Level::createShip` **70.5**, `Status::missionCompleted` **71.0**, `Radar::draw` **75.4**, `MGame::OnTouchEnd` **81.3**,
`Generator::createMission` **97.2**, `Level::createMission` **92.3**,
`Gun::calcCharacterCollision` **93.4**, `Level::createScene` **96.5**,
`HangarWindow::render` **94.1**, `SpaceLounge::drawLounge` **95.7**, `Hud::draw` **96.4**,
`MenuTouchWindow::OnTouchBegin` **98.1**, `ModStation` C1/C2 **99.2**,
`MGame::OnUpdate` **99.9**.

Wave-125 gain: `MGame::OnTouchEnd` **78.8 -> 81.3** — a `Mission *campaign` temporary reuses one
`getCampaignMission()` call (−4 B) and the trailing `hudTouchFlags == 0` block is folded into the
`wingmanCommand` `else if` chain (+4 B); the two are size-neutral together (8080 B) and keep
`.rel.plt` and `.ARM.exidx` byte-identical. See
`_work/reconstructions/mgame-ontouchend-wave125-w7513.md`.

Section deltas: `.text` **85,636** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,436** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-06 (wave 124)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.93%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.9**,
`LevelScript::process` **60.2**, `Level::createShip` **70.5**, `Status::missionCompleted` **71.0**, `Radar::draw` **75.4**, `MGame::OnTouchEnd` **78.8**,
`Generator::createMission` **97.2**, `Level::createMission` **92.3**,
`Gun::calcCharacterCollision` **93.4**, `Level::createScene` **96.5**,
`HangarWindow::render` **94.1**, `SpaceLounge::drawLounge` **95.7**, `Hud::draw` **96.4**,
`MenuTouchWindow::OnTouchBegin` **98.1**, `ModStation` C1/C2 **99.2**,
`MGame::OnUpdate` **99.9**.

Wave-124 gains: `Layout::Layout` **48.9 -> 48.9** (raw 48.894 -> 48.934; `field_0x158`
default arm derived from `field_0x154`); `Radar::draw` **74.5 -> 75.3** (planet-lock tail
before the landmark loop, reduced recompute); `Generator::createMission` **89.9 -> 90.0**
(named `toSystem` index temp + `Galaxy *&` alias); `Level::createShip` **70.3 -> 70.5**
(faithful jitter pairing + `z,y,x` declaration order; a non-semantic call-site RA tune reaches
71.4 but is not kept because it passes the wrong arguments);
`Level::createScene` **92.8 -> 93.6** (natural parts loop, `AERandom *&`, cap split, `geo`
hoist); `LevelScript::process` stays **60.3** but two case-157 fidelity fixes reduce its
`.text` delta. `MGame::OnTouchEnd` **82.4 -> 78.8** is a structural win: the failed-first
`won` local and `hasFailed() == true` hint make **`.rel.plt` byte-identical**; the fuzzy drop
is block-address placement, not missing code.

Section deltas: `.text` **85,975** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,474** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-06 (wave 123)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.9**,
`LevelScript::process` **60.2**, `Level::createShip` **70.3**, `Status::missionCompleted`
**71.0**, `Radar::draw` **74.5**, `MGame::OnTouchEnd` **82.4**,
`Generator::createMission` **89.9**, `Level::createMission` **92.3**,
`Level::createScene` **92.8**, `Gun::calcCharacterCollision` **93.4**,
`HangarWindow::render` **94.1**, `SpaceLounge::drawLounge` **95.7**, `Hud::draw` **96.4**,
`MenuTouchWindow::OnTouchBegin` **98.1**, `ModStation` C1/C2 **99.2**,
`MGame::OnUpdate` **99.9**.

Wave-123 gains: `Layout::Layout` **48.7 -> 48.9** (n9-first `field_0x184` arm, int
`field_0x124` arm, `field_0x2b8`/`field_0x110` order); `HangarWindow::render` **94.0 -> 94.1**
(free-credits `visible` as one expression); `Hud::draw` **95.3 -> 96.4** (boost if/else,
`Globals::status` anchor, `short barW`); `MGame::OnTouchEnd` **81.7 -> 82.4** (named `won`
local + inverted guard). `LevelScript::process` stays **60.3** but now reuses the saved
`player` local in mission-0 case 8 (raw 60.2547 -> 60.2567). `Level::createShip` **73.9 ->
70.3** is a structural win: the real-`new` body carries the original's 336 B cleanup-only
LSDA (`ttype 0xff`, 57 rows), so **`.ARM.extab` is size- and base-exact (`0x26298`)** and
**`.ARM.exidx` is byte-identical**; the score drop is the Greedy-RA cascade caused by the four
live cleanup pointers (see `_work/reconstructions/level-createship-wave123-w7306.md`).

Build-flag research (wave 123, two independent workers) re-confirmed the stock NDK r18b CMake
Release recipe: 192/192 app TUs object-identical, full default link flag set byte-identical,
and 259 + 258 global plus 140 per-TU flag screens found **zero** target improvements.
The remaining gaps are source shape, not build flags. See
`_work/reconstructions/build-flags-wave123-w7319.md` and
`_work/reconstructions/build-flags-wave123-w7320.md`.

Section deltas: `.text` **86,993** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,446** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**4** (2 swapped PLT entries); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-06 (wave 122)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.7**,
`Status::missionCompleted` **71.0**, `LevelScript::process` **60.2**, `Level::createShip`
**73.9**, `Radar::draw` **74.5**, `MGame::OnTouchEnd` **82.4**, `Generator::createMission`
**89.9**, `Level::createMission` **92.3**, `Level::createScene` **92.8**,
`Gun::calcCharacterCollision` **93.4**, `HangarWindow::render` **94.0**, `Hud::draw`
**95.3**, `SpaceLounge::drawLounge` **95.7**, `MenuTouchWindow::OnTouchBegin` **98.1**,
`ModStation` C1/C2 **99.2**, `MGame::OnUpdate` **99.9**.

Wave-122 gains: `Layout::Layout` now uses the original's `field_0x184` iPad/n9 arm order
(48.96 -> **48.70**, a fidelity fix); `Status::missionCompleted` **59.4 -> 71.0** (four branch-probability hints on the
plain switch recover most of the rank-table removal cost while keeping `.rodata` exact);
`Generator::createMission` **88.0 -> 89.9** (volatile `short padA` + volatile
`amount` store); `HangarWindow::render` **91.7 -> 94.0** (free-credits canvas local + faithful
width order); `Hud::draw` **95.99 -> 95.27** (marker per-arm image load + `int` dock total; the
marker/radar sub-draws are now instruction-identical, the score dip is the pre-existing boost
Canvas hoist plus dock-block allocator perturbation); `LevelScript::process` **60.2 -> 60.3**
(case-154 state-7 timer/state fix); `Level::createShip` **73.8 -> 73.9** (capital-ship position
vector). See the `*-wave122-w72*.md` notes.

Section deltas: `.text` **86,947** differing bytes (size exact); `.ARM.exidx` **1,327**;
`.ARM.extab` **156,304 B** (8 B short; only `Level::createShip` -8, 108,010 differing bytes);
`.rodata` **0** (byte-identical); `.rel.plt` **4**; `.bss` **1**; `.note.gnu.build-id` **20**.
File size is exact (2,187,328 B).

## Update 2026-11-06 (wave 121)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207** (identical to the original), lint **CLEAN**, drift **0**, symbol
parity **5370/5370** (0 extra / 0 missing), avg **99.93%**. 18 functions remain
non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **49.0**,
`Status::missionCompleted` **59.4**, `LevelScript::process` **60.2**, `Level::createShip`
**73.9**, `Radar::draw` **74.5**, `MGame::OnTouchEnd` **81.7**, `Generator::createMission`
**88.0**, `HangarWindow::render` **91.7**, `Level::createMission` **92.3**,
`Level::createScene` **92.8**, `Gun::calcCharacterCollision` **93.4**,
`SpaceLounge::drawLounge` **95.7**, `Hud::draw` **96.0**, `MenuTouchWindow::OnTouchBegin`
**98.1**, `ModStation` C1/C2 **99.2**, `MGame::OnUpdate` **99.9**.

Wave 121 made three structural gains. `PlayerEgo::calcCollision` is now **byte-exact**
(95.5 -> **100.0**, 968 B): the original's un-rotated loop and visible-arm register shuffle
are reproduced by two branch-weight hints (`__builtin_expect(isShrinking() || miningGame != 0, 1)`
on the wormhole `continue` and `__builtin_expect(isDying() || isDead(), 0)` on the steal-arm
`continue`), the real compare order (`collision != Vector{0,0,0}`) and the constant tail store
(`= 1`). The previous body's two redundant size guards and the `field_0x71` tail store are gone.
See `_work/reconstructions/playerego-calccollision-wave121-w7116.md`.

`Generator::createMission` improved **84.7 -> 88.0** and its `.ARM.extab` record is now exact
at 172 B (the `race < 4 && type == 13` branch hint plus the `volatile long long padA` width).
See `_work/reconstructions/generator-createmission-wave121-w7110.md`.

`Level::createShip` now uses the **faithful spawn coordinates** (`x = waypoint->x + jx`,
`y = waypoint->y + jy`, `z = waypoint->z + jz`, all `int`) instead of the wave-119 tuned
slot-map; the score dips 74.4 -> **73.9** but the source now matches the original's
disassembly semantics. See `_work/reconstructions/level-createship-wave121-w7107.md`.

The last manually linked data input was also removed: the 192-byte
`Status::missionCompleted` mission-type -> case-rank table (`tools/mission_case_table.cpp`,
fixed address `0x20b977`). The function now uses a plain `switch (m->getType())` with the
real mission-type labels in the original body order. **`.rodata` is byte-identical**:
269,159 B at `0x1c9e10`/`0x41b67`, 0 differing bytes. The function score drops
**92.454632% -> 59.372027%** (1432 B -> 1432 B, byte/linked still false), and the `.text`
content delta is **87,892** bytes; the `.text` size/base (0x69440/0x1337c8),
`.ARM.exidx` size/base, `.ARM.extab` size (0x26290, 8 B short) and the exact file size
(2,187,328 B) are unchanged. The remaining section deltas are `.text` 87,892,
`.ARM.exidx` 1,327, `.ARM.extab` 107,976 (8 B short), `.rel.plt` 6, `.bss` 1 and
`.note.gnu.build-id` 20.

Why the Status score drops: clang 7's `MachineBlockPlacement` lays a plain switch's case bodies
out in the jump table's value-sorted successor order, while the original's body layout is
two ascending runs (A = 150,151,152,153,154,155,158,162,166,170,184,189 then
B = 156,157,160,163,164,165,168,169,171,172). The rank table made the sorted case values
A-then-B, which is why it reproduced the original layout. A plain switch matches the
dispatch (`sub #150; cmp #39; tbh`), the TBH size/base, the low group and the shared tails,
but the A/B body order is value-interleaved, so the verifier's LCS drops. The table is the
only plain-C++ mechanism found that reproduces the original body order; removing it is a
structural win (`.rodata` exact, no manually linked data) at the cost of the function's
fuzzy score. See `_work/reconstructions/status-missioncompleted-wave121-w7113.md`.

## Update 2026-11-06 (wave 120, superseded by wave 121)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4505/4524**, linked_exact **4505/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207** (identical to the original), lint **CLEAN**, drift **0**, symbol
parity **5370/5370** (0 extra / 0 missing), avg **99.94%**. 19 functions remain
non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **49.0**,
`LevelScript::process` **60.0**, `Level::createShip` **74.4**, `Radar::draw` **74.5**,
`MGame::OnTouchEnd` **81.6**, `Generator::createMission` **84.7**, `HangarWindow::render`
**91.7**, `Level::createMission` **92.3**, `Status::missionCompleted` **92.5**,
`Level::createScene` **92.8**, `Gun::calcCharacterCollision` **93.4**,
`PlayerEgo::calcCollision` **95.5**, `SpaceLounge::drawLounge` **95.7**, `Hud::draw`
**96.0**, `MenuTouchWindow::OnTouchBegin` **98.1**, `ModStation` C1/C2 **99.2**,
`MGame::OnUpdate` **99.9**.

Wave-120 build-provenance research (10 parallel agents) established that the current
compile recipe is byte-equivalent to **stock NDK r18b CMake Release defaults**: 192/192
app TUs compile to identical objects, the full CMake/ndk-build default link flag set
produces the same `.so` hash, and a stock toolchain plus a minimal single-target
`CMakeLists.txt` reproduces the original's dynamic tables and `.rodata` exactly (the
canonical +192 B `.rodata` is the checked-in `tools/mission_case_table.cpp` matching
device). No tested global or per-TU compile flag improves any residual function; the
remaining gaps are source-shape. See
`_work/reconstructions/build-flags-cmake-equivalence-wave120-main.md`,
`_work/reconstructions/stock-cmake-reproduction-wave120-w7010.md`, and
`_work/reconstructions/per-tu-flag-sensitivity-wave120-w7005.md`.

Section deltas: `.text` **87,809** differing bytes (size exact); `.ARM.exidx` **1,327**;
`.ARM.extab` **156,304 B** (8 B short; only `Level::createShip` -8, 107,985 differing
bytes); `.rodata` **+192** (mission rank table); `.rel.plt` **6**; `.note.gnu.build-id`
**20**; `.bss` **3** (the known pointer-order residual). File size is exact (2,187,328 B).

## Update 2026-11-05 (wave 119)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4505/4524**, linked_exact **4505/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207** (identical to the original), lint **CLEAN**, drift **0**, symbol
parity **5370/5370** (0 extra / 0 missing), avg **99.94%**. 19 functions remain
non-byte-exact, all size-exact.

Wave 119 (20 parallel `codex exec -p dsv41` workers, worst-first) made
`MenuTouchWindow::MenuTouchWindow` C1/C2 **byte-exact** (99.82 -> **100.0**; hoisting row2
before `addButton(19)`), moving `byte_exact` 4503 -> **4505**. Other gains:
`Level::createScene` 90.7 -> **92.8**, `Radar::draw` 74.0 -> **74.4**,
`LevelScript::process` 58.2 -> **60.0** (merged wave-119 statement reorders at exactly
57232 B). `.text` content delta 89,836 -> **88,125**.

The `Status::missionCompleted` rank table is the last `.rodata` input
(`tools/mission_case_table.cpp`, fixed address `0x20b977`); every original `.rodata` byte
before it is exact, and the table accounts for the only 192 B of extra `.rodata`. All 27
original section headers are present and exact, with no extra section and an exact file size
(2,187,328 B).

**`.ARM.extab` is 8 B short** (`156,304` B, `0x26290`); the only record delta is
`Level::createShip` -8 (328 vs 336). `Generator::createMission` is now exact at 172 B
(wave-121 w7110) and `HangarWindow::render` is exact at 948 B (wave-119 w6978). `.rodata`
starts at the original `0x1c9e10`; the 8 B shortfall fits the 16-byte `.rodata` alignment
gap, so no function address moves.

The only differing section bytes are `.text` **87,892**; `.ARM.exidx` **1,327** raw bytes;
`.ARM.extab` **107,976** (8 B short; `Level::createShip` -8); `.rodata` **0** (byte-identical); and
`.note.gnu.build-id` **20**. Everything else (`.data`, `.data.rel.ro`, `.plt`, `.got`,
`.init_array`, `.fini_array`, `.dynamic`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version*`,
`.rel.dyn`, `.rel.plt`, `.comment`, `.note.android.ident`, `.note.gnu.gold-version`,
`.shstrtab`, `.ARM.attributes`) is byte-identical. `.rel.plt` now differs by **6 bytes**: the
`MGame::OnTouchEnd` cold-block layout swaps two adjacent `JUMP_SLOT` entries
(`MenuTouchWindow` / `MGame::setCinematicMode`); it returns to exact when that function matches.

The 19 non-byte-exact functions (fuzzy %, current report): `Layout::Layout` C1/C2 48.9,
`Generator::createMission` 84.3, `LevelScript::process` 60.0, `Level::createShip` 70.3,
`Radar::draw` 74.4, `MGame::OnTouchEnd` 81.3, `PlayerEgo::calcCollision` 95.5,
`HangarWindow::render` 91.3, `Level::createMission` 92.3, `Status::missionCompleted` 92.5,
`Level::createScene` 92.8, `Gun::calcCharacterCollision` 93.4, `SpaceLounge::drawLounge`
95.7, `Hud::draw` 96.0, `MenuTouchWindow::OnTouchBegin` 98.1, `ModStation` C1/C2 99.2,
`MGame::OnUpdate` 99.9. See `_work/reconstructions/build-provenance-flags-wave119-w6719.md`
and the wave-106..119 notes.

## Update 2026-10-26 — historical (superseded by the update above)

Then-current: **byte_exact 4499, linked_exact 4499, avg 99.9%**. libcrypto is OpenSSL 1.1.0l via
NDK r13b GCC 4.9 -O3 ARM (adds the original's genuine `memcpy`/`memset`/`mkstemp` imports). Adds `-g` (authentic ndk-build
default; keeps entry-block argument copies from PostRA sinking), the app flags for vendored
zlib/libzip (`-Oz -g -fstack-protector-strong -ffunction-sections`, was `-O2`), and source-order
placement of local functions in the then-current section-ordering file. The 2026-11-01 data-layout pass
dropped ten empty-`.text` CANTUNWIND sentinels that had inflated `.ARM.exidx` by 0x60 and
pushed `.rodata` to 0x1c9e50 (byte 2646 -> 2732), and restored the original string-literal
order in BloomShader/GlowPPShader/SandboxShader (byte 2732 -> 2734). The 2026-11-01
literal-pool pass then fixed the GOT first-reference order (byte 3921 -> 4105) and the
`LowResourceTable`/`ResourceMaterial` link order witnessed by `.rodata` (byte 4105 -> 4107);
the Level.cpp `.rodata` ordering pass then made `Level::createCampaignMission` byte-exact
(byte 4107 -> 4108). A further `.rodata` ordering pass fixed `Radar::drawCurrentLock`'s `%`
literal and the libzip `zip_err_str`/`zip_open` order (byte 4108 -> 4112), and the Level
padding-array removal restored the jumpgate/sky array sizes (byte 4112 -> 4115). The
2026-11-01 `.rodata` output-group pass then replaced the trailing `*` in the section-ordering
file with `.text*`/`.data*`/`.bss*`, so plain `.rodata` inputs stay at gold's index 0 and
interleave with the merge groups in input order (byte 4115 -> 4171). The 2026-11-01 data-layout
pass then fixed the last three `.rodata` size deltas (ModStation reward table, MissionsWindow goal
text default, DialogueWindow success script/alignment), making **every ELF section size- and
base-exact**, `.rodata` included (0x41b67 @ 0x1c9e10), and byte_exact 4171 -> 4182. The 2026-11-01 worst-match wave then added the Generator table dedup
(byte 4182 -> 4188) and the TU-start 4-byte alignment discovery in the exidx-marker generator
(byte 4188 -> 4233, section sizes unchanged) plus Layout::Layout 45.3 -> 46.6%, HangarWindow::render 78.2 -> 78.7% and
the Status rank-table fix (fuzzy-only, byte-neutral). The w3438 `.rodata` content-map pass then
moved `g_levelXPTable`/`Status::checkForLevelUp` ahead of `kMissionCase`, making
`0x1fc330..0x1fc390` byte-identical and `Status::checkForLevelUp` byte-exact (byte 4249 -> 4250;
linked 4496 unchanged). The w3451 data-content pass then forced the original `.data` emission
order for `gStoryFaces`/`gPirateFace` with a function-level `(void) gStoryFaces;` (byte 4254 ->
4255), dropping `.data` differing bytes 260 -> 3 and `.data.rel.ro` 744 -> 670. The w3457
`.data.rel.ro` content audit found the then-remaining 667 bytes were all `R_ARM_RELATIVE`
address mirrors (libc++abi `.text` vtables, 18 B `Level::createScene` `.rodata` table, 3 B `.bss`
pointers); its libzip pass aligned `zip_file_strerror` to 4 bytes (byte 4263 -> 4264, all section
sizes/bases unchanged). The w3460 libc++abi-placement pass then gave every archive member's
empty-`.text` sentinel a 4-byte alignment (the original's TU starts), removed the 0x24-byte
`levelTextAnchor` anchor and placed the `fallback_malloc.o`/`abort_message.o` markers next to
their owners: **`.data` is now byte-identical (3 -> 0 differing bytes)** and `.data.rel.ro` drops
667 -> 21 bytes (18 B `Level::createScene` `.rodata` table + 3 B `.bss` pointer order), byte
4265 -> 4408. The first
`.rodata` *content* divergence was at 0x1fb7f8 (VMA); 8,826 bytes differed. See
`_work/reconstructions/data_content_round2_w3451.md`,
`_work/reconstructions/rodata_content_map_round2.md`,
`_work/reconstructions/rodata_group_order.md`, `_work/reconstructions/literal_pool_audit.md`
and `_work/reconstructions/data_layout_audit_2.md`.

The w3462 `.ARM.exidx`/`.ARM.extab` pass then fixed the empty-`.text` CANTUNWIND sentinel
set (drop `Vector`/`BuildResourceList`/`LowResourceTable`/`Array`/`ArrayInstantiations`,
keep `BackButton`/`main`), moved the NdkHooks/NdkAutosave sentinels to the original's
witnessed boundaries, and added a 12-byte dead-local pad in `HangarWindow::render` to
restore its LSDA record. **`.ARM.exidx` is now `0x6f70` (3566 entries, CANT 272, inline
856, extab 2438) and `.ARM.extab` was `0x26298`**, both size- and base-exact at that point;
byte 4408 -> 4409. The remaining EH content is one `Generator::createMission` CANT/INL
offset (4-byte body shortfall) and seven extab record-length deltas. Those seven records
belong to the seven still-mismatched bodies (`Generator::createMission`,
`Level::createShip`, `StarMap::draw`, `RecordHandler::recordStoreWrite`,
`LevelScript::process`, `HangarWindow::render`, `Hud::draw`) and netted **-8 bytes** at
that point. The later `RecordHandler::recordStoreWrite`/`Hud::draw` record fixes leave
five deltas (`Generator::createMission` +4, `Level::createShip` -8,
`LevelScript::process` +4, `HangarWindow::render` -8, `Hud::draw` +4) netting **-4 bytes**.
The wave-102 LSDA-scope pass then fixed `Level::createShip`, `HangarWindow::render` and
`Hud::draw` and pulled `Generator::createMission` to 168 B, leaving only the cancelling
`Generator::createMission` -4 / `LevelScript::process` +4 pair; `.ARM.extab` is now
`0x26298` (156,312 B), size- and base-exact. See
`_work/reconstructions/exidx_extab_exact_w3462.md`,
`_work/reconstructions/exidx_duplicate_address_root_cause_w3565.md`,
`_work/reconstructions/exidx-records_w4237.md` and
`_work/reconstructions/hangarwindow-render-w4243.md`.

The w3472 `Status::missionCompleted` pass then recovered the 4-byte `.text` shortfall that
had blocked the Level `.rodata` block. The original switches directly on `m->getType()` and is
1432 B; our direct `int` switch emitted 1428 B because clang 7 places the case bodies in
ascending-value order instead of the original's source order. Switching on
`(short) m->getType()` restores the 1432 B size without a padding anchor (the signed-narrow
switch lowering places the 184/189 bodies after the default block). With the size fixed, the
Level `.rodata` corrections from w3469 could land: `g_ips_shipTypeMap[64]`,
`jumpgateMeshes[16]`, removal of the invented `kMissionCase[190]`, the 13-row
`gCreateChar2Table`, the `alignas(16)` LensFlare tables and the 10-int Hud `_RACE_IMAGES_BIG`.
Every ELF section stays size- and base-exact; **byte_exact 4414 -> 4441 (+27), linked 4496
unchanged**, `.rodata` differing bytes 13,121 -> 8,826 (first still `0x1fb7f8`). The
`missionCompleted` body itself remains a 57.7% near-miss (the direct `int` form was 60.2%);
the gate gain is from the corrected `.rodata` data, not from that function. See
`_work/reconstructions/Status-missionCompleted-4bytes-w3472.md`.

The w3477 `.rodata` content round-5 pass then closed the remaining 8,826-byte gap. The
baseline gap was pure table emission order (word multiset 6,586/6,586 common, 0 unique):
clang 7 emits local statics in function order and namespace-scope statics at first odr-use,
so the Level station/blueprint tables, `createScene`'s hangar-spot tables and the StarMap /
MenuTouchWindow tables were landing in the wrong order. Three `Level` function moves, one
`#pragma clang section rodata=".rodata"` on `PlayerCreature`'s 16-byte table, and three
`(void)` odr-use anchors reproduce the original order. **`.rodata` is now byte-identical
(0 / 269,159 differing bytes)**; every ELF section stays size- and base-exact, and
**byte_exact 4441 -> 4463 (+22), linked 4496 unchanged**. See
`_work/reconstructions/rodata_content_round5_w3477.md`.

The w3490 `kMissionCase` pass then restored `CUTSCENE_SHIP_HEIGHT[64]` and removed the
rank-table workaround, returning `.rodata` to **0 differing bytes**. A later pass reintroduced the
rank table in its own `.missioncase` section to force source-order switch-body layout (raising
`Status::missionCompleted`'s fuzzy match from 57.7% to 86.7%). The w3570 pass removed it again:
the direct `(short) m->getType()` switch keeps **byte_exact 4496 / linked_exact 4496**, `.text`
`0x1337c8`, `.rodata` 0 differing bytes, lint clean, drift 0, and removes the `.missioncase`
section (28 -> 27 sections, `.shstrtab` 0x124 -> 0x117); it also restores the original
type -> body mapping for types 161/162/166/168/169. `missionCompleted` returns to 57.7% (the
rank table was a codegen hack, not original data). See
`_work/reconstructions/kMissionCase_real_table_w3490.md` and
`_work/reconstructions/kMissionCase_removal_w3570.md`. Wave102 removed the w5003/wf108 rank table
again: the direct `int` switch with the case-166 `return m` size anchor keeps **byte_exact
4500 / linked_exact 4500**, `.text` `0x1337c8`, `.rodata` 0 differing bytes, lint clean, drift 0,
and restores the 27-section table (`.shstrtab` 0x117, `.rel.plt` `sh_info=20`, exact file size);
`missionCompleted` returns to 60.4%. See
`_work/reconstructions/status-missioncase-section-table-wave102-wm102.md`.

## Update 2026-07-26 — historical (superseded by the update above)

The verify metric was rebuilt (v2: symbol-aware call targets, PLT-aware, literal-pool words
compared as data, size-checked linked_exact, autojunk off — see docs/VALIDATION.md) and the
vendored zlib/libzip/SHA ISA modes fixed (131 functions were compared in the wrong ARM/Thumb
mode). Current: **compared 4524, byte_exact 4115, linked_exact 4495, avg 99.89%, stub_zero_size
0, missing 0, sodiff extra 0, undefined imports 207**. Historical figures in the sections below
(923/1636, 870/1569, 35/0/0, extra 9–35) are kept as a record of past states — do not quote them
as current. The no-degradation gate is now machine-checked: `tools/ratchet.py` vs
`tools/verify_baseline.json` (cmake `check` target + pre-push hook). Two new prerequisites for
byte-identicality were established from the original source-layout dumps
(`_work/original_layout/`): original **TU membership** (which functions/statics/literals compile
together — .rodata/.data order, `.init_array` (orig 3 entries, ours the same 3,
byte-identical), string pools and -Oz same-TU inlining all follow it) and original
object **link order**. The `drift` target (fixed to
the real AAPCS ABI) reports 0 drifted fields; the former 74-field backlog has been reconciled.

## Build reconstruction (wave 95, 2026-11-01)

The original build is now pinned by evidence, not by A/B alone:

* **`-ffunction-sections` was used.** The original `.ARM.exidx` has 193 `CANTUNWIND` entries
  that share their target address with a real entry; that is gold's `fix_exidx_coverage()`
  sentinel for the empty plain `.text` section every `-ffunction-sections` object carries. A
  rebuild without the flag produces only 52. The original was linked **without** a
  section-ordering file: its `.text` is grouped by original TU (2,231 mapped functions, 105 TUs,
  104 transitions, 0 interleaving). The current tree reproduces that with a plain input-order
  link: after the w4210 TU/function-order reconstruction (including the `BSphere` -> `Vector`
  merge), all 4,524 exported functions land at their original addresses and no ordering file is
  used. The object order is now a checked-in plain source list (`tools/source_order.txt`), read
  directly by `CMakeLists.txt` and linked as `$<TARGET_OBJECTS:gof2>`; the build no longer reads
  the original `.so` to decide link order. `tools/gen_link_order.py` is analysis-only and the old
  `gen_section_order.py`/`gen_exidx_markers.py` generators are deleted (w4217). See
  `_work/reconstructions/link_order_tu_reconstruction_w4210.md`,
  `_work/reconstructions/static_link_order_w4217.md`,
  `_work/reconstructions/ffunction_sections_and_order_file.md` and
  `_work/reconstructions/tu_membership_interleaving_w3613.md`.
* **`-fdata-sections` was NOT used.** The original's `.rodata`, `.data` and
  `.data.rel.ro` are byte-identical to the no-`-fdata-sections` build and differ by
  113,935 / 3,273 / 405 bytes with it. This is a real deviation from vanilla
  ndk-build r18b. See `_work/reconstructions/definitive_build_recipe_w3697.md`.
* **The link order is not vanilla ndk-build `c++_static`.** FMOD precedes
  `libc++abi.a`; `_Unwind_*` stay `UND` and `__gnu_Unwind_Find_exidx` is absent.
  A vanilla-order rebuild is 1345 byte-exact with `.text 0x135d0c`. See the same
  note.
* **`-g` is codegen-relevant at `-Oz`.** `-g`, `-g2`, `-g3` and all DWARF variants are identical
  (byte 4496); `-g0`, `-g1` and `-gline-tables-only` drop to byte 2069 because entry-block
  argument copies are no longer kept from PostRA sinking. The tree's `-g` is correct. See
  `_work/reconstructions/debug_info_flags_w3564.md`.
* **`.dynsym` order is gold's hash-map iteration = first-mention order over the link inputs.**
  The model (name_key ^ version_key, SGI `hash_map` ascending buckets) reproduces our entire
  `.dynsym` 5,577/5,577; `.dynstr` and `.hash` follow from it with no independent order. The
  app region is exactly first-mention over crtbegin + 193 objects (4,935/4,935). See
  `_work/reconstructions/dynsym_dynstr_hash_order_w3567.md` and
  `_work/reconstructions/tu_compile_link_order_w3560.md`.
* **Shortcut removal (same wave).** The `s_processPad` anchor is replaced by the real 16-byte
  unreferenced file-static object that `TargetFollowCamera.cpp` owns at `0x2183a0`; the eight
  `(void)` odr-use anchors in `Level.cpp` are replaced by `[[gnu::used]]` definitions at the
  same `.rodata` emission point. Both are gate-neutral (byte 4496 / linked 4496). See
  `_work/reconstructions/remove_s_processPad_anchor_w3571.md` and
  `_work/reconstructions/Level-remove-odr-anchors-w3574.md`.
* **Current differing bytes (2026-11-04).** `.text` (see table); `.ARM.extab` (see table);
  `.ARM.exidx` (see table); `.note.gnu.build-id` 20; `.rel.plt` 0; `.dynsym` 0; `.dynstr` 0;
  `.hash` 0; `.gnu.version` 0; `.rel.dyn` 0; `.shstrtab` 0;
  **section headers: all 27 original entries exact** (27 × 40
  bytes; including `.rel.plt` `SHF_INFO_LINK`/`sh_info=20`,
  `.init_array`/`.fini_array` `sh_entsize=4` and `.bss` `sh_offset=0x21596c`).
  `.rodata`, `.data`, `.data.rel.ro`, `.plt`, `.got`, `.dynamic`, `.init_array`, `.fini_array`,
  `.comment`, `.note.*` and `.ARM.attributes` are byte-identical; the section table matches the
  original's 27 sections. The w3616 pass made
  `.rel.dyn` byte-identical at the time and cut `.rel.plt` to one moved symbol
  (`PlayerEgo::deleteHackingGame`); the w3624 pass merged `BuildResourceList.cpp` +
  `LowResourceTable.cpp` + `ResourceMaterial.cpp` back into the original `GameResource.cpp` TU
  (`.dynsym` 62 -> 50, `.dynstr` 345 -> 211, `.hash` 42 -> 33, `.rel.plt` 15 -> 12,
  `.ARM.exidx` 2735 -> 2733). The w3695 pass replaced the post-link GNU
  `strip --strip-all` with gold `-s`, reproducing the original `.shstrtab` byte-for-byte
  (259 -> 0 differing bytes; see
  `_work/reconstructions/nontext_relplt_shstrtab_w3695.md`). The w3697 section-order pass
  changed the `.text` function order; the later link-order reconstruction restored the dynamic
  table order, and `.dynsym`/`.dynstr`/`.hash`/`.rel.dyn`/`.rel.plt` are byte-identical again
  (`sodiff` 0 extra / 0 missing).
* **Build metadata.** `.note.android.ident` is byte-identical to the NDK r18b API-16
  `crtbegin_so.o` note; the only path-like string in the `.so` is the libc++abi buildbot
  `abort_message.cpp` path, and the only app source-file witness is `main.c` in three log
  strings. No app source path survives. See
  `_work/reconstructions/build_metadata_source_paths_w3566.md`.

## Section-size deltas (stripped ours vs original)

| section | orig | ours | delta | meaning |
|---|---|---|---|---|
| **.text** | 1,259,464 | 1,259,464 | **0** | exact size and address (0x69440). 85,167 content bytes differ across the 18 non-byte-exact bodies. |
| **.rodata** | 269,159 | 269,159 | **0** | **byte-identical** (0 differing bytes; base 0x1c9e10, size 0x41b67). The former 192 B `Status::missionCompleted` rank table was removed in wave 121; the switch is a plain `switch (m->getType())`. |
| .ARM.exidx | 28,528 | 28,528 | **0** | **byte-identical** (0 differing bytes; size- and base-exact 0x19cc08); 3566 entries, CANTUNWIND 272, inline 856, extab 2438. The `Layout::Layout` body is size-exact 7072 B without a pad. Wave 123 gave `Level::createShip` its faithful 336 B LSDA record, removing the last record-length delta. |
| .ARM.extab | 156,312 | 156,304 | **-8** | base-exact (`0x1a3b78`), **8 B short** (0x26290). `Level::createShip` is a 328 B record (54 rows, `ttype 0x00`) because of the wave-131 `try`/`catch` RA barrier; `Generator::createMission` is exact at 172 B (wave-121 w7110) and `HangarWindow::render` is exact at 948 B (w6978). 108,023 raw content bytes differ (the 8 B shift plus the createShip landing-pad offsets). |
| .dynsym | 89,248 | 89,248 | 0 | **byte-identical** (5,578 entries, 5,370 defined symbols). Gold emits dynsym in SGI `__gnu_cxx::hash_map` ascending-bucket order (keyed by 1-based Stringpool insertion indices), i.e. first-mention order over the link inputs; the model reproduces our table 5,577/5,577. All 79 versioned imports match. See `_work/reconstructions/dynsym_dynstr_hash_order_w3567.md`, `_work/reconstructions/rel_plt_rel_dyn_ordering_w3616.md` and `_work/reconstructions/gameresource_tu_convergence_w3624.md`. |
| .dynstr | 189,208 | 189,208 | 0 | **byte-identical** (0 differing bytes; 189,208 B). |
| .rel.plt | 29,392 | 29,392 | 0 | **byte-identical** (0 differing bytes; 0x72d0 @ 0x5751c; `SHF_INFO_LINK`/`sh_info=20`). Wave 124 made the `MGame::OnTouchEnd` cold-block PLT order match. |
| .rel.dyn | 28,744 | 28,744 | 0 | **byte-identical** (0 differing bytes; 0x7048 @ 0x504d4). |
| .hash | 38,716 | 38,716 | 0 | **byte-identical** (0 differing bytes; 0x973c @ 0x441a8). |
| .data.rel.ro | 12,264 | 12,264 | **0** | **byte-identical** (0 differing bytes). |
| .shstrtab | 279 | 279 | **0** | **byte-identical**; 27 original names, no extra section. |
| .data | 10,604 | 10,604 | **0** | exact (0x214000/0x296c; original TU order/sizes). |
| .bss | 27,052 | 27,052 | **0** | exact (0x216970/0x69ac; `sh_offset` 0x21596c, the unaligned end of `.data` as bfd writes it). The invented `.aezdata` section is gone, so the RW LOAD and `.bss` VMA now match. |
| .got | 17,164 | 17,164 | 0 | exact (address 0x20fcf4, same slot order). |
| .dynamic | 328 | 328 | 0 | **exact** again after wave 108 moved the table out of `.fini_array` (`-z now` / BIND_NOW exact). |

## The remaining gaps (in priority order)

1. **18 non-byte-exact functions.** 4506 of 4524 compared functions are linked-exact. The worst
   are `Layout::Layout` (49.0%), `Generator::createMission` (84.3%), `LevelScript::process`
   (60.0%), `Level::createShip` (70.3%), `Radar::draw` (74.4%), `MGame::OnTouchEnd` (81.9%),
   `HangarWindow::render` (91.3%), `Level::createMission` (92.3%) and
   `Level::createScene` (92.8%). The `.text` total is exact (0x1337c8); these are
   per-function body/register-allocation deltas, not missing code.
2. **`.rodata` content — exact.** The former 192 B `Status::missionCompleted` rank table was
   removed in wave 121; `.rodata` is byte-identical (269,159 B). The plain switch shape costs
   the function's fuzzy score (92.5 -> 59.4) because clang 7 emits the case bodies in value
   order, not the original's A-then-B order. See
   `_work/reconstructions/status-missioncompleted-wave121-w7113.md`.
3. **EH tables — exact sizes.** `.ARM.exidx` is **byte-identical** (3566 entries, CANTUNWIND
   272, inline 856, extab 2438); `.ARM.extab` is **size- and base-exact** (156,312 B @
   0x1a3b78, 0x26298). `Level::createShip` now has the faithful 336 B cleanup-only record
   (57 rows, `ttype 0xff`, wave 123); `Generator::createMission` is exact at 172 B and
   `HangarWindow::render` is exact at 948 B. 1,446 raw `.ARM.extab` content bytes still differ
   (the createShip landing-pad offsets inside the exact-length records). The
   `Layout::Layout` body is size-exact 7072 B without a pad. See
   `_work/reconstructions/exidx-records_w4237.md` and
   `_work/reconstructions/extab_lsda_scopes_w102_wm103.md`. The w3840 `extern template` for
   `ArrayReleaseClasses<Vector*>` in `SpaceLounge.cpp` restored the original's invoke (the
   original only saw the declaration, so the call was not pruned as `nounwind`) and removed
   the `SpaceLounge::~SpaceLounge` -4 record. The 194 duplicate-address exidx entries are
   fully explained (193 empty-`.text` CANTUNWIND sentinels from `-ffunction-sections` + 1
   zero-size `AbyssEngine::getAppVersion`), and are already reproduced address-for-address.
   Raw prel31 operands still differ with local `.text` layout. See
   `_work/reconstructions/exidx_extab_exact_w3462.md` and
   `_work/reconstructions/exidx_duplicate_address_root_cause_w3565.md`.
4. **Link metadata.** `.dynstr`/`.hash`/`.rel.dyn`/`.rel.plt`/`.dynamic` are byte-identical
   (5,578 dynsym entries, 5,370 defined symbols); the gold hash-table iteration order is
   reproduced. `.dynsym` differs in 661 bytes and `.shstrtab` in 5 bytes only because of the
   extra `.jcr` section. The original's full-RELRO link (`-z now`) is reproduced (BIND_NOW,
   `.got` before `.data` inside `PT_GNU_RELRO`, `.got` at 0x20fcf4). `.data.rel.ro`
   exported-symbol addresses are exact. The GNU build-id is the remaining content hash
   (20 differing bytes) and converges last.

## Done
- **Pass 1: strip** the link output with the original's post-link GNU `strip --strip-all`
  (previously gold `-s`) -> section set matches the original (no .symtab/.strtab/.debug_*) and the
  bfd section-header normalization is exact. bfd rewrites `.shstrtab` into section-header order;
  `tools/reorder_shstrtab.py` restores the gold-Stringpool order from the unstripped link output.
  This also makes the build-id a hash of the unstripped link output, as in the original. Parity
  unaffected (35/0/0); verify/sodiff read .dynsym+.text.

## Reality
Same-hash == completing the decompile to byte-perfection: make the remaining 24 functions exact
and close the last EH/build-id bytes; `.rodata`, `.data` and the dynamic tables are already
byte-identical. Progress is tracked by: byte_exact/linked_exact function counts (verify) and the
section-size deltas above.

## Next levers (tractable, bounded first)
- Byte-exactness campaign: verify.py worst-first, fix near-miss (80-99%) high-value functions first.
- Remove the `.jcr` rank table without losing `Status::missionCompleted`'s 87.0% body order,
  restoring the 27-section table and the byte-identical `.dynsym`/`.shstrtab`.
- ~~Close the last 8 B of `.ARM.extab`: `Level::createShip` -8 (328 vs 336).~~ Done in
  wave 123: the real-`new` body gives the 336 B cleanup-only record, so `.ARM.extab` is
  size- and base-exact and `.ARM.exidx` is byte-identical.
- Implement missing engine functions in batches (math/Vector/Matrix shims are the densest cluster).

## Concrete leads found (for the byte-exactness campaign)

The verify report (cmake-build-match/verify/report.json) has per-function rows: 4500 byte_exact,
4500 linked_exact, 4524 size_match of 4524; pct buckets {100:4500, 90-99:10, 80-89:5, 70-79:6,
<60:3}. 0 function rows size-MISMATCH (the `Layout::Layout` body is size-exact 7072 B without a
pad); 16 size-match >=80% not-linked (the highest-value near-misses).

Systematic clusters (one cause fixes many):
1. **Shader `UpdateMeshData` cluster (resolved).** The former ~15 fns at 86-94%
   (GenericShader/VertexColorShader/BumpShaderV2/4/
   CubeMapping/EnergyShield/...). Diff via direct disasm: the only real (non-relocation) deltas are
   field-offset drift. In `GenericShader`, fields from `uSpecularColor` onward are +4 in the original
   (orig 0x5c/0x60/0x68/.. vs ours 0x58/0x5c/0x64/..) -> our decomp is missing a 4-byte uniform-location
   field between uDiffuseColor and uSpecularColor (or has them mis-ordered). Plus a 1-byte Mesh field
   drift (orig [r4,#9] vs ours [r4,#10]). Fixing the shared base/layout flipped the whole cluster;
   all of these are byte-exact in the current report.
   NOTE: this was layout drift that field_0xNN scanning can't see (semantic-named fields) -- a class of
   drift beyond [[drift-campaign]]. A generalized ASM-offset-diff drift detector would surface all of it.
2. **ParticleSettings_str[401401]={0} (resolved).** The former 392KB zero array (the .rodata
   bloat) was a mis-sized decomp artifact; the real particle-name string table is a few KB. The
   `.rodata` passes recovered the original data, and `.rodata` is now byte-identical (0 differing
   bytes).

## PaintCanvas layout reconstruction (in progress — concrete data)

offdrift + per-method disasm give the original offsets (vs ours) for PaintCanvas fields:
- `engine`        orig 0x34  ours 0x3c   (GetWidth, SetWorldViewMatrix: `ldr [r0,#0x34]`)
- `meshCount`     orig 0x24  ours 0x30   (MeshSetUv: `ldr.w ip,[r0,#0x24]`)
- `meshes`        orig 0x28  ours 0x34   (MeshSetUv: `ldr [r0,#0x28]`)
Deltas: meshCount/meshes are +0xc (our struct has 12 extra bytes before 0x24); engine is +8 (so our
struct is ALSO missing 4 bytes between `meshes` and `engine` — orig has 2 fields there [0x2c,0x30],
we have 1 [`gameOrientation`]). Root: `::Array<AELoadedTexture*> cubeTextures` (12B, count/data_/
capacity_) is MIS-PLACED at 0x20 in ours; the original has it elsewhere. All early fields
(culledCount, quad2dMesh, field_0xc/14/1c, mask2dImage, gameOrientation, cubeTextures) ARE used, so
this is a RE-ORDER, not a deletion.

### Safe methodology for any struct (no-degradation gated)
1. Disassemble every method of the struct (ours+orig), align 1:1 (size-match), map each our-offset to
   its orig-offset; translate offsets->field names via the source. This yields the COMPLETE original
   field order (a full layout reconstructor tool over offdrift is the next tooling step).
2. Reorder/repad the header to match, atomically (the whole struct, not a few fields).
3. Rebuild + relink + verify: keep ONLY if byte_exact/linked_exact counts RISE and parity stays
   35/0/0; otherwise revert. The verify counts are the hard no-degradation gate.

This is the engine for the byte-exactness campaign; each reconstructed struct flips many functions.
Dozens of structs drift (offdrift delta histogram: -8:14, +4:12, +8:11, +32:9, ... across 300 fns).

## GROUND-TRUTH unblock: Ghidra has the original loaded

The original (android_2.0.16_libgof2hdaa.so, 8096 fns, image_base 0x10000) is OPEN in Ghidra (MCP).
`get_struct_layout <Struct>` returns the real field offsets; `decompile_function <addr>` shows real
field usage. This is the authoritative source for the byte-exactness campaign — reconstruct each
struct from Ghidra's layout, not from guesses. The verify byte_exact/linked_exact count remains the
no-degradation gate when applying.

### PaintCanvas — definitive layout (Ghidra get_struct_layout, size 524):
0x0 gravityRotationEnabled(=initialized), 0x4 culledCount, 0x8 field_0x8(=quad2dMesh), 0xc field_0xc,
0x10 field_0x10, 0x14 field_0x14, [0x18 gap], 0x1c field_0x1c, 0x20 field_0x20(int), 0x24
field_0x24(=meshCount), 0x28 field_0x28(=meshes), [0x2c gap=mask2dImage], 0x30 orientation
(=gameOrientation), 0x34 engine, 0x38.. matrices, ... 0x150 images, 0x164 cameras, ...
DECOMP BUG: `ChangeCubeTexture` (decompiled) reads cube count@field_0x10 / data@field_0x14 -> the
cube-texture Array IS field_0x10/0x14/0x18 (count/data/capacity @ 0x10). Our decomp instead made a
SEPARATE `cubeTextures` Array at 0x20, displacing field_0x20/meshCount/meshes by +0xc (root of the
~10 PaintCanvas near-misses, e.g. engine 0x3c-should-be-0x34). FIX: map `cubeTextures`->0x10 (merge
with field_0x10/0x14/0x18 uses), add the standalone `field_0x20` int, shift meshCount/meshes/
gameOrientation/engine back by 0xc. Verify-gate each step.

## Session 2026-11-04 (wave 110 — 20-agent worst-first fleet)

- Gate after harvest: `byte_exact 4500/4524`, `linked_exact 4500/4524`, size 4524/4524,
  data 843/843, imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS.
- **Structural win: the `.jcr` section is gone.** `Status::missionCompleted`'s rank table is
  now the last `.rodata` input (`tools/mission_case_table.cpp`), landing at `0x20b977` (first
  byte past the original `.rodata` image). Result: 27 sections (exact), exact file size
  2,187,328 B, `.dynsym`/`.shstrtab` content byte-identical. Cost: `.text` 95,946 -> 95,999
  (+53 B in that function's literal-pool reference) and 192 B of extra `.rodata` (the table
  itself). This supersedes the wave-109 last-`.rodata` attempt that perturbed `.dynstr`.
- Landed fuzzy improvements:
  - `RocketGun::update` 92.4 -> **93.8** (heading/z-arm reorder).
  - `HangarWindow::render` 88.52 -> **88.71** (normalized `buttonInset` sums, upgrade test swap).
  - `Generator::createMission` 82.1 -> **82.6** (hoist `padA` next to `amount`).
  - `MGame::OnTouchEnd` 78.88 -> **78.92** (`char refinedAny` restores the `lsls` flag test).
  - `Layout::Layout` 48.269 -> **48.338** (`0x9c` int-phi + four int-phi wraps).
- Notes only (measured dead ends, local optima): Level::createShip (332 B LSDA target),
  Radar::draw, Globals::Globals, PlayerEgo::calcCollision, Level::createScene (93.3% pinned to
  sink+mask 35/37), Gun::calcCharacterCollision, Level::createMission, SpaceLounge::drawLounge,
  MenuTouchWindow ctor, MenuTouchWindow::OnTouchBegin + MGame::OnUpdate.
- Remaining 24 non-exact functions (all size-exact): Layout C1/C2 48.3, LevelScript::process
  56.1, Radar::draw 74.0, Level::createShip 74.4, MGame::OnTouchEnd 78.9, Globals C1/C2 80.5,
  Generator::createMission 82.6, Status::missionCompleted 87.0, PlayerEgo::calcCollision 87.6,
  Level::createScene 88.3, HangarWindow::render 88.7, Gun::calcCharacterCollision 90.0,
  Level::createMission 92.1, RocketGun::update 93.8, Hud::draw 94.6, SpaceLounge::drawLounge
  95.5, MenuTouchWindow C1/C2 95.7, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 98.6,
  MGame::OnUpdate 99.9.
- Section deltas: `.text` 95,999; `.ARM.exidx` 1,683; `.ARM.extab` 128,950 (size -4);
  `.rodata` 192 (appended table); build-id 20. Everything else exact, file size exact.

## Session 2026-11-05 (waves 111-112 — worst-first fleet + build archaeology)

- Gate: `byte_exact 4500/4524`, `linked_exact 4500/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS.
- **`.ARM.extab` is now size-exact (`0x26298`).** `Level::createShip` was restored to the
  faithful real-`new` shape (336 B / 57-row LSDA, 66.6%), which cancels `Hud::draw` +4 and
  `Generator::createMission` -4. `.ARM.exidx` differing bytes fell 1,683 -> 1,063.
- Landed fuzzy improvements:
  - `Level::createScene` 88.3 -> **90.5** (loop-variant Vector address + rnd mask 47).
  - `Level::createShip` 74.4 -> 61.9 (332 B device) -> **66.6** (faithful real `new`).
  - `Hud::draw` 94.6 -> **94.9** (second-bar fadeColor if/else).
  - `ModStation` ctor 98.6 -> **99.2** (yaw-first bitcasts, yaw live into far rotation).
  - `MenuTouchWindow` ctor 95.69 -> **96.16** (loop-scoped `i`, id-first tail).
  - `SpaceLounge::drawLounge` 95.5 -> **95.7** (per-file 7:1 branch weight).
  - `Layout::Layout` 48.269 -> **48.426** (0x130/0x134/0x138 branch-local fusion).
  - `HangarWindow::render` 88.52 -> **89.06** (textX+3 operand order, pV placement).
  - `LevelScript::process` 56.09 -> **57.6** (de-nest case-125 state chain, ascending case
    order; supersedes the 57.19 statement-order variant).
- **Build flags are no longer a guess.** Wave-111 research (w5917/w5918/w5919) re-derived the
  recipe from primary evidence: NDK r18b, clang 7.0.2, gold 1.12, API 16, armeabi-v7a,
  Thumb-2/NEONv1/softfp, static libc++, `-Oz -g -std=c++11 -fPIC -frtti -fexceptions
  -fstack-protector-strong -funwind-tables -ffunction-sections`, no `-fdata-sections`, default
  visibility; one plain gold link (`-shared -soname libgof2hdaa.so --build-id -z now
  --exclude-libs=libgcc.a`), then `strip --strip-all` + `.shstrtab` normalization. 1,136
  per-TU flag cells, 128 `-mllvm` cells, 160 combinations and 384 branch-weight cells found
  **no** strict gate improvement. A second official 2.0.16 build is NDK r14 (clang 3.8, gold
  1.11, gnustl), not r18b.
- Remaining 24 non-exact functions (all size-exact): Layout C1/C2 48.4, LevelScript::process
  57.6, Level::createShip 66.6, Radar::draw 74.0, Globals C1/C2 78.8, MGame::OnTouchEnd 78.9,
  Generator::createMission 82.6, Status::missionCompleted 87.0, PlayerEgo::calcCollision 87.6,
  HangarWindow::render 89.1, Gun::calcCharacterCollision 90.0, Level::createScene 90.5,
  Level::createMission 92.1, RocketGun::update 93.8, Hud::draw 94.9, SpaceLounge::drawLounge
  95.7, MenuTouchWindow C1/C2 96.2, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2,
  MGame::OnUpdate 99.9.
- Section deltas: `.text` 97,001; `.ARM.exidx` 1,063; `.ARM.extab` 118,463 (size exact);
  `.rodata` 192 (appended table); build-id 20. Everything else exact, file size exact.

## Session 2026-11-05 (wave 113 — worst-first fleet, continued)

- Gate: `byte_exact 4500/4524`, `linked_exact 4500/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS. `.ARM.extab` stays size-exact.
- Landed fuzzy improvements:
  - `LevelScript::process` 57.6 -> **58.0** (w5904 move subset: case-80/91/92/106/131).
  - `Layout::Layout` 48.34 -> **48.50** (0x2d8 sum condition, 0x184 test order, forced retina
    reload, 0x2d0 round-trip fix).
  - `Level::createShip` 66.6 -> **67.3** (setShipGroup flag + cargo switch).
  - `MenuTouchWindow` ctor 96.16 -> **97.21** (narrow row2 local, `i != 10` loop).
  - `HangarWindow::render` 89.06 -> **89.34** (free-credits Layout alias steers regalloc).
  - `Level::createMission` 92.094 -> **92.145** (setScaling receiver/position sweep).
  - `Globals` kept 78.8 but now has **both** the faithful 31/31 GOT-pool order and the
    `movw #583` hoist at index 34 in one store (late-block reorder).
- `.text` differing bytes fell 97,001 -> **94,443**. All other section deltas unchanged:
  `.ARM.exidx` 1,063; `.ARM.extab` 118,463 (size exact); `.rodata` 192 (appended table);
  build-id 20. 27 sections, exact file size.
- Remaining 24 non-exact functions (all size-exact): Layout C1/C2 48.5, LevelScript::process
  58.0, Level::createShip 67.3, Radar::draw 74.0, Globals C1/C2 78.8, MGame::OnTouchEnd 78.9,
  Generator::createMission 82.6, Status::missionCompleted 87.0, PlayerEgo::calcCollision 87.6,
  HangarWindow::render 89.3, Gun::calcCharacterCollision 90.0, Level::createScene 90.5,
  Level::createMission 92.1, RocketGun::update 93.8, Hud::draw 94.9, SpaceLounge::drawLounge
  95.7, MenuTouchWindow C1/C2 97.2, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2,
  MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 114 — worst-first fleet, continued)

- Gate: `byte_exact 4500/4524`, `linked_exact 4500/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS. `.ARM.extab` stays size-exact.
- Landed fuzzy improvements:
  - `HangarWindow::render` 89.3 -> **91.2** (faithful `selectedItem->ship` + `b`, `bool visible`).
  - `Gun::calcCharacterCollision` 90.0 -> **92.8** (named `playerPosition` temp, `hitFlags` at
    k-loop tail).
  - `Status::missionCompleted` 87.0 -> **88.3** (37-entry rank table + body-size fixes land the
    first body at 0xfe and latch at 0x522).
  - `Generator::createMission` 82.6 -> **83.3** (bitmask branch hint fixes spill-slot allocation).
  - `Level::createShip` 67.3 -> **67.4** (hitpoint type-chain source order 0x2c/0x31/0x33).
  - `LevelScript::process` 58.00 -> **58.02** (case-157 decl order + adjacent-statement swaps,
    cached player at two sites).
  - `MenuTouchWindow` ctor 97.214 -> **97.244** (signed loop bound restores the `bcc` backedge).
- `.text` differing bytes fell 94,443 -> **92,493**. All other section deltas unchanged:
  `.ARM.exidx` 1,063; `.ARM.extab` 118,463 (size exact); `.rodata` 192 (appended table);
  build-id 20. 27 sections, exact file size.
- Remaining 24 non-exact functions (all size-exact): Layout C1/C2 48.5, LevelScript::process
  58.0, Level::createShip 67.4, Radar::draw 74.0, Globals C1/C2 78.8, MGame::OnTouchEnd 78.9,
  Generator::createMission 83.3, PlayerEgo::calcCollision 87.6, Status::missionCompleted 88.3,
  Level::createScene 90.5, HangarWindow::render 91.2, Level::createMission 92.1,
  Gun::calcCharacterCollision 92.8, RocketGun::update 93.8, Hud::draw 94.9,
  SpaceLounge::drawLounge 95.7, MenuTouchWindow C1/C2 97.2, MenuTouchWindow::OnTouchBegin 98.1,
  ModStation C1/C2 99.2, MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 115 — first new byte-exact function)

- Gate: `byte_exact 4501/4524`, `linked_exact 4501/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS.
- **`RocketGun::update` is now byte-exact (100.0%)** — the first function to leave the
  non-exact set since the campaign began: `trailMatrices` `operator[]` + faithful shape.
  `byte_exact` moved 4500 -> **4501**.
- Landed fuzzy improvements:
  - `Status::missionCompleted` 88.3 -> **91.5** (rank table re-based to 0..39, dispatch drops
    the subtraction).
  - `Hud::draw` 94.86 -> **95.46** (docking-bar decl order/types + second-bar decl order merge
    EH pad; the Hud LSDA record is now the original's exact 476 B).
  - `Globals` 78.76 -> **80.5** (`status` in the early pointer block + the 80.5 late order).
  - `Level::createScene` 90.53 -> **90.72** (address-expression scheduling fix).
  - `LevelScript::process` 58.02 -> **58.10** (case-92 state-2/state-4 decl and statement
    order).
  - `Gun::calcCharacterCollision` 92.8 -> **93.4** (drop `dmg` hoist, `hitFlags` at scatter
    head, volatile `0.2f` read).
- `.text` differing bytes fell 92,493 -> **91,907**. `.ARM.extab` is 4 B short again
  (`0x26294`), but for a *cleaner* reason: `Hud::draw` is now exact, so
  `Generator::createMission` (168 B vs 172 B) is the only remaining record delta.
  `.ARM.exidx` 1,663; `.rodata` 192; build-id 20. 27 sections, exact file size.
- Remaining 23 non-exact functions (all size-exact): Layout C1/C2 48.5, LevelScript::process
  58.1, Level::createShip 67.4, Radar::draw 74.0, MGame::OnTouchEnd 78.9, Globals C1/C2 80.5,
  Generator::createMission 83.3, PlayerEgo::calcCollision 87.6, Level::createScene 90.7,
  HangarWindow::render 91.2, Status::missionCompleted 91.5, Level::createMission 92.1,
  Gun::calcCharacterCollision 93.4, Hud::draw 95.5, SpaceLounge::drawLounge 95.7,
  MenuTouchWindow C1/C2 97.2, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2,
  MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 116 — Layout pool audit, MenuTouchWindow 99.0)

- Gate: `byte_exact 4501/4524`, `linked_exact 4501/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS.
- Landed fuzzy improvements:
  - `MenuTouchWindow` ctor 97.24 -> **99.04** (faithful cinematic block + reassociation masks).
  - `LevelScript::process` 58.05 -> **58.20** (case-91/92 statement order + mission-40 state-6
    epilogue order).
  - `Layout::Layout` 48.545 -> **48.60** (faithful int-store typing + `field_0x2c8` float
    round-trip).
  - `Status::missionCompleted` 91.5 -> **91.9** (rank table re-based to the original TBH slot
    pattern).
  - `Hud::draw` 95.46 -> **95.61** (docking-bar total/barW widths).
- `.text` differing bytes fell 91,907 -> **88,975**. `.ARM.extab` remains 4 B short
  (`0x26294`), solely because `Generator::createMission`'s record is 168 B vs the original
  172 B; `.ARM.exidx` 1,663; `.rodata` 192; build-id 20. 27 sections, exact file size.
- Remaining 23 non-exact functions (all size-exact): Layout C1/C2 48.6, LevelScript::process
  58.2, Level::createShip 67.4, Radar::draw 74.0, MGame::OnTouchEnd 78.9, Globals C1/C2 80.5,
  Generator::createMission 83.3, PlayerEgo::calcCollision 87.6, Level::createScene 90.7,
  HangarWindow::render 91.2, Status::missionCompleted 91.9, Level::createMission 92.1,
  Gun::calcCharacterCollision 93.4, Hud::draw 95.6, SpaceLounge::drawLounge 95.7,
  MenuTouchWindow::OnTouchBegin 98.1, MenuTouchWindow C1/C2 99.0, ModStation C1/C2 99.2,
  MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 117 — createShip 73.5, MenuTouchWindow 99.7)

- Gate: `byte_exact 4501/4524`, `linked_exact 4501/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS.
- Landed fuzzy improvements:
  - `Level::createShip` 67.4 -> **73.5** (wave-89 placement-new + `try` shape with the correct
    ctor/declaration order). Trade-off: its LSDA shrinks 336 -> 328 B, so `.ARM.extab` goes
    from 4 B short to 12 B short.
  - `MenuTouchWindow` ctor 99.04 -> **99.70** (`sliders[2]` original left grouping + inner
    mask).
  - `Status::missionCompleted` 91.9 -> **92.5** (TBH-target LCS re-tune of the rank labels).
  - `Hud::draw` 95.61 -> **95.90** (`Globals::Canvas` null-test anchor).
  - `Level::createMission` 92.15 -> **92.29** (volatile `size()` latch in `case4Tail`).
  - `MGame::OnTouchEnd` 78.92 -> **79.05** (campaign-first dispatch + campaign reuse +
    `hasWon() == true`).
  - `LevelScript::process` 58.19 -> **58.20** (case-78 s12 store after return; case-154
    state-1 epilogue order).
- `Globals` 80.5 -> **78.8** but with stores 96-103 byte-exact (volatile settings tail) — a
  faithfulness-for-fuzzy trade, like the pool-faithful variant.
- `.text` differing bytes 88,975 -> **89,185** (the createShip placement shape is worse in raw
  bytes even though its fuzzy score is +6.1). `.ARM.extab` 12 B short; `.ARM.exidx` 1,704;
  `.rodata` 192; build-id 20. 27 sections, exact file size.
- Remaining 23 non-exact functions (all size-exact): Layout C1/C2 48.6, LevelScript::process
  58.2, Level::createShip 73.5, Radar::draw 74.0, Globals C1/C2 78.8, MGame::OnTouchEnd 79.0,
  Generator::createMission 83.3, PlayerEgo::calcCollision 87.6, Level::createScene 90.7,
  HangarWindow::render 91.2, Level::createMission 92.3, Status::missionCompleted 92.5,
  Gun::calcCharacterCollision 93.4, SpaceLounge::drawLounge 95.7, Hud::draw 95.9,
  MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2, MenuTouchWindow C1/C2 99.7,
  MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 118 — Globals exact, Generator LSDA exact)

- Gate: `byte_exact 4503/4524`, `linked_exact 4503/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.93**. RATCHET PASS.
- **`Globals::Globals` C1/C2 are now byte-exact (100.0%)** — restoring the original late-block
  source order. `byte_exact` moved 4501 -> **4503**.
- **`Generator::createMission` now has the original's exact 172 B LSDA record** (volatile
  `type` + base split + tail hint). This costs fuzzy (83.3 -> 50.1) but removes the last
  non-createShip `.ARM.extab` delta; `.ARM.extab` is now 8 B short solely because of
  `Level::createShip` (328 vs 336).
- Other landed gains:
  - `MGame::OnTouchEnd` 79.05 -> **80.14** (merge 96/127, refetch campaign mission, `== true`
    select).
  - `Hud::draw` 95.89 -> **95.99** (mission-panel `ldrd` operand order).
  - `Layout::Layout` 48.555 -> **48.595** (`rewardBoxHeight` ternary + 0x138 double round
    trip).
- `Level::createShip` stays 73.5 (placement-new + `try`, LSDA 328 B).
- `.text` differing bytes 89,185 -> **89,836** (the Generator LSDA fix reshapes the body).
  `.ARM.exidx` 1,327; `.ARM.extab` 108,063 (8 B short); `.rodata` 192; build-id 20. 27
  sections, exact file size.
- Remaining 21 non-exact functions (all size-exact): Layout C1/C2 48.6,
  Generator::createMission 50.1, LevelScript::process 58.2, Level::createShip 73.5,
  Radar::draw 74.0, MGame::OnTouchEnd 80.1, PlayerEgo::calcCollision 87.6,
  Level::createScene 90.7, HangarWindow::render 91.2, Level::createMission 92.3,
  Status::missionCompleted 92.5, Gun::calcCharacterCollision 93.4, SpaceLounge::drawLounge
  95.7, Hud::draw 96.0, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2,
  MenuTouchWindow C1/C2 99.7, MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 119 — MenuTouchWindow ctor exact)

- Gate: `byte_exact 4505/4524`, `linked_exact 4505/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.93**. RATCHET PASS.
- **`MenuTouchWindow::MenuTouchWindow` C1/C2 are now byte-exact (100.0%)** — hoisting row2
  before `addButton(19)`. `byte_exact` moved 4503 -> **4505**.
- `Level::createScene` 90.724 -> **92.757** (`-g` Vector address, y/z/x store order, rnd mask).
- `Radar::draw` 74.030 -> **74.427** (local `String` label in the landmark `i == 0` arm).
- `PlayerEgo::calcCollision` 87.644 -> **95.2** (A2+visible size guards, swapped compare,
  tail pad; 968 B).
- `Level::createShip` 73.5 -> **74.4** (tuned spawn slot-map), then the faithful
  real-`new` shape landed at **70.2** with the exact 336 B LSDA (the original really uses
  real `new`; `.ARM.extab` is now only 4 B short, from Generator).
- `Generator::createMission` 53.3 -> **83.5** (faithful shape + pad store before
  `fromSystemPtr`); its LSDA is 168 B, so `.ARM.extab` is **12 B short** again.
- `LevelScript::process` 58.2 -> **58.5** — merged w6603/w6604/w6704 reorders, body exactly
  57232 B (a 57240 B merge collapses byte_exact to 2740).
- `.text` differing bytes 89,836 -> **88,009**. `.ARM.exidx` 1,663; `.ARM.extab` 156,308
  (4 B short, Generator only); `.rodata` 192; build-id 20. 27 sections, exact file size.
- Remaining 19 non-exact functions (all size-exact): Layout C1/C2 48.6,
  Generator::createMission 83.5, LevelScript::process 58.5, Level::createShip 73.5,
  Radar::draw 74.4, MGame::OnTouchEnd 80.1, HangarWindow::render 91.2,
  Level::createMission 92.3, Status::missionCompleted 92.5, Level::createScene 92.8,
  Gun::calcCharacterCollision 93.4, PlayerEgo::calcCollision 95.2, SpaceLounge::drawLounge 95.7,
  Hud::draw 96.0, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2,
  MGame::OnUpdate 99.9.
- Build provenance re-audited (w6719): 72-variant global matrix + 378-cell per-TU matrix,
  no flag beats 4505; `-Oz` and `-g2` are uniquely correct; the residual is source shape.
