# `Level::createShip` — wave 125 (w7509): declaration/scope/const/reference forms around the four cleanup pointers

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `02513de33` (wave-124 final, faithful call-site args). Worktree
`/home/debian/GalaxyOnFire2/wt/w7509`. Owned TU: `src/game/world/Level.cpp`
(createShip only).

**Status: analysis only — no `src/` change landed.** ~190 source variants were
measured (all at 2764 B with the exact 336 B / 57-row / `ttype 0xff`
cleanup-only LSDA). None moves the first Greedy decision while keeping the
score: the checked-in body stays the best at **70.5415 %**, 2764 B, 1006 insns.

## Result

```
                                   original        HEAD (02513de33)
createShip fuzzy (official)        —               70.5415
createShip size                    2764 B          2764 B
createShip insns (orig/ours)       1007            1007 / 1006
createShip LSDA record             336 B           336 B
createShip LSDA rows               57              57
createShip LSDA ttype              0xff            0xff
byte_exact / linked_exact          —               4506 / 4506
data / imports / lint / drift      —               843 / 207 / CLEAN / 0
```

`check` on the unmodified tree: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.93`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`.

## 1. The residual, exactly

Original prologue (`c0220`..`c022e`):

```
c0220  str  r1, [sp, #60]   ; race spilled
c0222  mov  r9, r0          ; this  -> r9
c0224  ldr  r0, [pc, #572]
c0226  mov  fp, r3          ; type  -> fp
c0228  mov  r5, r2          ; shipClass -> r5 (moved to sl at c0238)
c022a  add  r0, pc
c022c  ldr  r4, [r0, #0]    ; &Globals::status -> r4
c022e  ldr  r0, [r4, #0]
```

HEAD prologue:

```
c0220  str  r2, [sp, #60]   ; shipClass spilled
c0222  mov  r9, r0          ; this -> r9
c0224  str  r1, [sp, #52]   ; race spilled
c0226  mov  sl, r3          ; type -> sl
c0228  ldr  r0, [pc, #560]
c022a  add  r0, pc
c022c  ldr  r4, [r0, #0]
```

Original stack slots are `race=[sp,#60]`, `this=[sp,#56]`, `type=[sp,#52]`;
HEAD's are `shipClass=[sp,#60]`, `race=[sp,#52]`, `type=[sp,#56]`.  The whole
downstream map follows from that first decision: original
`ship`/`fixed`=`r9`, `volumes`=`fp`, `fighter`=`sl`; HEAD `ship`/`fixed`=`fp`,
`volumes`=`r9`, `fighter`=`sl`.  `type` is `fp` in the original and `sl` in
HEAD; `this` is `r9` in both but is spilled at `c0332` (original) vs `c0308`
(HEAD).  The original also has an extra `mov r6, sl` (shipClass `sl -> r6` at
`c02b4`) that HEAD never emits — that is the 1007-vs-1006 instruction delta.

## 2. Why the source forms cannot move it (Greedy queue evidence)

`RAGreedy::enqueue` (LLVM 7.0.0, `lib/CodeGen/RegAllocGreedy.cpp:680`) orders
the queue by **live-range size, largest first**:

```cpp
// Allocate global and split ranges in long->short order.
Prio = (1u << 29) + Size;
...
CurQueue.push(std::make_pair(Prio, ~Reg));
```

Extracted from `-mllvm -print-machineinstrs` on the HEAD body (the
`After Simple Register Coalescing` MIR, def and last-use offsets):

| vreg | value | def | last use | uses | span |
|---|---|---:|---:|---:|---:|
| `%126` | `this` | 64B | 26160B | 4 | 26096 |
| `%129` | `type` | 16B | 25824B | 14 | 25808 |
| `%127` | `race` | 48B | 25664B | 8 | 25616 |
| `%128` | `shipClass` | 32B | 6624B | 3 | 6592 |

So `this`, `type`, `race` are dequeued first and each takes a callee-saved
register; `shipClass` (span 6.6 kB, 25 % of the function) is dequeued **last**
and is the interval the allocator chooses to spill.  This is a property of the
*IR live ranges*, not of how the four cleanup pointers are spelled: every
declaration/scope/const/reference form below leaves the same four def/last-use
points, so the same queue order and the same spill fall out.

