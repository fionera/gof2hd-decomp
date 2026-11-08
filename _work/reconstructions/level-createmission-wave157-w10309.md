# w10309 (wave 157): `Level::createMission` — temp=r5 + alien-gun branch
# weight + volatile-status companion (raw 5,487 -> 6,469, aligned 774 -> 1,230)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10309`, based on master
`08575efff` (wave 156). Continues `level-createmission-wave156-w10209.md`,
`…-wave155-w10210.md`, `…-wave155-w10107.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable in this environment; NDK `objdump` is the authority. Raw same-address
diff is `/dev/shm/w10309/fdiff.py`; instruction-index alignment is
`/dev/shm/w10309/align_diff.py` (worktree-local copies, `OURS` pointed at this
worktree's `cmake-build-match/libgof2hdaa.so`).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5487   align 774
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 85,831   .ARM.extab 1,292   .rel.plt 0 (byte-identical)
```

The wave-156 master still carried the pre-w10209 source: unnamed first rotate,
`gun->field_0x70 = 0;`, two `gun->cargo = nullptr;` stores, plain status read.
w10209/w10210 were never merged into master.

## 1. Landed source (`src/game/world/Level.cpp`)

```diff
-    if (alienGuns != 0) {
+    if (__builtin_expect(alienGuns != 0, 1)) { // lint: arm_builtin anchor: Level::createMission alien-gun guard
...
-            v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            Vector p = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            v = p;
             gun->geometry->setRotation(v);
-            gun->field_0x70 = 0;
             gun->cargo = nullptr;
...
-        Status *status = *(Status *volatile *) &Globals::status;
+        Status *volatile status = Globals::status;
```

This is `temp=r5` (named first rotate + drop the dead `field_0x70` store) +
`__builtin_expect(alienGuns != 0, 1)` + the `Status *volatile status`
companion. The **early `gun->cargo = nullptr;` is kept**: removing it (w10210's
`dc1`) drops the function to 17,164 B and raw explodes to 6,486 (see §3). The
original has one cargo store at the loop tail; keeping the early store is the
only 17,172-B form of this combination.

## 2. Register map of the landed alien loop

```text
        temp1        temp2        this         gun   i     k*24  t     v           t[k][1]
ORIG    r5(sp+212)   r5(sp+212)   r4(reload)   r6    fp    r9    sl    r4(sp+372)  r8
OURS    r5(sp+212)   r8(sp+200)   r5(reload)   r4    [sp+120] r9  r6    fp(sp+372)  sl
```

`temp1` (r5, `sp+212`) and `k*24` (r9) now match the original. `temp2` is still
a second slot (`sp+200`, r8) because the named `p`/`p2` pair splits stack
colouring; the original reuses `sp+212` for both rotates and the `setPosition`
vector. `this`/`gun`/`v`/`t` remain a cyclic permutation: ours keeps `gun` in
r4, `t` in r6, `v` in fp and `this` in r5, the original keeps `this` in r4,
`gun` in r6, `t` in sl, `v` in r4.

## 3. Companion sweep (all measurements on top of the landed shape)

All 17,172 B unless noted. raw / align / size.

