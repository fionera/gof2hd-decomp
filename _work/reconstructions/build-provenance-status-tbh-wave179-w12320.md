# Build-provenance audit and `Status::missionCompleted` TBH layout (wave 179, w12320)

Status: **research note, no source/build change landed**. Worktree
`/home/debian/GalaxyOnFire2/wt/w12320`, branch `wave179-w12320`, base
`64014ce6f`. Scratch `/home/debian/GalaxyOnFire2/scratch/w12320`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(ELF32 ARM, armeabi-v7a softfp, image base 0x10000). APK:
`/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk`.

This pass re-derived the compile/link provenance from primary ELF evidence,
compared the checked-in CMake build with the stock NDK r18b CMake Release
toolchain, re-measured the per-TU `-mllvm` knobs, and re-examined the
`Status::missionCompleted` switch/TBH body order. No flag or link-order lever
was found that improves any of the 17 residual functions; the note is the only
commit.

## 0. Bottom line

* The original is **exactly** NDK r18b (18.1.5063045), clang 7.0.2
  (build 4751641 / r328903), GNU gold 1.12, API 16, armeabi-v7a Thumb-2
  VFPv3/NEONv1 softfp. `.comment`, `.note.android.ident`,
  `.note.gnu.gold-version` and `.ARM.attributes` are byte-identical to our
  canonical build. `.ARM.exidx` is byte-identical (3,566 entries: 856 inline,
  2,438 extab, 272 cantunwind); `.ARM.extab` is size- and base-exact and
  differs only in 700 bytes of residual LSDA records.
* The checked-in CMake build is the stock r18b **CMake** Release compile line
  plus the ndk-build-style link order. The few stock CMake flags it omits
  (`-no-canonical-prefixes`, `--fix-cortex-a8`, `--warn-shared-textrel`,
  `--fatal-warnings`, `--exclude-libs,libatomic.a`,
  `--exclude-libs,libunwind.a`, `--no-undefined`) are codegen-neutral or
  inapplicable here; `-fexceptions` is on by default and is not spelled out.
  `-fdata-sections` is deliberately absent (the original matches the CMake
  toolchain, not ndk-build).
* The six per-TU `-mllvm` knobs are matching compensations, not original build
  flags. Re-measured at this HEAD, five are load-bearing and one
  (`Level.cpp -enable-earlycse-memssa=false`) is currently raw-neutral.
* `Status::missionCompleted` is still **raw 721 / aligned 327 / size 1432**.
  The original body order is the source case order
  `167/174, 150,151,152,153,154,155,158,162,166,170,184,189,
  156,157,160,163,164,165,168,169,171,172, default`; our plain switch lays the
  bodies out in **value order** because clang 7's `buildJumpTable` adds the
  jump-table successors in table (value) order and `MachineBlockPlacement`
  follows that successor list. The current source also has `case 162` before
  `case 158`, whereas the original body order has 158 before 162. Swapping
  them alone is not a win (raw 722 / aligned 322 / size 1432).
* No tested per-TU MBP/switch/branch-weight flag reproduces the original
  source-order body layout at size 1432. The hardcoded rank table remains the
  only known way to force it, and it costs the extra `.rodata` table.

## 1. Primary ELF evidence

### 1.1 Toolchain notes

Original `.comment` (303 bytes, two NUL-terminated strings):

```text
Android (4751641 based on r328903) clang version 7.0.2
(https://android.googlesource.com/toolchain/clang 003100370607242ddd5815e4a043907ea9004281)
(https://android.googlesource.com/toolchain/llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
(based on LLVM 7.0.2svn)
GCC: (GNU) 4.9.x 20150123 (prerelease)
```

`.note.android.ident` (152 bytes): API level `0x10` = 16, NDK name `r18b`,
build number `5063045`. `.note.gnu.gold-version` (28 bytes):
`GNU gold 1.12`. The local r18b `source.properties` is
`Pkg.Revision = 18.1.5063045`; `gold --version` is
`GNU gold (GNU Binutils 2.27.0.20170315) 1.12`.

`.ARM.attributes` (56 bytes) decodes to:

