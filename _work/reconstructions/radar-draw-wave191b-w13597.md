# Radar::draw wave 191b (w13597) — r10/r11 swap pinned to the literal-68 vreg; no size-exact win

**Status:** complete, 2026-11-13. Worktree `wt/w13597`, branch `wave191b-w13597`,
base `master` `b957fbd12`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13562.md` (RA root cause), `-w13574.md`
(eviction weights), `-w13582.md` (queue), `-w13587.md` (pair flips).
**No source change landed**: `src/` is untouched; this note is the only commit.

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,965 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.rel.plt` | byte-identical |

## 1. New evidence: the exact post-RA occupancy of the callee-saved set

Reconstructed by matching pre-RA MIR slots (`mir.txt:395123`) against the
VirtRegRewriter dump (`mir.txt:413436`) slot-by-slot. Ours:

| reg | vreg(s) (size) |
|---|---|
| r4 | `%380` this (105,776), `%2494` (9,328) |
| r5 | `%385` `__stack_chk_guard` GOT (105,792), `%91` |
| r6 | `%163` (13,872), `%306`, `%747` |
| r8 | `%420` alienOrbit (22,528), `%160`, `%299`, `%216`, `%334`, `%87` |
| r9 | `%139`, `%998`, `%253` |
| **r10** | **`%2487` shared zero/mbp (34,992)**, `%153`, `%2504`, `%23`, `%257` |
| **r11** | **`%900` literal 68 (9,504)**, `%2561` (3,184) |

`%422` campaign (22,320) is **spilled** to `%stack.45` (sp+0x90); `%416`
turretMode is spilled to `%stack.36` (sp+0x94, same as the original); `%382`
hud, `%383` mode, `%381` player are all spilled.

The zero is *split* by the allocator: the head (240..10,408) lives in r10 and
is spilled to `%stack.42` at slot 10,408 (start of bb.45), reloaded as `%2732`
at 11,000/20,360 and as `%2735` at 25,304 for the two `missionBlocksPlanets`
tests at slots 35,632/36,640. So `%2487` is not a single long interval at
allocation time — the "longest-first" model in w13562 is only the enqueue
order; the split at bb.45 is what decides the home register.

### 1.1 The swap is `{zero, literal 68} × {r10, r11}`

The original disassembly shows the same code with r10↔r11 exchanged:

| | original | ours |
|---|---|---|
| zero materialised | `mov.w r11,#0` @0x12e07a | `mov.w r10,#0` @0x12e07a |
| zero stores | `strb.w r11,[r4,#0x218/0x130]` | `strb.w r10,...` |
| campaign | `mov r10,r0` @0x12e0f8 | `str r0,[sp,#0x90]` |
| zero spill before planet block | `str.w r11,[sp,#0x70]` @0x12e3ba | `str.w r10,[sp,#0x74]` |
| stations reuses the reg | `mov r10,r0` @0x12e3c6 | `str r0,[sp,#0x84]` |
| planet-loop index | `cmp r11,r1` @0x12e3da | `cmp r10,r1` @0x12e3d6 |
| literal 68 | `mov.w r10,#0x44` @0x12ecd0/0x12eea2/0x12f128/0x12f196 | `mov.w r11,#0x44` @0x12e784/0x12eaf8 |

So the original's r10 timeline is `campaign → literal 68`; ours is
`zero → literal 68 in r11`. The residual is exactly this swap (plus the
campaign spill/reload pair it forces, which is why ours has 2 extra
instructions and the frame is shifted by 4 bytes: `mode` at sp+0x68 vs
sp+0x64 — the first differing byte at +0x0e).

## 2. Why the zero does not simply take the free-looking r11

