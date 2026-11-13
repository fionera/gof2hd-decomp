# `Gun::calcCharacterCollision` wave 191b (w13752): the 455 plateau holds;
# the residual is the `%stack.10` stack-slot merge, not only the RA order

Worker: w13752. Branch `wave191b-w13752`, base master `32b89cb8a`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13752`.
Predecessor note: `gun-calccollision-wave191b-w13736.md` (RA-priority plateau).

## 0. Result

**No `src/` change landed.** The committed body remains the raw optimum:

| build | raw | aligned | size | insns | `.text` | `.ARM.extab` | `.ARM.exidx` |
|---|---:|---:|---:|---:|---:|---:|---:|
| committed (HEAD, base `32b89cb8a`) | **455** | **93** | 1984/1984 | 660/660 | 0x7c0 | 0x10 | 0x8 |
| faithful C-D-E (`expect(magnitude > distance, 0)`) | 463 | **57** | 1984/1984 | 660/660 | 0x7c0 | 0x10 | 0x8 |

`check` re-run after restoring the pristine body: `byte_exact 4508 / linked_exact
4508 / size 4524 / data 843/843 / imports 207 / lint CLEAN / RATCHET PASS`.
`git status --porcelain` shows only this note.

## 1. Raw byte-run decomposition (51 runs, 455 bytes)

`python3 scratch/w13752/runs.py` (same `data()`/`sym()` helpers as
`tools/fn_diff.py`) prints the same-address byte runs. Grouped:

| function-relative range | bytes | cause |
|---|---:|---|
| `0x0df,0x0ea,0x0fe,0x13c,0x142,0x14c,0x1ca` | 7 | `i*3` in r5 (ours) vs r4 (orig) |
| `0x1de..0x204` (7 single bytes) | 7 | branch displacements from the 2-byte phase shift |
| `0x208` (26) + `0x223` (158) + `0x2c2` (42) | 226 | i-loop phase shift: ours emits the 2-byte `add r4,sp,#104` where the original emits the 4-byte `ldr.w fp,[sp,#40]` (`i` reload), then compensates with one extra 4-byte `ldr.w fp,[sp,#40]` at `0x2e8` |
| `0x438` (4) + `0x43e` (40) | 44 | view block: ours rematerialises `add r0/r1/r4,sp,#104` 3x; the original keeps the address in r5 (`add r5,sp,#104` once + 3 `mov`) |
| `0x494,0x4a8` | 2 | branch displacement / `i*3` |
| `0x4b0..0x576` | 117 | k-loop block placement (body-first vs header-first) + `i*3` r5/r4 |
| `0x67a,0x68e,0x6aa` | 3 | intra-function literal-pool addresses (downstream of the layout) |

The 226-byte blob is a pure 2-byte phase shift, not 226 independent errors.

## 2. New evidence: the stack-slot merge is the real root, and it is *swapped*

The w13736 note framed the residual as a greedy-RA priority decision
(`%308 = &%stack.10` span 14608 takes r4; `%99 = i*3` span 12832 falls to r5).
That is correct but incomplete: `%stack.10` is a **StackColoring merge group**,
and the group membership itself differs between the original and our build.

Frame objects (`Merge disjoint stack slots`, `mbp.err:107983`):

```
fi#10  60 B align 8   -> sp+104   (Matrix view slot)
fi#11  60 B align 4   -> sp+44    (MatrixSetTranslation sret slot)
```

