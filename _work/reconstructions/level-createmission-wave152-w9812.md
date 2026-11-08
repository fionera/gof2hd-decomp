# w9812 (wave 152): `Level::createMission` — volatile status read hoists the
# value load before `getRandomEnemyFighter`: raw 5657 -> 5649, align 714 -> 711

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9812`, based on master
`9d491d46a` (wave 151). Continues `level-createmission-wave150-w9712.md`,
`…-wave151-w9711.md`, `…-wave148-w9509.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w9812/fdiff.py`; instruction-index alignment is
`/dev/shm/w9812/align_diff.py` (local copies of the shared tools).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5657   align 714
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 87,271
```

## 1. Target raid prologue (original) — unchanged from w9712

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

Ours (HEAD) caches the `Status*` value in `fp` *after* the branch and reloads
`fortressGuns` into `r4`; the whole raid block is a register permutation
(`this` r5 vs r4, `guns` r4 vs r5, `raidType` sl vs r9, `alienGuns` r8 vs sl).

## 2. Volatile read of the static pointer (landed)

`Status *status = *(Status *volatile *) &Globals::status;` makes the read of
the static pointer a volatile load.  The compiler may no longer sink the load
past the branch, so it hoists the `Globals::status` value load to *before*
`getRandomEnemyFighter` (exactly the original's deref position relative to the
call, just into `fp` instead of through `r8`):

```text
b3752  ldr   r4, [sp,#48]    ; fortressGuns
b3754  ldr   r3, [sp,#88]    ; raid
b3756  cmp   r3, #0
b3758  beq.w b3964
b375c  ldr   r0, [sp,#108]   ; &Globals::status
b375e  ldr.w fp, [r0]        ; status value, HOISTED before the call
b3762  ldr   r0, [sp,#120]
...
b376a  blx   getRandomEnemyFighter
b376e  mov   sl, r0
b3770  mov   r0, fp
b3772  blx   hardCoreMode
```

This is a pure source-order change (no statement added/removed): the
instruction count and the 17,172 B size are unchanged.  The `.text` diff drops
by exactly 8 bytes, all inside the raid block (`0x377a..0x3994`: 484 -> 476).

## 3. Variants measured this wave (all keep 17,172 B unless noted)

`S` = `Status *status = Globals::status;`, `V` = the landed volatile read.

| variant | size | raw | align |
|---|---:|---:|---:|
| **HEAD `S`** | 17172 | 5657 | 714 |
| **`V` (landed)** | **17172** | **5649** | **711** |
| `Status *volatile status = Globals::status` | 17176 | 5699 | 777 |
| `Status **statusp = &Globals::status` (`(*statusp)->`) | 17172 | 5686 | 782 |
| `Status **volatile statusp = &Globals::status` | 17176 | 5691 | 790 |
| `Status *&status = Globals::status` | 17172 | 5686 | 782 |
| `Status *volatile &status = Globals::status` | 17172 | 5686 | 782 |
| `V` + `Level *self = this` (raid block) | 17172 | 5649 | 711 |
| `V` + `Level &self = *this` | 17172 | 5649 | 711 |
| `V` + `guns` after the `raidRace==8` if | 17172 | 6455 | 1207 |
| `V` + alien `cargo`/`shipGroup` swap | 17172 | 5652 | 709 |
| `V` + `Status **statusp` + volatile deref after call | 17172 | 5661 | 711 |
| `V` + `int fg = *(volatile int*)&fortressGuns` | 17172 | 8308 | 1622 |
| `volatile int alienGuns` | 17172 | 13569 | 1653 |
| `volatile int fortressGuns` | 17164 | 13274 | 1601 |
| opaque helper call (`volatile` fn-ptr returning `&Globals::status`) | 17184 | 5438 | 889 |
| `p-p2` alien-loop temps + `V` | 17164 | 6468 | 1198 |

The opaque-helper call is the only variant that lowers raw more (5,438) but it
adds 12 B of code (size gate) and *worsens* the aligned metric (889), so it is
not landable.  `volatile` on `alienGuns`/`fortressGuns` forces a stack reload
per use and is catastrophic.

## 4. Why the full target prologue is still out of reach

The remaining prologue differences are a single register permutation driven by
the alien-gun loop:

* ORIG alien loop: `this`=r4, `gun`=r6, `temp`=r5, `k`=r9, `t`=sl, `i`=fp.
* OURS alien loop: `this`=fp, `gun`=r4, `temp`=r6, `k`=r8, `t`=r5, `i`=r9.

Because ours keeps `this` in `fp` (r4 is taken by `gun`), the raid block's
`this` is r5 and `r8` still holds `alienGuns` at the branch, so `&Globals::status`
is rematerialised into caller-saved `r0` instead of `r8`.  The volatile read
cannot change that: it only moves the *value* load, not the address register.
A source shape that puts `this` in r4 in the alien loop (w9711's `p-p2` is the
closest, temp=r5/i=fp correct but still this=r5/gun=r4 and 8 B short) would
cascade the whole raid block into place.

## 5. Landed change (`src/game/world/Level.cpp`, raid block)

```diff
     if (raid) {
-        Status *status = Globals::status;
+        Status *status = *(Status *volatile *) &Globals::status;
         int raidType = Globals::globals->getRandomEnemyFighter(raidRace);
```

## 6. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5657 -> 5649   (-8)
                           align  714 -> 711    (-3)
.text diff             87,271 -> 87,263  (-8)
.ARM.extab              1,327 -> 1,327
.rel.plt                    0 -> 0   (byte-identical)
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

## 7. Open questions

1. The alien loop's `this`=r4 / `gun`=r6 colouring is the root of the raid-block
   permutation.  `p-p2` reaches temp=r5 and i=fp but is 8 B short and still
   this=r5/gun=r4.  Find the genuine 8 B alien-loop statement and a shape that
   rotates `this`/`gun`/`k`/`t` one step further.
2. The original reloads `alienGuns` from sp+52 into `sl` at the raid block;
   ours keeps it in r8.  If `alienGuns` can be moved to `sl`, r8 frees up and
   the volatile read's address may land in r8 (the target `ldr.w r8,[sp,#108]`).
3. The opaque-helper call proves the address *can* be forced into a register
   (raw 5,438) but costs 12 B.  A zero-cost opaque form (e.g. an existing
   external call that returns the address) would be the ideal landing.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9812/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9812/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9812/regions.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w9812/{fdiff.py,align_diff.py,regions.py,sbs.py,sbs2.py,
clusters.py,secdiff.py,m.sh,try*.py,Level.base.cpp,Level.ll,orig.txt,ours.txt}`.