```text
Tag_CPU_name: "ARM v7"          Tag_CPU_arch: v7
Tag_CPU_arch_profile: Application
Tag_ARM_ISA_use: Yes            Tag_THUMB_ISA_use: Thumb-2
Tag_FP_arch: VFPv3              Tag_Advanced_SIMD_arch: NEONv1
Tag_ABI_PCS_GOT_use: GOT-indirect
Tag_ABI_PCS_wchar_t: 4          Tag_ABI_FP_denormal: Needed
Tag_ABI_FP_exceptions: Needed   Tag_ABI_FP_number_model: IEEE 754
Tag_ABI_align_needed: 8-byte    Tag_ABI_enum_size: int
Tag_ABI_HardFP_use: Deprecated  Tag_CPU_unaligned_access: v6
Tag_ABI_FP_16bit_format: IEEE 754
```

All four note/attribute sections are **byte-identical** to our canonical
build (`readelf -x`/`-A` compare).

### 1.2 `.ARM.exidx` / `.ARM.extab` shape

| section | original | ours | content |
|---|---:|---:|---|
| `.ARM.exidx` | `0x6f70` @ `0x19cc08` | same | **byte-identical** |
| `.ARM.extab` | `0x26298` @ `0x1a3b78` | same | size/base exact; **700 differing bytes** |

`.ARM.exidx` has 3,566 entries in both: 856 inline, 2,438 extab, 272
cantunwind. `.ARM.extab` record count and encoded LSDA lengths are unchanged;
the 700-byte diff is the residual function bodies' EH records
(`Level::createShip`, `HangarWindow::render`, `Level::createMission`, ...),
not a section-order or flag difference.

### 1.3 APK

APK sha256 `9cfe4484a475c9a31a336df9dea6408fbb129d88aa19083afe808ef6bca9eb19`.
It contains four `lib/armeabi-v7a/*.so` files:

| library | size | sha256 |
|---|---:|---|
| `libgof2hdaa.so` | 2,187,328 | `6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58` |
| `libcrashlytics.so` | 202,580 | `afbe76e6b1b057b5650a85d8784a22030578df68be342a96072555ae483c604b` |
| `libfmodex.so` | 920,204 | `60ec4de090bf442c7b9db41d0731aa22a101db21aab0ae32af1af6a860c3fa81` |
| `libfmodevent.so` | 370,660 | `b92cd6ff8f8edb3d81023ee69c1f5239aeaf6cb4e89d6505101115189569c97d` |

