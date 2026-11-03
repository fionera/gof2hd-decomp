# Hud::draw — the status spill slot: exact slot map at the original 9332-byte size (w4253)

**Status:** code change landed, 2026-11-03. Worktree `wt/w4253`, branch `wave92-w4253`,
base `b379d00d`, parent `fefc4e9f`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`:
**88.8 % -> 91.7 %**, size **9332 -> 9332** (unchanged, which is mandatory — see §6).

Gate before/after (full `check`): **byte_exact 4499 / linked_exact 4499** (unchanged),
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`, lint CLEAN, drift 0, **RATCHET PASS**.

This note extends `hud-draw-status-slot-w4233.md`, `hud-draw-st-local-w4215.md`,
`hud-draw-slot-coloring-w4114.md`, `hud-draw-slot-rotation-w4014.md` and
`hud-draw-this-spill-w3910.md`.

## 0. Result

The landed source shape is

```cpp
    // Local `Status *` is load-bearing for codegen: it forces the branch-local spill of
    // `this` in the shield block, matching the original (see hud-draw-status-slot-w4253.md).
    Status *st = Globals::status;
    if (st->getGammaRayDamagePerSecond(st->getStation()->getIndex(),
                                       st->getCurrentCampaignMission()) > 0.0f) {
        ...
    }
    ...
    // The null guard is also load-bearing: it makes the allocator reload &Globals::status
    // in the damage block, which colours the status spill to the original's #108 slot.
    if (Globals::status == 0 || this->field_0x4b8 >= 1) {
        ...
    }
```

on top of the faithful `clean` base (HEAD minus the two invented anchors: the shield
`canvas->SetColor(savedColor)` and the `pad_` local + its `if (pad_ != 0) SetColor(savedColor)`).

It reproduces the original's **complete 16-slot stack map** and the original's
**9332-byte size**, and lifts the official fuzzy score from 88.8 % to 91.7 %.

## 1. Why size 9332 is a hard constraint

`verify.py`'s `byte_exact` compares the raw bytes of every function. `Hud.cpp` is
TU #179 of 203 in `tools/source_order.txt`, so any size change in `Hud::draw` shifts
every later function and changes their PIC literal deltas. Measured:

| build | Hud::draw size | gate `byte_exact` |
|---|---|---|
| HEAD (baseline) | 9332 | 4499 |
| `st` (clean + `Status *st` gamma local) | 9264 | **2994** |
| `st_n_b8` (`st` + `Globals::status &&` guard) | 9336 | **2944** |
| **landed `or_b8`** | **9332** | **4499** |

So a variant that changes the size cannot land, however good its fuzzy score. The
landed shape is the only one found that both fixes the slot map and keeps 9332 bytes.

## 2. Ground truth slot map (image base 0x10000)

Original frame 224:

```
#60  damage scratch   #64  timeLeft lo   #68  timeLeft hi   #72  fireImageIndex
#76  secondaryKind    #80  mining        #84  savedColor    #88  &guard
#92  iPad             #96  dividerOffset #100 &Globals::layout
#104 this             #108 Globals::globals / &Globals::status
#112 dt               #116 ego           #120 &Globals::Canvas
#160 String temp      #220 canary
```

Landed build frame 224 — **identical**:

```
#60  damage scratch   #64  timeLeft lo   #68  timeLeft hi   #72  fireImageIndex
#76  secondaryKind    #80  mining        #84  savedColor    #88  &guard
#92  iPad             #96  dividerOffset #100 &Globals::layout
#104 this             #108 &Globals::status
#112 dt               #116 ego           #120 &Globals::Canvas
```

Before this pass, HEAD put `pad_` at `#72`, `savedColor` at `#100`,
`&Globals::layout` at `#104` and had no `this` slot; the `st` local alone put
`status/divider/layout/this` at `#96/#100/#104/#108` (a +4 rotation).

## 3. The shield block now matches byte-for-byte

Original:

```
163e00: ldrb.w r0, [r5, #543]      ; this->hasShieldBar
163e04: str    r5, [sp, #104]      ; *** spill this ***
163e06: cmp    r0, #0
163e08: str.w  sl, [sp, #96]       ; spill dividerOffset
163e0c: beq.n  163efc
```

Landed build:

```
210: ldrb.w r0, [r5, #543]
211: str    r5, [sp, #104]
212: cmp    r0, #0
213: str.w  sl, [sp, #96]
214: beq.n  0x36c
```

This is the branch-local `this` split the previous passes could not reproduce
(`clean` coalesced the copies to `$r5`; `st` reproduced it but rotated the high
slots). The landed shape gets both.

## 4. The damage block: the null guard is what colours `status` to `#108`

Stack Slot Coloring processes intervals by spill weight (sum over frame-index
operands of `(isDef+isUse) * blockFreq/entryFreq`). In the `st` build the status
interval only has cold mission-panel references, so `this` (hot shield/gamma/
damage references) wins `#108` and `status` takes the leftover `#96`.

The landed null guard makes the allocator materialise `&Globals::status` in the
damage block (the hottest block that is still on the path to the mission panel),
which is exactly what the original does:

Original (3 references in the damage block, 2 of them dead):

