# HangarWindow::render — row-39 ULEB fix, and why the credits call is not size-free (wave 160, w10716)

Worktree `wt/w10716`, base `01c7f92d6` (wave 160).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, 9,700 B, `.ARM.extab` record
948 B / 156 rows.

## Landed shape (the wave-159/w10616 shape, re-verified)

```cpp
if (item->isShip()) {
    volatile int *dummy;                 // lifetime anchor
    (void) dummy;
    if (__builtin_expect(tab != 0, 1)) { // lint: arm_builtin anchor
        if (!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
            volatile int padX[1];        // LSDA sink
            ... buttons[1] ...
            if (this->upgradeMode && tab == 1) { ... buttons[2]/[10] ... }
        }
    }
}
```

| metric | base | landed |
|---|---:|---:|
| raw same-address diff | 1,965 | **2,514** |
| instruction-index alignment | 291 | **485** |
| function size | 9,700 | **9,700** |
| record diff bytes | 676 | **151** |
| total `.ARM.extab` diff | 1,256 | **733** |
| total `.text` diff | 85,518 | **86,067** |
| `.rel.plt` | 0 | 0 |

Gate: `RATCHET PASS`, byte_exact 4507, linked_exact 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.  Net total bytes +24
(.text +549, extab -525), exactly the w10616 measurement.

## New findings this wave

1. **The row-39 ULEB length is not fixed by the branch encoding alone.**
   The base already has the right 948/156 record; only row 39 encodes
   `118` (1-byte ULEB) instead of `150` (2-byte).  The landed shape
   changes the EH region boundaries so row 39 encodes correctly; the
   branch at the row-39 boundary is *not* the sole cause.

2. **The `Globals::status->getCredits()` call does NOT cut the base
   record to 256.**  Measured on the current base shape (no dummy/padX):
   `base + getCredits()` = size **9,680**, record diff **762**
   (not 9,704 / 256).  The note's 256 figure was measured on the landed
   shape, not on the base.

3. **Landed + getCredits is 9,716 B**, record diff **312** (not 9,704).
   The call shares the `Globals::status` load with the following
   `getShip()` comparison, saving 8 B, and the function still grows
   +16 B.  Dropping `dummy` gives 9,712; dropping both dummy and the
   credits gives 9,696.

4. **`cbz` source-shape sweep.**  The base's `tab == 0` branch is
   threaded by JumpThreading: the check block (load tab, `icmp eq 1`,
   load upgradeMode, `and`) has duplication cost 5 <= the 6-instruction
   threshold, so JT redirects the zero edge to END and the backend emits
   `cmp; beq.w`.  Shapes tried that keep the 9,700 size and 948/156
   record: `if (tab==0){}else`, pointer test `if (tab)`, `goto`,
   `||`/`&&` forms, `tab>0`, `tab>=1`, `expect` on either branch,
   `volatile` copies/references (all either still `beq.w` or change the
   stack layout and size).  Forcing the check-block cost above 6 with an
   extra condition does emit `cbz` (`Globals::w > 0` -> 9,704 B,
   `this->field_0x114 != 0` -> 9,692 B) but the added condition costs as
   much as the `cbz` saves, and the credits call then shrinks the
   function by 16 B, so none is size-exact at 9,700.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10716/fdiff.py _ZN12HangarWindow6renderEv        # 2514, 9700/9700
python3 /dev/shm/w10716/align_diff.py _ZN12HangarWindow6renderEv   # 485
python3 _work/reconstructions/extab_record_map.py --ours cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

Scratch (not committed), `/dev/shm/w10716/`: `fdiff.py`, `align_diff.py`,
`trylib.py`, `variants.py`, `dumpvar.py`, `cc.sh`.
