# Hud::draw — wave 132 (w7716): the 224-byte frame is a `this`-spill, not the gamma reload

**Status:** research note, 2026-11-07 (wave 132). Worktree `wt/w7716`, base
`master 70e451ffe`. **No source change landed**: HEAD remains the best measured
variant at **9332 B / 96.600000 %** (85 hunks). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4506`,
`linked_exact 4506`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`.

This note extends `hud-draw-wave128-w7658.md`, `hud-draw-wave127-w7637.md` and
`hud-draw-wave126-w7614.md`. It tests the wave-128 open question directly:
**reproduce the original shield/armour spill/reload schedule without the
cached `st` local, so the faithful gamma block fits the 224-byte frame.**

---

## 0. TL;DR

1. The original gamma block is the plain three-use form
   (`Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
   Globals::status->getCurrentCampaignMission())`): receiver = load #1 (`r4`),
   mission = reload (`ldr r0,[r6,#0]`). This is confirmed by the original
   disassembly at `0x164010..0x164032`.
2. Every faithful spelling reproduces that 19-instruction block exactly, but
   the whole function shrinks to **9312 B / 216-byte frame / 75.2 %** (538
   hunks). The gamma block is *not* the cause of the frame.
3. The frame difference is a **`this` spill**. The original spills `this` to
   `[sp,#104]` (`str r5,[sp,#104]` at `0x163e04`) and reloads it into `r6` at
   `0x164040`; `sl` stays `dividerOffset`. The faithful variant instead moves
   `this` into `sl` at `0x163fa6` (`mov sl,r5`) and spills `dividerOffset` to
   `[sp,#92]`. The original therefore has one extra 4-byte slot plus 4 bytes
   of 8-byte frame alignment.