The original's `shipClass` interval, by contrast, is **split** into three
pieces (`r5` -> `sl` -> `r6`), each with a high spill weight
`uses / (size + 25*InstrDist)`; splitting is what lets it stay in registers
while `race` (one long low-weight interval) is spilled globally.  Nothing in
the source spelling of the cleanup pointers changes the split decision.

## 3. Variants measured this pass (official one-function scorer)

All rows are 2764 B with the 336 B / 57-row / `ttype 0xff` LSDA unless noted.

### 3.1 Declaration / scope / const / reference forms (the assignment's ask)

| variant | % | prologue prefix |
|---|---:|---|
| **HEAD** | **70.5415** | `str r2,[sp,#60]` shipClass |
| `Player *const player` | 70.5415 | unchanged |
| `auto *player` | 70.5415 | unchanged |
| `Player &player = *new Player(...)` | 70.5415 | unchanged |
| `Array<BoundingVolume *> *const volumes` | 70.5415 | unchanged |
| `auto *volumes` | 70.5415 | unchanged |
| `Array<BoundingVolume *> &volumes = *new Array<...>()` | 70.5415 | unchanged |
| `new Array<BoundingVolume *>` (no parens) | 70.5415 | unchanged |
| `PlayerFighter *const fighter` / `auto *fighter` | 70.5415 | unchanged |
| `PlayerFixedObject *const fixed` / `auto *fixed` | 70.5415 | unchanged |
| both `auto` | 70.5415 | unchanged |
| `KIPlayer *ship` -> `PlayerFixedObject *ship` (+ cast) | 70.5415 | unchanged |
| `ship = (KIPlayer *) (fighter = new PlayerFighter(...))` (direct assign) | 70.5415 | unchanged |
| `ship = (PlayerFixedObject *) (fixed = new PlayerFixedObject(...))` | 70.5415 | unchanged |
| both direct-assign | 70.5415 | unchanged |
| `fighter`/`fixed`/`volumes` hoisted to function scope (`= nullptr`) | 70.5415 | unchanged |
| `volumes` hoisted to function scope | 70.5415 | unchanged |
| `Vector pos` -> `Vector pos(x,y,z)` / `Vector(...)` / `const Vector` / `Vector pos; pos.x=...` | 70.5415 | unchanged |
| `float px,py,pz` instead of `Vector pos` | 70.5415 | unchanged |
| `int sc = shipClass;` before switch/empPoints/empTime/hitpoints, `sc` used after | 70.5415 | unchanged |
| `if (0 == shipClass)` / `if (shipClass != 0)` / `bool isFighter` | 70.5415 | unchanged |
| `Level *self = this;` at the tail | 70.5415 | unchanged |
| `ship` declared at function top | 70.5415 | unchanged |
| `kiFighter` temp removed, `fighter->...` directly | 70.5415 | unchanged |
| `missionPtr != 0x17` / `!(missionPtr == 1 \|\| missionPtr == 23)` | 70.5415 | unchanged |
| `armor` as `if` / `bool hardcore` / inline expression | 70.5415 | unchanged |
| `jit`/`wp` declared then assigned (separate statement) | 70.5415 | unchanged |

**Every one of these leaves the prologue byte-identical.** The compiler
coalesces the copies, so the live intervals and the Greedy queue are
unchanged.

### 3.2 Forms that DO move the prologue (but lose score)

