# w7609: `Level::createMission` (92.326733 -> 92.326733 %, 17172 B) — wave-126
# call-site-argument / local-reuse / branch-layout sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7609`, branch `wave126-w7609`,
based on current master (`bf365a964`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).
Continues `level-createmission-wave121-w7112.md` … `…-wave125-w7516.md`.
`src/game/world/Level.cpp` is unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The fuzzy ratio is `2*M/(6055+6065)` with `M = 5595`.  One matching
instruction is `+0.0165 pp`.  The residual is the same backend list as
wave-125: alien-turret register allocation (~2.7 pp), `case4Tail`
`fp = 1` vs `fp = &Globals::rnd` (~1 pp), `raid`/`__stack_chk_guard`
`sp+84`/`sp+88` swap (17 sites), `case4Else`/`case4Tail` `this` register,
case-183 test/body order, branchless `&&` at `+0x2e04`/`+0x2f52`, and the
pirate-fortress `ldmia`-vs-`ldrd` SLP pair.

## 1. Method

Per-variant harness (`/dev/shm/w7609/batch.py`): apply a literal substring
replacement to a pristine copy of `Level.cpp`, `link_so` (2.3 s full
rebuild), score with the official verifier's normalized stream
(`verify.disassemble` + `reconcile` + `difflib`), restore.  ~75 variants
measured this session.  The exact score and the per-region diffs are from
`/dev/shm/w7609/score.py` and `/dev/shm/w7609/regions.py`.

## 2. `raid` loop call-site arguments / constant hoisting

Original (`+0x380a`): `str r0,[sp,#0]; movs r0,#1; strd r0,r6,[sp,#4]`
with `r6 = 0`, and `movw r1,#20000` materialised inside the loop.  Ours
hoists `r6 = 1`, `r9 = 0`, `sl = 20000` and emits `stmia.w sp,{r0,r6,r9}`.
All source spellings that could change this were neutral:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `true, false`** | **92.326733** | 17172 | 6065 |
| `1, 0` / `true, 0` / `1, false` | 92.326733 | 17172 | 6065 |
| `bool hostile = true; bool group = false;` before the loop | 92.326733 | 17172 | 6065 |
| same locals declared **inside** the loop | 92.326733 | 17172 | 6065 |
| `int hostile = 1; int group = 0;` inside the loop | 92.326733 | 17172 | 6065 |
| `volatile bool hostile = true;` inside the loop | 90.021442 | 17172 | 6071 |
| `Waypoint *wp = field_180->getWaypoint();` local | 92.326733 | 17172 | 6065 |
| `int type = raidType;` local | 92.326733 | 17172 | 6065 |
| `Route *route = this->field_180;` local | 91.372484 | 17184 | 6069 |
| `bool hard` -> `const bool` / `int` / `unsigned` / `const int` | 92.326733 | 17172 | 6065 |
| `volatile bool hard` | 89.845748 | 17196 | 6068 |
| `if (raid != 0)` | 92.326733 | 17172 | 6065 |
| `unsigned i` loop variable | 92.326733 | 17172 | 6065 |
| `++i` back-edge | 92.326733 | 17172 | 6065 |

## 3. `case4Tail` `fp = 1` vs `fp = &Globals::rnd`

Original (`+0x415c`): `ldr r0,[pc,#392]; mov.w fp,#1; mov.w r9,#0;
add r0,pc; ldr.w sl,[r0]` (sl = `&Globals::globals`), `&Globals::rnd`
spilled at `sp+120` and reloaded each iteration (`ldr r0,[sp,#120];
ldr r6,[r0]`).  Ours keeps `&Globals::rnd` in `fp` and materialises `1`
per iteration.  The `volatile unsigned n` latch is still the only
size-exact shape; the faithful `size()` re-read is 8 B short (17164).
New spellings measured this wave (all <= baseline):

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile unsigned n` latch** | **92.326733** | 17172 | 6065 |
| `bool hostile = true;` / `int hostile = 1;` before the loop | 92.326733 | 17172 | 6065 |
| `AERandom **rndp = &Globals::rnd;` + `(*rndp)->nextInt` | 92.326733 | 17172 | 6065 |
| `AERandom *rnd = Globals::rnd;` **inside** the loop | 92.277228 | 17172 | 6065 |
| `AERandom &rnd = *Globals::rnd;` inside the loop | 92.277228 | 17172 | 6065 |
| `AERandom *volatile rnd` before the loop | 92.161716 | 17172 | 6065 |
| `Globals *g = Globals::globals;` local | 92.187113 | 17172 | 6066 |
| `Route *route = this->playerRoute;` local | 92.136315 | 17172 | 6064 |
| `AERandom *rnd` before the loop | 91.113128 | 17168 | 6064 |
| `volatile AERandom *rnd` | 91.113128 | 17168 | 6064 |
| `Waypoint *wp = ...` local | 90.973597 | 17172 | 6065 |
| `volatile bool hostile = true;` + latch | 91.041082 | 17184 | 6067 |
| faithful `size()` re-read (no latch) | 91.341312 | 17164 | 6060 |
| faithful + `volatile AERandom *` / `**` | 91.194190 | 17168 | 6062 |
| faithful + `Level *volatile self` | 91.162637 | 17176 | 6064 |
| faithful + `i < size() && i < n` | 90.868597 | 17180 | 6068 |
| `volatile int` / `volatile size_t` / `volatile unsigned long n` | 92.326733 | 17172 | 6065 |
| `volatile unsigned char n` | 91.221122 | 17176 | 6065 |
| plain `unsigned n` / `const unsigned n` | 92.178218 | 17172 | 6065 |

The faithful re-read is 8 B short because the compiler never spills
`&Globals::rnd`; the `volatile` latch is what frees the register.  No
spelling moves `1` into `fp` while keeping `this` in `r8`.

## 4. Alien-turret table select (`+0x32c6`)

Original: in-loop `cbz bigAlienWave` diamond (`ldr/add` on each arm).
Ours: in-loop branchless `cmp.w r1, r9, asr #31; it ne; movne` from the
`(bigAlienWave + (i < 0))` anchor.  The `i < 0` anchor is still required
and still unique; every new condition spelling is neutral or worse:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `(bigAlienWave + (i < 0))` ternary** | **92.326733** | 17172 | 6065 |
| `(bigAlienWave + (i < 0) != 0)` / `> 0` / `^ (i < 0)` | 92.326733 | 17172 | 6065 |
| statement-`if` / `switch` / `goto` with the same anchor | 92.326733 | 17172 | 6065 |
| `(bigAlienWave == 1)` with anchor | 92.277228 | 17172 | 6065 |
| `(bigAlienWave \| (i < 0))` | 92.277228 | 17172 | 6065 |
| `(bigAlienWave + (i <= 0))` / `(i > 0)` | 90.934587 | 17180 | 6068 |
| plain `bigAlienWave` / `== 1` / `!= 0` / `> 0` (LICM hoists) | 88.413577 | 17140 | 6054 |
| table-of-tables `tables[(anchor) != 0]` | 92.060086 | 17172 | 6061 |

