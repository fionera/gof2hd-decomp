# Hud::draw — wave 121 (w7118): marker/radar block order, boost-block 6 B, no source win

**Status:** research note, 2026-11-06 (wave 121). Worktree `wt/w7118`, base
`master bd061a489`. **No source change landed**: every variant measured at or
below the 95.986038 % raw baseline. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (file
`0x163b90`).

Gate on entry and on exit (full `check`): **RATCHET PASS**, `byte_exact 4505`,
`linked_exact 4505`, size-exact 4524/4524, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, `data_exact 843/843`, imports **207**, lint
**CLEAN**, drift **0**.

Baseline measurement: **9332 B**, `verify.py` fuzzy **96.000000 %**, raw
`SequenceMatcher` ratio **95.986038 %**, normalised instruction streams
**3149** (orig) / **3154** (ours), diff cost **160**, **101** hunks.

This note extends `hud-draw-wave119-w6944.md` and
`hud-draw-wave119-w6923.md`; read those first for the full anchor/gamma/dock
history. Everything below is new measurement.

---

## 0. TL;DR

1. **The marker/radar sub-draw block is already source-correct.**  The whole
   block `0x1b80..0x1cb8` (fire-image ternary, `isDockingToAsteroid` /
   `isMining` / `isDockingToStream` / `isInDockingProcedure` /
   `isDockingToDockingPoint` / `isInTurretMode` chain, radar lock test,
   `autoTurretOffImage` draw) is instruction-for-instruction identical to the
   original.  Its diffs are **only branch-target offsets** caused by the
   preceding boost block being 6 B too large.
2. **The boost block is the root cause of the marker/radar diffs.**  Original
   `0x16566c..0x165696` is 36 B; ours is 42 B.  Ours hoists
   `Globals::Canvas` into `r4` across `getBoostRate()` (`mov r5,r4;
   ldr.w r4,[r9]` … `mov r0,r4; mov r4,r5`) and emits `mvn #255; it mi;
   mvnmi #208`; the original loads `[r9]` after the call and emits
   `ite pl; mvnpl #255; mvnmi #208`.  The 6 B shift moves every subsequent
   branch target, including the marker/radar block.
3. **Every faithful load-after-call spelling shrinks the whole function to
   9264–9268 B and scores 86.3–86.5 %.**  The 6 B is not a local edit: the
   hoist is load-bearing for the global register allocation.  Variants tried:
   `if/else` with two `SetColor` calls, `float rate` local, `bool low` local,
   `unsigned int color` local, `PaintCanvas *canvas` local (first only / both),
   nested scopes, and swapped ternary arms.  All give the original 36 B boost
   block but a 9264–9268 B function.
4. **Anchor sweeps on top of the faithful boost block do not recover it.**
   Removing the current `Globals::Canvas &&` anchor and re-inserting it at each
   of the 105 `if (` sites in `Hud::draw` gives a best of **89.3 % / 9324 B**
   at `if (!this->field_0x280)`.  The same sweep with `int` dock types gives a
   best of **93.7 % / 9328 B**; with `String`-before-`halfW` it gives
   **94.3 % / 9340 B**.  None beats HEAD.
5. **Type/declaration sweeps are neutral or worse.**  Thirty-five single
   type/order changes (dock, damage, labels, gamma, autopilot, hint) either
   compile to byte-identical code or score below 95.986038.  A full 120-way
   permutation of the second-bar declaration units `{barH, barYOffset,
   fade-block, top, y}` finds HEAD's `HYFyT` order best (95.986038; next
   `YHFyT` 95.890846).
