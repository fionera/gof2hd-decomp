# Hud::draw — wave 128 (w7658): the faithful gamma reload is real; the 224-byte frame is not its cause

**Status:** research note, 2026-11-07 (wave 128). Worktree `wt/w7658`, base
`master fee528cbe`. **No source change landed**: HEAD remains the best measured
variant at **9332 B / 96.600000 %** (85 diff hunks). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4506`,
`linked_exact 4506`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`.

This note extends `hud-draw-wave127-w7637.md`, `hud-draw-wave126-w7614.md`,
`hud-draw-wave125-w7520.md` and `hud-draw-gamma-reload-w102-wf215.md`. It tests
the wave-127 open question directly: **is the 224-byte frame a consequence of
the gamma `Globals::status` reload, or of the local layout elsewhere?**

---

## 0. TL;DR

1. The original gamma block is the **plain three-use / faithful `st` form**:
   receiver = load #1 (`r4`), mission = load #2 (`ldr r0,[r6,#0]` after
   `getIndex`). The Ghidra decompile is explicit:
   `this = Globals::status; getStation(Globals::status); getIndex();
   getCurrentCampaignMission(Globals::status); getGammaRayDamagePerSecond(this, ...)`.
2. The source spelling

   ```cpp
   if (Globals::status->getGammaRayDamagePerSecond(
           Globals::status->getStation()->getIndex(),
           Globals::status->getCurrentCampaignMission()) > 0.0f)
   ```

   reproduces the original's **19-instruction gamma block exactly** — same
   registers (`r6` = `&Globals::status`, `r4` = load #1, `r5` = index), same
   late `str r6,[sp,#108]`, same `mov r0,r4` / `mov r1,r5` receiver setup. The
   *only* difference is the spill offset (`sp+100` instead of `sp+108`) and the
   block's address (8 bytes earlier), i.e. a frame-size consequence.
3. The whole function then shrinks **9332 -> 9312 B** (`sub sp,#224` ->
   `#216`) and scores **75.2 % / 538 hunks**. The frame is not caused by the
   reload; the reload is correct and the frame is the missing piece.
4. Hoisting the station index restores the 224-byte frame and the original
   stack-slot set exactly (**9332 B / 96.5 % / 86 hunks**), but the receiver
   becomes load #2 (`mov r0,r5` / `mov r1,r4`) instead of load #1. This is the
   wave-126/127 result, re-confirmed on the wave-125 allocator.
5. The extra 8 bytes are **not allocated in the gamma block**. The original
   gamma block and the 216-byte faithful variant are instruction-identical
   apart from the spill offset. The extra slot is a dead 4-byte slot at
   `[sp,#56]` (plus 4 bytes of 8-byte frame alignment) created by the
   **shield/armour spill schedule earlier in the function**, not by the gamma
   local layout. The cached-`st` HEAD and the hoisted-index variant both raise
   register pressure enough to make the allocator reserve that slot; the
   faithful gamma block alone does not.

---

## 1. Measured baseline

```
_ZN3Hud4drawExxP9PlayerEgobjj orig=9332 ours=9332
  ratio=96.600000  insns=3149/3152  hunks=85
```

The 85 hunks are: gamma block (5), shield/armour spill schedule (16), dock1
register/type tie-breaks (~40), second-bar scheduling (~20), EH pad (4).

---

## 2. The faithful gamma block (E)

Source (replace only the gamma `if`; keep everything else at HEAD):

```cpp
    if (Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                    Globals::status->getCurrentCampaignMission()) > 0.0f) {
```

Original vs E, same normalised instruction sequence:

```
original 0x164010:                     E (0x164008):
ldr   r6, [r0, #0]    ; &Globals::status   ldr   r6, [r0, #0]
ldr   r4, [r6, #0]    ; load #1            ldr   r4, [r6, #0]
mov   r0, r4                               mov   r0, r4
blx   Status::getStation                   blx   Status::getStation
blx   Station::getIndex                    blx   Station::getIndex
mov   r5, r0                               mov   r5, r0
ldr   r0, [r6, #0]    ; load #2            ldr   r0, [r6, #0]
str   r6, [sp, #108]                       str   r6, [sp, #100]   <-- frame
blx   Status::getCurrentCampaignMission    blx   Status::getCurrentCampaignMission
mov   r2, r0                               mov   r2, r0
mov   r0, r4          ; receiver = #1      mov   r0, r4
mov   r1, r5                               mov   r1, r5
blx   Status::getGammaRayDamagePerSecond   blx   Status::getGammaRayDamagePerSecond
```

This is the strongest evidence yet that the original source is the plain
three-use form. The compiler CSEs the receiver and `getStation()` loads into
load #1 and reloads the global for the mission argument because the calls may
change it.

| variant | size | insns | raw % | hunks |
|---|---:|---:|---:|---:|
| **HEAD (`st` for mission, cached)** | **9332** | **3149/3152** | **96.600000** | **85** |
| E faithful plain three-use | 9312 | 3149/3146 | 75.200000 | 538 |
| E + `Status *st` receiver, `Globals::status` mission (F) | 9312 | 3149/3146 | 75.200000 | 538 |
| E + `Status *&st` / `*const &st` alias | 9312 | 3149/3146 | 75.200000 | 538 |
| E + `st` receiver, `Globals::status` station | 9312 | 3149/3146 | 75.200000 | 538 |

All faithful spellings produce the same 19-instruction block and the same
216-byte frame.

---

## 3. The 224-byte frame is the local layout, not the reload

The faithful E variant's stack-slot set is the original's shifted down by 4/8:

```
orig : 0 4 8 12 16 20 24 28 32 40 60 64 68 72 76 80 84 88 92 96 100 104 108 112 116 120 160 220
E    : 0 4 8 12 16 20 24 28 32 40 56 60 64 68 72 76 80 84 88 92  96 100 104 108 112 152 212
```

The only unreferenced slot in the original is `[sp,#56]` (no instruction in
the whole 9332-byte function touches it). E uses `[sp,#56]` for dock1 `barW`;
the original's `barW` is at `[sp,#60]`. The second 4 bytes are 8-byte frame
alignment at the top (`String` temporaries at `sp+196/208` vs `sp+188/200`;
canary `sp+220` vs `sp+212`).

Crucially, the gamma block itself is *identical* in both. `-mllvm
-print-machineinstrs` final frame objects are also identical between E and the
224-byte hoisted-index variant D (same `fi#` list, same `String` objects); the
difference is **spill slots**, not named locals. The allocator reserves the
dead `[sp,#56]` slot only when register pressure is high enough. HEAD's cached
`st` and the hoisted-index variant D both reach that pressure; E does not.

The hoisted-index variant:

```cpp
    int stationIdx = Globals::status->getStation()->getIndex();
    Status *st = Globals::status;
    if (st->getGammaRayDamagePerSecond(stationIdx,
                                       Globals::status->getCurrentCampaignMission()) > 0.0f) {
```

gives the original's exact stack-slot set and 9332 B, but its gamma block is

```
ldr   r5, [r0, #0]    ; &Globals::status
ldr   r0, [r5, #0]    ; load #1 (getStation)
blx   Status::getStation
blx   Station::getIndex
str   r5, [sp, #108]
mov   r4, r0          ; stationIdx
ldr   r5, [r5, #0]    ; load #2
mov   r0, r5          ; receiver = load #2  <-- differs
blx   Status::getCurrentCampaignMission
mov   r2, r0
mov   r0, r5          ; receiver = load #2
mov   r1, r4
blx   Status::getGammaRayDamagePerSecond
```

i.e. the receiver is the reload, not the first load. Score **96.5 % / 86
hunks**. No spelling found in this wave keeps the receiver as load #1 *and*
the 224-byte frame: naming the index (`st` receiver) collapses back to 216;
using `Globals::status` as receiver keeps 224 but makes the receiver load #2.

---

## 4. Where the extra pressure comes from

The dead `[sp,#56]` slot is allocated before the dock1 block. The gamma block
is after the shield/armour block, and its code is identical in original and E,
so the pressure must come from the **shield/armour spill schedule** (the 16
hunks listed in wave-125/127). The original has extra dead reloads around the
shield blocks (`ldr.w r8,[sp,#108]` at `0x164176`/`0x164210`, `ldrd
fp,r1,[sp,#116]` at `0x164216`) that E does not. HEAD's cached `st` raises
global pressure enough to reproduce the same slot layout; the faithful gamma
block does not. This is the next place to look for a source win: reproduce the
original shield/armour spill/reload schedule *without* the cached `st`, and the
faithful gamma block should then fit the 224-byte frame.

---

## 5. Other variants measured (all worse than HEAD)

- E + all-int dock types: 9308 B / 75.4 % (still 216-byte frame).
- E + `top` `unsigned short` / `short`, `barYOffset` `int` / `unsigned int`:
  identical code, 9308 B / 75.4 %.
- E + dock1 declaration-order sweep (frameH/textH, transferred/total,
  progress/barOffset, String/barW, barW/barH, top/barYOffset; 128 combos):
  best 9356 B / 83.0 %, all far below HEAD.
- E + guard variants (`field_0x4b8 >= 1 && Globals::status`, `!= 0`, `> 0`,
  duplicated status): 9312-9316 B / 75.1-75.2 %.
- E + function-scope `barOffset` / `shift`: 9312 B / 75.2 %.
- E + `(void)st`, `st;`, unused `volatile int pad;`: optimised away, 9312 B.
- E + `st` in the `field_0x4b8` guard: 9356 B / 61.0 % (too much pressure).

---

## 6. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w7658
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w7658`: `score1.py` (single-function raw
ratio/hunks), `variant.py` (patch + build + score), `slots.py` (stack-slot
census), `slotctx.py` (slot context), `test_gamma2.py`, `test_alias.py`,
`test_guard2.py`, `test_scope.py`, `sweep_dock2.py`. Pristine HEAD source
snapshot: `/dev/shm/w7658/Hud.base.cpp`. Ghidra decompile of the original:
`/dev/shm/w7658/ghidra_decomp.md` (`GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC
rea decompile ... 0x173b90`).
