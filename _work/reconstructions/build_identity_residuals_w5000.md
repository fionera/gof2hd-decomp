# Build identity residuals: what is left after the flags are proven (w5000)

Status: coordinator research note, 2026-11-04. Base master `44225192c`.
This note answers the user's question "we have no clue if the build flags are correct"
from the **shipped binary itself**, and separates the remaining whole-`.so` deltas into
build-flag, link-order, and source-shape causes. It does not replace
`build_archaeology_w4306.md`; it records the current, independently re-measured residuals.

## 0. Bottom line

The build recipe is **positively proven**, not guessed. On the current tree, every
non-code section except `.rel.plt` and `.note.gnu.build-id` is **byte-identical** to the
shipped `libgof2hdaa.so`:

| section | original | ours | bytes differing |
|---|---:|---:|---:|
| `.dynsym` | 89248 | 89248 | **0** |
| `.dynstr` | 189208 | 189208 | **0** |
| `.hash` | 38716 | 38716 | **0** |
| `.gnu.version*` | 11156/28/64 | same | **0** |
| `.rel.dyn` | 28744 | 28744 | **0** |
| `.plt` | 44108 | 44108 | **0** |
| `.rodata` | 269159 | 269159 | **0** |
| `.data.rel.ro` | 12264 | 12264 | **0** |
| `.init_array` / `.fini_array` | 12 / 8 | 12 / 8 | **0** |
| `.dynamic` | 328 | 328 | **0** |
| `.got` | 17164 | 17164 | **0** |
| `.data` | 10604 | 10604 | **0** |
| `.comment` | 303 | 303 | **0** |
| `.ARM.attributes` | 56 | 56 | **0** |
| `.note.android.ident` | 152 | 152 | **0** |
| `.note.gnu.gold-version` | 28 | 28 | **0** |
| `.shstrtab` | 279 | 279 | **0** |
| `.text` | 1259464 | 1259464 | 101683 |
| `.ARM.exidx` | 28528 | 28528 | 1637 |
| `.ARM.extab` | 156312 | 156308 | 134548 (shifted) |
| `.rel.plt` | 29392 | 29392 | 12 |
| `.note.gnu.build-id` | 36 | 36 | 20 |

The section sizes, addresses, alignments, flags, dynamic tags (`BIND_NOW`, `NOW`,
`RELCOUNT 1162`), NEEDED order, `.ARM.attributes` (ARMv7-A, Thumb-2, VFPv3, NEONv1,
softfp, wchar 4, enum int, unaligned v6), `.comment` (clang 7.0.2 r328903 + GCC 4.9.x),
`.note.android.ident` (API 16, `r18b`), and gold version (1.12) all match. This is only
possible with the same compiler, target, ABI, PIC mode, visibility, stack protector,
unwind tables, STL, and link inputs.

## 1. Proven compile/link recipe

```text
clang++ --target=armv7-none-linux-androideabi16
  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp
  -std=c++11 -fPIC -frtti -fexceptions
  -fstack-protector-strong -funwind-tables -ffunction-sections
  -Oz -g -DANDROID -DNDEBUG
  # ndk-build/CMake r18b defaults: -fno-omit-frame-pointer, -fuse-init-array,
  # -no-canonical-prefixes, -Wa,--noexecstack, no -fdata-sections
  # link: gold 1.12 -shared -soname libgof2hdaa.so --build-id -z now
  #       --exclude-libs=libgcc.a, crtbegin_so.o, objects in original TU order,
  #       --whole-archive libz.a libzip.a, -llog -lGLESv2 -lGLESv1_CM -lEGL
  #       -landroid libfmodex.so libfmodevent.so -lm libc++abi.a
  #       libandroid_support.a libgcc.a -ldl -lc crtend_so.o
  # then GNU bfd strip --strip-all (not gold -s)
```

Positive fingerprints (all re-measured on this tree):

* `-Oz`, not `-Os`/`-O2`: every one of the 192 app TUs is size-exact at `-Oz`; `-Os`
  drops byte_exact to 1263 and `.text` to `0x14bf28` (wave 93 §6).
* `-g` is load-bearing: `-g0` drops byte_exact 4500 -> 2069 at unchanged `.text` size.
  This is DBG_VALUE pinning of entry-block copies, not debug metadata in the shipped file.
* `-fstack-protector-strong`: exactly 756 canaries, same functions/registers.
* frame pointers kept: 2,712/2,712 exact prologues.
* `-funwind-tables`: 3,566 `.ARM.exidx` entries, 272 CANTUNWIND, identical kinds.
* `-frtti`: 176 `_ZTI` / 176 `_ZTS` / 99 `_ZTV` exported, identical.
* `-fPIC` + default visibility: `Tag_ABI_PCS_GOT_use: GOT-indirect`, 0 text relocs,
  `.got`/`.plt`/`.rel.dyn` byte-identical.
