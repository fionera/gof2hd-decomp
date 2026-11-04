# `Gun::calcCharacterCollision` pass 18: the 6-byte asteroid-join reload pair is the whole
# residual, and it is not reachable from any human-style source perturbation tried (w5413, wave 106)

Worker: w5413. Branch: `wave106-w5413`, base master `486f8a119`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Scratch: `/dev/shm/w5413/`.

## 0. Result

**No code change landed.** The tree body (`F H K L P` tail + `int dmg` local) is still the
best measured shape: **90.0 %**, 1984/1984 B, 585/650 normalized instructions (official
verifier). Gate after (unchanged): byte **4500/4524**, linked **4500/4524**, size-exact
**4524/4524**, data **843/843**, imports **207**, lint **CLEAN**, drift **0**, `RATCHET PASS`.

| build | gate byte/linked | target pct | target size | target matched |
|---|---:|---:|---:|---:|
| before (HEAD `486f8a119`) | **4500 / 4500** | **90.0** | **1984** | **585/650** |
| after this pass (note only) | **4500 / 4500** | **90.0** | **1984** | **585/650** |

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`,
`H` = `setHitVector`, `L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

The faithful source shape (oracle: `F P H L K`, no `dmg` local) is **64.1 % / 1920 B** —
re-measured this pass. The tuned tree body is 26 points higher.

## 1. What is actually left: a per-region address-delta profile

Aligning the two instruction streams (normalized, `difflib`) and comparing the *byte address*
of each matching block gives the residual in one table. `delta = orig_addr - ours_addr`:

| first matching insn | orig | ours | delta | what happens there |
|---|---|---:|---:|---|
| prologue .. box test | 0x152f90 | 0x152f90 | **+0x0** | identical |
| `mov r0, r8` (setBombForce) | 0x1531a2 | 0x15319c | **+0x6** | shipped reloads `i`+`td` at the asteroid-true join; we do not |
| `ldr r0,[sp,#16]` (damage block) | 0x15326e | 0x153262 | **+0xc** | shipped *also* reloads `i`+`td` at the asteroid-false join |
| damage-block arg setup | 0x1532d8 | 0x1532c8 | **+0x10** | our `int dmg` cache adds `ldr.w sl,[r9,#96]` (4 B) |
| plain-arm arg setup | 0x1532e4 | 0x1532d6 | **+0xe** | we use `mov r1,sl` (2 B) where the shipped reloads `this->damage` (4 B) |
| literal pool | 0x1532f4 | 0x1532e4 | **+0x10** | pool lands 16 B early because the code above is short |
| `ldmia r3,{r0,r2,r3}` (tail) | 0x15332c | 0x153312 | **+0x1a** | tail order `F P H L K` vs `F H K L P` |
| `ldr.w r0,[r9,#56]` (geometries) | 0x153358 | 0x15334e | **+0xa** | our extra `fp`/`r6` reloads in the explosive branch |
| `add r1, pc` (Canvas) | 0x15337c | 0x15337e | **-0x2** | we are now 2 B *long* |
| `ldr.w r0,[r9,#48]` (scatter) | 0x153432 | 0x153432 | **+0x0** | scatter block is byte-identical |
| epilogue | 0x1536dc | 0x1536dc | **+0x0** | identical |

So the whole function is **two identical regions (prologue..box test, scatter..epilogue)
separated by a 0x3a0-byte middle whose internal offsets wander between +0x0 and +0x1a and
come back to 0**. Every branch target that crosses the middle therefore differs, which is
where 62 of the 130 mismatched normalized instructions come from
(`pool` 14, `other` 54, `branch` 62 — 2×65 = 130).

## 2. The shipped reload pair is the trigger, not a symptom

The shipped asteroid-true join is 6 B longer than ours:

```
153194: d061  beq.n 153260
153196: 9809  ldr   r0, [sp, #36]   ; mine
153198: f8dd b028  ldr.w fp, [sp, #40]   ; i   <-- we do not emit this
15319c: 9e06  ldr   r6, [sp, #24]   ; td  <-- we do not emit this
15319e: 2800  cmp   r0, #0
1531a0: d15b  bne.n 15325a
```

and the asteroid-false join repeats it:

