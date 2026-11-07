# Hud::draw — wave 135 (w7780): a free `dividerOffset` use at the regen pivotY gives the original register allocation

**Status:** source change landed, 2026-11-07 (wave 135). Worktree `wt/w7780`,
base `master bbffef482`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4506`,
`linked_exact 4506`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, symbol parity 5370/5370.

Result: the function goes from **96.6 % / 85 hunks** to **96.7 % / 84 hunks**
at the same **9332 B**. Whole-`.text` differing bytes go **85,022 → 85,004**
(−18). All section sizes are unchanged.

---

## 0. TL;DR

1. The wave-134 open question was: keep `armorBarY` in `sl` at `0x163f0a`
   while `dividerOffset` is live across the gamma block, with a use cheaper
   than the `armorBarY + dividerOffset` add so the size stays 9332.
2. **It works.** Passing `dividerOffset` as the regen fill's `pivotY` (a stack
   slot whose value is unused because the angle is 0) is a **free** use: it
   replaces `strd r4, r4, [sp, #12]` with `strd r4, sl, [sp, #12]` (same
   4 bytes). It extends `dividerOffset`'s live range across the `DrawImage2D`
   function-pointer load at `0x163f2e`, and the greedy allocator then produces
   the original's exact register assignment and 224-byte frame.
3. The produced shield/armour/regen/gamma region now matches the original
   instruction-for-instruction except that one `strd` source register:
   * `str r5, [sp, #104]` — `this` spill at the original address (`0x163e04`)
   * `str.w sl, [sp, #96]` — `dividerOffset` spill at `0x163e08`
   * `ldrh.w sl, [r1]` — `armorBarY` in `sl` at `0x163f0a`
   * `ldr.w r8, [r0]` — `DrawImage2D` fn ptr in `r8` at `0x163f2e`
   * `ldr.w sl, [sp, #96]` — `dividerOffset` reloaded into `sl` at `0x163fb2`
   * `ldr r6, [r0]` / `ldr r4, [r6]` / `ldr r0, [r6]` / `str r6, [sp, #108]`
     — the faithful 19-instruction gamma condition block at `0x164010`
   * `add.w r3, r4, sl` — gamma lock-bracket y uses `sl` at `0x16406a`
4. The stack-slot set is now **exactly** the original's:
   `0 4 8 12 16 20 24 28 32 40 60 64 68 72 76 80 84 88 92 96 100 104 108 112 116 120 160 220`.
   The faithful plain-three-use variant (wave 128 "E") had
   `... 40 56 60 ... 152 212` (216-byte frame, no `this` spill).

---

## 1. Measured before/after

```
before (HEAD, cached `st`):   orig=9332 ours=9332  ratio=96.600000  hunks=85
after  (faithful + anchor):   orig=9332 ours=9332  ratio=96.700000  hunks=84
```

Whole-section deltas after the change:

| section | differing bytes | size |
|---|---:|---:|
| `.text` | **85,004** (was 85,022) | 1,259,464 (exact) |
| `.rodata` | 0 | exact |
| `.data` | 0 | exact |
| `.ARM.exidx` | 0 | exact |
| `.ARM.extab` | 1,416 | 156,312 (exact) |

---

## 2. The landed source

```cpp
    if (this->hasArmorRegen) {
        int regenW = (int) ((float) ego->player->getArmorDamageRate() * 0.01f * (float) this->field_0x446);
        // `dividerOffset` as the (unused, angle is 0) pivotY is a codegen anchor: it keeps
        // `dividerOffset` live in `sl` across the gamma block and spills `this`, matching the
        // original's register allocation and 224-byte frame (see hud-draw-wave134-w7760.md).
        Globals::Canvas->DrawRegion2D(this->armorRegenFillImage, 0, 0, regenW, this->field_0x44c, 0.0f, 0, dividerOffset,
                                      this->field_0x440, armorBarY);
    }
    // Plain three-use form: the compiler CSEs the receiver/station loads into one and
    // reloads `Globals::status` for the mission argument, exactly like the original's
    // 19-instruction gamma condition block (see hud-draw-wave128-w7658.md).
    if (Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                    Globals::status->getCurrentCampaignMission()) > 0.0f) {
```

