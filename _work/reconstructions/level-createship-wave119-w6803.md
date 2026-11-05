# Level::createShip — wave 119 (w6803): real-new RA is pinned by the 336 B LSDA

**Status:** analysis only, no source change landed (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch
`wave119-w6803`, base `a05e21d52`. Worktree left clean; only this note is
committed.

## Result

```
createShip fuzzy (raw)            67.400000  (unchanged)
createShip size                   2764 B      (unchanged)
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

All numbers below are on a one-TU mini harness that reproduces the linked
`createShip` byte-for-byte (same headers, same clang 7.0.2 flag line).  Its
score has a constant −0.5…−0.6 offset vs the official verifier
(mini `68.0` == official `67.4`), so it is used only for relative ranking.
Every candidate keeps real `new` for Player / PlayerFixedObject / Array /
PlayerFighter / 24×BoundingAAB, i.e. the exact 57-site, 28-landing-pad,
action-0 LSDA (336 B).

| variant | mini % | official % | size | extab |
|---|---:|---:|---:|---:|
| **HEAD (int x, float y, float z)** | **68.0** | **67.4** | 2764 | 336 |
| 216 waypoint-decl × waypoint-assign × jitter-order combos | ≤68.0 | — | 2764 | 336 |
| 120 `switch (race)` case orderings (incl. `default` last) | all 68.0 | — | 2764 | 336 |
| 2500 random combos of 19 statement-level rewrites | ≤68.0 | — | 2764 | 336 |
| `int x, int y, int z` (the *faithful* form, see below) | 63.6 | 63.1 | 2792 | 336 |
| `iif` / `ifi` / `fif` / `ffi` / `fii` / `fff` coords | 64.5/63.5/66.9/66.2/62.0/57.0 | — | 2764-2792 | 336 |
| `Status *status` local / `AERandom *rnd` local | 61.x / 66.x | — | 2768 | 336 |
| `armor` inline in the `new Player(...)` call | 63.5 | — | 2784 | 336 |
| coords before `new Player` | 62.7 | — | 2788 | 336 |
| `switch (shipClass)` dispatch | 63.4-63.8 | — | 2768 | 336 |
| `KIPlayer *ship` uninitialised / `= 0` / `Level *self` alias | 68.0 | 67.4 | 2764 | 336 |
| `missionPtr` nested-if / inverted-if / `int mp` copy | 68.0 | 67.4 | 2764 | 336 |
| `unsigned`/`const`/`auto`/`static_cast` spelling variants | 68.0 | — | 2764 | 336 |
| `fixed`/`volumes`/`fighter` declared first, decl-then-assign | 68.0 | — | 2764 | 336 |

Five of the 68.0 ties (`nested`, `ship_zero`, `self_top`, `kind_top`,
`et_init`) were re-built and re-scored with the official verifier: all
**67.4**, 2764 B, extab `0x26294`.  Nothing beats HEAD.

## The prologue is the RA fingerprint, and only EH changes move it

Original entry (`0xc0220`):

```
str  r1, [sp, #60]      ; race -> stack
mov  r9, r0             ; this -> r9
ldr  r0, [pc, #572]
mov  fp, r3             ; type -> fp
mov  r5, r2             ; shipClass -> r5
add  r0, pc
ldr  r4, [r0, #0]       ; status -> r4
...
mov  sl, r5             ; sl = shipClass
```

HEAD entry:

```
strd r1, r2, [sp, #56]  ; race -> #56, shipClass -> #60
mov  r9, r0             ; this -> r9
ldr  r0, [pc, #576]
mov  sl, r3             ; type -> sl
add  r0, pc
ldr  r6, [r0, #0]       ; status -> r6
```

So the whole callee-saved permutation is:

| value | original | HEAD |
|---|---|---|
| status | r4 | r6 |
| shipClass | r5 → sl → r6 | spilled at entry |
| nextInt fn ptr | r6 | r4 |
| mission | r8 | r8 |
| this | r9, spilled at `0xc0332` | r9, spilled at `0xc0332` |
| empPoints | sl | fp |
| type | fp, spilled at `0xc0336` | sl, kept |
| landing-pad exception ptr | r8 | r6 |
| landing-pad delete targets | sl, fp, r9, r5 (4 distinct) | r9, fp, r5 (3; fixed+fighter coalesce) |

The *only* way found to make Greedy RA reproduce the original entry is to
change the exception-handling shape.  Measured (mini %, extab size of the
createShip record):

| EH shape | mini % | extab |
|---|---:|---:|
| HEAD: all real `new` | 68.0 | **336 (exact)** |
| placement-`new` PlayerFixedObject only | 67.6 | 328 |
| placement-`new` PlayerFighter only | 67.6 | 328 |
| placement-`new` both ship ctors | 68.8 | 316 |
| `try` around `setEmpData` only | 64.1 | 352 |
| `try` + placement-`new` both ship ctors | **72.0** | 328 |

The last row reproduces the original entry *instruction for instruction*
(`str r1,[sp,#60]; mov r9,r0; ldr r0,[pc]; mov fp,r3; mov r5,r2; …
mov sl,r5`) and is the 72.7 % placement shape reported in
`level-createship-wave119-w6785.md`, but its LSDA record is 328 B (27
landing pads), so the 336 B requirement rules it out.  Typed-catch routes
that restore 336 B move `_ZTI*` dynsym indices / grow `.rel.dyn`, as
already recorded in the w6785 note.

## The one real source-shape discrepancy: the spawn coordinates are `int`

The original computes the three spawn coordinates as **integers** and
converts at each use site:

```
0xc03dc  sub.w fp, r0, r1        ; x = wx + jitterX - 20000   (int, fp)
0xc03e2  subs  r4, r2, r1        ; y = wy + jitterY - 20000   (int, r4)
0xc03e4  sub.w r8, r3, r1        ; z = wz + jitterZ - 20000   (int, r8)
0xc03e8  mov   r0, r5 / mov r1, sl / mov r2, r9 / blx setEmpData
...
0xc040c  vmov s0, fp / vcvt.f32.s32 s20, s0     ; (float)x, capital branch
0xc0416  vmov s0, r4 / vcvt.f32.s32 s18, s0     ; (float)y
0xc041e  vmov s0, r8 / vcvt.f32.s32 s16, s0     ; (float)z
...
0xc06c0  vmov s0, fp / vmov s2, r8 / vmov s4, r4  ; again in the fighter branch
```

HEAD declares `float y` / `float z`, so clang converts once and (more
importantly) *sinks* the whole coordinate computation past the
`setEmpData` call:

```
ours: Player ctor, setEmpData, then the coords   (3 + 26 instructions)
orig: Player ctor, coords, setEmpData            (16 + 7 instructions)
```

Instruction totals: original **1007**, HEAD **994**.  Switching to
`int x, y, z` gives 1008 instructions (nearly exact) and keeps the 336 B
LSDA, but it re-permutes the callee-saved registers (type is spilled at
entry, shipClass goes to sl) and scores **63.1 %** officially — worse than
HEAD.  All 8 int/float combinations were measured; `iff` (HEAD) is best.

## How much of the gap is RA

Re-scoring the same instruction streams with every register name
canonicalised to `R` and every immediate to `N` gives **82.06 %** instead
of 67.4 %.  So roughly two thirds of the residual is pure register
allocation (dominated by 24 identical `BoundingAAB` blocks whose only
difference is `volumes` in `fp` vs `r9`), and about a sixth is the
13-instruction structural delta described above.

## Conclusion

With the EH shape pinned to real `new` (57 call sites / 28 cleanup pads /
336 B LSDA) the Greedy register allocator deterministically picks the
HEAD permutation for every source shape tried; the target permutation is
reachable only by changing the EH shape, which breaks the LSDA.  HEAD's
67.4 % is a hard local optimum for the 336 B constraint.  The one genuine
source-shape finding (integer spawn coordinates) is recorded above but
scores lower, so it is not landed.

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

Scratch helpers (not committed): `/dev/shm/w6803/probe.py` (one-TU
compile + prologue/landing-pad probe), `mini_score2.py` (masked mini
score), `s/search*.py` (the sweeps tabulated above), `s/calls.py`
(per-call-region instruction counts), `s/diff.py` (aligned instruction
diff).
