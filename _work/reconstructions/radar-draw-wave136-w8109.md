# Radar::draw wave 136 (w8109) — enemy-tail block split: exact layout, OR-operand order, and the branch-polarity dead end (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w8109`, based on `master`
(`2e51d765d`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.354839 %** (official 75.4). Gate at start and end:
`RATCHET PASS`, byte **4506**, linked **4506**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave135-w7774.md`, `radar-draw-wave134-w7753.md`,
`radar-draw-wave133-w7732.md`, `radar-draw-wave131-w7712.md`.

This wave attacked the assignment's NEW hypothesis directly: **the enemy-tail
block split** (the one remaining large block-order delta). It reconstructs the
exact original block layout from a fresh Ghidra 12.1.2 decompile + Thumb
disassembly, and adds four pieces of evidence the earlier notes did not have:

1. the original's enemy-tail condition is **contiguous** (cond -> cont ->
   navWreck body -> clear -> recompute), and the cond block materialises **all
   three loop-exit booleans** (`bossSeen`, `alwaysEnemySeen`, `enemyHit`)
   before the branch;
2. the inner `||` operand order is a **real source difference**: the original
   evaluates `isDockingToDockingPoint() | turretMode | otherLock`, ours
   evaluates `(otherLock | turretMode) | isDockingToDockingPoint()`;
3. the source spelling `!(B || other || turret)` (call it `B_o_t`) reproduces
   the original's operand order **and moves the cont+navWreck blocks inline**
   (12f3ea / 12f408), but re-rolls the rest of the function to 9876 B /
   69.912394 %;
4. the remaining branch-polarity delta (`br i1 %a, label %skip, label %cont`
   vs the original's `br i1 %nota, label %cont, label %skip`) is **not
   reachable with a `bool` return**, and making `isDockingToAsteroid()` return
   `int` (which does flip the IR successor order) is a 482-function
   `byte_exact` regression because the loop-exit heuristic then weights the
   branch 37.5/62.5 and MBP still picks skip.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w8109
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~0.9 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Ghidra decompile (provider reachable with
`GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC`):

```sh
rea decompile /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure 0x13e050 --format md > ghidra_draw.md
```

Scratch in `/dev/shm/w8109` (not committed): `Radar.base.cpp` (pristine
source), `score.py` (single-symbol difflib harness reproducing 75.354839
exactly), `drv.py`/`batch*.py` (variant drivers), `orig.dis`/`ours.dis`
(full Thumb objdump), `ghidra_draw.md` (2019-line decompile), `Radar.ll` /
`mir.txt` (clang IR and `-mllvm -print-machineinstrs` MIR).

## 2. Exact enemy-tail block layout (original, `objdump -d`)

```
12f3a6  ldr r0,[r4,#0]                 ; cond
12f3a8  blx Level::getPlayer
12f3ac  blx PlayerEgo::isDockingToAsteroid
12f3b0  and.w r1,r6,#1                 ; bossSeen & 1
12f3b4  str   r1,[sp,#92]              ;   -> sp#92  (bossSeen)
12f3b6  ldr   r1,[sp,#80]              ; alwaysEnemy
12f3b8  cmp   r0,#0
12f3ba  and.w r1,r1,#1
12f3be  str   r1,[sp,#104]             ;   -> sp#104 (alwaysEnemySeen)
12f3c0  ldr   r1,[sp,#88]              ; enemyHit
12f3c2  and.w r1,r1,#1
12f3c6  str   r1,[sp,#132]             ;   -> sp#132 (enemyHit)
12f3c8  bne.n 12f40e                   ; A != 0 -> skip (recompute)
12f3ca  ldr   r0,[r4,#0]               ; cont
12f3cc  blx   Level::getPlayer
12f3d0  blx   PlayerEgo::isDockingToDockingPoint
12f3d4  ldr   r1,[sp,#148]             ; turretMode
12f3d6  orrs  r0,r1                    ; r0 = B | turret
12f3d8  ldr   r1,[sp,#144]             ; otherLock
12f3da  orrs  r0,r1                    ; r0 = (B | turret) | other
12f3dc  bne.n 12f40e                   ; any -> skip (recompute)
12f3de  ldr   r0,[r4,#8]               ; body: navWreck = dockNavPtr != 0 && ...
12f3e0  cmp   r0,#0
12f3e2  beq.w 12f91a                  ; navWreck = false
12f3e6  ldrb.w r1,[r0,#76]
12f3ea  cmp   r1,#0
12f3ec  beq.w 12f91a
12f3f0  blx   KIPlayer::isDead
12f3f4  cmp   r0,#0
12f3f6  beq.w 12fe6c                  ; isDying
12f3fa  mov.w r8,#1                    ; navWreck = true
12f3fe  b.w   12fe74                  ; -> rest of the body
12f402  movs  r0,#0                    ; clear: enemyHit/alwaysEnemySeen/bossSeen = 0
12f404  str   r0,[sp,#132]
12f406  movs  r0,#0
12f408  str   r0,[sp,#104]
12f40a  movs  r0,#0
12f40c  str   r0,[sp,#92]
12f40e  ldr   r0,[r4,#8]               ; recompute #2
...
```

Layout: **cond -> cont -> navWreck body -> clear -> recompute -> gas**.
The `clear` block is the `enemyHit = false` tail of the
`if (scannerAvailable && enemies != nullptr)` merge, reached from
`12ec4e beq.w 12f402` / `12ec5c beq.w 12f406`.

## 3. Ours (baseline)

```
12f3cc  ldr r0,[r4,#0]                 ; cond
12f3ce  blx Level::getPlayer
12f3d2  ldr r1,[sp,#80]                ; alwaysEnemy
12f3d4  and.w r1,r1,#1
12f3d8  str r1,[sp,#92]                ;   -> sp#92 (alwaysEnemySeen)
12f3da  blx PlayerEgo::isDockingToAsteroid
12f3de  cmp r0,#0
12f3e0  beq.w 12f888                   ; A == 0 -> cont (outlined)
12f3e4  ldr r5,[sp,#88]                ; skip/neg
12f3e6  ldr r0,[sp,#108]               ; bossSeen
12f3e8  and.w r0,r0,#1
12f3ec  str r0,[sp,#108]
12f3ee  and.w r0,r5,#1                 ; enemyHit
12f3f2  str r0,[sp,#132]
12f3f4  b.n 12f402
12f3f6  movs r0,#0                     ; clear
...
12f402  ldr r0,[r4,#8]                 ; recompute #2
...
12f888  ldr r2,[sp,#144]               ; cont (outlined after the gas block)
12f88a  cmp r2,#0
12f88c  it ne
12f88e  movne r2,#1
12f890  ldr r0,[r4,#0]
12f892  ldr r1,[sp,#152]               ; turretMode
12f894  orr.w r5,r1,r2                 ; turret | other
12f898  blx Level::getPlayer
12f89c  blx PlayerEgo::isDockingToDockingPoint
12f8a0  cmp r5,#0
12f8a2  bne.w 12f3e4
12f8a6  ldr r5,[sp,#88]                ; enemyHit
12f8a8  cmp r0,#0
12f8aa  bne.w 12f3e6
12f8ae  ldr r0,[r4,#8]                 ; body: navWreck
...
12f8ca  b.n 12fe74
```

Layout: **cond -> skip/neg -> clear -> recompute -> gas -> cont -> body**.
Only `alwaysEnemySeen` is stored in cond; `bossSeen` and `enemyHit` are sunk
to the skip block. The branch is `beq.w 12f888` (branch to cont), i.e. the
**skip path is the fall-through**.

## 4. MBP mechanism (MIR evidence)

The clang IR branch is, in both binaries' source spelling:

```
%a = call zeroext i1 @isDockingToAsteroid()
br i1 %a, label %skip, label %cont
```

so the successor list is `[skip, cont]`. The final MIR for the baseline is

```
bb.221:
  successors: %bb.696(0x40000000), %bb.325(0x40000000); %bb.696(50.00%), %bb.325(50.00%)
  ...
  tCMPri r0, 0
  t2Bcc %bb.325, 0, killed $cpsr      ; branch to cont on EQ; fall-through skip
```

On a 50/50 tie `MachineBlockPlacement::selectBestSuccessor` takes the first
successor, so skip becomes the fall-through. The original instead has cont as
the fall-through, which requires either the successor list `[cont, skip]`
(branch on `%nota`) or a hotter cont edge.

## 5. New result: the `||` operand order is a real source difference

The original's cont block evaluates `B`, then `turretMode`, then `otherLock`
(`orrs` chain). The baseline's IR is the reverse:

```
; baseline
%1610 = icmp ne i32 %1084, 0      ; turretMode
%1611 = or i1 %64, %1610          ; otherLock | turretMode
%1612 = or i1 %1611, %1609        ; | isDockingToDockingPoint
```

Only the source spelling `!(isDockingToDockingPoint() || otherLock || turretMode)`
(`B_o_t`) produces the original's evaluation order:

```
; B_o_t
%1610 = icmp ne i32 %1084, 0      ; turretMode
%1611 = or i1 %1610, %1609        ; turretMode | isDockingToDockingPoint
%1612 = or i1 %64, %1611          ; otherLock | (...)
```

and it **does** move the cont + navWreck blocks inline (cont at `12f3ea`,
body at `12f408`), but the rest of the function re-rolls to **9876 B /
69.912394 %**.

Full permutation matrix (single edit on the pristine baseline; all six
`||` operand orders):

| variant | source order | usize | raw % |
|---|---|---:|---:|
| **baseline** | `B \|\| turret \|\| other` | 9908 | **75.354839** |
| `B_o_t` | `B \|\| other \|\| turret` | 9876 | 69.912394 |
| `t_B_o` | `turret \|\| B \|\| other` | 9888 | 68.705139 |
| `t_o_B` | `turret \|\| other \|\| B` | 9880 | 69.596438 |
| `o_B_t` | `other \|\| B \|\| turret` | 9868 | 69.646450 |
| `o_t_B` | `other \|\| turret \|\| B` | 9872 | 69.415314 |

So the OR order is a genuine lever for the enemy-tail block split, but the
baseline's (wrong) order is a strong local optimum for the rest of the
function. This is the same coupled fixed point as waves 131-135, now with the
enemy tail pinned to the OR order.

## 6. New result: branch polarity is not reachable with `bool`

Every `bool`-returning spelling tested canonicalises to
`br i1 %a, label %skip, label %cont`:

| spelling | IR branch |
|---|---|
| `!A` (baseline) | `br i1 %1160, label %1808, label %1606` |
| `A == 0` | same |
| `A == false` | same |
| `A != true` | same |
| `!(A != 0)` | same |
| `A < 1` | same |
| `!!(!A)` | same |
| `int docking = A; docking == 0` | same |
| `bool docking = A; !docking` | same |
| `!(A \|\| B \|\| ...)` (orchain) | same |
| `!(B \|\| turret \|\| other)` (andchain) | same |
| empty-then / else | same |

Changing the *declared return type* of `PlayerEgo::isDockingToAsteroid()` from
`bool` to `int` does produce the desired successor order:

```
%1162 = call i32 @isDockingToAsteroid
%1163 = icmp eq i32 %1162, 0
br i1 %1163, label %cont, label %skip      ; [cont, skip]
```

but the loop-exit heuristic then weights the branch 37.5 % / 62.5 %, so MBP
still picks skip as the fall-through, and the function is 9904 B / 71.924290 %.
Worse, the `int` return type is a **482-function `byte_exact` regression**
(4506 -> 4024) because every caller's `i1` use becomes an `icmp ne i32, 0`.
The original's `isDockingToAsteroid` body is byte-identical to our `bool`
version (`22 B`, 9/9 insns), so the return type is `bool` (or an equivalent
1-byte type).

## 7. Why the original's cond block stores all three booleans

The original's body path reads `enemyHit` from the stack at `12fe90`
(`ldr r0,[sp,#132]; cbnz r0, 12feb4`), so the cond block must materialise
`sp#132` before branching. Ours keeps `enemyHit` in `r5` on the body path
(`12f8a6 ldr r5,[sp,#88]` then `12fe90 lsls r0,r5,#31`), so clang sinks the
`sp#132` store to the skip path. This is the same live-range/register-
allocation fixed point described in waves 133-135, now with the exact
`sp#92/sp#104/sp#132` mapping:

| variable | original | ours |
|---|---|---|
| `bossSeen` | `sp#92` | `sp#108` |
| `alwaysEnemySeen` | `sp#104` | `sp#92` |
| `enemyHit` | `sp#132` | `sp#88` (loop) / `sp#132` (normalised) |

## 8. Other new sweeps (all ties or worse)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | unmodified | 9908 | **75.354839** |
| `ae_before_loop` | `alwaysEnemySeen = alwaysEnemy;` before the loop | 9908 | 75.354839 (tie) |
| `ae_after` | move the assignment after the tail `if` | 9900 | 73.160235 |
| `exp_a1` | `__builtin_expect(!A, 1)` | 9872 | 62.818483 |
| `exp_a0` | `__builtin_expect(!A, 0)` | 9944 | 59.394632 |
| `exp_whole1` | `__builtin_expect(!A && !(...), 1)` | 9892 | 68.390062 |
| `exp_inner1` | inner `__builtin_expect(!(...), 1)` | 9876 | 62.992806 |
| `bot_exp1` | `B_o_t` + `__builtin_expect(!A, 1)` | 9876 | 63.097294 |
| `bot_expa0` | `B_o_t` + `__builtin_expect(A, 0)` | 9936 | 59.354470 |
| `int_ret` | `int isDockingToAsteroid()` | 9904 | 71.924290 |
| `bot_camp` | `B_o_t` + faithful `camp_reload` | 9868 | 69.511494 |
| `bot_norecomp` | `B_o_t` + remove post-loop recompute | 9824 | 67.195157 |

## 9. Root cause (unchanged, now with the enemy-tail pin)

The residual is the coupled register-allocation / block-placement fixed point:

1. the first `otherLock` guard's `cbz`+`itt` shape (ours) vs the original's
   `cbnz` chain, and the first literal pool at `12e310` vs `12e340`;
2. the invented post-loop `onPlanetCourse` recompute at `12e72c`;
3. the enemy-tail block split: the original's `||` order is
   `B | turret | other` (source `!(B || other || turret)`), and its cond block
   stores all three loop-exit booleans; ours is `other | turret | B` and sinks
   two of the stores to the skip block;
4. `stations` in `r6` instead of `sl`+spill, and the `turretMode` slot swap;
5. the bottom-placed landmark loop header (`12eb48` vs `12e7d4`);
6. the ship `inSight` `s24`/`s26` live range (`vpush {d8-d13}` vs `{d8-d11}`).

The new evidence pins the enemy-tail source shape (OR order + all-three-bool
stores) but shows that fixing it alone re-rolls the rest of the function by
5.4 points. The remaining work is to find the compensating planet-block /
`stations` changes that coexist with the `B_o_t` order.

## 10. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` 84,960 differing bytes (size/base
exact), `.ARM.extab` 1,404 (size exact 156,312), `.note.gnu.build-id` 20.
`.rodata`, `.data`, `.bss`, `.got`, `.ARM.exidx`, `.rel.plt`, `.rel.dyn`,
`.dynsym`, `.dynstr`, `.hash` are byte-identical.
