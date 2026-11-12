# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 190 / w13450

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave190-w13450`, base master `8d8f43bcd` (raw **2,984**,
aligned **1,119**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave190-w13427.md` (case-2 goods read before
`new Route`) and `level-createmission-wave190-w13399.md`.

## 1. Result — landed

```text
                                     size            raw    aligned  insn
base 8d8f43bcd                       17172/17172     2984   1119     6210/6197
landed                               17172/17172     2982   1123     6210/6197
```

Raw **2,984 -> 2,982** (-2), aligned **1,119 -> 1,123** (+4), size exact
17,172 B. `.rel.plt` byte-identical, 27/27 headers, file size 2,187,328
exact. Gate: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `data_exact 843/843`, imports **207**, lint
**CLEAN**, drift **0**, `RATCHET PASS`. No baseline update.

## 2. The change: `case 2` coords slot order

The `case 2` route coordinate initializer had the two random `x`/`z`
components in slots 2 and 4:

```cpp
int coords[6] = {0, 0, (int) (float) (-20000 - Globals::rnd->nextInt(20000)), 0,
                 (int) (float) (-20000 - Globals::rnd->nextInt(20000)), 0};
```

The original's first `nextInt` result is stored to `coords[0]` (the
`0xb16d4` prologue zero-inits `coords[1..3]` around the first `nextInt`
call). Moving the two random elements to slots 0 and 4:

```cpp
int coords[6] = {(int) (float) (-20000 - Globals::rnd->nextInt(20000)), 0, 0, 0,
                 (int) (float) (-20000 - Globals::rnd->nextInt(20000)), 0};
```

is the whole patch. It is a **semantic** change (the two RNG draws land in
different coordinate slots; the original's layout is the one used here).
Measured size-exact raw win of 2 B.

## 3. Angles tried and rejected (all size 17,172 unless noted)

* Other coords placements: `A@0 B@3` raw 2,983; `A@1 B@2` 2,992 (aligned
  1,117); `A@0 B@5` size-fatal 17,164; `A@0 B@1` 17,180; `A@4 B@5` 9,380.
* `case 2` `asteroidPos` hoisted to the top: 3,018; after `goods`: 3,010.
* `volatile int offset` variants: `unsigned` 2,984 (aligned 1,122),
  `short` size-fatal 17,180, `long long` 17,236, `volatile int total`
  17,180, `volatile goods` 17,204, `volatile escorts` 3,175 (aligned 768).
* `case 1` declaration permutations (15 valid): base best 2,984; next
  3,048; several 3,053–3,077.
* `case 1` loop forms: `unsigned` 2,986; `!=` size-fatal 17,228; `++i`
  neutral; `i+=1` neutral; `size_t` 2,986.
* Raid block: all 6 permutations of `raidType/hard/guns` neutral 2,984
  except `guns` first -> size-fatal 17,160; `int hard` neutral; `guns`
  after the `if` -> 4,537/17,164.
* `__builtin_expect` on `comingFromAlienWorld`: removing -> 3,003 (aligned
  1,138); changing to 1 -> 17,184; wrapping the whole condition -> 17,344.
* `hasWanted` as `int`/`unsigned char` -> 17,148 (fatal); `i==0 && hasWanted`
  -> 17,164; second-condition swap -> 15,460.
* `station-108` objective temp/local/split: neutral or 3,004.
* `extraAliens` loop `first`/`base` locals -> 17,164; `start` operand
  orders: `guns + stationPirates + pirateEscorts` neutral, other order
  17,364.
* Full 3-statement rotation sweep (both cyclic rotations of every
  consecutive simple-statement triple, lines 1524–2581, 205 unique
  triples): zero wins; all neutral or worse. This lens is exhausted.
* Case-183 switch arm order (`case 2` before `default`) -> 2,985.

## 4. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13450
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=2982 aligned=1123 size=17172/17172`, `.rel.plt` diff 0,
`RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13450/`
(`try.py`, `rundiff.py`, `dis.py`, `firstlen2.py`, `sweep_rot.py`,
`sweep_c2coords.py`, `secdiff.py`, `Level.base.cpp`, `v*.json`).
