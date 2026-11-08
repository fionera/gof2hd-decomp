# w9912 (wave 153): `Level::createMission` — raid-prologue high registers still
# need `this`=r4 in the alien loop; landed the cargo/shipGroup store-order swap
# (raw 5484 -> 5487, align 776 -> 774, size 17172)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9912`, based on master
`4a6104a43` (wave 152). Continues `level-createmission-wave152-w9812.md`,
`…-wave151-w9811.md`, `…-wave151-w9711.md`, `…-wave148-w9509.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w9912/fdiff.py`; instruction-index alignment is
`/dev/shm/w9912/align_diff.py` (worktree-local copies of the shared tools).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5484   align 776
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 87,131   .ARM.extab 1,324   .rel.plt 0   .note.gnu.build-id 20
```

## 1. Target raid prologue (original) — still unchanged

```text
b3752  ldr.w r8, [sp,#108]   ; &Globals::status
b3756  ldr   r3, [sp,#84]    ; raid
b3758  ldr.w fp, [sp,#40]    ; fortressGuns
b375c  cmp   r3, #0
b375e  beq.w b396c
b3762  ldr   r0, [sp,#120]   ; &Globals::globals
b3764  ldr   r6, [sp,#68]    ; raidRace
b3766  ldr   r0, [r0]
b3768  mov   r1, r6
b376a  blx   getRandomEnemyFighter
b376e  mov   r9, r0            ; raidType
b3770  ldr.w r0, [r8]          ; Globals::status (deref AFTER the call)
b3774  blx   hardCoreMode
```

Ours (HEAD, w9812 volatile read):

```text
b3752  ldr   r4, [sp,#48]    ; fortressGuns
b3754  ldr   r3, [sp,#88]    ; raid
b3756  cmp   r3, #0
b3758  beq.w b3964
b375c  ldr   r0, [sp,#108]   ; &Globals::status
b375e  ldr.w fp, [r0]        ; status VALUE -> fp (hoisted)
b3762  ldr   r0, [sp,#120]
b3764  ldr   r6, [sp,#68]
b3766  ldr   r0, [r0]
b3768  mov   r1, r6
b376a  blx   getRandomEnemyFighter
b376e  mov   sl, r0          ; raidType
b3770  mov   r0, fp
b3772  blx   hardCoreMode
b3776  add   r4, r8          ; guns = fortressGuns + alienGuns (early)
```

Register maps at the raid block (unchanged from w9712):

| | this | guns | raidRace | alienGuns | raidType | hard | fortressGuns | status |
|---|---|---|---|---|---|---|---|---|
| ORIG | r4 | r5 | r6 | sl | r9 | r8 | fp | &status in r8 |
| OURS | r5 | r4 | r6 | r8 | sl | r9 | r4 | value in fp |

`this`=r4 is still the trigger.  The original's alien-loop preheader loads
`this` into r4 once (`b3232 ldr r4,[sp,#124]`) and the loop reuses r4 for the
`&rotation`/`&v` address, rematerialising `this` at the loop tail
(`b3396 ldr r4,[sp,#124]`).  Ours keeps `this` in fp for the whole loop
(`b3282 ldr.w fp,[sp,#124]`) because `i` is spilled to `sp+116` instead of
living in fp; r4 is then taken by `gun` and sl by the address temp.

## 2. New hypotheses measured this wave (all keep 17,172 B unless noted)

### 2a. `volatile` fortressGuns (mission suggestion)

| variant | size | raw | align |
|---|---:|---:|---:|
| **HEAD (volatile status value)** | **17172** | **5484** | **776** |
| `volatile int fortressGuns` declaration | 17164 | 13179 | 1599 |
| `int guns = alienGuns + *(volatile int *)&fortressGuns` | 17172 | 8222 | 1636 |
| above + non-volatile status | 17172 | 8221 | 1636 |
| `volatile int fortressGuns` + non-volatile status | 17164 | 13178 | 1599 |

Forcing a per-use stack reload of `fortressGuns` is catastrophic; every form
loses the early `add r4,r8` and the whole raid block re-colours.

### 2b. Opaque helper call (mission suggestion)

```cpp
namespace {
    static Status *opaqueStatus() { return Globals::status; }
    static Status *(*volatile opaqueStatusFn)() = opaqueStatus;
}
...
Status *status = opaqueStatusFn();
```

Result: size **17180** (8 B over), raw 6359, align 830.  The indirect call
forces the value load into a register but adds a `blx` plus the function
pointer load; not landable at 17,172 B.  (w9812's `&Globals::status` variant
was 12 B over.)

### 2c. Raid-block orderings (mission suggestion)

| variant | size | raw | align |
|---|---:|---:|---:|
| **HEAD `S R H G`** | **17172** | **5484** | **776** |
| `R H G` no local (`Globals::status->…`) | 17172 | 5521 | 790 |
| `S` non-volatile local | 17172 | 5492 | 779 |
| `R S H G` (status after raidType) | 17172 | 5496 | 776 |
| `H R S G` (hard before raidType, unfaithful call order) | 17172 | 5500 | 773 |
| `G` after the `raidRace==8` if | 17172 | 6475 | 1215 |
| `fortressGuns + alienGuns` operand order | 17172 | 5484 | 776 |
| `Status *volatile status` | 17176 | 5529 | 808 |

As w9509 found, no raid-block ordering feeds back into the alien-loop map:
the raid block is downstream.  `H R S G` is the best aligned (773) but swaps
the two calls (unfaithful) and costs 16 raw bytes.

### 2d. Alien-loop declaration / store orderings

| variant | size | raw | align |
|---|---:|---:|---:|
| **HEAD** | **17172** | **5484** | **776** |
| **`shipGroup` before `cargo` (landed)** | **17172** | **5487** | **774** |
| `Vector v` before `Matrix rotation` | 17172 | 5482 | 794 |
| `setScaling` before `setPosition` | 17172 | 5485 | 778 |
| `shipGroup` before `setScaling` | 17172 | 5489 | 776 |
| `cargo` before the first rotate | 17172 | 5654 | 779 |
| `field_0x70` after `cargo` | 17172 | 5484 | 776 |
| `Vector p2; p2 = …` | 17324 | 16581 | 2768 |
| second rotate inlined (no `p2`) | 17168 | 6586 | 1208 |

The landed store-order swap matches the original's binary store order: the
original writes `cargo` (`str r0,[r6,#80]`, offset 0x50) at `b339a` **before**
`shipGroup` (`str r0,[r6,#40]`, offset 0x28) at `b33a6`; HEAD's source order
produced the inverse.  After the swap ours emits `str r0,[r4,#80]` then
`str r0,[r4,#40]`, matching.  That is the -2 aligned instructions.

## 3. Landed change (`src/game/world/Level.cpp`, alien-gun loop tail)

```diff
             ((PlayerTurret *) gun)->setScaling(6.0f);
-            gun->cargo = nullptr;
             gun->shipGroup = localRace;
+            gun->cargo = nullptr;
         }
     }
```

## 4. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5484 -> 5487   (+3)
                           align  776 -> 774    (-2)
.text diff             87,131 -> 87,134  (+3)
.ARM.extab              1,324 -> 1,324
.rel.plt                    0 -> 0   (byte-identical)
.note.gnu.build-id         20 -> 20
.rodata/.data/.bss      exact -> exact
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
```

All 27 section sizes remain exact; file size exact.

## 5. Open questions / next levers

1. The target prologue still needs `this`=r4 in the alien loop, i.e. `i` must
   live in fp instead of `sp+116`.  Every shape that reaches 17,172 B spills
   `i`; the original's `i`=fp is the last missing colouring.
2. Removing the `(i < 0)` ternary hack (`(bigAlienWave + (i < 0))` ->
   `bigAlienWave`) reproduces the original's `cbz` but shrinks the function to
   17,140 B (-32); the missing 32 B of genuine code is not identified.  If it
   is found, the alien loop would match the original's `cbz` and likely free
   the allocator to place `i` in fp.
3. `H R S G` shows the aligned metric can reach 773, but only by swapping the
   `getRandomEnemyFighter`/`hardCoreMode` call order (unfaithful).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9912/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9912/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w9912/{fdiff.py,align_diff.py,dump.py,regions.py,m.sh,
drive.py,drive2.py,drive3.py,sweep.py,Level.base.cpp}`.
