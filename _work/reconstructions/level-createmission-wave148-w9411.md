# w9411 (wave 148): `Level::createMission` — raid prologue is gated by
# `this`=r5 vs r4; the original has no cached `Status *status`; landed the
# faithful alien-block declaration order (`M v k b`), align 728 -> 716

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9411`, based on master
`cbe3b5f7f` (wave 147). Continues `level-createmission-wave146-w9310.md`,
`…-wave146-w9208.md`, `…-wave145b-w9014.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff
(`/dev/shm/w9411/fdiff.py`) and instruction-index alignment
(`/dev/shm/w9411/align_diff.py`, copy of the shared tool with the worktree
`OURS` path).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5560   align 728
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 86,160   .ARM.extab 1,309   .rel.plt 4   .note.gnu.build-id 20
```

## 1. The raid prologue, original vs ours (function-relative)

```text
ORIG
+0377a  ldr.w r8, [sp, #108]   ; &Globals::status  (hoisted)
+0377e  ldr   r3, [sp, #84]    ; raid
+03780  ldr.w fp, [sp, #40]    ; fortressGuns     (hoisted)
+03784  cmp   r3, #0
+03786  beq.w +0x3994
+0378a  ldr   r0, [sp, #120]   ; &Globals::globals
+0378c  ldr   r6, [sp, #68]    ; raidRace
+0378e  ldr   r0, [r0]
+03790  mov   r1, r6
+03792  blx   getRandomEnemyFighter
+03796  mov   r9, r0            ; raidType
+03798  ldr.w r0, [r8]          ; Globals::status (deref!)
+0379c  blx   hardCoreMode
+037a0  cmp   r6, #8
+037a2  bne.n +0x380a
+037a4  mov   r8, r0            ; hard

OURS (HEAD)
+0377a  ldr   r4, [sp, #48]     ; fortressGuns -> r4
+0377c  ldr   r3, [sp, #88]     ; raid
+0377e  cmp   r3, #0
+03780  beq.w +0x398c
+03784  ldr   r0, [sp, #108]    ; &Globals::status (NOT hoisted)
+03786  ldr   r6, [sp, #68]
+03788  ldr.w fp, [r0]          ; status pointer -> fp (cached local)
+0378c  ldr   r0, [sp, #120]
+0378e  mov   r1, r6
+03790  ldr   r0, [r0]
+03792  blx   getRandomEnemyFighter
+03796  mov   sl, r0
+03798  mov   r0, fp
+0379a  blx   hardCoreMode
+0379e  add   r4, r8            ; guns = alienGuns + fortressGuns (early)
+037a0  cmp   r6, #8
```

Three independent differences:

1. **`this` is r4 (orig) vs r5 (ours) at raid entry.**  The shared
   pirate/raid preheader in ours reloads `this` into r5 (`b3398
   ldr r5,[sp,#124]`); the original has no reload because the alien loop left
   `this` in r4.  This is the w9310 chain: the sret temp (`sp+212`) is hoisted
   into r5 in the original, ours rematerialises it into fp, so r5 stays free
   for `this`.  Because r4 is free in ours, `fortressGuns` reloads into r4;
   in the original r4 is `this`, so `fortressGuns` goes to fp.
2. **No cached `Status *status` in the original.**  `ldr.w r0,[r8]` (3798) and
   `ldr r0,[sp,#108]; ldr r0,[r0]` (37bc) show the original keeps the
   *address* `&Globals::status` in r8 and dereferences per use.  Ours caches
   the pointer in fp (w9208's `Status *status = Globals::status;`).
3. **`guns` is computed after the `if (raidRace == 8 …)` block** in the
   original (`380e add.w r5,sl,fp`), not at the top of the raid block.

## 2. Raid-block source orderings tried (all size 17,172 unless noted)

`S` = `Status *status = Globals::status;`, `G` = `int guns = alienGuns +
fortressGuns;`, `R` = `int raidType = …getRandomEnemyFighter(raidRace);`,
`H` = `bool hard = status->hardCoreMode();`.

| variant | size | raw | align | raid region 0x377a..0x3994 | note |
|---|---:|---:|---:|---:|---|
| **HEAD** | 17172 | 5560 | 728 | 484 | S G R H |
| S R H G (G after if, m3) | 17172 | 5560 | 728 | 484 | no-op |
| G R S H (m4) | 17172 | 5564 | **725** | 484 | align -3, raw +4 |
| R S G H (v7) | 17172 | 5560 | 728 | 484 | no-op |
| S R H, no local (v10) | **17156** | 6521 | 1197 | — | block moves early |
| no local, G first (v12) | 17172 | 5589 | 796 | 513 | worse |
| `Status *const` / `Status &` | 17172 | 5560 | 728 | 484 | no-op |
| S before `if (raid)` (w1) | 17172 | 5560 | 728 | 484 | no-op |
| S before if + G after if (w6) | 17156 | 6521 | 1197 | — | block moves |
| `__builtin_expect(raid,1)` | 17184 | 14777 | 1720 | — | bad |
| `__builtin_expect(raid,0)` | 17268 | 16590 | 2321 | — | bad |
| `__builtin_expect(alienGuns!=0,1)` | 17172 | 5579 | 764 | 484 | worse |
| `__builtin_expect(raidRace==8,0)` | 17172 | 5560 | 728 | 484 | folded |
| `__builtin_expect(inner if,0)` | 17180 | 6007 | 851 | — | worse |
| `__builtin_expect(inner if,1)` | 17172 | 6448 | 922 | — | worse |
| `__builtin_expect(inPirateLootOrbit,1)` | 17148 | 14666 | 1110 | — | worse |
| `__builtin_expect(inPirateLootOrbit,0)` | 17284 | 16557 | 2294 | — | worse |

No raid-block ordering produced `ldr.w fp,[sp,#40]` / `ldr.w r8,[sp,#108]`.
The prologue is a pure downstream consequence of `this`=r4, which the alien
loop controls.  `G` after the `if` is faithful but costs 16 B (the `guns`
value no longer has to stay live across the `if`), and the block is then
placed before the pirate-fortress loop, so it is not a usable lever on its own.

## 3. Alien-block declaration order sweep (24 permutations)

`M` = `Matrix rotation;`, `b` = `Vector base = …getPosition();`,
`v` = `Vector v = {0,0,0};`, `k` = `int k = 0;`.

| perm | size | raw | align |
|---|---:|---:|---:|
| `M b v k` (HEAD, w9208 "basefirst") | 17172 | 5560 | 728 |
| `M b k v` / `M k b v` / `k M b v` | 17172 | 5560 | 728 |
| **`M v k b` (faithful: rotation, v, base)** | **17172** | **5561** | **716** |
| `M v b k` (faithful) | 17172 | 5561 | 716 |
| `v M k b` / `v M b k` / `k v M b` / `v k M b` | 17172 | 5559 | 734 |
| `b M k v` … (base first) | 17172 | 5562 | — |

The original's asm initialises `Matrix rotation` (b323c..b326a), then `v`
(b326c..b3272), then calls `getPosition` into `base` (b3274..b3284): the
faithful source order is `M v k b`.  This is also the align winner
(728 -> 716, -12 non-matching instructions) at a cost of +1 raw byte; the
+1 is confined to the pre-alien region (`0x0..0x32c2` diff 2587 -> 2588).
`v M k b` is the raw winner (5559) but is *unfaithful* (v before rotation) and
align-regresses (734).

## 4. Landed change

`src/game/world/Level.cpp`, alien-gun block only: reorder the declarations from
the w9208 metric-only `M b v k` to the faithful `M v k b`:

```cpp
Matrix rotation;
Vector v = {0.0f, 0.0f, 0.0f};
int k = 0;
Vector base = (*this->enemies)[this->friendCount + this->field_164]->getPosition();
```

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
.text diff      86,160 -> 86,161  (+1)
.ARM.extab      1,309  -> 1,309
.rel.plt        4      -> 4
.note.gnu.build-id 20  -> 20
_ZN5Level13createMissionEv raw 5560 -> 5561 (+1), align 728 -> 716 (-12), size 17172
```

All 27 section sizes remain exact; file size exact.

## 6. Open questions

1. Hoist the alien loop's sret temp (`sp+212`) into r5.  Everything else in the
   chain (`this`=r4 -> `i`=fp -> `gun`=r6 -> `t`=sl, and then the pirate/raid
   preheader drops its `ldr r5,[sp,#124]` reload and `fortressGuns` goes to
   fp, `&Globals::status` to r8) follows.  w9310's `Vector tmp` attempts grew
   the frame; a shape that reuses the existing `sp+212` slot is still open.
2. Once `this`=r4, re-test the faithful raid block: no cached `Status *status`
   (use `Globals::status` directly) and `int guns` after the inner `if`.  Both
   are what the original asm shows; they currently cost 16 B because the
   register pressure is different.
3. The `t` select (`cbz` diamond in the original vs `it ne; movne` in ours) is
   still the reason `t` is r6 rather than sl (w9310 §3).

## Commands

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9411/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9411/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w9411/{fdiff.py,align_diff.py,dis.py,rdiff.py,rdiff2.py,
secdiff.py,measure.sh,try.py,mkvar*.py,sweep_decl.py,Level.base.cpp}`.
