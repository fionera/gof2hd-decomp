# Section-order removal: ChoiceWindow object order (w3745)

Status: research note + landed source reorder, 2026-11-03. Worktree `wt/w3745`,
branch `wave87-w3745`, based on master `85e9b0b6` (the tip after the
`MovingStars` emission-order pass). Every number below was measured in this
worktree against `../_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's
NDK r18b clang 7.0.2 / gold 1.12.

This note follows `_work/reconstructions/section_order_removal_w3745.md` §17,
which left the plain no-order frontier at `ChoiceWindow::ChoiceWindow()`
(original `0x1467ec`, ours `0x14719c`). It converges the `ChoiceWindow.cpp`
object to the original's function emission order.

## TL;DR

* Baseline at this HEAD: **byte_exact 4497 / linked_exact 4497**, `.text`
  `0x1337c8 @ 0x69440`, `.rodata` `0x41b67 @ 0x1c9e10` (0 differing bytes),
  lint `CLEAN ✓`, drift `0`. (The task brief's 4496 is stale; the current
  baseline is 4497/4497, as recorded in the parent note.)
* Plain no-order baseline: **byte_exact 3118 / linked 4497**, **3407 / 4524**
  exported functions at their original address, exact-address prefix **3179**.
  First mismatch: `ChoiceWindow::ChoiceWindow()` original `0x1467ec`, ours
  `0x14719c`.
* Cause: `ChoiceWindow.cpp` defines its functions in an order that does not
  match the original's emission order. The original emits
  `ctor, dtor, set(String), set(String,String,bool), set(String,String),
  set(8-arg), set(String,bool), removeButtons, hasChoice, setWidth, setHeight,
  setMedal, setMiscButton, update, draw, OnTouchBegin, OnTouchMove, OnTouchEnd,
  left, right, fire`; our object emitted the touch handlers first, the
  constructor near the end, and the destructor after `OnTouchBegin`.
* Fix: reorder the top-level definitions in `src/game/ui/ChoiceWindow.cpp`
  (pure move, no body edit). The static `MEDAL_*` tables move with
  `setMedal`/`draw`; their relative order is unchanged, so `.rodata` stays
  byte-identical.
* Gate after the move: **byte 4497 / linked 4497**, `.text 0x1337c8 @ 0x69440`,
  `.rodata` 0 differing bytes, lint `CLEAN ✓`, drift 0.
* Plain no-order after: **byte_exact 3131 / linked 4497**, **3427 / 4524**
  addresses, prefix **3249**. The whole `ChoiceWindow` block is now
  address-exact (`0x1467ec..0x1475cc`), and every row is `pct == 100.0`.
* The next plain no-order frontier is `HackingGame::~HackingGame()`
  (`0x14ef40` original, `0x14f094` ours), a different TU.

## 1. Baseline and commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Section fingerprint of the gate `.so`:

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78   (original 0x26298; known -0xc delta)
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

Plain no-order measurement (same helper as the parent note):

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/choice/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/choice/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/choice/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/choice/base_noorder.so
```

```text
compared 4524  avg 99.9%  byte_exact 3118  linked_exact 4497  fuzzy_perfect 4497
/tmp/w3745/choice/base_noorder.so: 3407/4524 same; prefix 3179;
  first (1337324, '_ZN12ChoiceWindowC1Ev', 1339612)
```

`1337324 = 0x1467ec` (original `ChoiceWindow::ChoiceWindow()`),
`1339612 = 0x14719c` (ours).

## 2. Original ChoiceWindow order

The original's `.dynsym` address order (`nm -nSD --defined-only`):

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN12ChoiceWindow' | c++filt
```

```text
0x1467ec 0x84  ChoiceWindow::ChoiceWindow()
0x146870 0x56  ChoiceWindow::~ChoiceWindow()
0x1468c8 0x2c  ChoiceWindow::set(String const&)
0x1468f4 0x60  ChoiceWindow::set(String const&, String const&, bool)
0x146954 0x5c  ChoiceWindow::set(String const&, String const&)
0x1469b0 0x3ac ChoiceWindow::set(String const&, String const&, bool,
                              String const&, String const&, String const&, int, int)
