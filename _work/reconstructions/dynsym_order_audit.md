# Dynamic-symbol order audit (TU membership)

**Worker:** research-w3307
**Repo:** `/home/debian/GalaxyOnFire2/wt/w3307`
**Branch:** `research-w3307` (based on `master` HEAD `9b46ec31`)
**Date:** 2026-11-01
**Deliverable:** this note only; `src/` and `CMakeLists.txt` are clean in the committed tree.
All experiments below were made in the worktree and reverted before commit.

## TL;DR

* Both `.dynsym` tables contain exactly **5370 defined symbols** (same set, no duplicates).
* The baseline alignment is **77 non-empty matching blocks** (78 including the sentinel), difflib ratio **0.970577**, **5212** matched
  symbols, **316** unmatched symbol occurrences (158 on each side), and only **805/5370** symbols in
  the same absolute position. There are **76 out-of-order gaps**.
* The order is *not* a pure "TU membership" fingerprint. Gold inserts a symbol into the dynamic
  symbol table at the first point it is **defined or referenced**, so some single-symbol moves do
  not change the dynsym position at all (measured: `g_android_back_button_pressed`). The long
  matching blocks are still a very good TU/link-order fingerprint; the small gaps are often
  first-reference or alias-ordering artifacts.
* Two low-risk experiments were measured:
  1. **`F` move** (`VirtualInput.cpp` -> `BlowfishSubkeyFile.cpp`): gate unchanged
     (byte 1458, linked 4493), dynsym matched **5212 -> 5214**, ratio **0.970577 -> 0.970950**,
     and the entire 8-symbol Blowfish block becomes exact.
  2. **Archive link order** (`${_openssl_out}` after `${_zlib_out} ${_libzip_out}`): gate unchanged,
     dynsym matched **5214 -> 5228**, ratio **0.970950 -> 0.973557**, unmatched **312 -> 284**,
     same-position **805 -> 807**. This fixes the 20-symbol SHA/OpenSSL block.
* The biggest *TU-membership* item is the **GameText split** (`GameText.cpp` vs a missing
  `GameText_iPhone.cpp`), followed by the **`AbyssEngine.cpp` mega-TU split** and the
  **Android/JNI `main.c` source order**.

## Ground truth and method

Original: `../_work/bins/android_2.0.16_libgof2hdaa.so` (stripped, `.dynsym` only).
Ours: `cmake-build-match/libgof2hdaa.so` after `cmake --build cmake-build-match --target check -j32`.

A small ELF32 parser was used to dump `.dynsym` in file order, preserving `st_info` (bind/type),
`st_shndx`, `st_value`, and `st_size`:

```bash
python3 /tmp/dynsym_dump.py ../_work/bins/android_2.0.16_libgof2hdaa.so > /tmp/orig_dynsym.tsv
python3 /tmp/dynsym_dump.py cmake-build-match/libgof2hdaa.so        > /tmp/ours_dynsym.tsv
```

The parser emits `bind<TAB>type<TAB>shndx<TAB>value<TAB>size<TAB>name`. The alignment used only
defined symbols (`st_shndx != 0`), which is exactly the 5370-symbol set:

```python
o = [name for defined symbols in original order]
u = [name for defined symbols in ours order]
sm = difflib.SequenceMatcher(None, o, u, autojunk=False)
sm.get_matching_blocks()   # maximal matching blocks
```

For each symbol, our owning object was resolved with the NDK `nm` over every
`cmake-build-match/CMakeFiles/gof2.dir/**/*.obj`; for weak symbols the first object in
`cmake-build-match/link_order.rsp` was used (that is the copy gold keeps). The original TU was
inferred from `_work/original_layout/tu_mapping.md`, `_work/original_layout/original_layout.json`
(the per-TU symbol dumps), and the original symbol's adjacency/address.

### Reproduction commands

```bash
cmake --preset default
cmake --build cmake-build-match --target check -j32
# then the two dump/alignment snippets above
```

The gate at the audited HEAD prints:

```
verify: byte 1458  linked 4493  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`report.json` has `size_exact = 4522/4523`; our `.text` is `0x1343d0`, the original's is
`0x1337c8`. (The task brief's "byte 1415" is stale for this HEAD; the committed baseline is 1458.)

### Reproduction scripts (self-contained)

`dynsym_dump.py`:

```python
import struct, sys
from pathlib import Path
SHT_DYNSYM = 11
def read(path):
    data = Path(path).read_bytes()
    assert data[:4] == b"\x7fELF" and data[4] == 1 and data[5] == 1
    e_shoff = struct.unpack_from("<I", data, 0x20)[0]
    e_shentsize = struct.unpack_from("<H", data, 0x2E)[0]
    e_shnum = struct.unpack_from("<H", data, 0x30)[0]
    secs = []
    for i in range(e_shnum):
        o = e_shoff + i * e_shentsize
        name, typ, flags, addr, off, size, link, info, align, ent = \
            struct.unpack_from("<IIIIIIIIII", data, o)
        secs.append(dict(typ=typ, off=off, size=size, link=link, ent=ent))
    ds = next(s for s in secs if s["typ"] == SHT_DYNSYM)
    st = secs[ds["link"]]
    strtab = data[st["off"]:st["off"] + st["size"]]
    def cstr(i):
        e = strtab.find(b"\0", i)
        return strtab[i:e].decode("utf-8", "replace")
    out = []
    for o in range(ds["off"], ds["off"] + ds["size"], ds["ent"] or 16):
        nm, val, sz, info, other, shndx = struct.unpack_from("<IIIBBH", data, o)
        out.append((info >> 4, info & 0xF, shndx, val, sz, cstr(nm)))
    return out
for bind, typ, shndx, val, sz, name in read(sys.argv[1]):
    print(f"{bind}\t{typ}\t{shndx}\t{val:08x}\t{sz}\t{name}")
```

Alignment / gap extraction:

```python
import difflib
def defined(path):
    rows = []
    for line in open(path):
        bind, typ, shndx, val, sz, name = line.rstrip("\n").split("\t")
        if int(shndx) != 0:
            rows.append(name)
    return rows
o = defined("/tmp/orig_dynsym.tsv")
u = defined("/tmp/ours_dynsym.tsv")
sm = difflib.SequenceMatcher(None, o, u, autojunk=False)
print(sm.ratio(), len(sm.get_matching_blocks()))
prev_o = prev_u = 0
for b in sm.get_matching_blocks():
    if b.a > prev_o or b.b > prev_u:
        print("GAP", prev_o, b.a, o[prev_o:b.a], prev_u, b.b, u[prev_u:b.b])
    prev_o, prev_u = b.a + b.size, b.b + b.size
```

Owner resolution (weak symbols: first object in `link_order.rsp`):

```python
import glob, json, os, subprocess
NM = ".cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/" \
     "prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm"
m = {}
for obj in glob.glob("cmake-build-match/CMakeFiles/gof2.dir/**/*.obj", recursive=True):
    r = subprocess.run([NM, "--defined-only", obj], capture_output=True, text=True)
    for line in r.stdout.splitlines():
        p = line.split()
        if len(p) == 3:
            m.setdefault(p[2], []).append((obj, p[1]))
