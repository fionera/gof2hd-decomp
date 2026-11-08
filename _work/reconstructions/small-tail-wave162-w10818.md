# Small tail wave 162 (w10818): MenuTouchWindow case-15 `top` rematerialisation

Status: **one `src/` change landed** (MenuTouchWindow).  Worktree
`wt/w10818`, base master `29248c1d0` (wave 161).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w10818`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.text` diff **85,374 -> 85,373**
(-1).  `.rel.plt` byte-identical; `.rodata`/`.data`/`.bss`/`.ARM.exidx`
byte-identical; `.ARM.extab` 725 unchanged.  All function sizes exact.

Metrics: raw same-address diff bytes (`/dev/shm/w10818/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10818/align_diff.py`).

| fn | raw before | raw after | aligned before | aligned after | size |
|---|---:|---:|---:|---:|---:|
| `MenuTouchWindow::OnTouchBegin` | 29 | **28** | 15 | 16 | 1896 exact |
| `Gun::calcCharacterCollision` | 463 | 463 | 57 | 57 | 1984 exact |
| `SpaceLounge::drawLounge` | 175 | 175 | 41 | 41 | 1948 exact |
| `ModStation::ModStation` C1 | 43 | 43 | 12 | 12 | 1008 exact |
| `Generator::createMission` | 162 | 162 | 64 | 64 | 1996 exact |

## 1. MenuTouchWindow: case-15 `field_0x20_top` must come from the reloaded global

Wave 161's `small-tail-wave161-w10719.md` recorded a 28/16 shape that was
not present on master 29248c1d0 (master measured 29/15).  The shape is the
one the note describes: keep the `Layout &layout` reference but use it for
`field_0xc_leftMargin`, while **both** `buttonInsetX` and `field_0x20_top`
come from `Globals::layout` directly:

```cpp
Layout &layout = *Globals::layout;
if (x > Globals::w - Globals::layout->buttonInsetX - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
    y < Globals::layout->field_0x20_top + (layout.field_0xc_leftMargin + Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])))
```

Master had `layout.field_0x20_top` (reference for `top`, direct for
`leftMargin`).  Flipping the two accesses restores the original's
rematerialisation of `&Globals::layout` for the `top` load and removes one
byte: raw **29 -> 28**, aligned 15 -> 16, 1,896 B exact.  The residual is
the case-8 `listEntryWidth` `ip`/`r4` tie plus the case-15 register
permutation (`r3/r6/r2/r5` vs `r2/r3/r6/r4`), both Greedy-RA ties that no
source-only lever found this wave.

## 2. New hypotheses measured, not landed

* **ModStation C1**: confirmed the wave-161 finding.  Base ternary is
  43/12/1,008 B with `r3=YawIPad`, `r4=Yaw` (correct pair) but pool
  `[Pitch, Yaw, YawIPad]` and `ldr r4` first.  The `if (iPad) yawTable =
  YawIPad` form flips the pool to `[Pitch, YawIPad, Yaw]` and loads
  YawIPad first, but the pair swaps (`r4=YawIPad`, `r3=Yaw`), which is
  semantically inverted.  Also tested `!iPad ? Yaw : YawIPad`,
  `iPad == 0 ? Yaw : YawIPad`, `iPad > 0`, a `bool` local, two pointer
  locals, `yawTable[race]` for the far key (984 B, gate-fatal) — all 43
  or worse.  The tie is the *register* the first yaw load gets (r3 vs r4),
  not the pool order.
* **Generator**: `nonvol rem` 627/59, `rawBonus - rem` 177/116 but 2,000 B
  (gate-fatal), `rem + rem` 583/64 (2,000 B), `int bonus = 0` 162/64,
  `volatile bonus` 171/155 (1,996 B) or 657/144 (2,004 B), `volatile
  rawBonus` 611/47 (2,000 B).  Base 162/64 stays.
* **SpaceLounge**: `x = w - inset - (width + pad + pad)` gives 176/42 with
  the original's `itt / addgt.w r1,r6,sl / subgt` clamp shape, but the
  condition sum reassociates to `(width + pad) + x`; explicit parentheses
  do not recover `(pad + x) + width`.  Base 175/41 stays.
* **Gun**: `positions[i]` vs `data()[i]`, `*(data()+i)`, a
  `reinterpret_cast<char*>` byte-offset form, pointer locals, swapped
  load order, `int i`: all 463 or worse (478/485).  The `i*3` r4/r5
  Greedy-RA tie is unchanged.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10818
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w10818/fdiff.py _ZN15MenuTouchWindow12OnTouchBeginEiiPv
python3 /dev/shm/w10818/align_diff.py _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

Scratch (not committed), `/dev/shm/w10818/`: `fdiff.py`, `align_diff.py`,
`sbs.py`, `resolve.py`, `mtwscan*.py`, `slscan*.py`, `gscan*.py`,
`guntry.py`, and the `.base.cpp` copies.

## 4. Files changed

* `src/game/ui/MenuTouchWindow.cpp` (case-15 scrollbar condition: `top`
  from `Globals::layout`, reference kept for `field_0xc_leftMargin`).
* `_work/reconstructions/small-tail-wave162-w10818.md` (this note).
