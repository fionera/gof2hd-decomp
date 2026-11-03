# Hud::draw — the exact gamma store order, the dock-block statement order, and the per-function LSDA budget (w4515)

**Status:** research note, 2026-11-03 (wave 95). Worktree `wt/w4515`, branch
`wave95-w4515`, base `master 4a88bd6e`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp:1091`), original size **9332 B**, current fuzzy **89.1 %**.

Gate on entry and on exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26290`, lint CLEAN, drift 0, **RATCHET PASS**.
No source change landed: no variant was found that is a strict gate improvement
(see §5). This note extends `hud-draw-extab-lsda-w4273.md`,
`hud-draw-status-slot-w4253.md`, and `hud-draw-slot-coloring-w4114.md`.

---

## 0. TL;DR

1. **The original gamma block is the plain three-use textual form.**
   Ghidra and the disassembly agree that the source is
   `Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(), Globals::status->getCurrentCampaignMission())`
   — no `Status *` local. Clang evaluates the receiver first (one load into
   `r4`), CSEs the `getStation` receiver with it, then reloads the global for
   the second argument after the two `getStation`/`getIndex` calls, and only
   then spills `&Globals::status` (`r6`) to `[sp,#108]`.
2. **The current `Status *st` local is load-bearing only because it is the
   only source shape found that makes clang 7.0.2 spill `this` at the
   `hasShieldBar` merge.** Every shape that reproduces the gamma reload
   (`arg2 = Globals::status->getCurrentCampaignMission()`) loses the `this`
   spill: the frame becomes 216, the function 9312 B, and the score 72.9 %.
   The landed `st` shape keeps the spill (frame 224, 9332 B, 89.1 %) but caches
   `Globals::status` in `r4` and spills the address early.
3. **The docking-transfer block's real statement order is now pinned down**
   (Ghidra + disassembly): `frameH, textH, transferred, total, getText,
   String(" "), progress, barOffset, operator+, ~String, barW, barH,
   field_0x468 += dt, fadeIn, fade`. In source that is
   `String text = *Globals::gameText->getText(...) + String(" ");` placed
   **before** `progress` (the compiler schedules `operator+` after
   `barOffset`). Reorders measured: `P B T W H D` = **90.7 %**,
   `T P B W H D` = 89.8 %, both at 9332 B — but both grow `.ARM.extab` by
   12 bytes.
4. **The LSDA budget is now itemised.** `.ARM.extab` is 0x26290, i.e. exactly
   **8 bytes of headroom** before `.rodata`'s 16-byte-aligned base moves. The
   dock reorder needs **12**. Comparing every function's `.ARM.extab` entry
   against the original shows the whole 8-byte deficit is due to **four**
   functions (table in §4); the only two that are *larger* than the original
   are `Generator::createMission` (+4) and `LevelScript::process` (+4). Either
   one shaved by 4 bytes would make the 90.7 % dock reorder landable.

---

## 1. Ground truth: the gamma block

Image base 0x10000. Original `Hud::draw` @ 0x173b90:

```
16400c  ldr   r0, [pc, #276]        ; GOT &Globals::status
16400e  add   r0, pc
164010  ldr   r6, [r0, #0]          ; r6 = &Globals::status
164012  ldr   r4, [r6, #0]          ; r4 = Globals::status   (receiver / arg1 receiver)
164014  mov   r0, r4
164016  blx   Status::getStation
16401a  blx   Station::getIndex
16401e  mov   r5, r0                ; r5 = station index
164020  ldr   r0, [r6, #0]          ; *** reload Globals::status (arg2) ***
164022  str   r6, [sp, #108]        ; *** spill &Globals::status AFTER the reload ***
164024  blx   Status::getCurrentCampaignMission
164028  mov   r2, r0
16402a  mov   r0, r4                ; receiver
16402c  mov   r1, r5                ; arg1
16402e  blx   Status::getGammaRayDamagePerSecond(int,int)
164032  vmov  s0, r0
164036  vcmpe.f32 s0, #0.0
16403a  vmrs  APSR_nzcv, fpscr
16403e  ble.n 1640e0
```

Ghidra renders the same thing as

