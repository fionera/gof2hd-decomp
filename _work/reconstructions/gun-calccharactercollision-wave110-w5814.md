# `Gun::calcCharacterCollision` wave 110 (w5814): the shared `1` constant is why the
# shipped map keeps `i` in `fp`; no source shape reaches `i3=r4 ∧ join_fp`

Worker: w5814. Branch: `wave110-w5814`, base master `390f58052`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Scratch: `/dev/shm/w5814/`.

## 0. Result

**No code change landed.** HEAD (`src/game/weapons/Gun.cpp`: `int dmg = this->damage` +
four `enemy->damage(...)` call sites + tail `F H K L P`) is still the best measured shape:
**90.0 %**, 1984/1984 B, 585/650 normalized instructions (official verifier).

| build | gate byte/linked | target pct | target size | target matched |
|---|---:|---:|---:|---:|
| before (HEAD `390f58052`) | **4500 / 4500** | **90.0** | **1984** | **585/650** |
| after this pass (note only) | **4500 / 4500** | **90.0** | **1984** | **585/650** |

Gate after: byte 4500/4524, linked 4500/4524, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`. No ELF section / `.ARM.extab` change
(no code change; the 28-section / `.jcr` 0xc0 / `.ARM.extab` `0x26294` / file-size `+44`
state of wave 108 is untouched).

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`,
`H` = `setHitVector`, `L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

## 1. New this pass: *why* `i` is not in `fp` in HEAD

The shipped code keeps the loop counter `i` in `fp` for the whole post-`isAsteroid` region
(reloaded at both joins, `ldr.w fp,[sp,#40]` at 0x153198 / 0x153262 / 0x1533f2).  HEAD
reloads `i` per use instead (`ldr r2,[sp,#40]` at 0x153330, …).  The reason is the **shared
`1` constant**:

* In the MIR (`-mllvm -print-after-all`, `mir_greedy_head.txt`) the two `= 1` stores of the
  tail (`field_54[0] = 1` and `level->field_30 = 1`) are one rematerializable value
  `%449:rgpr = t2MOVi 1` (13112B) used at 13120B (F) and 13568B (P).
* With tail `F H K L P` those two uses straddle the `setHitVector` (13312B) and
  `getKIPlayer` (13376B) calls, so `%449` needs a **callee-saved** register.  `fp` is free
  there and the allocator gives it `fp` (`mov.w fp,#1` at 0x153304).  `fp` is therefore
  unavailable for `i`, which is rematerialised from `[sp,#40]` at each use.
* With tail `F P H L K` (the shipped order) F and P are adjacent, `%449` goes to `r1`
  (caller-saved, `movs r1,#1` at 0x153316 in the original) and `fp` is free — but then the
  allocator keeps `i` in `r8` (not `fp`) and demotes `this` to `r4`, `enemy` to `r6`
  (`tree/FPHLK` = 64.8 %, 1912 B).  So freeing `fp` is necessary but not sufficient: the
  shipped map also needs `this=r9`, `enemy=r8`, `td=r6`, which no F-P-adjacent tail tried
  reproduces.

This is the mechanism behind wave 107/108/109's "`join_fp=True` ⇒ `i3≠r4`" anti-correlation:
the same register-pressure knob that frees `fp` for `i` also re-colours `this`/`enemy`/`i*3`.

## 2. Measured table (official verifier; `link_so` + `tools/verify/verify.py` scoring)

### 2.1 All 120 tail permutations of the tree body (re-confirmed)

| tail | pct | size | `this` | `i3` | `join_fp` | `join_td` | `expl` | `td` |
|---|---:|---:|---|---|---|---|---|---|
| **`F H K L P` (= HEAD)** | **90.0** | **1984** | **r9** | **r4** | False | False | 24 | 20 |
| `K F H L P` | 89.7 | 1984 | r9 | r4 | False | False | 24 | 20 |
| `F K H L P` | 89.7 | 1984 | r9 | r4 | False | False | 24 | 20 |
| `P H K F L` | 85.3 | 1980 | r9 | r4 | False | False | 24 | 20 |
| `F H P L K` | 85.3 | 1976 | r9 | r4 | False | False | 24 | 20 |
| `P K H F L` | 85.0 | 1980 | r9 | r4 | False | False | 24 | 20 |
| `F H P K L` / `F H K P L` | 84.8 | 1976 | r9 | r4 | False | False | 24 | 20 |
| `K F H P L` / `F K H P L` | 84.7 | 1976 | r9 | r4 | False | False | 24 | 20 |
| `F P H L K` (shipped source order) | 64.8 | 1912 | r4 | r5 | False | False | 24 | None |
| all remaining 108 tails | ≤ 76.2 | ≤ 1980 | r4 | r5 | False | False | 24 | — |

Only **10 of 120** tails keep `this=r9` (`FHKLP`, `KFHLP`, `FKHLP`, `PHKFL`, `FHPLK`,
`PKHFL`, `FHPKL`, `FHKPL`, `KFHPL`, `FKHPL`); **none** has `join_fp=True`.

### 2.2 Three-site `__builtin_expect` subsets on the shipped tail `F P H L K`

220 subsets × {tree, phi} damage shapes:

| variant | pct | size | `i3` | `join_td` | `expl` | `td` |
|---|---:|---:|---|---|---|---|
| phi, `FPHLK`, expect(mine)+expect(ki71)+expect(dist) | **87.6** | 1984 | r6 | **True** | **20** | **24** |
| tree, `FPHLK`, expect(dist)+expect(emp)+expect(owner) | 87.3 | 1984 | r5 | True | 20 | 24 |
| tree, `FPHLK`, expect(mine)+expect(dist)+expect(owner) | 85.4 | 1984 | r6 | True | 20 | 24 |
| phi, `FPHLK`, expect(mine)+expect(dist)+expect(emp) | 83.3 | 1984 | r6 | True | 20 | 24 |

The 87.6 % row is wave 109's `faithful2`; it is the closest source-level reconstruction
(correct tail, both slots, `td` join reload, single-call damage block) but has `i3=r6` and
no `fp` join reload.

### 2.3 `F P H L K` + `expect(playerGun,1)` (new corner)

`expect(playerGun,1)` is codegen-identical on the HEAD tail (90.0) but changes the FPHLK
family:

| variant | pct | size | `i3` | `join_td` | `expl` | `td` |
|---|---:|---:|---|---|---|---|
| `FPHLK` + pg1 + expect(dist)+expect(emp) | **88.0** | 1984 | r5 | True | 20 | 24 |
| `FPHLK` + pg1 + expect(mine) | 85.5 | 1984 | **r4** | False | 24 | None |
| `FPHLK` + pg1 + expect(dist) | 84.8 | 1980 | r5 | True | 20 | 24 |
| `FPHLK` + pg1 alone | 64.8 | 1912 | r5 | False | 24 | None |

### 2.4 Other spaces swept this pass (all ≤ 90.0)

| space | best |
|---|---|
| prologue statement order (12 permutations of `explosive=…`, `if(bomb\|\|mine)`, `lastHitKIPlayer=0`, `td` decl) | 90.0 (original order only; all others ≤ 84.8) |
| `{tree,phi,direct}` × `{FHKLP,FPHLK,FKHLP,KFHLP}` | 90.0 (tree/FHKLP) |
| single-site `expect` on the *tail* `if (explosive)`, `geometries`, `playerGun`, `getHitpoints`, `isActive`, `enemies==nullptr`, asteroid-block `if (mine)`, outer `if (mine)`, `enemyFlagsLo`, `deadFlag`, `egoFlag`, `outer`, `useCustomRadius` | ≤ 90.0 (pg1 and the existing HEAD sites are codegen-identical) |
| 250-sample random edit fuzz (declaration/type/cast/parenthesis/statement edits on HEAD) | 90.0 (all hits codegen-identical) |
| 200-sample random `{damage shape × tail × 0–2 expects}` | ≤ 85.2 |
| 200-sample random `{damage shape × tail × prologue order × 0–2 expects}` | ≤ 84.8 |

## 3. Proven vs hypothesis

**Proven (measured this pass):**
1. HEAD is still the maximum over: all 120 tail permutations (with `this`/`i3`/`join_*`/slot
   signature recorded), 440 three-expect subsets on `FPHLK`, the `playerGun`-expect family,
   12 prologue orders, the 13 single-site expect corners above, and 450 random samples.
2. The shared `1` constant is the reason `fp` is not available for `i` in HEAD: the MIR shows
   one rematerializable `%449 = t2MOVi 1` used at F (13112B) and P (13568B), straddling the
   `setHitVector` and `getKIPlayer` calls, so it is assigned `fp`.
3. `F P H L K` alone makes the constant caller-saved (`r1`) and frees `fp`, but the allocator
   then keeps `i` in `r8` and demotes `this`→`r4`, `enemy`→`r6` (low-pressure map, 1912 B).
4. Every `F P H L K` variant that restores the shipped `expl=20 / td=24` slots also has
   `i3 ∈ {r5,r6}` and `join_fp=False` (88.0 % max).
5. The `explosive`/`td` slot order is tied to the tail order (FHKLP ⇒ 24/20, FPHLK ⇒ 20/24);
   it is not reachable by declaration order, bool/int spelling, or `playerGun`/`mine`
   expects (all codegen-identical or worse).

**Hypothesis (not proven):**
1. The shipped `i3=r4 ∧ join_fp=True` needs the constant-1 live range to be short (tail
   `F P H L K`) *and* the `this=r9 / enemy=r8` assignment to survive; the two are driven by
   the same greedy-allocator priority order, so no statement-level source change reaches it.
2. The remaining 65 mismatches are ~25 branch-target shifts (caused by the two 6-byte join
   reload pairs being placed at the emitManual/geometry blocks instead), 5 slot-swap
   instructions, ~10 damage-block scheduling instructions, and ~15 tail-order instructions.
   If the join reloads moved, the branch targets would realign and the score would be ≈ 94–95 %.

## 4. Reproduction

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

Scratch harnesses (`/dev/shm/w5814/`): `try.py <body.cpp>` (splice body into `Gun.cpp`,
`link_so`, score), `fastscore.py` (official-verifier scorer for one symbol),
`sig2.py` / `sig3.py` (5-field and `this`+5-field signatures), `normdiff.py` (verifier's own
normalized instruction diff), `sweep2.py` (120-tail sweep), `sweep4.py` (tail + `this` map),
`sweep5.py` (FPHLK × 0–2 expects), `sweep6.py` (FPHLK × all 3-expect subsets),
`rand2.py` / `rand3.py` / `fuzz.py` (random searches), `batch2.py`–`batch7.py` (targeted
edits), `body_head.cpp` / `faithful2.cpp` (drop-in bodies), `mir_greedy_head.txt` and
`paa.txt` (`-mllvm -print-after-all` dumps).

## 5. Files changed

None under `src/` (note only).
