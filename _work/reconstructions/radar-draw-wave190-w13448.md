# Radar::draw wave 190 (w13448) — station-lock blip call order
# raw 6,267 -> 6,265 (-2), size-exact, `.rel.plt` identical

**Status:** landed, 2026-11-12. Worktree `wt/w13448`, branch `wave190-w13448`, base
`master` `8d8f43bcd`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13437.md` (the -6 branch-target shift), `-w13435.md`
(hint-pair removal), `-w13433.md` (planet-lock store order), `-w13426.md`, `-w13422.md`,
`-w13412.md` and the campaign/`sl` family `radar-draw-campaign-sl-wave164..167-*.md`.

## 0. The change

At the station-lock blip (inside the landmark loop, `blip->setFrame(frame);`), the
`setRefPixelPosition` and `draw` calls were emitted in the opposite source order from the
original. Swapping the two independent calls on the same `blipSprite` object removes two
same-address bytes:

```cpp
                     blip->setFrame(frame);
+                    this->blipSprite->draw(1.0f, 1.0f);
                     this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                                           (int) PlayerEgo::crosshairPos.y);
-                    this->blipSprite->draw(1.0f, 1.0f);
```

The compiler re-orders the two calls back to `setRefPixelPosition` then `draw` in the
emitted code (`12f948 blx setRefPixelPosition`, `12f958 blx draw`), byte-identical to the
original, so the call sequence is unchanged; the source order only perturbs the IR block
order and therefore the cold-block placement of the music tail. The whole -2 is in the
tail (`130490` 58->59, `1304d0` 61->58 per-64-byte diff); the blip block itself does not
change.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,267 | **6,265 (-2)** |
| instruction-index alignment | 1,966 | 2,010 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 72,595 | **72,593 (-2)** |
| `.ARM.extab` differing bytes | 584 | 584 (no growth, size/base exact) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total (text+extab+build-id) | 73,199 | **73,197 (-2)** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 1. How it was found

An exhaustive **statement-level** adjacent-statement swap sweep over the whole
`Radar::draw` body (446 statements, 83 swappable adjacent pairs at equal brace depth,
including multi-line statements such as the `DrawRegion2D`/`DrawString` calls, which the
earlier line-based sweeps excluded). Only the station-lock pair wins; every other pair is
neutral (`raw=6267`, size-exact) or worse. Scratch
`/home/debian/GalaxyOnFire2/scratch/w13448/stmtswap.py`.

## 2. Negative results measured this pass (all reverted)

All measured with `ulimit -v 12000000`, `link_so -j2`, `tools/fn_diff.py`; scratch
`/home/debian/GalaxyOnFire2/scratch/w13448/`.

* **`&&`/`||` operand swap sweep** — every one of the 200 `&&`/`||` operators in
  `Radar::draw`, swapping its immediate left/right operands at the same paren depth
  (`andsweep.py`). No row below 6,267. Neutral ties at 6,267: `dockNavPtr isDead/isDying`,
  `screenY > top` / `top > screenY-size*2`, `turretMode`/`distance<=range`,
  `field_0x11e`/`field_0x11f`, `!turretMode`/`!enemyHit`, `hasDockNav`/`lockedEnemy==nullptr`,
  `field_0x10 isDead/isDying`, `current != 151`/`alwaysEnemySeen`.
* **`__builtin_expect` value flips** — all 29 hints flipped individually (`flipsweep.py`)
  and all 28 pairs among the size-exact singles (`flippairs.py`). Best: `h14+h16` neutral
  at raw 6,267 (aligned 1,899); `h11` 6,269; `h7` 6,281; `h22` 6,456; `h27` 6,410.
* **Per-TU regalloc flags** — `-regalloc-csr-first-time-cost` 4,500 neutral, 5,000-8,000
  -> 6,269 (confirming the earlier sweep); `-split-spill-mode=speed`,
  `-disable-spill-fusing`, `-disable-spill-hoist`, `-condsider-local-interval-cost` all
  neutral; `-enable-deferred-spilling` 8,770 (size 9,916), `-split-spill-mode=size` 9,469,
  `-sink-insts-to-avoid-spills` 9,765, `-regalloc=basic` 9,894, `-stress-regalloc=6`
  10,594.
* **Hull/EMP `DrawRegion2D` block** — a local `float barMax = (float)this->field_0x1c8;`
  used in both blocks, a separate local per block, and an `int` local are all byte-identical
  to the base (6,267); clang CSEs the `(float)field_0x1c8` conversion into `s24` regardless.
  The original instead loads/converts `field_0x1c8` separately in each block (`12f278`,
  `12f310`). `this->barWidth = (int) hull;` **before** the hull `DrawRegion2D` (the
  original's order: `12f2a6 vstr s2,[r4,#388]` then `12f2c4 blx`) is size-fatal
  (9,912, raw 8,280); so are the same reorder with `int marked` (9,916) and with the
  `field_0x1bc` inversion (9,912).
* **Wreck x/y block** — hoisting `x = this->screenX;` before the `field_0x11d/0x11e/0x11f`
  chain is size-fatal (9,904); the original's four separate `ldr [r4,#0xfc]` are not
  reachable without volatile.
* **`enemy->field_0x73 = 0;` forms** — `false`, `(uint8_t)0`, `0u`, `'\0'`: all neutral.
  Moving it after the `cx`/`cy` declarations: 6,281.
* **Campaign recompute** — replacing the `campaign >= 2` use with a fresh
  `getCurrentCampaignMission()` call: 9,904 (size-fatal). `unsigned campaign`: 6,268.
  `campaign > 1` / `1 < campaign`: neutral.
* **Music-block duplicate `field_0x1bc = 0;` store** in both branches: 9,928 (size-fatal).

## 3. Root cause still open

The dominant residual remains the callee-saved rotation documented in
`radar-draw-campaign-sl-wave166-w11106.md`: original `zero/missionBlocksPlanets` in `fp`,
`campaign` in `sl`, `marked` on the stack (`sp+132`); ours `zero/mbp` in `sl`, `campaign`
spilled (`sp+144`), `marked` in `fp`. Every source shape tried here (type/order/volatile/
recompute) and every `-mllvm` regalloc knob tried leaves the assignment unchanged; the
`campaign` spill costs the `12e38a` `ldr`/`cmp` pair and the `stations` spill costs a
`ldr [sp,#132]` reload at each of its ~6 loop uses, which is the bulk of the
`12e3d0`-`12ef50` shift cascade.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6265 aligned= 2010 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