The APK's `libgof2hdaa.so` differs from the ground-truth file by exactly
**288 bytes**, all in `.shstrtab` (the ground truth is the APK copy after
`tools/reorder_shstrtab.py`). No code/data section differs. The other three
libraries are from older GCC toolchains and are link inputs only
(`libfmodex.so`/`libfmodevent.so` are the original APK's prebuilts).

## 2. Checked-in build vs stock NDK r18b CMake Release

Stock r18b `build/cmake/android.toolchain.cmake` for `armeabi-v7a` +
`c++_static` + Release emits:

```text
-g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong
-no-canonical-prefixes
-march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb
-std=c++11
-Oz -DNDEBUG
```

With `ANDROID_ARM_NEON=TRUE` it replaces `-mfpu=vfpv3-d16` with
`-mfpu=neon`; with `ANDROID_CPP_FEATURES=rtti;exceptions` it adds
`-frtti -fexceptions`. The CMake toolchain does **not** add
`-fdata-sections`; ndk-build does, and the original matches the no-
`-fdata-sections` build (wave-141 `-fdata-sections` A/B: byte_exact
4507 -> 4315, `.rodata` 269,159 -> 268,807).

The checked-in `cmake/toolchain.cmake` is equivalent:

| stock r18b CMake Release | checked-in | note |
|---|---|---|
| `-target armv7-none-linux-androideabi` + sysroot/API | `-target armv7-none-linux-androideabi16` | same API 16; `.note.android.ident` matches |
| `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp` | same | `.ARM.attributes` byte-identical |
| `-g -DANDROID -DNDEBUG -Oz` | same | |
| `-ffunction-sections -funwind-tables` | `target_compile_options(gof2 PRIVATE ...)` | same effect |
| `-fstack-protector-strong` | same | |
| `-std=c++11` (CXX) | same | |
| `-frtti -fexceptions` | `-frtti`; exceptions are clang's default | same effect |
| `-fPIC` | same | `-fpic` is byte-identical on this tree |
| `-no-canonical-prefixes` | absent | driver-only, no object bytes |
| `--fix-cortex-a8` (link) | absent | byte-identical on the unstripped link |
| `--warn-shared-textrel --fatal-warnings` (link) | absent | diagnostics only |
| `--exclude-libs,libgcc.a` + `libatomic.a` (+ `libunwind.a` for c++_static) | `--exclude-libs=libgcc.a` | libatomic/libunwind are not linked; libgcc exclusion is required for the local `__aeabi_ldivmod` |
| `--no-undefined` unless `ANDROID_ALLOW_UNDEFINED_SYMBOLS` | absent | the original has 207 unresolved app imports |

The link itself is a single plain gold link:
`-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`,
no `-s`, followed by the original's GNU `strip --strip-all` and
`tools/reorder_shstrtab.py`. The order-sensitive sections (`.rodata`,
`.data`, `.data.rel.ro`, `.got`, `.dynamic`, `.init_array`, `.fini_array`,
`.plt`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.rel.plt`, `.ARM.exidx`)
are size- and base-exact; `.rodata`, `.data`, `.rel.plt`, `.dynsym`,
`.dynstr`, `.hash`, `.rel.dyn` and `.ARM.exidx` are byte-identical. The only
differing section bytes are `.text` (77,936), `.ARM.extab` (700) and
`.note.gnu.build-id` (20); file size is exact (2,187,328 B) and all 27
section headers are exact.

## 3. Per-TU `-mllvm` knobs: re-measured

The checked-in knobs are matching compensations for residual source-shape
differences, not original build flags. Re-measured at this HEAD by removing
each knob and relinking:

| TU | knob | target | baseline raw/aligned/size | without knob | verdict |
|---|---|---|---|---:|---|
| `StarMap.cpp` | `-likely-branch-weight=7 -unlikely-branch-weight=1` | `StarMap::draw` | 0 / 0 / 2644 | 113 / 17 / 2644 | load-bearing |
| `SpaceLounge.cpp` | `-likely-branch-weight=7 -unlikely-branch-weight=1` | `SpaceLounge::drawLounge` | 175 / 41 / 1948 | 293 / 63 / 1948 | load-bearing |
| `LevelScript.cpp` | `-disable-postra-machine-sink` | `LevelScript::process` | 53082 / 20149 / 57232 | 53084 / 20149 / 57232 | load-bearing (+2 raw) |
| `Radar.cpp` | `-disable-postra-machine-licm` | `Radar::draw` | 6458 / 1832 / 9908 | 6460 / 1823 / 9908 | load-bearing (+2 raw) |
| `Radar.cpp` | `+cheap-predicable-cpsr` | `Radar::draw` | 6458 / 1832 / 9908 | 8396 / 1826 / **9912** | critical (size) |
| `Level.cpp` | `-enable-earlycse-memssa=false` | `Level::createMission` | 4467 / 952 / 17172 | 4467 / 951 / 17172 | raw-neutral at this HEAD |
| `Layout.cpp` | `+prof-unpr` | `Layout::Layout` | 5576 / 2410 / 7072 | 5577 / 2413 / 7072 | load-bearing (+1 raw) |

The Radar knobs interact: removing both gives raw 8404 and size 9912;
removing only `cheap-predicable-cpsr` gives raw 8396 / size 9912, while
removing only `disable-postra-machine-licm` gives raw 6460 / size 9908. The
`cheap-predicable-cpsr` feature is therefore required for the 9,908-byte
shape; the LICM knob is the final 2-byte raw adjustment.

The Level knob is currently raw-neutral (aligned improves by 1 when removed).
It is kept because it was a measured raw win at the time it was added and it
is harmless; removing it is not a size-exact raw win at this HEAD.

No other per-TU flag was found to improve any of the 17 residual functions.
The existing wave-176/177/178 notes cover the per-function source-shape work;
the flag space is exhausted for the current tree.

## 4. `Status::missionCompleted`: switch/TBH body order

### 4.1 Original dispatch

`Status::missionCompleted` is at `0xabf38`, size 1,432. It calls
`Mission::getType()` once and dispatches with two in-text TBH tables:

```text
abfd6: blx  Mission::getType()
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]      @ 40 halfwords at 0xabfe6 (types 150..189)
...
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]      @ 7 halfwords at 0xac052 (types 8..14)
```

The high TBH is indexed by `type-150` in **ascending type order**: entry
`i` is the body for type `150+i`. The low TBH is indexed by `type-8`. The
**bodies**, however, are laid out in the original source case order:

```text
0xac036 167/174
0xac110 150   0xac116 151   0xac11c 152   0xac150 153
0xac156 154   0xac16e 155   0xac180 158   0xac1dc 162
0xac242 166   0xac2a6 170   0xac2b4 184   0xac2ee 189
0xac32a 156   0xac354 157   0xac388 160   0xac3b6 163
0xac3d2 164   0xac3d6 165   0xac3de 168   0xac3fc 169
0xac400 171   0xac41a 172   0xac45a default
```

i.e. `167/174, 150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172, default`.

### 4.2 Why the plain switch lays the bodies out in value order

At `-Oz`, `Function::optForMinSize()` is true, so
`SelectionDAGBuilder::peelDominantCaseCluster` returns early; the 167/174
body is not peeled. It is placed first only because its two case values give
it 2x the frequency of the other bodies (MIR: 8.19% vs 4.10%, default 1.71%).

`SelectionDAGBuilder::buildJumpTable` adds the jump-table successors in
**table order** (`// Add successors. Note: use table order for determinism.`),
and the table is value-sorted by `sortAndRangeify`. All case bodies have the
same frequency, so `MachineBlockPlacement::selectBestSuccessor` /
`selectBestCandidateBlock` follow the successor list and produce value order:

```text
167/174, 150,151,152,153,154,155,156,157,158,160,162,163,164,165,166,
168,169,170,171,172,184,189, default
```

Our current build's MBP dump confirms the value-order successor list:

```text
%jump-table.1: %bb.36 %bb.38 %bb.39 %bb.44 %bb.45 %bb.46 %bb.77 %bb.80
               %bb.52 %bb.103 %bb.85 %bb.103 %bb.47 %bb.88 %bb.91 %bb.92
               %bb.62 %bb.16 %bb.93 %bb.96 %bb.66 %bb.97 %bb.98 ...
# 150,151,152,153,154,155,156,157,158,default,160,default,162,163,...
```

The original's source-order bodies are therefore a
`MachineBlockPlacement` artifact: the original's MBP must have used the
`getFirstUnplacedBlock` fallback (MBB order = source order), or the
jump-table successor list was source order. The current clang 7.0.2 build
uses the value-order successor list.

### 4.3 Source-order mismatch: 158/162

The current source's high cases are in the order
`... 155, 162, 158, 166, ...`; the original body order is
`... 155, 158, 162, 166, ...`. Swapping the two case blocks alone changes
raw **721 -> 722**, aligned **327 -> 322**, size 1432 (still exact). It is
not a raw win, but it is the correct source order and should be kept in mind
for any future MBP-order pass.

### 4.4 Per-TU flag sweep for `Status.cpp`

All variants were compiled with the project's exact flags plus the listed
`-mllvm` options, linked with the real gold link, and measured with
`tools/fn_diff.py`. Baseline is raw 721 / aligned 327 / size 1432.

