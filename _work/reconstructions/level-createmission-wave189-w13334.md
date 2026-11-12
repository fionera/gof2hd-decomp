# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 189 / w13334

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave189-w13334`, base master `eacfcc9ec`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: **independent per-case body pass** on the mission-type `switch
(mission->getType())` bodies (types 1..15 plus the out-of-table 183/184
bodies). Branch-layout / EH-LSDA and the `isEmpty`-branch semantic cells are
other workers' lanes and were not re-worked.

## 1. Result

**No size-exact raw win was found. `src/game/world/Level.cpp` is
byte-identical to the base** (`git status --porcelain` empty). Final
measurement:

```text
_ZN5Level13createMissionEv   raw=3444  aligned=1021  size=17172/17172  insn=6206/6197
```

`check` green: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
symbol parity **5370/5370**, `RATCHET PASS`. No baseline update.
`.rel.plt` diff 0, `.ARM.extab` diff 639 (size/base exact), `.text` diff
73,999, build-id 20.

## 2. Per-case body deltas (re-measured on this base)

The TBH is at VMA `0xb02a8`; its halfword table starts at `0xb02ac`
(base = `TBH_addr + 4`), 15 entries for mission types 1..15 (the switch is on
`getType() - 1`; types 183/184 are peeled off by the `bhi` default path at
`0xb10c6`).

```text
orig entries:  0903 0a14 0012 0b8a 0012 0baa 0c3a 000f 0cf8 0d86 000f 0f3e 000f 000f 105e
ours entries:  0905 0a15 0012 0b89 0012 0baa 0c3a 000f 0cf8 0d86 000f 0f3e 000f 000f 105e
```

Only entries 0, 1, 3 differ. In **bytes**: case-1 start `+4`, case-2 start
`+2`, case-4 start `-2`, everything after case 4 identical. The function is
size-exact overall, so the per-body size deltas cancel:

```text
case 1 body          -2   (ours 0x220 / orig 0x222)
case 2 body          -4   (ours 0x2e8 / orig 0x2ec)
case 3/5 + 183 region +4  (ours case-1 start 0xb14b6 / orig 0xb14b2)
case 4 body          +2   (ours 0x42 / orig 0x40)
cases 6,7,9,10,12,15,184   0
```

This matches w13311's `+2,-1,0,-3,0,-2,-2,0,-2,-2,0,-4,0,0,-4` **entry**
deltas only for the entries that actually differ here; the note's "ours"
column was measured with a different patch applied and is not the base table.

Consequence: the only case bodies with any code difference at all are
**183, 1, 2, 4**. Cases 6, 7, 9, 10, 12, 15 and 184 are byte-identical to the
original (case 10's tail shares a 6-byte `objectivesA = obj; return;` block
with `case4Tail` in ours, see §5). The large raw clusters outside these
bodies (`0x3240-0x3500` stationPirates/extraAliens, `0x3700-0x3e00`
extraAliens/convoy, `0x40c0-0x4300` case4Else/Tail/landing pads) belong to the
`isEmpty` branch and are not mission-type case bodies.

## 3. `case 2` — the `-4` body (biggest remaining case residual)

Original VMA `0xb16d4`-`0xb19c0`, ours `0xb16d6`-`0xb19be`.

The `-4` is exactly one `mov r4, fp` / `mov fp, r4` save/restore pair around
the first convoy loop. The original keeps `convoyEnd` in `fp`:

```text
b177a add.w fp, r0, #2        ; fp = convoyEnd = escorts + 2
b177e add.w r0, fp, r4        ; total = convoyEnd + goods
b17dc mov  r4, fp             ; save convoyEnd
b17de ldr.w fp, [r0,#292]     ; fp = waypoint->x
...
b1810 mov  fp, r4             ; restore convoyEnd
b1886 cmp  r8, fp             ; i < convoyEnd
```

Ours spills `convoyEnd` to `sp#120` and uses `fp` for `this` instead
(`b1886 mov fp, r5`, `b197a ldr r5,[sp,#120]`, `b197e add r1,r5,r8`). The
whole case-2 residual (~569 raw bytes) is downstream of this one register
choice plus the store/order differences below; the code itself is semantically
identical.

Other measured case-2 differences (all size-exact, none a win on its own):

* `coords[6]` values/store order. Original is `{-20000-rnd,0,-20000-rnd,0,0,0}`
  with store order `c1,c0,c3,c4,c5,c2`; base is `{0,0,-20000-rnd,0,-20000-rnd,0}`
  with `c0,c1,c3,c2,c5,c4`. Restoring the original values is raw **3,485**
  (handoff); explicit `int coords[6];` assignment orders are 3,485/3,491;
  `{0,0,0,0,0,0}` + two assignments is size-fatal (17,180).
* `Vector asteroidPos = {0,0,0}` is initialised at the very top of the body in
  the original (interleaved with the first `nextInt`) but after `new Array` in
  ours. Moving the declaration earlier: 3,460 (before `goods`), 3,507 (before
  `coords`), 3,456 (before `new Route`). `Vector asteroidPos;` (no init) and
  the explicit zero-assign form are exactly neutral (3,444).
* First convoy loop call order: original is `setAlwaysEnemy(true)` →
  `setRoute(clone())` → `getRoute()->reachWaypoint(0)`; base is
  `setRoute` → `reachWaypoint` → `setAlwaysEnemy`. Reordering is raw 3,452
  (w13311) / 3,448–3,450 measured here depending on form.
* Asteroid loop tail: original order is `cargo=nullptr`, `crateCaptured()=0`,
  `setSpeed(0)`, `setRotationSpeed(0)`, `setHitpoints(getMaxHitpoints()*3)`.
  Full 6-permutation × `Player *p` position sweep (12 variants): base
  `H,R,S` = 3,444; `H,S,R` = 3,448; `S,H,R` = 3,448; `R,H,S` = 3,450;
  `S,R,H` and `R,S,H` are size-fatal (17,164). The original order is
  size-fatal.
* Objectives: original emits `objectivesA` then `objectivesB`; base emits
  `B` then `A`. Swapping is raw 3,445.
* Removing the `Array<KIPlayer *> *enemies` local, or hoisting it out of the
  loop, is size-fatal (17,180 / 17,164); keeping it is load-bearing for the
  17,172-byte shape.

## 4. `case 1` — the `-2` body

Original `0xb14b2`-`0xb16d4`, ours `0xb14b6`-`0xb16d6`.

* `coords[9]`: base is `{rnd-50000, 0, 75000, rnd-50000, 0, 50000, ...}`;
  original is `{..., 50000, ..., 75000, ...}` (the 75000/50000 entries are
  swapped). Swapping is size-exact but raw **3,445** (aligned 1021 → 1015):
  the case-183 `+4` body shift moves the literal pool by 4, so the `ldr`
  immediates for the pool-resident 75000 no longer line up. This is the single
  closest near-win in the whole pass.
* `attackerCount` schedule: original hoists `vmov.f32 s0,#-0.5` +
  `vadd.f32 s0,s2,s0` (`opt44 - 0.5f`) **before** the `new Array` call; ours
  schedules them after. Computing `attackerCount` before `new Array`, using a
  `factor`/`mult` local, or algebraically simplifying to
  `attackers * (opt44 + 0.5f)` does not move the schedule (factor-local is
  neutral 3,444; the others are 5,259+ or 3,462).
* `opt44` declared before `attackers` is neutral; `friends` after `opt44` is
  3,453; `(float)(int)difficulty + 3.0f` is 3,446.

## 5. `case 183` — `+4` body (convoy switch + tail)

Original `0xb10d2`-`0xb14b2`, ours `0xb10d2`-`0xb14b6`. The body code is
identical up to `0xb1440` except for seven `ldr [pc,#imm]` literal-pool
immediates (`0x1208, 0x1250, 0x12a4, 0x12a8, 0x1322, 0x135c, 0x136c`); the
pool itself is 4 B later in ours. The `+4` is the `switch (k)` convoy-offset
arm: original is 48 B with all four arms `offset[1], offset[0], offset[2]`
and one shared `str [sp,#380]`; ours is 52 B because case 1 (`o1,o2,o0`) and
default (`o0,o2,o1`) cannot share the tail. Restoring the 48 B form makes
clang LICM the `offset[2]` constants into `vldr s16/s18/s20` in the preheader
and the function becomes 17,164 B (w13311 §3a; re-confirmed here: explicit
`o1,o0,o2` in all four arms with the base values → size 17,164, raw 11,283).

The case-183 tail (`setPosition` → `stationPosition=getPosition()` →
`timeLimit` → `objectivesA`) was re-swept with local forms:

```text
Vector pos = getPosition(); ... stationPosition = pos   raw 3,446
PlayerEgo *peg = this->player; ...                      raw 3,445
int tl = 181000; timeLimit = tl; ...                    raw 3,444 (neutral)
BA-D-C (timeLimit before objectivesA)                   raw 3,455
AB-D-C with tl local                                    raw 3,457
Objective *obj local, store split                       raw 5,273
```

Base `B,A,C,D` remains the raw optimum.

## 6. `case 4` — `+2` body

Original `0xb19c0`-`0xb1a00`, ours `0xb19be`-`0xb1a00`. Ours loads `this`
into `sl` *before* the `cmp r0,#0` and uses `sl` for the `playerRoute` store;
the original loads `this` from `sp#124` into `r8` *after* the `createRoute`
call. Source variants (`Route *r` local, `Level *self` local, early-goto
`==0` form, `else {}` form) are all exactly neutral (3,444). The 6-byte case
10 tail (`str r5,[r4,#40]; b.w b02ca` vs ours `b.w b41e8; nop`) is the same
`this`-register split (case 10 uses `r4` in the original, `r8` in ours, so the
`objectivesA = obj; return;` tail is shared with `case4Tail` only in ours);
`Objective *obj` locals and `break` are neutral.

## 7. Sweeps that did not land (real `link_so` + `tools/fn_diff.py`)

All are size-exact 17,172 unless marked fatal.

```text
case-1 coords swap (A)                             3445
case-2 coords original (B)                         3485
case-2 objectives A-then-B (G)                     3445
case-183 tail PlayerEgo local (P)                  3445
case-183 tail Vector local (V)                     3446
case-2 asteroidPos before goods (X)                3460
case-2 H,S,R (Y)                                   3448
A+B 3486  A+G 3446  A+P 3446  A+V 3447  A+X 3461  A+Y 3449
B+G 3486  B+P 3486  B+V 3487  B+X 3501  B+Y 3489
G+P 3446  G+V 3447  G+X 3461  G+Y 3449
P+X 3461  P+Y 3449  V+X 3462  V+Y 3450  X+Y 3464
all triples of {A,B,G,P,V,X,Y}: >= 3447
raid: threshold local / nested-if / inverted / expect  3444 / 3444 / 3444 / 17204
case-4 forms (route-local, self-local, early-goto)      3444
case-10 Objective local / break                         3444
case-2 enemies local removed / hoisted                 17180 / 17164 (fatal)
case-2 no-ship / ship-consistent                        3444 / 17140 (fatal)
case-183 setPosition(Vector{...}) / 3-float overload     17164 / 17160 (fatal)
```

No pair or triple of the non-neutral patches composes below 3,444: the base
is a local optimum in every neighbourhood probed.

Additional probes made while re-verifying this note (same recipe, all
size-exact 17,172 unless marked):

```text
case-183 o1,o0,o2 in all four arms (re-measured)   11283  17164 (fatal)
case-183 same + case-1 coords swap A               11284  17164 (fatal)
case-183 if/else chain instead of switch           11834  17224 (fatal)
case-183 two back-to-back switches (o1,o0 / o2)    11283  17164 (fatal)
case-2 setAlwaysFriend before asteroidPos          3449
case-2 setAlwaysFriend between getPosition/setPos  3446
case-2 drop the `enemies` local entirely           10728  17180 (fatal)
```

The case-183 `o1,o0,o2` reorder is *too regular*: clang merges the four arms
into a 4x3 constant table (`str r0,[sp,#120]` preheader address, then
`vldr s4/s2/s0` + `vstr` per iteration) and the body shrinks by 8 B instead
of the 4 B the original has. The original keeps per-arm `ldr` literal loads
with only the final `str [sp,#380]` sunk to the join. Two separate switches
are re-merged by clang into the same table form; an if/else chain is 60 B
worse. So the 48 B shape is not reachable from a plain per-arm assignment
order, which is why the recommended case-183+case-2 co-land in §8 also
fails to be size-neutral in practice (17,164 even with A folded in).

## 8. Why a win needs a co-designed pass

The four remaining case-body size deltas are coupled through the literal pool
and the callee-saved register rotation:

* case-183 `+4` shifts the pool by 4 and breaks the case-1 `75000` literal
  load (the `A` near-win at 3,445);
* case-2 `-4` is one `fp` save/restore pair, i.e. the `convoyEnd`-in-`fp`
  allocation, which in turn is what the `this`-in-`r5` allocation of the
  second loop depends on;
* case-4 `+2` is the `this`-in-`r8` vs `sl` split that also controls the
  case-10 tail merge and the `case4Tail`/`case4Else` block layout.

A pass that lands case-183 at 48 B **and** case-2 at `convoyEnd`-in-`fp`
would be size-neutral and would remove both the switch bytes and the pool
shift, but each half is individually size-fatal (17,164 / 17,164), so the two
must be landed together under one measurement. That is the recommended next
attempt.

## 9. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13334
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3444 aligned=1021 size=17172/17172`, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13334/`
(`Level.base.cpp`, `align.py`, `rawmap.py`, `mtry.py`, `exp1.py`..`exp19.py`,
`c1.orig`, `c1.ours`, `c2.orig`, `c2.ours`).
