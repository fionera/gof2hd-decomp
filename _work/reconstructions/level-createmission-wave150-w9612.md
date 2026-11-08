# w9612 (wave 150): `Level::createMission` — full raid-block permutation
# sweep on top of w9509's `Vector p`; the raid prologue still needs
# `this`=r4, and the `Globals::status` local that "forces r8" is the
# *value* not the address. Landed the faithful `R H [if] G` statement
# order (identical codegen, raw 5495 / align 731).

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9612`, based on master
`7eed2966e` (wave 149). Continues `level-createmission-wave148-w9509.md`,
`…-wave148-w9508.md`, `…-wave148-w9411.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is
`/dev/shm/w9612/fdiff.py`; instruction-index alignment is
`/dev/shm/w9612/align_diff.py` (worktree-local copies of the shared tools).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5495   align 731
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text 86,449   .ARM.extab 1,309   .rel.plt 4   .note.gnu.build-id 20
```

Region table (function-relative, unchanged from w9509):

| region | bytes | diff |
|---|---:|---:|
| `0x0000..0x32c2` pre-alien | 12994 | 2587 |
| `0x32c2..0x33bc` alien-gun loop | 250 | 230 |
| `0x33bc..0x3426` alien tail | 106 | 103 |
| `0x3426..0x3504` pirate-fortress loop | 222 | 199 |
| `0x3504..0x377a` pirate tail | 630 | 27 |
| `0x377a..0x3994` raid block | 538 | 484 |
| `0x3994..0x4314` tail | 2432 | 1865 |
| **total** | | **5495** |

## 1. Why the prologue is not a raid-block source-order problem

The original raid prologue (`b3752`):

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
b3770  ldr.w r0, [r8]          ; deref &Globals::status AFTER the call
b3774  blx   hardCoreMode
```

Ours (HEAD):

```text
b3752  ldr   r4, [sp,#48]    ; fortressGuns -> r4 (r4 is free; this=r5)
b3754  ldr   r3, [sp,#88]    ; raid
b3756  cmp   r3, #0
b3758  beq.w b3964
b375c  ldr   r0, [sp,#108]   ; &Globals::status
b375e  ldr   r6, [sp,#68]    ; raidRace
b3760  ldr.w r8, [r0]        ; status VALUE -> r8 (cached local)
b3764  ldr   r0, [sp,#120]
...
b376a  blx   getRandomEnemyFighter
b376e  mov   sl, r0            ; raidType
b3770  mov   r0, r8
b3772  blx   hardCoreMode
```

The decisive structural difference is **what is live across the
`getRandomEnemyFighter` call**:

* original: `&Globals::status` (a stack address) is live across the call, so
  it must sit in a callee-saved register → **r8**, and the deref happens after
  the call (`ldr.w r0,[r8]`);
* ours: `Globals::status` (the value) is live across the call → r8, and the
  address is dead before the call (loaded into caller-saved r0).

So "a `Globals::status` local that forces r8" already exists — it is HEAD's
`Status *status = Globals::status;`, but it forces r8 to hold the **value**.
The prologue's `ldr.w r8,[sp,#108]` needs the **address** in r8, which is a
downstream consequence of `this`=r4 in the alien loop (r4 busy ⇒ fortressGuns
must go to fp ⇒ &status goes to r8). w9508 proved `this`=r4 is not reachable
from source at 17,172 B.

## 2. Full raid-block permutation sweep (on top of `Vector p`)

`S` = `Status *status = Globals::status;`, `G` = `int guns = alienGuns +
fortressGuns;`, `R` = `int raidType = …getRandomEnemyFighter(raidRace);`,
`H` = `bool hard = status->hardCoreMode();`. `[if]` is the
`if (raidRace == 8 && … getLevel())` block. All keep size 17,172 B.

All 12 permutations with `S` before `H` (the only ones that compile the
`hard` use):

