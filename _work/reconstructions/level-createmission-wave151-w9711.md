# w9711 (wave 151): `Level::createMission` — sret temp hoisted to r5 at 17,172 B
# (named `p`/`p2` for both rotates + the 8-byte statement probe)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9711`, based on master
`db9658a51` (wave 150). Continues `level-createmission-wave148-w9508.md`,
`…-wave149-w9611.md`, `…-wave150-w9612.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable (`GHIDRA_INSTALL_DIR` has no `application.properties`); NDK `objdump`
is the authority. Raw same-address byte diff is `/dev/shm/w9711/fdiff2.py`;
instruction-index alignment is `/dev/shm/w9711/align_diff2.py`.

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5657   align 714
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 86,863   .ARM.extab 1,326   .rel.plt 0   .ARM.exidx 0
```

## 1. The w9611 `p-p2` shape reaches `temp=r5` but is 8 B short on HEAD

w9611's `p-p2` shape (name the first rotate `p` and the second `p2`, copy each
into `v`) reproduces on the wave-150 tree:

```cpp
Vector p  = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
v = p;
gun->geometry->setRotation(v);
Vector p2 = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
v = p2;
gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
((PlayerTurret *) gun)->setScaling(6.0f);
```

| shape | size | raw | align | alien map (temp,this,gun,i,t) |
|---|---:|---:|---:|---|
| HEAD (no named temps) | 17172 | 5657 | 714 | fp, r5, r4, r9(spill), r5 |
| `p-p2` | **17164** | 6461 | 1198 | **r5**, r5, r4, fp, r6 |
| `p-p2` + faithful order (HEAD's setScaling position) | 17164 | 6461 | 1198 | **r5**, r5, r4, fp, r6 |
| `p` only (first rotate) + order | 17180 | 5821 | — | r9, r6, r4, —, — |
| `p2` only (second rotate) + order | 17180 | 5819 | — | r9, fp, r4, —, — |

`p-p2` is the only shape that puts the sret temp in **r5** (the target). It is
8 B short of the mandatory 17,172 B (w9509 measured 17,148 for the pre-w9611
order, i.e. 24 B short; HEAD already carries the faithful order, so the gap is
8 B).

## 2. The 8 B is codegen, not a missing statement

A full semantic diff of the original alien loop (`0x32c2..0x33bc`, 90 insns,
250 B) against the `p-p2` loop (`0x32b0..0x33c2`, 95 insns, 274 B) shows the
same operations in the same order: `createStaticObject(6772)`, `gun =
enemies[i]`, `setMaxHitpoints(1000)`, table select, two
`MatrixRotateVector` -> `Vector::operator=` -> `v`, `setRotation`,
`setPosition`, `setScaling(6.0f)`, `cargo = nullptr`, `shipGroup = localRace`.
The original has **fewer** instructions (90 vs 95), so no source statement is
missing from the loop; the 8 B is the setup difference:

* original setup (`0x3256..0x32c2`, 108 B) loads `this` before the
  `alienGuns` test (`ldr r4,[sp,#124]`) and hoists the temp address
  (`add r5,sp,#212`);
* `p-p2` setup (102 B) does neither.

The 8 B therefore has no faithful source statement to spend it on. Every
"genuine" candidate that adds 8 B to `p-p2` was measured; the ones that hit
17,172 all change an operation:

| 8 B probe | size | raw | temp |
|---|---:|---:|---|
| `gun->player->setHitpoints(1000)` after `setRotation` (**landed**) | **17172** | **6335** | r5 |
| `gun->player->setMaxHitpoints(1000)` after `setRotation` | 17172 | 6335 | r5 |
| `gun->cargo = nullptr` after `setRotation` | 17172 | 6449 | r5 |
| `gun->cargo = nullptr` moved to after `setRotation` | 17164 | 6337 | r5 |
| `setMaxHitpoints` moved after `setScaling` | 17164 | 6381 | r5 |
| `gun->setSpacePoints(nullptr)` after `setRotation` | 17156 | 6421 | r5 |
| `gun->setDead()` after `setRotation` | 17156 | 6491 | r5 |
| `(*this->enemies)[i]->player->setMaxHitpoints(1000)` | 17312 | 16553 | r5 |

`setHitpoints(1000)` is semantically redundant (`Player::setMaxHitpoints`
already writes both `maxHitpoints` and `hitpoints`, `src/game/ship/Player.cpp:426`)
but it is the cheapest 8 B that keeps the loop's call/register shape and lands
exactly 17,172 B with `temp=r5`.

## 3. Landed change (`src/game/world/Level.cpp`, alien-gun loop only)

```diff
-            v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            Vector p = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            v = p;
             gun->geometry->setRotation(v);
-            v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
+            gun->player->setHitpoints(1000);
+            Vector p2 = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
+            v = p2;
             gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
```

Result: `temp=r5`, `i=fp`, `gun=r4`, `this=r5`, `t base=r6`, `k*24=sl`.
The original map is `temp=r5`, `i=fp`, `gun=r6`, `this=r4`, `t=sl`. So the
target temp and `i` match; `this`/`gun` and `t` are still swapped (the same
`r4`/`r5`/`r6` cyclic chain w9508 documented).

## 4. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5657 -> 6335   (+678, regression)
                           align  714 -> 1203   (+489, regression)
.text diff             86,863 -> 87,541 (+678)
.ARM.extab              1,326 -> 1,338  (+12)
.ARM.exidx                  0 -> 0
.rel.plt                    0 -> 0
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

1. The 8 B probe is not faithful. A source shape that hoists the temp address
   in the setup (`add r5,sp,#212` in the preheader) *and* shares one sret slot
   for both rotates would remove the need for the probe and likely recover the
   raw regression. `p-p2` splits the slot (`sp+212` first, `sp+188` second);
   the original uses `sp+212` for both and for `setPosition`.
2. `gun=r6`/`this=r4` needs `this` to acquire a hole before the gun load so r4
   is free for `rotation`/`v` and `this` rematerialises into r4 at the body
   end. `p-p2` rematerialises `this` into r5 instead because r4 is `gun`.
3. The 8 B probe should be removed once (1)+(2) land; it is a size-gate
   placeholder, not a faithful statement.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9711/fdiff2.py _ZN5Level13createMissionEv
python3 /dev/shm/w9711/align_diff2.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w9711/{fdiff2.py,align_diff2.py,dump.py,regions.py,
sweep*.py,Level.base.cpp,orig.txt,ours*.txt}`.
