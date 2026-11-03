# TouchButton emission order (w3745)

Status: research note + landed pass. Worktree `wt/w3745`, branch `wave87-w3745`,
based on master `0d2fc121`. Every number below was measured in this worktree
against `../_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's NDK r18b
clang 7.0.2 / gold 1.12.

This is the next TU after the DialogueWindow pass (`09a5b70d`). It reorders the
`TouchButton.cpp` definitions so the object's `.text.*` sections are emitted in
the original's address order. The enforced gate is unchanged; the plain
(no `--section-ordering-file`) link improves.

## 1. Baseline

`cmake --preset default`, then `cmake --build cmake-build-match --target check -j8`
(plus `lint` and `drift`):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

The brief's 4496/4496 baseline is stale; this HEAD is **4498 / 4498**.

Gate ELF layout (`readelf -SW cmake-build-match/libgof2hdaa.so`):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78   (original: 0x26298, the known -12)
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

Plain no-order baseline (`relink_noorder.py` + `verify.py` + `addr_match.py`):

```text
compared 4524  avg 99.91%  byte_exact 2989  linked_exact 4498  fuzzy_perfect 4498
3811/4524 same; prefix 3769;
first (0x1684f0, TouchButton::TouchButton(String const&, int, int, int, unsigned char),
       0x16979c)
```

## 2. Original TouchButton address order

From the original's `.dynsym`:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN11TouchButton'
```

```text
0x1684f0  TouchButton(String const&, int, int, int, unsigned char)         0xa8
0x168598  TouchButton::init(String const&, uint, int, int, int, int, int, int, uchar, uchar) 0x718
0x168cb0  TouchButton(uint, int, int, int, unsigned char)                 0xec
0x168d9c  TouchButton(uint, uint, int, int, int, unsigned char)           0xec
0x168e88  TouchButton(String const&, int, int, int, int, uchar, uchar)    0xa8
0x168f30  TouchButton(String const&, int, int, int, int, uchar, uchar,
                      uint, int)                                          0xcc
0x168ffc  TouchButton(uint, int, int, int, int, uchar, uchar)             0xe8
0x1690e4  TouchButton(int, int, String const&, int, int, unsigned char)   0xa4
0x169188  ~TouchButton()                                                  0x28
0x1691b0  setText(String const&)                                          0xf8
0x1692a8  setPosition(int, int, unsigned char)                            0x50
0x1692f8  setNumberText(String const&)                                    0x06
0x1692fe  setGamePadButtonImage(uint)                                     0x04
0x169304  replaceTextKeepSize(String const&)                              0x48
0x16934c  setSplitText(String const&)                                     0x06
0x169352  getText()                                                       0x0e
0x169360  getWidth()                                                      0x06
0x169366  getHeight()                                                     0x06
0x16936c  setPosition(int, int)                                           0x08
0x169374  setYPosition(int)                                               0x0c
0x169380  getPosition()                                                   0x1e
0x16939e  setHalfTransparent(bool)                                        0x06
0x1693a4  setVisible(bool)                                                0x06
0x1693aa  isVisible()                                                     0x06
0x1693b0  translate(int, int)                                             0x0e
0x1693be  setAlwaysPressed(bool)                                          0x06
0x1693c4  setTextColor(int)                                               0x06
0x1693ca  setPressProgress(float)                                         0x06
0x1693d0  setPressProgressHighlight(bool)                                 0x06
0x1693d8  draw()                                                          0x560
0x169938  touchedInside(int, int)                                         0x90
0x1699c8  resetTouch()                                                    0x08
0x1699d0  isTouched()                                                     0x06
0x1699d8  OnTouchBegin(int, int)                                          0x50
0x169a28  OnTouchMove(int, int)                                           0x2c
0x169a54  OnTouchEnd(int, int)                                            0x50
```

The `C1`/`C2` aliases are adjacent duplicates of the same address; the table
lists each signature once.

## 3. Baseline object order

Before the pass, `TouchButton.cpp.obj` emitted:

```text
ctor(uint,int,int,int,int,uchar,uchar)
init
setVisible
setPosition(int,int,uchar)
translate
setNumberText
~TouchButton
OnTouchMove
touchedInside
isVisible
setPressProgress
isTouched
getText
replaceTextKeepSize
setSplitText
getPosition
setTextColor
setAlwaysPressed
resetTouch
OnTouchBegin
OnTouchEnd
getWidth
setPosition(int,int)
setGamePadButtonImage
setHalfTransparent
setPressProgressHighlight
getHeight
setText
draw
ctor(String,int,int,int,int,uchar,uchar)
setYPosition
ctor(uint,uint,int,int,int,uchar)
ctor(String,int,int,int,uchar)
ctor(String,int,int,int,int,uchar,uchar,uint,int)
ctor(int,int,String,int,int,uchar)
ctor(uint,int,int,int,uchar)
```

