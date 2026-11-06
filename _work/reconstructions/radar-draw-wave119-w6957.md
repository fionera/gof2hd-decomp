# Radar::draw pass (wave119, w6957) — `Status *status` local + hoisted negated locals

**Status:** source change landed, 2026-11-06. Worktree `wt/w6957`, branch
`wave119-w6957`, based on `master` `401857181`. Target
`_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.

| | size | unrounded `difflib` ratio |
|---|---:|---:|
| baseline (pristine) | 9908 B | **74.426606 %** |
| this pass | 9908 B | **74.541284 %** |

Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484 (unchanged
instruction counts). 74.541284 % is the best measured variant of the ~30 tried
across `radar-draw-wave119-w6947.md`, `-w6910`, `-w6913` and this pass.

Gate after the change: `RATCHET PASS`, byte 4505, linked 4505, data 843/843,
imports 207, lint CLEAN, drift 0. Whole-file size still 2,187,328 B; no
section/`.ARM.extab` change (pure `.text` codegen inside a size-exact
function).

## 1. The change

Three edits inside `Radar::draw`, all in the landmark block:

```cpp
     Status *status = Globals::status;      // NEW: real local
     int otherLock;
     ...
     bool landmarkHit = false;
+    int notOtherLock = !otherLock;         // MOVED out of the guard body
+    int notTurretMode = !turretMode;
     if (this->landmarks != nullptr &&
-        !(Globals::status->getStation()->getIndex() == 109 ||
-          Globals::status->getStation()->getIndex() == 110 ||
-          (Globals::status->getStation()->getIndex() == 111 &&
-           Globals::status->getCurrentCampaignMission() > 93))) {
-        int notOtherLock = !otherLock;
-        int notTurretMode = !turretMode;
+        !(status->getStation()->getIndex() == 109 ||
+          status->getStation()->getIndex() == 110 ||
+          (status->getStation()->getIndex() == 111 &&
+           status->getCurrentCampaignMission() > 93))) {
         for (int i = 0; i < this->landmarks->size(); ++i) {
```

## 2. Mechanism

The original computes `notOtherLock = !otherLock` and `notTurretMode =
!turretMode` **before** the landmark loop, not inside it. In the original
Thumb at `0x12e...`:

```
eor.w r0, r0, #1        ; notOtherLock = !otherLock   (r0 = [sp,#148])
str   r0, [sp, #108]
eor.w r0, r5, #1        ; notTurretMode = !turretMode (r5 = turretMode)
str   r0, [sp, #116]
movs  r0, #0
str   r0, [sp, #152]    ; landmarkHit = false
```

Hoisting the two negations reproduces that setup block. On its own the hoist
grows the body by 4 B (9908 -> 9912, 72.266017 %); pairing it with a real
`Status *status` local in the 109/110/111 guard shrinks the guard's repeated
`Globals::status` reloads enough to pay for it, landing back on the exact
original size 9908 B while changing the guard's register shape (`r6` holds
the status pointer, the `movs r1, #0` skip-block shape appears).

Note the original actually keeps `&Globals::status` in a stack slot and
reloads the pointer (`ldr r6, [sp,#156]; ldr r0, [r6,#0]`), so the local is a
*tuned* shape, not a literal reconstruction — but it is the only shape that
has beaten the long-standing 74.426606 % local optimum.

## 3. Variants measured (all 9908 B unless noted)

| variant | % |
|---|---:|
| baseline | 74.426606 |
| **status local + hoisted negated locals (landed)** | **74.541284** |
| same, `uint8_t` locals / `Status *const` / pointer-to-pointer | 74.541284 (ties) |
| same, without the inner `__builtin_expect` | 74.512615 |
| same, plain `&&` for otherLock | 73.308486 |
| `status` local only, no hoist | 9900 B, 72.536928 |
| hoist only, no `status` local | 9912 B, 72.266017 |
| recompute deleted | 9824 B, 63.746398 |
| recompute moved after tail | 9936 B, 71.164590 |
| recompute `&& mode != 0` | 9916 B, 72.839329 |
| `Status *&status` reference / guard uses `Globals::status` | 9912 B, 72.266017 |
| all `Globals::status` in draw -> local `status` | 10176 B, 44.503871 |
| condition reorder `!hit && !otherLock && i!=3 && !turret` | 9912 B, 72.792431 |
| `i==0` faithful lockLabel store | 9900 B, 71.979361 |
| inSight reorders (VectorLength-first / x>,x<,y>,y<) | 9864/9936 B, 66.74 / 71.24 |

## 4. Remaining residuals (unchanged by this pass)

- `vpush {d8-d13}` (orig) vs `{d8-d11}` (ours): the original keeps 6 VFP
  callee-saved doubles live across calls.
- The original hoists loop-invariant member addresses (`&egoPos` 0x16c,
  `&targetPos` 0x178, `&lockLabel` 0x18c, `&field_0x1c`, `&image_0x60`,
  `&image_0xa0`, `&image_0xa8`, `&image_0xb0`) into stack slots before the
  enemy loop at `0x12ecb4`-`0x12ecc8`; ours does not.
- `mode`/`turretMode` stack-slot swap (sp#148 vs sp#152).
- The 17-instruction planet-lock tail at orig[637:654] and the 19-instruction
  landmark-loop setup at orig[673:692].

These are all register-allocation / LICM differences; no further source shape
tried in this pass moved them.

## 5. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w6957
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
```

Scratch harness (single-symbol unrounded `difflib` score, mirrors
`tools/verify/verify.py`) lives in `/dev/shm/w6957/score.py`.
