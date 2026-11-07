# Hud::draw — wave 133 (w7736): forcing the `this` spill with the faithful gamma reload

**Status:** research note, 2026-11-07 (wave 133). Worktree `wt/w7736`, base
`master 144fc16b0`. **No source change landed**: HEAD remains the best measured
variant at **9332 B / 96.600000 %** (85 hunks). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4506`,
`linked_exact 4506`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`.

This note extends `hud-draw-wave132-w7716.md`, `hud-draw-wave128-w7658.md` and
`hud-draw-wave127-w7637.md`. It tests the wave-132 open question directly:
**make the allocator spill `this` instead of moving it to `sl`, while keeping the
faithful gamma reload, without adding a call.**

---

## 0. TL;DR

1. Re-confirmed on the current HEAD: the *only* source lever that decides
   whether the gamma block reloads `Globals::status` is whether the **mission
   argument** is the cached `st` or `Globals::status`. All 8 combinations of
   `{st, Globals::status}` for receiver/station/mission collapse to two builds:
   mission=`st` -> **9332 B / 96.6 % / 85 hunks**; mission=`Globals::status` ->
   **9312 B / 75.2 % / 538 hunks**. Receiver/station spelling is CSE'd away.
2. The `this` spill is a **greedy-allocator spill-weight effect**: the cached
   `st` snapshot used for the mission has 3 uses on one long interval and wins a
   callee-saved register, forcing `this` to the stack. The faithful reload splits
   that interval into two shorter ones (2 uses + 1 use), so `this` keeps `sl`.
3. The wave-132 candidate levers were all tested and do **not** work:
   a later `dividerOffset` use past the gamma block (4 spellings, 9304-9312 B),
   a `this` copy in the damage block (coalesced, 9312 B), and `Globals::status`
   anchors in the shield/armour/rocket/damage blocks (9272-9328 B, none 9332).
4. The one construct that *does* reproduce both the `this` spill and the
   faithful reload is `if (st && Globals::status->getGammaRayDamagePerSecond(
   Globals::status->getStation()->getIndex(),
   Globals::status->getCurrentCampaignMission()) > 0.0f)`. It restores the
   224-byte frame and the reload, but the null test adds 4 bytes and perturbs
   the slot layout: **9352 B / 85.9 % / 363 hunks**. With the damage-block anchor
   removed it is **9292 B / 84.2 % / 398 hunks**. Neither beats HEAD.
5. `st`-receiver + hoisted station index + `Globals::status` mission is
   **9332 B / 96.5 % / 86 hunks** (the wave-128 G3 result), and
   `st`-receiver + hoisted index + `st` mission is **9332 B / 96.6 % / 85**
   (identical to HEAD). HEAD stays.

---

## 1. Measured baseline

```
_ZN3Hud4drawExxP9PlayerEgobjj orig=9332 ours=9332
  ratio=96.600000  insns=3149/3152  hunks=85
```

The 85 hunks are: gamma block (5), gamma-body `fp`/`r9` swap (4),
damage-indicator scheduling / dead `&Globals::status` reloads (~20),
mission-panel register allocation (~50), EH pad (4).

The current HEAD source (the landed w4253 shape) is:

```cpp
    // Local `Status *` is load-bearing for codegen: it forces the branch-local
    // spill of `this` in the shield block, matching the original.
    Status *st = Globals::status;
    if (st->getGammaRayDamagePerSecond(st->getStation()->getIndex(),
                                       st->getCurrentCampaignMission()) > 0.0f) {
```

## 2. The two-build collapse (re-confirmed)

All eight `{st, Globals::status}` assignments for the three `Globals::status`
uses of the gamma condition were built and scored on top of HEAD:

| receiver | station | mission | size | raw % | hunks |
|---|---|---|---|---:|---:|
| any | any | `st` | **9332** | **96.6** | **85** |
| any | any | `Globals::status` | 9312 | 75.2 | 538 |

The receiver and station spellings are CSE'd into the same load; only the
mission argument matters. This is the same conclusion as wave 127/128, but it
also shows the *receiver* cannot be used to create the extra live interval:
`st` as receiver alone (mission `Globals::status`) is still 9312.

## 3. Why the cached `st` wins the register (machine IR)

`-mllvm -print-machineinstrs` (greedy allocator) for HEAD vs the faithful build:

HEAD gamma (cached `st`):
```
%2866 = load &Globals::status
str %2866, %stack.50
%646 = load [%2866]          ; st snapshot
getStation(%646); getIndex -> %648
getCurrentCampaignMission(%646)   ; same value, no reload
getGammaRayDamagePerSecond(%646, %648, ...)
```
`%646` has **3 uses** on one long interval.

Faithful gamma (reload):
```
%2867 = load &Globals::status
%646 = load [%2867]
getStation(%646); getIndex -> %648
str %2867, %stack.50
%649 = load [%2867]          ; reload
getCurrentCampaignMission(%649)
getGammaRayDamagePerSecond(%646, %648, ...)
```
`%646` has 2 uses and `%649` has 1 use on a shorter interval. The greedy
allocator therefore gives `%646` a lower spill weight and `this` keeps `sl`
(`mov sl,r5`), whereas in HEAD `this` is spilled at the hasShieldBar merge
(`str r5,[sp,#104]`, reloaded at `0x164040`/`0x1640ec`/`0x16413e`).

## 4. Wave-132 candidate levers (all measured, none a win)

Baseline `faith` (plain three-use, 9312 B / 75.2 % / 538 hunks) plus:

