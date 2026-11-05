# Hud::draw — wave 119 (w6745): register-rotation audit, no source win

**Status:** research note, 2026-11-05 (wave 119). Worktree `wt/w6745`, branch
`wave119-w6745`, base `master b5cbf56bd`. No source change landed: every variant
measured at or below the 95.986038 % baseline. Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (file `0x163b90`).

Gate on entry and on exit (full `check`): **RATCHET PASS**, `byte_exact 4505`,
`linked_exact 4505`, size-exact 4524/4524, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**,
drift **0**. 27 sections, file size 2,187,328 B, `.ARM.extab 0x26290` (8 B short,
`createShip` only — unchanged), `.rodata 0x41c27`.

Baseline measurement: **9332 B**, fuzzy **95.986038 %**, diff cost **160**.

---

## 0. TL;DR

1. **Diff decomposition.** Normalised instruction streams: original **3149**
   instructions, ours **3154** (we carry 5 extra). `SequenceMatcher` gives
   **3025 equal**, **59 same-mnemonic pairs** (of which **30 are branch-target
   only**, i.e. pure address renames caused by size shifts) and **101
   different-mnemonic** instructions. The cost is concentrated in
   `0x1a00` (25), `0x1c00` (28), `0x1d00` (22), `0x1f00` (23), `0x2000` (21);
   `0x400` (9) and `0x700` (14) are the rest.
2. **The bulk of the remaining diff is one allocator tie-break.** In the second
   progress bar the original assigns `{barOffset=r4, barYOffset=r5,
   &Globals::Canvas=fp, &Globals::w=r4(after barOffset dies), top=r6, halfW=r8,
   y=r9}`; ours assigns `{barOffset=r5, barYOffset=fp, &Globals::Canvas=r4,
   &Globals::w=r5(after), top=r6, halfW=r8, y=r9}` — the three registers
   `r4/r5/fp` are rotated. That single rotation also produces the
   `ldrd fp,r9,[sp,#116]` (ours, 4 B) vs `mov r9,fp; ldr.w fp,[sp,#116]`
   (original, 6 B) restore at `0x21b2`, because in the original `fp` already
   holds `&Globals::Canvas` when `r9` is restored.
3. **No source spelling moves the rotation.** Canvas locals (before/after
   `halfW`, before/after the `String`, used for all/some calls), all 120
   permutations of the dock-bar declaration block, type changes on `barOffset`/
   `barYOffset`/`top`/`y`, and expression reorderings all either produce
   byte-identical code to HEAD or collapse the frame to 216 B (9264/9260 B,
   ~83–86 %).
4. **The `int` dock types are real but 4 B short.** `int frameH` + `int total` +
   `int barW` gives **9328 B / 93.663649 %**; adding the `Globals::Canvas &&`
   rate anchor restores 9332 B but only **95.712925 %** — still below HEAD.

---

## 1. Where the 160 cost lives

`regions.py` per-0x100 block (normalised diff cost / hunk count):

| region | cost | hunks | what |
|---|---:|---:|---|
| `0x00400` | 9 | 6 | gamma block (`st` spill shape + `field_0x44a`/`field_0x448` reg swap) |
| `0x00700` | 14 | 8 | `Globals::Canvas &&` anchor + `SetColor` base-load order |
| `0x01900` | 1 | 1 | — |
| `0x01a00` | 25 | 17 | boost `SetColor` base-load hoist (6 B) + branch renames |
| `0x01b00` | 8 | 5 | fire-image ternary load sinking |
| `0x01c00` | 28 | 16 | fire-image else arm + dock-bar landing pad |
| `0x01d00` | 22 | 13 | dock bar (`short`→`int` narrowing, `fp`/`r9` moves) |
| `0x01e00` | 5 | 5 | dock bar `DrawRegion2D` |
| `0x01f00` | 23 | 14 | second bar (`r4/r5/fp` rotation, `String`/`halfW` order) |
| `0x02000` | 21 | 12 | second bar (`GetImage2DWidth` hoist, `r4/r5/fp` rotation) |
| `0x02200` | 1 | 1 | — |
| `0x02300` | 3 | 3 | EH landing-pad filler |

Instruction-level decomposition of the same diff:

```
len(orig)=3149  len(ours)=3154
equal 3025
same-mnemonic pairs 59  (30 of them branch-target-only)
different-mnemonic 101
```

