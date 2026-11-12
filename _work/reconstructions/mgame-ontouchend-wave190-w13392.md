# MGame::OnTouchEnd — wave 190 (w13392): chain 3-cycles, block field perms, won-block shape

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13392`, branch
`wave190-w13392`, base master `bea73c49b`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13392`.

Prior notes: `mgame-ontouchend-wave190-w13374.md` (finish/escort permutations,
96/127 split exhausted), `mgame-ontouchend-wave189-w13318.md`,
`mgame-ontouchend-wave189-w13323.md`, `mgame-ontouchend-wave189-w13344.md`,
`mgame-ontouchend-wave189-w13357.md`.

## Result

**No size-exact raw win.** `src/` is unchanged. The base is a raw local minimum
under an *exhaustive* 3-cycle chain re-order, under every 4-field permutation of
the five non-original chain bodies, and under every instant-action-first
won-block shape. The residual is dominated by branch-target bytes and cold-block
placement, not by instruction content.

| metric | base |
|---|---:|
| raw same-address diff bytes | 2,913 |
| aligned (instruction) diff | 1,706 |
| function size | 8,080 / 8,080 |
| instructions | 2,861 / 2,865 |
| `.rel.plt` diff | 0 (byte-identical) |
| `.ARM.extab` size / diff | 156,312 / 606 |
| build-id diff | 19 |
| file size | 2,187,328 exact |

## 1. Exhaustive 3-cycle chain-order search — base is the unique 3-cycle minimum

The base chain order is
`[74,15,162,43,42,22,65,120,96,100,95,110,81,134,144,126,155,161]`.
Prior passes exhausted single moves and pair swaps. This pass ran **all 1,632
distinct 3-cycles** (C(18,3) × 2 rotations) through the full build:

- best raw found = **2,913** (the base itself); every other 3-cycle is ≥ 2,916
  (nearest: `[74,162,43,15,42,22,65,120,96,100,95,110,81,134,144,126,155,161]`,
  raw 2,916).
- 200 random 3-/4-cycles had already given the same picture (next best 2,985).

So the chain order is a 3-cycle local minimum as well as a 1-/2-move minimum.

## 2. Block field-permutation search — base body order is uniquely best

The five blocks whose HP-field assignment order differs from the original
(`100, 81, 110, 126, 162`) were each swept over **all 24 permutations** of the
four `field_64/5c/60/68 = getHitpoints/getShieldHP/getArmorHP/getGammaHP`
lines (other statements held fixed). For every one of the five blocks the base
permutation `(0,1,2,3)` is the unique raw minimum (2,913); every other
permutation is ≥ 2,913 and most are much worse. Reordering a block's fields does
not change its size, so this is a pure same-address byte comparison — the base
order is genuinely the best match at the address the block actually occupies.

## 3. Won-block shape: every instant-action-first form is size 8,084 (+4)

The original emits the instant-action test first (`cbz r0, campaign_check` at
`0xe3e`); the base emits the campaign test first and keeps `instantAction` live
in `r4` across the `isCampaignMission` call. All of the following source shapes
were measured on the base chain order:

| shape | raw | size |
|---|---:|---:|
| base (campaign-first, `else if (!instantAction)`) | **2,913** | 8,080 |
| `if (instantAction) {…return;} if (campaign) … else …` | 4,706 | 8,084 |
| `if (!mission->isInstantActionMission()) { if (campaign) … else … } else …` | 4,706 | 8,084 |
| `if (instantAction) {…} else if (campaign) … else …` | 4,706 | 8,084 |
| separate `if`s, nested variants, redundant `!instantAction` re-use | 4,706 | 8,084 |
| `__builtin_expect(instantAction, 1)` | 5,393 | 7,864 |
| `__builtin_expect(campaign, 1)` | 4,322 | 8,092 |

