# `.ARM.extab` split ownership + merge probes (wave 143, w8920)

Status: research note, no landed `src/` change (all merge candidates change a
function size or the `.ARM.extab` section size). Worktree `wt/w8920`, base
`f5093474f` (wave 143). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (source pristine): **RATCHET PASS**, byte **4507**,
linked **4507**, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0. `.ARM.extab` **1,499** differing bytes at exact 156,312 B;
`.text` 85,851; `.rodata`/`.data`/`.bss`/`.ARM.exidx` exact; build-id 20.

Read with `extab-ownership-wave143-w8818.md`, `radar-draw-wave143-w8807.md`,
`radar-draw-wave143-w8808.md`, `hud-draw-wave143-w8814.md`.

## 1. Every differing `.ARM.extab` byte, by owning function

The section is size- and base-exact and `.ARM.exidx` is byte-identical, so all
1,499 differing bytes are LSDA content. Every record has the same **row count**
as the original; only row boundaries / landing-pad offsets differ.

| extab diff | record len | exidx `fn` | owning function | rows | body raw diff |
|---:|---:|---|---|---:|---:|
| **762** | 948 | 0x1490d8 | `HangarWindow::render` | 156=156 | 1731 |
| **279** | 348 | 0x137d18 | `LevelScript::process` | 46=46 | 54619 |
| **158** | 956 | 0xaffd8 | `Level::createMission` | 148=148 | 5597 |
| **93** | 336 | 0xc0214 | `Level::createShip` | 57=57 | 1978 |
| **86** | 132 | 0x12e050 | `Radar::draw` | 19=19 | 8613 |
| **37** | 476 | 0x163b90 | `Hud::draw` | 77=77 | 1325 |
| **37** | 280 | 0x17a144 | `MGame::OnTouchEnd` | 44=44 | 4399 |
| **24** | 172 | 0x971b8 | `Generator::createMission` | 26=26 | 445 |
| **14** | 168 | 0x170d38 | `SpaceLounge::drawLounge` | 25=25 | 175 |
| **7** | 256 | 0x12d484 | `Radar::Radar` | 39=39 | **0** |
| **2** | 112 | 0xd52d0 | `ModStation::ModStation` | 15=15 | 11 |
| **1,499** | | | | | |

`Radar::Radar` is the only byte-exact body; its 7 bytes are the w8818
identical-label tie (lp 2462 vs 2464), not a source shape. `ModStation`'s 2
bytes are a row-length shift (390,30 vs 398,22), i.e. code layout.

## 2. The two real "mergeable splits" (VarDecl lifetime markers)

Both are caused by clang's `EmitAutoVarAlloca` pushing a `CallLifetimeEnd`
(`NormalEHLifetimeMarker`, a non-EH scope) at a local `VarDecl`; the next
`getInvokeDestImpl` then builds a *new* landing pad for the enclosing cleanup
scope. Removing/moving the `VarDecl` merges the two call-site rows.

### 2.1 `HangarWindow::render` rows 125/126 — the free-credits `iconX`/`iconY`

Original: one row `(6824, 210, lp 9654)`.
Ours: `(6826, 96, lp 9654)` + `(7004, 30, lp 9600)`.

The split is the `int iconX` / `int iconY` declarations inside
`if (count == 4 && Globals::showBestDeal)` (they sit between the
`DrawImage2D(tabIcons[count])` and `DrawImage2D(scrollHintImageA)` invokes).
The IR (`clang -Oz -emit-llvm`) has two landing pads (`%2464` / `%2468`) that
both branch to the same String cleanup; the backend emits lp 9600 as a
`b.n 14b68e` trampoline to lp 9654.

**Correction to w8818 §2:** lp 9600 is *not* a shallower cleanup. At
`0x14b658` it is `b.n 0x14b68e`, and `0x14b68e` destroys `sp+300` then
`sp+312` — identical to lp 9654. The two rows are a pure label split.

Measured variants (all keep `.text` byte-identical unless noted):

| variant | fn size | `.ARM.extab` | extab diff | raw |
|---|---:|---|---:|---:|
| base | 9700 | 156312 | 1499 | 1731 |
| `int iconX; int iconY;` at loop top, assign in `if` | **9660** | 156308 | (shifted) | 8579 |
| same, one-line / `= 0` init | **9660** | 156308 | | 8579 |
| decl before the `if` (after tabIcons call) | 9700 | 156312 | 1499 | 1731 |
| inline the two expressions into `DrawImage2D` | **9660** | 156308 | | 8579 |
| unconditional init at loop top (before setPosition) | **9720** | 156312 | | 9089 |
| ternary init at loop top (`count==4 && showBestDeal ? ... : 0`) | **9688** | 156304 | | 9032 |

