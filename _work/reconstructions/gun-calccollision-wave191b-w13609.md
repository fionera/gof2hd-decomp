# `Gun::calcCharacterCollision` wave 191b (w13609): the faithful C-D-E body
# stays an aligned-only win; the `%308`/`i*3` r4/r5 swap is an RA plateau

Worker: w13609. Branch `wave191b-w13609`, base master `c31ee05b0`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13609`.

## 0. Result

**No `src/` change landed.** The committed body remains the raw optimum:
**raw 455 / aligned 93 / 1984 B**, 660/660 insns.

| build | raw | aligned | size | insns | `i*3` reg |
|---|---:|---:|---:|---:|---|
| committed (HEAD `c31ee05b0`) | **455** | **93** | 1984/1984 | 660/660 | r5 |
| faithful C-D-E (`expect(mag>dist,0)`) | 463 | **57** | 1984/1984 | 660/660 | r5 |
| faithful C-D-E + diff-first scatter | 467 | 67 | 1984/1984 | 660/660 | r5 |
| faithful C-D-E + diff-first + `alignas(8) Matrix view` | 467 | 67 | 1984/1984 | 660/660 | r5 |
| C-E-D body, `expect(diff>0,0)` | 457 | 84 | 1984/1984 | 660/660 | r5 |
| C-D-E body, `expect(diff>0,0)` | 458 | 63 | 1984/1984 | 660/660 | r5 |
| clamp hoisted out of the `if` | 459 | 89 | 1984/1984 | 660/660 | r5 |

`.text` (0x1337c8), `.ARM.extab` (0x26298), `.ARM.exidx` (0x6f70) and `.rodata`
(0x41b67) are **identical in size** between the committed and faithful builds;
the faithful body changes no section size, so the aligned win is free on the
section budget (the 577 differing `.ARM.extab` bytes are unchanged content,
not growth).

`git status --porcelain` is empty apart from this note.

## 1. New evidence (a): the `%308` live interval is *not* fixable by
## separating the k-loop sret alone

From the `-mllvm -print-after-all` Live Interval dump of the faithful body:

| slot use | slot index |
|---|---:|
| `%308 = t2ADDri %stack.10` def (hoisted to bb.5, e-loop preheader) | 1568 |
| scatter `playerPosition` sret / `operator-` arg | 3616 / 3728 |
| view block: memcpy dest / `MatrixSetTranslation` arg / `TransformSetLocal` arg | 14704 / 14896 / 15072 |
| k-loop `getPosition` sret / `Vector::operator=` arg | 16080 / 16176 |

`%308` size = 14608; `%99` (`i*3`, def 2880, last use 15712) size = 12832.
`RAGreedy::enqueue` is size-descending, so `%308` is dequeued first and takes
r4; `i*3` falls to r5.

The arithmetic shows why w13576's "give the k-loop sret its own slot" lever is
*not sufficient*: with the k-loop sret separated, `%308` still spans
1568..15072 = **13504 > 12832**. Only two shapes can win:

* the hoist is prevented, so the def moves to the first use at 3616 →
  16176-3616 = **12560 < 12832** (margin only 272), or
* *both* the scatter temp and the view are separated from the k-loop sret →
  `%308` = 1568..3728 = 2160.

## 2. New evidence (b): disabling MachineLICM does not flip the swap

Compiling the committed body with `-mllvm -disable-machine-licm` (a probe, not
a landable change) removes the hoist entirely: the view/`%stack.10` address is
rematerialised per block (`add r5,sp,#104` at `0x410`/`0x494`, `add r6,sp,#104`
at `0x63c`/`0x668`). The allocator nevertheless re-runs and lands on a
**different 1928-B allocation** in which `i*3` is *still* r5 and **r4 is
`&enemy->radius`** (`add.w r4,r6,#64` at `0xfa`). The committed and faithful
bodies produce *identical* code under `-disable-machine-licm` (0x788 = 1928 B),
so the hoist is not the sole cause of the plateau — removing it just re-runs
the greedy allocator to a different fixed point.

## 3. New evidence (c): the swap *is* reachable, but only by changing the
## view's live range (and therefore the size)

Making `Matrix view` live across the scatter branch (so `%stack.10` cannot be
shared with the scatter temp / k-loop sret) **does** give `i*3` r4 — the
mechanism is confirmed — but every such form grows the function:

| variant | raw | aligned | size | insns | `i*3` |
|---|---:|---:|---:|---:|---|
| `Matrix view;` at fn scope, `view = *(...)` in block | 1890 | 333 | 2016 | 675 | **r4** |
| `Matrix view;` at i-loop top, `view = *(...)` in block | 1872 | 375 | 2024 | 682 | **r4** |
| `Matrix view;` at i-loop top, `__builtin_memcpy(&view, …, 60)` | 1866 | 367 | 2024 | 682 | **r4** |
| faithful + `Matrix view;` at i-loop top + memcpy | 1861 | 360 | 2024 | 682 | **r4** |

