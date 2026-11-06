# w7417: `Level::createMission` (92.326733 -> 92.326733 %, 17172 B) — wave-124
# nameLeader return shapes, alien-turret cached/fresh `gun`, case4Tail latch; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7417`, branch `wave124-w7417`,
based on current master (`6f8ee2320`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6…bed9483`, image base 0x10000; function `0xaffd8`-`0xb42ec`,
17172 B).  Continues `level-createmission-wave121-w7112.md`,
`…-wave122-w7214.md`, `…-wave123-w7310.md`.  `src/game/world/Level.cpp` is
unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The fuzzy ratio is `2*M/(6055+6065)` with `M = 5595`.  One extra matching
instruction is `+0.0165 pp` (92.3267 -> 92.3432).  The call sequence and every
literal-pool word are already identical (wave-122/123), so the residual is
entirely backend: register allocation, stack-slot colouring, SLP pairing and
MachineBlockPlacement.

## 1. `nameLeader`: every explicit return re-places `case4Else`

Original isEmpty-branch tail (`+0x40b2`):

```text
0x40b2  ldr   r0,[sp,#28]        ; nameLeader
0x40b4  cmp   r0,#0
0x40b6  beq.w <epilogue>         ; !nameLeader -> return
0x40ba  …  blx getText(1663)     ; name body
0x40da  b.w   <epilogue>         ; nameLeader -> return
0x40de  ldr.w r8,[sp,#124]       ; case4Else (only predecessor: the case-4 goto)
```

The baseline `else { return; }` keeps the name body as the fall-through
predecessor of `case4Else`, so the two blocks stay adjacent (the price is the
missing `b.w`).  Every spelling that makes the name body return unconditionally
gives `case4Else` only the case-4 predecessor; MBP then places `case4Else`
right after the case-4 block (at `+0x3774`) and the name body at the end of the
function (`+0x41de`), shrinking the function by 20 B and moving the pools.

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `else { return; }`** | **92.326733** | 17172 | 6065 |
| `if (nameLeader) { … } return;` | 88.432004 | 17152 | 6056 |
| `if (nameLeader) { …; return; }` (no else) | 91.237624 | 17172 | 6065 |
| `if (!nameLeader) return; name=…; return;` | 88.432004 | 17152 | 6056 |
| `if (nameLeader) { … } else { return; } return;` | 88.432004 | 17152 | 6056 |
| `if (nameLeader) { …; goto case4Return; } else goto case4Return;` + `case4Return:` after `case4Tail` | 88.432004 | 17152 | 6056 |
| `if (nameLeader) { …; return; } else goto case4Return;` + shared label | 88.432004 | 17152 | 6056 |
| `{ if (nameLeader) { …; return; } } return;` (helper scope) | 88.432004 | 17152 | 6056 |
| `do { if (nameLeader) { … } break; } while (0); return;` | 88.432004 | 17152 | 6056 |
| `if (nameLeader) { … } else if (1) { return; } return;` | 88.432004 | 17152 | 6056 |
| `if (nameLeader) { … } if (this == nullptr) return;` | 91.162637 | 17168 | 6064 |
| `if (nameLeader) { … } if (mission == nullptr) return;` | 91.162637 | 17168 | 6064 |
| `if (nameLeader) { … } if (nameLeader != 0) return;` | 91.237624 | 17172 | 6065 |
| move `case4Else`/`case4Tail` outside the isEmpty block + `return;` | 88.432004 | 17152 | 6056 |

The `91.237624` form (`if (nameLeader) { …; return; }`, no else) is the only
size-exact non-baseline shape.  It reproduces the original's `b.w <epilogue>`
at `+0x40a2` and `case4Else` at `+0x40a6`, but its false path falls through to
`case4Else` (the original returns) and its `case4Else` uses `sl` for `this`
instead of the original's `ldr.w r8,[sp,#124]`.  Moving the labels outside the
isEmpty block does not change MBP's decision.

## 2. Alien-turret loop: cached `gun` vs fresh `(*enemies)[i]`

Original (`+0x329a`): `this`=r4 (reloaded at `+0x336e` after `&rotation`
clobbers it), `gun`=r6, `v`(sp+212)=r5, `k*24`=r9, `i`=fp, table=sl.  Baseline:
`this`=r5 (live across the loop), `gun`=r4, `v`=r8, `i`=r9, `k*24`=fp,
table=r6.  Caching `gun` for `setScaling` removes the fresh load and drops the
instruction count to 6063, but the register permutation survives (this stays
r5, gun r4, v r9/r8, k r8/fp, table r6).

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline (fresh `(*enemies)[i]`, scale before rotate, shipGroup then cargo)** | **92.326733** | 17172 | 6065 |
| cached `gun`, scale before rotate, shipGroup then cargo | 92.275953 | 17172 | 6063 |
| cached `gun`, scale after position, shipGroup then cargo | 92.209936 | 17172 | 6063 |
| cached `gun`, scale after position, cargo then shipGroup (faithful) | 92.193431 | 17172 | 6063 |
| fresh, scale after position, shipGroup then cargo | 92.277228 | 17172 | 6065 |
| fresh, scale after position, cargo then shipGroup | 92.260726 | 17172 | 6065 |
| cached `gun`, scale after rotate (before position) | 90.955603 | 17164 | 6063 |
| statement-`if` table select (instead of ternary) | 92.275953 | 17172 | 6063 |
| `(bigAlienWave && i >= 0)` anchor | 89.745071 | 17180 | 6066 |
| `(bigAlienWave \|\| i < 0)` anchor | 89.770665 | 17180 | 6067 |
| faithful + `Vector a`/`Vector b` named locals | 90.571334 | 17156 | 6057 |
| faithful + `PlayerTurret *gun` | 92.193431 | 17172 | 6063 |
| faithful + named `t0..t5` | 92.193431 | 17172 | 6063 |
| `int k` in for-init / `unsigned k` | 92.326733 | 17172 | 6065 |
| `unsigned i` loop variable | 88.413577 | 17140 | 6054 |
| `Vector pos` local for `setPosition` | 92.326733 | 17172 | 6065 |
| `setPosition(base.x+v.x, …)` three float args | 90.805546 | 17164 | 6061 |
| `Vector v2 = MatrixRotateVector(...)` (two named results) | 88.698093 | 17160 | 6058 |
| inline `setRotation(MatrixRotateVector(...))` | 88.789830 | 17156 | 6059 |
| cache `base.x/y/z` in floats | 92.227723 | 17172 | 6065 |
| `PlayerTurret *gun` with baseline order | 92.326733 | 17172 | 6065 |

The original's table select is a real in-loop `cbz` branch; the current
`(bigAlienWave + (i < 0))` anchor is required to keep it in-loop but clang
if-converts it to `cmp.w r1,r9,asr #31; it ne; movne r6,r0`.  A plain ternary
or statement-`if` is hoisted by LICM (88.413577 / 17140).  No spelling reaches
the original's `cbz`, and no cached-`gun` form reaches the original's
`this`=r4 / `gun`=r6 assignment.

## 3. `case4Tail`: `fp = 1` vs `fp = &Globals::rnd`

Original (`+0x415c`): `this`=r8, `Globals::globals`=sl, `Globals::rnd` spilled
at `sp+120` and reloaded each iteration, `fp = 1` (the `hostile` argument),
`i`=r9, and the loop condition re-reads `this->enemies->size()`.  Baseline
keeps `fp = &Globals::rnd` and materialises `1` with `movs r0,#1; str
r0,[sp,#4]` instead of the original's `mov.w fp,#1` + `strd r0,fp,[sp]`.

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile unsigned n` latch** | **92.326733** | 17172 | 6065 |
| faithful `i < this->enemies->size()` (no latch) | 91.341312 | 17164 | 6060 |
| faithful + `const bool hostile = true` / `int hostile = 1` / two bools | 91.341312 | 17164 | 6060 |
| faithful + `1, 0` / `(bool)1,(bool)0` / `(unsigned)` cast | 91.341312 | 17164 | 6060 |
| faithful + `Array<KIPlayer*> *enemies` cache | 91.144673 | 17164 | 6060 |
| `volatile bool hostile = true` + faithful | 91.195643 | 17180 | 6064 |
| plain `unsigned n` / `const unsigned n` | 92.178218 | 17172 | 6065 |
| `volatile int n` / `volatile unsigned long` / `volatile size_t` | 92.326733 | 17172 | 6065 |
| `volatile unsigned n = (unsigned) size()` | 92.326733 | 17172 | 6065 |
| `volatile unsigned n = hunterCount` | 91.261655 | 17168 | 6064 |
| `const bool hostile = true` + latch | 92.326733 | 17172 | 6065 |
| `int hostile = 1` + latch | 92.326733 | 17172 | 6065 |
| `while (i < n)` | 92.326733 | 17172 | 6065 |
| `do { … } while (i < n)` | 91.210696 | 17168 | 6062 |
| `Waypoint *wp` local | 90.973597 | 17172 | 6065 |
| `int type` local | 92.326733 | 17172 | 6065 |
| `Route *route = this->playerRoute` local | 92.136315 | 17172 | 6064 |
| `AERandom *rnd = Globals::rnd` local | 92.277228 | 17172 | 6065 |
| `for (i = 0; i < n && i < size(); i++)` | 91.149056 | 17184 | 6068 |

The faithful re-read is 8 B short because the compiler never keeps the
constant `1` in `fp`; the `volatile` latch is the only shape that restores the
17172 B size, and it scores best.  No latch type, loop form, or `hostile`
spelling moves `1` into `fp` without changing the back-edge.

## 4. Other re-checks

* case-183 `offset` chain: `y,x,z` store order 88.639736 / 17224;
  `switch` 012/210 89.235595 / 17164; `Vector offset` 88.669139 / 17236;
  `float offset[3] = {0,0,0}` 89.191419 / 17176.  Baseline if-chain 2,1,0 with
  `float offset[3]` remains best.
* branchless `&&` (`+0x2e04`/`+0x2f52`): `!((i != bound) || !alienWave)` and
  the `||`/`!` variants canonicalise back to the short-circuit form
  (92.326733); `(i == bound) & alienWave` 75.705562; `(alienWave == 0)`
  92.095710.
* pirate-fortress `ldmia`-vs-`ldrd`: named `Vector a`/`b` 90.807064 / 17164;
  named floats 90.603086 / 17180; `t + 3` first is neutral (92.326733).
* `case4Else`: `Level *self = this` / `Level &self = *this` / struct coords
  neutral; `Vector pos = this->stationPosition` 92.260726.
* nameLeader × turret × tail combinations: best non-baseline is
  `N2+T0+D1` = 91.326236 / 17168; `N2+T0+C0` = 91.237624 / 17172.

## 5. Conclusion

HEAD remains a strict local optimum at **92.326733 %** (17172 B, 6055/6065
insns).  ~110 new variants were measured this session on top of the ~1100 in
the preceding wave-118/119/121/122/123 notes; none beat the baseline.  The
residual is entirely backend:

1. alien-turret `this` r5-vs-r4 / `gun` r4-vs-r6 / `v` r8-vs-r5 / SLP
   `ldrd`-vs-`ldr` (~2.7 pp) — the fresh `(*enemies)[i]` setScaling load keeps
   `this` live in r5; caching `gun` does not move it back to r4;
2. `case4Tail` `fp = 1` vs `fp = &Globals::rnd` (~1 pp) — the faithful re-read
   is 8 B short; the `volatile` latch is the only size-exact shape;
3. `raid`/`__stack_chk_guard` `sp+84`/`sp+88` swap (17 sites, ~0.28 pp) —
   the nameLeader `b.w` return does **not** flip it (N1 still `sp+84`);
4. `nameLeader` extra `b.w` (4 B) — any explicit return moves `case4Else`
   before the name body;
5. case-183 k1/k2 body order (+2 insns) — MBP;
6. branchless `&&` at `+0x2e04`/`+0x2f52` — LICM hoists `!alienWave`;
7. pirate-fortress `ldmia`-vs-`ldrd` SLP pair.

## 6. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=… -DGOF2_FMOD_LIBDIR=…
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7417/score.py            # 6-decimal ratio, size, insns
python3 /dev/shm/w7417/analyze.py          # difflib clusters with addresses
python3 /dev/shm/w7417/regions2.py         # worst regions with orig/ours addrs
python3 /dev/shm/w7417/batch.py v*.py      # apply/build/score/restore variants
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w7417` (`Level.base.cpp` = pristine HEAD copy;
`vn.py`, `vn2.py`, `vt.py`, `vu.py`, `vtail*.py`, `v183*.py`, `vand.py`,
`vpf.py`, `vc4.py`, `vtur*.py`, `vcombo*.py` = the ~110 variant specs).
