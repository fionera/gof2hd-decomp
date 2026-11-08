# Long-tail wave 142 (w8718): Gun / Generator / SpaceLounge —
# hoist-order and spill-slot probes, no landed `src/` change

Status: **no landed `src/` change.** Worktree `wt/w8718`, base `1d10d0694`
(wave 141). Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `long-tail-wave141-w8620.md`, `long-tail-wave139-w8420.md`,
`gun-calccharactercollision-wave133-w7739.md`,
`spacelounge-generator-wave133-w7740.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. Raw same-address diff bytes (fdiff):
Gun **463**, Generator **445**, SpaceLounge **175** (unchanged).

## 0. Targets attempted

| target | result |
|---|---|
| `Gun::calcCharacterCollision` (1984 B) | **new negatives:** `expr` form (`VectorLength(enemyPosition - getPlayer()->getPosition())`) = 467 raw / 1984 B (slots correct, regs still swapped); dropping the k-loop `__builtin_expect(distance < magnitude, 0)` = 1810 raw / **1908 B** (size breaks). The LICM hoist order is the *dominator-tree* DFS order, not source order: bb.81 (k-loop sret, fi#10) is visited before bb.72 (MST, fi#11) even though bb.72 < bb.81 linearly. |
| `Generator::createMission` (1996 B) | **new negatives:** the faithful `bonus = rawBonus - rem` form is a single canonical IR shape — `const`, `unsigned`, renamed, `no_rem_var`, `two_subs`, `bonus - rem - rem`, `volatile rem/rawBonus`, local `b` then `bonus = b`, inverted `if ((type|4) != 12)` with `bonus = 0` — **all** 304 raw / **2000 B** (frame 104). expect0+faithful = 386 / 2004. expect0+faithful+race-expect-0 = **1996 B** but 1833 raw (prologue r5/r8/r9 reshuffle). |
| `SpaceLounge::drawLounge` (1948 B) | **new negatives:** `int boxW = pad*2+width` before drawBox with the faithful clamp = 1833 raw / **1968 B** (frame 128, whole packing rotates); `short pad3 = pad` hoisted to just after `int pad` = 290 raw / 1948 B (pad still `fp`). |

## 1. Generator: the faithful form always spills `agent`, never `rem`

The shipped bonus block (`0x9770c..0x97722`) needs **three** values live across
the second `__aeabi_idivmod`: `bonus = rawBonus + rem` (r5), `rawBonus` (r6)
and `rem` (spilled to `sp+32`, the status slot).  The `rem*2` spelling keeps
only two (`bonus`, `rem`) live, so it needs no spill and stays at frame 96 /
1996 B — but emits `subne.w r6,r6,r5,lsl #1`, not the shipped
`subne r5,r6,r0`.

The faithful form reaches 3 live values and spills one, but our RA spills
**`agent`** (to a fresh `sp+36` slot) and keeps `rem` in `r9`; the shipped
keeps `agent` in `r9` and spills `rem` into the **status** slot.  Root cause:
our `agent` spill slot (`sp+36`) is still live at the bonus block (it is
reloaded at `0x9776a`), so evicting `agent` there is free; in the shipped the
`sp+24` agent slot has been overwritten by another value before `0x9770e`, so
evicting `agent` would cost a store and the allocator spills `rem` instead.

Measured (all faithful unless stated):

| variant | raw | size | frame |
|---|---:|---:|---:|
| HEAD (`bonus -= rem*2`) | **445** | 1996 | 96 |
| faithful `bonus = rawBonus - rem` | **304** | **2000** | 104 |
| faithful + `__builtin_expect((type\|4)==12, 0)` | 386 | 2004 | 104 |
| expect0 + faithful + race-expect 0 | 1833 | **1996** | 104 |
| `const` / `unsigned rem` / renamed / `no_rem_var` / `two_subs` / `bonus-rem-rem` | 304-306 | 2000 | 104 |
| `volatile int rem` / `volatile int rawBonus` | 701-702 | 2000 | 104 |
| expect on `offer==5` / `type==8` / `difficulty>=10` / campaign-16 / `type==12` | 386-1860 | 2000-2016 | 104 |

