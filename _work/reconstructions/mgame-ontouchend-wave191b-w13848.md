# MGame::OnTouchEnd (8,080 B) — w13848 (wave 191b): 95-clause
# `switch_to_target_setting` store moved to first statement

Target: `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`, 8,080 B).
Branch `wave191b-w13848`, base master `055321398`.
Slice: the campaign-mission chain (`mission->isCampaignMission() && ... == N`),
specifically the `== 95` clause body. Deliberately different from w13843
(cinematic skip-switch case order, landed), w13833 (95-clause store placement,
reverted), w13819/w13818 (shared tail / high-case dispatch, note-only).

**Landed a size-exact raw win.** The change is one line: in the `== 95` clause,
`Globals::switch_to_target_setting = 1;` is written as the **first** statement
(before the four `getHitpoints/getShieldHP/getArmorHP/getGammaHP` field stores)
instead of between `field_60 = getArmorHP()` and `field_68 = getGammaHP()`.

## 0. Baseline (pristine tree, rebuilt this pass)

```
_ZN5MGame10OnTouchEndEiiPv   raw=3548  aligned=697  size=8080/8080  insn=2865/2865
```

## 1. Result (landed)

| variant | raw | aligned | size | .text delta | .ARM.extab |
|---|---:|---:|---:|---:|---:|
| baseline | 3,548 | 697 | 8,080 | 75,770 | 544 |
| **95-clause store first** | **3,544** | **697** | **8,080** | **75,766** | **544** |

`.rel.plt` stays byte-identical (0 differing bytes), `.ARM.exidx` / `.rodata`
unchanged (0), `.ARM.extab` unchanged (544). Gate: `check` -> RATCHET PASS,
byte_exact **4508**, linked_exact **4508**, size-exact **4524/4524**,
data_exact **843/843**, imports **207**, lint **CLEAN**, drift **0**,
parity **5370/5370**.

The raw metric is not reflected in `byte_exact` (the function is still not
byte-exact), so the ratchet counts are unchanged; the win is a 4-byte `.text`
delta reduction with unchanged `.ARM.extab`, i.e. a net gain.

## 2. Rejected variants (all measured, all size-exact unless noted)

Campaign-chain clause placement of the `== 95` clause (source order is
`15,22,43,42,95,65,81,74,96,...`; the original binary emits
`15,22,43,42,65,81,74,95,96,...`):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **95 first (current, landed)** | **3,544** | **697** | **8,080** |
| 95 after 65 | 3,551 | 731 | 8,080 |
| 95 after 81 | 3,554 | 887 | 8,080 |
| 95 after 74 (source-correct) | 4,028 | 824 | **8,076** (size-fatal) |
| 95 after 74 + cargo `count != 0` | 6,341 | 1,250 | 8,080 |

The source-correct `95 after 74` order is *worse* on raw and 4 bytes short, so
the compiler's block ordering with 95 first matches more bytes. Do not "fix"
the source order without a size compensation that also lowers raw.

Other levers tried this pass:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| cargo `if (count != 0)` (orig shape `cmp #0; beq`) | 6,644 | 1,325 | 8,084 (spills `r9`) |
| 95 body: `Level::initStreamOutPosition=1` before `field_68` | 3,725 | 680 | 8,084 |
| 95 body: `Level::initStreamOutPosition=1` after `departStation` | 4,011 | 703 | 8,096 |
| `else` block: `setFreelanceMission` before `showMissionRewardMessage` | 3,546 | 697 | 8,080 |
| weapon-slot body: `hudMenuOpen=0` before `pauseOpen=0` (with 95 win) | 3,544 | **763** | 8,080 |

The weapon-slot store swap is byte-identical to the original at that site
(orig `strb.w [fp,#214]` then `[fp,#93]`) but raises the aligned proxy by 66
because the literal-pool blocks shift; rejected on the aligned criterion.

## 3. Residual shape

The campaign-chain case bodies are instruction-identical to the original
(checked with `bodydiff.py` for 15/22/43/42/65/81/74/95/96/100/110/120/126/
134/144/155/161/162) — only branch targets and literal-pool placement differ.
The 64-instruction aligned cluster at ours `17b378` is the literal pool + the
`== 95` body emitted before the `getType()` cold block; the original emits the
cold block at `17b374` and the chain at `17b416`. Address deltas (pristine):
0 to `17b0c2`, campaign chain `17b42e..17b632` +120, `17b6aa` +194, `17b76a`
+72, `17b7fa..17b90c` +8, `17b952..17b9c4` -36, `17ba3e..17bab0` -86,
`17bade..17bb68` +8/+86, `17bbc2..17bc1a` +22, `17bc52..17bcf2` 0,
`17bd16..17bf46` -10/-4, `17bf6e..17c0a0` 0.

## 4. Method

`tools/fn_diff.py` for raw/aligned/size, `objdump -d` side-by-side for the
exact instruction cells, `bodydiff.py` for per-clause normalized diffs, and
`aldiff.py` for the non-matching instruction clusters. Builds `-j2` under a
12 GB `ulimit -v`; each statement-order experiment is one incremental
`link_so` + `fn_diff` (~1.2 s). Only variants that keep size 8,080 and do not
raise aligned are kept.
