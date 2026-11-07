# Hud::draw — wave 134 (w7760): a pre-gamma `dividerOffset` use forces the `this` spill *and* the faithful gamma reload

**Status:** research note, 2026-11-07 (wave 134). Worktree `wt/w7760`, base
`master ec0acea1e`. **No source change landed**: HEAD remains the best measured
variant at **9332 B / 96.600000 %** (85 hunks). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4506`,
`linked_exact 4506`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`.

This note extends `hud-draw-wave133-w7736.md`, `hud-draw-wave132-w7716.md` and
`hud-draw-wave128-w7658.md`. It tests the wave-133 open question directly:
**make the allocator spill `this` instead of moving it to `sl`, while keeping
the faithful gamma reload, without adding a call.**

---

## 0. TL;DR

1. **New positive result.** Adding a *pre-gamma* `dividerOffset` use —
   `armorBarY + dividerOffset` in the `hasArmorRegen` `DrawRegion2D` — makes the
   allocator do **both** things the previous three waves could not combine:
   * it spills `this` at the shield block (`str r5,[sp,#104]` at `0x163e04`,
     exactly the original), and
   * it emits the faithful gamma block (`ldr r6,[r0]` / `ldr r4,[r6]` /
     `ldr r0,[r6]` reload / `str r6,[sp,#108]`), byte-for-byte the original
     19-instruction condition block.
   The variant is **9336 B / 88.3 % raw / 97.14 % masked-diff**.
2. It does **not** beat HEAD because the extra live range makes the greedy
   allocator hand **`sl` to the `DrawImage2D` function pointer and `r8` to
   `dividerOffset`**, the exact opposite of the original (`r8` = function
   pointer, `sl` = `dividerOffset`). That register swap cascades through the
   whole armour/gamma/damage region and costs ~8 raw points.
3. The extra `+ dividerOffset` also adds one `add` instruction, so the size is
   9336 not 9332. Removing the `Globals::status &&` anchor gives 9276 B /
   86.5 % (frame shrinks to 216).
4. A `dividerOffset` use in the *shield* block, *armour* block, *gamma body*,
   *rocket* block or *damage* block does **not** force the `this` spill (the
   allocator keeps `this` in `sl`); only the `hasArmorRegen` use does, because
   it extends `dividerOffset` across the function-pointer load at `0x163f2e`.
5. `this` aliases (`Hud *self = this;` at top, in the rocket block, in the
   damage block, in all damage blocks) are coalesced away and change nothing
   (9312 B / 75.2 %). `const int dividerOffset`, `unsigned short armorBarY`
   and `int armorY` are also no-ops.

---

## 1. Measured baseline

```
_ZN3Hud4drawExxP9PlayerEgobjj orig=9332 ours=9332
  ratio=96.600000  insns=3149/3152  hunks=85
```

Masked-diff ratio (stack offsets + branch targets + pool words masked):
**97.51 %**. The residual is the cached-`st` gamma condition (5 hunks), the
gamma-body `fp`/`r9` swap (4), the two dead `ldr.w r8,[sp,#108]` reloads, the
`Globals::status &&` anchor (3), the `r8`/`sl` swaps around the damage blocks,
and the mission-panel register allocation (~50 hunks).

---

## 2. The new variant: `armorBarY + dividerOffset` in `hasArmorRegen`

Source (on top of the plain three-use gamma; the only other change is the
`hasArmorRegen` draw):

```cpp
    if (this->hasArmorRegen) {
        int regenW = (int) ((float) ego->player->getArmorDamageRate() * 0.01f * (float) this->field_0x446);
        Globals::Canvas->DrawRegion2D(this->armorRegenFillImage, 0, 0, regenW, this->field_0x44c, 0.0f, 0, 0,
                                      this->field_0x440, armorBarY + dividerOffset);   // <-- new use
    }
```

Produced shield-block entry (objdump, ours `0x163e04`):

```
163e04: str   r5, [sp, #104]     ; this spill  -- original has this
163e06: cmp   r0, #0
163e08: str.w sl, [sp, #96]      ; dividerOffset spill -- original has this
```

Produced gamma condition block (`0x164010`):

```
164010: ldr   r0, [pc, #276]
164012: add   r0, pc
164014: ldr   r6, [r0, #0]       ; r6 = &Globals::status
164016: ldr   r4, [r6, #0]       ; r4 = Globals::status (load #1)
164018: mov   r0, r4
16401a: blx   Status::getStation
16401e: blx   Station::getIndex
164022: mov   r5, r0
164024: ldr   r0, [r6, #0]       ; reload (load #2)  -- original has this
164026: str   r6, [sp, #108]     ; -- original has this
164028: blx   Status::getCurrentCampaignMission
16402c: mov   r2, r0
16402e: mov   r0, r4             ; receiver = load #1
164030: mov   r1, r5
164032: blx   Status::getGammaRayDamagePerSecond
```

This is the original gamma block **exactly** (same registers, same reload, same
`[sp,#108]` spill), and the `this` spill is at the original address. The
gamma-body reload `ldr r6,[sp,#104]` at `0x164044` also matches the original's
`0x164040`.

### Why it still loses

The extra `dividerOffset` use extends its live range across the
`DrawImage2D` function-pointer load at `0x163f2e`. The greedy allocator then
assigns:

