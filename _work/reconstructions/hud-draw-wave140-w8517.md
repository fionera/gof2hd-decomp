# Hud::draw — wave 140 (w8517): dock1 locals are `int`, and the dock2 String/halfW reshape is EH-bound

**Status:** source change landed, 2026-11-07 (wave 140). Worktree `wt/w8517`,
base `master d4857f174`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, symbol parity 5370/5370.

Raw same-address diff (primary metric): **1416 -> 1362 B**. Whole-`.text`
differing bytes **84,651 -> 84,597** (measured on this worktree; the docs'
inline 85,2xx/85,5xx numbers are from earlier wave-139 commits). Function size
stays **9332 B**, every section size/base stays exact (`.ARM.extab 0x26298`,
`.rodata 0x1c9e10`). Fuzzy 96.65 -> **96.7 %** (85 -> 75 difflib opcodes).
All 27 section headers unchanged.

---

## 0. TL;DR

1. The original's dock1 locals are `int frameH`, `int total`, `int barW` (no
   `sxth`/`uxth` anywhere in the block). Wave 139 had settled on
   `unsigned short frameH`, `short total`, `unsigned short barW` as allocator
   tie-breaks. Recovering the three `int` spellings reproduces the original's
   dock1 register allocation almost exactly, including the `mov fp, r9` /
   `mov r9, r0` Canvas/textH preservation pair at `0x1d12` that every earlier
   `int` sweep had been missing.
2. `barH` stays `unsigned short` as the **size** tie-break: the faithful
   `int barH` build is 9324 B (8 B short). The `uxth` it emits is the cheapest
   way to keep the 9332-byte frame while the other three locals are faithful.
3. The dock2 `String text` / `halfW` order (the largest single hunk, 21 raw
   bytes at `0x2016`) is **not** landable as-is: the faithful String-first
   reshape gives 1352 raw but grows `.ARM.extab` by 4 B (`0x26298 ->
   0x2629c`), which pushes `.rodata` from `0x1c9e10` to `0x1c9e20` and drops
   `byte_exact` to 4044. Rejected on section-size grounds.
4. Reordering the damage-block anchor to `field_0x4b8 >= 1 && Globals::status`
   gives 1351 raw but grows the function to 9336 B. Rejected.

---

## 1. Landed change

```cpp
-            unsigned short frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
+            int frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
...
-            short total = ego->getDockTotalAmount();
+            int total = ego->getDockTotalAmount();
...
-            unsigned short barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
-            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
+            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
+            unsigned short barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
```

The original has no sign/zero extension at any of these sites:

```
1658b4: mov  r5,r0              ; total   (getDockTotalAmount, int)
165908: vmov s2,r0              ; frameH from [sp,#96] <-- no sxth/uxth
165942: mov  r8,r0              ; barW                 <-- no uxth
1659ca: vmov s0,r8 / vcvt.f32.s32 ; (float) barW      <-- signed convert
1659ec: add.w r2,r8,r8,lsr#31 / asrs / sub ...        ; barW/2 signed divide
165c10: ldrh.w r6,[sl,#994]     ; top = field_0x3e2  <-- zero-extend
```

`int frameH`/`int total`/`int barW` are therefore the faithful spellings, not
tie-breaks. `barH` is the one remaining tie-break; the original's `barH` is
also `int` (`mov fp,r0` with no `uxth`), but the `int` build is 9324 B.

### Why this fixes the dock1 allocation

With the three `int`s the dock1 block matches the original instruction for
instruction from `0x1cf0` to `0x1db2`, including the pair that all previous
`int` sweeps lost:

```
orig 1d10 mov r5,fp     ; ego -> r5
orig 1d12 mov fp,r9     ; Canvas -> fp
orig 1d14 mov r9,r0     ; textH -> r9
wave139 (short types)   ; only mov r5,fp + mov fp,r0, Canvas stays in r9
```

The remaining dock1 deltas are the `unsigned short barH` extend (4 B at
`0x1e62`), the `mov.w r1,r8,asr#1` vs `asrs r1,r4,#1` width (2 B), and the
missing `mov r5,fp` Canvas-preservation (2 B) — net +2 B, which is exactly
the dock2 shift.

---

## 2. Hunk census after the change

Raw same-address bytes by region (`regionbytes.py`):

