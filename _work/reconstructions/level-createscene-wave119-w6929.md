# `Level::createScene` — wave-119 w6929: 92.7573 % is a hard local maximum

**Status:** dead-end sweep, 2026-11-06. Worktree `wt/w6929`, branch
`wave119-w6929`. Target `_ZN5Level11createSceneEv`. No source change landed;
this note records the measurements so the next pass does not repeat them.

Continuation of `level-createscene-wave119-w6820.md` (same ceiling),
`level-createscene-wave119-w6735.md` and `level-createscene-wave119-w6709.md`
(landed the `-g` Vector-address trick, y/z/x store order and the 78 `rnd`
mask, 90.7243 -> 92.7573 %).

## TL;DR

* Baseline and best: **92.7573 % / 2252 B**, 783/791 instructions, first
  diff at instruction 478 (parts-loop store schedule).
* ~90 further source variants were measured this session (a full 7-site
  `rnd` local/global mask sweep, all-global `rnd` with every parts-loop
  store order, plain-`setRotation` basin probes, `volatile` pointer
  qualifiers, retry-loop rewrites, `rnd` declaration-position variants,
  plus a 272-build single-statement permutation sweep of the whole ship
  loop body). **None beat 92.7573 %.** The only variants that tie it are
  code-identical (`BDG`, `BCG`, `L_spots_before_rnd`, `L_const`,
  `retry_while`, `retry_dowhile`, six of the ten parts-loop store orders).
* Gate re-measured green: **byte_exact 4505/4524, linked_exact 4505/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN, drift 0, RATCHET PASS.** 27 sections, file size
  2,187,328 B. No section or `.ARM.extab` change (source untouched;
  `.ARM.extab` is 0x26290 on this branch vs the original's 0x26298, the
  pre-existing `Generator::createMission` LSDA delta).

## 1. Why the ceiling exists: one register too many

The original ship loop is **all-global** `Globals::rnd->` at every one of
its eight sites:

```
b4924: ldr   r0, [pc, #632]        ; GOT
b4926: ldr   r1, [pc, #636]
b4928: add   r0, pc
b492a: add   r1, pc
b492c: ldr.w r8, [r0]              ; r8 = &Globals::rnd
b4930: ldr   r0, [sp, #52]         ; sceneRace
b4932: ldr.w r6, [r1, r0, lsl #2]  ; capacity -> r6
b4936: ldr.w r0, [r8]              ; Globals::rnd (reloaded at every use)
b493a: adds  r1, r6, #1
b493c: blx   nextInt
```

`grep '\[r8\]'` in the original finds exactly the eight `ldr.w r0,[r8]`
sites (b4936, b49ae, b49be, b49ca, b49f6, b4a46, b4a56, b4ad8) plus two
`ldr.w r2,[r8]` `Globals::Canvas` loads in the parts loop. So the original
source has **no local `AERandom *rnd`** — a local would be cached in a
callee-saved register and every site would be `ldr rX,[reg]`, not
`ldr.w r0,[r8]`.

The original's ship loop needs exactly **seven** callee-saved registers
for seven live values, and it fits:

| value | original | landed source (Basin B) |
|---|---|---|
| `spotUsed` | r4 | r4 |
| `race` | r5 | **r6** |
| `capacity` | r6 | **fp** |
| `&Globals::rnd` | **r8** | **spilled `[sp,#44]`** |
| `i` | r9 | r9 |
| `&Globals::status` / `ship` | sl | sl |
| `type` | fp | **r8** |
| cached `Globals::rnd` value | — | **r5** |

The landed source adds an eighth live value (the cached `rnd` value in
r5). LLVM's greedy allocator then spills the *rematerializable* value —
the GOT-derived address `&Globals::rnd` — and the other four values rotate
one register down (race r6, capacity fp, type r8). That is exactly the
42-block diff that remains.

Removing the local `rnd` gives the original's ship loop but **collapses
the whole function to Basin A** (75.4286 %, 2232 B): the stack-canary
address is kept in r5 and spilled at `b432e` instead of at entry, and
`&Globals::status` is materialised in r4 and spilled at `b434c` instead of
living in `sl`. First diff moves from 478 to 6. The two basins are
mutually exclusive for every source shape found.

## 2. Measurements (all 2252 B unless noted)

Scored with the official single-function verifier (`tools/verify/verify.py`,
unrounded difflib ratio). `fd` is the first differing normalised
instruction index.

### 2.1 Full 7-site `rnd` local/global mask sweep

