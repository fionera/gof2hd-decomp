# `HangarWindow::render` — pass 13 (w5009): `right > 0`, `pad3` removal audit

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave100-w5009`, base master `44225192c`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra image base
0x10000; ELF addresses below are the real ones).

This pass follows up `hangarwindow-render-w4811.md`.  It lands one faithful source fix
(`if (right)` → `if (right > 0)`) and exhaustively re-audits the `pad3` volatile anchor:
removing it is *not* recoverable by any real source shape tested — the plain original
block scores 0.7912, while the dead volatile store is what pins the list-body register
roles (`this -> r6`, `amount -> r9`).

## TL;DR

- **Landed:** the second `if` in the selected-item `left`/`right` block changes from
  `if (right)` to `if (right > 0)`.  The original disassembly is `cmp.w r9,#1; blt`
  (`0x149ce4`), i.e. a `> 0` test; the bare truthiness form compiled to
  `cmp.w r9,#0; beq`.  This is a real source-shape fix, not an anchor.
  Raw match `0.8082059755 -> 0.8084813438` with an unchanged 9700-byte body.
- **`pad3` removal audit:** the original `if (0 < iVar12)` block is exactly the plain
  source block (Ghidra `0x159cac..0x159cc6`), no local and no store.  Removing the
  `volatile int pad3 = this->field_0x114;` anchor gives 9692 B / 0.7912 and flips the
  body roles (`this -> r5`, `amount -> r6`); every real replacement tested (cached
  `buttonY`, per-block `y2`, `yOff`, `TouchButton *` locals, `buttons` locals, eager
  `layout` reads, declaration moves, initialiser/type/placement sweeps) is neutral or
  worse.  The anchor stays; its comment is unchanged.
- The `.ARM.extab` residual is unchanged at `0x26294` (−4); removing `pad3` would make
  it `0x26288` (−16), another reason to keep it.

## Gate (before -> after)

```
verify: byte 4500 -> 4500  linked 4500 -> 4500  stubs 0  missing 0  extra 0  wrong_type 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
sections: .text 0x1337c8, .rodata 0x41b67, .ARM.exidx 0x6f70, .ARM.extab 0x26294 (-4)
render: 9700/9700 B, raw 0.8082059755 -> 0.8084813438, insns orig/ours 3626/3637, frame 376
```

Build (canonical):

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

## 1. The landed change

`src/game/ui/HangarWindow.cpp`, in the `if (item == this->selectedItem)` block:

```cpp
// before
if (right) {
// after
if (right > 0) {
```

Original `0x149ce4`:

```
149ce4: f1b9 0f01  cmp.w r9, #1
149ce8: db1d       blt.n 149d26
```

Ours before was `cmp.w r9,#0; beq.n`; after it is `cmp.w r9,#1; blt.n`, matching the
original.  The first block already used `if (left > 0)` and already matched
(`0x149ca8: cmp r0,#1; blt`).  The second `left` block (`0x149f70`) and the
`tab == 4 || right > 0` guard (`0x14a088: cmp.w sl,#1; blt`) already matched.

Only the `cmp` changed; the surrounding body is byte-for-byte the same shape, so the
function stays 9700 B and every later function keeps its address.

## 2. `pad3` removal audit

The original's first `left`/`right` block (Ghidra `0x159cac..0x159cc6`) is:

```
149cac: ldr r0,[r6,#36]      ; buttons
149cae: ldr r3,[r5,#0]       ; Globals::layout (r5 = &Globals::layout)
149cb0: ldr.w r1,[r6,#244]   ; hintOffsetX
149cb4: ldr r0,[r0,#4]       ; buttons->data
149cb6: ldr r3,[r3,#40]      ; layout->buttonInsetX
149cb8: ldr.w r2,[r6,#276]   ; field_0x114
149cbc: ldr r0,[r0,#32]      ; buttons[8]
149cbe: add r1,r3            ; hintOffsetX + buttonInsetX
149cc0: ldr r4,[sp,#116]     ; y
149cc2: add r2,r4            ; field_0x114 + y
149cc4: movs r3,#17
149cc6: blx setPosition
```

There is no local and no store: the real source shape is just the plain block.  But
removing `pad3` (`-8 B`) gives 9692 B / 0.7912 and flips the loop-body roles to
`this -> r5`, `amount -> r6` (the original is `this -> r6`, `amount -> r9`).  With the
anchor present the pre-RA code is

```
%1424 = LDR [%3391, 276]     ; field_0x114
STR %1424, %stack.7          ; pad3 volatile store
...
%1432 = ADD %1424, %3748     ; field_0x114 + y
```

so the load is CSE'd with the `setPosition` argument and only the dead store remains;
that store is what keeps `this` alive at the top of the block and forces the original
roles.

