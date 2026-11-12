# Small tails — wave 189 / w13312: Hud::draw and Generator::createMission re-swept

Status: **research note, no `src/` change landed.** Worktree `wt/w13312`,
branch `wave189-w13312`, base master `947c517b7` (wave-189 notes).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13312`.

The two smallest residuals after ModStation/MenuTouchWindow were re-diffed
instruction by instruction with `tools/fn_diff.py`, `objdump -d` and a
normalised-instruction `difflib` dump.  Every candidate that fixes a residual
either changes the function size, re-colours the whole function, or moves the
literal pool.  The tree is left clean at the base.

## 0. Baseline (re-confirmed on the base link)

| symbol | VMA | size | raw | aligned | insn |
|---|---|---:|---:|---:|---:|
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | **32** | 21 | 3226/3226 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `0x971b8` | 1996 | **40** | 21 | 744/744 |

Gate on exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS** — `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## 1. `Hud::draw()` — 32 raw is a local optimum

The residual is exactly two independent ties:

1. **Damage block, `hitFlags & 0x24`.**  The landed source has the unbraced
   volatile-pointer ballast

   ```cpp
   if (hitFlags & 0x24)
       Status **volatile p = &Globals::status;
       this->field_0x4b8 = 300;
   ```

   so the compiler emits `mov.w r0,#300; it ne; strne.w r8,[sp,#208];
   str.w r0,[sl,#1208]` (store to the field is *unconditional*).  The original
   emits the clean `itt ne; movne.w r0,#300; strne.w r0,[sl,#1208]` and reuses
   the same `movne` idiom for the following `0x18` block.  Removing the
   ballast makes the damage block byte-identical but shrinks the function to
   9328 B and re-colours the whole body: **7339 raw / 928 aligned**.

   New placements/forms tried this wave (all inside `Hud::draw`):

   | shape | raw | aligned | size |
   |---|---:|---:|---:|
   | landed `Status **volatile p` in `if (0x24)` | **32** | **21** | 9332 |
   | ballast in `if (1)` / `if (2)` | 8061 / 8062 | 854 / 853 | 9360 |
   | ballast in `if (0x18)` | 7077 | 423 | 9336 |
   | ballast before / after the four ifs | 8070 / 7111 | 1080 / 823 | 9360 / 9336 |
   | `Status **volatile const p` | 32 | 21 | 9332 |
   | `Status *const *volatile p` | 32 | 21 | 9332 |
   | `Status *volatile *p` / `Status **p` | 7494 | 920 | 9320 |
   | `volatile int hitFlags` (no ballast) | 7393 | 920 | 9340 |
   | `volatile unsigned int savedColor` | 8893 | 1639 | 9204 |
   | `volatile unsigned int sideImage` / `frontImage` | 8653 / 8745 | 1046 / 1371 | 9320 / 9296 |
   | `volatile int dummy = 0` at top / before / after / tail | 8877 / 2962 / 3030 / 6742 | 1001 / 889 / 894 / 923 | 9292 / 9332 / 9332 / 9332 |

   The store is the only knob that reproduces the original's global register
   allocation; no no-code ballast (`int dummy`, arrays, address-of, `double`,
   `long long`) survives the optimizer.

2. **Dock progress block, r5/r6 swap (13 B).**  Original: `Canvas` in `r5`
   (`mov r5,fp` at 0x16594c), `top` in `r6` (`ldrh.w r6,[sl,#994]`), then
   `font` in `r5` / `Canvas` in `r6` at the `DrawString`.  Ours is the exact
   mirror (`Canvas` in `r6`, `top` in `r5`).  Statement-order sweeps
   (`top` before `String text`, before `halfW`, `barYOffset` before `halfW`,
   `y`/`y2` before `barYOffset`/`fade`, `top` after `fadeColor`) either leave
   32 raw or inflate it to 100–130.  A local `PaintCanvas *canvas` changes
   codegen (9300–9320 B, 8262–8487 raw), and a local `font` changes it to
   8300 raw / 9300 B.  The swap is a global allocation tie.

