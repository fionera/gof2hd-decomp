# Hud::draw — wave 139 (w8417): full Thumb hunk census and a 1-byte raw-diff tie-break

**Status:** source change landed, 2026-11-07 (wave 139). Worktree `wt/w8417`,
base `master bf123bd47`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, symbol parity 5370/5370.

Raw same-address diff (primary metric): **1417 -> 1416 B**. `.text` differing
bytes **85,476 -> 85,475**. Function size stays **9332 B**. Fuzzy moves
96.68307 -> 96.65133 (84 -> 85 difflib opcodes): the 1-byte win is a raw-diff
tie-break, not an opcode-stream win. All section sizes stay exact.

---

## 0. TL;DR

1. This note re-diffs the **full Thumb listing** after w8317's dock1 `int top`
   fix and classifies all 84 remaining difflib opcodes by source region.
2. The largest single hunk is the dock2 `String text` / `halfW` order at
   `0x2016` (21 raw bytes; the `0x2016`+`0x201c`+`0x203c` cluster is 59 B).
   The faithful source order is `String text` first, then `halfW`; but that
   reshape (variant **S**) is **9328 B / 2139 raw diff** because it changes the
   whole-function register allocation and loses the dock1
   `mov r5,fp` + `mov r9,r0` pair (the same 4-byte deficit documented in
   `hud-draw-wave138-w8317.md`).
3. **Landed:** dock1 `frameH` is spelled `unsigned short` instead of `short`.
   The original's `frameH` is `int` (no `sxth`/`uxth`), so both spellings are
   allocator tie-breaks; `unsigned short` changes the emitted `sxth` to `uxth`
   and recovers exactly **1 raw `.text` byte** in the dock1 `progress`
   conversion while keeping 9332 B. It costs one extra difflib opcode.
4. New negative results: the full raw-diff type sweep (48 combinations), the
   pointer-temp `getText` hoist, dock2 statement-order permutations, dock1 /
   dock2 `PaintCanvas *` locals, a `Status *st` cache, and `barYOffset` /
   dock2 local type sweeps. None beats 1417 except `frameH unsigned short`.

---

## 1. Full hunk census (84 opcodes, 1417 raw bytes)

Addresses are function-relative (VMA - `0x163b90`). The function is 9332 B on
both sides; every hunk is size-neutral. Raw bytes are the number of
same-address differing bytes in the region (`regiondiff.py`), not the sum of
the difflib hunk spans.

| region | func offset | opcodes | raw diff | character |
|---|---|---:|---:|---|
| regen `pivotY` trace | `0x464` | 1 | 1 | `strd r4,sl` vs `strd r4,r4` (wave-135 anchor) |
| gamma body | `0x4c2`-`0x4ee` | 4 | 5 | `fp`/`r9` swap for `field_0x448`/`field_0x44a` |
| damage indicators | `0x5e6`-`0x7e0` | 16 | 481 | dead `ldr.w r8,[sp,#108]`; `Globals::status &&`; `sl`/`fp` rotation |
| dock1 (docking transfer bar) | `0x1cd4`-`0x1f44` | 32 | 413 | `short`/`unsigned short` locals, `barW`/`barH` register swap, branch targets |
| dock2 (jump-drive/cloak bar) | `0x1f44`-`0x2200` | 28 | 500 | `getText`/`halfW` order, `top`/`barYOffset` scheduling, branch targets |
| EH cleanup pad | `0x242c`-`0x2440` | 3 | 2 | branch-target layout |

The branch-target-only hunks (`beq.w 0x1fc0` vs `0x1fc2`, `cbz 0x1f3c` vs
`0x1f3e`, ...) are consequences of the +3 instruction count; they disappear
if the count is ever fixed.

### Largest individual hunks