order = [os.path.normpath(os.path.abspath(x.strip()))
         for x in open("cmake-build-match/link_order.rsp")]
pos = {x: i for i, x in enumerate(order)}
def owner(name):
    v = m.get(name)
    if not v:
        return "?"
    best = min(v, key=lambda ot: pos.get(os.path.normpath(os.path.abspath(ot[0])), 10**9))
    return os.path.basename(best[0]).replace(".obj", "") + ("(W)" if best[1] == "W" else "")
```

## 1. Aligned diff

### 1.1 Matching blocks

The baseline has **78** blocks from `difflib` (77 non-empty maximal matches plus the final
zero-length sentinel). The full list is in [Appendix A](#appendix-a-matching-blocks). The largest blocks are:

| block | orig defined | ours defined | size | first symbol |
|---|---|---|---|---|
| 28 | 1274:1825 | 1279:1830 | 551 | `AE::BumpRimCubeShader::ShaderIndex` |
| 35 | 2189:2709 | 2188:2708 | 520 | `SpacePoint::take()` |
| 45 | 3273:3613 | 3268:3608 | 340 | `void ArrayRelease<bool>(Array<bool>&)` |
| 20 | 692:1012 | 698:1018 | 320 | `void ArrayRemoveAll<AE::CheatCode*>(...)` |
| 34 | 1910:2188 | 1910:2188 | 278 | `void ArraySetLength<Item*>(...)` |
| 16 | 406:664 | 415:673 | 258 | `AE::String::GetAEChar() const` |
| 52 | 4562:4753 | 4562:4753 | 191 | `SpacePoint::~SpacePoint()` |
| 48 | 3882:4062 | 3878:4058 | 180 | `void ArrayRemove<Node*>(...)` |
| 51 | 4390:4562 | 4389:4561 | 172 | `SolarSystem::~SolarSystem()` |
| 36 | 2710:2873 | 2708:2871 | 163 | `Item::setUnsaleable(bool)` |

### 1.2 Out-of-order blocks

There are **76** gaps, covering **158** original symbols and **158** our symbols. The full table
with our owner file and the implied original TU/action is in
[Appendix B](#appendix-b-out-of-order-blocks). The gaps are not all the same kind:

| category | gaps | O-side symbols | what it means |
|---|---|---|---|
| Android/JNI glue (`main.c`, `NFC.cpp`, `NdkHooks.cpp`) | 0, 1, 8, 9, 10 | 1+1+0+0+17 = **19** | mostly `main.c` source order; `g_android_back_button_pressed` is a real TU move |
| Blowfish | 2, 6, 7 | 0+1+1 = **2** | `F` defined in `VirtualInput.cpp` instead of `BlowfishSubkeyFile.cpp` |
| VirtualInput/LODManager globals | 3, 4, 5, 29-33 | 0+0+0+1+1+2+3+0 = **7** | source declaration order within the same TU |
| `AbyssEngine.cpp` mega-TU | 11-19, 26, 27 | 1+1+0+1+1+2+1+0+3+1+1 = **12** | engine free functions in the wrong TU / wrong order |
| GameText | 20-24 | 0+0+0+0+13 = **13** | missing `GameText_iPhone.cpp` split + source order |
| `GetEngine` | 25, 28 | 0+1 = **1** | gold first-reference insertion, not TU membership |
| destructor aliases (`D1`/`D2`) | 34-41, 45, 48-54 | 1+1+0+1+1+1+1+1+0+0+0+0+0+0+1+0 = **8** | alias ordering; original emits `D1` early with `C1` |
| PlayerFighter/Agent | 42-44 | 1+1+0 = **2** | source order / early reference |
| ResourceTexture | 46, 47 | 1+0 = **1** | ctor in `ResourceMaterial.cpp` instead of the ResourceTexture TU |
| zlib/OpenSSL/libzip | 55-74 | 20+16+0+2+0+4+1+9+17+0+0+0+0+17+5+0+0+1+0+20 = **92** | archive-member / link order, not TU membership |
| libc++ | 75 | 1 | runtime symbol ordering |

(The O-side counts sum to 158; the table above groups the paired O/U gaps.)

## 2. TU analysis and migration plan

Ordered by the number of symbols the move can fix. "Symbols" means unique defined symbols in the
original out-of-order block; paired gaps are counted once.

### 2.1 Link-order fix: OpenSSL after zlib/libzip (20 symbols, measured)

The original's tail order is `zlib` -> `libzip` -> `OpenSSL`/`libc++`. Ours is
`OpenSSL` -> `zlib` -> `libzip`. The 20-symbol SHA/OpenSSL block (gaps 55/74) is entirely a link
order artifact.

**Change (CMakeLists.txt, `link_so` command):**

```diff
-            ${_openssl_out}
-            --whole-archive ${_zlib_out} ${_libzip_out} --no-whole-archive
+            --whole-archive ${_zlib_out} ${_libzip_out} --no-whole-archive
+            ${_openssl_out}
```

**Measured:** gate unchanged (byte 1458, linked 4493, data 843/843, lint CLEAN); dynsym
matched **5214 -> 5228**, ratio **0.970950 -> 0.973557**, unmatched **312 -> 284**,
same-position **805 -> 807**. This is the single best low-risk move found.

The remaining zlib/libzip gaps (56-73) are archive-member order inside `libz.a`/`libzip.a`
(`inflate.o`, `inffast.o`, `inftrees.o`, `crc32.o`, `zutil.o`, and the libzip objects). They need
the vendored archive members reordered to the original's per-source order; that is a build-system
change, not a `src/` TU move.

### 2.2 GameText split (13 symbols, not yet measured)

The source dumps have **two** GameText TUs:

* `Svn_iPhone_AE2/Classes/GameText.cpp`: `GameText::GameText()`, `getText(int)`, `release()`,
  `~GameText()`, `currentLang`.
* `Svn_iPhone_AE2/Classes/GameText_iPhone.mm`: `ReadLangFile(unsigned int, int)`,
  `setLanguage(short, int)`, `setLanguage(int)`.

Our `src/engine/core/GameText.cpp` merges both. The original's dynsym confirms the split: the
`GameText_iPhone` methods (`setLanguage`, `ReadLangFile`, `getRegionCode`,
`convertStringFromArabic`, `release`, `currentLang`) appear in one block around original `.dynsym`
index 1084, while the `GameText.cpp` block (`Array<int>` instantiations, `setSubstituteArray`,
`getText`, ctor/dtor, `__cxa_guard_*`) appears around original `.dynsym` index 1108. Our single TU
emits them in one run, which is exactly gaps 20-24.

**Plan:**

1. Create `src/engine/core/GameText_iPhone.cpp` (platform analog; the original is an `.mm` file).
2. Move `setLanguage(int)`, `setLanguage(short, int)`, `ReadLangFile(unsigned int, int)`,
   `getRegionCode()`, `convertStringFromArabic(String)`, `release()`, and the `currentLang`
   definition there.
3. Keep `GameText::GameText()`, `~GameText()`, `getText(int)`, `setSubstituteArray(int*, unsigned)`,
   and the `Array<int>` instantiations in `GameText.cpp`.
4. Reorder the remaining `GameText.cpp` symbols so the `Array<int>` instantiations come first,
   then `setSubstituteArray`, `getText`, ctor/dtor, `__cxa_guard_*` (original order).

This is the largest genuine TU-membership fix and should be done under the full gate. It may also
change `.rodata`/string-literal layout because the two TUs had separate string pools in the
original.

### 2.3 `AbyssEngine.cpp` mega-TU split (12 symbols)

`src/engine/core/AbyssEngine.cpp` is the known invented kitchen-sink TU. The original free
functions have their own TUs. Concrete moves, with evidence:

| symbol | current | destination | evidence |
|---|---|---|---|
| `esMatrixMultiply` | `AbyssEngine.cpp` | `engine/render/Engine.cpp` | dump: `Svn_iPhone_AE2/Classes/Engine.cpp` |
| `getAppVersion` | `AbyssEngine.cpp` | `engine/render/Engine.cpp` (likely) | original position immediately before `Engine::ShaderInit` |
| `SetFXMaterial` | `AbyssEngine.cpp` | `engine/render/Engine.cpp` or `PaintCanvas.cpp` | original position near shader/engine block |
| `GetStringLength(String)` | `AbyssEngine.cpp` | `game/core/String.cpp` or `FileInterfaceAndroid.cpp` | original position before `loge`/`logi`/`String` methods |
| `AELabelObject` | `AbyssEngine.cpp` | `engine/render/ImageFont.cpp` (likely) | original position before `PaintCanvas::Initialize`; no dump witness |
| `CurveCreate`/`CurveRelease`/`CurveGetValue` | `AbyssEngine.cpp` | new `Curve.cpp` (no witness) | original block at 689-691; cannot fix without inventing a TU |
| `MatrixRotateVector`/`MatrixTransformVector` | `Matrix.cpp` | `Matrix.cpp` (already correct) | dump: `Svn_iPhone_AE2/Classes/Matrix.cpp`; only source order differs |
| `BSphere::Merge(Transform)` | `BSphere.cpp` | `engine/render/Mesh.cpp` | `tu_mapping.md`: BSphere -> Mesh.cpp; dump has `Merge(BSphere)` in Vector.cpp |

`BSphere::Merge(Transform)` was tested: moving it to `Mesh.cpp` regressed `byte_exact`
**1458 -> 1426** (the copy helper / inlining changed). It needs a codegen-preserving move, not a
blind cut-and-paste.

`GetAEWChar` is already in `String.cpp` (dump: `AEString.mm`); it only needs source reordering.

### 2.4 Android/JNI glue (19 symbols, mostly source order)

The original `main.c` order is: `JNI_OnLoad`, the `ToJNI_*` functions, then the
`GOF2HD2012_*` JNI functions, then the `g_android_*` achievement globals, then the remaining
`g_p*`/`ndk_*` globals. Our `main.c` defines the `GOF2HD2012_*` functions and the
`g_android_*` globals earlier, before the `ToJNI_*` block. This is a pure source-order fix in
`src/platform/android/main.c` (gaps 8-10, 17 symbols).

`g_android_back_button_pressed` is different: the original has it immediately after
`Java_..._ToJNI_BackButtonPressed`, i.e. the same TU as the NFC JNI glue. Our definition is in
`main.c`; `NFC.cpp` only references it. Moving the definition to `NFC.cpp` is semantically the
right TU move, but **measured: no dynsym change** (gold had already inserted it early via the
`NFC.cpp` reference). It is still worth doing for TU correctness.

`g_pActivity` is early in the original but late in ours because our `NFC.cpp` does not reference
it. The original's NFC code evidently does. Adding an early reference (or moving the definition)
would fix it, but this is a code-level change, not a pure TU move.

### 2.5 Destructor alias ordering (8 symbols / 16 O+U occurrences)

For `RepairBeam`, `SpacePoint`, `SolarSystem`, `PendingProduct`, `Wanted`, `BluePrint`,
`RadioMessage`, and `ParticleSystemMesh` (8 D1 symbols, 16 O+U occurrences), the original emits
`D1` early (next to `C1`) and `D2` late (next to `C2`), while ours emits `D1` and `D2` together late. Both are aliases at the same
address. This is a gold insertion-order / alias-order artifact, not TU membership. It can be
influenced by which TU first references the destructor, but there is no simple file move.

### 2.6 Source-order-only items (no TU move)

* `VirtualInput.cpp`: `currentstate`, `gEngine`, `sound_off`, `stateChanged` are early in ours,
  late in the original (gaps 3-5, 29-31). Reorder the declarations/definitions.
* `LODManager.cpp`: `curTouchSize`, `maxTouchSize`, `touches` (gaps 32-33). Reorder.
* `PlayerFighter.cpp` / `Agent.cpp`: `removeTrail`, `isMale` (gaps 42-43). Reorder.
* `ResourceMaterial.cpp`: `ResourceTexture::ResourceTexture(String const&, float)` is a weak
  definition in the wrong TU (gaps 46-47). Move it to the ResourceTexture/GameResource TU.

### 2.7 Cannot fix without inventing a TU

* `AELabelObject` (no dump witness; likely `ImageFont.cpp`).
* `CurveCreate`/`CurveRelease`/`CurveGetValue` (no dump witness; likely a `Curve.cpp`).
* `GetEngine` is **not** a TU problem: the original address `0x93c89` is in the
  `ApplicationManager.cpp` `.text` region. Moving it to `GameInit.cpp` was tested: `byte_exact`
  improved **1458 -> 1459**, but dynsym matched dropped **5214 -> 5176** and same-position
  **805 -> 525**, because gold then inserted it before `LODManager`'s `AddTouch`/`GetTouch`.
  Keep it in `ApplicationManager.cpp` unless the byte_exact gain is wanted for its own sake.

## 3. OBJECT-vs-FUNC interleaving

**Yes, the original interleaves data objects with the functions of the same TU.** The defined
`.dynsym` has 4524 FUNCs and 843 OBJECTs; there are **500 adjacent FUNC/OBJECT type transitions**
in the original (492 in ours). The interleaving follows the per-object symbol-table order (roughly
source declaration order), not "all functions, then all data".

Examples from the original:

* NFC/JNI glue:
  `Java_..._BackButtonPressed` (FUNC) -> `g_android_back_button_pressed` (OBJECT) ->
  `NFC::showMoreGames` (FUNC) -> `NFC::interface_path` (OBJECT) -> more NFC FUNCs.
* `FileInterfaceAndroid.cpp`:
  `OpenAppend` (FUNC) -> `methodRead` (OBJECT) -> `GetFileSize` (FUNC) -> `fileCounter` (OBJECT) ->
  `methodWrite` (OBJECT) -> `FileEnumInit` (FUNC) -> ... -> `env` (OBJECT) -> `Read` (FUNC) ->
  `clazz` (OBJECT) -> `Output` (FUNC) -> `context` (OBJECT) -> `OpenRead` (FUNC).
* Blowfish:
  `Blowfish_decipher` (FUNC) -> `Blowfish_encipher` (FUNC) -> `F` (FUNC) ->
  `InitializeBlowfish` (FUNC) -> `P` (OBJECT) -> `S` (OBJECT) -> `SubkeyFile` (OBJECT) ->
  `opensubkeyfile` (FUNC).
* `AEFile.cpp`: `SetAppRootDir` (FUNC) -> `fileInterface` (OBJECT) -> `RegisterPakFile` (FUNC) ->
  `pakFileEntryList` (OBJECT) -> `SetSaveDirectory` (FUNC) -> `file` (OBJECT) -> `Read` (FUNC).
* Shader classes: every `XShader::ShaderIndex` OBJECT sits immediately before the corresponding
  `XShader::XShader()` FUNC (e.g. `GlowShader::ShaderIndex` -> `GlowShader::GlowShader()`).

Implication: our per-TU static ordering is **right in principle** where the TU is right. The
`BlowfishSubkeyFile.cpp` block is a good example: after moving `F` into the file, its order
(`Blowfish_decipher`, `Blowfish_encipher`, `F`, `InitializeBlowfish`, `P`, `S`, `SubkeyFile`,
`opensubkeyfile`) matches the original exactly. The remaining OBJECT/FUNC mismatches are mostly
caused by (a) the wrong TU owning a symbol, or (b) source declaration order within a correct TU.

## 4. Experiments (all reverted before commit)

### 4.1 `F` move (clean win, recommended)

**Patch:**

```diff
--- a/src/engine/input/VirtualInput.cpp
+++ b/src/engine/input/VirtualInput.cpp
@@
-extern "C" unsigned int S[4][256]; // lint: extern_c ...
-
-// Blowfish round function over the S-boxes loaded by InitializeBlowfish.
-extern "C" unsigned int F(unsigned int value) { // lint: extern_c ...
-    const unsigned int a = value >> 24;
-    const unsigned int b = (value >> 16) & 0xff;
-    const unsigned int c = (value >> 8) & 0xff;
-    const unsigned int d = value & 0xff;
-
-    return ((S[0][a] + S[1][b]) ^ S[2][c]) + S[3][d];
-}
--- a/src/engine/file/BlowfishSubkeyFile.cpp
+++ b/src/engine/file/BlowfishSubkeyFile.cpp
@@
 extern "C" unsigned int F(unsigned int value); // lint: extern_c ...
 