Merging the split is **-4 extab bytes** but the whole-function register
allocator changes by **-40 B** (the extra live locals at the loop top change
`r8`/`sl`/`r5` from the prologue on). No size-neutral placement found.
The ternary form keeps `.text` section size exact and `.ARM.extab` base exact
(156304, still 16-byte-aligned `.rodata`), but the function is 9688 B, so
`check` reports byte_exact 4507 -> 3743 (the following functions shift).

### 2.2 `Hud::draw` rows 73/74 — the dock2 `float filled`

Original: one row `(8416, 204, lp 9284)` covering `SetColor`..`StringD1Ev`.
Ours (YBTH): `(8420, 42, lp 9280)` + `(8496, 126, lp 9286)`.

The split is the `float filled = progress * (float) halfW;` VarDecl between
`DrawImage2D` and `DrawRegion2D`. IR confirms: declaring `filled` before
`SetColor` (or inlining the expression) makes all five invokes unwind to one
landing pad, and the `filled` computation stays *after* `DrawImage2D` (sunk).

Measured variants:

| variant | fn size | `.ARM.extab` | raw |
|---|---:|---|---:|
| base YBTH (split) | 9332 | 156312 | 1325 |
| THBY faithful order (split) | 9332 | **156316** (+4) | 1352 |
| THBY + inline `filled` | **9368** | 156312 | 8001 |
| THBY + `float filled;` before `SetColor`, assign after DrawImage2D | **9368** | 156312 | 8001 |
| YTHB + inline | **9368** | 156312 | 8001 |
| `float filled;` before `if (show)`, assign after DrawImage2D | **9368** | 156312 | 8001 |

Merging the split is **-4 extab bytes** but the function grows **+36 B**
(extra stack slot; every `[sp,#NN]` shifts by +4). This reproduces w8814 §4.

## 3. The section-size coupling (why +4 alone is fatal)

`.ARM.extab` ends exactly on `.rodata`'s 16-byte-aligned start
(`0x1a3b78 + 0x26298 = 0x1c9e10`). Any +4 growth moves `.rodata` to
`0x1c9e20` and every literal-pool word naming `.rodata` changes.

* `HangarWindow::render` + borderX local (w8818's faithful rows-19/20 split):
  fn **9700 exact**, `.ARM.extab` **156316**, raw **1731 -> 1698**.
  Full `check`: **byte_exact 4507 -> 2939, RATCHET FAIL**.
* borderX + free-credits merge (`iconX` at loop top): fn **9664**,
  `.ARM.extab` **156312 exact**, `.text` 0x1337a8 (-0x20), byte_exact drops.

So a landable fix needs the borderX split (+4) **and** a -4 merge that keeps
the function size. The only -4 merge found (`iconX`/`iconY` at loop top) costs
-40 B of body, and the borderX local only returns +4 of it.

## 4. Open questions / next leads

1. Find a size-neutral placement for `iconX`/`iconY` (or a different -4 merge
   in `HangarWindow::render`) so borderX can land. The original's free-credits
   block is 210 B and ours (merged) is ~170 B; the missing 40 B is a
   whole-function RA change, not a local one.
2. `Hud::draw`: the +36 B merge cost is also RA, not an alloca. Once the
   remaining 1325 raw bytes of the body are reconstructed, the merge may
   become size-neutral; the faithful dock2 order is T,H,B (getText/String,
   GetImage2DWidth, GetImage2DHeight) with the `filled` cleanup merged.
3. `LevelScript::process` (279 extab bytes) and `Level::createMission` (158)
   still have large structural row deltas; they are downstream of their
   non-exact bodies.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8920
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 _work/reconstructions/exidx_raw_audit.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch (not committed), `/dev/shm/w8920/`: `fdiff.py`, `measure.py`,
`hudrows.py`, `variant.py` (Hud::draw dock2 variants), `hwvariant.py`
(HangarWindow free-credits variants), `HW.borderX.cpp`, `Hud.base.cpp`,
`Hud_pre.ll`, `Hud_inline.ll`, `HW.ll`.
