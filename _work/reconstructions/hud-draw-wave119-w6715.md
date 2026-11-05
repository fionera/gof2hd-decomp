# Hud::draw — wave 119: dock-bar locals are `int`, but `int` costs the 9332-byte frame

**Status:** research note, 2026-11-05 (wave 119). Worktree `wt/w6715`, branch
`wave119-w6715`, base `master 47a1396bd`. No source change landed: every variant
measured at or below the 95.986038 % baseline. Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (file `0x163b90`).

Gate on entry and on exit (full `check`): **RATCHET PASS**, `byte_exact 4503`,
`linked_exact 4503`, size-exact 4524/4524, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**,
drift **0**. `.text 0x1337c8` (exact), `.ARM.extab 0x26290` (8 B short, `createShip`
only — unchanged), `.rodata 0x41c27`, 27 sections, file size 2,187,328 B.

---

## 0. TL;DR

1. **New finding.** In the docking-transfer bar the original's `frameH`, `total`
   and `barW` are all **signed `int`**, not `short`/`unsigned short`. The original
   has no `sxth`/`uxth` anywhere in the block and uses `vcvt.f32.s32` (signed) plus
   `asrs`/`add.w rX,rX,lsr#31` (signed ÷2) for `barW`. Our current source declares
   `short frameH`, `short total`, `unsigned short barW`, which emits two `sxth` and
   one `uxth` + `vcvt.f32.u32` + `ubfx` that the original does not have.
