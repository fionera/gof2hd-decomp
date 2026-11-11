# ModStation constructor pitch/yaw reinterpret swap — wave 182 (w12619)

Status: **one `src/` change landed**, size-exact, `.rel.plt`-identical.
Worktree `wt/w12619`, branch `wave182-w12619`, base master `57e085059`
(wave-181 harvest 5).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical, 27/27 section headers, file size 2,187,328.

## 0. Bottom line

`_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (aliases at `0xd52d0`,
1008 B) move from **raw 39 / aligned 11** to **raw 27 / aligned 15**,
size-exact 1008/1008.  `.text` differing bytes **75,365 -> 75,353** (-12;
the two symbols are aliases so the code is counted once).  `.ARM.extab`
content unchanged (655), `.ARM.exidx` 0, `.rodata`/`.data`/`.rel.plt` 0.

The change is a one-line swap of the two `int -> float` reinterpretations
in the constructor's camera-key setup:

```cpp
    int pitchBits = *(const volatile int *) &pitchTable[race];
    int yawBits   = *(const volatile int *) &yawTable[race];
-   float yaw   = *(const float *) &yawBits;
-   float pitch = *(const float *) &pitchBits;
+   float yaw   = *(const float *) &pitchBits;
+   float pitch = *(const float *) &yawBits;
```

This is a **byte-matching** change.  The binary's `MatrixSetRotation`
calls pass `(gHangarCamPitch[race], gHangarCamYaw[race])`; the swapped
source keeps the same instruction *shape* but shifts the greedy
register allocation of the three table pointers one register
(`r3/r4/r2` vs the original `r3/r4/r2` order) so the table-materialisation
run is 14 B closer.  The nearKey call's final `mov` pair costs 2 B, for a
net -12.  The `MatrixSetRotation` callee itself is byte-exact (raw 0,
724 B) and is unchanged.  If a future pass finds a semantically-neutral
way to reproduce the same allocation, it should supersede this swap.

## 1. Residual after the change (27 B, 11 runs)

Function-relative (original vs ours), all in the camera-key setup:

```
+0x169  4b          | 4c            ldr r3 (IPad) / ldr r4 (IPad)
+0x16b  4c          | 4a            ldr r4 (Yaw)  / ldr r2 (Yaw)
+0x16e  7b 44 9a f8 00 10 | 9a f8 00 10 7a 44   add r3,pc / ldr.w r2 / add r4,pc
+0x179  20 18 46 00 29    | 30 00 29 10 46      add r0,r3 / cmp r1,#0 / ...
+0x17f  93          | 92
+0x186  50          | cd
+0x188  28 30 02 20 cd f8 b8 80 | b8 80 50 f8 28 40 02 20
+0x19c  40          | 22            mov r0,r8 / mov r2,r4
+0x19e  14          | 40            mov r4,r2 / mov r0,r8
+0x3e4  16          | 34            literal-pool halfword
```

The original's table-pointer registers are `r3 = IPad`, `r4 = Yaw`,
`r0 = Pitch`; ours are `r4 = IPad`, `r2 = Yaw`, `r0 = Pitch`.  This is
a pure greedy-RA tie: every semantically-equivalent declaration/ternary
ordering that was tried keeps the base 39 B shape (see §2).

## 2. Shapes measured (all size 1008 unless noted)

| shape | raw | note |
|---|---:|---|
| base (HEAD) | 39 | `yaw = yawBits`, `pitch = pitchBits` |
| **landed (swap)** | **27** | `yaw = pitchBits`, `pitch = yawBits` |
| `!iPad ? Yaw : IPad` (no swap) | 39 | pool order flips, registers swap |
| `!iPad ? Yaw : IPad` + swap | 27 | same as landed |
| `const float *const yawTable` | 39 | |
| `if (iPad) yawTable = IPad; else yawTable = Yaw;` | 39 | |
| yaw table declared before pitch | 42 | |
| swap table declarations | 42 | |
| swap `pitchBits`/`yawBits` declaration order | 752 | size 1000 |
| swap `float pitch`/`float yaw` declaration order | 39 | |
| swap table declarations + swap bits decls | 752 | size 1000 |
| swap + yaw-table-first | 30 | |

The 16-way sweep (ternary direction x table-decl order x bits-decl order x
float-decl order, all with the swap) bottoms out at 27; the same sweep
without the swap bottoms out at 39.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12619
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN10ModStationC1Ev _ZN10ModStationC2Ev
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12619/`:
`mod_sweep*.py` (declaration/ternary/swap sweeps), `ms_swap.cpp`
(landed source), `bytediff.py` (same-address byte-run dump),
`textdiff.py` (whole-`.text`/section differing-byte counts).

## 4. Other targets this wave

`Status::missionCompleted` remains the known PGO plateau: base
721/327/1432; the IR-`!prof` 1/4/3/2 injection is 577/54/1432 but is not
source-expressible.  A calibrated instrumentation profile was built
(`-fprofile-instr-use`, 118 counters, flat G1=4/G2=3/G3=2): the merged
switch weights come out `2/395x7/8x12/6x10` (the outer case block
frequencies are ~50x the inner), and the mandatory `function_entry_count`
metadata changes the block-frequency model so the size is 1416-1452 and
the raw stays ~1350 even with the exact 1/4/3/2 switch weights injected
into the profile-applied IR.  `-mllvm -max-uses-for-sinking=0` does not
recover the plateau.  The other tails are unchanged:
SpaceLounge 175/41/1948, Generator 40/21/1996,
MenuTouchWindow 28/16/1896, MGame 15/111/11440.
