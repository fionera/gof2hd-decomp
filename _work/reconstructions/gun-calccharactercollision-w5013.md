# `Gun::calcCharacterCollision` pass 13: the `stack.10` region split is interference-driven,
# not CFG-driven — exhaustive `expect(mine,0)` tail search and six new knob families (w5013)

Worker: w5013. Branch: `wave100-w5013`, base master `44225192c`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base `0x10000`).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 instructions.
Scratch: `/dev/shm/w5013/`.

## 0. Result

**No code change landed.** The tree body (`F H K L P` tail, §2) is still the best measured
shape: **90.0 %**, 1984/1984 B, 585 of 650 normalized instructions matched (official verifier).

This pass adds the thing w4713 could not settle: the `stack.10` region split that decides
`this=r9` vs `this=r4` is **not** a CFG effect. The edge-bundle graph and every block frequency
are byte-for-byte identical between the two tail orders; only the *interference* seen by
`SpillPlacer` differs. Six new source-knob families were measured against that target and none
flips the split.

| build | gate byte/linked | target pct (official) | target size |
|---|---:|---:|---:|
| before (HEAD `44225192c`) | **4500 / 4500** | **90.0 %** | **1984 B** |
| after this pass (note only) | **4500 / 4500** | **90.0 %** | **1984 B** |

Full gate after: size-exact 4524/4524, data 843/843, imports 207, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, lint `CLEAN ✓`, drift 0, `RATCHET PASS`.

## 1. The two facts that pin the residual (re-verified)

