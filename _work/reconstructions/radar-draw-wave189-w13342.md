# Radar::draw wave 189 (w13342) — data-flow / local-lifetime pass: the +4-byte route-block
# copy is the whole residual; no size-exact raw win

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left at
the base. This pass commits only this note. Worktree `wt/w13342`, branch `wave189-w13342`,
base `master` `30e0a4013`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13315.md` (the campaign-in-`sl` body family and the
`volatile autoPilotTarget` + String-temp body), `radar-draw-wave189-w13308.md` (the
register/branch pass) and `radar-draw-wave189-w13319.md` (MIR / stack-slot evidence). This
note is self-contained: every number below was measured in this worktree on the 6,309
base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,309** |
| instruction-index alignment | 1,858 |
| instruction count | 3,541 ours / 3,532 orig |
| frame | `sub sp,#296` (`0x128`) |
| `mode` argument slot | `sp+0x64` (matches original) |
| `campaign` | spilled `sp+0x90` |
| shared zero | `sl` (`12e07a mov.w sl,#0`) |
| planet-loop counter | `sl` |
| planet-loop `stations` | spilled `sp+0x84` |
| `.text` diff (whole `.so`) | 73,982 |
| `.ARM.extab` diff | 639 (size/base exact at `0x1a3b78`, 156,312 B) |
| `.rel.plt` | byte-identical |
| build-id | 20 |
| total differing bytes | 74,617 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. The best size-exact campaign-in-`sl` body: raw 6,918

On the base, apply exactly five edits:

1. replace the `otherLockFlag` if/else with the plain OR chain
   `bool otherLock = this->lockedAsteroid != nullptr || this->lockedGasCloud != nullptr ||
   this->field_0x1c != nullptr || this->field_0x14 != 0;`
2. `bossSeen = bossSeen || enemy->shipGroup == 10;` (the rare `shipGroup==10` guard)
3. `int marked = 0;` (instead of `bool marked = false;`)
4. `volatile int autoPilotTarget = ...` (in the `onPlanetCourse` false branch)
5. keep `int alienOrbit` (do **not** make it `bool`)

Result: **raw 6,918**, aligned 2,014, size **9,908**, insn 3,520/3,532. `.rel.plt` unchanged.
It is *not* a win (6,918 > 6,309) and is not landed; it is the best campaign-in-`sl` body found
so far and the subject of the rest of this note.

Its skeleton is the original's:

```
12e07a: f04f 0b00  mov.w fp,#0        ; shared zero in fp
12e0f8: 4682       mov  sl,r0          ; campaign in sl
12e38a: f1ba 0f02  cmp.w sl,#2
12e3c6: blx getStations
12e3ca: 4682       mov  sl,r0          ; stations in sl
12e3cc: f04f 0b00  mov.w fp,#0        ; i in fp
12e3d2: f8cd a068  str.w sl,[sp,#0x68] ; cold stations copy (original: #0x74)
12e400: f8da 0004  ldr.w r0,[sl,#4]   ; hot loop keeps stations in sl
```

## 2. Why it loses: one 4-byte copy pair at 12e23c

The 6,918 body is byte-identical to the original up to `12e23c`, then inserts two extra
instructions in the route block:

```
orig  12e238: 4620       mov r0,r4
      12e23a: 4631       mov r1,r6
      12e23c: f738 eac4  blx Radar::update
      12e284: f504 79c6  add.w r9,r4,#396   ; &this->lockLabel -> r9
      12e288: 4648       mov r0,r9
6918  12e238: 4620       mov r0,r4
      12e23a: 4631       mov r1,r6
      12e23c: 46d1       mov r9,sl            ; <<< extra: save campaign
      12e23e: f738 eac4  blx Radar::update
      12e286: f504 7ac6  add.w sl,r4,#396   ; &this->lockLabel -> sl
      12e28a: 4650       mov r0,sl
      12e2e0: 46ca       mov sl,r9            ; <<< extra: restore campaign
```

The pair is 4 bytes, so every instruction after `12e23c` is shifted by +4 and the same-address
byte diff collapses (`0x12e250` onward: 246/256 bytes differ). The base avoids this by keeping
`campaign` spilled, which is why it is still the better local optimum for this metric.

## 3. MIR evidence: the allocator splits `campaign` to free `sl` for the address

`-mllvm -print-after-all` on the 6,918 body (142 MB dump, scratch `allpass6918.txt`; greedy
RA at line 1,567,724, VirtRegRewriter at 1,576,987) shows the exact split:

- `%2746` = `campaign` (`COPY $r0` after `getCurrentCampaignMission`, MIR offset 2128B)
  -> **r10 (`sl`)**.
- `%2745` = a copy of `campaign` created at the route block (MIR 6696B, no debug location)
  -> **r9**.
- `%12` = `t2ADDri %this, 396` (`&this->lockLabel`, MIR 7168B) -> **r10 (`sl`)**.

