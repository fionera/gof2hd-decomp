# `HangarWindow::render` — pass 11 (w4709): lazy-GOT / 376-byte-frame tie, debug-info root cause

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave97-w4709`, base master `9b58a39d5`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra image base
0x10000; ELF addresses below are the real ones).

This pass re-audits the known lazy-`&Globals::layout` / 376-byte-frame tie left open by
`hangarwindow-render-w4411.md` and `hangarwindow-render-w4511.md`, with three new evidence
sources: (a) the pre-RA machine code at the `greedy` pass (which shows *two* layout GOT
loads in the eager shape), (b) a 32-cell anchor sweep on the lazy shape with register
roles recorded, and (c) the iOS 1.1.4 Mach-O. **No source change beats the 80.6 %
baseline; only this note is committed.**

## TL;DR

- The original `render` is the **fully lazy** shape: `Globals::layout->...` at every use,
  no `Layout *layout` local. Its entry-block roles are `this -> fp`, `&Globals::appManager
  -> r8`, `&Globals::layout -> sl` (materialised at `0x149132`, `ldr.w sl,[r0]` at
  `0x149136`), frame **376**.
- Our 80.6 % baseline is the **eager** shape (`Layout *layout = Globals::layout;` at the
  top, used only at `drawBG`). It reproduces the original's three roles and frame 376, but
  hoists the `&layout` GOT load + value load into the prologue
  (`0x1490f6` / `ldr.w r4,[sl]` at `0x149104`).
- **New root cause (this pass):** the eager shape's prologue load is kept alive *only by
  the `-g` debug info*. The pre-RA machine code contains **two** layout GOT loads and
  **two** value loads — one pair in `bb.0` used solely by
  `DBG_VALUE %0, !"layout"`, and one pair in `bb.4` feeding the real `drawBG` call. Without
  the `DBG_VALUE` the `bb.0` pair is dead and removed (that is exactly the lazy shape).
  MachineSinking cannot sink the `bb.0` load past the `SetColor` call (a load from a global
  is not provably invariant across an opaque call), so the eager shape is stuck with it.
- A 32-cell anchor sweep on the lazy body shows **no cell** gives the original's
  `this -> fp` **and** `&layout -> sl` simultaneously. The `volatile float textYf` anchor
  is what pins `this` to `sl`: dropping it flips `this -> fp` but hands `sl` to
  `&appManager` and `r8` to `&layout`.
- `AM3` (cache the `appManager` *value*) remains the only exact-role lazy shape
  (`this -> fp`, `&layout -> sl`, frame 376, lazy load at `0x149136`); its best anchor
  variant is **74.7 % / 9632 B**.
- The base's body is progressively **~70 bytes shorter** than the original by the end of
  the function (`+2` at `0x149134` -> `-70` at `0x14b25e`): the original's body uses more
  high-register (`r8`-`r11`) 4-byte encodings, ours more 16-bit low-register encodings.
  The remaining gap is body register allocation, not the prologue.
- iOS 1.1.4 has **no DWARF** (stripped): `llvm-dwarfdump --debug-info` is empty and
  `llvm-nm` contains no `HangarWindow` symbols, so it adds no source-line evidence.

## Gate (before == after)

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  wrong_type 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
sections: .text 0x1337c8, .rodata 0x41b67, .ARM.exidx 0x6f70, .ARM.extab 0x26294 (-4)
render: 9700/9700 B, raw 0.8064427313, insns 3626/3638, frame 376
```

Build (canonical):

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

## 1. New: the eager prologue load is debug-info-driven

Pre-RA machine code (`-mllvm -print-machineinstrs=greedy`) of the 80.6 % eager build,
render function, `bb.0`:

```
%615:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals6layoutE
%618:gpr  = t2LDRi12 %615:tgpr, 0        ; GOT slot -> %618
%0:gpr    = t2LDRi12 %618:gpr, 0         ; value    -> %0
DBG_VALUE %0, !"layout"                  ; <-- the only use of %0
...
```

and `bb.4` (the `drawBG` block):

```
%627:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals6layoutE
%3368:gpr = t2LDRi12 %627:tgpr, 0        ; a SECOND GOT load
%629:gpr  = t2LDRi12 %3368:gpr, 0        ; a SECOND value load
$r0 = COPY %629
tBL @_ZN6Layout6drawBGEv
```

So the source-level local produces a `bb.0` load that is dead except for its
`DBG_VALUE`; regalloc/CSE then folds the two pairs and hoists `sl`/`r4` into the
prologue. Remove the local (lazy shape) and `bb.0` loses all four instructions, which is
exactly the original's entry block.

