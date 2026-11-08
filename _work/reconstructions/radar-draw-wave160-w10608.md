# Radar::draw wave 160 (w10608) — duplicate `stationLockTimer` store landed at 9,908 B;
# raw 8,124 -> 8,536, aligned 1,320 -> 1,314

**Status:** source change landed, 2026-11-08. Worktree `wt/w10608`, based on `master`
(`08af4633b`, wave 159). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave159-w10505.md`, `radar-draw-wave159-w10504.md`,
`wave159-merge-tests.md`.

Baseline (wave-159 HEAD): original 9,908 B; ours 9,908 B; raw same-address diff
**8,124**; instruction-index alignment **1,320**; `.text` differing bytes 85,722;
`.ARM.extab` 1,256; all 27 section sizes exact; file size 2,187,328 B; gate
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

## 1. Ground truth: the lock-reset `else` stores `stationLockTimer` in BOTH arms

Original disassembly (image base 0x10000, function 0x12e050):

```
12ebb0  ldr  r0,[r4,#36]      ; this->lockedStation (0x24)
12ebb2  cmp  r0,#0
12ebb4  beq.w 12fe60          ; if (lockedStation == nullptr) -> cold block
12ebb8  movs r0,#0
12ebba  str.w r0,[r4,#420]    ; stationLockTimer = 0   (non-null arm)
12ebbe  b.w  12e376

12fe60  movs r0,#0
12fe62  str.w r0,[r4,#420]    ; stationLockTimer = 0   (null arm)
12fe66  str   r0,[r4,#40]     ; field_0x28 = 0
12fe68  b.w  12e376
```

`grep -c '[r4,#420]'` on the original gives **7** references (4 stores, 2 loads,
1 `vldr`). The wave-159 HEAD had **5** (3 stores, 2 loads): it used the hoisted
`if (lockedStation == nullptr) { field_0x28 = 0; } stationLockTimer = 0;` form,
which emits one shared store (`cbnz` skip + shared `str.w`).

The faithful source is therefore

```cpp
} else {
    if (this->lockedStation == nullptr) {
        this->field_0x28 = 0;
        this->stationLockTimer = 0;
    } else {
        this->stationLockTimer = 0;
    }
}
```

which reproduces the original lock block **byte-for-byte in structure**: a
`cmp/beq.w` to an out-of-line 12-byte cold block (`movs; str.w timer; str
field_0x28; b.w`) plus the 18-byte hot arm (`ldr; cmp; beq.w; movs; str.w
timer; b.w`). Measured: the cold block lands at `12fe68` (original `12fe60`,
8-byte whole-function shift) with identical instruction bytes.

### Variants that do NOT reproduce the two stores

| shape | size | raw | aligned | note |
|---|---:|---:|---:|---|
| `__builtin_expect(lockedStation==nullptr, 0)` + hoisted timer | 9916 | 8136 | 1323 | cold block jumps back to the shared store; no duplication |
| `__builtin_expect(...,1)` + explicit dup | 9912 | 8489 | 1317 | wrong arm cold |
| `__builtin_expect(...,0)` + explicit dup | 9920 | 8141 | 1324 | wrong arm cold |
| `bool locked = ...; if (locked) {dup} else {dup}` | 9908 | 8536 | 1314 | identical to explicit dup (temp optimised away) |
| `int locked = ...` | 9908 | 8536 | 1314 | identical |
| swapped arms (`if (lockedStation != nullptr) timer else {field_0x28; timer}`) | 9900 | 8509 | 1318 | -8 B |
| timer hoisted before the `if` | 9892 | 8222 | 1329 | -16 B |
| `else { field_0x28 = 0; timer = 0; }` (wrong arm) | 9900 | 8509 | 1318 | -8 B |
| both arms `field_0x28 = 0; timer = 0;` | 9892 | 8224 | 1318 | -16 B |

Only the explicit duplicate-store form gives the original's cold/hot split.

## 2. Size reconciliation: the w10504 getter tail is no longer needed

The wave-159 HEAD carried two dead `Globals::status->getCurrentCampaignMission()`
calls at the tail as a size companion for the *missing* duplicate store
(w10504 §5). The original's tail is only `this->field_0x1bc = 0;` (4 B body,
`movs; str.w [r4,#444]` at `130326`) followed by the stack-canary epilogue; the
original has **no** tail `plasmaInRange = 0` (only 2 `[r4,#304]` stores: the
entry reset at `12e08c` and the gas-cloud `plasmaInRange = 1` at `12f56a`).

Measured tail matrix on the duplicate-store body (all `int alienOrbit`, direct
`lockLabel`, LSDA cst 112):

| tail | size | raw | aligned |
|---|---:|---:|---:|
| `field_0x1bc = 0;` (faithful) | 9904 | 8527 | 1312 |
| **`field_0x1bc = 0; plasmaInRange = 0;` (landed)** | **9908** | **8536** | **1314** |
| `field_0x1bc = 0;` + 1 getter | 9908 | 8500 | 1318 |
| `field_0x1bc = 0;` + 2 getters (w10504 tail) | 9916 | 8550 | 1319 |
| `field_0x1bc = 0; plasmaInRange = 0;` + 1 getter | 9912 | 8540 | 1319 |
| `field_0x1bc = 0; plasmaInRange = 0;` + 2 getters | 9920 | 8532 | 1319 |

The faithful tail alone is **4 B short**: there is a genuine ~4-byte body
difference elsewhere (see §4). The `plasmaInRange = 0` tail store is the
established w10505 size companion and keeps the size exact without touching the
LSDA. `field_0x54 = 0` as the 4-byte companion measures identically
(9,908 / 8,536 / 1,314).

## 3. Landed change

```diff
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
     this->field_0x1bc = 0;
     this->plasmaInRange = 0;
-    Globals::status->getCurrentCampaignMission();
-    Globals::status->getCurrentCampaignMission();
 }
```

| metric | before (wave 159) | after |
|---|---:|---:|
| size | 9,908 | **9,908** |
| raw same-address diff | 8,124 | **8,536** |
| instruction-index alignment | 1,320 | **1,314** |
| `.text` differing bytes | 85,722 | **86,134** |
| `.ARM.extab` differing bytes | 1,256 | **1,256** |
| byte_exact / linked_exact | 4507/4507 | **4507/4507** |
| size-exact | 4524/4524 | **4524/4524** |

The raw regression (+412) is the expected address-shift artefact: the duplicate
store moves the tail from `130326` (where the wave-159 HEAD coincidentally
matched the original) to `130332`. The aligned metric improves by 6. The LSDA is
unchanged (`.ARM.extab` diff 1,256, cst 112, 19 rows).

## 4. Open questions

1. **The faithful tail is 4 B short (9,904).** The body is 4 bytes shorter than
   the original somewhere between the lock block and the tail. One confirmed
   2-byte difference is the `alwaysEnemySeen` condition in the
   `if (field_0x1bc >= 1)` sound block: the original lowers
   `(current != 151 && alwaysEnemySeen)` as `cmp r1,#151; beq; ldr r2,[sp,#104];
   eors.w r2,r2,#1; bne` (12 B), ours as `ldr r2,[sp,#100]; cmp r2,#0; beq;
   cmp r1,#151; beq` (10 B). The remaining ~2 B is unidentified. Forcing
   `bool alwaysEnemySeen` gives 9,912 / 8,628 / 1,348 (worse). Swapping the
   source operand order does not change the lowering.
2. **Register allocation is still the dominant residual.** `campaign` lives on
   the stack (`[sp,#144]`) instead of `sl`; `missionBlocksPlanets` lives in `sl`
   instead of `fp`; the landmark `68` is hoisted into `fp` instead of
   rematerialised into `r6`. This shifts every block and is why the raw metric
   is high. `bool otherLock` is still the only lever that puts `campaign` in
   `sl`, but it is 120 B short (9,788) and aligned 1,807.
3. **`bool alienOrbit` + duplicate store + faithful tail** measures
   9,908 / 8,599 / **1,307** — a further aligned win (7) but a worse raw
   (+63). Not landed because w10504's landed `int alienOrbit` is kept (the
   coordinator should test the merge order).
4. The cleanup landing pads at the end of the function differ structurally
   (original has two extra `b.n` and one fewer `add r0,sp,#220; blx
   StringD1Ev` pair than ours); this is part of the 4-byte body deficit.

## 5. Gate

`check` green after the change: byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, data **843/843**, imports **207**,
lint CLEAN, drift 0, parity 5370/5370. All 27 section sizes unchanged; file size
2,187,328 B.
