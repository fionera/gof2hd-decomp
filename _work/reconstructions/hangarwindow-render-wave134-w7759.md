# `HangarWindow::render` — wave 134 (w7759): the reload is a live-range split, not a load order

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7759`,
base `ec0acea1e` (wave-133 docs; `render` 94.747001 %, gate byte 4506 /
linked 4506).

This pass: **94.747001 -> 94.747001** (236 diffops, 3626/3627 insns). No source
change landed; the deliverable is this evidence record. `check` is green
(byte 4506, linked 4506, size 4524, data 843, imports 207, lint CLEAN, drift 0).

## New hypothesis (from wave 133 / w7735)

Wave 133 pinned the missing 4 bytes to the original's `ldrd r5,r4` reload: the
original spills `this` across the `getShip()` call, reuses **r4** for the ship
value, and reloads the `(layout, this)` pair afterwards. This wave tested
"add a live-across-call local / change the `(layout, this)` pair order /
perturb `padX` liveness" and, for the first time, dumped the **LLVM IR** of the
base-order and flipped-order sources to see exactly what differs.

## Finding 1: the whole difference is *where the ship load sits relative to the call*

`clang -S -emit-llvm` on the two operand orders shows the ship branch's IR is
the only thing that changes:

Base order `this->selectedItem->ship == Globals::status->getShip()`:
```
; <label>:1720:
  %1721 = load this->selectedItem
  %1722 = gep %1721, 0, 3
  %1723 = load ship                     ; ship loaded FIRST
  %1724 = load Globals::status
  %1725 = invoke getShip
; <label>:1726:
  %1727 = icmp eq %1723, %1725          ; ship == getShip
```

Flipped order `Globals::status->getShip() == this->selectedItem->ship`
(the landed source):
```
; <label>:1721:
  %1722 = load Globals::status
  %1723 = invoke getShip                ; getShip FIRST
; <label>:1724:
  %1725 = load this->selectedItem
  %1727 = load ship
  %1728 = icmp eq %1723, %1727