| body | tail source order | pct | size | `this` | `enemy` | `i` | `i*3` | `td` slot | `explosive` slot |
|---|---|---:|---:|---|---|---|---|---|---|
| tree (HEAD) | `F H K L P` | **90.0** | **1984** | r9 | r8 | fp | r4 | [sp,#20] | [sp,#24] |
| shipped order | `F P H L K` | 64.8 | 1912 | r4 | r6 | r8 | — | r9 (unspilled) | [sp,#20] |
| shipped binary | `F P H L K` | — | 1984 | **r9** | **r8** | **fp** | **r4** | **[sp,#24]** | **[sp,#20]** |

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`, `H` = `setHitVector`,
`L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

So the shipped code is **the tree's `F H K L P` allocation with the shipped `F P H L K` tail
order**. The whole residual is: *make the `F P H L K` source order allocate like `F H K L P`.*

## 2. New: the split is interference-driven, not CFG-driven

w4713 showed the decisive trace lines:

```
HEAD   (F H K L P): $r11 static = 21.32312512, v=69, total = 110.4083111
                    with bundles EB#1 EB#6 EB#7 EB#8 EB#23 EB#24 EB#26 EB#27
                    EB#31 EB#32 EB#33 EB#39 EB#40 EB#41 EB#42 EB#43 EB#44 EB#45 EB#46
                    Split for $r11 in 12 bundles, intv 1.
FPHLK  (F P H L K): $r11 static = 21.32312512, v=69, total =  98.31180978
                    with bundles EB#1 EB#6 EB#7 EB#8 EB#23 EB#24 EB#26 EB#27
                    EB#31 EB#32 EB#33 EB#34 EB#35 EB#36 EB#37 EB#38 EB#39
                    EB#40 EB#41 EB#42 EB#43 EB#44 EB#45 EB#46
                    Split for $r11 in 24 bundles, intv 1.
```

The five extra bundles are `EB#34..#38`. This pass checked what they are:

* `llc -view-edge-bundles` on the stripped IR of both bodies produces **byte-identical dot
  files** (`md5 ce3491cbb9565f1999511ebd5658b0ac`, 537 lines each). The edge-bundle graph — and
  therefore the CFG, block numbering and block order — is the same for `F H K L P` and
  `F P H L K`.
* The per-block branch probabilities in the machine-code dump are also identical
  (`successors: ... %bb.78(37.50%), %bb.79(62.50%)` etc.).
* `EB#34..#38` are the edges of the damage / `isAsteroid` / `explosive` / `geometries` region
  (`bb.70`–`bb.83`), not the tail.

The only thing that changes is the **interference** `Cand.Intf` that `addThroughConstraints`
feeds to `SpillPlacer` while `growRegion` walks the through blocks. In `F H K L P` `%bb.92`
(the scatter block) is later isolated (`enterIntvBefore 16064r / leaveIntvAfter 16160r`,
creating `%466 [16056r,16160r)` with weight **2.818017e-01**); in `F P H L K` the same block is
`intf invalid-invalid, live-through 1 -> 1, straight through` and joins the main interval, so no
high-weight copy is ever created and `this` is never evicted from `$r4`.

Note that `EnableAdvancedRASplitCost` is **false on ARM** (`only X86Subtarget overrides
enableAdvancedRASplitCost()`, and `-condsider-local-interval-cost` defaults to 0), so
`splitCanCauseEvictionChain` is not consulted. The bundle set is decided purely by
`growRegion`/`SpillPlacer` from the interference pattern.

### 2.1 What `stack.10` actually is

`%307 = t2ADDri %stack.10, 0` is a single hoisted address value used at **three source sites**:

| trace site | source line | what |
|---|---|---|
| `3696r` / `3792r` | 606 / 607 | `getPlayer()->getPosition()` sret temp of the scatter-distance `mi` |
| `14688r` / `14880r` / `15056r` | 710 / 711 / 712 | `Matrix view` (`memcpy`, `MatrixSetTranslation`, `TransformSetLocal`) |
| `16064r` / `16160r` | 727 | `enemy->getPosition()` sret temp of the scatter loop |

`stack.10` is shared even more widely: two further `t2ADDri %stack.10, 0` values exist,
`%199` (lines 668/669, the mine non-asteroid `VectorNormalize(enemyPosition - position)`) and
`%209` (lines 670/671, the `velocities[i] * pitchRate * 100.0f` temp). The shipped binary uses
`sp+104` for `view` (`0x1533c8 add r5,sp,#104`) and for the scatter `getPosition` temp
(`0x153444 add r4,sp,#104`), so the sharing itself is faithful — the split, not the sharing, is
the residual.

## 3. Exhaustive `expect(mine,0)` tail search (the w4713 "highest-value experiment")

All 120 permutations of `{F,P,H,L,K}` were compiled **on top of**
`if (__builtin_expect(mine, 0)) radius *= 5;` (the only knob known to restore `this=r9` on the
shipped order). Full table in `/dev/shm/w5013/perm120exp.json`.

| rank | tail | pct | size |
|---|---|---:|---:|
| 1 | `F P H L K` (**shipped**) | **85.5** | 1984 |
| 2 | `F P H K L` / `F P L H K` / `F P K H L` / `K F P H L` | 85.2 | 1984 |
| 6 | `F P L K H` / `F P K L H` / `P H F L K` / `H F P L K` / `H L F P K` / `K F P L H` / `K F L P H` | 84.9 | 1984 |

No permutation beats `F P H L K` itself, and none reaches 90.0. `expect(mine,0)` is a trap for a
second reason: it produces a *clean swap* of the shipped map —

| value | shipped | `F P H L K` + `expect(mine,0)` |
|---|---|---|
| `this` | r9 | **r9** ✓ |
| `enemy` | r8 | r6 ✗ |
| `td` | r6 | r8 ✗ |
| `e` | [sp,#28] | [sp,#24] ✗ |
| `td` slot | [sp,#24] | [sp,#28] ✗ |
| `explosive` slot | [sp,#20] | [sp,#20] ✓ |
| mine block | inline | **out of line** ✗ |

## 4. Six new knob families measured (all against `F P H L K`)

### 4.1 `P`-block spelling (10 forms)

`if (playerGun)`, `!= 0`, no braces, `= true`, `!!`, `&&` expression, `if/else{}`,
`Level *lvl` local, `field_30 = playerGun ? 1 : field_30`, `| 0` — **all 64.8 / 1912** (except
the ternary store, 64.4 / 1916). The `P` spelling is irrelevant.

### 4.2 Tail scoping / caching

`{ … }` scope, `do{…}while(0)`, `if (true){…}`, `bool pg = playerGun` before/after `F`,
`(void)0` after `P`, cached `Level *lv` — **all 64.8 / 1912** (`pg_before_F` 64.8 / 1916).

### 4.3 `td` / `enemy` declaration order (on the `expect(mine,0)` base)

`Player *enemy` at function scope, `td` after the loop header, `td` inside the scatter block,
`Vector &td`, `Vector *const td` — **all 85.5 / 1984**. The `enemy`/`td` register swap is not
reachable from declaration order.

### 4.4 Scatter-temp / player-position / `view` shapes

| variant | pct | size |
|---|---:|---:|
| `Vector ep = enemy->getPosition(); *td = ep;` | 64.8 | 1912 |
| same + `expect(mine,0)` | 85.5 | 1984 |
| `*td = enemy->getPosition() - position;` | 64.8 | 1912 |
| `this->targetDir = …` (drop `td`) | 64.8 | 1912 |
| named `playerPos` local | 64.3 | 1912 |
| named `playerPos` + `ep` | 64.3 | 1912 |
| `Matrix view` hoisted to inner-loop top | 61.9 | 2032 |
| `Matrix view` hoisted to function top | 53.2 | 2008 |
| `float *viewp` then `Matrix view = *(Matrix*)viewp` | 64.8 | 1912 |
| `Matrix view;` then assignment | 64.8 | 1912 |
| `Vector ep` hoisted to inner-loop top (overlaps `view`) | 66.8 | 1976 |
| `Vector position/velocity` hoisted above the inner loop | 62.4 | 1972 |

### 4.5 Block order `{X(explosive/geometries), I(impact), S(scatter)}` on `F P H L K`

`X I S` 63.9 / 1984, `S X I` 55.6 / 1900, `S I X` 55.7 / 1896, `I S X` 66.4 / 1952,
`I X S` 63.9 / 1916. (`X I S` is the shipped order.)

### 4.6 `__builtin_expect` sweep

On `F P H L K`: `mine` (radius) 0→85.5 / 1→32.6; `distance` 0→84.8 / 1→64.8; `geometries` 0→79.1;
`scatter` 1→79.0; every other site ≤ 64.8.
On the tree body (`F H K L P`): `distance` 1→85.2 / 0→69.3; `isAsteroid` 0→59.0;
`mine` (asteroid) 0→54.1 / 1→59.1; `ki` 0→61.7; everything else worse.

### 4.7 Local-declaration order on the tree body

`position,velocity,enemyPosition` permutations: `pve` **90.0**, `pev` 86.8, `vpe` 86.8,
`vep` 84.8, `epv` 84.0, `evp` 83.7. The tree order is optimal.

### 4.8 Randomised multi-knob searches

* 280 samples over {9 tail orders} × {6 scatter forms} × {3 `view` forms} × {2 player-pos forms}
  × {`expect(mine,0)` on/off} × {`dmg` on/off}: best **85.5 / 1984** (all winners carry
  `expect(mine,0)`).
* 260 samples adding `ep` hoisting: best **85.5 / 1984**.
* `dmg`-local removal on `F P H L K`: 64.1 / 1920 (and 79.4 / 1996 with `member` + named
  player pos + `expect(mine,0)`).

### 4.9 Caching a member at function scope (new live value)

| cached value | pct | size |
|---|---:|---:|
| none (current) | 64.8 | 1912 |
| `Level *lvl = (Level *) this->level;` | 65.9 | 1936 |
| `Array<Player*> *enemies = this->enemies;` | 48.7 | 1924 |
| `lvl` + `expect(mine,0)` | 56.8 | 1900 |
| `lvl` on the tree body | 65.6 | 1936 |

Adding a long-lived value does perturb the interference, but never in the direction that
restores the shipped map.

### 4.10 Combined `expect(mine,0)` × `expect(distance,0/1)` × 10 tail orders

80 bodies; best is still `F P H L K` + `expect(mine,0)` alone at **85.5 / 1984**. Adding a
`distance` hint on top drops to ≤ 84.8.

## 5. Conclusion

The residual is exactly what w4713 said — the `stack.10` region split — but the lever is not the
CFG and not the tail statement order: the edge-bundle graph and block frequencies are identical
between the two tail orders, and the split pattern is chosen from the *interference* that
`SpillPlacer` sees on the through blocks. `F P H L K` happens to make `%bb.92` interference-free
for `$r11`, so the scatter use joins the main interval and the high-weight `%466` copy that
evicts `this` from `$r4` is never materialised. Every source-level shape tried in this pass
(and in w3805/w3829/w3909/w4010/w4110/w4211/w4229/w4236/w4244/w4261/w4416/w4516/w4713) either
leaves the codegen identical or changes the size away from 1984. The function remains a strong
local optimum at **90.0 % / 1984 B**.

## 6. Reproduction

```
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984 byte=False linked=False
```

Scratch harness (`/dev/shm/w5013/`):

| file | purpose |
|---|---|
| `try.py <body>` | splice a body into `Gun.cpp`, build `link_so`, score |
| `fastscore.py` | official-verifier-based scorer; reproduces the official pct exactly |
| `Gun.cpp.orig`, `body_head.txt`, `body_fphlk.txt` | pristine source + the two bodies |
| `perm120exp.py`, `perm120exp.json` | 120 tail permutations on `expect(mine,0)` |
| `sweep_exp.py`, `sweep_p.py`, `sweep_s.py`, `sweep_decl.py`, `sweep_scatter.py`, `sweep_head_exp.py` | the knob families in §4 |
| `gen.py` / `gen_results.json`, `gen2.py` / `gen2_results.json` | the randomised multi-knob searches |
| `mkir.sh` | emit the exact project IR for a body |
| `ra_head_pic.txt`, `ra_fphlk_pic.txt` | clang-7.0.2 greedy-allocator traces |
| `EdgeBundles-8eb1de.dot`, `dothead/EdgeBundles-816ad5.dot` | the identical edge-bundle graphs |
| `orig.dis`, `ours_head.dis`, `ours_expmine.dis` | disassembly of the original and the two best bodies |

## 7. Files changed

None under `src/` (note only). Gate: byte 4500 / linked 4500, size-exact 4524/4524,
data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
lint CLEAN, drift 0.
