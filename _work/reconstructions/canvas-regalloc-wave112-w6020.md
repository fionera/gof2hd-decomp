# Shared-cause register assignment: `&Globals::Canvas` in `r5` vs `fp` — wave 112 / w6020

**Status:** research note only, **no `src/` change landed**. Worktree `wt/w6020`,
branch `wave112-w6020`, base = current master `5d98bd391` (the wave-111
harvest). Targets:

* `_ZN12HangarWindow6renderEv` — original `0x1490d8`, **9700 B**, ours 9700 B,
  fuzzy **89.1 %**.
* `_ZN3Hud4drawExxP9PlayerEgobjj` — original `0x163b90`, **9332 B**, ours
  9332 B, fuzzy **94.6 %**.

Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra − 0x10000).

This note answers the wave-111 shared-cause (a) question: *is the
`Globals::Canvas` `r5`-vs-`fp` choice a source-shape difference, and what is
the concrete patch recipe?*

---

## 0. TL;DR

* The wave-111 claim is **proven for `Hud::draw`**: the value in question is
  not the `PaintCanvas *` but the **address of the GOT slot**,
  `&Globals::Canvas`.  In both binaries that address is live in `r9` from
  function offset `0x1326` to `0x1d10/0x1d12`.  At that point `r9` is needed
  for the `getDockTotalAmount()` result, so the allocator must shuffle:
  * original: `mov r5, fp` (ego → r5), `mov fp, r9` (**Canvas → fp**),
    `mov r9, r0`;
  * ours: `mov r5, r9` (**Canvas → r5**), `mov r9, r0`, `mov r0, fp`
    (ego stays in fp).
* This is a **pure greedy-RA tie-break**, not a different source expression.
  The two functions' instruction streams are identical up to that point.
* **One source lever does flip it**: swapping the two calls
  `ego->getDockTransferedAmount()` / `ego->getDockTotalAmount()` makes the
  allocator choose `mov fp, r9` (Canvas → fp), matching the original.  But
  the original's call order is transferred-then-total, so the swap is not
  faithful and costs **94.6 → 93.9 %**.  No tested shape gives both the
  original call order and the original register.
* **The wave-111 claim does not hold for `HangarWindow::render`.**  In that
  function `fp` is `this` (prologue) and later a `String` temporary
  (`mov fp, r0` at `0x149dd4`); the Canvas pointer never lives in `fp`.
  The first divergence there is a different shared cause: the `item` /
  `Globals::layout` `ldrd` pairing at offset `0x570` (`ldr r4,[sp,#120]` +
  `ldr r5,[sp,#132]` in the original vs `ldr r0,[sp,#120]` +
  `ldrd r5,r6,[sp,#132]` in ours).  Swapping the analogous
  `getStationAmount`/`getAmount` calls costs 89.1 → 85.0 %.
* **Secondary finding (real source-type bug):** the original's `frameH` and
  `transferred` in `Hud::draw` are **`int`**, not `short`.  The original has
  no `sxth`; our source emits two (`sxth.w r0,fp` at `0x1d64`, `sxth r0,r0`
  at `0x1d82`).  Changing both to `int` removes the two `sxth` (8 B) and
  gives size 9324 B / 93.6 %, so it cannot land alone (size must stay 9332).

**Best measured variant is the current baseline** (Hud 94.6 %, Hangar
89.1 %).  The note-only commit is the deliverable.

---

## 1. Exact commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w6020
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Baseline (verified this pass):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint:   CLEAN
drift:  0 field(s) across 0 class(es)
```

Section state (unchanged; `.ARM.extab` is **exact** on this base because the
wave-110/111 `Level::createShip` pass landed):

```text
file size            2,187,328 B
.text                0x1337c8
.rodata              0x41c27  (192 B rank table appended)
.ARM.exidx           0x6f70
.ARM.extab           0x26298  (original 0x26298 — exact)
```

Per-function side-by-side disassembly:

```sh
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump "$OBJ" \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
python3 tools/verify/verify.py ... --show _ZN12HangarWindow6renderEv
```

The raw dumps below were produced with a small harness that imports
`tools/verify/verify.py`'s `disassemble()` and prints both streams by
function-relative offset (scratch: `/dev/shm/w6020/dumpfn2.py`,
`/dev/shm/w6020/score2.py`).

---

## 2. `Hud::draw` — the exact `r5` vs `fp` site

Function-relative offsets; original on the left, ours on the right.
`[sp,#120]` is `&Globals::Canvas` (the GOT slot address, per the wave-92 slot
map in `hud-draw-status-slot-w4253.md`).  `fp` is `ego` before this point
(`ldr.w fp,[sp,#116]` at offset `0x1ce` in both).

