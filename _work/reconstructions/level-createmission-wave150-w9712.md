# w9712 (wave 150): `Level::createMission` — raid-prologue locals
# (`Status **`, `fortressGuns`) and the `p2` alien-loop map; no source
# change landed (baseline 5657/714/17172 remains the best 17172 variant)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9712`, based on master
`db9658a51` (wave 150). Continues `level-createmission-wave150-w9612.md`,
`…-wave149-w9611.md`, `…-wave148-w9509.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w9712/fdiff.py`; instruction-index alignment is
`/dev/shm/w9712/align_diff.py`.

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5657   align 714
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. Target raid prologue (original)

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

Ours (HEAD):

```text
b3752  ldr   r4, [sp,#48]    ; fortressGuns
b3754  ldr   r3, [sp,#88]    ; raid
b3756  cmp   r3, #0
b3758  beq.w b3964
b375c  ldr   r0, [sp,#108]   ; &Globals::status
b375e  ldr   r6, [sp,#68]    ; raidRace
b3760  ldr.w fp, [r0]        ; status VALUE -> fp (cached local)
...
b376e  mov   sl, r0          ; raidType
b3770  mov   r0, fp
b3772  blx   hardCoreMode
```

The register maps at the raid block:

| | this | guns | raidRace | alienGuns | raidType | hard | fortressGuns | status |
|---|---|---|---|---|---|---|---|---|
| ORIG | r4 | r5 | r6 | sl | r9 | r8 | fp | &status in r8 |
| OURS | r5 | r4 | r6 | r8 | sl | r9 | r4 | value in fp |

`this`=r4 is the trigger: with r4 busy, `fortressGuns` goes to fp and
`&Globals::status` to r8; ours is the exact inverse permutation.

## 2. New hypotheses measured this wave

All variants keep size 17,172 B unless noted. `S` = `Status *status =
Globals::status;`, `R` = `getRandomEnemyFighter`, `H` = `hardCoreMode`,
`G` = `int guns = alienGuns + fortressGuns;`.

### 2a. `Globals::status` address forms (the "forces r8" hypothesis)

| variant | raw | align | prologue |
|---|---:|---:|---|
| **HEAD `Status *status` (value)** | **5657** | **714** | `ldr r4,[sp,#48]` … `ldr r0,[sp,#108]` after branch |
| `Status **statusp = &Globals::status` (inside `if`) | 5686 | 782 | identical; address rematerialised after branch |
| `Status **statusp` declared before `if (raid)` | 5686 | 782 | identical; compiler sinks the address |
| `Status *&status = Globals::status` (reference) | 5686 | 782 | identical |
| no local (`Globals::status->…`) + `G` top | 5686 | 782 | identical |
| `Status *status` declared before `if (raid)` | 5657 | 714 | identical codegen (sunk back) |
| `Status *status` declared before alien loop | 17292 (size) | 2038 | — |
| `Status *status` declared before pirate loop | 17188 (size) | 1236 | — |
| `status` local also used in the raid loop | 17148 (size) | 1201 | — |

The address-of forms never hoist `ldr.w r8,[sp,#108]` above the `cmp`/branch:
`&Globals::status` lives in the sp+108 spill slot and is cheap to
rematerialise, so the compiler keeps it in caller-saved r0 after the branch.
The value form (`Status *status`) is still the raw winner.

### 2b. `fortressGuns` locals (the "forces fp" hypothesis)

`int fg = fortressGuns;` (before `G`, before `S`, before `if (raid)`, or
inside the block) is coalesced/rematerialised: **5657 / 714, byte-identical**
in every position. `int ag = alienGuns;` and `ag`+`fg` together are also
byte-identical. The compiler folds the copies because `fortressGuns` is a
`pirateFortress ? 5 : 0` value that is trivial to rematerialise.

### 2c. Mission-combination (`status` + `fg` + raid-loop use)

`Status *status = Globals::status; int fg = fortressGuns;` with
`status->inPirateLootOrbit()` in the raid loop: **17148 B**, raw 6530,
align 1201. Not landable (size short).

### 2d. Alien-loop shapes that move `i`/`this`

