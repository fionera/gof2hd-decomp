# Radar::draw wave 156 (w10206) — scoped copy + store reorder + `bool enemyHit`:
# raw 8,490 -> 8,449, aligned 1,456 -> 1,392, extab 1,292 -> 1,254 at 9,908 B

**Status:** source change landed, 2026-11-08. Worktree `wt/w10206`, based on
`master` (`cf74f4ee6`, wave 155). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave155-w10104.md`, `radar-draw-wave155-w10114.md`,
`radar-draw-wave152-w9902.md`, `extab-lp-ties-wave154-w10019.md`.

Baseline (HEAD, wave 155): original 9,908 B; ours 9,908 B; raw same-address
diff **8,490**; instruction-index alignment **1,456**; `.text` differing bytes
**86,044**; `.ARM.extab` differing bytes **1,292**; LSDA record 132 B / 19 rows /
call-site length 111. All 27 section sizes and VMAs exact; file size 2,187,328 B.

## 1. The merged shape

The wave-155 source was the w10104 `noscope` else arm (temp live through
`DrawString`) plus a tail `getCurrentCampaignMission()` companion.  The w10114
worktree had the asteroid-lock store reorder + `bool alwaysEnemySeen` on the
w9902 **scoped** else arm.  The two wins are combinable, but not by simply
stacking both: the noscope else arm is what costs the `.ARM.extab` bytes.

**Key measurement (new):** the tail getter is NOT what changed `.ARM.extab`.
Removing it from the wave-155 source leaves the extab diff at 1,292; the
`noscope` else arm itself is the +37 (its `calcDistance` invoke joins the
temp's cleanup range, so LSDA row 3 becomes `(2362,42,9850)` instead of the
original `(2394,10,9856)`).  The w9902 **scoped** else arm keeps the original's
EH shape: its `calcDistance` call is outside the temp cleanup range and the
record stays 132 B / 19 rows / call-site length **112**.

Landed source (all four edits):

```cpp
// 1. scoped String copy (w9902 shape) — restores the original EH structure
KIPlayer *landmark = this->landmarks->data()[i];
{
    String labelCopy2 = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
                                           this->cameraX, this->cameraY, this->cameraZ);
    this->lockLabel = labelCopy2;
}
Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
                            this->screenY + rowHeight, false);

// 2. asteroid-lock store order (w10114): prevents clang hoisting/sinking the
//    two asteroidLockTimer stores; emits the original's second str.w [r4,#412]
if ((candidateWreck ? this->field_0x1c : this->lockedAsteroid) == nullptr) {
    this->field_0x10 = 0;
    this->asteroidLockTimer = 0;
} else {
    this->asteroidLockTimer = 0;
}

// 3. bool enemyHit (was int) — big aligned win, -8 B
// 4. bool alwaysEnemySeen (was int) — +? companion
// 5. int marked (was bool) — +4 B companion that lands 9,908 exactly
```

The tail getter stays as the final +8 B companion (it is worth -106 raw here):
`this->field_0x1bc = 0; Globals::status->getCurrentCampaignMission();`

## 2. Variant matrix (all measured on the wave-155 HEAD base, full link)

| shape | usize | raw | aligned | extab | LSDA cst |
|---|---:|---:|---:|---:|---:|
| HEAD wave 155 (noscope + tail getter) | 9908 | 8490 | 1456 | 1292 | 111 |
| w10114 (scoped + reorder + bool alwaysEnemy) | 9908 | 8482 | 1477 | 1255 | 112 |
| noscope + reorder + bool alwaysEnemy + tail | 9912 | 8286 | 1446 | 1291 | 111 |
| noscope + reorder + bool alwaysEnemy (no tail) | 9904 | 8293 | 1439 | 1292 | 111 |
| scoped + reorder + bool enemyHit + tail | 9904 | 8491 | 1341 | 1254 | 112 |
| scoped + reorder + bool enemyHit + bool alwaysEnemy + tail | 9904 | 8320 | 1341 | 1255 | 112 |
| **scoped + reorder + bool enemyHit + bool alwaysEnemy + int marked + tail (landed)** | **9908** | **8449** | **1392** | **1254** | **112** |
| scoped + reorder + bool enemyHit + int marked + tail (no bool alwaysEnemy) | 9908 | 8561 | 1385 | 1253 | 112 |
| scoped + reorder + bool enemyHit + bool alwaysEnemy + int marked (no tail) | 9900 | 8555 | 1385 | 1255 | 112 |

The landed shape is the only measured 9,908 B hit that improves **both** text
metrics over wave 155 while also dropping `.ARM.extab` below the wave-154
1,255 mark.  `bool enemyHit` is the dominant aligned lever (-136 instructions
vs the w10114 shape); `int marked` is the +4 B companion that restores the exact
size without touching the EH structure.

## 3. LSDA rows (reported as requested)

Original record @ `0x1c203c`, **132 B / 19 rows / cst 112** (11 LP != 0):

```
(0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
(2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
(2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
(5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)
```

Landed record @ `0x1c203c`, **132 B / 19 rows / cst 112** (11 LP != 0):

```
(0,568,0) (568,8,9810) (576,1780,0) (2356,12,9854) (2368,136,0)
(2504,10,9850) (2518,16,9856) (2534,14,9840) (2548,12,9830) (2572,20,9820)
(2592,78,0) (2670,10,9812) (2680,1294,0) (3974,12,9874) (3986,1772,0)
(5758,8,9868) (5766,856,0) (6622,8,9808) (6630,3278,0)
```

Record size and call-site table length match the original exactly; only row
offsets differ.  Radar::draw's record contributes **49** differing bytes
(wave 155: 87).  Whole-section `.ARM.extab` differing bytes: **1,254**
(wave 155: 1,292; wave 154: 1,255).

## 4. Section sizes

All 27 section sizes and VMAs identical to the original: `.text` size 0x1337c8
@ 0x69440, `.ARM.exidx` @ 0x19cc08 / 0x6f70, `.ARM.extab` @ 0x1a3b78 / 0x26298,
`.rodata` @ 0x1c9e10 / 0x41b67; file size 2,187,328 B.  `.ARM.exidx`, `.rodata`,
`.data`, `.bss`, `.rel.plt` are byte-identical.  No linker pad hack.

## 5. Gate (after)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint:   CLEAN
RATCHET PASS
```

Radar::draw: size 9,908 B exact; raw **8,490 -> 8,449** (-41); aligned
**1,456 -> 1,392** (-64); `.text` differing bytes **86,044 -> 86,003** (-41);
`.ARM.extab` **1,292 -> 1,254** (-38).

## 6. Open questions

1. `bool enemyHit` is the strongest lever found so far (aligned 1,341 at
   9,900/9,904 B).  The landed shape keeps it and pays +7 aligned for the
   +8 B tail getter, which is worth -106 raw.
2. The `int marked` companion is a type toggle, not a witnessed original
   declaration; it is the only +4 B companion that kept the EH record at
   132 B / cst 112.
3. Remaining aligned mismatches are still dominated by the whole-function
   register permutation (`campaign` in `sl`, `i` in `fp`) and the navWreck
   block placement (w10114 cluster #1).
