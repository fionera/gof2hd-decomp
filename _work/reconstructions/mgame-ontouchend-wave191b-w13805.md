# MGame::OnTouchEnd (8,080 B) — w13805 (wave 191b): cargo / instant-action /
# 127-split note-only pass

Target: `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`, 8,080 B).
Branch `wave191b-w13805`, base master `444a44576bb7480bc274e56b6a4efdfb450b95bb`.
Worker w13805.  Prior passes on this function: w13774 (cinematic-steer `case 0`
explicit) and w13798 (case-0 store tail reorder), both landed; see
`_work/reconstructions/mgame-ontouchend-wave191b-w13798.md` and the wave 94..191
notes.

**No size-exact raw/aligned win was found this pass.  `src/` is unchanged.**
This note records the measured table, the exact residual shape, and the
source-level dead ends so the next worker does not repeat them.

## 0. Baseline (pristine tree, rebuilt this pass)

```
_ZN5MGame10OnTouchEndEiiPv   raw=3665  aligned=545  size=8080/8080  insn=2865/2865
```

`check` on this exact tree: byte_exact **4508**, linked_exact **4508**,
size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint
**CLEAN**, parity **5370/5370**, RATCHET PASS.

Section deltas vs the original, measured with `scratch/w13805/secdiff.py`
(`arm-linux-androideabi-objcopy -O binary --only-section=<s>` + byte compare):

| section | delta |
|---|---:|
| `.text` | 75,742 |
| `.ARM.extab` | 590 |
| `.note.gnu.build-id` | 20 |
| `.ARM.exidx` | 0 (identical) |
| `.rodata` | 0 (identical) |
| `.data` / `.bss` / `.rel.plt` / `.rel.dyn` / `.dynsym` / `.dynstr` / `.hash` / `.got` | 0 (identical) |

(The task prompt quoted `.text 75,146`; the fresh measurement on this base is
75,742.  `.ARM.extab` 590 matches the prompt.)

## 1. Residual shape (largest aligned clusters, current tree)

From `scratch/w13805/blocks.py _ZN5MGame10OnTouchEndEiiPv`:

| n | kind | O block | U block | what |
|---:|---|---|---|---|
| 63 | insert | `O[1609:1609]@17b374` | `U[1610:1673]@17b37a` | cargo-loop literal-pool island |
| 46 | replace | `O[1926:1972]@17b6ec` | `U[1963:1965]@17b758` | 95-clause tail / `b.n 17b77c` shared tail |
| 31 | insert | `O[2280:2280]@17bac6` | `U[2254:2285]@17ba7a` | weapon-slot body placement |
| 25 | replace | `O[2196:2221]@17b9d8` | `U[2186:2195]@17b9c0` | weapon-slot body placement |
| 22 | replace | `O[1629:1651]@17b3b4` | `U[1693:1694]@17b456` | cargo-loop literal pool |
| 21 | replace | `O[1470:1491]@17b1e2` | `U[1491:1492]@17b21e` | `PlayEngineSound` block placement |
| 16 | replace | `O[1369:1370]@17b0c2` | `U[1369:1385]@17b0c6` | `PlayEngineSound` block placement |
| 14 | delete | `O[2123:2137]@17b910` | `U[2128:2128]@17b922` | `isMining()` cold block before 100 body |
| 13 | replace | `O[1869:1882]@17b64e` | `U[1914:1918]@17b6cc` | cinematic switch test order (`==1` before `==0`) |
| 11 | replace | `O[2023:2034]@17b7e4` | `U[2025:2035]@17b7e8` | literal-pool island |
| 10 | replace | `O[1257:1259]@17af82` | `U[1257:1267]@17af82` | `isInstantActionMission` tested before `isCampaignMission` |
| 7 | delete | `O[2417:2424]@17bc2a` | `U[2425:2425]@17bc42` | standalone `==127` clause |
| 4 | replace | `O[2486:2490]@17bcf4` | `U[2487:2488]@17bcf6` | shared `dockToAsteroid` `blx` placement |

