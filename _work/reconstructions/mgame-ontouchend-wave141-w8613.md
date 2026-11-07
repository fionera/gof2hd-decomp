# MGame::OnTouchEnd — wave 141 (w8613): block-layout map of the campaign chain; `cutsceneActive == true` is a 4-byte-short near miss

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w8613`,
base `e18e0c2a4`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w8613`. **No source change landed** (base restored; full gate green:
byte 4507, linked 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN).

Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.
Baseline reproduced: **raw same-address diff 5816, size 8080/8080**.

## 1. Where the 5816 bytes actually are: one block move, not 5816 independent errors

Instruction-sequence alignment (`difflib` over normalised mnemonics) plus a
delta histogram of the aligned pairs gives:

| delta (orig addr − ours addr) | orig bytes |
|---|---:|
| `0x0` | 1008 |
| `+0x4` | 942 |
| unmatched (`X`) | 769 |
| everything else | 5361 |

So ~1950 B are already at a fixed 0/4-byte offset and the rest is one or more
**block moves**, not instruction-content differences. Confirmed: the instruction
multiset is identical (`getHitpoints`/`getShieldHP`/`getArmorHP`/`getGammaHP`
13/13/13/13, `departStation` 6/6, `isCampaignMission` 23/23,
`getCurrentCampaignMission` 25/25, `Galaxy::getStation` 5/5, `Status::getStation`
1/1 — all equal).

### The single root move

The first divergence is at `0x17a7f2` (orig) / `0x17a7f2` (ours): a branch to
the function epilogue. Orig emits `b.w 17b004` (4 B); ours emits `b.n 17af82`
(2 B) because our epilogue block sits 0x82 B earlier. Everything before that
point is a constant −4 B shift.

The epilogue is always placed immediately after the `hudTouchFlags` body
(`17afe8..17b002` orig, `17af66..17af80` ours). That body is preceded by the
join test, preceded by `initHudMenu(2)`. So the ordering question reduces to:

```
orig:  [star-map cleanup] [keepObjectives=false] [mission chain] [initHudMenu(2)]
       [join test] [hud flags] [epilogue] [stack_chk_fail] [closeHudMenu]
       [failed blocks] [changeCredits] [campaign chain ...]

ours:  [star-map cleanup] [initHudMenu(2)] [join test] [hud flags] [epilogue]
       [stack_chk_fail] [literal pool] [closeHudMenu] [failed blocks]
       [keepObjectives=false] [mission chain] [campaign chain ...]
```

i.e. ours places the **whole hud tail group before the `won`/mission chain**;
the original places it **after** the mission chain and before the failed
blocks. That one move accounts for the −0xaa shift of the campaign chain
(orig `17b106` vs ours `17b1b0`) and for the +4 B back-propagation into the
branch widths at the top of the function.

### The campaign chain itself is already correct

Inside the chain the block order is identical, just shifted:

```
15 test / 15 body(loadStation 98) / 22 test -> shared tail / 43 test /
43 body(loadStation 10) -> shared loadStation tail / 42 test / 65 / 81 / 74 /
95 / 96 / 100 / 110 / 120 / 126 / 127 / 134 / 144 / 155 / 161 / 162 /
else -> finishMission / "back to the station" tail
```

The shared `getHitpoints..getGammaHP` + `departStation` tail is emitted once in
both, and `finishMission:` lands immediately after the 42 body in both
(orig `17b26c`, ours `17b2f6`). The only campaign-chain residual is therefore
the global shift, not a body reorder. **No source reordering of the
`== NN` clauses can help** — their emitted order already matches.

## 2. The one source change that moves the layout: `cutsceneActive == true`

Sweeping `== true` over every `if (this->…)` in `OnTouchEnd` (single and
pairwise) found exactly one change that re-rolls the block layout:

```cpp
-            if (this->cutsceneActive) {
+            if (this->cutsceneActive == true) {
```

