# `HangarWindow::render` — wave 135 (w7779): opaque-`this` copy / hoist / volatile probes

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7779`,
base `bbffef482` (wave-134 notes; `render` 94.747001 %, gate byte 4506 /
linked 4506).

This pass: **94.747001 -> 94.747001** (141 diffops, 3626/3627 insns). No source
change landed; the deliverable is this evidence record. `check` is green
(byte 4506, linked 4506, size 4524, data 843, imports 207, lint CLEAN, drift 0).

## New hypothesis (from wave 134 / w7759)

The remaining ship-branch delta is the original's `ldrd r5,r4` reload. Wave 134
proved it is a live-range split (the original spills `this` across the
`getShip()` call and reuses r4 for the ship value). This wave tested the two
suggested levers: (a) an **opaque copy of `this` used only by the body** so the
ship-branch `this` dies at `selectedItem`, and (b) a **same-TU inlining / spill
weight** change.

## Ground truth re-confirmed

Original `0x149b1e..0x149b44` (38 B):

```
149b1e ldr  r0,[sp,#108]        ; tab
149b20 ldrd r5,r4,[sp,#132]     ; layout=r5, this=r4
149b24 cbz  r0,149b96           ; tab==0 -> upgrade test
149b26 cmp  r0,#3
149b28 bne.n 149b44
149b2a ldr  r0,[pc,#340]        ; &Globals::status
149b2c ldr  r1,[r4,#104]        ; this->selectedItem  (this=r4)
149b2e add  r0,pc
149b30 ldr  r0,[r0,#0]          ; Globals::status
149b32 ldr  r4,[r1,#12]         ; ship -> r4  (clobbers this)
149b34 ldr  r0,[r0,#0]
149b36 blx  getShip
149b3a cmp  r4,r0
149b3c ldrd r5,r4,[sp,#132]     ; RELOAD layout+this
149b40 beq.w 14a6d0             ; ship-equal -> join
149b44 body                     ; uses r4=this, r5=layout
```

The original is the **base evaluation order** (`this->selectedItem->ship`
before `getShip()`) with the **flipped pair/body allocation** (`this=r4`,
`layout=r5`). Our base order gives the right evaluation order but shifts the
pair/body to `this=r5, layout=r6`; the flipped order gives the right pair/body
but loads the ship after the call into r1. The reload is exactly the allocator
choosing to keep `this` in r4 and reuse r4 for the ship value during the hole
between `selectedItem` and the body.

## New experiments (all reverted; landed stays 94.747001 %, 9700 B)

`pair` = `ldrd r5,r4` at branch entry; `shippre` = `ldr r4,[r1,#12]` before
`blx`; `ldrd#` = number of `ldrd` in `0x149b1e..0x149b48`.

### `goto`+`assume` control (the CFG-exact base for this wave)

The wave-132 shape (`goto ship_done` + `__builtin_assume(tab < 6)` in the
upgrade block) was rebuilt as the starting point: **90.319912 %, 9696 B,
246 diffops**, pair `r5,r4` (this=r4), `shippre=False`, `ldrd#=1`. Its body
already uses `this=r4, layout=r5` and its CFG is exact; it is only 4 B short
(the missing reload). So the whole wave targeted "add the reload to this shape".

### Opaque / volatile copies of `this` (the wave-134 suggestion)

| variant (on `goto` base) | pct | size | pair | shippre | ldrd# |
|---|---:|---:|---|---|---|
| control (flipped) | 90.319912 | 9696 | r5,r4 | no | 1 |
| `HangarWindow *volatile self = this;` body uses `self` | 86.604190 | 9708 | – | no | 0 |
| same, flipped order | 86.659316 | 9708 | – | no | 0 |
| `volatile self` + one-shot `bodyThis = self` in body | 84.528406 | 9708 | – | no | 0 |
| `HangarWindow *volatile ths = this;` used in ship cond (base) | 83.838941 | 9692 | – | no | 0 |
| same, flipped | 84.013230 | 9708 | – | no | 0 |
| `ths` declared but cond uses `this` (flipped) | 86.269644 | 9704 | – | no | 0 |
| `HangarWindow *self = this;` body uses `self` (base) | 89.961390 | 9696 | r6,r5 | yes | 1 |

The volatile copies force extra stack traffic and move the whole function off
the 9700 B shape; the non-volatile copy is coalesced away by the allocator
(identical to the base order). No variant produced a second `ldrd`.

### Local ship / selectedItem hoists (on `goto` base)

| variant | pct | size | pair | shippre | ldrd# |
|---|---:|---:|---|---|---|
| `Ship *s = this->selectedItem->ship;` + `getShip() == s` | 83.830022 | 9676 | – | no | 0 |
| `Ship *s = ...;` + `s == getShip()` | 83.857616 | 9676 | – | no | 0 |
| `ListItem *sel = this->selectedItem;` + `sel->ship == getShip()` | 83.830022 | 9676 | – | no | 0 |
| `ListItem *sel = ...;` + `getShip() == sel->ship` | 83.885210 | 9676 | – | no | 0 |
| `HangarWindow *self = this;` + `self->...` base | 86.025659 | 9688 | – | no | 0 |
| `Ship *s` / `ListItem *sel` / `self` declared *before* `if (tab != 0)` | 90.319912 | 9696 | r5,r4 | no | 1 |

The last row is important: a local declared in the `isShip` block but used only
in the ship condition is **fully coalesced** (byte-identical to the control).
The same local used in the body shifts the pair to `r6,r5` (base-order
allocation). So the compiler never creates the second SSA value the hypothesis
needs.

### `padX` sink type (on `goto` base)

| variant | pct | size | pair | shippre | ldrd# |
|---|---:|---:|---|---|---|
| `volatile int padX = 0;` (int) | 88.309898 | 9704 | r8,r4 | no | 1 |
| `volatile void *padX = 0;` | 90.319912 | 9696 | r5,r4 | no | 1 |
| `volatile char padX = 0;` | 88.266924 | 9704 | r8,r4 | no | 1 |
| `int padX = 0; (void)padX;` | 90.264755 | 9696 | r5,r4 | no | 1 |
| `volatile int *padX = (volatile int *)this;` | 90.319912 | 9696 | r5,r4 | no | 1 |
| `volatile int *padX = &buttonOffset;` | 90.319912 | 9696 | r5,r4 | no | 1 |

### `this`/`buttons` hoist (the wave-133 `btns` shape, re-measured)

`Array<TouchButton *> *btns = this->buttons;` before `if (tab != 0)`, body uses
`(*btns)[1]`, base order:

| variant | pct | size | branch |
|---|---:|---:|---|
| `btns` + base order | **91.747171** | **9700** | `this=r4, layout=r9, ship=r5`, no `ldrd` at entry, no reload |
| `btns` + flipped order | 89.416310 | 9692 | `r8,r5` |

This is the closest 9700 B shape (`this` finally lands in r4), but `layout`
moves to r9 and the ship takes r5, so there is still no clobber/reload. It is
worse than the landed 94.747001 % (225 vs 141 diffops).

### Positive / De Morgan forms (on `goto` base)

| variant | pct | size | pair |
|---|---:|---:|---|
| `tab != 3 \|\| ship != getShip()` (base) | 89.961390 | 9696 | r6,r5 |
| `tab != 3 \|\| getShip() != ship` (flipped) | 90.319912 | 9696 | r5,r4 |

## Finding: the allocator will not split `this` for a source-level copy

Every attempt to create a second `this` value (volatile pointer, one-shot
reload, local ship/selectedItem, hoisted `buttons`) either (a) is coalesced
away, (b) adds stack traffic and moves the function off 9700 B, or (c) moves
`layout`/`ship` instead of splitting `this`. The `btns` hoist proves `this` can
be pinned to r4 with base evaluation order, but the allocator then prefers
`ship=r5` over reusing r4 and spilling `this`. The reload remains a pure greedy
tie-break that no source-level copy/hint tried so far reproduces.

## Open questions / next levers

* The exact greedy assignment order: `btns` gives `this=r4, layout=r9,
  ship=r5`; the original is `this=r4, layout=r5, ship=r4`. A construct that
  raises `ship`'s priority **without** taking a callee-saved register for a new
  live value (so `layout` stays r5) is still untried.
* A same-TU inlining change (e.g. a helper inlined into `render`) that adds an
  extra `this` use *between* the ship load and the body was not found: the ship
  branch contains only `getShip()` and `isShip()` (both PLT), and the body's
  first use is `this->buttons` at `149b44`.
* The independent `(layout, this)` pair swap at `0x1496da` (`ldrd sl,r9` vs
  `ldrd r9,sl`) is still present and would keep the function non-exact even
  after the ship branch is fixed.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w7779/{score.py,batch.py,batch2.py,
mk_v4.py,mkgoto.py,mkbtns.py,mkhoist.py,HangarWindow.landed.cpp,
HangarWindow.goto.cpp,ra_landed.txt,ra_base.txt,pa_all.txt,pa_base.txt}`.
