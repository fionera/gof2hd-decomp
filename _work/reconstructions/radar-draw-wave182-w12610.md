# Radar::draw wave 182 (w12610) — per-site `radarFont()` hoists (raw 6,339 -> 6,333)

**Status:** landed, 2026-11-11. Worktree `wt/w12610`, branch `wave182-w12610`,
base `master` `57e085059` (wave-181 harvest 5). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave181-w12507.md`, `-w12508.md`, `-w12508-pass2.md`,
`-w12509.md` (the 6,339 base this pass starts from) and
`radar-draw-wave180-w12407.md`. This note is self-contained: every number was
measured in this worktree.

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,339 | **6,333** (-6) |
| instruction-index alignment | 1,757 | 1,761 |
| instruction count | 3,542 ours / 3,532 orig | 3,542 / 3,532 |
| whole-`.so` `.text` differing bytes | 75,365 | **75,359** (-6) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 655 (size/base exact at `0x1a3b78`, 156,312 B) | **655** (unchanged) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` / `.rel.dyn` / `.got` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| file size | 2,187,328 | 2,187,328 |
| total differing bytes | 76,040 | **76,034** |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (three per-site `radarFont()` hoists, net size 0)

`radarFont()` is `(unsigned int) (uintptr_t) Globals::font` — a pure load of
the `Globals::font` global. At three `DrawString` call sites the reconstruction
evaluated it inline in the argument list; naming it in a local immediately
before the call changes only the scheduler's load placement (the value is
identical and the call has no side effects), and each hoist moves the load one
slot earlier in the basic block.

```diff
@@ landmark-loop label draw (line ~457)
                 int offset = i == 0 ? this->originX : this->originY;
-                Globals::Canvas->DrawString(radarFont(), *this->labelStrings->data()[i], this->screenX + offset,
-                                            this->screenY, false);
+                unsigned int lmFont = radarFont();
+                Globals::Canvas->DrawString(lmFont, *this->labelStrings->data()[i], this->screenX + offset,
+                                            this->screenY, false);

@@ enemy lock-label draw (line ~803)
-                        Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX - this->halfScreenHeight,
-                                                    this->screenY + this->halfScreenWidth, false);
+                        unsigned int enFont = radarFont();
+                        Globals::Canvas->DrawString(enFont, this->lockLabel, this->screenX - this->halfScreenHeight,
+                                                    this->screenY + this->halfScreenWidth, false);

@@ landmark i==0 tech-level label draw (line ~462)
+                        unsigned int techFont = radarFont();
                         Globals::Canvas->DrawString(
-                                radarFont(),
+                                techFont,
                                 *Globals::gameText->getText(133) + String(": ") +
```

## 2. Search evidence

