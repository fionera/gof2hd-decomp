# MGame::OnTouchEnd — wave191b w13672: campaign-first arm-81 test + arm-126 store order (landed)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13672`, branch
`wave191b-w13672`, base master `2064dfd40`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13672`.

Prior notes: `mgame-ontouchend-wave191b-w13626.md` (arm-134 store order landed,
campaign-first sweep), `-w13622.md` (campaign-134 store swap, diff-cluster map),
`-w13639.md` (95/81/126/161/back-to-station store orders, particle block),
`-w13611.md` (diff-cluster map + local sweeps). w13662 (parallel) works the
arm-81 *body* / mining-report seam; this pass deliberately touches the arm-81
*condition* and the arm-126 body instead.

## Result (landed)

Two independent, semantically-neutral fidelity fixes:

1. **Arm 81 condition order.** The original tests `Mission::isCampaignMission()`
   first and only then `Status::getCurrentCampaignMission() == 81`
   (`0x17b4a4: blx isCampaignMission; cmp r0,#0; beq …; ldr r0,[r8];
   blx getCurrentCampaignMission; cmp r0,#81`). The base source had the operands
   reversed (`cm == 81 && mission->isCampaignMission()`).
2. **Arm 126 body order.** The original emits `switch_to_target_setting = 1`
   first (`0x17bbc2: movs r1,#1; str r1,[r0]`), then the four getters
   `64, 5c, 60, 68`, then `Level::initStreamOutPosition = 0`. The base source had
   the switch store after the getters.

```cpp
} else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 81) {
...
} else if (mission->isCampaignMission() && Globals::status->getCurrentCampaignMission() == 126) {
    Globals::switch_to_target_setting = 1;
    Globals::status->field_64 = this->player->player->getHitpoints();
    ...
```

Both stores/reads are independent (the getters are pure field reads), so behaviour
is unchanged.

| | raw | aligned | size | insn | `.text` diff | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| base (arm-134 landed) | **2,878** | 1,572 | 8,080/8,080 | 2,862/2,865 | 71,227 | 156,312 (577 diff) |
| arm-81 condition only | 2,882 | 1,549 | 8,080/8,080 | 2,862/2,865 | 71,231 | 156,312 (577) |
| **landed (cond + 126)** | 2,885 | **1,527** | **8,080/8,080** | 2,862/2,865 | 71,234 | 156,312 (577) |

`check` green: byte_exact 4508/4524, linked_exact 4508/4524, size-exact 4524/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370. `.rel.plt` byte-identical (compared against
the original), `.ARM.exidx` 0 diff, `.rodata` 0 diff, `.ARM.extab` size unchanged.
The raw/`.text` cost is +7 B for a −45-instruction aligned gain; the hard gate
counts are unchanged.

## Campaign-first condition order — fresh sweep (arm-134 landed)

The original tests `isCampaignMission()` first in **every** chain arm. The base
source reverses the operands in 8 arms (15, 43, 65, 74, 81, 134, 162 and the
combined 96/127). Each swap is semantically neutral.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,878** | 1,572 | 8,080 |
| co_15 | 2,884 | 1,572 | 8,080 |
| co_43 | 2,880 | 1,571 | 8,080 |
| co_65 | 2,879 | 1,571 | 8,080 |
| co_74 | 2,880 | 1,572 | 8,080 |
| co_81 | 2,882 | **1,549** | 8,080 |
| co_134 | 2,880 | 1,572 | 8,080 |
| co_162 | 2,880 | 1,572 | 8,080 |
| co_96127 (combined, campaign-first) | 2,879 | 1,572 | 8,080 |
| co_all | 2,898 | 1,547 | 8,080 |
| co_all except 81 | 2,894 | 1,570 | 8,080 |
| co_all except 81 + 126 switch | 2,897 | 1,548 | 8,080 |

Arm 81 alone captures essentially all of the aligned benefit (−23) at the
smallest raw cost (+4); every other single swap is a tie or −1 aligned.
`co_all` buys only 2 more aligned instructions for +16 raw.

## Arms 96/127 are separate arms in the original (structural finding)

The original has **two** campaign tests for this body:

* `0x17b708`: `blx isCampaignMission; cmp r0,#0; beq 0x17b93c; ldr r0,[r8];
  blx getCurrentCampaignMission; cmp r0,#96; bne 0x17b93c` then falls into the
  shared body at `0x17b722`;
