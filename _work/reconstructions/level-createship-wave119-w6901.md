# Level::createShip — wave 119 (w6901): no source win; the 70.3 % RA cascade is rooted in the null-check liveness

**Status:** analysis only, no `src/` change landed (2026-11-05). Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns). Branch
`wave119-w6901`, base `50d2df436`. Worktree left clean; only this note is
committed.

## Result

```
                                   baseline (HEAD)      best measured
createShip fuzzy (official)        70.300000            70.300000
createShip size                    2764 B               2764 B
createShip insns (orig/ours)       1007 / 1005          1007 / 1005
createShip LSDA record             336 B (exact)        336 B (exact)
.text                              0x1337c8 (exact)     0x1337c8 (exact)
.ARM.exidx                         0x19cc08 / 0x6f70    0x19cc08 / 0x6f70
.ARM.extab                         0x26290              0x26290
.rodata                            0x41c27              0x41c27
file size                          2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact          4505 / 4505          4505 / 4505
```

`check` (final, unchanged tree): **RATCHET PASS** — `verify: byte 4505
linked 4505 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of
843`, `sodiff: imports 207`, lint CLEAN, drift 0.  No section size, base,
import, data or symbol-parity movement.

## What was measured this pass

~200 further source variants were built and scored with the single-
function official scorer (a mini one-TU `.so` reproduces the linked
function byte-for-byte, so the percentages are exact).  Every candidate
keeps real `new` for Player / PlayerFixedObject / Array / PlayerFighter /
24×BoundingAAB (the exact 57-site, 28-landing-pad, action-0, 336 B LSDA)
and is exactly 2764 B unless marked.  None beats the checked-in **70.3 %**.

| variant family | best % | size | note |
|---|---:|---:|---|
| **HEAD (one-call clamps + `nc` before `ship`, `iff` coords, XZY jitter)** | **70.3** | 2764 | base |
| null-check position: before `x` / `y` / `z` / `setEmpData` / after `setEmpData` | 62.3–63.3 | 2776–2784 | |
| null-check position: after `ship` / after decl / `!status` / `==0` / cast / `Status *st` | 70.3 | 2764 | tie |
| null-check before `options` line / before `mission==0x9a` / before `armor` / before `Player` | 32.5–59.5 | 2788–2800 | |
| null-check before `setLevel` / before `return` | 63.5 | 2760/2764 | |
| null-check inside fighter / capital branch | 60.2–60.3 | 2768–2772 | |
| extra null-check forms (`level=0`, `hitpoints+=0`) before `armor` + HEAD `nc` | 70.3 | 2764 | tie (merged) |
| `Status *st` local for all accesses (with/without `nc`) | 60.2–62.4 | 2764–2784 | |
| clamp combos {1L,2L}×{1E,2E} × 7 `nc` positions | 58.0–70.3 | 2764–2800 | only 1L1E+`nc`@ship is 2764 |
| zero-call / reused-`level` empPoints | 67.1 | 2764 | |
| `iii` / `iif` / `ifi` / `fif` / `fii` / `ffi` / `fff` coords | 57.5–68.7 | 2764–2788 | `iff` best; `ffi` 68.7 |
| jitter call orders XYZ/XZY/YXZ/YZX/ZXY/ZYX × y/z use swap × `nc` before/after `ship` | 62.5–70.3 | 2764–2768 | six 70.3 ties |
| preamble block order: 50 random topological orders | 27.8–58.2 | 2588–2824 | base order best |
| targeted `F..O` reorders (empPoints early, empTime early/late, armor/Player) | 61.4–69.5 | 2760–2776 | base best |
| hitpoint switch: 6 case permutations / `default` / if-chain | 69.9–70.3 | 2764 | base order best |
| mission 49..56: range / switch / reversed | 65.3–70.3 | 2764–2776 | `switch` ties |
| hitpoints expression reassociations (level-first, paren, split-add, if-form) | 66.0–70.3 | 2764–2784 | ties except `lvl_paren` |
| commutative operand swaps (`mission*4`, `level*14`, `empPoints*5`, float mults) | 70.3 | 2764 | all tie |
| empTime default-init / if-else hitpoints / `empPoints=empPoints*3` | 70.3 | 2764 | ties |
| armor forms (`!`, if, `==0`, `1000-hc*350`) | 70.3 | 2764 | ties |
| `ship` as `PlayerFixedObject *`, `static_cast`/`reinterpret_cast` return | 70.3 | 2764 | ties |
| declaration hoisting (x/y/z, all locals) | 70.3 | 2764 | ties |
| `options` access forms (`*(float*)&`, `((float*)options)[11]`, `scale` local) | 70.3 | 2764 | ties |
| dispatch as `switch(shipClass)` / empTime as `switch` | 41.4 / 70.3 | 2768 / 2764 | switch dispatch bad |
| one-call clamp ternaries (`level = level>20?20:level`) | 70.3 | 2764 | ties |

## The root of the RA cascade (new finding)

The current build's register map is a two-way swap of the original's:

```
value            original          ours (HEAD)
status           r4                r4
rnd              r5                r5
nextInt fn ptr   r6                r6
mission          r8                r8
this             r9 (sp#56)        r9 (sp#32)
shipClass        r5 -> sl -> r6    spilled (sp#60)
type             fp                sl
empPoints        sl                fp
hitpoints        r5 -> fp          r5 -> sl
player           r5                r5
fixed            r9                fp
volumes          fp                r9
fighter          sl                fp
landing-pad exn  r8                r6
delete stubs     sl, fp, r9, r5    r9, fp, r5 (fighter coalesces)
```

The single decision that cascades is **`type` -> `sl` instead of `fp`**.
Once `type` owns `sl`, `shipClass` has no callee-saved register and is
spilled at entry; `empPoints` then takes `fp`; `hitpoints` takes `sl`
after `type` dies; and the capital/fighter branches inherit the whole
permutation (`x` in r6, `z` in sl, fixed in fp, volumes in r9).

The reason `type` beats `shipClass` here is the `nc` hack.  The null check
after `setEmpData` extends the `Globals::status` pointer's live range past
`hardCoreMode`; `r4` therefore cannot be reused for the hardCore result,
so that result goes to `r6`; `r6` is consequently unavailable when
`shipClass` needs a home after the jitter calls, and `shipClass` is
spilled.  In the original, `status` dies at `hardCoreMode` (`0xc0398`),
`r4` is reused for the hardCore result, and `r6` carries `shipClass`
(`0xc02b4` `mov r6,sl`).

The faithful two-call/clamp shape (no `nc`) does free `r4` that way, but
its `Globals::status` GOT-address vreg then loses `r4` to `rnd` at the
prologue (`status -> r6`, `rnd -> r4`, `nextInt -> r5`), so the rest of
the permutation is still wrong and it scores 67.4 %.  No source-level
form found this pass both (a) keeps `status` in `r4` and (b) lets it die
at `hardCoreMode` while staying at exactly 2764 B.

The 2760 B `nc`-before-`setLevel` shape is **not landable**: the full
`check` gives `byte_exact 4505 -> 4490` (the +4 B shift moves `.text` and
breaks unrelated PC-relative literals), confirming that this function
must stay exactly 2764 B.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w6901/score.py` (single-
function official scorer), `runvar.sh` (edit -> link -> score), and the
`sweep_*.py` variant generators.
