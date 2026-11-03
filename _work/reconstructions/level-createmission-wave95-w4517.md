# w4517: `Level::createMission` (90.721649 %, 17172 B) — wave-95 re-measurement of the
# alienGuns `Vector` lifetime (GOT-indirect vs PLT), the `b3dd0` loop-bound association,
# and a sweep of the remaining local shapes

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w4517`, branch `wave95-w4517`,
base `4a88bd6e`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address − 0x10000).
**No source change is committed**: every variant below is neutral or a regression.
The deliverable is this note.

## Baseline (unchanged, gate green before and after)

```text
_ZN5Level13createMissionEv   fuzzy 90.721649485 %   size 17172 B   @0xaffd8
                             orig 6055 insns / ours 6070 insns
gate: byte 4500 / linked 4500, size 4524/4524, stubs 0, missing 0, wrong_type 0,
      extra 0, data 843/843, imports 207, .text 0x1337c8, .rodata 0x41b67,
      .ARM.exidx 0x6f70, .ARM.extab 0x26290 (-8), lint CLEAN, drift 0,
      RATCHET PASS
```

`cmake --build cmake-build-match --target check -j8` passes before and after this
pass. `report.json` is not modified/committed.

## Result in one line

The `alienGuns` turret loop **is** a direct-initialisation (`Vector turretPos = {t[0],t[1],t[2]};`
after `setRotation`), not a copy-assignment: that variant reproduces the original's
**13 `Vector::operator=` PLT calls and exactly 6055 instructions**, but it compiles to
**17144 B / 88.406276 %** because it destroys the register allocation of the
`b3740`–`b3a00` raid block (that region falls from 70.5 % to 16.6 %). The baseline's
extra `turretPos = Vector{…}` copy-assign is therefore *wrong but score-positive*.
The `b3dd0` `extraAliens` bound also has a different association in the original
(`start + pirates + extraAliens`, not `start + extraAliens + pirates`), but the faithful
order compiles to 17164 B / 90.457732 %. Both are documented below with the exact
instruction evidence so a later pass can combine them with the missing compensations.

## 1. `alienGuns`: the original has 2 `operator=` calls, ours has 3

Source (lines 1961–1969 of `src/game/world/Level.cpp`):

```cpp
            // turretPos is constructed after the direction rotation: the original keeps
            // t[0..2] live across the first MatrixRotateVector and reuses the same
            // 12-byte stack slot for both vectors.
            Vector turretPos;
            Vector turretDir = {t[3], t[4], t[5]};
            v = MatrixRotateVector(rotation, turretDir);
            gun->geometry->setRotation(v);
            turretPos = Vector{t[0], t[1], t[2]};
            v = MatrixRotateVector(rotation, turretPos);
