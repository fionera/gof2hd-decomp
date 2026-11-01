# Removing the two `#pragma pack(push,1)` blocks in `Globals.cpp` (w3573)

Status: research note (2026-11-01). Worktree `wt/w3573`, branch `wave87-w3573`,
based on master `54e36581`. Every number below is from a controlled rebuild of this
tree against `../_work/bins/android_2.0.16_libgof2hdaa.so`.

The file is `src/game/core/Globals.cpp` (the task text says `src/engine/core/`; there is
no `src/engine/core/Globals.cpp` in this tree).

## TL;DR

The two pragma blocks are **removable with a source restructuring, and the removal is
gate-neutral**: `byte_exact` 4496 -> 4496, `linked_exact` 4496 -> 4496, all ELF
sections size/base exact, `lint CLEAN`, `drift 0`. The final `.so` differs from the
baseline `.so` only in the 20-byte GNU build-id (the debug-info type names changed).

A **naive deletion of the pragma lines does not compile** (20+ `static_assert`s fail),
and deleting only the `HintsBuffer` pragma changes codegen (`vst1.8` -> `vst1.64 :128`)
and regresses two byte-exact functions (4496 -> 4494). The pragmas are load-bearing in
the current source; the byte-array formulation below is a lint-clean equivalent.

Because the removal is **neutral**, not a strict improvement, only this note is
committed. The exact patch is in §6; apply it if the coordinator wants the source
cleanup.

## 1. Baseline

Commands:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

Baseline gate:

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Sections (unchanged by every neutral experiment below):

| section | address | size |
|---|---:|---:|
| `.text` | `0x69440` | `0x1337c8` |
| `.ARM.exidx` | `0x19cc08` | `0x6f70` |
| `.ARM.extab` | `0x1a3b78` | `0x26290` |
| `.rodata` | `0x1c9e10` | `0x41b67` |
| `.data.rel.ro` | `0x20cbb0` | `0x2fe8` |
| `.data` | `0x214000` | `0x296c` |
| `.bss` | `0x216970` | `0x69ac` |

Function rows relevant here (baseline `report.json`):

| function | pct | byte |
|---|---:|---|
| `_ZN7Globals10resetHintsEv` | 100.0 | yes |
| `_ZN7Globals4initEPN11AbyssEngine18ApplicationManagerEPNS0_6EngineE` | 100.0 | yes |
| `_ZN7GlobalsC1Ev` / `C2Ev` | 80.5 | no |

## 2. What the pragmas actually do

### Block 1 (`GameSettingsRecord` + friends, lines 8/90)

`GameSettingsRecord` is a view over `Globals::options[100]` (100 bytes, `alignas(4)`).
It has deliberately **misaligned** `int16_t`/`int32_t` fields at `0x0f`, `0x31`,
`0x35`, `0x3b`, `0x40`, `0x4c`, `0x60`. With `#pragma pack(1)` the layout is exactly
the original's; `alignas(4)` rounds `sizeof` to `0x64`, which fits the 100-byte
`options` array.

Deleting the pragma without restructuring gives the natural layout:

| field | packed | natural |
|---|---:|---:|
| `flagAt0f` | `0x0f` | `0x10` |
| `flagAt11` | `0x11` | `0x12` |
| `glowR` | `0x14` | `0x18` |
| `shortAt40` | `0x40` | `0x4a` |
| `shortAt4c` | `0x4c` | `0x58` |
| `shortAt60` | `0x60` | `0x6c` |
| `sizeof(GameSettingsRecord)` | `0x64` | `0x70` |

`sizeof` `0x70` would overrun `Globals::options[100]` by 12 bytes. `InitZeroObject`
also grows `0x3b` -> `0x3c`.

The original binary confirms the packed offsets directly. `Globals::init` (real
address `0xe3618`, `.text` base `0x69440`) contains:

```
e3666: 7466       strb    r6, [r4, #17]     ; flagAt11  @0x11
e3668: f884 6030  strb.w  r6, [r4, #48]     ; enableFlag30 @0x30
e368e: f8a4 300f  strh.w  r3, [r4, #15]     ; flagAt0f  @0x0f
```

