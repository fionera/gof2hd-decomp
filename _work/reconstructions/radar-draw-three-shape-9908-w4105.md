# Radar::draw — three correct source shapes at constant 9908 B (w4105)

**Status:** research note, 2026-11-03. Worktree `wt/w4105`, branch `wave91-w4105`,
base `a4557300`. No source change landed: the three-shape variant is size- and
gate-neutral (`byte_exact` stays 4498), so per the pass instruction it is not
landed. This commit contains this note only.

## Baseline (confirmed before and after)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
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

## 1. The exact patch for the three shapes

```diff
@@ -790,9 +790,9 @@ enemies:
                     int cx = (int) PlayerEgo::crosshairPos.x;
                     int cy = (int) PlayerEgo::crosshairPos.y;
                     bool inSight = false;
-                    if (this->screenX > cx - this->field_0x124 && this->screenY > cy - this->field_0x124 &&
-                        this->screenX < cx + this->field_0x124 && this->screenY < cy + this->field_0x124 &&
-                        AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f) {
+                    if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
+                        this->screenX > cx - this->field_0x124 && this->screenX < cx + this->field_0x124 &&
+                        this->screenY > cy - this->field_0x124 && this->screenY < cy + this->field_0x124) {
                         inSight = true;
                     }
@@ -863,8 +863,8 @@ enemies:
         }
         alwaysEnemySeen = alwaysEnemy;
 
-        if (!this->level->getPlayer()->isDockingToAsteroid() &&
-            !(this->level->getPlayer()->isDockingToDockingPoint() || turretMode || otherLock)) {
+        if (__builtin_expect(!this->level->getPlayer()->isDockingToAsteroid() && // lint: arm_builtin
+            !(this->level->getPlayer()->isDockingToDockingPoint() || turretMode || otherLock), 1)) {
             bool navWreck = this->dockNavPtr != nullptr && this->dockNavPtr->hasCargo &&
                             (this->dockNavPtr->isDead() || this->dockNavPtr->isDying());
@@ -1093,8 +1093,8 @@ asteroids:
             bool asteroidHit = false;
             for (uint32_t i = 0; i < this->asteroids->size(); ++i) {
                 PlayerAsteroid *asteroid = (PlayerAsteroid *) this->asteroids->data()[i];
-                bool dead = asteroid->isDead() || asteroid->isDying();
-                if (dead && !asteroid->hasCargo) {
+                if (!(asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying())) &&
+                    (asteroid->isDead() || asteroid->isDying())) {
                     continue;
                 }
                 this->update(asteroid);
```

The `// lint: arm_builtin` waiver must sit on the line containing the
`__builtin_expect` token; a waiver on the continuation line does not count
(the linter is line-based). With it, `goal_lint` is clean.

## 2. Measured variants (official verifier, `autojunk=False`)

All rows use the baseline Radar source plus the listed edits. `usize` is the
linked `Radar::draw` size; `pct` is the verifier's normalized-instruction
difflib ratio.

| variant | usize | pct | gate byte_exact |
|---|---:|---:|---:|
| baseline | 9908 | **73.9** | 4498 |
| asteroid `dead` reorder only | 9928 | 71.2 | 2701 (size shift) |
| `inSight` distance-first, x/y interleaved only | 9952 | — | size shift |
| `inSight` distance-first, x-pair then y-pair only | 9924 | 71.2 | 2794 (size shift) |
| `dead` + `inSight` (x-pair then y-pair) | 9944 | 70.4 | 2794 |
| `dead` + `inSight` + enemy `__builtin_expect(...,1)` | **9908** | 65.9 | **4498** |

The last row is the only combination tested that restores the exact 9908-byte
size. Three equivalent `__builtin_expect` spellings produce the same code:
wrapping the whole condition, wrapping `!(A || B || C || D)`, and wrapping
both `!A` and `!(B || C || D)` separately. All three are gate-neutral:
`byte_exact` 4498 -> 4498, `linked_exact` 4498, `data_exact` 843/843,
imports 207, lint clean, drift 0.

## 3. Asteroid `dead` reorder: instruction-for-instruction match

The variant's asteroid-loop prologue is instruction-for-instruction identical
to the original (only branch targets differ, as expected from the shifted
layout). Original (`0x12faa6`, `Radar::draw` base `0x12e050`):

```
12faa6 ldrb.w r0,[r6,#76]   ; hasCargo
12faaa cbz    r0,12fabc
12faac mov    r0,r6
12faae blx    isDead
12fab2 cbnz   r0,12fad4
12fab4 mov    r0,r6
12fab6 blx    isDying
12faba cbnz   r0,12fad4
12fabc mov    r0,r6
12fabe blx    isDead         ; second dead evaluation
12fac2 cmp    r0,#0
12fac4 bne.w  12fcd2
12fac8 mov    r0,r6
12faca blx    isDying
12face cmp    r0,#0
12fad0 bne.w  12fcd2
12fad4 mov    r0,r4          ; this->update(asteroid)
12fad6 mov    r1,r6
12fad8 blx    Radar::update(KIPlayer*)
```

Variant (object-relative, `Radar::draw` at 0):