The `operator=` / explicit-memcpy rewrite is what costs the extra instructions
(682 vs 660); the plain `Matrix view = *(...)` form (memcpy inline) only works
when the alloca is created in the geometries block, i.e. when it *is* shared
with the scatter temp.

## 4. Cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

Size-exact 1984 / 660 insns unless stated; none beats raw 455.

### 4.1 k-loop body order / guard cross-product (committed base)

| order | guard | raw | aligned |
|---|---|---:|---:|
| E-C-D | `expect(diff>0,0)` | **455** | **93** |
| E-C-D | `expect(mag>dist,0)` | 460 | 89 |
| C-E-D | `expect(diff>0,0)` | 457 | 84 |
| C-E-D | `expect(mag>dist,0)` | 462 | 78 |
| C-D-E | `expect(diff>0,0)` | 458 | 63 |
| C-D-E | `expect(mag>dist,0)` | 463 | 57 |
| D-C-E | `expect(diff>0,0)` | 463 | 83 |
| D-C-E | `expect(mag>dist,0)` | 466 | 80 |
| D-E-C / E-D-C | any | 1794–1803 | 157–171 | (1940 B) |

`expect(this->magnitude - distance > 0, 0)` is codegen-identical to
`expect(diff>0,0)`; `expect(distance < this->magnitude, 0)` is identical to
`expect(mag>dist,0)`.

### 4.2 scatter-operand / slot-shape sweep (committed + faithful base)

Codegen-identical to the base (455/93 or 463/57): `const Vector
playerPosition/enemyPosition`, `Vector playerPosition` + `Vector diff` named,
`const Vector diff`, `Vector *pp = &playerPosition`, `Level *lvl` local,
`Vector dv = enemyPosition - playerPosition` then `VectorLength(dv)`,
`this->positions[i]` (Array `operator[]`), `reinterpret_cast` hitPositions
index, `int baseRadius = enemy->radius` local, `useCustomRadius != 0`.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| diff-first scatter (`Vector diff = enemyPosition - getPosition()`) | 467 | 67 | 1984 |
| diff-first + `alignas(8) Matrix view` | 467 | 67 | 1984 |
| diff-first + C-E-D body | 466 | 88 | 1984 |
| `alignas(8) Vector enemyPosition` | 490 | 95 | 1984 |
| cached `Vector *posData`/`velData` outside the i-loop | 1654 | 218 | 1992 |
| cached `posData` inside the i-loop | 477 | 97 | 1984 |
| `alignas(16) Matrix view` | 1862 | 209 | 1996 |
| `Matrix view;` before the two `SetAnimationState` | 1891 | 248 | 2056 |

### 4.3 k-loop arithmetic / loop forms (committed base)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `*td = enemy->getPosition(); *td -= position;` (committed) | **455** | **93** | 1984 |
| `*td = enemy->getPosition() - position;` | 1793 | 308 | 1956 |
| `VectorLength(*td - position)` (length before `-=`) | 1813 | 404 | 1888 |
| `Vector kp = getPosition(); *td = kp - position;` | 1794 | 300 | 1956 |
| `VectorLength(*td)` before `*td -= position` | 1796 | 347 | 1936 |
| `while (k < size) { …; ++k; }` (body-first) | 455 | 93 | 1984 |
| `do { … } while (k < size)` | 1027 | 156 | 1988 |
| `expect(k < size, 0/1)` on the loop condition | 1710/1798 | 341/379 | 1944/1948 |

### 4.4 i-loop index / asteroid-path forms (committed base)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `i != count` / `i = i + 1` | 456/455 | 94/93 | 1984 |
| `while (i < count)` | 1731 | 379 | 1892 |
| cached `positions.count` | 1681 | 187 | 1988 |
| `if (!mine) { setBombForce; if (explosive) … }` | 1735 | 453 | 1956 |
| setBombForce before `if (mine) continue` | 456 | 101 | 1984 |
| `hitFlags[i] = 0` before `continue` (forces an `i` use) | 1665 | 175 | 1996 |

## 5. Conclusion

The residual is unchanged from w13576/w13588/w13602: one greedy-RA decision
(`%308` = the shared 60-B `%stack.10` slot takes r4, pushing `i*3` to r5).
`%308`'s last use is the k-loop `getPosition` sret at slot 16176; the view
block ends at 15072, so separating the k-loop sret alone still leaves
13504 > 12832. The only source lever that flips the swap is making the view
live across the scatter branch, which grows the function to 2016–2024 B.

The faithful C-D-E body (463/57/1984, `.ARM.extab` unchanged) is a genuine
*aligned* win but an 8-byte *raw* regression; because prior waves
(w13365/w13388) deliberately traded aligned for raw to reach 455, the
committed body is kept and the faithful form is recorded here.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13609
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# committed base -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

Faithful C-D-E body (raw 463 / aligned 57 / 1984 exact):

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

## 7. Files changed

`_work/reconstructions/gun-calccollision-wave191b-w13609.md` only.
