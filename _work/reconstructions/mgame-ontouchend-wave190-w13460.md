# MGame::OnTouchEnd — wave 190 (w13460): campaign-81/134 operand swaps (raw 2895 -> 2889)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13460`, branch
`wave190-w13460`, base master `83d52fe52` (+ w13450 note). Scratch
`/home/debian/GalaxyOnFire2/scratch/w13460`.

Prior notes: `mgame-ontouchend-wave190-w13447.md` (campaign-65 getter/statement
order), `-w13441.md`, `-w13425.md` (campaign-chain operand swaps), `-w13421.md`,
`-w13405.md` (chain order / IR mining loop / `__builtin_expect` sweep),
`-w13401.md`, `-w13392.md`, `-w13378.md`, `-w13374.md`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,895 | **2,889** |
| aligned (instruction) diff | 1,712 | 1,712 |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 72,257 | **72,251** |
| `.ARM.extab` size / diff | 156,312 / 584 | 156,312 / **584** |
| `.note.gnu.build-id` diff | 20 | 20 |
| total (text+extab+build-id) | 72,861 | **72,855** |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| file size | 2,187,328 | 2,187,328 |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 section headers.

## Win — swap `isCampaignMission()` behind the mission-id test for arms 81 and 134

The w13425 pass swapped eight chain arms to put the
`getCurrentCampaignMission() == N` literal test *first* and
`mission->isCampaignMission()` second.  On the **current** base (after
w13441/w13447's block-65 reorders and w13450) the operand-order landscape has
changed: arms 81 and 134 still had the original `isCampaignMission()`-first
spelling, and swapping them is now a size-exact raw win.

```cpp
// arm 81
} else if (Globals::status->getCurrentCampaignMission() == 81 && mission->isCampaignMission()) {
// arm 134
} else if (Globals::status->getCurrentCampaignMission() == 134 && mission->isCampaignMission()) {
```

Both operands are side-effect-free predicates and `&&` short-circuits either
way, so this is behaviour-preserving.  Function stays exactly 8,080 B,
`.rel.plt` byte-identical, `.ARM.extab` content unchanged (584), and raw drops
**2,895 -> 2,889** (`.text` 72,257 -> 72,251), a net -6 total.

Marginal values measured from the plain base (each row adds one more swap):

| swap added | raw | `.text` | extab |
|---|---:|---:|---:|
| 81 alone | 2,891 | 72,253 | 584 |
| 134 alone | 2,893 | 72,255 | 584 |
| 81 + 134 | **2,889** | **72,251** | 584 |

Neutral toggles (120, 126, 155) do not change raw; adding 162/42/65/110 (+1
each), 100 (+102), 95 (+16), 144/161 (+18) or the 22 arm (size-fatal 8,084) is
worse.  This is the local optimum of the single-arm operand-swap family on the
current base.

## Re-measured chain-order space (all reverted)

- Full 153 pair-swap sweep from the current base: best **2,893** (`15 <-> 162`,
  `.text` 72,255) but `.ARM.extab` diff grows 584 -> **591** (the MGame LSDA
  call-site entry gains a 2-byte ULEB, shifting the rest of the table), so it
  is a net **+5** and was not landed alone.
  On top of the 81+134 win it measures raw **2,887** / `.text` 72,249 /
  extab 591 — a net -1; left unlanded here because the extab regression is
  larger than the marginal text gain.
- Full 306 block-move sweep: no order beats the pair-swap result; best
  size-exact raw 2,893 (same `15 <-> 162`).
- Full tail 3-cycle sweep (arms 96/100/95/110/81/134/144/126/155/161, 240
  builds): base is a strict local minimum; no size-exact order beats 2,895.
- `instantAction`-first won-block restructure (the original's emitted
  `isInstantActionMission()`-first shape): size **8,084 (+4)**, raw **4,715**,
  extab 1,304 — the chain start shifts and the base body packing breaks.
- Re-tested String-schedule forms (`String t("t ")` before the expression,
  named head temp, `report = report + ...`, `String("t ", false)`): all either
  change size (8,064-8,084) or are raw-neutral / worse; the 22-byte
  `add.w r8, sp, #80` hoist at `+0x4a4` remains.

## Residual map (unchanged)

First differing byte is still `count >= 0` emitting `bgt.n` where the original
emits `beq.n` at `+0x48b`; then the 22-byte String-ctor schedule at `+0x4a4`
and its literal-pool word at `+0x6b4`.  The dominant residual remains the
campaign-chain block placement from `+0xe3e`.

## Reproduction

Scratch (`/home/debian/GalaxyOnFire2/scratch/w13460/`): `chain.py` (arm
parser/renderer), `measure2.py` (raw/aligned/size + text/extab/build-id diff),
`opsweep.py` (per-arm operand-order toggle), `combo.py` (toggle combinations),
`search.py` / `moves.py` / `cyc3.py` (chain-order sweeps), `strvar.py`
(String-schedule forms).  All builds `ninja -j2` under `ulimit -v 12000000`.
