# Radar::draw wave 156 (w10204) — `bool enemyHit` + tractorAuto store swap + tail String temp: raw 8,490 -> 8,320, aligned 1,456 -> 1,296 at 9,908 B

**Status:** source change landed, 2026-11-08. Worktree `wt/w10204`, based on
`master` (`cf74f4ee6`, wave 155). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave155-w10104.md`, `radar-draw-wave155-w10114.md`,
`radar-draw-wave155-w10105.md`, `radar-draw-wave154-w10002.md`.

Baseline (HEAD wave 155): original 9,908 B; ours 9,908 B; raw same-address diff
**8,490**; instruction-index alignment **1,456**; `.text` 86,044 differing bytes;
`.ARM.extab` 1,292; all 27 section sizes/VMAs exact; file size 2,187,328 B.

## 1. Base reproduction (w10104 + w10114, no tail getter)

The coordinator's "w10104+w10114 base (no tail getter)" is reproduced exactly by
removing the w10104 tail getter and adding the w10114 store reorder
(`field_0x10 = 0; asteroidLockTimer = 0;`) plus `bool alwaysEnemySeen`:

| shape | size | raw | aligned |
|---|---:|---:|---:|
| base (int `enemyHit`, reorder, bool `alwaysEnemySeen`, no tail getter) | 9904 | 8293 | 1439 |
| **+ `bool enemyHit`** | **9892** | **8368** | **1297** |

`bool enemyHit` is the aligned lever (matches the mission's 9,892/8,368/1,297).

## 2. The +16 B companion

Two changes, both real code, land exactly 9,908 B:

1. **tractorAuto branch store swap** (+4 B, raw -51, aligned -5). The original
   emits `dockNavPtr = enemy` *before* `field_0x1c = enemy` and keeps the
   `dockNavPtr` store separate from the shared `enemyHit = true` tail (7 stores
   to `[r4,#8]`). Swapping the source order in the `field_0x1c == nullptr &&
   tractorAuto` arm makes clang un-merge it (ours also reaches 7 stores):

   ```diff
   -                    this->dockNavPtr = enemy;
                        this->field_0x1c = enemy;
   +                    this->dockNavPtr = enemy;
   ```

2. **tail `String dummy;`** (+12 B). A genuine default-constructed String at the
   function tail (after `field_0x1bc = 0;`), replacing the w10104 getter
   companion. It compiles to a default ctor + `blx String::~String` and keeps
   `.ARM.extab` at 132 B / 19 rows (cst_len 111 vs original 112, unchanged from
   the w10104 landed shape).

Landed diff:
```diff
-    int alwaysEnemySeen = false;
+    bool alwaysEnemySeen = false;
...
-    int enemyHit = false;
+    bool enemyHit = false;
...
                 if (this->field_0x1c == nullptr && this->tractorAuto) {
-                    this->dockNavPtr = enemy;
                     this->field_0x1c = enemy;
+                    this->dockNavPtr = enemy;
                     enemyHit = true;
...
                     if ((candidateWreck ? this->field_0x1c : this->lockedAsteroid) == nullptr) {
-                        this->asteroidLockTimer = 0;
                         this->field_0x10 = 0;
+                        this->asteroidLockTimer = 0;
                     } else {
...
     this->field_0x1bc = 0;
-    Globals::status->getCurrentCampaignMission();
+    String dummy;
```

## 3. Measured matrix (all on the wave-155 HEAD base)

| variant | size | raw | aligned | note |
|---|---:|---:|---:|---|
| HEAD wave 155 (noscope + tail getter) | 9908 | 8490 | 1456 | baseline |
| base, int `enemyHit` (no tail getter) | 9904 | 8293 | 1439 | mission base |
| **`bool enemyHit`** | **9892** | **8368** | **1297** | best aligned, 16 B short |
| `bool enemyHit` + `int asteroidHit` | 9908 | 8525 | 1544 | +16 but aligned > 1456 |
| `bool enemyHit` + `int alwaysEnemy` + `int asteroidHit` | 9908 | 8514 | 1546 | aligned > 1456 |
| `bool enemyHit` + `int missionBlocksPlanets` + `int asteroidHit` | 9908 | 8510 | 1590 | aligned > 1456 |
| `bool enemyHit` + `int planetHit` + `int bossSeen` | 9908 | 8634 | 1824 | aligned > 1456 |
| `bool enemyHit` + 2 tail getters | 9908 | 8429 | 1304 | getter pair |
| `bool enemyHit` + tail getter + tail String | 9908 | 8406 | 1304 | |
| `bool enemyHit` + tractorAuto swap + `dup_if` + tail getter | 9908 | 8396 | 1298 | 2 store un-merges |
| `bool enemyHit` + `int marked` + tail String | 9908 | 8375 | 1353 | |
| **`bool enemyHit` + tractorAuto swap + tail String (landed)** | **9908** | **8320** | **1296** | **best both-metric, gate-green** |
| `bool enemyHit` + tractorAuto swap + tail String + getter + `bool astWreck` + `int alwaysEnemy` | 9908 | 8166 | 1308 | lower raw, but `.ARM.extab` +12 B (rodata shift) → byte_exact 4507→4044, rejected |

The last row is the lowest raw measured but it adds a second EH landing pad,
growing `.ARM.extab` 0x26298→0x262a4 and shifting `.rodata` by 16 B; the ratchet
drops `byte_exact` to 4044, so it is not landable without a data-layout fix.

## 4. LSDA rows

Original and ours both: `pers 0x5eb60 uw 3w lp 0xff tt 0xff cst 0x1 len 112/111
rows 19`, record 132 B. Row offsets differ (body still 1,296 instructions off):

```
orig: (0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
      (2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
      (2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
      (5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)
ours: (0,568,0) (568,8,9810) (576,1786,0) (2362,42,9854) (2404,110,0)
      (2514,10,9850) (2528,18,9856) (2546,14,9840) (2560,12,9830) (2584,20,9820)
      (2604,78,0) (2682,10,9812) (2692,1274,0) (3966,12,9874) (3978,1776,0)
      (5754,8,9868) (5762,856,0) (6618,8,9808) (6626,3282,0)
```

## 5. Gate (after)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
drift:  0 field(s) across 0 class(es)
```

All 27 section sizes and VMAs exact; file size 2,187,328 B; `.rodata`/`.data`/
`.bss`/`.ARM.exidx`/`.rel.plt` byte-identical.

* raw same-address **8,490 -> 8,320** (-170)
* instruction-index alignment **1,456 -> 1,296** (-160)
* `.text` differing bytes **86,044 -> 85,874** (-170)
* `.ARM.extab` differing bytes 1,292 (unchanged)

## 6. Open questions

1. `enemyLockTimer` is still 5 original stores vs 4 ours (the else-branch merge).
   `dup_if` un-merges it (+4 B) but raw worsens; a source shape that un-merges it
   without the tail String would be strictly better.
2. `bool enemyHit` + `int asteroidHit` reaches 9,908 with no companion, but its
   aligned 1,544 exceeds the 1,456 budget; a layout fix that pulls it under would
   remove the tail String entirely.
3. The tractorAuto store swap proves the original's source order is
   `field_0x1c = enemy; dockNavPtr = enemy;` (clang then emits dockNavPtr first).