```
piVar33 = *(int **)(iRam00174124 + 0x174012);   // &Globals::status
this_01 = (Status *)*piVar33;                    // one value load
pSVar18 = Status::getStation(this_01);
iVar41 = Station::getIndex(pSVar18);
iVar11 = Status::getCurrentCampaignMission((Status *)*piVar33);   // reload
fVar14 = (float)Status::getGammaRayDamagePerSecond(this_01,iVar41,iVar11);
```

so the source is the plain nested expression with **three textual
`Globals::status` uses**:

```cpp
if (Globals::status->getGammaRayDamagePerSecond(Globals::status->getStation()->getIndex(),
                                                Globals::status->getCurrentCampaignMission()) > 0.0f) {
```

The `r4` receiver load and the `getStation` receiver load are the same load
(adjacent, no intervening call); the `getCurrentCampaignMission` load is a
reload because `getStation`/`getIndex` may write the global. The address
`r6` is spilled to `#108` only after the reload, because it is live across
those calls and again in the mission panel.

HEAD instead caches the value:

```
164010  ldr   r0, [r0, #0]          ; &Globals::status
164012  str   r0, [sp, #108]        ; *** spill BEFORE the value load ***
164014  ldr   r4, [r0, #0]          ; r4 = Globals::status
164016  mov   r0, r4
164018  blx   Status::getStation
16401c  blx   Station::getIndex
164020  mov   r5, r0
164022  mov   r0, r4                ; *** no reload ***
164024  blx   Status::getCurrentCampaignMission
```

Same length (0x16400c..0x16403e); only the register (`r0` vs `r6`) and the
spill position differ.

---

## 2. The `this`-spill / gamma-store tension

The shield block is byte-identical at HEAD, including the branch-local
`this` spill:

```
163e00  ldrb.w r0, [r5, #543]       ; this->hasShieldBar
163e04  str    r5, [sp, #104]       ; *** spill this ***
163e06  cmp    r0, #0
163e08  str.w  sl, [sp, #96]        ; spill dividerOffset
163e0c  beq.n  163efc
...
163e1e  ldr    r0, [sp, #104]       ; reload this
163e20  ldrb.w r0, [r0, #580]       ; shieldHitFlash
163e24  cbz    r0, 163e2e
163e26  ldr    r5, [sp, #104]       ; reload this
163e28  add.w  r0, r5, #680
...
163e2e  ldr    r5, [sp, #104]       ; reload this
163e30  add.w  r0, r5, #676
```

This spill is the single upstream cause of the rest of the function's
allocation (it frees `r5` for the station index, lets `sl` keep
`dividerOffset`, and lets `&Globals::status` live to the mission panel). It is
a greedy-allocator live-range split at the `hasShieldBar` merge
(`%2926 = COPY %2925`, see `hud-draw-slot-coloring-w4114.md` §6).

Measured gamma-expression variants (all keep the HEAD `st` + damage-block
`Globals::status == 0 ||` guard unless noted; official verifier,
`autojunk=False`):

| gamma expression (`a` = receiver, `b` = `getStation` receiver, `c` = mission arg) | size | `.ARM.extab` | fuzzy |
|---|---|---|---|
| HEAD: `st->f(st->g(), st->h())` | **9332** | 0x26290 | **89.1** |
| `st->f(st->g(), Globals::status->h())` | 9312 | 0x26290 | 72.9 |
| `st->f(Globals::status->g(), Globals::status->h())` | 9312 | 0x26290 | 72.9 |
| `Globals::status->f(st->g(), st->h())` | 9332 | 0x26290 | 89.1 |
| `Globals::status->f(Globals::status->g(), st->h())` | 9332 | 0x26290 | 89.1 |
| `st->f(Globals::status->g(), st->h())` | 9332 | 0x26290 | 89.1 |
| plain three-use textual (no `st`), guard textual | 9312 | 0x26290 | 72.9 |
| plain three-use textual (no `st`), guard removed | 9264 | 0x26290 | 82.0 |

**Rule:** the `this` spill survives iff the **mission argument** is `st->…`.
Any form that reloads `Globals::status` for the mission argument loses it.
The reload and the spill are mutually exclusive with every source spelling
tried so far (including a reference `Status *&`, a `float gamma` temporary,
`&& st != 0`, and moving `st` before the shield block).