```text
orig 0x1cf4  ldr.w r0, [r9]          ; Canvas = [&Globals::Canvas]
orig 0x1cf8  blx   GetImage2DHeight
orig 0x1cfc  str   r0, [sp, #96]     ; frameH
orig 0x1d06  ldr.w r0, [r9]
orig 0x1d0c  blx   GetTextHeight
orig 0x1d10  mov   r5, fp            ; ego  -> r5
orig 0x1d12  mov   fp, r9            ; *** Canvas GOT -> fp ***
orig 0x1d14  mov   r9, r0            ; textH -> r9
orig 0x1d16  mov   r0, r5            ; ego
orig 0x1d18  blx   getDockTransferedAmount
orig 0x1d1c  mov   r4, r0            ; transferred -> r4
orig 0x1d1e  mov   r0, r5            ; ego
orig 0x1d20  blx   getDockTotalAmount
orig 0x1d24  mov   r5, r0            ; total -> r5
```

```text
ours 0x1cf4  ldr.w r0, [r9]
ours 0x1cf8  blx   GetImage2DHeight
ours 0x1cfc  str   r0, [sp, #96]
ours 0x1d06  ldr.w r0, [r9]
ours 0x1d0c  blx   GetTextHeight
ours 0x1d10  mov   r5, r9            ; *** Canvas GOT -> r5 ***
ours 0x1d12  mov   r9, r0            ; textH -> r9
ours 0x1d14  mov   r0, fp            ; ego (stays in fp)
ours 0x1d16  blx   getDockTransferedAmount
ours 0x1d1a  mov   r1, fp            ; ego
ours 0x1d1c  mov   fp, r0            ; transferred -> fp
ours 0x1d1e  mov   r0, r1
ours 0x1d20  blx   getDockTotalAmount
ours 0x1d24  mov   r6, r0            ; total -> r6
```

The whole `{r4,r5,r6,fp}` colouring is rotated:

| value | original | ours |
|---|---|---|
| `&Globals::Canvas` | `fp` (then `r5` after `0x1dbc`) | `r5` |
| `ego` | `r5` | `fp` |
| `transferred` | `r4` | `fp` |
| `total` | `r5` | `r6` |
| `text` (String) | `r6` | `r4` |

The live range of `&Globals::Canvas` in `r9` is huge: `ldr.w r9,[sp,#120]`
at offset `0x1326` (both binaries) through `0x1d10/0x1d12`.  The shuffle is
forced by `r9` becoming the `total` result.  Which of `ego` / Canvas keeps
`fp` is the tie-break.

### 2.1 What the original source is not

The original does **not** cache the `PaintCanvas *` in a local.  Every use is
`ldr r0, [fp]` (load the pointer through the GOT slot), not `mov r0, fp`.
A local `PaintCanvas *canvas = Globals::Canvas;` therefore changes the
instruction shape and is wrong (measured 90.5 %, 9304 B — §4.2).

The original also does **not** use a reference local in a way that changes
the IR: `PaintCanvas *&canvas = Globals::Canvas;` is codegen-identical
(94.6 %, 9332 B — §4.2).

### 2.2 The one lever that flips it: call placement

Swapping the two `ego` calls in the source

```cpp
int total = ego->getDockTotalAmount();
short transferred = ego->getDockTransferedAmount();
```

makes the allocator emit the original's Canvas move:

```text
ours (swapped) 0x1d10  mov r4, fp
ours (swapped) 0x1d12  mov fp, r9     ; Canvas -> fp  (matches original)
ours (swapped) 0x1d14  mov r9, r0
ours (swapped) 0x1d16  mov r0, r4
```

Score **93.9 %**, size 9332 B, `.ARM.extab 0x26298`, full `check` still
`RATCHET PASS` (byte 4500 / linked 4500).  The 0.7-point loss is the
different call order itself (the original calls transferred first).

No tested spelling gives the original call order *and* the `fp` colouring:
* `int total = 0;` before the calls, then assign in the original order — no
  change (94.6 %);
* `float progress = (float) total; progress = (float) transferred / progress;`
  — no change (94.6 %);
* `PlayerEgo *e = ego;` — no change (94.6 %);
* declaration-order swaps without changing the initializer order — no change.

That is the strongest evidence that this is a **greedy-RA priority tie-break**
whose input is the SSA definition order, not a source-level lifetime that can
be independently adjusted.

