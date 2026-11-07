# Long-tail wave 134 (w7782): ModStation semantic `pitch` / pool-order lever,
# Generator/SpaceLounge/Gun/MGame re-probes — no source win

Status: **no landed `src/` change.** Worktree `wt/w7782`, base `bbffef482`
(wave-134 master). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Extends `long-tail-wave134-w7762.md`, `gun-calccharactercollision-wave133-w7739.md`,
`spacelounge-generator-wave133-w7740.md`, `near-miss-wave133-w7741.md`.

Gate before/after (source restored): byte_exact **4506 -> 4506**,
linked_exact **4506 -> 4506**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.ARM.extab` differing bytes **1416**.

## 0. Targets attempted

| target | result |
|---|---|
| `ModStation::ModStation` (5 insns) | **new: original second call is `pitch`, not `yaw`; `iPad == 1` gives the original pool order but `cmp #1`** |
| `Generator::createMission` (97.2 %, 1996 B) | `bonus -= rem + rem` stays 1996 B but keeps `rem` in r5; `bonus = rawBonus - rem` grows frame 96 -> 104 |
| `SpaceLounge::drawLounge` (95.7 %, 1948 B) | `boxW` variants with the faithful clamp are 1968 B / 619 diff lines |
| `Gun::calcCharacterCollision` (93.4 %, 1984 B) | `expr` form 230 diff lines (HEAD 226); `-disable-machine-licm` 1928 B |
| `MGame::OnUpdate` (99.9 %, 11440 B) | removing the `Status *st` anchor is 11392 B (confirms w7661) |
| `.ARM.extab` (1416 B) | decoder re-run; 12 records >1 byte, Radar::Radar 7 rows unchanged |

## 1. ModStation::ModStation — the original really passes `pitch`

The committed source (`far = yaw`, `short pad3`) is 5 structural lines from the
original. Reading the original's first `MatrixSetRotation` block
(`0xd5456..0xd5470`) gives the *semantics* away:

```
d5446  ldr.w r2, [r0, r8, lsl #2]   ; r2 = gHangarCamPitch[race]  (pitch)
d5456  ldr.w r3, [r0, r8, lsl #2]   ; r3 = yawTable[race]        (yaw)
...
d546e  mov   r4, r2                  ; save pitch across the call
d5470  blx   MatrixSetRotation       ; (nearKey, pitch, yaw, 0, 2)
...
d54f0  mov   r2, r4                  ; r2 = pitch
d5502  blx   MatrixSetRotation       ; (farKey, pitch, yawTable[race], 0, 2)
```

So the original's **second** call is `MatrixSetRotation(farKey, pitch, ...)`.
The committed source passes `yaw` there, i.e. it is semantically wrong but
happens to match 5 instructions because the yaw value is then live across the
first call (`ldr r4` + `mov r3,r4` instead of the original `ldr r3` +
`mov r4,r2`).  The `pitch` shape (the faithful one) is **18 structural
lines**, 1008 B.

### 1.1 NEW lever: `Globals::iPad == 1` restores the original LDRLIT order

The 18-line `pitch` shape differs from the original only in the two yaw-table
pointer loads and the schedule around them.  The committed `yaw` shape has the
original pool order `Pitch, YawIPad, Yaw`; the `pitch` shape reverses it to
`Pitch, Yaw, YawIPad` (checked with `objdump -dr` on the `.o`: reloc order at
`0x3e0/0x3e4/0x3e8`).  The pool order follows the materialisation order of the
`select` operands, i.e. the true operand first.

Writing the condition as `Globals::iPad == 1` (instead of `iPad`) prevents the
`icmp eq 0` canonicalisation and keeps the true operand (`YawIPad`) first:

```
const float *yawTable = Globals::iPad == 1 ? gHangarCamYawIPad : gHangarCamYaw;
...
MatrixSetRotation(farKey, pitch,
                  (Globals::iPad == 1 ? gHangarCamYawIPad : gHangarCamYaw)[race], ...);
```

Object reloc order is then `Pitch, YawIPad, Yaw` (matching the original), but
the comparison becomes `cmp r1,#1` (original `cmp r1,#0`), so the shape is
**19** structural lines (18 + the `cmp`).  The register assignment is still
`r4 = YawIPad, r3 = Yaw` where the original has `r3 = YawIPad, r4 = Yaw`
(the cmov operand order is tied to the `eq 0` condition).

Variants measured this wave (all 1008 B unless noted; `try_mod.sh` harness):

| variant | structural lines |
|---|---:|
| HEAD (`yaw`, `short pad3`) | **5** |
| `pitch` + inline ternary | 18 |
| `pitch` + `*(const float *) &pitchBits` | 18 |
| `pitch` + `pitchTable[race]` | 18 |
| `pitch` + `yaw` declared after the first call | 18 |
| `pitch` + `if (!iPad) Yaw else YawIPad` / `if (iPad) YawIPad else Yaw` | 18 |
| `pitch` + `(void) yaw;` after the first call | 18 |
| `pitch` + `iPad != 0` / `> 0` / `>= 1` / `!=(unsigned char)0` / volatile load | 18 |
| `pitch` + `iPad == 1` / `== true` | **19** (right pool order) |
| `pitch` + `bool ipadFlag = iPad` then `ipadFlag ? ...` | 984 B / 271 |
| `pitch` + `iPad & 1` | 50 |
| `-mllvm -enable-misched=false` (committed `yaw`) | 6 |
| `-mllvm -enable-misched=false` (`pitch`) | 18 |
| `-mllvm -pre-RA-sched=list-hybrid` (`pitch`) | 18 |
| `-mllvm -pre-RA-sched=source` / `list-ilp` / `list-burr` | 984 / 1008 / 984 B, 292 / 340 / 332 |
| `-mllvm -sink-insts-to-avoid-spills`, `-misched-postra=false` | 18 |