Both changes are needed together. The anchor alone on the cached-`st` HEAD
build scores 96.6 % (no gain); the faithful gamma alone scores 75.2 %
(9312 B, 216-byte frame). Together they give 96.7 % / 9332 B.

---

## 3. Why `pivotY` and not another position

`PaintCanvas::DrawRegion2D(index, srcX, srcY, width, height, angle, pivotX,
pivotY, posX, posY)` lays its stack arguments out as
`[sp+0]=width, [sp+4]=height, [sp+8]=angle, [sp+12]=pivotX, [sp+16]=pivotY,
[sp+20]=posX, [sp+24]=posY`. The original regen fill stores `0,0` at
`[sp+12]/[sp+16]` (`0x163ff4 e9cd 4403 strd r4,r4`). Replacing the *second*
zero (`pivotY`) with `dividerOffset` costs no instruction: the compiler
already has `dividerOffset` in `sl` and emits `strd r4, sl` instead of
`strd r4, r4`.

Measured positions on top of the faithful gamma (all other code identical):

| variant | size | raw % |
|---|---:|---:|
| **regen `pivotY = dividerOffset`** | **9332** | **96.7** |
| regen `pivotX = dividerOffset` | 9336 | 88.6 |
| regen `pivotY = armorBarY + dividerOffset` (wave-134) | 9336 | 88.3 |
| regen `posX = dividerOffset` | 9328 | 88.7 |
| regen `posY = dividerOffset` | 9324 | 62.0 |
| armour `pivotX/pivotY = dividerOffset` | 9328 | 73.7 |
| shield `pivotX/pivotY = dividerOffset` | 9324 | 75.3 |
| regen `srcX/srcY = dividerOffset` | 9364 | 70.0 |
| regen `width = regenW + dividerOffset` | 9352 | 87.1 |

`pivotY` is the only slot that is both after the `0x163f2e` fn-pointer load
and a free replacement for an existing zero. `pivotX` (one slot earlier)
perturbs the allocation enough to grow the function.

**Zero-valued expressions do not work.** `dividerOffset - dividerOffset`,
`dividerOffset * 0`, `0 * dividerOffset`, `dividerOffset & 0`,
`dividerOffset ^ dividerOffset`, `(dividerOffset, 0)`,
`armorBarY - armorBarY`, `armorBarY * 0` and
`(dividerOffset > 0) ? 0 : 0` all fold to the constant 0 before register
allocation and fall back to the 9312 B / 75.2 % faithful build. The value
must actually be consumed.

---

## 4. Remaining hunks (84)

The residual is now:

* the one `strd r4, sl` vs `strd r4, r4` regen-pivotY instruction (section 2);
* the gamma-body `fp`/`r9` swap (4 hunks): original loads `field_0x44a` into
  `fp` and `field_0x448` into `r9`; ours loads them into `r9`/`fp`. Reordering
  the `gammaBarY` expression (`448+448-44a`, `2*448-44a`,
  `448+(448-44a)`, named locals) does not change it;
* the damage-indicator dead `ldr.w r8,[sp,#108]` reloads and `fp`/`sl`
  rotation (~20 hunks);
* the mission-panel register allocation (~50 hunks), unchanged by this pass.

---

## 5. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w7780
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w7780`: `score1.py` (single-function ratio),
`score2.py` (ratio + difflib hunk count, `-v` for the hunk bodies),
`rebuild.sh` (incremental link + score), `batch.py` / `run1..run8.py`
(variant generators), `mi.sh` (`-mllvm -print-machineinstrs`). Original
disassembly: `/dev/shm/w7780/orig_draw.asm`; Ghidra decompile:
`/dev/shm/w7780/ghidra_draw.md` (via `rea decompile ... 0x173b90`).