`%308 = t2ADDri %stack.10` (fi#10, sp+104) is the hoisted address of the
**playerPosition** alloca; StackColoring then merges the view and the k-loop
`getPosition` temp into the same physical slot. Which alloca lands in which
60-byte slot differs:

| slot | original | ours |
|---|---|---|
| `sp+104` (fi#10, align 8) | `delta`, `view`, k-loop `getPosition` temp | `playerPosition`, `view`, k-loop `getPosition` temp |
| `sp+44` (fi#11, align 4) | `playerPosition`, mine-branch `enemyPosition-position` temp, `MatrixSetTranslation` sret | `delta`, mine-branch `enemyPosition-position` temp, `MatrixSetTranslation` sret |

Direct disasm witnesses (function-relative `add rX, sp, #N`):

```
orig 0x142  add r4, sp, #44      ; getPosition sret  = playerPosition @ sp+44
orig 0x14c  add r5, sp, #104     ; operator- sret    = delta          @ sp+104
orig 0x438  add r5, sp, #104     ; view memcpy dst   = view           @ sp+104
orig 0x448  add r0, sp, #44      ; MatrixSetTranslation sret          @ sp+44
orig 0x4b4  add r4, sp, #104     ; k-loop getPosition sret            @ sp+104

ours 0x142  add r4, sp, #104     ; playerPosition @ sp+104
ours 0x14c  add r5, sp, #44      ; delta          @ sp+44
ours 0x43a/0x44c/0x45a  add r0/r1/r4, sp, #104
ours 0x44a  add r0, sp, #44
ours 0x208/0x260  add r4, sp, #104
```

The mine-branch temps are identical in both (`sp+44` for the `operator-`
temp, `sp+104` for the `VectorNormalize` sret, at `0x65a..0x696`).

Why ours is the "natural" first-fit: with the five relevant intervals in IR
order — `playerPosition [161,163]`, `delta [163,164]`, mine temp, mine
normalize sret, `view [731,742]`, `MSTR [742,742]`, ktemp `[819,823]` —
greedy first-fit gives `S1={playerPosition,mine-temp,view,ktemp}@sp+104`,
`S2={delta,normalize-sret,MSTR}@sp+44`, i.e. our assignment. The original
needs `view` to merge into `delta`'s slot and `playerPosition` into the
`MSTR` slot, which no source permutation tried so far produces while keeping
the 208-byte frame and 1984-byte size.

`%308` hoisting (`pre.licm` vs `post.licm`): before LICM there are five
separate `t2ADDri %stack.10` (`pre.licm:295 %114`, `:878 %198`, `:903 %208`,
`:1234 %275`, `:1359 %308`). LICM hoists only the last (`%308`) to `bb.5`
(`post.licm:142`) and the earlier sites are then CSE'd onto it, giving the
7-use, 14608-slot live range. `%198`/`%208` (single-use, in the mine branch
`bb.52`) are *not* hoisted — single-use addresses stay put. Preventing the
hoist of the multi-use address is not reachable from source (a stack-slot
`add` is always loop-invariant), and every shape that removes
`playerPosition`/`delta` entirely (`expr`, 459/103) or grows/shrinks the
frame moves the whole stream.

## 3. The k-loop guard, re-confirmed

Original k-loop is body-first / rotated (`movs r5,#0; b 0x4fc` at
`0x4f6`, body at `0x4b2`, latch `adds r5,#1` at `0x4fa`, header at
`0x4fc`), guard `cmp r0,r1; ble.n latch` (body is the fall-through, so
`magnitude > distance` is treated as likely). Ours is header-first with the
latch inline (`subs r1,r0,r1; cmp r1,#1; bge body`), i.e. the
`__builtin_expect(diff > 0, 0)` cold-body layout.

Reachable from source in exactly two ways, both measured:

* `expect(magnitude > distance, 0)` -> 463 raw / **57 aligned** / 1984 B
  (best aligned; the body is still out-of-line, so the k-loop block
  placement is not fully fixed);
* `expect(magnitude <= distance, 1) { continue; }` with emp-then-damage
  -> 460 raw / 89 aligned / 1984 B.

Removing the `expect` entirely (plain `if (diff > 0)` or
`if (magnitude > distance)`) or flipping it to `1` drops the frame to
**1908 B / 646 insns** (`raw 1885-1887`, `aligned 277-281`) — gate-fatal.
So the base's `expect(diff > 0, 0)` is load-bearing for the 1984-byte size.

## 4. Cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

Size-exact 1984 / 660 insns unless stated; none beats 455.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed | **455** | **93** | 1984 |
| decl order `velocity,position,enemyPosition` | 470 | 116 | 1984 |
| decl order `enemyPosition,position,velocity` | 538 | 137 | 1984 |
| decl order `enemyPosition,velocity,position` | 538 | 140 | 1984 |
| decl order `position,enemyPosition,velocity` | 500 | 117 | 1984 |
| decl order `velocity,enemyPosition,position` | 509 | 132 | 1984 |
| decl `radius` before the three `Vector`s | 1785 | 239 | 1968 (656) |
| `Matrix view(cam)` copy-ctor (two spellings) | 455 | 93 | 1984 |
| view via named `float *cam` | 455 | 93 | 1984 |
| view via `Matrix &vref` alias | 455 | 93 | 1984 |
| view + trailing `(void) view;` | 455 | 93 | 1984 |
| `canvas` cached in a local | 1342 | 202 | 1984 (657) |
| `geom = geometries[i]` cached | 1160 | 182 | 1988 (662) |
| impact inner `nullptr` check removed | 1096 | 135 | 1980 (658) |
| `playerPosition` reused for the delta | 1555 | 160 | 1992 (663) |
| `Vector delta; delta = enemyPosition - playerPosition;` | 1651 | 318 | 1964 (661) |
| `Vector delta = enemyPosition - getPlayer()->getPosition()` | 459 | 103 | 1984 |
| `playerPosition` + named `delta` + `VectorLength(delta)` | 455 | 93 | 1984 |
| `const Vector &playerPosition` / `const Vector &pp` | 455 | 93 | 1984 |
| `auto playerPosition` | 455 | 93 | 1984 |
| `Vector playerPosition(...)` copy-ctor / `{...}` braces | 455 | 93 | 1984 |
| `PlayerEgo *player` cached, then `player->getPosition()` | 455 | 93 | 1984 |
| `Level *lvl` cached | 455 | 93 | 1984 |
| `VectorLength(enemyPosition - player->getPosition())` inline | 459 | 103 | 1984 |
| `-playerPosition + enemyPosition` / `enemyPosition + -playerPosition` | 1777 | 306 | 1956 (658) |
| plain `if (diff > 0)` | 1887 | 281 | 1908 (646) |
| plain `if (magnitude > distance)` | 1885 | 277 | 1908 (646) |
| `expect(magnitude > distance, 1)` | 1885 | 277 | 1908 (646) |
| `expect(diff > 0, 1)` | 1887 | 281 | 1908 (646) |
| `hitPositions`/`hitFlags` store swapped | 476 | 99 | 1984 |
| `hitFlags`/`hitPositions` moved after the k-loop | 1850 | 343 | 1956 (661) |
| k-loop bound cached (`n = enemies->size()`) | 1714 | 306 | 1960 (653) |
| `hitFlags[i] = true` | 455 | 93 | 1984 |
| k-loop `do { } while (++k < size)` | 1027 | 156 | 1988 (662) |
| k-loop `while (k < size) { ... ++k; }` | 455 | 93 | 1984 |
| i-loop `do { } while (++i < count)` | 1858 | 296 | 1944 (651) |

Prior w13736 table (~90 cells: `alignas(8)` any, `Vector *posData` cache,
`Vector *pp = positions.data()+i`, `const Vector` any/all, `long/short
distance`, `int/unsigned long i`, `int e`, `int k`, `lifetimes[i]=0xfff0bdc0`,
`setBombForce(0)`, `owner==0`, the `SetAnimationState` swap, the
`enemyPosition,velocity,position` reorders, `Matrix view[1]`,
`Matrix view;`+`operator=`, `*td = kpos = ...`, `diff_first`, impact/scatter
block swap, `TransformGetTransform` cache, `enemies`/`level`/`td`/`impact`
caches, the mine-branch `expect` axis/sense sweep, the main collision-test
`expect` sweep, `expect(diff > 0, 1)`/`>= 1`/`!= 0`, ...) remains valid and
is not repeated here.

## 5. Conclusion

The 455 plateau is stable across ~125 measured source shapes. Two distinct
root causes, both upstream of the visible clusters:

1. **StackColoring merge swap** (§2): the original puts `playerPosition` in
   the `MatrixSetTranslation` slot (`sp+44`) and `delta` in the `Matrix view`
   slot (`sp+104`); ours is the reverse. This is what makes `%308` the
   hoisted playerPosition address with a 14608-slot live range.
2. **Greedy-RA priority** (w13736 §2): that 14608-slot range takes r4, so
   `i*3` (12832) falls to r5, which cascades into the 7 `i*3` bytes, the
   2-byte i-loop phase shift, the view-block rematerialisation and the
   post-RA k-loop rotation.

A future pass should attack the merge order (e.g. by finding the original
source's declaration/scope shape for `playerPosition`/`delta`/`view`) rather
than the RA directly; the RA order follows from it.

The faithful C-D-E body (463 / **57** / 1984 / 660, `.text` 0x7c0,
`.ARM.extab` 0x10, `.ARM.exidx` 0x8) remains the best aligned candidate and
is recorded here for a future pass that values aligned over raw.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13752
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# committed base -> raw=455 aligned=93 size=1984/1984 insn=660/660
python3 /home/debian/GalaxyOnFire2/scratch/w13752/runs.py   # 51 runs / 455 bytes
python3 /home/debian/GalaxyOnFire2/scratch/w13752/dd.py     # aligned instruction diff
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Per-function section sizes (`arm-linux-androideabi-objdump -h
cmake-build-match/CMakeFiles/gof2.dir/src/game/weapons/Gun.cpp.obj`):

```
.text._ZN3Gun22calcCharacterCollisionEv            000007c0
.ARM.extab.text._ZN3Gun22calcCharacterCollisionEv  00000010
.ARM.exidx.text._ZN3Gun22calcCharacterCollisionEv  00000008
```

## 7. Files changed

`_work/reconstructions/gun-calccollision-wave191b-w13752.md` only.