`Globals::Globals` (`0xe31e0`) stores `intAt31`/`intAt35`/`intAt3b`/`shortAt40`/
`shortAt4c`/`shortAt60` at `[r1,#0x31]`/`[r1,#0x35]`/`[r1,#0x3b]`/`[r1,#0x40]`/
`[r1,#0x4c]`/`[r1,#0x60]` (`str`, `str`, `str`, `strh`, `strh`, `strh`).

### Block 2 (`Q16` / `HintsBuffer`, lines 335/346)

`HintsBuffer` is a 0x40-byte view over `Globals::hints[59]`. `Q16` is a 16-byte NEON
quad. With `#pragma pack(1)` `alignof(Q16) == 1`, so `hints->quad[0].v = z` is emitted
as an **unaligned** `vst1.8`; without it `alignof(Q16) == 16`, and clang emits
`vst1.64 {d16-d17}, [r2 :128]` (aligned hint).

`Globals::resetHints` is byte-exact in the baseline and is also inlined into
`Globals::init`, so this one alignment change regresses **two** functions.

Original `Globals::resetHints` (`0xe3938`, 44 bytes):

```
e3946: f942 0a01  vst1.8 {d16-d17}, [r2], r1
e3950: f942 0a0f  vst1.8 {d16-d17}, [r2]
e3954: f941 0a0f  vst1.8 {d16-d17}, [r1]
e3958: f940 0a0f  vst1.8 {d16-d17}, [r0]
```

Plain pragma deletion (Q16 natural alignment):

```
e3946: f942 0ae1  vst1.64 {d16-d17}, [r2 :128], r1
e3950: f942 0a0f  vst1.8  {d16-d17}, [r2]
e3954: f941 0aef  vst1.64 {d16-d17}, [r1 :128]
e3958: f940 0aef  vst1.64 {d16-d17}, [r0 :128]
```

## 3. Experiment table

All variants were built with the normal `check` target; no baseline was updated.

| # | variant | byte | linked | resetHints | Globals::init | Globals::Globals | verdict |
|---|---|---:|---:|---:|---:|---:|---|
| 0 | baseline | **4496** | **4496** | 100.0 | 100.0 | 80.5 | — |
| 1 | delete pragma lines only | build fails | — | — | — | — | 20+ `static_assert`s |
| 2 | remove block 2 only, plain (`Q16 { int32x4_t v; }`) | 4494 | 4494 | 80.0 | 99.0 | 80.5 | **regression** |
| 3 | remove block 2 only, byte-array `Q16` + direct cast | **4496** | **4496** | 100.0 | 100.0 | 80.5 | neutral |
| 4 | remove block 1 only, byte-array fields | **4496** | **4496** | 100.0 | 100.0 | 80.5 | neutral |
| 5 | remove both, byte-array fields | **4496** | **4496** | 100.0 | 100.0 | 80.5 | neutral |

Notes on the negative variants:

* **#1**: deleting the four pragma lines leaves `GameSettingsRecord` at natural
  offsets; the `static_assert`s for `flagAt0f` (0x0f), `flagAt11` (0x11), `glowR`
  (0x14), `shortAt40` (0x40), `shortAt4c` (0x4c), `shortAt60` (0x60) and
  `sizeof(InitZeroObject) == 0x3b` all fail.
* **#2**: the only codegen change is the Q16 alignment hint above; `resetHints` drops
  to 80.0%, and its inlined copy in `Globals::init` drops `init` to 99.0%.
* **#3/#4/#5**: the final `.so` is identical to the baseline `.so` in every byte
  except the 20-byte GNU build-id (`0x1dd..0x1f0`). All ELF section sizes/bases are
  unchanged, `lint` is `CLEAN`, `drift` is 0, and `sodiff` reports the same 207
  imports.

Two additional block-2 formulations were tried and rejected:

| formulation | result |
|---|---|
| helper `static inline int32x4_t &q16(Q16 &q)` | helper return loses the byte-array alignment; clang emits `vst1.64 :128` again, byte 4494 |
| `vst1q_u8` intrinsic | `resetHints` shrinks 44 -> 40 bytes (`vst1.8 [r1]!` post-increments by 16, not 0x2b), which cascades through `.text`; byte 3383 |