```
164176: ldr.w r8, [sp, #108]       ; dead (r8 clobbered at 1641e4)
164210: ldr.w r8, [sp, #108]       ; dead (r8 clobbered at 16431c)
16436e: ldr   r6, [sp, #108]       ; live: r8 = &Globals::status for the mission panel
16437a: mov   r8, r6
```

Landed build (the same three references, one of them a real null test):

```
613: ldr.w r1, [r8]                ; Globals::status   (null guard)
614: ldr.w r0, [sl, #1208]         ; this->field_0x4b8
615: cbz   r1, 0x76c               ; guard
616: cmp   r0, #1
617: blt.n 0x7fa
...
662: ldr   r6, [sp, #108]          ; &Globals::status
663: ldr.w r9, [sp, #120]
664: asrs  r2, r0, #1
665: mov   r0, r4
666: mov   r8, r6
667: blx   DrawImage2D
```

The extra `ldr/cbz` pair is 4 bytes; the `st` base was 68 bytes short, so the
result lands exactly on 9332. The guard is an anchor (the original source has no
null test there), and it is documented in the source comment.

Placement matters: the same `Globals::status == 0 ||` guard on the 1st/2nd/3rd
damage condition gives 9340/9340/9340 bytes and 88.3 %, and on `hasShieldBar`,
`shieldHitFlash`, `hasArmorRegen`, `isInRocketControl`, `hasCloak`,
`getBoostRate`, the mission-panel `if`s gives 9252-9336 bytes and 62.9-89.3 %.
Only the 4th indicator (`field_0x4b8`) lands on 9332.

## 5. Variant table (official verifier, `autojunk=False`)

| variant | size | fuzzy |
|---|---|---|
| HEAD (before) | 9332 | 88.8 % |
| clean (faithful entry/shield/gamma, no `this` slot) | 9240 | 68.1 % |
| `st` = clean + `Status *st` before gamma | 9264 | 83.3 % |
| `st` + `Globals::status &&` on b8 | 9336 | 90.5 % |
| `st` + `Globals::status &&` on b0 / b4 | 9340 | 88.3 % |
| `st` + `Globals::status &&` on ac | 9348 | 85.5 % |
| `st` + `Globals::status &&` on shieldHitFlash | 9336 | 89.3 % |
| `st` + `Globals::status == 0 \|\|` on b8 | **9332** | **91.7 %** |
| `st` + `Globals::status == 0 \|\|` on ac/b0/b4 | 9340 | 88.3 % |
| `clean` + `Globals::status == 0 \|\|` on b8 (no `st`) | 9312 | 74.3 % |
| `st` + `Globals::status->getMission() != 0` on b8 | 9348 | 88.1 % |
| `st` + `field_0x180 >= 0` on b8 | 9344 | 88.2 % |
| `st` + two guards (ac+b8, b0+b8, ...) | 9292-9712 | 57.0-88.2 % |
| gamma `recv_st_args_clean` + guard | 9312 | 74.3 % |
| gamma `recv_clean_args_st` / `recv_st_arg2_st` + guard | 9332 | 91.7 % |
| `Status *&` / `Status **` gamma + guard | 9312 | 74.3 % |
| `st` moved before iPad/savedColor/shield/divider + guard | 9328-9356 | 63.9-74.5 % |

The guard spelling (`== 0`, `== nullptr`, `!`, `== 0 ||` vs `!= 0 &&`) does not
change the code; `const` on `st` does not either.

## 6. What still differs (91.7 %, 232 diff hunks)

All remaining differences are register-allocation / scheduling artifacts, not
source semantics:

1. **Gamma block store placement** (2 hunks at 0x16460/0x16474). The original
   keeps `&Globals::status` in `r6` across `getStation`/`getIndex` and spills it
   after the reload (`ldr r6,[r0,#0]; ldr r4,[r6,#0]; ...; ldr r0,[r6,#0];
   str r6,[sp,#108]`). The `st` local caches the value, so ours spills early
   (`ldr r0,[r0,#0]; str r0,[sp,#108]; ldr r4,[r0,#0]; ...; mov r0,r4`).
   Getting the clean gamma shape *and* the `this` spill *and* 9332 bytes was not
   reachable: `clean` gamma + any `Status *` local either hoists the load or
   loses the `this` spill.
2. **Two dead status reloads** at 0x164176/0x164210 are absent in ours (the guard
   supplies one hot reference instead of two dead ones).
3. **Branch offsets** are shifted by 2 in the damage block and downstream because
   the guard adds 4 bytes before the code that the original's dead reloads occupy
   differently. These account for most of the 232 hunks.
4. **Mission-panel register allocation** (`fp`/`r9` swaps, `mov r5,r4` /
   `ldr r4,[r9]` ordering, a `nop`) — downstream of (1)/(2).
5. `mov sl, r8` (the `sl` restore before the 4th `DrawImage2D`) is missing.

The gamma store placement is the single largest remaining structural item. A
source shape that keeps `Globals::status->...` written three times in the gamma
condition, still spills `this` in the shield block, and still lands on 9332 bytes
would be the next target.

## 7. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8     # byte 4499 linked 4499, RATCHET PASS
cmake --build cmake-build-match --target drift -j8     # 0 drifted fields
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```