### 2.1 Real replacements for `pad3` (all 9700 B unless noted; baseline 0.8084813)

| shape | raw | size | insns |
|---|---:|---:|---:|
| `pad3 = this->field_0x114` (kept) | **0.8084813** | 9700 | 3637 |
| no `pad3` (plain block) | 0.7911846 | 9692 | 3634 |
| no `pad3`, `right` bare | 0.7914601 | 9692 | 3634 |
| `int y2 = this->field_0x114 + y;` per block, used | 0.7904159 | 9696 | 3636 |
| `int y2 = y + this->field_0x114;` per block, used | 0.7904159 | 9696 | 3636 |
| `int yOff = this->field_0x114;` before the ifs, used | 0.7845730 | 9688 | 3634 |
| `int buttonY = this->field_0x114 + y;` before the ifs, used | 0.7866685 | 9692 | 3635 |
| `volatile int pad3 = this->field_0x114 + y;` used | 0.7814825 | 9684 | 3632 |
| `int pad3 = this->field_0x114;` (non-volatile) inside `if` | 0.7906913 | 9696 | 3636 |
| `TouchButton *b8 = (*this->buttons)[8];` | 0.7847892 | 9688 | 3632 |
| `TouchButton *b9 = (*this->buttons)[9];` | 0.7803197 | 9684 | 3630 |
| both button locals | 0.7787586 | 9672 | 3624 |
| `Array<TouchButton *> *buttons = this->buttons;` | 0.7799228 | 9676 | 3626 |
| eager `layout` in the `isItem` branch | 0.5204420 | 9668 | 3614 |
| eager `layout` in the whole body | 0.4530816 | 9552 | 3578 |

`pad3` initialiser/type sweep (all 9700 B unless noted): `0` 0.8082059; `y`
0.8061674 (3638 insns); `field_0x114` 0.8084813; `unsigned int` / `(int)` cast /
`+ 0` / `pad3 = pad3` all 0.8084813; `pad3++` 0.7884959 (9708 B); `float` 0.7883292
(9712 B); `short`/`char` 0.7910242 (9704 B); `long long` 0.6593255 (9704 B).

Placement with `right > 0`: top of `if (left > 0)` is best (0.8084813); before the ifs
0.7855667 (9688 B); before `if (right > 0)` 0.7871 (9696 B, w4811); before
`buttonOffset` 0.8060 (9700 B, w4811).

Declaration moves (with `pad3`): `int left, right;` neutral; `int buttonOffset;`
hoisted before the item loop neutral; `int right, left;` hoisted to loop scope
0.7812672 (9684 B); `baseY` inlined into `y` 0.8076552 (9700 B).

### 2.2 Faithful variants re-measured together

`pad3` on/off × `description = description` ×3 on/off × `volatile float textYf`
on/off × `freeCredits` `getWidth`-before-`getHeight` on/off (16 cells).  The current
cell (`pad3` on, 3 fillers, volatile `textYf`, current `freeCredits` order) is the
maximum at 0.8084813.  The next best is plain `pad3` + no `freeCredits` swap with the
fillers removed (0.7825248, 9680 B).  The faithful `freeCredits` order alone is
0.6225038 (9748 B); plain `textYf` alone is 0.6191132 (9752 B).

## 3. What remains

- The body is still register-allocation driven.  The largest remaining mismatch runs
  are literal-pool placement (`pool:` words at different instruction indices) and the
  `.ARM.extab` landing-pad row ordering, not source shape.
- The original's `if (tab == 1 || i > 1)` (`0x149906`) evaluates `tab == 1` first;
  ours evaluates `i > 1` first (`0x149906: cmp r0,#2; bcc`).  Same source, different
  branch ordering — regalloc, not fixable by spelling the condition differently
  (`i > 1 || tab == 1` also evaluates `i` first).
- `.ARM.extab` stays `0x26294` (−4).  The single missing call-site row is still the
  final `rightText` `operator+` destination materialisation documented in w4811.

## 4. Reproduction

```sh
python3 /dev/shm/w5009/raw.py     # 0.8084813438 size 9700 insns 3626/3637
python3 /dev/shm/w5009/roles.py   # frame 376 this fp
# Ghidra decompilation of the original render:
ADDR=0x1590d8 /tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless \
  /home/debian/GalaxyOnFire2/_work/bins android_2.0.16_libgof2hdaa.so_ghidra \
  -process -noanalysis -scriptPath /home/debian/ghidra_scripts -postScript DecompAt2.java
```

Scratch scripts used this pass live in `/dev/shm/w5009` (`raw.py`, `roles.py`,
`sweep*.py`, `ghidra_render.txt`, `orig_full.txt`, `ours_r0.txt`, `mi_base.txt`).
