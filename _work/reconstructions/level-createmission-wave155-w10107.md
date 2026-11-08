# w10107 (wave 155): `Level::createMission` — temp=r5 landed at 17,172 B
# (raw 5,487 -> 6,449, aligned 774 -> 1,196); `GOF2_GOLD_PAD_CAP` wired

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10107`, based on master
`f4e0ea40b` (wave 154). Continues `level-createmission-wave154-w10011.md`,
`…-wave154-w10012.md`, `…-wave153-w9912.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w10107/fdiff.py`; instruction-index alignment is
`/dev/shm/w10107/align_diff.py` (worktree-local copies of the shared tools).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5487   align 774
alien loop: temp1=r9(sp+212) temp2=r9(sp+212)  this=fp  gun=r4  i=[sp+116]
            k=r6  t=r5  rotation=sl  v=sl
target:     temp1=r5(sp+212) temp2=r5(sp+212)  this=r4  gun=r6  i=fp
            k=r9  t=sl  rotation=r4  v=r4
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 86,347   .ARM.extab 1,255   .rel.plt 0 (byte-identical)
```

## 1. Landed: temp1=r5 (w10011 shape) + `volatile` status pointer local

The w10011 shape is the only 17,172-B source that moves the first sret temp to
r5: name the first rotate `Vector p`, copy `v = p`, and drop the (non-original)
`gun->field_0x70 = 0;` store.  The original alien loop has no `field_0x70`
store and exactly one `cargo = nullptr` (the early one is DSE'd).

```diff
-            v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            Vector p = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            v = p;
             gun->geometry->setRotation(v);
-            gun->field_0x70 = 0;
             gun->cargo = nullptr;
```

On top of that, the raid-block status read becomes a volatile pointer local
(`Status *volatile status = Globals::status;`).  It keeps 17,172 B and raw
6,449, and improves the aligned metric 1,210 -> **1,196** (-14).  This is the
best 17,172-B raid-prologue form found; the mission's `Status **` / `Status *&`
forms lose 8 B (17,164).

```diff
-        Status *status = *(Status *volatile *) &Globals::status;
+        Status *volatile status = Globals::status;
```

## 2. Both orders (merge lesson)

The two edits are independent and commute in the final source, but the
intermediate measurements differ:

| order | shape | size | raw | align |
|---|---|---:|---:|---:|
| temp=r5 first | w10011 only | 17172 | 6449 | 1210 |
| raid lead first | baseline + `volatile` status local | 17176 | 5532 | 806 |
| **both (either order)** | **w10011 + volatile status local** | **17172** | **6449** | **1196** |

The raid lead alone grows `.text` to `0x1337e8` (gold stub-table pad +0x20);
the combined shape lands back at `0x1337c8` (no pad growth), so the default
gold cap `0x40` is correct and no patch is needed for this shape.  The
`GOF2_GOLD_PAD_CAP` plumbing is still wired (see §3) so a future shape that
does cross the boundary can be matched without a second link pass.

## 3. `GOF2_GOLD_PAD_CAP` wired into `CMakeLists.txt`

`tools/patch_gold_pad.py` (wave 152) rewrites gold's hard-coded ARM stub-table
padding cap (`min(delta*0.02, 0x40)`).  It is now a cache variable:

```cmake
set(GOF2_GOLD_PAD_CAP "0x40" CACHE STRING "gold ARM stub-table auto-padding cap (hex)")
```

At the default `0x40` the stock `${GOF2_NDK_GOLD}` is used unchanged (no copy,
no patch, byte-identical link).  Any other value copies gold into the build
dir, patches it, and links with the copy.  Verified: `-DGOF2_GOLD_PAD_CAP=0x20`
links the same objects at `.text 0x1337a8` (the +0x20 pad removed); the default
build is unaffected (`0x1337c8`).

## 4. Register map of the landed alien loop

```text
        temp1       temp2       this        gun   i          k     t     rotation    v
ORIG    r5(sp+212)  r5(sp+212)  r4(reload)  r6    fp         r9    sl    r4(sp+312)  r4(sp+372)
OURS    r5(sp+212)  r8(sp+188)  r5(reload)  r4    r4/sp+116  r9    r6    r8(sp+312)  fp(sp+372)
```

`temp1` and `k*24` match the original.  `temp2` is still a second slot because
the named `p`/`p2` pair splits stack colouring; the original uses one `sp+212`
slot for both rotates.  `this`/`gun`/`i`/`t` remain one rotation off.

## 5. Raid prologue of the landed shape (offset b34e8; layout shifted -0x26a)

```text
b34e8  ldr   r3, [sp,#88]    ; raid
b34ea  cmp   r3, #0
b34ec  beq.w b3954
b34f0  ldr   r0, [sp,#108]   ; &Globals::status
b34f2  ldr   r1, [r0]        ; status VALUE (hoisted)
b34f4  ldr   r0, [sp,#120]   ; &Globals::globals
b34f6  ldr   r0, [r0]
b34f8  str   r1, [sp,#224]   ; spill status value
b34fa  ldr.w r8, [sp,#68]    ; raidRace
b34fe  mov   r1, r8
b3500  blx   getRandomEnemyFighter
b3504  str   r0, [sp,#112]   ; spill raidType
b3506  ldr   r0, [sp,#224]   ; reload status value
b3508  blx   hardCoreMode
```

The target still differs: the original loads `&Globals::status` into r8 at
`b3752` and derefs it after `getRandomEnemyFighter` (`b3770`), and loads
`fortressGuns` into fp at `b3758`.  The volatile local hoists the value instead
and spills it.  The `Status **` / `Status *&` forms that survive the alien loop
get closer to the original raid block but lose 8 B (17,164).

## 6. Other measurements this wave (all 17,172 B unless noted)

| variant | raw | align | size |
|---|---:|---:|---:|
| **landed: w10011 + volatile status local** | **6449** | **1196** | **17172** |
| w10011 only (temp=r5) | 6449 | 1210 | 17172 |
| w10011 + `Status *status = Globals::status` before alien loop, `Globals::status->getLevel()` in raid | 6450 | 1196 | 17172 |
| w10011 + `Status **statusp` before alien loop | 6457 | 1198 | 17164 |
| w10011 + `Status *&status` before alien loop | 6457 | 1198 | 17164 |
| w10011 + `int fg = fortressGuns` before alien loop (raid and pirate blocks) | 6449 | 1210 | 17172 |
| w10011 + `Status *status = *(Status *volatile *)&Globals::status` before alien loop | 6518 | 903 | 17184 |
| w10011 + `__builtin_expect(alienGuns != 0, 1)` | 6469 | 1244 | 17172 |
| w10011 + `__builtin_expect(bigAlienWave + (i < 0), 0)` | 6449 | 1210 | 17172 |
| baseline + volatile status local (raid lead alone) | 5532 | 806 | 17176 |
| baseline + `__builtin_expect(alienGuns != 0, 1)` + drop early cargo (w10012) | 5662 | 748 | 17172 |
| both unnamed rotates, no `field_0x70` | 6440 | 890 | 17164 |
| both unnamed rotates, no `field_0x70`, `expect(alienGuns!=0,1)` | 5498 | 817 | 17172 |
| `Vector p` first, second rotate unnamed, no `field_0x70` | 5812 | 823 | 17180 |
| two scoped `Vector p` blocks, no `field_0x70` | 6440 | 890 | 17164 |
| two scoped `Vector p` blocks, no `field_0x70`, `expect(alienGuns!=0,1)` | 5498 | 817 | 17172 |
| reuse one `Vector p` for both rotates | 16587 | 3110 | 17312 |

The `pre volval` shape (17,184 B, align 903, `.text 0x1337e8`) is the best
aligned result but is 12 B over the original size; it is the shape the
`GOF2_GOLD_PAD_CAP` knob was wired for.  Landing it would need the missing
12 B removed first.

## 7. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5487 -> 6449   (+962)
                           align  774 -> 1196   (+422)
.text diff             86,347 -> 87,309  (+962)
.ARM.extab              1,255 -> 1,267   (+12)
.ARM.exidx                  0 -> 0
.rel.plt                    0 -> 0   (byte-identical, 29,392 B)
.note.gnu.build-id         20 -> 20
.rodata/.data/.bss/.got    0 -> 0
dynsym/dynstr/hash/rel.dyn 0 -> 0
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

All 27 named section sizes exact; file size exact (2,187,328 B).

## 8. Open questions

1. `temp2` must reuse `temp1`'s `sp+212` slot.  The two-scoped-`p` and
   both-unnamed shapes do reuse one slot (temp r9/r9, 17,164 B) but do not put
   it in r5.  The named-`p` shape puts temp1 in r5 but splits temp2 to
   `sp+188`/r8.  A source form that both forces r5 and keeps one slot is still
   unknown.
2. The `(i < 0)` ternary hack is still present.  Removing it gives the
   original's `cbz` but shrinks the function to ~17,140 B; the missing 32 B of
   genuine code is still unidentified (w9812/w9912 open question).
3. `pre volval` (status value loaded before the alien loop) reaches aligned 903
   but is 17,184 B (12 B over).  If the 12 B (volatile load + spill + reload)
   can be removed, it would be the best structural shape by a wide margin.
4. The raid prologue target (`&Globals::status` in r8, `fortressGuns` in fp)
   needs r8 free across the alien loop, i.e. temp2 back in r5.  See (1).