```
6696B  $r9  = COPY killed $r10        ; mov r9,sl
7168B  $r10 = t2ADDri $r4, 396        ; &lockLabel in sl
8088B  $r10 = COPY killed $r9         ; mov sl,r9
8328B  $r10 = COPY killed $r9
```

`%12` is live from 7168B to 8032B (it is both the `String::operator=` receiver and the
`DrawString` argument), so it needs a callee-saved register. The allocator chose to move
`campaign` out of `sl` into `r9` and give `sl` to the address, instead of leaving `campaign`
in `sl` and using the free `r9` for the address. That is the whole +4.

Source-level attempts to flip that tie-break all leave the body byte-identical (the compiler
CSEs the member address): `String &label = this->lockLabel;`, `String *label =
&this->lockLabel;`, `label`/`*label` for the assignment, `this->lockLabel.operator=(...)`,
`(*this).lockLabel`, declaring the reference before `this->update(waypoint)`, plain
parentheses, and dropping the `(float)` casts. Measured forms:

| edit (on the 6,918 body) | raw | size |
|---|---:|---:|
| base 6,918 body | **6,918** | 9,908 |
| `String dist = calcDistance(...); this->lockLabel = dist;` | 8,526 | **9,904** |
| nested-scope `{ String dist = ...; this->lockLabel = dist; }` | 6,918 | 9,908 |
| `String &label` / `String *label` / `operator=` / `(*this)` / before `update` | 6,918 | 9,908 |

## 4. The planet loop: the first 49 instructions already match the original

Aligning the 6,918 body's planet loop at `12e3d8` with the original's `12e3d4` (the +4 shift
removed) shows the first 49 instructions are **identical**, including
`ldr.w r0,[sl,#4]` / `ldr.w r8,[r0,fp,lsl#2]` / `mov r9,r0` (currentStation) /
`cmp r8,r9` / `movs r1,#10`. The divergence is instruction 49:

```
orig  12e45c: 9827       ldr   r0,[sp,#0x9c]   ; &Globals::status
      12e45e: 6800       ldr   r0,[r0]         ; Globals::status
      12e460: blx getCurrentCampaignMission
      12e468: 9827       ldr   r0,[sp,#0x9c]   ; reload
      12e46a: 6800       ldr   r0,[r0]
6918  12e45e: f8dd a09c  ldr.w sl,[sp,#0x9c]   ; &Globals::status -> sl
      12e462: f8da 0000  ldr.w r0,[sl]
      12e466: blx getCurrentCampaignMission
      12e470: f8da 0000  ldr.w r0,[sl]         ; reuse
```

The machine IR has one vreg `%2596` for the `[sp,#0x9c]` load, used in both campaign-if
blocks (MIR 27336B and 27504B), so it is live across the first call and the allocator puts it
in `sl` (spilling `stations` to `sp+0x68`). The original reloads the address from the stack
for the second call and keeps `sl = stations`. This is the same greedy-allocator tie-break as
section 3, one block later.

## 5. The two other size-exact bodies and their exact offsets

| body | raw | aligned | size | route-block address | early offset |
|---|---:|---:|---:|---|---|
| base | **6,309** | 1,858 | 9,908 | n/a (`campaign` spilled) | 0 |
| 6,918 (`volatile` + `int alienOrbit`) | 6,918 | 2,014 | 9,908 | `sl` + `mov r9,sl` pair | 0 |
| 6,721 (`volatile` + String temp + `bool alienOrbit`) | 6,721 | 2,018 | 9,908 | `r6` (no pair) | 0 |
| 7,074 (`bool alienOrbit`, no `volatile`) | 7,074 | 1,836 | 9,908 | `r9` (matches original) | +6 (`alienOrbit` spill at `12e194`) |

- **7,074**: the route block is the original's exactly (`12e28a add.w r9,r4,#396`), but the
  out-of-line landmark block evicts `alienOrbit` from `r8` at `12e194` (`mov r1,r8;
  str.w r8,[sp,#0x90]`, 6 bytes), shifting the whole tail by +6.
- **6,721**: `volatile autoPilotTarget` removes the early spill, and the named `String dist`
  temp moves the address to `r6` (no pair), but the temp's destructor is relocated from
  `12e290` to the end of the block (`12e2f8`) and the planet loop still has the `sl`
  status-pointer CSE.
- **6,918**: no early offset and the original's route-block *shape*, but pays the 4-byte
  `mov r9,sl` pair.

The stack-slot rotation is a separate residual: 6,918 has `mode 0x6c` (orig `0x64`),
`stations` cold `0x68` (orig `0x74`), `landmarkHit 0x70` (orig `0x6c`), `planetHit 0x90`
(orig `0x98`). The `stationId` spill at `sp+0x64` (in the campaign `isTarget` block) is what
pushes `mode` to `0x6c`; the original keeps `stationId` in `r8` and uses `sl` as the
`stations->data()[i]` temp, restoring `stations` from `sp+0x74`.

## 6. Cells measured this pass (all reverted)

