# `Gun::calcCharacterCollision` pass 11: the `__builtin_expect(mine,0)` trap, the exact
# 52-hunk residual, and an exhaustive tail-order search (w4516)

Worker: w4516. Branch: `wave95-w4516`, base master `4a88bd6e`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base `0x10000`). Scratch: `/dev/shm/w4516/`.
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`.

## 0. Result

**No code change landed.** The tree body (`E A B C D` tail, see §2) is still the best measured
shape: **90.0 %**, 1984/1984 B, **585 of 650** normalized instructions matched. This pass adds:

1. an **exact catalogue of the 52 residual diff opcodes** (43 replace / 5 insert / 4 delete,
   22 of the replaces are branch-target-only) with the marginal score value of each;
2. proof that the previously reported `__builtin_expect(mine, 0)` "lever" is a **trap**: it
   does move `this` to `r9`, but it flips `enemy`/`td` and moves the mine block out of line,
   so it measures 85.5 % — *worse* than the current body;
3. an **exhaustive search of all 120 permutations** of the five post-damage statements
   (× `dmg` local on/off × mine-expect on/off = 240 bodies) — `E A B C D` wins outright;
4. the reason the residual is not reachable from the tail: **any** tail reorder drops the
   function to 1912 B (`this` → `r4`), which by itself fails size-exact.

| build | gate byte/linked | target pct (official) | target size |
|---|---:|---:|---:|
| before (HEAD `4a88bd6e`) | **4500 / 4500** | **90.0 %** | **1984 B** |
| after this pass (note only) | **4500 / 4500** | **90.0 %** | **1984 B** |

Full gate after: size-exact 4524/4524, data 843/843, imports 207, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, lint `CLEAN ✓`, drift 0, `RATCHET PASS`.

## 1. The shipped register map and stack slots (re-verified from the binary)

Shipped `0x152f90`:

| value | reg | stack |
|---|---|---|
| `this` | `r9` | — |
| `enemy` | `r8` | — |
| `td` (`&this->targetDir`) | `r6` | `[sp,#24]` |
| `i` | `fp` | `[sp,#40]` |
| `i*3` | `r4` | — |
| `&position` | `sl` (from the damage block on) | `position` at `sp+192` |
| `explosive` | — | `[sp,#20]` |
| `explodeOnContact` | — | `[sp,#16]` |
| `e` | — | `[sp,#28]` |
| `scatter` | — | `[sp,#32]` |
| `mine` | — | `[sp,#36]` |

Frame `208`. `td` is materialised by the `lastHitKIPlayer = 0` store:
`mov r6,r9; str.w r2,[r6],#216; str r6,[sp,#24]` — i.e. `this->lastHitKIPlayer = 0;` followed
by `Vector *td = &this->targetDir;` is exactly the shipped source shape, and the current tree
already emits it.

The shipped **tail** order (after `enemy->damage(...)`) is, verbatim from `0x153316`:

```
E  reinterpret_cast<uint8_t*>(&enemy->field_54)[0] = 1;   movs r1,#1 ; strb.w r1,[r8,#84]
D  if (this->playerGun) level->field_30 = 1;              ldrb.w r0,[r9,#240] ; cbz ; strb.w r1,[r0,#48]
A  enemy->setHitVector(-velocity.x,-velocity.y,-velocity.z);  ldmia/eor x3 ; blx setHitVector
C  this->lifetimes[i] = -1000000;                         ldr.w r0,[r9,#60] ; ldr r1,[pc,#916] ; str.w r1,[r0,fp,lsl#2]
B  this->lastHitKIPlayer = (int)(intptr_t) enemy->getKIPlayer();  blx getKIPlayer ; str.w r0,[r9]
```

The current tree emits `E A B C D`. That is the **only** remaining source-order divergence in
the whole function; every other block is in shipped order.

## 2. The exact 52-hunk residual of the current body

`tools/verify/verify.py --show` on HEAD: 650 vs 650 instructions, 585 matched, 90.0 %.
`SequenceMatcher` opcodes: **43 replace, 5 insert, 4 delete**; 22 of the replaces differ only
in a `loc_*` branch target (they disappear as soon as the block layout matches).

The non-branch-target hunks, grouped:

| # | hunk | orig | ours | cause |
|---|---|---|---|---|
| a | `O[32]`, `O[46]`, `O[185]`, `O[371]`, `O[517]` | `explosive=[sp,#20]`, `td=[sp,#24]` | `td=[sp,#20]`, `explosive=[sp,#24]` | spill-slot order (`§5`) |
| b | `O[257]`, `O[261:263]`, `O[276:279]`, `O[292:295]` | `add.w sl,sp,#192` / `ldr.w r1,[r9,#96]` / `vldr s0,[r9,#96]` | `ldr.w sl,[r9,#96]` / `vmov s0,sl` / `mov r1,sl` | `int dmg` local (`§4`) |
| c | `O[178:180]`, `O[242:244]`, `O[327:328]` | `ldr.w fp,[sp,#40]; ldr r6,[sp,#24]` reloads | absent | RA spilling |
| d | `O[296:302]`, `O[303]`, `O[309:312]`, `O[314]`, `O[315:317]` | `E D A C B` | `E A B C D` | tail order (`§3`) |
| e | `O[375:376]`, `O[377:380]`, `O[388:390]` | `movs r0,#1; strb.w r0,[r8,#85]` / `movs r1,#1; strb.w r1,[r0,fp]` | `movs r5,#1` hoisted and reused | RA / hoisting |
| f | `O[6]`, `O[34]`, `O[36]`, `O[250]`, `O[323]` | different `[pc,#N]` / literal-pool offsets | — | consequence of the code-size delta |

Marginal score of each hunk if it alone were fixed (measured by splicing the orig instructions
into our stream): `O[315:317]` (the `explosive` test placement) **90.7**, `O[296:302]` (the
`E D` prefix) **90.6**, `O[377:380]` **90.4**, `O[276:279]` **90.4**, `O[327:328]` **90.3**,
`O[309:312]` **90.3**, `O[517]` **90.2**, … The residual is a chain of ~0.2–0.7 pt hunks.
Swapping *only* the `explosive`/`td` stack slots (a pure textual `[sp,#20]↔[sp,#24]` swap in
the instruction stream) is worth **+0.9 pt → 90.9 %** — the single largest coherent fix.

## 3. The tail order: exhaustive search (new)

All 120 permutations of the five post-damage statements `E A B C D` were compiled and measured
on the current body (240 bodies including `dmg` on/off and mine-expect on/off):

| rank | tail order | pct | ours size |
|---|---|---:|---:|
| 1 | `E A B C D` (**current**) | **90.0** | 1984 |
| 2 | `B E A C D` / `E B A C D` | 89.7 | 1984 |
| 3 | `E A D C B` (shipped, `dmg` local, `expect(mine,0)`) | 85.5 | 1984 |
| 4 | `E A D C B` (shipped, `dmg` local, no expect) | 85.3 | 1976 |
| 5 | `E A B D C` | 84.8 | 1976 |

The shipped order `E D A C B` **with no other change** measures **64.8 % / 1912 B**: the
reorder moves `this` out of `r9` into `r4`, and 1912 ≠ 1984 fails size-exact on its own.
So the tail order cannot be fixed in isolation.

## 4. `__builtin_expect(mine, 0)` is a trap, not a lever

The previous note reported `expect(mine,0)` as "the" way to recover the shipped allocation.
It does move `this` back to `r9`, but it changes three other things at once:

| body | pct | ours size | `this` | `enemy` | `td` | mine block |
|---|---:|---:|---|---|---|---|
| `E A B C D`, no expect (HEAD) | **90.0** | **1984** | `r9` | `r8` | `r6` | **inline** (`cbz r0,loc_132`) |
| `E D A C B`, no expect | 64.8 | 1912 | `r4` | `r6` | `r9` | inline |
| `E D A C B`, `expect(mine,0)` | 85.5 | 1984 | `r9` | `r6` | `r8` | **out of line** (`bne.w loc_574`) |
| `E A B C D`, `expect(mine,0)` | 81.8 | 1992 | `r9` | `r6` | `r8` | out of line |

The shipped code has the mine block **inline** (`cbz r0,0x132; add.w fp,fp,fp,lsl#2; b.n 0x1d0`)
and `enemy=r8`, `td=r6`. `expect(mine,0)` produces the opposite in both respects, so the
apparent "improvement" over the 1912 B body is a different, also-wrong allocation.

`expect(mine,0)` also moves the `explosive` slot to `[sp,#20]` (correct) but swaps `td`/`e`
(`td=[sp,#28]`, `e=[sp,#24]`; shipped `td=[sp,#24]`, `e=[sp,#28]`).

## 5. The `explosive` / `td` spill-slot swap

Shipped spill order in the entry block: `explosive` → `stack.0` (`sp+20`), `td` → `stack.1`
(`sp+24`). The current body gets them the other way round. The store order in the entry block
is identical in both (`str r2,[sp,#20]` for `explosive` at index 32, `str r6,[sp,#24]` for `td`
at index 46), so this is the **greedy allocator's spill order**, not a source-order effect.

Everything tried to move it, all codegen-identical to HEAD (90.0 / 1984):

* `int explosive = 0;`, `int explosive = -1;`, `bool explosive;`
* `explosive` declared at the top / after `bomb` / after `scatter` / after `explodeOnContact` /
  at its assignment
* `explodeOnContact` declared after the `if (bomb||mine)` block
* `Vector *td` moved before `lastHitKIPlayer = 0`, after the `explosive` assignment, after the
  `if (bomb||mine)` block, into the outer loop, into the inner loop, into the `scatter` block
* `Vector &td = this->targetDir;`, `Vector *const td`, `Vector *td = (Vector*)&this->targetDir;`
* swapping the `explosive`/`explodeOnContact` declarations

Moving `td` to the top of the function (right after the `enemies == nullptr` guard) is the only
variant that changes anything, and it is much worse (66.2 / 1964).

## 6. The `int dmg` local (the `sl` register)

Shipped reloads `this->damage` at all three damage sites (`0x1532b8`, `0x1532d4`, `0x153308`)
and keeps `sl = &position` from the damage block to the end of the loop body. The tree caches
`int dmg = this->damage;` in `sl` and addresses `position` directly as `sp+192`.

Removing the local does give `sl = &position` (matching shipped), but:

| damage-block shape | pct | ours size |
|---|---:|---:|
| `int dmg` local (HEAD) | **90.0** | **1984** |
| no local (`this->damage` in both plain arms) | 84.0 | 1996 |
| no local, `(int) this->damage` in the else arm | 84.0 | 1996 |
| no local, nested `else { if (egoFlag) … else … }` | 84.0 | 1996 |
| no local, `int dmg` only in the else-if arm | 84.0 | 1996 |
| `int &dmg = this->damage;` | 84.0 | 1996 |
| `const int dmg`, `unsigned dmg`, `int dmg` declared late | 90.0 | 1984 |

The 12-byte excess is a **duplicated plain-damage block**: with no local, clang hoists
`ldr.w r1,[r9,#96]` above `cmp r0,#0` in the `else if (enemy->egoFlag)` arm, so the shared
"plain" block (`ldr r3,[r9,#88]; ldr r1,[r9,#96]; ldrb r2,[r9,#249]; blx damage`) is emitted
twice instead of once. In the shipped code it is a single join block reached from three
predecessors. No source spelling tried (cast, nested if, per-arm local, branch inversion)
prevents the hoist.

## 7. Other measured knobs (all on the current body)

| knob | pct | size |
|---|---:|---:|
| `__builtin_expect` on scatter / explosive / geometries / impact / empDamage / playerGun / explodeOnContact | 32–79 | 1888–2000 |
| `int` loop indices (`e`, `i`, `k`) | 89.8 | 1984 |
| `int mine/scatter`, `bool explosive`, `bool explodeOnContact`, `int weaponType` | 90.0 | 1984 |
| `hitFlags[i] = true`, `field_54[0] = true`, `(int) enemy->getKIPlayer()` | 90.0 | 1984 |
| `int radius = enemy->radius; if (useCustomRadius) radius = customRadius;` | 90.0 | 1984 |
| `bool outer = ki && ki->field_0x40;` / `if (!outer \|\| !ki->outerCollide(position))` | 90.0 | 1984 |
| `velocity.x + enemyPosition.x - position.x` (all three axes) | 85.7 | 1984 |
| `> -r` instead of `> (float) -radius` | 81.1 | 1988 |
| `position`/`velocity` declaration order swapped | 61.8 | 1912 |
| `KIPlayer *ki` hoisted to the top of the inner loop | 58.0 | 1932 |
| `float r` hoisted to the top of the inner loop | 48.5 | 1568 |
| `if (mine) radius *= 5;` then `if (!mine && scatter)` | 66.7 | 1984 |
| `mine == 1` / `(bool)mine` / `mine != 0` / `!!mine` / `mine > 0` | 64.8 | 1912 |

A 60-sample random search over 13 independent knobs (tail order, `dmg`, seven `__builtin_expect`
sites, `explosive`/`mine`/`scatter` types, loop-index types) found nothing above 90.0.

## 8. Conclusion

The function is a **strong local optimum**. The shipped source order is `F P H L K` for the
inner-loop blocks (the tree already matches it) and `E D A C B` for the post-damage tail (the
tree has `E A B C D`). The tail is the last source-order divergence, but it is locked to the
register allocation: any reorder moves `this` from `r9` to `r4` and shrinks the function to
1912 B, which fails size-exact. The only knob found that restores `r9` under the shipped tail
(`__builtin_expect(mine, 0)`) simultaneously breaks `enemy`/`td` and the mine-block layout.

Remaining work, in priority order:

1. A source construct that makes clang 7's greedy allocator spill `explosive` before `td`
   (worth +0.9 pt on its own) without moving anything else.
2. A source construct that keeps `this` in `r9` under the `E D A C B` tail — i.e. that changes
   the number/order of live intervals before the loop without changing the emitted preamble.
3. Removing the `int dmg` local **and** preventing the plain-damage-block duplication (the two
   are coupled: the local is what currently keeps `sl` from being `&position`, but removing it
   costs 12 B).

## 9. Reproduction

```
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984 byte=False linked=False
```

Scratch harness (`/dev/shm/w4516/`): `mkvar.py` (splice a body into `Gun.cpp` and measure),
`grid.py` (tail/`dmg`/mine-expect knob generator), `perm5.py` (120-permutation tail search),
`ndiff.py` / `win.py` (normalized-instruction diff and windowed side-by-side), `marg.py`
(marginal score of each diff hunk), `sig.py` (register-map extractor).

## 10. Files changed

None under `src/` (note only). Gate: byte 4500 / linked 4500, size-exact 4524/4524,
data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
lint CLEAN, drift 0.
