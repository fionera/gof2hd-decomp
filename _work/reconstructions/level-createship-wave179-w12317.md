# `Level::createShip` — wave 179 (w12317): fighter-branch LOD layout, raw 1,225 -> 1,221

Worktree `wt/w12317`, branch `wave179-w12317`, base `master 64014ce6f`
(wave 178 note).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 `-Oz`).

**Status: source change landed.**  In the `shipClass == 0` (fighter) branch the
`missionPtr` LOD guard gains a branch hint:

```cpp
if (__builtin_expect(missionPtr != 1, 1) && missionPtr != 23) { // lint: arm_builtin
```

`createShip` raw **1,225 -> 1,221** (-4), aligned **383 -> 394** (+11),
size **2,764/2,764** exact, insns **1,117/1,117** (exact).  `.text`
differing bytes **77,936 -> 77,932** (-4).  `.ARM.extab` stays the original
**336 B / 57 rows / `ttype 0xff`** cleanup-only record, size- and base-exact
(`0x26298` @ `0x1a3b78`), `.ARM.exidx` byte-identical, `.rel.plt`
byte-identical (0 differing bytes).

## 1. Result

| metric | before (wave 178) | after |
|---|---:|---:|
| raw same-address diff | **1,225** | **1,221** |
| instruction-index alignment | **383** | **394** |
| size | 2,764 | 2,764 |
| insns (orig/ours) | 1,117 / 1,117 | **1,117 / 1,117** |
| `.text` differing bytes | 77,936 | **77,932** |
| `.ARM.extab` size | `0x26298` (exact) | `0x26298` (exact) |
| `.ARM.extab` differing bytes | 700 | 700 |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rel.plt` | byte-identical | byte-identical |

Full `check`: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, symbol parity 5370/5370,
`RATCHET PASS`.

## 2. The landed change

The original lays the `missionPtr` LOD block out as the **fall-through** of the
guard, with the `type == 0x2c || type == 0x31` block out-of-line:

```
c071e: ldr  r6,[sp,#56]      ; this
c0720: ldr  r0,[r6,#192]     ; missionPtr
c0724: cmp  r0,#1
c0726: it   ne
c0728: cmpne r0,#23
c072a: beq  c073c            ; ==1 || ==23 -> skip LOD
c072c: ldr  r1,[sl,#12]      ; parentGeometry   (fall-through: addObject)
c0730: ldr  r0,[r6,#0]       ; lodManager
c0732: cbnz r1,c0738
c0734: ldr  r1,[sl,#8]       ; geometry
c0738: blx  LODManager::addObject
c073c: cmp  r4,#44           ; type block
```

The wave-178 body emitted the inverse layout (`bne` over the type block to an
out-of-line LOD block, then `b.n` back).  `MachineBlockPlacement` picks the
fall-through from the branch-probability model: for `a && b` with two 50/50
comparisons the then-block is 25% likely, so the 75% else-block (the type
block) became the fall-through.  Hinting the first comparison likely flips the
model and reproduces the original's layout.  The hint is semantically free
(`&&` is commutative over side-effect-free `int` comparisons) and the waiver
is same-line, so `lint` stays CLEAN (Level.cpp's `arm_builtin` violation count
is unchanged: the new occurrence is waived).

Measured variants of the same guard:

| form | raw | aligned |
|---|---:|---:|
| `missionPtr != 1 && missionPtr != 23` (wave 178) | 1,225 | 383 |
| `missionPtr != 1 && __builtin_expect(missionPtr != 23, 1)` | 1,222 | 377 |
| `__builtin_expect(missionPtr != 1 && missionPtr != 23, 1)` | 1,222 | 377 |
| `missionPtr != 23 && __builtin_expect(missionPtr != 1, 1)` | 1,221 | 394 |
| **`__builtin_expect(missionPtr != 1, 1) && missionPtr != 23` (landed)** | **1,221** | 394 |

The landed form is the best raw.  It reverses the compare order (`cmp #23;
cmpne #1`) relative to the original (`cmp #1; cmpne #23`), which is why the
aligned count rises; the raw same-address metric is the project's ground truth
and is 1 byte better than the compare-order-preserving form.  Every variant
keeps size 2,764 and `.ARM.extab` `0x26298`.

## 3. LSDA analysis (durable finding)

Decoding the original's `.ARM.extab` call-site table
(`_work/reconstructions/extab_lsda_decode.py`) shows that its cleanup landing
pads belong **only to `new` expressions**, not to guard objects around plain
calls:

| original row | offset range | landing pad | call |
|---|---|---:|---|
| 0 | 0..406 | 0 | getCurrentCampaignMission, nextInt x3, getLevel x4, gameWon, inAlienOrbit, hardCoreMode, operator new Player |
| 1 | 406..436 | 2488 | Player ctor |
| 2 | 436..530 | 0 | setEmpData, operator new PFO |
| 3 | 530..562 | 2482 | PlayerFixedObject ctor |
| 5 | 570..574 | 2476 | Array ctor |
| 7..55 (odd) | ... | 2448..2422 | BoundingAAB ctors |
| 29 | 1224..1256 | 2470 | PlayerFighter ctor |
| 56 | 2316..2764 | 0 | getShipGroup, setShipGroup, addObject, setBV, setLevel |

i.e. the original has **no `EmpGuard`-style cleanup** for
`getCurrentCampaignMission`, `setEmpData` or `setLevel`; its 29 cleanup rows
are the `new Player`, `new PlayerFixedObject`, `new Array`, 24-25
`new BoundingAAB` and `new PlayerFighter` expressions.  A no-guard rewrite
(`int mission = ...;` / plain `setEmpData` / plain `setLevel`, and
`new PlayerFixedObject(...)` / `new PlayerFighter(...)` instead of
`::operator new` + placement new) reproduces the original's first two LSDA
rows exactly and drops the `.ARM.extab` diff from 148 to 93 bytes, but it
costs raw text (1,980) because the scheduler then sinks the coordinate
computation *after* `setEmpData` and the whole register allocation shifts.
The checked-in `EmpGuard` shape is therefore kept: it happens to produce the
same 57-row / 336-byte record and a much smaller raw text diff.

## 4. Sweeps that did not beat 1,221 (all size 2,764, extab `0x26298`)

* **Jitter pairing** (which `nextInt` result feeds x/y/z).  Original is
  J123 (x=first, y=second, z=third).  On the wave-178 base: J123 1,228,
  J132/J312 1,226, J231 1,227, J321 (checked-in, x=third) 1,225.  On top of
  the landed guard hint: J123 1,224, J132/J312 1,222, J231 1,223, J321 1,221.
* **Coordinate int/float combinations** (x,y,z each int or float).  Only
  `int x, float y, float z` (checked-in) keeps size 2,764; `fif` 1,230,
  `ffi` 1,233, `iii` 2,238 (size 2,752), `iif/ifi/fii` 2,477-2,480 (size
  2,780), `fff` 2,466 (size 2,732, extab `0x2629c`).
* **Coordinate statement order** (6 permutations) and declaration order of
  `jitter1/2/3`: all canonicalise to the same code as the corresponding
  pairing.
* **`__builtin_expect` on other branches** (`waypoint != nullptr`,
  `shipClass == 0/1`, `type == 14`, `mission == 0x9a`, the mission-49 block,
  the `parentGeometry` ternary, `type == 0x2c || 0x31`): all either no-op or
  worse (2,029-2,565, several change size/extab).
* **Tail statement order** in the fixed branch (`setShipGroup`, `addObject`,
  `field_0x40 = 1`, `setBV`, `ship = fixed`): the landed wave-177 order is
  still the best; the other 119 permutations are 1,221-1,223 raw.
* **`fixed`/`volumes` allocation levers** (`volumes` first, split
  declarations, `= nullptr` then assign, `new Array<...>` without `()`,
  Array-new before the PFO ctor): all 2,219-2,220 raw, size 2,760
  (not exact) except the no-op forms.
* **`Level *self = this;` alias**, direct `fighter->parentGeometry` instead
  of the `kiFighter` local, `static_cast` return: all 1,221 (no change).

## 5. Remaining mismatch structure

The residual 1,221 bytes are still dominated by the fixed (`shipClass == 1`)
branch, `0xc03d4..0xc06d4` and `0xc0a54..0xc0b94`, where the `BoundingAAB`
argument setup differs by a pure register-allocation swap: the original keeps
`fixed` in `r9` and `volumes` in `fp`, ours the reverse, so every
`ldr.w r0,[fp,#4]` becomes `ldr.w r0,[r9,#4]` and the float-bit registers
become `r6`/`sl` instead of `sl`/`r4`.  The trigger remains the coordinate
block: the original keeps `int x,y,z` in `fp/r4/r8` across `setEmpData` and
converts them afterwards, while our build converts y/z before the call and
sinks x below it, leaving `sl` free for `this`.  The `0xc0314` bucket
(26 bytes) is the `this` spill placement (`0xc031c` vs `0xc0332`).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12317
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target lint -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12317/`: `Level_base.cpp`
(wave-178 base), `h.py` (build + raw/aligned/size/extab harness), `sweep.py`
.. `sweep14.py` (pairing, int/float, expect-hint, tail and allocation
sweeps), `buckets.py` (per-0x40 raw-diff histogram), `v_mpA.cpp` (landed),
`v_mp_e2.cpp` (compare-order-preserving alternative), `v_noguard_new.cpp`
(no-guard / new-expression LSDA experiment), `lsda.txt`/`lsda2.txt`
(call-site-table dumps).
