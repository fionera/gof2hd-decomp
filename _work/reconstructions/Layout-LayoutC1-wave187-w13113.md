# `Layout::Layout` C1/C2 — wave 187 / worker w13113

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B** (object section size **7,064 B**; the
linked size includes 8 B of inter-symbol alignment padding), source
`src/game/ui/Layout.cpp`.  Branch `wave187-w13113`, base master `8b5d4281c`
(post-wave-186 tree; w13012's two permutation wins are landed, raw **5,354**).

**Result: no size-exact raw win found.  `src/` left at the base.**

| metric | base |
|---|---:|
| raw same-address bytes | **5,354** |
| aligned (difflib) | 2,680 |
| size | **7,072/7,072** |
| instructions | 2,521/2,552 |

Gate after the (empty) pass: `RATCHET PASS` — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, data_exact 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

Task angle: the `Globals::iPadHD/n9/retinaDisplay/iPadLarge/iPad` ternaries,
the `volatile unsigned char *rd` block, and the `rewardBoxY2` bug.  No blind
W/K permutation search was re-run (that is w13012's cell); the new searches
below are condition-chain and inner-block searches plus a move-sensitivity
probe.

## 1. New finding: the current body is almost completely order-insensitive

The ctor was parsed into **203 top-level units** (statements / complete
if-else chains; the parser round-trips `Layout.cpp` byte-for-byte).  A
single-unit move to a *far* position (distance > 20; w13012 only searched
distance 11..20 moves) was measured with a full `link_so` for **250 random
(move-from, move-to) pairs**.  Every single one produced **exactly**
`raw=5354 size=7072 insn=2521` — byte-identical code.  Likewise sorting the
whole early block (units 1..30 / 1..50 / 20..60 / 30..70 / 40..90) by field
offset produced `raw=5354 size=7072` unchanged.

Conclusion: clang's post-RA scheduler + SLP re-canonicalises independent
stores, so most source permutations are codegen-neutral.  This explains why
w13012's exhaustive K<=6 search found only two wins: only permutations that
change a *constant-pool adjacency* (SLP grouping) can move the needle, and
those are rare.  It also means further permutation search on this body has a
very low expected yield.

## 2. Ternary condition-chain search (new)

Every `Globals::X ? ... : (Globals::Y ? ...)` chain in the ctor was parsed
(outer and nested) and every **adjacent condition pair** was swapped
(value-preserving because the mode flags are mutually exclusive), rebuilt,
linked and measured: **765 variants**.

* 94 were size-exact; the best was **5,354** (the base).  All size-exact
  variants scored `>= 5354`.
* Runner-ups: `field_0x2d8` / `rewardBoxY` chain swaps `5359` (size-exact),
  `field_0x300`-area swaps `5400`/`5429`, `field_0x30c`-area `5386`.
* Full `||` operand swaps (`retina||n9`, `retina||iPad||n9`,
  `retina||n9||iPad`) are codegen-neutral (5,354/7,072).  `n9||*rd` is
  size-fatal (6,956).

## 3. Inner if/else block permutations (new)

w13012's unit parser treated a complete `if/else` chain as one unit, so
permutations *inside* a branch body were never searched.  The ctor has 14
depth-1 blocks; 3 have 2 statements and 5 have 3 statements.  All
`2!`/`3!` permutations were linked and measured (36 variants).  Best was
**5,354** (base); the closest non-base was `5365` (`field_0x130` chain,
`(0,2,1)` and `(2,0,1)`), then `5371`.

## 4. `volatile unsigned char *rd` block

The block is value-correct.  Variants tried on the current body (all full
`link_so`):

| variant | raw | size |
|---|---:|---:|
| base | **5,354** | **7,072** |
| `const volatile` / `*const` / `volatile &` pointer forms | 5,354 | 7,072 |
| `0x2d8` condition uses `*rd` (rd hoisted before it) | 5,354 | 7,072 |
| `0x2d4` as one ternary (exact shape) | 5,501 | 7,072 |
| `0x2d4` as one ternary, merged last `else` | 5,544 | 7,056 |
| `rd` -> plain `Globals::retinaDisplay` (no volatile) | 5,500 | 7,064 |
| `*rd` split into `else if (*rd) ... else if (n9) ...` | 5,872 | 7,064 |
| `unsigned char rdv = *rd` local | 6,306 | 6,764 |
| `n9 || *rd` | 5,893 | 6,956 |

No size-exact improvement.  Note the original computes `0x2d4` and `0x2d8`
in one block and emits a single `strd r4,r6,[r0,#724]`; HEAD stores `0x2d8`
early (`str.w`) and `0x2d4` later (`str.w`).  All faithful merges shrink the
function by 128-200 B (size-fatal), confirming w13013's result.

## 5. `rewardBoxY2` bug (confirmed, still size-fatal)

Original block-C tail (`0xd2514..0xd252c`) is

```
r2 = rewardBoxY + 17
r2 = r2 << retinaDisplay
s0 = (float)r2; s0 = (int)(float)s0
str.w r1, [r0, #1012]   ; rewardBoxY
vstr  s0, [r0, #1016]   ; rewardBoxY2
```

so the source is `rewardBoxY2 = (int)(float)((rewardBoxY + 17) << retinaDisplay);`.
HEAD omits the `(int)(float)` round-trip.  On the current (wave-186) body:

| variant | raw | size |
|---|---:|---:|
| base | **5,354** | **7,072** |
| add `(int)(float)` round-trip | 5,530 | 7,092 |
| `(float)(...)` (implicit int) | 5,530 | 7,092 |
| `* (retina ? 2 : 1)` instead of `<<` | 5,422 | 7,084 |
| `<< (retina ? 1 : 0)` | 5,970 | 7,080 |
| round-trip before the shift | 5,530 | 7,092 |

The round-trip is real but adds 20 B; no compensating size-exact change was
found.

## 6. Correction to the wave-186 w13013 note: `field_0x1e8`

The wave-186 note claimed the original is `(n9 || iPadHD) ? 1 : 0` and that
HEAD is wrong.  Re-decoding the tail prologue shows the original is

```
lr = !(n9 | iPadHD)          ; clz/orr/lsr#5
r1 = 1
if (lr != 0) r1 = r8         ; r8 = retinaDisplay
field_0x1e8 = r1
```

i.e. `field_0x1e8 = (n9 || iPadHD) ? 1 : retinaDisplay`.  This is
**value-equivalent** to HEAD's
`retinaDisplay ? 1 : (iPadHD ? 1 : (n9 ? 1 : 0))` for all 0/1 flag values, so
there is no semantic bug here.  Writing the original shape literally
(`(n9 || iPadHD) ? 1 : retinaDisplay`) still shrinks the function to 6,948 B.

## 7. Store-type / float-round-trip audit (new)

A per-offset store-type map (original vs HEAD) was extracted from the
disassembly.  One concrete difference stands out: in the non-HD branch the
original computes `ip = retinaDisplay ? 10 : 5` once and stores it to
`field_0x20`, `field_0x24` and `buttonInsetX` with `strd ip,ip,[r0,#32]` +
`str.w ip,[r0,#40]`, whereas HEAD's `field_0x20_top` keeps the float type of
its `iPadHD ? 5 * 1.40625f : ...` ternary and emits
`vmov/vcvt.f32.s32/vcvt.s32.f32/vstr s0,[r0,#32]`.  Making `field_0x20_top`
int-typed (`... ? 7 : ...`) is value-identical but shrinks the function to
**6,876 B** (raw 6,177), so it cannot be landed alone.  The same pattern
(`N * 1.40625f` making a ternary float-typed) appears for
`field_0x148` (int-typing -> 7,064 B, raw 5,481) and `field_0x1e4`
(adding the round-trip -> 6,964 B, raw 5,922).

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13113
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5354 aligned=2680 size=7072/7072 insn=2521/2552
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13113/`
(`Layout.base.cpp`, `measure.sh`, `units.py` 203-unit parser, `ternary2.py`
condition-chain parser, `sweep_ternary.py`, `sweep_inner.py`, `randmove.py`,
`proxy.py` reduced-TU scorer, `storemap.py`, result logs).