```

So the source order controls the IR order, and the IR order controls the
machine-code order (the `-Oz` scheduler preserves it here). The allocation then
follows:

| source order | pair at branch entry | ship | body regs | reload |
|---|---|---|---|---|
| base (`ship == getShip`) | `ldrd r6,r5` (layout r6, this r5) | **r4, before call** | this r5, layout r6 | no |
| flipped (`getShip() == ship`, landed) | `ldrd r5,r4` (layout r5, this r4) | r1, **after call** | this r4, layout r5 | no |
| original | `ldrd r5,r4` (layout r5, this r4) | **r4, before call** | this r4, layout r5 | **yes** |

The original is exactly the hybrid: the flipped order's pair/body registers
(this r4, layout r5) **plus** the base order's ship-before-call position. The
reload exists because the ship load then clobbers r4 (this) and the body's
`this` must be rematerialised from the spill slot at `sp+136`.

## Finding 2: it is a live-range split, not a load-order or `padX` effect

In the base order all three values are live across the call (this, layout,
ship) and the allocator has r4/r5/r6, so it keeps all three in registers
(this r5, layout r6, ship r4) and never reloads. In the original only **two**
values cross the call (layout, ship): `this`'s live range is split at the ship
load (`[0x149b20,0x149b2c]` + `[0x149b3c,body]`), so ship and the first `this`
segment can share r4. The missing transformation is therefore the greedy
allocator's decision to split the `this` reload at the ship load — not a
source-order effect, and not `padX` (which is a dead `volatile` stack slot at
`sp+4` in both builds).

## Variants measured this wave (all reverted; landed stays 94.747001 %, 9700 B)

Every variant was built and its ship branch disassembled at
`0x149b1e..0x149b50`.

### Operand / evaluation order

| variant | pct | size | note |
|---|---:|---:|---|
| landed (flipped, `!(tab==3 && getShip()==ship)`) | 94.7 | 9700 | control |
| base order (`!(tab==3 && ship==getShip())`) | 94.4 | 9700 | ship before call, this r5 |
| positive `tab != 3 \|\| ship != getShip()` | 94.4 | 9700 | same as base |
| positive `tab != 3 \|\| getShip() != ship` | 94.7 | 9700 | same as landed |
| `!(ship == getShip() && tab == 3)` (ship first) | 94.1 | 9700 | |
| `!(getShip() == ship && tab == 3)` | 94.6 | 9700 | `ittt eq` predication |
| empty `then` block, flipped | 94.7 | 9700 | same as landed |
| empty `then` block, base | 94.4 | 9700 | same as base |
| redundant parens around `ship` | 94.7 | 9700 | same as landed |
| `item->ship` (loop item == selectedItem), base | 87.2 | 9676 | |
| `item->ship`, flipped | 94.7 | 9700 | same as landed, ship still after call |
| `HangarWindow *self = this` around body, base | 89.1 | 9692 | |
| `HangarWindow *self = this` around body, flipped | 89.5 | 9692 | |
| `ListItem *sel = this->selectedItem` inside `if` | 86.4 | 9684 | |

### Live-across-call locals (the wave-133 suggestion)

| variant | pct | size | note |
|---|---:|---:|---|
| `Status *st = Globals::status` + flipped | 94.7 | 9700 | same as landed |
| `Status *st = Globals::status` + base | 94.4 | 9700 | same as base |
| `int inset = Globals::layout->buttonInsetX` + base | 89.5 | 9708 | this r6, ship r5, no reload |
| `int inset` + flipped | 94.2 | 9700 | |
| `int inset` + `int rh` + base | 88.6 | 9704 | |
| `int inset` + `int hx` + base | 88.1 | 9708 | |
| `int lw = listWidth` + base | 89.1 | 9692 | |
| `int yy = y` + base | 89.1 | 9692 | |
| `int dummy = tab` used after | 85.9 | 9704 | |

### `padX` liveness / placement / type

| variant | pct | size |
|---|---:|---:|
| `padX = &buttonOffset` | 94.4 | 9700 |
| `padX = (volatile int *)&tab` | 94.4 | 9700 |
| `padX` used after the block | 94.4 | 9700 |
| `padX` declared after the block | 87.0 | 9700 |
| `volatile int padX = 0;` (int, not pointer) | 86.8 | 9708 |
| `volatile char padX = 0;` | 86.8 | 9708 |
| `volatile void *padX = 0;` | 94.4 | 9700 |

### `goto` + `assume` CFG (wave-132 shape) with a live-across-call local

| variant | pct | size |
|---|---:|---:|
| `goto` + `assume(tab<6)`, base (control) | 90.0 | 9696 |
| + `HangarWindow *self = this` around body | 85.8 | 9688 |
| + `int inset = layout->buttonInsetX` | 85.8 | 9688 |
| + `self`, flipped | 86.0 | 9688 |
| `Ship *s` local + `goto`, flipped | 83.8 | 9676 |
| `Ship *s` local + `goto`, base | 83.9 | 9676 |

### Body expression reorder

| variant | pct | size |
|---|---:|---:|
| `this->hintOffsetX + layout->buttonInsetX + listWidth` (base) | 94.4 | 9700 |
| same (flipped) | 94.7 | 9700 |
| `y + this->field_0x114` (base) | 94.4 | 9700 |
| both swaps (base) | 94.4 | 9700 |

## Finding 3: a second, independent allocation tie at `0x1496da`

The full instruction diff also shows a separate `(layout, this)` pair swap:

```
orig 1496da: ldrd sl, r9, [sp,#132]   ; sl=layout, r9=this
ours 1496da: ldrd r9, sl, [sp,#132]   ; r9=layout, sl=this
```

and the corresponding `[sl]`/`[r9]` member accesses are swapped for the rest of
that block. At `0x149c16` both builds use `ldrd sl,r9`. So even if the ship
branch reload were recovered, this tie (and the `ldr [pc,#N]` literal-pool
offsets it drags along) would still keep the function non-byte-exact. Fixing
the ship branch would remove the 4-byte shift (and with it ~116 branch-target
diffops), so it is still the single highest-value change.

## Conclusion / open questions

The ship branch is not a source-order problem: the base order already puts the
ship load in the right place (r4, before the call) and the flipped order already
puts `this`/`layout` in the right registers (r4/r5). The last 4 bytes need the
greedy allocator to split the `this` reload at the ship load so r4 can be reused
for the ship value. Every source form tried this wave (operand order, positive
`||`/empty-`then`, loop-item/`selectedItem`/`self` locals, live-across-call
`inset`/`rh`/`hx`/`lw`/`yy`/`dummy` locals, `padX` placement/type/liveness,
`goto`+`assume` + locals, body reorder) either left the allocation unchanged or
moved the whole function off the 9700 B shape.

Next lever candidates: a construct that forces `this`'s reload to be split at
the ship load without adding instructions (e.g. an opaque copy of `this` used
only by the body so the ship-branch `this` dies at `selectedItem`), or a
same-TU inlining change that alters the reload's spill weight.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

IR dump (base vs flipped order):

```sh
CXX=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
$CXX -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  -stdlib=libc++ --sysroot=.cache/ndk/android-ndk-r18b/sysroot \
  -D__ANDROID_API__=16 -DANDROID -DNDEBUG -I src -I third_party/fmod/inc \
  -I third_party/gl -I third_party/jni -I third_party/libzip -I third_party/crypto \
  -S -emit-llvm -o /dev/shm/w7759/base.ll src/game/ui/HangarWindow.cpp
# then grep for "icmp eq %class.Ship" — the base order has the ship load in the
# same block as the invoke, the flipped order has it in the successor block.
```

Scratch (not committed): `/dev/shm/w7759/{mkbatch.py,sweep_new.py,sweep2_new.py,
sweep3_new.py,sweep4_new.py,sweep5_new.py,sweep7.py,sweep8.py,sweep9.py,
sweep10.py,sweep11.py,sweep12.py,sweep13.py,sweep14.py,fulldiff.py,base.ll,
baseorder.ll,orig_full.dis,pa.txt,HangarWindow.base.cpp}`.
