# Build provenance at wave 160 (w10620): ELF metadata is byte-identical; the
# residual is not a build flag

Status: **research note only**, 2026-11-08. Worktree `wt/w10620`, base master
`08af4633b` (wave 159). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Scratch `/dev/shm/w10620`. No `src/`, `tools/`, `CMakeLists.txt`, baseline,
allowlist or doc change is landed; this note is the only commit.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` **85,722** differing bytes (size
1,259,464 B), `.ARM.extab` **1,256** (size 156,312 B), `.rel.plt` 0,
`.note.gnu.build-id` 20. All 27 section sizes exact; file size 2,187,328 B.

## 0. Bottom line

1. **The toolchain provenance is proven, not inferred.** Every non-code ELF
   byte is already identical: `.comment`, `.note.android.ident`,
   `.note.gnu.gold-version`, `.ARM.attributes`, `.shstrtab`, `.dynsym`,
   `.dynstr`, `.hash`, `.gnu.version{,_d,_r}`, `.rel.dyn`, `.rel.plt`, `.plt`,
   `.rodata`, `.data.rel.ro`, `.init_array`, `.fini_array`, `.dynamic`,
   `.got`, `.data`, `.bss`, `.ARM.exidx`, plus the ELF header, program
   headers and dynamic section. The original is the same NDK r18b clang
   7.0.2 build (`Android (4751641 based on r328903)`) + GCC 4.9.x
   20150123 + gold 1.12, exactly as the recipe says.
2. **`-fomit-frame-pointer` is unobservable at `-Oz`.** `-fno-omit-frame-pointer`
   is a *no-op* on all six residual TUs (object `.text` identical) and on a
   synthetic test function; both binaries contain exactly 220 `mov r7, sp`
   frame-pointer prologues. The flag cannot explain any residual.
3. **`.ARM.extab` 1,256 differing bytes are exactly the LSDA of 12 functions**
   (the 11 residual bodies with EH scopes plus `Radar::Radar(Level*)`, whose
   `.text` is byte-exact). The extab residual is a downstream consequence of
   `.text`/cold-block placement, not a separate flag/TU issue.
4. **No new per-TU flag improves a residual.** 40+ new cells (below); every
   size-preserving mover is neutral or worse on both metrics, and every
   size-changing mover is gate-fatal. This extends wave 158/159 from
   whole-build and opt-level sweeps to per-TU *codegen* flags.

## 1. Provenance evidence (byte-identical, measured)

Section byte diffs of the canonical build vs the original:

| section | size | diff bytes |
|---|---:|---:|
| `.note.android.ident` | 152 | 0 |
| `.note.gnu.build-id` | 36 | 20 |
| `.comment` | 303 | 0 |
| `.note.gnu.gold-version` | 28 | 0 |
| `.ARM.attributes` | 56 | 0 |
| `.shstrtab` | 279 | 0 |
| `.dynsym` | 89,248 | 0 |
| `.dynstr` | 189,208 | 0 |
| `.hash` | 38,716 | 0 |
| `.gnu.version` | 11,156 | 0 |
| `.gnu.version_d` / `_r` | 28 / 64 | 0 |
| `.rel.dyn` | 28,744 | 0 |
| `.rel.plt` | 29,392 | 0 |
| `.plt` | 44,108 | 0 |
| `.text` | 1,259,464 | **85,722** |
| `.ARM.exidx` | 28,528 | 0 |
| `.ARM.extab` | 156,312 | **1,256** |
| `.rodata` | 269,159 | 0 |
| `.data.rel.ro` | 12,264 | 0 |
| `.init_array` / `.fini_array` | 12 / 8 | 0 |
| `.dynamic` | 328 | 0 |
| `.got` | 17,164 | 0 |
| `.data` | 10,604 | 0 |
| `.bss` | 27,052 | 0 |

`readelf -h`, `readelf -lW` and `readelf -dW` produce **zero diff lines**
between the two files (same `e_flags`, same 4 program headers, same
`DT_*` set/order).

Exact strings (byte-identical in both):

```
.comment: \0Android (4751641 based on r328903) clang version 7.0.2
  (https://android.googlesource.com/toolchain/clang 003100370607242ddd5815e4a043907ea9004281)
  (https://android.googlesource.com/toolchain/llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
  (based on LLVM 7.0.2svn)\0GCC: (GNU) 4.9.x 20150123 (prerelease)\0
.note.android.ident: Android / NT_VERSION, "r18b", "5063045" (API 16)
.note.gnu.gold-version: GNU / gold 1.12
.ARM.attributes: 41 37 00 00 00 61 65 61 62 69 00 01 2d 00 00 00 05 41 52 4d
  20 76 37 00 06 0a 07 41 08 01 09 02 0a 03 0c 01 11 02 12 04 14 01 15 01
  17 03 18 01 1a 02 1b 03 22 01 26 01
```

Section alignments also match: `.text` `2**5` (32), `.rodata`/`.data`/`.bss`
`2**4` (16), `.ARM.extab` `2**2` (4), `.ARM.exidx` `2**2` (4).

**Implication.** `-fomit-frame-pointer` (indistinguishable), `-fvisibility`,
`-fno-rtti`, `-fno-threadsafe-statics`, `-fdata-sections`, `-fno-common`,
`-mno-movt`, `-fno-math-errno` cannot be inferred from metadata but are all
*neutral* on the residual bodies (section 3), so none can be the cause of a
residual. `-fno-exceptions`, `-fno-stack-protector` and `-fno-unwind-tables`
are excluded by size (wave 158: whole-build `.text` collapses to 1,168,804 /
1,237,288 / same-1,259,464-but-raw-worse respectively).

## 2. `.ARM.extab` decomposition

Parsing `.ARM.exidx` (3,566 entries, byte-identical) and mapping each
non-compact extab entry to its function shows the 1,256 differing extab
bytes belong to exactly 12 entries:

| function | extab diff bytes |
|---|---:|
| `HangarWindow::render` | 676 |
| `LevelScript::process` | 182 |
| `Level::createMission` | 159 |
| `Level::createShip` | 77 |
| `Radar::draw` | 50 |
| `Hud::draw` | 37 |
| `MGame::OnTouchEnd` | 30 |
| `SpaceLounge::drawLounge` | 14 |
| `Generator::createMission` | 13 |
| `MGame::OnUpdate` | 9 |
| `Radar::Radar(Level*)` | 7 |
| `ModStation::ModStation` (C2) | 2 |
| **total** | **1,256** |

The first eleven are the residual `.text` bodies. `Radar::Radar(Level*)` is
`.text`-byte-exact yet its LSDA landing-pad offsets are uniformly `+2`
(0x139e -> 0x13a0): the landing pad lives in a cold block whose placement
depends on other cold-section sizes in the TU, i.e. still a `.text`
consequence. **No extab difference exists outside the residual TU bodies.**

## 3. New controlled per-TU flag experiments

Method: compile one TU with the canonical command + extra flags (object-level
filter; `objdiff` = differing bytes of the residual function body in the
`.o`, `sizedelta` = size change), then full `link_so` + `verify` for every
size-preserving mover. Baseline raw/aligned (whole residual set):
`85,722 / 23,484` over 15 unique bodies; `.text` 1,259,464 B.

Object-level filter (6 residual TUs: LevelScript, Layout, Radar, MGame,
Level, Status). Flags that are **exactly neutral** on all six residual
functions (0 object bytes): `-fno-omit-frame-pointer`, `-fno-rtti`,
`-fno-threadsafe-statics`, `-fno-common`, `-mno-movt`, `-fno-math-errno`,
`-fno-asynchronous-unwind-tables`, `-fno-delete-null-pointer-checks`,
`-fno-jump-tables` (except Status/Level), `-mllvm -enable-gvn-hoist=false`.
Flags that change codegen but change size (gate-fatal): `-ffast-math`,
`-fno-strict-aliasing`, `-fwrapv`/`-fno-strict-overflow` (MGame),
`-fno-elide-constructors`, `-fno-exceptions`, `-fno-inline(-functions)`,
`-fno-builtin`, `-fno-stack-protector`, `-fno-optimize-sibling-calls`,
`-mllvm -static-likely-prob=*`, `-mllvm -enable-load-pre=false`,
`-fno-honor-nans` (LevelScript/MGame), `-fno-signed-zeros`,
`-fno-jump-tables` (Status/Level), `-fvisibility=hidden` (Level only).

Linked results for every size-preserving mover (raw / aligned / size):

| TU | flags | raw | aligned | size | byte_exact |
|---|---|---:|---:|---:|---:|
| — | baseline | 85,722 | 23,484 | exact | **4507** |
| Radar | `-fwrapv` | 8,124 | 1,320 | 9,908 | 4507 |
| Radar | `-fno-strict-overflow` | (same as `-fwrapv`) | | | |
| Radar | `-ffast-math` | 8,124 | 1,326 | 9,908 | 4503 |
| Radar | `-mno-unaligned-access` | 8,205 | 1,326 | 9,908 | 4479 |
| Radar | `-fno-honor-nans` | 8,124 | 1,322 | 9,908 | 4506 |
| Status | `-fno-exceptions` | 809 | 320 | 1,432 | 1498 |
| Status | `-mllvm -enable-load-pre=false` | 752 | 320 | 1,432 | 2305 |
| Level | `-fno-honor-nans` | 5,654 (createMission) | 711 | exact | 4482 |
| LevelScript | `-fno-builtin` | 55,080 | 17,067 | exact | 3689 |
| LevelScript | `-mllvm -disable-ifcvt-diamond` | 55,056 | 17,060 | exact | 4507 |
| LevelScript | `-fno-jump-tables` | 55,859 | 24,244 | 57,552 | 2834 |
| Layout | `-mllvm -disable-ifcvt-diamond` | 5,749 | **1,976** | 7,072 | 4504 |
| MGame | `-fno-optimize-sibling-calls` | 7,244 | 2,153 | 7,904 | 3080 |
| MGame | `-fvisibility=hidden` | function becomes local (dynsym changes) | | | 1670 |

Best cells: `Radar -fwrapv` is exactly neutral on both metrics (its 1-byte
object change is a literal encoding, not a body move); `Layout
-disable-ifcvt-diamond` trades aligned -2 for raw +10 and loses 3 byte_exact.
Nothing improves both metrics, and nothing raises `byte_exact`.

Negative controls that re-confirm wave 158/159 at the per-TU level:
`-fno-exceptions` (Status, size-exact body but raw 722 -> 809),
`-fno-jump-tables` (LevelScript, size +320), `-fno-optimize-sibling-calls`
(MGame, size -176). The MGame tail-call-thunk hypothesis is *not* explained
by sibling-call optimization: disabling it makes `OnTouchEnd` smaller and
much worse (7,244/2,153 vs 4,387/901).

## 4. Conclusion / open questions

The build recipe is now evidenced end-to-end: same clang build string, same
gold, same ARM attributes, same section alignments, same dynsym/relocs/
rodata/data/bss/exidx, same frame-pointer usage. The 85,722 `.text` residual
bytes and the 1,256 `.ARM.extab` bytes are **source-shape**, not flags. The
remaining open questions are the wave-159 ones (block order / shared tails /
`__builtin_expect` placement), plus the new observation that
`Radar::Radar(Level*)`'s LSDA shifts with cold-block placement even though its
`.text` is byte-exact — a useful probe for cold-section ordering.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10620
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# object-level flag filter (fast): compile one TU with the ninja command +
# extra flags, compare the residual function's own section bytes
python3 /dev/shm/w10620/objfilter.py /dev/shm/w10620/spec1.json 6 out.json
python3 /dev/shm/w10620/objcmp.py out.json cmp.json

# linked verification of a size-preserving mover
python3 /dev/shm/w10620/linktest.py /dev/shm/w10620/spec_lt4.json 2

# extab decomposition
python3 /dev/shm/w10620/extab2.py
```

## 6. Files changed

* `_work/reconstructions/build-provenance-wave160-w10620.md` (this note).
* No source, tool, CMake, baseline, allowlist or doc change.