0x146d5c 0x38  ChoiceWindow::set(String const&, bool)
0x146d94 0x08  ChoiceWindow::removeButtons()
0x146d9c 0x06  ChoiceWindow::hasChoice()
0x146da4 0x20  ChoiceWindow::setWidth(int)
0x146dc4 0xc8  ChoiceWindow::setHeight(int)
0x146e8c 0x1dc ChoiceWindow::setMedal(int, int)
0x147068 0x14c ChoiceWindow::setMiscButton(String const&)
0x1471b4 0x06  ChoiceWindow::update(int)
0x1471bc 0x310 ChoiceWindow::draw()
0x1474cc 0x4c  ChoiceWindow::OnTouchBegin(int, int)
0x147518 0x4c  ChoiceWindow::OnTouchMove(int, int)
0x147564 0x60  ChoiceWindow::OnTouchEnd(int, int)
0x1475c4 0x02  ChoiceWindow::left()
0x1475c6 0x02  ChoiceWindow::right()
0x1475c8 0x04  ChoiceWindow::fire()
```

The HD source-layout dump independently witnesses the same TU membership and
strong-function order (`_work/original_layout/original_layout.json`,
`Classes/Game/ChoiceWindow.cpp`, object `ChoiceWindow-f9a22f27...o`):

```text
_ChoiceWindow::ChoiceWindow()
_ChoiceWindow::ChoiceWindow()
_ChoiceWindow::~ChoiceWindow()
_ChoiceWindow::~ChoiceWindow()
_ChoiceWindow::set(AbyssEngine::String const&)
_ChoiceWindow::set(AbyssEngine::String const&, AbyssEngine::String const&, bool)
_ChoiceWindow::set(AbyssEngine::String const&, AbyssEngine::String const&)
_ChoiceWindow::set(AbyssEngine::String const&, AbyssEngine::String const&, bool,
                   AbyssEngine::String const&, AbyssEngine::String const&,
                   AbyssEngine::String const&, int, int)
_ChoiceWindow::set(AbyssEngine::String const&, bool)
_ChoiceWindow::hasChoice()
_ChoiceWindow::setWidth(int)
_ChoiceWindow::setHeight(int)
_ChoiceWindow::setMedal(int, int)
_ChoiceWindow::setMiscButton(AbyssEngine::String const&)
_ChoiceWindow::update(int)
_ChoiceWindow::draw()
_ChoiceWindow::OnTouchBegin(int, int)
_ChoiceWindow::OnTouchMove(int, int)
_ChoiceWindow::OnTouchEnd(int, int)
_ChoiceWindow::left()
_ChoiceWindow::right()
```

(`removeButtons` is present in the Android binary at `0x146d94` but absent
from this dump; the other platform dump has it first. The binary is the
authority.)

## 3. Baseline object order

```sh
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/ui/ChoiceWindow.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt
```

```text
ChoiceWindow::OnTouchMove(int, int)
ChoiceWindow::hasChoice()
ChoiceWindow::setWidth(int)
ChoiceWindow::set(String const&, String const&)
ChoiceWindow::set(String const&, String const&, bool, String const&,
                   String const&, String const&, int, int)
