# `Gun::calcCharacterCollision` wave 107 (w5515): the asteroid-join reload pair is a
# spill-placement artefact; no source shape tried reaches it

Worker: w5515. Branch: `wave107-w5515`, base master `f857b5171`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Scratch: `/dev/shm/w5515/`.

## 0. Result

**No code change landed.** The tree body (HEAD: `int dmg = this->damage;` + four
`enemy->damage(...)` call sites + tail `F K H L P`) remains the best measured shape:
**90.0 %**, 1984/1984 B, 585/650 normalized instructions (official verifier).

| build | gate byte/linked | target pct | target size | target matched |
|---|---:|---:|---:|---:|
| before (HEAD `f857b5171`) | **4500 / 4500** | **90.0** | **1984** | **585/650** |
| after this pass (note only) | **4500 / 4500** | **90.0** | **1984** | **585/650** |

Gate after: byte 4500/4524, linked 4500/4524, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`. No ELF section / `.ARM.extab` change
(no code change; the 28-section / `0x26294` state of wave 106 is untouched).

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`,
`H` = `setHitVector`, `L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

## 1. What this pass established

### 1.1 The shipped source shape is the *oracle* damage block, not the tree body

The shipped main damage block has **one** `blx Player::damage` (0x153312) with a phi
argument: the two float arms compute `s0`, the two plain arms load `this->damage`
directly (`ldr r1,[r9,#96]` at 0x153308), and all four converge on the shared call.
`sl` is `&position` (`add.w sl,sp,#192` at 0x15329a / 0x1532ce), not the damage cache.
That is exactly:

```cpp
int dmg;
if (owner == nullptr || owner->enemyFlagsLo) {
    if (egoFlag && level->getPlayer() && level->getPlayer()->isDockedToDockingPoint())
        dmg = (int)((float)damage * 3.0f * 0.25f);
    else dmg = this->damage;
} else if (enemy->egoFlag) {
    dmg = (int)((float)damage * 0.2f);
} else dmg = this->damage;
enemy->damage(dmg, friendGun, itemIndex);
```

i.e. the oracle body (`body_oracledmg.txt`), which is 1980 B and scores **84.1 %** with
the `F H K L P` tail.  The tree body's `int dmg = this->damage` cache is a *tuning*
device that adds 4 B and flips the allocator into the shipped map; the shipped itself
rematerialises `this->damage`.

### 1.2 The shipped tail order `F P H L K` is preserved 1:1 by clang

Source order equals emitted order for every perm tried: source `F P H L K` emits
`F P H L K`; source `F H P L K` emits `F H P L K`; source `F K H L P` emits
`F H K L P` (one scheduler swap).  So the shipped tail really is source `F P H L K`.

### 1.3 `F P H L K` alone collapses the register map

With either damage shape, source tail `F P H L K` under `-Oz` produces the
**low-pressure** map (`this=r4, enemy=r6, i=r8, i3=r5, td=r9`, 1904–1912 B, 61–65 %).
The shipped's high-pressure map (`this=r9, enemy=r8, i=fp, i3=r4, td=r6`) needs
`__builtin_expect` pressure knobs.  The best such combination found this pass is
`expect(mine,0)` on the radius branch + `expect(distance<magnitude,0)` +
`expect(ki->field_0x71,0)` on the oracle body with tail `F P H L K`:

| variant | pct | size | map |
|---|---:|---:|---|
| oracle dmg, `F P H L K` | 61.8 | 1904 | low-pressure |
| oracle dmg, `F P H L K` + `expect(mine,0)` + `expect(distance,0)` | 83.5 | 1980 | `this=r9 i=fp enemy=r8 i3=r6 td=r6 slot24 expl20 join_r6` |
| **+ `expect(ki->field_0x71,0)`** | **87.6** | **1984** | same; **tail `F P H L K`, slots 24/20, td join reload, damage block rematerialises `this->damage`** |
| tree dmg, `F P H L K` + `expect(mine,0)` | 85.5 | 1984 | `this=r9 i=fp enemy=r6 td=r8` (enemy/td swap) |
| tree dmg, `F P H L K` + `expect(mine,0)` + `expect(distance,0)` | 82.3 | 1984 | `i3=r6`, td slot24/expl20, join_r6 |
| tree dmg, `F P H L K` + `expect(distance,0)` | 84.8 | 1980 | `i3=r5`, td slot24/expl20, join_r6 |
| oracle dmg, `F P H L K` + `expect(mine,1)` + `expect(distance,0)` | 72.6 | 1992 | `i3=r4` full map, join_r6, **8 B long** |

The 87.6 % row is the closest *source-level* reconstruction found: it reproduces the
shipped tail order, both spill slots, the `td` asteroid-join reload, and the
single-call damage block.  Its only residuals are `i3` in `r6` (shipped `r4`) and the
missing `ldr.w fp,[sp,#40]` at the two asteroid joins.

### 1.4 The missing `fp` join reload is not reachable from source

The shipped reloads **both** `i` (`fp`) and `td` (`r6`) at the two asteroid joins:

```
153196: ldr r0,[sp,#36]      ; mine
153198: ldr.w fp,[sp,#40]    ; i     <-- not reproduced by any variant
15319c: ldr r6,[sp,#24]      ; td
```

The `td` half is reproducible (§1.3).  The `fp` half was not produced by:
- all 120 tail permutations of the tree body (0/120 have either join reload);
- all 120 tail permutations of the oracle body;
- all 120 tail permutations of the `expect(mine,0)+expect(distance,0)` body;
- 240 random multi-`expect` samples on the tree body;
- the full 81-point `{outer-mine, radius-mine, distance, ki->field_0x71} × {none,0,1}`
  cross product on both the tree-damage and oracle-damage bodies;
- every single-site `__builtin_expect` site/value pair (existing `sweep*_feat.out`).

The reason is structural: `fp` is clobbered by the radius (`vmov fp,s0` at 0x15315c in
both binaries), so `i` lives in `[sp,#40]` from the loop header.  The shipped's
allocator splits the `i` live range at the `isAsteroid` block and reloads at both
successors; ours keeps it in the slot until the `lifetimes[i]` store and reloads there
(`ldr r2,[sp,#40]`).  This is LLVM 7 `SpillPlacement` + greedy-allocator region
selection, not a source statement.  Because the reload is 6 B at each join, every
branch target across the middle shifts by 6/12 B — that is the bulk of the 65
mismatched instructions.

## 2. Measured table (official verifier, `link_so` + `tools/verify/verify.py` scoring)

All rows target `_ZN3Gun22calcCharacterCollisionEv`; `osize` is always 1984 unless noted.

| variant | pct | usize | note |
|---|---:|---:|---|
| **tree body (HEAD: dmg cache + 4 calls, tail `F K H L P`)** | **90.0** | **1984** | current best; shipped map; no join reloads |
| tree body, tail `F K H L P` (same) | 90.0 | 1984 | |
| tree body, tail `F H K L P` | 90.0 | 1984 | codegen-identical to HEAD |
| tree body, tail `F K H L P` → `F K H L P` | 89.7 | 1984 | |
| tree body, tail `F H P L K` | 85.3 | 1976 | shipped map, 8 B short |
| tree body, tail `F H P K L` / `F H K P L` | 84.8 | 1976 | |
| tree body, tail `F H L P K` / `F H L K P` | 76.0 | 1964 | |
| tree body, tail `F P H L K` | 64.8 | 1912 | low-pressure map |
| oracle dmg, tail `F H K L P` | 84.1 | 1980 | shipped map, 4 B short |
| oracle dmg, tail `F H P L K` | 84.5 | 1972 | shipped map |
| oracle dmg, tail `P H F L K` | 84.8 | 1976 | shipped map |
| oracle dmg, tail `F P H L K` | 61.8 | 1904 | low-pressure map |
| oracle dmg, `F P H L K` + mine0 + dist0 | 83.5 | 1980 | join_r6, slots 24/20 |
| oracle dmg, `F P H L K` + mine0 + dist0 + ki0 | 87.6 | 1984 | tail/slots/td-join correct; `i3=r6`, no join_fp |
| tree dmg, `F P H L K` + mine0 | 85.5 | 1984 | enemy/td swapped |
| tree dmg, `F P H L K` + mine0 + dist0 | 82.3 | 1984 | `i3=r6`, join_r6 |
| tree dmg, `F P H L K` + dist0 | 84.8 | 1980 | `i3=r5`, join_r6 |
| tree dmg, `F P H L K` + mine0 + dist0 + ki0 | 73.0 | 1996 | `i3=r4`, join_r6, expl slot 16 |
| oracle dmg, `F P H L K` + mine1 + dist0 | 72.6 | 1992 | full map `i3=r4`, join_r6, 8 B long |
| tree body, `int dmg` removed (direct `this->damage`) | 84.0 | 1996 | |
| tree body, `int dmg` moved to loop top | 80.3 | 1992 | `i3=r6`, td=r4 |
| tree body, `int dmg = 0` at loop top + else-arm assigns | 84.0 | 1996 | |
| tree body, `int dmg` at function top | 90.0 | 1984 | codegen-identical |
| tree body, `int dmg` before `empDamage` | 85.0 | 1980 | |
| tree body, `for (int i …)` / `for (int e …)` | 89.8 | 1984 | signed compare |
| tree body, `!this->owner` / `(int)getKIPlayer()` / `playerGun != 0` | 90.0 | 1984 | codegen-identical |
| tree body, `explosive`/`explodeOnContact` declaration swap / init 0 | 90.0 | 1984 | codegen-identical |
| tree body, `explosive = isRocketLike() || bomb` | 50.9 | 1868 | |
| tree body, `if (mine || bomb)` / parenthesised flag exprs | 90.0 | 1984 | codegen-identical |
| tree body, declaration-initialised `explosive`/`explodeOnContact` | 90.0 | 1984 | codegen-identical |
| all single-site `__builtin_expect` (tree body) | ≤ 90.0 | 1984 | `sweep4_feat.out` |
| 240 random multi-`expect` (tree body) | ≤ 90.0 | — | `rand2.out` |

## 3. Proven vs hypothesis

**Proven (measured):**
1. HEAD's tree body is a strict local optimum of the explored source space: 90.0 % is
   the maximum over every tail permutation, damage-block shape, `__builtin_expect`
   site/value (single and 81-point cross products), loop-index signedness, `dmg`
   declaration placement, flag type/spelling, and the direct/`Array`/`Vector&`
   spellings tried here and in w4815/w5013/wf112/wf213/wf313/w5317/w5413.
2. The shipped damage block is the oracle shape (single `damage` call, `sl = &position`,
   `this->damage` rematerialised), not the tree body's `sl = dmg` cache.
3. The shipped tail order is source order `F P H L K`; clang preserves tail order.
4. `F P H L K` alone compiles to the low-pressure map.  Restoring the shipped map needs
   `__builtin_expect` pressure knobs; the best found (mine0+dist0+ki0 on the oracle
   body) reaches 87.6 % with the correct tail, slots, td join reload and damage block,
   but `i3` in `r6` and no `fp` join reload.
5. The `fp` asteroid-join reload is not reachable from any source perturbation tried;
   it is the LLVM 7 spill-placement region split around the `isAsteroid` block.

**Hypothesis (not proven):**
1. If the `i` live range were split at the `isAsteroid` successors (as in the shipped),
   the 6-byte shift would disappear and the ~62 branch mismatches would realign,
   taking the score to ≈ 94–95 %.  No source knob found moves the split.
2. The `explosive`/`td` spill-slot swap is a regalloc spill-order artefact (both
   prologues store in the same textual order), not a source-order difference.

## 4. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j4        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984
```

Scratch harnesses: `/dev/shm/w5515/try.py` (splice body into `Gun.base.cpp`, link, score),
`tryh.py` (splice into `Gun.head.cpp`), `score.py` (official verifier scorer),
`feat2.py` (register/slot map), `mkvar.py` (tail permutation), `perm_base_feat.py` /
`perm_oracle.py` / `perm_tailorder.py` (120-permutation sweeps), `combo2.py` / `combo3.py`
(81-point expect cross products), `sweep_od.py` (single-site expect sweep on the oracle
body).  Body files: `body_base.txt` (= HEAD), `body_oracledmg.txt`, `body_head.txt`,
`body_fphlk_mine0_dist0.txt`, `body_od_m0d0_ki0.txt`.

## 5. Files changed

`_work/reconstructions/gun-calccharactercollision-wave107-w5515.md` only (note).
Gate: byte 4500 / linked 4500, size-exact 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, `RATCHET PASS`.
