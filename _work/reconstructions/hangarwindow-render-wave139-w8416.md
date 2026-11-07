# `HangarWindow::render` — wave 139 (w8416): the `getShip()` call *is* an invoke;
# the ship value is global in base order / local in flipped order

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w8416`,
base `bf123bd47` (wave-138 docs; `render` 94.7 %, gate byte 4507 / linked 4507).

This pass: **94.7 -> 94.7 %** (no source change landed). `check` is green
(byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN, drift 0).
Deliverable = this evidence record.

Metric used below: **diffbytes** = differing bytes in the 9700-byte function body
(`/dev/shm/w8416/fdiff.py`, a copy of `/dev/shm/fdiff.py` with worktree paths).
Landed baseline: **diffbytes 2062, 9700 B**. Base-order control: **diffbytes 2082**.

## Finding 1 (NEW, falsifies w8316's premise): the original's `getShip()` call is
## an invoke — the LSDA call-site row covers it

w8316 hypothesised that the original's `getShip()` call is a plain call ("no
branch after the call"), which would make the ship value local. The original's
own LSDA says otherwise. Dumping the call-site table with
`_work/reconstructions/gameresource_gen/lsda.py`:

```
orig  0x149aa4 +150 lp=14b698     ; covers 0x149aa4..0x149b3a, incl. getShip @0x149b36
orig  0x149b60 +140 lp=14b632
ours  0x149aa4 +118 lp=14b698     ; ends before getShip
ours  0x149b36 +182 lp=14b600     ; getShip gets its own region
```

and the original's landing pad `0x14b698` is:

```
14b698: mov  r4, r0
14b69a: add  r0, sp, #312   ; 0x138 = `String name`
14b69c: blx  String::~String
14b6a0: mov  r0, r4
14b6a2: blx  _Unwind_Resume
```

`String name;` is constructed at `0x14949a` (`add r0,sp,#312; blx StringC1Ev`),
so the call at `0x149b36` is inside `name`'s cleanup region and is a genuine
invoke in the original too. The "non-invoke" lever is therefore not available
without moving `name`'s scope, which changes the 156-row `.ARM.extab`.

## Finding 2 (NEW): the ship value is global in base order and local in flipped order

The final IR (`-S -emit-llvm`) for the landed flipped source
(`Globals::status->getShip() == this->selectedItem->ship`) is:

```
1719: switch i32 %93, label %1728 [ i32 0, label %1959  i32 3, label %1720 ]
1720: %1721 = load @Globals::status
      %1722 = invoke getShip(%1721) to label %1723 unwind label %1761
1723: %1724 = load this->selectedItem
      %1725 = gep ... 3
      %1726 = load %1725->ship
      %1727 = icmp eq %1722, %1726
      br %1727, label %1959, label %1728
```