## 2. `Generator::createMission()` — the faithful body is +4 B

The landed 40-byte shape keeps `volatile int bonus` and a dead `volatile int bv`
anchor.  Its bonus block is

```
cbz r1, skip; ldr r0,[sp,#44]; subs r5,r6,r0; str r6,[sp,#92]; str r5,[sp,#96]
```

whereas the original is

```
ldr r0,[sp,#32]; cmp r1,#0; it ne; subne r5,r6,r0; str r5,[sp,#16]
```

i.e. the original reads `rem` unconditionally, stores `bonus` at `sp+16`
(reusing the dead `galaxy` slot), and has no `bv` store.  The faithful source
is the wave-186 shape:

```cpp
int bonus;
...
int rawBonus = ...;
int remVal = rawBonus % 50;
int b = rawBonus + remVal;
volatile int rem = remVal;
int remCopy = rem;              // unconditional volatile read
if (b % 50 != 0)
    b = rawBonus - remCopy;
bonus = b;
```

Re-measured on this base it is **90 raw / 31 aligned at size 2000** (not
1996).  The body is instruction-for-instruction identical; the +4 is one extra
`.ARM.exidx` cleanup `b.n` (+2) plus the literal-pool alignment `nop` (+2).
The LSDA record length stays 172 B, but its call-site/action bytes differ.

New shapes tried this wave (all keep the body or fail to shrink it):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| landed (volatile `bonus` + `bv`) | **40** | **21** | 1996 |
| wave-186 faithful (non-volatile `bonus`, `remCopy`, no `bv`) | 90 | 31 | 2000 |
| `volatile int rem` direct (no `remCopy`) | 716 | 88 | 2000 |
| `rem` at function scope / before the if / else-scope | 169 / 169 / 90 | 113 / 113 / 31 | 2000 |
| `rem` initialised at function scope | 1825 | 337 | 1960 |
| `bv` moved into the `else` block | 44 | 25 | 1996 |
| `bv = b` / `bv = 0` / `bv = bonus` | 45 / 607 / 607 | 31 / 69 / 69 | 1996 / 2000 / 2000 |
| `bv` before the `if` / before `int b` | 49 / 627 | 32 / 76 | 1996 / 2004 |
| `volatile short` / `volatile char` `bv` | 605 | 52 | 2000 |
| `volatile long long bv` | 636 | 144 | 2000 |
| no-code stack ballast (`int[1]`, `char[4]`, `&dummy`, `double`, `long long`) | 90 | 31 | 2000 |
| `volatile int dummy = 0` at top / before bonus / in else / at tail | 1825 / 330 / 331 / 212 | 337 / 120 / 120 / 43 | 1960 / 2000 / 2000 / 2000 |
| `volatile int targetStation` / `difficulty` / `itemId` / `amount` / `race` | 1894 / 1274 / 1869 / 1037 / 1138 | 504 / 191 / 405 / 162 / 339 | 1940 / 1992 / 1992 / 1992 / 1988 |

The landed 40-byte / 1996 B shape remains the only size-exact form.  The
faithful body is blocked by the compiler's cleanup landing-pad layout: the
non-volatile `bonus` frame (96 B) shifts the `String` slots and adds one
cleanup `b.n`; the volatile-`bonus` frame (104 B) keeps the landing pad count
but forces the conditional `rem` load and the `bv` store.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Hud4drawExxP9PlayerEgobjj \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
# raw=32/40, aligned=21/21, sizes 9332/1996
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13312/`
(`dumpdiff.py`, `aldiff.py`, `meas.sh`, `hudvar.py`, `hudanchor.py`,
`hudplace.py`, `hudtype.py`, `hudvol.py`, `hudorder.py`, `hudlocal.py`,
`gensweep.py`, `genexpr.py`, `genrem.py`, `genstack.py`, `genvolvar.py`,
`genballast.py`, `genvar.py`, `*.base.cpp`).