No spelling reached 1996 B with the faithful block.  The `-8` companion would
have to make our `agent` slot dead before the bonus block (so the allocator
spills `rem` into the status slot) without changing the block layout.

## 2. Gun: the hoist order is the dominator-tree DFS, not source order

Pre-LICM MIR (`-mllvm -print-after=...`, `expr` form):

```
bb.72 (%ir-block.512):  %282 = t2ADDri %stack.11   ; pp / MST      <- earlier in layout
bb.81 (%ir-block.586):  %308 = t2ADDri %stack.10   ; delta / view / k-loop sret
```

After `Early Machine Loop Invariant Code Motion` the i-loop preheader bb.5 is

```
%108 = t2ADDri %stack.3    ; enemyPosition
%152 = t2ADDri %stack.1    ; position
%291 = t2ADDri %stack.2    ; velocity
%308 = t2ADDri %stack.10   ; hoisted FIRST  -> gets r4
%282 = t2ADDri %stack.11   ; hoisted second -> gets r5
```

`MachineLICMBase::HoistOutOfLoop` walks the **dominator tree** (`DT->getNode(header)`
then a DFS over `Node->getChildren()`), not `CurLoop->getBlocks()`; the DFS
visits bb.81 before bb.72.  Greedy RA then assigns fi#10 -> r4, fi#11 -> r5,
the opposite of the shipped (pp fi#11 -> r4, delta fi#10 -> r5).  No source
reordering of the two source blocks changes the dominator-tree child order.

The shipped k-loop is also **rotated** (preheader `b.n` to the condition at
`0x1534fc`, body at `0x153442`) while ours is a top-tested loop
(`0x153440` condition, body `0x15344c`).  `-mllvm -rotation-max-header-size=100`
does not rotate it either, so the header-size heuristic is not the lever.

| variant | raw | size |
|---|---:|---:|
| HEAD (`stmt`, named `Vector playerPosition`) | **463** | 1984 |
| `expr` (inline `getPlayer()->getPosition()`) | 467 | 1984 |
| `expr` + no k-loop `__builtin_expect` | 1810 | **1908** |

## 3. SpaceLounge: `boxW` reaches pad=r9 only at frame 128

`int boxW = pad*2 + width;` immediately before `drawBox` (and used in
`DrawRectangle`) with the faithful `width + pad*2` clamp reaches the target
colouring in the unlinked `.o` but the whole packing rotates: linked
**1968 B**, frame **128** (`sub sp,#128` vs shipped `#120`), 734 insns vs the
shipped 741.  `short pad3 = pad` moved up to directly after `int pad` keeps
1948 B but pad is still `fp` (290 raw).

| variant | raw | size | pad |
|---|---:|---:|---|
| HEAD (`short pad3` clamp) | **175** | 1948 | fp |
| plain clamp (`width + pad*2`) | 445 | 1948 | fp |
| `boxW` local + faithful clamp | 1833 | 1968 | r9 |
| `pad3` hoisted after `int pad` | 290 | 1948 | fp |

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8718
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
python3 /dev/shm/w8718/fdiff.py <mangled>                 # raw same-address diff
```

Scratch (not committed), `/dev/shm/w8718/`: `fdiff.py`, `sbs.py` (index-based
side-by-side), `align.py` (difflib-aligned instruction diff), `cc_{sl,gen,gun}.sh`
(exact per-TU compile), `gen_try*.py`, `gun_try.py`, `sl_try*.py`,
`SL.head.cpp` / `GEN.head.cpp` / `GUN.head.cpp`, `gun_expr_paa.txt`
(`-mllvm -print-after-all`).

## 5. Files changed

`_work/reconstructions/long-tail-wave142-w8718.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
