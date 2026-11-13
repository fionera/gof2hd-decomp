# LevelScript::process — wave 191b / w13677: recover the w13657/w13667 raw cost

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13677`, branch
`wave191b-w13677`, base master `b221b3826` (raw **51,534**, aligned **21,862**,
`.text` diffbytes 71,247, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The wave-191b handoff (w13667 case-158 state-1/2 order, w13657 case-154
state-4 camera-matrix block) landed two faithful semantic reconstructions at
raw +10 and +17 respectively.  This pass re-measures the pre-reconstruction
code and searches those two blocks (and their immediate neighbours) for
compensating shapes — store order, temporary lifetimes, vector-operator
spelling — that recover the raw cost while keeping the faithful semantics.

## Reverting each reconstruction (on the current base)

| revert | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| baseline (both landed) | 51,534 | 21,862 | 57,232 | 577 |
| revert case-154 state-4 (w13657) | 51,517 | 22,124 | 57,232 | 577 |
| revert case-158 state-1/2 (w13667) | 51,524 | 21,918 | 57,232 | 577 |

So the two costs are exactly the +17 (case-154) and +10 (case-158) reported by
the landing passes.  Both reverts are size-exact and extab-neutral: the raw
delta is a pure codegen-shape effect, not a size effect.

## Why the raw cost is not local

The state-4 block disassembles to the *same instruction sequence* as the
original (`0x14313e`..`0x14329a` vs our `0x14329a`..`0x1433ea`); only the
register assignment differs (`r5`/`r9`, `fp`/`sl`, `r6`/`r4`) and the block
sits 0x162 bytes later.  The state-158 state-1 block is likewise a near
instruction-for-instruction match (`0x138878` vs `0x138862`).  A whole-function
byte cross-correlation peaks at shift 0 (4,399 matching bytes) and never
recovers a global shift, so `raw` is dominated by clang's global register
allocation, not by a local statement shape.  Changing the case-154 block
perturbs code as far away as `0x138866` (case-158 state 1) via the allocator.

## Landed change (case-154 state-4)

Move `moveForward(step)` after the first vector term and give the direction
and right-vector reads a local lifetime (the member stores are kept, so the
observable state is unchanged):

```cpp
                     m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
-                    m_dirVec = player->GetDirVector();
-                    m_tempVec += m_dirVec * (step * 0.008f);
-                    m_dirVec = player->geometry->getRightVector();
-                    m_tempVec += m_dirVec * (step * -0.05f);
+                    Vector dir = player->GetDirVector();
+                    m_dirVec = dir;
+                    m_tempVec += dir * (step * 0.008f);
+                    (*enemies)[0]->geometry->moveForward(step);
+                    Vector right = player->geometry->getRightVector();
+                    m_dirVec = right;
+                    m_tempVec += right * (step * -0.05f);
```

`moveForward` writes `enemies[0]->geometry`; the three vector terms write
`m_tempVec`/`m_dirVec`.  The statements are independent, so the move is
semantics-preserving; the locals carry the same values into the same member
stores.

Measured on the real build (exact NDK r18b `-Oz` compile + full gold link +
`--strip-all`, `tools/fn_diff.py`):

```
raw        51,534 -> 51,525  (-9)
aligned    21,862 -> 21,902  (+40)
size       57,232 / 57,232   (exact)
.text      diffbytes 71,247 -> 71,238 (-9); size/base unchanged
.ARM.extab 577 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

This recovers 9 of the 17 raw bytes of the case-154 reconstruction.  The
aligned metric gives back 40 of the 262 aligned bytes that reconstruction
bought; the baseline is the minimum of the aligned metric over every
size-exact shape measured (see table), so no shape recovers raw *and* aligned.

## Candidate table — case-154 state 4 (all size-exact unless noted)

