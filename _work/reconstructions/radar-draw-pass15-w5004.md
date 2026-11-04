# Radar::draw pass 15 — the 80:1 per-file branch-weight override is removable:
# the flag's effect is entirely the landmark `onScreen` expect (w5004)

**Status:** landed change + analysis note, 2026-11-04. Worktree `wt/w5004`, branch
`wave100-w5004`, base `44225192c`.

**Change:** remove the per-file
`-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` override for
`src/game/weapons/Radar.cpp` from `CMakeLists.txt`, and hint the first bounds comparison in
addition to `onScreen`:

```diff
-            if (__builtin_expect(this->onScreen, 1) && // lint: arm_builtin anchor: Radar::draw; plain form shrinks body 9908 -> 9896
-                this->screenX > this->centerX - this->field_0x128 &&
+            if (__builtin_expect(this->onScreen, 1) && // lint: arm_builtin anchor: Radar::draw
+                __builtin_expect(this->screenX > this->centerX - this->field_0x128, 1) && // lint: arm_builtin anchor: Radar::draw
                 this->screenX < this->centerX + this->field_0x128 &&
                 this->screenY > this->centerY - this->field_0x128 &&
                 this->screenY < this->centerY + this->field_0x128) {
```

**Result:** `_ZN5Radar4drawEP6PlayerP3Hudi` raw verifier ratio
**73.925501 % -> 73.868195 %** (`usize 9908` unchanged, normalized stream 3492/3488 both).
The official report percentage is unchanged at **73.9 %**. Gate unchanged:
`byte 4500 / linked 4500 / size 4524 / data 843 / imports 207 / lint CLEAN / drift 0 /
RATCHET PASS`.

The landed source is **flag-independent**: it measures 73.868195 % both with the old 80:1
override and with the default 2000:1 weight, so the override was doing nothing for this shape.

## 0. Baseline (measured before this pass)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN5Radar4drawEP6PlayerP3Hudi` with the 80:1 override:

```
osize 9908  usize 9908  byte False linked False
raw SequenceMatcher ratio 73.925501 %   normalized stream 3492 (orig) / 3488 (ours)
```

With the override removed and the old source, the same function measures
**72.482452 %** (norm 3492/3489); this is the regression the old override was hiding.

## 1. The flag's effect is entirely mediated by the `onScreen` expect

`-likely-branch-weight` / `-unlikely-branch-weight` only change the weights of
`llvm.expect` branches (lowered from `__builtin_expect`).  Removing all three expects makes
the flag irrelevant (`no_all` is 70.935396 % at both 80:1 and 2000:1).  Single-expect
ablations isolate which one matters (official verifier, `autojunk=False`):

| variant | 80:1 | default 2000:1 | flag matters? |
|---|---:|---:|---|
| baseline (all three expects) | **73.925501** | 72.482452 | yes |
| keep only `otherLock` expect | 70.964045 | 70.964045 | no |
| keep only `onScreen` expect | 72.589196 | 71.232092 | **yes** |
| keep only `i==1\|\|i==3` expect | 70.869752 | 70.869752 | no |
| remove `otherLock` expect | 72.169676 | 70.783780 | yes |
| remove `onScreen` expect | 71.021344 | 71.021344 | **no** |
| remove `i==1\|\|i==3` expect | 72.110013 | 70.753366 | yes |
| remove all three | 70.935396 | 70.935396 | no |

The pattern is exact: **if the `onScreen` expect is absent the flag has no effect; if it is
present the flag changes the layout.**  The other two expects only modulate the result.
So the question "can the 80:1 override be removed?" reduces to "can the `onScreen` branch
get the 80:1 block placement at the default 2000:1 weight?".

## 2. The shape that does it: hint the first bounds test too

Sweeping every contiguous prefix/segment of the five-conjunct `&&` chain (and all 31
subsets) with an `__builtin_expect(..., 1)` at the default weight found one family that
reproduces the 80:1 *early* layout: hinting **both** `onScreen` and the first bounds test.

```c++
if (__builtin_expect(this->onScreen, 1) &&
    __builtin_expect(this->screenX > this->centerX - this->field_0x128, 1) &&
    this->screenX < this->centerX + this->field_0x128 &&
    this->screenY > this->centerY - this->field_0x128 &&
    this->screenY < this->centerY + this->field_0x128) {
```

At the default weight this is **9908 B / 73.868195 %** (norm 3492/3488).  It is also
73.868195 % with the 80:1 flag, i.e. the flag is irrelevant for this shape.