| value | original | `regen` variant |
|---|---|---|
| `dividerOffset` | `sl` (`0x163df4`), reloaded `sl` `0x163fb2` | `r8` (`0x16400e`), `sl` given to fn ptr |
| `DrawImage2D` fn ptr | `r8` (`0x163f2e`) | `sl` (`0x163f2e`) |
| `armorBarY` | `sl` (`0x163f0a`) | `r8` (`0x163f0a`) |

The `armorBarY`/fn-ptr swap at `0x163f0a`/`0x163f2e` is the root: because
`armorBarY` no longer occupies `sl`, `sl` is free when the fn pointer is
defined, so the fn pointer takes it and `dividerOffset` is pushed to `r8`.
That swap cascades through all three armour draws, the gamma body
(`add.w r3,r4,r8` instead of `add.w r3,r4,sl`) and the damage blocks.

| variant | size | insns | raw % | masked % |
|---|---:|---:|---:|---:|
| **HEAD (`st` mission, cached)** | **9332** | **3149/3152** | **96.6** | **97.51** |
| `regen` (`armorBarY + dividerOffset`) | 9336 | 3149/3155 | 88.3 | 97.14 |
| `regen`, no `Globals::status &&` anchor | 9276 | 3149/3151 | 86.5 | — |
| `regen` + gamma-body use | 9340 | 3149/3156 | 88.1 | — |
| `regen` + shield use | 9332 | 3149/3153 | 88.9 | 93.30 |
| `regen` + armour use | 9336 | 3149/3152 | 59.6 | — |
| `regen` + rocket use | 9352 | 3149/3147 | 77.4 | — |
| `regen` + `unsigned short armorBarY` | 9336 | 3149/3155 | 88.3 | — |
| `regen` + `int armorY` | 9336 | 3149/3155 | 88.3 | — |

`regen + shield` is the only one that keeps 9332 B, but it loses the `this`
spill (`0x163e04` is `ldr.w r0,[fp]`) and drops to 93.3 % masked.

---

## 3. Variants that do *not* force the `this` spill

All measured on top of the plain three-use gamma (faithful). None reaches
9332 B with the `this` spill; all keep `this` in `sl` (`mov sl,r5` at
`0x163faa`).

| variant | size | insns | raw % |
|---|---:|---:|---:|
| plain three-use gamma (faithful) | 9312 | 3149/3146 | 75.2 |
| `dividerOffset` in shield `DrawRegion2D` y | 9324 | 3149/3151 | 75.7 |
| `dividerOffset` in armour `DrawRegion2D` y | 9332 | 3149/3153 | 82.5 |
| `dividerOffset` in gamma-body `DrawRegion2D` y | 9324 | 3149/3149 | 75.5 |
| `dividerOffset` in rocket reticle x | 9352 | 3149/3149 | 82.5 |
| `dividerOffset` in rocket image_0x31c y | 9316 | 3149/3148 | 75.2 |
| `dividerOffset` in damage draw x | 9336 | 3149/3151 | 72.3 |
| `dividerOffset` in damage draw y | 9328 | 3149/3147 | 65.5 |
| `dividerOffset` in mission-panel image y | 9340 | 3149/3149 | 82.8 |
| `dividerOffset` in mission-panel DrawString x | 9320 | 3149/3144 | 75.8 |
| `dividerOffset` in pause/map button y | 9320 | 3149/3144 | 75.7 |
| `ego->isInRocketControl() && dividerOffset >= 0` | 9316 | 3149/3148 | 75.7 |
| `const int dividerOffset` | 9312 | 3149/3146 | 75.2 |
| `unsigned short armorBarY` | 9312 | 3149/3146 | 75.2 |
| `Hud *self = this;` (top / rocket / damage / all damage) | 9312 | 3149/3146 | 75.2 |
| `Hud *self` in damage + `+ dividerOffset` in SetColor | 9316 | 3149/3147 | 73.3 |
| hoisted index G3 (`Globals::status` receiver/mission) | 9332 | 3149/3152 | 96.5 |

The `regen` use is special because it is the only pre-gamma `dividerOffset`
use that lands *after* the fn-pointer load and *before* the gamma block, so it
extends the interval across the fn-pointer definition without extending it past
the gamma body.

---

## 4. Open question for the next wave

The `regen` variant proves the `this` spill and the faithful gamma reload are
simultaneously reachable from source. The remaining blocker is the
`armorBarY`/fn-pointer `sl`/`r8` tie-break. The next lever is a source shape
that keeps `armorBarY` in `sl` at `0x163f0a` while `dividerOffset` is live
across the gamma block — e.g. a `dividerOffset` use that is *cheaper* than the
`armorBarY + dividerOffset` add (so the size stays 9332) but still extends the
interval across `0x163f2e`. The mission-panel allocation (~50 hunks) is
independent of the gamma variant: it is identical in HEAD and in the hoisted
index G3 variant, so it must be attacked separately.

---

## 5. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w7760
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w7760`: `score1.py` (single-function raw
ratio/hunks via the verify module), `rebuild.sh` (incremental link + score),
`maskdiff.py` (stack-offset/branch/pool-masked diff), `patch.py` /
`patch2.py` / `batch.py` / `run_batch1..7.py` (variant generators). Pristine
HEAD source: `/dev/shm/w7760/Hud.base.cpp`. Original disassembly:
`/dev/shm/w7760/orig_draw.asm`. Ghidra decompile confirms the original source
is the plain three-use gamma and has **no** `Globals::status &&` anchor in the
damage block (`if (0 < *(int *)(this_01 + 0x4b8))`).