```
153260: 9809  ldr   r0, [sp, #36]
153262: f8dd b028  ldr.w fp, [sp, #40]   ; i
153266: 9e06  ldr   r6, [sp, #24]   ; td
153268: 2800  cmp   r0, #0
15326a: f040 8157  bne.w 15351c
```

Our build reloads `i` only at the box-test exit (`0x1531f6`) and the inner-loop continue
(`0x153250`), and `td` only at the explosive/geometries branches (`0x15336a`, `0x153376`,
`0x1533f8`). Instruction counts are equal (650/650); the 12 missing bytes at the two joins
are paid for by our `int dmg` cache (+4 B), the extra branch reloads (+16 B) and the shorter
`mov r1,sl` (-2 B) elsewhere.

The spill slots are also swapped: shipped `explosive=[sp,#20]`, `td=[sp,#24]`; ours
`explosive=[sp,#24]`, `td=[sp,#20]`. The slot stores in the prologue are in the same textual
order (`str r2,[sp,#20/24]` at `0x152fe4`, then `str r6,[sp,#24/20]` at `0x153010`), so this
is a register-allocator spill-order difference, not a source-order difference.

## 3. Measured table (official verifier, `link_so` + `tools/verify/verify.py` scoring)

All rows target `_ZN3Gun22calcCharacterCollisionEv`; `osize` is always 1984.

| variant | pct | usize | matched | note |
|---|---:|---:|---:|---|
| **tree body (`F H K L P`, `int dmg`)** | **90.0** | **1984** | **585** | current best |
| tree + `expect(distance,1)` | 85.2 | 1984 | 554 | |
| tree + `expect(mine,1)` top-level | 86.2 | 1984 | 560 | |
| tree + `expect(empDamage != kNoAttribute,1)` | 89.5 | 1984 | 582 | |
| tree + `expect(owner==nullptr\|\|owner->enemyFlagsLo,1)` | 88.0 | 1984 | 572 | |
| tree + `expect(egoFlag,1)` / `expect(playerGun,1)` | 90.0 | 1984 | 585 | codegen-identical |
| tree + `expect(explodeOnContact,0)` | 84.7 | 1984 | 550 | |
| tree + `expect(isAsteroid(),0)` | 59.0 | 1964 | 383 | |
| tree + `expect(scatter,1)` on radius branch | 82.6 | 1988 | 538 | |
| tree + `expect(scatter,0)` | 55.0 | 1940 | — | |
| tree, `int dmg` removed (direct `this->damage`) | 84.0 | 1996 | 547 | plain arms stop merging |
| tree, oracle `int dmg` phi (one call) | 84.1 | 1980 | 546 | 4 B short |
| tree, `const int &dmg` | 84.0 | 1996 | 547 | |
| tree, `unsigned`/`bool`/`char` `dmg` | 90.0 | 1984 | 585 | codegen-identical |
| tree, `volatile int dmg` | 67.6 | 1968 | 438 | |
| tree, `short`/`int8_t` `dmg` | 85.1 | 1984 | 553 | |
| `F P H L K` + `int dmg` | 64.8 | 1912 | 417 | low-pressure map |
| `F P H L K`, no `dmg` (faithful) | 64.1 | 1920 | 414 | true source shape |
| `F P H L K` + `expect(mine,0)` | 85.5 | 1984 | 556 | `this=r9`, but `enemy`/`td` swap |
| `F P H L K` + `expect(mine,0)` + `expect(distance,0)` | 82.3 | 1984 | 535 | early `td` reload, but `i*3` in r6 |
| `F P H L K` + `expect(scatter,1)` | 79.0 | 1976 | — | |
| `F H K L P` + `Matrix view` hoisted to fn scope | 63.4 | 2032 | 419 | shipped map, +48 B |
| `F P H L K` + `Matrix view` hoisted | 64.0 | 2032 | 423 | |
| tree, inner `for` → `while` (explicit `++i` on continue) | 90.0 | 1984 | 585 | codegen-identical |
| tree, `td` as `Vector&` | 90.0 | 1984 | 585 | codegen-identical |
| tree, `this->targetDir` direct (no `td`) | 90.0 | 1984 | 585 | codegen-identical |
| tree, `Array::operator[]` ↔ `.data()` ↔ `.size()` | 90.0 | 1984 | 585 | codegen-identical |
| tree, `mine`/`scatter`/`explodeOnContact` inlined at use | 89.8–90.0 | 1984 | 584–585 | |
| tree, `td` declared before `weaponType` | 66.2 | 1964 | 431 | |
| tree, `td` declared before `explosive` | 67.1 | 1968 | 437 | |
| tree, `lastHitKIPlayer`/`td` moved before `explosive` | 80.4 | 1984 | 525 | |
| tree, `isAsteroid()` inverted / `!= false` / `&& mine` split | 51.8–90.0 | 1956–1984 | 334–585 | |
| tree, `if (mine) continue` spellings (`!=0`,`==1`,`!!`,`&&true`) | 90.0 | 1984 | 585 | codegen-identical |
| tree, `setBombForce(0)` / `damage(9999,0,…)` | 90.0 | 1984 | 585 | codegen-identical |