The 1-D `const float *t = (...)[k]` shape (wave-98) is worse than the
current 2-D `const float (*t)[6]` + `t[k][...]`:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline 2-D `t[k][...]`** | **92.326733** | 17172 | 6065 |
| 1-D `t[...]` | 92.192143 | 17172 | 6061 |

## 5. Alien/pirate-fortress loop body / declarations

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline (`setScaling` before rotate, shipGroup then cargo)** | **92.326733** | 17172 | 6065 |
| `setScaling` after `setPosition` (faithful) | 92.277228 | 17172 | 6065 |
| `cargo = nullptr` before `shipGroup` (faithful) | 92.310231 | 17172 | 6065 |
| cached `gun` for `setScaling` | 90.955603 | 17164 | 6063 |
| `Vector base` declared before `Vector v` (alien) | 92.161716 | 17172 | 6065 |
| `Vector base` declared before `Vector v` (fortress) | 92.145215 | 17172 | 6065 |
| `int k` in the `for`-init | 92.326733 | 17172 | 6065 |
| alien bound `... + field_168 + alienGuns` | 92.326733 | 17172 | 6065 |
| fortress bound `... + field_168 + fortressGuns` | 92.326733 | 17172 | 6065 |
| `unsigned i` alien loop | 88.413577 | 17140 | 6054 |
| `if (alienGuns)` instead of `!= 0` | 92.326733 | 17172 | 6065 |
| `guns = fortressGuns + alienGuns` | 92.326733 | 17172 | 6065 |
| `const int alienGuns` / `const int fortressGuns` | 92.326733 | 17172 | 6065 |

## 6. Conclusion

HEAD remains a strict local optimum at **92.326733 %** (17172 B, 6055/6065
insns).  ~75 new variants were measured on top of the ~1300 in the
preceding wave-118/119/121/122/123/124/125 notes; none beats the baseline.
The residual is entirely backend:

1. alien-turret `this` r5-vs-r4 / `gun` r4-vs-r6 / `v` r8-vs-r5 / table
   r6-vs-sl (~2.7 pp) — caused by `alienGuns` staying in r6 across the
   loop in ours; the original spills it to `sp+52` and reloads it.
2. `case4Tail` `fp = 1` vs `fp = &Globals::rnd` (~1 pp) — the faithful
   `size()` re-read is 8 B short; the `volatile` latch is the only
   size-exact shape and it still keeps `&rnd` in `fp`.
3. `raid`/`__stack_chk_guard` `sp+84`/`sp+88` swap (17 sites).
4. `raid` loop constant hoisting (`r6=1`, `r9=0`, `sl=20000`).
5. `case4Else`/`case4Tail` `this` — original r8 throughout, ours `sl`
   then r8.
6. case-183 test order 2,1,0 + body order 0,1,2,default; MBP reorders
   k1/k2 in MIR before layout.
7. branchless `&&` at `+0x2e04`/`+0x2f52` — LICM hoists `!alienWave`.
8. pirate-fortress `ldmia`-vs-`ldrd` SLP pair.

## 7. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/w7609/score.py            # exact ratio, size, insns
python3 /dev/shm/w7609/regions.py          # contiguous mismatch blocks
python3 /dev/shm/w7609/batch.py vN.py      # apply/build/score/restore variants
cmake --build cmake-build-match --target check -j8
```

Scratch under `/dev/shm/w7609` (`Level.base.cpp` = pristine HEAD copy;
`score.py`, `regions.py`, `addrblocks2.py`, `dump.py`, `batch.py`;
`v1.py` … `v9.py` = the ~75 variant specs).

## 8. Gate counts (before -> after)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
stub_zero_size     0 -> 0
missing            0 -> 0
wrong_type         0 -> 0
extra              0 -> 0
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
```