| candidate | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| baseline | 51,534 | 21,862 | 57,232 | 577 |
| **landed: moveForward after add1 + local dir/right** | **51,525** | **21,902** | **57,232** | **577** |
| moveForward after GetDirVector + local right | 51,527 | 21,892 | 57,232 | 577 |
| moveForward after MatrixGetPosition + local dir/up | 51,522 | 22,112 | 57,232 | 577 |
| moveForward after add2 + local dir/right | 51,529 | 21,916 | 57,232 | 577 |
| setFixed at block top / after canvas | 51,532 | 22,088 | 57,232 | 577 |
| setFixed after MatrixSetTranslation | 51,589 | 22,122 | 57,232 | 577 |
| local dir temp only (moveForward unchanged) | 51,527 | 22,112 | 57,232 | 577 |
| local right temp only | 52,049 | 22,110 | 57,228 | 576 |
| local up temp only | 52,052 | 21,870 | 57,228 | 576 |
| Vector*float → float*Vector, all three terms | 51,534 | 21,862 | 57,232 | 577 |
| commuted float multiply (`0.008f * step`) | 51,534 | 21,862 | 57,232 | 577 |
| two-multiply spelling (`m_dirVec * step * 0.008f`) | 52,041 | 22,126 | 57,244 | 1,260 |
| `m_tempVec = m_tempVec + …` (operator+ then =) | 52,055 | 22,131 | 57,268 | 1,339 |
| `-(step * 0.05f)` instead of `(step * -0.05f)` | 51,984 | 22,079 | 57,224 | 577 |
| moveForward truly faithful (before canvas) | 51,536 | 21,862 | 57,232 | 577 |
| moveForward after GetDirVector, no locals | 51,969 | 21,643 | 57,224 | 577 |
| moveForward after add1, no locals | 51,970 | 21,656 | 57,224 | 577 |
| moveForward after MatrixGetPosition, no locals | 51,969 | 21,861 | 57,224 | 577 |
| state-2 setFixed moved to faithful position | 52,094 | 21,866 | 57,236 | 1,263 |
| state-2 setFixed faithful + local dir (state 4) | 51,534 | 22,111 | 57,232 | 577 |

The `-8`-byte forms (moveForward moved, no local compensation) give the best
aligned numbers (21,643) but shrink the function; no faithful `+8` reshape was
found that keeps the aligned gain.  The landed form is the best raw recovery
that is size-exact.

## Candidate table — case-158 state 1/2

| candidate | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| baseline (w13667 faithful order) | 51,534 | 21,862 | 57,232 | 577 |
| approach commuted (`1.0f + …`) | 51,534 | 21,862 | 57,232 | 577 |
| approach as `1.0f - timer/34000` | 51,534 | 21,862 | 57,232 | 577 |
| `float step = delta;` (no cast) | 51,534 | 21,862 | 57,232 | 577 |
| step declared before approach | 51,534 | 21,862 | 57,232 | 577 |
| approach inlined in translate | 51,534 | 21,862 | 57,232 | 577 |
| field_0x24 after resetGunDelay (unfaithful) | 51,533 | 21,866 | 57,232 | 577 |
| swap forceUpdate / timerB (d3) | 51,532 | 21,872 | 57,232 | 577 |
| forceUpdate after setVulnerable (d4 1↔5) | 51,533 | 21,871 | 57,232 | 577 |
| setSpeed after hud (unfaithful) | 51,535 | 21,869 | 57,232 | 577 |
| resetGunDelay via local `player` | 52,840 | 21,629 | 57,224 | 577 |
| timer increment via int local | 54,443 | 21,875 | 57,224 | 579 |

The state-1 arithmetic has only one codegen shape: every faithful spelling of
`(float) m_nScriptTimerA / -34000.0f + 1.0f` compiles to identical bytes.  The
state-2 teardown is already at the raw/aligned optimum of the faithful order;
the only raw-lowering moves are small deviations from the w13667 order.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13677/` (`base.cpp`, `measure.sh`,
`batch.py`, `try.py`, `diffmap.py`, `sec.py`, `gen*.py`, `v*.json`, `r*.json`).
