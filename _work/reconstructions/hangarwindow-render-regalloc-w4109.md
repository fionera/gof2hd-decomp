# `HangarWindow::render` — pass 5: lazy-prologue proof + EH-scope reconstruction (w4109)

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave91-w4109`, base master `a4557300`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`. This pass is
source-shape / EH-table analysis; **no code change landed** (no shape beats 80.6 %,
and `byte_exact` only rises if `render` becomes byte-identical).

## TL;DR

Baseline is **80.6 % / 9700 B** (raw difflib `0.8064427312775331`, 3626 vs 3638
normalized insns), gate byte 4498 / linked 4498 / size 4524 / data 843 /
imports 207, lint CLEAN, drift 0.

New results this pass:

1. **The original source is the fully-lazy shape.** The lazy build's prologue is
   *byte-identical* to the original's for the whole first basic block except for
   one register: `mov sl,r0` (ours) vs `mov fp,r0` (original), and the frame is
   384 vs 376 (`sub sp,#384` vs `#376`), shifting every stack slot by 8. Every
   other instruction (guard/Canvas GOT loads, `SetColor`, `&Globals::appManager`
   load into `r8`, `GetApplicationModule`, `this->viewMode` check, `&Globals::layout`
   load at `drawBG`) is the same. So the entire 80.6 → 61.6 % gap is the
   `this`/`&Globals::layout` register tie (plus the 8-byte frame it drags with it).