The load cannot be sunk instead of removed: `MachineSinking::isProfitableToSinkTo`
(`MachineSink.cpp`, LLVM 7) would allow the sink structurally, but the `bb.0` load is a
load from `Globals::layout` and the intervening `SetColor` call is opaque, so the load is
not safe to move past it.

## 2. New: 32-cell anchor sweep on the lazy body (roles recorded)

`h` = remove `hintLayout`, `p3` = remove `pad3`, `p0` = remove `pad0`/`pad0b`,
`d` = remove the three `description = description`, `t` = plain `float textYf`.

| mask | raw | size | frame | `this` | `&layout` | `&appManager` |
|---|---:|---:|---:|---|---|---|
| lazy (none) | 61.6 | 9724 | 384 | sl | fp | r8 |
| h | 62.6 | 9712 | 384 | sl | fp | r8 |
| p0 | 62.2 | 9716 | 384 | sl | fp | r8 |
| h,p0 | 63.0 | 9704 | 384 | sl | fp | r8 |
| d | 62.0 | 9700 | 384 | sl | fp | r8 |
| h,p3,p0,d | 61.4 | 9672 | 384 | sl | fp | r8 |
| t | 52.9 | 9712 | 384 | sl | r9 | fp |
| h,t | 52.8 | 9672 | 384 | **fp** | r8 | sl |
| h,p0,t | 53.5 | 9660 | 384 | **fp** | r8 | sl |
| h,p3,p0,d,t (faithful) | 52.8 | 9632 | 384 | **fp** | r8 | sl |

Two facts fall out:

* Every non-`t` cell keeps `this -> sl` / `&layout -> fp`, i.e. the `volatile float
  textYf` anchor is what pins `this` to `sl` in the lazy shape.
* Every `t` cell that flips `this -> fp` also swaps `&appManager`/`&layout` (`sl`/`r8`),
  so no cell reproduces `this -> fp`, `&appManager -> r8`, `&layout -> sl`.

## 3. `AM3` (exact-role lazy shape) re-measured

```cpp
Globals::Canvas->SetColor(0xffffffffu);
AbyssEngine::ApplicationManager *appManager = Globals::appManager;
ModStation *station = (ModStation *) appManager->GetApplicationModule(5);
...
HangarAppData *data = (HangarAppData *) appManager->GetApplicationData();
```

with no top `Layout *layout` local.

| shape | raw | size | frame | `this` | `&layout` | load |
|---|---:|---:|---:|---|---|---:|
| AM3 | 73.5 | 9688 | 376 | fp | sl | 0x149136 |
| AM3 + h,p3,p0,d,t | **74.7** | 9632 | 376 | fp | sl | 0x149136 |
| AM3 + t | 66.0 | 9704 | 376 | sl | r9 | 0x149138 |
| AM3 + h,p0,t | 70.0 | 9656 | 376 | fp | r8 | 0x149138 |

AM3 gets the roles right because caching the `appManager` *value* shortens the
`&Globals::appManager` GOT vreg to a single block, so `&layout` becomes the longest global
live range and wins `sl`. But the cached value costs an extra `mov r0,r8` in the prologue
and removes the original's repeated `ldr.w rX,[r8]` reloads, re-colouring the body.

## 4. Pointer / reference forms (all measured this pass)

| shape | raw | size | frame | `this` | `&layout` |
|---|---:|---:|---:|---|---|
| baseline (eager local) | **80.6** | 9700 | 376 | fp | sl |
| eager local after `SetColor` | 80.6 | 9700 | 376 | fp | sl |
| `Layout **layoutPtr = &Globals::layout`, `(*layoutPtr)->drawBG()` (lazy) | 55.3 | 9692 | 384 | sl | fp |
| `Layout *&layout = Globals::layout` (lazy) | 61.6 | 9724 | 384 | sl | fp |
| `ApplicationManager **amPtr = &Globals::appManager` (lazy) | 61.6 | 9724 | 384 | sl | fp |
| `ApplicationManager *&amRef = Globals::appManager` (lazy) | 61.6 | 9724 | 384 | sl | fp |
| `ApplicationManager **amPtr` + eager layout local | 80.6 | 9700 | 376 | fp | sl |
| eager local, `hintLayout` replaced by `layout` | 65.9 | 9720 | 376 | r9 | — |
| eager local, `drawBG` via `Globals::layout`, `layout` used in the iPad block | 76.4 | 9704 | 376 | fp | — |
| eager local, `description`/`price` hoisted out of the credit loop | 79.6 | 9700 | 376 | fp | sl |
| eager local, `String description; String price;` | 78.0 | 9692 | 376 | fp | sl |
| eager local, `String()` in the switch default | 78.4 | 9692 | 376 | fp | sl |

