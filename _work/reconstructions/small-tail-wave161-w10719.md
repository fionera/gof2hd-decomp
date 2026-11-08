# Small tail wave 161 (w10719): MenuTouchWindow tail-block reload lever

Status: **one `src/` change landed** (MenuTouchWindow).  Worktree `wt/w10719`,
base master `01c7f92d6` (wave 160).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w10719`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.text` diff **85,518 -> 85,517**
(-1).  All function sizes exact; `.rel.plt` untouched (byte-identical).

Metrics: raw same-address diff bytes (`/dev/shm/w10719/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10719/align_diff.py`).

| fn | raw before | raw after | aligned before | aligned after | size |
|---|---:|---:|---:|---:|---:|
| `MenuTouchWindow::OnTouchBegin` | 29 | **28** | 15 | 16 | 1896 exact |
| `ModStation::ModStation` C1 | 43 | 43 | 12 | 12 | 1008 exact |
| `Generator::createMission` | 162 | 162 | 64 | 64 | 1996 exact |
| `Gun::calcCharacterCollision` | 463 | 463 | 57 | 57 | 1984 exact |
| `SpaceLounge::drawLounge` | 175 | 175 | 41 | 41 | 1948 exact |

## 1. MenuTouchWindow: the case-15 block is a rematerialisation tie

The whole 1,896-byte body is instruction-identical to the original except
the `case 15` scrollbar block (and one `case 8` `listEntryWidth` load that
is a consequence of it).  The original `case 15` code is:

```
ldr r0,=Globals::w ; ldr r1,=Globals::Canvas
ldr.w r3,[sl,#312] ; heapBufB
ldr.w r6,[sl,#480] ; field_0x1e0
ldr r0,[r0]        ; Globals::w
ldr.w fp,[r1]      ; &Globals::Canvas
ldr r2,[r4]        ; Globals::layout  (r4 = [sp,#16] = &Globals::layout)
ldr.w r1,[r3,r6,lsl#2]
ldr r5,[r0]        ; w
ldr.w r0,[fp]      ; Canvas
ldr r2,[r2,#40]    ; buttonInsetX
subs r5,r5,r2
blx  GetImage2DWidth
...
ldr r2,[sp,#16]    ; &Globals::layout   <-- reloaded
ldr r2,[r2]        ; Globals::layout
ldr r5,[r2,#12]    ; leftMargin
ldr r6,[r2,#32]    ; top
blx  GetImage2DHeight
adds r1,r6,r5
```

i.e. the original source uses `Globals::layout` **directly** in both
conditions (no block-local reference), so `&Globals::layout` is
rematerialised from the stack slot `[sp,#16]` after the call, and `fp`
holds `&Globals::Canvas`.  The previous source used a block-local
`Layout &layout = *Globals::layout;` for `field_0x20_top`, which keeps the
layout pointer live in `r6` across the call and hoists the `top` load
before `GetImage2DHeight`.

Dropping the reference entirely (`Globals::layout` for both fields) is
semantically right and removes the hoist, but the allocator then keeps
`&Globals::layout` in `fp` and rematerialises it with one load instead of
two: the body shrinks to **1,892 B** (gate-fatal).  The landed shape keeps
the reference but moves it to `field_0xc_leftMargin`:

```cpp
Layout &layout = *Globals::layout;
if (x > Globals::w - Globals::layout->buttonInsetX - Globals::Canvas->GetImage2DWidth(...) &&
    y < Globals::layout->field_0x20_top + (layout.field_0xc_leftMargin + Globals::Canvas->GetImage2DHeight(...)))
```

This restores the original's late `top` load (from the reloaded global)
and keeps the size at 1,896 B.  Raw **29 -> 28**; aligned 15 -> 16 (the
greedy alignment now sees one more reordered instruction).  The residual
`case 8` `listEntryWidth` `ip`/`r4` tie is a downstream effect of the same
global allocation.

