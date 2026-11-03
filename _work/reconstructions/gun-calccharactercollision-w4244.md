# `Gun::calcCharacterCollision` pass 8: the statement order and the allocator are coupled (w4244)

Worker: w4244. Branch: `wave92-w4244`, base master `b379d00d`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base `0x10000`). Scratch:
`/dev/shm/w4244/`. Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump
`0x152f90`, size `1984 B`.

## 0. Result

No code change landed. The tree already carries the w4229 pass-7 body
(`62313153`), which is still the best measured shape.

| build | gate byte/linked | target pct (official) | target size |
|---|---:|---:|---:|
| HEAD `eb472137` (Gun.cpp unchanged) | **4498 / 4498** | **90.0 %** | **1984 B** |
| after this pass (note only) | **4498 / 4498** | **90.0 %** | **1984 B** |

Full gate after: size-exact 4524/4524, data 843/843, imports 207,
`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`
(size- and base-exact), lint `CLEAN ✓`, drift 0, `RATCHET PASS`. The target is not byte-exact
(`byte: false`), so the hard counts are unchanged.

This pass proves *why* the three cosmetic residuals are coupled, and rules out
the obvious "faithful order" reconstruction: the shipped **source** order is
almost certainly `f54, playerGun, hitvec, lifetimes, lastKIP` (the shipped
emitted order), but that source order makes clang 7 pick a completely different
callee-saved allocation (`this=r4`, 1912 B, 64.8 %). The tree's `f54, hitvec,
lastKIP, lifetimes, playerGun` order is the only order that reproduces the
shipped allocation (`this=r9`, 1984 B) — at the cost of the emitted order.

## 1. The shipped post-damage block (ground truth)

```
153310  mov   r0, r8
153312  blx   Player::damage
153316  movs  r1, #1
153318  strb.w r1, [r8, #84]        ; F  field_54[0] = 1
15331c  ldrb.w r0, [r9, #240]       ;    this->playerGun
153320  cbz   r0, 15332a
153322  ldr.w r0, [r9, #56]         ;    this->level
153326  strb.w r1, [r0, #48]        ; P  level->field_30 = 1
15332a  add   r3, sp, #176
15332c  ldmia r3, {r0, r2, r3}      ;    -velocity
15332e  eor.w r1, r0, #0x80000000
153332  mov   r0, r8
153334  eor.w r2, r2, #0x80000000
153338  eor.w r3, r3, #0x80000000
15333c  blx   Player::setHitVector  ; H
153340  ldr.w r0, [r9, #60]         ;    this->lifetimes
153344  ldr   r1, [pc, #916]        ;    -1000000
153346  str.w r1, [r0, fp, lsl #2]  ; L
15334a  mov   r0, r8
15334c  blx   Player::getKIPlayer
153350  str.w r0, [r9]              ; K  this->lastHitKIPlayer
153354  ldr   r0, [sp, #20]         ;    explosive
```

The `cbz` at `153320` is a real conditional branch: the `playerGun` store is a
**separate basic block** placed between `F` and `H`. Blocks cannot be
interleaved, so in the shipped IR the block containing `F` must end at that
`cbz`; i.e. the shipped source order has `playerGun` immediately after `f54`.
`L` before `K` in the emitted code is a within-block scheduler reorder.

The tree's current build emits the source order verbatim:

```
153300  blx   Player::damage
153304  mov.w fp, #1
153308  add   r3, sp, #176
15330a  strb.w fp, [r8, #84]        ; F
15330e  add.w sl, sp, #192          ; &position
153312  ldmia r3, {r0, r2, r3}
...
153322  blx   Player::setHitVector  ; H
153326  mov   r0, r8
153328  blx   Player::getKIPlayer   ; K
15332c  ldr.w r1, [r9, #60]
153330  ldr   r2, [sp, #40]
153332  str.w r0, [r9]
153336  ldr   r0, [pc, #932]
153338  str.w r0, [r1, r2, lsl #2]  ; L
15333c  ldrb.w r0, [r9, #240]
153340  cbz   r0, 15334a
153342  ldr.w r0, [r9, #56]
153346  strb.w fp, [r0, #48]        ; P
```

## 2. The faithful order `F P H L K` changes the whole allocation

Re-building the *same* body with the five statements in the shipped order
(`f54, playerGun, hitvec, lifetimes, lastKIP`) reproduces the shipped statement
order exactly, and even the shipped `explosive` slot, but the callee-saved
allocation collapses:

| property | shipped | HEAD (`F H K L P`) | `F P H L K` |
|---|---|---|---|
| `this` | **r9** | **r9** | r4 |
| `enemy` | **r8** | **r8** | r6 |
| inner `i` | **fp (r11)** | **[sp,#40]** | r8 |
| `td` | **r6, spilled [sp,#24]** | **r6, spilled [sp,#20]** | r9 (not spilled) |
| `&position` | sl (r10) | sl (r10) | sl (r10) |
| `explosive` slot | **[sp,#20]** | [sp,#24] | **[sp,#20]** |
| `scatter` / `mine` | [sp,#32] / [sp,#36] | [sp,#32] / [sp,#36] | [sp,#36] / [sp,#40] |
| `e` (outer) | [sp,#28] | [sp,#28] | [sp,#24] |
| size | **1984 B** | **1984 B** | **1912 B** |
| parity | — | **90.0 %** | **64.8 %** |

`this` moving from `r9` to `r4` is worth 72 bytes: every `ldr/str [r9,#imm]`
becomes a 16-bit `ldr/str [r4,#imm]` (`ldr.w`/`str.w` → `ldr`/`str`). The
function therefore cannot be 1984 B with `this=r4`.

The `F P H L K` build is the *structurally* correct one (order and `explosive`
slot both match) and it is still worse (64.8 %) because every `this` access now
differs. The tree's `F H K L P` build wins only because its allocation happens
to match the shipped one.

## 3. Exhaustive statement-order sweeps

All 120 permutations of `{F,P,H,L,K}` were measured for each damage-block shape
(official verifier, `autojunk=False`; `osize=1984` for all):

| shape | best pct | best size | best orders |
|---|---:|---:|---|
| `local` (`int dmg = this->damage;` + 4 source calls) | **90.0** | **1984** | `FHKLP` 90.0, `KFHLP` 89.7, `FKHLP` 89.7 — **all put `playerGun` last** |
| `4call` (`this->damage` in all four) | 84.7 | 1992 | `FHPLK` |
| `phi` (`int dmg;` assigned in every branch) | 84.8 | 1976 | `PHFLK` |
| `assign` (`dmg = this->damage`, overwritten in 2 branches) | 64.4 | 1916 | `LHFKP` |
| `ternary` (nested `?:` as the call argument) | 84.8 | 1976 | `PHFLK` |

Only the `local` shape ever reaches 1984 B, and only with `playerGun` last.
Every `P`-not-last order drops to ≤ 1980 B and ≤ 85.3 %.

## 4. MIR evidence for the `explosive`/`td` slot swap

Built with the real pipeline (`clang++ -Oz -mllvm -print-after-all
-mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv`).

Frame layout after PEI (frame objects `fi#13..fi#20` are the eight 4-byte
spill slots; higher `fi` gets the lower `sp` offset):

```
fi#13 -> [sp,#40]   fi#14 -> [sp,#36]   fi#15 -> [sp,#32]   fi#16 -> [sp,#28]
fi#17 -> [sp,#24]   fi#18 -> [sp,#20]   fi#19 -> [sp,#16]   fi#20 -> [sp,#12]
```

Pre-colouring stack objects (Greedy RA dump) and post-colouring names
(Stack Slot Coloring / PEI dumps):

| value | pre-colour | post-colour | sp |
|---|---|---|---|
| `i` (inner loop) | `%stack.20` | `%stack.13` | 40 |
| `mine` | `%stack.15` | `%stack.14` | 36 |
| `scatter` | `%stack.14` | `%stack.15` | 32 |
| `e` (outer loop) | `%stack.19` | `%stack.16` | 28 |
| **`explosive`** | **`%stack.17`** | **`%stack.17`** | **24** |
| **`td`** | **`%stack.18`** | **`%stack.18`** | **20** |
| `explodeOnContact` | `%stack.16` | `%stack.19` | 16 |
| stack canary | `%stack.13` | `%stack.20` | 12 |

So `explosive` and `td` keep their pre-colouring indices in our build; the swap
against the shipped `[sp,#20]=explosive / [sp,#24]=td` is purely the
Stack-Slot-Colouring weight ranking. The llc-7 `-debug-only=stack-slot-coloring`
run (on the same function, `-O2` pipeline) reports

```
SS#16 (explosive) weight 13.770   rank 6 of 8
SS#18 (td)        weight  6.868   rank 8 of 8
Assigning fi#14 to fi#13 ... fi#16 to fi#18 ... fi#18 to fi#20
```

i.e. higher weight -> lower colour index. Shipped must rank `td` **above**
`explosive` for the swap. Note this is not a simple use-count argument: the
shipped `td` is loaded only twice (`15319c`, `1533f6`) while ours is loaded five
times (`15319c`, `15336a`, `153376`, `1533f8`, plus the initial spill), so the
weights must differ through the **block frequencies / CFG**, not the raw operand
counts. Every source-level lever tried (declaration order of `explosive`/`td`,
`bool`/`int`, `const`, parenthesised, `Vector&`, moving `td` before/after
`lastHitKIPlayer`, `explodeOnContact = bomb || mine` vs `mine || bomb`,
`if (x != 0)`, `scatter != 0`) leaves the slot unchanged.

## 5. The `dmg`-in-`sl` residual is the same scheduling decision

Shipped keeps `&position` (sp+192) in `sl` across the `damage` call and
re-materialises `dmg` in the plain block:

```
15328a  ldr   r0, [r9, #4]        ; owner
15328e  cbz   r0, 153296
153290  ldrb  r0, [r0, #92]
153294  cbz   r0, 1532ca
153296  ldrb  r0, [r8, #105]      ; egoFlag
15329a  add.w sl, sp, #192        ; &position hoisted into the branch
...
1532ca  ldrb  r0, [r8, #105]
1532ce  add.w sl, sp, #192        ; &position hoisted into the other branch
...
153304  ldr   r3, [r9, #88]       ; itemIndex
153308  ldr   r1, [r9, #96]       ; dmg reloaded here
15330c  ldrb  r2, [r9, #249]      ; friendGun
153310  mov   r0, r8
153312  blx   Player::damage
```

Ours keeps `dmg` in `sl` and computes `&position` once after the call:

```
15327e  ldr   r0, [r9, #4]
153282  ldr.w sl, [r9, #96]       ; dmg
153286  cbz   r0, 15328e
...
1532fe  mov   r1, sl              ; plain path
153300  blx   Player::damage
15330e  add.w sl, sp, #192        ; &position only now
```

The two are mutually exclusive: `sl` cannot hold both. Whichever value the
pre-RA scheduler hoists into the branch blocks wins, and the other is
re-materialised. This is the same scheduler/RA interaction that decides the
`explosive`/`td` colouring and the `playerGun` block placement.

## 6. Variants tried this pass (all ≤ 90.0 %)

On top of the winning `local` body, with the official verifier:

| variant | pct | size |
|---|---:|---:|
| HEAD (`F H K L P`) | **90.0** | **1984** |
| `F P H L K` (shipped order) | 64.8 | 1912 |
| `F P H K L` / `F P L H K` | 64.8 | 1912 |
| `F P K H L` | 64.6 | 1912 |
| `F H P L K` | 85.3 | 1976 |
| `dmg` before the `empDamage` block, `F H K L P` | 85.0 | 1980 |
| `dmg` before the `empDamage` block, `F P H L K` | 64.7 | 1908 |
| `dmg` at function top | 63.3 | 1904 |
| `dmg` after the radius block | 81.3 | 1988 |
| `dmg` as `unsigned` / `long` / `+0` / `(int)` / `(this->damage)` | 90.0 | 1984 |
| `dmg` as `const` | 90.0 | 1984 |
| `dmg` as `volatile` | 67.6 | 1968 |
| `dmg` as `int&` | 84.0 | 1996 |
| `4call` (no `dmg` local) | 84.0 | 1996 |
| `position`/`velocity`/`enemyPosition` `const` or ctor-init | 90.0 | 1984 |
| swap `position`/`velocity` declarations | 86.8 | 1984 |
| `int i` / `int e` loop counters | 90.0 | 1984 |
| `enemy` hoisted out of the loop | 90.0 | 1984 |
| `td` before `lastHitKIPlayer`, `td` as `const`/`Vector&`/parenthesised | 90.0 | 1984 |
| `bool explosive`, `bool explodeOnContact`, `int mine/scatter/bomb/weaponType` | 90.0 | 1984 |
| `F P H L K` + all of the above declaration variants | 64.8 | 1912 |

The `F P H L K` allocation is extremely robust: every declaration/type/scope
perturbation tried still gives `this=r4` and 1912 B.

## 7. Conclusion

The remaining 10 % is a single coupled degree of freedom:

* The shipped **source** order is `f54, playerGun, hitvec, lifetimes, lastKIP`
  (proved by the `cbz` block boundary between `F` and `H`).
* That order makes clang 7's greedy allocator pick `this=r4` (1912 B), which
  cannot be the shipped binary (1984 B).
* The tree's `f54, hitvec, lastKIP, lifetimes, playerGun` order reproduces the
  shipped allocation (`this=r9`, `enemy=r8`, frame 208, 1984 B) but emits the
  statements in source order.

So either the original source is *not* a plain reordering of these five
statements (some other construct must pin the allocation independently of the
order), or the shipped build's allocator was steered by something not present in
the reconstructed body. None of the 120 `local` permutations, nor any of the
`4call`/`phi`/`assign`/`ternary` shapes, gives both the shipped order **and**
1984 B.

Remaining work, in priority order:

1. Find a source construct that pins `this` to `r9` (or equivalently forces
   `i`/`td` out of `r8`/`r9`) *while* `playerGun` sits between `f54` and
   `setHitVector`. Candidates not yet exhausted: a named `Level *level` /
   `PlayerEgo *player` local live across the block, or a different
   `playerGun`/`field_30` spelling that keeps the store in the same block as
   `f54` (predicated `it eq` then converted to `cbz` by Thumb2 size reduction).
2. If (1) lands, the `explosive`/`td` colouring and the `dmg`-in-`sl` reload
   should fall out of the same allocation; re-check the SSC weight ranking with
   `-mllvm -print-after-all` before assuming they are independent.
3. Keep 1984 B: any candidate that changes the size shifts every later `.text`
   symbol and fails the hard gate (`byte_exact` 4498 -> 3800 for a 1992 B
   variant, measured in w4229 §1).

## 8. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
```

Per-function oracle (fast, same code as `tools/verify/verify.py`):

```
python3 /dev/shm/w4244/fastverify.py _ZN3Gun22calcCharacterCollisionEv
# pct=90.0 osize=1984 usize=1984 byte=False linked=False
```

Statement-order sweep harness: `/dev/shm/w4244/flex.py` (builds
`PRE + damage-block + statement-order + POST`, links, and runs the oracle).
MIR dump: `clang++ -Oz -mllvm -print-after-all
-mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv -c Gun.cpp`.