The two dead `ldr.w r8,[sp,#108]` reloads at 0x164176 and 0x164210 in the
original are allocator reloads of `&Globals::status` for the mission panel
that are clobbered before use (`ldr.w r8,[r1]` at 0x1641e4 loads
`Globals::Canvas`). HEAD has neither; they are downstream of the same
allocation decision.

---

## 3. Ground truth: the docking-transfer block

Original call order, 0x165880..0x1659c0 (confirmed by Ghidra
`Hud_draw` lines 1170-1225):

```
frameH      = GetImage2DHeight(image_0x37c)     -> [sp,#96]
textH       = GetTextHeight(font)               -> [sp,#92] / r9
transferred = getDockTransferedAmount()         -> r4
total       = getDockTotalAmount()              -> r5
getText(3205/3204)                              -> r6
String(" ")                                     -> [sp,#196]
progress    = transferred / total               (vdiv s0, s0, s2)
barOffset   = (int)(textH + frameH*2.5)         -> s16
operator+   (*getText + String(" "))            -> [sp,#208]
~String([sp,#196])
barW        = GetImage2DWidth(image_0x37c)      -> r8
barH        = GetImage2DHeight(image_0x37c)     -> fp
field_0x468 += dt; fadeIn; fade
```

The corresponding source order is

```cpp
int frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int textH  = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font);
int transferred = ego->getDockTransferedAmount();
int total       = ego->getDockTotalAmount();
String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
float progress = (float) transferred / (float) total;
if (this->field_0x279)
    progress = 1.0f - progress;
barOffset = (int) ((float) textH + (float) frameH * 2.5f);
int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
this->field_0x468 += dt;
float fadeIn = (float) this->field_0x468 / 1000.0f;
float fade = 1.0f;
```

i.e. `String text` is declared **before** `progress` (order `T P B W H D`).
The compiler schedules the `operator+` call after `barOffset`, which is why
the disassembly shows `String(" ")` before `progress` but `operator+` after
`barOffset`. HEAD has `String text` last (`P B W H D T`).

All T-early orders measured at 9332 B:

| order | size | `.ARM.extab` | fuzzy |
|---|---|---|---|
| HEAD `P B W H D T` | 9332 | **0x26290** | 89.1 |
| `P B T W H D` | 9332 | 0x2629c | **90.7** |
| `P T B W H D` | 9332 | 0x2629c | 89.9 |
| `T P B W H D` (faithful) | 9332 | 0x2629c | 89.8 |
| `P B W T H D` | 9316 | 0x26294 | 88.6 |
| `P B W H T D` | 9340 | 0x26290 | 85.3 |

`P B T W H D` (90.7 %) is the best dock order, but it adds two call-site
entries (the `String(" ")` ctor and the `operator+` call) to Hud::draw's LSDA:
77 entries / 453 bytes -> 79 entries / 465 bytes (+12).

---

## 4. The `.ARM.extab` budget, itemised

`.ARM.extab` starts at `0x1a3b78`; `.rodata` is 16-byte aligned at
`0x1c9e10`. The original extab is `0x26298` bytes and ends exactly on
`0x1c9e10`. Our extab is `0x26290`, ending at `0x1c9e08`, so there are
**8 bytes of headroom**; the dock reorder's +12 overshoots by 4 and forces
`.rodata` to `0x1c9e20` (+0x10), which changes every `.text` literal-pool
word that names a `.rodata` address and drops `byte_exact` from 4500 to 4040.

Hud::draw's LSDA call-site table (parsed from the extab):

| build | entries | bytes |
|---|---|---|
| original | 77 | 454 |
| HEAD | 77 | 453 |
| `P B T W H D` | 79 | 465 |

The dock reorder's two extra entries are the real `String(" ")` ctor and
`operator+` cleanup ranges, which the original also has:

```
original:  (6684,890,0) (7574,10,9060) (7598,4,9058) (7614,4,9056) (7696,412,9280) ...
reorder :  (6684,890,0) (7574,10,9058) (7598,4,9056) (7612,4,9054) (7696,112,9184) (7840,272,9274) ...
```