6. **Gamma variants still cannot beat HEAD.**  `st + Globals::status` arg2,
   `Status **stp`, `st` + hoisted index from `st`, and `st` + `stp` +
   hoisted index all lose the 8-byte slot and land at 9312 B / 75.0 %.
   The note's `Globals::status`-receiver hoisted-index variant reproduces
   both loads and the `this` split but is 95.900 % (receiver is load #2).

---

## 1. Marker/radar block: instruction-identical

Method: address-tagged normalised diff (`diff3.py`, same resolver as
`tools/verify/verify.py`) over `0x1b80..0x1cb8`.

Original `0x1b80..0x1c3c`:

```
1b80 ldrb r0,[sl,#644] ; touchFlags
1b84 lsls r0,r0,#27    ; bit 0x10
1b86 bmi  1b8a
1b88 ldrb r0,[sl,#1189] ; fireForTutorial
1b8c cmp  r0,#0
1b8e beq  1c38
1b90 ldrh r3,[sl,#998] ; field_0x3e6
1b94 ldrh r2,[sl,#996] ; field_0x3e4
1b98 ldr  r1,[sl,#736] ; orbitMarkerIdleImage
1b9c ldr  r6,[sp,#92]  ; &Globals::iPad
1ba0 ldr  r0,[r9]      ; Canvas
1ba4 ldrb r6,[r6,#0]
1ba6 cbz  r6,1bae
1ba8 movs r6,#68
1baa movs r5,#17
1bac strd r5,r6,[sp]
1bb0 blx  DrawImage2D(...,0x11,0x44)
1bb4 b    1bba
1bb6 blx  DrawImage2D(...,0x11,0x44)
1bba mov  r0,fp
...
1bee ldr  r0,[fp,#20]  ; ego->field_0x14
1bf2 ldr  r1,[r0,#20]  ; radar->field_0x14
1bf4 cbnz r1,1c02
1bf6 ldr  r1,[r0,#12]  ; radar->lockedAsteroid
1bf8 cbnz r1,1c02
1bfa ldr  r1,[r0,#36]  ; radar->lockedStation
1bfc cmp  r1,#0
1bfe beq.w 22fe       ; cold lockedEnemy test
1c02 ldr  r5,[sp,#92]
1c04 ldrh r2,[sl,#1002] ; field_0x3ea
1c08 ldrh r0,[sl,#998]  ; field_0x3e6
1c0c ldrh r6,[sl,#996]  ; field_0x3e4
1c10 adds r3,r0,r2
1c12 ldrb r5,[r5,#0]
1c14 ldr  r1,[sl,#744]  ; autoTurretOffImage
1c18 add  r2,r6
1c1a ldr  r0,[r9]
1c1e cmp  r5,#0
1c20 beq  1cb4
1c22 movs r6,#68
1c24 movs r5,#17
1c26 strd r5,r6,[sp]
1c2a blx  DrawImage2D(...,0x11,0x44)
1c2e b    1cb8
1c30 ldrh r3,[sl,#998]
1c34 ldrh r2,[sl,#996]
1c38 ldr  r1,[sl,#740] ; autoTurretOnImage
1c3c b    1b96
```

Ours has the same mnemonic/register sequence at `0x1b86..0x1c3e`; the only
differences in the whole block are the literal branch targets (`1c38` vs
`1c30`, `1c02` vs `1c0a`, `1cb8` vs `1cb0`, etc.), all shifted by the +6 B
boost block.  The cold `lockedEnemy` test at `0x22fe` is also identical.

**Conclusion:** no source reconstruction is missing in the marker/radar
sub-draws.  The assignment's "block order" hypothesis is disproved: the call
sequence census is 419/420 and the only call-order mismatch is the dock
`String`/`GetImage2DWidth` pair, not the marker/radar block.

---

## 2. Boost block: 36 B vs 42 B

Original `0x16566c..0x165696` (36 B):

```
16566c mov  r0,fp
16566e blx  getBoostRate
165672 vmov.f32 s0,#1.0
165676 vmov s2,r0
16567a ldr.w r0,[r9]      ; Canvas loaded AFTER the call
16567e vcmpe.f32 s2,s0
165682 vmrs APSR_nzcv,fpscr
165686 ite  pl
165688 mvnpl.w r1,#255
16568c mvnmi.w r1,#208
165690 blx  SetColor
```

Ours `0x16566c..0x165696` (42 B):

```
16566c mov  r0,fp
16566e mov  r5,r4         ; save &Globals::mouseCursorActivated
165670 ldr.w r4,[r9]      ; Canvas loaded BEFORE the call
165674 blx  getBoostRate
165678 vmov.f32 s0,#1.0
16567c mvn.w r1,#255
165680 vmov s2,r0
165684 mov  r0,r4
165686 mov  r4,r5         ; restore &Globals::mouseCursorActivated
165688 vcmpe.f32 s2,s0
16568c vmrs APSR_nzcv,fpscr
165690 it   mi
165692 mvnmi.w r1,#208
165696 blx  SetColor
```

`r4` in both builds is `&Globals::mouseCursorActivated` (loaded at
`0x1655b6` from `[sp,#96]`); `r9` is `&Globals::Canvas`.  The allocator
tie-break is which of the two live values gets `r4` across `getBoostRate()`.
The original keeps mouseCursor in `r4` and loads Canvas into `r5` after the
call; ours repurposes `r4` for Canvas and parks mouseCursor in `r5`.

The 6 B is exactly the block-size difference: extra `mov r5,r4` (2 B) +
`mov r4,r5` (2 B) + the `mvn`/`ite` shape (ours 4+2+4 = 10 B vs original
2+4+4 = 10 B, equal) + the extra `mov r0,r4` (2 B) = 6 B.  Every branch that
crosses the block therefore differs by 6 B, including all marker/radar
branches.

---

## 3. Faithful boost spellings: all collapse to 9264–9268 B

The original block is exactly what clang emits for a load-after-call shape.
Measured variants (full link + official metric):

| variant | size | insns | raw pct | cost |
|---|---:|---:|---:|---:|
| HEAD (ternary, Canvas hoisted) | **9332** | 3154 | **95.986038** | **160** |
| `if (rate < 1.0f) SetColor(...) else SetColor(...)` | 9264 | 3146 | 86.400000 | 347 |
| `float rate = getBoostRate(); SetColor(rate < 1.0f ? ...)` | 9264 | 3146 | 86.400000 | 347 |
| `bool low = getBoostRate() < 1.0f; SetColor(low ? ...)` | 9264 | 3146 | 86.400000 | 347 |
| `int low = getBoostRate() < 1.0f; SetColor(low ? ...)` | 9264 | 3146 | 86.400000 | 347 |
| `unsigned int color; if/else; SetColor(color)` | 9264 | 3146 | 86.400000 | 347 |
| `PaintCanvas *canvas = Globals::Canvas;` first SetColor only | 9332 | 3154 | 95.986038 | 160 |
| `PaintCanvas *canvas` both SetColors + if/else | 9264 | 3147 | 86.400000 | — |
| `PaintCanvas *canvas` both SetColors + ternary | 9264 | 3147 | 86.300000 | — |
| `PaintCanvas *&canvas = Globals::Canvas;` second SetColor | 9332 | 3154 | 95.986038 | 160 |
| nested scope around `float rate` | 9264 | 3146 | 86.400000 | 347 |
| ternary arms swapped (`>= 1.0f ? 0xffffff00 : 0xffffff2f`) | 9332 | 3154 | 95.986038 | 160 |

The load-after-call variants all produce the original 36 B boost block but
shrink the **rest** of the function by ~62 B: the hoist is what keeps the
allocator using the original high-register shape.  The anchor sweep in §4
tries to buy that pressure back and fails.

---

## 4. Anchor sweeps on the faithful boost block

Method: remove `Globals::Canvas &&` from `if (this->field_0x4b8 >= 1)`, then
insert `Globals::Canvas && ` after each of the 105 `if (` tokens in
`Hud::draw`, one at a time.  Full link + raw score.

| base | best anchor site | size | raw pct |
|---|---|---:|---:|
| faithful boost (`float rate`) | `if (!this->field_0x280)` | 9324 | 89.3 |
| `int frameH`/`int total`/`int barW` | `if (this->field_0x4b8 >= 1)` | 9328 | 93.7 |
| `String` before `halfW` | `if (show)` | 9340 | 94.3 |
| HEAD | `if (this->field_0x4b8 >= 1)` | **9332** | **95.986038** |

No anchor position recovers the faithful boost block.  The current HEAD site
is the unique global optimum.

Anchor-object sweep at the current site (HEAD base):

| anchor object | size | raw pct |
|---|---:|---:|
| `Globals::Canvas` | **9332** | **95.986038** |
| `Globals::status` | 9332 | 95.700460 |
| `Globals::layout` | 9332 | 94.177376 |
| `this->currentSecondaryWeapon` | 9336 | 88.673858 |
| `this->levelScript` | 9336 | 88.673858 |
| `Globals::galaxy` | 9344 | 88.522511 |
| `Globals::gameText` | 9280 | 85.377884 |
| `Globals::font` | 9228 | 83.982131 |
| `ego` | 9264 | 83.386277 |
| `Globals::hints` | 9264 | 83.386277 |

`Globals::Canvas` is confirmed best.

---

## 5. Type/declaration sweeps: no improvement

Thirty-five single changes were measured on HEAD; each either compiles to
byte-identical code (95.986038) or is worse.  Selected results:

| change | size | raw pct |
|---|---:|---:|
| `int dt` → `unsigned int dt` | 9332 | 95.986038 |
| `bool mining` → `int mining` | 9332 | 95.986038 |
| `unsigned int savedColor` → `int` | 9332 | 95.986038 |
| `int dividerOffset` → `unsigned int` | 9332 | 95.986038 |
| `unsigned short armorY` → `int` | 9332 | 95.986038 |
| `int armorBarY` → `unsigned int` | 9332 | 95.986038 |
| `int hitFlags` → `unsigned int` | 9332 | 95.986038 |
| `unsigned int sideImage/frontImage` → `int` | 9332 | 95.986038 |
| `int barOffset` → `unsigned int` | 9332 | 95.986038 |
| `int fadeColor` → `unsigned int` | 9332 | 95.986038 |
| `unsigned short top` → `int` | 9332 | 95.986038 |
| `int shift`/`amount`/`labelY`/`labelH`/`labelGap`/`textGap`/`textY` → `unsigned` | 9332 | 95.986038 |
| `int shieldW`/`hullW`/`regenW`/`gammaW` → `unsigned` | 9332 | 95.954307 |
| `int textH`/`transferred` → `unsigned` | 9332 | 95.954307 |
| `int halfW` → `unsigned` | 9332 | 95.922577 |
| `int alpha` → `unsigned` | 9332 | 95.890846 |
| `int canvasW`/`labelX` → `unsigned` | 9328 | 92.256427 |
| hitFlags declared before side/front images | 9336 | 87.690355 |
| `Radar *radar` → `int` | 9336 | 87.690355 |

Second-bar declaration-unit permutation (`{barH, barYOffset, fade-block, top,
y}`, 120 orders; `H`=barH, `Y`=barYOffset, `F`=fade, `T`=top, `y`=y):

| order | size | raw pct |
|---|---:|---:|
| `HYFyT` (HEAD) | **9332** | **95.986038** |
| `HYFTy` | 9332 | 95.986038 |
| `YHFyT` / `YHFTy` | 9332 | 95.890846 |
| `HYyTF` / `HYyFT` / `HYTyF` / `HYTFy` | 9332 | 95.763922 |
| `HTyYF` / `HTYyF` / `HTYFy` | 9332 | 95.763922 |
| `THyYF` / `THYyF` / `THYFy` | 9332 | 95.700460 |
| `THyFY` | 9332 | 95.668729 |

The current order is the best of the 120.

---

## 6. Gamma block: still no free two-load + slot

| variant | size | raw pct |
|---|---:|---:|
| HEAD (`Status *st`, 3 uses) | **9332** | **95.986038** |
| `st` + `Globals::status` arg2 | 9312 | 75.000000 |
| `Status **stp` (3 derefs) | 9312 | 75.000000 |
| `st` + `stp` (arg2 via `(*stp)`) | 9312 | 75.000000 |
| `st` + hoisted `stationIdx` from `st` | 9312 | 75.000000 |
| `st` + `stp` + hoisted `stationIdx` from `st` | 9312 | 75.000000 |
| `st` first + hoisted `stationIdx` from `Globals::status` | 9312 | 75.000000 |
| hoisted `stationIdx` from `Globals::status`, receiver `Globals::status` | 9332 | 95.900000 |

The faithful two-load shape always loses the 8-byte `st` slot; the hoisted
index recovers the slot but puts the receiver on load #2.  HEAD remains best.

---

## 7. Conclusion

The remaining 101 hunks are register-allocation artefacts, not missing
source blocks.  The marker/radar sub-draws are already exact; their branch
targets are displaced by the boost block's 6-byte Canvas hoist.  The hoist is
load-bearing for the global allocator: every faithful load-after-call
spelling shrinks the function by ~68 B and scores 86.3–86.5 %.  No anchor
position, anchor object, type change, declaration permutation, or gamma
spelling beats HEAD's **9332 B / 95.986038 %**.

The next lever remains the one identified in wave 119: find a source construct
that keeps the original 36 B boost block **and** the original high-register
pressure elsewhere, or a spelling that makes the dock1 allocator keep `barH`
in `fp` without an `sxth`.

---

## 8. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w7118
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS, byte 4505
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w7118`: `build.sh` (link + score), `score.py`
(rounded), `score2.py` (raw ratio + cost), `diff3.py` (address-tagged
normalised diff), `sweep_anchor.py` / `sweep_anchor_int3.py` /
`sweep_anchor_strfirst.py` (105-site anchor sweeps), `sweep_types.py`
(35 type/order variants), `sweep_secondbar.py` (120 declaration
permutations).  Pristine copy: `/dev/shm/w7118/Hud.cpp.orig`.
