# PlayerEgo::calcCollision — wave 119 (w6708): the 968 B ceiling holds;
# the v2 RA flip needs guards in *both* cold arms

**Status:** research note, 2026-11-05. Worktree `wt/w6708`, branch
`wave119-w6708`, base `47a1396bd`. **No source change landed** — the
checked-in 87.6436781609 % / 968 B body (the "landed" body: nested
`{ Vector surface … }` block, `A1` = `if (i > objects->size() - 1) continue;`,
`A2` = `if (objects->size() == i) continue;`, no `i == 0` in the visible
arm) remains the best gate-safe measurement. This note extends
`playerego-calccollision-wave118-w6610.md`.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions).

## TL;DR

* Baseline re-verified: **87.6436781609 % / 968 B / 347-349**
  normalised instructions; `check` **RATCHET PASS** with byte **4503**,
  linked **4503**, size 4524, data 843/843, imports 207, lint CLEAN,
  drift 0, 27 sections, file size 2,187,328 B.
* **v2 re-confirmed: 89.3982808023 % / 976 B / 347-351.** It is the
  faithful shape (`i == 0` present, no redundant checks) plus the two
  `objects->size()` guards (`A2` in the wormhole arm, `size0` in the
  visible arm) and minus the visible-arm `i == 0` test. The 976 B is
  gate-breaking (`.text` 1259464 → 1259496, byte 4503 → 2109).
* ~250 new builds this wave. **Nothing beats 87.6436781609 % at
  ≤ 968 B.** The 968 B ceiling is re-confirmed.
* New mechanistic result: the original register file (r5=this,
  r6=temp, r9=dock, sl=distance, guard spilled to `[sp,#24]`) is
  restored by the guards **only when they sit in two different cold
  arms** (wormhole + visible). A single guard in either arm, or both
  guards in the same arm, leaves the faithful RA (sl=this, r5=temp,
  r6=guard) and the 996 B rotation. The cheapest two-arm pair that
  flips the RA is `size_i` + `size0` = 14 B → 976 B.

## 1. Exact scores (this wave)

All 968 B unless stated; `lens` = orig/ours normalised instructions.

```
landed (A1=gt, A2, nested surface, no i0)                 87.6436781609  968  347/349
landed, compare swapped (collision != zero)               87.3563218391  968  347/349
landed, A1 -> i >= size                                   87.2832369942  968  347/345
landed, A1 -> size <= i                                   86.9942196532  968  347/345
landed, A1 -> size - 1 < i / A2 -> i == size              87.6436781609  968  (identical)
landed, visible-arm matrix via Matrix *a local            86.7816091954  968  347/349
landed, A1/A2 moved to end of body                        59.7701149425  988  347/349
landed, A1 at end + A2 in wormhole arm                    70.7736389685  984  347/351
landed, A2 only (no A1)                                   79.5389048991  964  347/347
landed, A1 only (no A2)                                   79.5389048991  964  347/347
landed + visible i0 (both anchors)                        84.9785407725  976  347/352
landed + visible i0, A2 removed                           82.5899280576  968  347/348
landed + visible i0, A1 removed                           70.6723891273  988  347/352
landed + visible i0, both anchors removed                 59.7997138770  996  347/352
landed + visible i0 after setPosition / before setPosition 56.2/55.3     992
landed + visible i0 after hitCamera                       83.7142857143  984  347/353
landed, hitCamera shared (steal falls into visible)       87.0689655172  968  347/349
landed, geometry local in visible arm                     87.6436781609  968  (identical)
landed, dock/moveTarget local aliases                     87.6436781609  968  (identical)
landed, const/ctor surface, !object, isVisible()==0       87.6436781609  968  (identical)

v2 (faithful - i0 + A2 + size0)                           89.3982808023  976  347/351  <- best, gate-breaking
v2, A2 -> i >= size / size <= i                           89.3982808023  976  (identical)
v2, A2 -> i > size - 1                                    89.1428571429  980  347/353
v2, compare swapped back (zero != collision)              88.5386819484  976  347/351
v2 + visible i0                                           89.3009985735  984  347/354
v2, size0 -> i >= size                                    85.6321839080  976  347/349
v2, A2 removed + i0                                       59.6843615495  988  347/350
v2, size0 removed + i0                                    62.4821683310  996  347/354
v2, both guards removed + i0 (== faithful)                60.0858369099  996  347/352
v2, A2 moved after pull / after sub / after dist           85.39 / 64.67 / 64.10  976/968/968
v2, A2 moved after isPlaying / before strength>=1         69.2417739628  956  347/352
v2, A2 moved inside strength>=1                           68.7589158345  964  347/354
v2, shared `unsigned n = objects->size()` at loop top     69.0751445087  952  347/345

faithful (no anchors, i0, nested surface)                 60.0858369099  996  347/352
faithful, loop-scope surface                              63.7931034483  968  347/349
faithful, loop-scope surface + i0 (== "nested if")        64.0804597701  968  347/349
faithful + `AEGeometry *geo` at loop scope                69.3409742120  968  347/351
faithful, surface declared before null check              58.9413447783  984  347/352
faithful, surface declared at loop scope after null       48.6486486486  1008  347/356
faithful, surface direct argument                         60.0858369099  996  347/352
faithful, `Vector surface;` at function scope             44.6629213483  1028  347/365
faithful, shared hitCamera (steal falls through)          60.0000000000  992  347/350
faithful, Ghidra `if (!steal) visible else steal` order   52.4000000000  992  347/351
faithful, `if (object != nullptr)` nesting                60.1000000000  996  347/352
faithful, unused Vector/int/ptr locals                    60.0858369099  996  (identical)
faithful, self alias / no this-> / int i / ++i            60.0858369099  996  (identical)

two-arm guard matrix (faithful + guard in wormhole arm + guard in visible arm):
  size_i  + mining0                                       89.4285714286  984  347/353  <- best guard pair
  size0   + mining0                                       89.4285714286  984  347/353
  size_i  + size0 (== v2 + i0)                            89.3009985735  984  347/354
  steal   + size0                                         87.3038516405  988  347/354
  vis     + mining0                                       87.1794871795  992  347/355
  size_i  + i0                                            73.4870317003  976  347/347
  i0      + size0                                         58.4878744650  996  347/354
  i0      + i0                                            68.1159420290  952  347/343
  size_i  + size_i                                        85.2223816356  980  347/350
  (single guard in either arm: all ~60 %, RA does not flip)
  (both guards in same arm: all ~57-63 %, RA does not flip)
```

