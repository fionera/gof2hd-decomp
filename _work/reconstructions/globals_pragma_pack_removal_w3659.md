# Removing the two `#pragma pack(push,1)` blocks in `Globals.cpp` (w3659)

Status: research note (2026-11-02). Worktree `wt/w3659`, branch `wave87-w3659`,
based on `a8dc3d35`. Every number below was measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` by a controlled rebuild of this tree.

The file is `src/game/core/Globals.cpp`. This note independently reproduces and
extends `globals_pragma_pack_removal_w3573.md` on the current tree.

## TL;DR

Both `#pragma pack(push, 1)` blocks can be removed by replacing the deliberately
misaligned typed fields with byte arrays and casting at the use sites. The removal is
**gate-neutral**: `byte_exact` 4496 -> 4496, `linked_exact` 4496 -> 4496, `.text`
`0x1337c8`, lint `CLEAN`, drift `0`. The stripped `.so` differs from the baseline only
in the 20-byte GNU build-id hash (the debug-info type names changed).

A naive deletion of the pragma lines does **not** compile (20 `static_assert`s fail).
Removing only the `HintsBuffer` pragma while keeping `Q16 { int32x4_t v; }` regresses
two byte-exact functions (4496 -> 4494) because clang changes the `vst1.8` store to an
aligned `vst1.64 :128` store.

Because the removal is neutral, not a strict gate improvement, only this note is
committed; the exact source patch is in §6 and is ready to apply.

## 1. Baseline on `a8dc3d35`

Commands:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

Baseline gate:

```text
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Section sizes/bases (unchanged by the neutral experiment):

| section | address | size |
|---|---:|---:|
| `.text` | `0x69440` | `0x1337c8` |
| `.ARM.exidx` | `0x19cc08` | `0x6f70` |
| `.ARM.extab` | `0x1a3b78` | `0x26290` |
| `.rodata` | `0x1c9e10` | `0x41b67` |
| `.data.rel.ro` | `0x20cbb0` | `0x2fe8` |
| `.data` | `0x214000` | `0x296c` |
| `.bss` | `0x216970` | `0x69ac` |

## 2. What the original binary proves

The original uses the packed data layout, but the binary cannot by itself distinguish
`#pragma pack` from explicit byte arrays/padding. It does prove the resulting offsets
and the unaligned NEON store.

`Globals::Globals` (`0xe31e0`) stores the misaligned settings fields at odd offsets:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xe31e0 --stop-address=0xe3444 \
  ../_work/bins/android_2.0.16_libgof2hdaa.so