ChoiceWindow::set(String const&, bool)
ChoiceWindow::set(String const&, String const&, bool)
ChoiceWindow::OnTouchEnd(int, int)
ChoiceWindow::set(String const&)
ChoiceWindow::removeButtons()
ChoiceWindow::OnTouchBegin(int, int)
ChoiceWindow::~ChoiceWindow()
ChoiceWindow::setHeight(int)
ChoiceWindow::setMedal(int, int)
AbyssEngine::String::String(int)                 # weak, discarded in the plain link
ChoiceWindow::ChoiceWindow()
Array<AbyssEngine::String*>::Array()             # weak, discarded
Array<AbyssEngine::String*>::~Array()            # weak, discarded
ChoiceWindow::setMiscButton(String const&)
ChoiceWindow::update(int)
ChoiceWindow::draw()
ChoiceWindow::left()
ChoiceWindow::right()
ChoiceWindow::fire()
```

The plain no-order link keeps the strong functions in exactly this order
(`0x1467ec..0x1475cc`), which is why the first mismatch is the constructor.

## 4. Emission-order analysis

The source order that produces the target is not simply the target order.
Clang emits a definition when its top-level declaration is reached, and a
called function that has not been emitted yet is emitted immediately after
the caller (the same "first textual reference" rule recorded in the parent
note §8.3 and §17).

Let:

```text
A = ChoiceWindow::ChoiceWindow()
B = ChoiceWindow::~ChoiceWindow()
C = set(String const&)                       (calls D)
D = set(String const&, String const&, bool)  (calls G)
E = set(String const&, String const&)        (calls G)
G = set(8-arg)
F = set(String const&, bool)                 (calls D)
```

The original order is `A B C D E G F ...`. If the source order were
`A B C D E G F`, then `C` would pull `D` forward and `D` would pull `G`
forward before `E`, giving `A B C D G E F`. The original instead has
`A B C D E G`, so the source must define `E` before `D`:

```text
source: A B C E D G F ...
emit:   A B C D E G F ...
        (C pulls D; E pulls G; D's call to G is already satisfied)
```

This was verified by compile-only experiments (same `compile_commands.json`
command, input/output paths substituted) before the full rebuild. The
resulting object order is exactly the original strong-function order; the
only extra sections are the weak `String(int)` / `Array<String*>` copies,
which earlier objects define and gold discards in the plain link.

## 5. The fix

Pure move in `src/game/ui/ChoiceWindow.cpp`, no body edits. New definition
order:

```text
ctor
dtor
set(String)
set(String, String)
set(String, String, bool)
set(8-arg)
set(String, bool)
removeButtons
hasChoice
setWidth
setHeight
MEDAL_TIER_IMAGES_DLC / MEDAL_TIER_IMAGES / MEDAL_IMAGES
setMedal
setMiscButton
update
MEDAL_TIER_COLORS_DLC / MEDAL_TIER_COLORS / MEDAL_TIER_CREDITS
draw
OnTouchBegin
OnTouchMove
OnTouchEnd
left
right
fire
```

The static tables move with their users but keep the same relative order
(`MEDAL_TIER_IMAGES_DLC`, `MEDAL_TIER_IMAGES`, `MEDAL_IMAGES`,
`MEDAL_TIER_COLORS_DLC`, `MEDAL_TIER_COLORS`, `MEDAL_TIER_CREDITS`), so the
object's `.rodata`, `.rodata.cst16` and `.rodata.str1.1` are byte-identical
to the baseline object:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
for s in .rodata .rodata.cst16 .rodata.str1.1; do
  $OD -s -j $s <baseline.obj> > /tmp/base_$s.txt
  $OD -s -j $s <after.obj>    > /tmp/after_$s.txt
  diff /tmp/base_$s.txt /tmp/after_$s.txt && echo "$s SAME"
done
```

All three print `SAME`; the linked `.rodata` is still **0 differing bytes**
against the original.

## 6. Measured A/B

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `a6e134f6…` | **3118** | 3407 / 4524 | 3179 | `ChoiceWindow::ChoiceWindow()` `0x1467ec` vs `0x14719c` |
| reordered (landed) | **4497 / 4497** | `19a4e7f9…` | **3131** | **3427 / 4524** | **3249** | `HackingGame::~HackingGame()` `0x14ef40` vs `0x14f094` |

Delta: **+13 plain byte_exact, +20 functions at their original address,
+70 exact-address prefix**, with the gate counts unchanged. `.text`,
`.rodata`, `.data.rel.ro`, `.data`, `.plt`, `.got` and `.dynamic` are
byte-identical between the baseline and landed gate `.so`; the 408 differing
bytes are `.note.gnu.build-id` (20), `.ARM.exidx` (20) and `.ARM.extab` (368).

After the move the whole `ChoiceWindow` block is address-exact in the plain
no-order link:

```text
001467ec 00000084 T ChoiceWindow::ChoiceWindow()
00146870 00000056 T ChoiceWindow::~ChoiceWindow()
001468c8 0000002c T ChoiceWindow::set(AbyssEngine::String const&)
001468f4 00000060 T ChoiceWindow::set(AbyssEngine::String const&, AbyssEngine::String const&, bool)
00146954 0000005c T ChoiceWindow::set(AbyssEngine::String const&, AbyssEngine::String const&)
001469b0 000003ac T ChoiceWindow::set(AbyssEngine::String const&, AbyssEngine::String const&, bool,
                                          AbyssEngine::String const&, AbyssEngine::String const&,
                                          AbyssEngine::String const&, int, int)
00146d5c 00000038 T ChoiceWindow::set(AbyssEngine::String const&, bool)
00146d94 00000008 T ChoiceWindow::removeButtons()
00146d9c 00000006 T ChoiceWindow::hasChoice()
00146da4 00000020 T ChoiceWindow::setWidth(int)
00146dc4 000000c8 T ChoiceWindow::setHeight(int)
00146e8c 000001dc T ChoiceWindow::setMedal(int, int)
00147068 0000014c T ChoiceWindow::setMiscButton(AbyssEngine::String const&)
001471b4 00000006 T ChoiceWindow::update(int)
001471bc 00000310 T ChoiceWindow::draw()
001474cc 0000004c T ChoiceWindow::OnTouchBegin(int, int)
00147518 0000004c T ChoiceWindow::OnTouchMove(int, int)
00147564 00000060 T ChoiceWindow::OnTouchEnd(int, int)
001475c4 00000002 T ChoiceWindow::left()
001475c6 00000002 T ChoiceWindow::right()
001475c8 00000004 T ChoiceWindow::fire()
```

In the plain verify report every `ChoiceWindow` row is `pct == 100.0` and
`linked == True`. 18 of 23 are `byte == True`; the five `byte == False` rows
(`set(String)`, `set(String,bool)`, `setMiscButton`, `setHeight`, `update`)
differ only in a local tail-call/veneer target at the end of `.text`
(`b.w 0x19c448` original vs `b.w 0x19c468` ours), which the gate masks as a
local (`#X`) call. That is a separate veneer-order issue, not this TU's
function order.

## 7. EH-record evidence

The `.ARM.exidx` extab pointers for the `ChoiceWindow` range now follow the
original's order with the known constant `-4` absolute offset (the global
`.ARM.extab` size delta at this point in the table):

