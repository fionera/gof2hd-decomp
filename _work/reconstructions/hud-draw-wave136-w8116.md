# Hud::draw — wave 136 (w8116): the pivotY anchor is still the only way to tip the `this`/`dividerOffset` allocation

**Status:** research note, 2026-11-07 (wave 136). Worktree `wt/w8116`, base
`master 2e51d765d`. **No source change landed**: the wave-135 pivotY anchor is
still the best measured variant. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4506`,
`linked_exact 4506`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`.

Measured baseline (exact, difflib on the normalized instruction stream):
**96.68307 % / 84 hunks / 3149 vs 3152 insns**, 9332 B.

---

## 0. TL;DR

1. The wave-135 `dividerOffset`-as-regen-`pivotY` anchor is present and is
   still the best variant. It is the only source construct found so far that
   makes the greedy allocator (a) spill `this` at `0x163e04`
   (`str r5,[sp,#104]`), (b) keep `armorBarY` in `sl` at `0x163f0a`
   (`ldrh.w sl,[r1]`), and (c) reload `dividerOffset` into `sl` at
   `0x163fb2` (before the gamma condition) instead of lazily inside the gamma
   body.
2. Its only visible trace is the regen `DrawRegion2D` pivot store:
   ours `strd r4, sl, [sp,#12]` vs the original `strd r4, r4, [sp,#12]`
   (`0x163ff4`). That is the first of the 84 hunks.
3. **Every** attempt to remove that trace while keeping the allocation either
   folds the use to a constant before register allocation (back to the
   9312 B / 75.17 % plain build) or perturbs the allocation (worse).
4. A new, useful negative result: hoisting the gamma mission into a local
   (`int mission = Globals::status->getCurrentCampaignMission();`) reproduces
   the exact `this` spill **and** the exact `strd r4, r4`, but it reorders the
   gamma condition block (mission before station) and scores only
   **96.39740 % / 85 hunks**. Caching the receiver instead
   (`Status *st = Globals::status; ...`) reproduces the original gamma block
   byte-for-byte but loses the spill (9312 B / 75.17 %). The two levers are
   mutually exclusive from source.

---

## 1. Mechanism (machine IR evidence)

`-mllvm -print-machineinstrs -mllvm -filter-print-funcs=_ZN3Hud4drawExxP9PlayerEgobjj`
after the greedy allocator:

* **Plain three-use build** (no pivotY): `dividerOffset` lives in spill slot
  `%stack.59`. It is stored at 5464B (`0x163e04`) and reloaded at 7812B
  (armor lock bracket, `0x163f2e`) and 10104B (gamma bracket, `0x16405a`,
  inside the gamma body). `this` is moved into `sl` at `0x163faa`
  (`mov sl, r5`).
* **pivotY build** (wave 135): `this` is in `%stack.59` and `dividerOffset`
  in `%stack.64`. The second reload is hoisted to 8712B — right after the
  `hasArmorRegen` byte load and before its `cbz` — matching the original's
  `ldr.w sl,[sp,#96]` at `0x163fb2`. The reload is in the same basic block as
  the `hasArmorRegen` branch because the value is now used in the regen
  successor block (the pivotY), so the allocator hoists it to cover both
  successors. `this` therefore loses `sl` and is spilled at `0x163e04`.

So the lever is exactly: **a use of `dividerOffset` in the `hasArmorRegen`
block that costs no instruction**. The regen `DrawRegion2D` pivot slot is the
only such slot.

---

## 2. Free-use sweep (all measured on top of the wave-135 pivotY anchor)

The regen call is
`DrawRegion2D(armorRegenFillImage, 0, 0, regenW, field_0x44c, 0.0f, 0, PIVOT_Y, field_0x440, armorBarY)`.
Replacing the pivotY `0` with `dividerOffset` is free (`strd r4, sl` replaces
`strd r4, r4`, same 4 bytes). Every other zero slot was tried:

| variant | size | raw % | hunks |
|---|---:|---:|---:|
| **regen pivotY = dividerOffset (landed, wave 135)** | **9332** | **96.68307** | **84** |
| regen pivotX = dividerOffset | 9336 | 88.62 | 308 |
| regen pivotX = pivotY = dividerOffset | 9352 | 86.7 | — |
| regen srcX / srcY = dividerOffset | 9364 | 70.0 | — |
| regen posX = dividerOffset | 9328 | 88.70 | 308 |
| regen posY = dividerOffset | 9324 | 62.01 | 657 |
| armor fill pivotY = dividerOffset | 9328 | 73.7 | — |
| armor fill pivotX / both = dividerOffset | 9328 | 73.7 | — |
| armor fill srcX / srcY = dividerOffset | 9332 | 63.5 | — |
| shield fill pivotY / pivotX = dividerOffset | 9324 | 75.3 | — |
| gamma fill pivotY = dividerOffset | 9324 | 75.5 | — |
| regen pivotY = `(float)dividerOffset * 0.0f` (angle slot) | 9356 | 88.4 | — |

