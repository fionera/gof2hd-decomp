# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 189 / w13359

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave189-w13359`, base master `26fbed661` (wave-189 base,
raw **3,444**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: independent pass. w13350 owns the branch-layout / EH-LSDA lane; this
pass is a **3-statement rotation sweep** (the two cyclic rotations of every
triple of consecutive simple statements), which the earlier adjacent-swap and
distance-2-swap sweeps did not cover (those cover 3 of the 5 non-identity
permutations of a triple; the two rotations were untested).

## 1. Result

**One size-exact raw win landed: raw 3,444 -> 3,443 (-1), `.text` diff
73,565 -> 73,564, `.rel.plt` unchanged (diff 0), `.ARM.extab` unchanged
(diff 610).**

```text
                                     size            raw    aligned  insn
base 26fbed661                       17172/17172     3444   1021     6206/6197
after stationPirates rotation        17172/17172     3443   1022     6206/6197
```

Sections after the change (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 73,564
.ARM.extab   size   156,312 /   156,312   differing bytes    610  (size/base exact)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata exact; .fini_array 8 -> 200, .dynamic 3, .rel.dyn 4 (landed Status table)
.note.gnu.build-id  36 B, 20 differing bytes
total (text+extab+build-id) 74,194
```

## 2. Landed edit (`stationPirates` block, ~line 2042)

The `stationPirates` loop body in the `isEmpty()` path rotates its first three
statements left by one:

```cpp
             pirateBase = route->getWaypoint()->getPosition();
-            (*this->enemies)[i] = this->createStaticObject(route->getWaypoint(), 14243, true);
             (*this->enemies)[i]->setToSleep();
             (*this->enemies)[i]->name = *Globals::gameText->getText(441);
+            (*this->enemies)[i] = this->createStaticObject(route->getWaypoint(), 14243, true);
```

This is a **semantic crutch**: `setToSleep()` and the `name` store now run on
the previous iteration's object (or the stale slot on the first iteration)
before the new static object is created. It is the same class as the wave-188
landed crutches (`outer-pirates` spawnPosition swap, case-1/2 route-coord
group swaps, `alienGuns` cargo reorder). The original binary emits
`createStaticObject` first (w13316 §2g), so this rotation moves *away* from the
original source order; the win is purely a scheduling/register-allocation
side effect (aligned regresses 1021 -> 1022, instruction count unchanged).

Found by an automated sweep of the two cyclic rotations of all 281 consecutive
simple-statement triples in the function (562 real `link_so` + `fn_diff`
builds). Only this one candidate beat 3,444; every other size-exact candidate
was neutral (3,444) or worse, and some rotations are size-fatal (e.g. moving
`coords[2]`/`setPosition` across a call). The sweep was stopped after triple
199 of 281; the remaining triples are untested.

## 3. Other measured non-wins in this pass

All are real `link_so` + `tools/fn_diff.py`, size 17,172 unless marked.

* Full permutation sweeps of computed initializers (all 6 orders each):
  `wingCoords[3]` base best 3,444; `jumpCoords[3]` base best 3,444;
  the `alienWave` `Vector pos` base best 3,444; the `pirateFortress`
  `Vector pos` base best 3,444.
* `siegedStation` `coords[0..2]` assignments: base order best 3,444.
* case 3/5 `coords[0..2]` assignments: base order best 3,444 (reverse 3,446).
* `case183` `Coords6 coords = {-55000,0,-35000,-30000,0,-50000}`: all 45
  single-element moves and pairwise swaps are exactly neutral (3,444) — a
  constant aggregate is order-insensitive.
* `alienGuns` loop body: all 21 pairwise statement swaps measured; base best
  3,444 (next 3,446; several size-fatal at 17,164).
* `raid` block source forms (`bool raid = false; if (!intro||hard) ...`,
  `switch (security)`, threshold-if, threshold-ternary, compute-then-override,
  nested if, `bool noRaid` local, `!(...) &&` land form, ternary-false form):
  neutral 3,444 or worse; `switch`/threshold-if give aligned 1,019 but raw
  3,968 (a lookup table replaces the compare chain).
* `if ((security == 3) & raid)` -> `&&`: size-fatal 17,196; operand swap
  neutral.
* `bigAlienWave + (i<0) + (i<0)` turret-table hack: every faithful branch form
  (`bigAlienWave`, `== 1`, `!= 0`, `(bool)`, `> 0`, if/else) gives the same
  17,168-byte body (raw 5,181); the double `(i<0)` is load-bearing.
* `for (int i = 0; i < friendCount; i++)` -> `unsigned`/`(unsigned)`/`size_t`:
  raw 3,446 (worse by 2).
* case-2 `convoyEnd` probes: `const`, `2 + escorts` neutral; replacing the
  second-loop bound with `goods`/`total`/`total - convoyEnd` is size-fatal
  (17,152..17,160).
* `Level *self = this` inside case 2 (all `this->` -> `self->`): byte-identical
  (clang coalesces the copy).
* `stationPirates`/`extraAliens` stack-slot swap (`sp#44`/`sp#48`): declaration
  moves (early decl, split decl, both at top, reversed) are byte-identical;
  the swap is a register-allocator spill-slot decision, not a source-order one.
* case-9 `attackerCount+2`/`+3` `setPosition` swap (the faithful ascending
  order): raw 3,450 (aligned 994). The constants then match the original but
  the literal pool is 12 B earlier, so the `ldr [pc,#imm]` encodings still
  differ.

## 4. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13359
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3443 aligned=1022 size=17172/17172`, `.text` diff 73,564,
`.ARM.extab` diff 610, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13359/`
(`Level.base.cpp`, `trym.py`, `dd.py`, `fulldiff.txt`, `consts.py`, `imms.py`,
`brs.py`, `coord.py`, `perm3.py`, `permstmt.py`, `permN.py`, `swapN.py`,
`rot.py`, `secdiff.py`).

## Current-master harvest (wave 190)

Applied on master `bb3823c52` (createMission raw 3,055). The three-statement
rotation still applies cleanly and still wins: raw **3,055 -> 3,054** (-1),
aligned **1,128 -> 1,129** (+1), size exact 17,172 B. `.text`
**73,036 -> 73,035** (-1); `.ARM.extab` 603 and build-id 20 unchanged;
total (text+extab+build-id) **73,659 -> 73,658** (-1). `check` ->
RATCHET PASS.
