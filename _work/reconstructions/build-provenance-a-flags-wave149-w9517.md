# Build provenance A: independent compile/link flag re-derivation (wave 149, w9517)

Status: **note-only research commit**, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w9517`, base `fece77a64` ("Wave 148 docs: Radar
aligned 1,657, Layout 2,213, createMission 716, aligned total 22,945").
Scratch: `/dev/shm/w9517`. No `src/`, `tools/`, `CMakeLists.txt`, baseline,
allowlist or doc change is landed. This note is the only commit.

Mission: the user has "no clue whether the flags are correct". This pass
re-derives the compile/link flag set **from the shipped `.so` itself** and then
runs fresh full-build and per-TU A/B cells for hypotheses not previously
measured. Every cell is a real NDK r18b clang 7.0.2 compile + gold link +
GNU strip. The answer is unchanged: **the canonical flag set is the
original's, and no new cell raises `byte_exact`.**

Gate at the base (fresh `cmake --build cmake-build-match --target check -j6`):

```text
verify: byte 4507/4524  linked 4507/4524  size 4524/4524  avg 99.93
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
RATCHET PASS
```

Section deltas of the canonical build (independently re-measured this pass):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | **86,225** |
| `.ARM.extab` | 156,312 | **1,309** |
| `.rel.plt` | 29,392 | **4** (`setVisible`/`deleteHackingGame` swap) |
| `.note.gnu.build-id` | 36 | **20** (build-dir dependent) |
| `.ARM.exidx`, `.rodata`, `.data`, `.bss`, `.got`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.dynamic`, `.comment`, `.ARM.attributes`, `.shstrtab`, all others | exact | **0** |

The 17 residual functions are the same set as wave 148. The two function-level
metrics at the base:

* raw same-address diff sum over the 17 residuals: **92,011** bytes;
* instruction-index alignment (`align_diff.py`), per-function sum **25,170**
  non-matching instructions; alias-deduplicated (Layout C1/C2 and ModStation
  C1/C2 counted once) = **22,945**, exactly the wave-148 doc total.

---

## 0. Bottom line

1. **The shipped ELF independently pins the compile flags.** `.ARM.attributes`
   is byte-identical to our build (ARM v7 / Thumb-2 / VFPv3 / NEONv1 /
   GOT-indirect / wchar 4 / enum int / unaligned v6, no optimisation or
   virtualisation tag). `.comment` is clang 7.0.2 + GCC 4.9.x. The notes are
   r18b / API 16 / gold 1.12. All 27 section headers (size, address, offset,
   alignment, entsize) are byte-identical.
2. **The unwind tables pin `-funwind-tables` + `-fexceptions`.** The original
   has 3,566 `.ARM.exidx` entries: 856 compact, 2,710 inline-pointer, **0
   `EXIDX_CANTUNWIND`**, and a 156,312-byte `.ARM.extab`. Our build reproduces
   every one of those counts.
3. **The literal-pool / constant-island layout matches.** `ldr rX,[pc]` 14,577
   vs 14,576; `vldr sX,[pc]` 1,054 vs 1,055; `add rX,pc` 11,809 vs 11,815.
   The few differences are inside the residual functions.
4. **52 valid full-build cells and 52 per-TU cells were measured.** No cell
   exceeds `byte_exact 4507`; no cell keeps 4507 while changing `.text` or
   `.ARM.extab` except the byte-identical neutral cells.