| variant | raw | align | size |
|---|---:|---:|---:|
| **landed: temp=r5 + expect + keep early cargo + volatile status** | **6469** | **1230** | **17172** |
| landed without volatile status | 6469 | 1244 | 17172 |
| `temp=r5` only (no expect, no volatile) | 6449 | 1210 | 17172 |
| `temp=r5` + volatile status (w10209 landed) | 6449 | 1196 | 17172 |
| expect only (no temp=r5) + drop early cargo (w10210) | 5662 | 748 | 17172 |
| expect + drop early cargo, no `Vector p` (both unnamed) | 5652 | 709 | 17172 |
| both unnamed + drop field + drop early cargo, no expect | 5652 | 709 | 17172 |
| temp=r5 + expect + **drop early cargo** | 6486 | 1230 | 17164 |
| temp=r5 + expect + drop early cargo + volatile status | 6588 | 1269 | 17184 |
| temp=r5 + expect + keep `field_0x70` + drop early cargo | 6324 | 1250 | 17176 |
| temp=r5 + expect + drop early cargo, `KVRB` decl order | 6478 | 1243 | 17164 |
| temp=r5 + expect + keep early cargo, `KVBR` decl order | 6473 | 1266 | 17172 |
| temp=r5 + expect + keep early cargo, `KVRB` decl order | 6471 | 1257 | 17172 |
| temp=r5 + expect + drop early cargo, plain table select | 6574 | 1241 | 17132 |
| `Level *self = this` alias | 6469 | 1244 | 17172 (no effect) |
| named `Vector pos` for `setPosition` | 6469 | 1230 | 17172 (no effect) |
| `const float *t` + `t[k*6+n]` | 6469 | 1230 | 17172 (no effect) |
| `Vector(...)` 3-arg ctor instead of `{...}` | 6469 | 1230 | 17172 (no effect) |

Key findings:

1. **The expect hint is counterproductive in the temp=r5 shape.** It moves the
   alien-gun guard to the right `beq.w` (matching the original's `b339c`) but
   costs +20 raw / +34 aligned (6449/1210 -> 6469/1244). In the baseline alien
   shape the same hint is a large win (5,487/774 -> 5,662/748).
2. **Removing the dead early cargo store costs exactly 8 B** in the temp=r5
   shape (17,172 -> 17,164) because clang no longer emits the early
   `str r0,[r4,#80]`; the original has one cargo store, so this is faithful but
   breaks the 17,172-B size gate. Keeping it restores both size and raw.
3. **`Status *volatile status` is the only companion that improves the aligned
   metric without changing raw or size** (1244 -> 1230), exactly as in w10209
   (1210 -> 1196). `Level *self`, `Vector pos`, `const float *t` and the 3-arg
   `Vector(...)` constructor are all no-ops.
4. **No companion recovers the +962 raw / +422 aligned.** The best 17,172-B
   form of the combination is 6469/1230 (+982/+456 over baseline). The
   numerically best shape overall is the *both-unnamed* form
   (5652/709/17172), but it puts the shared sret temp in r6, not r5.
5. The original's individual `ldr`/`str` vector construction (vs our
   `ldrd`/`strd`) is **not** reproduced by `const float *t`, `t[k*6+n]` or the
   3-arg `Vector(...)` constructor; clang still pairs the loads. This remains
   an open source-shape question.

## 4. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5487 -> 6469   (+982)
                           align  774 -> 1230   (+456)
.text size             0x1337c8 -> 0x1337c8 (exact)
.text diff             85,831 -> 86,813  (+982)
.ARM.extab size        0x26298 -> 0x26298 (exact)
.ARM.extab diff         1,292 -> 1,304   (+12)
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

1. `temp2` must reuse `temp1`'s `sp+212` slot (and the `setPosition` vector must
   reuse it too). The named `p`/`p2` pair splits the slot; the both-unnamed form
   merges it but moves the temp to r6. A source form that both forces r5 and
   keeps one slot is still unknown.
2. The `(i < 0)` ternary hack is still present. `bigAlienWave ? … : …` gives the
   original's `cbz` but shrinks the function (17,132 B with the landed shape);
   the missing code is unidentified.
3. The original's individual `ldr`/`str` vector construction vs our
   `ldrd`/`strd` is still unexplained (~14 B).
4. The raid prologue target (`&Globals::status` in r8 across
   `getRandomEnemyFighter`, `fortressGuns` in fp) is still not reached.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10309/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10309/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w10309/{fdiff.py,align_diff.py,mk.py,sweep.py,measure.sh,
variants*.json,results.json,orig.asm,comb.asm,both.asm,dc0.asm,best.asm,final.asm}`.