## 4. Proof of exactness for the neutral variant

`Globals::resetHints` bytes in the original and in variant #5 are identical:

```
orig 0948c0ef50002b2178440068024642f9010a00f12001103042f90f0a41f90f0a40f90f0a704700bf4cc91200
ours 0948c0ef50002b2178440068024642f9010a00f12001103042f90f0a41f90f0a40f90f0a704700bf4cc91200
```

The whole stripped `.so` comparison for variant #5 vs baseline:

```sh
cmp -l /tmp/baseline.so /tmp/both_removed.so   # 20 differing bytes
# all at file offsets 0x1dd..0x1f0 = the 20-byte .note.gnu.build-id hash
```

The build-id changes because the link-time (pre-strip) debug info now names
`uint8_t[2]`/`uint8_t[4]`/`uint8_t[16]` instead of `int16_t`/`int32_t`/`int32x4_t`.
No shipped section changes.

## 5. Conclusion

* The binary is consistent with the current packed layout: the original emits
  unaligned `vst1.8` NEON stores and separate `str` stores at odd offsets.
* `#pragma pack` is not uniquely required, but **a naive deletion is wrong**. The
  byte-array view below reproduces the original codegen exactly.
* The removal is **gate-neutral** (4496/4496), not a strict improvement. Per the
  task rule, only this research note is committed; the patch is ready in §6.

## 6. Exact patch (neutral; not committed)

This is the full `diff -u` of the working tree against the baseline. It applies
cleanly to `54e36581` and produces variant #5 above.

```diff
--- a/src/game/core/Globals.cpp
+++ b/src/game/core/Globals.cpp
@@ -5,7 +5,6 @@
 #include <cstdint>
 
 namespace {
-#pragma pack(push, 1)
 
     struct alignas(4) GameSettingsRecord {
         float colorR;
@@ -23,7 +22,7 @@
         //   flagAt10     int16  @0x10 -> byte-offset 0x10 cast
         //   flagAt11     uint8  @0x11 -> flagAt11
         uint8_t _region[3];
-        int16_t flagAt0f;
+        uint8_t flagAt0f[2];
         uint8_t flagAt11;
         uint8_t _pad12[2];
 
@@ -35,33 +34,33 @@
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
 
@@ -77,8 +76,11 @@
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
@@ -87,8 +89,6 @@
         uint8_t flag;
     };
 
-#pragma pack(pop)
-
 #if __SIZEOF_POINTER__ == 4
 #include <cstddef>
     static_assert(offsetof(GameSettingsRecord, colorR) == 0x00, "colorR");
@@ -332,18 +332,16 @@
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
@@ -671,18 +669,18 @@
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
@@ -712,7 +710,7 @@
     Globals::instantActionWave = 0;
     Globals::lastCampaignMissionFailed = -1;
     this->soundResources = 0;
-    settings->shortAt60 = 0x100;
+    reinterpret_cast<int16_t &>(settings->shortAt60) = 0x100;
     Globals::lastCampaignMissionFailCount = 0;
 }
 Globals::~Globals() {
@@ -782,7 +780,7 @@
     char *zeroByte = (char *) &Globals::startLiteVersionWithMoreCredits;
     GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
 
-    settings->flagAt0f = 0x101;
+    reinterpret_cast<int16_t &>(settings->flagAt0f) = 0x101;
     settings->flagAt11 = 1;
     settings->glowR = 1.0f;
     settings->glowG = 1.0f;
@@ -866,10 +864,10 @@
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

# variant #5: apply the patch in §6, then
cmake --build cmake-build-match --target check -j8   # 4496/4496
cmake --build cmake-build-match --target lint  -j8   # CLEAN
cmake --build cmake-build-match --target drift -j8   # 0

# inspect a function
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xe3938 --stop-address=0xe3964 ../_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d --start-address=0xe3938 --stop-address=0xe3964 cmake-build-match/libgof2hdaa.so
```