```text
function    original extab   baseline extab   landed extab
0x1467ec    0x1c2cdc         0x1c2df4         0x1c2cd8
0x146870    0x1c2cf8         0x1c2d90         0x1c2cf4
0x1468c8    0x1c2d04         0x1c2d78         0x1c2d00
0x1468f4    0x1c2d10         0x1c2d60         0x1c2d0c
0x146954    0x1c2d1c         0x1c2ce4         0x1c2d18
0x1469b0    0x1c2d28         0x1c2cf0         0x1c2d24
0x146d5c    0x1c2d8c         0x1c2d54         0x1c2d88
0x146dc4    0x1c2d98         0x1c2d9c         0x1c2d94
0x146e8c    0x1c2da4         0x1c2da8         0x1c2da0
0x147068    0x1c2df0         0x1c2e10         0x1c2dec
0x1471bc    0x1c2e10         0x1c2e30         0x1c2e0c
0x1474cc    0x1c2e48         0x1c2d84         0x1c2e44
0x147518    0x1c2e54         0x1c2cd8         0x1c2e50
0x147564    0x1c2e60         0x1c2d6c         0x1c2e5c
```

Every landed pointer is `original - 4`, in the original's ascending order;
the baseline pointers are scrambled. The `.ARM.exidx` differing-byte count
against the original improves **1737 -> 1731**. The `.ARM.extab` total
differing-byte count moves **113811 -> 113842** because the records are now
in the correct order but still carry the global `-4` absolute shift; the
per-record content is identical apart from that shift (the extab record
length deltas are unchanged: the same 8 records, net `-12` bytes).

## 8. Reordered-all control

Re-measured the parent note's control (permute every object's own `.text.*`
sections into `section_order.txt` order, then relink without the ordering
file) with the landed source:

```sh
mkdir -p /tmp/w3745/choice/reordered
python3 - <<'PY'
import os, subprocess
src = 'cmake-build-match/exidx_markers/link_order.rsp'
lines = [l.strip() for l in open(src) if l.strip()]
out = []
for i, p in enumerate(lines):
    dst = f'/tmp/w3745/choice/reordered/{i:03d}_{os.path.basename(p)}'
    subprocess.run(['python3',
                    '_work/reconstructions/w3696/reorder_elf_sections.py',
                    p, dst, 'cmake-build-match/section_order.txt'], check=True)
    out.append(dst)
open('/tmp/w3745/choice/reordered/link_order.rsp', 'w').write('\n'.join(out) + '\n')
PY
python3 _work/reconstructions/w3696/relink_noorder.py \
  /tmp/w3745/choice/reordered/noorder.so /tmp/w3745/choice/reordered/link_order.rsp
# verify.py + addr_match.py as in §1
```

```text
compared 4524  avg 99.9%  byte_exact 4497  linked_exact 4497  fuzzy_perfect 4497
4524/4524 same; prefix 4524; first None
```

So the per-object section permutation still reproduces the ordering-file gate
exactly after the move.

## 9. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  `.rodata` 0 differing bytes, lint `CLEAN ✓`, drift 0.
* The plain no-order baseline is `byte 3118 / linked 4497`, 3407/4524 exact
  addresses, prefix 3179; the first mismatch is `ChoiceWindow::ChoiceWindow()`
  (`0x1467ec` vs `0x14719c`).
* The original's `ChoiceWindow` order is the `.dynsym` order listed in §2,
  independently witnessed by the HD source-layout dump.
* The baseline object emits the touch handlers first, the constructor near
  the end and the destructor after `OnTouchBegin`, which is not the original
  order.
* A pure source reorder (ctor, dtor, `set(String)`, `set(String,String)`,
  `set(String,String,bool)`, `set(8-arg)`, `set(String,bool)`,
  `removeButtons`, `hasChoice`, `setWidth`, `setHeight`, `setMedal`,
  `setMiscButton`, `update`, `draw`, `OnTouchBegin`, `OnTouchMove`,
  `OnTouchEnd`, `left`, `right`, `fire`) produces the original object order.
  The key detail is that `set(String,String)` must be defined before
  `set(String,String,bool)` in the source so that the 3-arg overload is
  pulled forward by `set(String)` and the 8-arg overload is pulled forward
  by `set(String,String)`, giving `D E G` rather than `D G E`.
* The reorder is gate-neutral (`4497 / 4497`, lint `CLEAN ✓`, drift 0) and
  improves the plain no-order build to `3131 / 4497`, 3427/4524 addresses,
  prefix 3249. The whole `ChoiceWindow` block is address-exact.
* `.rodata` stays byte-identical (object-level `.rodata`, `.rodata.cst16`,
  `.rodata.str1.1` and the linked `.rodata`).
* The `ChoiceWindow` `.ARM.exidx` extab pointers now match the original's
  order with a constant `-4` offset; the `.ARM.exidx` differing-byte count
  improves 1737 -> 1731.
* The reordered-all control is still `4497/4497`, 4524/4524.

**Hypothesis / open**

* The five `byte == False` `ChoiceWindow` rows in the plain link are a local
  veneer-target difference (`b.w 0x19c448` vs `0x19c468`), not a
  `ChoiceWindow.cpp` emission-order problem. The veneer table layout is not
  investigated here.
* The next plain no-order frontier is `HackingGame::~HackingGame()`
  (`0x14ef40` original, `0x14f094` ours). `HackingGame.cpp`'s source order is
  already `ctor, reInit, dtor, ...`, but `reInit`'s calls pull
  `rotateRightCW(bool)`, `rotateLeftCW(bool)` and `solvableInNSteps` ahead of
  the destructor. That TU needs its own emission-order pass; it was not
  changed here.
* No claim is made that the ordering file can be dropped now; the plain link
  is still `3131`, and the parent note's weak-copy TU-membership blockers
  remain.