The +4 is **not** in the won block itself: both shapes emit 62 bytes in
`0xe3e..0xe7c`. It is a global block-placement shift — with instant-action-first
the compiler places the `Hud::initHudMenu` cold block at `0xc86` instead of
`0xe9a`, which shifts the whole `won`/`keepObjectives` region. The base's
campaign-first form keeps `0xc60..0xce0` **byte-identical** to the original.

## 4. Mining block `0x4a4` hoist — 22 bytes, robust to every source shape

The first non-branch-target diff is 22 bytes at `0x4a4..0x4ba`: clang hoists
`add.w r8, sp, #80` (the `String("t ")` temp address) *above* the first
`operator+` call and swaps `mov r0,r8`/`movs r2,#0`; the original computes it
after the call. Both streams re-converge at `0x4be`, so it is a pure scheduling
difference. Tested and all neutral-or-worse:

- named `String` temporaries for `"\n"`, `count`, `"t "`, the `getText` result;
- `String("t ")` vs `+ "t "` / `+ "t "` c-string overloads (byte-identical);
- `String head = String("\n") + String(count); head += …` forms;
- `+=` chains, associativity groupings, `getText`-first order;
- `const char*` aliases for `"\n"` / `"t "`;
- declaration re-orderings (`report` last, `ship`/`refinedAny` swaps, `int
  refinedAny`), `_bca = 0` hoisting, `fee`/`amount`/`count` declaration moves.

The hoist is a post-RA scheduler decision and is not reachable from the source
shapes tried.

## 5. `count != 0` (the faithful original condition) costs +8

The original tests `amount != 0` (`cmp r9,#0; beq.n` at `0x486`/`0x48a`); the
base uses `count >= 0` (`bgt.n`, one byte different at `0x48b`). Every spelling
of the `!= 0` form (`count != 0`, `amount != 0`, `amount < 0`, `-amount != 0`,
`(count & 0x7fffffff) != 0`, positive-counter `count++`) measures **size 8,088
(+8)**, because the compiler materialises both `amount` and `count`
(`ldr.w r9,[sp,#24]; cmp.w r8,#0` instead of `cmp.w r9,#0`). `count > 0` /
`amount < 0` / `count >= 1` are size-exact but raw 2,915 (one more byte than the
base). `count >= 0` and `count > 0 || count == 0` are byte-identical (2,913).

## 6. Block 100 reorder

Original block 100 emits `HP, Shield, Armor, Gamma, switch=0, initStream=0,
departStation, module(5)`; the base emits `module(5), HP, Armor, Gamma, Shield,
switch=0, departStation, initStream=0, active=0`. Writing the original order
measures **size 8,048 (−32)**; moving only `module(5)` to the end is size 8,080
but raw **3,018**. The base order is the raw best.

## 7. Where the residual actually is

The whole region `0x0..0xe64` is **byte-identical to the original except 42
bytes**: 1 byte at `0x48b` (`bgt`/`beq`), 22 bytes at `0x4a4..0x4ba` (the hoist),
1 byte at `0x6b4` (the `"t "` literal-pool word, a `.rodata`-layout artefact),
and 18 branch-target bytes at `0xba4/0xc22..0xc3e/0xdba..0xdec` that point into
the chain. The remaining ~2,870 bytes are the chain `0xf00..0x1d00` and are
dominated by 4-byte `bne.w`/`beq.w` branch targets to chain blocks whose
addresses differ, plus cold-block placement. Confirming this, block 144 lands
at the same address in both binaries (`0x1bcc`) and differs in only 37/132
bytes — essentially all branch targets.

## 8. Structural conclusion

The base's chain order is a deliberate raw-optimal permutation: it puts many
blocks at addresses where their *branch targets* and literal-pool words happen
to line up, even though the block bodies are not in the original order. The
original order + original bodies has aligned diff **964** (vs 1,706) but size
8,084 (+4) and raw 4,284, because the cold-block placement shifts every block by
a different amount. Making that a net win requires reproducing the original's
`MachineBlockPlacement` cold-block layout, which is not reachable from any
statement order tested here.
