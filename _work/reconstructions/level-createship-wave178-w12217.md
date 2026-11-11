# `Level::createShip` — wave 178 (w12217): order/branch sweep, no win (raw 1,225 unchanged)

Worktree `wt/w12217`, branch `wave178-w12217`, base `master 1b48f990f`
(wave 177).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 `-Oz`).

**Status: note-only — no `src/` change landed.**  The wave-177 body is kept.
`createShip` raw **1,225**, aligned **383**, size **2,764/2,764**,
insns **1,117/1,117**.  `.text` differing bytes **79,251**,
`.ARM.extab` **731** differing bytes with the section size/base exact
(`0x26298` @ `0x1a3b78`, createShip record 336 B / 57 rows / `ttype 0xff`),
`.rel.plt` byte-identical (0).

## 1. Gate on the unchanged tree

```
byte_exact 4507/4524   linked_exact 4507/4524   size-exact 4524/4524
stub 0   missing 0   wrong_type 0   extra 0
data 843/843   imports 207   lint CLEAN   drift 0   parity 5370/5370
RATCHET PASS
```

Section byte diff vs the original: `.text` 79,251, `.ARM.extab` 731,
`.note.gnu.build-id` 20.  Every other section is byte-identical.

## 2. What was swept this wave (all full link, exact TU, `-Oz`, clang 7.0.2)

### 2.1 Jitter/coordinate pairing

`DeepOpen` (`Level.java:1572`) computes `spawnX += nextInt(40000) - 20000`
first, then `spawnY`, then `spawnZ`, i.e. **x pairs with the first
`nextInt` result, y with the second, z with the third**.  The checked-in
wave-177 body declares `jitter3, jitter2, jitter1` (first call → `jitter3`)
and computes `x = wpx + jitter1`, so x actually pairs with the **third**
result.  The original disassembly confirms the natural pairing:

```
c026c blx nextInt ; str r0,[sp,#36]   ; result 1
c0276 blx nextInt ; str r0,[sp,#32]   ; result 2
c0280 blx nextInt ; str r0,[sp,#28]   ; result 3
c03cc ldr r0,[sp,#44]  ; wpx
c03ce ldr r1,[sp,#36]  ; result 1
c03d6 add r0,r1        ; x = wpx + result 1
```

