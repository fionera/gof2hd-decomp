# Radar::draw wave 160 (w10609) — LSDA/`.ARM.extab` enumeration; cleanup-scope
# pad chain is text-layout-bound; size-exact extab 50 -> 49 landed

**Status:** source change landed, 2026-11-08. Worktree `wt/w10609`, based on
`master` (`08af4633b`, wave 159). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave159-w10505.md`, `radar-draw-wave159-w10504.md`.

## 0. Baseline (HEAD, wave 159)

Original 9,908 B; ours 9,908 B; raw same-address diff **8,124**;
instruction-index alignment **1,320**; `.text` 85,722 differing bytes;
`.ARM.extab` 1,256 differing bytes; Radar LSDA record diff **50**;
all 27 section sizes exact; file size 2,187,328 B; gate byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN.

## 1. The record is a cleanup-only LSDA (no `try`/`catch`)

`readelf`/objdump decode of the original's `.ARM.exidx` entry for `0x12e050`
(exidx at `0x1a10b8`, second word `0x20f80` -> `.ARM.extab` record at
`0x1c203c`):

```
pers 0x5eb60 (__gxx_personality_v0)  size 2 words
lps_enc 0xff (omit)  tt_enc 0xff (omit, NO type table)  cs_enc 0x01 (uleb)  n 112
record 132 B, 19 rows
```

`tt_enc == 0xff` proves there is **no catch clause** anywhere in `Radar::draw`.
The record is the ARM EHABI *call-site table* for destructor cleanups. There is
no missing `try`/`catch` to add: every row with a non-zero landing pad is a call
that can throw while a live `String` temporary needs destroying.

## 2. Every throwing call site and its cleanup scope

Original rows (start, length, landing-pad offset, action):

```
 (0,   568, 0)     (568,   8, 9818)  route: lockLabel = calcDistance(...)   {e8}
 (576, 1818,0)     (2394, 10, 9856)  landmark else: lockLabel = calcDistance {e8}
 (2404,158, 0)     (2562, 10, 9852)  getText(133) + String(": ")             {d0}
 (2576, 16, 9858)  getStation/getTecLevel/String(int)                        {dc,d0}
 (2592, 12, 9842)  String(String(...), false)                                {b8,dc,d0}
 (2604, 16, 9832)  operator+ (second)                                        {c4,b8,dc,d0}
 (2638, 20, 9822)  DrawString(label)                                         {e8,c4,b8,dc,d0}
 (2658, 78, 0)     (2736, 14, 9820)  landmark i==0: lockLabel = calcDistance {e8}
 (2750,1256,0)     (4006,  8, 9872)  planet: lockLabel = calcDistance        {e8}
 (4014,1796,0)     (5810,  8, 9870)  enemy: lockLabel = calcDistance         {e8}
 (5818, 788, 0)    (6606,  8, 9816)  gas: lockLabel = calcDistance           {e8}
 (6614,3294, 0)