Every pointer/reference spelling of the global either sinks the address to `drawBG`
(lazy -> the `this`/`&layout` swap) or also loads the value (eager -> 4 extra prologue
instructions). There is no plain-C++ form that materialises the `&Globals::layout` GOT
address in `bb.0` *without* loading its value.

## 5. New: where the remaining 19.4 % lives (body, not prologue)

Aligning the 80.6 % build against the original instruction-by-instruction (register names
and absolute operands masked) and recording the address delta at every equal run:

```
orig 0x149134 ours 0x149136 delta  +2
orig 0x14919c ours 0x1491a0 delta  +4
orig 0x149414 ours 0x149410 delta  -4
orig 0x14956e ours 0x149560 delta -14
orig 0x14966e ours 0x14965a delta -20
orig 0x149928 ours 0x14990e delta -26
orig 0x149bb4 ours 0x149ba4 delta -16
orig 0x149d82 ours 0x149d64 delta -30
orig 0x14a068 ours 0x14a038 delta -48
orig 0x14a75a ours 0x14a72c delta -46
orig 0x14b036 ours 0x14b006 delta -48
orig 0x14b25e ours 0x14b218 delta -70
```

The base is *shorter* than the original through the whole body even though both functions
are 9700 B (the base's literal pool is correspondingly larger). The original uses more
`ldr.w`/`add.w`/`strd` forms on `r8`-`r11`; the base uses more 16-bit low-register forms.
That is a body register-allocation difference (the original's `r8`/`sl`/`fp` pressure
forces 4-byte encodings), not a prologue artefact — so even a perfect lazy prologue would
not by itself make the branch targets line up.

Score decomposition of the 80.6 % baseline (official verifier, `autojunk=False`):

| normalisation | ratio |
|---|---:|
| as scored | 80.6 |
| `loc_*` targets masked | 85.3 |
| `loc_*` targets + register names masked | 90.1 |

So ~389 of the 880 mismatch slots are intra-function branch targets shifted by the
prologue, and ~360 are real body instruction differences.

## 6. iOS 1.1.4 Mach-O

`/home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD` (Mach-O armv7, 4.9 MB) is **stripped
of DWARF**: `llvm-dwarfdump --debug-info` prints an empty `.debug_info` and `llvm-nm`
contains no `HangarWindow` symbols at all (1527 symbols, all imports/exports). It adds no
source-line or local-variable evidence. The `source file list dump/gof2hd_1.1.19_ios.cpp`
TU dump remains the only layout witness: `HangarWindow::render` is the ninth function of
`Classes/Game/HangarWindow.cpp`, and `FREE_CREDIT_AMOUNTS` / `HangarWindow::lastTab` are
the TU's only statics.

## 7. What remains

- The tie is a **RegAllocGreedy global-range size ordering** between
  `&Globals::layout` and `&Globals::appManager`. In the eager shape the layout GOT vreg is
  defined in `bb.0` and is the longest global range, so it takes `sl`; in the lazy shape
  it is defined in `bb.4`, `&appManager` outlives it, and the pair swaps. `AM3` shows the
  roles can be restored by shortening the `&appManager` range, but only at the cost of the
  original's per-use `ldr.w rX,[r8]` reloads.
- A source shape that materialises the `&Globals::layout` GOT address in `bb.0` *without*
  loading its value would match the original entry block and the 38 `[sl]` reloads.
  `Layout **` / `Layout *&` / `Layout *volatile *` all either sink the address to
  `drawBG` or also load the value.
- The body is ~70 B short of the original's (denser 16-bit encodings); closing that is a
  body register-allocation problem, independent of the prologue tie.
- `.ARM.extab` is `0x26294` (`-4`), the known w4270 `render` EH residual.

## 8. Reproduction

```sh
# score render (official verifier internals, autojunk=False)
python3 /dev/shm/w4709/raw.py            # 0.8064427313 size 9700 insns 3626/3638

# register roles / frame / lazy-GOT load address
python3 /dev/shm/w4709/roles.py          # frame 376 this fp &layout sl &appManager r8

# pre-RA machine code (shows the two layout GOT loads of the eager shape)
CXX=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
FLAGS=$(grep '^CXX_FLAGS' cmake-build-match/CMakeFiles/gof2.dir/flags.make | sed 's/^CXX_FLAGS = //')
$CXX $FLAGS -mllvm -print-machineinstrs=greedy -c src/game/ui/HangarWindow.cpp -o /dev/shm/w4709/hw.o \
    2>/dev/shm/w4709/mi_greedy.txt

# gate
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

Scratch scripts used this pass live in `/dev/shm/w4709` (`raw.py`, `roles.py`,
`ranges.py`, `livint2.py`, `deltas2.py`, `bigdiff.py`, `sweep_roles.py`, `hwtry.py`,
`batch1..7.py`).