Zero-valued expressions all fold before register allocation and fall back to
the plain 9312 B / 75.17077 % build: `dividerOffset - dividerOffset`,
`dividerOffset * 0`, `dividerOffset & 0`, `dividerOffset & 0x7fffffff & 0`,
`dividerOffset ^ dividerOffset`, `dividerOffset % 1`,
`dividerOffset % 2 - dividerOffset % 2`, `dividerOffset / 2147483647 * 0`,
`(dividerOffset, 0)`, `(float)(dividerOffset - dividerOffset)`.

A volatile read (`*(volatile int *)&dividerOffset`) does keep a use, but
forces the local to memory and destroys the allocation (9196 B / 67.67 %).

---

## 3. Gamma-condition forms (the other allocation lever)

The original gamma block is
`Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(), Globals::status->getCurrentCampaignMission())`
(station evaluated first, then a reload of `Globals::status` for the mission,
receiver is load #1). On the **plain** (no pivotY) base:

| form | size | raw % | hunks | notes |
|---|---:|---:|---:|---|
| faithful three-use (current) | 9312 | 75.17077 | 538 | `this` stays in `sl` |
| `int mission = Globals::status->getCurrentCampaignMission();` hoisted | **9332** | **96.39740** | **85** | correct `this` spill + `strd r4,r4`; gamma block reordered (mission first) |
| `Status *st = Globals::status; idx = st->...; mission = Globals::status->...;` | 9312 | 75.17077 | 538 | gamma block byte-exact, but no spill |
| `int idx = ...; int mission = ...;` (both hoisted) | 9328 | 73.70929 | 540 | 3 `Globals::status` loads |
| `float gamma = ...; if (gamma > 0.0f)` | 9312 | 75.17077 | 538 | same as faithful |

On top of the pivotY anchor, the `Status *st` form is identical to the base
(96.68307 %, 84 hunks); the mission-hoisted form is 96.36566 % / 86 hunks.

So the mission hoist is a *second* independent way to tip the
`this`/`dividerOffset` allocation, and it even removes the `strd` trace — but
it costs the gamma block order, which is a net loss (96.397 < 96.683).

---

## 4. Other variants tried (all no-ops or worse)

* `unsigned short armorBarY` / `int armorY` / both: **identical**
  (96.68307 %, 84 hunks).
* Reordering the `gammaY`/`gammaBarY` declarations, named locals
  (`g448`/`g44a`), `(int)` casts, `2 * 448 - 44a`, `448 + 448 - 44a`:
  **identical** (the gamma-body `fp`/`r9` swap is a vreg-number tie-break,
  not a source-order effect).
* Moving the `Globals::status &&` damage anchor from the 4th damage block
  (`field_0x4b8`) to the 1st/2nd/3rd (`field_0x4ac`/`4b0`/`4b4`): 88.6 / 89.1
  / 89.2 % on the pivotY base; removing it entirely: 9272 B / 87.1 %.
* `Globals::status != 0 &&` spelling: identical. Reordering the anchor
  (`field_0x4b8 >= 1 && Globals::status`): 9336 B / 89.08 %.
* `hasArmorRegen && dividerOffset >= 0` in place of the pivotY anchor:
  9324 B / 72.25 %.

---

## 5. Remaining hunks (84)

* `strd r4, sl` vs `strd r4, r4` at `0x163ff4` — the pivotY trace (1 hunk).
* gamma-body `fp`/`r9` swap (4 hunks).
* damage-indicator dead `ldr.w r8,[sp,#108]` reloads and `sl` rotation
  (~20 hunks). The original reloads `&Globals::status` into `r8` at
  `0x164176` and `0x164210`; ours reloads once at `0x16428c`.
* mission-panel / jump-drive-bar register allocation (~50 hunks), unchanged.
* EH pad (3 hunks).

The damage-block dead reloads are the most promising remaining target: they
are a pure `&Globals::status` spill/reload placement artifact, independent of
the gamma block. Reproducing them would need a source use of `Globals::status`
in the first damage block that costs no instruction, which was not found this
wave (the `Globals::status &&` anchor adds a 4-byte null test).

---

## 6. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w8116
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w8116`: `score.py` (verify single-function
score), `score2.py` (exact ratio + hunk count), `hunks.py` (hunk bodies),
`dump.py` (address-range disassembly), `sweep.py` (variant JSON sweep),
`mi_cmd2.sh` (`-print-machineinstrs` filtered to `Hud::draw`),
`mi_plain.txt` / `mi_pivot.txt` (greedy-allocator dumps),
`greedy_plain.txt` / `greedy_pivot.txt` (extracted greedy sections).
Original disassembly: `/dev/shm/w8116/orig_draw.asm`.