Every row is a full incremental `link_so -j2` + `tools/fn_diff.py` (`ulimit -v 12000000`;
scratch `/home/debian/GalaxyOnFire2/scratch/w13342/`).

### 6.1 `alienOrbit` / `marked` / `autoPilotTarget` types (on the 6,918 body)

| edit | raw | size |
|---|---:|---:|
| `alienOrbit` as `char`/`uchar`/`short`/`ushort`/`unsigned`/`long`/`int8..int32`/`long long` | 6,918 | 9,908 |
| `alienOrbit` as `bool` | 8,843 | **9,912** |
| `marked` as `int`/`char`/`uchar`/`short`/`ushort`/`long`/`unsigned`/`int8..int32` | 6,918 | 9,908 |
| `marked` as `bool` | 8,669 | **9,896** |
| `autoPilotTarget` as `volatile int`/`unsigned`/`long`/`int32`/`uint32` | 6,918 | 9,908 |
| `autoPilotTarget` as `volatile short`/`char`/`uchar` | 8,792-8,793 | **9,912** |

### 6.2 `volatile` on other locals (on the 6,918 body)

| edit | raw | size |
|---|---:|---:|
| `volatile bool planetHit` | **6,929** | 9,908 |
| `volatile bool otherLock` | 9,301 | 9,908 |
| `volatile bool onPlanetCourse` | 9,300 | 9,908 |
| `volatile bool bossSeen` | 9,430 | **9,816** |
| `volatile bool landmarkHit` / `enemyHit` | 9,597 / 9,287 | **9,888 / 9,904** |
| `volatile bool turretMode` | 10,212 | **10,540** |
| `volatile int missionBlocksPlanets` | 9,479 | **9,860** |
| `volatile bool alwaysEnemySeen` / `alwaysEnemy` | 9,274 / 9,543 | **9,876 / 9,852** |
| `volatile int notTurretMode` / `notOtherLock` | 9,457 / 9,365 | **9,916 / 9,904** |
| `volatile int textOffset` | 9,931 | **10,252** |
| `volatile int stationId` / `currentStation` / `warpGate` | 9,877 / 9,319 / 9,787 | **10,256 / 9,924 / 10,232** |

### 6.3 Campaign-if / route-block / false-branch shapes

| edit | raw | size |
|---|---:|---:|
| `expect(...,1)` on the campaign `if` | 9,248 | **9,948** |
| plain `if` (no expect) | 8,973 | **9,900** |
| `expect` on the mission part only | 9,048 | **9,884** |
| `int cur = getCurrentCampaignMission(); (cur < 148 \|\| cur >= 152)` | 9,151 | **9,872** |
| `(campaign < 148 \|\| campaign >= 152)` (no calls) | 9,535 | **9,844** |
| one call + `campaign` | 9,621 | **9,860** |
| `if (route != nullptr && campaign >= 0)` | 8,726 | **9,896** |
| `if (route != nullptr && campaign != -1)` | 8,825 | **9,904** |
| false-branch declaration permutations (5 orders) | 8,996-9,318 | **9,880-9,896** |
| inline `system` in `getStationEnumIndex` | 7,074 | 9,908 |

### 6.4 On the base body

| edit | raw | size |
|---|---:|---:|
| `volatile int autoPilotTarget` alone | 9,364 | **9,872** |
| `volatile` + `int marked` | 9,336 | **9,876** |
| `volatile` + `bossSeen \|\|` | 9,413 | **9,960** |
| `volatile` + `int marked` + `bossSeen \|\|` | 9,434 | **9,872** |

## 7. Conclusion

The 6,918 body is the first size-exact body that matches the original's *pre-loop and
planet-loop skeleton* (campaign `sl`, zero `fp`, `stations` `sl`, `i` `fp`) and the first 49
instructions of the planet loop instruction-for-instruction. Its entire residual is one
greedy-allocator tie-break: the allocator splits `campaign`'s live range at the route block to
free `sl` for `&this->lockLabel`, adding a 4-byte `mov r9,sl` / `mov sl,r9` pair at
`12e23c`/`12e2e0` and shifting the tail by +4. The two siblings show the two halves of the
fix: 7,074 already has the original's `r9` route block but pays a +6 `alienOrbit` spill at
`12e194`; 6,721 removes both but relocates the String destructor and keeps the `sl`
status-pointer CSE in the planet loop. No size-exact, `.rel.plt`-identical body with
raw < 6,309 was found in the assigned cells. `src/` is left at the base.

## 8. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6309 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13342/`: `Radar.base.cpp` (6,309 base),
`b6918.so` (6,918 body), `b7074.so` (7,074), `b6721.so` (6,721), `base.so`,
`allpass6918.txt` (`-mllvm -print-after-all`, greedy RA at line 1,567,724), `mk6918b.py`
(builds the 6,918 body), `r1.py`..`r17.py` (the sweeps above), `ld2.py` (instruction diff),
`bd2.py` (same-address byte-run report), `orig.dis`/`b6918.dis`.
