# `HangarWindow::render` — pass 10 (w4511): greedy-allocator priority re-audit of the lazy-load / 376-byte-frame tie

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave95-w4511`, base master `4a88bd6e`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra image base
0x10000; ELF addresses below are the real ones). This pass is a source-shape re-audit of the
known lazy-`&Globals::layout` / 376-byte-frame tie. **No source change beats the 80.6 %
baseline; only this note is committed.**

## TL;DR

- The original `render` is the **fully lazy** shape: `Globals::layout->...` at every use, no
  `Layout *layout` local. Its entry-block roles are `this -> fp`, `&Globals::appManager -> r8`,
  `&Globals::layout -> sl` (materialised at `drawBG`, `0x149132`/`ldr.w sl` at `0x149136`),
  frame **376**.
- Our 80.6 % baseline is the **eager** shape (`Layout *layout = Globals::layout;` at the top,
  used only at `drawBG`). It gets the same three roles and frame 376, but the `&layout` GOT
  load + value load are hoisted into the prologue (`0x1490f6`/`ldr.w r4,[sl]` at `0x149104`),
  i.e. 4 extra instructions and a 12-byte code shift that moves every later intra-function
  branch target.
- Removing the local gives the original prologue exactly, but the allocator flips the two
  long-lived pointers (`this -> sl`, `&layout -> fp`) and grows the frame to 384; the score
  collapses to 61.6 %.
- **New evidence (this pass):** LLVM 7's `RAGreedy::enqueue` orders global live ranges by
  `Size` (larger first; `(1<<31)|(1<<29)|Size`, plus `(1<<30)` for a known preference) with a
  *lower-vreg-number* tie-break. The tie can be flipped to `this -> fp` in the lazy shape only
  by the combination **no `hintLayout` + width-first free-credits order**, and it then puts
  `&appManager` in `sl` and `&layout` in `r8` (not `sl`), keeps frame 384 and scores 51.4 %.
  Every individual edit keeps `this -> sl`.
- The `AM3` appManager-value cache is still the only exact-role lazy shape
  (`this -> fp`, `&layout -> sl`, frame 376, lazy load at `0x149136`), but its cached
  appManager value costs one instruction in the prologue (`ldr r0,[r0]; ldr.w r8,[r0];
  mov r0,r8` vs the original `ldr.w r8,[r0]; ldr.w r0,[r8]`) and re-colours the body;
  re-measured **73.5 % / 9688 B**.
- 40+ further shapes measured this pass (anchor combinations, local placements, expression
  forms, appManager/layout cache forms, `this` aliases, EH sinks) are all strictly worse.
  The 80.6 % body is a robust local optimum.

## Gate (before == after)

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  wrong_type 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
sections: .text 0x1337c8, .rodata 0x41b67, .ARM.exidx 0x6f70, .ARM.extab 0x26290 (-8)
render: 9700/9700 B, raw 0.8064427313, insns 3626/3638, frame 376
```

Build (canonical):

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

## 1. Entry blocks, side by side (re-measured)

Original (`0x1490d8`):

```
1490e4: sub  sp, #376
1490e6: mov  fp, r0                     ; this -> fp
1490e8: ldr  r0,[pc,#808]  ; guard GOT
1490ea: ldr  r1,[pc,#812]  ; Canvas GOT
1490ec: add  r0, pc
1490ee: add  r1, pc
1490f0: ldr  r0,[r0,#0]
1490f2: ldr  r2,[r1,#0]
1490f4: str  r2,[sp,#140]
1490f6: str  r0,[sp,#112]
1490f8: ldr  r1,[r0,#0]
1490fa: ldr  r0,[r2,#0]
1490fc: str  r1,[sp,#372]
1490fe: mov.w r1,#-1
149102: blx  SetColor
149106: ldr  r0,[pc,#788]  ; &appManager
149108: movs r1,#5
14910a: add  r0, pc
14910c: ldr.w r8,[r0]                   ; r8 = &appManager
149110: ldr.w r0,[r8]                   ; appManager value (reloaded per use)
149114: blx  GetApplicationModule
...
149132: ldr  r0,[pc,#748]  ; &Globals::layout (lazy)
149134: add  r0, pc
149136: ldr.w sl,[r0]                   ; &layout -> sl
14913a: ldr.w r0,[sl]                   ; layout value
14913e: blx  drawBG
```

80.6 % baseline (eager local):

