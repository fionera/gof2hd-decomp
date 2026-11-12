# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 188 / w13215

Target: `src/game/world/Level.cpp`, function at VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave188-w13215`, base master `ce16f45f1` (wave-187 harvest 5).
Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: branch layout / EH-LSDA / mission-type switch continuation. Nothing from
waves 181–187 is re-landed; the edit is on top of the wave-187 tree (raw **3,510**,
`.text` diff 74,223, `.ARM.extab` diff 639).

## 1. Result

**Two size-exact code wins landed: (1) in the `alienGuns` turret loop, move
`gun->cargo = nullptr` after `gun->geometry->setRotation(v)`; (2) in the
`case183` convoy `switch(k)`, rotate case 0's three `offset[]` stores so
`offset[2]` is last.**

```text
                                     size            raw    aligned  insn
base ce16f45f1                       17172/17172     3510    998     6205/6197
after win 1 (cargo reorder)          17172/17172     3508    999     6205/6197
after win 2 (case0 offset rotate)    17172/17172     3506    997     6205/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 74,223 -> 74,219  (-4)
.ARM.extab   size   156,312 /   156,312   differing bytes    639 ->    639  (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id 20 (unchanged count)
```

Total differing bytes 74,882 -> 74,878. `.ARM.extab` stays size- and base-exact
(@ `0x1a3b78`, 156,312 B). The function is still non-byte-exact, so the ratcheted
`byte_exact` count is unchanged (4507/4524).

Gate after the change: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**, 27/27 headers, file size 2,187,328 exact.
`RATCHET PASS`; no baseline update needed.

## 2. Landed edit

`alienGuns` turret loop (`~line 1971`):

```cpp
             v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
-            gun->cargo = nullptr;
             gun->geometry->setRotation(v);
+            gun->cargo = nullptr;
             gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
```

This is an **address-alignment crutch**, not a semantic reconstruction: moving the
`cargo` store after the virtual `setRotation` call changes the live range of the
`v` vector across the call and re-schedules the following `setPosition` block by
two bytes into alignment with the original. It is size-exact; the original's
store order was not separately decoded (the `cargo` store is dead here — the
turret object is immediately re-created as a `PlayerTurret`).

## 2a. Second edit

`case183` convoy `switch(k)` (`~line 2203`):

```cpp
                         case 0:
-                            offset[2] = -50000.0f;
                             offset[0] = 20000.0f;
                             offset[1] = 0.0f;
+                            offset[2] = -50000.0f;
                             break;
```

With `offset[2]` last in case 0, clang merges case 0's and case 2's final
`offset[2]` store into one shared `str r1,[sp,#380]` landing pad, exactly the
shape the original has at `0xb1436`. Raw 3,508 -> 3,506 (-2), size-exact.
Moving `offset[2]` last in **all** four arms instead makes clang convert the
whole switch to a table lookup and the function shrinks to 17,164 B
(size-fatal); the wave-187 `offset[1]` rotation plus this single-arm rotation
is the size-exact combination.

## 3. What was swept and did not win

* Full adjacent-statement swap sweep over the function body (`Level.cpp` lines
  1524–2580, 421 candidates) on the wave-187 base, each compiled and linked with
  the real gold link. Only three candidates beat 3,510 under the fast `.o`
  predictor; on the real link only this one holds:
  - `~1970` swap (`v = MatrixRotateVector(t[0..2])` before `t[3..5]`) predicted
    3,506 but measures **3,514** (predictor over-optimistic across a shifted
    relocation slot).
  - `~2126` swap (`this->enemies = new Array` before the `hunters` declaration)
    predicted 3,501 but measures **3,523**.
  - `~1971` swap (landed) predicted 3,508 and measures **3,508** exactly.
* Composed variants `1970+1971` (3,519) and `2126+1970` (3,527) lose.
* `case183` tail: restoring the original `player->setPosition` before
  `stationPosition = getPosition()` measures 3,511 (base 3,510); restoring the
  original `timeLimit`-before-`Objective` order measures 3,517.
* `case183` convoy `switch(k)` offset table: the semantically-correct constant
  order (`case0: offset[1]=-3000, offset[0]=20000, offset[2]=-50000`, etc.)
  makes clang emit a **table-lookup** form and shrinks the function to 17,164 B
  (size-fatal, raw 11,300). The wave-187 `offset[1]` rotation is what forces the
  original's branchy switch form; it is kept.

## 4. Remaining residuals (unchanged shape)

Dominant clusters: function offsets `0x1400..0x1a00` (case-183/1/2 bodies) and
`0x32c0..0x3e00` (raid/stationPirates/extraAliens), plus the tail
`0x40c0..0x4290`. The mission-type TBH table entries for types 1/2/4 still
differ because our case-183 body ends `+4` (the convoy `switch(k)` is 52 B vs
the original 48 B — ours inlines the `offset[2]` store in each arm, the original
branches to one shared `str r1,[sp,#380]`), and case1/case2/case4 sizes absorb
that delta (`+4 -2 -4 +2 = 0`). Fixing the switch to 48 B alone would make the
function 17,168 B, so it needs case1/case2/case4 reconstructed to their exact
sizes in the same pass.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13215
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3508 aligned=999 size=17172/17172`, `.text` diff 74,221,
`.ARM.extab` diff 639, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13215/`
(`fastdiff.py`, `predict.py`, `link_cand.py`, `sweep_adj.py`, `secdiff.py`).