+// Blowfish round function over the S-boxes loaded by InitializeBlowfish.
+extern "C" unsigned int F(unsigned int value) { // lint: extern_c ...
+    const unsigned int a = value >> 24;
+    const unsigned int b = (value >> 16) & 0xff;
+    const unsigned int c = (value >> 8) & 0xff;
+    const unsigned int d = value & 0xff;
+
+    return ((S[0][a] + S[1][b]) ^ S[2][c]) + S[3][d];
+}
```

**Measured:** `check` passes; byte 1458, linked 4493, data 843/843, lint CLEAN. Dynsym:
blocks **78 -> 75**, matched **5212 -> 5214**, ratio **0.970577 -> 0.970950**, unmatched
**316 -> 312**, same-position **805 -> 805**. The Blowfish block is now exact.

### 4.2 Archive link order (clean win, recommended)

See §2.1. Measured: matched **5214 -> 5228**, ratio **0.970950 -> 0.973557**, unmatched
**312 -> 284**, same-position **805 -> 807**, gate unchanged.

### 4.3 `g_android_back_button_pressed` move (semantic, no dynsym change)

Moved the definition from `main.c` to `NFC.cpp`. Gate unchanged; dynsym metrics unchanged
(matched 5214, same-position 805). Gold had already inserted the symbol early via the `NFC.cpp`
reference, so the position is determined by first reference, not definition TU.

### 4.4 `BSphere::Merge(Transform)` move (regression)

Moved from `BSphere.cpp` to `Mesh.cpp` (with the `CopyVector` helper and forward declarations).
`byte_exact` regressed **1458 -> 1426**; reverted. A codegen-preserving move is still possible but
needs work.

### 4.5 `GetEngine` move (byte win, dynsym regression)

Moved from `ApplicationManager.cpp` to `GameInit.cpp`. `byte_exact` improved **1458 -> 1459**,
but dynsym matched dropped **5214 -> 5176** and same-position **805 -> 525**. The original address
`0x93c89` is in the `ApplicationManager.cpp` region, so this is not a TU fix; keep it in
`ApplicationManager.cpp` unless the byte_exact gain is separately wanted.

## 5. Recommended order of work

1. **Archive link order** (OpenSSL after zlib/libzip): 20-symbol SHA block, no `src/` change,
   gate unchanged, measured +16 matched. Do this first.
2. **`F` move**: 8-symbol Blowfish block, gate unchanged, measured +2 matched. Do this with the
   link-order change in the same pass if the combined gate stays clean.
3. **`main.c` source reorder**: 17-symbol JNI/global block (gaps 8-10). Pure source order; no TU
   invention. Verify with the full gate.
4. **GameText split**: 13-symbol block; create `GameText_iPhone.cpp` from the dump witness. This is
   the largest real TU-membership change; expect `.rodata`/string-pool movement.
5. **`AbyssEngine.cpp` split**: `esMatrixMultiply` -> `Engine.cpp` first (dump witness), then
   `getAppVersion`/`SetFXMaterial`/`GetStringLength`, then the no-witness `AELabelObject`/`Curve*`
   items last.
6. **zlib/libzip archive-member order**: reorder the vendored archive members to the original
   per-source order. This is the remaining ~90-symbol tail and is build-system work.
7. **Destructor alias ordering**: low priority; 20 symbols but no simple TU move.

## 6. Caveats

* **Gold's insertion order is not strictly input-object order.** A symbol is inserted when first
  defined *or referenced*. `g_android_back_button_pressed` and `GetEngine` are the measured
  counterexamples. Long matching blocks still fingerprint TU membership/link order; single-symbol
  gaps often do not.
* **Alias order (`D1`/`D2`, `C1`/`C2`)** is compiler/gold symbol-table order, not TU membership.
* **The original is stripped**, so there is no `.symtab`/object boundary to read. The implied TUs
  come from the iOS source dumps and symbol adjacency, not from the Android binary's object table.
* **The task brief's baseline (byte 1415) is stale** for `master` HEAD `9b46ec31`; the measured
  baseline is byte 1458, linked 4493, size-exact 4522/4523, `.text` 0x1343d0.

---

## Appendix A: Matching blocks (baseline)

| # | orig defined | ours defined | size | first symbol | last symbol |
|---|---|---|---|---|---|
| 0 | 0:1 | 0:1 | 1 | `Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed` | `Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed` |
| 1 | 2:31 | 1:30 | 29 | `NFC::showMoreGames()` | `_JNIEnv::CallStaticBooleanMethod(_jclass*, _jmethodID*, ...)` |
| 2 | 32:43 | 31:42 | 11 | `g_pEnv` | `gi_iap_buy_dlc5_pressed` |
| 3 | 43:138 | 44:139 | 95 | `MouseInput(int, int)` | `AE::ApplicationManager::GetCurrentApplicationModule() const` |
| 4 | 138:139 | 141:142 | 1 | `keys` | `keys` |
| 5 | 139:141 | 143:145 | 2 | `stackX` | `stackY` |
| 6 | 141:144 | 146:149 | 3 | `wheelStack` | `Blowfish_encipher` |
| 7 | 145:147 | 149:151 | 2 | `InitializeBlowfish` | `P` |
| 8 | 148:152 | 151:155 | 4 | `SubkeyFile` | `JNI_OnLoad` |
| 9 | 152:193 | 166:207 | 41 | `Java_net_fishlabs_gof2hdallandroid2012_ToJNI_STARTUP` | `flstat_rootPath` |
| 10 | 193:226 | 214:247 | 33 | `g_pClass` | `zipDirectory` |
| 11 | 243:293 | 247:297 | 50 | `void ArrayAdd<AE::Mesh*>(AE::Mesh*, Array<AE::Mesh*>&)` | `glVertexPointer` |
| 12 | 294:300 | 297:303 | 6 | `AE::PaintCanvas::Initialize(bool)` | `AE::FBOContainer::FBOContainer(AE::Engine*, AE::String)` |
| 13 | 301:351 | 303:353 | 50 | `AE::Engine::ShaderInit()` | `glTexEnvi` |
| 14 | 351:354 | 362:365 | 3 | `APKArchive` | `OpenAppend(unsigned short*, int, bool, unsigned int)` |
| 15 | 355:405 | 365:415 | 50 | `loge(char*)` | `_JNIEnv::CallVoidMethod(_jobject*, _jmethodID*, ...)` |
| 16 | 406:664 | 415:673 | 258 | `AE::String::GetAEChar() const` | `AE::AEMath::VectorIsEqual(AE::AEMath::Vector const&, AE::AEMath::Vector const&)` |
| 17 | 666:673 | 673:680 | 7 | `AE::AEMath::Vector::operator float*()` | `AE::AEMath::Vector::operator*=(float)` |
| 18 | 674:688 | 680:694 | 14 | `AE::AEMath::operator/(AE::AEMath::Vector const&, AE::AEMath::Vector const&)` | `AE::AEMath::Vector::operator[](int) const` |
| 19 | 688:689 | 697:698 | 1 | `AE::IApplicationModule::SetApplicationManager(AE::ApplicationManager*)` | `AE::IApplicationModule::SetApplicationManager(AE::ApplicationManager*)` |
| 20 | 692:1012 | 698:1018 | 320 | `void ArrayRemoveAll<AE::CheatCode*>(Array<AE::CheatCode*>&)` | `Array<unsigned int>::~Array()` |
| 21 | 1012:1017 | 1022:1027 | 5 | `GameText::currentLang` | `GameText::getRegionCode()` |
| 22 | 1017:1018 | 1028:1029 | 1 | `GameText::convertStringFromArabic(AE::String)` | `GameText::convertStringFromArabic(AE::String)` |
| 23 | 1018:1019 | 1030:1031 | 1 | `GameText::release()` | `GameText::release()` |
| 24 | 1019:1108 | 1038:1127 | 89 | `AE::FBOContainer::Release()` | `AE::Quaternion::operator[](int) const` |
| 25 | 1121:1124 | 1127:1130 | 3 | `void ArrayRelease<AE::IApplicationModule*>(Array<AE::IApplicationModule*>&)` | `void ArrayAdd<long long>(long long, Array<long long>&)` |
| 26 | 1124:1259 | 1131:1266 | 135 | `AE::currentFps` | `AE::SandboxShader::SandboxShader()` |
| 27 | 1260:1273 | 1266:1279 | 13 | `AE::TexOnlyShader::TexOnlyShader()` | `AE::ShaderBaseStruct::Update()` |
| 28 | 1274:1825 | 1279:1830 | 551 | `AE::BumpRimCubeShader::ShaderIndex` | `GetTouch(int)` |
| 29 | 1826:1866 | 1830:1870 | 40 | `RecordHandler::saveOptions()` | `checkFirstCreditPackBoughtWriteAction` |
| 30 | 1867:1868 | 1870:1871 | 1 | `forceExit` | `forceExit` |
| 31 | 1869:1886 | 1871:1888 | 17 | `gRealHeight` | `setBaughtCredits` |
| 32 | 1888:1891 | 1888:1891 | 3 | `zip_get_name` | `zip_open` |
| 33 | 1894:1910 | 1891:1907 | 16 | `void ArrayRemove<AEGeometry*>(AEGeometry*, Array<AEGeometry*>&)` | `Globals::Canvas` |
| 34 | 1910:2188 | 1910:2188 | 278 | `void ArraySetLength<Item*>(unsigned int, Array<Item*>&)` | `RepairBeam::RepairBeam(int, int)` |
| 35 | 2189:2709 | 2188:2708 | 520 | `SpacePoint::take()` | `SpacePoint::isFree()` |
| 36 | 2710:2873 | 2708:2871 | 163 | `Item::setUnsaleable(bool)` | `RepairBeam::RepairBeam(int, int)` |
| 37 | 2873:2905 | 2872:2904 | 32 | `RepairBeam::~RepairBeam()` | `SolarSystem::isVisible()` |
| 38 | 2906:2910 | 2904:2908 | 4 | `Achievements::incPirateKills()` | `PendingProduct::PendingProduct(BluePrint*)` |
| 39 | 2911:3050 | 2908:3047 | 139 | `SystemPathFinder::getSystemPath(Array<SolarSystem*>*, int, int)` | `Wanted::setActive(bool)` |
| 40 | 3051:3083 | 3047:3079 | 32 | `Globals::resetHints()` | `BluePrint::BluePrint(int)` |
| 41 | 3084:3113 | 3079:3108 | 29 | `void ArrayRemove<AE::String*>(AE::String*, Array<AE::String*>&)` | `RadioMessage::RadioMessage(int, int, int, int, int)` |
| 42 | 3114:3115 | 3108:3109 | 1 | `LodMeshMerger::~LodMeshMerger()` | `LodMeshMerger::~LodMeshMerger()` |
| 43 | 3116:3152 | 3109:3145 | 36 | `PlayerFighter::setBoostProb(int)` | `Ship::getRace()` |
| 44 | 3153:3273 | 3145:3265 | 120 | `Array<AbstractGun*>::Array()` | `SentryGun::SentryGun(Gun*, int, int, int, Level*)` |
| 45 | 3273:3613 | 3268:3608 | 340 | `void ArrayRelease<bool>(Array<bool>&)` | `MissionsWindow::~MissionsWindow()` |
| 46 | 3613:3879 | 3609:3875 | 266 | `ScrollTouchBox::setPosition(int, int)` | `AE::ResourceTexture::ResourceTexture(char const*, float)` |
| 47 | 3880:3882 | 3875:3877 | 2 | `AE::ResourceMaterial::ResourceMaterial(unsigned short, AE::BlendMode)` | `AE::ResourceMaterial::ResourceMaterial(unsigned short, unsigned short, AE::BlendMode)` |
| 48 | 3882:4062 | 3878:4058 | 180 | `void ArrayRemove<Node*>(Node*, Array<Node*>&)` | `Wanted::Wanted(int, AE::String, int, int, bool, int, int, int, int, int, int, int, int,...` |
| 49 | 4062:4323 | 4059:4320 | 261 | `Wanted::~Wanted()` | `RadioMessage::RadioMessage(int, int, int, int, int)` |
| 50 | 4323:4390 | 4321:4388 | 67 | `RadioMessage::~RadioMessage()` | `SolarSystem::SolarSystem(int, AE::String, int, bool, int, int, int, int, int, int, int*...` |
| 51 | 4390:4562 | 4389:4561 | 172 | `SolarSystem::~SolarSystem()` | `SpacePoint::SpacePoint(int, AE::AEMath::Vector const&, AE::AEMath::Vector const&, int)` |
| 52 | 4562:4753 | 4562:4753 | 191 | `SpacePoint::~SpacePoint()` | `BluePrint::BluePrint(int)` |
| 53 | 4753:4882 | 4754:4883 | 129 | `BluePrint::~BluePrint()` | `ParticleSystemMesh::ParticleSystemMesh(AE::PaintCanvas*, AE::AEMath::Matrix const*, Arr...` |
| 54 | 4883:4931 | 4883:4931 | 48 | `ParticleSystemManager::emitManual(int, AE::AEMath::Vector const&, int, float)` | `ParticleSystemMesh::ParticleSystemMesh(AE::PaintCanvas*, AE::AEMath::Matrix const*, Arr...` |
| 55 | 4931:4935 | 4932:4936 | 4 | `ParticleSystemMesh::~ParticleSystemMesh()` | `vtable for ParticleSystemMesh` |
| 56 | 4935:4936 | 4956:4957 | 1 | `adler32` | `adler32` |
| 57 | 4952:4958 | 4958:4964 | 6 | `compress` | `deflateInit_` |
| 58 | 4958:4975 | 4967:4984 | 17 | `_dist_code` | `z_errmsg` |
| 59 | 4977:4980 | 4986:4989 | 3 | `inflateBack` | `inflateBackInit_` |
| 60 | 4980:4983 | 5005:5008 | 3 | `zError` | `zlibVersion` |
| 61 | 4987:4988 | 5008:5009 | 1 | `_zip_error_set` | `_zip_error_set` |
| 62 | 4989:4991 | 5009:5011 | 2 | `_zip_replace` | `zip_add` |
| 63 | 5000:5001 | 5012:5013 | 1 | `zip_source_buffer` | `zip_source_buffer` |
| 64 | 5018:5021 | 5014:5017 | 3 | `_zip_cdir_free` | `_zip_cdir_write` |
| 65 | 5021:5022 | 5018:5019 | 1 | `_zip_dirent_init` | `_zip_dirent_init` |
| 66 | 5022:5024 | 5020:5022 | 2 | `_zip_dirent_torrent_normalize` | `_zip_dirent_write` |
| 67 | 5024:5025 | 5023:5024 | 1 | `_zip_free` | `_zip_free` |
| 68 | 5025:5028 | 5025:5028 | 3 | `zip_close` | `zip_source_zip` |
| 69 | 5045:5046 | 5029:5030 | 1 | `zip_delete` | `zip_delete` |
| 70 | 5051:5054 | 5030:5033 | 3 | `_zip_cdir_grow` | `_zip_read4` |
| 71 | 5054:5055 | 5046:5047 | 1 | `zip_error_get` | `zip_error_get` |
| 72 | 5055:5056 | 5049:5050 | 1 | `zip_file_error_clear` | `zip_file_error_clear` |
| 73 | 5057:5058 | 5076:5077 | 1 | `zip_unchange_all` | `zip_unchange_all` |
| 74 | 5058:5063 | 5079:5084 | 5 | `_edata` | `__cxa_call_unexpected` |
| 75 | 5083:5090 | 5084:5091 | 7 | `std::get_terminate()` | `__cxa_decrement_exception_refcount` |
| 76 | 5091:5370 | 5091:5370 | 279 | `__cxa_end_cleanup` | `__cxa_demangle` |


## Appendix B: Out-of-order blocks (baseline)

| gap | orig defined | n | symbols (demangled, shortened) | our owner(s) | implied original TU / action |
|---|---|---|---|---|---|
| 0 | 1:2 | 1 | `g_android_back_button_pressed` | main.c | **NFC.cpp** — Move `g_android_back_button_pressed` definition from `main.c` to `NFC.cpp` (same TU as `Java_..._BackButtonPressed`). Measured: no dynsym change. |
| 1 | 31:32 | 1 | `g_pActivity; g_android_back_button_pressed` | main.c | **main.c / NFC.cpp** — `g_pActivity` is early in the original; our `NFC.cpp` does not reference it. Add an early reference or move the definition. Uncertain. |
| 2 | 43:43 | 0 | `F; S` | BlowfishSubkeyFile.cpp, VirtualInput.cpp | **BlowfishSubkeyFile.cpp** — Move `F` from `VirtualInput.cpp` to `BlowfishSubkeyFile.cpp`. Measured: fixes the whole Blowfish block (8 symbols). |
| 3 | 138:138 | 0 | `currentstate; gEngine` | VirtualInput.cpp | **VirtualInput.cpp** — Reorder `currentstate`, `gEngine` after the `keys/stackX/stackY/wheelStack` group. |
| 4 | 139:139 | 0 | `sound_off` | VirtualInput.cpp | **VirtualInput.cpp** — Reorder `sound_off`. |
| 5 | 141:141 | 0 | `stateChanged` | VirtualInput.cpp | **VirtualInput.cpp** — Reorder `stateChanged`. |
| 6 | 144:145 | 1 | `F` | VirtualInput.cpp | **BlowfishSubkeyFile.cpp** — Same as gap 2 (`F`). |
| 7 | 147:148 | 1 | `S` | BlowfishSubkeyFile.cpp | **BlowfishSubkeyFile.cpp** — `S` follows `F`; fixed by the gap-2 move. |
| 8 | 152:152 | 0 | `Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetAchi...; Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLead...; Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLink...; … (+8)` | main.c | **main.c** — Reorder the 11 `GOF2HD2012_*` JNI functions after the `ToJNI_*` block. |
| 9 | 193:193 | 0 | `g_android_current_achievements; g_android_gp_is_linked; g_android_leaderboard_scores; … (+4)` | main.c | **main.c** — Reorder the 6 `g_android_*` achievement globals and `g_pActivity`. |
| 10 | 226:243 | 17 | `Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetAchi...; Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLead...; Java_net_fishlabs_gof2hdallandroid2012_GOF2HD2012_GetLink...; … (+14)` | main.c | **main.c** — Same as gaps 8-9: 17 JNI/global symbols out of source order. |
| 11 | 293:294 | 1 | `AELabelObject(unsigned int, unsigned int, char const*)` | AbyssEngine.cpp | **ImageFont.cpp? (no witness)** — `AELabelObject` sits before `PaintCanvas::Initialize` in the original. Cannot fix without inventing a TU. |
| 12 | 300:301 | 1 | `AE::getAppVersion()` | AbyssEngine.cpp | **Engine.cpp (likely)** — Move `getAppVersion` from `AbyssEngine.cpp` to `Engine.cpp`. |
| 13 | 351:351 | 0 | `AELabelObject(unsigned int, unsigned int, char const*); GetStringLength(AE::String); AE::CurveCreate(void**, unsigned short, AE::Curve**); … (+6)` | AbyssEngine.cpp, String.cpp | **AbyssEngine.cpp mega-TU** — Split: `AELabelObject` (ImageFont?), `GetStringLength` (String/FileInterface), `Curve*` (Curve.cpp?), `SetFXMaterial`/`getAppVersion`/`esMatrixMultiply` (Engine.cpp), `GetAEWChar` (String.cpp). |
| 14 | 354:355 | 1 | `GetStringLength(AE::String)` | AbyssEngine.cpp | **String.cpp or FileInterfaceAndroid.cpp** — Move `GetStringLength` from `AbyssEngine.cpp`. |
| 15 | 405:406 | 1 | `AE::String::GetAEWChar() const` | String.cpp | **String.cpp** — `GetAEWChar` is already in `String.cpp`; reorder within the TU. |
| 16 | 664:666 | 2 | `AE::AEMath::MatrixRotateVector(AE::AEMath::Matrix const&,...; AE::AEMath::MatrixTransformVector(AE::AEMath::Matrix cons...` | Matrix.cpp | **Matrix.cpp** — `MatrixRotateVector`/`MatrixTransformVector` are already in `Matrix.cpp`; reorder within the TU. |
| 17 | 673:674 | 1 | `AE::AEMath::BSphere::Merge(AE::Transform const&)` | BSphere.cpp | **Mesh.cpp (tu_mapping)** — Move `BSphere::Merge(Transform)` from `BSphere.cpp` to `Mesh.cpp`. Measured: byte_exact 1458 -> 1426, needs codegen work. |
| 18 | 688:688 | 0 | `AE::AEMath::MatrixRotateVector(AE::AEMath::Matrix const&,...; AE::AEMath::MatrixTransformVector(AE::AEMath::Matrix cons...; AE::AEMath::BSphere::Merge(AE::Transform const&)` | BSphere.cpp, Matrix.cpp | **Matrix.cpp + Mesh.cpp** — Same as gaps 16-17. |
| 19 | 689:692 | 3 | `AE::CurveCreate(void**, unsigned short, AE::Curve**); AE::CurveRelease(AE::Curve**); AE::CurveGetValue(unsigned long long, AE::Curve*)` | AbyssEngine.cpp | **Curve.cpp? (no witness)** — `CurveCreate/Release/GetValue` need a Curve TU; cannot fix without inventing one. |
| 20 | 1012:1012 | 0 | `void ArrayRemoveAll<int>(Array<int>&); void ArrayAdd<int>(int, Array<int>&); Array<int>::Array(); Array<int>::~Array()` | GameText.cpp(W) | **GameText.cpp** — `Array<int>` instantiations are already emitted in `GameText.cpp`; reorder after `setSubstituteArray`. |
| 21 | 1017:1017 | 0 | `GameText::setSubstituteArray(int*, unsigned int)` | GameText.cpp | **GameText.cpp** — Reorder `setSubstituteArray`. |
| 22 | 1018:1018 | 0 | `GameText::getText(int)` | GameText.cpp | **GameText.cpp** — Reorder `getText`. |
| 23 | 1019:1019 | 0 | `GameText::GameText(); GameText::GameText(); GameText::~GameText(); … (+4)` | ?, GameText.cpp | **GameText.cpp** — Reorder ctor/dtor and `__cxa_guard_*`. |
| 24 | 1108:1121 | 13 | `void ArrayRemoveAll<int>(Array<int>&); void ArrayAdd<int>(int, Array<int>&); Array<int>::Array(); … (+10)` | ?, GameText.cpp, GameText.cpp(W) | **GameText.cpp + GameText_iPhone.cpp** — Split `GameText.cpp`: keep ctor/dtor/getText/setSubstituteArray/Array<int> in `GameText.cpp`; move `setLanguage`, `ReadLangFile`, `getRegionCode`, `convertStringFromArabic`, `release`, `currentLang` to a `GameText_iPhone.cpp` platform TU (dump witness). |
| 25 | 1124:1124 | 0 | `GetEngine()` | ApplicationManager.cpp | **ApplicationManager.cpp** — `GetEngine` is in `ApplicationManager.cpp` in the original (address 0x93c89); the gap is gold insertion order, not TU membership. |
| 26 | 1259:1260 | 1 | `AE::SetFXMaterial(AE::BlendMode)` | AbyssEngine.cpp | **Engine.cpp or PaintCanvas.cpp** — Move `SetFXMaterial` from `AbyssEngine.cpp`. |
| 27 | 1273:1274 | 1 | `AE::esMatrixMultiply(AE::ESMatrix*, AE::ESMatrix*, AE::ES...` | AbyssEngine.cpp | **Engine.cpp (dump witness)** — Move `esMatrixMultiply` from `AbyssEngine.cpp` to `Engine.cpp`. |
| 28 | 1825:1826 | 1 | `GetEngine()` | ApplicationManager.cpp | **ApplicationManager.cpp** — Same as gap 25. |
| 29 | 1866:1867 | 1 | `currentstate` | VirtualInput.cpp | **VirtualInput.cpp** — Reorder `currentstate`. |
| 30 | 1868:1869 | 1 | `gEngine` | VirtualInput.cpp | **VirtualInput.cpp** — Reorder `gEngine`. |
| 31 | 1886:1888 | 2 | `sound_off; stateChanged` | VirtualInput.cpp | **VirtualInput.cpp** — Reorder `sound_off`, `stateChanged`. |
| 32 | 1891:1894 | 3 | `curTouchSize; maxTouchSize; touches` | LODManager.cpp | **LODManager.cpp** — Reorder `curTouchSize`, `maxTouchSize`, `touches`. |
| 33 | 1910:1910 | 0 | `curTouchSize; maxTouchSize; touches` | LODManager.cpp | **LODManager.cpp** — Same as gap 32. |
| 34 | 2188:2189 | 1 | `RepairBeam::~RepairBeam()` | RepairBeam.cpp | **RepairBeam.cpp** — D1/D2 alias ordering; original emits D1 early with C1, ours emits D1 late with D2. |
| 35 | 2709:2710 | 1 | `SpacePoint::~SpacePoint()` | SpacePoint.cpp | **SpacePoint.cpp** — D1/D2 alias ordering. |
| 36 | 2873:2873 | 0 | `RepairBeam::~RepairBeam()` | RepairBeam.cpp | **RepairBeam.cpp** — D1/D2 alias ordering. |
| 37 | 2905:2906 | 1 | `SolarSystem::~SolarSystem()` | SolarSystem.cpp | **SolarSystem.cpp** — D1/D2 alias ordering. |
| 38 | 2910:2911 | 1 | `PendingProduct::~PendingProduct()` | PendingProduct.cpp | **PendingProduct.cpp** — D1/D2 alias ordering. |
| 39 | 3050:3051 | 1 | `Wanted::~Wanted()` | Wanted.cpp | **Wanted.cpp** — D1/D2 alias ordering. |
| 40 | 3083:3084 | 1 | `BluePrint::~BluePrint()` | BluePrint.cpp | **BluePrint.cpp** — D1/D2 alias ordering. |
| 41 | 3113:3114 | 1 | `RadioMessage::~RadioMessage()` | RadioMessage.cpp | **RadioMessage.cpp** — D1/D2 alias ordering. |
| 42 | 3115:3116 | 1 | `PlayerFighter::removeTrail()` | PlayerFighter.cpp | **PlayerFighter.cpp** — Source order / early reference. |
| 43 | 3152:3153 | 1 | `Agent::isMale()` | Agent.cpp | **Agent.cpp** — Source order / early reference. |
| 44 | 3273:3273 | 0 | `__cxa_end_catch; PlayerFighter::removeTrail(); Agent::isMale()` | ?, Agent.cpp, PlayerFighter.cpp | **libc++ + PlayerFighter/Agent** — `__cxa_end_catch` plus the two functions; runtime/source order. |
| 45 | 3613:3613 | 0 | `PendingProduct::~PendingProduct()` | PendingProduct.cpp | **PendingProduct.cpp** — D1/D2 alias ordering. |
| 46 | 3879:3880 | 1 | `AE::ResourceTexture::ResourceTexture(AE::String const&, f...` | ResourceMaterial.cpp(W) | **ResourceTexture.cpp / GameResource.cpp** — Move `ResourceTexture::ResourceTexture(String const&, float)` from `ResourceMaterial.cpp` to the ResourceTexture/GameResource TU. |
| 47 | 3882:3882 | 0 | `AE::ResourceTexture::ResourceTexture(AE::String const&, f...` | ResourceMaterial.cpp(W) | **ResourceTexture.cpp / GameResource.cpp** — Same as gap 46. |
| 48 | 4062:4062 | 0 | `Wanted::~Wanted()` | Wanted.cpp | **Wanted.cpp** — D1/D2 alias ordering. |
| 49 | 4323:4323 | 0 | `RadioMessage::~RadioMessage()` | RadioMessage.cpp | **RadioMessage.cpp** — D1/D2 alias ordering. |
| 50 | 4390:4390 | 0 | `SolarSystem::~SolarSystem()` | SolarSystem.cpp | **SolarSystem.cpp** — D1/D2 alias ordering. |
| 51 | 4562:4562 | 0 | `SpacePoint::~SpacePoint()` | SpacePoint.cpp | **SpacePoint.cpp** — D1/D2 alias ordering. |
| 52 | 4753:4753 | 0 | `BluePrint::~BluePrint()` | BluePrint.cpp | **BluePrint.cpp** — D1/D2 alias ordering. |
| 53 | 4882:4883 | 1 | `ParticleSystemMesh::~ParticleSystemMesh()` | ParticleSystemMesh.cpp | **ParticleSystemMesh.cpp** — D1/D2 alias ordering. |
| 54 | 4931:4931 | 0 | `ParticleSystemMesh::~ParticleSystemMesh()` | ParticleSystemMesh.cpp | **ParticleSystemMesh.cpp** — D1/D2 alias ordering. |
| 55 | 4935:4935 | 0 | `SHA224_Init; sha256_block_data_order; SHA224_Update; … (+17)` | ? | **OpenSSL archive** — Link order: move `${_openssl_out}` after `${_zlib_out} ${_libzip_out}`. Measured: fixes the 20-symbol SHA/OpenSSL block; ratio 0.970950 -> 0.973557. |
| 56 | 4936:4952 | 16 | `crc32; inflate; inflateCopy; … (+14)` | ? | **zlib inflate.o** — zlib archive-member order; reorder `inflate.o`/`inffast.o`/`inftrees.o`. |
| 57 | 4958:4958 | 0 | `crc32; crc32_combine; get_crc_table` | ? | **zlib crc32.o** — zlib archive-member order. |
| 58 | 4975:4977 | 2 | `crc32_combine; get_crc_table; zcalloc; zcfree` | ? | **zlib crc32.o vs zutil.o** — zlib archive-member order. |
| 59 | 4980:4980 | 0 | `inflate_fast; inflate_table; inflate; … (+13)` | ? | **zlib inflate.o** — zlib archive-member order. |
| 60 | 4983:4987 | 4 | `inflate_copyright; uncompress; _tr_tally; adler32_combine` | ? | **zlib inflate.o/adler32.o** — zlib archive-member order. |
| 61 | 4988:4989 | 1 | `_zip_memdup` | ? | **libzip** — libzip archive-member order. |
| 62 | 4991:5000 | 9 | `_zip_filerange_crc; _zip_get_name; _zip_error_strerror; … (+7)` | ? | **libzip** — libzip archive-member order. |
| 63 | 5001:5018 | 17 | `zip_source_function; zip_stat_init; _zip_error_clear; … (+15)` | ? | **libzip** — libzip archive-member order. |
| 64 | 5021:5021 | 0 | `_zip_dirent_finalize` | ? | **libzip** — libzip archive-member order. |
| 65 | 5022:5022 | 0 | `_zip_dirent_read` | ? | **libzip** — libzip archive-member order. |
| 66 | 5024:5024 | 0 | `_zip_filerange_crc` | ? | **libzip** — libzip archive-member order. |
| 67 | 5025:5025 | 0 | `_zip_memdup` | ? | **libzip** — libzip archive-member order. |
| 68 | 5028:5045 | 17 | `zip_add_dir; zip_source_free; _zip_err_type; … (+15)` | ? | **libzip** — libzip archive-member order. |
| 69 | 5046:5051 | 5 | `zip_set_file_comment; zip_stat; zip_get_file_comment; … (+2)` | ? | **libzip** — libzip archive-member order. |
| 70 | 5054:5054 | 0 | `_zip_entry_free; _zip_unchange_data; _zip_entry_new; … (+10)` | ? | **libzip** — libzip archive-member order. |
| 71 | 5055:5055 | 0 | `_zip_error_strerror; zip_error_to_str` | ? | **libzip** — libzip archive-member order. |
| 72 | 5056:5057 | 1 | `zip_error_to_str; zip_file_error_get; _zip_file_get_offset; … (+24)` | ? | **libzip** — libzip archive-member order. |
| 73 | 5058:5058 | 0 | `zip_unchange_archive; _zip_mkstemp` | ? | **libzip** — libzip archive-member order. |
| 74 | 5063:5083 | 20 | `SHA224_Init; sha256_block_data_order; SHA224_Update; … (+17)` | ? | **OpenSSL** — OpenSSL archive order; fixed by the same link-order swap as gap 55. |
| 75 | 5090:5091 | 1 | `__cxa_end_catch` | ? | **libc++** — Runtime symbol ordering; not TU membership. |