5. **New unobservable flags** (byte-identical output, no section change):
   `-fmerge-all-constants`, `-fno-merge-constants` (clang 7 ignores it),
   `-fno-vectorize`, `-mllvm -arm-promote-constant=false` (the default),
   `-mllvm -arm-promote-constant-max-size=0`,
   `-mllvm -arm-promote-constant-max-total=0`,
   `-mllvm -arm-constant-island-max-iteration=10` (the default),
   `-mllvm -arm-global-merge` (default on),
   `-mllvm -arm-adjust-jump-tables` (default on),
   `-mllvm -arm-use-mulops` (default on),
   `-mllvm -arm-implicit-it=always|never`,
   `-mllvm -arm-no-restrict-it`, `-mllvm -enable-load-pre`,
   `-mllvm -enable-cond-stores-vec`,
   `-mllvm -enable-interleaved-mem-accesses`,
   `-mllvm -enable-subreg-liveness`, `-mllvm -enable-post-misched`,
   `-mllvm -enable-shrink-wrap`, `-mllvm -enable-if-conversion`,
   `-mllvm -enable-earlycse-memssa`, `-mllvm -enable-loop-distribute`,
   `-mllvm -enable-pipeliner`, `-mllvm -enable-simple-loop-unswitch`,
   `-mllvm -enable-store-refinement`, `-mllvm -enable-phi-of-ops`.
6. **New falsifications:** `-fshort-enums` (2,871; `Level.cpp` alone is the
   discriminator at 2,367), `-fno-exceptions` (1,344), `-fno-rtti` (1,701),
   `-fomit-frame-pointer` (1,312), `-momit-leaf-frame-pointer` (1,390),
   `-fno-unwind-tables` (2,878), `-fno-inline-functions` (1,219),
   `-mno-unaligned-access` (1,367), `-fno-slp-vectorize` (1,484),
   `-fno-threadsafe-statics` (1,368), `-fno-stack-protector` (1,369),
   `-fno-strict-aliasing` (1,371), `-fvisibility=hidden` (57),
   `-mllvm -arm-promote-constant` (1,386),
   `-arm-global-merge=false` (1,388),
   `-arm-adjust-jump-tables=false` (1,571),
   `-arm-use-mulops=false` (1,472), `-arm-restrict-it` (1,278),
   `-enable-tail-merge` (2,677), `-enable-gvn-hoist` (1,437),
   `-enable-aa-sched-mi` (1,414), `-enable-partial-inlining` (1,483),
   `-disable-branch-fold` (1,355), `-arm-constant-island-max-iteration=2`
   (4,451).

---

## 1. Independent evidence from the shipped ELF

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
2,187,328 B, `sha256 d4c17be6…bed9483`. NDK r18b
`arm-linux-androideabi-readelf`/`objdump` and the repo's `tools/verify/elf.py`.

### 1.1 `.ARM.attributes` — byte-identical, decoded

```text
Tag_CPU_name: "ARM v7"        Tag_CPU_arch: v7
Tag_CPU_arch_profile: Application
Tag_ARM_ISA_use: Yes          Tag_THUMB_ISA_use: Thumb-2
Tag_FP_arch: VFPv3            Tag_Advanced_SIMD_arch: NEONv1
Tag_ABI_PCS_GOT_use: GOT-indirect
Tag_ABI_PCS_wchar_t: 4        Tag_ABI_enum_size: int
Tag_ABI_align_needed: 8-byte  Tag_CPU_unaligned_access: v6
Tag_ABI_HardFP_use: Deprecated
Tag_ABI_FP_denormal/exceptions/number_model/16bit_format: Needed/IEEE 754
```

No `Tag_ABI_optimization_goals`, no `Tag_Virtualization_use`. This is a
`-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp` build with default
`-fshort-enums` **off** (`enum int`) and `-mno-unaligned-access` **off**
(`v6`). `-mfpu=neon-vfpv4` would add `Tag_FP_arch: VFPv4` and the Fused-MAC
tag; it is absent.

### 1.2 `.comment`, notes, build-id

* `.comment` is exactly the r18b clang 7.0.2 string (offset 1) followed by
  `GCC: (GNU) 4.9.x 20150123 (prerelease)` (offset 108) — the latter is the
  vendored `libcrypto_gof2.a` `sha256.o` member. Our `.comment` is
  byte-identical.
* `.note.android.ident`: `api=0x10` (16), `ndk="r18b"`, `build="5063045"`,
  byte-identical to `crtbegin_so.o`'s note.
