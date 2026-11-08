# Long-tail wave 143 (w8817): Gun / Generator / SpaceLounge —
# MachineBlockPlacement rotation, frame-96 `rem` reuse, boxW RA rotation

Status: **no landed `src/` change.** Worktree `wt/w8817`, base `bf083ab41`
(wave 142). Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `long-tail-wave142-w8718.md`, `long-tail-wave141-w8620.md`,
`spacelounge-generator-wave133-w7740.md`, `gun-calccharactercollision-wave133-w7739.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. Raw same-address diff bytes (fdiff):
Gun **463**, Generator **445**, SpaceLounge **175** (unchanged).

## 0. Targets attempted

| target | result |
|---|---|
| `Gun::calcCharacterCollision` (1984 B) | **new finding:** the shipped "rotation" is *MachineBlockPlacement*, not IR LoopRotate. Final `-emit-llvm` IR and the `-print-after-all` LICM dump keep the scatter k-loop **top-tested**. The whole function is a pure `r4 <-> r5` value swap. |
| `Generator::createMission` (1996 B) | **new finding:** the +8 B is a `rem` spill slot. Shipped reuses the status slot `sp+32`; ours spills to a fresh slot. Hoisting `int *imageParts = agent->getImageParts();` before the bonus block reaches frame 96 (shipped) but 677 raw / 2000 B. |
| `SpaceLounge::drawLounge` (1948 B) | **new finding:** the `boxW` 7-insn gap is a global RA rotation, already visible at insn 8 (`Globals::layout` moves `r5`->`sl`, `pad*2` `sl`->`fp`). |

## 1. Gun: the k-loop rotation is MachineBlockPlacement, not LoopRotate

Final `-emit-llvm` IR and the `-print-after-all` LICM dump keep the scatter
k-loop **top-tested**:

```
header %420:  size load + icmp + br i1 %426, label %427, label %466
latch  %464:  br label %420
```

No IR rotation exists.  The shipped asm is bottom-tested
(preheader `b.n` to the condition at `0x1534fc`, body at `0x153442`), which is
**block layout** driven by the `!prof` metadata from `__builtin_expect`.
Reproducer `/dev/shm/w8817/rot12.cpp` (with expect) lays out top-tested;
`rot12c.cpp` (no expect) lays out bottom-tested.

The whole function is a pure `r4 <-> r5` value swap: shipped `r4 = sp+44`
(MST result), `r5 = sp+104` (view); ours `r4 = sp+104`, `r5 = sp+44`.
The LICM hoist order is the dominator-tree DFS order, not source order
(bb.81 k-loop sret fi#10 visited before bb.72 MST fi#11), so greedy RA
assigns fi#10 -> r4, fi#11 -> r5, opposite of shipped.

| variant | raw | size |
|---|---:|---:|
| HEAD (`stmt`, named `Vector playerPosition`) | **463** | 1984 |
| `expr` (inline `getPlayer()->getPosition()`) | 467 | 1984 |
| named `Vector diff = enemyPosition - playerPosition` | 463 | 1984 |
| `continue;` after k-loop | 463 | 1984 |
| `Vector kpos;` outside k-loop | 1648 | **2004** |
| scatter block before impact | 1776 | **1972** |
| scatter block before explosive | 1832 | **2164** |
| `expr` + no k-loop `__builtin_expect` | 1810 | **1908** |

## 2. Generator: the +8 is a `rem` spill slot; shipped reuses `sp+32`

Faithful `bonus = rawBonus - rem` + `__builtin_expect((type|4)==12,0)` =
**386 raw / 2004 B** (frame 104).  The shipped bonus block needs three values
live across the second `__aeabi_idivmod`: `bonus` (r5), `rawBonus` (r6) and
`rem` (spilled).  Shipped spills `rem` into the status slot `sp+32`; ours
spills `agent` to a fresh `sp+36` slot and keeps `rem` in `r9`.

**NEW:** hoisting `int *imageParts = agent->getImageParts();` before the bonus
block reaches **frame 96** (shipped frame) but 677 raw / **2000 B**
(`bonus` -> `fp`, `rem` -> `r5`; `subne.w` 32-bit vs shipped 16-bit
`subne r5,r6,r0`).  Same hoist after the bonus block = 514 / **2004 B**.

| variant | raw | size | frame |
|---|---:|---:|---:|
| HEAD (`bonus -= rem*2`) | **445** | 1996 | 96 |
| faithful `bonus = rawBonus - rem` | 304 | **2000** | 104 |
| faithful + expect0 | 386 | **2004** | 104 |
| expect0 + faithful + race-expect 0 | 1833 | **1996** | 104 |
| `Agent *ag = agent` alias + expect0 | 386 | 2004 | 104 |
| inverted `if ((type\|4)!=12)` + expect0 | 386 | 2004 | 104 |
| `int bonus = 0;` before if + expect0 | 386 | 2004 | 104 |
| `if (bonus % 50)` + expect0 | 386 | 2004 | 104 |
| `bonus -= rem + rem` + expect0 | 579 | **1996** | 96 |
| `bonus -= rem*2` + expect0 | 579 | **1996** | 96 |
| `getMissionBonus(race)` (cached race) + expect0 | 612 | **1996** | 96 |
| faithful + cached race | 687 | **1992** | 96 |
| HEAD + cached race | 644 | **1988** | 96 |
| **imageParts hoist before bonus** | 677 | **2000** | **96** |
| imageParts hoist after bonus | 514 | 2004 | 104 |

Conclusion: the -8 companion must both free the `rem` slot by reusing `sp+32`
and keep `bonus` in `r5` / `rem` in `r0`.

## 3. SpaceLounge: the `boxW` 7-insn gap is a global RA rotation

`boxW` variant's first diff is already at insn 8:
`orig ldr r5,[r0,#0]` vs `ours ldr.w sl,[r0]`.  `Globals::layout` moves
`r5`->`sl`, `pad*2` `sl`->`fp`, and the whole `mode==0` block re-encodes with
32-bit high-register forms: **734 insns / 1968 B** vs shipped **741 / 1948 B**.

| variant | raw | size | pad |
|---|---:|---:|---|
| HEAD (`short pad3` clamp) | **175** | 1948 | fp |
| `int pad2 = pad*2` reused top/clamp/box | 445 | 1948 | fp |
| all `boxW` spellings (`width+pad*2`, `const`, `pad+pad+width`, boxW only in DrawRectangle) | 1833 | **1968** | r9 |
| `centerX` in clamp | 175 | 1948 | fp |
| `int x; x = centerX;` | 175 | 1948 | fp |
| `x < inset+pad` | 486 | **1944** | fp |
| split `w - inset - width - pad3*2` | 175 | 1948 | fp |

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8817
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
python3 /dev/shm/w8817/fdiff.py <mangled>                 # raw same-address diff
```

Scratch (not committed), `/dev/shm/w8817/`: `fdiff.py`, `sbs.py`
(index-based side-by-side), `aldiff.py` (difflib instruction diff),
`sizediff.py` (first instruction-size divergence), `regdiff.py`,
`cc_gun.sh` (exact per-TU compile), `gen_try*.py`, `sl_try*.py`, `rot*.cpp`,
`paa.txt` (`-mllvm -print-after-all` of Gun.cpp), `gun_fn*.ll`,
`sl_orig.txt` / `sl_show.txt`, `GUN.head.cpp` / `GEN.head.cpp` / `SL.head.cpp`.

## 5. Files changed

`_work/reconstructions/long-tail-wave143-w8817.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