Conclusion: the original is `pitch` with pool order `Pitch, YawIPad, Yaw` and
`cmp #0`.  The remaining tie is the cmov operand order (`r3`/`r4` swap), which
`iPad == 1` does not fix.  The committed `yaw` shape remains the byte-closest
(5 lines) but is semantically wrong; the faithful `pitch` shape is 18 lines.
No shape reached 0.

## 2. Generator::createMission — `rem` spill vs `bonus` register

The residual is the `bonus` block (`src/game/mission/Generator.cpp:949-955`).
Original:

```
adds  r5, r1, r6        ; bonus = rawBonus + rem
str   r1, [sp, #32]     ; rem -> sp+32
movs  r1, #50
mov   r0, r5
blx   __aeabi_idivmod   ; bonus % 50
ldr   r0, [sp, #32]     ; rem
cmp   r1, #0
it    ne
subne r5, r6, r0        ; bonus = rawBonus - rem
```

HEAD keeps `rem` in `r5` and `bonus` in `r6`, and emits
`subne.w r6, r6, r5, lsl #1`.  New variants this wave:

| variant | size | note |
|---|---:|---|
| HEAD (`bonus -= rem * 2`) | 1996 | rem r5, bonus r6 |
| `bonus -= rem + rem` | 1996 | still `subne.w r6, r6, r5, lsl #1` |
| `bonus = bonus - rem - rem` | 2000 | frame 104 |
| `bonus -= rem; bonus -= rem;` | 2000 | frame 104 |
| `bonus = rawBonus - rem;` | 2000 | frame 104 |
| `volatile int rem` | 1996 | 202 diff lines, official **96.8 %** (worse); spills rem to sp+44, bonus r6 |
| `bonus -= (rem << 1)` / `bonus = bonus - (rem << 1)` | 1996 | same r5/r6 |
| `bonus = rawBonus; if ((rawBonus+rem)%50) bonus = rawBonus-rem;` | **1992** | frame 96, `subne r5,r5,r6`; 4 B short (no rem spill) |

The `bonus = rawBonus - rem` form (the value the original computes) forces a
104-byte frame; the 96-byte frame requires keeping the `rawBonus + rem`
intermediate, which is exactly why our allocator keeps it in `r6` and spills
nothing.  No 96-byte shape with `rem` at `sp+32` and `bonus` in `r5` found.

## 3. SpaceLounge::drawLounge — `boxW` is 1968 B

Re-measured the w7740 `boxW` lever with the faithful right arm
(`width + pad * 2` in the clamp and `boxW` in both `drawBox`/`DrawRectangle`):

| variant | size | structural lines |
|---|---:|---:|
| HEAD (`short pad3`) | 1948 | 189 |
| `boxW = pad*2 + width` before `lineHeight` | 1968 | 619 |
| `lineHeight` then `boxW` | 1968 | 619 |
| `boxW = width + pad*2` (both orders) | 1968 | 619 |
| `boxW = width + pad + pad` / `pad + pad + width` | 1968 | 619 |
| `pad2 = pad + pad; boxW = width + pad2` (both orders) | 1968 | 619 |

The target colouring (`pad = r9`, `x = r5`) is reached but the extra
`lineHeight`/`pad*2` packing costs 20 B.  No 1948 B shape found.

## 4. Gun::calcCharacterCollision — `expr` and MachineLICM

* `expr` form (inline `getPlayer()->getPosition()`): 1984 B, **230** diff
  lines (HEAD `stmt` form: 226; note's official 93.4 % vs 92.6 %).
* `-mllvm -disable-machine-licm` on the `expr` form: **1928 B** (function
  shifted, 645 diff lines) — disabling the hoist also removes the loop-body
  remats the original has, so not a win.
* `-mllvm -disable-licm` fails to build the TU (IR LICM is required).

## 5. MGame::OnUpdate / `.ARM.extab`

* Removing the `Status *st` anchor (the wave-129 candidate #1) gives
  **11392 B** (48 B short), confirming the anchor is load-bearing for the
  `r9` GOT base.  Restored.
* `.ARM.extab` re-decoded: 1416 differing bytes total; the >1-byte records
  are `0x971b8` (24), `0xb42ec` (9), `0x12d484` (7), `0x12e050` (29),
  `0x170d38` (14), `0x17a144` (30), `0x12d484` = `Radar::Radar` (7 rows).
  The w7742/w7762 `init-statement`/`compound` shapes leave the 7 rows
  unchanged; no new shape tried here.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7782
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                       # extab 1416 B
```

Scratch (not committed), `/dev/shm/w7782/`: `try_mod.sh`, `mod_variants*.py`,
`mod_cond*.py`, `mod_flags.py`, `gen_variants.py`, `sl_variants*.py`,
`gun_flags*.py`, plus the `mod_*.txt` / `gen_*.txt` / `sl_*.txt` verify logs.

## 7. Files changed

`_work/reconstructions/long-tail-wave134-w7782.md` only.
Gate: byte 4506 / linked 4506, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
