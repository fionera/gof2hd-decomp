# Radar::draw wave 175 (w11906) — skip-branch `shipGroup == 10` re-test + `bool wreck`

**Status:** landed, 2026-11-11. Worktree `wt/w11906`, branch `wave175-w11906`,
base `master` `5f599a137`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave174-w11908.md` (lock-image friend-first + far-plain
else-first), `radar-draw-wave174-w11909.md` (frame-path blip locals +
planet-lock expect), and `radar-draw-wave173-w11807.md` (image-chain /
exclude-flag / shipGroup re-sweep).

## 0. Baseline (rebuilt on `5f599a137`)

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | 9,908 |
| raw same-address diff | 6,817 | **6,804** |
| instruction-index alignment | 1,602 | **1,590** |
| instruction count | 3,536 ours / 3,532 orig | 3,535 / 3,532 |
| `.text` differing bytes | 80,439 | **80,426** |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 750 | 750 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |
| `.rodata` / `.data` / `.bss` | 0 / 0 / 0 | 0 / 0 / 0 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (one commit, two sites, net size 0)

```diff
                 if (enemy->shipGroup == 10 && enemy->player->isActive() && !enemy->isDead() && !enemy->isDying()) {
                     this->field_0x1bc++;
-                    bossSeen = true;
+                    bossSeen = enemy->shipGroup == 10;
                 }
                 continue;
             }
             this->update(enemy);
