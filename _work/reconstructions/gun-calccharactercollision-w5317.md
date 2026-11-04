# `Gun::calcCharacterCollision` pass 17: fresh REA decompile re-confirms the shipped
# `F P H L K` tail and the `int dmg` phi, and the best `F P H L K` body is two
# registers (enemy/td) away from the shipped map (w5317, wave 105)

Worker: w5317. Branch: `wave105-w5317`, base master `835748c18`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Scratch: `/dev/shm/w5317/` (REA output `rea.json` / `rea_decomp.txt`, disassemblies,
sweep harnesses).

## 0. Result

**No code change landed.** The tree body (`F H K L P` tail + `int dmg` local) is still the
best measured shape: **90.0 %**, 1984/1984 B, 585/650 normalized instructions (official
verifier). Gate after (unchanged): byte **4500/4524**, linked **4500/4524**, size-exact
**4524/4524**, data **843/843**, imports **207**, lint **CLEAN**, drift **0**, `RATCHET PASS`.

| build | gate byte/linked | target pct | target size | target matched |
|---|---:|---:|---:|---:|
| before (HEAD `835748c18`) | **4500 / 4500** | **90.0** | **1984** | **585/650** |
| after this pass (note only) | **4500 / 4500** | **90.0** | **1984** | **585/650** |

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`,
`H` = `setHitVector`, `L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

## 1. New REA evidence (independent of the previous Ghidra notes)

`rea decompile` on the stripped original (`GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC`)
was run fresh for this pass.  The full normalized result is committed alongside this note as
`gun-calccharactercollision-w5317-rea.txt`.  Three things it settles:

1. **Both loops are top-tested `for` loops.**  `for (uVar18 = 0; uVar18 < *puVar8; ...)` and
   `for (uVar21 = 0; uVar21 < *(uint *)(this + 8); ...)`.  This agrees with the shipped
   disassembly (`b.n 0x582` pre-test; `bcc` back edge) and rules out the oracle's `do/while`.
2. **The tail order is exactly `F P H L K`:**
   `this_02[0x54] = 1; if (this[0xf0]) level->field_30 = 1; setHitVector(...);
   lifetimes[i] = -1000000; lastHitKIPlayer = getKIPlayer();`
3. **The damage block is an `int dmg` phi with the two plain arms merged into one block.**
   The decompile shows the docked/ego arms jumping to a shared `LAB_001632e0`
   (`itemIndex; friendGun; dmg = (int)f`), while both `dmg = damage` arms jump to a single
   `LAB_00163304` (`itemIndex; dmg = damage; friendGun`), then one shared call.
   The shipped disassembly matches: float arg-setup at `0x1532e0`, plain arg-setup at
   `0x153304`, one `damage` call at `0x153310`.

The rest of the decompile (bomb/mine/explosive/explodeOnContact flags, radius branch,
mine box check, asteroid/medal blocks, scatter loop, impact block) matches the tree body
statement-for-statement, so the residual is **pure codegen**, not a missing source block.

## 2. The shipped register map, and the two bodies that nearly reach it

| value | shipped | HEAD (`F H K L P` + `dmg`) | `F P H L K` + `dmg` + `expect(mine,0)` | `int dmg` phi (`F H K L P`) |
|---|---|---|---|---|
| `this` | r9 | **r9** | **r9** | r9 |
| `enemy` | r8 | **r8** | r6 | r6 |
| `i` | fp (spill `[sp,#40]`) | `[sp,#40]` | **fp** | **fp** |
| `i*3` | r4 | **r4** | **r4** | r5 |
| `td` | r6 (spill `[sp,#24]`) | `[sp,#20]` | r8 | r9 |
| `explosive` | `[sp,#20]` | `[sp,#24]` | `[sp,#20]` | `[sp,#20]` |
| pct / size / insns | — | **90.0 / 1984 / 650** | 85.5 / 1984 / 650 | 84.1 / 1980 / 648 |

The `expect(mine,0)` body is the closest map: it already has `this=r9`, `i=fp`, `i*3=r4`;
only `enemy`/`td` are swapped (r6/r8).  If that swap is fixed, the shipped map is reached.

## 3. Why the map matters: a 6-byte layout shift