| raw | orig range | opcode |
|---:|---|---|
| 21 | `0x2016` | `ldr r0,[pc,#188]` vs `GetImage2DWidth` + `getText` sequence |
| 19 | `0x201c` | dead `ldrb.w r2,[sl,#631]` |
| 19 | `0x203c` | second `GetImage2DWidth` vs reuse of `halfW` |
| 14 | `0x20e4` | `mov fp,r9` vs `mov r4,r9` (Canvas register) |
| 13 | `0x1e3a` | `vmov s0,r8`/`vcvt.f32.s32` vs `uxth`/`vcvt.f32.u32` (`barW`) |
| 12 | `0x1e5c` | signed `barW/2` vs `ubfx` unsigned divide |
| 11 | `0x1db2` | dock1 `GetImage2DHeight`/Canvas preservation |
| 11 | `0x20f2` | dock2 `fp`/`r5` rotation |
| 10 | `0x06a4` | damage branch target |
| 10 | `0x06c6`/`0x06ca` | literal-pool offsets |
| 10 | `0x1db4` | dock1 Canvas preservation |
| 9 | `0x078a` | damage `r8`/`sl`/`fp` rotation |
| 9 | `0x1d56` | dock1 `cmp.w r8/r9` |
| 9 | `0x1df4` | dock1 `top`/`layout` order |

---

## 2. The largest hunk: dock2 `String text` before `halfW`

The original dock2 block is:

```
165ba6: ldr r0,[pc,#188]      ; Globals::gameText
165ba8: mov.w r1,#318
165bac: ldrb.w r2,[sl,#631]   ; field_0x277
...
165bbe: blx GameText::getText
165bc2: mov r1,r0
165bc4: add r0,sp,#208
165bc6: movs r2,#0
165bc8: blx String::String(String const&, bool)
165bcc: ldr.w r1,[sl,#892]    ; image_0x37c
165bd0: ldr.w r0,[r9]         ; Canvas
165bd4: blx PaintCanvas::GetImage2DWidth
165bd8: vmov s2,r0
...
165be8: vcvt.s32.f32 s18,s0   ; halfW
```

The current tree computes `halfW` first and constructs `String text` second.
The faithful reshape is variant **S**:

```cpp
String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
```

Measured: **9328 B / 2139 raw diff / 120 opcodes**. The 4-byte loss is the
dock1 `mov r5,fp` + `mov r9,r0` pair: the String temporary's longer live range
changes the whole-function allocation, `total` moves from `r8` to `r5`,
`text` from `r5` to `r6`, and `Canvas` from `r6` to `fp`. The dock2 cluster is
fixed but the allocation changes cost far more than 59 bytes.

Pointer-temp variant **P** (`const String *textSrc = getText(...); halfW = ...;
String text(*textSrc, false);`) keeps 9332 B but is **1472 raw diff**: it fixes
the 21-byte `getText` hunk and introduces a 22-byte `String ctor` hunk at
`0x203c`.

---

## 3. New sweep: raw-diff type sweep (not hunk-count)

Wave 138 swept dock1 types by difflib hunk count. This wave re-swept them by
**raw same-address diff**, which is the primary metric. 48 combinations of
`frameH` x `total` x `barW` x `barH` x `top` (short/int/unsigned short/unsigned
int) were built and scored. The current `short/short/unsigned short/int/int`
is still the optimum at 1417, except:

| frameH | total | barW | barH | top | raw diff | size | opcodes | fuzzy |
|---|---|---|---|---|---:|---:|---:|---:|
| **unsigned short** | **short** | **unsigned short** | **int** | **int** | **1416** | **9332** | **85** | **96.65133** |
| short | short | unsigned short | int | int | 1417 | 9332 | 84 | 96.68307 |
| unsigned short | short | unsigned short | int | unsigned short | 1416 | 9332 | 85 | 96.65133 |
| unsigned short | unsigned short | unsigned short | int | int | 1416 | 9332 | 86 | 96.61958 |
| short | short | int | int | int | 1696 | 9328 | 115 | 94.79365 |
| int | short | unsigned short | int | int | 1797 | 9328 | 110 | 94.99921 |
| int | int | int | int | int | 1873 | 9328 | 128 | 94.36060 |