This is the source definition order, with two forward-reference pulls:
`ctor(uint,...7 args)` pulls `init` immediately after itself, and `OnTouchMove`
pulls `touchedInside`.

## 4. Source reorder

The definitions were reordered to the original address order:

```text
ctor(String,int,int,int,uchar)
init
ctor(uint,int,int,int,uchar)
ctor(uint,uint,int,int,int,uchar)
ctor(String,int,int,int,int,uchar,uchar)
ctor(String,int,int,int,int,uchar,uchar,uint,int)
ctor(uint,int,int,int,int,uchar,uchar)
ctor(int,int,String,int,int,uchar)
~TouchButton
setText
setPosition(int,int,uchar)
setNumberText
setGamePadButtonImage
replaceTextKeepSize
setSplitText
getText
getWidth
getHeight
setPosition(int,int)
setYPosition
getPosition
setHalfTransparent
setVisible
isVisible
translate
setAlwaysPressed
setTextColor
setPressProgress
setPressProgressHighlight
draw
touchedInside
resetTouch
isTouched
OnTouchBegin
OnTouchMove
OnTouchEnd
```

The static `g_TB_*` tables stay immediately before `init` (they are unchanged;
`.rodata` is byte-identical before and after).

### The one non-obvious placement: `~TouchButton` before `init`

`ctor(String,int,int,int,uchar)` is defined first and calls `init`, so clang
emits `init` immediately after that ctor. `init`'s body calls `setText` and
`setPosition(int,int,uchar)`; clang emits those deferred callees at `init`'s
**definition** point, not at its emitted address. Therefore the dtor must be
defined before `init` in the source:

- source order `..., ctor8, dtor, init, setText, setPosition3, ...`
  gives object order `ctor1, init, ctor3..ctor8, dtor, setText, setPosition3, ...`
  (the original order);
- source order `..., ctor8, init, dtor, setText, ...` puts `setText` and
  `setPosition3` before the dtor.

The landed file uses the first form.

