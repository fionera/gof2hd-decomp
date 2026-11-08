# `Level::createShip` — wave 144 (w8916): the parameter-spill lever is the
# coordinate **type**, not the declaration order.  `int z, int y, float x`
# (`iif`) restores the original `shipClass`-in-register / `type`-spilled shape
# and cuts the raw same-address diff 1978 -> 1647, but the body grows to 2768 B.

Status: **note-only** (no `src/` change landed; master body kept).
Gate on the unchanged tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.
Worktree `wt/w8916`, base master `f5093474f`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903, `-Oz`, image base 0x10000).

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`),
original 2764 B @ `0xc0214`, exact 336 B / 57-row `ttype 0xff` LSDA.
Raw same-address diff bytes (not the PYTHONHASHSEED-dependent fuzzy %) is the
metric; baseline 1978.

## 0. The original's parameter map (re-derived from Thumb)

`this=r0, race=r1, shipClass=r2, type=r3` (member ABI), then
`waypoint/hostile/group` on the stack.  Original prologue:

```
c0220 str  r1,[sp,#60]   ; race        (the ONLY parameter spill)
c0222 mov  r9,r0         ; this
c0226 mov  fp,r3         ; type        -> r11
c0228 mov  r5,r2         ; shipClass   -> r5 (then sl, then r6)
c022c ldr  r4,[r0,#0]    ; Globals::status -> r4
c0238 mov  sl,r5         ; shipClass -> sl (r5 wanted for rnd)
c02b4 mov  r6,sl         ; shipClass -> r6 (sl wanted for empPoints)
```

So the target map is `r4=status, r5=rnd/shipClass, r6=nextInt/shipClass,
r8=mission, r9=this, sl=shipClass, fp=type`, with `race` and all three
jitters spilled.  Master (`float z; float y; int x;`, z/y/x order) instead
spills **race + shipClass** and puts `type` in `sl`:
`r4=nextInt, r5=rnd, r6=status, r10=type, r11=jitterX`.

## 1. New result: the coordinate *type* selects the spill

The coordinate locals' types (not their declaration order) decide whether
`type` or `shipClass` is the second spill.  Measured on the real TU
(raw diff / body size):

| coordinate types (x,y,z) | raw diff | size | spilled params |
|---|---:|---:|---|
| `float,float,int` (master) | **1978** | **2764** | race + shipClass |
| `float,float,float` | 2090 | 2784 | race + shipClass |
| `int,int,int` (faithful) | 2334 | 2792 | race + type |
| `float,int,float` | 1979 | 2764 | race + shipClass |
| `int,float,float` | 1978 | 2764 | race + shipClass |
| **`int,int,float`** (`iif`) | **1647** | 2768 | **race + type** |
| `int,float,int` | 1651 | 2768 | race + type |
| `float,int,int` | 1655 | 2768/2772 | race + type |

The `iif` shape is the first form that reproduces the original's
*shipClass-in-a-register* half of the map (`shipClass -> sl`, `type` spilled
to `[sp,#60]`, `race -> [sp,#56]`), and it cuts the raw diff by 331 bytes.
But it is 4 B too long (2768), so `size_exact` would drop to 4523/4524 and
the section size would grow; it is not landable under the gate.

The declaration order of `x/y/z` is codegen-neutral within a type vector:
all six `iif` permutations give the same 1647/2768.  Jitter order is not
free once the coords are `iif`: `X,Z,Y` gives 1656/2772.  Computing the
coords before `new Player` gives 2528/2808.

## 2. Why `iif` is 4 B too long

Sequence alignment against the original shows the extra bytes are in the
`BoundingAAB` zero-argument setup: the original reuses its `r6=0` constant
(`str r6,[sp,#0]`), while `iif` has `r6=status` and materialises a fresh
`movs r0,#0; str r0,[sp,#0]` at several constructor sites.  That is the same
`r6`-vs-`r0` consequence of `status` living in `r6` instead of `r4`; the
`iif` shape fixes the `type`/`shipClass` half of the map but not the
`status` half.

## 3. Conclusion / open item

The remaining gap is one register: the original keeps `status` in **r4**,
`nextInt` in **r6** and `type` in **fp**; every source shape tried gets two
of the three.  The `iif` coordinate vector is the closest measured shape
(1647 vs 1978) and pins the `shipClass`/`type` half, but needs a 4-byte
saving to land.  A source form that also moves `status` from r6 to r4 (so
`r6` can hold the zero constant and `fp` can hold `type`) is the open item.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8916/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch `/dev/shm/w8916/`: `fdiff.py`, `try.py` (replace the function body,
rebuild, score), `gen_coords.py` (type/order sweep), `v_*.cpp` variants,
`orig.asm` / `intxyz.asm` / `iif.asm`, `cs.txt` / `cs2.txt` / `cs3.txt`
(`-mllvm -print-after-all -mllvm -filter-print-funcs=...` greedy RA dumps).
The greedy dump confirms the vreg map directly: master `%133 type -> r10`,
`%132 shipClass -> stack.3`, `%131 race -> stack.2`; `iif` `%132 shipClass ->
r10`, `%133 type -> stack.1`, `%131 race -> r4 -> stack.2`.