* `.note.gnu.gold-version`: `gold 1.12` (the r18b binutils).
* `.note.gnu.build-id`: 20 bytes (`sha1` style). As wave 146 proved, the
  shipped id is **not** the SHA-1 of the shipped (stripped) file, so the link
  was unstripped and a separate GNU strip ran afterwards. Our build-id differs
  only because the build directory enters the unstripped `.debug_*` bytes.

### 1.3 Section headers and alignment

All 27 section headers match on `sh_type`, `sh_addr`, `sh_offset`, `sh_size`,
`sh_addralign` and `sh_entsize`:

```text
.text      size 0x1337c8 @ 0x69440  align 32
.ARM.exidx size 0x006f70 @ 0x19cc08 align 4   (8-byte entries)
.ARM.extab size 0x026298 @ 0x1a3b78 align 4
.rodata    size 0x041b67 @ 0x1c9e10 align 16
.data      size 0x00296c @ 0x214000 align 16
.bss       size 0x0069ac @ 0x216970 align 16
```

`.text` 32-byte alignment is clang's default `-falign-functions=32`-class
layout at `-Oz`; `-falign-functions=4` was already neutral in wave 87.

### 1.4 `.ARM.exidx` / `.ARM.extab` encodings

| observable | original | ours |
|---|---:|---:|
| `.ARM.exidx` entries | 3,566 | 3,566 |
| compact entries (bit 31 set) | 856 | 856 |
| inline-pointer entries | 2,710 | 2,710 |
| `EXIDX_CANTUNWIND` (word0 == 1) | 0 | 0 |
| `.ARM.extab` size | 156,312 | 156,312 |

This is a positive witness for `-funwind-tables` on **every** TU and for
`-fexceptions` (personality routines referenced from the extab records).
`-fno-unwind-tables` removes the exidx entries and shrinks `.ARM.exidx`
28,528 → 26,232 (measured below).

### 1.5 Literal-pool / constant-island layout

| pattern | original | ours |
|---|---:|---:|
| `ldr rX, [pc, #N]` | 14,577 | 14,576 |
| `vldr sX, [pc, #N]` | 1,054 | 1,055 |
| `add rX, pc, …` | 11,809 | 11,815 |

The constant-island pass is on and converges to the same layout; the handful
of differences are inside the 17 residual functions. `-arm-promote-constant`
(true) moves 6,528 bytes of `.rodata` into `.text` and drops `byte_exact` to
1,386, so the default (promotion off) is the original's.

### 1.6 Codegen fingerprints

| flag | observable | original | ours |
|---|---|---:|---:|
| `-fstack-protector-strong` | `__stack_chk_fail` sites | 788 | 788 |
| `-fomit-frame-pointer` off | `add r7, sp, #N` | 2,473 | 2,473 |
| " | `mov r7, sp` | 220 | 220 |
| " | `push {…, r7, …}` | 2,706 | 2,705 |
| `-fno-slp-vectorize` off | `vpush` / `vpop` | 232 / 265 | 232 / 265 |
| `-fno-builtin` off | `__aeabi_idiv` / `__aeabi_uidiv` | 96 / 43 | 96 / 43 |
| `-mno-movt` off | `movt` | 26 | 25 |
| `-fno-threadsafe-statics` off | `__cxa_guard_*` | 4 | 4 |

The one-instruction differences (`push`, `movt`) are inside residual
functions. Every global flag is therefore confirmed by a direct count, not
only by an A/B.

---

## 2. Full-build A/B matrix (new cells)

Method: one scratch build directory (`/dev/shm/w9517/b1`) configured with a
compiler launcher that appends the cell flags to the canonical line; delete
all app objects and rebuild `link_so` for every cell (the vendored archives
are reused). `byte_exact` is the exact raw-byte comparison of every
`.dynsym` function defined in both binaries (the same definition as
`verify.py`); `raw` is the same-address differing-byte sum over the residual
functions. Control: `byte_exact 4507`, `raw 92,011`, sections exact.

