# Radar::draw wave 175 (w11907) — planet-lock expect moved to the dock guard

**Status:** landed, 2026-11-11. Worktree `wt/w11907`, branch `wave175-w11907`,
base `master` `5f599a137`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave174-w11909.md` (the landed planet-lock
`expect(!lockConfirmed, 0)` + `int missionBlocksPlanets` + frame-blip locals),
`radar-draw-wave174-w11908.md` (image-chain operand order) and
`radar-draw-wave173-w11809.md` (the OR swap that the `int` change later
undid).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 6,817 |
| instruction-index alignment | 1,602 |
| instructions | 3,536 ours / 3,532 orig |
| whole-`.so` `.text` differing bytes | 80,439 |
| `.ARM.exidx` differing bytes | 0 |
| `.ARM.extab` differing bytes | 750 (size/base exact) |
| `.rel.plt` | byte-identical (0) |
| gate | RATCHET PASS (byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370) |

The wave-174 brief's `.text` 80,447 is 8 bytes stale; the checked-in
`docs/IDENTICALITY.md` and a fresh build both give **80,439** on `5f599a137`.

## 1. The residual, exactly

The planet-lock block (original `0x130454`) is the only block still shifted
from the original's layout.  The wave-174 `expect(!lockConfirmed, 0)` on the
lock-sound test is what does it:

```
original 130454:  ldrb r0,[r4,#536]      ; lockConfirmed
         130458:  cbnz r0,130470         ; skip sound, body inline
         13045a:  ... play(26) ...
         130470:  ldrb r0,[r4,#424]      ; field_0x1a8
         130474:  ldr  r1,[r4,#24]       ; field_0x18
         130476:  cmp  r0,#0
         130478:  str  r1,[r4,#20]       ; field_0x14 = field_0x18
         13047a:  beq  130486            ; dock body inline
         13047c:  ... dockToPlanet() ...
base (w11909):  ldrb r0,[r4,#536]
         1303ca:  cmp  r0,#0
         1303cc:  beq.w 1305d8           ; sound body moved out of line
```

`expect(..., 0)` marks the sound body cold and moves it into the exception
cleanup tail, adding the +8 B landing-pad site that wave 174 used as a size
compensator.  Removing the expect restores the original `cbnz` + inline
sound body, but drops the function to 9,900 B — the "size-fatal" part of the
brief.

## 2. Landed change (one commit, four sites)

```diff
-                    if (__builtin_expect(!this->lockConfirmed, 0)) {  // lint: arm_builtin
+                    if (!this->lockConfirmed) {
                         Globals::sound->play(26, nullptr, nullptr, 0.0f);
                     }
                     this->field_0x14 = this->field_0x18;
-                    if (this->field_0x1a8) {
+                    if (__builtin_expect(this->field_0x1a8, 0)) {  // lint: arm_builtin
                         this->level->getPlayer()->dockToPlanet();
                     }
                 }
-                if (this->planetLockTimer < 1 || missionBlocksPlanets) {
+                if ((this->planetLockTimer < 1) | (missionBlocksPlanets != 0)) {
                     goto enemies;
                 }
-                Sprite *blip = this->blipSprite;
+                Sprite *blip;
                 int frame;
                 if (__builtin_expect((this->field_0x18 != 0), 1) && this->field_0x18 != this->field_0x14) {  // lint: arm_builtin
+                    blip = this->blipSprite;
                     frame = (int) ((float) (blip->getRawFrameCount() - 1) *
                                    ((float) this->planetLockTimer / (float) this->field_0x1b8));
                 } else {
                     if (__builtin_expect((__builtin_expect(this->level->getPlayer()->isDockingToPlanet(), 1)), 0)) { // lint: arm_builtin anchor: Radar::draw
                         goto enemies;
                     }
-                    frame = this->blipSprite->getRawFrameCount() - 1;
+                    blip = this->blipSprite;
+                    frame = blip->getRawFrameCount() - 1;
                 }
-                this->blipSprite->setFrame(frame);
+                blip->setFrame(frame);
```

All four sites are semantics-preserving:

* **`!lockConfirmed` plain.**  The sound is still played exactly when the
  lock is not yet confirmed; only the branch weight hint is removed.
* **`expect(field_0x1a8, 0)`.**  `field_0x1a8` is the "dock to planet" flag,
  normally false; the hint is a pure branch-weight annotation.
* **`(timer < 1) | (mbp != 0)`.**  `|` instead of `||`; both operands are
  side-effect-free loads/compares, so the truth value is identical.
* **planet `Sprite *blip` assigned in both arms.**  Same pointer, same
  calls; only the SSA shape changes (the original loads the sprite pointer
  into `r5` in both arms and reuses it for `setFrame`).

### 2.1 Size accounting (all measured with `link_so` + `fn_diff`)

| site | Δ size |
|---|---:|
| remove planet-lock `expect(!lockConfirmed, 0)` | -8 |
| `expect(field_0x1a8, 0)` (new landing-pad site) | +8 |
| `(timer < 1) \| (mbp != 0)` (replaces `\|\|`) | -4 |
| planet `blip` local in both arms (was first-arm only) | +4 |
| **net** | **0 -> 9,908 B** |

The new `field_0x1a8` expect is the "different size-exact shape": it puts the
+8 B cold-block/landing-pad back at a site the original does not care about
(the dock call is the genuinely rare path), while the lock-sound test gets
the original's inline `cbnz` shape back.

## 3. Result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | 9,908 |
| raw same-address diff | 6,817 | **6,798** |
| instruction-index alignment | 1,602 | 1,646 |
| instructions | 3,536 / 3,532 | 3,541 / 3,532 |
| whole-`.so` `.text` differing bytes | 80,439 | **80,420** |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 750 | 750 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |

`check` after the commit: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub 0, missing 0, wrong_type 0,
extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**, drift **0**,
parity **5,370/5,370**.  `RATCHET PASS`.

The aligned count rises 1,602 -> 1,646 (+44) while raw falls 6,817 -> 6,798
(-19): the raw same-address metric is the fleet's primary local metric and it
is the direct byte-identity measure, so the -19 is the landed win.  The
aligned rise is the two extra `blip = this->blipSprite` assignments (P6) and
the dock-block move; it is reported honestly rather than hidden.

## 4. Variant table (all size 9,908 unless noted)

Knobs: **station** S0 = no blip local, S1 = hoisted local (wave-174 base),
S5 = local assigned in both arms; **planet** P0 = no local, P2 = wave-174
first-arm local, P6 = local assigned in both arms; **expect** E0 = plain,
E1 = `expect(!lockConfirmed,0)` (wave-174 base); **guard** Gor = `||`,
Gbit = `|`; **mbp** int/bool; **frame** F1 = `expect(field_0x18!=0,1)`
(wave-174 base), F0 = plain; **x3** = `expect(field_0x1a8,0)`.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| **S1 P6 E0 Gbit int + x3 (landed)** | **6,798** | 1,646 | **9,908** |
| S5 P2 E0 Gbit int (no extra expect) | 6,800 | 1,653 | 9,908 |
| S1 P2 E1 Gor int (wave-174 base) | 6,817 | 1,602 | 9,908 |
| S5 P0 E0 Gor int | 6,826 | 1,618 | 9,908 |
| S0 P6 E0 Gor int | 6,828 | 1,616 | 9,908 |
| S0 P0 E0 Gor bool (original-like `orrs`) | 6,833 | 1,611 | 9,908 |
| S1 P2 E0 Gor int + x3 | 6,836 | 1,562 | 9,908 |
| S1 P6 E0 Gor bool (original-like `orrs`) | 6,840 | 1,605 | 9,908 |
| S1 P6 E1 Gbit int + x3 | 6,810 | 1,659 | 9,908 |
| S1 P2 E0 Gor int (expect removed only) | 6,900 | 1,555 | 9,900 |
| S1 P2 E1 Gor int F0 (plain frame test) | 6,817 | 1,602 | 9,908 |
| E0 + station `expect(!lockConfirmed,0)` | 6,846 | 1,607 | 9,908 |

Key negative results:

* **`bool missionBlocksPlanets`** restores the original `orrs r0,r5,r1`
  guard shape (the wave-173 win), but costs +4 B and is raw-worse
  (6,833-6,840).  With `int` clang canonicalises every `|`/`||` spelling to
  `movlt r5,#1; cmp r5,#0`, which is the same size but not the original
  encoding.
* **Removing only the planet-lock expect** is 9,900 B (size-fatal) and
  raw-worse (6,900) because the inline sound body shifts the whole tail.
* **Moving the expect to the station `!lockConfirmed`** is size-exact
  (9,908) but raw-worse (6,846).
* **F0 (plain frame test)** restores the original `cbz` but is +4 B and
  raw-worse (6,864 on the S5 P2 Gbit base); the frame-test expect is also a
  size compensator.
* `dock` (`isDockingToPlanet` expect vs plain) is codegen-neutral.

## 5. Remaining residual

The dominant residual is unchanged: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`) and `missionBlocksPlanets` in `fp`, while ours spills
`campaign` to `sp+144` and uses `sl` for the shared zero.  That cascade is
still the biggest single lever (`radar-draw-wave174-w11907.md` §3-4: forcing
the allocation alone is -92 B).  The planet block itself is now close: the
remaining local deltas are the `movlt r5,#1` vs `orrs` guard encoding and the
`r0` vs `r5` blip register.

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w11907/` —
`Radar.cpp.base` (base source), `gen.py` (variant generator),
`evalvar.py`/`measure.sh` (build+measure), `sweep.py`/`sweep2.py`/
`sweep3.py`/`sweep4.py`/`sweep_extra.py` (knob grids), `region.py`
(per-region byte diff), `textdiff.py` (whole-`.so` section diff),
`cand_6798.cpp` (landed source).
