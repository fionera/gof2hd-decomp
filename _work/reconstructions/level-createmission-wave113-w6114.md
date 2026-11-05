# w6114: `Level::createMission` (92.094405 -> 92.145215 %, 17172 B) — wave-113:
# `PlayerTurret::setScaling` receiver/position sweep; small landable gain

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6114`, branch `wave113-w6114`,
base `a979ace18`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; addresses below are linked `.so` addresses, function
`0xaffd8`–`0xb431c`).

## Result (committed)

```text
                        before            after
fuzzy                   92.094405 %       92.145215 %
shape (regs collapsed)  94.949662 %       95.049505 %
size                    17172 B           17172 B (size-exact)
insns                   6055 / 6063       6055 / 6065
raw diff regions        317               314
gate                    RATCHET PASS      RATCHET PASS
                        byte 4500/4524, linked 4500/4524, size 4524/4524,
                        stubs 0, missing 0, wrong_type 0, extra 0,
                        data 843/843, imports 207, lint CLEAN, drift 0
ELF shape               27 sections exact, file 2,187,328 B,
                        .ARM.extab 0x26298 (exact), .rodata 0x41b67/0x41c27
                        (ours +0xC0, the known Status rank-table tail)
```

The only source change is inside the `alienGuns` loop of `createMission`:
`PlayerTurret::setScaling(6.0f)` is now issued **after** the first
`MatrixRotateVector` call and through a fresh `(*this->enemies)[i]` load
instead of the cached `gun` local:

```cpp
KIPlayer *gun = (*this->enemies)[i];
gun->player->setMaxHitpoints(1000);
const float (*t)[6] = (bigAlienWave + (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
((PlayerTurret *) (*this->enemies)[i])->setScaling(6.0f);
gun->geometry->setRotation(v);
...
```

The `(i < 0)` codegen anchor and the tuned `setScaling`-before-`setRotation`
order from wave-112 are retained; only the receiver and the exact slot moved.

## Why this helps

The residual of this function is dominated by `RegAllocGreedy` register /
spill-slot assignment.  The `setScaling` call is the one call in the loop whose
source position is not pinned by the original's call order (the original issues
it after `setPosition`; the wave-112 baseline issued it before the table
select).  Moving it one statement later and forcing the `(*enemies)[i]` reload
changes the live-range split around the table select and removes 3 diff regions
(317 -> 314) without changing the function size.  It is a tuned shape, not a
faithful one: the original's call order is `setMaxHitpoints, rot, setRotation,
rot, setPosition, setScaling`.

## Measurements (all on top of the committed baseline)

Every row is a fresh `cmake --build cmake-build-match --target link_so -j6`
followed by the official verifier normalisation (unrounded).

### `setScaling` receiver / position (the winning dimension)

| variant | pct | size |
|---|---:|---:|
| **baseline (`gun`, before table select)** | **92.094405** | 17172 |
| `(*enemies)[i]`, before table select | 92.128713 | 17172 |
| `(*enemies)[i]`, after `setMaxHitpoints` | 92.128713 | 17172 |
| **`(*enemies)[i]`, after first `MatrixRotateVector`** | **92.145215** | 17172 |
| `(*enemies)[i]`, after `setRotation` | 92.145215 | 17172 |
| `(*enemies)[i]`, after `setPosition` | 90.742574 | 17180 |
| `(*enemies)[i]`, after `shipGroup` | 92.095710 | 17172 |
| `(*enemies)[i]`, after `cargo` | 92.079208 | 17172 |
| `gun`, after first `MatrixRotateVector` | 90.774055 | 17164 |
| `gun`, after `setPosition` (wave-112 variant E) | 88.668812 | 17148 |

### Receiver sweep for the other loop statements (best position, 2^5)

Only `setScaling` benefits from the direct receiver; every other combination is
worse:

```text
ggggg (only setScaling direct)   92.145215
dgggg                            92.145215
gdggg                            90.760601
ggdgg                            90.753114
ggggd                            90.744102
gggdg                            90.744102
```

(`mhp, rot, pos, grp, cargo`; `g` = cached `gun`, `d` = `(*enemies)[i]`.)

### Other statements in the same loop

| variant | pct | size |
|---|---:|---:|
| **best (above)** | **92.145215** | 17172 |
| `setMaxHitpoints` moved before the table select | 92.145215 | 17172 |
| `setMaxHitpoints` after `setScaling` | 90.599983 | 17164 |
| `setMaxHitpoints` after `setPosition` | 92.112211 | 17172 |
| `gun` declared after `setScaling` | 90.760601 | 17180 |
| `cargo` before `shipGroup` | 92.128713 | 17172 |
| `PlayerTurret *gun` instead of `KIPlayer *gun` | 92.094405 | 17172 |

### Anchor spellings (on top of the best)

`(bigAlienWave + (i < 0))`, `^`, `-`, `+ (k < 0)` and
`+ ((i | k) < 0)` are all identical codegen (92.145215); `|` is 92.095710 and
the unanchored ternary drops to 88.231894 / 17140 B.

### Fortress loop (2^5 receiver sweep)

No combination beats the committed `gun` form; the best is the unchanged
92.145215, the first direct-receiver variant is 90.760601.

### Previously-neutral tweaks re-checked on top of the best

| variant | pct |
|---|---:|
| `int stationPirates` | 92.145215 |
| `unsigned guns` / `const int guns` | 92.145215 |
| `unsigned extraAliens` | 92.128713 |
| convoy `switch (k)` cases 0,1,2 | 91.996700 |
| `int raidStart = guns + friendCount + ...` (bound expr unchanged) | 92.145215 |

## Still-open clusters (unchanged by this pass)

* `orrs` merged `alienWave` / `pirateFortress` predicates (`0xb2e04`,
  `0xb2f2c`): still short-circuit.  `bool alienWave` fixes it but costs
  ~3.8 pp (wave-112 §1).  `alienWave == 1`, `& 1`, `(unsigned char)` do not
  flip the `InstCombine` operand order (checked: `orrs` count stays 1).
* Branchy in-loop `kAlienStationTurrets` select (`0xb32c6`): still a CMOV.
  `LowerSELECT_CC` always returns `getCMOV` for i32 and `-Oz` disables
  `CodeGenOptimize::optimizeSelectInst`; the branchy form needs a
  non-speculatable instruction in the if/else arms (wave-112 finding 3).
* Convoy `if (k == 2/1/0)` chain (`0xb1408`): original tests 2,1,0 with bodies
  laid out 0,1,2,default; source order 0,1,2 reproduces the test order but the
  machine block placement still lays the bodies out 0,2,1,default.  The
  `switch (k)` form is 91.945866 (literal-pool shift costs more than the chain
  fix gains).
* `raid`/`Globals::status` spill slots are swapped (orig `raid` sp+84,
  `status` sp+88; ours `raid` sp+88, `status` sp+84).  Declaration moves are
  neutral; the assignment order is a `RegAllocGreedy` artefact.
* case 3/5 loop: original reloads `this->enemies->size()` every iteration, ours
  LICM-hoists it (`for (...; i < this->enemies->size(); ...)` = 91.257327).

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w6114/score.py _ZN5Level13createMissionEv [diff|map]
```

Scratch tooling (not committed): `/dev/shm/w6114/score.py`, `shapescore.py`,
`slots.py`, `mn.py`, `ctx.py`, `runvar.py`, `exp*.py`.