| variant | raw | align | alien `this` |
|---|---:|---:|---|
| **`S G R H` [if] (HEAD)** | **5495** | **731** | r6 |
| `S R G H` [if] | 5495 | 731 | r6 |
| `S R H` [if] `G` | 5495 | 731 | r6 |
| `G S R H` [if] | 5495 | 731 | r6 |
| `G R S H` [if] | 5499 | 728 | r6 |
| `R G S H` [if] | 5499 | 728 | r6 |
| `R S G H` [if] | 5499 | 728 | r6 |
| `R S H` [if] `G` | 5499 | 728 | r6 |
| `G S H R` [if] | 5503 | **725** | r6 |
| `S G H R` [if] | 5503 | **725** | r6 |
| `S H G R` [if] | 5503 | **725** | r6 |
| `S H R G` [if] | 5503 | **725** | r6 |
| `R H` [if] `G` (faithful, no `S`) | 6472 | 1222 | r6 |
| `S R H` [if] `G` (faithful + `S`) | 6474 | 1215 | r6 |
| `S H R` [if] `G` | 6466 | 1231 | r6 |
| `R H G` [if] (no `S`, `G` top) | 5524 | 799 | r6 |
| `G R H` [if] (no `S`) | 5524 | 799 | r6 |

`__builtin_expect` on the raid/inner/loot branches is a large regression
(`expect(raid,1)` raw 14826 / align 1747; `expect(raid,0)` 11371 / 1381;
`expect(inPirateLootOrbit,*)` 14698/11328).

**No raid-block ordering produces the target prologue.** The prologue is
unchanged in every variant (`ldr r4,[sp,#48]` … `ldr r0,[sp,#108]` after the
branch). The only metric movement is in the raid block's own instruction
order, and it trades raw against align:

