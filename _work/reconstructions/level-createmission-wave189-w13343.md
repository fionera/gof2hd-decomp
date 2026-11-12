# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 189 / w13343

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave189-w13343`, base master `cd3f94ba2` (wave-189 base,
raw **3,444**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: **full-body register-allocation / statement-order** pass. Branch layout
and EH-LSDA are w13322's lane; per-case bodies are w13334's lane. This note
records the sweeps and the RA evidence; **no size-exact raw win was found, so
`src/` is left at the base.**

## 1. Result

```text
_ZN5Level13createMissionEv   raw=3444  aligned=1021  size=17172/17172  insn=6206/6197
```

Sections (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 73,981
.ARM.extab   size   156,312 /   156,312   differing bytes    615  (base exact @ 0x1a3b78)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id  36 B, 20 differing bytes
```

Total differing bytes **74,616** (`73981 + 615 + 20`), unchanged.

Gate on the base tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**, 27/27 headers, file size 2,187,328 exact,
`RATCHET PASS`. No baseline update.

## 2. Whole-function adjacent-statement swap sweep (420 candidates)

Automated sweep over every adjacent pair of "simple" statements (trailing `;`,
balanced parens/brackets, no braces/preprocessor/`case`/`default`) inside the
function (`src` lines 1524–2581). 420 candidates, each a real `link_so` +
`tools/fn_diff.py`. **No candidate beat 3,444.** Size-exact results:

```text
raw 3444 (neutral, 52 candidates)   e.g.
  1531/1532  int level / int count / float base
  1557      missionPtr = 3 / bool introSystem
  1564      Mission *freelance / int pirates
  1601      field_168 = 0 / extraFriend = 0
  1608      bigShipsCount / jumperCount
  1621      raid = false / hostileCount = 0
  1631      raid = true / raidRace = 9
  1634      Wanted *wanted / bool hasWanted
  1644      alienPercent / extraAliens
  1651-1656 nameLeader block stores
  1661      alarmRequested / field_18a
  1673-1684 resetStation switch stores
  1689-1713 station-index / black-market / pirate-loot blocks
  2555-2570 case-184 convoy tail stores
raw 3445-3464 (size-exact, worse): 1612, 2556, 2559, 2570, ...
raw > 4000 (size-exact but much worse): 1627, 2557, 2564, 2567
size-fatal: 1615, 2554, 2558, ...
```

The largest single-line win available from this neighbourhood is zero; the base
is a local optimum for adjacent statements over the whole body.

## 3. Declaration-order permutation sweeps (real builds)

All measured with `link_so` + `fn_diff`; only size-exact results listed.

```text
alienGuns prologue {Matrix rotation; Vector v; int k; Vector base;}
  24 permutations: base (rotation,v,k,base) = 3,444; next best 3,444
  (rotation,v,base,k), (rotation,k,v,base), (k,rotation,v,base);
  all others >= 3,472, several >= 4,385.

pirateFortress prologue {Matrix rotation; Vector v; Vector base; const float *t;}
  24 permutations: base = 3,444; also (rotation,v,t,base),
  (rotation,t,v,base), (t,rotation,v,base) = 3,444; all others >= 3,445.

raid prologue {int raidType; bool hard; int guns;}
  6 permutations: base = 3,444; (raidType,guns,hard) and (guns,raidType,hard)
  also 3,444; all others size-fatal (17,160).
```

Moving `int guns = alienGuns + fortressGuns;` (the outer declaration) above the
`if (raid)` block, with or without the inner shadowing declaration, is
**size-fatal**: 17,168 B (raw 4,365). Keeping the outer declaration where it is
is required for the 17,172-byte shape.

## 4. Block-level moves / case-label order

```text
swap alienGuns block and pirateFortress block      17,152  raw 5,238  (fatal)
move if(raid) block after outer pirates loop        17,316  raw 16,763 (fatal)
move `int raidRoll` after stationIndex/freelance    17,172  raw 3,462
move `int raidRoll` after security                  17,220  raw 16,271 (fatal)
move `int raidRoll` just before the raid if         17,392  raw 16,717 (fatal)
swap adjacent case bodies in switch(getType()):
  case1<->case2   17,172 raw 3,573
  case6<->case7   17,172 raw 3,444 (neutral)
  case9<->case10  17,172 raw 3,444 (neutral)
  case12<->case15 17,172 raw 3,444 (neutral)
  case15<->case4  17,172 raw 3,444 (neutral)
  case4<->case184 17,172 raw 3,444 (neutral)
  case5<->case1   17,168 raw 15,528 (fatal)
```

Case-label order is not a lever: the compiler lays the bodies out by branch
probability, and the neutral swaps do not compose.

## 5. Register-allocation evidence (why the base is hard to move)

The whole `0x3200`–`0x3e00` cluster (raw **1,560 B**) is callee-saved register
rotation, not statement order. The original and ours use disjoint assignments
for the same source values:

