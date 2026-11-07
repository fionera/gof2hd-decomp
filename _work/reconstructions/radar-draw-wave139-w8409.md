# Radar::draw wave 139 (w8409) — nested `inSight` + `Vector delta` local: 9908 B with `.rel.plt` byte-identical

**Status:** source change landed, 2026-11-07. Worktree `wt/w8409`, based on `master`
(`bf123bd47`). The target `src/game/weapons/Radar.cpp` gains a `Vector delta` local in the
enemy `inSight` block and the nested length-first `inSight` shape.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3477 (was 3483).
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.354839 %** (official 75.4); after this change = **66.3 %** (raw diff 7724 -> 8346).
Gate before and after: `RATCHET PASS`, byte **4507** -> **4507**, linked **4507** -> **4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, avg 99.94 -> 99.93.
All 27 section sizes unchanged; `.rel.plt` content **byte-identical**.

Read with `radar-draw-wave138-w8309.md`, `radar-draw-wave137-w8211.md`,
`radar-draw-wave136-w8109.md`.

## 1. The mission and the new hypothesis

w8309 found the exact nested `inSight` shape (length-first, `x>,x<,y>,y<`) but it is 9924 B
alone; adding the `!(B || other || turret)` enemy-tail order (`B_o_t`) brings it to exactly
9908 B, yet that spelling moves the enemy lock-timer block (the first `getRawFrameCount`
call) before the gas loop, so gold creates the four gas-cloud PLT entries in the order
`getRawFrameCount, getSparks, isSparkAlive, setSparkInSight` instead of the original
`getSparks, isSparkAlive, setSparkInSight, getRawFrameCount` (`.rel.plt` entries
2736-2739). The mission: keep the nested `inSight` shape, keep 9908 B, and keep
`.rel.plt` byte-identical.

## 2. NEW: a `Vector delta` local supplies the missing 16 bytes without touching the tail

The 16-byte gap is exactly the enemy-tail inlining that `B_o_t` triggers. A different,
semantically natural source change closes the same gap **without** changing the tail order:
bind the difference vector to a local before the length test.

```cpp
bool inSight = false;
AbyssEngine::AEMath::Vector delta = this->egoPos - this->targetPos;   // NEW
if (AbyssEngine::AEMath::VectorLength(delta) < 60000.0f) {
    inSight = this->screenX > cx - this->field_0x124 && this->screenX < cx + this->field_0x124 &&
              this->screenY > cy - this->field_0x124 && this->screenY < cy + this->field_0x124;
}
enemy->field_0x73 = inSight;
```

The tail stays the pristine `!(B || turret || other)`, so the enemy lock-timer block stays
after the gas loop and `.rel.plt` is byte-identical. Measured (single edit on the pristine
baseline unless noted):

| variant | inSight | tail | usize | raw % | `.rel.plt` diff |
|---|---|---|---:|---:|---:|
| **baseline** | flat screen-first, length last | base | **9908** | **75.354839** | **0** |
| `nested` (w8309) | nested length-first | base | 9924 | 65.664806 | 0 |
| `nested + B_o_t` (w8309) | nested length-first | `B_o_t` | **9908** | 67.7 | **4** |
| **`delta` (this wave)** | nested length-first, `Vector delta` | base | **9908** | **66.3** | **0** |

The `delta` local is the only spelling found that is simultaneously 9908 B and
`.rel.plt`-exact with the nested `inSight`. The four inSight comparison orders were
swept (`xxyy` 66.3, `xyxy` 66.1, `yyxx` 66.2, `yxyx` 66.0); `xxyy` — the original's
order — is the best and is the one landed.

## 3. NEW: the `delta` local reproduces the original `inSight` block almost exactly

This is the important structural result. The landed `inSight` block (objdump of the `.o`):

