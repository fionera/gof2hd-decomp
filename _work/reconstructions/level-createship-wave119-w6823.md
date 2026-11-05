# Level::createShip — wave 119 (w6823): real-new 336 B LSDA is a hard 67.4 % optimum

**Status:** analysis only, no `src/` change landed (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch
`wave119-w6823`, base `d4d2d3f91`. Worktree left clean; only this note is
committed.

## Result

```
createShip fuzzy (raw)            67.400000  (unchanged)
createShip size                   2764 B      (unchanged)
createShip insns (orig/ours)      1007 / 994
createShip LSDA record            336 B / 57 sites / TType 0xff  (exact)
.ARM.extab size / addr            0x26294 (original 0x26298; Generator -4)
.text                             0x1337c8    (exact)
byte_exact / linked_exact         4505 / 4505
imports / data / lint / drift     207 / 843 / CLEAN / 0
```

`check` (final): **RATCHET PASS** — `verify: byte 4505 linked 4505 stubs 0
missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff: 0/0
imports 207`, lint CLEAN, drift 0.  No section size, base, import, data or
symbol-parity movement.

## What was measured this pass

All numbers below come from a one-TU mini harness (same headers, same
clang 7.0.2 `-Oz` flag line, gold link) whose score reproduces the official
verifier exactly for the 2764 B shapes (`67.4`).  Every candidate keeps real
`new` for Player / PlayerFixedObject / Array / PlayerFighter / 24×BoundingAAB,
i.e. the exact 57-site, 28-landing-pad, action-0 LSDA (336 B), unless marked.

| variant | % | size | LSDA | note |
|---|---:|---:|---:|---|
| **HEAD (real new, `iff` coords)** | **67.4** | 2764 | 336 | base |
| `if (Globals::status == nullptr) return nullptr;` before dispatch | **68.3** | 2780 | 336 | +16 B, not landable |
| same null check before `ship` / after `ship` / `!status` / `== 0` | 68.3 | 2780 | 336 | same shape |
| same null check before `setEmpData` | 64.4 | 2788 | 336 | |
| same null check before `setLevel` | 63.8 | 2776 | 336 | |
| same null check at top / after waypoint / after level / after hitpoints | 58.4–59.0 | 2792–2804 | 336 | |
| `new Player(..., Globals::status != nullptr, 1, 0)` (3rd arg) | 65.3 | **2764** | 336 | zero-cost status use, but RA worse |
| `new Player(..., 1, Globals::status != nullptr, 0)` (4th arg) | 65.1 | **2764** | 336 | |
| `armor + (Globals::status == nullptr)` | 65.0 | 2768 | 336 | |
| `return Globals::status ? ship : nullptr;` | 63.9 | 2780 | 336 | |
| `return (PlayerFixedObject *)(Globals::status ? ship : ship);` | 67.4 | 2764 | 336 | tie |
| `int x, y, z` (faithful int coords, `iii`) | 63.1 | 2792 | 336 | 1008 insns |
| `iii` with decl-top / assign-only / explicit float locals | 63.1 | 2792 | 336 | all identical |
| `iif` / `ifi` / `fif` / `ffi` / `fii` / `fff` coords | 64.0/63.0/66.3/65.6/61.5/56.9 | 2764–2792 | 336 | |
| `int sc = shipClass`, `int ty = type`, `int rc = race` copies (top / after wp / dispatch only) | 67.4 | 2764 | 336 | tie |
| `int ty = type` after the hitpoints switch, used in cargo/ctors | 67.4 | 2764 | 336 | tie |
| explicit `float fx/fy/fz` locals in capital branch | 67.4 | 2764 | 336 | tie |
| explicit `float fx/fy/fz` locals in fighter branch | 67.3 | 2764 | 336 | |
| `Vector spawn = {...}` then int/float coords | 56.4 | 2792 | 336 | |
| dummy `if (x == 0x7fffffff) setEmpData(0,0);` to force coords before `setEmpData` | 67.4 | 2764 | 336 | tie |
| armor/alien-orbit block swapped (armor first) | 62.8 | 2784 | 336 | |
| `if (shipClass == 2) return nullptr;` late use | 48.8 | 2792 | 336 | |
| `if (type == 2) return nullptr;` late use | 51.1 | 2768 | 336 | |
| placement-new both ship ctors (wave-89 shape, no try) | 72.7 | 2764 | 328 | |
| **placement-new both ship ctors + `try` around `setEmpData`** | **73.5** | 2764 | **328** | best measured overall |
| placement-new both, no try | 69.5 | 2780 | 320 | |
| placement-new fixed only + try | 61.6 | 2768 | 336 | |
| placement-new fighter only + try | 63.0 | 2768 | 336 | |
| placement-new both + try around `setShipGroup` | 66.4 | 2764 | 336 | |
| placement-new both + try around `setLevel` | 66.4 | 2768 | 336 | |
| placement-new both + try around `addObject` | 66.4 | 2768 | 336 | |
| placement-new both + try around `ArraySetLength` (case 1/2/3) | 66.4 | 2768 | 336 | |
| placement-new both + try around `setWreckedMeshId` | 66.3 | 2768 | 336 | |

No variant at exactly **2764 B** with the 336 B LSDA beats **67.4 %**.
The 68.3 % shape is the only measured improvement and it is 16 B too large
(any size change moves `.text` off `0x1337c8` and collapses `byte_exact`).

## Why 67.4 is the real-new ceiling

`-mllvm -print-machineinstrs=virtregrewriter` on the mini TU gives the
post-RA prologue.  Real `new` (HEAD) allocates:

```
HEAD   $r10 = COPY $r3        ; type      -> sl
       str  $r2, stack.3      ; shipClass -> stack
       str  $r1, stack.2      ; race      -> stack
       $r9  = COPY $r0        ; this      -> r9
       $r6  = &Globals::status           -> r6
       $r4  = nextInt fn ptr             -> r4
       $r5  = rnd ptr                    -> r5
```

The original allocates:

```
orig   str  r1, [sp,#60]      ; race      -> stack
       mov  r9, r0            ; this      -> r9
       mov  fp, r3            ; type      -> fp
       mov  r5, r2            ; shipClass -> r5 (then sl, then r6)
       ldr  r4, [r0]          ; status    -> r4
       ldr  r6, [r1]          ; nextInt   -> r6
       ldr  r5, [r0]          ; rnd       -> r5 (after shipClass moved)
```

The whole difference is a cyclic permutation of the callee-saved set:
`status r6↔r4`, `nextInt r4↔r6`, `type sl↔fp`, `shipClass stack↔r5`.
`this→r9`, `mission→r8` and `rnd→r5` already match.

The null check is the only source-level lever found that moves `status` to
`r4`: it adds a late `ldr` of `Globals::status` after `setEmpData`, which
lengthens the `&Globals::status` live interval (`%137`, 3824 slots → ~4300)
enough to outrank the local `rnd`/`nextInt` ranges.  It costs 16 B
(5 instructions), which is fatal because `.text` is size-exact at
`0x1337c8`.  Cheaper status uses that fit in 2764 B (e.g. passing
`Globals::status != nullptr` as a `Player` ctor argument) move `status` to
`r4` but perturb the rest of the allocation and score 65.1–65.3.

The placement-new shape (wave-89: `::operator new` + placement `new` for
both ship ctors) reproduces the original prologue instruction-for-instruction
and reaches 73.5 %, but it has one fewer landing pad, so its LSDA record is
**328 B** (`.ARM.extab` 0x2628c).  Adding any `try` to restore the 28th
landing pad costs ~7 points (66.3–66.4 %), because the extra landing-pad
exception vreg re-permutes the whole callee-saved allocation.  The
"placement-new fixed only + try" / "fighter only + try" shapes do restore
336 B but score 61.6 / 63.0.

## Structural state

27 sections, file size 2,187,328 B, `.text` `0x1337c8`, `.ARM.exidx`
`0x6f70`, `.ARM.extab` `0x26294` (original `0x26298`; the only remaining
short record is `Generator::createMission` 168 vs 172, in another TU),
`.rodata` `0x41c27`.  `createShip`'s own 336 B LSDA (57 sites, TType `0xff`)
is exact in the baseline and in every real-new variant above.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch helpers (not committed): `/dev/shm/w6823/sweep.py` (fast one-TU
variant scorer), `measure.py` (score + size + `.ARM.extab` size),
`extab.py` (per-function LSDA record sizes from `.ARM.exidx`),
`vdiff2.py` (canonicalised instruction diff), `build_vrr.sh`
(`-print-machineinstrs=virtregrewriter` dump).