* no LTO / ThinLTO / ICF / MergeFunctions: `.text` order and sizes are the plain
  `-ffunction-sections` object order.
* no `-fdata-sections`: `.rodata` is byte-identical only without it; with it `.rodata`
  shrinks `0x41b67 -> 0x41a07` and differs by 113,935 bytes.

Genuinely unobservable (codegen-neutral on this source): `-fPIC` vs `-fpic`,
`-std=c++11` vs `gnu++11`, `-fmerge-constants`, `-fno-plt`,
`-DNDEBUG`/`-DANDROID` (measured neutral), `-fno-sized-deallocation` (byte-identical
except the build-id). They are *unknown*, not disproven.

## 2. `.rel.plt` 12-byte rotation is a source-shape symptom

`readelf -rW` shows 3674 `R_ARM_JUMP_SLOT` entries with identical offsets. Exactly six
`r_info` words differ: `PlayerEgo::deleteHackingGame` is first-referenced five slots too
early (index 2831 instead of 2836), shifting `StarSystem::switchPlanetForIntro`,
`Level::switchSkyboxForIntro`, `ParticleSystemManager::reset`,
`PlayerEgo::startSmokeEmission`, and `PlayerStation::setVisible` down one slot.

Gold allocates PLT entries in first-relocation order; with `-ffunction-sections` the
first reference is the instruction order inside `LevelScript::process`. The original
places the mission-0 state-9 chain (which calls `deleteHackingGame`) **before** the
mission-125 `>= 4` chain; our MachineBlockPlacement places it after. This is not a linker
flag. `levelscript-process-relplt-w4803.md` measured every size-exact shape that restores
the original order: the best is 51.72 % (vs 54.8 % now), i.e. ~1,700 extra `.text` bytes
to save 12 `.rel.plt` bytes. Defer until the `process` body is closer.

## 3. `.ARM.extab` -4 is EH call-site grouping, not code bytes

`.ARM.exidx` has the same 3,566 entries and the **same function-address sequence**
(verified entry-by-entry, including local functions). The `-4` is five LSDA records whose
call-site-table length differs. All five are among the 24 non-exact functions:

| function | exidx addr | score | orig record | ours | delta |
|---|---:|---:|---:|---:|---:|
| `Generator::createMission` | `0x971b8` | 74.3 % | 172 | 176 | +4 |
| `Level::createShip` | `0xc0214` | 74.4 % | 336 | 328 | -8 |
| `LevelScript::process` | `0x137d18` | 54.8 % | 348 | 352 | +4 |
| `HangarWindow::render` | `0x1490d8` | 80.8 % | 948 | 940 | -8 |
| `Hud::draw` | `0x163b90` | 91.7 % | 476 | 480 | +4 |

Net -4. The LSDA call-site tables differ in the split points of cleanup ranges (e.g.
`PaintCanvas::DrawStringColor` has a clean split at `0x108` in the original and `0x10e`
here, same total range), while the instruction bytes are identical. This is an
EH-scope/source-shape residual, not a flag: `-fno-exceptions` does not build (the source
uses `try`), and `-funwind-tables` is required for the 3,566-entry exidx. It follows the
same five function bodies; it is not an independent blocker.

`.ARM.exidx`'s 1,637 differing bytes are the same landing-pad offsets encoded in the
exidx words; they follow the extab/LSDA grouping.

## 4. `.note.gnu.build-id` is the unstripped-output hash

Gold's `--build-id` is SHA-1 over the **entire unstripped output file** with the build-id
descriptor zeroed (`gold/layout.cc:write_build_id`). The shipped file's build-id is not
the SHA-1 of the stripped file (tested: zeroing the descriptor at `0x1dc` gives
`d7c05d…`, not `c01181…`). Since `-g` is load-bearing, the original unstripped output
contains debug info; matching the build-id therefore requires matching the original
debug info (source paths and line numbers) as well as `.text`/`.data`. This is the
last, highest-bar item; it is not a flag question.

## 5. What this means for the remaining work

1. The 24 non-exact functions are the whole `.text` delta; flags are not the blocker.
2. `.rel.plt` will follow `LevelScript::process`'s block order.
3. `.ARM.extab`/`.ARM.exidx` need the original EH scopes in five byte-exact functions.
4. The build-id needs a byte-identical unstripped link, including debug info.
