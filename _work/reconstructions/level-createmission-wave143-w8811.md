# w8811 (wave-143): `Level::createMission` — alien-gun-loop statement order and
# the register-allocation root of the "gun in r6 / tail reloads" anatomy

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8811`, based on master
`bf083ab41` (wave 142). Continues `level-createmission-wave142-w8712.md`,
`…-wave141-w8611.md`, `…-wave140-w8511.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w8811/fdiff.py`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5756
```

## 1. Original loop statement order, reconstructed from Thumb

The original alien-gun loop (`b329a`..`b3394`, 254 B) executes the source
statements in this order (call sequence is authoritative):

```text
b329a  createStaticObject(nullptr, 6772, true)
b32a8  store result into (*this->enemies)[i]
b32bc  gun = (*this->enemies)[i]           (cached in r6)
b32c2  gun->player->setMaxHitpoints(1000)
b32c6  t = bigAlienWave ? station : outpost   (in-loop `ldr; cbz` diamond)
b32f6  v = MatrixRotateVector(rotation, Vector{t[k][3..5]})
b3308  gun->geometry->setRotation(v)
b3320  v = MatrixRotateVector(rotation, Vector{t[k][0..2]})
b3364  gun->setPosition(base + v)           (virtual)
b336a  ((PlayerTurret *) gun)->setScaling(6.0f)   <-- uses cached r6
b3372  gun->cargo = nullptr
b337e  gun->shipGroup = localRace
```

So the checked-in HEAD order is wrong in one place: HEAD calls `setScaling`
*before* `setRotation` and through a fresh `(*enemies)[i]` load, whereas the
original calls it *after* `setPosition` on the cached `gun` in r6. The
wave-13x "reload anchor" is not in the original.

## 2. The natural (faithful) loop is 32 B shorter and hoists the select

Rewriting the loop to the order above with the cached gun
(`vB.txt` / `vF2.txt` in `/dev/shm/w8811`) gives:

| variant | size | raw diff | select in loop? |
|---|---:|---:|---|
| HEAD (`reload` + `(i<0)` anchor) | 17172 | **5756** | yes, `cmp asr#31` form |
| cached gun, setScaling last, plain select | 17140 | 6606 | no (hoisted, `cmp/it/movne`) |
| cached gun, setScaling last, `(i<0)` anchor | 17172 | 5906 | yes, `cmp asr#31` form |
| cached gun, setScaling last, `(i<0)` anchor, cargo/shipGroup swapped | 17172 | 5857 | yes |
| cached gun, setScaling last, `(k<0)` anchor | 17140 | 6606 | no (`k` provably >=0) |
| cached gun, setScaling last, `(i>>31)` anchor | 17172 | 5906 | yes |
| cached gun, setScaling last, `volatile` condition | 17132 | 12948 | load yes, if-converted |
| cached gun, setScaling last, `bool bigAlienWave` | 17164 | 12388 | no |
| cached gun, setScaling last, `bool alienWave` | 17124 | 12500 | guard fixed, −48 B |
| HEAD order, cached gun (`gun->setScaling`) | 17164 | 6420 | yes |

None beats HEAD's 5756.

## 3. Root cause: the register map is decided *before* the loop

The original loop register map is:

```text
r4 = &rotation (and &v)      r5 = &Vector temporary
r6 = gun                     r8 = t[k][1]
r9 = k*24                    sl = table base (then alienGuns in the tail)
fp = i                       this, alienGuns, localRace spilled on the stack
```

`this` is in r4 at loop entry (`mov r0,r4`), dies after the second
`this->enemies` load (`b32b6`), and r4 is immediately reused for `&rotation`
(`b32da add r4,sp,#312`); hence the tail reload `b336e ldr r4,[sp,#124]`.
`alienGuns` is in sl at the `if (alienGuns != 0)` guard (`b322e ldr.w sl,[sp,#52]`)
and reused as the table base; hence the tail reload `b3386 ldr.w sl,[sp,#52]`.

In every variant we build, the allocator instead keeps `this` in r5 and
`alienGuns`/table in r6, and puts `gun` in r4 and `&rotation` in sl. That choice
is made **before** the loop, at the `if (alienGuns != 0)` guard: the original
loads `alienGuns` into the high register `sl` (`ldr.w sl` + `cmp.w sl` = 8 B),
ours into low `r6` (`ldr r6` + `cmp r6` = 4 B). That 4-byte preamble difference
is the start of the 16-byte phase shift that makes the whole alien region
(0xb2e04..0xb3428) compare at shifted addresses.

The first real difference in the region is the alien guard itself
(`b2e04`): the original emits the branchless `bool` form
`subs/mov/it ne/movne/ldr/eor/orrs/bne`, ours the two-compare short-circuit
`ldr/cmp/beq/cmp/bne`. This is the wave-140/141 `bool alienWave` issue. The
`bool` type fixes the guard but drops the 18-byte redundant phi re-init at
`b0f16` (clang keeps the i1 phi in a register), costing 16 B; with the faithful
loop that becomes −48 B (17124 B) because the loop is also 32 B shorter.

## 4. Negative results for keeping the select in-loop with a `cbz`

* A plain `bigAlienWave ? A : B` is hoisted + if-converted by clang 7.0.2 `-Oz`
  even at this register pressure (`cmp/it/movne` in the preheader).
* `(i < 0)` / `(i >> 31)` is the only anchor that keeps the select in the loop
  at the right size, but it lowers to `cmp.w r1,r9,asr#31; it ne; movne`
  (8 B) instead of the original's `ldr; cbz` + two `ldr [pc]` arms (18 B),
  and it makes `i` live at the select, which forces `i` to be spilled and
  `this` to be kept in r5.
* `(k < 0)` is folded away (k is provably non-negative).
* `volatile` keeps the load in-loop but if-converts the select.
* `bool bigAlienWave`, `if/else`, `(bool)` cast at the guard: no change to the
  `cbz` form.

## 5. Conclusion

The alien-gun loop's remaining 5,756 diff is dominated by the 16-byte phase
shift rooted in the `bool`-vs-`int` alien guard and the `alienGuns` register
choice, not by the loop body itself. The faithful statement order (cached gun,
`setScaling` last) is 32 B shorter than the original because the original keeps
the table select in-loop as an 18-byte diamond, which clang 7.0.2 only
reproduces with the `(i<0)` anchor at the cost of the wrong `cmp asr` form.
HEAD (17,172 B, raw 5,756) is retained; no source change this wave is a metric
win.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8811/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w8811/runs.py _ZN5Level13createMissionEv
python3 /dev/shm/w8811/norm.py 0xb2e04 0xb2f30
python3 /dev/shm/w8811/regiondiff.py _ZN5Level13createMissionEv 0xb3280 0xb33a0
```

Scratch: `/dev/shm/w8811/{Level.base.cpp,fdiff.py,runs.py,norm.py,regiondiff.py,
apply.py,apply2.py,measure.sh,vA.txt,vB.txt,vBk.txt,vBs.txt,vF.txt,vF2.txt,
vFp.txt,vH.txt,vEn.txt,vif.txt,vvol.txt,bools.json,baw.json,bigb.json,
castbool.json,orig_full.asm,Level.ll}`.