```
1490e4: sub  sp, #376
1490e6: mov  fp, r0                     ; this -> fp          (same)
1490e8: ldr  r0,[pc,#816]  ; guard GOT
1490ea: ldr  r1,[pc,#820]  ; &Globals::layout GOT   <-- hoisted
1490ec: ldr  r2,[pc,#820]  ; Canvas GOT
1490ee: add  r0, pc
1490f0: add  r1, pc
1490f2: add  r2, pc
1490f4: ldr  r0,[r0,#0]
1490f6: ldr.w sl,[r1]                   ; &layout -> sl        (same reg, eager)
1490fa: ldr  r2,[r2,#0]
1490fc: str  r2,[sp,#140]
1490fe: str  r0,[sp,#112]
149100: ldr  r1,[r0,#0]
149102: ldr  r0,[r2,#0]
149104: ldr.w r4,[sl]                   ; layout value -> r4 (cached)
149108: str  r1,[sp,#372]
14910a: mov.w r1,#-1
14910e: blx  SetColor
...
14913e: mov  r0, r4                     ; drawBG uses cached value
149140: blx  drawBG
```

Lazy shape (local removed):

```
1490e4: sub  sp, #384                   ; <-- 8 bytes larger
1490e6: mov  sl, r0                     ; this -> sl   <-- flipped
...
149132: ldr  r0,[pc,#756]  ; &Globals::layout (lazy, same position)
149136: ldr.w fp,[r0]                   ; &layout -> fp  <-- flipped
14913a: ldr.w r0,[fp]
14913e: blx  drawBG
```

The lazy shape reproduces the original's *load position* and *appManager* codegen exactly and
differs only in the `this`/`&layout` swap and the resulting 8-byte frame shift.

## 2. New: why the tie flips — LLVM 7 `RAGreedy::enqueue`

Fetched from `llvm-mirror/llvm release_70` (the NDK r18b clang is 7.0.2). The greedy priority
queue is a `std::priority_queue<std::pair<unsigned,unsigned>>`; `enqueue` computes:

```cpp
// global / split ranges: long -> short
Prio = (1u << 31) + Size;
// boost ranges that have a physical register hint
if (VRM->hasKnownPreference(Reg))
  Prio |= (1u << 30);
// local (single-MBB) ranges: linear instruction order
Prio = LI->beginIndex().getInstrDistance(Indexes->getLastIndex());
Prio |= RC.AllocationPriority << 24;
...
// The virtual register number is a tie breaker for same-sized ranges.
// Give lower vreg numbers higher priority to assign them first.
CurQueue.push(std::make_pair(Prio, ~Reg));
```

So for the two long-lived pointers:

- `this` is a **global** range (live across the `bb.0` calls, spilled at `0x14911a`, reloaded in
  the loop) with a `COPY $r0` hint.
- `&Globals::layout` is a **global** range defined at `bb.4` (`0x149132`) with no hint.

The eager shape defines `&layout` in `bb.0`, so both ranges start together and `this` (lower
vreg number, plus hint) wins `fp`; `&layout` gets `sl`. The lazy shape defines `&layout` later,
its interval is shorter, and the allocator's `Size`/hint ordering hands it `fp` first, leaving
`sl` for `this`. This is a *global* allocation decision: the two vregs are the same in both
shapes apart from their definition block.

`-mllvm -print-machineinstrs=prologepilog` confirms the final assignment in each build:

| shape | frame | `this` | `&layout` |
|---|---:|---|---|
| original | 376 | fp (r11) | sl (r10), at `drawBG` |
| baseline (eager) | 376 | fp (r11) | sl (r10), prologue; value cached `r4` |
| lazy | 384 | sl (r10) | fp (r11), at `drawBG` |
| lazy + no `hintLayout` + width-first free credits | 384 | fp (r11) | r8 (r10 = `&appManager`) |
| `AM3` (appManager value cache) | 376 | fp (r11) | sl (r10), at `drawBG` |

## 3. New: the tie can be flipped, but only by changing the body

16-combination sweep of the four "body-shape" edits on the lazy shape
(`h` = remove `hintLayout`, `p` = remove `pad3`/`pad0`/`pad0b`, `d` = remove the three
`description = description` self-assigns, `w` = width-first free-credits order). Only the
`h w` combination (and its supersets) flips `this` to `fp`:

| mask (h p d w) | frame | `this` | `&layout` | render raw |
|---|---:|---|---|---:|
| `0 0 0 0` (lazy) | 384 | sl | r11 | 61.6 |
| `0 0 0 1` (w only) | 384 | sl | r9 | 53.3 |
| `1 0 0 0` (h only) | 384 | sl | r11 | 62.6 |
| `1 0 0 1` (h+w) | 384 | **fp** | r8 | 52.9 |
| `1 1 1 1` (h+w+p+d) | 384 | **fp** | r8 | 51.4 |

Note the flipped lazy shape puts `&Globals::appManager` in `sl` and `&Globals::layout` in `r8`
— not the original's `appManager -> r8`, `&layout -> sl`. It is a three-way re-colouring, not
just a two-way swap, and the frame stays 384. So even the tie-flip does not reproduce the
original's register roles.

## 4. `AM3` (the only exact-role lazy shape), re-measured

```cpp
Globals::Canvas->SetColor(0xffffffffu);
AbyssEngine::ApplicationManager *appManager = Globals::appManager;
ModStation *station = (ModStation *) appManager->GetApplicationModule(5);
...
HangarAppData *data = (HangarAppData *) appManager->GetApplicationData();
```

with no top `Layout *layout` local. Measured **73.5 % / 9688 B / 3626:3635 insns**, frame 376,
`this -> fp`, `&layout -> sl`, lazy load at `0x149136`. Its prologue differs only in the
appManager sequence:

```
AM3:                                    original:
149106: ldr  r0,[pc,#796]               149106: ldr  r0,[pc,#788]
149108: movs r1,#5                      149108: movs r1,#5
14910a: add  r0, pc                     14910a: add  r0, pc
14910c: ldr  r0,[r0,#0]   ; &appManager 14910c: ldr.w r8,[r0]   ; &appManager
14910e: ldr.w r8,[r0]     ; value       149110: ldr.w r0,[r8]   ; value
149112: mov  r0, r8                      149114: blx  GetApplicationModule
149114: blx  GetApplicationModule
```

The cached value costs the extra `mov r0,r8` and removes the original's repeated
`ldr.w rX,[r8]` reloads; the body re-colours from the second item-loop block onward
(`r5`↔`r9`, stack slots shift), which is why it is 12 B short and 7 points below the baseline.

Variants of the appManager cache that try to keep the original first-call codegen all fail:

| shape | raw | size | frame | `this` |
|---|---:|---:|---:|---|
| AM3 (`appManager` value cache, lazy layout) | 73.5 | 9688 | 376 | fp |
| AM4 (cache after `SetColor`, first call via `Globals::appManager`) | 73.5 | 9688 | 376 | fp |
| AM5 (cache after `GetApplicationModule`) | 55.9 | 9712 | 384 | sl |
| AM6 (cache before `SetColor`, first call via `Globals::appManager`, lazy layout) | 55.8 | 9712 | 384 | sl |
| AM7 (AM6 + eager layout local) | 72.8 | 9676 | 376 | fp |
| `&Globals::appManager` pointer-to-pointer (AP1, lazy) | 61.6 | 9724 | 384 | sl |
| AP2 (pointer-to-pointer + eager layout) | 80.6 | 9700 | 376 | fp |

## 5. Other measured shapes (all strictly worse than 80.6)

