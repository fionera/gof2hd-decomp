# Level::createShip — wave 119 (w6916): real-new Greedy-RA permutation is a hard 70.3 % wall

**Status:** analysis only, no source change landed (2026-11-06).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Branch `wave119-w6916`, base `0bbd27a1a`. Worktree left clean; only this
note is committed.

## Result

```
createShip fuzzy (official)        70.300000  (unchanged)
createShip size                    2764 B      (unchanged)
createShip insns (orig/ours)       1007 / 1005
createShip LSDA record             336 B / 57 sites / TType 0xff / table 315 B  (exact)
.ARM.extab size / addr             0x26290 @0x1a3b78  (original 0x26298; 8 B short, other TUs)
byte_exact / linked_exact          4505 / 4505
data / imports / lint / drift      843 / 207 / CLEAN / 0
```

`check` (final): **RATCHET PASS** — `verify: byte 4505 linked 4505 stubs 0
missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff: imports
207`.  No section/base/import/data/parity movement.

## What was measured this pass (official scorer, one-TU mini `.so`)

The mini one-TU link reproduces the linked function byte-for-byte, so the
scores below are exact.  Every row is real `new`, no `try`/`catch`, and
keeps the 336 B LSDA unless marked otherwise.

| variant | raw % | size | LSDA |
|---|---:|---:|---:|
| **HEAD: if/if clamps + status null check + faithful spawn + XZY jitter** | **70.3** | 2764 | 336 |
| faithful shape: `getLevel()>20?20:getLevel()` ternary for *both* level and empPoints, **no** null check | 67.2 | 2764 | 336 |
| ternary level only + if empPoints + null check | 68.1 | 2772 | 336 |
| if level + ternary empPoints + null check | 68.4 | 2772 | 336 |
| if/if + no null check | 63.8 | 2748 | 336 |
| ternary/ternary + no null check + tuned spawn | 65.0 | 2768 | 336 |
| ternary/ternary + null check + tuned spawn | 65.3 | 2760 | 336 |
| `>=21`, `?:`, brace, yoda clamp forms (25 combinations) | 70.3 | 2764 | 336 |
| `bool isCapital`/`bool isFighter`/`int cls` copies at top or at the empTime block | 70.3 | 2764 | 336 |
| `fighter`/`fixed`/`volumes` declared at top (decl-then-assign) | 70.3 | 2764 | 336 |
| `player` decl-then-assign / declared at top | 70.3 | 2764 | 336 |
| spawn reassociation (`(wx-20000)+jx`, `wx+(jx-20000)`, y/z/x order, no casts) | 70.3 | 2764 | 336 |
| spawn split-subtract (`x += ...; x -= 20000;`) | 67.7 | 2776 | 336 |
| coords before `new Player` | 60.9 | 2764 | 336 |
| null check before `setEmpData` | 63.3 | 2776 | 336 |
| null check before `ship` / at end | 63.3 / 69.8 | 2776 / 2764 | 336 |
| **placement-new + `try` (wave-119 w6729 body)** | **74.4** | 2764 | **328** (54 sites, TType 0x00) |
| placement-new, no try | 70.5 | 2776 | 316 (53 sites) |
| placement + try around `new Player` | 62.1 | 2780 | 336 (55 sites, TType 0x00) |
| placement + tail try (setLevel), setEmpData try removed, if-form clamps | 66.6 | 2764 | 336 (TType 0x00) |
| placement + tail try, setEmpData try kept | 67.3 | 2768 | 340 |
| real new for Fighter only (Fixed placement) | 63.6 | 2768 | — |
| real new for Fixed only (Fighter placement) | 65.4 | 2784 | — |

No real-new variant beats 70.3 at 2764 B with the exact 336 B LSDA.  The
only >70.3 body is the placement-new + `try` shape, and it costs 8 B of
`.ARM.extab` (328 vs 336), which the coordinator's structural state pins.

## Why: the pre-RA MIR prologue differs

`-mllvm -print-before-all -mllvm -filter-print-funcs=...` on the same
`Level.cpp` (NDK r18b clang 7.0.2, exact flags) gives the MIR right before
the Virtual Register Rewriter (i.e. after Greedy RA):

```
placement-new + try (74.4 %)          real new (HEAD, 70.3 %)
16B  %684 = COPY $r3   ; type         16B  %129 = COPY $r3   ; type
32B  %701 = COPY $r2   ; shipClass    32B  %128 = COPY $r2   ; shipClass
48B  str $r1, %stack.2 ; race         48B  %127 = COPY $r1   ; race
64B  %681 = COPY $r0   ; this         64B  %126 = COPY $r0   ; this
```

Greedy RA then assigns:

| value | placement-new | real new (HEAD) | original |
|---|---|---|---|
| `type` | `fp` | `sl` | `fp` |
| `shipClass` | `r5 -> sl -> r6` (split) | spilled (`sp#60`) | `r5 -> sl -> r6` |
| `race` | spilled (`sp#60`) | spilled (`sp#52`) | spilled (`sp#60`) |
| `empPoints` | `sl` | `fp` | `sl` |
| `hitpoints` | `fp` (after type spill) | `sl`/`fp` | `fp` |
| exception ptr | — | `r6` | `r8` |

The original is real `new` (57 cleanup-only sites, TType 0xff) yet has the
*placement-new* prologue (`str r1,[sp,#60]`, `mov fp,r3`, `mov r5,r2`,
status in `r4`, nextInt fn ptr in `r6`).  The three real-`new` cleanup
landing pads keep the allocated pointer live across each ctor call, which
raises pressure at exactly the point where `shipClass` is live; Greedy RA
then spills `shipClass` instead of splitting it, and the whole allocation
cascades (status `r6`<->`r4`, nextInt `r4`<->`r6`, type `sl`<->`fp`,
`ship` `fp` vs `r9`, 3 delete stubs instead of 4).  No source-level
rewrite found here changes that first Greedy decision while keeping
2764 B and the 336 B record.

## Structural deltas left at 70.3 (registers normalised: 80.9 % LCS)

With all register names canonicalised the LCS rises to 80.9 %, so ~10
points are pure RA and ~19 points are structure:

* the original computes `x`/`y`/`z` (all `int`) **before** `setEmpData`
  (`0xc03c8..0xc03e4`); ours sinks the 13-instruction block after the call
  because the `Globals::status == nullptr` branch sits between them;
* the 24 `BoundingAAB` ctor argument setups use different constant-load /
  `strd` store sequences;
* the fighter-branch `type` compares use `cmp.w sl` vs `cmp r4`;
* the landing-pad block is at `+0x9b8` on both sides (24 trampolines match).

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w6916/{build.sh,score.sh,
tryvar.py,batch.py,parscore.py,measure.py,lsda3.py,build_mir2.sh}`.
The placement-new body used above is the w6729 body (commit `e2fa39a15`)
with the wave-119 w6833 XZY jitter order; it scores 74.4 / 2764 B / 328 B.
