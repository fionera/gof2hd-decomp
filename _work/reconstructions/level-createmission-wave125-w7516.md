# w7516: `Level::createMission` (92.326733 -> 92.326733 %, 17172 B) — wave-125
# case-183 permutation grid, alien-turret `alienGuns` liveness, case4Tail `fp=1`,
# pirate-fortress `ldmia` form, raid constant hoisting; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7516`, based on current master
(`02513de33`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6…bed9483`, image base 0x10000; function `0xaffd8`-`0xb42ec`,
17172 B).  Continues `level-createmission-wave121-w7112.md`,
`…-wave122-w7214.md`, `…-wave123-w7310.md`, `…-wave124-w7417.md`.
`src/game/world/Level.cpp` is unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The fuzzy ratio is `2*M/(6055+6065)` with `M = 5595`.  One extra matching
instruction is `+0.0165 pp`.  The mnemonic-only alignment (difflib over the
opcode field) is **98.027 %**; the ~120 opcode mismatches are the case-183
`cbnz`, the two branchless-`&&` sites, the alien-turret/pirate-fortress/raid
loop bodies, the case4Tail `strd`, and the EH landing pads.  Everything else is
operand-only (register / stack-slot colouring).

## 1. New evidence: alien-turret `this` reload is caused by `alienGuns` liveness

Original alien-turret loop entry (`+0x329a`):

```text
+0x3286  ldr.w  sl, [sp, #52]      ; alienGuns -> sl
+0x3288  cmp.w  sl, #0
+0x328a  beq.w  <skip>
+0x328c  ldr    r4, [sp, #124]     ; this -> r4   (ONE load)
...
+0x32c2  mov    r0, r4             ; this
+0x32cf  ldr.w  r6, [r0, fp, lsl #2]   ; gun -> r6
...
+0x32e8  ldmia  r2, {r0, r1, r2}   ; friendCount, field_164, field_168
+0x32ea  ldr.w  sl, [sp, #52]      ; alienGuns RELOADED from stack
+0x32ec  add.w  r3, r0, sl
+0x32ee  add    r3, r1
+0x32f0  add    r3, r2
+0x32f2  cmp    fp, r3
```

Ours:

```text
+0x3280  ldr    r6, [sp, #52]      ; alienGuns -> r6 (kept in a register)
+0x3282  cmp    r6, #0
...
+0x3298  ldr    r4, [sp, #124]     ; this -> r4
...
+0x32a4  ldr    r5, [sp, #124]     ; this -> r5  (SECOND load)
...
+0x32c1  ldr.w  r4, [r0, r9, lsl #2]   ; gun -> r4
...
+0x32e6  ldmia  r2, {r0, r1, r2}
+0x32e8  adds   r3, r0, r6         ; alienGuns still in r6
```

So the whole permutation (`this` r5-vs-r4, `gun` r4-vs-r6, `v` r8-vs-r5,
`k*24` fp-vs-r9, `i` r9-vs-fp, table r6-vs-sl) follows from one fact: **ours
keeps `alienGuns` in r6 across the loop** (it is re-used by the loop-bound
recomputation at the bottom), whereas the original spills it to `sp+52` and
reloads it there, leaving r6 free for `gun` and r4 free for `this`.  Forcing
the spill with `volatile int alienGuns` does reproduce the reload but also
changes the initialisation and `guns = alienGuns + fortressGuns`, costing 7 pp:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.326733** | 17172 | 6065 |
| `volatile int alienGuns` | 85.266199 | 17168 | 6060 |
| `volatile int &ag = alienGuns` (unused) | 92.326733 | 17172 | 6065 |
| `const int alienGuns` / `const int fortressGuns` / `const int guns` | 92.326733 | 17172 | 6065 |
| bound reorder `alienGuns + friendCount + …` | 92.326733 | 17172 | 6065 |
| bound reorder `field_164 + friendCount + …` | 92.310231 | 17172 | 6065 |
| bound reorder `field_168 + friendCount + …` | 92.310231 | 17172 | 6065 |
| `int start`/`int end` cache | 87.022523 | 17180 | 6066 |
| `PlayerTurret *turret = (PlayerTurret *) gun` local | 90.955603 | 17164 | 6063 |
| `KIPlayer *gun` hoisted out of the loop | 92.326733 | 17172 | 6065 |
| `if (alienGuns)` instead of `!= 0` | 92.326733 | 17172 | 6065 |
| `Vector v;` (default ctor) | 92.326733 | 17172 | 6065 |
| `for (int i = …, k = 0; …)` | 92.326733 | 17172 | 6065 |
| `++i, ++k` / `i += 1, k += 1` | 92.326733 | 17172 | 6065 |
| `int k` declared before `Matrix`/`Vector base` | 92.326733 | 17172 | 6065 |

The `i < 0` anchor is still required and still unique; every alternative anchor
either lets LICM hoist the select or costs instructions:

| anchor | % | size | insns |
|---|---:|---:|---:|
| **`(bigAlienWave + (i < 0))` (baseline)** | **92.326733** | 17172 | 6065 |
| `(bigAlienWave + (k < 0))` | 88.413577 | 17140 | 6054 |
| `(bigAlienWave + (k >> 31))` | 88.413577 | 17140 | 6054 |
| `(i < 0) ? outpost : (bigAlienWave ? station : outpost)` | 92.161716 | 17172 | 6065 |
| `(bigAlienWave && i >= 0) ? …` | 92.161716 | 17172 | 6065 |
| `(i >= 0 && bigAlienWave) ? …` | 92.161716 | 17172 | 6065 |

## 2. case-183 `offset` chain: all 12 case orders measured

The original tests `2, 1, 0` and lays the bodies out `0, 1, 2, default`.
All six if-chain permutations and the `switch` forms were re-measured:

| source order | % | body order emitted |
|---|---:|---|
| **if `2,1,0` (baseline) / switch `2,1,0`** | **92.326733** | `2,0,1,default` |
| if `2,0,1` | 92.326733 | `2,0,1,default` |
| switch `0,1,2` / if `0,1,2` / if `0,2,1` | 92.178218 | `0,2,1,default` |
| if `1,0,2` / if `1,2,0` | 92.194719 | `1,0,2,default` |
| store order `y,x,z` inside each body | 88.639736 | 17224 B |
| reverse chain + `y,x,z` stores | 89.554455 | 17172 B |

`if 0,1,2` and `switch 0,1,2` are the only forms that reproduce the original's
**test order 2,1,0**, but MBP then lays k2 before k1 (`0,2,1`), never `0,1,2`.
No source order yields both.  The original's k1-before-k2 body order is a
MachineBlockPlacement decision made before layout.

## 3. `case4Tail`: `fp = 1` vs `fp = &Globals::rnd`

Original (`+0x415c`):

```text
+0x415c  ldr    r0, [pc, #392]      ; &Globals::rnd
+0x415e  mov.w  fp, #1              ; hostile = 1
+0x4162  mov.w  r9, #0              ; i = 0
+0x4166  add    r0, pc
+0x4168  ldr.w  sl, [r0]            ; Globals::globals
...
+0x417a  ldr    r0, [sp, #120]      ; &Globals::rnd (spilled at +0x26)
+0x4180  ldr    r6, [r0]            ; Globals::rnd
...
+0x4198  strd   r0, fp, [sp]        ; (waypoint, 1)
+0x419c  movs   r0, #0
+0x419e  str    r0, [sp, #8]        ; (0)
```

Ours keeps `&Globals::rnd` in `fp` (rematerialised from the GOT, not from
`sp+120`) and materialises `1`/`0` per iteration:

```text
+0x41a4  ldr    r0, [pc, #392]
+0x41a6  ldr.w  r8, [sp, #124]      ; this
+0x41a8  add    r0, pc
+0x41aa  ldr.w  fp, [r0]            ; fp = &Globals::rnd
...
+0x41bd  str    r0, [sp, #0]
+0x41bf  movs   r0, #1
+0x41c1  str    r0, [sp, #4]
+0x41c3  movs   r0, #0
+0x41c5  str    r0, [sp, #8]
```

The `volatile unsigned n` latch is still the only size-exact (17172 B) shape.
New this wave (all <= baseline):

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile unsigned n`** | **92.326733** | 17172 | 6065 |
| faithful `i < this->enemies->size()` | 91.341312 | 17164 | 6060 |
| faithful + `bool hostile` / `int hostile = 1` / two bools | 91.341312 | 17164 | 6060 |
| faithful + `++i` / `while` | 91.341312 | 17164 | 6060 |
| faithful + `KIPlayer *ship` local | 91.290349 | 17156 | 6058 |
| faithful + `AERandom *const rnd` local | 91.234731 | 17164 | 6061 |
| `bool hostile` / `const bool hostile` at function top + faithful | 91.341312 | 17164 | 6060 |
| `for (unsigned i = 0, n = size(); …)` comma-init | 92.178218 | 17172 | 6065 |
| `i != n` | 92.326733 | 17172 | 6065 |
| `size_t` / `const volatile unsigned` | 92.326733 | 17172 | 6065 |
| `for (volatile unsigned i = …)` | 91.098094 | 17176 | 6066 |
| `KIPlayer *ship = createShip(...); slot = ship;` | 91.269186 | 17164 | 6063 |
| `this->enemies->data_[i]` instead of `(*enemies)[i]` | 92.326733 | 17172 | 6065 |
| `AERandom *const rnd` local before the loop | 91.032093 | 17172 | 6066 |
| `AERandom *const rnd` at function top | 57.848789 | 17352 | 6087 |

## 4. `case4Else` / `case4Tail`: `this` register

Original keeps `this` in `r8` from the `case4Else` entry (`+0x40de
ldr.w r8,[sp,#124]`) through the whole tail.  Ours uses `sl` for `this` in
`case4Else` and only reloads `r8` at `+0x41a6`.  This is the same
`sl`/`r8`/`fp` pressure that drives the `fp=1` residual; no source spelling of
the `case4Else` `coords[3]` / `Route` block changes it.

## 5. `raid` loop: constant hoisting

Original (`+0x382a`): `this`=r4, `raidType`=r9, `i`=sl, `hard`=r8, and the
`createShip` bools are materialised (`movs r0,#1; strd r0,r6,[sp,#4]` with
r6=0; `movw r1,#20000` inside the loop).  Ours: `this`=r5, `raidType` spilled
to `sp+104`, `i`=fp, `hard`=r9, and three constants are hoisted out of the loop
(`r6=1`, `r9=0`, `sl=20000`), so the call is `stmia.w sp,{r0,r6,r9}`.  The
`raid`/`__stack_chk_guard` `sp+84`/`sp+88` swap is unchanged by every source
form tried (see wave-121 §4).  `bool raid = false;` is neutral; `int raid`
costs 0.16 pp (92.161716).  Reversing the `nextInt(2) == 0 ? 1 : -1` ternary
is neutral.

## 6. Pirate fortress: `ldmia` form is close but loses the register draw

`const float (*t)[6] = kBaseTurrets; t[k][…]` reproduces the original's two
`ldmia` copies (baseline emits `ldrd`+`ldr`), and is only one instruction off
(6064 vs 6065), but it flips `this` to r6 / table to r5 (original: this r5 /
table r6), so it scores slightly lower:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `const float *t = kBaseTurrets[k]`** | **92.326733** | 17172 | 6065 |
| `const float (*t)[6] = kBaseTurrets` | 92.284842 | 17172 | 6064 |
| `const float (*const t)[6]` | 92.284842 | 17172 | 6064 |
| `t` hoisted before the loop | 92.326733 | 17172 | 6065 |
| `Vector v` first / `Vector base` first decl | 92.161716 / 92.095710 | 17172 | 6065 |
| `const Vector base` / `const Matrix rotation` | 92.326733 | 17172 | 6065 |
| `int k` before the vectors / `++i,++k` | 92.326733 | 17172 | 6065 |
| `pf_t6` + alien `gun` outer + tail `i != n` | 92.284842 | 17172 | 6064 |

## 7. Branchless `&&` (`+0x2e04`, `+0x2f52`)

Adding a loop-variant `i >= 0` conjunct to defeat LICM leaks the anchor and
adds instructions:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline short-circuit** | **92.326733** | 17172 | 6065 |
| `i == X && alienWave && i >= 0` | 90.958588 | 17180 | 6067 |
| `i == X && (i >= 0) && alienWave` | 90.958588 | 17180 | 6067 |

## 8. Conclusion

HEAD remains a strict local optimum at **92.326733 %** (17172 B, 6055/6065
insns).  ~90 new variants were measured this session on top of the ~1200 in the
preceding wave-118/119/121/122/123/124 notes; none beat the baseline.  The
residual is entirely backend, and the new causal chain found this wave is:

1. **alien-turret** (~2.7 pp): ours keeps `alienGuns` in r6 across the loop
   because the bound is recomputed at the loop bottom; the original spills it
   to `sp+52` and reloads.  That single difference produces the whole
   `this` r5-vs-r4 / `gun` r4-vs-r6 / `v` r8-vs-r5 / `k*24` fp-vs-r9 / `i`
   r9-vs-fp / table r6-vs-sl permutation and the extra `ldr r5,[sp,#124]`.
   `volatile int alienGuns` forces the spill but over-perturbs the
   initialisation (85.27 %).
2. **`case4Tail`** (~1 pp): `fp = 1` vs `fp = &Globals::rnd`; the original
   reloads `&rnd` from `sp+120` and emits `strd r0,fp,[sp]`; ours
   rematerialises `&rnd` from the GOT.  The faithful re-read is 8 B short; the
   `volatile` latch is the only size-exact shape.
3. **`raid`/`__stack_chk_guard`** `sp+84`/`sp+88` swap (17 sites, ~0.28 pp) —
   not movable from source.
4. **`case4Else`/`case4Tail` `this`** — original r8 throughout, ours `sl` then
   r8.
5. **case-183** — test order 2,1,0 + body order 0,1,2,default; MBP reorders
   k1/k2 in MIR before layout.
6. **branchless `&&`** at `+0x2e04`/`+0x2f52` — LICM hoists `!alienWave`.
7. **pirate-fortress** `ldmia`-vs-`ldrd` SLP pair — reachable only with
   `const float (*t)[6]`, which loses the register draw.

## 9. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=… -DGOF2_FMOD_LIBDIR=…
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7516/score.py            # 6-decimal ratio, size, insns
python3 /dev/shm/w7516/analyze.py          # difflib clusters with addresses
python3 /dev/shm/w7516/regionmatch2.py     # per-100-instruction matched census
python3 /dev/shm/w7516/opdiff.py           # mnemonic-only alignment
python3 /dev/shm/w7516/multi.py v*.py      # apply/build/score/restore variants
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w7516` (`Level.base.cpp` = pristine HEAD copy;
`v183*.py`, `vfaith.py`, `vpf*.py`, `vmix.py`, `vbroad.py`, `vmicro.py`,
`vfinal*.py`, `vscope2.py`, `vconst.py`, `vvol.py`, `vbound.py`, `vinc.py`,
`vrnd.py`, `vcomb3.py` = the ~90 variant specs).
