# Link-level residuals: `.ARM.extab`, `.rel.plt`, `.note.gnu.build-id` (w7620)

Status: research note + one landed source fix, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7620`. Scratch `/dev/shm/w7620`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result

| section | before | after |
|---|---:|---:|
| `.rel.plt` differing bytes | **0** | **0** |
| `.ARM.exidx` differing bytes | **0** | **0** |
| `.ARM.extab` differing bytes | **1438** | **1436** |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| byte_exact / linked_exact | 4506 / 4506 | 4506 / 4506 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

`.text` is still 85,552 differing bytes (18 non-exact bodies). `.rel.plt`,
`.ARM.exidx`, `.rodata`, `.data`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`,
`.shstrtab` and all 27 section headers are byte-identical.

## 1. `.rel.plt` is already exact

The current build's `.rel.plt` is **byte-identical** to the original (0 differing
bytes of 29,392; `SHF_INFO_LINK`/`sh_info=20` already match). No action needed.

## 2. `.ARM.extab`: every remaining byte is source-level EH shape

`.ARM.exidx` is byte-identical (3,566 entries; CANT 272 / INL 856 / EXT 2,438),
and every `.ARM.extab` record has the exact original length. The 1,438 differing
bytes were **16 records** whose LSDA call-site tables differ (15 records / 1,436
bytes after the landed `StatusWindow` fix below). The record owner is
the function *at* the exidx entry (the nearest preceding dynsym is the previous
function; e.g. the record named `PaintCanvas::DrawString+91` belongs to
`PaintCanvas::DrawStringColor`).

| record owner | body byte-exact? | diff bytes | cause |
|---|---|---:|---|
| `PaintCanvas::DrawStringColor` | **yes** | 3 | extra invoke on `ArrayReleaseClasses<String*>` |
| `PlayerJumpgate::PlayerJumpgate` | **yes** | 15 | landing-pad grouping/assignment |
| `RecordHandler::recordStoreRead` | **yes** | 3 | two cleanup pads swapped (rows 161/162/164) |
| `RecordHandler::readOptionsFileAsByteArray` | **yes** | 3 | `GetFileSize` grouped with `OpenRead` vs later calls |
| `Radar::Radar` | **yes** | 29 | landing-pad grouping/assignment |
| `StatusWindow::~StatusWindow` | **yes** | 2 | `ArrayReleaseClasses<TouchButton>` invoke vs nounwind |
| `Generator::createMission` | no (90.0%) | 27 | body layout |
| `Level::createMission` | no (92.3%) | 139 | body layout |
| `Level::createShip` | no (70.5%) | 70 | body layout |
| `ModStation::ModStation` | no (99.2%) | 2 | body layout |
| `Radar::draw` | no (75.3%) | 29 | body layout |
| `LevelScript::process` | no (60.3%) | 292 | body layout |
| `HangarWindow::render` | no (94.4%) | 762 | body layout |
| `Hud::draw` | no (96.6%) | 40 | body layout |
| `SpaceLounge::drawLounge` | no (95.7%) | 14 | body layout |
| `MGame::OnTouchEnd` | no (81.9%) | 30 | body layout |

The 10 non-exact bodies will close their records automatically when the bodies
become exact. The 6 byte-exact bodies are pure EH-scope/cleanup-shape deltas:
same instructions, different invoke/landing-pad structure. They are **not** a
link-flag or link-order artifact (the sections are size- and base-exact and the
exidx table is byte-identical).

## 3. Landed fix: `StatusWindow::~StatusWindow`

`StatusWindow.cpp` did not include `engine/core/ArrayReleaseClasses.h`, so all
four `ArrayReleaseClasses` calls were `invoke`s. The original treats
`ArrayReleaseClasses<TouchButton>` as nounwind but keeps `<ImagePart>` and
`<String>` as invokes. Adding the include reproduces exactly that per-instantiation
inference (clang 7 marks the non-virtual `TouchButton` destructor call nounwind,
but not the virtual `ImagePart`/`String` destructor calls):

```diff
 #include "game/ui/TouchButton.h"
+#include "engine/core/ArrayReleaseClasses.h"
```

The destructor body stays byte-exact; only its LSDA changes. `.ARM.extab`
1438 -> 1436 differing bytes; gate unchanged (byte 4506 / linked 4506).

The other 5 byte-exact records are subtler cleanup-scope shapes:
* `DrawStringColor` / `readOptionsFileAsByteArray`: our IR has an extra invoke
  (or a different landing-pad grouping) from `ArrayReleaseClasses` /
  `GetFileSize`; the original's declaration is **not** globally `noexcept`
  (adding `noexcept` to `ArrayReleaseClasses` breaks ~1,000 records: extab diff
  jumps to 124,436), so this is a per-TU body-visibility / cleanup-scope effect.
* `recordStoreRead`, `Radar::Radar`, `PlayerJumpgate`: two identical cleanup
  landing-pad stubs are assigned to different call sites. This is the ARM
  backend's tail-merging of landing-pad blocks, driven by the IR cleanup
  structure; it is not controllable by a link flag.

## 4. `.note.gnu.build-id`: algorithm re-proved, pipeline exact

Preserving the unstripped gold output and hashing it:

| file | size | stored build-id | SHA-1 whole | SHA-1 descriptor zeroed |
|---|---:|---|---|---|
| our unstripped gold output | 19,357,872 | `16e141dfd69e10f478ee17353df8ee813b5c7a1e` | `513292a9...` | **`16e141df...` (match)** |
| our stripped `.so` | 2,187,328 | `16e141df...` | `ca35fbe3...` | `48402669...` |
| original stripped `.so` | 2,187,328 | `c011810da45777b83ed721b2a87d517efed0d310` | `39528e8a...` | `d7c05dc5...` |

So gold's `--build-id` is the SHA-1 of the **whole unstripped** output with the
20-byte descriptor zeroed; GNU `strip --strip-all` preserves the note. The
original `c011810d...` is the hash of an unstripped output we do not have, not
the hash of the shipped stripped file. The strip + `reorder_shstrtab.py` pipeline
is exact: `.shstrtab` is byte-identical and all 27 section headers match, so the
remaining 20 bytes are a consequence of the remaining `.text`/`.ARM.extab` (and
unstripped `.symtab`/`.debug_*`/path) differences, not a pipeline bug.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7620
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
python3 _work/reconstructions/exidx_raw_audit.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```
