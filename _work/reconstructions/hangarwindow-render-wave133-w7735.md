# `HangarWindow::render` — wave 133 (w7735): forcing the `ldrd r5,r4` reload

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7735`,
base `144fc16b0` (wave-132 Radar; `render` 94.747001 %, gate byte 4506 /
linked 4506).

This pass: **94.747001 -> 94.747001** (141 diffops, 3626/3627 insns). No source
change landed; the deliverable is this evidence record. `check` is green
(byte 4506, linked 4506, size 4524, data 843, imports 207, lint CLEAN, drift 0).

## New hypothesis (from wave 132 / w7715)

`goto ship_done` + a non-foldable `assume` in the upgrade block reproduces the
original's *asymmetric* Jump Threading (case 0 -> upgrade test, ship-equal ->
join). The only remaining ship-branch delta is the original's `ldrd r5,r4`
reload: the original spills `this` across `getShip()` and reuses **r4** for the
ship value; our allocator keeps `this` in a callee-saved register. This wave
tried to force that spill from source.

## Ground truth: the three ship-branch shapes

Original `0x149b1e..0x149b44` (38 B):

```
149b1e ldr  r0,[sp,#108]        ; tab
149b20 ldrd r5,r4,[sp,#132]     ; layout=r5, this=r4
149b24 cbz  r0,149b96           ; tab==0 -> upgrade test
149b26 cmp  r0,#3
149b28 bne.n 149b44             ; tab!=3 -> body
149b2a ldr  r0,[pc,#340]        ; status
149b2c ldr  r1,[r4,#104]        ; selectedItem   (this=r4)
149b2e add  r0,pc
149b30 ldr  r0,[r0,#0]          ; Globals::status
149b32 ldr  r4,[r1,#12]         ; ship -> r4  (clobbers this)
149b34 ldr  r0,[r0,#0]          ; *status
149b36 blx  getShip
149b3a cmp  r4,r0
149b3c ldrd r5,r4,[sp,#132]     ; RELOAD layout+this
149b40 beq.w 14a6d0             ; ship-equal -> join
149b44 body                     ; uses r4=this, r5=layout
```

Landed source (flipped order, no `goto`), also 38 B:

```
149b20 ldrd r5,r4,[sp,#132]     ; pair matches
149b24 cmp  r0,#0
149b26 beq.w 14a6d0             ; case 0 -> join (wrong: JT threaded it)
149b2a cmp  r0,#3
149b2c bne.n 149b44
149b2e ldr  r0,[pc,#336]
149b30 add  r0,pc
149b32 ldr  r0,[r0,#0]
149b34 ldr  r0,[r0,#0]
149b36 blx  getShip             ; getShip FIRST
149b3a ldr  r1,[r4,#104]
149b3c ldr  r1,[r1,#12]         ; ship -> r1
149b3e cmp  r0,r1
149b40 beq.w 14a6d0
```

Base order (`this->selectedItem->ship == getShip()`) + `goto` + `assume`
(34 B, 9696 B function):

```
149b20 ldrd r6,r5,[sp,#132]     ; pair shifted: layout=r6, this=r5
149b24 cbz  r0,149b92           ; case 0 -> upgrade test (MATCHES)
...
149b2c ldr  r1,[r5,#104]        ; selectedItem (this=r5)
149b32 ldr  r4,[r1,#12]         ; ship -> r4 BEFORE call (MATCHES)
149b36 blx  getShip
149b3a cmp  r4,r0
149b3c beq.w join               ; ship-equal -> join (MATCHES)
149b40 body                     ; uses r5=this, r6=layout (WRONG)
```

So base order gives the right *evaluation order* but shifts the pair and the
body; flipped order gives the right *pair/body* but the wrong order. The
original is the hybrid: pair `r5,r4`, body `r4,r5`, ship in r4 before the call,
plus the reload.

## What was tried (all reverted; landed stays 94.747001 %, 9700 B)

Every variant below was built and its ship branch disassembled at
`0x149b1e..0x149b50`; `pair` = `ldrd r5,r4` at branch entry, `shippre` =
`ldr r4,[r1,#12]` before `blx`, `reload` = `ldrd r5,r4` after `cmp`.

### Live-across-call locals (the wave-132 suggestion)

| variant | pct | size | pair | shippre | reload |
|---|---:|---:|---|---|---|
| base + `goto` + `assume` (control) | 89.961390 | 9696 | no | yes | no |
| `Ship *s = this->selectedItem->ship;` + `s == getShip()` | 89.933811 | 9696 | no | yes | no |
| `Ship *s = ...;` + `getShip() == s` | 89.906233 | 9696 | no | yes | no |
| `ListItem *it = this->selectedItem;` + `it->ship == getShip()` | 89.961390 | 9696 | no | yes | no |
| `ListItem *it = ...;` + `getShip() == it->ship` | 89.906233 | 9696 | no | no | no |
| `HangarWindow *self = this;` + `self->...` in cond | 89.961390 | 9696 | no | yes | no |
| same, body uses `self` | 89.961390 | 9696 | no | yes | no |
| `self = (HangarWindow*)((char*)this + (tab & 0));` (anti-coalesce) | 89.961390 | 9696 | no | yes | no |
| `self = (HangarWindow*)((uintptr_t)this | (tab & 0));` | 89.961390 | 9696 | no | yes | no |
| `Layout *lay = Globals::layout;` + body uses `lay` | 86.324786 | 9700 | no | no | no |
| `HangarWindow *self` + `Layout *lay`, body uses both | 83.979578 | 9680 | no | no | no |

### Hoisting body expressions (forces `this` into r4 but not the reload)

| variant | pct | size | pair | shippre | reload |
|---|---:|---:|---|---|---|
| `Array<TouchButton*> *btns = this->buttons;` body uses `(*btns)[1]` | **91.747171** | **9700** | no | no | no |
| + `Layout *lay` (body uses `lay`) | 85.430464 | ? | no | no | no |
| hoist `this->field_0x114` / `this->hintOffsetX` into ints | 85.970232 | ? | no | yes | no |
| `TouchButton *b1 = (*this->buttons)[1];` body uses `b1` | 89.137336 | ? | no | no | no |

The `btns` variant is the closest to the original's *register*: it produces
`ldrd r9,r4` (this=**r4**, layout=r9) and ship in **r5**, i.e. `this` finally
lands in r4 — but layout moves to r9 and ship takes r5, so there is still no
reload. This proves the allocator *can* put `this` in r4 with base evaluation
order; the missing piece is making it reuse r4 for ship (spill `this`) instead
of moving layout/ship around.

### `padX` liveness / placement / type

| variant | pct | size |
|---|---:|---:|
| `padX` after the comparison | 89.961390 | 9696 |
| `padX` inside `if (tab != 0)` | 89.961390 | 9696 |
| `volatile int **padX` | 89.961390 | 9696 |
| `padX = (volatile int*)this` | 89.961390 | 9696 |
| `String tmp;` / `String tmp("")` as the sink | 84.167011 / 86.033058 | 9696 |
| second `volatile int padY = tab;` (4-byte pad) | 89.740761 | **9700** |
| same on flipped order | 89.878654 | 9700 (pair lost) |

### Structural splits

| variant | pct | size |
|---|---:|---:|
| `if (tab==3) { Ship *s = ...; if (s == getShip()) goto; }` | 86.817430 | 9688 |
| `if (tab==3) { Ship *s = ...; Ship *cur = getShip(); if (s == cur) goto; }` | 86.789851 | ? |
| `if (tab==3) { Ship *cur = getShip(); if (this->selectedItem->ship == cur) goto; }` | 86.025659 | ? |
| `item->ship == getShip()` (use the loop `item` instead of `this->selectedItem`) | 87.051353 | ? |
| `item->ship` flipped | 90.319912 | 9696 (pair yes, ship after call) |

The nested `if (tab==3)` split is notable: it *does* produce a 4-byte reload
after `cmp` (`ldr r4,[sp,#120]`), but the reloaded value is the loop `item`,
not `this`/`layout`, and the function shrinks to 9688.

### Assume placement / expression

`assume` in the ship block (`tab != 0`, `this != 0`, `Globals::status != 0`,
`tab == 3`) and `tab < 100`, `tab != 5`, `tab < 6 && tab >= 0` all leave the
allocation unchanged (89.961390 / 9696) or perturb it globally.

### Qualifier variants

`const Ship *s`, `auto s`, `Ship *const s`, `Ship *volatile s` all leave the
base-order allocation unchanged (89.93 / 89.91 / 89.91 / 88.06).

## Finding: the reload is a live-range split, not a source-order effect

`-mllvm -print-after-all` on the base-order variant shows the ship block
`bb.383` has `liveins: $r5, $r6` (this=r5, layout=r6) and the ship value
`%403` is live across the `getShip` call (`bb.383 -> bb.384`). The greedy
allocator gives the call-crossing `%403` r4 and shifts the `this`/`layout`
pair to r5/r6. The original instead keeps the pair at r4/r5, lets `%403` reuse
r4, and splits `this`'s live interval at the call (the reload). This is a
greedy tie-break; none of the source perturbations above changed it. The
`btns` hoist shows `this` *can* be pinned to r4, but the allocator then moves
layout to r9 and ship to r5 rather than spilling `this`.

## Conclusion / open questions

The asymmetric JT is solved (goto + assume). The last 4 bytes are the
`ldrd r5,r4` reload, which requires the greedy allocator to spill `this` at the
`getShip` call and reuse r4 for ship. Source-level live-across-call locals,
hoisting, `padX` liveness, structural splits, assume placement and qualifiers
have not produced it. The closest 9700 B shape is the `btns` hoist
(91.747171 %, 225 diffops, this=r4/layout=r9/ship=r5), still below the landed
94.747001 % (141 diffops).

Next lever candidates: a construct that raises the ship value's spill weight
without moving layout (e.g. a second use of the ship value after the call), or
a body shape whose first `this` use is a copy that the allocator can coalesce
into the reload.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

The `btns` variant (9700 B, 91.747171 %, this=r4) is:

```cpp
if (item->isShip()) {
    volatile int *padX = 0;
    Array<TouchButton *> *btns = this->buttons;
    if (tab != 0) {
        if (tab == 3 && this->selectedItem->ship == Globals::status->getShip())
            goto ship_done;
        (*btns)[1]->setPosition(
            Globals::layout->buttonInsetX + this->hintOffsetX + listWidth,
            this->field_0x114 + y, 18);
        (*btns)[1]->setVisible(true);
        (*btns)[1]->draw();
        buttonOffset = (*btns)[1]->getWidth() + Globals::layout->field_0x2c_rowHeight;
    }
    if (__builtin_assume(tab < 6), this->upgradeMode && tab == 1) { // lint: arm_builtin
        ...
    }
ship_done: ;
}
```

Scratch (not committed): `/dev/shm/w7735/{batch.py,batch2.py,batch3.py,
detect.py,diffops.py,size.py,search2.py,t1..t21.json,base.cpp,orig.txt,ours.txt,
greedy_render_base.txt,virtrewrite_base.txt,pa.out}`.
