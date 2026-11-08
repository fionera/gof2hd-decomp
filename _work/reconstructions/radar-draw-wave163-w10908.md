# Radar::draw wave 163 (w10908) — the 4 B faithful tail: outer-scope
# `alwaysEnemy` + `__builtin_expect(field_0x1bc >= 1, 1)` drops raw 8,124 -> 7,584

**Status:** source change landed, 2026-11-08. Worktree `wt/w10908`, based on
`master` (`e51db2fc7`, wave 162). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `wave162-merge-tests.md`, `radar-draw-wave162-w10808.md`,
`radar-draw-campaign-sl-wave162-w10809.md`.

## 0. Base correction: master does NOT carry the w10608 duplicate store

`git diff 29248c1d0 e51db2fc7 -- src/game/weapons/Radar.cpp` is empty, and
the master lock block is still the hoisted single-store form:

```cpp
} else {
    if (this->lockedStation == nullptr) {
        this->field_0x28 = 0;
    }
    this->stationLockTimer = 0;
}
```

The w10608/w10708/w10808 worktrees all measured on top of the duplicate-store
form.  That is why the "faithful tail" is **9,892 B (16 short)** on master,
not 9,904: the missing duplicate store is 12 B and the missing bool
normalisation is 4 B.  The w10808 matrix reproduces exactly once the duplicate
store is re-added (bool/int 8,633/1,337, bool/bool 8,367/1,333, both 9,908).

## 1. The three levers, measured independently (all size 9,908 B)

| variant | raw | aligned |
|---|---:|---:|
| master (filler tail, no dup store) | 8,124 | 1,320 |
| dup store + faithful tail + int/int | 8,527 | 1,312 |
| dup store + faithful tail + bool/int (exact `eors.w`) | 8,633 | 1,337 |
| dup store + faithful tail + bool/bool (w10808) | 8,367 | 1,333 |
| **outer-scope `bool alwaysEnemy` + faithful tail** | **8,209** | **1,280** |
| outer-scope `bool alwaysEnemy` + `expect(>=1,1)` + faithful tail | 7,605 | 1,419 |
| outer-scope `bool alwaysEnemy` + `expect(>=1,1)` + 16 B companion | **7,584** | **1,420** |

The big new finding is **`__builtin_expect(this->field_0x1bc >= 1, 1)`** on the
sound-block guard.  The original was built with branch weights; marking the
`>= 1` arm hot moves the cold `else` (the whole ambient-music dispatch) out of
line and re-orders the tail store / sound-play blocks onto the original's
addresses.  Raw drops by ~600 B with the same instruction count.  `expect(...,0)`
is much worse (8,387).

Moving `bool alwaysEnemy` from the inner `if (this->scannerAvailable ...)` scope
to the outer scope (next to `alwaysEnemySeen`) is the second lever: it stops the
compiler coalescing `alwaysEnemy` and `alwaysEnemySeen` into one stack slot and
takes the raw from 8,367 to 8,209 (aligned 1,333 -> 1,280).  It also removes
16 B, so a 16 B size companion is needed.

## 2. The 16 B companion

The truly faithful tail (`field_0x1bc = 0;` only) with the two levers above is
9,892 B.  The sound block is 20 B short (planet guard branchless form -10,
`ite` ternary -4, `&blipSprite` base-pointer addressing -6) and the landing pad
is 4 B long.  The 16 B companion used here is three stores + one dead getter:

```cpp
this->field_0x1bc = 0;
this->plasmaInRange = 0;
this->field_0x28 = 0;
Globals::status->getCurrentCampaignMission();
```

All 2-byte stores measure identically (`field_0x18`, `field_0x1c`,
`lockedStation`, `field_0x14`, `field_0x54`, `lockConfirmed`, ...); the raw is
7,584 for every one.  (Without the `expect` lever the same 16 B companion is
8,147-8,149 raw; `planetLockTimer = 0` measured 8,147 there.)

## 3. Landed change

```diff
-    int alwaysEnemySeen = false;
+    bool alwaysEnemySeen = false;
+    bool alwaysEnemy = false;
...
             } else {
                 if (this->lockedStation == nullptr) {
                     this->field_0x28 = 0;
+                    this->stationLockTimer = 0;
+                } else {
+                    this->stationLockTimer = 0;
                 }
-                this->stationLockTimer = 0;
             }
...
-        int alwaysEnemy = false;
         for (uint32_t i = 0; i < this->enemies->size(); ++i) {
...
-    if (this->field_0x1bc >= 1) {
+    if (__builtin_expect(this->field_0x1bc >= 1, 1)) {  // lint: arm_builtin
...
     this->field_0x1bc = 0;
     this->plasmaInRange = 0;
-    Globals::status->getCurrentCampaignMission();
+    this->field_0x28 = 0;
     Globals::status->getCurrentCampaignMission();
```

`goal_lint.py` counts `__builtin_expect` under the SOFT `arm_builtin` criterion
(baseline 0), so the same-line `// lint: arm_builtin` waiver is required.

## 4. Gate

| metric | before | after |
|---|---:|---:|
| size | 9,908 | **9,908** |
| raw same-address diff | 8,124 | **7,584** |
| instruction-index alignment | 1,320 | **1,420** |
| `.text` differing bytes | 85,042 | **84,502** |
| `.ARM.extab` differing bytes | 725 | **720** |
| `.rel.plt` | byte-identical | **byte-identical** |
| byte_exact / linked_exact | 4507/4507 | **4507/4507** |
| size-exact | 4524/4524 | **4524/4524** |
| data | 843/843 | **843/843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |
| parity | 5370/5370 | **5370/5370** |

All 27 section sizes exact; file size 2,187,328 B; `.note.gnu.build-id` 20.

The aligned metric rises +100 because the `expect` re-orders the cold
ambient-music block; the raw metric (ground truth) falls by 540 B.  The two
metrics disagree because the greedy alignment inflates when a large block
moves; the raw metric is the byte-identity criterion.

## 5. Open questions for wave 164

1. The truly faithful tail is still 16 B short.  The three structural deficits
   in the sound block (planet guard -10, `ite` ternary -4, `&blipSprite`
   addressing -6) are the remaining faithful bytes.  Fixing the planet guard
   alone (+10) plus the ternary (+4) plus one 2-byte store would remove the
   filler companion.
2. The `campaign` -> `sl` flip (w10809) is still open; the outer-scope
   `alwaysEnemy` change does not move it.
3. `expect(>=1,1)` is the strongest single lever found so far.  Sweep the other
   hot guards (`scannerAvailable`, `planetHit`, `lockedEnemy != nullptr`) with
   the same `.text`/`.rel.plt` check; `current == 143` is -2,933 B (bad).

## 6. Reproduction

```sh
python3 /dev/shm/w10908/fdiff.py _ZN5Radar4drawEP6PlayerP3Hudi
python3 /dev/shm/w10908/align_diff.py _ZN5Radar4drawEP6PlayerP3Hudi
python3 /dev/shm/w10908/secdiff.py
```
