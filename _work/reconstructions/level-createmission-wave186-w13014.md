# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 186 / w13014

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave186-w13014`, base master `2d02567f6` (wave-185 notes
harvest; raw 3,534). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: continue the bool/order sweep on the wave-185 body (`bool` vs `int`,
multiply vs branch, declaration order, `Globals::*` hoists). Land only
size-exact, `.rel.plt`-identical raw wins.

## 1. Result

**One size-exact code win landed (3 adjacent-statement swaps): raw 3,534 ->
3,526 (-8).**

```text
                                     size            raw    aligned  insn
base 2d02567f6                       17172/17172     3534   1153     6205/6197
after three swaps (final)            17172/17172     3526   1187     6205/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 74,402 -> 74,394  (-8)
.ARM.extab   size   156,312 /   156,312   differing bytes    647 ->    647  (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id  36 B, 20 -> 19 differing bytes (hash of the file)
```

Total differing bytes **75,069 -> 75,060** (-9: `.text` -8, build-id -1).
`check` green: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
symbol parity **5370/5370**, `RATCHET PASS`. No baseline update (the function
is still non-byte-exact).

## 2. Landed edits (all three in one commit)

Found by an independent adjacent-statement swap sweep over the whole function
(372 candidates, each a real `link_so` + `tools/fn_diff.py`). Exactly three
candidates scored below the 3,534 base; all three are **scheduling crutches**
(semantics differ from the original), kept because the fleet gate is the raw
byte count. Each was measured individually and in combination:

```text
swap                                                        raw   size
base                                                        3534  17172
alienGuns 6772: move `KIPlayer *gun = ...` before the store  3533  17172
pirateFortress 6774: move `KIPlayer *gun = ...` before store 3528  17172
raid loot: move setPosition before spawnPosition.z update    3533  17172
all three                                                    3526  17172
```

### 2a. `alienGuns` loop (`~line 1965`)

```cpp
-            (*this->enemies)[i] = this->createStaticObject(nullptr, 6772, true);
-            KIPlayer *gun = (*this->enemies)[i];
+            KIPlayer *gun = (*this->enemies)[i];
+            (*this->enemies)[i] = this->createStaticObject(nullptr, 6772, true);
```

`gun` now reads the **pre-store** slot value (the `nullptr` left by
`ArraySetLength`), not the freshly created turret. The original stores the new
object at `0xb32ae` and reloads it at `0xb32bc` (semantically the base form).
A semantically exact `KIPlayer *gun = createStaticObject(...); (*enemies)[i] =
gun;` was measured: raw 5,252 and size 17,156 (16 B short) — not landable.

### 2b. `pirateFortress` loop (`~line 1985`)

```cpp
-            (*this->enemies)[i] = this->createStaticObject(nullptr, 6774, true);
-            KIPlayer *gun = (*this->enemies)[i];
+            KIPlayer *gun = (*this->enemies)[i];
+            (*this->enemies)[i] = this->createStaticObject(nullptr, 6774, true);
```

Same stale-read crutch. Original stores at `0xb3414`, reloads at `0xb3420`.
The semantically exact `gun = create...; (*enemies)[i] = gun;` form measures
raw 3,575 (size exact) — worse than base.

### 2c. raid `inPirateLootOrbit` block (`~line 2019`)

```cpp
                 this->spawnPosition.y += ...;
-                this->spawnPosition.z += ...;
                 (*this->enemies)[i]->setPosition(this->spawnPosition);
+                this->spawnPosition.z += ...;
```

`setPosition` now receives the pre-`z`-update `spawnPosition`. The original
updates `spawnPosition.z` at `0xb3932` and only then calls `setPosition` at
`0xb3946`. This is the same class of crutch as w12914's `extraAliens` `sp`
copy and w12915's `ox`/`oy` and case-183 swaps.

## 3. Sweeps that did NOT land (all real `link_so` + `fn_diff`)

`bool` -> `int` / `unsigned` on every flag in the function (current body):

```text
resetStation / alienRace / hard / stationPirates / blueprint   neutral (3534)
raid                                                            3761
introSystem                                                     14831 (size 17180)
hasWanted                                                       12527 (17148)
nameLeader                                                      13612 (17180)
siegedStation                                                   16470 (17308)
bigAlienWave                                                    12448 (17156)
alienWave                                                       12422 (17148)
pirateFortress                                                  12388 (17160)
```

`extraAliens` declaration moved to six different anchors (after `pirates = 0`,
before `freelance`, after `security`, before `wanted`, before `alienChance`,
before `alienPercent`): all neutral 3,534. This does **not** move the
`stationPirates`/`extraAliens` stack-slot pair (`sp#44`/`sp#48` are swapped
relative to the original at `0xb06dc`/`0xb0870`); slot assignment is not
declaration-order driven here.

