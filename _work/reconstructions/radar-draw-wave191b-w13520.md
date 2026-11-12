# Radar::draw wave 191b (w13520) — fifth lens: gas/planet chains, music mask,
# expect sweep, stack-slot swap; no size-exact raw win

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left at
the base. This pass commits only this note. Worktree `wt/w13520`, branch
`wave191b-w13520`, base `master` `5c99979c4`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191b-w13510.md` (fourth lens, base 6,016),
`radar-draw-wave191-w13488.md` (planet-loop stationId hoist + music predicate order),
`radar-draw-wave191-w13487.md` (operand-order cells), `radar-draw-wave191-w13505.md`
(asteroid-lock value ternary) and `radar-draw-wave189-w13319.md` (the `campaign`-in-`sl`
register map).  Every number below was measured in this worktree on the **current** 6,016
base (the w13510 note quotes 6,055 from the pre-w13505 base).

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,016** |
| instruction-index alignment | 1,941 |
| instruction count | 3,541 ours / 3,532 orig |
| `.rel.plt` | byte-identical |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`src/` is byte-identical to `HEAD`; no code change is committed by this pass.

## 1. Cells measured this pass (all size-exact 9,908 unless noted; base raw 6,016)

### 1.1 Gas-cloud 24000 chain (source order is x-z-y, original is x-y-z)

The source at `Radar.cpp:1004` tests `targetPos.x`, then `.z`, then `.y`.  The original's
gas chain (`12f64c`-`12f6aa`) tests **x, y, z** and ends with `bpl` on the last term.
Reordering the source to x-y-z reproduces the original's instruction order and the `bpl`
tail, but the register allocation re-rolls (`s2/s18/s20` instead of the original
`s0/s16/s18`, because `camZ` occupies `s16`), so it is a raw loss:

| cell | raw | aligned | note |
|---|---:|---:|---|
| base (x-z-y) | **6,016** | 1,941 | |
| x-y-z (`gas_xyz`) | 6,018 | 1,941 | order matches, `s2` vs `s0` |
| x-y-z + `this->cameraZ` inline (`gas_xyz_inline`) | 6,135 | 1,934 | constants snap to `s16/s18`, layout re-rolls |
| `this->cameraZ` inline only (`gas_inline`) | 6,135 | 1,934 | |
| `camZ` declared inside `if (cloud == locked)` | 6,138 | 1,938 | |
| 16 axis-order x operand-form permutations of the gas chain | 6,016-8,198 | — | `yxz`/`zxy` are size-fatal (9,912/9,904) |

### 1.2 Planet box (24 permutations)

The base is `CDAB` (`screenY > top && top + size > screenY && screenX > left &&
left + size > screenX`), i.e. the y-pair first and the x-pair reversed — the w13487
landing.  All 24 permutations were re-measured on the current base: `CDAB`/`CDBA`/`DCAB`/
`DCBA` = 6,018; `ABCD`/`BACD`/`ABDC`/`BADC` = 6,019; the other 16 are 8,972-9,264
(most size-fatal).  The original's *instruction* order is actually x-pair first
(`12e538`-`12e55a` compares `screenX` against `left`), but reordering the source to match
costs raw bytes.  Base is optimal.

### 1.3 Music mask predicate order

The source at `Radar.cpp:1272` is `range || expect(current != 151 && alwaysEnemySeen) ||
mask == 0`.  The original evaluates the **mask second** and the `alwaysEnemySeen` term
third (`12f280` `sub #136`/`cmp #15`/`tst 0xe071`, then `12f298` `cmp #151`/`eors`).
Reordering the source to `range || mask == 0 || expect(...)` reproduces the original's
order but is a raw loss:

| cell | raw | aligned | insn |
|---|---:|---:|---:|
| base | **6,016** | 1,941 | 3,541 |
| mask second (`mus_maskfirst`) | 6,039 | 1,939 | 3,542 |
| mask second, no expect | 7,183 (size 9,900) | 1,925 | 3,539 |
| `expect(current != 151) && alwaysEnemySeen` (`mus_expcur`) | 6,020 | 1,942 | 3,541 |

### 1.4 Exhaustive `__builtin_expect` flip sweep (29 sites)

All 29 `__builtin_expect` sites in `draw` were flipped 0<->1 one at a time on the current
base.  Best raw values: site 14 (`enemyFlagsLo` at `Radar.cpp:787`, 1->0) = **6,016**
(neutral, identical codegen); site 16 (`carriesFriendCargoFlag` at `:826`, 0->1) = 6,016
raw but aligned 1,993; site 11 (`isDockingToPlanet`, `:655`) = 6,018.  Every other site
is 6,030-9,569, and 16 of the 29 are size-fatal.  No flip is a raw win.

### 1.5 `campaign` / `missionBlocksPlanets` placement and types