So ~30 of the 160 cost are pure `b.n loc_XXXX` renames that would disappear if
the function size were exactly 9332 B *and* the block boundaries lined up.

---

## 2. The `int` dock types are 4 B short

Confirmed independently this wave (matches `hud-draw-wave119-w6715.md`):

| variant | size | fuzzy |
|---|---:|---:|
| HEAD | **9332** | **95.986038** |
| `int frameH` + `int total` + `int barW` | 9328 | 93.663649 |
| + `Globals::Canvas &&` rate anchor | 9332 | 95.712925 |
| + `Globals::Canvas ? 1.0f : 1.0f` progress ternary | 9332 | 95.712925 |
| + rate anchor + `String`-before-`halfW` | 9332 | 95.300095 |
| + rate anchor + boost-colour local | 9264 | 86.073132 |

With the `int` types the *only* structural difference left in the whole
function is `0x21b2` (`ldrd fp,r9,[sp,#116]`, 4 B, vs `mov r9,fp;
ldr.w fp,[sp,#116]`, 6 B) plus the EH-tail filler split at `0x2430`
(net 0).  The `ldrd` merge is a direct consequence of the second-bar register
rotation: in the original `fp` holds `&Globals::Canvas` at `0x21b2`, so the
epilogue restores `r9` from `fp`; in ours `fp` holds `barYOffset`, so both
registers must come from the stack.

---

## 3. Variants measured this wave (all full build + official `verify.py`)

### 3.1 Gamma block (`Status *st`)

| variant | size | fuzzy |
|---|---:|---:|
| HEAD (`st` used 3×) | **9332** | **95.986038** |
| `st` receiver + `Globals::status` for arg2 | 9312 | 74.988090 |
| `Status **stp = &Globals::status` (3 derefs) | 9312 | 74.988090 |
| `st` receiver + global arg2 + `(void) st` after | 9312 | 74.988090 |
| `Status *const st` | 9332 | 95.986038 |
| `Globals::status` receiver + `st` arg2 | 9332 | 95.986038 |
| two locals `st`, `st2` | 9332 | 95.986038 |

The 9312 variants are the faithful two-load shape but lose the 8-byte `st`
stack slot → 216-byte frame → every `[sp,#N]` shifts by 8.

### 3.2 Second bar (`halfW` / `String` / `barH` / `barYOffset`)

| variant | size | fuzzy |
|---|---:|---:|
| HEAD (`halfW`, `String`, `barH`, `barYOffset`) | **9332** | **95.986038** |
| `String` before `halfW` (Ghidra order) | 9260 | 83.611509 |
| `String` before `halfW` + int dock types | 9256 | 83.627685 |
| `PaintCanvas *canvas` before `halfW`, all calls | 9336 | 60.324015 |
| `canvas` after `String`, all calls | 9332 | 95.986038 |
| `canvas` before `halfW`, `halfW` only | 9332 | 95.986038 |
| `canvas` after `String`, `halfW` only | 9332 | 95.986038 |
| `barYOffset` before `barH` | 9332 | 95.890846 |
| `barYOffset` unsigned | 9332 | 95.986038 |
| `top` as `unsigned short` | 9332 | 95.986038 |
| `y = top * 2 + barOffset` | 9332 | 95.986038 |
| `y` computed before `top` | 9332 | 95.986038 |
| `fadeColor` before the fade block | 9332 | 95.986038 |
| `top` first | 9332 | 95.700460 |
| `barYOffset` after the fade block | 9332 | 95.668729 |
| `barYOffset` after `top` | 9332 | 95.668729 |

Moving `halfW` after the `String` is the *faithful* Ghidra order
(`String::String` at `0x2038`, `GetImage2DWidth` at `0x2044`) but it changes
the whole function's allocation: it deletes the `ldr.w r8,[sp,#108]` at
`0x5e6` and the `ldrd fp,r1,[sp,#116]` at `0x686` in the damage block, i.e.
the gamma `st` spill disappears, and the score collapses.

### 3.3 Dock bar declaration order (120 permutations of `{barW, barH, fade-block, top, barYOffset}`)