An exhaustive single-site `__builtin_expect` sweep (all 39 `if (...)` occurrences in the
body × {0,1}, applied at that exact occurrence — the earlier pair harnesses matched by
condition *text* and so always hit the first `mine`) also tops out at 90.0; every site that
is not codegen-identical is a regression (best regressions: `empDamage != kNoAttribute,1`
89.5, `owner==nullptr||owner->enemyFlagsLo,1` 88.0, `distance<magnitude,1` 85.2).

Random multi-knob searches this pass (700 samples over tail order × dmg shape × 0–3
`__builtin_expect` edits) never exceeded 90.0; the runner-up is `F K H L P` at 89.7.

## 4. Proven vs hypothesis

**Proven (measured):**
1. The tree body is a strict local optimum of the explored source space: 90.0 % is the
   maximum over every tail permutation, every damage-block shape, every `__builtin_expect`
   site/value, every loop spelling, every `td`/`dmg` type and declaration order, and the
   direct/`Array::operator[]`/`Vector&` spellings tried here and in w4815/w5013/wf112/wf213/
   wf313/w5317.
2. The residual is localised to the middle region (§1). The prologue..box-test region and
   the scatter..epilogue region are byte-identical; only the asteroid joins, the damage
   block, the tail and the literal pool differ.
3. The shipped `F P H L K` tail order compiles to the *low-pressure* map under clang 7.0.2
   `-Oz` (this=r4, enemy=r6, no `i` spill), which is 12 instructions shorter and scores
   64.1–64.8 %. The shipped binary has the *high-pressure* map (this=r9, enemy=r8, i=fp,
   i*3=r4) with the `F P H L K` order — a hybrid our compiler does not produce from any
   source shape tried.
4. `__builtin_expect(mine,0)` on the radius branch is the only single knob that restores
   `this=r9` on the `F P H L K` order, and it swaps `enemy`/`td` (r6↔r8) and moves `i*3`
   into r6, so it loses 4.5 points.

**Hypothesis (not proven):**
1. The shipped's two asteroid-join reload pairs are the output of LLVM's `SpillPlacer`
   region split, not of a source statement. If the split point could be moved to the
   `isAsteroid` successors, the 6-byte shift would disappear and ~62 branch instructions
   would realign (potential ≈ 94–95 %). No source knob found here moves it.
2. The `explosive`/`td` spill-slot swap is a spill-order artefact (the prologue stores are in
   the same textual order on both sides), so it is not reachable by reordering declarations.

## 5. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984
```

Scratch harnesses: `/dev/shm/w5413/fastscore.py` (official-verifier scorer),
`try.py <body>` (splice + `link_so` + score), `aligndiff.py` (aligned opcode diff),
`cat.py` (branch/pool/other mismatch counts), `pairsearch.py` (all `__builtin_expect`
pairs), `randsearch.py` / `randsearch2.py` (multi-knob random search), plus the body files
`body_head.txt` (tree), `body_fphlk.txt` (`F P H L K` + `dmg`), `body_faithful.txt`
(`F P H L K`, no `dmg`), `v_fphlk_mine0_dist0.txt` (near-miss).

## 6. Files changed

`_work/reconstructions/gun-calccharactercollision-wave106-w5413.md` only (note).
Gate: byte 4500 / linked 4500, size-exact 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, `RATCHET PASS`.