| shape | raw | size |
|---|---:|---:|
| baseline (eager local, all anchors) | **80.6** | 9700 |
| local after `SetColor` | 80.6 | 9700 |
| local after `GetApplicationModule` (variant D) | 80.5 | 9700 |
| `Layout *volatile` / `Layout *const` local | 80.6 | 9700 |
| lazy, no top local | 61.6 | 9724 |
| lazy + no `hintLayout` | 62.6 | 9712 |
| lazy + no `hintLayout` + no `pad3` | 61.0 | 9708 |
| lazy + no `hintLayout` + plain `float textYf` | 52.8 | 9672 |
| lazy + width-first free credits | 53.3 | 9712 |
| local after `SetColor` (P3) | 80.6 | 9700 |
| `Layout *&layoutRef` at top (used at `drawBG`) | 61.6 | 9724 |
| `Layout **layoutPtr = &Globals::layout` (lazy) | 61.6 | 9724 |
| `Layout **volatile layoutPtr` (value load in prologue) | 66.0 | 9704 |
| `Layout *volatile layout` local | 80.6 | 9700 |
| `Layout *volatile &layout` reference | 80.6 | 9700 |
| `HangarWindow *self = this;` (all `this->` -> `self->`, lazy/eager) | 61.6 / 80.6 | 9724 / 9700 |
| `self` only inside the item loop (eager) | 80.6 | 9700 |
| `hintLayout` before the iPad `if` | 67.5 | 9724 |
| `hintLayout` at function top | 65.9 | 9720 |
| eager + cache `Globals::layout` in the free-credits block | 60.4 | 9780 |
| eager + cache `Globals::layout` in the buy-credits block | 76.7 | 9676 |
| lazy + cache `Globals::layout` in the free-credits block | 56.1 | 9716 |
| lazy + cache `Globals::layout` in the buy-credits block | 50.9 | 9684 |
| station before `SetColor` | 58.4 | 9736 |
| `if (!station || ...)` / `if (!viewMode)` / `if (items)` | 80.6 | 9700 |
| `if (0 < barHeight)` / `if (0 < left)` / `if (right != 0)` / `if (right > 0 \|\| tab == 4)` | 80.6 | 9700 |
| `if (item->inTabIndex >= 0 && tab == 0)` | 80.6 | 9700 |
| ship-condition `\|\|` / negated forms | 80.6 | 9700 |
| `left >= 1` / `right >= 1` | 80.6 | 9700 |
| swap `hintOffsetX + buttonInsetX` addends | 78.9 | 9704 |
| swap `buttonInsetX + hintOffsetX` addends | 79.0 | 9700 |
| `y` computed in Ghidra association (no `baseY`) | 80.6 | 9700 |
| `baseY` reordered (`scrollOffset` first) | 80.5 | 9700 |
| plain `float textYf` (all spellings) | 61.6 | 9740-9756 |
| `double` textY | 61.6 | 9756 |

Full 64-cell anchor sweep (`hintLayout`, `pad3`, `pad0`, `pad0b`, `volatile textYf`,
`description = description` x3) on the eager body: the all-anchors cell is the unique best
(80.6); next best is 79.4 (drop `pad0`/`pad0b`, keep the rest).

## 6. `.ARM.extab` (unchanged, `-8`)

The 80.6 % build keeps `.ARM.extab 0x26290` (original `0x26298`). Re-confirmed this pass:
adding the missing amount-block EH row always costs render score:

| EH sink | render raw | `.ARM.extab` |
|---|---:|---:|
| none (baseline) | **80.6** | 0x26290 |
| `volatile int padS = 0;` before `buttons[0]->setPosition` | 78.7 | 0x26298 |
| `String padS;` in the `left > 0` block | 72.4 | 0x26298 |
| `String padS;` in the `buttons[0]` block | 78.2 | 0x2629c |

The `-8` is therefore accepted (known w4270 residual).

## 7. What remains

- The tie is a **RegAllocGreedy global-range priority** decision, not a source-level statement
  order: `this` (hinted `COPY $r0`) and `&Globals::layout` (unhinted) both span the function,
  and the eager shape's earlier `&layout` definition is what keeps `this` in `fp`.
- A source shape that materialises the `&Globals::layout` GOT address in `bb.0` **without**
  loading its value (so the value is still reloaded at `drawBG`) would match the original
  entry block and the 38 `[sl]` reloads. Every plain-C++ form tried (`&Globals::layout`
  reference, pointer-to-pointer, `volatile` pointer-to-pointer, local declared after the
  branch) either sinks the address to `drawBG` (lazy → tie flip) or also loads the value
  (eager → 4 extra prologue instructions).
- The lazy body needs an extra 8-byte spill slot (`buttonOffset`) that the original does not
  have; that is what makes the frame 384. It is a consequence of the lazy shape's register
  allocation, not an independent source-level object.
- `.ARM.extab` remains 8 bytes short (known w4270 `render` EH residual).

## 8. Reproduction

```sh
# score render (official verifier internals, autojunk=False)
python3 /dev/shm/w4511/raw.py            # 0.8064427313 size 9700 insns 3626/3638

# register roles / frame (post-RA machine code)
CXX=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
FLAGS=$(grep '^CXX_FLAGS' cmake-build-match/CMakeFiles/gof2.dir/flags.make | sed 's/^CXX_FLAGS = //')
$CXX $FLAGS -mllvm -print-machineinstrs=prologepilog -c src/game/ui/HangarWindow.cpp -o /dev/shm/w4511/hw.o

# gate
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```