## 2. The RA-flip rule (new)

The original register file is reproduced by the two-guard shape only
when the guards are in **different cold arms**:

* `A2` (`objects->size() == i`) in the wormhole arm, after the
  `isShrinking() || miningGame != 0` check, and
* `size0` (`objects->size() == 0`) in the visible arm, after the
  `!isVisible()` check.

`-debug-only=regalloc` on the function-only bitcode shows the greedy
allocator's first assignments are identical for v2 and faithful
(`%10→r4, %9→r5, %2→r6, %1→r6, %2→r8`), then diverge at the two
loop-invariant intervals: v2 assigns `%0→r6` (later evicted to
`r10`/sl) and `%1→r9`; the faithful assigns `%0→r9` directly and
`%1→r10`. The two-arm guards raise the `objects->size()` use density
in cold blocks enough to swap that order, which puts the temp vector
in `r6`, spills the stack-guard pointer to `[sp,#24]`, frees `sl` for
`distance`, and keeps `BFCHANGED=0` (no loop rotation).

A single guard in either arm (or both in the same arm) leaves the
faithful assignment (`sl`=this, `r5`=temp, `r6`=guard, distance shares
`r6`) and the 996 B rotated loop.

## 3. Why the 8 B cannot be shaved

v2 is exactly `original + A2 (8 B) + size0 (6 B) - i0 (6 B)` = +8 B.
The guards are load-bearing: deleting either one flips the RA back to
the faithful shape (and *grows* the function to 988-996 B). Every
cheaper guard pair tried (byte-field guards, `i == 0`, `dockedFlag`,
`miningGame`, `stealFlagByte`, shared `n = objects->size()`) either
fails to flip the RA or costs ≥ 14 B. The `size_i` branch is `beq.w`
(4 B) because the latch sits 262 B away; no placement tried makes it
short without flipping the RA.

The landed body pays for the RA with `A1` (8 B) + `A2` (8 B) and omits
`i0` (6 B), net +10 B of anchors vs the original's +6 B of `i0` +
shuffle + top dock spill; the compiler absorbs the difference and the
result is exactly 968 B.

## 4. Conclusion / recommendation

The original source is the faithful shape (nested `{ Vector surface }`,
bottom `i == 0 && object->field_0x71`, `collision != Vector{…}`), but
clang 7.0.2 at `-Oz` rotates it to 996 B. The only lever between
87.6 % and ~100 % remains a **zero-byte construct that keeps the
nested-surface lifetime (frame 112, original RA) and breaks the
`bb.33`/`bb.20` one-instruction tail merge** (the `ldr r6,[sp,#20]`
stack-guard restore). This wave's ~250 builds (guard placement/forms,
surface scoping, shared-hitCamera CFG, Ghidra else-chain order, local
aliases, unused locals, `n = objects->size()` shapes) found no such
construct. 87.6436781609 % / 968 B stands.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6708
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
# per-variant: splice the body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6708/score.py                         # pct/size/lens
python3 /dev/shm/w6708/score.py -d                      # normalised diff
```

Scratch harness (`/dev/shm/w6708`): `score.py`, `splice.py`,
`sweep.py`, `ir.sh` (NDK clang → LLVM 7 bitcode → patched
`/tmp/llvm_android_build/bin/llc`), body files (`body_landed.txt`,
`body_v2.txt`, `body_faithful.txt`, `body_faithful_loopscope.txt`,
`body_shared_hit.txt`, `body_ghidra_order.txt`) and the `g1`…`g25`
variant directories.

## 6. Gate

Unchanged: `byte_exact 4503 / linked_exact 4503 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0, `RATCHET PASS`.
27 sections, file size 2,187,328 B. No source change, so
`tools/verify_baseline.json` is untouched.
