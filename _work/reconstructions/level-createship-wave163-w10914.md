# `Level::createShip` — wave 163 (w10914): register-map sweep (negative)
# and the landed case-order raw win

Worktree `wt/w10914`, base `master e51db2fc7` (wave 162).  Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  All
evidence is objdump of the original (authoritative); the Ghidra provider
was not used.

Gate on entry and exit (full `check`): `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.rel.plt` still
byte-identical (0); all 27 section sizes exact; file size exact.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,563** | **1,560** |
| instruction-index alignment | **449** | **460** |
| size | 2,764 | 2,764 |
| `.text` differing bytes | 85,042 | 85,039 |

## 1. Landed change

Reordered the `switch (type)` hitpoints cases in the source to
`0x31, 0x2c, 0x33`.  The original's case-body order is `0x2c, 0x33,
0x31` and its compare order is `0x33, 0x31, 0x2c`; the base source has
the `0x2c, 0x33, 0x31` body order.  Starting the source order with
`0x31` moves the three `vmov`/`vldr` constant-pool loads into the
original's order: raw **1,563 -> 1,560** at the cost of 11 aligned
instructions (the block re-layout).  This is the w10814 measured
variant that wave 162 left unlanded.  All six permutations were
re-measured: only the two starting with `0x31` reach 1,560; the other
four are 1,563 (aligned 449-451).

## 2. The 4-byte register-allocation shift (still open)

Original entry (c0220-c023a):

```
str   r1, [sp, #60]     ; race -> [60]
mov   r9, r0            ; this -> r9
ldr   r0, [pc, #572]
mov   fp, r3            ; type -> fp
mov   r5, r2            ; shipClass -> r5
add   r0, pc
ldr   r4, [r0, #0]      ; status -> r4
ldr   r0, [r4, #0]
blx   getCurrentCampaignMission
mov   r8, r0            ; mission -> r8
ldr   r0, [r7, #8]      ; waypoint
mov   sl, r5            ; shipClass r5 -> sl
cbz   r0, ...
```

Ours (base):

```
strd  r1, r2, [sp, #56] ; race -> [56] AND shipClass -> [60]
mov   r9, r0            ; this -> r9
ldr   r0, [pc, #576]
mov   sl, r3            ; type -> sl
add   r0, pc
ldr   r4, [r0, #0]      ; status -> r4
...
```

Register map (original vs base):

| value | original | base |
|---|---|---|
| `race` | `[sp,#60]` | `[sp,#56]` (strd) |
| `shipClass` | r5 -> sl -> r6 | `[sp,#60]` (strd) |
| `type` | fp (r11) | sl (r10) |
| `this` | r9, spilled at c0332 to [56] | r9, spilled at c0332 to [52] |
| `status` | r4 | r4 |
| `mission` | r8 | r8 |
| `jitterX` | `[sp,#36]` (spilled at c026e) | fp (r11), spilled at c0318 |
| `jitterY` | `[sp,#32]` | `[sp,#28]` |
| `jitterZ` | `[sp,#28]` | `[sp,#24]` |
| `x` | `[sp,#44]` | `[sp,#44]` |
| `y` | `[sp,#48]` | `[sp,#36]` |
| `z` | `[sp,#40]` | `[sp,#40]` |

The original's `shipClass` is live-range-split three ways
(`r5` entry -> c0238, `sl` c0238 -> c02b4, `r6` c02b4 -> c03fe).  Ours
spills it whole.  Because `type` takes `sl` in ours, `fp` is free and
the allocator gives it to `jitterX`; in the original `type` occupies
`fp`, so `jitterX` spills and `shipClass` gets `sl`.  The chain is
therefore: **whoever wins the second callee-saved slot (`sl` vs `fp`)
decides the whole map.**

## 3. Hypotheses measured this wave (all full link, size checked)

Every variant below is size-exact 2,764 B unless noted.  "entry map"
is the first callee-saved movs/spills.

| variant | raw | aligned | size | entry map |
|---|---:|---:|---:|---|
| base | 1,563 | 449 | 2,764 | `strd r1,r2[56]; mov r9,r0; mov sl,r3` |
| **case order `0x31,0x2c,0x33` (landed)** | **1,560** | **460** | **2,764** | same as base |
| case + `int cls = shipClass` (both checks) | 1,560 | 460 | 2,764 | same as base |
| case + `expect(mission==0x9a,1)` | 1,560 | 460 | 2,764 | same as base |
| case + `shipClass*0` in hitpoints | 1,560 | 460 | 2,764 | same as base |
| case + `Waypoint &wp = *waypoint` | 1,560 | 460 | 2,764 | same as base |
| case + `int cls` late (before `ship=nullptr`) | 1,560 | 460 | 2,764 | same as base |
| case + `int x` declared first | 1,560 | 460 | 2,764 | same as base |
| case + coords after `setEmpData` | 1,560 | 460 | 2,764 | same as base |
| case + `switch(mission)` bitmask | 1,560 | 535 | 2,764 | same as base |
| case + `mission==56 || 49..52` single-if | 1,573 | 465 | 2,764 | same as base |
| case + `(unsigned)(mission-49)<=3` | 1,573 | 465 | 2,764 | same as base |
| `int` coords (all 6 decl orders) | — | — | 2,788 | `mov r6,r3; mov sl,r2` (shipClass->sl) |
| `int` coords + single-if mission | — | — | 2,784 | base map |
| `int` coords + `switch(mission)` | — | — | 2,800 | `mov r4,r3; mov sl,r2; mov r9,r0` |
| `int` coords + `expect(shipClass==0,1)` | 2,457 | 1,318 | 2,764 | `str r3[60]; mov sl,r0; mov r6,r2; mov r4,r1` |
| `expect(shipClass==0,0)` | — | — | 2,712 | `mov r9,r3; mov fp,r2; mov sl,r1` (this spilled) |
| `expect(shipClass==1,0)` (empTime block) | — | — | 2,768 | base map |
| `expect(type==14,1)` | — | — | 2,796 | `mov r8,r0; mov r5,r3; mov r9,r2; mov r4,r1; mov sl,r0` |
| `if (shipClass)` empTime | 2,492 | 496 | 2,780 | `mov r9,r3; mov sl,r2` (shipClass->sl) |
| `else if (shipClass != 0)` branch | — | — | 2,756 | `mov r5,r0; mov sl,r3; mov fp,r2; mov r9,r1` |
| `else if (shipClass >= 1)` branch | — | — | 2,772 | `mov r5,r0; mov r9,r3; mov sl,r2` |
| `level` before jitters | — | — | 2,792 | `mov r4,r0; mov r9,r3; mov r5,r1; mov fp,r0` |
| `mission` after jitters | — | — | 2,768 | base map |
| `unsigned` hitpoints/level/empPoints/empTime/mission/armor/coords | 1,560 | 460 | 2,764 | same as base |
| local `int t = type` (switch / everywhere) | 1,560 | 460 | 2,764 | same as base |
| local `int cls` (top / after mission / after wp / late) | 1,560 | 460 | 2,764 | same as base (coalesced) |
| `switch (shipClass)` for the branch | — | — | 2,780 | `mov r9,r3; mov sl,r2` |
| `shipClass - 1 == 0` (empTime only) | — | — | 2,780 | `mov r9,r3; mov sl,r2` |

## 4. What this rules out / next lever

* The **`int`-coordinate shape is real** (the original computes int x/y/z
  and converts at both call sites), and it *does* put `shipClass` in
  `sl` (`mov r6,r3; mov sl,r2`), but it costs 24 B (2,788) and the raw
  diff jumps to 2,457.  The original's 2,764 B therefore needs the
  int-coord shape **plus** the original's callee-saved map; our
  int-coord build spills `this` (`strd r0,r1` at entry) and puts
  `status` in r9 instead of r4.
* Every pure source spelling of `shipClass`/`race`/`type` locals is
  coalesced by clang 7 at `-Oz`; only a *non-canonical comparison*
  (`if (shipClass)`, `!= 0`, `>= 1`, `- 1 == 0`) changes the RA, and
  every such form changes the size.
* The remaining lever is the original's **PGO branch weights**: the
  greedy allocator's spill weight is `uses / live-range` scaled by
  block frequency, and `type`'s uses in the (cold) capital-ship branch
  must be discounted enough that `shipClass` outranks it for `sl`.
  `__builtin_expect` on a single guard does not reproduce it; a
  synthetic profile (wave-162 `prof_reconstruct_w10820.py` harness) fed
  to clang 7 is the next experiment.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10914
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10914/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb       # 1560
python3 /dev/shm/w10914/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb  # 460
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch `/dev/shm/w10914/`: `run.py` (splice body + compile + size),
`entry.py` (first N instructions), `scan*.py` (variant sweeps),
`linkmeasure.sh` (splice + full gold link + both metrics),
`secdiff.py` (per-section byte diff), `fdiff.py` / `align_diff.py`
(worktree-local copies of the shared metrics).