Sites: B = `if (... nextInt(100) <= 29)`, C = `race = ...nextInt(4)`,
D = `if (... nextInt(100) < 30)`, E = `int pick = ...nextInt(3)`,
F = `int spot = ...nextInt(capacity)`, G = retry-loop
`spot = ...nextInt(capacity)`, H = the `nextInt(300)` rotation argument.
All 128 combinations of "site uses the local `rnd`" were built from an
all-global canonical base (the previous note's 5-site sweep B/C/D/E/G did
not cover F or H).

| mask | pct | fd |
|---|---|---|
| **BDG, BCG** | **92.7573** | 478 |
| DHG, CHG | 92.5040 | 478 |
| DFG, CFG | 92.3761 | 478 |
| CDHG, BDHG, BCHG | 92.3761 | 28 |
| DEHG, CEHG | 92.2490 | 28 |
| CDFG, BDFG, BCFG | 92.1220 | 28 |
| DFHG, BFHG, ... | 92.0–92.1 | 28 |
| G only | 66.5404 | 4 |
| no local (all-global) | 66.5 / 75.4 (trick) | 4 / 6 |

`BDG` and `BCG` compile to byte-identical code (site C is dead in the
common path and the allocator coalesces it), so the landed `BCG` mask is
one of two equal maxima. Every mask without G collapses to Basin A.

### 2.2 All-global `rnd` (Basin A) — nothing pins Basin B

| variant | pct | size | fd |
|---|---|---|---|
| all-global + `-g` trick | 75.4000 | 2232 | 6 |
| + `Status *st = Globals::status` local | 75.4000 | 2232 | 6 |
| + `unsigned capacity` | 75.4000 | 2232 | 6 |
| + `shipCount` declared before `capacity` | 75.4000 | 2232 | 6 |
| + fill loop on one line | 75.4000 | 2232 | 6 |
| + `spots` declared after the fill loop | 75.4000 | 2232 | 6 |
| + `spots` declared before `capacity` | 74.3000 | 2232 | 6 |
| + `race` / `type` / `spot` declared then assigned | 75.4000 | 2232 | 6 |
| + `const int *table = nullptr` | 75.4000 | 2232 | 6 |
| + plain `g->setRotation(rot)` | 74.7000 | 2224 | 6 |
| + `-g` trick, store order yxz / xyz / xzy / zxy / zyx | 75.4 / 75.3 / 75.4 / 75.4 / 75.2 | 2232 | 6 |
| + plain, store order yzx / yxz / xyz / xzy / zxy / zyx | 74.7 / 74.7 / 75.0 / 74.7 / 74.7 / 75.0 | 2224–2228 | 6 |
| + `Vector rot(0, pi, 0)` ctor | 75.0000 | 2228 | 6 |
| + ctor with `-g` trick | 75.3000 | 2232 | 6 |

The parts-loop shape, the `spots`/`capacity`/`shipCount` declaration order,
the `Status` local and the `table` initialiser all leave Basin A
untouched. Only the *existence* of the local `rnd` value selects Basin B.

### 2.3 Local-`rnd` variants

| variant | pct | size | fd |
|---|---|---|---|
| **landed (BCG, `-g` trick, y/z/x)** | **92.7573** | 2252 | 478 |
| `spots` declared before `rnd` | 92.7573 | 2252 | 478 |
| `AERandom *const rnd` | 92.7573 | 2252 | 478 |
| retry loop rewritten as `while` | 92.7573 | 2252 | 478 |
| retry loop rewritten as `do { ... } while (true)` | 92.7573 | 2252 | 478 |
| `rnd` declared at the loop top (inside the body) | 91.5000 | 2252 | 133 |
| `rnd` re-assigned at the loop top | 91.5000 | 2252 | 133 |
| site F local instead of global | 92.1220 | 2252 | 28 |
| sites B,C,F,G local | 87.7000 | 2248 | 28 |
| sites B,C,F,G,H local | 86.5000 | 2244 | 9 |
| `rnd` at the loop top, all 8 sites local | 86.6000 | 2244 | 9 |
| `rnd = Globals::rnd;` before the retry loop | 80.1000 | 2240 | 6 |
| `AERandom *volatile rnd` (pointer itself volatile) | 77.5949 | 2240 | 8 |
| `AERandom *volatile const rnd` | 77.5949 | 2240 | 8 |
| `AERandom *&rnd = Globals::rnd` (reference) | 75.4286 | 2232 | 6 |