`frameH unsigned short` changes the dock1 `progress` conversion from
`sxth.w r0,r8` to `uxth.w r0,r8`, which matches one more byte of the original's
`vmov s0,r8` at `0x1e3a`; the rest of the dock1 allocation is unchanged
(`barW` still `fp`, `barH` still `r8`). The original's `frameH` is `int`
(no extend at all), so this is a tie-break spelling, not a type recovery.

---

## 4. Other new hypotheses tried (all worse)

All measured on the wave-138 base (9332 B / 1417 raw / 84 opcodes).

| variant | raw diff | size | opcodes | note |
|---|---:|---:|---:|---|
| dock2 **S** (`String text` before `halfW`) | 2139 | 9328 | 120 | faithful order, allocation breaks |
| dock2 **S+T** (`frameH int`) | 1691 | 9328 | 105 | |
| dock2 **S+V** (`barW int`) | 1590 | 9328 | 110 | best S combination, still worse |
| dock2 **S+T+U+V+Z** (`y` after `SetColor`) | 1998 | 9332 | 117 | keeps size, worst diff |
| dock2 **Y** (`top`/`barYOffset` after `fade`) | 1495 | 9332 | 87 | closest reorder |
| dock2 **Z** (`y` after `SetColor`) | 2099 | 9320 | 116 | |
| dock2 **Y+Z** | 2036 | 9320 | 112 | |
| dock2 order **A/B/C/D** (top/barYOffset permutations) | 1495-1498 | 9332 | 85-87 | |
| dock2 order **E** (top/barYOffset before fade) | 2023 | 9320 | 112 | |
| dock1 `PaintCanvas *canvas` (barW+barH) | 1529 | 9332 | 92 | |
| dock1 `PaintCanvas *canvas` (all calls) | 3682 | 9312 | 212 | |
| dock1 `canvas` (first four calls only) | 3023 | 9332 | 169 | |
| dock2 `PaintCanvas *canvas` (SetColor+draws) | 3530 | 9316 | 196 | |
| dock2 `PaintCanvas *&canvas` reference | 1417 | 9332 | 84 | no-op |
| whole-function `PaintCanvas *&canvas` reference | 2106 | 9332 | 122 | |
| `Status *st` cache (damage block) | 8824 | 9304 | 590 | |
| `Status *st` cache (mission panel) | 8795 | 9292 | 588 | |
| remove `Globals::status &&`, anchor in mission panel | 6611-7448 | 9268-9340 | 308-335 | frame collapses |
| dock1 `barYOffset` int/unsigned/short/long | 1417-1418 | 9332 | 84 | no-op |
| dock2 `top`/`y`/`halfW`/`barH` type sweep | 1417-3431 | 9328-9340 | 84-268 | no win |
| dock1 `top` unsigned int/short/long | 1417-1419 | 9332 | 84-85 | no win |

The `PaintCanvas *&canvas` reference is a no-op because clang already models
`Globals::Canvas` as a reference to the global slot and keeps
`&Globals::Canvas` in a register (`r6` in ours, `fp` in the original). A
value cache (`PaintCanvas *canvas = Globals::Canvas`) is *not* equivalent: it
extends the pointer's live range and changes the whole allocation.

---

## 5. Landed change

```cpp
-            short frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
+            unsigned short frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
```

This is the only source change. It keeps the original 9332-byte frame, keeps
every section size exact, and lowers the raw `.text` diff by 1 byte
(1417 -> 1416). The difflib opcode count rises 84 -> 85 and the fuzzy score
drops 96.68307 -> 96.65133; per the wave instructions, raw same-address diff is
the primary metric and the fuzzy score is process-nondeterministic.

`docs/IDENTICALITY.md` is updated in the same commit: `.text` differing bytes
85,476 -> 85,475 and a new `Hud::draw` w8417 bullet.

---

## 6. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w8417
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w8417`: `fdiff.py` (raw same-address diff),
`classify.py` / `classify_arg.py` (difflib opcode census with addresses),
`hunks.py` (region totals), `regiondiff.py` (raw diff per source region),
`base.so` / `USH.so` (before/after builds), `sweep*.py` (the type/order/local
sweeps above), `orig_dock1b.asm` / `orig_damage.asm` / `orig_dock2.asm`
(original Thumb listings).