```text
value            original                    ours
this (turret)    r4 (0xb3232)                r8 (0xb3230)
this (fortress)  r5 (0xb33f6)                r6 (0xb33f6)
this (raid)      r4 (0xb34de)                r8 (0xb34de)
Globals::status  r8 (0xb3752)                sl (0xb3752)
alienGuns        sl (0xb322e)                r6 (0xb322e)
Globals::rnd     spilled, reloaded at        kept in r4 from 0xb3234
                 0xb3788/0xb388a/0xb39e2/    through the pirateFortress
                 0xb3cde                     block (3 reloads)
```

The causal chain is circular: the original reloads `Globals::rnd` at
`0xb3788` (inside the raid condition), which frees `r4` for `this`; ours keeps
`rnd` in `r4` across the `pirateFortress` block (needed on the
`alienGuns == 0` branch to `0xb339a`), so `this` is pushed to `r8`. Every
source-level attempt to break the cycle was neutral or size-fatal:

```text
AERandom *rnd = Globals::rnd; before raid condition   17,172 raw 3,444
KIPlayer *gun declared outside alienGuns loop         17,172 raw 3,444
const float (*t)[6] declared outside alienGuns loop   17,172 raw 3,444
both gun and t outside the loop                       17,172 raw 3,444
Status *status = Globals::status; in raid block       17,160 raw 5,137 (fatal)
Array<KIPlayer*> *enemies local in alienGuns loop     17,236 raw 15,171 (fatal)
Array<KIPlayer*> *enemies local in pirateFortress     17,248 raw 15,874 (fatal)
load gun after createStaticObject (both turret loops) 17,172 raw 3,451
```

The `load gun after createStaticObject` variant is the faithful original order
(w13316 §2a) but is raw-worse because the `this`/`rnd` rotation is unchanged.

### 5a. `stationPirates` / `extraAliens` stack-slot swap (2 B + downstream)

The original stores `stationPirates` at `sp#48` and `extraAliens` at `sp#44`;
ours swaps them (`sp#44` / `sp#48`). The swap appears at `0xb06b4`, `0xb0848`,
`0xb2a58`, `0xb2a72`, `0xb3a6a`, `0xb3a82`, `0xb3cdc`, `0xb3dda`, `0xb3de6`,
`0xb3e60`, `0xb3e6a`, `0xb3f8e` — a handful of raw bytes. It is a
StackColoring decision, not declaration order: moving `int extraAliens = 0;`
above `unsigned stationPirates = ...` is neutral (3,444) and does **not** swap
the slots. Changing `unsigned stationPirates` to `int` is neutral; changing
`int extraAliens` to `unsigned` is raw 3,445.

### 5b. `case4Tail` loop shape

Original emits `strd r0, fp, [sp]` for `{waypoint, true}` with `fp = 1`
hoisted, and reloads `this->enemies` for the size test. Ours materialises
`movs r0,#1; str r0,[sp,#4]` inside the loop and keeps `fp = this->enemies`
via the `Array<KIPlayer *> *enemies = this->enemies;` local. Removing the
local (`this->enemies->size()`) is **size-fatal** (17,168, raw 3,599); using
the local for the store is also 17,168 (raw 3,576). The local is a load-bearing
size crutch.

### 5c. `case 1` return epilogue (6 B)

Original inlines `str r5,[r4,#40]; b.w 0xb02ca` at `0xb20f6`; ours branches to
the shared `case4Tail` epilogue (`b.w 0xb41e8; nop`). Size-neutral but a
6-byte raw diff. Adding an explicit `return` after `objectivesA = ...` is
size-fatal (17,164, raw 10,964, w13311 §4b).

## 6. Residual raw-byte map (base, per 0x100 block)

```text
0200 3   0400 10  0600 1   0800 1   0a00 1   0e00 1   0f00 1   1000 1
1200 5   1300 4   1400 146 1500 199 1600 225 1700 116 1800 236 1900 234
1a00 21  2100 6   2a00 17  2d00 3   3200 86  3300 217 3400 112 3500 15
3600 6   3700 125 3800 223 3900 239 3a00 224 3b00 249 3c00 225 3d00 140
3e00 33  3f00 1   4000 38  4100 201 4200 77  4300 2
total 3444
```

* `0x1400`–`0x1a00` (1,156 B): mission-type case bodies and the ±1–4 B TBH
  target deltas — w13334's lane.
* `0x3200`–`0x3e00` (1,560 B): callee-saved register rotation (§5).
* `0x4100`–`0x4300` (280 B): `case4Tail` loop shape (§5b) and the EH cleanup
  landing-pad chain (`String::~String` / `operator delete` order and stack
  slots) — w13322's lane.
* Everything else is ~60 B of literal-pool shifts and small branch-offset
  deltas caused by the case-body sizes.

## 7. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13343
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3444 aligned=1021 size=17172/17172`, `.text` diff 73,981,
`.ARM.extab` diff 615, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13343/`
(`Level.base.cpp`, `exp.sh`, `diff.py`, `rawblocks.py`, `rawranges.py`,
`sweep_adj.py`, `perm_ag.py`, `perm_pf.py`, `perm_raid.py`, `swapcase.py`,
`vA.py`..`vW.py`, `secdiff.py`).
