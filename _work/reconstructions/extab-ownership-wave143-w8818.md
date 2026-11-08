# `.ARM.extab` byte ownership + `--build-id` re-proof (wave 143, w8818)

Status: research note, no landed `src/` change. Worktree `wt/w8818`, base
`bf083ab41` (wave 142). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.

## 1. Every differing `.ARM.extab` byte, by owning function

`.ARM.extab` is size- and base-exact (156,312 B) and `.ARM.exidx` is
byte-identical, so all 1,395 differing bytes are LSDA content. The exidx
`fn` of each record is the *next* function after the dynsym label (the
dynsym label is the previous function; e.g. `HangarWindow::render3D+15` is
`HangarWindow::render`). The 11 records and their owners:

| extab diff | record len | exidx `fn` | owning function | body raw diff |
|---:|---:|---|---|---:|
| **762** | 948 | 0x1490d8 | `HangarWindow::render` | 1740 |
| **279** | 348 | 0x137d18 | `LevelScript::process` | 54691 |
| **138** | 956 | 0xaffd8 | `Level::createMission` | 5756 |
| **70** | 336 | 0xc0214 | `Level::createShip` | 1411 |
| **40** | 280 | 0x17a144 | `MGame::OnTouchEnd` | 4464 |
| **37** | 476 | 0x163b90 | `Hud::draw` | 1362 |
| **24** | 172 | 0x971b8 | `Generator::createMission` | 445 |
| **22** | 132 | 0x12e050 | `Radar::draw` | 8106 |
| **14** | 168 | 0x170d38 | `SpaceLounge::drawLounge` | 175 |
| **7** | 256 | 0x12d484 | `Radar::Radar` | **0** |
| **2** | 112 | 0xd52d0 | `ModStation::ModStation` | 11 |
| **1395** | | | | |

Ten owners still have non-exact bodies; their LSDA rows are a consequence of
the body layout. Only `Radar::Radar` has a byte-exact body. Its 7 bytes are a
pure EH-label tie: rows 1/3/11/13/15 use landing pad 2462 in the original and
2464 in ours, rows 19/21 the reverse. Both landing pads are the identical
`b.n 12de28` stubs at `0x12de22`/`0x12de24`, so the difference is which
identical label the tail-merger kept, not a cleanup-scope difference.

## 2. Largest shape: `HangarWindow::render` rows 19/20

The first LSDA divergence in the 948-byte record is row 19 (call-site
offset 1530 = `0x1496d2`):

| row | original | ours (base) |
|---|---|---|
| 19 | `(1530, 144, lp 9410)` | `(1530, 228, lp 9446)` |
| 20 | `(1712, 46, lp 9358)` | — (merged into row 19) |

The original splits the blueprint-progress block into two cleanup regions:
the first three calls (`getCompletionRate`, `DrawImage2D`, `DrawRegion2D`)
share landing pad 9410; the fourth `DrawImage2D` + `SetColor` use 9358.
Both landing pads are byte-identical (`b.n 14b698` → destroy `sp+312` →
`_Unwind_Resume`), but clang emitted two labels because the fourth call is a
separate source statement. Our inline argument expression puts all five
invokes in one `cleanup` region.

**New hypothesis / measured variant.** Hoisting the border-x expression into
a local immediately before the fourth call reproduces the original split
exactly:

```cpp
int borderX = this->hintOffsetX +
              (textX + 5 + Globals::layout->buttonInsetX +
               (int) (rate * (float) (this->progressBarWidth - 4)));
Globals::Canvas->DrawImage2D(this->progressBarBorderImage, borderX,
                             y + 2 + Globals::layout->field_0x70_rowHeight / 2, 17, 20);
```

Result: rows 19/20 become `(1530,144,9410)` / `(1712,46,9358)` exactly, and
the `HangarWindow::render` record diff drops **762 -> 687**. But the variant
is **not landed**: it changes global register allocation (the credit loop
turns `asrs`+`add` into `mla r6,r3,r0,r1`), which shifts the free-credits
block and leaves its pre-existing row split (`(6826,96,9654)` +
`(7004,30,9600)` vs original `(6824,210,9654)`) in place. The record grows
from 156 to 157 call-site rows and `.ARM.extab` grows **156,312 -> 156,316**
(+4), which violates the size-exact rule. `const int`, a nested block, and
the w8714 draw-first reorder all give the same +4; a plain `{}` block and
`if (true) {}` are folded with no effect.

The remaining lever is the free-credits split: our `if (count == 4)` block
uses the shallower cleanup 9600 (destroy only `sp+312`) while the original
uses 9654 (destroy `sp+300` then `sp+312`). If that split is merged first,
the `borderX` local would be size-exact and would remove 75 extab bytes.

## 3. `.note.gnu.build-id`: algorithm re-proved, pipeline exact

The link is `gold -shared -soname libgof2hdaa.so --build-id -z now ...`,
then GNU `strip --strip-all`, then `tools/reorder_shstrtab.py`. Re-running
the gold link and keeping the unstripped output:

| file | size | stored build-id | SHA-1 whole | SHA-1 descriptor zeroed |
|---|---:|---|---|---|
| our unstripped gold output | 19,345,504 | `75d53e42c9a738dc4aa9afb2ec5358073656d1fb` | `76ce111d...` | **`75d53e42...` (match)** |
| our stripped `.so` | 2,187,328 | `75d53e42...` | `f6492eac...` | — |
| original stripped `.so` | 2,187,328 | `c011810da45777b83ed721b2a87d517efed0d310` | `39528e8a...` | — |

`readelf -n` confirms `NT_GNU_BUILD_ID` type 3, 20-byte descriptor, and the
descriptor is exactly the SHA-1 of the whole unstripped file with the
descriptor zeroed. GNU `strip --strip-all` preserves the note. The original
`c011810d...` is therefore the hash of the original *unstripped* link output
(not the shipped stripped file); the 20-byte difference is a consequence of
the remaining `.text`/`.ARM.extab` and unstripped `.symtab`/`.debug_*`/path
differences, not a different `--build-id` flavour. It will follow `.text` and
`.ARM.extab` to zero once those are exact.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8818
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 _work/reconstructions/exidx_raw_audit.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch (not committed), `/dev/shm/w8818/`: `extab_detail.py`,
`extab_owner.py`, `fdiff.py`, `linkcmd_noremove.sh`, `lsda_decode.txt`,
`lsda_var.txt`.

## 5. Files changed

`_work/reconstructions/extab-ownership-wave143-w8818.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0. `.ARM.extab` 1,395 differing bytes at exact 156,312 B;
`.note.gnu.build-id` 20.