| order | size | fuzzy |
|---|---:|---:|
| `WBTFO` (HEAD) | **9332** | **95.986038** |
| `WBFTO` | 9332 | 95.986038 |
| `WBFOT` | 9332 | 95.922577 |
| `WFBTO` | 9332 | 95.700460 |
| `WTFBO` / `WFTBO` / `WFBOT` | 9332 | 95.636998 |
| `OFWBT` / `OFBWT` / `FOWBT` / `FOBWT` | 9332 | 95.014290 |
| `FWBTO` | 9332 | 94.887266 |
| `TFWBO` / `FTWBO` | 9332 | 94.855510 |
| `FWTBO` / `FWOBT` / `FWBOT` | 9332 | 94.823754 |
| `WBTOF` / `WBOTF` / `WBOFT` | 9328 | 94.35 / 94.26 / 94.26 |

(`W`=barW, `B`=barH, `F`=the `field_0x468`/`fadeIn`/`fade` block, `T`=top,
`O`=barYOffset.)  The current order is optimal.

### 3.4 Boost `SetColor` base-load hoist

Original (`0x1adc`):

```
1adc: mov r0,fp
1ade: blx  getBoostRate
1ae2: vmov.f32 s0,#112
1ae6: vmov s2,r0
1aea: ldr.w r0,[r9]          ; Canvas loaded AFTER the call
1aee: vcmpe.f32 s2,s0
1af2: vmrs APSR_nzcv,fpscr
1af6: ite pl
1af8: mvnpl.w r1,#255
1afc: mvnmi.w r1,#208
1b00: blx  SetColor
```

Ours:

```
1adc: mov r0,fp
1ade: mov r5,r4              ; save &Globals::iPad
1ae0: ldr.w r4,[r9]          ; Canvas loaded BEFORE the call
1ae4: blx  getBoostRate
1ae8: vmov.f32 s0,#112
1aec: mvn.w r1,#255
1af0: vmov s2,r0
1af4: mov r0,r4
1af6: mov r4,r5
1af8: vcmpe.f32 s2,s0
1afc: vmrs APSR_nzcv,fpscr
1b00: it mi
1b02: mvnmi.w r1,#208
1b06: blx  SetColor
```

| variant | size | fuzzy |
|---|---:|---:|
| HEAD | **9332** | **95.986038** |
| `>=` reversed ternary | 9332 | 95.954307 |
| `!(>=)` | 9332 | 95.954307 |
| `1.0f >` reversed | 9332 | 95.986038 |
| `(unsigned int)` / `(int)` cast of the ternary | 9332 | 95.986038 |
| `0xffffff2fU : 0xffffff00U` | 9332 | 95.986038 |
| extra parens around the condition | 9332 | 95.986038 |
| `1.0` (double) comparison | 9332 | 95.986038 |
| `PaintCanvas *canvas` local inside the `if` | 9332 | 95.986038 |
| `PaintCanvas *canvas` local before the `if` | 9264 | 86.308767 |
| `unsigned int boostColor` local | 9264 | 86.354249 |
| `int boostColor` local | 9264 | 86.354249 |
| `bool low` local | 9264 | 86.354249 |
| `if`/`else` two `SetColor` calls | 9264 | 86.417792 |

The same "base-before-argument" hoist is what breaks the `0x4b8` damage block
(`0x762`): the original loads `Canvas` at `0x776` *after* the `udiv`, ours
loads it at `0x762` because of the `Globals::Canvas &&` anchor.  The anchor is
load-bearing for the 9332-byte frame (removing it gives 9264 B / 83.39).

### 3.5 Anchor spellings / locations (4b8 anchor)

| variant | size | fuzzy |
|---|---:|---:|
| `Globals::Canvas && field_0x4b8 >= 1` (HEAD) | **9332** | **95.986038** |
| `1 <= field_0x4b8` | 9332 | 95.986038 |
| `(field_0x4b8 >= 1)` | 9332 | 95.986038 |
| `>= 1 && 1` | 9332 | 95.986038 |
| `>= 1 && Globals::Canvas` | 9332 | 95.986038 |
| `>= 1 && Globals::Canvas != 0` | 9332 | 95.986038 |
| `>= 1 || 0` | 9332 | 95.986038 |
| `field_0x4b8 > 0` | 9332 | 95.986038 |
| `>= 1u` | 9332 | 95.922577 |
| `(bool) field_0x4b8` | 9332 | 95.922577 |
| `!!field_0x4b8` | 9332 | 95.922577 |
| `field_0x4b8 != 0` | 9332 | 95.922577 |
| `0 != field_0x4b8` | 9332 | 95.922577 |
| `unsigned char alpha` local in the block | 9332 | 95.986038 |
| `PaintCanvas *canvas` local before the `if` | 9332 | 95.986038 |
| `PaintCanvas *canvas` local inside the `if` | 9332 | 95.986038 |
| anchor moved to `field_0x4ac` block | 9336 | 86.004121 |
| anchor moved to `field_0x4b0` block | 9264 | 83.227446 |
| anchor moved to `field_0x4b4` block | 9264 | 83.227446 |
| anchor moved to `if (show)` (second bar) | 9280 | 85.174603 |
| anchor moved to `hasArmorRegen` | 9324 | 74.912671 |
| anchor moved to `hasShieldBar` | 9252 | 67.037331 |
| anchor moved to `isInRocketControl()` | 9284 | 68.392630 |
| anchor moved to `hasBoostButton` | 9272 | 83.283061 |
| anchor moved to the progress-bar outer `if` | 9280 | 85.061121 |

