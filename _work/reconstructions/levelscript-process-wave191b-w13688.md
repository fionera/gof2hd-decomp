# LevelScript::process — wave 191b / w13688: case-158 s1 marker temp lifetimes

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13688`, branch
`wave191b-w13688`, base master `b3fd7e6a2` (raw **51,535**, aligned **21,707**,
`.text` diffbytes 71,270, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

Second parallel angle: recover the raw cost of the w13676 (case-24 state-1,
raw +15) and w13667 (case-158 state-1/2, raw +10) semantic reconstructions
with compensating shapes in those blocks or their neighbours.  Both blocks are
now statement-faithful; the remaining raw is dominated by block placement and
register allocation, not by statement order:

* case-24 state-1 (`0x13a960`): ours starts at `0x13a954` (delta -12) and is
  4 B longer (three `ldr.w rX,[fp,#24]` vs the original's 16-bit
  `ldr rX,[r6,#24]`); the block is instruction-identical apart from the
  `m_nState = 2` store, which clang sinks into the shared tail at `0x144640`
  while the original keeps it in the block.
* case-158 state-1 (`0x138878`): ours starts at `0x138862` (delta -22) and is
  6 B longer (same fp-vs-low-register widening plus different stack-slot
  colouring for the four `Vector` temporaries).
* case-158 state-2 teardown (`0x140414`): the block is **instruction-identical**
  to the original (same registers, same stack slots) but shifted -832 B.

## Landed change

The only faithful, size-exact shape found that lowers raw is a temporary-
lifetime change in the case-158 state-1 marker-direction statement:

```cpp
                     AEGeometry *marker = m_pGeometry4;
                     Vector target{9000.0f, 0.0f, -13000.0f};
-                    marker->setDirection(AbyssEngine::AEMath::VectorNormalize(target - marker->getPosition()),
-                                         Vector{0.0f, 1.0f, 0.0f});
+                    Vector pos = marker->getPosition();
+                    Vector up{0.0f, 1.0f, 0.0f};
+                    marker->setDirection(AbyssEngine::AEMath::VectorNormalize(target - pos), up);
```

Same statements, same call order, same semantics (the original also stores the
`getPosition()` result and the up vector in stack temporaries).  The compiler
picks a different stack-slot permutation, which happens to match more original
bytes in the block.

Measured on the real build (exact NDK r18b `-Oz` compile + full gold link +
`--strip-all` + `tools/fn_diff.py`):

```
raw        51,535 -> 51,532  (-3)
aligned    21,707 -> 21,714  (+7)
size       57,232 / 57,232   (exact)
.text      diffbytes 71,270 -> 71,267 (-3); size/base unchanged (1,259,464 B)
.ARM.extab 577 delta, 156,312 B, md5 ae1f5fc3… (content unchanged)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

The raw-weighted trade is the same kind as w13668 (raw -14 / aligned +33); the
block's own same-address diff drops 266 -> 263.

## Candidate table (all size-exact unless noted)

| candidate | raw | aligned | size | extab | rel.plt |
|---|---:|---:|---:|---|---|
| baseline | 51,535 | 21,707 | 57,232 | 577 | ok |
| **case-158 s1 `pos`/`up` locals (landed)** | **51,532** | **21,714** | **57,232** | **577** | **ok** |
| `up` local only | 51,534 | 21,713 | 57,232 | 577 | ok |
| `pos` local only | 51,535 | 21,708 | 57,232 | 577 | ok |
| `pos`+`dir`+`up` locals | 51,534 | 21,713 | 57,232 | 577 | ok |
| `up` before `pos` | 51,534 | 21,713 | 57,232 | 577 | ok |
| `pos` before `target` | 51,534 | 21,731 | 57,232 | 577 | ok |
| `pos`+`up`+`target` order | 51,956 | 21,716 | 57,232 | 577 | ok |
| `target = target - pos` reuse | 55,081 | 21,741 | 57,240 | 577 | ok |
| `target + -pos` | 55,125 | 21,725 | 57,240 | 577 | ok |
| case-24 anchor after `setVisible`/before `reset` | 51,533 | 21,895 | 57,232 | 577 | ok |
| case-24 anchor in body before `setVisible` | 51,536 | 21,719 | 57,232 | 577 | ok |
| case-24 anchor before `open` | 51,534 | 21,895 | 57,232 | 577 | ok |
| case-24 anchor after `open` | 51,536 | 21,686 | 57,232 | 577 | **no** |
| case-24 anchor at case top | 52,687 | 21,680 | 57,232 | 577 | ok |
| case-24 anchor + 2 extra `autoTurretIsEnabled()` reads | 54,488 | 21,704 | 57,248 | 577 | **no** |
| case-158 s2 `field_0x24` after `resetGunDelay` (pre-w13667 order) | 51,534 | 21,711 | 57,232 | 577 | ok |
| case-158 s2 timers after `setCloakingPossible` | 55,376 | 21,605 | 57,276 | 577 | ok |
| case-158 s2 `setSpeed` after HUD | 51,536 | 21,714 | 57,232 | 577 | ok |
| case-158 s2 `setDirection` dir/up locals | 52,853 | 21,558 | 57,224 | 577 | ok |
| remove `Status *status` local | 51,536 | 21,705 | 57,232 | 577 | ok |
| `status` declared first | 55,332 | 21,884 | 57,312 | 577 | ok |

Adjacent-statement swaps over the whole case-158 state-2 teardown (19 cells)
and the case-24 state-1 body (5 cells) found no other size-exact raw win; the
only negative cell is the pre-w13667 `field_0x24`/`resetGunDelay` order
(raw -1, aligned +4), which undoes w13667's faithful order.

## Semantic finding (not landed: needs an 8 B compensation)

The original runs the case-24 state-2 body **on the same frame** as the
state-1 -> 2 transition: the state-1 body ends `b.w 0x13c690`, and `0x13c690`
is the state-2 body (`getPosition` / `updateEvent3DAttributes`), reached by
the `m_nState == 2` dispatch path at `0x13c68a`.  Our source uses
`} else if (m_nState == 2) {`, so the state-2 body is skipped on the transition
frame (ours branches to the shared tail store at `0x144640` and then the
post-switch tail).

Spelling it as two independent `if`s restores the original control flow and
the original in-block `m_nState = 2` store:

```cpp
                     m_nState = 2;
-                } else if (m_nState == 2) {
+                }
+                if (m_nState == 2) {
```

Measured: raw **54,384**, aligned **21,706**, size **57,240** (+8), extab
dc921c90.  The +8 comes from register-allocation widening after the restructure
(one fewer instruction, but wider encodings).  It needs an 8 B size-exact
compensation elsewhere before it can land; the aligned metric is already -1.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13688/` (`harness.py`, `sweep1.py` …
`sweep26.py`, `block.py`, `shiftmap.py`, `base_LevelScript.cpp`, `base.so`,
`ifif.so`, `base.txt`, `ifif.txt`).