```
f02 ldr r0,[pc,#708]
f04 mov.w fp,#0            ; bool in fp (original: r6, 2-byte movs)
f08 strb.w fp,[r8,#115]
f0c add r6,sp,#232         ; temp vector (original: r5)
f0e add r0,pc
f10 ldr r0,[r0,#0]
f12 vldr s26,[r0]
f16 vldr s24,[r0,#4]
f1a mov r0,r6
f1c ldrd r2,r1,[sp,#108]   ; egoPos, targetPos  <-- sp#108, same as original
f20 blx AEMath::mi
f24 mov r0,r6
f26 vcvt.s32.f32 s24,s24
f2a vcvt.s32.f32 s26,s26
f2e blx AEMath::VectorLength
f32 vmov s0,r0
f36 ldr r6,[sp,#116]       ; locked reload  <-- same reload as original
f38 vcmpe.f32 s0,s20
f3c vmrs APSR_nzcv,fpscr
f40 bpl.n f80
f42 vmov r1,s26
f46 ldr.w r0,[r4,#292]
f4a ldr.w r2,[r4,#252]
f4e mov.w fp,#0
f52 subs r3,r1,r0
f54 cmp r2,r3
f56 ble.n f80
f58 add r1,r0
f5a cmp r2,r1
f5c bge.n f80
f5e vmov r1,s24
f62 ldr.w r2,[r4,#256]
f66 subs r3,r1,r0
f68 cmp r2,r3
f6a ble.n f7c
f6c add r0,r1
f6e mov.w fp,#0
f72 cmp r2,r0
f74 it lt
f76 movlt.w fp,#1
f7a b.n f80
f7c mov.w fp,#0
f80 strb.w fp,[r8,#115]
```

Compare the original (`0x12ef4c..0x12efc0`): **every instruction, immediate, stack slot and
branch is identical** — including `ldrd [sp,#108]` and the `ldr [sp,#116]` reload — except
the `inSight` bool register. The original uses `r6` (2-byte `movs`/`movlt`); ours uses `fp`
(4-byte `mov.w`/`movlt.w`). That is the only residual in the block, and it is a pure greedy
register-allocation tie-break (the enemy-loop index/temp-vector `fp`/`r5` swap first
documented in w8309). The `delta` local is what makes the temp-vector stack slot and the
`locked` reload match; the earlier nested spelling kept `locked` live across the length call
and used `sp#112`.

## 4. Why the whole-function pct still drops (66.3 vs 75.4)

The `inSight` block is now nearly exact, but the nested shape re-rolls the enemy-tail block
placement: the baseline's flat `inSight` is a strong local optimum for the rest of the
function (the enemy tail, gas loop and `otherLock` blocks land in the order that matches the
original's *other* 90 %). The landed shape trades that for the correct `inSight` block.
Raw diff 7724 -> 8346; the inSight block gain is real but smaller than the block-placement
loss. This is the same coupled fixed point as waves 136-138, now with the inSight block
pinned and `.rel.plt` preserved.

## 5. What was tried and ruled out (all measured)

* **All six `||` orders with nested inSight**: only `B_o_t` gives 9908 B; every other order
  is 9808/9824/9904/9912/9924 B. All six keep the nested shape; `B_o_t` is the only one that
  reorders `.rel.plt` (entries 2736-2739).
* **`__builtin_expect(enemyHit, 0)`** on the enemy lock-timer `if`: moves the lock-timer
  block after the gas loop and makes `.rel.plt` exact, but the function grows to 9948 B
  (`.text` size regression) and the pct falls to 59.2. `expect(enemyHit,1)` is a 9908-B tie
  but still reorders `.rel.plt`.
* **Moving the whole `if (!A && !(...))` guard after the gas loop**: `.rel.plt` exact but
  9792 B (116 B short); moving only the `if (enemyHit)` body after the gas loop: 9816 B.
* **Moving the spark sub-loop before the enemy loop**: 10196 B, `.rel.plt` diff 5.
* **Small inSight spellings**: `int`/`unsigned char` bool, declaration-first, `nested2`,
  no dead store — all either 9908 B with `.rel.plt` diff 4 or a size change.
* **`int i`** in the enemy loop: 9908-B tie but still `.rel.plt` diff 4.

## 6. Open questions

* The `inSight` bool is still `fp`; the original's `r6` needs the enemy-loop index in `fp`
  and the loop temp vector rematerialised in `r5` (w8309's RA swap). With the `delta` local
  now in place the inSight block is otherwise exact, so this is the single remaining
  inSight delta.
* The nested shape's block placement re-rolls the enemy tail; recovering the original's
  `cond -> cont -> navWreck -> clear -> recompute -> gas -> lock-timer` layout while keeping
  the nested inSight and `.rel.plt` is the next step. The `delta` local is a stable base for
  that search (it no longer depends on the `B_o_t` tail spelling).

## 7. Gate / section deltas

Before: byte 4507, linked 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, avg 99.94. After: byte **4507**, linked **4507**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, avg **99.93**. All 27 ELF section sizes are exact;
`.rel.plt` content is byte-identical (3674 entries, 0 differing).
