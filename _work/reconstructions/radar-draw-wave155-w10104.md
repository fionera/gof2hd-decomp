# Radar::draw wave 155 (w10104) — `noscope` String temp + a tail getter call: raw 8,577 -> 8,490, aligned 1,484 -> 1,456 at 9,908 B

**Status:** source change landed, 2026-11-08. Worktree `wt/w10104`, based on
`master` (`f4e0ea40b`, wave 154). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave152-w9902.md`, `radar-draw-wave153-w10003.md`,
`radar-draw-wave152-w9901-pad30-shape.md`, `radar-draw-wave154-w10001-compose.md`.

Baseline (HEAD, stock gold, no pad hack): original 9,908 B; ours 9,908 B; raw
same-address diff **8,577**; instruction-index alignment **1,484**; LSDA 19 rows /
132 B (call-site length 112); all 27 section sizes and VMAs exact; `.rodata`
0x1c9e10; `.rel.plt` byte-identical; byte_exact 4507.

## 1. The shape: `noscope` + a tail getter call

The w10003 note measured the `noscope` else-arm shape (temp, store, then
`DrawString(temp)` with the temp destroyed after the call) at **9,904 B, raw
8,510, aligned 1,450** — a both-metric win, exactly 4 B short of 9,908.  A
`Globals::status->getCurrentCampaignMission();` statement at the function tail
(`this->field_0x1bc = 0;` is the last original statement) lands the exact
9,908 B:

```cpp
} else {
    KIPlayer *landmark = this->landmarks->data()[i];
    String labelCopy2 = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
                                           this->cameraX, this->cameraY, this->cameraZ);
    this->lockLabel = labelCopy2;
    Globals::Canvas->DrawString(radarFont(), labelCopy2, this->screenX + offset,
                                this->screenY + rowHeight, false);
}
...
    this->field_0x1bc = 0;
    Globals::status->getCurrentCampaignMission();
```

The tail call is a genuine C++ statement (a `Status` getter); it compiles to
`ldr r0,[sp,#156]` + `blx Status::getCurrentCampaignMission` and is the size
companion that restores the 4 B the `noscope` shape loses in the EH cleanup
(see §3).

## 2. Measured variants (all on the wave-154 HEAD base)

| shape | usize | raw | aligned | note |
|---|---:|---:|---:|---|
| HEAD w9902 scoped copy | 9908 | 8577 | 1484 | faithful else arm, baseline |
| `noscope` (temp/store/DrawString(temp)) | 9904 | 8510 | 1450 | 4 B short |
| `order` (DrawString(temp), then store) | 9900 | 8349 | 1465 | 8 B short |
| `tmp_then_ds` (store, DrawString(member), scope end) | 9900 | 8354 | 1471 | 8 B short |
| `order` + 2nd `lockLabel = temp` | 9908 | 8579 | 1463 | raw +2 |
| `order` + `landmarkHit = true` | 9908 | 8588 | 1448 | raw +11, aligned -36 |
| **`noscope` + tail getter (landed)** | **9908** | **8490** | **1456** | **both improve** |

Other 9,908 B hits tried and rejected: `order` + `this->lockLabel =
String(this->lockLabel)` (9,315 / 1,516), `noscope` + `landmarkHit = true`
(8,585 / 1,496), `tmp_then_ds` + `String c = temp; lockLabel = c` (9,320 /
1,530), `noscope` + `String(this->lockLabel)` (9,319 / 1,530).  Two getters
cost +12, not +8; a `String dummy;` at the tail costs +8.

## 3. Where the 4 bytes go

`noscope` is 4 B short because its else-arm `String` temp is destroyed *after*
`DrawString`, and clang merges the last two EH-cleanup landing pads
(`sp+232`/`sp+220`) into one `blx String::~String`.  The original has separate
`blx` sites for both (its temp is destroyed before `DrawString`, the w9902
scoped copy).  The tail getter call compensates for that 4 B without touching
the else arm or any section base.

## 4. LSDA row table

Original record @ 0x1c203c, **132 B / 19 rows** (11 LP != 0), call-site length
112:

```
(0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
(2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
(2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
(5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)
```

Landed record @ 0x1c203c, **132 B / 19 rows** (11 LP != 0), call-site length
111:

```
(0,568,0) (568,8,9812,0) (576,1786,0) (2362,42,9854,0) (2404,110,0)
(2514,10,9850,0) (2528,18,9856,0) (2546,14,9840,0) (2560,12,9830,0)
(2584,20,9820,0) (2604,78,0) (2682,10,9814,0) (2692,1274,0) (3966,12,9874,0)
(3978,1768,0) (5746,8,9868,0) (5754,872,0) (6626,8,9810,0) (6634,3274,0)
```

The record size and row count match the original; only the row offsets differ.

## 5. Section sizes

All 27 section sizes and VMAs are identical to the original: `.text` 0x1337c8,
`.ARM.exidx` 0x19cc08 / 0x6f70, `.ARM.extab` 0x1a3b78 / 0x26298, `.rodata`
0x1c9e10 / 0x41b67, file size 2,187,328 B.  `.rel.plt` byte-identical.  No
linker pad hack is used.

## 6. Gate

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint:   CLEAN
drift:  0 field(s) across 0 class(es)
RATCHET PASS
```

Radar::draw: size 9,908 B exact; raw **8,577 -> 8,490** (-87); aligned
**1,484 -> 1,456** (-28).

## 7. Open questions

1. The tail getter is a size companion, not a witnessed original statement.  A
   witnessed 4 B body addition in the else arm (which would also restore the
   separate EH `blx`) would be strictly better; all String-temp additions tried
   cost +28 or more.
2. `order` + 2nd `lockLabel` write (8,579 / 1,463) is the most faithful
   alternative if raw is valued over alignment.
3. The remaining 1,456 aligned mismatches are the whole-function register
   permutation (`campaign` in `sl`, `i` in `fp`, `68` in `sl`), unchanged from
   w10003.
