# w6804: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# convoy-chain / case4Tail-loop / declaration-order sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6804`, branch `wave119-w6804`,
based on current master. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).
This continues `level-createmission-wave119-w6794.md` and `…-w6782.md`.
`src/game/world/Level.cpp` is unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this wave)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

## 1. New: the `case4Tail` hunter loop has a real source bug (`volatile unsigned n`)

The source at Level.cpp:2110 reads

```cpp
    volatile unsigned n = this->enemies->size();
    for (unsigned i = 0; i < n; i++) {
```

The original does **not** cache the size: it re-reads `this->enemies->size()`
on every back edge.

```text
orig  b41c8: ldr.w r0, [r8, #248]     ; this->enemies
orig  b41cc: ldr   r0, [r0, #0]       ; ->size()
orig  b41ce: cmp   r9, r0
orig  b41d0: bcc.n b416e

ours  b41c8: ldr   r0, [sp, #312]     ; cached volatile n
ours  b41ca: cmp   r9, r0
ours  b41cc: bcc.n b416c
```

Removing the local (`for (unsigned i = 0; i < this->enemies->size(); i++)`)
reproduces the original loop exactly (and also fixes the `hunterCount`
sp+120 -> sp+124 spill slot), but the function becomes 17164 B (-8 B, one
fewer EH landing-pad entry in the `.ARM.extab` cleanup chain) and the whole
tail shifts 8 B, so the *sequence* score drops:

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile unsigned n`** | **92.293729** | 17172 | 6065 |
| `for (i < this->enemies->size())` (faithful) | 91.308296 | 17164 | 6060 |
| `unsigned n` (non-volatile) | 92.145215 | 17172 | 6065 |
| `const unsigned n` | 92.145215 | 17172 | 6065 |
| `int n` + `(unsigned) n` | 92.145215 | 17172 | 6065 |
| `volatile n` but condition uses `size()` | 90.867090 | 17176 | 6066 |
| faithful + cached-`gun` turret body | 91.174771 | 17164 | 6058 |
| non-volatile `n` + cached-`gun` turret body | 92.011883 | 17172 | 6063 |

Per-region match for the faithful form: `case6` 99.29 -> **100.00 %**, but
`tail` 88.89 -> 77.60 % (pure 8-byte alignment loss).  The faithful form is
the *correct* source shape; the tuned one is what the fuzzy metric prefers.

## 2. Convoy k-chain (`0xb140c`-`0xb1458`): body order is `[0][1][2][default]`

The original's comparison chain and body layout:

```text
orig b140c: cmp r1,#2 / b1418 beq b1438   ; k==2 body at b1438
orig b141a: cmp r1,#1 / b141c beq b142c   ; k==1 body at b142c
orig b141e: cbnz r1, b1444                ; default at b1444
orig b1420: k==0 body (fallthrough)       ; [0][1][2][default]
```

An `if (k==0) … else if (k==1) … else if (k==2) …` chain (or the equivalent
`switch`) now reproduces the **descending** chain (`cmp #2`, `cmp #1`,
`cbnz`) and puts the `k==0` body at `b1420`, but LLVM's block placement lays
the remaining bodies out `[0][2][1][default]`, so the score still drops:

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline if-chain 2,1,0** | **92.293729** | 17172 | 6065 |
| if-chain / switch 2,0,1 | 92.293729 | 17172 | 6065 |
| if-chain / switch 2,1,0 | 92.293729 | 17172 | 6065 |
| if-chain / switch 1,0,2 | 92.161716 | 17172 | 6065 |
| if-chain / switch 1,2,0 | 92.161716 | 17172 | 6065 |
| if-chain / switch 0,1,2 | 92.145215 | 17172 | 6065 |
| if-chain / switch 0,2,1 | 92.145215 | 17172 | 6065 |

Constant-pool census confirms the body order: the original pool is
`-3000, 20000, -50000, -7000, 10000, -30000, 25000, -36000, 6000, 12000,
-45000` (k0, k1, k2, default); ours is `25000, -36000, -3000, 20000,
-50000, -7000, 10000, -30000, -45000, 6000, 12000` (k2, k0, k1, default).
All four bodies store in the order `offset[1], offset[0], offset[2]` with the
`offset[2]` (`sp+380`) store shared in a common tail — that part already
matches.  Re-writing the four bodies in explicit `offset[1]; offset[2];
offset[0];` order makes it much worse (88.59 %, 17224 B, 6075 insns).

