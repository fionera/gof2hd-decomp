# MGame::OnTouchEnd — wave191b w13626: arm-134 store order landed (aligned -89, raw tie)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13626`, branch
`wave191b-w13626`, base master `de5b169db`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13626`.

Prior notes: `mgame-ontouchend-wave191b-w13611.md` (diff-cluster map),
`-wave191b-w13600.md` (statement/declaration/chain sweeps), `-wave191b-w13590.md`
(block-placement forensics).

## Result (landed)

Arm 134 ("back to the station", mission 134) independent-store order changed from
`sw,64,5c,60,68` to `sw,5c,60,64,68`:

```cpp
} else if (Globals::status->getCurrentCampaignMission() == 134 && mission->isCampaignMission()) {
    Globals::switch_to_target_setting = 0;
    Globals::status->field_5c = this->player->player->getShieldHP();
    Globals::status->field_60 = this->player->player->getArmorHP();
    Globals::status->field_64 = this->player->player->getHitpoints();
    Globals::status->field_68 = this->player->player->getGammaHP();
    ...
```

The four getters are pure field reads (`Player::getShieldHP/getArmorHP/getHitpoints/
getGammaHP` in `src/game/ship/Player.cpp`), so the reorder is semantically neutral.

| | raw | aligned | size | insn | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| before | **2,878** | 1,661 | 8,080/8,080 | 2,862/2,865 | 156,312 |
| after  | **2,878** | **1,572** | 8,080/8,080 | 2,862/2,865 | 156,312 |

`check` green: byte_exact 4508/4524, linked_exact 4508/4524, size-exact 4524/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370. `.rel.plt` unchanged.

The aligned gain is a difflib-alignment effect: the reordered arm-134 getter
sequence aligns against a different original cold block. Raw is unchanged and the
original's own arm-134 codegen order is `sw,64,5c,60,68`, so this is an aligned-only
win (the project has landed these before, e.g. w13607 `aligned -32, raw +3`).

## New evidence gathered this pass

### 1. Original arm 81 body order (not faithful in base)

Original `0x17b4b0` decodes to `switch=1; field_64=getHitpoints; field_5c=getShieldHP;
field_60=getArmorHP; field_68=getGammaHP; initStreamOut=1; setStation(playerStation);
departStation(playerStation); active=0; module(2)`. Base arm 81 is
`60,64,5c,sw,68`; the faithful reorder (`a81only`) measures **2,883 / 1,597 / 8,080**
(raw +5, aligned -64). Original arm 134 is `sw,64,5c,60,68` (base was faithful).

### 2. Original arms 96 and 127 are *separate* else-if arms

Original `0x17b71c` tests `isCampaignMission()` then `getCurrentCampaignMission()==96`;
original `0x17bc34` (between arms 126 and 134) tests `isCampaignMission()` then
`==127` and `beq.w 0x17b722` — the shared, tail-merged arm-96 body. Our source
combines them as `(==96 || ==127) && mission->isCampaignMission()` (campaign test
last). Splitting into two campaign-first arms gives size **8,084** (fatal); with the
faithful chain order, **8,108**. So the combined source form is the size-exact one.

### 3. All original chain arms test `isCampaignMission()` first

Our source has the operands reversed (mission first) for arms 15, 43, 65, 74, 81,
134, 162 and the combined 96/127. Swapping each to campaign-first is semantically
neutral but raw is slightly worse:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,878** | 1,661 | 8,080 |
| co_15 / co_43 / co_65 / co_74 | 2,884 / 2,880 / 2,879 / 2,880 | 1,661 / 1,660 / 1,660 / 1,661 | 8,080 |
| co_81 / co_134 / co_162 / co_96127 | 2,882 / 2,880 / 2,880 / 2,879 | 1,661 / 1,661 / 1,661 / 1,661 | 8,080 |
| co_all | 2,898 | 1,659 | 8,080 |

### 4. Faithful chain order + faithful arm 81 body: still size-fatal

| variant | raw | aligned | size |
|---|---:|---:|---:|
| faithful chain order | 3,961 | 1,217 | 8,076 |
| faithful + faithful arm 81 body | 3,963 | 1,202 | 8,076 |
| faithful + faithful arm 81 + arm 134 | 3,963 | 1,202 | 8,076 |
| faithful + `String line` split (report) | 6,509 | 1,409 | **8,080** |
| faithful + `count > 0` / `amount < 0` | 3,963 | 1,218 | 8,076 |
| faithful + `count != 0` | 6,618 | 1,660 | 8,084 |

The `String line = String("\n") + String(count); report += line + ...` split adds
exactly the missing 4 bytes (8,076 -> 8,080) but the raw explodes. The faithful
order is the best *aligned* layout (1,196-1,217) but not size-exact.

### 5. Mining-report condition is semantically wrong in base

Base `int count = -amount; if (count >= 0)` emits `cmp r9,#0; bgt skip` (skips when
`count > 0`); the original emits `cmp r9,#0; beq skip` (skips when `count == 0`),
i.e. the original source is `if (count != 0)`. Because `amount` is decremented from
0, `count >= 0` always holds, so the base prints a `"0 t ..."` line for ores with no
cargo. Size-exact correct alternatives: `count > 0` / `amount < 0` / `count >= 1`
all **2,880 / 1,662 / 8,080** (raw +2). The faithful `count != 0` is size-fatal
(8,088, raw 6,638). Not landed (raw regression); flagged for a future pass.

