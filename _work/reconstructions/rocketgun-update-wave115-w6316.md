# `RocketGun::update(int)` — wave 115 (w6316)

**Status:** landed. `_ZN9RocketGun6updateEi` is now **byte-exact** (and
linked-exact): original **1908 B / 635 insns**, ours **1908 B / 635 insns**,
`SequenceMatcher` ratio **100.0 %**, raw bytes identical.

| | pct | size | insns | byte | linked |
|---|---:|---:|---:|---|---|
| before (wave-114 shape) | **93.8** | 1908 | 636 | no | no |
| after (this wave) | **100.0** | 1908 | 635 | **yes** | **yes** |

```
verify: byte 4501  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint:   CLEAN ✓
drift:  0 field(s) across 0 class(es)
```

No section/extab regression: **27 sections**, file size **2,187,328 B**,
`.text` `0x1337c8` @ `0x69440`, `.ARM.exidx` `0x6f70`, `.ARM.extab`
**0x26298** (all identical to the original). `.rodata` remains the known
`Status::missionCompleted` rank-table tail (`0x41c27` vs `0x41b67`).

Baseline locked with `python3 tools/ratchet.py --update-baseline`
(byte_exact 4500 -> 4501, linked_exact 4500 -> 4501).

---

## 1. The winning source shape

The wave-113/114 analysis was right that the residual was a single greedy-RA
flip (`elapsed` r5 -> r4, `this` r5 -> sl, `player` r4 -> r5), and that the
faithful shape (plain `if`, `dt` late, `trailTimers` store after
`resetSystem`, plain `VectorNormalize(VectorCross(...))`) is exactly the
original **once that flip happens**. This wave found the source lever that
triggers the flip.

Six changes from the wave-114 baseline (all ordinary C++, no anchors):

```diff
-    if (__builtin_expect(this->hasGeometry != 0 && this->gun->delayActive != 0, 1)) { // lint: ...
+    if (this->hasGeometry != 0 && this->gun->delayActive != 0) {
...
         this->geometry->setScaling(scale, scale, scale);
+        AEGeometry *geom = this->geometry;
         const Vector forward = MatrixGetDir(...);
         Vector up = {0.0f, 1.0f, 0.0f};
-        this->geometry->setDirection(forward, up);
+        geom->setDirection(forward, up);
...
-    const float dt = (float) elapsed;
     if (gun->hitSmall != 0) {
...
             this->particleManager->enableSystemEmit((*this->trailSystems)[cur], false);
-            (*this->trailTimers)[cur] = 0;
             this->particleManager->resetSystem((*this->trailSystems)[cur]);
+            (*this->trailTimers)[cur] = 0;
...
+    const float dt = (float) elapsed;
     for (uint32_t i = 0; i < this->gun->positions.count; ++i) {
...
-                    struct V3 { float x, y, z; };
-                    V3 crossBuf;
-                    Vector &cross = *(Vector *) &crossBuf;
-                    cross = VectorCross(...);
-                    this->gun->positions.data()[i] += VectorNormalize(cross) * (s + s) * dt;
+                    Vector cross = VectorNormalize(VectorCross(...));
+                    this->gun->positions.data()[i] += cross * (s + s) * dt;
...
-                Matrix &trail = this->trailMatrices->data()[i];
+                Matrix &trail = (*this->trailMatrices)[i];
```

The last line is the load-bearing one. `Array<T>::operator[]` and
`Array<T>::data()[i]` are semantically identical, but `operator[]` keeps the
array element access as a single `t2LDRs`-style address computation in the
trail block, which changes the greedy allocator's interference graph enough
to make `%413` (the cluster temp LEA) evict `this` from `r5`; `this` then
lands in `sl`, `elapsed` keeps `r4`, and `player` takes `r5` — exactly the
original colouring. With the V3/`crossBuf` hack removed the call stream is
also exact (no extra `Vector::operator=`), so the whole function collapses
to the original bytes.

Note: `trail_data` alone (on the wave-114 baseline) already flips the
colouring, but the other five changes are needed to remove the remaining
size/pool deltas (plain `if` +4 B, `geom` cache +4 B, `dt` late -8 B,
z-store order +4 B, plain cluster -6 B, trail `operator[]` 0 B). The six
together are byte-identical.

## 2. Measurements of the intermediate shapes

All measured with a full gold link + `verify.py` score; the target function
size is 1908 B in every row unless noted.

| variant | pct | size | note |
|---|---:|---:|---|
| wave-114 baseline | 93.8 | 1908 | `V3 crossBuf`, `__builtin_expect`, dt early |
| `plainif+geomlocal` | 90.6 | 1912 | elapsed r4, but pool +4 |
| `plainif+geomlocal+dtlate+zfaith` | 96.8 | 1916 | elapsed r4, this sl; only cluster operator= left |
| `plainif+geomlocal+dtlate+zfaith+cF` | 63.5 | 1836 | plain cluster collapses RA (this r5) |
| `hom_inv` (OR homing condition, cF) | 99.4 | 1908 | right RA, wrong `cbz` target |
| **`trail_data` (all six)** | **100.0** | **1908** | byte-exact |

The `hom_inv` row is the near-miss that pointed at the RA: writing the
homing test as `homing == 0 || (...)` gives the original's register
colouring but the wrong first-branch target. `(*trailMatrices)[i]` gives
the same colouring *and* keeps the correct `homing != 0 && (...)` branch.

## 3. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi
```

Scratch for this wave lived in `/dev/shm/w6316` (`probe3.py`, `gen3.py`,
`T_trail_data.cpp`); none of it is in the repo.
