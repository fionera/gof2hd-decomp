# Radar::draw wave 162 (w10809) — `campaign` -> `sl` register-permutation attack

**Status:** source unchanged (base is the best measured size-exact variant); note
landed 2026-11-08. Worktree `wt/w10809`, base master `29248c1d0` (wave 161).
Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`.

Read with `wave161-merge-tests.md`, `radar-draw-extab-wave161-w10709.md`,
`radar-draw-wave160-w10609-lsda.md`, `radar-draw-wave160-w10608.md`.

Baseline (wave-161 HEAD): original 9,908 B; ours 9,908 B; raw same-address diff
**8,124**; instruction-index alignment **1,320**; Radar LSDA record diff **50**
bytes; `.text` 85,374; `.ARM.extab` 725; `.rel.plt` byte-identical; all 27
section sizes exact; file size 2,187,328 B; gate byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

## 1. Ground truth: the permutation is real and is the *first* divergence

The original's prologue is byte-identical to ours up to the campaign call:

```
12e0ec  blx inAlienOrbit
12e0f0  mov r8, r0             ; alienOrbit
12e0f2  ldr r0,[r5,#0]
12e0f4  blx getCurrentCampaignMission
12e0f8  mov sl, r0             ; campaign -> sl   (original)
```

Ours instead does `str r0,[sp,#144]` and reloads it at `12e38c`.  The very first
divergence is earlier still, at `12e07a`: the rematerialised zero /
`missionBlocksPlanets` vreg (`%2482:rgpr` in the pre-Greedy MIR) is `fp` (r11)
in the original but `sl` (r10) in ours.  Both functions push all eight
callee-saved registers; the difference is purely which vreg gets `sl`:

| value | original | ours |
|---|---|---|
| zero / `missionBlocksPlanets` | `fp` (r11) | `sl` (r10) |
| `campaign` | `sl` (r10) | `[sp,#144]` |
| status ptr | `[sp,#156]` | `[sp,#156]` |
| `turretMode` | `[sp,#148]` | `[sp,#152]` |
| `mode` | `[sp,#100]` | `[sp,#104]` |

The `campaign` live range is a single use (`cmp.w sl,#2` at `12e38a`) over
0x292 bytes, so its greedy spill weight is low.  The zero/mbp vreg has six
defs + two uses and wins `sl` first in ours; in the original it gets `fp`.

## 2. Source shapes that DO put `campaign` in `sl`

Every one of these flips the allocation (`mov sl,r0` after the getter and
`subs.w fp,r0,#172` for `missionBlocksPlanets`), but all are size-short:

| shape | size | raw | aligned | note |
|---|---:|---:|---:|---|
| `bool otherLock` (instead of `int`) | 9,784 | 9,163 | 1,815 | campaign `sl`, mbp `fp`; status kept in r6 |
| `bool notTurretMode` | 9,840 | 9,490 | 1,699 | campaign `sl`, mbp in r0 |
| `int`/`char`/`unsigned turretMode` | 9,808 | 9,199 | 1,930 | campaign `sl`, mbp `fp` |
| `bool notOtherLock` | 9,844 | 8,852 | 1,607 | campaign `sl` |
| `int otherLockFlag` | 9,852 | 9,107 | 1,436 | campaign `sl` |

The original stores `notTurretMode`/`notOtherLock`/`otherLock` as full words
(`str r0,[sp,#108]`, `str r0,[sp,#116]`, `str r0,[sp,#144]`), so their source
types are `int`; `bool` is not the original type.  The original's `turretMode`
is also `bool` (`!turretMode` lowers to `eor`, not `clz`).  So the base types
are correct and the permutation is a pure vreg-numbering artefact.

Declaration order / scope does not help: `int campaign;` at the very top,
`int campaign = 0;` at the top, `const`, `unsigned`, `long`, `(void)campaign;`,
a `campaignCopy` local, and all 24 permutations of the mission `getType()` test
all leave `campaign` on the stack and the size at 9,908.

## 3. New size-exact shape: the w10709 row13 landing-pad lead

The w10709 note's open lead was the row13 (enemy-branch `calcDistance`)
landing pad: ours needs an extra 4-byte `mov r4,r0; add r0,sp,#232; b.n`
because the `String` sret temp is at `sp+220` instead of the original's
`sp+232`.  A named temp *does* move the slot and makes the pad size-exact:

```cpp
String dist = this->calcDistance(this->targetPos.x, this->targetPos.y, this->targetPos.z,
                                 this->cameraX, this->cameraY, this->cameraZ);
this->lockLabel = dist;
```

Combined with the w10709 operand swap
(`planetLockTimer >= 1 && !missionBlocksPlanets`), this measures:

| variant | size | raw | aligned | LSDA | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| base | 9,908 | 8,124 | 1,320 | 50 | 85,374 | 725 |
| swap only | 9,912 | 8,492 | 1,309 | **39** | — | — |
| swap + named temp | **9,908** | 8,526 | **1,280** | 48 | 85,776 | **723** |
| swap − `plasmaInRange` tail | 9,908 | 8,543 | 1,302 | 50 | — | — |

The named-temp shape is gate-safe (9,908 B, `.rel.plt` unchanged) and is the
best aligned result found (1,280, −40), but it costs +402 raw bytes and only
buys 2 `.ARM.extab` bytes (total differing bytes 86,119 -> 86,499).  It is not
landed; the base stays the best raw variant.

## 4. Measured, not landed (all size-exact unless noted)

| candidate | raw | aligned | size |
|---|---:|---:|---:|
| `campaign` assigned between `turretMode` and `alienOrbit` | 8,131 | 1,325 | 9,908 |
| `campaign` after `planetTargets` | 8,201 | 1,325 | 9,908 |
| mission `getType()` order `11,0,13,171` (base) | 8,124 | 1,320 | 9,908 |
| mission `getType()` order `0,11,13,171` | 8,136 | 1,326 | 9,908 |
| `if (campaign >= 2u ...)` | 8,125 | 1,320 | 9,908 |
| `if (campaign > 1 ...)` / `if (2 <= campaign ...)` | 8,124 | 1,320 | 9,908 |
| `if (... alienOrbit == 0 ...)` | 8,133 | 1,323 | 9,908 |
| `SetColor` before the two zero stores | 8,155 | 1,328 | 9,908 |
| `bool missionBlocksPlanets` -> `int` | 8,242 | 1,366 | 9,904 |
| `bool alwaysEnemySeen` | 8,415 | 1,352 | 9,912 |
| `bool alienOrbit` | 8,486 | 1,321 | 9,912 |

## 5. Open questions

1. The campaign->sl flip needs the zero/mbp vreg (`%2482`) to lose `sl`.
   All shapes that do it change a type or the allocation globally and are
   68–124 B short.  A shape that flips only the vreg numbering without
   changing codegen was not found.
2. The `swap + named temp` shape fixes the w10709 row13 pad and is size-exact
   at 9,908 B, but the named temp adds +402 raw.  If the named temp can be
   avoided (a shape that moves the sret slot without emitting extra code),
   the swap would give LSDA 39 at 9,908 B and probably a much lower raw.
3. `swap only` is 9,912 B with LSDA 39.  Removing 4 B anywhere before the
   landing pads shifts them and puts the LSDA back to 50; the 4 B must come
   out of the row13 pad itself.

## 6. Gate

`check` green on the unchanged base: byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, data **843/843**, imports **207**,
lint CLEAN, drift 0, parity 5370/5370.  All 27 section sizes exact; file size
2,187,328 B; `.rel.plt` byte-identical.

## 7. Reproduction

```sh
python3 /dev/shm/w10809/fdiff.py _ZN5Radar4drawEP6PlayerP3Hudi
python3 /dev/shm/w10809/align_diff.py _ZN5Radar4drawEP6PlayerP3Hudi
python3 /dev/shm/w10809/lsda2.py
python3 /dev/shm/w10809/secdiff.py
```