```

Relevant original stores include:

```text
e32c2: f881 2039  strb.w r2, [r1, #57]   ; flagAt39 @0x39
e32c6: f8c1 2035  str.w  r2, [r1, #53]   ; intAt35  @0x35
e32ca: f8c1 2031  str.w  r2, [r1, #49]   ; intAt31  @0x31
e32d2: f881 203f  strb.w r2, [r1, #63]   ; flagAt3f @0x3f
e32d6: f8c1 203b  str.w  r2, [r1, #59]   ; intAt3b  @0x3b
e32da: f881 204e  strb.w r2, [r1, #78]   ; flagAt4e @0x4e
e32de: f8a1 204c  strh.w r2, [r1, #76]   ; shortAt4c @0x4c
```

`Globals::resetHints` (`0xe3938`) uses unaligned `vst1.8` stores:

```sh
$OD -d --start-address=0xe3938 --stop-address=0xe3964 \
  ../_work/bins/android_2.0.16_libgof2hdaa.so
```

```text
e3946: f942 0a01  vst1.8 {d16-d17}, [r2], r1
e3950: f942 0a0f  vst1.8 {d16-d17}, [r2]
e3954: f941 0a0f  vst1.8 {d16-d17}, [r1]
e3958: f940 0a0f  vst1.8 {d16-d17}, [r0]
```

## 3. Controlled A/B table

All variants were built with the normal `check` target; the baseline was never
updated.

| # | variant | byte | linked | `.text` | lint | drift | verdict |
|---|---|---:|---:|---:|---|---|---|
| 0 | baseline | **4496** | **4496** | `0x1337c8` | CLEAN | 0 | reference |
| 1 | delete the four pragma lines only | build fails | — | — | — | — | 20 `static_assert`s |
| 2 | remove block 2 only, keep `Q16 { int32x4_t v; }` | 4494 | 4494 | unchanged | — | — | **regression** |
| 3 | remove both blocks, byte-array views | **4496** | **4496** | `0x1337c8` | CLEAN | 0 | neutral |

Variant 1 output ends with:

```text
fatal error: too many errors emitted, stopping now [-ferror-limit=]
20 errors generated.
ninja: build stopped: subcommand failed.
```

Variant 2 output ends with:

```text
verify: byte 4494  linked 4494  stubs 0  missing 0  extra 0  avg 99.89
  FAIL  byte_exact 4496 -> 4494
  FAIL  linked_exact 4496 -> 4494 (beyond the +-1 band)
RATCHET FAIL
```

Variant 3 output ends with:

```text
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 4. Proof that variant 3 is neutral

The stripped `.so` comparison against the baseline has exactly 20 differing bytes,
contiguous at file offsets `0x1dc..0x1ef`:

```sh
cmp -l /tmp/w3659_baseline.so cmake-build-match/libgof2hdaa.so
# 20 lines, offsets 0x1dc..0x1ef
readelf -SW cmake-build-match/libgof2hdaa.so | grep '.note.gnu.build-id'
# [ 2] .note.gnu.build-id NOTE 000001cc 000001cc 000024 ...
```

Those are the 20 hash bytes of `.note.gnu.build-id` (section starts at `0x1cc`;
the hash payload starts at `0x1dc`). No shipped code/data section changes.

`Globals::resetHints` is byte-identical in the original and variant 3:

```text
orig 0948c0ef50002b2178440068024642f9010a00f12001103042f90f0a41f90f0a40f90f0a704700bf4cc91200
ours 0948c0ef50002b2178440068024642f9010a00f12001103042f90f0a41f90f0a40f90f0a704700bf4cc91200
```

## 5. Why the byte-array form is needed

* `GameSettingsRecord` is a 100-byte view over `Globals::options[100]`. Its
  `int16_t`/`int32_t` fields intentionally sit at odd offsets. Natural alignment
  moves them (`flagAt0f` `0x0f -> 0x10`, `shortAt40` `0x40 -> 0x4a`, etc.) and makes
  `sizeof(GameSettingsRecord)` `0x70` instead of `0x64`.
* `InitZeroObject` must stay `0x3b` bytes. A natural `int32_t slots[12]` member rounds
  the struct to `0x3c`.
* `CtorSecondaryObject` must keep alignment 1. With natural `int32_t` members clang
  merges `slot0`/`slot1` zero stores into one `strd`; the original emits two separate
  `str` instructions.
* `Q16` must keep alignment 1. With natural `int32x4_t` alignment, clang emits
  `vst1.64 {d16-d17}, [r2 :128]` instead of the original unaligned `vst1.8`.

The byte-array views preserve all four properties without any pragma.

## 6. Exact patch (neutral; not committed)

```diff
diff --git a/src/game/core/Globals.cpp b/src/game/core/Globals.cpp
index c6cf953f..07594d25 100644
--- a/src/game/core/Globals.cpp
+++ b/src/game/core/Globals.cpp
@@ -5,7 +5,6 @@
 #include <cstdint>
 
 namespace {
-#pragma pack(push, 1)
 
     struct alignas(4) GameSettingsRecord {
         float colorR;
@@ -23,7 +22,7 @@ namespace {
         //   flagAt10     int16  @0x10 -> byte-offset 0x10 cast
         //   flagAt11     uint8  @0x11 -> flagAt11
         uint8_t _region[3];
-        int16_t flagAt0f;
+        uint8_t flagAt0f[2];
         uint8_t flagAt11;
         uint8_t _pad12[2];
 
@@ -35,33 +34,33 @@ namespace {
         float brightness;
         float contrast;
         uint8_t enableFlag30;
-        int32_t intAt31;
-        int32_t intAt35;
+        uint8_t intAt31[4];
+        uint8_t intAt35[4];
         uint8_t flagAt39;
         uint8_t _pad3a;
-        int32_t intAt3b;
+        uint8_t intAt3b[4];
         uint8_t flagAt3f;
-        int16_t shortAt40;
+        uint8_t shortAt40[2];
         uint8_t _pad42[2];
         float qualityLevel;
         int32_t intAt48;
-        int16_t shortAt4c;
+        uint8_t shortAt4c[2];
         uint8_t flagAt4e;
         uint8_t _pad4f;
         int32_t intAt50;
         int32_t resWidth;
         int32_t resHeight;
         uint8_t _pad5c[4];
-        int16_t shortAt60;
+        uint8_t shortAt60[2];
     };
 
     struct InitZeroObject {
-        // Region 0x00..0x3a. Canonical storage is the aligned int32 slot array
-        // (slots[0..11] cover 0x00..0x2f); the misaligned tail ints overlap the
-        // tail of that region and the trailing bytes, accessed via byte-offset
-        // reinterpret_cast over slots:
+        // Region 0x00..0x3a. Byte storage keeps the struct's alignment at 1 so
+        // sizeof stays 0x3b (an int32_t[12] member would round it to 0x3c).
+        // The misaligned tail ints overlap the region, accessed via byte-offset
+        // reinterpret_cast:
         //   tailAt2b int32 @0x2b, tailAt2f @0x2f, tailAt33 @0x33, tailAt37 @0x37
-        int32_t slots[12];
+        uint8_t slots[0x30];
         uint8_t _tail_0x30[0x0b];
     };
 
@@ -77,8 +76,11 @@ namespace {
     };
 
     struct CtorSecondaryObject {
-        int32_t slot0;
-        int32_t slot1;
+        // Byte storage keeps the struct's alignment at 1: with a natural
+        // int32_t member clang merges the adjacent slot0/slot1 zero stores into
+        // one strd, while the original emits two separate str instructions.
+        uint8_t slot0[4];
+        uint8_t slot1[4];
         uint8_t _pad08[0x0b];
         uint8_t flagAt13;
     };
@@ -87,8 +89,6 @@ namespace {
         uint8_t flag;
     };
 
-#pragma pack(pop)
-
 #if __SIZEOF_POINTER__ == 4
 #include <cstddef>
     static_assert(offsetof(GameSettingsRecord, colorR) == 0x00, "colorR");
@@ -332,18 +332,16 @@ unsigned char Globals::iap_hack_dlc3Bought;
 #include <new>
 
 
-#pragma pack(push, 1)
 struct Q16 {
-    int32x4_t v;
+    uint8_t v[16];
 };
 
 struct HintsBuffer {
-    // Region 0x00..0x3f. Canonical storage is the aligned NEON-quad array
+    // Region 0x00..0x3f. Canonical storage is the NEON-quad array
     // (quad[0..3] at 0x00/0x10/0x20/0x30); the misaligned quadAt2b (Q16 @0x2b)
     // overlaps quad[2]/quad[3] and is accessed via byte-offset reinterpret_cast.
     Q16 quad[4];
 };
-#pragma pack(pop)
 
 #if __SIZEOF_POINTER__ == 4
 #include <cstddef>
@@ -671,18 +669,18 @@ Globals::Globals() {
     settings->contrast = 0.5f;
     settings->_region[2] = 1;
     settings->enableFlag30 = 1;
-    settings->shortAt40 = 0;
+    reinterpret_cast<int16_t &>(settings->shortAt40) = 0;
     settings->intAt50 = 0;
     settings->flagAt39 = 0;
-    settings->intAt35 = 0;
-    settings->intAt31 = 0;
-    settings->intAt3b = 0;
+    reinterpret_cast<int32_t &>(settings->intAt35) = 0;
+    reinterpret_cast<int32_t &>(settings->intAt31) = 0;
+    reinterpret_cast<int32_t &>(settings->intAt3b) = 0;
     settings->flagAt4e = 0;
-    settings->shortAt4c = 0;
+    reinterpret_cast<int16_t &>(settings->shortAt4c) = 0;
     settings->intAt48 = 0;
-    secondary->slot1 = 0;
+    reinterpret_cast<int32_t &>(secondary->slot1) = 0;
     secondary->flagAt13 = 0;
-    secondary->slot0 = 0;
+    reinterpret_cast<int32_t &>(secondary->slot0) = 0;
     settings->flagAt3f = 0;
 
     Globals::lastSpaceMusicPlayed = -1;
@@ -712,7 +710,7 @@ Globals::Globals() {
     Globals::instantActionWave = 0;
     Globals::lastCampaignMissionFailed = -1;
     this->soundResources = 0;
-    settings->shortAt60 = 0x100;
+    reinterpret_cast<int16_t &>(settings->shortAt60) = 0x100;
     Globals::lastCampaignMissionFailCount = 0;
 }
 Globals::~Globals() {
@@ -782,7 +780,7 @@ void Globals::init(AbyssEngine::ApplicationManager *app, AbyssEngine::Engine *en
     char *zeroByte = (char *) &Globals::startLiteVersionWithMoreCredits;
     GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
 
-    settings->flagAt0f = 0x101;
+    reinterpret_cast<int16_t &>(settings->flagAt0f) = 0x101;
     settings->flagAt11 = 1;
     settings->glowR = 1.0f;
     settings->glowG = 1.0f;
@@ -866,10 +864,10 @@ void Globals::init(AbyssEngine::ApplicationManager *app, AbyssEngine::Engine *en
 void Globals::resetHints() {
     HintsBuffer *hints = (HintsBuffer *) Globals::hints;
     const int32x4_t z = vdupq_n_s32(0);
-    hints->quad[0].v = z;
-    reinterpret_cast<Q16 &>(reinterpret_cast<uint8_t *>(hints->quad)[0x2b]).v = z;
-    hints->quad[2].v = z;
-    hints->quad[1].v = z;
+    reinterpret_cast<int32x4_t &>(hints->quad[0].v) = z;
+    reinterpret_cast<int32x4_t &>(reinterpret_cast<uint8_t *>(hints->quad)[0x2b]) = z;
+    reinterpret_cast<int32x4_t &>(hints->quad[2].v) = z;
+    reinterpret_cast<int32x4_t &>(hints->quad[1].v) = z;
 }
 
 void Globals::longToTimeString(long long ms, String &out) {
```

## 7. Reproduction

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8   # baseline 4496/4496

# apply the patch in §6, then:
cmake --build cmake-build-match --target check -j8   # 4496/4496
cmake --build cmake-build-match --target lint  -j8   # CLEAN
cmake --build cmake-build-match --target drift -j8   # 0
```

## 8. Conclusion

* Proven: both pragma blocks are removable with the byte-array restructuring, and the
  result is gate-neutral (4496/4496, all section sizes/bases unchanged, lint CLEAN,
  drift 0).
* Proven: a naive pragma deletion is invalid, and removing only the `HintsBuffer`
  pragma without changing `Q16` regresses `byte_exact`/`linked_exact` by 2.
* Not proven: whether the original source used `#pragma pack`, explicit padding, or
  byte arrays. The binary only fixes the layout and the unaligned store, not the
  source spelling.
* No source change is committed because the experiment is neutral, not a strict
  improvement.
