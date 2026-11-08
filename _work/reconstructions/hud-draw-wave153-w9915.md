# Hud::draw — wave 153 (w9915): no anchor-free shape keeps `dividerOffset` in `sl`

**Status:** analysis-only, 2026-11-08 (wave 153). Worktree `wt/w9915`,
base `master 4a6104a43` (wave 152). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original **9332 B** (VMA `0x163b90`).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1325** | **1325** (no source landed) |
| aligned (instruction-index) diff | **249** | **249** |
| size | 9332 | 9332 |
| frame | 224 | 224 |

Gate at base: `check` green (`byte_exact 4507`, `linked_exact 4507`, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

---

## 0. TL;DR

1. **The w9815 mission is falsified for every shape tried.**  The
   `pivotY = dividerOffset` anchor is the *unique* source lever that keeps
   `dividerOffset` in `sl` across the armour-regen `DrawRegion2D` call while
   keeping the 224-byte frame and 9332 B.
2. The reason is a *liveness* fact, not a value fact: the original's
   `dividerOffset` **is not used inside the regen block at all** (disasm
   `163fb8..164004` never touches `sl`).  Its third live range is
   `163fb2 (ldr.w sl,[sp,#96]) -> 16406a (add.w r3,r4,sl)`, i.e. it spans the
   regen call only because the *gamma* block uses it afterwards.  The original
   compiler hoisted the reload out of the gamma branch; ours sinks it into the
   branch (`ldr r0,[sp,#92]` at `16405a`) unless something forces the value to
   be live at the regen call.  The only free such use is the regen fill's
   unused `pivotY` (angle is 0).
3. Every "free" alternative either (a) folds to the constant 0 before
   register allocation (loses the liveness), or (b) is not free and adds
   instructions / changes the RA.  Table below.
4. **New observation:** `Globals::layout->field_0x1e8` (the source of
   `dividerOffset`) is a 0/1 retina flag (`Layout::Layout` line 199:
   `retinaDisplay ? 1 : (iPadHD ? 1 : (n9 ? 1 : 0))`).  That does not help the
   codegen: the compiler still cannot fold it.

---

## 1. Measured variants (full build; base = 1325 / 249 / 9332 / 224)

All variants replace the anchor line with `pivotY = 0` plus the listed change,
unless noted.

| variant | size | raw | align | frame |
|---|---:|---:|---:|---:|
| **base (anchor `pivotY=dividerOffset`)** | **9332** | **1325** | **249** | **224** |
| `pivotY=0` (no anchor) | 9312 | 8261 | 1009 | 216 |
| `const int p = dividerOffset; pivotY=p` | 9332 | 1325 | 249 | 224 |
| `pivotX=dividerOffset, pivotY=0` | 9336 | 7312 | 454 | 224 |
| `pivotX=pivotY=dividerOffset` | 9352 | 7927 | 589 | 224 |
| `pivotY=(int)((float)dividerOffset*0.0f)` | 9380 | 8013 | 623 | 224 |
| `pivotY=(int)((float)dividerOffset-(float)dividerOffset)` | 9372 | 7985 | 644 | 224 |
| `angle=(float)dividerOffset*0.0f` | 9356 | 7856 | 549 | 224 |
| `pivotY=(dividerOffset&~dividerOffset)` | 9312 | 8261 | 1009 | 216 |
| `pivotY=dividerOffset%1` | 9312 | 8261 | 1009 | 216 |
| `pivotY=(dividerOffset>>31)&0` | 9312 | 8261 | 1009 | 216 |
| `pivotY=(dividerOffset,0)` | 9312 | 8261 | 1009 | 216 |
| `pivotY=this->field_0x43c` | 9332 | 2290 | 973 | 216 |
| `pivotY=this->field_0x446` | 9328 | 7421 | 836 | 216 |
| `pivotY=this->field_0x44c` | 9312 | 8261 | 1010 | 216 |
| `pivotY=armorBarY` | 9320 | 8114 | 1051 | 216 |
| `pivotY=(int)armorRegenFillImage` | 9312 | 8261 | 1010 | 216 |
| hull-bar `pivotY=dividerOffset` | 9328 | 7456 | 1028 | 216 |
| shield-bar `pivotY=dividerOffset` | 9324 | 8134 | 820 | 216 |
| hull+regen `pivotY=dividerOffset` | 9324 | 8042 | 630 | 224 |
| `if (dividerOffset!=0x7fffffff && gamma>0)` | 9364 | 8298 | 700 | 224 |
| `if (dividerOffset>=0 && gamma>0)` | 9360 | 8265 | 676 | 224 |
| `if (hasArmorRegen && dividerOffset!=0x7fffffff)` | 9332 | 2784 | 1024 | 216 |
| `if (__builtin_expect(gamma>0,1))` | 9336 | 7975 | 982 | 216 |
| `if (__builtin_expect(gamma>0,0))` | 9332 | 8757 | 1232 | 216 |
| `if (__builtin_expect(hasArmorRegen,1))` | 9320 | 8110 | 1051 | 216 |
| `if (__builtin_expect(hasArmorRegen,0))` | 9336 | 8331 | 1414 | 216 |
| `(void)__builtin_expect(dividerOffset,0)` in regen | 9312 | 8261 | 1009 | 216 |
| `__builtin_assume(dividerOffset!=0x7fffffff)` | 9312 | 8261 | 1009 | 216 |
| `regenW=__builtin_expect(regenW,dividerOffset)` | 9312 | 8261 | 1009 | 216 |
| `if (dividerOffset==0x7fffffff) regenW=0;` | 9356 | 7890 | 575 | 224 |
| `regenW=dividerOffset==0x7fffffff?0:regenW` | 9356 | 7890 | 575 | 224 |
| hoist `gammaY/gammaBarY/gammaLockY` before gamma if | 9344 | 8537 | 1459 | 224 |
| hoist `gammaLockY` before gamma if | 9348 | 8533 | 1465 | 216 |
| hoist `gammaY/gammaBarY/gammaLockY` before regen if | 9344 | 8734 | 1112 | 224 |
| hoist `gammaLockY` before regen if | 9336 | 8786 | 1230 | 224 |
| `Hud *self=this` in regen block | 9312 | 8261 | 1009 | 216 |
| `Hud &self=*this` in regen block | 9312 | 8261 | 1009 | 216 |
| `dividerOffset` as `bool` | 9324 | 8041 | 889 | 216 |
| `dividerOffset` as `short` | 9324 | 7972 | 982 | 216 |
| `dividerOffset` as `unsigned char` / `char` | 9312 | 8262 | 1010 | 216 |
| `dividerOffset` as `long long` | 9312 | 8261 | 1009 | 216 |
| gamma `gammaBarY` decl swap (448 first) | 9332 | 1325 | 249 | 224 |
| gamma call order swap | 9332 | 1420 | 273 | 224 |
| `Status *volatile st4b8=Globals::status` in 4b8 test | 9336 | 7179 | 782 | 224 |
| `int dv=*(volatile int*)&dividerOffset; pivotY=dv-dv` | 9296 | 8841 | 1054 | 224 |
| `*(Status*volatile*)&Globals::status` in 4b8 test | 9332 | 1325 | 249 | 224 |
| `Status *st4b8=Globals::status` local in 4b8 test | 9332 | 1325 | 249 | 224 |

---

## 2. Why the anchor is unique

The original regen block (`163fb8..164004`) is:

```
163fb8 ldr r0,[fp]          ; player
163fbc blx getArmorDamageRate
...
163fd4 ldr r1,[r5,#704]     ; armorRegenFillImage
163fd8 ldrh r2,[r5,#1100]   ; field_0x44c (height)
163fe0 ldrh r3,[r5,#1088]   ; field_0x440 (posX)
163fe4 ldr r0,[r9]          ; Canvas
163ff0 strd r2,r4,[sp,#4]   ; height, angle=0
163ff4 strd r4,r4,[sp,#12]  ; pivotX=0, pivotY=0     <-- original
163ffa strd r3,r6,[sp,#20]  ; posX, posY=armorBarY
164004 blx DrawRegion2D
```

`sl` (= `dividerOffset`) is **dead** across the whole block.  The only reason
the original has it in `sl` at `163fb2` is the later gamma use at `16406a`.
`this` is in `r5` and is spilled to `[sp,#104]` at `163e04`; `r5` is reused
for `Canvas` in the gamma block, so the gamma block reloads `this` from the
stack.  The 224-byte frame is exactly this spill.

With `pivotY=0` the allocator instead:

```
163faa mov sl,r5            ; sl = this
163fb2 blx getArmorDamageRate
163fbe ldrh r0,[sl,#1094]   ; this via sl
...
16405a ldr r0,[sp,#92]      ; dividerOffset reloaded *inside* the gamma branch
```

so `this` never spills and the frame is 216.

The anchor makes `dividerOffset` live *at* the regen call, which forces the
reload to `163fb2` and reproduces the original allocation exactly.  It costs
one register in one `strd` (`strd r4,sl` vs `strd r4,r4`), which is the first
of the 140 residual hunks.

**A zero-valued expression cannot do it**: clang 7 folds
`x-x`, `x*0`, `x&0`, `x^x`, `x&~x`, `x%1`, `(x,0)`, `x*0.0f` (as an integer
result, via `(int)`) in InstCombine before register allocation, so the value
is no longer live.  The float forms that survive (`(int)((float)x*0.0f)`)
cost 4-6 instructions and shift the whole tail (raw 7985-8013).

The only *free* argument slot is `pivotY` (the angle is 0, so pivot is
unused by the callee).  `pivotX` is one slot earlier and makes the compiler
emit `add.w ip,sp,#4; stmia.w ip,{r2,r4,sl}` instead of the two `strd`s
(9336 B, raw 7312).

---

## 3. Residual hunks (unchanged)

* the one `strd r4,sl` vs `strd r4,r4` regen-pivotY instruction;
* the gamma-body `fp`/`r9` swap (4 hunks): original loads `field_0x44a` into
  `fp` and `field_0x448` into `r9`; ours loads them into `r9`/`fp`
  (declaration/call reordering does not change it);
* the two dead `ldr.w r8,[sp,#108]` (`Globals::status`) reloads at
  `164176`/`164210` in the damage-indicator block (~481 raw bytes).  Our
  compiler eliminates the load; `Status *volatile` locals change the code
  (9336/7179/782/224) while plain locals and `*(Status*volatile*)&...`
  are eliminated (1325 unchanged);
* the mission-panel / switch-tail register allocation (~838 raw bytes),
  unchanged by this pass.

---

## 4. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w9915
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9915/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w9915/align_diff.py _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w9915`: `fdiff.py` (raw same-address diff,
worktree-local copy of `/dev/shm/fdiff.py`), `align_diff.py` (instruction-index
diff, worktree-local copy of `_work/reconstructions/align_diff.py`),
`diffins.py` (per-hunk instruction diff), `rawrange.py` (raw diff by address
range), `try.py`/`batch*.py` (variant runners), `Hud.cpp.base` (baseline).