Every measurement is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`).

### 2.1 Per-site `radarFont()` hoists

Each `DrawString(radarFont(), ...)` site was rewritten to name the font in a
local first. All are size-exact:

| site | raw | size |
|---|---:|---:|
| base | 6,339 | 9,908 |
| landmark label (`labelStrings`) | **6,337** | 9,908 |
| enemy lock label | **6,336** | 9,908 |
| landmark tech-level label | **6,338** | 9,908 |
| landmark distance label (`rowHeight*2`) | 6,340 | 9,908 |
| landmark distance label (`rowHeight`) | 6,339 | 9,908 |
| planet name label | 6,341 | 9,908 |
| waypoint label | 6,342 | 9,908 |
| gas-cloud label | 6,357 | 9,908 |
| asteroid docking label | 6,350 | 9,908 |

The three winners combine almost additively (landmark+enemy = 6,334;
landmark+enemy+tech = **6,333**). Adding the `rowHeight*2` distance hoist makes
it worse (6,334), so only the three winners are landed.

### 2.2 Declaration-order / alias sweeps (all size-exact unless noted)

| variant | raw | size |
|---|---:|---:|
| base | 6,339 | 9,908 |
| `campaign` declared after `turretMode` | 6,346 | 9,908 |
| `campaign` declared after `planetTargets` | 6,416 | 9,908 |
| `campaign` declared after `missionBlocksPlanets` | 9,142 | **9,900** |
| `campaign` declared before `turretMode` | 8,521 | **9,900** |
| `campaign` declared after the transform block | 9,098 | **9,912** |
| `Player *drawPlayer = player;` alias | 6,339 | 9,908 |
| `Hud *drawHud = hud;` alias | 6,339 | 9,908 |
| `Status *st = Globals::status;` alias | 9,497 | **10,060** |
| `PaintCanvas *canvas = Globals::Canvas;` alias | 9,438 | **9,996** |
| `FModSound *sound = Globals::sound;` alias | 9,524 | **9,768** |
| `Level *level = this->level;` alias | 9,698 | **10,200** |
| `unsigned int drawFont = radarFont();` at function top | 9,624 | **9,740** |

The broad aliases are all size-fatal or neutral; the compiler already keeps
those values in the same registers, so only the per-site scheduling hoists
move bytes.

### 2.3 Other cells re-confirmed on the 6,339 base

| variant | raw | size |
|---|---:|---:|
| landmark crosshair `int left` (was `float`) | 6,342 | 9,908 |
| landmark crosshair condition order (`left<screenX` first) | 6,345 | 9,908 |
| landmark crosshair condition order (`left<screenX` second) | 6,345 | 9,908 |
| enemy loop `update; wreck; field_0x75 = 0` (original call order) | 6,341 | 9,908 |
| music-tail `||` operand order `range, mask, expect` | 6,362 | 9,908 |
| music-tail `||` operand order `expect, range, mask` | 6,343 | 9,908 |
| `bossSeen = true` inside the `shipGroup == 10` arm | 8,496 | **9,900** |
| planet-lock `blip->` for `setRefPixelPosition`/`draw` | 6,434 | **9,904** |
| enemy-lock `blip->` for `setRefPixelPosition`/`draw` | 6,873 | **9,904** |
| gas `blip->` for `setRefPixelPosition`/`draw` | 9,191 | **9,976** |
| enemy-loop `int enemyShipGroup = enemy->shipGroup;` hoist | 8,767 | **9,752** |
| enemy-loop `Player *enemyPlayer = enemy->player;` alias | 9,085 | **9,924** |

## 3. Remaining residual (unchanged)

The dominant residual is still the `campaign` allocation: the original keeps
`campaign` in `sl` (`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and then reuses
`sl` for the stations array and the station id in the planet loop
(`12e3c6 mov sl,r0`, `12e4be ldr.w r0,[sl,#4]`, `12e4c4 ldr.w sl,[r0,fp,lsl#2]`),
with the planet-loop index in `fp`. Ours spills `campaign` to `sp+0x90`,
`stations` to `sp+0x84` and uses `sl` for the loop index. Every
campaign-in-`sl` source shape measured in waves 174-182 is size-short
(9,816-9,872 B), so the allocation cannot be fixed without the ~28-39
instructions the original carries elsewhere. The music-tail cold-block layout
(ours places the `field_0x1bc < 1` else block after the `if` arm's out-of-line
nested blocks, the original places it before the epilogue) and the `marked`
stack normalization (`12e6c2`) are the other visible residuals.

## 4. Reproduction

```sh
ulimit -v 12000000
cd /home/debian/GalaxyOnFire2/wt/w12610
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6333 aligned= 1761 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check      # RATCHET PASS
cmake --build cmake-build-match --target drift      # 0 field(s)
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12610/` — `Radar.base.cpp`
(base source), `cand1.cpp` (landed source), `measure.sh`, `sweep.py`,
`spec_*.json` (variant sweeps), `secdiff.py`, `diffseg.py`, `firstreal.py`,
`calls.py`, `region.py`.
