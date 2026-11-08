# w9611 (wave 149): `Level::createMission` — sret-temp hoist (`sp+212` -> r5)
# reached only by a 24-byte-short `p2` shape; landed the faithful alien-loop
# call order (setScaling after setPosition): align 731 -> 714, raw 5495 -> 5657

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9611`, based on master
`7eed2966e` (wave 149). Continues `level-createmission-wave148-w9508.md`,
`…-wave148-w9509.md`, `…-wave148-w9410.md`, `…-wave148-w9411.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is
`/dev/shm/w9611/fdiff.py`; instruction-index alignment is
`/dev/shm/w9611/align_diff.py`.

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5495   align 731
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text 0x1337c8 (exact)
```

Register map of the alien loop (function-relative disassembly):

```text
ORIG  temp=r5  this=r4  gun=r6  i=fp  t=sl  rotation=r4 (reused)  k*24=r9
HEAD  temp=fp  this=r6  gun=r4  i=r9  t=r5  rotation=sl          k*24=r8
```

The original's `this` is **rematerialised** (`ldr r4,[sp,#124]` at the end of
the body, live only across the back-edge), which frees `r4`/`r5` for the
rotation and the sret temp. Ours keeps `this` in `r6` across the whole body and
greedy rematerialises the temp (`t2ADDri %stack.9`) instead.

## 1. New hypotheses measured this wave

`M`/`v`/`k`/`b` are unchanged from HEAD; all variants keep size 17,172 unless
noted. `p2` = `Vector p2 = MatrixRotateVector(...); v = p2;` for the *second*
rotate (HEAD only names the first rotate `p`).

| variant | size | raw | align | alien map (temp,this,i,t) |
|---|---:|---:|---:|---|
| **HEAD (landed w9509)** | **17172** | **5495** | **731** | fp, r6, r9, r5 |
| **landed `plain+order`** (this pass) | **17172** | **5657** | **714** | r9, r6, fp, r5 |
| `plain` alien, landed pirate | 17156 | 6452 | 797 | — |
| `plain` both loops (w9411 tree) | 17172 | 5561 | 716 | fp, r5, r4(spill), r6 |
| `p` + `setScaling` after `setPosition` | 17180 | 5821 | 806 | — |
| `p-p2` (both rotates named) | **17148** | 6527 | 1203 | **r5**, r5, r9, r6 |
| `p-p2 + order` | **17164** | 6461 | 1198 | **r5**, r5, r9, r6 |
| `p-p2` both loops | 17148 | 6523 | 1205 | **r5** |
| `p-p2 + order` both loops | 17164 | 6467 | 1200 | **r5** |
| helper `rotateInto(Vector&,Matrix&,Vector&)` + named temp | 17172 | 5528 | 817 | r8, fp, r9, r5 |
| same helper, `inline` (not inlined) | 17172 | 5528 | 817 | r8, fp, r9, r5 |
| helper writes `v` directly | 17124 | 6537 | 1211 | — |
| `Vector *temp = &base` / `temp[0].x` alias | 17172 | 5495 | 731 | unchanged (SROA) |
| `Vector &bref = base` alias | 17172 | 5495 | 731 | unchanged (SROA) |
| `Vector *temp = &v` alias | 17156 | 6452 | 797 | — |
| `Vector temp;` outside + `temp = MatrixRotateVector(...)` | 17328 | 16612 | 3271 | — |
| `if/else` table select (no `(i<0)` anchor) | 17132 | 6579 | 1270 | hoisted out of loop |
| `switch (bigAlienWave)` select | 17132 | 6579 | 1270 | hoisted out of loop |
| while-loop form | 17164 | 6575 | 1399 | — |
| `__builtin_expect(alienGuns != 0, 1)` | 17172 | 5512 | 756 | — |
| `__builtin_expect(raid, 0/1)` | 17172 | 5495 | 731 | folded |
| `__builtin_expect(i < 0, 0)` anchor | 17172 | 5495 | 731 | folded |
| loop-condition `__builtin_expect(i < end, 1)` | 17340 | 15917 | 2987 | — |
| anchor `i >> 31` / `(unsigned)i >> 31` / `i <= -1` | 17172 | 5495 | 731 | identical codegen |

