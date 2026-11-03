# Radar::draw — asteroid-loop `dead`/`wreck` source order (w4005)

**Status:** research note, 2026-11-03. Worktree `wt/w4005`, branch `wave90-w4005`,
base `360c7467`. No source change landed (no variant raises `byte_exact`); this
commit contains this note only.

## Baseline (confirmed before and after)

```
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
                       -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`Radar::draw` = `osize 9908 usize 9908 pct 73.9 byte False linked False`.
Normalized-instruction difflib (`autojunk=False`): **3492 orig / 3488 ours
instructions, 2580 matching, ratio 0.7393**.

## 1. New result: the asteroid-loop prologue is a real source-order divergence

The original's asteroid loop computes the "dead" predicate **twice**, with
`hasCargo` tested *first*:

```
12faa6 ldrb.w r0,[r6,#76]   ; asteroid->hasCargo
12faaa cbz    r0,12fabc
12faac mov    r0,r6
12faae blx    isDead
12fab2 cbnz   r0,12fad4      ; if (hasCargo && dead) -> update
12fab4 mov    r0,r6
12fab6 blx    isDying
12faba cbnz   r0,12fad4
12fabc mov    r0,r6
12fabe blx    isDead         ; second dead evaluation
12fac2 cmp    r0,#0
12fac4 bne.w  12fcd2         ; dead && !hasCargo -> continue
12fac8 mov    r0,r6
12faca blx    isDying
12face cmp    r0,#0
12fad0 bne.w  12fcd2
12fad4 mov    r0,r4          ; this->update(asteroid)
12fad6 mov    r1,r6
12fad8 blx    Radar::update(KIPlayer*)
```

That is the codegen of

```cpp
if (!(asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying())) &&
    (asteroid->isDead() || asteroid->isDying())) {
    continue;
}
this->update(asteroid);
```

i.e. `!(hasCargo && dead) && dead`, **not** the current source's
`bool dead = ...; if (dead && !hasCargo) continue;` (which evaluates `dead`
first and only once, `12fa8c..12faa2` in the baseline).

Replacing the current three lines (src/game/weapons/Radar.cpp:1085-1088) with
the `!(hasCargo && dead) && dead` form makes the **18-instruction prologue
instruction-for-instruction identical** to the original (only the branch
targets differ, as expected from the shifted layout):

```
  ldrb.w r0,[r6,#76]        | ldrb.w r0,[r6,#76]
! cbz    r0,<continue>      | cbz    r0,<continue>
  mov    r0,r6              | mov    r0,r6
  blx    isDead             | blx    isDead
! cbnz   r0,<update>        | cbnz   r0,<update>
  mov    r0,r6              | mov    r0,r6
  blx    isDying            | blx    isDying
! cbnz   r0,<update>        | cbnz   r0,<update>
  mov    r0,r6              | mov    r0,r6
  blx    isDead             | blx    isDead
  cmp    r0,#0              | cmp    r0,#0
! bne.w  <continue>         | bne.w  <continue>
  mov    r0,r6              | mov    r0,r6
  blx    isDying            | blx    isDying
  cmp    r0,#0              | cmp    r0,#0
! bne.w  <continue>         | bne.w  <continue>
  mov    r0,r4              | mov    r0,r4
  mov    r1,r6              | mov    r1,r6
```

The same fix also makes the post-`update` `wreck` recomputation match
(`hasCargo; cbz; isDead; cbz; mov sl,#1; ...; isDying; mov sl,r0`).

DeepOpen's J2ME `Radar.java` (line 705) writes the same test in the positive
form, `(hasCargo && (isDead || isDying)) || (!isDead && !isDying)`, with the
body inside the `if`.  Reconstructing that positive form in C++ produces
**byte-identical codegen** to the `!(...) && (...)` form (9928 B, 71.2%,
3495 insns), confirming that the two spellings are the same source shape and
that the divergence from the current `dead && !hasCargo` is real.

Measured (Radar 80:1 weights unchanged):

| variant | usize | pct | insns | match |
|---|---:|---:|---:|---:|
| baseline | 9908 | **73.9** | 3492/3488 | 2580 |
| asteroid `dead` reorder | 9928 | 71.2 | 3492/3495 | 2489 |
| `wreck` before update, reused after | 9888 | 68.3 | 3492/3493 | — |

The reorder is *faithful but not gate-safe alone*: it adds 20 B and re-rolls
the allocator (the whole-function difflib ratio falls because the extra
instructions move the asteroid block's neighbours). It is the same phenomenon
as the `inSight` reorder: a locally correct source shape that cannot be landed
until the block placement of the enemy/gas-cloud region also matches.

The size change is fatal to the gate even though `linked_exact` is unchanged:
`Radar::draw` grows 9908 -> 9928, which shifts every later function's address
and therefore every PC-relative literal-pool load, so a full `check` reports

```
verify: byte 2701  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
  FAIL  byte_exact 4498 -> 2701
RATCHET FAIL
```

This is why the asteroid reorder must be landed together with a compensating
change that restores the 9908 B size (most likely the enemy-block layout
flip), not on its own.

The alternative "hoist `bool wreck = hasCargo && dead` before the continue
check and reuse it after `update`" is **not** the original: it materialises
`wreck` (`mov r9,#1`) before `update`, which the original does not do
(9888 B, 68.3%).

## 2. The duplicate autopilot block is confirmed by the call multiset

Removing the second `if (!alienOrbit) { getAutoPilotTarget(); ... }` after the
planet loop (src/game/weapons/Radar.cpp:612-621) gives `usize 9740 pct 62.3
insns 3424`. The original is 9908 B / 3492 insns, so nodup is 168 B / 68
instructions short.

The call multiset of nodup vs the original differs by exactly **four** calls:

```
orig  nodup  callee
  9      8   PaintCanvas::DrawString(uint, String const&, int, int, bool)
 17     16   KIPlayer::isDead()
 13     12   KIPlayer::isDying()
  7      6   FModSound::play(int, Vector*, Vector*, float)
```

All four are in blocks whose *placement* differs (the landmark `DrawString`
tail-merge, the asteroid `dead` pair, and the final music `play` block at
`0x1305da..0x1306a2`). There is no missing call site: removing the duplicate
autopilot block is correct, and the 168 B is the enemy/asteroid/landmark block
placement, not absent code.

## 3. Enemy-block fall-through: expect flips it, but re-rolls the body

The original's enemy condition is

```
12f3ac blx isDockingToAsteroid
12f3b0..12f3c6  normalise three bools
12f3c8 bne.n 12f40e           ; A -> skip
12f3ca blx isDockingToDockingPoint
12f3d4..12f3da or turretMode / otherLock
12f3dc bne.n 12f40e           ; (B||C||D) -> skip
12f3de body-start (navWreck)  ; fall-through
```

The IR (`radar.ll`) is `br i1 %isDockingToAsteroid, label %skip, label %cond2`
with **no `!prof` metadata**; with a 50/50 tie MachineBlockPlacement takes the
first successor (`skip`), which is why ours lays out the skip path first.
`__builtin_expect(!isDockingToAsteroid(), 1)` flips the tie and gives the
original's local layout:

```
12f3cc blx isDockingToAsteroid
12f3d0 cbnz r0,12f41e         ; A -> skip
12f3d2..12f3ea cond2
12f3ea cbnz r5,12f41e
12f3ee cmp r0,#0
12f3f0 bne.w 12f5aa           ; B -> skip
12f3f4 body-start             ; fall-through
```

but the body-start then branches to nearby `12f418`/`12f422` instead of the
original's far `12f91a`/`12fe6c`, because the gas-cloud block is placed
between the body-start and the body main in the original and after the body
main in ours. Measured:

| variant | usize | pct | insns |
|---|---:|---:|---:|
| `__builtin_expect(!A,1)` | 9872 | 61.4 | 3492/3460 |
| `__builtin_expect(!A,1)` + expect on cond2 | 9892 | 68.0 | 3492/3477 |
| `__builtin_expect(!A,0)` | 9916 | 65.0 | 3492/3489 |
| `__builtin_expect(whole cond,1)` | 9892 | 68.0 | 3492/3477 |
| `__builtin_expect(whole cond,0)` | 9908 | 64.9 | 3492/3489 |
| nested `if`, `||`-chain, bool var, early goto | 9908 | 73.9 | 3492/3488 |

The last row is the key negative result: every source-form rewrite of the
condition canonicalises to the *same* IR (verified by `-emit-llvm`: the branch
is still `br i1 %A, label %skip, label %cond2`), so only a weight change can
flip the layout, and every weight change re-rolls the body.

## 4. Combining the correct local shapes

Applying the asteroid `dead` reorder + the enemy expect + the `inSight`
distance-first reorder (the three locally-correct shapes) gives a
**size-exact** 9908 B function, but only 65.9%:

| variant | usize | pct | insns |
|---|---:|---:|---:|
| dead + expect + inSight | **9908** | 65.9 | 3492/3493 |
| dead + inSight | 9944 | 70.4 | 3492/3502 |
| dead + nodup | 9760 | 61.8 | 3492/3432 |
| dead + nodup + expect + inSight | 9732 | 56.6 | 3492/3430 |

A weight sweep of the dead+expect shape (Radar.cpp's `-mllvm
-likely-branch-weight/-unlikely-branch-weight`) found no better point:
2:1 → 9896/67.7, 4:1 → 9900/66.2, 8:1 → 9896/66.3, 2000:1 → 9892/56.5.

## 5. Root cause / next step

The remaining 26% is still whole-function **basic-block placement + register
allocation**, not a wrong constant, callee, field or missing body. The new,
durable finding is that the asteroid-loop `dead` predicate is
`!(hasCargo && dead) && dead` (18-instruction exact match), which should be
landed together with the enemy-block fall-through and the `inSight` reorder in
one pass, once the enemy/gas-cloud block placement is reproduced. Landing any
one of them alone lowers the fuzzy score even though it makes its own block
instruction-identical.

## 6. Evidence locations

Scratch (not committed): `/dev/shm/w4005/` — `orig_full.txt`, `ours_full.txt`,
`nodup_full.txt`, `dr_full.txt`, `aligned*.txt`, `radar.ll`,
`radar_nested.ll`. The source was restored to `360c7467` before committing.
