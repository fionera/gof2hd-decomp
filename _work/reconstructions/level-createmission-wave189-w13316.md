# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 189 / w13316

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave189-w13316`, base master `b1455e159` (wave-188
harvest, raw **3,444**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: independent **semantic / data-flow** pass on the mission-type case
tails and the `alienGuns`/`pirateFortress`/raid/`stationPirates`/
`extraAliens` region (`0xb3200–0xb3e00`): bool-vs-int locals, `Array`/`Vector`
construction/assignment forms, multiply-vs-branch, `Globals::*` hoists, and
exact `Mission`/`Station`/`Ship` field store order. Branch layout / EH-LSDA
is w13311's lane.

## 1. Result

**No code win landed. `src/game/world/Level.cpp` is byte-identical to the
base** (`git status --porcelain` empty). Final measurement:

```text
_ZN5Level13createMissionEv   raw=3444  aligned=1021  size=17172/17172  insn=6206/6197
```

`check` green: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
symbol parity **5370/5370**, `RATCHET PASS`. No baseline update.

The pass did recover the **original source semantics** of several blocks from
the disassembly (below). Each individual semantic fix is size-exact but
raw-neutral or raw-worse; the wave-188 body is a local optimum in every
neighbourhood probed. The base's semantic crutches are load-bearing for the
17,172-byte size, so matching the binary more closely in one case tail forces
a compensating shape change in another (cf. wave-188 §4).

## 2. Recovered original semantics (evidence from the binary)

### 2a. `alienGuns` loop (`~line 1958`)

Original loads `enemies[i]` **after** the `createStaticObject` store:

```asm
b32a4  blx  createStaticObject
b32a8  ldr.w r1,[r4,#248]      ; this->enemies
b32ac  ldr  r1,[r1,#4]
b32ae  str.w r0,[r1,fp,lsl#2]  ; (*enemies)[i] = new object
b32b6  ldr.w r0,[r4,#248]      ; reload this->enemies
b32bc  ldr.w r6,[r0,fp,lsl#2]  ; gun = (*enemies)[i]  (the NEW object)
b32c2  blx  Player::setMaxHitpoints
```

so the source is `(*this->enemies)[i] = createStaticObject(...); KIPlayer
*gun = (*this->enemies)[i]; gun->player->setMaxHitpoints(1000);` (or the
equivalent repeated-expression form). The base instead loads `gun` **before**
the store (the old object). Same pattern in the `pirateFortress` loop
(`0xb340c`/`0xb3420`). Fixing both is size-exact but raw **3,444 → 3,451**.

### 2b. Raid loot block (`~line 2011`)

Original call order is `p->setHitpoints(p->getMaxHitpoints()*2)` →
`setSpeed(3.5f)` → `setRotate(3)` → `spawnPosition = player->getPosition()`
(`0xb3836–0xb387a`). The base order is `setRotate` → `setSpeed` →
hitpoints → `spawnPosition`. Reordering to the original is size-exact but
raw **3,444 → 3,452**.

### 2c. `case 2` second (asteroid) loop (`~line 2295`)

Original order after `setPosition`: `cargo = nullptr`, `crateCaptured() = 0`,
`setSpeed(0.0f)`, `setRotationSpeed(0.0f)`, then
`p->setHitpoints(p->getMaxHitpoints()*3)` (`0xb18??`). The base order is
hitpoints → setRotationSpeed → setSpeed. Reordering is size-fatal
(17,164) — the base order is a size crutch.

### 2d. `case 183` convoy switch (`~line 2205`)

The original literal pool at `0xb15bc` and the shared-tail `offset[2]` store
give the true values and **store order `offset[1]`, `offset[0]`, `offset[2]`
for every arm**:

```text
case 0:    o1=-3000   o0=20000  o2=-50000
case 1:    o1=-7000   o0=10000  o2=-30000
case 2:    o1=0       o0=25000  o2=-36000
default:   o1=6000    o0=12000  o2=-45000
```

The base carries a permutation of these values and a different per-case
store order. Reconstructing the semantics is size-fatal: full semantic switch
17,164 B (-8); case-1 semantic alone 17,156 (-16); case-2 alone 17,164 (-8);
case-0 alone stays 17,172 but raw 3,459. This is the wave-188 §4 size
balance: the base's wrong values/order are exactly what keeps the function
17,172 B.

### 2e. `case 183` tail (`~line 2240`)

Original order: `player->setPosition(-30000,5000,-100000)` →
`stationPosition = player->getPosition()` → `timeLimit = 181000` →
`objectivesA = new Objective(3,181000,this)` (`0xb145e–0xb1478`). The base
has `stationPosition` first and `timeLimit` last. All 24 permutations of
these four statements were measured: base `B,A,C,D` is best at **3,444**;
`A,B,C,D` (setPosition first) is **3,446**; `A,B,D,C` is **3,457**; the
other 21 are ≥5,283. The original order is therefore *not* the raw optimum.

### 2f. `extraAliens` block (`~line 2059`)

Original computes `coords[3]` from `this->spawnPosition` **after** all three
`x/y/z` updates (`0xb3d9e–0xb3dba`) — there is no `Vector sp` copy. The base
takes `Vector sp = this->spawnPosition` after the `x` update and uses `sp`
for `coords`. Removing the copy is size-exact but raw **3,444 → 3,532**;
moving the copy after the `z` update is **3,528**. The copy is a size crutch
for the loop-bound register/stack schedule.

### 2g. `stationPirates` block (`~line 2038`)

Original emits `createStaticObject(route->getWaypoint(),14243,true)` **before**
`pirateBase = route->getWaypoint()->getPosition()` (`0xb3b22–0xb3b56`); the
base has `pirateBase` first. Swapping is size-fatal (17,176); hoisting a
`Waypoint *wp` local is 17,168.

### 2h. `case 4` tail (`~line 2122`)

Original computes `hunters`, then loads `options[44]`, then `hunterCount`,
then `new Array` (`0xb40c8–0xb4120`). All 24 valid orderings of
`{hunters decl, opt44 decl, hunterCount decl, new Array}` were measured;
base `O,H,N,C` is best at **3,444**; `H,O,N,C` is 3,447; `H,O,C,N` 3,448.
The original order is 3,447, i.e. base is again the raw optimum.

## 3. Other sweeps that did not land (real `link_so` + `fn_diff`)

* Adjacent-statement swap sweep over lines **1958–2110** (alienGuns,
  fortress, raid, pirates, stationPirates, extraAliens, convoy, blueprint):
  no candidate below 3,444; best neutral 3,444.
* Full 24-permutation sweep of the alienGuns tail
  `{cargo, setPosition, setScaling, shipGroup}`: base order 3,444; next
  3,446; no win.
* `bool` → `int` for `raid`, `hasWanted`, `nameLeader`, `bigAlienWave`,
  `alienWave`, `pirateFortress`, `introSystem`, `resetStation`: only
  `resetStation` is neutral (3,444); the rest are ≥3,672 and mostly
  size-fatal. All bools are correct.
* `pirateEscorts` / `blueprint` declaration moves: 16,752–16,791 raw,
  size +164/+188. Declaration order is a strong size lever; base is best.
* `case 4` tail `globals`/`enemies` local removal or reorder: 3,549–3,601,
  some size-fatal.
* `case 4` tail `enemies` local used for the createShip target: 3,576,
  size 17,168.
* Pairwise combinations of the eight neutral 3,444 swaps (28 pairs): all
  remain exactly 3,444. Neutral changes do not compose into a win.
* `case 183` tail `ABCD` (3,446) + each neutral swap: all remain 3,446.

## 4. Residual map (base, per 0x100-byte block)

```text
0200 3   0400 10  0600 1   0800 1   0a00 1   0e00 1   0f00 1   1000 1
1200 5   1300 4   1400 146 1500 199 1600 225 1700 116 1800 236 1900 234
1a00 21  2100 6   2a00 17  2d00 3   3200 86  3300 217 3400 112 3500 15
3600 6   3700 125 3800 223 3900 239 3a00 224 3b00 249 3c00 225 3d00 140
3e00 33  3f00 1   4000 38  4100 201 4200 77  4300 2
total 3444
```

The large blocks `0x3700–0x3e00` are dominated by **callee-saved register
rotation**, not semantic differences: original uses `this=r4/r5/r8`,
`i=fp`, `k=r9`, `gun=r6` in the turret loops, ours uses `this=r8`,
`i=sl`, `k=r6`, `gun=r4`. The same rotation shows up as the `sp#44`/`sp#48`
and `sp#116`/`sp#124` stack-slot swaps in `0x2a00`, `0x3f00`, `0x4100`.
These are not reachable from the statement/declaration orders probed here.
`0x4200`/`0x4300` are the switch jump table (branch layout, w13311's lane).

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13316
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3444 aligned=1021 size=17172/17172`, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13316/`
(`Level.base.cpp`, `cmp2.sh`, `blockdiff.py`, `perm.py`, `c4sweep.py`,
`c183tail.py`, `boolsweep.py`, `combo.py`, `combo2.py`).