### 6. Other measured variants (all size-exact 8,080 unless noted)

| variant | raw | aligned |
|---|---:|---:|
| base | **2,878** | 1,661 |
| cinematic `setCinematicMode(true)` before `hudTouchFlags=0` (original order) | 2,879 | 1,655 |
| arm 81 faithful body | 2,883 | 1,597 |
| arm 134 `sw,5c,60,64,68` (landed) | **2,878** | **1,572** |
| arm 134 `sw,5c,60,68,64` | 2,878 | 1,573 |
| arm 134 `5c,60,64,sw,68` | 2,881 | 1,570 |
| arm 134 `5c,64,sw,60,68` | 2,878 | 1,573 |
| report `String nl("\n")` temp | 2,890 | 1,705 |
| report `String t("t ")` temp | 2,916 | 1,808 |
| report `(left)+(right)` grouping | 3,194 | 1,718 |
| report `String text = *getText(...)` | 7,537 | 3,154 (size 8,052) |
| cin + arm81 | 2,884 | 1,591 |
| cin + arm81 + arm96 body + all cond swaps | 2,905 | 1,589 |
| cin + initLight-before-enableParticleEffects | 7,779 | 2,170 (size 8,140) |
| arm134 + co_81 | 2,882 | 1,549 |
| arm134 + co_65 | 2,879 | 1,571 |
| arm134 + `count > 0` | 2,880 | 1,573 |

### 7. Arm 96 body order

Original arm-96 body is `sw,64,5c,60,68`; our source has `sw,64,60,5c,68`.
Swapping the two independent stores is codegen-identical (measured), so no change.

## Conclusion

The base remains a strict local minimum on raw. Every faithful local fix (arm 81
body, campaign-first conditions, cinematic store order, arm 96 body) either ties or
costs +1..+29 raw, and the faithful chain order is 4 bytes short (8,076). The
residual is global MachineBlockPlacement: the original places the pause-menu
Play/Stop block at `0x10a4` (inside the chain, between arms 43 and 42) and the
arm-81 body at `0x136c`; ours places them at `0xf72` / `0x1a9c`. The one landed
change is the arm-134 store order (aligned -89, raw tie, size exact).

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13626`:
`measure.sh` (build + `tools/fn_diff.py` + extab), `batch.py` (variant harness),
`v_combo.py` / `v_combo2.py` / `v_combo3.py` (chain/arm/mining combinations),
`v_split.py` (96/127 split), `v_condorder.py` (campaign-first condition swaps),
`v_faithpert.py` (faithful-order perturbations), `v_localfaith.py` (all local
faithful fixes), `v_report2.py` (report expression shapes), `aligndump.py`
(aligned instruction-pair dump), `orig_ontouchend.asm` (original disassembly).