HEAD's 65 mismatches break down as:

| category | ~count | cause |
|---|---:|---|
| branch-target literals | 20 | code layout shifted 6 B at `0x153196` |
| spill-slot swaps | 8 | `td`/`explosive` at `[sp,#20]`/`[sp,#24]` swapped |
| spill reloads | 6 | shipped reloads `fp` (`i`) and `r6` (`td`) at `0x153198`/`0x15319c` |
| `dmg` local | 8 | HEAD caches `this->damage` in `sl`; shipped reloads at all 3 arms |
| tail order | 15 | `F H K L P` vs `F P H L K` |
| const remat | 5 | shipped `movs r0,#1`; HEAD keeps `1` in `fp`/`r5` |

The 6-byte shift is exactly the two shipped reloads at `0x153198`:
`ldr.w fp,[sp,#40]` (4 B) + `ldr r6,[sp,#24]` (2 B).  HEAD keeps `i` and `td` in memory, so
those two instructions are absent and every later function-relative branch target differs.
Removing the shift alone would recover ~20 instructions (~93 %).

## 4. What was tried this pass (all measured with the official verifier)

* **120 tail permutations** on the HEAD (`int dmg`) base: `F H K L P` is a sharp optimum
  (90.0); `F K H L P` and `K F H L P` 89.7; next best 85.3.  Shipped `F P H L K` = 64.8
  (1912 B) because the map flips to `this=r4`.
* **`F P H L K` knob sweep** (40 builds): `expect(mine,0)` on the radius branch is the only
  hint that restores `this=r9` (85.5 / 1984).  `expect` on isActive / hitpoints / asteroid /
  scatter / explodeOnContact / impact / ki all lose.  Do-while scatter + `expect(distance,0)`
  = 86.4 / 1984; adding `expect(mine,0)` drops to 79.7.
* **Damage-block shapes** (15 builds): `int dmg` phi (84.1 / 1980, 648 insns), direct calls
  (84.0 / 1996), inverted conditions (84.1), plain-first (84.1), scoped variants.  The
  `int dmg` phi is semantically faithful but the compiler hoists `ldr r3`/`ldrb r2` into the
  shared tail instead of duplicating them into the two arg-setup blocks, so it is 2
  instructions short of the shipped 650.
* **Declaration/type/loop knobs**: `position/velocity/enemyPosition` permutations (`pve`
  optimal), `unsigned`↔`int` loop vars, function-scope `i`/`enemy`, `bool`↔`int` flags,
  `true`/`(uint8_t)1` constant spellings, `hitFlags[i]=true`, member caching
  (`positions`/`velocities`/`levels`/`lifetimes`), `td` placement (before/inside/after
  loops, reference form, `this->targetDir` direct), radius `if/else` vs ternary.
  None changes the pct.

## 5. Next hypothesis

The lever is the region-split of `%stack.10` that decides `this=r9` vs `this=r4` (see the
w5013 note).  `F P H L K` + `expect(mine,0)` already produces the shipped `this`/`i`/`i*3`
assignment; only `enemy` (r6) and `td` (r8) are swapped.  The greedy allocator gives `r6`
to whichever of the two intervals it reaches first / with the higher spill weight.  A
source perturbation that makes `td`'s live range start earlier (or `enemy`'s denser) should
flip them.  Candidates not yet exhausted: hoisting the `td` computation above the
`lastHitKIPlayer = 0` store *and* keeping the post-increment idiom (`str r2,[r6],#216`),
or introducing a second `td` use in the radius branch.  Once the swap lands, the two
`0x153198` reloads should reappear and the layout should align to the shipped 650-insn
stream.

## 6. Reproduction

```
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984
```

Scratch harnesses: `/dev/shm/w5317/fastscore.py` (official-verifier scorer),
`/dev/shm/w5317/try.py <body>` (splice + build + score), `sweep_dir.py <dir>` (batch sweep),
`perm/` (120 tail permutations), `mx2/`, `sc/`, `sc2/`, `dmg2/`, `inv/`, `decl/`, `td/`,
`td2/`, `cache/`, `const/`, `const2/`, `types/`, `loops/`, `hyb/`, `fin/`, `other/`.

## 7. Files changed

None under `src/` (note only).
