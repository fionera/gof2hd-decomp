# LevelScript::process — wave 191b / w13668: case-154 state-4 raw recovery

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13668`, branch
`wave191b-w13668`, base master `2064dfd40` (raw **51,524**, aligned **21,918**,
`.text` 71,227, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

w13657 reconstructed the case-154 state-4 camera-matrix block faithfully and
paid raw +17 for aligned -262.  This pass re-derived the block from the Thumb
disassembly, confirmed it is now **instruction-identical** to the original
except for the `moveForward`/canvas-load swap, and searched for a
semantics-preserving shape that recovers the raw cost without changing size.

## Block is already faithful

Original `0x14313e..0x14329a` and ours (`0x143278..0x1433d4`, function-relative
`0xb560..0xb6bc`) are the same instruction sequence (register names differ):

```
step = delta * 0.25
moveForward(step)                 (original before the canvas load; ours after)
canvas = Globals::Canvas
local  = *CameraGetLocal(CameraGetCurrent())
m_tempVec = MatrixGetPosition(local)
m_dirVec  = player->GetDirVector()
m_tempVec += m_dirVec * (step * 0.008f)
m_dirVec  = player->geometry->getRightVector()
m_tempVec += m_dirVec * (step * -0.05f)
m_dirVec  = player->GetUpVector()
m_tempVec += m_dirVec * (step * 0.005f)
setFixed(true)
MatrixSetTranslation(local, m_tempVec)
setLocal(local)
```

The block starts at function-relative `0xb560` in ours vs `0xb426` in the
original (shift +0x13a), so the same-address byte diff in the block region is
dominated by the shift, not by the block's own shape.  The faithful order is a
local optimum for the aligned metric: every statement reorder tried below
(MatrixGetPosition/GetDirVector, moveForward, setFixed) raises aligned by
~245-250.

## Landed change

The only size-exact shape found that lowers raw without touching the faithful
term order is a vector-operator spelling on the first add:

```cpp
                     m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
                     m_dirVec = player->GetDirVector();
-                    m_tempVec += m_dirVec * (step * 0.008f);
+                    m_tempVec = m_tempVec + m_dirVec * (step * 0.008f);
                     m_dirVec = player->geometry->getRightVector();
                     m_tempVec += m_dirVec * (step * -0.05f);
```

`m_tempVec = m_tempVec + v` is semantically identical to `m_tempVec += v`; it
lowers to `AEMath::operator+` + `Vector::operator=` instead of
`Vector::operator+=`.  The fn_diff aligned metric strips callee names, so the
extra call shows up as +33 aligned, but the same-address raw diff drops 14.

Measured on the real build (exact NDK r18b `-Oz` compile + full gold link +
`--strip-all` + `tools/fn_diff.py`):

```
raw        51,524 -> 51,510  (-14)
aligned    21,918 -> 21,951  (+33)
size       57,232 / 57,232   (exact)
.text      71,227 -> 71,213  (-14)
.ARM.extab 577 delta, 156,312 B (size/base/content unchanged)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06... (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

The raw recovery is 14 of the 17 bytes w13657 cost; the aligned cost is 33 of
the 262 w13657 gained, i.e. ~82% of the raw for ~13% of the aligned.

## Candidate table (all size-exact unless noted)

| candidate | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| baseline | 51,524 | 21,918 | 57,232 | 577 |
| **first add `= m_tempVec + ...` (landed)** | **51,510** | **21,951** | **57,232** | **577** |
| second add `= m_tempVec + ...` | 51,513 | 21,949 | 57,232 | 577 |
| first add `= ... + m_tempVec` (lhs swap) | 51,510 | 21,951 | 57,232 | 577 |
| first add via named temp `v0` | 51,510 | 21,951 | 57,232 | 577 |
| first add `= m_tempVec + (step*0.008f)*m_dirVec` (float*Vector) | 51,965 | 22,158 | 57,224 | 577 |
| `MatrixGetPosition` after `GetDirVector` | 51,522 | 21,940 | 57,232 | 577 |
| `moveForward` before the canvas load (truly faithful) | 51,526 | 21,918 | 57,232 | 577 |
| `moveForward` after `GetDirVector` | 51,519 | 22,167 | 57,232 | 577 |
| `setFixed` after `MatrixSetTranslation` | 51,579 | 22,178 | 57,232 | 577 |
| old order + faithful vector terms (hybrid) | 51,513 | 22,166 | 57,232 | 577 |
| `+=` -> `= +` on both first and second adds | 52,031 | 21,958 | 57,244 | 577 |
| first add `step / 125.0f` | 51,524 | 21,954 | 57,232 | 577 |
| `step` as `(float)(delta/4)` | 51,532 | 21,934 | 57,232 | 577 |
| local-vector form (`dir`/`right`/`up`) | 52,042 | 22,131 | 57,196 | 577 |
| products-first form | 51,959 | 22,139 | 57,224 | 577 |
| MGame-style `+=` -> `= +` on all three adds | 52,046 | 22,187 | 57,268 | 577 |

All other size-exact candidates that beat 51,524 also raise aligned to
~22,16x (the old pre-w13657 order family).  `tempvec_alias`,
`const_first_paren`, `temp_named_all`, `player_local`, `local_ref`,
`local_ctor`, `local_ptr_then_copy`, `step_const_first` and
`step_div4f` compile byte-identically to the baseline.

## Neighbouring-block sweeps (no size-exact raw win)

* d1 transpositions, source lines 4000-5400 (250 cells): only the two
  case-154 cells and three semantically invalid cells
  (`m_tempVec +=` before its assignment at L4855, L4964; `rotate` before
  `setTarget` at L5249, which is the pre-w13645 order).
* d1/d2, lines 4400-4900; d1-d6, lines 4620-4700 and 4694-4720: no further
  size-exact raw win.
* State-6 (the neighbouring identical block, lines 4694-4708) `+=` -> `= +`
  is not size-exact (+16 B) because that block has no slack.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13668/` (`fast.py`, `tryit.py`,
`sweep.py`, `permsearch.py`, `blockcmp.py`, `shiftmap.py`, `diffmap.py`,
`variants*.py`, `c_*.so`).