All of these are clang 7.0.2 `-Oz` **machine-block / literal-pool placement**
differences, not statement-order differences: the same instruction sequences
exist in both binaries, just at different addresses.  Source reordering that
tries to force the original placement changes the emitted size (see §3).

## 2. Key disassembly facts (original, image base 0x10000)

- `17a5ca` cargo/refine clause: `cmp.w r9,#0 / beq` (`amount == 0` skip).  Ours
  emits `cmp.w r9,#0xffffffff / bgt` — clang canonicalises the `amount--` +
  `if (amount)` loop into a **count-up** `add.w r8,r8,#1` + `cmp r8,#0`.
- `17af7e` `isInstantActionMission()` is tested **before** `17afa8`
  `isCampaignMission()`; ours tests campaign first.
- `17b374` failed==1 freelance path (`getType()==3 || 5 || 11` → cargo loop).
  The 12-word literal pool at `17b3b4` sits between `b.n 17b40e` and the loop
  body `17b40e`.  Ours has the same block at `17b416` with no pool.
- `17b6a8` 95 clause: `switch_to_target_setting = 1` **first**, then
  `field_64/5c/60`, then `field_68`, then
  `initStreamOutPosition=1; departStation(galaxy->getStation(10))`, then the
  shared tail `b.n 17b77c`.
- `17b722` 96 body: `switch_to_target_setting = 1` first, HP chain,
  `getStation(98)`, shared tail `17b77c` (`strb r5,[r2,#0]` + `getStation`).
- `17bc24` standalone `==127` clause: `isCampaignMission()` then
  `getCurrentCampaignMission()==127`, then `beq.w 17b722` (re-enters the 96
  body).  It is placed **after** the 126 clause, not adjacent to 96.
- `17b910` `isMining()` cold block precedes the 100 body; ours is after 110.
- `17b9ca` weapon-slot body sits between the 100 and 110 bodies; `movs r6,#3`
  falls into it, and the `0/1/2` cases `b.n 17b9cc` into it.
- `17bf4e` `isDockingToStream` branch jumps to the shared `dockToAsteroid`
  `blx` at `17bcf4`; ours puts the shared `blx` at `17bf5e` (same merge, other
  address).
- `17b0b6` order: `setFreelanceMission` → `layout` load →
  `getReward/getBonus` → `showMissionRewardMessage`.  Ours emits reward first.
- `17b0e2` `changeCredits(getReward()+getBonus())` then
  `levelScript->m_nTimeLimit = 0` (direct `str r1,[r0,#0]`).
- `17b64c` cinematic switch tests `cmp r0,#1` before `cmp r0,#0`.

## 3. Cargo / refine clause (the #1 blocker), inlined

`src/game/menu/MGame.cpp:1373-1405`:

```cpp
int choice = this->choiceWindow->OnTouchEnd(x, y);
switch (choice) {
    case 0: {
    String report;
    Ship *ship = Globals::status->getShip();
    bool refinedAny = false;
    for (int ore = 154; ore < 166; ore++) {
        int refined = ore + 11;
        if (ore == 165) {
            refined = 218;
            ore = 217;
        }
        int fee = Globals::status->hardCoreMode() ? 100 : 30;
        int amount = 0;
        while (ship->hasCargo(ore, fee)) {
            ship->removeCargo(ore, fee);
            ship->addCargo((*Globals::items)[refined]->makeItem(1));
            amount--;
            refinedAny = true;
        }
        int count = -amount;
        if (count > 0) {
            report += String("\n") + String(count) + String("t ") +
                      *Globals::gameText->getText(1274 + refined);
        }
    }
    this->_bca = 0;
    if (refinedAny) {
        this->choiceWindow->set(*Globals::gameText->getText(291) + String("\n") + report);
    } else {
        this->choiceWindow->set(*Globals::gameText->getText(292));
    }
        break;
    }
```