`AERandom *volatile rnd` is the only variant that moves the spill off
`&Globals::rnd` (the pointer is re-read from the stack at each site), but
it lands in a third basin at 2240 B and scores 77.6.

### 2.4 Plain `g->setRotation(rot)` probes (local `rnd`)

The original's parts loop is the plain form (`add r1, sp, #56`, three
separate stores, zero materialised between the y and x stores). Every
attempt to get the plain form *and* Basin B fails:

| variant | pct | size | fd |
|---|---|---|---|
| `if (g != nullptr) g->setRotation(rot);` | 84.8793 | 2248 | 28 |
| `Vector *rp = &rot; g->setRotation(*rp);` | 84.8793 | 2248 | 28 |
| `Vector &rr = rot; g->setRotation(rr);` | 84.8793 | 2248 | 28 |
| `g->setRotation(rot); if (g == nullptr) return;` | 84.8793 | 2248 | 28 |
| `const Vector &rr = rot; g->setRotation(rr);` | 84.8793 | 2248 | 28 |

The plain form keeps the Basin-B prologue but moves the first diff to the
parts-loop preheader (`kHangarMeshes` base r8 vs the original r9), 78 diff
blocks. The `-g` trick is the only lever that pins the preheader and the
prologue at once, and it necessarily costs the two-instruction address
(`add r0, sp, #56; subs r1, r0, r5`) plus the `strd` store merge.

### 2.5 Parts-loop store-order sweep with the `-g` trick

| variant | pct | fd |
|---|---|---|
| **y, z, x (landed)** | **92.7573** | 478 |
| y, x, z | 92.6302 | 477 |
| `rot.y = pi; rot.z = rot.x = 0;` | 92.6302 | 477 |
| `rot.y = pi; rot.x = 0; rot.z = rot.x;` | 92.6302 | 477 |
| `rot.y = pi; rot.x = rot.z = 0;` | 92.7573 | 478 |
| `rot.y = pi; rot.z = 0; rot.x = rot.z;` | 92.7573 | 478 |
| `rot.y = pi; rot.z = 0; rot.x = rot.y - pi;` | 92.7573 | 478 |
| `rot.y = pi; rot.z = 0; rot.x = 0;` (`0` not `0.0f`) | 92.7573 | 478 |
| `rot.y = pi; rot.z = 0; rot.x = -rot.z;` | 87.7460 | 28 (2256 B) |

### 2.6 Single-statement permutation sweep

All 272 single-statement moves of the 17 statements in the ship-loop body
were built and scored (144 build, 128 fail to compile because a moved
declaration is used before it is defined). The best is the unmodified
order at 92.7573 %; every move is equal or worse. The ties are exactly the
moves that only shuffle independent stores (`spotUsed[spot] = 1` past the
`const int *table;` declaration, `setAlwaysFriend`/`setToSleep`/
`setExhaustVisible` past each other, ...), which reproduce byte-identical
code. No `IMPROVE` line was emitted.

## 3. Conclusion

The landed wave-119 shape is a genuine local maximum for this compiler at
`-Oz`, and the two allocation basins are mutually exclusive:

* Basin B (landed, 92.7573 %) needs the local `rnd` value to keep
  `Globals::status` in `sl`, `this` spilled and the parts-loop preheader
  base in `r9`; the price is that `&Globals::rnd` is spilled and the ship
  loop's four registers rotate.
* Basin A (75.4286 %) is the semantically faithful all-global source and
  reproduces the original's ship loop, but its prologue and parts-loop
  preheader differ.

A future pass would need the source feature that pins the prologue basin
**without** adding a ship-loop live value. Nothing in the parts loop, the
`spots`/`capacity`/`shipCount`/`ownHangar` preheader, the `Status`/
`Globals` locals, the `table` initialiser, the branch order or the ship
loop statement order does it. The remaining candidates are a different
`Globals::status` use count in the mission-23 branch (the original has ten
`ldr.w r0,[sl]` uses) or a different shape of the mission-4 / mission-2
branches, which share the function-wide allocation.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6929
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (not committed): `/dev/shm/w6929/` — `Level.base.cpp`
(pristine copy of the landed source), `score.py` (official verifier import,
unrounded ratio + first-diff index), `batch.py` / `vt.py` (replace + build
+ score), `b1.py`–`b8.py` (this session's variant batches), `moves.py`
(272-build statement-permutation sweep), `orig.asm` / `ours.asm` /
`plain_ag.asm` / `looptop.asm` / `flocal.asm` (resolved disassembly).