Variants measured (all size-exact unless noted), raw/aligned/size:

* base (`layout.field_0x20_top`): 29 / 15 / 1896
* landed (`Globals::layout->field_0x20_top`, `layout.field_0xc_leftMargin`): **28 / 16 / 1896**
* both fields via `Globals::layout` (no reference): 358 / 76 / **1892**
* both fields via the reference: 304 / 73 / **1892**
* pointer `Layout *layout = Globals::layout` for `top`: 29 / 15 / 1896
* pointer for the first-condition `buttonInsetX` only: 358 / 76 / 1892
* `PaintCanvas *canvas = Globals::Canvas` local: 312 / 40 / 1892
* `volatile` pointer read (`*(Layout *volatile *)&Globals::layout`): 127 / 76 / 1896
* reordered subtraction `w - width - inset`: 333 / 85 / 1884
* `int top`/`int left` locals: 347 / 42 / 1888
* `Layout **layoutPtr = &Globals::layout`: 358 / 76 / 1892

The irreducible step is the allocator's choice of `fp`: the original
prefers `&Globals::Canvas` (so `&Globals::layout` is rematerialised), the
no-reference source prefers `&Globals::layout`.  No source-only lever found
that flips it while keeping 1,896 B.

## 2. ModStation C1: pool order flips, register pair does not

Confirmed the w10619 finding.  The original literal pool is
`[Pitch, YawIPad, Yaw]` and loads `r3=YawIPad`, `r4=Yaw`; the current
ternary gives `[Pitch, Yaw, YawIPad]` with `r3=YawIPad`, `r4=Yaw` (pair
correct, pool order wrong).  The if-form

```cpp
const float *yawTable = gHangarCamYaw;
if (Globals::iPad) yawTable = gHangarCamYawIPad;
```

flips the pool to `[Pitch, YawIPad, Yaw]` but swaps the pair
(`r4=YawIPad`, `r3=Yaw`), leaving raw/aligned at 43/12, 1,008 B.  Also
tested `!iPad ? Yaw : YawIPad`, `iPad == 0 ? ...`, `iPad != 0 ? ...`, the
`if (!iPad)` form and an if/else form: all 43/12, 1,008 B.  The remaining
tail `ldrh` pair (`r6/r0` vs `r2/r4`) is a second independent RA tie.

## 3. Generator / Gun / SpaceLounge

No new size-exact shape found this wave.

* **Generator**: the `bonus` block still differs at the `volatile` spill
  slot (`[sp,#44]` vs `[sp,#32]`) and the branch shape
  (`cbz` + `sub.w r6,r6,r0,lsl#1` vs `cmp/it ne/subne r5,r6,r0`), and the
  `type|4==12` arm stores `bonus` to `[sp,#16]` immediately in the
  original but keeps it in `r6` in ours.  Raw 162 / aligned 64 / 1,996 B.
* **Gun**: unchanged; the `i*3` r4/r5 Greedy-RA tie and the sret/delta
  pair remain.  Raw 463 / aligned 57 / 1,984 B.
* **SpaceLounge**: unchanged; the `pad`/`x` pair tie remains.  Raw 175 /
  aligned 41 / 1,948 B.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10719
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w10719/fdiff.py <mangled>
python3 /dev/shm/w10719/align_diff.py <mangled>
python3 /dev/shm/w10719/sbs.py <mangled>
```

Scratch (not committed), `/dev/shm/w10719/`: `fdiff.py`, `align_diff.py`,
`sbs.py`, `try.py`..`try8.py`, `trymod.py`, `trymod2.py`,
`MenuTouchWindow.base.cpp`, `ModStation.base.cpp`.

## 5. Files changed

* `src/game/ui/MenuTouchWindow.cpp` (case-15 scrollbar condition: keep the
  original's late `field_0x20_top` reload; move the reference to
  `field_0xc_leftMargin`).
* `_work/reconstructions/small-tail-wave161-w10719.md` (this note).
