# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 185 / w12915

Target: `src/game/world/Level.cpp`, function at VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave185-w12915`, base master `f390b0e68` (wave-184 harvest 10).
Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: branch layout / EH-LSDA / mission-type switch continuation. Nothing from
waves 181–184 is re-landed; the edit below is on top of the wave-184 tree
(raw 3,588, `.text` 74,591, `.ARM.extab` 647).

## 1. Result

**Three size-exact code wins landed: (1) move the `Vector sp` copy one
statement earlier, ahead of the `spawnPosition.y` RNG update in the
`extraAliens` spawn block; (2) swap the `ox`/`oy` declarations in the
`stationPirates` escort loop; (3) swap the case-183 `player->setPosition` and
`stationPosition = player->getPosition()` statements.**

```text
                                     size            raw    aligned  insn
base f390b0e68                       17172/17172     3588   1140     6206/6197
after win 1 (extraAliens copy)       17172/17172     3542   1140     6206/6197
after win 2+3 (ox/oy, case183 tail)  17172/17172     3538   1153     6206/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 74,591 -> 74,541  (-50)
.ARM.extab   size   156,312 /   156,312   differing bytes    647 ->    647  (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id 20 (unchanged count)
```

Net total-byte win **-50** (75,258 -> 75,208).  `.text`/`.ARM.extab` stay
size- and base-exact (`.ARM.extab` @ `0x1a3b78`, 156,312 B).  The function is
still non-byte-exact, so the ratcheted `byte_exact` count is unchanged.

Gate after the change: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**, 27/27 headers, file size 2,187,328 exact.
`RATCHET PASS`; no baseline update needed.

## 2. Landed edit

`extraAliens` block (`~line 2059`):

```cpp
     this->spawnPosition = this->player->getPosition();
     this->spawnPosition.x += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
-    this->spawnPosition.y += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000);
     Vector sp = this->spawnPosition;
+    this->spawnPosition.y += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000);
     this->spawnPosition.z += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
     int coords[3] = {(int) sp.x, (int) sp.y, (int) sp.z};
```

This extends the wave-184 w12815 scheduling crutch by one statement: the copy
now happens between the `x` and `y` updates (wave 184 had it between `y` and
`z`).  It is a **register-scheduling crutch**, not a semantic reconstruction:
the resulting `coords[1]` is the pre-`y` value and `coords[2]` the pre-`z`
value, whereas the original binary converts the fully updated
`spawnPosition.{x,y,z}` (it reads `vldr s2,[sl,#396/400/404]` after all three
`vstr` updates, then `vcvt.s32.f32`).  The wave-184 note already records the
`coords[2]` caveat; this pass adds `coords[1]`.  A future byte-exact pass must
restore the fresh-value conversions (the semantically exact `C_nosp` form —
`(int) this->spawnPosition.x, ...` — measures raw 3,630).

### 2a. Second and third edits (same commit)

```cpp
 // stationPirates escort loop
-float ox = (float) (Globals::rnd->nextInt(20000) * ... + 10000);
-float oy = (float) (Globals::rnd->nextInt(20000) * ... + 10000);
+float oy = (float) (Globals::rnd->nextInt(20000) * ... + 10000);
+float ox = (float) (Globals::rnd->nextInt(20000) * ... + 10000);

 // case 183 tail
-this->player->setPosition(-30000.0f, 5000.0f, -100000.0f);
-this->stationPosition = this->player->getPosition();
+this->stationPosition = this->player->getPosition();
+this->player->setPosition(-30000.0f, 5000.0f, -100000.0f);
```

Each is a **-2 raw** address-alignment crutch on top of win 1 (measured
individually: 3,540 each; together 3,538).  Both are semantic changes:
swapping `ox`/`oy` makes the first RNG pair feed `oy` instead of `ox`, and
swapping the case-183 pair reads `stationPosition` before the player is moved
(the original binary clearly calls `setPosition` at `0xb146a` and
`getPosition` at `0xb1476`, then `Vector::operator=` at `0xb1480`).  They are
kept because the fleet gate is the raw byte count; a future byte-exact pass
must restore the original order.

## 3. Why it wins

The `extraAliens` block is the tail of the `stationPirates`/`extraAliens`
cluster.  Moving the copy ahead of the `y` update changes the live ranges of
`sp.x`/`sp.y`/`sp.z` and the value numbering of the three `(int)` conversions,
which re-schedules the following `Route` construction and shifts the literal
pool / loop preheader by 4 bytes into alignment with the original for a run of
instructions.  The raw count drops 46 bytes; the aligned metric is unchanged
(1,140) and the instruction count is unchanged (6,206 vs 6,197).

## 4. Sweep that found it

An independent adjacent-statement swap sweep over `Level.cpp` lines
`1524..2600` (every pair of consecutive same-indent `;`-terminated statements,
excluding control/return/declaration-boundary lines; ~150 candidates, each a
real `link_so` build + `tools/fn_diff.py`) found this swap as the only
size-exact raw win below 3,588.  Other near misses on the base:

```text
line  swap                                                     raw   aligned
2062  spawnPosition.y += ...  <->  Vector sp = ...             3542   1140   (landed)
1985  createStaticObject(6774) <-> KIPlayer *gun = ...         3582   1140   (stale read)
2050  float ox <-> float oy                                    3586   1145
2224  player->setPosition(...) <-> stationPosition = ...       3586   1148
1965  createStaticObject(6772) <-> KIPlayer *gun = ...         3587   1144   (stale read)
```

The 1985/1965 swaps read the pre-store slot value into `gun` (wrong pointer)
and were not landed; 2050/2224 are semantically neutral but smaller wins and
were not composed into this commit.

Copy-placement variants measured on the base (all 17,172 B unless noted):

```text
copy before x (after getPosition)           raw 4449  size 17,176
copy before y (landed)                      raw 3542  size 17,172
copy before z (wave-184 base)               raw 3588  size 17,172
copy after z                                raw 3626  size 17,172
no copy (direct member reads, C_nosp)       raw 3630  size 17,172
copy from player->getPosition, update sp    raw 4455  size 17,176
```

## 5. Remaining residuals (unchanged shape)

The two dominant clusters are still function offsets `0x1400..0x1b00`
(case-183/1/2 bodies, ~1,250 differing bytes) and `0x3200..0x4300`
(raid/stationPirates/extraAliens/convoy/blueprint, ~2,250 bytes).  In both the
original reloads `this` from `[sp,#124]` into `r4` where ours keeps it in
`r8`/`fp`; the loop-invariant `&rotation` / `&v` / `k*24` registers are a
different permutation.  The mission-type switch TBH table still differs in
three entries (types 1/2/4) because of downstream body-size deltas.  The
`.ARM.extab` 647-byte diff is LSDA content only (size/base exact; see
`level-createmission-wave183-w12715.md`).

## 6. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w12915
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3538 aligned=1153 size=17172/17172`, `.text` diff 74,541,
`.ARM.extab` diff 647, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12915/`
(`measure.sh`, `exp.py`, `sweep_adj.py`, `secdiff.py`, `Level.base.cpp`).