The original merges two later ranges that our build splits (the
jump/cloak block's `SetColor`/`DrawImage2D` group vs its
`DrawRegion2D`/`GetTextWidth`/`DrawString` group, and a second split in the
same block). Those splits are downstream of the `this`-spill allocation, not
of the dock reorder.

**Where the 8-byte deficit lives.** Comparing every function's `.ARM.extab`
entry size (unwind opcodes + LSDA) against the original, only four functions
differ:

| function | ours | original | delta |
|---|---|---|---|
| `Generator::createMission(Agent*, Array<SolarSystem*>*)` @ 0x971b8 | 176 | 172 | **+4** |
| `Level::createShip(int,int,int,Waypoint*,bool,bool)` @ 0xc0214 | 328 | 336 | -8 |
| `LevelScript::process(int)` @ 0x137d18 | 352 | 348 | **+4** |
| `Level::createScene()` @ 0x1490d8 | 940 | 948 | -8 |

Sum = -8, exactly the observed extab deficit. The two `+4` functions are the
only shave candidates: `Generator::createMission` is 74.3 % matched
(LSDA +1, unwind opcodes +3) and `LevelScript::process` is 52.0 % matched
(LSDA +7, unwind opcodes -3). Both are non-byte-exact targets in their own
right; neither can be fixed as a side effect of Hud::draw.

---

## 5. Other variants measured (no strict improvement)

All at 9332 B unless noted; official verifier, `autojunk=False`.

| variant | `.ARM.extab` | fuzzy |
|---|---|---|
| HEAD | 0x26290 | 89.1 |
| damage guard `this->field_0x4b8 >= 1 \|\| Globals::status == 0` | 0x26290 | 86.7 (9340 B) |
| damage guard `Globals::status != 0 && this->field_0x4b8 >= 1` | 0x26290 | 88.5 (9336 B) |
| damage guard `st == 0 \|\| ...` | 0x26290 | 63.3 (9360 B) |
| `String text` before `progress` (`T P B W H D`) | 0x2629c | 89.8 |
| `String text` between `barW`/`barH` | 0x26294 | 88.6 (9316 B) |
| `String text` before `SetColor` in the jump/cloak block | 0x26284 | 86.4 (9340 B) |
| `String text` after `DrawImage2D` in the jump/cloak block | 0x2627c | 56.7 (9316 B) |
| `float halfW` instead of `int halfW` | 0x26290 | 88.6 (9324 B) |
| dock reorder + `String text` before `SetColor` | 0x26290 | 86.2 |
| dock reorder + `float halfW` | 0x2629c | 90.2 (9324 B) |
| dock reorder + `text = text + String(" ")` | 0x262a8 | 87.6 (9360 B) |

No variant both keeps 9332 B and improves the score while leaving
`.ARM.extab <= 0x26298`.

---

## 6. What remains

1. **Land the dock reorder `P B T W H D` (90.7 %)** once 4 bytes of
   `.ARM.extab` are recovered. The cheapest route is to shave the `+4` extab
   entry of `Generator::createMission` (0x971b8) or `LevelScript::process`
   (0x137d18) by 4 bytes without changing their `.text` size. Both are
   non-byte-exact (74.3 % / 52.0 %), so this is a separate pass each.
2. **The gamma store placement** is still irreducible: the faithful three-use
   textual form is known exactly, but it costs the shield-block `this` spill
   and drops the function to 9312 B / 72.9 %. A shape that keeps both has not
   been found.
3. The two dead `ldr.w r8,[sp,#108]` reloads (0x164176, 0x164210) and the
   mission-panel `fp`/`r9` swaps remain downstream of (2).
4. Any landing change must keep `.ARM.extab <= 0x26298`; a change that adds
   call-site-table entries must be paired with one that removes at least as
   many in the same commit.

---

## 7. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target check -j8      # RATCHET PASS

python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj

arm-linux-androideabi-readelf -S -W cmake-build-match/libgof2hdaa.so | grep -E 'ARM.extab|\.rodata'
```

The per-function extab table in §4 was produced by parsing `.ARM.exidx`
(0x19cc08, 0x6f70 bytes) to map each function to its `.ARM.extab` entry and
comparing the distance to the next entry. Scratch lived in `/dev/shm/w4515`
(`diff.py`, `order.py`, `cmp_lsda.py`, `exidx.py`).