The original loop body is `sub.w r9,r9,#1` then `cmp.w r9,#0` (one variable
decremented and tested), and the report string uses `rsb r1,r9,#0`
(`String(-amount)`).  Clang 7.0.2 `-Oz` canonicalises every source spelling of
this loop into the count-up form.  Verified with standalone repros
(`scratch/w13805/mini.cpp`, `mini3.cpp`): `int amount = 0; while (hasCargo)
{ …; amount--; } if (amount != 0) g(-amount);` always becomes
`add.w r8,r8,#1` + `cmp r8,#0`.  LLVM 7.0.2 `isNegatibleForFree` is FP-only;
the int negate comes from ISel / `ARMTargetLowering::PerformDAGCombine`, so no
source spelling reaches `sub.w` + `cmp.w` + `rsb` at this call shape.

## 4. Measured table (cumulative for this worker; all size-exact unless marked)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **baseline (pristine)** | **3,665** | **545** | **8,080** |
| `setFreelanceMission` before reward | 3,667 | 545 | 8,080 |
| 96/127 reversed to 127/96 | 3,665 | 545 | 8,080 |
| cinematic if/else-if (154/157/158, 1, 0) | 3,665 | 545 | 8,080 |
| cinematic if/else-if (154/157/158, 0, 1) | 3,665 | 545 | 8,080 |
| cinematic switch `case 1` before `case 0` | 3,665 | 545 | 8,080 |
| 95 clause after 81 (before 74) | 3,759 | 769 | 8,080 |
| 95 clause after 65 (before 81) | 3,755 | 767 | 8,080 |
| 95 clause after 74 (before 96) | 3,751 | 799 | 8,080 |
| 95 after 74 + `switch_to_target_setting` top | 3,758 | 843 | 8,080 |
| 95 clause `switch_to_target_setting` top (in place) | 4,071 | 733 | 8,084 (fatal) |
| 95 clause `switch` after field_64 | 4,072 | 733 | 8,084 (fatal) |
| 95 clause `switch` after field_5c | 4,073 | 733 | 8,084 (fatal) |
| instant-action first (if/else-if) | 4,716 | 1,140 | 8,092 (fatal) |
| instant-action first (separate `if` + `return`) | 4,716 | 1,140 | 8,092 (fatal) |
| instant-action first + freelance reorder | 4,730 | 1,141 | 8,092 (fatal) |
| instant-action as `if(!IA){…}else{…}` w/ freelance-first | 4,730 | 1,141 | 8,092 (fatal) |
| cargo `amount != 0` (all spellings) | 6,609 | 1,302 | 8,084 (fatal) |
| cargo `if (refinedAny)` | 6,607 | 1,271 | 8,084 (fatal) |
| cargo `if (amount < 0)` | 6,557 | 1,128 | 8,084 (fatal) |
| cargo `count++` loop | 4,387 | 737 | 8,088 (fatal) |
| cargo `unsigned amount` (+cast) | 6,609 | 1,302/1,303 | 8,084 (fatal) |
| cargo `amount < 0` / `<= -1` / `0 > amount` + `String(-amount)` | 3,665 | 545 | 8,080 (identical) |
| cargo `for(;;)break` / `for(;cond;dec)` / `while(true)break` / `--amount` / `-= 1` / hoisted `amount` | 6,609 | 1,302 | 8,084 (fatal) |
| cargo `count` inside/after `if`, cond variants, `&&` | 6,609 | 1,302 | 8,084 (fatal) |
| cargo empty `if` probes after block | 6,609 | 1,302 | 8,084 (fatal) |
| cargo ternary | 7,516 | 3,292 | 8,096 (fatal) |
| `refinedAny` before `Ship*` | 3,810 | 573 | 8,080 |
| `String report` after `Ship*` | 7,532 | 2,081 | 8,104 (fatal) |
| `String report; bool refinedAny; Ship*` | 3,810 | 573 | 8,080 |
| `bool refinedAny; Ship*; String report` | 7,532 | 2,081 | 8,104 (fatal) |
| `long long amount` | 7,585 | 2,215 | 8,116 (fatal) |
| no builtin on 162 | 3,689 | 588 | 8,080 |
| builtin on 15 | 3,689 | 918 | 8,080 |
| builtin on 95 | 3,936 | 979 | 8,084 (fatal) |
| weaponSlot store swap | 3,665 | 611 | 8,080 |
| `_bc8` close store swap | 3,667 | 549 | 8,080 |
| wingman block store swap | 3,665 | 549 | 8,080 |
| split 96 / standalone 127 (dup body) | 4,756 | 798 | 8,100 (fatal) |
| split 96 / standalone 127 (dup body, this session) | 4,827 | 932 | 8,104 (fatal) |
| split 96 / standalone 127 (`goto` body) | 4,847 | 950 | 8,148 (fatal) |
| `soundsMuted` by value (not reference) | 4,198 | 725 | 8,080 |
| fee/amount swap, 0<count, count>=1, count=0-amount, const count, hasCargo!=0, pauseOpen!=0, soundsMuted bool, inline switches, failed truthy, won simplify | 3,665 | 545 | 8,080 (identical) |