* best raw: HEAD `S G R H` = 5495 / 731;
* best align: `G S H R` / `S G H R` / `S H G R` / `S H R G` = 5503 / 725
  (+8 raw, −6 aligned). This is *unfaithful* (`H` before `R`; the original
  calls `getRandomEnemyFighter` first), and the aligned win is a difflib
  artefact (`mov r9,r0` after `hardCoreMode` happens to align with the
  original's `mov r9,r0` after `getRandomEnemyFighter`), so it was not landed.

## 3. `Globals::status` local variants (all keep 17,172 B)

| variant | raw | align |
|---|---:|---:|
| `Status *status = Globals::status;` (HEAD) | **5495** | **731** |
| `Status **statusp = &Globals::status;` (`(*statusp)->…`) | 5524 | 799 |
| `Status *&status = Globals::status;` | 5524 | 799 |
| `Status *const status` / `auto status` | 5495 | 731 |
| `Status *status` before `if (raid)` | 5495 | 731 |
| `Status *status` before the alien loop | 5495 | 731 |
| `Status *status` before the pirate loop | 5495 | 731 |
| `Status *status` before alien + reused in raid | 17300 | 2072 |
| `hard = Globals::status->hardCoreMode()` (local only for `getLevel`) | 6263 | 1226 |
| `getLevel` via `Globals::status` (local only for `hard`) | 5553 | 902 |
| `inPirateLootOrbit` via the local | 6481 | 1230 |

The address-of forms (`Status **`) do **not** hoist the address above the
call: the compiler rematerialises `ldr r0,[sp,#108]` after the branch and
derefs immediately, exactly like the no-local form. The cached-value local is
the only form that keeps a callee-saved register live across the call, and it
is the raw winner.

## 4. Alien-loop shapes that change the `this` register

The raid block cannot move `this`; only the alien/pirate loop shapes can.
Measured `this` (the register in `mov r0,rX` before the alien
`createStaticObject`):

| alien-loop shape | size | raw | align | `this` |
|---|---:|---:|---:|---|
| HEAD (`Vector p` in both loops, `M v k b`) | 17172 | **5495** | **731** | r6 |
| `v M k b` (v before rotation) | 17172 | 5493 | 750 | r6 |
| `M b v k` | 17172 | 5494 | 743 | r6 |
| no `Vector p` (w9509's pre-pass shape) | 17156 | 6452 | 797 | fp |
| `KIPlayer *gun` / `t` reorders | 17172 | 5495–5503 | 731–733 | r6 |
| `setScaling` after `setPosition` (`Dlast`) | 17172 | 5821 | 806 | r6 |
| cache `this->enemies` in a local | 17172 | 6527 | 1221 | fp |
| `__builtin_expect(raid,1)` on `if (raid)` | 17172 | 14826 | 1747 | **r5** |
| `__builtin_expect(pirateFortress,1)` | 17172 | 12870 | 3275 | fp |
| `__builtin_expect(alienGuns!=0,0)` | 17172 | 14707 | 1557 | fp |

No source shape tried reaches `this`=r4. The original's alien loop uses six
callee-saved registers (`r4`=this/rotation/v, `r5`=&p, `r6`=gun, `r9`=k,
`sl`=t, `fp`=i); ours uses seven (`r4`=gun, `r5`=t, `r6`=this, `r8`=k,
`r9`=i, `sl`=v/rotation, `fp`=&p). The extra register is `this`: in the
original it is dead between `b32b6` and `b336e` and shares `r4` with
`rotation`/`v`; ours keeps it live across the whole body. Caching
`this->enemies` shortens the range but moves `this` to `fp`, not `r4`.

## 5. Merge test with w9611 (both orders)

w9611's current candidate (uncommitted at the time of writing) removes the
`Vector p` temp and moves `setScaling` after `setPosition`. Applied on top of
this tree (source lines are disjoint, so both merge orders produce the same
source and the same code):

| combination | size | raw | align |
|---|---:|---:|---:|
| w9611 `Dlast` alone | 17172 | 5657 | 714 |
| w9611 `Dlast` + this raid order (`S R H G`) | 17172 | 5661 | 711 |
| w9611 `Dlast` + `G S H R` | 17172 | 5665 | 708 |
| w9611 `pb` variant alone | 17156 | 6452 | 797 |
| w9611 `pb` + this raid order | 17156 | 6451 | 794 |

The two changes are additive: the raid reorder costs the same +4 raw / −3
aligned on top of w9611 as on HEAD. w9611's shape trades a large raw
regression for a large aligned win; neither combination reaches HEAD's raw
5495.

## 6. Landed change (`src/game/world/Level.cpp`)

Move `int guns = alienGuns + fortressGuns;` from the top of the raid block to
after the inner `if` — the original's witnessed order (`R H [if] G`,
`add.w r5,sl,fp` at `b37e6`) — keeping the cached `Status *status` local
(which is the raw winner):

```diff
     if (raid) {
         Status *status = Globals::status;
-        int guns = alienGuns + fortressGuns;
         int raidType = Globals::globals->getRandomEnemyFighter(raidRace);
         bool hard = status->hardCoreMode();
+        int guns = alienGuns + fortressGuns;
         if (raidRace == 8 && Level::initStreamOutPosition &&
```

This is a **source-faithfulness** change only: the emitted
`_ZN5Level13createMissionEv` bytes are **bit-identical** to HEAD (verified by
extracting the function from both `.so`s; the only ELF difference is the
20-byte `.note.gnu.build-id`).

## 7. Gate counts (before -> after)

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
.text diff      86,449 -> 86,449
.ARM.extab      1,309  -> 1,309
.rel.plt        4      -> 4
.note.gnu.build-id 20  -> 20
_ZN5Level13createMissionEv raw 5495 -> 5495, align 731 -> 731, size 17172
```

All 27 section sizes remain exact; file size exact.

## 8. Open questions

1. The prologue still needs `this`=r4. The alien loop's `this` interval must
   acquire a hole (dead between the `this->enemies` accesses and the loop
   bound) so it can share `r4` with `rotation`/`v`. Caching `this->enemies`
   does not create the hole in the right place (it moves `this` to `fp`).
2. The `Status *status` local is a *value* cache and is the raw winner
   (5495 vs 5524 for the faithful no-local form). Any future attempt to
   restore the faithful no-local form must first fix `this`=r4, otherwise it
   costs 977 raw bytes.
3. The `G S H R`-family align win (725) is a difflib artefact of the
   `mov r9,r0` position and is not worth its +8 raw bytes.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9612/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9612/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9612/region.py _ZN5Level13createMissionEv
python3 /dev/shm/w9612/map.py
```

Scratch: `/dev/shm/w9612/{fdiff.py,align_diff.py,region.py,map.py,sbs.py,
gen.py,gen2.py,gen3.py,raid_sweep.py,raid_nos.py,raid_exp.py,raid_misc.py,
raid_x.py,alien_sweep.py,alien2.py,alien3.py,combo.py,combo2.py,combo3.py,
one.sh,measure.sh,Level.base.cpp,mi.log}`.
