# w10409 (wave 158): `Level::createMission` — temp=r5 + branch weight + raid-decl
# companion (raw 5,487 -> 6,466, aligned 774 -> 1,230, size 17,172)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10409`, based on master
`9e4795206` (wave 157). Continues `level-createmission-wave157-w10309.md`,
`…-wave156-w10310.md`, `…-wave156-w10209.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w10409/fdiff.py`; instruction-index alignment is
`/dev/shm/w10409/align_diff.py` (worktree-local copies with `OURS` pointing at this
worktree's `cmake-build-match/libgof2hdaa.so`).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5487   align 774
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. Landed source (`src/game/world/Level.cpp`)

Alien-gun block (the w10309 temp=r5 shape):

```diff
-    if (alienGuns != 0) {
+    if (__builtin_expect(alienGuns != 0, 1)) { // lint: arm_builtin
...
-            v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            Vector p = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            v = p;
             gun->geometry->setRotation(v);
-            gun->field_0x70 = 0;
             gun->cargo = nullptr;
```

Raid block companion (new this wave — declaration order `guns, status, hard, raidType`):

```diff
-        Status *status = *(Status *volatile *) &Globals::status;
-        int raidType = Globals::globals->getRandomEnemyFighter(raidRace);
-        bool hard = status->hardCoreMode();
         int guns = alienGuns + fortressGuns;
+        Status *volatile status = Globals::status;
+        bool hard = status->hardCoreMode();
+        int raidType = Globals::globals->getRandomEnemyFighter(raidRace);
```

The volatile-status read is the w10309 companion (aligned −14). The raid-block
declaration reorder is the only 17,172-B companion found that improves on the
w10309 landed shape: raw 6,469 -> 6,466, aligned unchanged at 1,230. The early
`gun->cargo = nullptr;` is kept (removing it drops the function to 17,164 B).

## 2. Register map of the landed alien loop

```text
        temp1        temp2        this         gun   i          k*24  t     v           t[k][1]
ORIG    r5(sp+212)   r5(sp+212)   r4(reload)   r6    fp         r9    sl    r4(sp+372)  r8
OURS    r5(sp+212)   r8(sp+200)   r5(reload)   r4    [sp+120]   r9    r6    fp(sp+372)  sl
```

`temp1` (r5, `sp+212`) and `k*24` (r9) match the original. `temp2` is still a
second slot (`sp+200`, r8) because the named `p`/`p2` pair splits stack
colouring; the original reuses `sp+212` for both rotates and the `setPosition`
vector. `this`/`gun`/`i`/`t`/`v` remain one cyclic permutation off.

## 3. Companion sweep (all 17,172 B unless noted)

Every measurement is on top of the temp=r5 + expect + volatile shape; raw / align / size.

| variant | raw | align | size |
|---|---:|---:|---:|
| **landed: temp=r5 + expect + volatile + raid decl `guns,status,hard,raidType`** | **6466** | **1230** | **17172** |
| w10309 landed: temp=r5 + expect + volatile (raid decl unchanged) | 6469 | 1230 | 17172 |
| temp=r5 + expect + volatile + raid decl `guns,status,raidType,hard` | 6469 | 1230 | 17172 |
| temp=r5 + expect + volatile + raid decl `status,guns,hard,raidType` | 6469 | 1230 | 17172 |
| temp=r5 + expect + volatile + `Level *self = this` alias | 6469 | 1230 | 17172 |
| temp=r5 + expect + volatile + `Array *enemies` local | 6396 | 1256 | 17164 |
| temp=r5 + expect + **second rotate unnamed** | 5837 | 860 | 17180 |
| temp=r5 + expect + second rotate unnamed + volatile | 6538 | 858 | 17184 |
| temp=r5 + expect + second rotate unnamed + `enemies` local + volatile | 5555 | 843 | 17176 |
| temp=r5 + expect + second rotate unnamed + `enemies` local (no vol) | 5555 | 843 | 17176 |
| all 24 declaration permutations of `{rotation,v,k,base}` | 6469–6474 | 1244–1323 | 17172 |
| hack `i>>31` / `-i>>31` / `-i<0` (instead of `i<0`) | 5837–6469 | 860–1244 | 17172–17180 |

Key findings this wave:

1. **The second rotate is the whole regression.** Naming the first rotate `Vector p`
   (temp=r5) but leaving the second unnamed recovers 632 raw / 384 aligned
   (6,469/1,230 -> 5,837/860) — but the function grows to **17,180 B** and the
   first sret moves from r5 to fp. With the volatile-status companion the same
   shape is 6,538/858/17,184. No 17,172-B form of it was found.
2. **The raid-block declaration order is the only 17,172-B companion with a
   measured win.** `guns` first, then `status`, `hard`, `raidType` gives
   6,466/1,230 (-3 raw). The other orders are 6,469.
3. **`enemies` local** (`Array<KIPlayer*> *enemies = this->enemies;` inside the
   loop) gives 6,396/1,256 but is 8 B short (17,164).
4. **All 24 `{rotation,v,k,base}` permutations** are worse than RVKB (6,469/1,244
   without volatile); none reaches the original's `this=r4, i=fp` allocation.
5. `Level *self = this` is a no-op (6,469/1,230), confirming w10309.

## 4. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5487 -> 6466   (+979)
                           align  774 -> 1230   (+456)
.text size             0x1337c8 -> 0x1337c8 (exact)
.ARM.extab size        0x26298 -> 0x26298 (exact)
.rel.plt                    0 -> 0   (byte-identical)
.rodata/.data/.bss/.ARM.exidx 0 -> 0
.note.gnu.build-id         20 -> 20
file size             2,187,328 -> 2,187,328 (exact)
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

`check` is green.

## 5. Open questions

1. The temp=r5 property (first sret in r5) and the single-slot reuse (`sp+212`
   for both rotates) are mutually exclusive in every source form tried: naming
   the second rotate forces `sp+200` for it, leaving it unnamed reuses the slot
   but moves the first sret to fp and costs 8 B.
2. The best structural shape overall is still the both-unnamed form
   (`noexp/U/SU/df/dc/hack` = 5,652/709/17,172); it beats the landed temp=r5
   shape on both metrics but does not put the sret in r5.
3. The `(i < 0)` ternary hack is still present; removing it (`bigAlienWave ? … : …`)
   gives the original's `cbz` but shrinks the function to 17,140 B. The missing
   32 B is the original's individual `ldr`/`str` vector construction (ours emits
   `ldrd`/`strd`/`ldmia`/`stmia`), ~16 B, plus unidentified code.
4. The early-region diff at `0xb1442` (a 2-byte literal-pool shift, ~1.3 KB of
   raw diff before the alien block) is pre-existing in the baseline and is a
   separate reconstruction target.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10409/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10409/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w10409/{fdiff.py,align_diff.py,harness.py,sweep*.py,regions.py,
Level.base.cpp,block.base.txt,*.asm}`.