| variant | raw | aligned | size | note |
|---|---:|---:|---:|---|
| baseline (plain/nested switch) | **721** | **327** | **1432** | value order |
| `-disable-block-placement` | 1294 | 390 | 1436 | not source order; size +4 |
| `-block-placement-exit-block-bias=1` | 721 | 327 | 1432 | neutral |
| `-block-placement-exit-block-bias=5..90` | 753 | 327 | 1432 | worse |
| `-block-placement-exit-block-bias=100` | 1351 | 702 | 1476 | worse |
| `-tail-dup-placement` / `-disable-tail-duplicate` | 721 | 327 | 1432 | neutral |
| `-tail-dup-placement-threshold=0..10` | 721 | 327 | 1432 | neutral |
| `-tail-dup-size=100` | 754 | 327 | 1432 | worse |
| `-branch-fold-placement=0` | 757 | 327 | 1432 | worse |
| `-force-precise-rotation-cost` | 986 | 387 | 1448 | worse |
| `-precise-rotation-cost` | 721 | 327 | 1432 | neutral |
| `-switch-peel-threshold=0..101` | 721 | 327 | 1432 | inert at `-Oz` (`optForMinSize`) |
| `-likely-branch-weight=7..20,80..2000` | 721 | 327 | 1432 | neutral |
| `-likely-branch-weight=5` | 1011 | 328 | 1428 | size -4 |
| `-likely-branch-weight=50` | 893 | 387 | 1436 | worse |
| `-likely-branch-weight=1..3` | 887-899 | 346-356 | 1428 | worse |

The `-disable-block-placement` variant is not the original order either: it
lays the high bodies out in value order with `167/174` last (the high switch
becomes a `tbb` byte table at `0xac4ee`), and the function grows to 1,436 B.
The sequential two-switch form from wave 89 (w3906) reproduces the exact body
order but is 1,508 B; the `__builtin_expect`-nested form reproduces the order
at 1,388-1,396 B but changes the register allocation (see
`status-missioncompleted-source-order-tbh-w3906.md`).

**Conclusion:** no tested per-TU branch-weight or MBP flag reproduces the
original source-order body layout at size 1,432. The remaining unknown is the
original's exact MBP fallback trigger. The hardcoded rank table is the only
known way to force the order at 1,432 B, but it costs the extra `.rodata`
table and is not human-style C++.

## 5. Gate state at this HEAD

```text
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
SYMBOL PARITY ✓ (5370/5370)
size-exact: 4524/4524
```

Section deltas: `.text` 77,936 differing bytes (size exact, 1,259,464 B);
`.ARM.extab` 700 (size/base exact, 156,312 B @ `0x1a3b78`); `.note.gnu.build-id`
20; total 78,656. `.ARM.exidx`, `.rodata`, `.data`, `.rel.plt`, `.dynsym`,
`.dynstr`, `.hash`, `.rel.dyn`, `.comment`, `.note.android.ident`,
`.note.gnu.gold-version` and `.ARM.attributes` are byte-identical. File size
is exact (2,187,328 B); all 27 section headers are exact.

Our canonical build sha256 is
`e118949e4e0a9058f41d7b1e53a8f4da45aca1a1f44742930cf72bbc19103a71`;
the original is
`d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12320
ulimit -v 12000000
cmake --preset default
cmake --build cmake-build-match --target link_so -j2
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target diff -j2
cmake --build cmake-build-match --target lint -j2
cmake --build cmake-build-match --target drift -j2

# toolchain / attribute evidence
ORIG=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
readelf -p .comment "$ORIG"
readelf -x .note.android.ident "$ORIG"
readelf -x .note.gnu.gold-version "$ORIG"
readelf -A "$ORIG"
readelf -SW "$ORIG"

# APK copy (288-byte .shstrtab delta from the ground truth)
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -p "$APK" lib/armeabi-v7a/libgof2hdaa.so > /tmp/apk_gof2.so
cmp -l /tmp/apk_gof2.so "$ORIG" | wc -l   # 288

# Status body order (decode the high TBH at 0xabfe6, target = base + 2*entry)
python3 - <<'PY'
import struct
d=open('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
base=0xabfe6
for i in range(40):
    e=struct.unpack_from('<H',d,base+2*i)[0]
    print(150+i, hex(base+2*e))
PY
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12320/` (`run_variant.sh`,
`per_tu.cmake`, `mbp_mission.txt`, `mir_before_mbp_mission.txt`,
`orig_status.txt`, `ours_status.txt`, `apk/`). No `src/`, CMake or
baseline change was landed; `report.json` was restored to the base.