```

`{...}` = the stack `String` temporaries destroyed by that landing pad
(`e8`=sp+232, `dc`=sp+220, `d0`=sp+208, `c4`=sp+196, `b8`=sp+184). The pad
chain is the canonical reverse-creation-order cascade: e.g. the `i==0` arm's
`DrawString` pad destroys `e8,c4,b8,dc,d0`.

## 3. The landing-pad chain: ours is semantically identical, offset by +2

Decoding every LP address and following the pad branches shows the same
destructor sets in both, with one exception: original row 9 (planet
`operator=`) destroys `{e8}`; ours destroys `{dc}` because our register
allocator puts the planet-loop `calcDistance` sret temp at `sp+220` instead of
`sp+232`. Everything else is identical.

The LP *offsets* are almost all `orig = ours + 2`:

| row | orig LP | ours LP | delta |
|---|---:|---:|---:|
| 1 (route) | 9818 | 9816 | +2 |
| 2 (landmark else) | 9856 | 9854 | +2 |
| 3 | 9852 | 9850 | +2 |
| 4 | 9858 | 9856 | +2 |
| 5 | 9842 | 9840 | +2 |
| 6 | 9832 | 9830 | +2 |
| 7 | 9822 | 9820 | +2 |
| 8 (landmark i==0) | 9820 | 9818 | +2 |
| 9 (planet) | 9872 | 9874 | -2 |
| 10 (enemy) | 9870 | 9868 | +2 |
| 11 (gas) | 9816 | 9808 | +8 |

Cause: ours emits the gas-cloud `operator=` cleanup pad **before** the literal
pool (`0x1306a0`: `b.n` + `nop`, then the pool at `0x1306a4`), while the
original emits all pads after the pool (`0x1306a8`). If ours' pads were shifted
+2, rows 1-8 and 10 would match exactly.

## 4. New measured lever: a cleanup scope in the asteroid block

Adding a default-constructed `String` at the end of the asteroid
`isDockingToAsteroid` block (`String dummy;`) and dropping the non-original
tail `plasmaInRange = 0` store gives a pad chain that matches the original for
**rows 3-11 and 15** (LP values 9856/9852/9858/9842/9832/9822/9820/9870), with
the record diff dropping to **45** — but the function grows to **9,912 B**
(+4), which is gate-fatal (size-exact must stay 4524/4524). Measured:

| shape | size | raw (obj) | extab (obj) | record |
|---|---:|---:|---:|---:|
| baseline | 9908 | 8393 | 54 | 132 |
| asteroid dummy + no-plasma + 2 getters | **9912** | 8523 | **45** | 132 |
| asteroid dummy + no-plasma + 1 getter | 9904 | 8572 | 55 | 132 |
| asteroid dummy + no-plasma + 3 getters | 9920 | 8596 | 53 | 132 |
| asteroid dummy + plasma + 1 getter | 9908 | 8571 | 54 | 132 |
| asteroid dummy + 1 getStation()->getIndex() tail | 9908 | 8571 | 54 | 132 |

The extab-45 layout is an absolute-address sweet spot: every 4-byte change to
the tail or body tested (getter count 0-4, plasma on/off, arbitrary 4-byte
stores, statement removals) moves the record back to 54.  Sweeps of 117
size-exact tail/filler combinations and 38 single-statement removals found no
size-9908 shape with extab < 53.

## 5. Landed change (size-exact, extab 50 -> 49)

```diff
@@ landmark else-arm (i<=1, else branch)
                         Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
                                                     this->screenY + rowHeight, false);
+                        String dummy;
@@ tail
     this->field_0x1bc = 0;
-    this->plasmaInRange = 0;
-    Globals::status->getCurrentCampaignMission();
     Globals::status->getCurrentCampaignMission();
```

The `String dummy;` adds a cleanup scope in the landmark else-arm; the tail
loses the non-original `plasmaInRange = 0` store and one of the two dead
getters.  Net size is unchanged (9,908 B).

| metric | before | after |
|---|---:|---:|
| size | 9,908 | **9,908** |
| raw same-address diff | 8,124 | **8,495** |
| instruction-index alignment | 1,320 | **1,321** |
| Radar LSDA record diff | 50 | **49** |
| `.ARM.extab` differing bytes | 1,256 | **1,255** |
| `.text` differing bytes | 85,722 | **86,093** |
| byte_exact / linked_exact | 4507/4507 | **4507/4507** |

The extab win is 1 byte; the raw metric regresses by 371 (the dummy shifts the
landmark block).  The aligned metric is essentially unchanged (+1).  This is
the only size-exact extab improvement found; the coordinator may prefer to
revert it if the `.text` delta matters more than the `.ARM.extab` delta.

## 6. Open questions

1. The extab-45 pad chain (rows 3-11,15 exact) needs a 4-byte body reduction
   that does *not* move the literal pool.  No such reduction was found among
   tail stores, getter counts, or single-statement removals.
2. Row 9's semantic mismatch (`{dc}` vs `{e8}`) is the planet-loop sret temp
   stack slot (`sp+220` vs `sp+232`), a pure register-allocation artefact.
3. The `+2` global LP shift is the gas-cloud pad emitted before the literal
   pool.  If a source shape makes clang emit all pads after the pool, rows
   1-8/10 would match without touching the body.
4. `.ARM.extab` is 1,256 bytes off overall; Radar is only **50** of them.
   Per-record attribution (raw extab diff): `HangarWindow::render` (`0x1490d8`)
   **676**, `LevelScript::process` (`0x137d18`) 182, `0xaffd8` 159,
   `0xc0214` 77, Radar **50**.  `HangarWindow::render`'s 676 bytes are caused
   by a 2-byte text difference around record offset 5918 that flips the uleb
   row length and misaligns every later row — a much larger extab target.