### New rows measured this pass (w13805)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| mining `int choice` folded into `switch (OnTouchEnd(...))` | 3,665 | 545 | 8,080 (identical) |
| supernova `int choice` before `bool supernovaChallenge` | 6,982 | 804 | 8,068 (fatal) |
| `keepObjectives` as `hasWon() && !isCampaignMission()` | 3,665 | 545 | 8,080 (identical) |
| `won` without `== true` | 3,665 | 545 | 8,080 (identical) |
| `mission` as `if (!mission->hasWon()) mission = …` | 4,273 | 634 | 8,076 (fatal) |
| `__builtin_expect(failed == 1, 1)` | 5,405 | 2,438 | 7,824 (fatal) |
| `__builtin_expect(failed == 1, 0)` | 5,311 | 1,503 | 8,076 (fatal) |
| `__builtin_expect(hudMenuOpen != 0, 1)` | 6,075 | 986 | 8,080 |
| `__builtin_expect(hudMenuOpen != 0, 0)` | 6,285 | 1,052 | 8,056 (fatal) |

## 5. Why the remaining clusters are not source-reachable

- **Cargo literal pool (`O17b3b4`)**: a 12-word GOT-relative constant island
  emitted after `b.n 17b40e`.  Ours has the same instructions but no island
  because the surrounding block layout differs.  Not reachable by reordering
  statements.
- **Weapon-slot body (`O17b9ca`)**: the original places the shared
  `getShip()->getEquipment(1)` body between the 100 and 110 bodies; ours places
  it after 110.  Both have the `movs r6,#N; b.n <body>` fan-in; only the body's
  address differs.  This is `MachineBlockPlacement`, driven by branch
  probabilities, not source order.
- **`PlayEngineSound` block (`O17b1e8`)**: original interleaves it before the 42
  clause; ours after `setFreelanceMission`.  Same instructions, different
  address.
- **Shared `dockToAsteroid` `blx` (`O17bcf4`)**: ours already tail-merges the
  two calls, but places the merged `blx` at `17bf5e` instead of `17bcf4`.
- **Standalone `==127`**: the original's `17bc24` re-enters the 96 body at
  `17b722`.  Writing it as a second clause duplicates the body and costs
  +20..+68 B (clang 7.0.2 does not tail-merge the two identical bodies at this
  size); `goto` into the body costs more.

## 6. Recommendation

The function is size-exact at 8,080.  Every source-level reordering that
attempts to match the original block order changes `.text` size by +4..+68 B,
which is gate-fatal unless exactly compensated elsewhere in the same TU.  The
next productive angle is probably a TU-level or link-order change that shifts
the literal-pool / machine-block placement globally, not a local statement
reorder in `OnTouchEnd`.