| variant | % | prologue prefix | why |
|---|---:|---|---|
| `jit` block moved to the top of the body | 63.6637 | `str r1,[sp,#60]` race, `mov r9,r0`, `ldr r0,[pc,#560]`, `mov r6,r3`, `mov sl,r2`, `ldr r4,[r0,#0]` | matches 5 of the original's 7 prologue ops (race spilled, this->r9, &status->r4, rnd->r5) but reorders the whole entry schedule |
| `Globals::status == nullptr` guard moved to the top | 57.8947 | `str r1,[sp,#60]`, `mov r9,r0`, `ldr r0,[pc,#572]` (first 3 exact) then `mov sl,r3`, `ldr r6,[r0,#0]`, `cbz` | guard's `cbz` sits in the entry block |
| no `Globals::status == nullptr` guard | 63.5589 | `strd r1,r2,[sp,#56]` | `&Globals::status` -> `r6`, `rnd` -> `r4`, `nextInt` -> `r5` (wave-119 w6951 finding) |
| no guard + all-int coords | 62.0518 | `str r3,[sp,#60]` type, `mov r9,r0`, `str r1,[sp,#56]`, `mov sl,r2` | type spilled, shipClass -> `sl` |
| two-call `getLevel()>20?20:getLevel()` clamps (both) | 68.6478 | unchanged | faithful clamp shape, but costs 8 register ops |
| two-call `level` only / `empPoints` only | 68.6508 / 68.3532 | unchanged | |
| `volumes` created before `fixed` | 61.3387 | `str r2,[sp,#56]`, `mov r5,r0` | swaps the volumes/fixed live ranges |
| all-int coordinates (`(float)` at ctors) | 61.4770 | `str r2,[sp,#56]`, `mov r9,r3` | |
| all-float coordinates | 57.1855 | `strd r2,r1,[sp,#56]` | |
| `Vector pos` computed before `setEmpData` | 61.3 (source order; clang still sinks it) | unchanged | |
| `jitter` declared `Z,X,Y` (wrong pairing) | 70.5765 | unchanged | 1005 insns — the +0.035 is purely the smaller denominator, not more matches |

### 3.3 Top-statement permutation sweep

All `7!/(3!·3!) = 35` orderings of `{mission, wz, wy, wx, jX, jY, jZ}` (keeping
the wp and jitter relative orders) were scored.  The checked-in
`mission, wz, wy, wx, jX, jY, jZ` is the **maximum** at 70.5415; the next best
is `mission, jX, jY, jZ, wz, wy, wx` at 69.4486.  Many permutations start with
`str r1,[sp,#60]` (race spilled) — e.g. `jX,jY,jZ,mission,wz,wy,wx`,
`mission,wz,jX,jY,jZ,wy,wx`, `jZ,jX,jY,mission,wz,wy,wx` — but all of them
reorder the entry schedule and score 55–69.

## 4. Remaining root cause

The four real-`new` cleanup pointers (`Player` in `r5`, `PlayerFighter` in
`sl`, `PlayerFixedObject` in `r9`, `Array<BoundingVolume*>` in `fp`) plus the
four incoming arguments (`this`, `race`, `shipClass`, `type`) and the two
long-lived globals (`&Globals::status`, `mission`) exceed the seven usable
callee-saved registers (`r4,r5,r6,r8,r9,sl,fp`; `r7` is the frame pointer).
Greedy RA enqueues by live-range size (largest first), so `shipClass` — whose
interval ends at the first `shipClass` branch, ~25 % into the function — is
dequeued last and spilled.  The original instead **splits** that interval
(`r5 -> sl -> r6`) and spills the long, low-weight `race` interval.

No source-level declaration/scope/const/reference form around the four cleanup
pointers changes the four def/last-use points, so no form changes the queue
order or the spill choice.  The only levers that move the prologue are
semantic/scheduling changes (drop the null guard, reorder the top statements,
all-int coordinates, `volumes` before `fixed`), and every one of them costs
more score than the prologue match buys.

The next plausible lever is not source shape: the original's `level` /
`empPoints` clamps are two-call branch diamonds (`c0286`+`c0296`,
`c0316`+`c0324`) and the original computes `x`/`y`/`z` **before**
`setEmpData` (`c03c8`..`c03e4`), while HEAD's clang sinks the coordinate block
after the call (`c03c0`..`c03e0`).  Both are structural IR differences that
feed the RA cascade, but making either faithful drops the score to 68.6 / 61.3
under the 2764 B + 336 B LSDA constraint.

## 5. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

MIR live intervals:

```
<clang++> ... -c src/game/world/Level.cpp \
  -mllvm -print-machineinstrs -mllvm -filter-print-funcs=_ZN5Level10createShipEiiiP8Waypointbb
```
