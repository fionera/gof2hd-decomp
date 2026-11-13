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

## Caveat: LSDA bytes are partly a *consequence* of the `.text` diff

A call-site entry encodes `start`, `length`, `landing pad` (a prel31 code address) and
`action`. The landing-pad field therefore changes whenever the function's code shifts,
even if the source `try`/`catch` structure is identical. Evidence: landing the w13905
`for`-loop change (`41353c5bd`) moved `LevelScript::process` `.text` by 5 bytes and left
its LSDA diff at exactly **186/348** — the record did not move at all, because the loop
is not inside a try scope. Conversely, a scope change moves both.

So the LSDA diff is **not an independent target**: expect most of the 597 bytes to
disappear automatically once the 9 functions' `.text` matches. What is *structurally*
meaningful is the record **size** and the **number/shape of call-site entries** — a
different entry count means a real missing/extra `try` scope. Use the byte count only as
a hint, not as a score.

## `HangarWindow::render` LSDA is `.text`-derived (w13912)

> Numbers in this section are from base `.text` 76,882 / `.ARM.extab` 597.
> Current master is `.text` 76,911 / `.ARM.extab` 552 (w13911 landed the
> Radar extab -45), so re-measure before relying on the absolute values.

Record `0x1c30fc` (948 B, Generic, personality `0x5eb60`), LSDA proper at record
offset `0x0c`, CS table `0x1c3111..0x1c34b0`, **156 call-site trips in both
binaries**. The action table is **identical** (every differing trip has
`action = 0`); all 15 differing trips are pure ±2 B start / landing-pad shifts
that track the `.text` shape:

| trips | orig | ours (HEAD, raw 1,395) |
|---|---|---|
| 39 | `0x9cc..0xa62` | `0x9cc..0xa78` |
| 40 | `0xa88..0xb14` | `0xa80..0xb16` |
| 41–44, 98–100, 130–131, 133–135 | — | ±2 B pad/start shifts |

There is **no `try`/`catch` in `HangarWindow.cpp`**: every landing pad is a
`String`-temporary destructor cleanup (`add r0,sp,#N` with N in 216…324). The
tail pads (`0x14b5e0..0x14b6a6`) are the same 12 destructor chains on both
sides; only their entry addresses shift.

**Shape driver:** the original hoists the ship-guard `ldrd r5,r4,[sp,#132]`
to `0x149b20` and reloads it after `Status::getShip` at `0x149b3c`. Our source
keeps `setVisible` -> `int upgTab = tab;` -> `draw` -> `setPosition`.

**Experiment (not landed).** Hoisting `int upgTab = tab;` above the
`if (tab == 0) {} else if (...)` guard:
- raw **1,395 -> 1,193**, aligned **396 -> 400**, size **9,700 exact**, insn 3,733/3,734.
- `.ARM.extab` diff **597 -> 1,238 B**; record region 36 -> 677 B; 34 differing
  trips (vs 15). Trip 39 shrinks `0x96 -> 0x76`, trip 40 becomes
  `0xa62/0xb2`; new ±2/±4 shifts at trips 17, 46, 65, 101–122.
- Variant keeping the `upgradeMode` block *inside* the guard body (using the
  hoisted `upgTab`): raw 1,193 / aligned 400 / size exact, record region
  **676 B** — proving the trip-39/40 boundary move is caused by the `upgTab`
  hoist itself, not by moving the block.

Net loss by the "extab growth must not exceed `.text` savings" rule
(+641 extab vs -202 text), so HEAD stays at raw 1,395 / aligned 396.