2. **`int` makes the block byte-identical but shrinks the function to 9328 B.**
   With `int frameH` + `int total` the whole dock block `0x1cfe–0x1d92` becomes
   instruction-for-instruction identical to the original (only the `frameH` stack
   slot differs: `[sp,#92]` vs the original's `[sp,#96]`). The function is then
   **4 bytes short**, which rotates every `b.n` target in the ~60-instruction EH
   landing pad and drops the fuzzy score to 93.82 %.
3. **The fuzzy metric is size-dominated.** Because the verifier compares the
   normalised landing-pad branch targets literally (`b.n 0x2444` vs `b.n 0x2440`),
   a 4-byte size error costs ~1.9 points on its own. Any future fix for the dock
   types must also restore exactly 4 bytes somewhere else.
4. All ~35 variants tried this wave are listed in §3. The best measured shape is
   still HEAD (95.986038 %, 9332 B).

---

## 1. Ground truth: the dock block is all-`int`

Original, file offset = VMA (function base `0x163b90`):

```
16488a: blx  PaintCanvas::GetImage2DHeight(image_0x37c)
16488e: str  r0, [sp, #96]          ; frameH  -> 32-bit store, no narrowing
164890: ldr  r0, [pc, #952]         ; &Globals::font
164894: str  r1, [sp, #92]
164896: blx  PaintCanvas::GetTextHeight(Globals::font)
16489a: mov  r5, fp
16489c: mov  fp, r9                 ; fp = &Globals::Canvas
16489e: mov  r9, r0                 ; r9 = textH
1648a0: mov  r0, r5
1648a2: blx  PlayerEgo::getDockTransferedAmount
1648a6: mov  r4, r0                 ; transferred
1648a8: mov  r0, r5
1648aa: blx  PlayerEgo::getDockTotalAmount
1648ae: mov  r5, r0                 ; total
...
1648dc: vmov s0, r4                 ; (float) transferred
1648e0: cmp.w r8, #0
1648e4: vmov s2, r5                 ; (float) total      <- no sxth
1648e8: vcvt.f32.s32 s0, s0
1648ec: vcvt.f32.s32 s2, s2
1648f0: ldr  r0, [sp, #96]          ; frameH (full 32-bit)
1648f2: vmov s4, r9                 ; textH  (full 32-bit)
...
1648f8: vmov s2, r0                 ; (float) frameH     <- no sxth
1648fc: vcvt.f32.s32 s2, s2
```

Second bar, `barW`:

```
1649e6: blx  PaintCanvas::GetImage2DWidth(image_0x37c)
1649ea: mov  r8, r0                 ; barW
...
164a7a: vmov s0, r8                 ; (float) barW
164a7e: add.w r2, r4, r6, lsl #1    ; barYOffset + top*2
164a82: vcvt.f32.s32 s0, s0         ; SIGNED conversion
...
164a9c: add.w r2, r8, r8, lsr #31   ; signed /2 of barW
164aa0: asrs r3, r3, #1
164aa2: sub.w r2, r3, r2, asr #1
```

Our HEAD emits, for the same sites:

```
  ...  sxth.w r0, r8                ; total   (short)
  ...  sxth    r0, r0               ; frameH  (short)
  ...  uxth.w  r0, fp               ; barW    (unsigned short)
  ...  vcvt.f32.u32 s0, s0          ; unsigned conversion
  ...  ubfx    r3, fp, #1, #15      ; unsigned /2
```

So the faithful declaration is:

```c++
            int frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
            int textH  = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font);
            int transferred = ego->getDockTransferedAmount();
            int total  = ego->getDockTotalAmount();
            ...
            int barW   = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
            int barH   = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
```

Note that `hud-draw-short-frameh-w5318.md` already observed that `int frameH` is
the semantically faithful type and that `short` was kept only as a *codegen lever*
(it keeps the Hud LSDA at 77 rows / 476 B). At the current tree the LSDA is no
longer the constraint: `int frameH` + `int total` + `int barW` leaves
`.ARM.extab` at **0x26290** and `.rodata` at **0x41c27**, both unchanged.

## 2. What `int` fixes

With `int frameH` + `int total` the dock block becomes byte-identical to the
original except for one stack slot:

```
ours (int)                                original
1648a0: mov   r5, fp                      1648a0: mov   r5, fp
1648a2: mov   fp, r9                      1648a2: mov   fp, r9
1648a4: mov   r9, r0                      1648a4: mov   r9, r0
1648a6: mov   r0, r5                      1648a6: mov   r0, r5
1648a8: blx   getDockTransferedAmount     1648a8: blx   getDockTransferedAmount
1648ac: mov   r4, r0                      1648ac: mov   r4, r0
1648ae: mov   r0, r5                      1648ae: mov   r0, r5
1648b0: blx   getDockTotalAmount          1648b0: blx   getDockTotalAmount
1648b4: mov   r5, r0                      1648b4: mov   r5, r0
...
1648dc: vmov  s0, r4                      1648dc: vmov  s0, r4
1648e0: cmp.w r8, #0                      1648e0: cmp.w r8, #0
1648e4: vmov  s2, r5                      1648e4: vmov  s2, r5
1648e8: vcvt.f32.s32 s0, s0               1648e8: vcvt.f32.s32 s0, s0
1648ec: vcvt.f32.s32 s2, s2               1648ec: vcvt.f32.s32 s2, s2
1648f0: ldr   r0, [sp, #92]   <-- diff   1648f0: ldr   r0, [sp, #96]
1648f2: vmov  s4, r9                      1648f2: vmov  s4, r9
```

i.e. the `sxth`s disappear and the register roles (`fp` = `&Globals::Canvas`,
`r9` = `textH`) fall into place exactly as in the original.

`int barW` likewise removes the `uxth`/`vcvt.f32.u32`/`ubfx` triple in the second
bar (`0x1e3a`/`0x1e5c` in HEAD's normalised diff).

## 3. Measured variant table (full build + official `verify.py`)

All numbers are the official fuzzy ratio on `_ZN3Hud4drawExxP9PlayerEgobjj`.

| variant | size | fuzzy |
|---|---:|---:|
| **HEAD (baseline)** | **9332** | **95.986038** |
| `int frameH` (A) | 9328 | 94.302492 |
| `int total` (B) | 9328 | 93.970168 |
| `int barW` (C) | 9328 | 94.097112 |
| A + B | 9328 | 93.822455 |
| A + C | 9328 | 93.748017 |
| B + C | 9324 | 93.572449 |
| A + B + C | 9328 | 93.663649 |
| A + B + `int top` (first bar) | 9328 | 93.822455 |
| A + B + C + `int top` | 9324 | 93.710292 |
| C + `short y` (second bar) | 9332 | 94.128848 |
| A + B + C + `short y` | 9332 | 93.695411 |
| A + C + `short y` | 9332 | 93.779752 |
| B + C + `short y` | 9328 | 93.667672 |
| A + B + C, anchor `Globals::iPad` | 9332 | 90.236545 |
| A + B + C, anchor `Globals::mouseCursorActivated` | 9332 | 86.902683 |
| A + B + C, anchor `Globals::sound` | 9340 | 88.130752 |
| A + B + C, anchor `Globals::gameText` | 9276 | 85.682434 |
| A + B + C, anchor `Globals::font` | 9228 | 84.222293 |
| A + B + C, anchor `Globals::globals` | 9280 | 82.714106 |
| A + B + C, anchor `Globals::hints` | 9260 | 83.434022 |
| A + B + C, no anchor | 9260 | 83.434022 |
| A + B, no anchor | 9260 | 83.593005 |
| A + B + C, `barW` moved before `String text` | 9328 | 94.097112 |
| A + B + C, `barW` moved before `barOffset` | 9344 | 91.373295 |
| A + B + C, `barW` moved after `total` | 9340 | 91.373295 |
| A + B + C, `unsigned int halfW` | 9328 | 93.631888 |
| A + B + C, `unsigned int barH` | 9328 | 93.631888 |
| A + B + C, `unsigned int y` | 9328 | 93.631888 |
| A + B + C, `unsigned int fadeColor` | 9328 | 93.663649 |
| A + B + C, `unsigned short top` (2nd bar) | 9328 | 93.663649 |
| plain gamma (3 × `Globals::status`) | 9312 | 74.988090 |
| `st` + arg2 `Globals::status` | 9312 | 74.988090 |
| gamma condition in a `float` local | 9312 | 74.988090 |
| gamma condition in a `bool` local | 9312 | 74.988090 |
| gamma `0.0f < call(...)` | 9312 | 74.988090 |
| A + B + plain gamma | 9308 | 75.313941 |
| A + plain gamma | 9308 | 74.694202 |
| B + plain gamma | 9308 | 74.809403 |

Section facts for the `int` variants (checked with `readelf -S -W`): `.text`
shrinks by 4 B, `.ARM.extab` stays **0x26290**, `.rodata` stays **0x41c27**, so
the `int` types are gate-safe on their own — they are simply 4 bytes short.

## 4. Why 9328 loses ~2 points

The `.ARM.extab`/LSDA landing pad at the end of `Hud::draw` is ~60 instructions,
almost all `b.n <addr>`. The normaliser keeps the literal target, so a 4-byte
shift in the function body renames every target and every one of those
instructions counts as a mismatch. Concretely, `int frameH` + `int total` +
`int barW` costs:

| region | HEAD cost | int-types cost |
|---|---:|---:|
| `0x0` (prologue/gamma) | 21 | 18 |
| `0x165800` (dock bar) | 15 | 10 |
| `0x165900` (dock bar) | 27 | 25 |
| `0x165a00` (second bar) | 7 | 35 |
| `0x165b00` | 19 | 21 |
| `0x165c00` | 18 | 20 |
| `0x165d00` | 2 | 8 |
| `0x165e00` | 1 | 19 |
| `0x165f00` (landing pad) | 2 | 37 |

The dock bar genuinely improves (15→10, 27→25) but the second bar and the
landing pad regress because the whole frame rotates.

## 5. Remaining work / hypotheses for the next wave

1. **Find the missing 4 bytes.** With the `int` types the function is 9328 B and
   the original 9332 B. The one instruction the original has that we do not is
   the split restore at `0x21b2`:
   ```
   original:  mov r9, fp ; ldr.w fp, [sp, #116]      (2 + 4 bytes)
   ours:      ldrd fp, r9, [sp, #116]                (4 bytes)
   ```
   plus a further 2-byte difference later in the landing pad. If a source shape
   is found that keeps the `ldrd` from merging, the `int` types should become a
   large win (the dock bar becomes exact and the landing-pad addresses realign).
2. **Gamma reload + `this` split.** Unchanged from
   `hud-draw-gamma-reload-w101-wf114.md`: the original has two `Globals::status`
   loads in the gamma block *and* the `this` live-range split at `bb.28`. Every
   two-load spelling tried (this wave: plain 3-use, `st` + arg2 global, condition
   in a `float`/`bool` local, `0.0f <` reversed) drops the frame to 216 B and the
   score to ~75.
3. The dead `ldr.w r8,[sp,#108]` reloads at `0x164176`/`0x164210` still suggest a
   missing `Globals::status` read in the damage block.
4. Boost block (`0x1adc`): the original calls `getBoostRate()` *before* loading
   `Globals::Canvas` and emits `ite pl; mvnpl #255; mvnmi #208`; ours caches
   Canvas in `r4` across the call (`mov r5,r4; ldr r4,[r9]` … `mov r0,r4;
   mov r4,r5`) and emits `mvn #255; it mi; mvnmi #208`.
5. Fire-button ternary (`0x1b88`): the original loads the selected image directly
   in each arm (`ldr.w r1,[sl,#736]` / `ldr.w r1,[sl,#740]`); ours materialises
   the address (`add.w r0,sl,#736` / `#740`) and loads once at the merge point.

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS, byte 4503

python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj

arm-linux-androideabi-readelf -S -W cmake-build-match/libgof2hdaa.so \
  | grep -E 'ARM.extab|\.rodata|\.text '
```

Scratch harness in `/dev/shm/w6715`: `score.py` (fuzzy/size/insn count),
`dump.py` (`o.asm`/`u.asm` raw objdump), `diff5.py` (address-tagged normalised
diff, both sides printed), `regions2.py` (per-0x100 cost table), `sweep.py`
(applies `VARIANTS` to `git show HEAD:src/game/ui/Hud.cpp`, builds `link_so`,
scores). Variant files used this wave: `v_plain.py`, `v_ab.py`, `v_dtypes.py`,
`v_abc.py`, `v_final.py`, `v_anchor.py`, `v_pad.py`, `v_pad2.py`, `v_anc3.py`,
`v_gam2.py`, `v_barwpos.py`.
