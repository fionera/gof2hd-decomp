# `Gun::calcCharacterCollision` wave 191b (w13619): the k-loop rotation and
# the 1984-B RA anchor are driven by the *same* `expect` weight; no landed win

Worker: w13619. Branch `wave191b-w13619`, base master `7301219a1`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13619`.

## 0. Result

**No `src/` change landed.** The committed body remains the raw optimum:
**raw 455 / aligned 93 / 1984 B**, 660/660 insns, `i*3` in `r5`.

| build | raw | aligned | size | insns | `i*3` |
|---|---:|---:|---:|---:|---|
| committed (HEAD `7301219a1`) | **455** | **93** | 1984/1984 | 660/660 | r5 |
| faithful C-D-E `expect(mag>dist,0)` | 463 | **57** | 1984/1984 | 660/660 | r5 |

`.text`, `.ARM.extab`, `.ARM.exidx` and `.rodata` are byte-size identical
between the committed and faithful builds (`.text` 0x69440, `.ARM.extab`
0x1a3b78, `.ARM.exidx` 0x19cc08, `.rodata` 0x1c9e10), so the faithful body is
still a free *aligned* win — but an 8-byte raw regression, as w13602/w13609
already recorded.

`check` is green on the untouched tree: byte 4508, linked 4508, size 4524,
stub 0, missing 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
`RATCHET PASS`.

`git status --porcelain` is empty apart from this note.

## 1. New evidence (a): *why* the k-loop is not rotated — the MBP chain

At `-Oz`, `MachineBlockPlacement::findBestLoopTop` returns the loop header
(`if (F->getFunction().optForSize()) return L.getHeader();`), so the k-loop
top is the condition block. `findBestLoopExit` then picks the only exiting
block (the condition block itself, edge to the i-loop latch, 3.12%).

`buildChain` walks the *hot* successor at every step. From the MIR before the
late post-RA `Branch Probability Basic Block Placement`:

```
bb.89 (cond)  successors bb.90(96.88%) bb.31(3.12%)
bb.90 (body)  successors bb.91(0.05%)  bb.95(99.95%)   <-- the `expect` then-block is cold
bb.95 (latch) successor  bb.89
```

so the chain is `[bb.89, bb.90, bb.95, bb.91, bb.92, bb.93, bb.94]` — the
latch is pulled in third and the cold then-blocks are appended at the end.
`rotateLoop` then bails on its last guard:

```cpp
if (ViableTopFallthrough) { ... if (!Bottom->isSuccessor(Top)) return; }
```

`Bottom` is `bb.94` (its only successor is `bb.95`), not a successor of
`Top = bb.89`, so **no rotation**. In the original the then-block is the
fallthrough of the body-start, so the chain ends at the latch and
`Bottom->isSuccessor(Top)` is true → the chain is rotated body-first, exactly
the original's layout.

This is the missing link between w13564 ("the rotation is downstream of the
RA") and w13583 ("increment-at-top rotates the loop"): the chain order is
decided by the *branch weight on the k-loop `if`*, i.e. by the same
`__builtin_expect(diff > 0, 0)` that anchors the 1984-B spill-heavy RA.

## 2. New evidence (b): the RA anchor and a hot then-block are mutually exclusive

Every source shape that makes the k-loop then-block hot (so the chain follows
it and the loop rotates) loses the 1984-B RA and drops to 1908–1920 B:

| k-loop guard | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| committed `expect(diff>0,0)` (then cold) | **455** | **93** | 1984 | 660 |
| faithful `expect(mag>dist,0)` (then cold) | 463 | **57** | 1984 | 660 |
| `!expect(mag<=dist,1)` (then cold) | 463 | 57 | 1984 | 660 |
| `!expect(mag<=dist,0)` (then **hot**) | 1886 | 257 | **1908** | 646 |
| `!expect(diff<=0,0)` (then **hot**) | 1886 | 257 | **1908** | 646 |
| plain `mag>dist`, `expect(empDamage>0,0)` | 1880 | 257 | **1912** | 648 |
| plain `mag>dist`, `expect(empDamage>0,1)` | 1881 | 263 | **1912** | 648 |
| plain `mag>dist`, plain `empDamage>0` | 1886 | 257 | **1908** | 646 |
| plain `mag>dist`, `expect(playerGun,0)` | 1865 | 255 | **1920** | 651 |
| plain `mag>dist`, `expect(falloff>1,0)` | 1886 | 257 | **1908** | 646 |
| plain `mag>dist`, `expect(explodeOnContact,0)` | 1887 | 293 | **1908** | 645 |

The 1984-B RA is anchored *only* by the cold k-loop magnitude/diff branch.
No expect placement elsewhere reproduces it.

## 3. New evidence (c): the source is at a very wide codegen fixed point

~30 independent perturbations of the committed body are **codegen-identical**
(455 / 93 / 1984 / 660, `i*3`=r5), i.e. the residual is not reachable by
wording changes:

```
const Vector playerPosition;  Vector diff = enemyPosition - playerPosition;
Vector *pp = &playerPosition;  Vector playerPosition; playerPosition = ...;
Matrix &vp = view;  Matrix *vp = &view;  Vector *td removed (this->targetDir);
this->positions.size();  positions.data()+i;  int mine/scatter/bomb;
if (mine||bomb);  if (!this->enemies);  enemies->data()[e];  isActive()==0;
getHitpoints()<=0;  radius if/else;  radius*5;  radius/2.0f;  20000<distance;
ki && field_0x40;  setBombForce(0);  damage(9999,0,...);  !this->owner;
empDamage != -979797979;  ++field_12c split;  static_cast/paren/*1.0f/+0 on
VectorLength;  for(;;){...;++k} / while(...){...;++k} / split `if (scatter)`.
```

## 4. New evidence (d): slot-separation / alignment shapes all grow the function

Aimed at the task's option (2) ("separate both the scatter temp and the view
from the k-loop sret"). `alignas(16)` on any of the 12-B temps or the 60-B
view grows the frame by 12 B and the function by 12 B (4 insns); struct
padding and `Matrix`-sized temps are worse:

| variant | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| `alignas(16) Vector playerPosition` | 1859 | 245 | **1996** | 664 |
| `alignas(16) Matrix view` | 1859 | 245 | **1996** | 664 |
| `alignas(16) Vector kpos` (k-loop sret) | 1859 | 245 | **1996** | 664 |
| `alignas(4) Matrix view` | 455 | 93 | 1984 | 660 |
| `alignas(8) Vector kpos` | 455 | 93 | 1984 | 660 |
| `struct { Vector v; int pad; }` scatter temp | 1802 | 173 | **2000** | 666 |
| `struct { Vector v; int pad; }` k-loop temp | 1768 | 359 | **1972** | 668 |
| `struct { Vector v; char pad[52]; }` k-loop temp | 1776 | 386 | **1972** | 668 |
| `Matrix kpos` k-loop temp | 1841 | 319 | **2048** | 687 |
| `Matrix *volatile vp = &view` | 1128 | 174 | **1988** | 662 |

`alignas(4)`/`alignas(8)` leave the codegen untouched (the slots are already
8-aligned and merge), so the k-loop sret cannot be separated from the view
slot without paying ≥12 B.

## 5. Cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

Size-exact 1984 / 660 insns unless stated; none beats raw 455.

### 5.1 scatter-operand / slot shapes (committed base)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed | **455** | **93** | 1984 |
| `const Vector playerPosition` | 455 | 93 | 1984 |
| `Vector diff = enemyPosition - playerPosition;` (named diff) | 455 | 93 | 1984 |
| no named temp: `VectorLength(enemyPosition - getPlayer()->getPosition())` | 459 | 103 | 1984 |
| diff-first: `Vector diff = enemyPosition - getPlayer()->getPosition()` | 459 | 103 | 1984 |
| faithful + diff-first | 467 | 67 | 1984 |
| faithful + no named temp | 467 | 67 | 1984 |
| faithful + diff-first + `alignas(8) Matrix view` | 467 | 67 | 1984 |
| committed + no named temp + `alignas(8) Matrix view` | 459 | 103 | 1984 |
| `float distance` | 1586 | 351 | **1964** |
| `Vector diff` computed then clobbered (`diff = playerPosition`) | 1578 | 306 | **1960** |

Note: the original's scatter block *does* use a named `diff`
(`add r5,sp,#104` then `operator-` then `VectorLength`), but the `diff`-first
source form that reproduces its `sp+44`/`sp+104` colouring still leaves the RA
plateau intact and costs 4–12 raw bytes.

### 5.2 k-loop forms (committed base)

| variant | raw | aligned | size | insns | `i*3` |
|---|---:|---:|---:|---:|---|
| committed `for` | **455** | **93** | 1984 | 660 | r5 |
| `while { ++k; enemy=data[k]; ... }` (increment at top) | 1248 | 143 | 1984 | 660 | r6 |
| `for (; ; ) { ...; ++k; }` / `while { ...; ++k; }` | 455 | 93 | 1984 | 660 | r5 |
| `do { ...; ++k; } while (k<size)` | 1027 | 156 | **1988** | 662 | r5 |
| `if (k<size) do { ... } while (k<size)` | 1812 | 331 | **1952** | 652 | — |
| `Vector target` local instead of `enemy` | 1880 | 386 | **1884** | 649 | — |
| `Vector kd` local instead of `*td` | 1879 | 479 | **1876** | 643 | — |

### 5.3 statement reorderings (committed base)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `hitFlags`/`hitPositions` split into a second `if (scatter)` | 455 | 93 | 1984 |
| `hitFlags`/`hitPositions` before the impact block | 1394 | 165 | **1992** |
| `hitFlags`/`hitPositions` before the view block | 1797 | 284 | **1964** |
| whole `if (scatter)` block before the view block | 1895 | 379 | **1904** |
| `-(float) radius` instead of `(float) -radius` | 1582 | 176 | **1980** |
| `int e` instead of `unsigned e` | 456 | 94 | 1984 |

## 6. Conclusion

The residual is unchanged from w13553…w13609: one greedy-RA choice
(`%308` = the shared 60-B `%stack.10` address takes r4, pushing `%99` = `i*3`
to r5). What this pass adds:

1. **The k-loop rotation and the 1984-B RA anchor are the same lever.** The
   late post-RA MBP chain follows the latch (99.95%) instead of the cold
   `expect` then-block (0.05%), so the chain bottom is `bb.94`, not the latch,
   and `rotateLoop`'s `Bottom->isSuccessor(Top)` guard fails. Any shape that
   makes the then-block hot rotates the loop but loses the 1984-B RA
   (1908–1920 B).
2. **The source is at a very wide codegen fixed point**: ~30 independent
   wording/type/order perturbations are byte-identical, so no small textual
   change can move the RA.
3. **Slot separation costs ≥12 B**: every `alignas(16)`/struct/`Matrix`-sized
   temp shape is 1972–2048 B, so option (2) cannot be size-exact.

The committed `expect(diff > 0, 0)` + E-C-D body remains the unique raw
optimum. The faithful C-D-E body (463 / **57** / 1984, sections unchanged) is
recorded again as the best *aligned* shape.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13619
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# committed base -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

Faithful C-D-E body (raw 463 / aligned 57 / 1984 exact, sections unchanged):

```c++
int distance = (int) AbyssEngine::AEMath::VectorLength(*td);
if (__builtin_expect(this->magnitude > distance, 0)) {  // lint: arm_builtin
    int diff = this->magnitude - distance;
    float falloff = (float) diff / (float) this->magnitude;
    if (falloff > 1.0f) {
        falloff = 1.0f;
    } else if (falloff < 0.0f) {
        falloff = 0.0f;
    }
    enemy->damage((int) (falloff * (float) this->damage), this->friendGun, this->itemIndex);
    if (this->empDamage > 0) {
        enemy->damageEmp((int) (falloff * (float) this->empDamage), this->friendGun);
    }
}
```

MBP recipe (same as w13588 §8):

```sh
clang++ <original flags> -g0 -S -o mbp.s \
  -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv \
  -c src/game/weapons/Gun.cpp 2> mbp.err
# "IR Dump After Branch Probability Basic Block Placement" -> block order
#   bb.89 bb.90 bb.95 bb.91 bb.92 bb.93 bb.94   (chain, latch third)
```

## 8. Files changed

`_work/reconstructions/gun-calccollision-wave191b-w13619.md` only.
