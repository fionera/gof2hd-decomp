# `HangarWindow::render` — wave 137 (w8217): RA queue is size-ordered; `this`=r4 is reachable, the split is not

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w8217`,
base `655e3bcc5` (wave-136 docs; `render` 94.7 %, gate byte 4507 / linked 4507).

This pass: **94.7 -> 94.7 %** (no source change landed). `check` is green
(byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN, drift 0).
The deliverable is this evidence record plus the LLVM 7.0.2 allocator facts below.

## New hypothesis (from wave 136 / w8115)

The base-order `goto`+`assume` shape is IR-identical to the original; the missing
4 bytes are the `ldrd r5,r4,[sp,#132]` reload of `(layout, this)` after the
`getShip()` call. Find a source shape that changes the allocator's choice of
`this` vs `&Globals::layout` without moving off the target size.

## Finding 1: the base-order `goto`+`assume` shape is reproduced exactly (9696 B)

Rebuilt the wave-132 shape on this base. The `assume` must be in the *condition*
(`(__builtin_assume(tab < 6), tab == 1)`) — an `assume` inside the then-block is
folded because `tab == 1` already implies `tab < 6`. The condition form gives:

```
980 ldr  r1,[sp,#108]        ; tab
981 ldrd r6,r5,[sp,#132]     ; layout=r6, this=r5
982 cbz  r1, upgrade
983 cmp  r1,#3
984 bne.n body
985 ldr  r0,[pc,#336]        ; &Globals::status
986 ldr  r1,[r5,#104]        ; this->selectedItem
987 add  r0,pc
988 ldr  r0,[r0,#0]
989 ldr  r4,[r1,#12]         ; ship -> r4  (BEFORE the call, as original)
990 ldr  r0,[r0,#0]
991 blx  getShip
992 cmp  r4,r0
993 beq.w join
994 ldr  r0,[r5,#36]         ; body uses this=r5
```

`pct=89.8`, `usize=9696`, 3626/3626 insns. This is byte-for-byte the wave-136
finding: only the pair registers (`r6,r5` vs `r5,r4`) and the missing reload
differ.

## Finding 2 (NEW): LLVM 7.0.2 `RAGreedy` orders by live-interval *size*, not weight

Read `lib/CodeGen/RegAllocGreedy.cpp` (release_70):

```cpp
// Prioritize live ranges by size, assigning larger ranges first.
const unsigned Size = LI->getSize();
...
if (ExtraRegInfo[Reg].Stage == RS_Split) Prio = Size;            // split ranges last
else if (... RS_Memory) ...
else {
  bool ForceGlobal = !ReverseLocal && (Size/InstrDist) > 2*RC.getNumRegs();
  if (Stage == RS_Assign && !ForceGlobal && LIS->intervalIsInOneMBB(*LI))
    Prio = LI->beginIndex().getInstrDistance(Indexes->getLastIndex());   // local: later start = smaller
  else
    Prio = (1u << 29) + Size;                                    // global: larger size first
  Prio |= (1u << 31);                                            // global+local before split
  if (VRM->hasKnownPreference(Reg)) Prio |= (1u << 30);          // hint
}
CurQueue.push(std::make_pair(Prio, ~Reg));   // tie-break: LOWER vreg number wins
```

`LiveIntervals::getSpillWeight` is `(isDef+isUse) * blockFreq`; the `weight`
field is only used by `shouldEvict`, **not** by the queue order. So the
`this`-vs-`ship` tie is decided by interval size and then by machine vreg number.

The machine vregs in the initial selection dump are:

| value | vreg |
|---|---|
| `tab` | `%4` |
| `ship` (`this->selectedItem->ship`) | `%403` |
| `this` (function arg `COPY $r0`) | `%613` |

`ship` has a **lower** vreg number than `this`. On an equal-size tie `ship`
wins r4. `this` is a giant interval and is split by the allocator; the split
pieces are `RS_Split` and are queued *after* unsplit ranges like `ship`, so
`ship` gets the first free callee-saved register (r4) before `this` is placed.
That is why the base-order build is `ship=r4, this=r5, layout=r6`.

## Finding 3 (NEW): the landed flipped shape already has the original pair/body

The landed source (`getShip() == this->selectedItem->ship`, 9700 B, 94.7 %)
disassembles at the ship branch as:

```
981 ldrd r5,r4,[sp,#132]     ; layout=r5, this=r4   <-- MATCHES original
982 cmp  r0,#0
983 beq.w join
984 cmp  r0,#3
985 bne.n body
... getShip FIRST ...
991 ldr  r1,[r4,#104]        ; this->selectedItem
992 ldr  r1,[r1,#12]         ; ship -> r1  (AFTER the call)
993 cmp  r0,r1
994 beq.w join
995 ldr  r0,[r4,#36]         ; body uses this=r4   <-- MATCHES original
```

So the landed shape already has the original's `(this=r4, layout=r5)` pair and
body registers. It differs from the original only in (a) the ship load sitting
*after* the call, and (b) `cmp;beq.w` (6 B) instead of `cbz` (2 B) + the 4 B
reload. The original is exactly "landed pair/body + base-order ship position".

## Experiments this wave (all reverted; landed stays 94.7 %, 9700 B)

Every variant was built and its ship branch inspected. `pair` = registers of the
`ldrd [sp,#132]` at branch entry; `ship` = register of the ship value; `reload`
= a second `ldrd [sp,#132]` after the call.

### Locals / inline helpers on the base `goto`+`assume` shape (9696 B control)

| variant | pct | size | pair | ship | reload |
|---|---:|---:|---|---|---|
| control (`goto`+`assume`, base) | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `ListItem *sel = this->selectedItem;` inside `if (tab != 0)` | 83.8 | 9676 | – | – | no |
| `Ship *ship = this->selectedItem->ship;` inside `if (tab != 0)` | 83.7 | 9676 | – | – | no |
| `Ship *s = ...` before `if (tab != 0)` | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `ListItem *sel = ...` before `if (tab != 0)` | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `HangarWindow *self = this;` before `if (tab != 0)`, body uses `self` | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `HangarWindow *self = this;` at start of body, body uses `self` | 85.7 | 9688 | r6,r5 | r4 (pre) | no |
| `volatile int *padX = (volatile int *)this;` | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `int keep = this->field_0x114;` before branch, body uses `keep` | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `(void)this->field_0x100.a;` after `if (item->isShip())` | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `int dummyThis = this->field_0x100.a;` before branch | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `HangarWindow &self = *this;` before branch, body uses `self.` | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `HangarWindow *self = this;` at function top, body uses `self->` | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `bool upg = this->upgradeMode;` before branch | 89.9 | 9696 | r6,r5 | r4 (pre) | no |
| `__builtin_assume(this != 0)` before branch | 89.7 | 9696 | r6,r5 | r4 (pre) | no |
| `__builtin_assume(this->selectedItem != 0)` before branch | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `__builtin_assume(Globals::layout != 0)` before branch | 89.7 | 9696 | r6,r5 | r4 (pre) | no |
| `__builtin_assume(tab<6 && this != 0)` in upgrade cond | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| nested `if (tab == 3) { if (...) goto; }` | 89.8 | 9696 | r6,r5 | r4 (pre) | no |
| `this->selectedItem != 0` appended to ship cond | 89.3 | 9708 | r6,r5 | r4 (pre) | no |
| `this->upgradeMode` appended to ship cond | 89.3 | 9708 | r6,r5 | r4 (pre) | no |
| `this->upgradeMode` prepended to ship cond | 94.1 | 9700 | r6,r5 | r4 (pre) | no |
| `Globals::layout != 0` prepended to ship cond | 85.8 | 9716 | – | – | no |

### Same-TU inline helpers (all fully inlined and coalesced)

Added `static inline` helpers next to `hangarFont()` and used them in the
condition and/or body: `hangarSelectedShip(HangarWindow*)`,
`hangarSelectedItem(HangarWindow*)`, `hangarButtons(HangarWindow*)`,
`hangarHintX(HangarWindow*)`, and a whole `hangarShipButton1(HangarWindow*, int,
int, int&)` extracted body. Every one is byte-identical to the control
(89.8 %, 9696 B) — the inlined parameter is coalesced with `this`.

### Locals that pin `this` to r4 but not the split

Hoisting `Array<TouchButton *> *btns = this->buttons;` (wave-135 shape) on the
base `goto` shape gives `this=r8, layout=r9, ship=r5, btns=r4` (87.9 %, 9696 B).
Adding `Layout *lay = Globals::layout;` as well gives the first shape with
**`this=r4`**:

```
AA: Array<TouchButton*> *btns = this->buttons;
    Layout *lay = Globals::layout;
    ...
977 ldr  r4,[sp,#136]      ; this -> r4   <-- original pair
981 ldr.w r9,[sp,#132]     ; layout -> r9
990 ldr  r6,[r1,#12]       ; ship -> r6   (does NOT clobber r4)
993 cmp  r6,r0
994 beq.w join
995 ldr  r3,[r5,#4]        ; body
```

`AA` = 85.3 %, 9696 B: `this=r4` is reachable, but `ship=r6` (no clobber, no
reload) and `layout` moved to r9. Adding `int hx`/`int fy` on top re-rolls the
whole allocation (`this=r6`, `layout=sl`, `ship=r4`; or 9720 B).

### Landed flipped shape perturbations

| variant | pct | size | note |
|---|---:|---:|---|
| landed (control) | 94.7 | 9700 | pair r5,r4; ship r1 after call |
| + `btns` hoist | 89.6 | 9696 | pair r8,r5 |
| + `int hx = this->hintOffsetX;` | 94.3 | 9700 | pair split `ldr r6,[sp,#132]`/`ldr r5,[sp,#136]` |
| + `int bx = Globals::layout->buttonInsetX;` in body | 89.5 | 9692 | pair r5,r4, ship after call |

## Conclusion

The last delta is a *live-range split* of `this` at the ship load. The allocator
only splits when it runs out of a callee-saved register at the ship def; in the
base-order build r6 is free (this=r5, layout=r6), so ship takes r4 and nothing
is split. The landed flipped shape already has `this=r4, layout=r5`; the base
order re-rolls it because `ship` becomes a second live-across-call value and
`ship`'s machine vreg (`%403`) is lower than `this`'s (`%613`).

## Open questions / next levers

* Force the allocator to split `this` at the ship def while keeping `this=r4`:
  fill r5..r11 with live-across-call values so the ship def has no free
  callee-saved register and the lowest-priority interval (`this`) is split.
  `AA` already gives `this=r4`; a companion that consumes r6 (ship's slot)
  without re-rolling `this` is the missing piece.
* Raise `this`'s machine vreg priority above `ship`'s. `this` is `%613`, ship is
  `%403`; the tie-break is `~Reg` (lower wins). A source shape that makes the
  ship load's vreg number larger than `this`'s would win the tie.
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

Scratch (not committed): `/dev/shm/w8217/{score.py,dump.py,run.py,batch.py,
mkgoto.py,ir_cmd.sh,HangarWindow.landed.cpp,HangarWindow.goto.cpp,AA.cpp,BTNS.cpp,
v1.py..v7.py}`.
