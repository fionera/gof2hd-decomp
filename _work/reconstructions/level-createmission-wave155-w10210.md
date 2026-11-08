# w10210 (wave 155): `Level::createMission` — alien-loop branch weight
# (aligned 774 -> 748, raw 5,487 -> 5,662, size 17,172); `this=r4` still open

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10210`, based on master
`cf74f4ee6` (wave 155). Continues `level-createmission-wave155-w10107.md`,
`…-wave154-w10012.md`, `…-wave153-w9912.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w10210/fdiff.py`; instruction-index alignment is
`/dev/shm/w10210/align_diff.py` (worktree-local copies of the shared tools).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5487   align 774
alien loop: this=fp  gun=r4  i=r4(sp+116)  k=r6  t=r5  temp=r9  rotation=sl  v=sl
raid block: fortressGuns=r4  status VALUE=fp  raidType=sl  raidRace=r6
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 86,044   .ARM.extab 1,292   .rel.plt 0 (byte-identical)
```

## 1. Landed: `__builtin_expect(alienGuns != 0, 1)` + drop the dead early cargo

```diff
-    if (alienGuns != 0) {
+    if (__builtin_expect(alienGuns != 0, 1)) {  // lint: arm_builtin
         ...
             gun->field_0x70 = 0;
-            gun->cargo = nullptr;
             Vector p2 = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
```

This is the w10012 shape (raw 5,662 / aligned 748) re-measured on the wave-155
base.  The early `gun->cargo = nullptr` is dead (DSE'd in HEAD); the faithful
tail store is kept.  The branch weight moves the alien-gun guard's block
layout: `operand:b.n` 116 -> 54, at the cost of more `ldr`/`str` operand
mismatches (`operand:ldr` 115 -> 137, `operand:str` 17 -> 35).

## 2. Register map of the landed alien loop (target vs ours)

```text
        this        i            k    temp      t    gun   rotation/v
ORIG    r4(reload)  fp           r9   r5        sl   r6    r4
OURS    r5          r4(sp+120)   r9   fp        r6   r4    sl
```

`k=r9` now matches.  `this`/`i`/`temp`/`t`/`gun` are still a cyclic permutation
of the original.  The raid prologue is unchanged (`fortressGuns=r4`,
`status VALUE=fp`, `raidType=sl`); the target (`fortressGuns=fp`,
`&Globals::status=r8`, `raidType=r9`) is NOT reached.

## 3. NEW lever found: declaration order hoists `this` into r4

Permuting the four alien-block declarations (`Matrix rotation; Vector v; int k;
Vector base;`) is not free: the order `k, v, base, rotation` (`KVBR`) and
`k, v, rotation, base` (`KVRB`) make clang emit `ldr r4,[sp,#124]` *before* the
rotation init (at `b3242`/`b3234`), matching the original's `ldr r4,[sp,#124]`
at `b3232` (the original hoists the `this` reload above the rotation init; HEAD
loads it at `b3264`, after the init).  But the allocator then moves `this` to
`fp`/`r5` for the loop anyway, and `i` takes r4.  Measurements (all 17,172 B):

| variant | raw | align |
|---|---:|---:|
| **HEAD** | **5487** | **774** |
| `KVBR` (k,v,base,rotation) | 5485 | 795 |
| `KVRB` (k,v,rotation,base) | 5485 | 792 |
| `BKVR` (base,k,v,rotation) | 5486 | 862 |
| `KVBR` + expect | 5683 | 775 |
| `KVRB` + expect | 5681 | 771 |
| `KVBR` + drop early cargo | 6451 | 809 |

`KVBR`/`KVRB` improve raw by 2 but regress aligned by ~20, so they were not
landed.  They are the closest source shape yet to the target `this=r4` hoist.

## 4. Why `this` never stays in r4

The target needs `i` in `fp` (loop-carried) so `this` can keep r4 and be
reloaded once at the loop tail (`b336e`).  In every 17,172-B source shape the
allocator spills `i` (`sp+116`/`sp+120`) and keeps `this` in a callee-saved
register instead.  The chain is: the sret `temp` occupies `fp` (or `r9`), so
`fp` is not free for `i`; `i` then lands in r4 (or is spilled), and `this` is
pushed to `fp`/`r5`.  Getting `temp` into `r5` (as in the original) is the
prerequisite.  The only shape that puts `temp` in r5 is the named-`Vector p`
form, which splits `temp2` to a second slot and regresses raw to ~6,460
(w10107).  No source form found that gives `temp=r5` *and* one shared temp slot.

## 5. Other measurements this wave (all 17,172 B unless noted)

| variant | raw | align |
|---|---:|---:|
| **landed: expect + drop early cargo** | **5662** | **748** |
| expect only | 5683 | 753 |
| expect + self alias | 5683 | 753 |
| expect + rot/k/v reorder | 5683 | 753 |
| expect + k first | 5683 | 753 |
| expect + `Status**` local | 5718 | 824 |
| expect + base before rotation | 5684 | 824 |
| `Status**` / `Status*&` / `volatile Status**` local before alien loop | 5524 | 788 |
| no `status` local (`Globals::status->…` twice) | 5524 | 788 |
| `Array<KIPlayer*> *enemies` local before alien loop | 6481 | 1196 |
| `Level *self = this` alias | 5487 | 774 |
| `int fg = fortressGuns` before alien loop | 5487 | 774 |
| `Level *volatile self` | 7003 | 1859 |
| remove `(i<0)` hack (`bigAlienWave`) | 6592 (17140 B) | 1243 |
| `t` as if/else (any form) | 6592 | 1243 |
| `v` before `rotation` | 5485 | 792 |
| `i,k` declared outside `for` | 11714 | 1569 |
| `++i,++k` | 5487 | 774 |
| `rotation = {}` value-init | 5487 | 774 |

The `Status**`/`Status*&`/`volatile Status**` locals are all rematerialised
(link-time-constant address), so they do not stay live across the alien loop;
they only perturb the raid block (raw 5487 -> 5524, align 774 -> 788).

## 6. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5487 -> 5662   (+175)
                           align  774 -> 748    (-26)
.text diff             86,044 -> 86,219  (+175)
.ARM.extab              1,292 -> 1,292
.rel.plt                    0 -> 0   (byte-identical)
.note.gnu.build-id         20 -> 20
.rodata/.data/.bss/.exidx exact -> exact
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

All 27 section sizes exact; file size exact (2,187,328 B).

## 7. Open questions

1. `temp` must move to `r5` (and keep a single shared slot) so `fp` is free for
   `i`; only then can `this` stay in `r4`.  The named-`Vector p` form splits
   `temp2`, so a source form that names the sret buffer without splitting the
   two rotates is still unknown.
2. The `(i < 0)` ternary hack is still present.  Removing it reproduces the
   original's `cbz` but shrinks the function to 17,140 B (-32); the missing 32 B
   of genuine code is still unidentified (w9812/w9912 open question).
3. The `switch (k)` block order (case1 before case2, default before common) is
   PGO-driven and invariant to source order; it is the source of the large
   shifted region starting at `0xb1418`.
4. `KVBR`/`KVRB` hoist the `this` reload into r4 before the rotation init
   (matching the original's `b3232`), but the loop body still moves `this` to
   `fp`/`r5`.  Combining that hoist with a `temp=r5` shape may be the path.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10210/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10210/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w10210/{fdiff.py,align_diff.py,regmap.py,measure.sh,sweep.py,
try.py,blocks.py,v1..v8.py,Level.base.cpp}`.