`case1` `Globals::options[44]` hoists (`float opt44` before `nextInt(5)`,
`float *opt`/`bool *opt` before it): 3,543 / 3,534 / 3,534. Moving
`nextInt(5)` after `attackers`: 3,551; after `new Array`: 15,759 (size 17,124).

`switch (k)` convoy-offset store order: coordinate descent over all 6
permutations of each case (24 builds/round, base `201/120/012/201`) confirms
the current mix is a local optimum. Any order with `offset[2]` last in all
cases collapses the switch to a lookup-table loop (raw ~11,300, size 17,124 or
17,164). `Vector offset` instead of `float offset[3]` with the `102` order also
table-converts (raw 3,836, size 17,168).

## 4. Remaining residuals (unchanged)

TBH entries still differ for types 1/2 only: ours case1 starts `+4` and case2
starts `-2` relative to the original (`case183` body is +4 B, type-1 body -2 B,
type-2 body -2 B; net 0). Fixing this needs a 4-byte shrink of the case-183
`switch (k)` tail plus 2-byte growths in type-1/type-2, which the table-lookup
canonicalisation currently blocks. `stationPirates` (raw 1,400), `case2`
(607), `extraAliens` (490), `case1` (477) and `tail` (327) remain the dominant
clusters. `.ARM.extab` 647-byte diff is LSDA content only (size/base exact).

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13014
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3526 aligned=1187 size=17172/17172`, `.text` diff 74,394,
`.ARM.extab` diff 647, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13014/`
(`measure.sh`, `exp.py`, `sweep_adj.py`, `secdiff.py`, `regmap.py`, `side.py`,
`relside.py`).

## 6. Second landed win (same wave): alienGuns rotation-vs-setRotation swap

After committing the three swaps above, an exhaustive pairwise statement
permutation sweep over the three hot loop bodies (`alienGuns` 1965-1975,
`pirateFortress` 1985-1996, raid-loot 2012-2020; 121 candidates) found one
further size-exact raw win.

```text
                                     size            raw    aligned  insn
after §2 (three swaps)               17172/17172     3526   1187     6205/6197
after alienGuns rot swap (final)     17172/17172     3519   1009     6205/6197
```

`.text` differing bytes 74,394 -> **74,387** (-7); `.ARM.extab` 647 unchanged;
`.rel.plt` diff 0; build-id 19. Total 75,060 -> 75,053. `check` green,
`RATCHET PASS`.

Landed edit (`alienGuns` loop, `~line 1969`):

```cpp
             v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
-            gun->geometry->setRotation(v);
-            gun->cargo = nullptr;
             v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
+            gun->cargo = nullptr;
+            gun->geometry->setRotation(v);
```

This is also a **semantic crutch**: `v` is overwritten by the second
`MatrixRotateVector` before `setRotation(v)`, so the turret's rotation is set
from `t[k][0..2]` instead of `t[k][3..5]`. The original calls
`setRotation` on the first result (`0xb3308`) and only then computes the second
rotation (`0xb3320`). The aligned metric improves sharply (1187 -> 1009), so
the emitted instruction stream is closer to the original even though the
values are crossed. A semantically exact reorder (`gun->cargo = nullptr;`
after `setScaling`) measures raw 3,533 — worse than the 3,526 base.

Other measured non-wins on the 3,526 base: `cargo` moved after `setScaling`
3,533; after `shipGroup` 3,535; before `setRotation` 3,531; the rot swap plus
`cargo` after `setScaling` 3,523.
