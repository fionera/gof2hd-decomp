# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 185 / w12914

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave185-w12914`, base master `f390b0e68` (wave-184
harvest 10: raw 3588). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: continue the bool/order sweep on the wave-184 body (`bool` vs `int`,
multiply vs branch, declaration order, `Globals::*` hoists).  Land only
size-exact, `.rel.plt`-identical raw wins.

## 1. Result

**One size-exact code win landed: the `alienGuns` turret-table anchor
`2*(i<0)` -> `(i<0)+(i<0)`.**

```text
                                     size            raw    aligned  insn
base f390b0e68                       17172/17172     3588   1140     6206/6197
after (final)                        17172/17172     3584   1140     6206/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 74,591 -> 74,587  (-4)
.ARM.extab   size   156,312 /   156,312   differing bytes    647 ->    647  (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id 20 (unchanged)
```

Total differing bytes **75,258 -> 75,254** (-4).  `check` green:
`byte_exact 4507/4524`, `linked_exact 4507/4524`, `size_exact 4524/4524`,
`stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
symbol parity **5370/5370**, `RATCHET PASS`.  No baseline update (the
function is still non-byte-exact).

## 2. Landed edit

In the `if (alienGuns != 0)` turret loop (`~line 1968`):

```cpp
-            const float (*t)[6] = (bigAlienWave + 2 * (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
+            const float (*t)[6] = (bigAlienWave + (i < 0) + (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
```

`i` is always non-negative, so `(i < 0) + (i < 0)` is 0 and the selector is
semantically `bigAlienWave ? station : outpost`.  The anchor is required to
keep clang from hoisting the select out of the loop (plain selector: size
17,160).  Wave-180 (w12413) measured `add2` as 4 raw better but 195 aligned
worse on that wave's body; on the current wave-184 body the aligned metric is
**unchanged at 1140**, so this is a clean raw-only win.

## 3. Sweep results that did NOT land (all real `link_so` + `fdiff`)

`bool` -> `int` on each flag (current body raw 3588 / size 17172):

```text
resetStation, alienRace, hard   raw 3588  size 17172   (neutral, no win)
raid                            raw 5153  size 17168
introSystem                     raw 14827 size 17180
hasWanted                       raw 12503 size 17148
nameLeader                      raw 13548 size 17180
siegedStation                   raw 16334 size 17308
bigAlienWave                    raw 12426 size 17156
alienWave                       raw 12392 size 17148
pirateFortress                  raw 12322 size 17160
```

Declaration-order permutations of the three wave flags
(`bigAlienWave, alienWave, pirateFortress`): only the current `012` order is
raw 3588; `021` 6426, `102` 8796, `120` 10283 (size 17164), `201` 12082
(17180), `210` 11931 (17180).

Anchor spellings on the current body: `2*` (base) 3588; `4*` 3588; `<<1`
3588; `|` 3588; `+0` 3588; `add2` **3584** (landed); `3*` 3916.