The ship load is in the *successor* block after the invoke, so its interval is
local; the allocator gives `this=r4`, `layout=r5` (the original's pair) and the
ship takes the scratch `r1` after the call.

Base order (`this->selectedItem->ship == Globals::status->getShip()`) puts the
ship load in the *predecessor* block, so the ship interval spans the invoke and
is global. The allocator queues global before split (w8217), so ship takes `r4`
first, then `this=r5`, `layout=r6`:

```
base  a48: ldrd r6,r5,[sp,#132]   ; layout=r6, this=r5
      ...
      a5e: ldr r4,[r1,#12]        ; ship -> r4   (BEFORE the call, as orig)
      a62: bl getShip
      a66: cmp r4,r0
      a68: beq.w join
```

The base-order instruction sequence is otherwise **identical** to the original
(`ldr/ldr/add/ldr/ldr/bl/cmp/beq`), only the register pair and the missing
`ldrd` reload differ. The landed flipped build has the right pair but the ship
load after the call.

## Experiments this wave (all reverted; landed stays 94.7 %, 9700 B, 2062 diffbytes)

Built with the full link (`cmake --build --target link_so`, ~1 s incremental) and
scored with `fdiff.py`; the ship branch was inspected on every variant.

### Base-order variants (control 2082 diffbytes, pair `r6,r5`, ship `r4` pre-call)

| variant | diffbytes | size | pair | ship |
|---|---:|---:|---|---|
| base-order (control) | 2082 | 9700 | r6,r5 | r4 pre |
| `(void)this->field_0x114` / `(void)Globals::layout->...` at sink | 2082 | 9700 | r6,r5 | r4 pre |
| `volatile int *padX = (volatile int*)this` / `...layout` | 2082 | 9700 | r6,r5 | r4 pre |
| `if (tab)` / `if (tab > 0)` / `if ((unsigned)tab != 0)` | 2082 | 9700 | r6,r5 | r4 pre |
| `if (tab != 0 && tab >= 0)` | 2082 | 9700 | r6,r5 | r4 pre |
| `if (this->upgradeMode == 1 && tab == 1)` | 2082 | 9700 | r6,r5 | r4 pre |
| `if ((bool)this->upgradeMode && tab == 1)` | 2082 | 9700 | r6,r5 | r4 pre |
| `if (this->upgradeMode ? tab == 1 : false)` | 2082 | 9700 | r6,r5 | r4 pre |
| `if (this->selectedItem == item)` / `this->selectedItem->isShip()` | 2082 | 9700 | r6,r5 | r4 pre |
| `if (tab != 0 && tab != 5)` | **6035** | 9700 | **r5,r4** | r1 post |
| `if (tab != 0 && tab != 2)` | 8752 | 9708 | – | – |
| `if (tab != 0 && Globals::layout != 0)` | 7306 | 9720 | – | – |
| `if (tab != 0 && this != 0)` | 6209 | 9696 | – | – |
| `if (tab != 0 && this->buttons != 0)` | 6791 | 9708 | – | – |
| `if (tab != 0 && listWidth != 0)` | 8400 | 9656 | – | – |
| `if (tab != 0 && y != 0)` | 7086 | 9704 | – | – |
| `if (tab != 0 && item != 0)` | 8636 | 9684 | – | – |
| `if (tab != 0 && tab < 6)` / `tab < 4` | – | 9728 | – | – |
| `if (tab != 0 && tab < 3)` | – | 9692 | – | – |
| `if (tab != 0 && tab != 1)` | – | 9684 | – | – |
| `if (tab != 0 && tab != 4)` / `tab != 6` | – | 9708/9700 | – | – |
| `volatile int padX = this->field_0x114` (read) | 7500 | 9712 | – | – |
| `volatile int padX = Globals::layout->buttonInsetX` | 7501 | 9712 | – | – |
| `volatile int padX = this->field_0x114 + layout->buttonInsetX` | 7498 | 9720 | – | – |
| `volatile unsigned char` read of `upgradeMode` | 6560 | 9708 | – | – |
| `String name;` -> `static String name;` | – | 9756 | – | – |
| `String *namep = new String(); String &name = *namep;` | – | 9496 | – | – |
| AA shape (`btns`+`lay` hoist, body uses them) | – | 9696 | sl,r9 | – |
| `bool same` + nested `if (tab==3) { call; same=...; }` | 6665 | 9696 | – | – |
| `switch (tab) { case 0: break; case 3: if(ship!=getShip()) body; default: body }` | 7492 | 9772 | – | – |

### Flipped-order variants (control 2062 diffbytes, pair `r5,r4`, ship `r1` post-call)

| variant | diffbytes | size |
|---|---:|---:|
| landed (control) | 2062 | 9700 |
| `+ tab != 5` | 6035 | 9700 |
| `volatile int padX = this->field_0x114` (read) | 5132 | 9704 |
| `volatile int padX = Globals::layout->buttonInsetX` | 5207 | 9704 |
| remove `padX` sink | 6916 | 9700 |
| `if (tab == 1 && this->upgradeMode)` (wave-136 retest) | 2062 | 9700 |
| nested `if (tab == 1) { if (upgradeMode) ... }` | 2062 | 9700 |
| `if (tab != 3 \|\| getShip() != ship)` (De Morgan, call first) | 2062 | 9700 |
| `if (tab != 3 \|\| ship != getShip())` (De Morgan, ship first) | 2082 | 9700 |

**The only 9700-B shape found that moves the pair from `r6,r5` to `r5,r4` with
the base-order source is `if (tab != 0 && tab != 5)`** — but the extra comparison
re-rolls the whole function (6035 diffbytes). It is not a win.

## Conclusion

* The original's `getShip()` call is an invoke (LSDA row `0x149aa4+150`,
  `lp=14b698` destroys `name` at `sp+0x138`). w8316's "non-invoke" premise is
  falsified.
* The ship value's interval is global in base order and local in flipped order;
  that, not the invoke split, is what decides the `this`/`layout` pair.
* The landed flipped source already has the original's `this=r4, layout=r5` pair
  and body. The only remaining ship-branch delta is the ship load position
  (after the call in ours, before in the original) plus the `cmp/beq.w join`
  guard vs the original's `cbz -> upgrade test` (4 B saved, which pays for the
  4-B `ldrd` reload).
* Every base-order shape that keeps 9700 B gives `this=r5, layout=r6`. The
  register pair and the ship-load position appear to be coupled: a callee-saved
  ship (needed to sit before the call) is allocated before the split `this`.

## Open questions / next levers

* The `tab==0` guard: ours threads `tab==0` straight to the join, the original
  jumps to the upgrade test (`cbz r0, 0x149b96`). If jump threading can be
  blocked **without** changing the 9700-B layout (e.g. a source form the
  compiler cannot prove `tab==0 => !(tab==1 && upgradeMode)`), the 4 B saved
  would pay for the `ldrd` reload and the base-order ship branch would match.
* The `0x1496da` `(layout,this)` pair swap (`ldrd sl,r9` vs `ldrd r9,sl`) is a
  separate, independent RA tie in the blueprint-progress block.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w8416/fdiff.py _ZN12HangarWindow6renderEv   # diffbytes
python3 _work/reconstructions/gameresource_gen/lsda.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w8416/{cc.sh,dis.sh,probe.py,batch.py,score.py,
fdiff.py,run1.py..run9.py,s1.py..s11.py,one.py..one4.py,HangarWindow.landed.cpp,
HangarWindow.switch.cpp,hw.ll,hw2.ll,mi.txt,pa.txt,pa_landed.txt,lsda_orig.txt,
lsda_ours.txt,orig_render.s,ours.s}`.
