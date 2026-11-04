# wf214: `Level::createMission` (91.128167 -> 92.028388 %, 17172 B) — wave-102

Scope: worktree `/home/debian/GalaxyOnFire2/wt/wf214`, branch `wave102-wf214`,
base `53d9ee4d5` (master). Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).
This pass lands two source-shape fixes in `src/game/world/Level.cpp`; no flags,
linker tricks, padding anchors, or data were touched.

## Baseline and result

```text
before: _ZN5Level13createMissionEv  fuzzy 91.128167 %  size 17172 B  insns 6055/6062
after:  _ZN5Level13createMissionEv  fuzzy 92.028388 %  size 17172 B  insns 6055/6063
gate before/after: byte 4500/4500, linked 4500/4500, size 4524/4524,
                   data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The official verifier (`tools/verify/verify.py`) reports the function as
`pct 92.0` (it rounds the row), `osize == usize == 17172`. `score1.py` (same
normalization as verify.py) reports the unrounded `92.028388`.

## Landed change 1 — case 9 crate store order

Original `b3696` (real address `0xb3696`):

```text
b3696 ldr.w r0,[r0,r6,lsl #2]   ; enemies[i]
b369a str   r1,[r0,#80]         ; cargo = nullptr
b369c strb.w r1,[r0,#76]        ; crateCaptured() = 0
```

The old source wrote the two stores through two independent
`(*this->enemies)[i]` expressions. Clang CSE'd the array load but then reloaded
the array element between the stores, producing `str; ldr; strb`. Binding the
element once and writing `crateCaptured()` before `cargo` makes clang keep the
element in one register and emit the original `str; strb` pair:

```cpp
KIPlayer *ship = (*this->enemies)[i];
((PlayerFighter *) ship)->crateCaptured() = 0;
ship->cargo = nullptr;
```

This is a real shape fix: the crate block at `0x36b6..0x36c4` now matches the
original instruction-for-instruction. On its own it removes 4 bytes
(`17172 -> 17168`), so it is combined with change 2 below.

## Landed change 2 — case4Tail hunter loop

The old source was:

```cpp
Globals *g = Globals::globals;
for (unsigned i = 0; i < this->enemies->size(); i++) {
    (*this->enemies)[i] = this->createShip(
            8, 0, g->getRandomEnemyFighter(8),
            this->playerRoute->getWaypoint(Globals::rnd->nextInt(this->playerRoute->length())), true, false);
    (*this->enemies)[i]->setToSleep();
}
```

The original hunter loop (`0x4184..0x41f8`) keeps `this` in `r8`,
`Globals::globals` in `sl`, the constant `1` in `fp`, and `i` in `r9`; it also
reloads `this->enemies->size()` each iteration. The old source instead put
`Globals::globals` in `r9`, `i` in `sl`, and `Globals::rnd` in `fp`, and
materialized `1` on every iteration. Caching the size in a local and dropping
the `g` local shifts the register allocation much closer to the original and
restores the missing 4 bytes:

```cpp
unsigned n = this->enemies->size();
for (unsigned i = 0; i < n; i++) {
    (*this->enemies)[i] = this->createShip(
            8, 0, Globals::globals->getRandomEnemyFighter(8),
            this->playerRoute->getWaypoint(Globals::rnd->nextInt(this->playerRoute->length())), true, false);
    (*this->enemies)[i]->setToSleep();
}
```

`n` is a tuning choice, not proven source: the original still reloads
`enemies->size()` in the loop condition. It is reported separately from the
proven crate fix.

## Measured variants

All measurements are `score1.py _ZN5Level13createMissionEv` on a fresh
`link_so` after each edit; the full `check` was run on the landed variant.

| variant | size | insns (orig/ours) | fuzzy % |
|---|---:|---:|---:|
| pristine base | 17172 | 6055/6062 | 91.128167 |
| crate fix only | 17168 | 6055/6061 | 91.135688 |
| crate fix + `unsigned n` + direct `Globals::globals` | **17172** | **6055/6063** | **92.028388** |
| crate fix + direct `Globals::globals`, no `n` | 17164 | 6055/6058 | 91.191282 |
| crate fix + `Globals *g` + `n` | 17172 | 6055/6064 | 90.849080 |
| crate fix + `n = hunterCount` | 17164 | 6055/6060 | 91.159719 |
| crate fix + `int i` loop | 17164 | 6055/6060 | 91.176228 |
| crate fix + `Route *route` local | 17168 | 6055/6061 | 90.871575 |
| crate fix + `Array *enemies` local | 17172 | 6055/6064 | 90.882086 |
| crate fix + flat turret pointer | 17172 | 6055/6063 | 92.028388 (neutral) |
| crate fix + `Vector` ctor turret args | 17172 | 6055/6063 | 92.028388 (neutral) |
| crate fix + `kBaseTurrets` 2-D row | 17172 | 6055/6062 | 91.986465 |
| crate fix + `raid` as `int`/`char`/`u8` | 17172 | 6055/6063 | 91.863344 |
| crate fix + `nameLeader` as `int`/`u8` | 17180 | 6055/6066 | 88.606551 |

Convoy if-chain order (case 183) with the landed change: `210`/`201` tie at
92.028388; `102`/`120` = 91.896353; `012`/`021` = 91.879848. The source keeps
`k==2, k==1, k==0, default`.

## Remaining structural differences (not landed)

- alienGuns selector: original branchy in-loop `ldr bigAlienWave; cbz` plus six
  individual `ldr`s; ours branchless `cmp.w/it/movne` plus `ldrd` pairs. The
  `i < 0` anchor is still required to keep the select in the loop.
- `raid`/spill slot: original `sp+88`, ours `sp+84` (prologue
  `str.w fp,[sp,#88]` vs `[sp,#84]`). Changing `raid`/`nameLeader` types does
  not fix it; it is a compiler stack-slot allocation difference.
- EH-pad tail: original main code ends at `0x4214`, ours at `0x420e`; the EH
  block is 2 bytes longer and the literal pool 4 bytes longer in ours. The
  totals cancel to size 17172.
- stationPirates/extraAliens register allocation and the merged predicate at
  `0xb2e04` (`(i != X) | !alienWave`) are still different.

## Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/wf214
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/wf214/score1.py _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

## Proven vs hypothesis

Proven:
- The crate store order/register shape at `0x36b6..0x36c4` is correct and now
  matches the original exactly.
- The landed variant keeps `size 4524/4524` and leaves every ratchet count
  unchanged; `check` passes and `drift` is 0.

Hypothesis:
- The `unsigned n` local is a plausible source shape but is not proven by the
  binary: the original reloads `enemies->size()` in the loop condition. It is
  landed because it restores the exact function size and improves the fuzzy
  metric; the faithful no-`n` variant is documented above (91.191282, 17164 B).
- The remaining selector/stack-slot/EH-tail differences are source/TU-shape
  residuals, not flag or data differences.