2. **The selected-item amount block is the ternary form.** Converting
   `int right, left; if (tab==1) {...} else {...}` to two `tab == 1 ? ... : ...`
   ternaries reproduces **all six** isolated 4-byte `getStationAmount()`/`getAmount()`
   EH rows (the original's `+4` rows), not just "some of them". The addresses are
   shifted because the ternary body is 24 B shorter, but the row *pattern* and the
   per-call landing pads are right.
3. The credit-dialog `{ volatile int pad0; volatile int pad0b; setSplitText(price); }`
   anchor currently **adds two EH rows the original does not have**: the original
   has one contiguous `14ab80 +210` region, ours splits it into
   `14ab6c +28` / `14ab9c +6` / `14abac +174`.
4. A **non-faithful** shape that caches the `Globals::appManager` *value* in a
   top-level local (`ApplicationManager *am = Globals::appManager;`) is the only
   shape found that reproduces the original's three register roles
   (`this=fp`, `r8=&appManager`, `&layout=sl` at `drawBG`) with frame 376 — but it
   scores 73.5 %, because the original keeps the *address* of the global in `r8`
   (`ldr r8,[r0]; ldr r0,[r8]`), not the pointer value.

## 0. Baseline (canonical, this tree)

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
render: 9700/9700 B, pct 80.6, raw 0.8064427312775331, insns 3626/3638
LSDA:   155 rows, .ARM.extab record 939 B at 0x1c30ec
```

Original: 156 rows, record **948 B** at `0x1c30fc`.

### 0.1 Environment (needed to reproduce the canonical gate)

`cmake --preset default` alone points `GOF2_ORIGINAL_SO` at a nonexistent
`wt/_work/bins/...`. Configure explicitly:

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
```

and copy the **darwin** r18b `libc++abi.a` + `libandroid_support.a` into
`.cache/ndk/android-ndk-r18b/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/`
(gitignored). The Linux r18b archives carry `/buildbot/...` instead of
`/Volumes/Android/buildbot/...` and drop `.rodata` to `0x41b57`, byte 4344.

`FN=<mangled> ... verify-fn` is broken in this tree (empty `--show`); call
`tools/verify/verify.py` directly.

## 1. The original prologue is the lazy shape, byte for byte

Original (`orig.txt`):

```
1490d8: push {r4,r5,r6,r7,lr}; add r7,sp,#12; stmdb sp!,{r7,r8,r9,sl,fp}
1490e0: vpush {d8-d10}
1490e4: sub sp, #376
1490e6: mov  fp, r0                       ; this -> fp
1490e8: ldr  r0,[pc]  ; guard
1490ea: ldr  r1,[pc]  ; Canvas
1490ec: add  r0,pc
1490ee: add  r1,pc
1490f0: ldr  r0,[r0]                      ; guard addr
1490f2: ldr  r2,[r1]                      ; Canvas ptr
1490f4: str  r2,[sp,#140]
1490f6: str  r0,[sp,#112]
1490f8: ldr  r1,[r0]                      ; guard value
1490fa: ldr  r0,[r2]                      ; Canvas vtable
1490fc: str  r1,[sp,#372]
1490fe: mov.w r1,#-1
149102: blx  SetColor
149106: ldr  r0,[pc]  ; &Globals::appManager
149108: movs r1,#5
14910a: add  r0,pc
14910c: ldr.w r8,[r0]                     ; r8 = &appManager (GOT entry)
149110: ldr.w r0,[r8]                     ; r0 = appManager value
149114: blx  GetApplicationModule
149118: cmp  r0,#0
14911a: str.w fp,[sp,#136]                ; save this
14911e: itt ne; ldrbne r0,[r0,#24]; cmpne r0,#0
149124: bne.n 1491cc
149126: mov  r6,fp
149128: ldr.w r0,[r6,#88]!                ; this->viewMode
14912c: cmp  r0,#0
14912e: bne.w 14a83c
149132: ldr  r0,[pc]  ; &Globals::layout
149134: add  r0,pc
149136: ldr.w sl,[r0]                     ; &layout -> sl  (lazy, at drawBG)
14913a: ldr.w r0,[sl]
14913e: blx  drawBG
```

The **lazy** build (`Layout *layout` removed; `Globals::layout->drawBG()`) is
identical except:

```
1490e4: sub sp, #384          (was #376)
1490e6: mov  sl, r0           (was mov fp, r0)
1490f4: str  r2,[sp,#148]     (was #140)
1490f6: str  r0,[sp,#120]     (was #112)
1490fc: str  r1,[sp,#380]     (was #372)
14911a: str.w sl,[sp,#144]    (was str.w fp,[sp,#136])
149126: mov  r6,sl            (was mov r6,fp)
149136: ldr.w fp,[r0]         (was ldr.w sl,[r0])
14913a: ldr.w r0,[fp]         (was ldr.w r0,[sl])
```

That is the *whole* difference in the prologue. The original does **not** load
`&Globals::layout` in the prologue, and it does **not** cache the layout value:
it reloads `[sl]` at each `Globals::layout->` use (38 sites). The current
baseline does both (prologue GOT load + value cached in `r4`, 8 `[sl]` sites).

So the original source is the fully-lazy shape; the only thing clang 7 gets
"wrong" for us is the callee-saved assignment of `this` (fp vs sl) and the
consequent 8-byte frame growth.

## 2. Why the tie flips: greedy-allocator evidence

`-mllvm -print-machineinstrs` after Greedy RA, `bb.0`:

baseline (`pmi.log`):

```
renamable $r11 = COPY $r0                 ; this -> r11 (fp)
renamable $r1  = tLDRLIT_ga_pcrel @_ZN7Globals6layoutE
renamable $r2  = tLDRLIT_ga_pcrel @_ZN7Globals6CanvasE
renamable $r0  = t2LDRi12 $r0, 0
renamable $r10 = t2LDRi12 $r1, 0          ; &layout -> r10 (sl)
renamable $r2  = t2LDRi12 $r2, 0
renamable $r4  = t2LDRi12 $r10, 0         ; layout value -> r4
```

lazy (`pmi_lazy.log`):

```
renamable $r10 = COPY $r0                 ; this -> r10 (sl)
renamable $r1  = tLDRLIT_ga_pcrel @_ZN7Globals6CanvasE
...
```

The only IR difference in `bb.0` is the extra `@_ZN7Globals6layoutE` GOT load.
Its live interval starts before `this`'s first *use*, and the greedy allocator
gives it `r10`, pushing `this` to `r11`. In the lazy IR `this` is the only
early long-lived value, so it takes `r10`. Every source-level attempt to
re-introduce a prologue value in the lazy shape either (a) is not faithful
(caches the `appManager` *value*: `am`/`am_after`, see §4) or (b) does not add
a register (a reference/pointer-to-global folds back to the GOT address:
`ApplicationManager *&am`, `ApplicationManager **am`, all 61.6 %).

## 3. Selected-item amount block: the ternary reproduces all six `+4` rows

Original selected branch (`0x149648`..`0x149668`):

```
149648: ldr  r4,[sp,#120]      ; item
14964a: ldr  r1,[sp,#108]      ; tab
14964c: ldr  r6,[sp,#136]      ; this
14964e: ldr  r0,[r4,#16]       ; item->item
149650: cmp  r1,#1
149652: bne.w 149c88
149656: blx  getStationAmount
14965a: ldr  r5,[sp,#132]      ; &layout
14965c: mov  r9,r0             ; right
14965e: ldr  r0,[r4,#16]
149660: blx  getAmount
149664: ldr  r8,[sp,#140]      ; Canvas
149668: b.n  149ca8
```

The three call sites each get their own 4-byte EH row with a *distinct*
landing pad, even though every pad is just `b.n 14b698` (`_Unwind_Resume`):

```
149656 +4 lp=14b5a2   getStationAmount
149660 +4 lp=14b64c   getAmount
149c88 +4 lp=14b5a2   getAmount            (else path)
149e26 +4 lp=14b600   getStationAmount     (non-selected branch)
149e30 +4 lp=14b658   getAmount            (non-selected branch)
149f14 +4 lp=14b600   getAmount            (non-selected branch)
```

`14b5a2/14b64c/14b600/14b658` are all `b.n 14b698`. This is an EH *scope*
difference, not a cleanup-body difference.

The current source writes the selected branch as `if/else`, which merges the
two `tab == 1` calls into one `149656 +18 lp=14b64a` row and the else path into
`149c88 +30 lp=14b64a`. Rewriting it as the two ternaries

```c
int right = tab == 1 ? item->item->getStationAmount()
                     : item->item->getAmount();
int left  = tab == 1 ? item->item->getAmount()
                     : this->bluePrint->getCurrentAmount(item->item->getIndex());
```

produces all six isolated `+4` rows (shifted by the 24 B body shrink):

```
149642 +4 lp=14b58a   149c70 +4 lp=14b58a
14964a +4 lp=14b5f8   149c7e +16 lp=14b5f8
149e16 +4 lp=14b5f6   149f08 +4 lp=14b5f6
149e22 +4 lp=14b640
```

The non-selected branch in the current source is *already* ternary, and its
three `+4` rows already match the original pattern (at shifted addresses).

### 3.1 Residual LSDA deltas after the ternary

Ternary variant: **157 rows, record 951 B** (original 156 / 948). The extra row
is the `if (left > 0)` block: original folds it into `149c98 +154`; ours emits
`149c7e +16` (getIndex + getCurrentAmount) followed by `149cb0 +32`. The
blueprint branch is also still merged on our side (`1496ba +230` vs original
`1496d2 +144` + `149788 +46`).

## 4. The credit-dialog anchors add two EH rows the original does not have

Original (one contiguous region):

```
14ab80 +210 lp=14b68e
```

Ours (split by the `pad0`/`pad0b` scoped block):

```
14ab6c +28  lp=14b68e     ; replaceTextKeepSize
14ab9c +6   lp=14b678     ; setSplitText   <- extra scope
14abac +174 lp=14b68e
```

So the `{ volatile int pad0; volatile int pad0b; setSplitText(price); }` block
is not faithful to the original's EH shape. It is nevertheless load-bearing for
the *body*: removing it drops the score (see §5).

## 5. Variant sweep (official verifier, `autojunk=False`)

Baseline `0.8064427312775331 / 9700`. All shapes measured this pass:

| variant | raw | size | `this` | `&layout` | frame |
|---|---:|---:|---|---|---:|
| **baseline (top local, if/else)** | **0.8064427** | **9700** | **fp** | **sl (prologue)** | 376 |
| top local `const` / `volatile` / `(void)` / declare-then-assign | 0.8064427 | 9700 | fp | sl (prologue) | 376 |
| lazy (no top local) | 0.6161242 | 9724 | sl | fp @drawBG | 384 |
| lazy + `hintLayout` removed | 0.6258073 | 9712 | sl | fp @drawBG | 384 |
| lazy + no hint + **ternary** | 0.6351389 | **9700** | sl | fp @drawBG | 384 |
| lazy + no hint + width-first free credits | 0.5286913 | 9664 | fp | sl | 384 |
| lazy + no hint + ternary + width-first | 0.5305449 | 9668 | fp | sl | 384 |
| top local used only at `drawBG` (pin-at-drawBG) | 0.7706069 | 9716 | fp | sl (prologue) | 376 |
| pin-at-drawBG + ternary | 0.7805954 | 9676 | fp | sl (prologue) | 376 |
| **ternary (if/else → ?:) only** | 0.7805954 | 9676 | fp | sl (prologue) | 376 |
| `ApplicationManager *am = Globals::appManager` + lazy | 0.7329569 | 9688 | fp | sl @drawBG | 376 |
| same, `am` declared after `SetColor` | 0.7348850 | 9688 | fp | sl @drawBG | 376 |
| `am` + ternary | 0.7328560 | 9692 | fp | sl @drawBG | 376 |
| `am` + width-first | 0.6648352 | 9704 | fp | sl @drawBG | 376 |
| `ApplicationManager *&am` / `**am` + lazy | 0.6161242 | 9724 | sl | fp @drawBG | 384 |
| lazy + hoisted `viewMode` local | 0.6524373 | 9660 | sl | fp @drawBG | 384 |
| `ImageFactory *` local + lazy | 0.5544691 | 9696 | — | — | — |
| `PaintCanvas *canvas` local + lazy | 0.5111297 | 9516 | — | — | — |

Anchors removed individually from the baseline (all regressions):

| anchor removed | raw | size |
|---|---:|---:|
| `pad0`+`pad0b` block | 0.7942716 | 9692 |
| `pad0` only | 0.7925100 | 9696 |
| `pad0b` only | 0.7925100 | 9696 |
| `description = description;` ×3 | 0.7804878 | 9680 |
| both | 0.7800910 | 9668 |
| `pad3` | 0.7914601 | 9692 |

## 6. Proven vs hypothesis

Proven (measured in this tree):

* Baseline is `0.8064427312775331 / 9700`; gate byte 4498 / linked 4498 /
  size 4524 / data 843 / imports 207; lint CLEAN; drift 0.
* The original's prologue is the **lazy** shape: its first basic block is
  byte-identical to our lazy build except `mov fp,r0` vs `mov sl,r0` and the
  8-byte frame growth (`#376` vs `#384`) that follows from it. The original
  reloads `[sl]` 38×; the baseline caches the value in `r4` (8×).
* The selected-item amount block is the **ternary** form: it reproduces all six
  isolated 4-byte `getStationAmount()`/`getAmount()` EH rows with per-call
  landing pads, matching the original's row pattern.
* The `pad0`/`pad0b` credit-dialog anchor adds two EH rows the original does not
  have (one contiguous `14ab80 +210` region in the original).
* The only source shape found that gives the original's three roles
  (`this=fp`, `r8=&appManager`, `&layout=sl` @drawBG) with frame 376 is a
  top-level `ApplicationManager *am = Globals::appManager;` (value cache), which
  is **not** faithful: the original keeps `&appManager` in `r8` and reloads the
  pointer (`ldr r8,[r0]; ldr r0,[r8]`).
* Every anchor in the current source is load-bearing for the 80.6 % score.

Hypothesis / open:

* The original is the fully-lazy source with the ternary amount block. To land
  it we still need the greedy allocator to pick `fp` for `this` in the lazy IR
  (and therefore the 376-byte frame). No plain-C++ shape tried does this; the
  tie is decided by the `bb.0` live-interval ordering, not by anything we can
  express without adding a non-faithful prologue value.
* Once the tie is flipped, the remaining body deltas to chase are (a) the
  `if (left > 0)` EH-scope fold (`+16`+`+32` → `+154`), (b) the blueprint-branch
  `+144`/`+46` split, and (c) the credit-dialog contiguous region.

## 7. Commands

```sh
cmake --build cmake-build-match --target check -j8   # RATCHET PASS byte 4498 linked 4498
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
python3 _work/reconstructions/gameresource_gen/lsda.py \
  cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
```

Scratch (this pass): `/dev/shm/w4109/score.py`, `try.sh`, `trydump.sh`,
`roles.sh`, `mk_*.py`, `orig.txt`, `ours.txt`, `lsda_orig.txt`, `lsda_ours.txt`,
`pmi.log`, `pmi_lazy.log`.