## 5. Object order after

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ui/TouchButton.cpp.obj`
now lists the 36 strong sections in exactly the original address order:

```text
ctor(String,int,int,int,uchar)
init
ctor(uint,int,int,int,uchar)
ctor(uint,uint,int,int,int,uchar)
ctor(String,int,int,int,int,uchar,uchar)
ctor(String,int,int,int,int,uchar,uchar,uint,int)
ctor(uint,int,int,int,int,uchar,uchar)
ctor(int,int,String,int,int,uchar)
~TouchButton
setText
setPosition(int,int,uchar)
setNumberText
setGamePadButtonImage
replaceTextKeepSize
setSplitText
getText
getWidth
getHeight
setPosition(int,int)
setYPosition
getPosition
setHalfTransparent
setVisible
isVisible
translate
setAlwaysPressed
setTextColor
setPressProgress
setPressProgressHighlight
draw
touchedInside
resetTouch
isTouched
OnTouchBegin
OnTouchMove
OnTouchEnd
```

## 6. Gate A/B

Both builds are the full `check` gate. The reorder is gate-neutral:

| build | byte_exact | linked_exact | .text | .rodata | lint | drift |
|---|---:|---:|---:|---:|---|---|
| baseline | 4498 | 4498 | 0x1337c8 @ 0x69440 | 0x41b67 @ 0x1c9e10 | CLEAN ✓ | 0 |
| TouchButton reorder | **4498** | **4498** | 0x1337c8 @ 0x69440 | 0x41b67 @ 0x1c9e10 | CLEAN ✓ | 0 |

Gate `.so` A/B (`cmp` of the two stripped links): only these sections differ:

| section | differing bytes |
|---|---:|
| `.note.gnu.build-id` | 20 |
| `.ARM.exidx` | 29 |
| `.ARM.extab` | 403 |

`.text` (1,259,464 B) and `.rodata` (269,159 B) are **byte-identical** between the
baseline and reordered gate `.so`, so the gate `report.json` is unchanged.

Against the original, the same sections move as follows:

| section | baseline diff bytes | reordered diff bytes |
|---|---:|---:|
| `.text` | 107,393 | 107,393 |
| `.rodata` | 0 | 0 |
| `.ARM.exidx` | 1,737 | **1,724** |
| `.ARM.extab` | 128,098 | 128,117 |

The `.ARM.extab` raw byte count rises by 19 because the records are now in the
original order but still carry the pre-existing cumulative `-8` offset (our
`.ARM.extab` is 0x2628c vs the original 0x26298). For every `TouchButton`
function with an extab record, the reordered gate `.so` has the original's
record order with a uniform `-8`:

```text
function                         original extab   reordered extab   baseline extab
ctor(String,int,int,int,uchar)   0x23e34          0x23e2c           0x23f4c
init                             0x23e54          0x23e4c           0x23e58
ctor(uint,int,int,int,uchar)     0x23e7c          0x23e74           0x23fb4
ctor(uint,uint,...)              0x23ea8          0x23ea0           0x23f00
ctor(String,...,uchar,uchar)     0x23ed4          0x23ecc           0x23ed0
ctor(String,...,uint,int)        0x23ef4          0x23eec           0x23f4c
ctor(uint,...,uchar,uchar)       0x23f14          0x23f0c           0x23dfc
ctor(int,int,String,...)         0x23f40          0x23f38           0x23f64
~TouchButton                     0x23f60          0x23f58           0x23e50
setText                          0x23f64          0x23f5c           0x23e90
draw                             0x23f4c          0x23f44           0x23e6c
touchedInside                    0x23f50          0x23f48           0x23e28
OnTouchBegin                     0x23f4c          0x23f44           0x23e30
OnTouchMove                      0x23f50          0x23f48           0x23e04
OnTouchEnd                       0x23f54          0x23f4c           0x23e2c
```

The `-8` is the pre-existing cumulative extab record-size delta, not caused by
this pass. The `.ARM.exidx` improvement (-13 differing bytes) is the corrected
exidx targets/extab pointers for the reordered block.

## 7. Plain no-order A/B

Same `relink_noorder.py` + `verify.py` + `addr_match.py` commands:

| plain no-order link | byte_exact | linked_exact | funcs at orig addr | exact-address prefix | first mismatch |
|---|---:|---:|---:|---:|---|
| baseline | 2989 | 4498 | 3811 / 4524 | 3769 | `TouchButton(String,int,int,int,uchar)` `0x1684f0` vs `0x16979c` |
| TouchButton reorder | **3001** | 4498 | **3858 / 4524** | **3817** | `SpriteGun::render()` `0x169abc` vs `0x169ace` |

Delta: **+12 plain byte_exact, +47 functions at their original address,
+48 exact-address prefix**.

Every `TouchButton` exported function (36 strong sections; 46 `.dynsym` entries
including the `C1`/`C2` aliases) is now at its original address in the plain
no-order link:

```sh
$NM -nD --defined-only /tmp/w3745_tb/final_noorder.so | grep '_ZN11TouchButton'
# 0x1684f0 ctor1 ... 0x169a54 OnTouchEnd, every address equal to the original's
```

The next plain no-order frontier is `SpriteGun::render()`:

```text
original 0x169abc  SpriteGun::render()
ours     0x169ace  SpriteGun::render()
```

The `SpriteGun` block itself starts at the original address (`0x169aa4`), but
its internal order is `setEnemies, update, setEnemy, render` instead of the
original `render, update, setEnemies, setEnemy`. That is a separate TU and was
not changed in this pass.

## 8. Proven vs hypothesis

**Proven by measurement**

- Baseline gate is **4498 / 4498**, `.text 0x1337c8 @ 0x69440`, `.rodata
  0x41b67 @ 0x1c9e10`, lint `CLEAN ✓`, drift 0.
- Baseline plain no-order is **2989 / 4498**, 3811 / 4524 at original
  addresses, prefix 3769, first mismatch `TouchButton(String,int,int,int,uchar)`
  `0x1684f0` vs `0x16979c`.
- The original `TouchButton` address order is the 36-definition order in §2.
- The reordered source makes the object's `.text.*` section order exactly that
  order (§5).
- `ctor(String,int,int,int,uchar)` pulls `init` after itself; `init`'s callees
  are emitted at `init`'s definition point, so `~TouchButton` must be defined
  before `init` in the source to land before `setText`/`setPosition` (§4).
- The reorder is gate-neutral: **4498 / 4498**, every ELF section size/base
  unchanged, `.text` and `.rodata` byte-identical to the baseline gate `.so`,
  lint `CLEAN ✓`, drift 0.
- The plain no-order link improves to **3001 / 4498**, 3858 / 4524 at original
  addresses, prefix 3817, and every `TouchButton` function is address-exact.
- `.ARM.exidx` differing bytes improve 1,737 -> 1,724; `.ARM.extab` records are
  now in the original order with the pre-existing uniform `-8` offset.

**Hypothesis / open**

- The ordering file still cannot be removed: the plain no-order build is 3001,
  not 4498.
- `SpriteGun.cpp` is the next per-TU source-order experiment (same technique:
  order `render, update, setEnemies, setEnemy`).
- The `.ARM.extab` raw byte delta is the pre-existing cumulative record-size
  shortfall; it will converge when the remaining mismatched bodies are fixed,
  not by this pass.

## 9. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + verify
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_tb/final_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_tb/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_tb/final_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_tb/final_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ui/TouchButton.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN11TouchButton'
```

The source reorder is a pure permutation of the 36 top-level definitions plus
the unchanged `g_TB_*` static block; no function body, signature, comment or
table value was edited.