| cell | raw | size | note |
|---|---:|---:|---|
| base | **6,016** | 9,908 | |
| `campaign` declared after `enabled` guard | 8,649 | **9,900** | fatal |
| `campaign` before `mission` | 8,614 | **9,900** | fatal |
| `campaign` after `mission` / after the bools | 9,183 | **9,900** | fatal |
| `campaign` `long` | 6,016 | 9,908 | neutral |
| `campaign` `unsigned int` | 6,017 | 9,908 | |
| `campaign` `char`/`short`/`uint8_t` | 8,448-8,493 | **9,912** | fatal |
| `missionBlocksPlanets` `char`/`short`/`uint8_t`/`int8_t`/`long`/`unsigned` | 6,016 | 9,908 | all neutral |
| `missionBlocksPlanets` `bool` | — | **9,912** | fatal (re-confirmed) |

### 1.6 Other single-knob cells (all neutral or worse)

- `bool alienOrbit` instead of `int`: 6,025 / 1,989 / 3,540 insn (size-exact, worse).
- `bool otherLock` / `bool notTurretMode` / `bool notOtherLock`: size-fatal (9,860/9,868/9,876).
- `otherLock` first computation rewritten as the equivalent
  `lockedAsteroid || lockedGasCloud || field_0x1c || field_0x14` (24 permutations):
  `ABCD` = 6,016 (identical codegen); all other permutations 6,017-8,493, 10 of them
  size-fatal (9,904/9,928).  The `bool otherLockFlag` if/else form compiles to the same
  bytes as the `||` chain.
- `otherLock` second computation (line 954) spelling variants (`!= nullptr` vs pointer
  truthiness, extra parens, `field_0x14` truthiness): all exactly 6,016 (identical code).
- Tractor guard 24 permutations: base `tractorMode2 && wreck && isDead && 1c==nullptr`
  = 6,016; `ACBD` = 6,016 raw / aligned 2,016; all others 6,017-8,822 (8 size-fatal).
- Loop index type (`uint32_t` -> `int`/`size_t`) on the planet/enemy/gas loops:
  6,016/6,016/6,017.
- `wreck` as `int`/`unsigned`/`short`/`unsigned char`: **9,912** fatal; guard reorder
  `(field_0x1b0 && !field_0x1a8) || (wreck && hasTractor13)`: **9,904** fatal.
- Landmark `offset` declaration order (`lmFont` first, `const`, if/else, `unsigned`):
  all exactly 6,016 (identical code).  Hoisting `offset` above the `if (i != 3)` block:
  6,054 / 1,987 / **3,534 insn** (size-exact, 7 fewer instructions but raw-worse);
  inlining the ternary at both uses: **9,920** fatal.
- `stations` hoisted to the top of `draw`: **9,800** fatal (the hoist lets clang fold the
  whole planet block); hoisted + onScreen-first: **9,900** fatal.
- Planet-loop `if (!onScreen) continue;` moved before `stationId`/`currentStation`
  (`pl_onfirst`): 6,017 / 1,937 (aligned -4, raw +1).
- 40 random 2-4-cell combinations of the near-neutral cells above: best 6,018.

### 1.7 Stack-slot analysis (the `mode` / `offset` swap)

`mode` (the third parameter) is spilled to **`sp+100` in the original** and to
**`sp+104` in ours**; the landmark-loop local `offset` (`i != 0 ? originY : originX`) is
spilled to `sp+104` in the original and `sp+100` in ours.  The two slots are swapped.
The swap shows up as single-byte immediate differences at every `mode` use
(`12e05e`, `12f85e`, `12febc`, `12ff9c`, `1303c2`; ours `12e05e`, `12f85e`, `12febc`,
`130054`, `130350`) and at every `offset` use.  It is a spill-slot allocation order
difference (PEI), not reproducible from any declaration-order / type / spelling change
tried (see 1.6); the frame is `sub sp,#296` in both and the set of live stack offsets is
identical (ours additionally shows `sp+20`, but that is the original's `stmia r3!,{...}`
outgoing-argument store, not a new slot).

## 2. Root cause of the residual (confirmed, unchanged)

The first divergence is `12e07a` `mov.w fp,#0` (orig) vs `mov.w sl,#0` (ours) — the
shared zero/loop-index vreg.  The original keeps `campaign` in **`sl`**
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and reuses `sl` for `stations`
(`12e3c6 mov sl,r0`, cold copy `12e3ce str.w sl,[sp,#116]`); ours spills `campaign`
to `sp+144` and `stations` to `sp+132` and gives `sl` to the zero/`i` vreg.  The
planet/enemy/gas tail is then shifted by 2-248 B, which is the bulk of the 6,016 raw
diff.  Every source spelling tried across waves 162-191 leaves the allocator's choice
unchanged; the `campaign`-in-`sl` bodies that do exist (w13319 `bsor`) are 771 B
raw-worse.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6016 aligned= 1941 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS (src unchanged)
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13520/`: `fast.py` / `buildvar.py` /
`batch.py` (1 s compile+link+fn_diff harness), `dump.py` (per-function disasm),
`extra.py` (extra-instruction attribution), `range.py` (per-region byte diff),
`musv.py` / `gasv.py` / `pbox.py` / `tractorp.py` / `olockp.py` / `expv.py` /
`combo.py` (cell sweeps), `o_orig.txt` / `o_ours.txt` (current disassembly).
