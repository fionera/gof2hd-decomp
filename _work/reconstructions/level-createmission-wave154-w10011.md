# w10011 (wave 154): `Level::createMission` — temp=r5 alien-loop shape lands at
# 17,172 B without growing the gold pad; `GOF2_GOLD_PAD_CAP` wired

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10011`, based on master
`4417db0b4` (wave 153). Continues `level-createmission-wave153-w9911.md`,
`…-wave153-w9912.md`, `…-wave151-w9811.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w10011/fdiff.py`; instruction-index alignment is
`/dev/shm/w10011/align_diff.py` (worktree-local copies of the shared tools).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5487   align 774
alien loop: temp=r9/r9  this=fp  gun=r4  i=[sp,#116]  k=r6  t=r5  rot/v=sl
target:     temp=r5/r5  this=r4  gun=r6  i=fp         k=r9  t=sl  rot/v=r4
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 86,623   .ARM.extab 1,265   .rel.plt 0 (byte-identical)
```

## 1. The w9911 17,172-B shape is `p`+`p2` with the `field_0x70` probe removed

w9911 measured "`p`+`p2`, early `cargo=nullptr` only" at 17,172 / raw 6,449 /
align 1,212.  Re-measured on the wave-153 tree, the exact source is: name the
*first* rotate `Vector p` and **delete** the `gun->field_0x70 = 0;` probe
(keeping both `cargo = nullptr` stores).  The `cargo` store is dead code in
this shape (clang DSEs the early one), so "both" and "late-only" are identical.

| shape | size | raw | align | .text | temp 1/2 |
|---|---:|---:|---:|---|---|
| HEAD (p2 only, `field_0x70`) | 17172 | 5487 | 774 | 0x1337c8 | r9 / r9 |
| `p`+`p2`, `field_0x70`, both cargo | 17196 | 6593 | 1231 | 0x1337e8 | r5 / r8 |
| `p`+`p2`, `field_0x70`, early cargo | 17156 | 6519 | 1195 | 0x1337c8 | r5 / r8 |
| `p`+`p2`, `field_0x70`, late cargo | 17176 | 6305 | 1216 | 0x1337e8 | r5 / r8 |
| **`p`+`p2`, no `field_0x70`, both cargo (landed)** | **17172** | **6449** | **1210** | **0x1337c8** | **r5 / r8** |
| `p`+`p2`, no `field_0x70`, early cargo | 17164 | 6338 | 1211 | 0x1337c8 | r5 / r8 |
| `p`+`p2`, no `field_0x70`, no hack (cbz) | 17140 | 6591 | 1243 | 0x1337a8 | — |
| `p`+`p2`, no `field_0x70`, `v` first | 17172 | 6451 | 1223 | 0x1337c8 | r5 / r8 |
| `p`+`p2`, no `field_0x70`, cargo before rotate | 17172 | 6468 | 1210 | 0x1337c8 | r5 / r8 |
| `p`+`p2`, no `field_0x70`, setScaling before setPosition | 17172 | 6451 | 1232 | 0x1337c8 | r5 / r8 |
| `p`+`p2`, no `field_0x70`, `__builtin_expect(table,1)` | 17172 | 6449 | 1210 | 0x1337c8 | r5 / r8 |
| `p`+`p2`, no `field_0x70`, `__builtin_expect(alienGuns,1)` | 17172 | 6469 | 1244 | 0x1337c8 | r5 / r8 |

The landed shape is the only one that both puts the first sret temp in r5 and
keeps `.text` at `0x1337c8`, so it needs no linker-cap change.  It is also the
faithful source: the original alien loop has **no** `field_0x70` store (its
body goes straight from `setRotation` at `b3308` to the second argument build at
`b330c`), and it has exactly one `cargo = nullptr` (`b3372`).

## 2. `GOF2_GOLD_PAD_CAP` wired into `CMakeLists.txt`

`tools/patch_gold_pad.py` (wave 152) rewrites gold's hard-coded ARM stub-table
padding cap (`min(delta*0.02, 0x40)`).  It is now a cache variable:

```cmake
set(GOF2_GOLD_PAD_CAP "0x40" CACHE STRING "gold ARM stub-table auto-padding cap (hex)")
```

At the default `0x40` the stock `${GOF2_NDK_GOLD}` is used unchanged (no copy,
no patch, byte-identical link).  Any other value copies gold into the build dir,
patches it, and links with the copy.  Verified: with
`-DGOF2_GOLD_PAD_CAP=0x20`, the 17,196-B `p`+`p2`+`field_0x70` shape links at
`.text 0x1337c8` (the +0x20 pad is removed); the default build is unaffected.

## 3. Landed change (`src/game/world/Level.cpp`, alien-gun loop)

```diff
-            v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            Vector p = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            v = p;
             gun->geometry->setRotation(v);
-            gun->field_0x70 = 0;
             gun->cargo = nullptr;
```

## 4. Register map of the landed alien loop

```text
        temp1       temp2       this        gun   i          k     t     rotation    v
ORIG    r5(sp+212)  r5(sp+212)  r4(reload)  r6    fp         r9    sl    r4(sp+312)  r4(sp+372)
OURS    r5(sp+212)  r8(sp+200)  r5(reload)  r4    r4/sp+116  r9    r6    r8(sp+312)  fp(sp+372)
```

`temp1` and `k` now match the original.  `temp2` is a second slot because the
two named temps (`p`, `p2`) split stack colouring (the original uses one
`sp+212` slot for both rotates); `this`/`gun`/`i`/`t` are still one rotation
off.  The original reloads `this` at the loop tail (`b336e ldr r4,[sp,#124]`)
and `alienGuns` at `b3386`; ours reloads both too (`b3326`/`b3328`), but into
r5/sl instead of r4/sl.

## 5. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5487 -> 6449   (+962)
                           align  774 -> 1210   (+436)
.text diff             86,623 -> 87,585  (+962)
.ARM.extab              1,265 -> 1,277   (+12)
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

All 26 named section sizes exact; file size exact (2,187,328 B).

## 6. Open questions

1. `temp2` must reuse `temp1`'s `sp+212` slot.  `p`+`p2` as separate named
   locals splits it (`sp+212` / `sp+200`); a single named temp reused for both
   assignments loses copy elision and grows to 17,312–17,352 B.  The original
   is the unnamed `v = MatrixRotateVector(...)` twice (one slot) but with the
   sret in r5, so the remaining lever is pure allocation, not source shape.
2. The `(i < 0)` ternary hack is still present.  Removing it gives the
   original's `cbz` but shrinks the function to 17,140 B; the missing 32 B of
   genuine code is still unidentified (w9812's open question).
3. `this`=r4 / `gun`=r6 / `i`=fp / `t`=sl is still one register rotation away.
   The original's `this` live range is split at the `getPosition` call and
   rematerialised at the loop tail; ours keeps `this` in r5 and reloads it at
   the tail (b3326), so the shape is close but the allocator still prefers
   r4=gun.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10011/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10011/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w10011/{fdiff.py,align_diff.py,regions.py,regions2.py,
meas.sh,mk*.py,Level.base.cpp,Level.pp2.cpp}`.