At enqueue time the zero (unhinted, 34,992) is dequeued after the 7 hinted
long ranges and the 11 larger unhinted GOT ranges, so r11 is already held by
one of the big intervals; `RAGreedy::tryEvict` picks the cheapest *satisfied*
interference by `(BrokenHints, MaxWeight)` (`RegAllocGreedy.cpp:276,1094`),
and campaign's eviction (weight 5.29e-5, no broken hint) beats whatever sits
in r11 at that moment. `%900` (literal 68) is *not* the blocker at that point —
it is allocated later (size 9,504) and only lands in r11 because r11 is free by
then. This is a multi-eviction cascade, not a single pairwise comparison
(consistent with w13574's finding that no single weight perturbation flips it).

## 3. New cells measured this pass (all reverted; base 6,013 / 9,908)

### 3.1 Semantic fixes (individually confirmed from disassembly, all raw-worse)

The music tail is genuinely **A,C,B** in the original (`subs r3,r1,#136;
cmp r3,#15; bhi body; lsls; tst #0xe071; beq body; cmp r1,#151; ...` at
0x130280), ours is A,B,C. The asteroid candidate arm is genuinely
`if (field_0x10 != data[best]) asteroidLockTimer = 0;` (0x12fd88 `itt ne;
movne r1,#0; strne r1,[r4,#0x19c]`) with `asteroidLockTimer += mode` in the
`else` arm (0x12fe40 `add r0,r2`), i.e. w13587's `ast_fix`.

| cell | raw | size |
|---|---:|---:|
| `music_ACB` | 6,036 | 9,908 |
| `ast_fix` | 6,190 | 9,908 |
| `loop_reorder` | 6,014 | 9,908 |
| music+ast | 6,213 | 9,908 |
| loop+music+ast | 6,214 | 9,908 |
| loop+music+ast+mbp_else | 6,214 | 9,908 |

### 3.2 `missionBlocksPlanets` live-range / hint reshapes

| cell | raw | size | zero/campaign |
|---|---:|---:|---|
| base | **6,013** | 9,908 | r10 / spill |
| decl before the two `strb` stores | 6,013 | 9,908 | r10 / spill |
| decl between the stores and `SetColor` | 6,013 | 9,908 | r10 / spill |
| `plasmaInRange = lockConfirmed = 0;` (chained) | 6,015 | 9,908 | r10 / spill |
| `= 0, = 0` (comma) | 6,013 | 9,908 | r10 / spill |
| `mbp = (int) this->plasmaInRange` | 9,428 | 9,892 | r12 / r10 |
| `mbp = this->plasmaInRange != 0` | 6,838 | **9,908** | r12 / spill |
| `mbp = this->enabled & 0` / `mode & 0` / `mode - mode` | 6,013 | 9,908 | r10 / spill |
| `mbp` `unsigned` | 6,013 | 9,908 | r10 / spill |
| `mbp` `(bool) false` | 6,013 | 9,908 | r10 / spill |
| `lockConfirmed = plasmaInRange` (store→load fwd) | 6,013 | 9,908 | r10 / spill |
| `bool mbpZero=false; int mbp = mbpZero;` | 6,013 | 9,908 | r10 / spill |
| `bool missionBlocksPlanets` + `(int)` casts at both uses | 6,172 | 9,912 | r10 / spill |

Use-site spellings: `if (mbp != 0)` / `if (mbp > 0)` / `(mbp == 1)` at the
`hudEvent(21)` test all 6,013; `(planetLockTimer<1) || missionBlocksPlanets` /
`== 1` / `> 0` all 6,013; `== 0` at either site 6,699 / 6,426 (size exact).

### 3.3 Full bool→{int,unsigned char,char} sweep of every `bool` local in `draw`

66 cells. Only these are size-exact: `onPlanetCourse`, `topOk`, `isTarget`
(×2), `inSight` (×2), `navWreck`, `hasDockNav`, `candidateWreck`, `ambient`
(all 6,013, colouring unchanged); `gasHit`→uchar/char 6,022; `gasHit`→int
9,904. Everything else is size-fatal. The colouring-flipping ones:

| cell | raw | size | zero/campaign |
|---|---:|---:|---|
| `alwaysEnemySeen`→int/uchar/char | 9,191 | 9,952 (+44) | **r11** / spill |
| `turretMode`→int/uchar/char | 9,226 | 9,860 (−48) | **r11 / r10** |
| `bossSeen`→int | 9,186 | 9,924 (+16) | r11 / spill |
| `otherLockFlag`→int | 9,447 | 9,896 (−12) | r12 / r10 |
| `alwaysEnemy`→int | 8,599 | 9,920 (+12) | r10 / spill |
| `landmarkHit`→int | 8,730 | 9,936 (+28) | r10 / spill |
| `planetHit`→int | 8,460 | 9,912 (+4) | r10 / spill |
| `marked`→int | 8,464 | 9,912 (+4) | r10 / spill |
| `enemyHit`→int | 8,615 | 9,940 (+32) | r10 / spill |
| `wreck`→int | 8,236 | 9,912 (+4) | r10 / spill |
| `asteroidHit`→int | 7,535 | 9,924 (+16) | r10 / spill |

`turretMode`→int is the only single size-exact-adjacent cell that produces the
*exact* original colouring (zero r11, campaign r10), but it is −48 B.
`alwaysEnemySeen`→int produces zero r11 but leaves campaign spilled and is
+44 B. No single type change is both size-exact and colouring-correct.

### 3.4 `gb` (campaign-mission guard one-liner) family

`gb` reorders the guard to `!isEmpty() && != nullptr && isVisible() && (...)`:
6,729 / 9,908, campaign→**r11** (zero stays r10). Combined with every other
op: `gb+mbp_else` 6,729; `gb+zs_after` 6,756; `gb+music` 6,752; `gb+ast` 6,909;
`gb+loop` 8,991 (9,900); `gb+loop+ast` 8,994 (9,900); `gb+loop+mbp_else` 8,991
(9,900); `gb+aes_int` 9,191 (9,952); `gb+music+ast` 6,932. None is a win.

### 3.5 Exhaustive pair/triple search over the colouring-flipping ops

17 ops {gb, music_ACB, loop, and the 14 bool-type variants}: all 136 pairs +
680 triples, 50 min of cells. Best size-exact raw = **6,014**
(`candidateWreck_int+loop` / `loop+exp12`); no cell < 6,013. A separate
500-trial random search over 80 ops (expect flips, music orders, ast_fix,
loop, mbp_else, zs orders, 28 type variants, campaign const/unsigned,
mbp initialisers) also bottoms out at 6,013. `loop + every op` (50 cells)
bottoms out at 6,014.

## 4. Conclusion

The residual is the `{shared zero, literal 68} × {r10, r11}` colouring swap
plus the campaign spill it forces. Every source spelling that produces the
original colouring is size-fatal (`turretMode`→int −48, `alwaysEnemySeen`→int
+44), and every size-exact spelling keeps the wrong colouring. The pre-RA IR
is invariant under all ~700 cells measured here and in w13562/74/82/87. The
next lever is an exact LLVM-7 greedy-RA replay (the LLVM-19 MIR harness in
scratch is close but still fails the verifier on 91 `t2ADDri`/`t2LDRBi12`
vregs), so that the eviction cascade can be perturbed offline instead of at
~1 s per clang cell.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13597/`: `base.cpp`, `cc.txt`,
`link.txt`, `batch.py` (cell harness), `cellsJ–cellsO.json` (this pass's
sweeps), `gbsweep.py`, `rs2.py`, `pair2.py`, `loopsweep.py`, `mir.txt`
(`-print-machineinstrs`: pre-RA @395123, post-greedy @404142, rewriter
@413436), `orig_full.dis` / `ours_full.dis`, `dif2.py`.
