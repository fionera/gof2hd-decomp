# MenuTouchWindow::OnTouchBegin + MGame::OnUpdate — wave 109 (w5720)

Status: research pass, 2026-11-04. Worktree `wt/w5720`, branch `wave109-w5720`,
base `7c34c4098`. **No source change landed**: both functions are already at
their best measured shape. This note extends
`menutouchwindow-wave106-w5417.md` (OnTouchBegin) and
`mgame-onupdate-wave108-w5620.md` (MGame::OnUpdate) with a new batch of
measured source shapes, a full `Status*` usage-site matrix for MGame, and the
7.0.2 machine-IR evidence for the OnTouchBegin register split.

Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`; real VMA = Ghidra − 0x10000).

| symbol | size orig/ours | diff bytes | insn diffs / runs | fuzzy |
|---|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | 29 | 18 / 11 | 98.100000 |
| `_ZN5MGame8OnUpdateEv` | 11440 / 11440 | 41 | (address-shift polluted) | 99.900000 |

Gate before and after (pristine source restored): `RATCHET PASS`,
byte_exact 4500 / linked_exact 4500 / size 4524 / data 843 / imports 207 /
lint `CLEAN ✓` / drift 0.

## 1. OnTouchBegin — the residual is one register/value decision

The 18 structural lines are two regions:

* **case 3** (2 lines): `ldr.w ip,[sl,#340]` vs `ldr.w r4,[sl,#340]` plus
  `add r3,ip` vs `add r3,r4`. `r4` is free in both binaries at that point
  (verified by scanning the whole case-3 block); this is a pure greedy
  tie-break for the short-lived `this->listEntryWidth` load.
* **case 15** (16 lines): the scrollbar block. Original vs ours (baseline):

```
original (0x12a1bc..)                 ours baseline
ldr.w r0,[sl,#312]  heapBufB          ldr.w r0,[sl,#312]
ldr.w r1,[sl,#480]  field_0x1e0       ldr.w r1,[sl,#480]
ldr r2,[sp,#16]     &Globals::layout  ldr r2,[sp,#16]
ldr.w r1,[r0,r1,lsl#2]                ldr r4,[r6,#32]     ; top from cached layout
ldr r2,[r2,#0]      Globals::layout   ldr.w r1,[r0,r1,lsl#2]
ldr.w r0,[fp]       Canvas            ldr r2,[r2,#0]
ldr r5,[r2,#12]     leftMargin        ldr.w r0,[fp]
ldr r6,[r2,#32]     top               ldr r5,[r2,#12]
blx GetImage2DHeight                  blx GetImage2DHeight
adds r1,r6,r5       top+leftMargin    adds r1,r5,r4       leftMargin+top
add r0,r1                             add r0,r1
```

The original **reloads `&Globals::layout` from `[sp,#16]` after the
GetImage2DWidth call and uses the fresh `Globals::layout` value for both
`field_0xc_leftMargin` and `field_0x20_top`**. Our baseline caches the value
in `r6` (callee-saved) across the call and uses it for `top`; the fresh load
is used only for `leftMargin`. Both are 1896 B / 754 insns.

### 1.1 7.0.2 machine-IR evidence

`-mllvm -print-after-all` on the all-direct shape (the IR the original must
have) shows the pre-RA block `bb.42`:

```
%297 = tLDRLIT_ga_pcrel @Globals::w
%299 = tLDRLIT_ga_pcrel @Globals::Canvas
%300 = t2LDRi12 %297, 0          ; &Globals::w
%301 = t2LDRi12 %508, 0          ; Globals::layout  (508 = &Globals::layout, stack.0)
%302 = t2LDRi12 %93, 312         ; heapBufB
%303 = t2LDRi12 %93, 480         ; field_0x1e0
%304 = t2LDRi12 %299, 0          ; &Globals::Canvas
...
tBL GetImage2DWidth
```

and `bb.44`/`bb.47` reload `Globals::layout` through the same `%508`.
Post-RA (`Virtual Register Rewriter`) the compact piece gets:

```
%508 (&Globals::layout) -> $r11 (fp), live across the GetImage2DWidth call
%304 (&Globals::Canvas) -> $r5
```

The original has `fp = &Globals::Canvas` and rematerialises `%508` from
`[sp,#16]` at each use. So the source shape (all-direct) is right; the
difference is purely `RAGreedy`'s choice to keep the stack-slot address in
`fp` instead of rematerialising it. The baseline's cached `Layout` value is
what makes the same compiler rematerialise the address (it occupies a
callee-saved register), at the cost of the `r6` cache.

### 1.2 New measured shapes (all 1896 B unless noted)

`H = Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])`;
`Td/Tc = Globals::layout->field_0x20_top` / `layout.field_0x20_top`;
`Ld/Lc = Globals::layout->field_0xc_leftMargin` / `layout.field_0xc_leftMargin`;
`ref = Layout &layout = *Globals::layout;`.

| variant | size | diff bytes | insn diffs |
|---|---:|---:|---:|
| **baseline `Tc + (Ld + H)`** | **1896** | **29** | **18** |
| `Td + (Lc + H)` | 1896 | 28 | 18 |
| `Lc + (Td + H)` | 1896 | 28 | 18 |
| `Ld + (Tc + H)` | 1896 | 29 | 18 |
| `(Tc + Ld) + H` / `(Ld + Tc) + H` | 1896 | 45 | 23 |
| `H + (Tc + Ld)` / `H + (Ld + Tc)` | 1896 | 116 | 54 |
| all-direct `Td + (Ld + H)` (no ref) | 1892 | 358 | 156 |
| `Layout *const &layout = Globals::layout` | 1892 | 358 | 156 |
| `Layout *&layout = Globals::layout` | 1892 | 358 | 156 |
| `Layout &layout = *(Layout *&)Globals::layout` | 1892 | 304 | 155 |
| `Layout *layout = &*Globals::layout` | 1892 | 304 | 155 |
| `Layout *layout` + comma re-assign in c2 | 1892 | 358 | 156 |
| `Layout &layout` + comma re-assign in c2 | 1904 | 328 | 160 |
| `static inline Layout &mtw_layout()` helper, all sites | 1892 | 358 | 156 |
| `PaintCanvas *canvas = Globals::Canvas` + direct layout | 1892 | 355 | 149 |
| `int inset = Globals::w - Globals::layout->buttonInsetX` | 1872 | 352 | 159 |
| `int top` (wave-105 shape) | 1896 | 45 | 23 |
| case-3 `Layout *layout = Globals::layout` local | 1896 | 29 | 18 |
| case-3 `Layout &layout = *Globals::layout` local | 1896 | 29 | 18 |
| case-3 `int left = Globals::layout->field_0xc_leftMargin` | 1896 | 45 | 26 |
| case-3 `top + left + w` / `left + (top + w)` | 1896 | 29 | 18 |
| case-3 `left + w + top` / `int w = this->listEntryWidth` | 1896 | 538 | 427 |

The case-3 `ip`/`r4` tie-break is unaffected by every case-3 spelling tried
(the compiler canonicalises the reordered expressions back to the same IR).

## 2. MGame::OnUpdate — full `Status*` usage-site matrix

The residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2767-2776`). The baseline `Status *st = Globals::status;`
used at `getPlayingTime()` is an RA anchor: it occupies `r5` in the compact
region, makes `tryAssign` fail, and forces `&Globals::status` into `r9`
(wave-107 §2). The original has no cached `Status*` (four fresh `ldr [r9]`)
but still uses `r9`.

I swept all 15 non-empty subsets of `{ccm, getMission, getPlayingTime, field}`
for the `st` usage site (baseline is `..X.`):

| st used at | size | diff bytes |
|---|---:|---:|
| `getPlayingTime` only (baseline) | **11440** | **41** |
| `ccm` + `getPlayingTime` | 11440 | 41 |
| `getPlayingTime` + `field` | 11436 | 4981 |
| `ccm` + `getPlayingTime` + `field` | 11436 | 4981 |
| `getMission` + `getPlayingTime` | 11436 | 4982 |
| `ccm` + `getMission` + `getPlayingTime` | 11436 | 4982 |
| `field` only | 11436 | 4993 |
| `ccm` + `field` | 11436 | 4993 |
| `getMission` + `field` | 11436 | 4966 |
| `ccm` + `getMission` + `field` | 11436 | 4966 |
| `getMission` only | 11396 | 5129 |
| `ccm` + `getMission` | 11396 | 5129 |
| `getPlayingTime` + `getMission` + `field` | 11432 | 5033 |
| all four | 11432 | 5033 |
| `ccm` only (no `st`, 4 fresh loads) | 11392 | 5148 |

Only the two variants that keep `st` live across the `ccm` call **and** use it
at `getPlayingTime` reproduce the 11440 B size and the 41-byte residual; every
other site either drops the size or re-colours the function.

### 2.1 Code-neutral rewrites (no effect)

Each of these compiles to the same machine code and leaves the residual at
11440 B / 41 diff bytes (tested on both the `st` baseline and the no-`st`
shape):

* `23 == Globals::status->getCurrentCampaignMission()`
* `Globals::status->getMission()->isEmpty() == true` / `!= false`
* `!(this->player->isMining())` / `this->player->isMining() == 0`
* `3600000 < Globals::status->getPlayingTime() - ...`
* `3600000LL`, `0x36EE80`, `23u`
* `*(long long *)&Globals::status->field_0x100` instead of the
  `reinterpret_cast`
* `Status *const st`, `decltype(Globals::status) st`
* `static inline Status *mg_status()` helper at every site (11392 / 5148)

`Status &st = *Globals::status;` / `auto &st` / `auto st` are all worse
(11440 B, 727 diff bytes); `Status *&st = Globals::status;` drops to
11392 B. `const Status *st` does not compile (`getPlayingTime` is not const).

## 3. What is proven vs hypothesis

Proven:

* OnTouchBegin's case-15 residual is exactly "original rematerialises
  `&Globals::layout` and uses the fresh value for both fields; ours caches the
  value in `r6`". The all-direct source shape has the correct pre-RA IR but
  `RAGreedy` keeps `%508` in `fp` and gives `&Globals::Canvas` `r5`, so it is
  4 B shorter (1892).
* The case-3 `ip`/`r4` difference is a pure allocator tie-break; `r4` is dead
  in both binaries at that instruction.
* MGame's `st` anchor is the unique 11440 B shape; the full 15-subset usage
  matrix and all code-neutral rewrites leave the 41-byte residual unchanged.
* The 7.0.2 `-print-after-all` MIR for the all-direct OnTouchBegin shape
  confirms the source-level IR is correct and the residual is post-RA.

Hypothesis (not proven):

* The original OnTouchBegin source is the all-direct shape, and the original
  MGame source is the no-`st` shape; in both cases a code-neutral IR
  difference elsewhere in the TU (vreg numbering / live-range order) flipped
  the 7.0.2 greedy decision. No C++ spelling producing that difference without
  adding an instruction was found.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5720
TMPDIR=/dev/shm/w5720/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5720/tmp cmake --build cmake-build-match --target check -j6

python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

MIR dumps used (scratch, not tracked): `/dev/shm/w5720/mt_all_direct.txt`
(4.44 M lines, all-direct OnTouchBegin), `/dev/shm/w5720/mg_all_nost.txt`
(4.23 M lines, no-`st` MGame), `/dev/shm/w5720/mt_701_dbg.txt`
(7.0.1 `-debug-only=regalloc` on the baseline).