| variant | size | raw % | hunks |
|---|---:|---:|---:|
| `int y = barOffset + top*2 + dividerOffset;` (2nd bar) | 9304 | 77.6 | 479 |
| `int barYOffset = Globals::layout->field_0x218 + dividerOffset;` | 9304 | 77.5 | 486 |
| `DrawImage2D(..., y + dividerOffset, ...)` (2nd bar) | 9304 | 75.4 | 533 |
| `halfW += dividerOffset` (2nd bar) | 9312 | 77.3 | 484 |
| `Hud *self = this;` in damage block (plain / const / reference) | 9312 | 75.2 | 538 |
| `Hud *self = (Hud*)((char*)this+0);` before shield, used in damage | 9312 | 75.2 | 538 |
| `Globals::status && this->hasShieldBar` | 9320 | 74.1 | 539 |
| `Globals::status && this->hasArmorRegen` | 9280 | 66.9 | 588 |
| `Globals::status && ego->isInRocketControl()` | 9328 | 71.7 | 520 |
| `Globals::status && this->hasCloak` | 9312 | 74.0 | 550 |
| `Globals::status && ego->player->getShieldHP() >= 2 && ...` | 9324 | 75.4 | 540 |
| `Globals::status && this->hasShieldBar` + `&& hasArmorRegen` | 9296 | 66.6 | 618 |

The later `dividerOffset` use does extend the interval but the allocator still
prefers `this` in `sl`; the frame stays 216 and the extra arithmetic only makes
the body worse. A `this` copy in the damage block is coalesced away.

## 5. The `st &&` construct: the only one that does both

```cpp
    Status *st = Globals::status;
    if (st && Globals::status->getGammaRayDamagePerSecond(
                   Globals::status->getStation()->getIndex(),
                   Globals::status->getCurrentCampaignMission()) > 0.0f) {
```

The null test forces `st` to be materialised as a distinct value, so it stays
live across the gamma calls (spilled to `[sp,#108]`) and the allocator spills
`this` to `[sp,#100]`; the mission still reloads `Globals::status` from the
spilled `st` (`ldr r0,[sp,#108]; ldr r0,[r0,#0]`). The frame is 224.

Gamma block produced (objdump, normalized):
```
ldr r0,[r0,#0]      ; Globals::status
str r0,[sp,#108]    ; spill st
ldr r5,[r0,#0]      ; st
cmp r5,#0           ; <-- null test (4 bytes)
beq.n ...
mov r0,r5
blx getStation
blx getIndex
mov r4,r0
ldr r0,[sp,#108]
ldr r0,[r0,#0]      ; reload Globals::status
blx getCurrentCampaignMission
mov r2,r0
mov r0,r5
mov r1,r4
blx getGammaRayDamagePerSecond
```

The extra null test and the resulting slot rotation (`&Globals::layout` at
`#104` instead of `#100`, `this` at `#100` instead of `#104`) cost 20 bytes and
14 points:

| variant | size | raw % | hunks |
|---|---:|---:|---:|
| HEAD | **9332** | **96.6** | **85** |
| `st && faithful gamma` | 9352 | 85.9 | 363 |
| `st && faithful gamma`, no damage anchor | 9292 | 84.2 | 398 |
| `st && faithful gamma`, anchor on hasShieldBar | 9316 | 64.3 | 630 |

## 6. Hoisted-index variants (re-measured)

| variant | size | raw % | hunks |
|---|---:|---:|---:|
| `st` receiver, hoisted index via `st`, `st` mission | **9332** | **96.6** | **85** |
| `Globals::status` receiver, hoisted index via `st`, `Globals::status` mission | 9332 | 96.5 | 86 |
| `st` receiver, hoisted index via `st`, `Globals::status` mission | 9312 | 75.2 | 538 |
| `st` receiver, hoisted index via `Globals::status`, `Globals::status` mission | 9312 | 75.2 | 538 |

The G3 shape (2nd row) has the correct reload and 224 frame but the receiver is
load #2 (`mov r0,r5` instead of `mov r0,r4`), one hunk worse than HEAD.

## 7. Conclusion / next lever

The `this` spill and the faithful reload are individually reachable but the
combination only via a construct that adds at least one instruction (the `st &&`
null test). The remaining 85 hunks of HEAD are therefore not a gamma-block
problem: they are the gamma-body `fp`/`r9` swap and the downstream
damage/mission-panel allocation that follow from the cached-`st` gamma block.

The next untried lever is the **damage-indicator block**: the original has two
dead `ldr.w r8,[sp,#108]` reloads (`0x164176`, `0x164210`) and a `ldrd
fp,r1,[sp,#116]` (`0x164216`) that HEAD lacks, plus `mov r8,sl; mov sl,fp`
(`0x1641e8`) vs HEAD's `mov r8,fp`. Reproducing the original's `&Globals::status`
live range in the *first* damage indicator (rather than the 4th, where the
current anchor sits) is the most promising way to remove the dead-reload and
`sl`-rotation hunks without changing the gamma block.

---

## 8. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w7736
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w7736`: `score1.py` (single-function raw
ratio/hunks), `rebuild.sh` (incremental link + score), `sweep_gamma.py`
(8-way receiver/station/mission sweep), `sweep_gamma2.py`, `sweep_new.py`,
`sweep_self.py`, `sweep_self2.py`, `sweep_div.py`, `sweep_anchor.py`,
`sweep_anchor2.py`, `sweep_hoist.py`, `sweep_st2.py`, `sweep_move.py`,
`sweep_final.py`, `mi.sh` (`-mllvm -print-machineinstrs`). Pristine HEAD source:
`/dev/shm/w7736/Hud.base.cpp`. Original disassembly: `/dev/shm/w7736/orig.asm`.
