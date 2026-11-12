# Radar::draw wave 191b (w13535) — lockedEnemy isDying/isDead order: raw 6,016 -> 6,015

**Status:** landed, 2026-11-12. Worktree `wt/w13535`, branch `wave191b-w13535`,
base `master` `7ace220f3`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191b-w13520.md` (base 6,016), `-w13510.md` (station-blip /
24000 / tractor cells), `-w13507.md` (statement/operand sweeps) and
`radar-draw-wave191-w13488.md` (music predicate / planet-loop order).

## 0. The change

In the `enemies:` block (`Radar.cpp:874`), the `lockedEnemy` dead test was written
`isDead() || isDying()`; the original evaluates `isDying()` first:

```cpp
-            if (this->lockedEnemy != nullptr && (this->lockedEnemy->isDead() || this->lockedEnemy->isDying())) {
+            if (this->lockedEnemy != nullptr && (this->lockedEnemy->isDying() || this->lockedEnemy->isDead())) {
```

The original's `12fe7a`-`12fe88` sequence is `isDying` -> `isDead`; after the swap
ours reproduces that instruction sequence exactly (modulo the -12 B block shift):

```
ours after:  12fe6e blx isDying ; cbnz 12fe7e ; ldr [sp,#128] ; ldr [r0] ; blx isDead ; cbz 12fe84
orig:        12fe7a blx isDying ; cbnz 12fe8a ; ldr [sp,#128] ; ldr [r0] ; blx isDead ; cbz 12fe90
```

Note the neighbouring `navWreck` test (`Radar.cpp:873`, `dockNavPtr->isDead() ||
isDying()`) is already in the original's `isDead`-first order and must stay that
way; swapping it is raw-neutral on its own (6,016) and does not add to this win.

## 1. Measured result

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,016 | **6,015 (-1)** |
| instruction-index alignment | 1,941 | 1,941 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 | 3,541 / 3,532 |
| `.text` differing bytes (whole `.so`) | 71,506 | **71,505 (-1)** |
| `.ARM.extab` differing bytes | 586 | 586 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.data` differing bytes | 2 | 2 |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 2. Negative cells measured this pass (all reverted; base 6,016 / 1,941 / 9,908)

The assigned lenses were re-measured on the current base; none beats it.

### 2.1 gas-cloud 24000 chain — ego-first x-y-z (not previously tried)

w13520 tested target-first `targetPos - egoPos` in x-y-z / x-z-y orders.  The
original actually computes **ego-first, x-y-z** (`12f64c` `vsub s0,s2,s0` with
`r5=&targetPos`, `fp=&egoPos`), so all 24 (axis order x pair order x operand
order) forms were swept.  Ego-first x-y-z is structurally right but raw-worse:

| cell | raw | aligned |
|---|---:|---:|
| base (target-first x-z-y) | **6,016** | 1,941 |
| ego-first x-y-z | 6,021 | 1,941 |
| ego-first x-y-z + inline `this->cameraZ` (s16/s18 constants, s6 reload) | 6,135 | 1,934 |
| ego-first x-y-z + `isDead`-first gas guard | 6,022 | 1,941 |
| all 72 enemy/tractor/gas axis×pair×operand permutations | 6,016+ | — |

The ego-first x-y-z form does reproduce the original's `s16/s18` constant pair and
the `s6` cameraZ reload, but the gas-loop register allocation re-rolls and costs
119 raw bytes; not landable.

### 2.2 gas `inSight` box `+` form (re-measured)

| cell | raw | aligned |
|---|---:|---:|
| base (`-` form) | **6,016** | 1,941 |
| `screenX < left + size*2` / `screenY < top + size*2` | 6,020 | 1,937 |
| x half only | 6,017 | 1,937 |
| y half only | 6,019 | 1,941 |

### 2.3 planet box — all 24 permutations

`pbox_0123` (base) and `pbox_1023` are the only 6,016 cells; `2301`/`2310`/
`3201`/`3210` are 6,017 (aligned 1,930-1,933); the rest are 6,018+ or size-fatal.
Base is optimal.

### 2.4 music-tail predicate order

`inAlienOrbit() || isAttackedByAliens() || campaign==16` (the original's evaluation
order at `1302ac`-`1302c6`) is raw-worse: 6,031 / aligned 1,936.  The base
`isAttackedByAliens() || campaign==16 || inAlienOrbit()` stays.

### 2.5 `navWreck || dockNavPtr->field_0x24` hoisting

Hoisting the repeated `||` into a local is size-fatal (9,832); `__builtin_expect`
on it is 6,656-8,232; an explicit if/else `lockLimit` is exactly 6,016.

### 2.6 `campaign` / `missionBlocksPlanets` colouring (root cause, unchanged)

Re-confirmed with the MIR dump (`-mllvm -print-machineinstrs`): greedy spills
`campaign` to `%stack.45` immediately (`t2STRi12 $r0, %stack.45`) while the original
keeps it in `sl`.  Declaration/definition reordering, `bool`/`unsigned`/`long`
types, duplicate uses (`campaign >= 2 && campaign > 1`), and uninitialised+else
forms are all neutral or size-fatal (6,016 / 9,908, or 9,900/9,912).  The
`campaign`-in-`sl` bodies that exist are 700+ raw-worse.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6015 aligned= 1941 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13535/`: `try.py` (1 s
compile+link+fn_diff harness), `sweep_radar.py` (72-chain sweep), `v*.json` cell
definitions, `o_orig.txt` / `o_ours.txt` disassembly, `mi.txt` machine-instr dump.
