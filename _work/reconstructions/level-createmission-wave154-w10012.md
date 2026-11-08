# w10012 (wave 154): `Level::createMission` — branch-weight hint on the
# alien-gun guard: aligned 774 -> 748 at 17,172 B (raw 5,487 -> 5,662)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10012`, based on master
`4417db0b4` (wave 153). Continues `level-createmission-wave153-w9912.md`,
`…-wave152-w9812.md`, `…-wave151-w9711.md`, `…-wave148-w9509.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w10012/fdiff.py`; instruction-index alignment is
`/dev/shm/w10012/align_diff.py`; section diff is `/dev/shm/w10012/secdiff.py`.

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5487   align 774
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 86,623   .ARM.extab 1,265   .rel.plt 0   .note.gnu.build-id 20
```

## 1. Mission hypotheses (both rejected)

The coordinator suggested (a) a `Status **` / `Status *&` local that survives
the alien loop, and (b) a `fortressGuns` local used in both the raid and alien
blocks.  Both were measured at the mandatory 17,172 B:

| variant | size | raw | align |
|---|---:|---:|---:|
| **HEAD** | 17172 | **5487** | **774** |
| `Status **statusp = &Globals::status;` before alien loop, `(*statusp)->` in raid | 17172 | 5524 | 788 |
| `Status *&status = Globals::status;` before alien loop, `status->` in raid | 17172 | 5524 | 788 |
| `Status *status = Globals::status;` before alien loop (value cached) | 17172 | 16002 | 3221 |
| `int guns = alienGuns + fortressGuns;` hoisted before alien loop, used in raid+tail | 17172 | 7452 | 1792 |
| same, alien loop bound written `guns - fortressGuns` | 17172 | 7452 | 1792 |
| `fortressGuns` declared before `alienGuns` | 17172 | 5487 | 774 |
| `Level *self = this;` + `self->` in alien loop | 17172 | 5487 | 774 |
| cache `this->enemies` in a local + use it in the alien loop | 17172 | 6377 | 1206 |

The `Status **` local is rematerialised (it is a link-time constant address), so
it does **not** actually stay live across the alien loop; the alien-loop body is
byte-identical to HEAD.  Hoisting `guns` across the loop is catastrophic.
`Level *self = this` is canonicalised away by the optimiser.  None of these move
`this` to r4 in the alien loop.

## 2. New finding: branch weight on the alien-gun guard

The original was built with sample-profile (PGO-like) branch weights.  Adding a
branch-weight hint to the alien-gun guard is the first thing this wave that
changes the alien-loop colouring:

```cpp
if (__builtin_expect(alienGuns != 0, 1)) {  // lint: arm_builtin
```

On its own it keeps 17,172 B and gives raw 5,683 / align 753.  Removing the
redundant *early* `gun->cargo = nullptr;` store (the faithful final store at the
loop tail is kept) takes it to **raw 5,662 / align 748** at 17,172 B.  The
aligned gain is concentrated in branch-target alignment
(`operand:b.n` 116 -> 54); it is partly offset by more `ldr`/`str` operand
mismatches (`operand:ldr` 115 -> 137, `operand:str` 17 -> 35).

The alien-loop map moves one step toward the original:

```text
        this  gun  i        k*24  t     temp  rotation/v
ORIG    r4    r6   fp       r9    sl    r5    r4
HEAD    fp    r4   [sp,#116] r6    r5    r9    sl
NEW     r5    r4   [sp,#120] r9    r6    fp    sl
```

`k*24` now matches the original's r9, but `this`/`gun`/`i`/`t`/`temp` are still
permuted; the raid prologue is unchanged (`ldr r4,[sp,#48]` fortressGuns,
status value in fp).

## 3. Landed change (`src/game/world/Level.cpp`, alien-gun block)

```diff
-    if (alienGuns != 0) {
+    if (__builtin_expect(alienGuns != 0, 1)) {  // lint: arm_builtin
         ...
             gun->geometry->setRotation(v);
             gun->field_0x70 = 0;
-            gun->cargo = nullptr;
             Vector p2 = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
```

The `// lint: arm_builtin` same-line waiver keeps the `arm_builtin` soft count
at 1 for `Level.cpp` (the existing `__builtin_memcpy`).

## 4. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5487 -> 5662   (+175)
                           align  774 -> 748    (-26)
.text diff             86,623 -> 86,798  (+175)
.ARM.extab              1,265 -> 1,265
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

All 27 section sizes remain exact; file size exact.  Docs updated in the same
commit (`.text` 86,798; createMission raw 5,662 / aligned 748; aligned total
23,952).

## 5. Other measurements this wave (all 17,172 B unless noted)

| variant | raw | align |
|---|---:|---:|
| **landed: `expect(alienGuns!=0,1)` + drop early cargo** | **5662** | **748** |
| `expect(alienGuns!=0,1)` only | 5683 | 753 |
| `expect(alienGuns!=0,1)` + `field_0x70` after `setScaling` | 5673 | 754 |
| `expect(alienGuns!=0,1)` + `field_0x70` before `setScaling` | 5679 | 755 |
| `expect(alienGuns!=0,1)` + drop `field_0x70` | 5676 | 819 |
| `expect(alienGuns!=0,1)` + drop cargo + drop `field_0x70` | 17180 | 5829 |
| `expect(alienGuns!=0,0)` | 14801 | 1562 |
| `expect(alienGuns==0,0)` | 14705 | 1567 |
| `expect(pirateFortress,1)` | 6465 | 1219 |
| `expect(raid,1)` | 14837 | 1737 |
| `expect(raid,0)` | 11377 | 1346 |
| drop `field_0x70` only | 17164 (8 B short) | 713 |
| drop early cargo only | 17164 (8 B short) | 788 |
| `field_0x70` after `setScaling` only | 5492 | 775 |
| remove `(i < 0)` hack | 6592 | 1243 |
| `switch (k)` default store order `offset[0],offset[2],offset[1]` | 5493 | 774 |
| same, `offset[0],offset[1],offset[2]` | 17164 | 11348 |
| same, `offset[1],offset[0],offset[2]` | 17188 | 11335 |
| `switch (k)` rewritten as `if/else if` (any weights) | 10535+ | 939+ |

The `switch (k)` block-order difference (ours emits case0, case2, case1,
common, default; the original emits case0, case1, case2, default, common) is
invariant to source case order and to `if/else` rewriting, and is the source of
the large shifted region starting at `0xb1418`.  The `if/else` rewrite is much
worse, so the `switch` form was kept.

## 6. Open questions

1. The raid prologue still needs `this`=r4 in the alien loop, i.e. `i`=fp.  The
   branch-weight hint moves `k*24` to r9 but not `this`/`i`.  A weight that
   makes `i` (the loop-carried index) outrank `this` is still unknown.
2. `nof70` (drop `field_0x70`) reaches aligned 713 but is 8 B short (17,164);
   the missing 8 B of genuine code is not identified.  If found, it would
   combine with the branch hint.
3. The `switch (k)` block order (case1 before case2, default before common)
   looks PGO-driven; source order and `if/else` do not reproduce it.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10012/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10012/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10012/secdiff.py
```

Scratch: `/dev/shm/w10012/{fdiff.py,align_diff.py,secdiff.py,regions.py,adiff.py,
dump.sh,try.py,run.sh,sweep*.py,Level.base.cpp}`.