-            int wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
+            bool wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
```

### 1.1 Why the re-test is the right shape

The original re-loads `shipGroup` after the three calls and re-tests it:

```
12ed48  ldr.w r0, [r4, #444]   ; field_0x1bc++
12ed4c  adds  r0, #1
12ed4e  str.w r0, [r4, #444]
12ed52  ldr.w r0, [r8, #40]    ; enemy->shipGroup
12ed56  cmp   r0, #10
12ed58  it    eq
12ed5a  moveq r6, #1           ; bossSeen
12ed5c  b     12f184
```

The base emitted only `movs r5,#1` (ours uses `r5` for `bossSeen`).  The
re-test exists because the source reads `enemy->shipGroup` again after the
`isActive` / `isDead` / `isDying` calls; those calls can clobber the field as
far as the compiler knows, so the load is not CSE'd.  Writing the assignment as
`bossSeen = enemy->shipGroup == 10;` produces exactly that reload.  Our
codegen then folds `(x == 10)` to `subs/clz/lsrs` rather than `cmp/it/moveq`,
but the byte count is the same (10 B) and the semantics are identical:

```
12ed16  ldr.w r0, [r8, #40]
12ed1a  subs  r0, #10
12ed1c  clz   r0, r0
12ed20  lsrs  r5, r0, #5       ; bossSeen = (shipGroup == 10)
12ed22  b     12f140
```

The `bossSeen` value is only consumed as a boolean later, so the `clz` form is
equivalent to the original's `cmp/it/moveq`.  `skip_nested`
(`if (enemy->shipGroup == 10) bossSeen = true;`) gives the `cmp/it/moveq`
shape but measures raw 6,818 with the same compensator, i.e. +1 vs base; the
`expr` form is the raw winner.

### 1.2 Why `bool wreck` is the compensator

`wreck` is used only in boolean contexts in `draw`:

* `if (wreck)` (on-screen image),
* `!(enemy->countsAsEnemyExcludeFlag != 0 || wreck)` (enemy count),
* `(!wreck || enemy->isDead())` (tractor gate),
* `(wreck && this->hasTractor13)` (tractor),
* `this->tractorMode2 && ... && wreck && ...` (tractor tail).

The original's `wreck` is a boolean; declaring it `bool` is semantically
correct and is exactly -8 B.  The skip re-test is +8 B, so the pair is
size-neutral.  Measured alone, `bool wreck` is 9,900 B / raw 8,061 and the
skip re-test is 9,916 B / raw 8,546; only the pair is size-exact **and**
raw-improving (6,804 / 1,590 / 9,908).  The interaction is a layout effect:
the two edits together restore the original's stack-slot / block layout in the
enemy tail better than either alone.

## 2. Sweeps run (all size-exact cells only)

Base for every cell is 6,817 / 1,602 / 9,908.  All builds were `-j2` under
`ulimit -v 12000000`.  Scratch: `/home/debian/GalaxyOnFire2/scratch/w11906/`
(`eval.py`, `edits.py`, `sweep*.py`, `search_combo.py`, `search_round2.py`,
`aldiff.py`, `blockdelta.py`, `rangediff.py`, `secdiff.py`).

### 2.1 Image-chain permutations (all 6 source orders per chain)

| chain | size-exact raw values |
|---|---|
| lock (0x5c/0x64/0x74) | FEP 6,817 (base), FPE 6,817, EFP 6,819, EPF 6,819, PEF 6,819; PFE 8,679 (9,896) |
| far plain (0x60/0x68/0x78) | base PFE-with-expect 6,817; EFP/EPF/PEF 6,831; PFE-plain 6,832; FEP/FPE 8,412 (9,912) |
| far locked (0x7c/0x80/0x84) | EFP/EPF/PEF 6,817; FEP/FPE 8,417 (9,912); PFE 8,075 (9,900) |
| frame (0xe0/0xe4/0xe8 + bars) | EFP/EPF/PEF 6,817; FEP/FPE 6,817 (1,613); PFE 8,418 (9,912) |
| off-screen (0x90/0x94/0x98) | all plain orders 9,912 (size-fatal); base expect(...,0) 6,817 |

No single chain permutation beats the base raw.  The original's branch shapes
are enemy-first for the lock, far-plain and far-locked chains, but the base's
friend-first/else-first source already produces the same *semantics* and the
raw metric is dominated by the global allocation shift (see §3).

### 2.2 `countsAsEnemyExcludeFlag`, `shipGroup == 9/10` spellings

All neutral (6,817 / 1,602 / 9,908): `!flag && !wreck`,
`flag == 0 && !wreck`, `!(flag || wreck)`, `stealFlagByte == 0`,
`(flag == 0 || wreck)` in the tractor gate, `flag != 0` in the wreck
off-screen chain, `shipGroup == 9u`, `shipGroup >= 9`, `shipGroup > 8`.
The later-block `bossSeen = enemy->shipGroup == 10;` is size-fatal (9,884);
`bossSeen |= ...` is 9,924.  `sg9_ge` / `sg9_gt8` keep raw 6,804 with the
landed pair but are not semantically the original `== 9`, so the base spelling
is kept.

### 2.3 Compensator search

A one-structural + up-to-two-compensator search over
`lock_EFP`, `far_EFP`, `off_plain`, `skip_expr`, `skip_nested` and
`P2off`, `planetexp`, `gasint`, `wreckbool`, `mbpbool`, `markedint`,
`S1off`, `bossint` found exactly one size-exact raw winner:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,817 | 1,602 | 9,908 |
| **`skip_expr + wreckbool` (landed)** | **6,804** | **1,590** | **9,908** |
| `skip_expr + wreckbool + lock_EFP` | 6,806 | 1,586 | 9,908 |
| `skip_expr + wreckbool + far_EFP` | 6,825 | 1,560 | 9,908 |
| `skip_nested + wreckbool` | 6,818 | 1,591 | 9,908 |
| `off_plain + wreckbool` | 7,010 | 1,537 | 9,908 |

The landed pair is the only cell that improves both raw and aligned.

## 3. Remaining residual

Unchanged from wave 173/174: the original keeps `campaign` in `sl`
(`12e0f8: mov sl,r0`, `12e38a: cmp.w sl,#2`) and `missionBlocksPlanets` in
`fp`; ours spills `campaign` to `sp+144` and uses `sl` for the shared zero.
The enemy loop therefore starts 44 B earlier in ours (orig `12ec1c`, ours
`12ebf0`), and the image-pointer stack slots are permuted
(`0x5c`/`0x64` at `sp+28`/`sp+40`; `0x60`/`0x68`/`0x78` at
`sp+64`/`sp+76`/`sp+72` vs the original's `sp+76`/`sp+68`/`sp+60`).  The
off-screen chain is also ~14 B larger in ours because the enemy arm is
out-of-line and the register shuffles (`mov fp,r5; mov r5,r9; mov r9,r6`)
are needed; matching it requires the campaign/`sl` allocation fix, which is
still the next lever.  The lock-image chain's branch order now matches the
original's enemy-first shape in the `lock_EFP` cell, but that cell is raw +2
on its own (6,819) because the `Globals::Canvas` load is scheduled after the
chain in ours and before it in the original.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /home/debian/GalaxyOnFire2/scratch/w11906/
#   eval.py, edits.py, sweep1.py, sweep2.py, sweep3.py, sweep_far.py,
#   sweep_skip.py, sweep_skip2.py, sweep_spell.py, sweep_comp.py,
#   measure_edits.py, search_combo.py, search_round2.py, sweep_round3.py,
#   aldiff.py, blockdelta.py, rangediff.py, secdiff.py,
#   Radar.base.cpp, Radar.win.cpp, base.so
```
