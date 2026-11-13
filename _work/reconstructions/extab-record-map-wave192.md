# `.ARM.extab` record map — wave 192, 2026-11-13

Tool: `_work/reconstructions/extab_map.py` (self-contained; run from the repo root).
It parses `.ARM.exidx` (8-byte entries, prel31) and `.ARM.extab` from both binaries,
derives each LSDA record's byte range from the sorted extab offset set, and diffs the
records at the same offsets. Base master `c3d1ae5f2`.

## Result

`.ARM.extab` is size- and base-exact (156,312 B @ `0x1a3b78`) and its 3,294 records sit
at **identical offsets** in both files. The 597 differing bytes are confined to exactly
**9 records**, and those 9 records are exactly 9 of the 14 residual table rows:

| differing B | record B | function |
|---:|---:|---|
| 186 | 348 | `LevelScript::process` (`0x137d18`) |
| 144 | 956 | `Level::createMission` (`0x0affd8`) |
| 93 | 336 | `Level::createShip` (`0x0c0214`) |
| 88 | 132 | `Radar::draw` (`0x12e050`) |
| 36 | 948 | `HangarWindow::render` (`0x1490d8`) |
| 27 | 280 | `MGame::OnTouchEnd` (`0x17a144`) |
| 14 | 168 | `SpaceLounge::drawLounge` (`0x170d38`) |
| 7 | 256 | `Radar` C1/C2 (`0x12d484`) |
| 2 | 112 | `ModStation` C1/C2 (`0x0d52d0`) |
| **597** | | **total** |

The other 5 residual rows (`Layout` C1/C2, `Gun::calcCharacterCollision`, `Hud::draw`,
`MenuTouchWindow::OnTouchBegin`, `MGame::OnUpdate`) have **byte-identical LSDA records** —
their residual is `.text` only.

## Consequences

1. **There is no hidden extab bug.** Every differing extab byte belongs to a known
   residual function; fixing those functions is the only way to close `.ARM.extab`.
2. **`Level::createShip` (w13884/w13891/w13898).** Its record is 336 B in both binaries
   today, with 93 bytes differing. Dropping the invented `EmpGuard` makes it 332 B
   (`.ARM.extab` 156,312 -> 156,308), so the original's 336 B record has **4 B more than
   the EmpGuard-free version**. Since every other record's offset is already correct, the
   +4 B must come from **createShip's own LSDA**, not from a compensating change in
   another function: one extra 4-byte element (an action record / a landing-pad entry),
   i.e. the original has one more catch/cleanup action than the EmpGuard-free source.
   The EmpGuard version matching 336 is a size coincidence — its content still differs
   in 93 bytes. So the landing shape is "EmpGuard-free code + one extra 4-byte LSDA
   element", not "EmpGuard-free code + a +4 B patch elsewhere".
3. `LevelScript::process` owns the largest extab residual (186 B / 348 B record), so its
   LSDA will only converge together with its `.text` (many try/catch scopes).

## Companion check: `.text` is fully attributed too

The 14 residual table rows sum to exactly the `.text` differing-byte count:
53,208 + 6,909 + 5,614 + 4,265 + 3,544 + 1,395 + 1,190 + 45 + 463 + 175 + 19
+ 11 + 28 + 16 = **76,882**, which is the current `.text` contentdiff. So there is no
hidden `.text` delta outside the table either (no padding/alignment residue).