## 3. Turret loop: further declaration-order permutations (all no-ops or worse)

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.293729** | 17172 | 6065 |
| `Vector v` declared before `Matrix rotation` | 92.161716 | 17172 | 6065 |
| `int k` before `Vector v` | 92.293729 | 17172 | 6065 |
| `int k` first | 92.293729 | 17172 | 6065 |
| `Vector base` first | 92.062706 | 17172 | 6065 |
| `Vector base; Vector v; Matrix rotation; int k` | 91.914191 | 17172 | 6065 |
| `const float (*t)[6] = outpost; if (bigAlienWave + (i<0)) t = station;` | 92.293729 | 17172 | 6065 |
| `… = station; if (!(bigAlienWave + (i<0))) t = outpost;` | 92.293729 | 17172 | 6065 |
| `… if (bigAlienWave + (i<0) != 0) …` / braced form | 92.293729 | 17172 | 6065 |
| `KIPlayer *&gun = (*enemies)[i]` (reference) | 92.121112 | 17172 | 6066 |
| `Player *p = gun->player; p->setMaxHitpoints(1000)` | 92.293729 | 17172 | 6065 |
| `Array<KIPlayer*> &enemies = *this->enemies; gun = enemies[i]` | 92.293729 | 17172 | 6065 |
| `setMaxHitpoints` moved after the table select | 90.904589 | 17156 | 6061 |
| hoisted loop bound (`first`/`last` locals) | 87.251423 | 17180 | 6064 |
| hoisted bound + faithful body | 88.197425 | 17172 | 6061 |
| `Vector v` declared inside the loop | 88.542183 | 17156 | 6059 |

The `(i < 0)` anchor is still load-bearing: every faithful table-select form
keeps the `t2MOVCCr` and the in-loop placement only because of it.

## 4. `&&` at Level.cpp:1904 / 1924 — operand order and bool locals are no-ops

The original lowers both `i == X && flag` tests branchlessly
(`subs; mov; it ne; movne #1; ldr; eor #1; orrs; bne`); ours short-circuits
(`ldr; cmp #0; beq; cmp; bne`).  Swapping the `&&` operands, hoisting
`bool isFirst = i == X;`, or `bool isA = isFirst && alienWave;` all produce
*identical* code to the baseline (92.293729 / 17172 / 6065) — LLVM
canonicalises them back to the short-circuit form.

## 5. `raid` / `&__stack_chk_guard` 84/88 spill-slot swap — still immovable

The first instruction of the function still differs
(`str.w fp,[sp,#88]` vs `[sp,#84]`, and `raid` the other way round; 17
instructions affected, ~+0.31 pp if flipped).  New perturbations tried this
wave, all identical to the baseline (92.293729 / 17172 / 6065):

- `bool raid = false; if (!(introSystem && !hardCoreMode())) raid = …;`
- `bool raid = !(introSystem && !hardCoreMode()) && raidRoll < …;`
- `bool raid = introSystem && !hardCoreMode() ? false : raidRoll < …;`
- `bool raid;` declared before `int security`
- `bool raid;` declared before `int raidRoll`

## 6. Other measured negatives

| variant | official % | size | insns |
|---|---:|---:|---:|
| `convoyShip->cargo = nullptr` / `crateCaptured() = 0` swapped | 92.293729 | 17172 | 6065 |
| `Vector v` / `int k` / `Vector base` permutations (see §3) | — | — | — |

## 7. Conclusion

HEAD remains a local optimum at **92.293729 %** (17172 B, 6055/6065 insns).
The two genuinely-wrong source shapes found this wave are:

1. `case4Tail`'s `volatile unsigned n` (original re-reads
   `this->enemies->size()`), and
2. the convoy k-chain body order `[0][1][2][default]` (LLVM only gives
   `[0][2][1][default]` from any source order).

Both are faithful-source wins that *lower* the fuzzy metric because of the
8-byte tail shift / block-placement change they cause.  The remaining ~7.7 %
is greedy-allocator colour choice (`this` r4 vs r5 in the turret loop,
`&rotation` rematerialised vs hoisted, `raid`/guard slot order) plus
literal-pool ordering, none of which is reachable from C++ source without
changing semantics.

## 8. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6804/score.py                  # official %, size, insns
python3 /dev/shm/w6804/regions2.py               # per-region match census
python3 /dev/shm/w6804/sweep.py <variants.json>  # apply/build/score/restore
```

Scratch under `/dev/shm/w6804` (`Level.base.cpp` = pristine HEAD copy,
`cc.sh` = direct clang compile with the exact build flags,
`sem2.py` = structural mismatch blocks after masking registers/branches).