```
1c04 ldrb.w r0,[r6,#76]
1c08 cbz    r0,1c1a
1c0a mov    r0,r6
1c0c bl     isDead
1c10 cbnz   r0,1c32
1c12 mov    r0,r6
1c14 bl     isDying
1c18 cbnz   r0,1c32
1c1a mov    r0,r6
1c1c bl     isDead
1c20 cmp    r0,#0
1c22 bne.w  1e22
1c26 mov    r0,r6
1c28 bl     isDying
1c2c cmp    r0,#0
1c2e bne.w  1e22
1c32 mov    r0,r4
1c34 mov    r1,r6
1c36 bl     Radar::update(KIPlayer*)
```

The same source shape also makes the post-`update` `wreck` recomputation
(`hasCargo; cbz; isDead; cbz; mov sl,#1; ...; isDying; mov sl,r0`) match the
original.

## 4. `inSight`: distance first, then x-pair, then y-pair

The original (`0x12ef4c`) evaluates the distance first, then both X bounds,
then both Y bounds:

```
12ef5a vldr s26,[r0]          ; crosshair x
12ef5e vldr s24,[r0,#4]       ; crosshair y
12ef64 ldrd r2,r1,[sp,#108]   ; egoPos, targetPos
12ef68 blx  AEMath::operator-
12ef6e vcvt s24,s24           ; convert AFTER the call
12ef72 vcvt s26,s26
12ef76 blx  AEMath::VectorLength
12ef80 vcmpe s0,s20           ; < 60000.0f
12ef88 bpl  12efc0
12ef8a vmov r1,s26            ; x
12ef8e ldr  r0,[r4,#292]      ; field_0x124
12ef92 ldr  r2,[r4,#252]      ; screenX
12ef98 subs r3,r1,r0
12ef9a cmp  r2,r3
12ef9c ble  12efc0
12ef9e add  r1,r0
12efa0 cmp  r2,r1
12efa2 bge  12efc0
12efa4 vmov r1,s24            ; y
12efa8 ldr  r2,[r4,#256]      ; screenY
12efac subs r3,r1,r0
12efae cmp  r2,r3
12efb0 ble  12efbe
12efb2 add  r0,r1
12efb4 movs r6,#0
12efb6 cmp  r2,r0
12efb8 it   lt
12efba movlt r6,#1
```

This corrects the w4005 note's shorthand: the distance-first form with the
screen terms still interleaved x/y is **9952 B** (not 9944). The +16 B figure
is the distance-first **x-pair then y-pair** form on top of the `dead` reorder
(9928 + 16 = 9944), matching the original's comparison order.

## 5. Enemy fall-through: the IR has no `!prof`, and a 50/50 tie takes the first successor

The current source's condition is `!A && !(B || C || D)`. Clang 7 at `-Oz`
canonicalises every plain source spelling (nested `if`, `&&` chain, early
`goto`, `bool` variable, `!(A || B || C || D)`) to the same IR:

```
%1173 = call zeroext i1 @_ZN9PlayerEgo19isDockingToAsteroidEv(...)
br i1 %1173, label %skip, label %cond2
```

There is **no `!prof` metadata** on that branch. MachineBlockPlacement's 50/50
tie therefore takes the first successor (`skip`) as the fall-through, which is
why the baseline lays the skip path first. The original lays the body first:

```
12f3ac blx  isDockingToAsteroid
12f3b0..12f3c6  normalise three bools
12f3c8 bne.n 12f40e          ; A -> skip
12f3ca blx  isDockingToDockingPoint
12f3d4..12f3da or turretMode / otherLock
12f3dc bne.n 12f40e          ; (B||C||D) -> skip
12f3de body-start            ; fall-through
```

Adding `__builtin_expect(whole condition, 1)` is the only source-level way to
flip the tie. It produces `!prof = !{!"branch_weights", i32 1, i32 80}` on the
first branch (false/body likely) and makes the body the fall-through:

```
1390 bl     isDockingToAsteroid
1394 cmp    r0,#0
1396 bne.w  skip
139a..13ba  cond2
13be body-start             ; fall-through
```

The cost is a whole-function block re-roll: the gas-cloud loop moves from
`0x13fc` (baseline) to `0x15c0` (variant), and the fuzzy ratio drops
73.9 -> 65.9 even though the three local blocks are now correct. The
`__builtin_expect` is therefore an anchor justified by the original's
fall-through layout, not a substitute for understanding the branch.

## 6. Gate result and why the code was not landed

With the patch in section 1:

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`byte_exact` does **not** rise (4498 -> 4498), so per the pass instruction
("land only if byte_exact rises") the patch is not landed. It is recorded here
so the next Radar pass can apply it verbatim when the block placement /
register-allocation work makes the whole function byte-exact.

## 7. Evidence locations

Scratch (not committed): `/dev/shm/w4105/` — `Radar.baseline.cpp`,
`radar_variant.diff`, `radar.ll`, `radar_whole1.ll`, `orig_radar.txt`,
`ours_radar.txt`, `variant_draw.txt`, `variant_radar.txt`,
`variant_show.txt`, `check_variant.txt`.