* `0x17bc24`: `blx isCampaignMission; cbz r0,0x17bc3a; ldr r0,[r8];
  blx getCurrentCampaignMission; cmp r0,#127; beq.w 0x17b722` — a backward
  branch to the *same* shared body.

The base source combines them as
`(cm == 96 || cm == 127) && mission->isCampaignMission()`, which emits **one**
`isCampaignMission` call. Counting `bl`/`blx` targets over the whole function:
original 464 calls, base 463 — the single missing call is exactly
`Mission::isCampaignMission()`. So the combined form is structurally one call
short of the binary.

Splitting it back into two `else if` arms is size-fatal in every shape measured.
The two bodies are textually identical, so clang tail-merges them (the
`Galaxy::getStation` count stays 5, matching the original), but the extra
condition test costs bytes:

| split variant | raw | aligned | size |
|---|---:|---:|---:|
| base order, 96/127 adjacent, base conditions | 3,935 | 1,746 | 8,108 |
| base order, 96/127 adjacent, campaign-first | 4,691 | 1,610 | 8,096 |
| base order, 127 after 126, campaign-first | 4,790 | 1,571 | 8,104 |
| faithful chain order, campaign-first, all body fixes | 3,983 | **911** | 8,088 |
| faithful chain order, campaign-first, arm-96 body only | 3,970 | 1,131 | 8,088 |
| faithful chain order, base conditions, all body fixes | 4,313 | 1,202 | 8,112 |

A 256-way sweep of campaign-first subsets on the faithful-order split (with the
faithful arm-81/95/96/126/161 body orders) found **no** size-8,080 result; the
minimum was 8,088. The faithful chain order itself
(`15,22,43,42,65,81,74,95,96,100,110,120,126,127,134,144,155,161,162`) with the
combined 96/127 arm is 8,076 (4 B short), matching w13626. The split therefore
cannot be landed without a codegen change elsewhere; it is recorded here for a
future pass that can pin the 12 B of instruction-selection difference (the
faithful split has the exact original call multiset, 464/464, but 3 extra
instructions / +12 B).

## Other measured candidates (all size 8,080, `.ARM.extab` 156,312 unchanged)

| variant | raw | aligned |
|---|---:|---:|
| base | **2,878** | 1,572 |
| arm-81 condition (landed) | 2,882 | 1,549 |
| arm-126 switch-first (landed) | 2,881 | 1,560 |
| **both (landed)** | 2,885 | **1,527** |
| both + arm-96 body `64,5c,60,68` | 2,886 | 1,527 |
| both + arm-161 switch-first | 2,887 | 1,529 |
| both + back-to-station faithful | 2,895 | 1,514 |
| both + arm-95 switch-first | 2,925 | 1,519 |
| both + arm-96 body + 161 + bts | 2,898 | 1,516 |
| both + arm-96 body + 161 + bts + 95 | 2,938 | 1,508 |
| arm-134 `sw,5c,60,68,64` + arm-81 cond | 2,882 | 1,550 |
| arm-134 `5c,60,64,sw,68` + arm-81 cond | 2,885 | 1,547 |
| arm-134 `5c,64,sw,60,68` + arm-81 cond | 2,882 | 1,550 |

The base remains a strict local minimum for raw; the landed pair is the best
aligned-per-raw trade found. Adding the other faithful store orders keeps
lowering aligned (to 1,508) but at +60 raw, which is no longer "a little".

## Conclusion

The residual is still dominated by MachineBlockPlacement's cold-block order (the
Play/Stop block placement, cluster `0x0f72..0x156c`). This pass landed the two
cheapest faithful fixes in the campaign-chain region: the arm-81 campaign-first
condition test and the arm-126 `switch_to_target_setting` store order. The
96/127 split is the one remaining *structural* infidelity (one missing
`isCampaignMission` call) but is size-fatal in every source shape measured.

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13672`: `measure.sh` (build +
`tools/fn_diff.py` + `.ARM.extab` size), `variant.py` (replacement harness),
`build_chain.py` (identity-checked chain parser/rebuilder with per-arm
campaign-first and body-order fixes), `sweep_cond.py` (256-way condition-subset
sweep on the faithful split), `spec*.py` (each experiment batch),
`orig.asm` / `ours*.asm` (disassembly).
