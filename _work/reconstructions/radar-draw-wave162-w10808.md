# Radar::draw wave 162 (w10808) — the 4-byte faithful-tail deficit is the
# `bool alwaysEnemySeen` normalisation: sound-block `eors.w` (+2) and the
# assignment `and.w #1` (+4) minus the literal-pool nop (-2)

**Status:** source change landed, 2026-11-08. Worktree `wt/w10808`, based on
`master` (`29248c1d0`, wave 161). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `wave161-merge-tests.md`, `radar-draw-wave161-w10708.md`,
`radar-draw-wave160-w10608.md`.

## Baseline (wave-161 HEAD)

Original 9,908 B; ours 9,908 B; raw same-address diff **8,124**;
instruction-index alignment **1,320**; `.text` differing bytes 85,374;
`.ARM.extab` 725; `.rel.plt` byte-identical; all 27 section sizes exact;
file size 2,187,328 B; gate byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.

The wave-161 HEAD still carries the wave-159 filler tail
(`field_0x1bc = 0; plasmaInRange = 0; getCurrentCampaignMission(); ...`) and
the single shared `stationLockTimer = 0` store.

## 1. The faithful tail is 4 B short — and the 4 B are exactly the bool

With the duplicate `stationLockTimer` store and the faithful tail (only
`this->field_0x1bc = 0;`) the function is **9,904 B** (4 short), raw 8,527,
aligned 1,312.  The missing 4 bytes are the two effects of
`alwaysEnemySeen` being a `bool` in the original:

| site | original | ours (`int`) | delta |
|---|---|---|---:|
| `if (field_0x1bc >= 1)` sound guard | `cmp r1,#151; beq; ldr r2,[sp,#104]; eors.w r2,r2,#1; bne` (12 B) | `ldr r2,[sp,#100]; cmp r2,#0; beq; cmp r1,#151; beq` (10 B) | **+2** |
| `alwaysEnemySeen = alwaysEnemy` | `ldr r1,[sp,#80]; and.w r1,r1,#1; str r1,[sp,#104]` (8 B) | coalesced into the same slot, no store | **+4** |
| literal-pool alignment nop before the cold sound block | none | `nop` (2 B) | **-2** |
| | | | **net +4** |

So the "2+2" is the sound-block `eors.w` (+2) and the assignment
`and.w #1` (+4) offset by the alignment nop (-2).  The original's
`and.w` proves `alwaysEnemy` is **not** a `bool` (a bool-to-bool copy needs
no normalisation); the sound block's `eors.w` proves `alwaysEnemySeen` **is**
a bool.  This is exactly what the Ghidra decompilation shows at the sound
guard (`iVar7 != 0x97 && bVar41` with `bVar41` a bool).

## 2. Measured matrix (all size 9,908 B unless noted)

| variant | raw | aligned | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|
| wave-161 HEAD (filler tail) | 8,124 | 1,320 | 85,374 | 725 |
| faithful tail, `int`/`int` | 8,527 | 1,312 | — (9,904 B) | — |
| **landed: `bool alwaysEnemySeen` + `bool alwaysEnemy`** | **8,367** | **1,333** | **85,617** | **726** |
| `bool alwaysEnemySeen` + `int alwaysEnemy` (exact `eors.w`) | 8,633 | 1,337 | — | — |
| `bool alwaysEnemySeen` + `int alwaysEnemy` + `& 1` / `!= 0` assignment | 8,640 | 1,336 | — | — |
| `bool alwaysEnemySeen` + `int alwaysEnemy` + reordered planet guard | 8,430 | 1,326 | 85,680 | 724 |
| `bool alwaysEnemySeen` + `bool alwaysEnemy` + reordered planet guard | 9,912 B | — | — | — |
| `bool alwaysEnemy` only (int `alwaysEnemySeen`) | 9,912 B | — | — | — |
| `uint8_t alwaysEnemySeen` + `bool alwaysEnemy` | 9,912 B | — | — | — |
| `bool alwaysEnemySeen` + `uint8_t alwaysEnemy` | 9,912 B | — | — | — |
| `bool otherLock` + landed bools | 9,784 B | — | — | — |

The landed variant is the best raw among the size-exact faithful-tail
shapes (-160 vs the `int`/`int` faithful tail, -266 vs the exact-`eors.w`
shape).  The aligned metric is 13 worse than the baseline; the
register/stack permutation (`campaign` on the stack instead of `sl`,
`alwaysEnemySeen` at `[sp,#96]` instead of the original `[sp,#104]`) is the
dominant residual and is unchanged by this pass.

## 3. The second candidate: the planet guard is not a size lever

The original lowers the planet guard

```cpp
if (!missionBlocksPlanets && this->planetLockTimer >= 1) {
```

as the branchless

```
130486  ldr.w r0,[r4,#416]
13048a  movs  r1,#0
13048c  cmp   r0,#1
13048e  it    lt
130490  movlt r1,#1
130492  orrs.w r0,r5,r1
130496  bne.w 12ec24          ; goto enemies
```

while ours splits the check (a `cmp r5,#0; bne.w` before the blip block and
a plain `ldr; cmp; blt.w` at the guard).  Rewriting the source as
`if (this->planetLockTimer >= 1 && !missionBlocksPlanets)` or
`bool planetNotLocked = ...; if (!(missionBlocksPlanets || planetNotLocked))`
changes the raw metric (8,527 -> 8,432 on the `int`/`int` body, 8,367 ->
8,430 with the landed bools) but **not the size** — the compiler
re-splits the check.  So this is not the missing 4 bytes; it is a
block-placement artefact of the original's branch weights.

## 4. Landed change

```diff
-    int alwaysEnemySeen = false;
+    bool alwaysEnemySeen = false;
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
+        bool alwaysEnemy = false;
...
     this->field_0x1bc = 0;
-    this->plasmaInRange = 0;
-    Globals::status->getCurrentCampaignMission();
-    Globals::status->getCurrentCampaignMission();
 }
```

| metric | before | after |
|---|---:|---:|
| size | 9,908 | **9,908** |
| raw same-address diff | 8,124 | **8,367** |
| instruction-index alignment | 1,320 | **1,333** |
| `.text` differing bytes | 85,374 | **85,617** |
| `.ARM.extab` differing bytes | 725 | **726** |
| `.rel.plt` | byte-identical | **byte-identical** |
| byte_exact / linked_exact | 4507/4507 | **4507/4507** |
| size-exact | 4524/4524 | **4524/4524** |

The raw / `.text` rise is the deliberate price of the faithful tail: the
wave-161 filler tail (`plasmaInRange = 0` + two dead getters) happened to
keep the tail store and the following sound-play blocks at the original's
addresses, so removing it shifts ~240 bytes.  The alternative is the
`bool alwaysEnemySeen` + `int alwaysEnemy` shape, which reproduces the
original's `eors.w` exactly but is 266 raw bytes worse.

## 5. Gate

`check` green: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, parity 5370/5370.  All 27 section sizes exact; file size
2,187,328 B; `.rel.plt` byte-identical.  `.note.gnu.build-id` 20.
