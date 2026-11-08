# Radar::draw wave 149 (w9607) — the play-site table is 7/7, not 8/7; dropping the invented otherLock expect is a real win

**Status:** source change landed, 2026-11-08. Worktree `wt/w9607`, based on `master`
(`7eed2966e`, wave 149). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Baseline: original 9,908 B; ours 9,908 B; raw same-address diff **8,712**;
instruction-index alignment **1,657**; frame `sub sp,#296`; **7** `blx play`.
After: raw **8,461** (-251), alignment **1,646** (-11), size still 9,908 B,
frame 296, 7 plays, `.text` differing bytes **86,449 -> 86,198** (-251).
`.ARM.extab` 1,309 (size exact 156,312), `.rel.plt` 4, `.note.gnu.build-id` 20,
`.rodata`/`.data`/`.bss`/`.ARM.exidx` byte-identical. Gate: byte 4507, linked 4507,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

Read with `radar-draw-wave149-w9501.md`, `radar-draw-wave148-w9401.md`,
`radar-draw-wave147-w9402.md`, `radar-draw-wave147-w9309.md`.

## 1. The play-site table: the original emits SEVEN `blx play`, not eight

The mission premise ("original has 8 `FModSound::play` sites, ours 7") is not what
the shipped binary contains. Linear objdump of the whole `Radar::draw` body
(`0x12e05c..0x130714`, 9,908 B) finds exactly **7** `blx 61548
<_ZN9FModSound4play...>` sites in the original and **7** in ours. The wave-147
w9402 note and `docs/IDENTICALITY.md` both already say "7 like the original"; the
"8" is stale. (There *are* 23 source-level `play(...)` statements: 5x `play(26)`
plus 10 in the `field_0x1bc >= 1` block and 8 in the `field_0x1bc < 1` block.
clang merges them into 5+1+1 emitted call sites. If "8" was counted from the
`< 1` music block, ours also has 8 statements there — the emitted site count is
what the binary proves.)

| # | original | ours (after) | id(s) | source statement |
|---|---|---|---|---|
| 1 | `12f88e` | `12f85e` | 26 | gas lock, `Radar.cpp:1038` (`!lockConfirmed && !isPlaying(0)`) |
| 2 | `12ff82` | `12ff2e` | 26 | asteroid lock, `Radar.cpp:1209` (same guard) |
| 3 | `1300a2` | `1302dc` | 26 | enemy lock, `Radar.cpp:876` (`lockedEnemy != dockNavPtr`) |
| 4 | `1302d6` | `1301d6` | 136,140,141,142,149,150,151 (+145,143) | shared tail, `field_0x1bc >= 1` block |
| 5 | `1303ea` | `1303ee` | 26 | station lock, `Radar.cpp:501` (`!lockConfirmed`) |
| 6 | `13046c` | `130474` | 26 | planet lock, `Radar.cpp:633` (`!lockConfirmed`) |
| 7 | `13069c` | `13069c` | 146,147,2241,2242,148,152,`kRaceMusic[race]` (+145,143) | shared tail, `field_0x1bc < 1` block |

Site 4 is reached by `b.n 1302d2` from the 136/140/141/142/149/150/151 branches
*and* from the `< 1` block's `play(145)`/`play(143)` (`1303ba`, `1305ac`).
Site 7 is reached from `1305d8` (146), `130608` (147), `13066a` (2241/2242 ite),
`13067e` (152), `130688` (148) and `130694` (`kRaceMusic[race]`). Both sides have
the same id multiset and the same 2 `FModSound::stop` sites (`1302a4`, `13038e`).
The only layout difference is that the original places the enemy `play(26)` block
(`1300a2`) *before* the music tail (`1302d6`), while the baseline places it after
(`1302d0`). That is a block-ordering/branch-weight symptom, not a missing call.

## 2. New hypothesis that landed: the `otherLock` expect is an invented pad

`__builtin_expect(this->lockedAsteroid == nullptr && this->lockedGasCloud == nullptr
&& this->field_0x1c == nullptr, 0)` was added by wave 143 (w8807) as a metric pad,
not witnessed. Removing it (plain `if`) changes the block weights enough to move
the hot/cold split and drops **both** metrics at the same 9,908 B:

| variant | usize | raw | align | frame | plays |
|---|---:|---:|---:|---:|---:|
| baseline (HEAD) | 9908 | 8712 | 1657 | 296 | 7 |
| **A: drop otherLock expect (landed)** | **9908** | **8461** | **1646** | 296 | 7 |
| A + split into 3 expects (wave-143 form) | 9916 | 8505 | 1668 | 296 | 7 |

Full 16-combination sweep of the four remaining source-level expects
(A = otherLock `411`, B = `onScreen` `431`, C = `screenX` `432`,
D = campaign mission `553`; `C` is a no-op on this base):

| mask | usize | raw | align |
|---|---:|---:|---:|
| - | 9908 | 8712 | 1657 |
| **A** | **9908** | **8461** | **1646** |
| C, AC | 9908 | 8461 | 1646 |
| B, BC | 9832 | 9489 | 1843 |
| AB, ABC | 9832 | 9513 | 1834 |
| D, CD | 9956 | 8308 | 1656 |
| AD, ACD | 9956 | 8852 | 1647 |
| BD, BCD | 9944 | 8814 | 1647 |
| ABD, ABCD | 9944 | 8312 | 1636 |

`D` (drop the campaign-mission expect) has the best raw (8,308) and `ABD` the best
alignment (1,636), but both grow the function (+48 / +36 B), which shifts every
later `.text` symbol and fails the size gate. `A` is the only variant that keeps
9,908 B and improves both metrics. It also moves the gas/asteroid play sites
8 B / 84 B closer to the original (`12f85e` vs `12f88e`, `12ff2e` vs `12ff82`).

## 3. Remaining residual

The enemy `play(26)` block is still emitted after the music tail
(`1302dc` vs original `1300a2`), and `sl`/`r9`/`fp` still rotate differently in
the prologue (w9402 §1). The `D`-family results show the campaign expect is also
part of the layout lever: a variant that drops D *and* finds a witnessed 48 B
shrink would land raw ~8,308 with the original block order. Candidate 48 B
sources were not found inside the function this pass (the function is already
size-exact; any shrink must come from a witnessed dead local or a merged tail).

## 4. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)   parity: 5370/5370
sections: .text 86,198 differing bytes (was 86,449), .ARM.extab 1,309 (size
exact 156,312), .rel.plt 4, .note.gnu.build-id 20; all 27 section sizes exact.
```
