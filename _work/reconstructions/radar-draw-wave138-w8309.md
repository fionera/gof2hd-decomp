# Radar::draw wave 138 (w8309) — exact enemy `inSight` split: nested source shape found, but the `.rel.plt` pin rules it out (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w8309`, based on `master`
(`96b069792`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.354839 %** (official 75.4). Gate at start and end:
`RATCHET PASS`, byte **4507**, linked **4507**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave137-w8211.md`, `radar-draw-wave136-w8109.md`,
`radar-draw-wave136-w8110.md`, `radar-draw-wave134-w7754.md`.

This wave attacked the assignment's NEW hypothesis directly: reconstruct the
enemy `inSight` block from Thumb and find the source shape that reproduces the
original's 36+33 instruction split (the two `replace` blocks w8110 measured at
`0x12ef58`/`0x12ef88`). It adds five pieces of evidence the earlier notes did
not have:

1. a **fresh Ghidra 12.1.2 decompile** of the whole function (2019 lines) that
   shows the original `inSight` is a **nested** `if (length) { if (x) { if (y) } }`
   shape, not a flat `&&` chain — and that the original materialises the
   `inSight = false` phi **three times** (`0x12ef96`, `0x12efb4`, `0x12efbe`);
2. an exhaustive **120-permutation sweep** of the five `&&` terms: every
   length-first flat chain is **9896/9904/9880 B**, never 9908; the best
   length-first flat chain is 68.8 %;
3. the exact **nested source shape** that reproduces the original's two-block
   split: `bool inSight = false; if (VectorLength(...) < 60000.0f) { inSight =
   screenX > ... && ...; }` — it is **exactly 9908 B** and passes the gate, but
   scores 67.7 %;
4. the **root cause of the remaining register delta**: the original's enemy-loop
   index `i` is `fp` and the loop's temp-`Vector` pointer is `r5`; ours has them
   **swapped** (`i`=r5, temp=`fp`). That swap is why the original's `inSight`
   bool is `r6` (2-byte `movs`) and ours is `r0` (4-byte `mov.w`);
5. the **`.rel.plt` pin**: every `!(B || other || turret)` enemy-tail spelling
   that makes the nested shape 9908 B also swaps four gas-cloud PLT entries
   (`getSparks`/`isSparkAlive`/`setSparkInSight`/`Sprite::getRawFrameCount`,
   `.rel.plt` entries 2736-2739), so it violates the "keep `.rel.plt` exact"
   constraint.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w8309
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~1 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Ghidra decompile (provider reachable with `GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC`):

```sh
rea decompile /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure 0x13e050 --format md > ghidra_draw.md
```

Scratch in `/dev/shm/w8309` (not committed): `Radar.base.cpp` (pristine source),
`score.py` (single-symbol scorer importing `tools/verify/verify.py`, reproduces
75.354839 exactly), `blocks.py` (difflib block map), `perms.py` (120-permutation
driver), `gen.py`/`batch*.py` (variant drivers), `relcheck*.py` (`.rel.plt`
comparator), `orig.dis`/`ours*.dis` (Thumb objdump), `ghidra_draw.md`.

## 2. NEW: the original `inSight` is a nested `if`, not a flat `&&` chain

Ghidra 12.1.2 decompile of the offscreen enemy branch (`ghidra_draw.md`
lines 980-1000):

```c
KVar34 = (KIPlayer)0x0;
pKVar36[0x73] = (KIPlayer)0x0;          /* enemy->field_0x73 = 0 */
uVar1  = PlayerEgo::crosshairPos._4_4_;  /* cy float */
uVar11 = PlayerEgo::crosshairPos._0_4_;  /* cx float */
AbyssEngine::AEMath::operator-(aAStack_98, this_05, pVVar30);
fVar44 = (float)AbyssEngine::AEMath::VectorLength((Vector *)aAStack_98);
if (fVar44 < 60000.0) {
    iVar7 = *(int *)(this + 0x124);      /* f */
    KVar34 = (KIPlayer)0x0;
    if (((int)(float)uVar11 - iVar7 < *(int *)(this + 0xfc)) &&
        (*(int *)(this + 0xfc) < (int)(float)uVar11 + iVar7)) {
        if ((int)(float)uVar1 - iVar7 < *(int *)(this + 0x100)) {
            KVar34 = (KIPlayer)(*(int *)(this + 0x100) < iVar7 + (int)(float)uVar1);
        } else {
            KVar34 = (KIPlayer)0x0;
        }
    }
}
pKVar36[0x73] = KVar34;
```

The `KVar34 = 0` before the x-pair and the `KVar34 = 0` in the y-`else` are the
two extra `inSight = false` materialisations visible in Thumb at `0x12ef96` and
`0x12efbe` (plus `0x12efb4` before the last compare). The Thumb block is
`0x12ef4c..0x12efc0`:

```
12ef4c ldr   r0,[pc,#688]       ; &crosshairPos
12ef4e movs  r6,#0
12ef50 strb.w r6,[r8,#115]      ; enemy->field_0x73 = 0
12ef54 add   r5,sp,#232         ; &tmpVector
12ef56 add   r0,pc
12ef58 ldr   r0,[r0,#0]
12ef5a vldr  s26,[r0]           ; cx float
12ef5e vldr  s24,[r0,#4]        ; cy float
12ef62 mov   r0,r5
12ef64 ldrd  r2,r1,[sp,#108]    ; egoPos, targetPos
12ef68 blx   AEMath::mi
12ef6c mov   r0,r5
12ef6e vcvt.s32.f32 s24,s24     ; cy
12ef72 vcvt.s32.f32 s26,s26     ; cx
12ef76 blx   AEMath::VectorLength
12ef7a vmov  s0,r0
12ef7e ldr   r5,[sp,#116]       ; locked (reload)
12ef80 vcmpe.f32 s0,s20
12ef84 vmrs  APSR_nzcv,fpscr
12ef88 bpl.n 12efc0
12ef8a vmov  r1,s26
12ef8e ldr.w r0,[r4,#292]       ; f
12ef92 ldr.w r2,[r4,#252]       ; screenX
12ef96 movs  r6,#0              ; inSight = false  <-- early phi
12ef98 subs  r3,r1,r0
12ef9a cmp   r2,r3
12ef9c ble.n 12efc0
12ef9e add   r1,r0
12efa0 cmp   r2,r1
12efa2 bge.n 12efc0
12efa4 vmov  r1,s24
12efa8 ldr   r2,[r4,#256]       ; screenY
12efac subs  r3,r1,r0
12efae cmp   r2,r3
12efb0 ble.n 12efbe
12efb2 add   r0,r1
12efb4 movs  r6,#0              ; inSight = false  <-- before last compare
12efb6 cmp   r2,r0
12efb8 it    lt
12efba movlt r6,#1
12efbc b.n   12efc0
12efbe movs  r6,#0              ; inSight = false  <-- y-else
12efc0 strb.w r6,[r8,#115]      ; enemy->field_0x73 = inSight
```

The source shape is therefore:

```cpp
enemy->field_0x73 = 0;
int cx = (int) PlayerEgo::crosshairPos.x;
int cy = (int) PlayerEgo::crosshairPos.y;
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f) {
    inSight = this->screenX > cx - this->field_0x124 && this->screenX < cx + this->field_0x124 &&
              this->screenY > cy - this->field_0x124 && this->screenY < cy + this->field_0x124;
}
enemy->field_0x73 = inSight;
```

(`nested_assign` below). This is the only spelling that reproduces the early
`movs r6,#0` at `0x12ef96` and the `0x12efb4` materialisation.

## 3. NEW: 120-permutation sweep of the flat `&&` chain

`perms.py` builds all `5!` orderings of `{LEN, XG, XL, YG, YL}` with the flat
`if (...) inSight = true;` body. Distribution:

| size | orders | best score |
|---:|---|---:|
| **9908** | 4 (all `LEN` last) | **75.4** (baseline) |
| 9908 | 4 (`LEN` in the middle) | 75.3 |
| 9904 | 8 | 65.4 |
| 9896 | 8 | **68.8** |
| 9880 | 8 | 64.7 |
| 9936/9940/9952/9956 | remainder | 72.2 |

Every `LEN`-first flat chain is **9896, 9904 or 9880 B — never 9908**. The four
9908-B orders are exactly the ones clang canonicalises to the baseline
(`YLXGYGXLL`, `YGXGYLXLL`, `XLYGXGYLL`, `XGYGXLYLL`).

## 4. NEW: the nested shape is exactly 9908 B and gate-green, but 67.7 %

`nested_assign` alone is 9924 B (the bool lands in `fp`, 4-byte `mov.w`).
Adding the w8109 `B_o_t` enemy-tail order
(`!(isDockingToDockingPoint() || otherLock || turretMode)`) brings it to
**exactly 9908 B**:

| variant | source | usize | raw % | `.rel.plt` diff |
|---|---|---:|---:|---:|
| **baseline** | screen-first flat, `LEN` last | **9908** | **75.354839** | **0** |
| `nested_assign` | nested `if (LEN) inSight = screen...` | 9924 | 65.664806 | 0 |
| `nested_assign + B_o_t` | + `!(B \|\| other \|\| turret)` | **9908** | **67.7** | **6** |
| `nested_flat + B_o_t` | nested `if (LEN) { if (screen) ... }` | 9908 | 67.6 | 6 |
| `nested_2 + B_o_t` | 3-level nested | 9908 | 67.6 | 6 |
| `nested_3 + B_o_t` | 3-level nested, y-else | 9908 | 67.7 | 6 |
| `len_xxyy` | flat `LEN` first, `x>,x<,y>,y<` | 9896 | 68.8 | 6 |
| `len_xxyy + B_o_t` | + tail | 9892 | 69.0 | 6 |
| `xxyy_len` | flat screen `x>,x<,y>,y<`, `LEN` last | 9936 | 72.2 | 0 |
| `xxyy_len` (no dead store) | same, drop `field_0x73 = 0` | 9928 | 72.5 | 0 |
| `len_xyxy` | flat `LEN` first, `x>,y>,x<,y<` | 9904 | 65.4 | 0 |

`nested_assign + B_o_t` passes the full ratchet (byte 4507, linked 4507, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0) — but avg drops
99.94 -> 99.93 and the function 75.4 -> 67.7, so it was not landed.

## 5. NEW: root cause of the register delta is a `fp`/`r5` swap

The original's inSight block uses `r5` for the temp `Vector` and `r6` for the
`inSight` bool. Ours uses `r6`/`r0` (flat) or `r6`/`fp` (nested). The reason is
the **enemy-loop index register**:

| quantity | original | ours (baseline) |
|---|---|---|
| enemy-loop `i` | **`fp`** (`12ecf6`, `12eee4`, `12f188`) | **`r5`** (`12ecfe`, `12ef00`) |
| loop temp-`Vector` ptr | **`r5`** (`12edfa add r5,sp,#232`) | **`fp`** (`12ecd4 add.w fp,sp,#232`) |
| `inSight` bool | `r6` (2-byte `movs`) | `r0` (4-byte `mov.w`) |
| temp-`Vector` in inSight | `r5` (`12ef54`) | `r6` (`12ef70`) |

The original **rematerialises** `add r5,sp,#232` inside the loop (2 B) and again
in the inSight block; ours **hoists** `add.w fp,sp,#232` before the loop (4 B)
and reuses `fp`. This is a pure greedy-RA tie-break: the two live ranges are
swapped. The nested shape moves `i` to `sl` and the bool to `fp` — a third
allocation, not the original's.

Attempts to force `i` into `fp` (all measured, none beat 75.4):

| variant | what | usize | raw % |
|---|---|---:|---:|
| `len_int` / `len_int2` | `int i` instead of `uint32_t i` | 9896 | 68.8 |
| `len_sharedI` | one `sharedI` reused by planet + enemy loops | 9896 | 68.8 |
| `len_local_enemies` | `Array<KIPlayer*>* enemies = this->enemies;` local | 9860 | 64.0 |
| `len_count` | hoisted `count = enemies->size()` | 9860 | 64.0 |
| `len_poslocal` | `Vector pos = player->getPosition();` local | 9832 | 65.0 |
| `base_norecomp` | drop the invented planet `getAutoPilotTarget` recompute | 9856 | 70.8 |
| `len_norecomp` | both | 9844 | 66.3 |

## 6. NEW: the `.rel.plt` pin forbids the 9908-B shape

The original's `.rel.plt` is byte-identical to the baseline, and entries
2736-2739 are

```
2736 getSparks  2737 isSparkAlive  2738 setSparkInSight  2739 Sprite::getRawFrameCount
```

Every `B_o_t`/`demorgan`/`all_bot` tail spelling re-rolls the gas loop and
swaps these four entries:

```
ours: 2736 getRawFrameCount  2737 getSparks  2738 isSparkAlive  2739 setSparkInSight
```

so `.rel.plt` differs by 6 bytes (`entry 2736..2739`). `nested_assign + base`
keeps `.rel.plt` exact but is 9924 B; `xxyy_len` keeps it exact but is 9936 B;
`len_xyxy` keeps it exact but is 9904 B. **There is no length-first spelling
that is simultaneously 9908 B and `.rel.plt`-exact.**

## 7. Ghidra-confirmed source differences that remain

* the invented planet-block `onPlanetCourse` recompute: the original calls
  `PlayerEgo::getAutoPilotTarget` **once** (`12e6f4`, the prologue cold tail);
  ours calls it twice (`12e6d8` prologue + `12e756` planet block). Removing the
  recompute shrinks the function to 9856 B and does **not** move `i` to `fp`.
* the enemy-tail `||` order: the original machine order is `B | turret | other`
  (`12f3d0..12f3da`); the source spelling that reproduces it is
  `!(B || other || turret)` (w8109), but that spelling is what breaks `.rel.plt`.
* the enemy-tail cond block materialises `bossSeen`/`alwaysEnemySeen`/`enemyHit`
  before the branch (`12f3b0..12f3c6`); ours sinks two of the three.

## 8. Root cause / open questions

The inSight *source* shape is now pinned (nested, length-first,
`x>,x<,y>,y<`), and it reproduces the original's 36+33 two-block split and the
three `inSight = false` materialisations. The remaining blocker is the
`fp`/`r5` swap: until the enemy-loop index is allocated to `fp` (which requires
the loop temp-`Vector` pointer to be rematerialised in `r5`), the inSight block
can never be byte-identical, and every spelling that fixes the split either
shrinks the function (9896/9904/9880) or reorders the gas-cloud `.rel.plt`
entries.

Candidate next probes:

* find the source change that makes clang **rematerialise** the loop temp
  `add r5,sp,#232` instead of hoisting `add.w fp,sp,#232` — the original's
  `r5`/`fp` roles are exactly swapped, so this is a single RA tie-break;
* recover the ~72 B of original code that the invented `getAutoPilotTarget`
  recompute currently stands in for, so the recompute can be removed without
  shrinking the function;
* check whether the enemy-tail `||` spelling can be chosen so that the gas-loop
  first-reference order is preserved (the `.rel.plt` entries 2736-2739).

## 9. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes vs the original: `.text` 0x1337c8, `.rodata` 0x41b67, `.data`
0x296c, `.bss` 0x69ac, `.rel.plt` 0x72d0, `.ARM.exidx` 0x6f70, `.ARM.extab`
0x26298, `.note.gnu.build-id` 0x24 — all exact. `.rel.plt` content is
byte-identical at baseline.