| alien-loop shape | size | raw | align | this | i |
|---|---:|---:|---:|---|---|
| **HEAD** | 17172 | **5657** | **714** | fp | r9 (spilled) |
| `Vector p` + named `p2` for second rotate (`p2`) | 17164 | 6461 | 1198 | r5 | **fp** |
| `p2` + `setScaling` before `setPosition` | 17164 | 6459 | 1218 | r5 | fp |
| `p2` + `Status **statusp` | 17164 | 6456 | 1211 | r5 | fp |
| `p2` + faithful no-local `R H [if] G` | 17176 | 6332 | 1230 | r5 | fp |
| `p2` + cached `S R H [if] G` | 17176 | 6332 | 1216 | r5 | fp |
| named `p` only (first rotate) | 17180 | 5821 | 806 | r6 | fp |
| `p2` only (second rotate) | 17180 | 5819 | 854 | — | — |
| `gun->shipGroup` before `gun->cargo` | 17172 | 5660 | 712 | fp | r9 |
| `i` declared outside the `for` | 17172 | 5657 | 714 | fp | r9 |
| `__builtin_expect(alienGuns != 0, 1)` | 17172 | 5836 | 833 | r5 | r9 |
| `__builtin_expect(pirateFortress != 0, 1)` | 17172 | 6312 | 1134 | — | — |
| `__builtin_expect(raid != 0, 1)` | 17200 | 14883 | 1778 | — | — |
| cache `this->enemies` in the loop | 17148 | 6456 | 972 | fp | r9 |
| cache `friendCount`/`field_164`/`field_168` | 17156 | 6508 | 1414 | — | — |
| `v` declared inside the loop | 17148 | 6547 | 1202 | — | — |
| `Matrix rotation` inside the loop | 17168 | 6509 | 1164 | — | — |

**Key new result:** the `p2` shape (a named temp for the *second* rotate)
moves `i` into `fp` — the original's `i` register — and gives
`this`=r5, `gun`=r4, `k`=sl, `t`=r6, `temp`=r9. That is a one-register
rotation of the original map (`this`=r4, `temp`=r5, `gun`=r6, `k`=r9,
`t`=sl, `i`=fp). It is **8 B short** (17,164), so it cannot be landed under
the size gate (size_exact would drop 4524 -> 4523). This is the same `p2`
shape w9611 measured at 17,148/17,164; on the current base it is 17,164.

The `p2` map is the closest any source shape has come to the original:
`i`=fp is now correct, and only `this`/`temp`/`gun`/`k`/`t` remain rotated.
If a genuine 8-byte statement is found for the alien loop, `p2` becomes
landable and is the natural base for the sret-temp hoist (w9711's target).

## 3. Why the locals cannot force the registers

The greedy allocator's machine output (`-mllvm -print-machineinstrs`,
`After Greedy Register Allocator` / `After Virtual Register Rewriter`) shows
the loop's `this` is a *separate* live range from the preheader's `this`:
the preheader loads `this` into r4 (`renamable $r4 = t2LDRi12 %stack.74`),
then the loop reloads it into r11 (`renamable $r11 = t2LDRi12 %stack.74`)
because r4 is taken by `gun`. The `Status`/`fortressGuns` locals are outside
that loop and cannot change the loop's colouring; they are folded before RA.

## 4. Landed change

None: the baseline (`S R H G`, cached `Status *status`, `Vector p` in the
pirate loop only) is still the best 17,172 B variant at raw 5657 / align 714.
All 17,172 B alternatives measured this wave are raw-worse and/or
align-worse. Source restored to HEAD; the only commit is this note.

## 5. Gate counts (before -> after)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
_ZN5Level13createMissionEv raw 5657 -> 5657, align 714 -> 714, size 17172
```

## 6. Open questions

1. `p2` proves `i`=fp is reachable and leaves only a one-register rotation
   (`this` r5->r4, `temp` r9->r5, `gun` r4->r6, `k` sl->r9, `t` r6->sl).
   Find the 8 B the `p2` shape is short (a genuine alien-loop statement)
   and then a shape that rotates `this`/`temp`/`gun`.
2. `&Globals::status` is only hoisted into r8 once r8 is free; in ours r8
   holds `alienGuns` across the raid block. Reloading `alienGuns` from
   sp+52 (as the original does at b3386) is the missing piece, and it is
   itself a consequence of the `this`/`i` colouring.
3. The `gun->shipGroup`/`gun->cargo` store order is source-order
   `cargo, shipGroup` but ours schedules `shipGroup, cargo`; swapping the
   source restores the emission order at raw 5660 / align 712 (a wash).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9712/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9712/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9712/map.py
```

Scratch: `/dev/shm/w9712/{fdiff.py,align_diff.py,map.py,harn.py,batch1..23.py,
Level.base.cpp,Level.ll,mi.txt,greedy_cm.txt,vrr_cm.txt}`.