| flag | byte_exact | raw | section deltas | verdict |
|---|---:|---:|---|---|
| `(control)` | **4507** | 92,011 | none | canonical |
| `-fmerge-all-constants` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-fno-merge-constants` | **4507** | 92,011 | none | **UNOBSERVABLE** (clang 7 ignores) |
| `-fno-vectorize` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -arm-promote-constant=false` | **4507** | 92,011 | none | **UNOBSERVABLE** (default) |
| `-mllvm -arm-promote-constant-max-size=0` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -arm-promote-constant-max-total=0` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -arm-constant-island-max-iteration=10` | **4507** | 92,011 | none | **UNOBSERVABLE** (default) |
| `-mllvm -arm-global-merge` | **4507** | 92,011 | none | **UNOBSERVABLE** (default on) |
| `-mllvm -arm-adjust-jump-tables` | **4507** | 92,011 | none | **UNOBSERVABLE** (default on) |
| `-mllvm -arm-use-mulops` | **4507** | 92,011 | none | **UNOBSERVABLE** (default on) |
| `-mllvm -arm-implicit-it=always` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -arm-implicit-it=never` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -arm-no-restrict-it` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -enable-load-pre` | **4507** | 92,011 | none | **UNOBSERVABLE** (default on) |
| `-mllvm -enable-cond-stores-vec` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -enable-interleaved-mem-accesses` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -enable-subreg-liveness` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -enable-post-misched` | **4507** | 92,011 | none | **UNOBSERVABLE** (default on) |
| `-mllvm -enable-shrink-wrap` | **4507** | 92,011 | none | **UNOBSERVABLE** (default on) |
| `-mllvm -enable-if-conversion` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -enable-earlycse-memssa` | **4507** | 92,011 | none | **UNOBSERVABLE** (default on) |
| `-mllvm -enable-loop-distribute` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -enable-pipeliner` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -enable-simple-loop-unswitch` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -enable-store-refinement` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-mllvm -enable-phi-of-ops` | **4507** | 92,011 | none | **UNOBSERVABLE** |
| `-fshort-enums` | 2871 | 143,039 | none | falsified |
| `-fno-exceptions` | 1344 | 51,470 | `.text` −90,948, `.ARM.extab` −125,580 | falsified |
| `-fno-rtti` | 1701 | 157,208 | `.rodata` −2,841, `.data.rel.ro` −972 | falsified |
| `-fomit-frame-pointer` | 1312 | 37,450 | `.text` −23,072, `.ARM.extab` −19,880 | falsified |
| `-momit-leaf-frame-pointer` | 1390 | 181,835 | `.text` −352, `.ARM.extab` −612 | falsified |
| `-fno-unwind-tables` | 2878 | 107,818 | `.ARM.exidx` −2,296, `.ARM.extab` −3,384 | falsified |
| `-fno-inline-functions` | 1219 | 74,076 | `.text` +34,784 | falsified |
| `-mno-unaligned-access` | 1367 | 179,764 | `.text` −736 | falsified |
| `-fno-slp-vectorize` | 1484 | 140,928 | `.text` −2,336 | falsified |
| `-fno-threadsafe-statics` | 1368/4521 | 171,946 | `.text` −324, `.rodata` −432 | falsified (3 symbols vanish) |
| `-fno-stack-protector` | 1369 | 78,839 | `.text` −22,240 | falsified |
| `-fno-strict-aliasing` | 1371 | 127,921 | `.text` +1,248 | falsified |
| `-fvisibility=hidden` | 57/244 | 2,129 | `.dynsym` 89,248→12,576 | falsified |
| `-mllvm -arm-promote-constant` | 1386 | 102,815 | `.text` +5,216, `.rodata` −6,528 | falsified |
| `-mllvm -arm-global-merge=false` | 1388 | 197,130 | `.text` +356, `.data` −32 | falsified |
| `-mllvm -arm-adjust-jump-tables=false` | 1571 | 192,075 | `.text` +480 | falsified |
| `-mllvm -arm-use-mulops=false` | 1472 | 138,807 | `.text` −512 | falsified |
| `-mllvm -arm-restrict-it` | 1278 | 144,158 | `.text` −832 | falsified |
| `-mllvm -arm-constant-island-max-iteration=2` | 4451 | 92,783 | none | falsified |
| `-mllvm -arm-constant-island-max-iteration=1` | build fail | — | — | "Constant Island pass failed to converge!" |
| `-mllvm -enable-tail-merge` | 2677 | 132,891 | `.text` +224 | falsified |
| `-mllvm -enable-gvn-hoist` | 1437 | 118,093 | `.text` −1,152 | falsified |
| `-mllvm -enable-aa-sched-mi` | 1414 | 154,988 | `.text` +32 | falsified |
| `-mllvm -enable-partial-inlining` | 1483 | 166,610 | `.text` +128 | falsified |
| `-mllvm -disable-branch-fold` | 1355 | 113,048 | `.text` +3,712 | falsified |
| `-mllvm -enable-machine-outliner` | build fail | — | — | backend failure |

No cell has `byte_exact > 4507`. Every neutral cell is byte-identical to the
control (same `.text`, `.ARM.extab`, `.rodata`, `.data`, `.bss`).

---

## 3. Per-TU A/B matrix (new cells)

Method: one object restored from a pristine copy, recompiled with the cell
flag appended after the canonical line, full relink; the object is restored
after each cell. `=` means byte-identical to control (`4507`, raw 92,011,
sections exact).

### 3.1 `-fno-exceptions` (new: all 13 residual TUs)

| TU | byte_exact | raw | section deltas |
|---|---:|---:|---|
| Layout.cpp | 1369 | 190,880 | `.text` −416, `.ARM.extab` −540 |
| LevelScript.cpp | 2133 | 81,760 | `.text` −448, `.ARM.extab` −596 |
| Radar.cpp | 1369 | 154,329 | `.text` +96, `.ARM.extab` −660 |
| Level.cpp | 1369 | 226,914 | `.text` −4,608, `.ARM.extab` −10,452 |
| Status.cpp | 1498 | 229,577 | `.text` −2,592, `.ARM.extab` −2,364 |
| MGame.cpp | 1369 | 154,046 | `.text` −1,184, `.ARM.extab` −1,644 |
| Gun.cpp | 1369 | 160,154 | `.text` −64, `.ARM.extab` −84 |
| HangarWindow.cpp | 2058 | 136,403 | `.text` −1,120, `.ARM.extab` −2,260 |
| SpaceLounge.cpp | 2143 | 127,404 | `.text` −4,512, `.ARM.extab` −5,784 |
| Hud.cpp | 2121 | 125,704 | `.text` −1,440, `.ARM.extab` −1,960 |
| Generator.cpp | 2026 | 170,846 | `.text` −160, `.ARM.extab` −384 |
| MenuTouchWindow.cpp | 1369 | 182,755 | `.text` −1,024, `.ARM.extab` −2,424 |
| ModStation.cpp | 1484 | 202,080 | `.text` −1,120, `.ARM.extab` −1,916 |

Every TU uses EH cleanup code; `-fno-exceptions` is falsified on all 13.

### 3.2 `-fshort-enums` (new: all 13 residual TUs)

Neutral (`=`) on 12 of 13 TUs. **`Level.cpp` is the discriminator**:
`byte_exact 2367`, raw 140,711, `.text` 1,259,464 → 1,259,496 (+32), all other
sections exact. This is direct evidence that the original's enums are int-sized
(matching `Tag_ABI_enum_size: int`), and that the enum-using code lives in the
`Level.cpp` TU.

### 3.3 `-mllvm -arm-constant-island-max-iteration=2` (new: all 13 TUs)

Neutral on 12 TUs. **`Status.cpp` is the discriminator**: `byte_exact 4454`,
raw 92,605, sections exact (the literal-pool rotation changes bytes without
changing sizes). The whole-build cell is 4,451, so a few other TUs also move.
The default (10) is the original's.

### 3.4 `-fomit-frame-pointer` (new: all 13 residual TUs)

| TU | byte_exact | raw |
|---|---:|---:|
| Layout.cpp | 2446 | 169,507 |
| LevelScript.cpp | 2728 | 80,589 |
| Radar.cpp | 2690 | 121,259 |
| Level.cpp | 2457 | 194,921 |
| Status.cpp | 2285 | 197,886 |
| MGame.cpp | 3084 | 101,482 |
| Gun.cpp | 2871 | 118,549 |
| HangarWindow.cpp | 2758 | 125,880 |
| SpaceLounge.cpp | 2973 | 110,879 |
| Hud.cpp | 2926 | 116,991 |
| Generator.cpp | 2214 | 167,194 |
| MenuTouchWindow.cpp | 2667 | 138,724 |
| ModStation.cpp | 2470 | 175,753 |

Frame pointers are on in every residual TU.

---

## 4. Conclusions

* **Compile line (confirmed):** `-target armv7-none-linux-androideabi16
  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC
  -frtti -fexceptions -fstack-protector-strong -funwind-tables
  -ffunction-sections -Oz -g -DANDROID -DNDEBUG -stdlib=libc++`, default
  visibility, default strict aliasing, `-fwrapv` off, threadsafe statics on,
  `-fcommon`, loop vectorizer off at `-Oz`, SLP on, frame pointers on,
  unaligned access on, enum int, constant islands on with default promotion
  off, global merge on, jump-table adjustment on, `restrict-it` off.
* **Link line (confirmed):** single plain gold 1.12 link
  `-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`
  (no `-s`, no version script, no section-ordering file), then GNU bfd strip.
  `.rel.plt` is size- and base-exact but 4 bytes differ (the known
  `setVisible`/`deleteHackingGame` entry swap), which is a source-order issue,
  not a flag issue.
* **No new flag raises `byte_exact`.** The residual 17 functions are source
  shape, not build configuration. The new "unobservable" list above is useful
  as a negative: those flags cannot be excluded as *used but neutral*, but
  there is no positive evidence for any of them, and the canonical set is the
  simplest line consistent with every ELF observable.

---

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9517
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# scratch A/B harness (not committed)
# /dev/shm/w9517/toolchain_extra.cmake includes cmake/toolchain.cmake and sets
# CMAKE_{C,CXX}_COMPILER_LAUNCHER=/dev/shm/w9517/launch.sh; launch.sh appends
# $GOF2_EXTRA_FLAGS (whole build) and, when the source basename matches
# $GOF2_TU, $GOF2_TU_FLAGS. Score with /dev/shm/w9517/score.py (raw .dynsym
# byte compare, identical to verify.py's byte_exact).
```

Scratch (not committed): `/dev/shm/w9517/` — `b1/` (scratch build),
`pristine2/` (control objects), `score.py`, `launch.sh`,
`toolchain_extra.cmake`, `batch_whole.sh`, `batch_tu.sh`,
`cells_whole.txt`, `cells_mllvm.txt`, `cells_tu.txt`,
`whole_results.txt`, `tu_results.txt`, `align_diff.py`.

## 6. What would falsify this note

* A full-build or per-TU cell with `byte_exact > 4507` and no section-size
  change. None of the 104 new cells does.
* A cell that keeps `byte_exact 4507` but changes `.text` or `.ARM.extab`.
  None does (the neutral cells are byte-identical).
* An ELF observable that contradicts the canonical set (different
  `.ARM.attributes`, a `CANTUNWIND` entry, a different canary/guard/RTTI/
  frame-pointer/literal-pool count). The fresh census in §1 shows none.
