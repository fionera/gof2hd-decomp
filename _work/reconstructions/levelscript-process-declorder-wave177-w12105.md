# LevelScript::process — wave 177 / w12105: case-80 pirate declaration order (raw 53,568 -> 53,564)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12105`, branch
`wave177-w12105`, base master `9445ac5fb` (wave 176).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12105/`.

## Result (landed)

```
                         before (base)     after (landed)
process raw diffbytes    53,568            53,564            (-4)
process aligned insns    18,498            18,498            (0)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text section diff       79,591            79,587            (-4)
.rel.plt                 29,392 B ident    29,392 B ident    (byte-identical)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
symbol parity            5370/5370         5370/5370
RATCHET                  PASS              PASS
```

`raw` is the same-address byte diff over the function body (literal pools
included), from `tools/fn_diff.py`; it is the primary metric.  `.rel.plt` md5
is `289b2a069f8504d1e8a3919e43915e04` for both the original and ours.

## The landed source change

One declaration moves down two statements inside the case-80 (mission 80)
pirate-scatter loop (`src/game/world/LevelScript.cpp` ~line 2400):

```diff
@@ case 80: message #5 triggered, m_nState == 3 @@
                     Vector origin = (*enemies)[0]->getPosition();
                     for (int i = 5; i != 9; i++) {
                         int dx = Globals::rnd->nextInt(10000) + 15000;
-                        KIPlayer *pirate = (*enemies)[i];
                         int x = (Globals::rnd->nextInt(2) == 0 ? 1 : -1) * dx;
                         int dy = Globals::rnd->nextInt(10000);
+                        KIPlayer *pirate = (*enemies)[i];
                         int dz = Globals::rnd->nextInt(10000) + 15000;
                         int z = (Globals::rnd->nextInt(2) == 0 ? 1 : -1) * dz;
                         pirate->setPosition(origin + Vector{(float) x, (float) (dy - 5000), (float) z});
```

It is **semantics-preserving**: `(*enemies)[i]` is a pure array load, and the
five `Globals::rnd->nextInt` calls keep their exact order
(10000, 2, 10000, 10000, 2).  Only the load of `pirate` moves in the
scheduler's window.

### Why it matches better

Original Thumb, `0x13daa0` (loop body):

```
13daa0  ldr   r0,[sp,#96]          ; enemies
13daa2  movw  r1,#10000
13daa6  ldr   r0,[r0,#4]
13daa8  ldr.w fp,[r0,r6,lsl#2]     ; pirate = (*enemies)[i]
13daac  ldr   r0,[r4,#0]           ; rnd
13daae  blx   r8                   ; nextInt(10000)  -> dx
13dab0  movw  r9,#15000
13dab4  add.w r5,r0,r9             ; dx += 15000
13dab8  ldr   r0,[r4,#0]
13daba  movs  r1,#2
13dabc  blx   r8                   ; nextInt(2)      -> x sign
```

Base (before) put the `pirate` load between `nextInt(10000)` and
`nextInt(2)`.  The moved declaration makes clang sink the load past the first
three RNG calls (`nextInt(10000)`, `nextInt(2)`, `nextInt(10000)`), which
changes the register assignment around the loop and drops four bytes of
same-address diff.  It does not reproduce the original's exact load position
(the original hoists it above all five RNG calls); the -4 is the measured
net of the re-scheduling, and the function stays exactly 57,232 B with
`.rel.plt` untouched.

The five possible positions of the `pirate` declaration among the five RNG
locals were all measured on the wave-176 base:

| position of `pirate` declaration | raw | size |
|---|---:|---:|
| before `dx` | 55,197 | 57,260 (size-inexact) |
| between `dx` and `x` (base) | 53,568 | 57,232 |
| between `x` and `dy` | 53,565 | 57,232 |
| **between `dy` and `dz` (landed)** | **53,564** | **57,232** |
| between `dz` and `z` | 53,570 | 57,232 |
| after `z` | 53,589 | 57,232 |

## What was swept (negative results)

All runs below are size-exact and `.rel.plt`-identical unless noted.

* **Top-level declaration permutations.**  All 23 non-base permutations of
  `messages, player, enemies, status`: base is best (53,568); next best
  `enemies, player, messages, status` = 53,572.  Three permutations that move
  `status` earlier are size-inexact (57,260..57,316).  Confirms w11104's
  result on the wave-176 base.
* **`Status*` shape.**  `Status *&status = Globals::status` and
  "drop the local, use `Globals::status` twice" are both +1 raw (53,569,
  size-exact); `Status *const` is neutral.  The prologue still keeps
  `&Globals::status` in `r9` (original: `r4`) and `this` in `fp`
  (original: `sl`); no local shape moves them.
* **One-use temporaries.**  Inlining `introMsg`, `msg6`, `msg2`,
  `float enemyZ`, `Vector diff`, `PlayerFixedObject *port`, `Vector playerDir`
  is codegen-identical (or worse).  `Vector camDir` inline is size-inexact
  (57,256).  The compiler already coalesces these.
* **Adjacent declaration swaps.**  All 81 adjacent same-indent declaration
  pairs in the body were swapped and measured on the wave-176 base; the only
  winner is the landed `pirate`/`x`/`dy` move (the same pair measured as
  `swap_2403_2404` = 53,565 before the second nudge).  A second sweep on the
  post-win tree finds no further adjacent-swap win (all >= 53,564).
* **Declaration moves past one statement.**  288 "move declaration down past
  the next same-indent line" variants and 213 "move declaration up past the
  previous same-indent line" variants on the post-win tree: no further win
  (all >= 53,564).
* **Vector/Matrix declaration order.**  `lightDir`/`playerPos` swaps and
  `int off` first in the case-154 state-9 scatter are neutral or worse
  (53,564..53,565); `playerPos`/`enemyPos` swaps in the post-switch tail and
  the case-40 distance test are neutral.
* **Case-11 `(float)(delta*3)`.**  The original computes `3*delta` with
  `add.w r0,r4,r4,lsl#1` + `vcvt`, i.e. the source form
  `(float)(delta * 3)` (as in case 9), not `(float) delta * 3.0f`.  Changing
  it reproduces the original's instruction sequence at the block, but the
  block moves 12 B and raw rises to 53,581 (size-exact, `.rel.plt` ident);
  reverted.
* **Mission-0 state 3/4/5 jump-table merge.**  `if (m_nState >= 6)` as a
  separate statement (55,269 / 57,260, `.rel.plt` DIFF) and as an
  `else if` (55,424 / 57,276, `.rel.plt` DIFF) are both size-inexact; the
  merged 13-entry `tbh` stays.
* **Wingmen loop rotation.**  Removing the `__builtin_expect(isWingMan(), 0)`
  makes the loop bottom-tested exactly like the original, but raw rises to
  53,635 (+67) because the loop body shifts 2 B; reverted.
* **`m_pLevel`/`enemies` aliases in the shared tail.**  Using the top-level
  `enemies` instead of `m_pLevel->getEnemies()` in the wingmen loops
  (54,337 / 55,033) and inlining `Level *lvl` (55,055 / 57,240) are worse.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12105
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53564 aligned= 18498 size= 57232/57232 insn= 20726/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=53564 size=57232/57232 text=79587 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -j2   # CLEAN
cmake --build cmake-build-match --target drift -j2   # 0 fields
```

Scratch harnesses (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12105/`
`batch.py` (apply a list of exact-string replacements, relink, measure),
`rundiff.py` / `shape.py` / `sreg.py` (aligned diff, shape and s-register
mismatch finders), `declswap.json` / `declmove.json` / `declup.json`
(declaration-order sweeps), `LevelScript.base.cpp` (wave-176 base) and
`LevelScript.win1.cpp` (post-win tree).

## Residual

`process` is still 53,564 raw.  The dominant residual is block placement:
the mission-0 state machine merges states 3/4/5 into the 6..15 `tbh`, which
shifts the first/later literal pools (ours `0x1380c0`/`0x138d60`, original
`0x1380a4`/`0x138e0c`) and re-colours the whole instruction stream.  The
largest already-identical same-address run is 93 B at `0x138b8f`; it is a
pool-relative block whose `vldr`/`ldr [pc]` displacements differ only because
the pool base differs.