| region | func offset | raw diff |
|---|---|---:|
| regen `pivotY` trace | `0x464` | 1 |
| gamma body | `0x4c2`-`0x4ee` | 5 |
| damage indicators | `0x5e6`-`0x7e0` | 481 |
| literal pool (damage) | `0x974`-`0x990` | 8 |
| dock1 (docking transfer bar) | `0x1cd4`-`0x1f44` | 316 |
| dock2 (jump-drive/cloak bar) | `0x1f44`-`0x2200` | 542 |
| EH cleanup pad | `0x242c`-`0x2440` | 7 |
| literal pool (tail) | `0x2450`-`0x2474` | 2 |
| **total** | | **1362** |

Wave-139 baseline was regen 1, gamma 5, damage 481, literal 8, dock1 413,
dock2 500, EH 2, tail 2 = 1416. The type recovery moves 97 bytes out of dock1
and the dock2 shift costs 42, net -54. The function's LSDA also improves
slightly: `.ARM.extab` differing bytes **1,395 -> 1,392** (size/base unchanged).

---

## 3. The dock2 `String text` / `halfW` order is EH-bound

The original constructs the jump-drive `String text` **before** `halfW`:

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
...
165be8: vcvt.s32.f32 s18,s0   ; halfW
```

On top of the landed `int` types, the faithful reshape

```cpp
String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
```

measures **1352 raw / 9332 B** — 10 bytes better than the landed variant — but
it grows `.ARM.extab` from `0x26298` to `0x2629c` (+4 B). `.rodata` is
16-byte aligned immediately after `.ARM.extab`, so it moves from `0x1c9e10`
to `0x1c9e20` (+16). Every PC-relative `.rodata` literal in the library then
changes address: `byte_exact` collapses **4507 -> 4044** and the ratchet fails.
This is a section-size regression, so the reshape is rejected.

The `.ARM.extab` growth is the LSDA call-site table for `Hud::draw`: moving
the `String` constructor changes the protected call-site ranges. The base
build's `.ARM.extab` is byte-exact, which is independent evidence that the
original's source order produces the same LSDA as the current
`halfW`-then-`String` order — the disassembly order alone is not enough.

Other dock2 permutations (all on the landed types):

| order | raw | size | `.ARM.extab` |
|---|---:|---:|---|
| `halfW, text, barH` (landed) | **1362** | **9332** | exact |
| `text, halfW, barH` | 1352 | 9332 | +4 |
| `text, barH, halfW` | 1375 | 9332 | +4 |
| `halfW, barH, text` | 1905 | 9328 | +4 |
| `barH, halfW, text` | 1906 | 9328 | +4 |

---

## 4. Damage-block anchor reorder (1351, but 9336 B)

The damage-block anchor can be reordered to match the original's
`ldr/cmp/blt` sequence position:

```cpp
-    if (Globals::status && this->field_0x4b8 >= 1) {
+    if (this->field_0x4b8 >= 1 && Globals::status) {
```

It emits `ldr.w r0,[sl,#1208]; cmp r0,#1; blt` first (matching the original)
and the status null test after, for **1351 raw**. But the reordered condition
changes the whole-function allocation: the function grows to **9336 B** and
`.text`/`.ARM.exidx`/`.ARM.extab`/`.rodata` all shift. With `int barH` it
returns to 9332 B but the raw diff jumps to 6080. Rejected; the landed
variant keeps the anchor in the wave-117 order.

---

## 5. Type sweep around the landed variant

All measured on top of the three `int`s (`barH` varied). Every variant below
keeps 9332 B and all section sizes exact.

| variant | raw diff |
|---|---:|
| `barH unsigned short` (landed) | **1362** |
| `barH short` | 1362 |
| `barH unsigned char` | 1362 |
| `barW long` | 1362 |
| `total long` | 1362 |
| `frameH long` | 1362 |
| `barW unsigned short` | 1371 |
| `barW short` | 1444 |
| `barH int` | 1967 (9324 B) |
| `barW unsigned int` | 1614 (9328 B) |

Declaration-order and `String text` placement sweeps in dock1 (text before
`progress`, text before `barOffset`, `textH` before `frameH`, `transferred`
before `textH`) are all worse (1409-3764 raw). The compiler already hoists
the `getText` + `String(" ")` temporary before the progress float math, so
the current statement order is the one that reproduces the original's call
order.

---

## 6. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w8517
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

Scratch harness in `/dev/shm/w8517`: `fdiff.py` (raw same-address diff),
`regionbytes.py` (raw diff per source region), `textdiff.py` (whole-`.text`
differing bytes), `matrixB.py`/`matrixC.py`/`matrixD.py` (type/order sweeps
with section-header checks), `Hud.base.cpp` (pristine wave-139 source),
`B.so`/`BI.so`/`BS.so`/`BSR.so` (measured variants), `orig.asm`/`B.asm`
(Thumb listings).