(line 1509; the same textual change also fires at lines 775/2704 in other
functions, which does not affect this TU's target).

| variant | size | raw diff |
|---|---:|---:|
| base | 8080 | **5816** |
| `cutsceneActive == true` (`== 1` is byte-identical) | 8084 | **4433** |
| `cutsceneActive != false` / `!!cutsceneActive` | 8080 | 5816 (no change) |

It flips the `cutsceneActive` branch from `beq` to `bne` at `0x17aaf2`, which
changes the fall-through and re-orders the cold block chains. The epilogue moves
to `0x17b8d4` and the delta-0 byte count rises from 1008 to **1717**.

### Why it is 4 B short

The variant is **8084 B** (+4). The +4 is exactly the branch-width ledger:

| site | base | variant | Δ |
|---|---|---|---:|
| `0x17a7f2` branch to epilogue | `b.n` | `b.w` | +2 |
| `0x17adae` `cameraMode==0` test | `beq.n` | `beq.w` | +2 |
| `0x17adbe` `freeCamTouchEnd` tail | `b.n` | `b.w` | +2 |
| `0x17adc2` free-cam → epilogue | `b.n` | *(deleted)* | −2 |

Note the original agrees with the **base** at 2 of these 3 sites
(`beq.n 17aeac`, `b.n 17afe8`) and with the variant only at `0x17a7f2`
(`b.w`), so the raw win is partly coincidental alignment, not extra fidelity.

## 3. Companion-change search (all measured, 8080 B required)

`cutsceneActive == true` combined with every other candidate, looking for a
−4 B companion that keeps the re-rolled layout:

| companion | size | raw |
|---|---:|---:|
| (none) | 8084 | 4433 |
| `touchesStream == true` | 8084 | **4426** |
| `keepObjectives` plain `?:` | 8084 | 4433 |
| `field_0x1e4 == true` / `usingJumpDrive == true` / `hudMenuOpen == true` / `field_0xc4 == 0` | 8084 | 4433 |
| `pauseOpen != 0` + `choiceWindowOpen != 0` | 8084 | 4433 |
| `field_0xcf != false` | 8084 | 4433 |
| `freeCamMode == true` | 8088 | 4725 |
| `menuTouchOpen == true` | 8084 | 6147 |
| `autopilotMenuOpen == true` | 8076 | 6882 |
| `field_0xcf == true` | 8092 | 6959 |
| `choiceWindowOpen == true` | 8024 | 7524 |
| `_bca == true` | 8060 | 7564 |
| `starMapOpen == true` | 8104 | 5491 |
| `won` bare (`hasWon()` without `== true`) | 8092 | 4695 |
| `count != 0` | 8080 | 6571 |
| `won` bare + `supernovaChallenge` inlined | 8080 | 6621 |
| `won` bare + `hasFailed() == true` | 8088 | 6089 |

Only `count != 0` and `won`-bare + supernova-inline land on 8080, and both are
*raw regressions* (6571 / 6621 vs 5816). No 8080-B combination beats the base.

## 4. Also re-confirmed this wave (do not re-run)

- `bool won = …; if (won)` vs inlining the `||` expression directly into the
  `if`: byte-identical (5816/8080). The `won` temporary is not the lever.
- `if (won == true)`, `if (won != false)`, `if (hudMenuOpen != 0)`,
  `if (hudMenuOpen == 1)`, `if (hudTouchFlags == 0)` → `if (!hudTouchFlags)`,
  `if (freeCamMode != false)`, `if (menuTouchOpen != false)`,
  `if (pauseOpen != false)`: all byte-identical to base.
- `supernovaChallenge` temp inlined (`if (!this->field_0x1e4)`) removes 8 B
  (8072) — a usable −8 lever if a +8 companion is ever needed.

## 5. Open question / next lead

The whole residual is the placement of the **hud tail group** relative to the
`won`/mission chain. The `cutsceneActive == true` experiment proves the group
*can* be moved from "before the mission chain" to "after it" purely from source
shape, and does so at 8084 B. The next pass should look for a **faithful**
branch-direction flip near `0x17aae8` that also costs −4 B — e.g. rewriting the
`cutsceneActive` / `menuTouchOpen` / `autopilotMenuOpen` / `!field_0xc4` else-if
ladder as a nested `if (!cutsceneActive) { … } else { … }` form (the wave-140
w8514 "nested else" trick, applied one level higher). That is the only
untried family that changes the fall-through of the `0x17aae8` test without
changing the instruction multiset.

## 6. Reproduction

```
cp /dev/shm/w8613/MGame.base.cpp src/game/menu/MGame.cpp     # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8613/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 5816 / 8080

# the 8084 near miss:
sed -i 's/if (this->cutsceneActive) {/if (this->cutsceneActive == true) {/' src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8613/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4433 / 8084
```

Scratch: `/dev/shm/w8613/{fdiff.py,hunks.py,delta.py,shiftprof.py,blocksig.py,
sweep7.py,sweep8.py,sweep9.py,sweep10.py,sweep11.py,sweep12.py,sweep13.py,
sweep14.py,sweep15.py,orig.dis,ours.dis,w.dis,v5.dis}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
