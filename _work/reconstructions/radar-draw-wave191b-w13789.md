# Radar::draw wave 191b (w13789) — `bool missionBlocksPlanets` + two hint removals: raw 7,088 → 7,044, aligned 1,346 → 1,329

**Status:** complete, 2026-11-13. Worktree `wt/w13789`, branch `wave191b-w13789`,
base `master` `78dc73c0a`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13784.md` (the previous Radar pass) and the
wave-191b `docs/IDENTICALITY.md` update section.

**Landed:** one source-type correction plus two independent
`__builtin_expect`-hint removals, size-neutral (9,908 B exact), and a strict
win on both raw and aligned.

| pass | change | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---|---:|---:|---:|---:|---:|
| base (session start) | — | 7,088 | 1,346 | 9,908 | 75,749 | 590 |
| 1 (`w13789`) | `int missionBlocksPlanets` → `bool` | 7,032 | 1,351 | 9,908 | 75,693 | 590 |
| 2 (`w13789`) | + drop `cargo == nullptr` hint (h17) | 7,038 | 1,336 | 9,908 | 75,699 | 590 |
| 3 (`w13789`) | + drop `(current != 151 && alwaysEnemySeen)` hint (h19) | **7,044** | **1,329** | **9,908** | **75,705** | **590** |

`.ARM.exidx` 0, `.rodata` 0, `.rel.plt` byte-identical. Full `check` gate green
after the change: byte_exact 4508, linked_exact 4508, size-exact 4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
lint CLEAN, parity 5370/5370.

## 1. Landed changes

* `missionBlocksPlanets` is only ever assigned `false`/`true` (line 351 stores
  `mission->getType() != 172`), so `bool` is the faithful source type.  The
  narrower type changes the register class/width at the two use sites and is a
  strict raw win on its own (7,088 → 7,032), at the cost of +5 aligned.
* **h17** — `if (__builtin_expect(cargo == nullptr, 0))` in the locked-enemy
  cargo block: the original has no branch-weight hint here.
* **h19** — `__builtin_expect((current != 151 && alwaysEnemySeen), 0)` in the
  sound-stop condition: likewise unhinted in the original.

The joint combination is the best size-exact candidate measured: h17+h19 alone
(without the `bool`) reaches aligned 1,324 but raw 7,100, a regression in the
primary hard count.

## 2. Root cause of the dominant residual (confirmed, unchanged)

Original: `campaign` lives in `sl` (`mov sl,r0` @0x12e0f8 →
`cmp.w sl,#2` @0x12e38a), `missionBlocksPlanets` in `fp`, `marked` spilled at
`sp+0x84`, `turretMode` 0x94, `planetHit` 0x98.  Ours spills `campaign` to
`sp+0x90` and reloads it 4× at the `campaign >= 2` test; `stations` sits at
0x84 and `marked` stays in `fp`.  The whole downstream stack-slot cascade
follows from that one spill.  `campaign` has exactly one source use (line 535)
yet the original keeps it in a callee-saved register across ~700 bytes — a
live-range/pressure difference, not a source-order one.  `int marked` /
`int planetHit` / `int turretMode` variants are size-fatal (9,912 / 9,928 /
9,856), as are `bool otherLock`, `campaignLater`, `campaignFirst` and a
function-scope `markedFuncScope` rewrite.

## 3. Rejected variants (all size-exact unless marked)

| variant | raw | aligned |
|---|---:|---:|
| `bool mBP` + h17 + h0 | 7,039 | 1,336 |
| `bool mBP` + h17 + h7 | 7,046 | 1,336 |
| `bool mBP` + h17 + h19 + h7 | 7,052 | 1,329 |
| `bool mBP` + h17 + h19 + h4/h12/h18 | 7,044 | 1,329/1,330 |
| `bool mBP` + h17 + h19 + h20 (boss) | 7,110 | 1,354 |
| `campaignReady` / `campaign > 1` / `mb2` rewrite | 7,032 | 1,351 |
| nohints (all 22) | 9,130 | 1,506 (size 9,960 fatal) |
