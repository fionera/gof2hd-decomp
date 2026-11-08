# w9311 (wave-147): `Level::createMission` — pirate-tail region 0x3504..0x377a
# is a pure +4 shift caused by the pirate-fortress loop preheader; the w9208
# `cachedgun` change is what moves `this` out of r4

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9311`, based on master
`9c9744ca5` (wave 146). Continues `level-createmission-wave146-w9208.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w9311/fdiff.py`, `rdiff.py`, `region_align.py`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5560
region 0x3504..0x377a (pirate tail) 630 B  diff 196
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. The 630-byte region is the `case4Tail` block, not a pirate block

The region labelled "pirate tail" is the `case4Tail:` block (hunters /
`ArraySetLength` / `createShip` loop / `Objective(18,0,hunterCount,this)`),
placed physically between the pirate-fortress loop and the `if (raid)` block.
Its instruction stream is **identical to the original** apart from a uniform
+4-byte displacement:

```text
orig  b34ea  movs r0,#0        <- case4Tail start (fn+0x3512)
ours  b34ee  movs r0,#0        <- +4
```

`region_align.py 0x3504 0x377a` reports only **31 non-matching instructions of
239**; the rest are `ldr [pc,#N]` / literal-pool address differences that are
pure consequences of the shift.  The raw 196 is the byte-wise cost of comparing
`orig[i]` with `orig[i+4]` (ARM encodings are 2/4 B, so ~31% of bytes coincide).

## 2. Root cause: the pirate-fortress loop preheader is +4 B

```text
orig  b33ec add.w r2,r4,#352        ours  b33ec add.w r2,r5,#352
      b33f0 ldmia r2,{r0,r1,r2}           b33f0 ldmia r2,{r0,r1,r2}
      b33f2 ldr  r6,[pc,#448]  ; t        b33f2 ldr  r5,[pc,#448]  ; t
      b33f4 adds r3,r1,r0                 b33f4 adds r3,r1,r0
      b33f6 ldr  r5,[sp,#124]  ; this     b33f6 ldr  r4,[sp,#48]   ; fortressGuns (+2)
      b33f8 add  r6,pc                    b33f8 ldr  r6,[sp,#124]  ; this
      b33fa adds r4,r3,r2      ; i (2)    b33fa add  r5,pc
      b33fc b.n  b34d2                    b33fc add.w r8,r3,r2    ; i (4, +2)
                                          b3400 b.n  b34dc
```

So the tail shift is exactly:
* `this` is r5 (orig) vs r6 (ours);
* `i` is r4 (orig, low reg -> 2 B `adds`) vs r8 (ours, high reg -> 4 B `add.w`);
* ours additionally materialises `fortressGuns` in r4 in the preheader (+2 B)
  because the latch is entered past its `ldr r4,[sp,#48]` reload.

The loop body is otherwise the original's, but the allocator's choice cascades:
the original spills `i` (`str r4,[sp,#116]` / `ldr r1,[sp,#116]`) around the
`gun` assignment and keeps `r8/r9/sl/fp` for the four stack temporaries; ours
keeps `i` in r8 and juggles the temporaries (+8 B in the `MatrixRotateVector`
setup), which nets the +4.

## 3. The trigger is w9208's `cachedgun` change

`cachedgun` (use the cached `gun` for `setScaling` instead of a fresh
`(*this->enemies)[i]`) changes the alien loop's allocation: the original alien
loop holds `this` in **r4** (so the pirate preheader can put `this` in r5 and
`i` in r4); with `cachedgun` our alien loop holds `this` in **r5**, so the
pirate preheader must put `t` in r5 and `this` in r6 and `i` spills to r8.

Measured on the landed tree (all sizes 17172 unless noted):

| variant | tail 0x3504 | alien loop 0x32c2 | alien tail 0x33bc | total raw |
|---|---:|---:|---:|---:|
| landed (cachedgun, no R) | **196** | 215 | **17** | **5560** |
| revert `cachedgun` (= pristine) | **15** | 233 | 100 | 5592 |
| revert `cachedgun`, keep `status` | 15 | 236 | 100 | 5617 |
| revert `cachedgun`, keep `status`, drop R (size 17176) | 17 | 236 | 100 | 5535 |
| revert `cachedgun`, drop `status`, drop R (size 17176) | 17 | 236 | 100 | 5513 |

Reverting `cachedgun` alone restores the pristine 15-byte tail; the +4 B size
that `cachedgun` had bought is given back by the `case4Tail` literal-pool
padding (the block grows 612 -> 616 B when it starts at the original address),
so the total stays 17,172 B.

## 4. What was tried (all faithful, none moved the pirate allocation)

`unsigned i`, `++i`, bound reorder/parenthesisation, `const int fortressGuns`,
`int guns = alienGuns + fortressGuns;` hoisted, `Array<KIPlayer*> *enemies`
local, `PlayerTurret *turret` local, `(*this->enemies)[i]->player->...`,
`while` loop, `i` declared before `base`, `t` as `const float (*)[6]`,
`i++, k++`/`k++, i++` in the alien loop.  All either no-op or worse.
Hoisting the pirate loop start/bound into a local (`int pfirst`) changes the
stack frame and regresses the whole function (raw 6580).

## 5. Landed change

Reverted the wave-146 `cachedgun` + `status` + `case4Tail rnd` edits to the
faithful pre-w9208 source (`git show 34d6e6951^:src/game/world/Level.cpp`).
This is the only measured variant that satisfies the mission's two hard
constraints simultaneously: **region 0x3504..0x377a = 15** and **size 17,172**.

Trade-off (honest): the total raw is 5592 vs the landed 5560 (+32), because
the alien-tail win (17 vs 100) is lost.  If the coordinator prefers the lower
total raw, keep w9208 and treat the tail as its known price; if the tail region
is the priority, this variant is the faithful one.

## 6. Region table before -> after

| region | bytes | before (w9208) | after (this) |
|---|---:|---:|---:|
| `0x0000..0x32c2` pre-alien | 12994 | 2587 | 2586 |
| `0x32c2..0x33bc` alien-gun loop | 250 | 215 | 233 |
| `0x33bc..0x3426` alien tail | 106 | 17 | 100 |
| `0x3426..0x3504` pirate-fortress loop | 222 | 196 | 198 |
| `0x3504..0x377a` pirate tail | 630 | **196** | **15** |
| `0x377a..0x3994` raid block | 538 | 484 | 480 |
| `0x3994..0x4314` tail | 2432 | 1865 | 1980 |
| **total** | | **5560** | **5592** |

## 7. Gate counts (before -> after)

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
_ZN5Level13createMissionEv raw diff 5560 -> 5592, size 17172
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9311/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9311/rdiff.py _ZN5Level13createMissionEv 0x32c2-0x33bc ...
python3 /dev/shm/w9311/region_align.py _ZN5Level13createMissionEv 0x3504 0x377a
```

Scratch: `/dev/shm/w9311/{fdiff.py,rdiff.py,region_align.py,tail_align.py,
sizemap.py,measure.sh,patch*.py,lib.pristine.so,lib.landed.so,Level.*.cpp}`.

## Open questions for the next wave

1. Make the alien loop hold `this` in r4 while keeping `cachedgun`: then the
   pirate preheader emits `adds r4,r3,r2` (2 B) and drops the preheader
   `fortressGuns` load, recovering the 181 tail bytes *and* keeping the
   alien-tail 17 — total would be ~5379.  The trigger is the alien loop's
   `this`/`gun` (r5/r4) vs the original's (r4/r6); the `(i<0)` anchor and the
   cached `setScaling` both influence it.
2. The `case4Tail` literal pool is 4 B shorter when the block starts at
   `+0x3516` (2 nops vs 3 at `+0x358e`); this is what absorbs the +4 and keeps
   the total exact.  Any pre-tail shrink is therefore size-neutral.