4. Adding a dead `st->getStation()->getIndex();` before the plain gamma block
   restores the **224-byte frame and the original gamma block** (receiver =
   load #1, mission reload), but the extra call shifts the whole body and the
   score collapses to **9332 B / 82.8 % / 395 hunks**. The pressure is the
   extra `getIndex` call, not the reload itself.
5. The hoisted-index variant (`int stationIdx = ...; Status *st = ...;`) keeps
   the 224-byte frame and the original slot set but makes the receiver
   load #2: **9332 B / 96.5 % / 86 hunks** (one hunk worse than HEAD).
6. No tested spelling keeps both the 224-byte frame *and* the load-#1
   receiver *and* the original instruction count. HEAD stays.

---

## 1. Measured baseline

```
_ZN3Hud4drawExxP9PlayerEgobjj orig=9332 ours=9332
  ratio=96.600000  insns=3149/3152  hunks=85
```

The 85 hunks are: gamma block (5), shield/armour spill schedule (16),
dock1 register/type tie-breaks (~40), second-bar scheduling (~20), EH pad (4).

---

## 2. Original gamma block (authoritative objdump)

```
164008: strd  fp, r9, [sp, #116]
16400c: ldr   r0, [pc, #276]     ; &Globals::status (GOT)
16400e: add   r0, pc
164010: ldr   r6, [r0, #0]       ; r6 = &Globals::status
164012: ldr   r4, [r6, #0]       ; r4 = Globals::status  (load #1)
164014: mov   r0, r4
164016: blx   Status::getStation
16401a: blx   Station::getIndex
16401e: mov   r5, r0             ; index
164020: ldr   r0, [r6, #0]       ; reload Globals::status (load #2)
164022: str   r6, [sp, #108]     ; spill &Globals::status
164024: blx   Status::getCurrentCampaignMission
164028: mov   r2, r0
16402a: mov   r0, r4             ; receiver = load #1
16402c: mov   r1, r5
16402e: blx   Status::getGammaRayDamagePerSecond
```

The plain source form reproduces this block instruction-for-instruction. The
only difference in the faithful build is the spill offset (`sp+100` instead of
`sp+108`) and the block address (8 bytes earlier).

---

## 3. The frame is a `this` spill

Stack-slot census (all `[sp,#N]` operands anywhere in the function):

```
orig : 0 4 8 12 16 20 24 28 32 40 60 64 68 72 76 80 84 88 92 96 100 104 108 112 116 120 160 220
faith: 0 4 8 12 16 20 24 28 32 40 56 60 64 68 72 76 80 84 88 92  96 100 104 108 112 152 212
```

The original's middle band is shifted +4 and its top band +8 relative to the
faithful variant. The extra low slot is the `this` spill at `[sp,#104]`; the
extra high 4 bytes are 8-byte frame alignment. The original spills **both**
`this` and `dividerOffset`; the faithful variant spills only `dividerOffset`
and keeps `this` in `sl`:

| value | original | faithful |
|---|---|---|
| `this` | `[sp,#104]`, reloaded `r6`/`r5`/`sl` | `sl` (moved at `0x163fa6`) |
| `dividerOffset` | `sl`, spilled `[sp,#96]` | spilled `[sp,#92]`, reloaded in gamma block |

The allocator's choice is driven by the gamma block's register pressure: the
original keeps `dividerOffset` in `sl` across the gamma block and reloads
`this` into the caller-saved `r6` after spilling `&Globals::status`. The
faithful variant frees `sl` for `this` before the gamma block and reloads
`dividerOffset` instead.

---

## 4. Variants measured (full link, official raw ratio)

| variant | size | insns | raw % | hunks |
|---|---:|---:|---:|---:|
| **HEAD (`st` for mission, cached)** | **9332** | **3149/3152** | **96.6** | **85** |
| plain three-use (faithful) | 9312 | 3149/3146 | 75.2 | 538 |
| `Status *st` + `Globals::status` mission (G1) | 9312 | 3149/3146 | 75.2 | 538 |
| hoisted index, `Globals::status` receiver (G3) | 9332 | 3149/3152 | 96.5 | 86 |
| hoisted index, `st` receiver | 9312 | 3149/3146 | 75.2 | 538 |
| `st` + dead `st->getStation()->getIndex();` + plain gamma | 9332 | 3149/3153 | 82.8 | 395 |
| `Globals::status` + dead `getStation()->getIndex();` + plain gamma | 9332 | 3149/3153 | 82.8 | 395 |
| `st` + dead `getStation();` + plain gamma | 9320 | 3149/3149 | 75.0 | 533 |
| plain + dead `ego->getShieldDamageRate();` | 9340 | 3149/3153 | 77.8 | 503 |
| plain + dead `Globals::Canvas->GetColor();` | 9328 | 3149/3151 | 73.9 | 533 |
| plain + empty `if (Globals::status) {}` in damage block | 9312 | 3149/3146 | 75.2 | 538 |
| plain + `(void) Globals::status;` in damage block | 9312 | 3149/3146 | 75.2 | 538 |
| plain + `Hud *self = this;` in shield block | 9312 | 3149/3146 | 75.2 | 538 |
| plain + `Status *st` at function top (receiver `st`) | 9344 | 3149/3133 | 65.2 | 677 |
| plain + `PaintCanvas *canvas` outside the `if` | 9308 | 3149/3147 | 74.5 | 549 |
| plain + no `canvas` local | 9308 | 3149/3144 | 75.1 | 544 |
| plain + inline `Globals::layout->field_0x1e8` | 9352 | 3149/3142 | 71.4 | 574 |
| plain + `dividerOffset` moved after `SetColor` | 9308 | 3149/3145 | 74.9 | 543 |
| plain + `dividerOffset` `short` | 9324 | 3149/3151 | 74.3 | 558 |
| plain + all-int dock1 types | 9308 | 3149/3140 | 75.4 | 528 |
| plain + `int frameH` / `int total` / `int barW` individually | 9308 | 3149/3144-3145 | 74.7-75.0 | 535-537 |
| plain + gamma in nested scope | 9312 | 3149/3146 | 75.2 | 538 |
| plain + no `Globals::status &&` anchor | 9248 | 3149/3140 | 69.1 | 512 |
| HEAD + no anchor | 9272 | 3149/3148 | 87.0 | 337 |
| HEAD + `if (Globals::status) if (field_0x4b8 >= 1)` | 9332 | 3149/3152 | 96.5 | 86 |

The dead-call variants prove the mechanism: the 224-byte frame needs an extra
live value/call across the gamma block. The cheapest one that keeps the
original gamma block is `st->getStation()->getIndex();` (10 bytes), which
restores 9332 B but costs 14 points because it shifts the body.

---

## 5. Why the dead reloads at `0x164176` / `0x164210` are not the cause

The original has two dead `ldr.w r8,[sp,#108]` reloads of `&Globals::status`
in the damage-indicator blocks (`0x164176`, `0x164210`). They are *after* the
gamma block and never read (the next `r8` write is `ldr r8,[r1]` at
`0x1641e4`). They are a consequence of the 224-byte slot layout / live-range
splitting, not its cause. The faithful variant has one dead reload at
`0x164278` (for the `Globals::status &&` anchor test) and no test in the
original. Removing the anchor drops the function to 9248 B / 69.1 %.

---

## 6. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w7716
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w7716`: `score.sh` (link + single-function
side-by-side), `score1.py` (raw ratio/hunks), `align.py` (stack-offset-masked
diff), `v*.py` (variant generators). Pristine HEAD snapshot:
`/dev/shm/w7716/Hud.base.cpp`. Original disassembly:
`/dev/shm/w7716/orig.asm`.

---

## 7. Open question for the next wave

The only remaining lever is to make the allocator spill `this` (instead of
moving it to `sl`) **without adding a call**. Candidates not yet tried:
`-mllvm`-visible source shape that extends `dividerOffset`'s live range past
the gamma block (e.g. a later `dividerOffset` use that the original source may
have), or a `this` use in the damage-indicator blocks that prevents the
`mov sl,r5` at `0x163fa6`. A source-level `dividerOffset` use after the gamma
block would keep `sl` occupied and force the `this` spill, but must not add
instructions to the original body.
