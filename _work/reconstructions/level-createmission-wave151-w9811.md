# w9811 (wave 151): `Level::createMission` — alien-loop `p2` shape lands at
# 17,172 B with raw 5,492 / aligned 779 (temp=r9, single sret slot)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9811`, based on master
`9d491d46a` (wave 151). Continues `level-createmission-wave151-w9711.md`,
`…-wave150-w9712.md`, `…-wave148-w9508.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w9811/fdiff.py`; instruction-index alignment is
`/dev/shm/w9811/align_diff.py`; section diff is `/dev/shm/w9811/secdiff.py`.

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5657   align 714
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 87,271   .ARM.extab 1,327   .rel.plt 0 (byte-identical)   build-id 20
```

## 1. Named-temp shapes re-measured on the wave-151 tree

The w9711/w9712 notes were measured on the same `Level.cpp` (no changes between
`db9658a51` and HEAD), so their numbers reproduce.  All sizes are ours.

| alien-loop shape | size | raw | align | temp (1st/2nd sret) | this | gun | i | k | t |
|---|---:|---:|---:|---|---|---|---|---|---|
| HEAD (no named temp) | 17172 | **5657** | **714** | r6 / r6 (same slot) | fp | r4 | r9 (spill) | r6 | r5 |
| `Vector p` (1st only) | 17180 | 5821 | 806 | r9 / r9 | fp | r4 | spill | r6 | r5 |
| `Vector p2` (2nd only) | 17180 | 5819 | 854 | r9 / r9 | fp | r4 | spill | sl | r5 |
| `p` + `p2` (both) | 17164 | 6461 | 1198 | r5 / r8 | r5 | r4 | fp | sl | r6 |
| `p`+`p2` + `setHitpoints(1000)` (w9711) | 17172 | 6335 | 1203 | r5 / r8 | r5 | r4 | fp | sl | r6 |

`p`+`p2` is the only shape that puts the first sret temp in r5, but it splits the
sret slot (`sp+212` then `sp+188`) and its loop carries an extra `i` spill
(`fp` is reused for `t[k][1]`), so both metrics regress versus HEAD.

## 2. New hypothesis: the `p2`-only shape + a genuine byte store

The `p2`-only shape (name the *second* rotate) keeps the **single** sret slot
`sp+212` for both rotates — exactly like the original (`b32f6`/`b3320` both
`mov r0,r5` in the original) — and moves `temp` to **r9**.  On its own it is
17,180 B (8 B over).  Reordering the `cargo` store to just after
`setRotation` (while keeping the faithful final `cargo` store) takes it to
17,164 B, and adding `gun->field_0x70 = 0;` (a one-byte store, not a
redundant call) lands exactly 17,172 B:

```cpp
            v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
            gun->geometry->setRotation(v);
            gun->field_0x70 = 0;
            gun->cargo = nullptr;
            Vector p2 = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
            v = p2;
            gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
            ((PlayerTurret *) gun)->setScaling(6.0f);
            gun->cargo = nullptr;
            gun->shipGroup = localRace;
```

Note: the `cargo = nullptr` store appears twice (early and faithful-final).  The
early store is what moves the allocation; the final store is the original's
witnessed position (`b3372`).  Together with `field_0x70 = 0` they are the
cheapest genuine (non-call) 8 B that keeps the single sret slot.

### Register map of the landed loop

```text
        temp(1st/2nd) this  gun   i        k*24  t     rotation    v
ORIG    r5 / r5       r4    r6    fp       r9    sl    sp+312(r4)  sp+372(r4)
OURS    r9 / r9       fp    r4    [sp,#116] r6   r5    sl          sl
```

`temp` is r9 (not the original r5) but the loop now uses **one** sret slot for
both rotates like the original, and `gun`=r4 matches the coordinator's `p2`
map.  The `i` spill remains (fp is reused for `t[k][1]`).

## 3. `temp=r5` variants measured (all 17,172 B)

Every shape that forces the first sret into r5 needs `p`+`p2` (two slots) and is
raw-worse than the `p2`-only landing:

| variant | raw | align |
|---|---:|---:|
| `p`+`p2` + `setHitpoints(1000)` (w9711) | 6335 | 1203 |
| `p`+`p2` + `setHitpoints` after `setScaling` | 6304 | 1233 |
| `p`+`p2` + `field_0x70=0` | 6303 | 1218 |
| `p`+`p2` + cache `this->enemies` in a local (inside `if`) | **6307** | 1214 |
| `p`+`p2` + cache + `shipGroup`/`cargo` swap | 6307 | 1212 |
| `p`+`p2` + `shipGroup`/`cargo` swap + `setHitpoints` | 6335 | 1201 |
| **landed `p2`-only + early cargo + `field_0x70=0`** | **5492** | **779** |

The `p2`-only landing also beats the pristine HEAD raw (5657 -> 5492).

## 4. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5657 -> 5492   (-165)
                           align  714 -> 779    (+65)
.text diff             87,271 -> 87,106 (-165)
.ARM.extab              1,327 -> 1,327
.ARM.exidx                  0 -> 0
.rel.plt                    0 -> 0
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

## 5. Open questions

1. `temp` is r9, not r5.  The original keeps `this` in r4 and reloads it at the
   loop tail (`b336e ldr r4,[sp,#124]`), which frees r4 for the rotation/v
   addresses and lets r5 hold the sret.  Our loop keeps `this` in fp, so the
   rotation/v addresses take r8/r9 and the sret falls to r9.  A source shape
   that makes `this` rematerialise at the tail is still the missing lever.
2. The `i` spill (`str.w r4,[sp,#116]`) persists because `fp` is reused for
   `t[k][1]`; the original uses r8 for that scratch.  Freeing r8 (by
   rematerialising the rotation address) would remove the spill.
3. The early `cargo = nullptr` store is a size/allocation lever, not a witnessed
   original statement; a cleaner single-store shape at 17,172 B has not been
   found (single-store variants land at 17,164 B).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9811/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9811/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9811/secdiff.py
```

Scratch: `/dev/shm/w9811/{fdiff.py,align_diff.py,secdiff.py,harness.py,batch.py,
gen.py,dump.py,sbs.py,Level.pristine.cpp,blk_*.txt,batch_*.log}`.