The top-level autopilot cold block lands at `0x682` (the 80:1 build's address) instead of
`0xa20` (the old default-weight address), and the landmark-filter blocks at `0x304/0x31c`
land early as in the original.  The **only** remaining difference from the 80:1 build is the
`otherLock` cold-block address: `0x261e` (new shape, default weight) vs `0x2480` (80:1).
`diff` of the two linked disassemblies is otherwise only the branch operands that name that
cold block; both bodies are 3492/3488 normalized instructions.

Other shapes that hint the same branch in a different spelling are equivalent:
`__builtin_expect(onScreen && screenX > ..., 1)` (compound), `expect(onScreen,1)` alone
(72.482452 %), `expect(onScreen,1)` + `expect(b0,1)` (73.868195 %), and the same with
`b2`/`b3` also hinted (73.868195 %).  Hinting `b1` (`screenX < centerX + ...`) instead
drops to 65.8 %; hinting the whole chain grows the body to 10160 B.

## 3. The remaining 0.057 raw points are the `otherLock` cold-block address

The `otherLock` cold block is `otherLock = this->field_0x14 != 0;` (5 instructions:
`ldr/cmp/it/movne/b`).  At 80:1 it is placed at `0x2480`; at the default weight the new
shape places it at `0x261e`, 0x19e bytes later in the cold tail.  The original places it at
`0xb72` (after the landmark loop), which neither build reaches.

This is a block-placement consequence of the `otherLock` expect's weight, not a statement
difference.  Source-level attempts to move it at the default weight all failed:

| variant (on top of the landed shape) | usize | ratio |
|---|---:|---:|
| landed shape | 9908 | **73.868195** |
| `otherLock` expect polarity 1 | 9912 | 70.782484 |
| `otherLock` plain (no expect) | 9904 | 72.169676 |
| `otherLock` positive OR | 9904 | 72.169676 |
| `otherLock` positive OR + `expect(...,1)` | 9908 | 73.868195 |
| `otherLock` positive OR + `expect(...,0)` | 9912 | 70.782484 |
| second `otherLock` assignment = first expression | 9916 | 71.782391 |
| second `otherLock` assignment removed | 9852 | 70.977011 |
| `otherLock` cold value `? 1 : 0` / `(bool)` / `!!` | 9908 | 73.868195 |
| move first `otherLock` block before the route block | 9832 | 70.008618 |
| move first `otherLock` block inside the landmark `if` | 9868 | 64.103300 |
| `onPlanetCourse` local (pass-13 `q2`) + landed shape | 9908 | 73.810888 |
| `i`-first gas-cloud declaration (pass-14 `q2fix`) + landed shape | 9908 | 73.810888 |

The `otherLock` cold block's position is invariant under every source spelling tried; it is
the same "cold-tail placement" residual documented in passes 8/12/13.

## 4. What was swept this pass

All measurements use the official verifier API (`autojunk=False`).

* **Ablation:** the eight single/combined expect-presence variants in §1, at both weights.
* **`onScreen` branch shape:** all 31 subsets of the five `&&` conjuncts wrapped in
  `__builtin_expect(...,1)`, plus all contiguous prefixes/segments, plus polarity-0 forms,
  plus onScreen-last, whole-chain, and `bool`-local forms.  Best default-weight result:
  73.868195 % (the landed shape).  The flag-independent forms (onScreen-last, bool-local)
  are 71.82 % / 72.48 %.
* **`otherLock` shape:** 14 prefix/subset placements of the three null checks in
  `__builtin_expect(...,0/1)`, plus positive-OR, plain, second-assignment and cold-value
  spellings.  None beats 73.868195 % at the default weight.
* **Other branch hints at the default weight:** `landmarks != nullptr`, `campaign >= 2`,
  `route != nullptr`, `scannerAvailable`, `enabled`, `alienOrbit`, `turretMode`,
  `gasClouds != nullptr`, `lockedEnemy != nullptr` — all worse or size-changing.
* **Faithful-source variants:** remove the duplicate campaign autopilot block (`nodup`),
  `nodup` + positive-OR `otherLock`, `nodup` + no `onScreen` expect, `nodup` + onScreen-last
  — all 66.5-67.5 % at the default weight.
* **Landmark-loop reorderings:** `int offset` before/after the `i != 3` draw, `egoPos`/
  `targetPos` before/after the draw — 71.07-73.75 %.

## 5. Conclusion

* The 80:1 per-file override **can be removed**.  Hinting the first bounds comparison in
  addition to `onScreen` reproduces the 80:1 early layout at the default 2000:1 weight.
* The landed shape is flag-independent (73.868195 % at both weights), so the override was
  doing nothing for it.  The official report percentage is unchanged at 73.9 %.
* The raw ratio drops 0.057 points (73.925501 -> 73.868195).  The entire deficit is the
  `otherLock` cold-block address (`0x261e` vs `0x2480`), the last cold-tail placement
  residual; no source spelling tried moves it.
* If the raw ratio is preferred over removing the flag, the 80:1 override plus the
  single-`onScreen`-expect source is still 73.925501 %.  The trade-off is recorded here so a
  later pass can revisit it if the `otherLock` cold block is ever placed at `0x2480` at the
  default weight.

## 6. Evidence / scratch (all under `/dev/shm/w5004`, not committed)

* `measure.py` — official-verifier ratio for one symbol (same API as
  `tools/verify/verify.py`, `autojunk=False`).
* `run.py` — build one source with/without the 80:1 override, measure, optionally dump.
* `Radar.base.cpp`, `CMakeLists.base` — pristine pre-pass baseline.
* `cx_p2s0_lint.cpp` — the landed source (with the two same-line lint waivers).
* `base80.dis`, `basedef.dis`, `cx_p2s0_def.dis`, `om4p0.dis` — side-by-side dumps.
* `opcodes80.txt` — SequenceMatcher opcode map of the 80:1 build.
* `pba80.txt`, `pbadef.txt`, `*_order.txt` — `-print-after-all` machine block order and the
  six blocks (`ir153`, `ir304`, `ir326/331`, `ir1276/1301`) that move between weights.
* `sweep_expect.py`, `sweepW.py`, `cx_*.cpp`, `c2o_*.cpp`, `sub_*.cpp`, `c2x_*.cpp`,
  `c2t_*.cpp`, `c2p_*.cpp`, `c2r_*.cpp`, `c2v_*.cpp`, `lr_*.cpp` — the variant sweeps.