```

Original (`b32c6`–`b3328`, 98 bytes; `r4` = `&v` = `sp+372`, `r5` = sret temp,
`sl` = table base, `r9` = `k*24`):

```text
  b32c6  ldr r0,[sp,#64]          ; bigAlienWave
  b32c8  cbz r0,b32d0
  b32ca  ldr r1,[pc,#732]         ; kAlienStationTurrets
  b32cc  add r1,pc
  b32ce  b.n b32d4
  b32d0  ldr r1,[pc,#728]         ; kAlienOutpostTurrets
  b32d2  add r1,pc
  b32d4  add.w r0,r1,r9           ; &t
  b32d8  mov sl,r1
  b32da  add r4,sp,#312           ; &rotation
  b32dc  ldr r1,[r0,#8]           ; t[2]   -> stashed
  b32de  ldr r2,[r0,#16]          ; t[4]
  b32e0  str r1,[sp,#116]
  b32e2  str r2,[sp,#204]         ; turretDir.y
  b32e4  add r2,sp,#200           ; &turretDir
  b32e6  ldr r1,[r0,#12]          ; t[3]
  b32e8  ldr.w r8,[r0,#4]         ; t[1]   kept in r8
  b32ec  ldr r0,[r0,#20]          ; t[5]
  b32ee  str r1,[sp,#200]         ; turretDir.x
  b32f0  mov r1,r4
  b32f2  str r0,[sp,#208]         ; turretDir.z
  b32f4  mov r0,r5                ; sret temp
  b32f6  blx MatrixRotateVector
  b32fa  add r4,sp,#372           ; &v
  b32fc  mov r1,r5
  b32fe  mov r0,r4
  b3300  blx Vector::operator=@plt          <-- call #1
  b3304  ldr r0,[r6,#8]           ; gun->geometry
  b3306  mov r1,r4
  b3308  blx AEGeometry::setRotation
  b330c  ldr.w r0,[sl,r9]         ; t[0]
  b3310  add r1,sp,#312
  b3312  str.w r8,[sp,#204]       ; turretPos.y = t[1]   (sp+200..208 == turretDir slot)
  b3316  str r0,[sp,#200]         ; turretPos.x = t[0]
  b3318  add r2,sp,#200           ; &turretPos
  b331a  ldr r0,[sp,#116]         ; t[2]
  b331c  str r0,[sp,#208]         ; turretPos.z
  b331e  mov r0,r5
  b3320  blx MatrixRotateVector
  b3324  mov r0,r4
  b3326  mov r1,r5
  b3328  blx Vector::operator=@plt          <-- call #2
```

Ours (`b32ca`–`b3328`, 94 bytes):

```text
  b32ca  movs r0,#0
  b32cc  add r3,sp,#200           ; &turretDir
  b32ce  strd r0,r0,[sp,#212]     ; turretPos = {0,0,0}   (separate slot sp+212)
  b32d2  add r5,sp,#188           ; sret temp at sp+188
  b32d4  str r0,[sp,#220]
  b32d6  add.w r9,sp,#312
  b32da  ldrd r0,r1,[r8,#-8]      ; t[3],t[4]
  b32de  ldr.w r2,[r8]            ; t[5]
  b32e2  stmia r3!,{r0,r1,r2}     ; turretDir = {t[3],t[4],t[5]}
  b32e4  add r2,sp,#200
  b32e6  mov r0,r5
  b32e8  mov r1,r9
  b32ea  blx MatrixRotateVector
  b32ee  ldr r0,[pc,#696]         ; GOT slot for Vector::operator=
  b32f0  add.w r9,sp,#372
  b32f4  mov r1,r5
  b32f6  add r0,pc
  b32f8  ldr r6,[r0]              ; r6 = &Vector::operator=
  b32fa  mov r0,r9
  b32fc  blx r6                             <-- call #1 (GOT-indirect)
  b32fe  ldr r0,[r4,#8]
  b3300  mov r1,r9
  b3302  blx AEGeometry::setRotation
  b3306  sub.w r2,r8,#20          ; &t
  b330a  add.w sl,sp,#212         ; &turretPos
  b330e  add r3,sp,#188           ; &temp2
  b3310  ldmia r2,{r0,r1,r2}      ; t[0..2]
  b3312  stmia r3!,{r0,r1,r2}     ; temp2 = {t[0],t[1],t[2]}
  b3314  mov r0,sl
  b3316  mov r1,r5
  b3318  blx r6                             <-- call #2: turretPos = temp2  (EXTRA)
  b331a  add r1,sp,#312
  b331c  mov r0,r5
  b331e  mov r2,sl
  b3320  blx MatrixRotateVector
  b3324  mov r0,r9
  b3326  mov r1,r5
  b3328  blx r6                             <-- call #3 (GOT-indirect)
```

Two independent facts:

1. **Call count.** `objdump -d` over the whole function: the original has **13**
   `blx …VectoraSERKS1_@plt` sites; ours has **11** PLT sites **+ 3** GOT-indirect
   sites (one GOT load at `b32f8` shared by the three `blx r6`). The three extra
   GOT-indirect calls are exactly the `alienGuns` block; the extra call (#2) is the
   `turretPos = Vector{…}` copy-assignment. `Level.o` carries exactly one
   `R_ARM_GOT_PREL _ZN11AbyssEngine6AEMath6VectoraSERKS1_` relocation, and the IR
   (`Level.ll`) has three direct `call … @_ZN11AbyssEngine6AEMath6VectoraSERKS1_`
   in this block (`%1769`, `%1780`, `%1781`) — so the GOT form is a `-Oz` code-size
   lowering of *repeated calls in one block*, not a source-level function pointer.
2. **Slot sharing.** The original keeps `turretDir` and `turretPos` in the **same**
   slot `sp+200..208`; ours uses `sp+200` and `sp+212` plus a third temp at `sp+188`.

### The faithful shape: `Vector turretPos = {t[0], t[1], t[2]};`

```cpp
            Vector turretDir = {t[3], t[4], t[5]};
            v = MatrixRotateVector(rotation, turretDir);
            gun->geometry->setRotation(v);
            Vector turretPos = {t[0], t[1], t[2]};
            v = MatrixRotateVector(rotation, turretPos);
```

compiles to `b32c6`–`b3312` and produces **exactly 13 PLT `operator=` calls**
(matching the original) and **exactly 6055 instructions** (matching the original's
6055). But it is **17144 B / 88.406276 %**. Region-level ratios
(`difflib`, same normalisation as `verify.py`):

```text
  region            baseline   direct-init
  b3290-b34e0        51.16 %     55.56 %     <- alienGuns improves
  b3740-b3a00        70.51 %     16.62 %     <- raid block collapses (182 vs 143 insns)
  b3dc0-b3e60        70.59 %     61.54 %
  b41ea-b42d2 (EH)   95.73 %     84.21 %
```

So the direct-init is *structurally* right (right call count, right instruction
count) but it perturbs the stack layout enough to make the raid block spill
massively. It is not landable on its own; it needs the raid-block compensation
that wave-94 §5 describes.

Note also that the direct-init variant still does **not** merge the two `Vector`
slots (`turretDir` → `sp+212`, `turretPos` → `sp+200`, temp → `sp+188`), so the
original's slot sharing must come from something else (a scope/lifetime shape we
did not find; see §4).

## 2. `b3dd0`: the original's `extraAliens` bound associates `pirates` before `extraAliens`

Source (lines 2083–2087):

```cpp
        int start = stationPirates + guns + pirateEscorts;
        for (int i = pirates + start + this->friendCount + this->field_164 + this->field_168 + this->hostileCount;
             i < start + extraAliens + pirates + this->friendCount + this->field_164 + this->field_168 + this->hostileCount;
             i++) {
```

Original (`b3dcc`–`b3dfa`):

```text
  b3dcc  add.w r3,sl,#352          ; &this->field_160
  b3dd0  mov.w r8,#0
  b3dd4  mov.w r9,#1
  b3dd8  ldmia r3,{r0,r1,r2,r3}    ; friendCount, field_164, field_168, hostileCount
  b3dda  ldr r6,[sp,#48]           ; stationPirates
  b3ddc  ldr r4,[sp,#92]           ; guns
  b3dde  add r6,r4
  b3de0  ldr r4,[sp,#20]           ; pirateEscorts
  b3de2  add.w ip,r6,r4            ; ip = start = (stationPirates+guns)+pirateEscorts
  b3de6  ldr r6,[sp,#44]           ; pirates
  b3de8  add.w r4,ip,r6            ; r4 = start + pirates
  b3dec  ldr r6,[sp,#32]           ; extraAliens
  b3dee  add r4,r6                 ; r4 = start + pirates + extraAliens   <-- BOUND
  b3df0  add r6,ip                 ; r6 = pirates + start
  b3df2  add r6,r0                 ; + friendCount
  b3df4  add r6,r1                 ; + field_164
  b3df6  add r6,r2                 ; + field_168
  b3df8  add r6,r3                 ; + hostileCount                        <-- INIT
  b3dfa  b.n b3e3e
```

`ip` is the three-term `start` group (confirmed by the init: `r6 = pirates + ip`,
which is our `pirates + start`), `[sp+44]` is `pirates` and `[sp+32]` is
`extraAliens`. The bound is therefore `((start + pirates) + extraAliens) + fields`,
i.e. the source order **`start + pirates + extraAliens`**, whereas our source has
`start + extraAliens + pirates`.

Ours (`b3dd2`–`b3de6`) reassociates to `(sp+32 + sp+40) + (sp+44 + sp+112) + sp+20`:

```text
  b3dd2  ldr r6,[sp,#40]
  b3dd4  ldr r4,[sp,#44]
  b3dd6  add.w ip,lr,r6
  b3dda  ldr r6,[sp,#112]
  b3ddc  add r4,r6
  b3dde  ldr r6,[sp,#20]
  b3de0  add r6,r4
  b3de2  add.w r4,ip,r6
  b3de6  add r6,lr
```

Measured variants:

| variant | size | raw score |
|---|---:|---:|
| **baseline** | **17172** | **90.721649** |
| bound `start + pirates + extraAliens + …` | 17164 | 90.457732 |
| bound reordered **and** init `start + pirates + …` | 17164 | 90.457732 |
| init `start + pirates + …` only | 17172 | 90.721649 (neutral) |

So the bound reorder is exactly the original's association but costs 8 B and
0.26 pp on its own.

## 3. `alienGuns` variants measured this pass (all reverted)

Object sizes are a direct `Level.cpp` compile (NDK r18b clang 7.0.2, `-Oz`);
score is the official `verify.py` normalisation (`autojunk=False`).

| variant | size | raw score | insns |
|---|---:|---:|---:|
| **baseline** (`Vector turretPos;` + `turretPos = Vector{…}`) | **17172** | **90.721649** | 6055/6070 |
| `Vector turretPos = {t[0],t[1],t[2]};` after `setRotation` | 17144 | 88.406276 | 6055/6055 |
| `Vector turretPos(t[0],t[1],t[2]);` | 17144 | 88.406276 | 6055/6055 |
| `Vector turretPos = Vector(t[0],t[1],t[2]);` | 17144 | 88.406276 | 6055/6055 |
| `const Vector turretPos = {…}` | 17144 | 88.406276 | 6055/6055 |
| `const` on both vectors | 17144 | 88.406276 | 6055/6055 |
| `turretPos` initialised **before** `turretDir` | 17144 | 88.547601 | 6055/6056 |
| reuse `turretDir = Vector{t[0],t[1],t[2]};` | 17160 | 88.217822 | 6055/6065 |
| `Vector turretPos;` declared after `setRotation` | 17168 | 88.212489 | 6055/6068 |
| same, paren-init | 17168 | 88.212489 | 6055/6068 |
| `{ Vector turretPos = {…}; … }` inner block | 17152 | 88.296467 | 6055/6061 |
| both vectors in their own `{}` blocks (3 spellings) | 17152 | 88.296467 | 6055/6061 |

Combinations with the two other "faithful" shapes:

| variant | size | raw score |
|---|---:|---:|
| alienGuns direct-init + `b3dd0` bound reorder | 17136 | 88.158547 |
| alienGuns direct-init + `nameLeader` `return;` | 17132 | 88.804429 |
| alienGuns direct-init + `nameLeader` `return;` + `b3dd0` | 17124 | 88.622656 |
| `nameLeader` `return;` + `b3dd0` bound reorder | 17152 | 89.123618 |

## 4. `*bigShipsCount > 0` (line 1837): neutral variants

Original `b0eac: cmp r0,#0 ; b0eae: ble.n b0eec`; ours `b0eac: cmp r0,#1 ;
b0eae: blt.n b0eec`. Same size, same semantics, different DAG form. Every source
spelling tried is neutral or worse:

| variant | size | raw score |
|---|---:|---:|
| **`if (*bigShipsCount > 0 && localRace == 0)`** | **17172** | **90.721649** |
| `if (0 < *bigShipsCount && …)` | 17172 | 90.721649 |
| `if (*bigShipsCount >= 1 && …)` | 17172 | 90.721649 |
| `if (this->field_168 > 0 && …)` (no pointer alias) | 17172 | 90.721649 |
| `if (*bigShipsCount && …)` | 17164 | 89.184061 |
| `if (*bigShipsCount != 0 && …)` | 17164 | 89.184061 |
| `if (*bigShipsCount > 0u && …)` | 17164 | 89.184061 |

(`> 0`, `0 < x` and `>= 1` produce byte-identical objects — clang canonicalises
them before ISel — so the `cmp #1; blt` form is not reachable from this site.)

## 5. Other observations re-confirmed on this base

* The `nameLeader` site really does `return;` in the original
  (`b408e beq.w b02ca` / `b40b2 b.w b02ca`), and the faithful shape compiles to
  17160 B / 89.288661 % (wave-94 §3 reproduced).
* The extra EH landing pad in ours is a `delete` whose pointer is spilled to
  `sp+120`; the original keeps it in a register. Ours has 14 pads, the original 13:
  ours `b4296: mov r9,r0 ; ldr r0,[sp,#120] ; b b42c4 (blx _ZdlPv)`, the original
  has no counterpart between `b4292 (mov r9,r0; mov r0,r8)` and
  `b429e (mov r9,r0; mov r0,r5)`. Pad filler run-lengths: orig
  3,5,2,5,3,10,12,17,4,3,2,1,4,1 vs ours 2,6,2,5,3,9,11,19,4,3,2,1,4,1.
* The `raid`/`hostile` preheader hoist (`ldr r4,[sp,#48]; add r4,r6; str r4,[sp,#112]`
  before the `if (raid)` test) and the `raidType`/`hard` register swap are unchanged
  from wave-94 §5.

## 6. Method / commands

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
# exact Level.cpp compile (NDK r18b clang 7.0.2, -Oz)
clang++ -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  -stdlib=libc++ ... -ffunction-sections -funwind-tables -c src/game/world/Level.cpp
# official score (same normalisation as verify.py, autojunk=False)
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump <ndk objdump> \
  --show _ZN5Level13createMissionEv
```

Scratch (not committed): `/dev/shm/w4517/` (`cc.sh`, `fnsize.py`, `measure.sh`,
`rawscore.py`, `align.py`, `region.py`, `reg2.py`, `try.py`, `batch.py`, `b3dd.py`,
`combo.py`, `cond.py`, `ag3.py`, `ag4.py`, `orig.s`, `ours.s`, `direct.s`,
`base_align.txt`, `direct_align.txt`, `Level.cpp.base`).

## 7. What remains

1. **Land `alienGuns` direct-init together with the raid-block compensation.**
   Direct-init is proven correct (13 PLT `operator=` calls, 6055 insns) but costs
   28 B and collapses `b3740`–`b3a00` to 16.6 %. The compensating source shape in
   the raid block is the missing piece; until it is found, every faithful variant
   is a regression.
2. **`turretDir`/`turretPos` slot sharing** (`sp+200` for both in the original).
   Direct-init, `const`, inner blocks and reordering all fail to merge the slots;
   clang still assigns `sp+212`/`sp+200`. Unknown lifetime shape.
3. **`b3dd0` bound order** (`start + pirates + extraAliens`) is proven but −8 B;
   it needs an 8-byte compensation elsewhere.
4. **raid/guard SSC flip**: unchanged from wave-94 — the guard needs
   `freq(bb.519) > 0.676489`, i.e. the `inAlienOrbit()` branch ≥ 67.2 % taken.
5. **EH pads**: ours carries one extra cleanup (the spilled `delete` at `sp+120`);
   pad order is invoke-order driven.
