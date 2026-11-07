# Radar::draw wave 137 (w8211) — exact enemy `inSight` order + enemy-tail split from Thumb (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w8211`, based on `master`
(`655e3bcc5`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.354839 %** (official 75.4). Gate at start and end:
`RATCHET PASS`, byte **4507**, linked **4507**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave136-w8109.md`, `radar-draw-wave136-w8110.md`,
`radar-draw-wave135-w7774.md`, `radar-draw-wave134-w7753.md`.

This wave attacked the assignment's NEW hypothesis directly: reconstruct the
**exact `inSight` condition order** and the **enemy-tail split** from Thumb, and
combine them with the w8109 OR-order lever (`!(B || other || turret)`). It adds
five pieces of evidence the earlier notes did not have:

1. the original's `inSight` condition is **length-first**, then
   `screenX > cx-f`, `screenX < cx+f`, `screenY > cy-f`, `screenY < cy+f`
   (i.e. `x>, x<, y>, y<`), confirmed instruction-by-instruction;
2. the original's enemy-tail cond block materialises **all three** loop-exit
   booleans in the order `bossSeen -> alwaysEnemySeen -> enemyHit` (sp#92,
   sp#104, sp#132), and the body path reads `enemyHit` from sp#132;
3. the `inSight` fix in isolation makes the `inSight` region nearly identical
   (the 56-instruction residual collapses) but **re-rolls the enemy tail into a
   161-instruction duplicated block**, dropping the whole-function ratio to
   68.8 % and shrinking the function to 9896 B;
4. a nested `inSight` spelling (`if (length) inSight = screen...;`) reproduces
   the original's *control flow* exactly but puts the bool in `fp` (4-byte
   `mov.w`/`movlt.w` instead of 2-byte `movs`/`movlt`), so it is 9924 B alone
   and 9908 B only when combined with the OR lever (67.65 %);
5. a 24-cell grid (inSight order x OR order x `alwaysEnemySeen` placement x
   `enemyHit` type) finds **no** cell above the 75.354839 baseline.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w8211
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~0.8 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch in `/dev/shm/w8211` (not committed): `Radar.base.cpp` (pristine
source), `score.py` (single-symbol scorer importing `tools/verify/verify.py`,
reproduces 75.354839 exactly), `blocks.py` (difflib block map),
`diffblocks.py`, `mkvar*.py`/`batch*.py`/`grid.py` (variant drivers),
`orig_insight.dis` / `orig_full.dis` (Thumb objdump with addresses).

Section deltas (baseline, unchanged): `.text` **84,709** differing bytes,
`.ARM.extab` **1,395**, `.rel.plt` **4**, `.note.gnu.build-id` **20**; every
section size and base is exact.

## 2. Exact original `inSight` block (Thumb, `0x12ef4c..0x12efc0`)

This is the offscreen-draw `else` branch of the enemy loop (source lines
~789-803). The original evaluates the **vector length first**, then the four
screen checks in `x>, x<, y>, y<` order:

```
12ef4c ldr r0,[pc,#688]        ; &PlayerEgo::crosshairPos
12ef4e movs r6,#0
12ef50 strb.w r6,[r8,#115]     ; enemy->field_0x73 = 0
12ef54 add r5,sp,#232          ; &tmpVector          (r5 = vector ptr)
12ef56 add r0,pc
12ef58 ldr r0,[r0,#0]
12ef5a vldr s26,[r0]           ; crosshairPos.x
12ef5e vldr s24,[r0,#4]        ; crosshairPos.y
12ef62 mov r0,r5
12ef64 ldrd r2,r1,[sp,#108]    ; egoPos, targetPos
12ef68 blx AEMath::mi          ; tmp = egoPos - targetPos
12ef6c mov r0,r5
12ef6e vcvt.s32.f32 s24,s24    ; cy = (int)crosshairPos.y
12ef72 vcvt.s32.f32 s26,s26    ; cx = (int)crosshairPos.x
12ef76 blx AEMath::VectorLength
12ef7a vmov s0,r0
12ef7e ldr r5,[sp,#116]        ; locked (reload; r5 reused)
12ef80 vcmpe.f32 s0,s20        ; length < 60000.0f?
12ef84 vmrs APSR_nzcv,fpscr
12ef88 bpl.n 12efc0            ; no -> store false
12ef8a vmov r1,s26             ; cx
12ef8e ldr.w r0,[r4,#292]      ; f = field_0x124
12ef92 ldr.w r2,[r4,#252]      ; screenX
12ef96 movs r6,#0              ; inSight = false (2 B)
12ef98 subs r3,r1,r0           ; cx - f
12ef9a cmp r2,r3
12ef9c ble.n 12efc0            ; screenX > cx-f
12ef9e add r1,r0               ; cx + f
12efa0 cmp r2,r1
12efa2 bge.n 12efc0            ; screenX < cx+f
12efa4 vmov r1,s24             ; cy
12efa8 ldr r2,[r4,#256]        ; screenY
12efac subs r3,r1,r0           ; cy - f
12efae cmp r2,r3
12efb0 ble.n 12efbe            ; screenY > cy-f
12efb2 add r0,r1               ; cy + f
12efb4 movs r6,#0
12efb6 cmp r2,r0
12efb8 it lt
12efba movlt r6,#1             ; screenY < cy+f
12efbc b.n 12efc0
12efbe movs r6,#0
12efc0 strb.w r6,[r8,#115]     ; enemy->field_0x73 = inSight
```

The corresponding source is a flat 5-term `&&` chain, **length first**:

```cpp
enemy->field_0x73 = 0;
int cx = (int) PlayerEgo::crosshairPos.x;
int cy = (int) PlayerEgo::crosshairPos.y;
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
    this->screenX > cx - this->field_0x124 && this->screenX < cx + this->field_0x124 &&
    this->screenY > cy - this->field_0x124 && this->screenY < cy + this->field_0x124) {
    inSight = true;
}
enemy->field_0x73 = inSight;
```

Our baseline has the same five terms but **screen-first** and in the order
`x>, y>, x<, y<`, with the length last:

```cpp
if (this->screenX > cx - f && this->screenY > cy - f &&
    this->screenX < cx + f && this->screenY < cy + f &&
    AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f) ...
```

So both the length position and the screen order are wrong in the baseline.
The reconstructed `length-first, x>,x<,y>,y<` spelling is `ins_lenfirst`
below; it makes the `inSight` block match the original almost instruction for
instruction (only the bool/vector register numbers differ: original
`r5=vector, r6=bool`; ours `r6=vector, r0=bool`).

## 3. Exact original enemy-tail split (Thumb, `0x12f3a6..0x12f40e`)

```
12f3a6 ldr r0,[r4,#0]          ; this->level
12f3a8 blx Level::getPlayer
12f3ac blx PlayerEgo::isDockingToAsteroid   ; A
12f3b0 and.w r1,r6,#1          ; bossSeen
12f3b4 str r1,[sp,#92]         ;   -> sp#92
12f3b6 ldr r1,[sp,#80]         ; alwaysEnemy
12f3b8 cmp r0,#0               ; A
12f3ba and.w r1,r1,#1
12f3be str r1,[sp,#104]        ;   -> sp#104 (alwaysEnemySeen)
12f3c0 ldr r1,[sp,#88]         ; enemyHit
12f3c2 and.w r1,r1,#1
12f3c6 str r1,[sp,#132]        ;   -> sp#132
12f3c8 bne.n 12f40e            ; A != 0 -> skip
12f3ca ldr r0,[r4,#0]          ; cont
12f3cc blx Level::getPlayer
12f3d0 blx PlayerEgo::isDockingToDockingPoint   ; B
12f3d4 ldr r1,[sp,#148]        ; turretMode
12f3d6 orrs r0,r1              ; B | turret
12f3d8 ldr r1,[sp,#144]        ; otherLock
12f3da orrs r0,r1              ; (B | turret) | other
12f3dc bne.n 12f40e            ; any -> skip
12f3de ldr r0,[r4,#8]          ; body: navWreck = dockNavPtr != null && ...
12f3e0 cmp r0,#0
12f3e2 beq.w 12f91a
...
12f402 movs r0,#0              ; clear (scanner/enemies merge)
12f404 str r0,[sp,#132]
12f406 movs r0,#0
12f408 str r0,[sp,#104]
12f40a movs r0,#0
12f40c str r0,[sp,#92]
12f40e ldr r0,[r4,#8]          ; recompute
```

Facts pinned by this listing:

* the cond block materialises **all three** loop-exit booleans before the
  branch, in the order `bossSeen (sp#92) -> alwaysEnemySeen (sp#104) ->
  enemyHit (sp#132)`;
* the body path reads `enemyHit` from sp#132 (`12fe90 ldr r0,[sp,#132];
  cbnz r0,12feb4`), so the spill is required, not dead;
* the inner `||` machine order is `B | turret | other`, which corresponds to
  the source spelling `!(B || otherLock || turretMode)` (w8109's `B_o_t`);
* the block layout is `cond -> cont -> navWreck body -> clear -> recompute ->
  gas`, i.e. the cont and navWreck blocks are **inline** right after the cond.

Our baseline instead lays out `cond -> skip/neg -> clear -> recompute -> gas ->
cont -> body`: the cont block is outlined after the gas block, and only
`alwaysEnemySeen` is stored in the cond block (bossSeen/enemyHit are sunk to
the skip path). This is the "enemy-tail split".

## 4. What fixing `inSight` alone does (new measurement)

`ins_lenfirst` (single edit on the pristine baseline):

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | screen-first, `x>,y>,x<,y<`, length last | **9908** | **75.354839** |
| `ins_lenfirst` | length first, `x>,x<,y>,y<` | 9896 | 68.837877 |

The `inSight` region itself becomes an almost exact match (the two 28-insn
residual blocks collapse), but the enemy tail re-rolls into a **161-insn
duplicated block**: ours emits the `navWreck` computation twice (once at the
loop-exit join and once after the gas block) where the original has a single
copy. That duplication is the entire reason the whole-function ratio drops.

The `ins_lenfirst` `inSight` block is:

```
12ef68 ldr r0,[pc,#692]
12ef6a movs r1,#0
12ef6c strb.w r1,[r8,#115]
12ef70 add r6,sp,#232          ; vector ptr in r6 (original: r5)
...
12ef96 vmov s0,r0
12ef9a vcmpe.f32 s0,s20
12efa2 bpl.n 12efda
12efa4 vmov r2,s26
...
12efd0 mov.w r0,#0             ; bool in r0 (original: r6, 2-byte movs)
12efd4 it lt
12efd6 movlt r0,#1
12efd8 b.n 12efde
12efda ldr r6,[sp,#144]
12efdc movs r0,#0
12efde ldr r1,[sp,#116]
12efe0 strb.w r0,[r8,#115]
```

The remaining delta is the register allocation: the original uses `r5` for
the temp vector and `r6` for the bool (both 2-byte `movs`), ours uses `r6`
for the vector and `r0` for the bool (4-byte `mov.w r0,#0`). The original can
use `r5` because it reloads `locked` from sp#116 *after* the VectorLength
call (`12ef7e ldr r5,[sp,#116]`); ours keeps `locked` live in `r5` across the
length computation.

## 5. Nested `inSight` spelling reproduces the control flow, not the registers

`ins_nested`:

```cpp
bool inSight = false;
if (AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f) {
    inSight = this->screenX > cx - f && this->screenX < cx + f &&
              this->screenY > cy - f && this->screenY < cy + f;
}
enemy->field_0x73 = inSight;
```

This reproduces the original's *two* `inSight = false` materialisations
(`12ef72 mov.w fp,#0` after the length branch, `12efba mov.w fp,#0` before the
last compare) and the separate false block (`12efe4 mov.w fp,#0`), i.e. the
control-flow skeleton is exact. But the bool lands in `fp` (r11), so every
materialisation is a 4-byte `mov.w`/`movlt.w` instead of a 2-byte
`movs`/`movlt`, and the function is 9924 B.

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | — | **9908** | **75.354839** |
| `ins_nested` | nested `if (length) inSight = screen...` | 9924 | 65.664806 |
| `ins_nested2` | nested `if (length) if (screen) inSight = true` | 9924 | 65.607557 |
| `ins_3nest` | 3-level nesting (x pair, then y pair) | 9924 | 65.607557 |
| `ins_nested_or` | nested + `B_o_t` OR order | **9908** | 67.650007 |
| `ins_nested2_or` | nested2 + `B_o_t` | **9908** | 67.564084 |
| `ins_3nest_or` | 3-level + `B_o_t` | **9908** | 67.564084 |

`ins_nested_or` is the only new spelling that keeps **9908 B** while matching
the original `inSight` control flow, but its whole-function ratio is 67.65 %.

## 6. `inSight` order/position sweeps (all new spellings)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | screen-first, length last | **9908** | **75.354839** |
| `ins_xxyy_last` | screen `x>,x<,y>,y<`, length last | 9936 | 72.222222 |
| `ins_xxyy_last_expr` | same, single bool expression | 9932 | 71.927814 |
| `ins_delta` | `Vector delta = egoPos-targetPos;` then length first | 9912 | 65.872902 |
| `ins_dist` | `float dist = VectorLength(...);` then length first | 9924 | 65.607557 |
| `ins_dist_expr` | same, single bool expression | 9924 | 65.664806 |
| `ins_direct` | `if (length && screen...) enemy->field_0x73 = 1;` | 9888 | 68.494330 |
| `ins_len_f` | length first with `int f = field_0x124` local | 9920 | 65.836554 |
| `ins_len_f_or` | `ins_len_f` + `B_o_t` | 9900 | 67.927231 |
| `ins_decl_first` | `bool inSight` declared before `cx`/`cy`, length first | 9896 | 68.837877 |
| `ins_zero_after` | `field_0x73 = 0` after `cx`/`cy`, length first | 9896 | 68.837877 |

So the exact original order is a genuine source difference, but every spelling
that adopts it either shrinks the function (9896/9892/9888/9884) or re-rolls
the enemy tail; none beats the screen-first baseline.

## 7. Enemy-tail sweeps (new spellings)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | `!A && !(B \|\| turret \|\| other)` | **9908** | **75.354839** |
| `or_bot` | `!A && !(B \|\| other \|\| turret)` (w8109 `B_o_t`) | 9876 | 69.912394 |
| `or_nest_bot` | nested `if (!A) if (!(B\|\|other\|\|turret))` | 9876 | 69.912394 |
| `or_nest_base` | nested `if (!A) if (!(B\|\|turret\|\|other))` | **9908** | **75.354839** (tie) |
| `or_locals` | `bool docking/otherDock` locals | 9880 | 67.960328 |
| `or_demorgan` | `!A && !B && !other && !turret` | 9876 | 69.912394 |
| `or_all` | `!(A \|\| B \|\| other \|\| turret)` | 9876 | 69.912394 |
| `or_all_bot` | `!(A \|\| B \|\| turret \|\| other)` | **9908** | **75.354839** (tie) |
| `or_locals_bot` | locals + `B_o_t` | 9888 | 67.846862 |

`or_nest_base` and `or_all_bot` are byte-identical to the baseline: clang
flattens the nesting and canonicalises the De Morgan/`||` spellings, so they
are not levers. `or_bot` gives the original's inline `cont` block but is 32 B
short and re-rolls the tail.

## 8. Type / declaration / `locked` sweeps (new)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | — | **9908** | **75.354839** |
| `eh_int` | `int enemyHit` | 9924 | 71.422432 |
| `bs_int` | `int bossSeen` | 9904 | 73.493976 |
| `ae_int` | `int alwaysEnemy` | 9900 | 72.557739 |
| `all_int` | all three `int` | 9912 | 70.461715 |
| `aes_int` | `bool alwaysEnemySeen` | 9904 | 72.555205 |
| `eh_vol` / `bs_vol` / `both_vol` | `volatile` bools | 9848/9844/9852 | 62.4/68.7/63.0 |
| `aes_after_bs` | move `alwaysEnemySeen` decl after `bossSeen` | **9908** | 75.326165 |
| `aes_before_bs` | move it before `bossSeen` | **9908** | **75.354839** (tie) |
| `aes_after_eh` | move it after `enemyHit` | 9876 | 70.468256 |
| `eh_before_bs` | swap `bossSeen`/`enemyHit` declarations | 9896 | 66.350711 |
| `nest_or_img` | nested+`B_o_t`, `enemy == this->lockedEnemy` in image select | **9908** | 51.404011 |
| `nest_or_post` | same for the post-`inSight` compare | 9928 | 51.955868 |
| `nest_or_both` | both | 9928 | 51.984525 |

`aes_after_bs` changes the clear-block store order to the original's
`enemyHit -> alwaysEnemySeen -> bossSeen` but leaves the slots swapped
(`alwaysEnemySeen` sp#92, `bossSeen` sp#108 vs the original's sp#92/sp#104),
so it is a 0.03-point loss. Replacing the `locked` local with
`this->lockedEnemy` re-rolls the whole function (≈51 %), so the local is
load-bearing.

## 9. 24-cell grid (inSight x OR x `alwaysEnemySeen` placement x `enemyHit` type)

Best cells:

| inSight | OR | `alwaysEnemySeen` | `enemyHit` | usize | raw % |
|---|---|---|---|---:|---:|
| base | base | base | bool | **9908** | **75.354839** |
| base | base | after-if | bool | 9900 | 73.160235 |
| base | bot | after-if | int | **9908** | 73.136468 |
| base | bot | base | int | **9908** | 73.050459 |
| len | base | after-if | bool | 9924 | 72.435530 |

No cell beats the baseline. The exact `inSight` order (`len`) is always
dominated by the enemy-tail re-roll.

## 10. Root cause / open questions

The baseline is a coupled `MachineBlockPlacement` / greedy-regalloc fixed
point. The two named levers are real source differences:

1. `inSight` is **length-first, `x>,x<,y>,y<`** (not screen-first,
   `x>,y>,x<,y<`, length last);
2. the enemy tail is **`!(B || other || turret)`** with `bossSeen`,
   `alwaysEnemySeen`, `enemyHit` all materialised before the branch, and the
   `cont`/`navWreck` blocks inline.

Fixing either one alone re-rolls the other (and the planet/`stations` block).
The missing compensating change is the same one wave 136 identified: the
original's `r5`-as-temp-vector / `r6`-as-bool allocation (which requires
reloading `locked` from sp#116 after the VectorLength call) coexisting with
the inline enemy tail. Candidate next probes:

* force `locked` to be reloaded after the length call without re-rolling the
  rest — e.g. a narrower `locked` live range only in the offscreen branch;
* make the enemy-tail `cont` block inline **and** keep the `inSight` block
  length-first, i.e. find the source order that makes both `12f3ca` and
  `12ef88` match in the same build;
* check whether the 161-insn enemy-tail duplication under `ins_lenfirst` is a
  `goto tractor` block-placement artefact (the original's `goto` is at
  `12ef08 b 12f152`; ours is at `12eeea b 12f15e`).

## 11. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **84,709** differing bytes (size/base
exact), `.ARM.extab` **1,395** (size exact 156,312), `.rel.plt` **4** (size
exact 29,392), `.note.gnu.build-id` **20**. `.rodata`, `.data`, `.bss`,
`.got`, `.ARM.exidx`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are
byte-identical.
