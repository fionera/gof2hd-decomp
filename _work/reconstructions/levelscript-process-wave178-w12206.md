# LevelScript::process — wave 178 / w12206 (dispatch order, pools, data flow)

Target `_ZN11LevelScript7processEi` (57,232 B, original `0x137d18`). Base master
`1b48f990f` (wave-177 merge). Baseline raw **53,507**, aligned **18,941**, size
**57,232 exact**. Gate at the start and end of this pass: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. No code change landed; `src/` is at the base.

## 1. What the residual actually is

The function is size-exact but almost every byte differs because of a **global
callee-saved register swap** in the prologue:

```
original:  mov sl, r0            ; this  -> sl
           ldr.w fp, [r0]        ; guard -> fp
ours:      mov fp, r0            ; this  -> fp
           ldr.w sl, [r0]        ; guard -> sl
```

Everything downstream inherits the swap: `[sl,#24]` vs `[fp,#24]`, the
exception landing pads (`mov sl,r0` in the original, `mov fp,r0` in ours), and
the choice of scratch registers. The original also reuses `fp` as a general
scratch register later (e.g. `ldr.w fp,[sp,#72]` at `0x138140`), which our build
cannot do because `fp` holds `this`.

A second, independent cause is **literal-pool placement**. The first pool is at
`0x1380a4` in the original and `0x1380c0` in ours (28 B later); the pool used by
the mission-89 camera block is at `0x138e1c` vs `0x138d70` (172 B later). Every
`ldr rX,[pc,#N]` / `vldr` that targets those pools therefore differs, even when
the instruction shape is identical. The longest same-byte run in the function is
93 B at `0x138b8f..0x138beb`; it is bounded on both sides by pool-relative
loads.

## 2. Mission-dispatch order is wrong

The original's pre-switch mission chain is (comparison order):

```
5, 81, 26, 78, 50, 89, 106, 95/99/109/119/126/133/160/161, 51
```

Our source order is:

```
81, 78, 89, 106, 95..., 5, 26, 50, 51
```

The original also runs the shared "active" tail (`0x137f7c`) **before** the
mission-5 chain; our source places the tail after the whole chain. The original
mission-5 block is at `0x13812e`, immediately after the tail; ours is at
`0x137f82` when moved before mission 81 without moving the tail.

A full restructure (tail before the chain, chain reordered to
`5,81,26,78,50,89,106,95,51`) does put mission 5 at the original `0x13812e`,
but the function grows to 57,476 B (+244) because the switch guard is laid out
immediately after mission 5 and the 106/95/51 blocks become cold tail blocks.
Size is sacred, so this was not landed.

## 3. Experiments (all size-exact unless noted)

| change | raw | size | verdict |
|---|---:|---:|---|
| base | **53,507** | 57,232 | — |
| drop `Status *status` local, call `Globals::status->...` twice | 53,508 | 57,232 | +1, rejected |
| `Level *level = m_pLevel` for the three getters | 53,554 | 57,232 | +47 |
| `LevelScript *self = this` for the three getters | 53,507 | 57,232 | neutral |
| `bool fading` -> `int` | 53,507 | 57,232 | neutral |
| `bool docked` -> `int` (both sites) | 53,507 | 57,232 | neutral |
| `for (unsigned int i...)` -> `int i` (one loop) | 53,507 | 57,232 | neutral |
| all 47 `for (unsigned int` -> `for (int` | 53,901 | 57,236 | +4 B |
| `if (!(depth < 1.0f))` -> `if (depth >= 1.0f)` | 54,850 | 57,224 | -8 B, `vmin` instead of `vcmpe` |
| same + `rumble < 0` -> `!(rumble >= 0)` | 54,286 | 57,232 | still worse |
| `m_bStartSequence != 0` -> `m_bStartSequence` | 53,507 | 57,232 | neutral |
| `reinterpret_cast<float&>` -> `*(float*)` | 53,507 | 57,232 | neutral |
| `lvl->getPlayer()->setCollide` -> `player->setCollide` | 53,525 | 57,232 | +18 |
| `lvl->getPlayer()->player->setVulnerable` -> `player->...` | 53,525 | 57,232 | +18 |
| swap `m_bStartSequence` / `m_bStartSequenceOver` stores | 53,507 | 57,232 | neutral |
| `-mllvm -disable-postra-machine-sink` removed | 53,509 | 57,232 | +2 |
| `-mllvm -regalloc-csr-first-time-cost=0/1/2` | 53,507 | 57,232 | neutral |

`vmin` is 6 B shorter than the original `vcmpe/vmrs/it/vmovmi` clamp, so the
`depth >=` cell is not a code-shape match; its raw movement is pure layout.

## 4. Recommendations for the next pass

1. **Attack the `sl`/`fp` swap first.** It is worth far more than any local
   cell. Try source shapes that change the live range of `this` or of the
   stack-guard pointer (e.g. a local `Status *` that is *not* live across the
   whole function, a `Level *level` alias, or a different first-use order).
   A single flip would align the prologue, the mission-1 block and the landing
   pads at once.
2. **Then align the first pool.** The mission-1 block is instruction-for-
   instruction identical except for the pool distance (28 B). If the
   pre-pool code shrinks by 28 B (or the post-pool code grows by 28 B), the
   mission-1 `vldr`/`ldr` offsets match and the block becomes a large same-byte
   run.
3. **Do not move the mission chain without moving the tail.** The original tail
   is at `0x137f7c`, before mission 5. Moving blocks inside the current
   `else if` chain only makes the function larger.
4. **106/95/51 are chain blocks, not switch cases.** The branch chain reaches
   them in the order `106, 95..., 51`; the switch guard (`0x13879e`) is laid out
   between the mission-51 check and the mission-106 body. Any restructure must
   preserve that layout, or the switch guard moves and the size changes.