### 2.3 Secondary: the original's `frameH`/`transferred` are `int`

The original has **no `sxth`** anywhere in `Hud::draw`.  Ours has two:

```text
ours 0x1d64  sxth.w r0, fp     ; (short) transferred
ours 0x1d82  sxth   r0, r0     ; (short) frameH
```

`GetImage2DHeight` returns a zero-extended `ldrh` value, and
`getDockTransferedAmount` returns a 32-bit field load, so the original source
must use `int frameH` and `int transferred`.  Our source uses `short` for
both.  Measured:

| `frameH` / `transferred` | size | fuzzy |
|---|---:|---:|
| `short` / `short` (landed) | 9332 | **94.6** |
| `short` / `int` | 9328 | 94.4 |
| `int` / `short` | 9328 | 93.8 |
| `int` / `int` | 9324 | 93.6 |

Fixing the types is faithful but removes 4–8 B, so it cannot land alone
(the 9332-byte size is a hard constraint for `byte_exact`).  It is recorded
here so a future size-neutral pass can pair it with the missing 4–8 B
elsewhere.

---

## 3. `HangarWindow::render` — the claimed shared cause does not hold

`fp` in `HangarWindow::render` is `this` from the prologue
(`mov fp, r0` at `0x1490e6`) and is later reused for a `String` temporary
(`mov fp, r0` at `0x149dd4`).  The Canvas pointer is kept in the stack slot
`[sp,#140]` and loaded into `r4`, `r8`, `r9`, `r0`, `r5`, `r6` at the
various call sites; **it is never in `fp`**.  The original does use `r5` for
the Canvas pointer at `0x14ada6` (`ldr r5,[sp,#140]`) while ours uses `sl`
at the corresponding site — a different register pair, not `r5` vs `fp`.

The **first** semantic divergence in `HangarWindow::render` is at offset
`0x570` (inside the `if (tab == 1)` arm of the item branch):

```text
orig 0x570  ldr   r4, [sp, #120]      ; item
orig 0x572  ldr   r1, [sp, #108]      ; tab
orig 0x574  ldr   r6, [sp, #136]      ; this
orig 0x576  ldr   r0, [r4, #16]       ; item->item
orig 0x578  cmp   r1, #1
orig 0x57a  bne.w 0xbb0
orig 0x57e  blx   Item::getStationAmount
orig 0x582  ldr   r5, [sp, #132]      ; Globals::layout
orig 0x584  mov   r9, r0
orig 0x586  ldr   r0, [r4, #16]       ; item->item again (item kept in r4)
```

```text
ours 0x570  ldr   r0, [sp, #120]      ; item
ours 0x572  ldr   r1, [sp, #108]
ours 0x574  ldr   r0, [r0, #16]
ours 0x576  cmp   r1, #1
ours 0x578  ldrd  r5, r6, [sp, #132]  ; layout + this paired
ours 0x57c  bne.w 0xbb0
ours 0x580  blx   Item::getStationAmount
ours 0x584  ldr.w r8, [sp, #140]      ; Canvas
ours 0x588  mov   r9, r0
ours 0x58a  ldr   r0, [sp, #120]      ; item reloaded
ours 0x58c  ldr   r0, [r0, #16]
```

The original keeps `item` in `r4` across the `getStationAmount` call and
loads `Globals::layout` only inside the `tab == 1` arm.  Ours rematerialises
`item` from `[sp,#120]` and hoists `layout` into an `ldrd` before the branch.
This is a scheduling/rematerialisation difference, not a Canvas-lifetime
difference.

Tested HangarWindow levers (all `Hud` unchanged at 94.6 %):

| variant | size | fuzzy |
|---|---:|---:|
| baseline | 9700 | **89.1** |
| move `ListItem *item = (*items)[i];` to the top of the loop body | 9700 | 89.1 |
| `ListItem *const item` | 9700 | 89.1 |
| `ListItem *volatile item` | 9700 | 59.7 |
| swap `int right, left;` declaration order | 9700 | 89.1 |
| swap `getStationAmount` / `getAmount` calls | 9708 | 85.0 |
| `PaintCanvas *&canvas = Globals::Canvas;` at top of loop | 9700 | 89.1 |
| `PaintCanvas *canvas = Globals::Canvas;` at top of loop | 9512 | 63.5 |
| remove the `volatile int pad3 = left;` anchor | 9696 | 86.7 |

The analogous "swap the two calls" recipe therefore does **not** transfer to
`HangarWindow::render`; it costs 4.1 points and does not move `item` to `r4`.

---

## 4. `Hud::draw` variant table (all measured, official verifier)