### 3.6 Fire-image ternary (`0x1b7a`)

Original loads the image in each ternary arm (`ldr.w r1,[sl,#736]` /
`ldr.w r1,[sl,#740]`, 4 B each); ours materialises the address
(`add.w r0,sl,#736` / `#740`, 4 B) and loads once at the join, which is 2 B
longer per site.

| variant | size | fuzzy |
|---|---:|---:|
| HEAD (`unsigned int fireImage = cond ? a : b`) | **9332** | **95.986038** |
| `unsigned int &fireImage` | 9332 | 95.986038 |
| `const unsigned int fireImage` | 9332 | 95.986038 |
| `auto fireImage` | 9332 | 95.986038 |

### 3.7 Misc (all neutral 9332 / 95.986038 unless noted)

`unsigned int barOffset`; `unsigned int dt`; `int savedColor`; `int mining`;
`int armorY`; `unsigned int armorBarY`; `unsigned int dividerOffset`;
`barOffset = (int)((float)frameH * 2.5f + (float)textH)`; `barOffset =
(int)((float)frameH * 2.5f) + textH` → **9340 / 90.418782**; `progress = (float)
transferred / total`; `progress = (float)transferred / (float)(int)total`.

---

## 4. Why the rotation does not move

The second bar's live set at the rotation point (`0x20e4`) is
`{this=sl, ego=fp, &Globals::iPad=r4, &Globals::Canvas=r9, mining=[sp,#80],
Globals::layout=r1}`.  The compiler needs one callee-saved register for
`&Globals::Canvas` (live to the end of the function, used at `0x223e`, `0x22c4`,
`0x22d6`), one for `barYOffset` (live `0x2086`–`0x2110`) and one for
`barOffset` (live `0x1fae`–`0x20e8`).  The original picks `fp`/`r5`/`r4`; ours
picks `r4`/`fp`/`r5`.  Because all three are callee-saved and none is
constrained by an ABI argument, this is a pure greedy-allocator tie-break on
the order the live intervals are created.  Every source lever that changes
that order (declaration order, expression order, local vs global, type) either
leaves the tie-break unchanged (identical bytes) or changes the *frame size*,
which is far more destructive than the tie-break itself.

The two places where the same phenomenon is visible in isolation — the boost
`SetColor` at `0x1adc` and the `field_0x4b8` `SetColor` at `0x762` — both show
clang evaluating the `Globals::Canvas` base *before* the call argument in our
build and *after* it in the original.  No C++ spelling tried (local colour,
local canvas, `if`/`else`, casts, reversed comparisons, parenthesisation)
changes that ordering without also changing the frame.

---

## 5. Reproduction

```
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

Scratch harness in `/dev/shm/w6745`: `score.py` (size/fuzzy), `sweep.py`
(applies `VARIANTS` to a pristine copy, builds `link_so`, scores, restores),
`one.py` (build one named variant and leave it in place), `lib.py` (`ed()`
exactly-once replacement helper + `ints()` dock-type helper), `dump.py`
(address-tagged objdump of both sides), `delta2.py` (normalised diff with byte
sizes and cumulative delta), `regions.py` (per-0x100 cost table), `hill2.py`
(multi-mutation hill climb with plateau moves).  Variant files used this wave:
`v_g1.py`, `v_g2.py`, `v_b1.py`, `v_b2.py`, `v_sb.py`, `v_mix.py`, `v_anc.py`,
`v_anc2.py`, `v_anc3.py`, `v_dockord.py` (120 permutations), `v_combo4.py`,
`v_i3.py`, `v_baroff.py`, `v_intonly.py`.