Notes:

* **`p-p2` is the only shape that reaches `temp=r5`** (and reuses `r5` for
  `this`, exactly like the original's `r4` reuse). It is **24 B short**
  (17,148) and the `p2` named second-rotate temp is unfaithful (the original
  shares one sret slot for both rotates), so it is not landable under the
  size gate. Adding the faithful call order recovers 8 B (17,164) but still
  misses 17,172.
* The `Vector*`/`Vector&` aliases are all SROA'd away; they cannot influence
  the allocator.
* The helper is **not inlined** even with `inline` (clang 7 `-Oz`), so it adds
  a local function and grows `.text`; not landable.
* `if/else` and `switch` selects are if-converted and then hoisted by LICM
  (17,132 B); the `(i<0)` anchor remains the only way to keep the select in
  the loop at 17,172 B.
* `__builtin_expect` on the alien/raid guards either folds or regresses.

## 2. Landed change: faithful alien-loop call order

The original's alien loop calls `setRotation`, then `setPosition`, then
`setScaling` (b3308 / b335e / b336a). HEAD had `setScaling` before the second
rotate. The landed change reverts the first rotate to the plain
`v = MatrixRotateVector(...)` (the original shares one sret slot for both
rotates) and moves `setScaling` after `setPosition`:

```diff
-            Vector p = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
-            v = p;
+            v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
             gun->geometry->setRotation(v);
-            ((PlayerTurret *) gun)->setScaling(6.0f);
             v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
             gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
+            ((PlayerTurret *) gun)->setScaling(6.0f);
```

The pirate-fortress loop keeps the landed `p` (its `setScaling` already follows
`setPosition`). Result: `i` moves to `fp` (matching the original) and the temp
to `r9`; the alien loop raw diff drops 230 -> 224 and the alien tail 103 -> 95.
The cost is the pirate tail (27 -> 195), which is what the alien-loop `p` was
buying.

## 3. Gate counts (before -> after)

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
.text           0x1337c8 -> 0x1337c8 (exact)
_ZN5Level13createMissionEv raw 5495 -> 5657 (+162), align 731 -> 714 (-17), size 17172
```

Region table (function-relative):

| region | bytes | HEAD | landed |
|---|---:|---:|---:|
| `0x0000..0x32c2` pre-alien | 12994 | 2587 | 2589 |
| `0x32c2..0x33bc` alien-gun loop | 250 | 230 | 224 |
| `0x33bc..0x3426` alien tail | 106 | 103 | 95 |
| `0x3426..0x3504` pirate-fortress loop | 222 | 199 | 205 |
| `0x3504..0x377a` pirate tail | 630 | 27 | 195 |
| `0x377a..0x3994` raid block | 538 | 484 | 484 |
| `0x3994..0x4314` tail | 2432 | 1865 | 1865 |
| **total** | | **5495** | **5657** |

## 4. Open questions

1. `p-p2` proves `temp=r5` is reachable, but it needs the 24 B that the
   original spends elsewhere (the `p2` shape has 6195 instructions vs the
   original's 6197). A faithful source shape that keeps one shared sret slot
   *and* the `p2` allocation is still open.
2. The `(i<0)` anchor is the only in-loop table select at 17,172 B; the
   original's `cbz` is a real branch, so a source form that survives
   if-conversion without the fake i-dependency would remove the anchor.
3. The pirate tail (27 -> 195) is the entire raw cost of the landed change;
   the alien-loop `p` is what keeps it at 27.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9611/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9611/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w9611/{fdiff.py,align_diff.py,rdiff.py,map.py,icount.py,
sweep.py,apply.py,v1..v29.py,Level.base.cpp,mi.log,greedy.txt}`.
