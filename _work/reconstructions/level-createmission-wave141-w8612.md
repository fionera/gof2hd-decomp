# w8612 (wave-141): `Level::createMission` — station-pirate escort loop,
# w8512's sign/jitter hoist is **not** RNG-faithful (no source change)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8612`, based on master
`e18e0c2a4` (wave 140 docs). Continues
`level-createmission-wave140-w8511.md`, `…-wave139-w8412.md`,
`…-wave138-w8311.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Manual
NDK `objdump` disassembly of the original is the authority. Raw same-address
byte diff is the primary metric (`/dev/shm/w8612/fdiff.py`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5763
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## 1. The original's RNG call order (Thumb, authoritative)

The escort loop body starts at `b3bb8`.  The six `blx r9` calls in the
`Vector pos` initializer are (`r9` = `AERandom::nextInt`, `r5` = address of
`Globals::rnd`):

```text
b3be2  movw r1,#20000 ; b3bec blx r9   -> nextInt(20000)   (jx)
b3bf0  movs r1,#2     ; b3bf4 blx r9   -> nextInt(2)       (sx raw)
b3bfa  movw r1,#20000 ; b3c02 blx r9   -> nextInt(20000)   (jy)
b3bf0  movs r1,#2     ; b3c0a blx r9   -> nextInt(2)       (sy raw)
b3c32  movw r1,#20000 ; b3c3c blx r9   -> nextInt(20000)   (jz)
b3c42  movs r1,#2     ; b3c4c blx r9   -> nextInt(2)       (sz raw)
```

So the original order is **jitter, sign, jitter, sign, jitter, sign**:
`20000, 2, 20000, 2, 20000, 2`.  This is exactly the checked-in nested
expression

```cpp
nextInt(20000) * (nextInt(2) == 0 ? 1 : -1) + 10000
```

evaluated left-to-right per component (x, y, z).  The same interleaved order
appears in the neighbouring `extraAliens` block (`b3cfa`: `nextInt(50000)`
then `nextInt(2)`), confirming the source idiom.

## 2. w8512's hoist changes that order

`/dev/shm/w8612_hoist_w8512.diff` moves the three signs before the three
jitters:

```cpp
int sx = Globals::rnd->nextInt(2) == 0 ? 1 : -1;   // call 1: 2
int sy = Globals::rnd->nextInt(2) == 0 ? 1 : -1;   // call 2: 2
int sz = Globals::rnd->nextInt(2) == 0 ? 1 : -1;   // call 3: 2
int jx = Globals::rnd->nextInt(20000) * sx + 10000; // call 4: 20000
int jy = Globals::rnd->nextInt(20000) * sy + 10000; // call 5: 20000
int jz = Globals::rnd->nextInt(20000) * sz + 10000; // call 6: 20000
```

Emitted order (verified in the built `.so`): `2, 2, 2, 20000, 20000, 20000`.
That is a different RNG sequence (and therefore different escort positions)
from the original's `20000, 2, 20000, 2, 20000, 2`.  The hoist is **not
faithful**.

## 3. Measured variants (all size-exact 17,172 B)

| variant | RNG order | raw diff |
|---|---|---:|
| **checked-in nested expression (faithful)** | 20000,2,20000,2,20000,2 | **5763** |
| w8512 hoist (signs first) | 2,2,2,20000,20000,20000 | 5762 |
| interleaved `jx,sx,jy,sy,jz,sz` locals | 20000,2,20000,2,20000,2 | 5771 |
| interleaved bool signs | 20000,2,20000,2,20000,2 | 5771 |
| interleaved raw `nextInt(2)` + `==0?1:-1` | 20000,2,20000,2,20000,2 | 5771 |
| interleaved combined `dx,dy,dz` | 20000,2,20000,2,20000,2 | 5771 |
| `Vector pos; pos.x = …` (faithful) | 20000,2,20000,2,20000,2 | 5763 |
| `float px,py,pz` locals (faithful) | 20000,2,20000,2,20000,2 | 5763 |
| `const int jitter = 10000` (faithful) | 20000,2,20000,2,20000,2 | 5763 |

The hoist's 1-byte win is an RA artifact, not a match: it does put
`movw fp,#10000` in the preheader (as the original does), but it pays for it
by spilling/reloading `this` (`mov sl,r0` … `ldr.w sl,[sp,#124]`) and
reordering the body.  The faithful interleaved-local forms are 8 bytes worse
than the nested expression because all six values stay live to the end.

## 4. Why the faithful forms cannot reproduce the original's `fp` literal

The original keeps `10000` in `fp` (r11) and `this` in `sl` (r10) throughout
the loop.  Ours rematerialises the literal in the body and swaps
`this` into `fp`:

```text
orig:  b3ba4 movw fp,#10000
ours:  b3bf6 mov fp,sl ; b3bf8 movw sl,#10000 ; … ; b3c52 mov sl,fp
```

Post-RA MIR (`After Greedy Register Allocator`, `bb.282`):

```text
%5418:gpr = COPY %5419:gprnopc   ; this: r10 -> r11
%5416:rgpr = t2MOVi16 10000      ; literal: r10
%3523 = t2MLA %3521, %3517, %5416
```

The literal's interval is already loop-invariant in the preheader before RA
(`%3522` in `bb.276`); Greedy RA splits it at the loop body and rematerialises
it (`canRematerializeAt` is true for `t2MOVi16`).  The loop needs all seven
allocatable callee-saved registers (`r4,r5,r6,r8,r9,r10,r11`; `r7` is the
frame pointer), and the literal's spill weight beats `this`'s, so `this` is
evicted from r10.  Every faithful source spelling tested leaves the pre-RA MIR
identical and therefore the same RA decision.

## Conclusion

The w8512 hoist is **not RNG-faithful** (it changes the six-call order from
`20000,2,20000,2,20000,2` to `2,2,2,20000,20000,20000`), and its 1-byte raw
win is a coincidental RA artifact (it matches the preheader `movw fp,#10000`
but spills/reloads `this` and rewrites the body).  It is not landed; the
checked-in nested expression remains the best faithful formulation
(raw diff 5763, 17,172 B).

## Gate counts (unchanged; note-only commit)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
_ZN5Level13createMissionEv   raw diff 5763, size 17172
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w8612/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w8612/cmp_insn.py 0xb3b90 0xb3cd0 /dev/shm/w8612/ours_base.so
```

Scratch: `/dev/shm/w8612/{Level.base.cpp,fdiff.py,try.py,try2.py,try3.py,
try4.py,cmp_insn.py,regions.py,ours_base.so,ours_hoist.so,mir_base.txt}`.