The semantically correct source is therefore `int jitter1 = nextInt();
int jitter2 = nextInt(); int jitter3 = nextInt();` with
`x = wpx + jitter1`, `y = wpy + jitter2`, `z = wpz + jitter3`
("J123").  It is size-exact but **raw 1,228 / aligned 425** — 3 bytes
*worse* than the checked-in body's 1,225.  All six jitter declaration
orders with the correct first/second/third pairing compile identically
(the names are irrelevant to the compiler); the difference is purely the
stack-slot colouring.  J123's slots are `wpx=[sp,32] wpy=[sp,48]
wpz=[sp,44] j1=[sp,28] j2=[sp,40] j3=[sp,36]`, the original's are
`wpx=[sp,44] wpy=[sp,48] wpz=[sp,40] j1=[sp,28] j2=[sp,32] j3=[sp,36]`.

### 2.2 Declaration/assignment orders

* `int wpz, wpx, wpy;` declaration order: `zwx` (checked-in) 1,228,
  `zyx` 1,229, `xzy` 1,557, `xyz`/`yzx`/`wyxz` 1,558 (J123 pairing).
  On the checked-in (third-result) pairing every order is 1,225.
* All six `wpx/wpy/wpz` assignment permutations inside the
  `waypoint != nullptr` if/else: identical code (compiler canonicalises).
* `int x = wpx + jitter1 - 20000`, `int x = wpx + jitter1; x -= 20000;`,
  `wpx - 20000 + jitter1`, `jitter1 + wpx - 20000`, and a split
  `int y0 = wpy + jitter2; float y = (float)(y0 - 20000);` — all 1,225.
* `int x,y,z` (all ints, correct pairing): size 2,756 (8 B short), raw
  2,226 — the `this` pointer spills and the whole coordinate block sinks
  below `setEmpData`.
* `volatile int x`: size 3,020.  `volatile` x/y/z: 3,316.

### 2.3 Tail order (all 120 permutations) on the J123 pairing

Best `(1,2,3,0,4)` = `setShipGroup, addObject, field_0x40=1, setBV,
ship=fixed` (the landed wave-177 order) at **1,228 / 425**.  No
permutation beats the checked-in body's 1,225.

### 2.4 Guard reshuffling (the `.ARM.extab` 4-byte blocker)

The 2-guard shape (`setEmpData`+`setLevel`, no `mission` guard) is the
best raw shape known: **1,189 / 356**, size 2,764, but `.ARM.extab`
`0x26294` — **4 B short**.  Measured single-guard `.ARM.extab`
contributions over the no-guard body (`0x26284`):

| guard | Δ extab |
|---|---:|
| `getCurrentCampaignMission` (mission) | +4 |
| `setEmpData` | +4 |
| `setLevel` | +12 |
| `gameWon` / `getLevel` / `empPoints` / `hardCoreMode` / `inAlienOrbit` / `addObject` | +12 |
| `operator new Player` / `setWreckedMeshId` | +8 |

Only the `mission` guard contributes the single 5-byte LSDA row
(start 28 < 128, length 4 < 128, landing pad ≥ 128) that the 2-guard
shape needs, and it costs 36 raw bytes.  Guarding any other call in the
first 128 bytes (`nextInt` at 0x58/0x62/0x6c, first `getLevel` at 0x72)
changes the register allocation badly (size 2,748–2,760 or raw
2,261–2,467).  Guarding `setWreckedMeshId`, `setBV` or `addObject`
gives `0x2629c` (still 4 B short).  Replacing the `mission` guard with
`mission+setEmpData+X` for X ∈ {`gameWon`, `level`, `empPoints`,
`hardCoreMode`, `playerNew`, `getShipGroup`, `addObject`, `setBV`,
`inAlienOrbit`, `setWrecked`} reaches `0x26298` only for `X=empPoints`
(raw 1,226) and `X=level`/`gameWon` (1,250/1,253) — none beats 1,225.

### 2.5 Expression forms (compiler canonicalises, all 1,225)

`int mission = 0;` before the guard; `float zero = 0.0f;` /
`const float zero = 0.0f;` threaded through all 24 `BoundingAAB`
`0.0f` arguments; `int empTime = shipClass == 1 ? 45000 : 15000;`;
moving `empTime = 45000` to the top of the `shipClass == 1` block.

## 3. Root cause of the residual (unchanged from wave 177)

The original computes the integer x/y/z **before** `Player::setEmpData`
and keeps them in core callee-saved registers (`fp`, `r4`, `r8`); the
`vcvt.f32.s32` conversions happen after the call, once per branch
(`c040c..c0422` fixed, `c06c0..c06d8` fighter).  Our build sinks the x
integer add below the call:

```
ours  c03c8..c03e8  y,z int add + sitofp -> s16,s18
      c03ee         blx Player::setEmpData
      c03f6         ldrd r1,r0,[sp,#28]   ; x computed here
      c0402         sub.w r4,r0,r4
```

The machine IR confirms the scheduler moved `%34` (x) into `bb.26`
(`%ir-block.124`, after the invoke) even though the LLVM IR has the add
in block 114 before the invoke.  This cascade produces the fixed-branch
register swap (`fixed` r9↔fp, `volumes` fp↔r9, `race` r6↔r4, `type`
kept vs `[sp,#52]` reload, `0.0f` hoisted vs rematerialised) and the
`this` spill at `0xc031c` vs the original `0xc0332`.  A source-level
lever that pins the x add above the call without changing size was not
found: the only ways to force the value live across the call (all-int
declarations, a float copy used in one branch, `volatile`) change the
size or wreck the allocation.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12217
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12217/`: `Level_base.cpp`
(wave-177 base), `Level_2g.cpp` (2-guard shape), `Level_iii.cpp`
(all-int coordinate shape), `h.py` (build + raw/aligned/size/insn/extab
report), `exp6.py` (jitter×wp declaration sweep), `exp8.py`/`exp9.py`/
`exp10.py`/`exp11.py`/`exp12.py` (guard reshuffle/extab contribution
sweeps), `exp14.py` (assignment-order sweep), `tailsweep_j123.py`
(120-tail sweep on the correct pairing), `cmpseq.py`/`cmp2.py`
(instruction-sequence diffs), `secdiff.py` (section byte diff).
