# Radar::draw wave 159 (w10505) — landmark-lock duplicate `stationLockTimer` store + no tail `String`; register permutation re-measured

**Status:** source change landed, 2026-11-08. Worktree `wt/w10505`, based on `master`
(`cfde1f5f2`, wave 158). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave157-w10405.md`, `radar-draw-wave157-w10306.md`,
`radar-draw-wave154-w10002.md`.

Baseline (HEAD, wave 158): original 9,908 B; ours 9,908 B; raw same-address diff
**8,181**; instruction-index alignment **1,278**; all 27 section sizes exact;
file size 2,187,328 B; gate byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## 1. New ground truth: the landmark-lock `else` has two `stationLockTimer = 0` stores

Manual disassembly of the original (image base 0x10000, function 0x12e050):

```
12ebb0  ldr  r0,[r4,#36]      ; this->lockedStation (0x24)
12ebb2  cmp  r0,#0
12ebb4  beq  12fe60           ; if (lockedStation == nullptr)
12ebb8  movs r0,#0
12ebba  str  r0,[r4,#420]     ; stationLockTimer = 0   (else arm)
12ebbe  b    12e376           ; -> otherLock recomputation

12fe60  movs r0,#0
12fe62  str  r0,[r4,#420]     ; stationLockTimer = 0   (nullptr arm)
12fe66  str  r0,[r4,#40]      ; field_0x28 = 0
12fe68  b    12e376
```

So the original source is

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

(the two stores are emitted in the order `field_0x28` then `stationLockTimer` in
the nullptr arm, i.e. the source order above; the swapped order was also measured,
see §4). Our wave-158 HEAD had a single unconditional store.

`grep -c '[r4,#420]'` on the original gives **7** references (4 stores, 2 loads,
1 `vldr`); wave-158 HEAD had **5** (3 stores, 2 loads). The missing store is the
duplicate above.

## 2. Ground truth re-confirmed: the original has no tail `String`

The original's last body instruction is `str.w r0,[r4,#444]` at `0x130328`,
followed directly by the stack-canary epilogue at `0x13032c`. There is **no**
`StringC1Ev`/`StringD1Ev` pair at the tail. Wave-158 HEAD still had the
wave-156 `String dummy;` filler (12 B) at the end of `Radar::draw`.

Independently, the original has the duplicate store (+12 B) and no tail
`String` (−12 B), so the two changes net to the original size 9,908 B.

## 3. Landed change

```cpp
// src/game/weapons/Radar.cpp (Radar::draw)
            } else {
                if (this->lockedStation == nullptr) {
                    this->field_0x28 = 0;
                    this->stationLockTimer = 0;
                } else {
                    this->stationLockTimer = 0;
                }
            }
...
    this->field_0x1bc = 0;
    this->plasmaInRange = 0;
    // (tail `String dummy;` removed)
```

| metric | before (wave 158) | after |
|---|---:|---:|
| size | 9,908 | **9,908** |
| raw same-address diff | 8,181 | **8,520** |
| instruction-index alignment | 1,278 | **1,273** |
| size-exact functions | 4524/4524 | **4524/4524** |
| byte_exact / linked_exact | 4507/4507 | **4507/4507** |

The aligned metric improves by 5; the raw metric regresses by 339. The raw
regression is a pure address-shift artefact: wave-158 HEAD was missing 12 B in
the middle (the duplicate store) and had 12 B of non-original filler at the very
end, so the two errors partly cancelled in the same-address comparison. The
landed source is the faithful one (both facts are independently proven from the
original), keeps the size exact and the gate green.

## 4. Register map and the failed `68`/`campaign` permutation

Original (0x12e050, 9,908 B):

| value | original |
|---|---|
| `campaign` | `sl` (`12e0f8 mov sl,r0`), spilled at `12e7ba` |
| `missionBlocksPlanets` | `fp` (`12e07a`/`12e0d8`), spilled `[sp,#112]` at `12e3ba` |
| planet-loop index | `fp` (`12e3c8 mov.w fp,#0`) |
| landmark-loop index | `sl` (`12e7be mov.w sl,#0`) |
| landmark `68` | `r6` remat (`12e918`, `12ea10 movs r6,#68`) |
| `&lockLabel` | `r9` (`12e284`, `12e7b6`, `12ecc8`, `12f436`) |
| enemy-loop index | `fp` (`12ecd8 mov.w fp,#0`) |
| enemy `68` | `sl` (`12ecd0`, `12eea2`, `12f128`, `12f196 mov.w sl,#68`) |
| `bossSeen` | `r6` (`12ece0 movs r6,#0`) |
| `marked` (planet loop) | `[sp,#132]` (4-byte int stores) |
| `alwaysEnemySeen` | `[sp,#104]` (4-byte int stores) |

Wave-158 HEAD (9,908 B):

| value | HEAD |
|---|---|
| `campaign` | `[sp,#144]` (`12e0f8 str r0,[sp,#144]`) |
| `missionBlocksPlanets` | `sl` |
| planet-loop index | `sl` |
| landmark-loop index | `sl` |
| landmark `68` | `fp` hoist (`12e77e mov.w fp,#68`) |
| `&lockLabel` | `[sp,#108]`/`[sp,#76]` |
| enemy-loop index | `r5` |
| enemy `68` | `r6` remat |
| `bossSeen` | `r9` |
| `marked` (planet loop) | `fp` (`bool`) |

The original's planet loop is emitted **before** the landmark loop in the binary
(planet loop `12e3d4`-`12e748`, landmark loop `12e7be`-`12eb42`), exactly as in
our HEAD; the source order (landmark block first, then `otherLock`, then the
planet block) is already correct.

Measured variants (all on the wave-158 base unless stated; size / raw / aligned):

| shape | size | raw | aligned | campaign | landmark 68 | enemy 68 |
|---|---:|---:|---:|---|---|---|
| HEAD | 9908 | 8181 | 1278 | stack | fp | r6 |
| **landed: dup store + no tail `String`** | **9908** | **8520** | **1273** | stack | fp | r6 |
| dup store only (tail kept) | 9920 | 8541 | 1274 | stack | fp | r6 |
| no tail `String` only | 9896 | 8317 | 1292 | stack | fp | r6 |
| swapped dup order (`stationLockTimer` first) + no tail | 9888 | 8477 | 1291 | stack | fp | r6 |
| `bool otherLock` | 9792 | 9142 | 1633 | **sl** | r9 | r9 |
| `bool otherLock` + dup + no tail | 9792 | — | — | sl | r9 | r9 |
| `int marked` | 9912 | 8328 | 1335 | stack | fp | r6 |
| `int planetHit` | 9940 | 8672 | 2150 | **fp** | **r6** | r6 |
| `int planetHit` + `int bossSeen` | 9920 | 8662 | 1817 | fp | r6 | fp |
| `int planetHit` + `int marked` | 9940 | 8666 | 2155 | fp | r6 | r6 |
| `bool alwaysEnemy` | 9916 | 8472 | 1303 | stack | fp | r6 |
| `bool otherLock` + `int marked` | 9796 | 9166 | 1658 | sl | r9 | r9 |
| `bool otherLock` + `int planetHit` | 9808 | 9056 | 1680 | sl | r9 | r9 |
| `bool otherLock` + temp + `DrawString(this->lockLabel)` | 9796 | 9171 | 1625 | sl | r9 | r9 |
| `DrawString(this->lockLabel)` in the landmark else arm | 9904 | 8519 | 1307 | stack | fp | r6 |
| planet block moved before landmark block (`int`) | 9836 | 9492 | 2024 | r9 | — | — |
| planet block moved before landmark block (`bool`) | 9812 | 9436 | 1887 | sl | r9 | r9 |

`int planetHit` is the only single toggle that reproduces the original's
**landmark `68` = `r6` remat** shape (and it also moves `campaign` to `fp`), but
it costs +32 B and 2,150 aligned. `bool otherLock` is still the only lever that
puts `campaign` in `sl`, but it is 116 B short and gives landmark/enemy `68` =
`r9`. Combining the two (`bool otherLock` + `int planetHit`) gives `campaign` =
`sl` but landmark `68` back in `r9`.

## 5. New ground-truth detail: the landmark `i<=1` `else` arm keeps the `String` temp

The original's landmark `i<=1` `else` arm (`12e974`-`12e9e8`) is

```
12e9a6  blx  calcDistance          ; returns a String temp at sp+232
12e9aa  ldr  r6,[sp,#104]          ; r6 = &this->lockLabel
12e9b0  blx  String::operator=     ; lockLabel = temp
12e9b6  blx  String::~String()     ; destroy temp
12e9de  mov  r2,r6
12e9e0  blx  DrawString            ; DrawString(..., this->lockLabel, ...)
```

i.e. the temp is `String labelCopy2 = calcDistance(...); this->lockLabel =
labelCopy2;` and the `DrawString` argument is **`this->lockLabel`**, not
`labelCopy2`. Wave-158 HEAD passes `labelCopy2` to `DrawString`. Changing only
that argument (keeping the temp) measures 9,904 / 8,519 / 1,307 — a −4 B body
change, so it was not landed (it would break size-exact).

## 6. Conclusion / open questions

1. The landed change is the faithful landmark-lock `else` (duplicate store) plus
   removal of the non-original tail `String`; it keeps size 9,908 and the gate
   green, and improves the instruction-index alignment 1,278 → 1,273 (raw
   8,181 → 8,520, an address-shift artefact of the two cancelled 12-byte errors
   in HEAD).
2. The `campaign`→`sl` / landmark-`68`→`r6` / enemy-`68`→`sl` permutation still
   has no 9,908 B source shape. `bool otherLock` gives `campaign`=`sl` but is
   116 B short and puts `68` in `r9`; `int planetHit` gives landmark `68`=`r6`
   but puts `campaign` in `fp` and costs +32 B.
3. The remaining ~116 B of body difference in the `bool otherLock` family is not
   explained by the duplicate store (+12), the tail `String` (−12), `int marked`
   (+4) or `int planetHit` (+32); a further, still-unidentified body difference
   (or a different `otherLock`/`wreck`/`marked` type mix) is required.
4. The original's planet loop also keeps `stations` in `sl` and `marked` on the
   stack (`[sp,#132]`); every source shape tried here keeps `stations` on the
   stack in the wave-158 base. That is the next register-pressure lever to try.

## 7. Gate

`check` green after the change: byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, data **843/843**, imports **207**,
lint CLEAN, drift 0, parity 5370/5370. All 27 section sizes unchanged; file size
2,187,328 B.