Every row is a real C++ source edit in `src/game/ui/Hud.cpp`, built with
`cmake --build cmake-build-match --target link_so -j6` and scored with
`tools/verify/verify.py` (`autojunk=False`).  Baseline: 9332 B / 94.6 % /
`.ARM.extab 0x26298`.

### 4.1 Declaration / type shapes

| variant | size | fuzzy |
|---|---:|---:|
| baseline | 9332 | **94.6** |
| hoist `short frameH; int textH; short transferred; int total;` then assign | 9332 | 94.6 |
| `short transferred = 0; int total = 0;` then assign | 9332 | 94.6 |
| `float progress = (float) total; progress = (float) transferred / progress;` | 9332 | 94.6 |
| `PlayerEgo *e = ego;` then `e->getDock...` | 9332 | 94.6 |
| `int transferred` | 9328 | 94.4 |
| `int frameH` | 9328 | 93.8 |
| `int frameH; int transferred` | 9324 | 93.6 |

### 4.2 Canvas-local shapes

| variant | size | fuzzy |
|---|---:|---:|
| `PaintCanvas *&canvas = Globals::Canvas;` in the first-bar block | 9332 | 94.6 |
| `PaintCanvas *canvas = Globals::Canvas;` in the first-bar block | 9304 | 90.5 |

### 4.3 Call placement

| variant | size | fuzzy | Canvas reg |
|---|---:|---:|---|
| baseline: `transferred` then `total` | 9332 | **94.6** | `r5` |
| swap: `total` then `transferred` | 9332 | 93.9 | **`fp`** |
| swap `frameH`/`textH` calls | 9332 | 94.1 | `r5` |
| swap both pairs | 9336 | 93.0 | — |
| `String text` before `frameH` | 9340 | 91.2 | — |
| `String text` after `progress` | 9340 | 91.7 | — |

### 4.4 Second-bar / gamma shapes (cross-checks)

| variant | size | fuzzy |
|---|---:|---:|
| inline `filled` (no local) | 9356 | 84.4 |
| `int` types + inline `filled` | 9348 | 84.4 |
| gamma second call `Globals::status->getCurrentCampaignMission()` | 9312 | 74.8 |

These confirm the wave-111 result: the second-bar `filled` local and the
gamma `Status *st` local are load-bearing for the 9332-byte shape, and
changing them re-colours the whole frame.

---

## 5. Proven vs hypothesis

**Proven:**

1. The `Hud::draw` `r5`-vs-`fp` difference is the **GOT-slot address**
   `&Globals::Canvas`, not the `PaintCanvas *`.  It is live in `r9` from
   offset `0x1326` to `0x1d10/0x1d12` in both binaries.
2. The original moves it to `fp` (`mov fp, r9`); ours moves it to `r5`
   (`mov r5, r9`).  The entire `{r4,r5,r6,fp}` colouring is rotated.
3. Swapping the `getDockTransferedAmount` / `getDockTotalAmount` calls
   flips the Canvas register to `fp` (93.9 %, 9332 B, extab exact, gate
   still `RATCHET PASS`).  This is the concrete patch recipe, but it is not
   a score win because the original call order is transferred-first.
4. The original's `frameH` and `transferred` are `int`; our `short` adds two
   `sxth` (8 B).  `int`/`int` gives 9324 B / 93.6 %.
5. `HangarWindow::render` does **not** exhibit the `r5`-vs-`fp` Canvas swap:
   `fp` is `this` / a `String` temporary there.  Its first divergence is the
   `item`/`layout` `ldrd` rematerialisation at `0x570`.

**Hypothesis:**

* The tie-break is LLVM greedy's interval priority (SSA definition order and
  spill weight).  A source shape that keeps the transferred-first call order
  but makes `total`'s interval outrank `ego`'s would reproduce the original
  without the 0.7-point call-order penalty.  No such shape was found in the
  local declaration/lifetime space tested here.
* The `int`-type fix plus the missing 4–8 B elsewhere in the first bar is the
  most promising route to a *faithful* 9332-byte shape; it is not a fuzzy
  win on its own.

---

## 6. Reproduction / scratch

```sh
# one-variant rebuild + score (≈21 s)
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --top 0
```

Scratch harnesses (not committed): `/dev/shm/w6020/dumpfn2.py` (raw
side-by-side by function-relative offset), `/dev/shm/w6020/score2.py`
(two-symbol scorer + `.ARM.extab` size), `/dev/shm/w6020/score.sh`
(rebuild + full verify).

Worktree left clean; only this note is committed.
