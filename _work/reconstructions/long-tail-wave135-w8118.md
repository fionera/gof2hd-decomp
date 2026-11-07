# Long-tail wave 135 (w8118): Gun swap is a no-op, Generator `rem` spill
# needs a live `agent`, SpaceLounge pad/x is a two-register rotation

Status: **no landed `src/` change.** Worktree `wt/w8118`, base `2e51d765d`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Extends `long-tail-wave134-w7782.md`, `long-tail-wave134-w7762.md`,
`gun-calccharactercollision-wave133-w7739.md`,
`spacelounge-generator-wave133-w7740.md`.

Gate before/after (source restored): byte_exact **4506 -> 4506**,
linked_exact **4506 -> 4506**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.text` differing bytes 84,960,
`.ARM.extab` 1,404 (unchanged).

## 0. Targets attempted

| target | result |
|---|---|
| `Gun::calcCharacterCollision` (93.4 %, 1984 B) | **NEW negative: the explosive/geometries arm swap is a byte-identical object**; LICM trace is unchanged |
| `Generator::createMission` (97.2 %, 1996 B) | **NEW: the exact original `rem`-spill block is reachable, but only when `agent` is live across it; every such shape is +8 B or worse** |
| `SpaceLounge::drawLounge` (95.7 %, 1948 B) | **NEW: the `boxW` lever is a pad/x two-register rotation; `lineHeight` before `x` gives pad=r5/frame 120 at 95.2 %** |
| `ModStation::ModStation` (99.2 %, 1008 B) | faithful `pitch` re-measured 18 lines; `iPad == 1` 19 lines (prior note stands) |
| `MenuTouchWindow::OnTouchBegin`, `MGame::OnUpdate`, `.ARM.extab` | not reached (time-boxed) |

## 1. Gun: the arm swap does **not** move the LICM hoist

The handoff hypothesis was that swapping the `explosive` / `geometries`
arms moves bb.72 before bb.81 in the MachineLICM dominator DFS, making
`%282` (`%stack.11`) hoist before `%308` (`%stack.10`).

Measured with the instrumented clang (`-mllvm -debug-only=machinelicm`,
`/dev/shm/w8118/cc_gun_inst.sh`), on `/dev/shm/w8118/Gun_swap.cpp`:

```
Entering %bb.72 ... Exiting %bb.72
Entering %bb.81
Hoisting %308 = t2ADDri %stack.10.view, 0, 14   from %bb.81 to %bb.5
...
Entering %bb.71
Hoisting %282 = t2ADDri %stack.11.tmp, 0, 14    from %bb.71 to %bb.5
```

The `%stack.11` add is in **bb.71** (the geometries arm), not bb.72, and
bb.71 is still visited after bb.81.  The emitted `.o` is **byte-identical**
to HEAD (`score_gun.sh Gun_swap.cpp swap` and `Gun_head_src.cpp head` give
the same `add r4, sp, #104` / `add r5, sp, #44` order at `0x142/0x14c`).
Full link + verify: still 93.4 %, 1984 B.  Reverted.

The remaining lever is therefore *not* the source arm order; it is the
dominator-tree block order or the `IsProfitableToHoist` cheap-inst
heuristic (w7739 §4).

## 2. Generator: the exact `rem` spill is reachable, but costs 8 B

The original block (`0x9770c..0x97720`) is:

```
adds  r5, r1, r6        ; bonus = rem + rawBonus   (r1=rem, r6=rawBonus, r5=bonus)
str   r1, [sp, #32]     ; rem -> sp+32
movs  r1, #50
mov   r0, r5
blx   __aeabi_idivmod
ldr   r0, [sp, #32]     ; rem
cmp   r1, #0
it    ne
subne r5, r6, r0        ; bonus = rawBonus - rem
```

HEAD (`bonus -= rem * 2`) keeps `rem` in r5 and `bonus` in r6 (96-byte
frame).  The faithful `bonus = rawBonus - rem` (w7740) keeps `rem` in r9
(104-byte frame).  The reason is **agent liveness**: in the original,
`agent` occupies r9 across the whole bonus block (it is reloaded at
`0x97306/0x97322` and used again at `0x9776a` `getImageParts`).  In our
build the allocator splits `agent` at the float-`rem` branch and reuses
r9 for `rem`.

### 2.1 The winning shape

Any source that keeps `agent` live *immediately after* the bonus block
forces the exact original block.  The smallest such shape is a real
null-check:

```c++
int rawBonus = ...;
int rem = rawBonus % 50;
bonus = rawBonus + rem;
if (bonus % 50 != 0) {
    bonus = rawBonus - rem;
}
if (agent == 0) bonus = 0;          // <- keeps agent live, forces rem spill
```

Object (`/dev/shm/w8118/gen_p3.cpp`):

```
554: adds  r5, r1, r6        ; exact original
556: str   r1, [sp, #32]     ; exact original
558: movs  r1, #50
55a: mov   r0, r5
55c: bl    __aeabi_idivmod
560: ldr   r0, [sp, #32]
562: cmp   r1, #0
564: it    ne
566: subne r5, r6, r0        ; exact original
568: cmp.w r9, #0            ; extra null check
56c: it    eq
56e: moveq r5, r9
```

The bonus block is **byte-identical** to the original, but the null check
costs 8 B: the function is 2004 B, not 1996 B.

### 2.2 Variants measured (all keep the spill but cost bytes)

| variant | size | note |
|---|---:|---|
| HEAD (`bonus -= rem * 2`) | 1996 | rem r5, bonus r6 |
| `bonus = rawBonus - rem` | 2000 | rem r9, frame 104 |
| `+ if (agent == 0) bonus = 0;` (p3) | **2004** | **exact block**, +8 B |
| `+ if (agent == 0) bonus = bonus - 0;` | 1996 | check optimised away, rem r9 |
| `+ if (agent == 0) bonus = rawBonus + rem;` (q6) | 2000 | bonus r6, result r0 |
| `if (agent != 0 && bonus % 50 != 0)` (p1) | 1996 | rem spilled, but bonus r6 / extra `cmp` |
| `if (agent == 0) return 0;` (q8) | 2000 | rem spilled, extra branch |
| hoist `agent->getImageParts()` above the float-`rem` block (t5) | **1996** | **exact block**, but 431 diff lines (whole tail reordered) |
| `__builtin_assume(agent != 0)` (t1) | 1996 | no effect (rem r9) |

The original also has an extra `ldr r5, [sp, #24]` at `0x97722` (right
after the bonus block, before the float-`rem` block) that our source does
not emit; it is a Mission-constructor argument reload, not an `agent`
use.  Matching that reload without hoisting `getImageParts` is the
remaining lever.

## 3. SpaceLounge: pad/x are a two-register rotation

The original mode-0 path keeps:

```
pad      = r9
pad*2    = sl   (later reused as boxW)
x        = r5
width    = r6
lineHeight = fp
```

HEAD keeps `pad = fp`, `pad*2 = sl`, `x = r9`, `lineHeight = fp`
(1948 B, 95.7 %).  The `boxW = pad*2 + width` local (w7740) reaches
`pad = r9`, `x = r5`, but `pad*2` moves to `fp`, which pushes
`lineHeight` to r8 and spills `top` to the stack: frame **128** (1968 B).

New shapes measured (faithful `width + pad*2` clamp everywhere):

| variant | pad | frame | size | structural lines | raw |
|---|---|---:|---:|---:|---:|
| HEAD (`short pad3`) | fp | 120 | 1948 | 189 | **95.7** |
| faithful, no local | fp | 120 | 1948 | 179 | 94.0 |
| `boxW` after clamp | **r9** | **128** | 1968 | 619 | — |
| `boxW` after lineHeight | **r9** | **128** | 1968 | 619 | — |
| `boxW` before clamp | fp | 120 | 1948 | — | — |
| `lineHeight` before `x`, no `boxW` | **r5** | **120** | 1948 | **115** | **95.2** |
| `lineHeight` before `x`, `boxW` after | r5 | 128 | 1968 | — | — |
| `pad2` local + `boxW` | r9 | 128 | 1968 | — | — |

`lineHeight` before `x` (variant `lh_before2`) is the closest shape by
structural lines (115 vs 189) and keeps `pad*2 = sl`, but it swaps
`pad`/`x` (pad=r5, x=r9) and the official raw drops to 95.2 %.  The
target is `pad = r9`, `x = r5`, `pad*2 = sl`; every shape that reaches
`pad = r9` pays the `top` spill.

The `top` spill is the direct consequence of `pad*2` taking `fp`:
`lineHeight` then takes `r8`, and `top` (live across the `drawBox` /
`DrawRectangle` calls) has no callee-saved register left.  If `pad*2`
stays in `sl`, `lineHeight` takes `fp` and `top` stays in `r8`, exactly
as in the original.

## 4. ModStation / MenuTouchWindow

* `ModStation::ModStation`: the committed `far = yaw` shape is still
  5 structural lines; the faithful `far = pitch` shape is 18 lines
  (pool order `Pitch, Yaw, YawIPad` + schedule).  `Globals::iPad == 1`
  restores the pool order but makes the compare `cmp #1` (19 lines).
  No new shape.
* `MenuTouchWindow::OnTouchBegin` (98.1 %, 1896 B, 19 lines): the two
  diff clusters are pure Greedy-RA register naming (`ip` vs `r4` in the
  hit-test chain, and a `r5`/`r6`/`r4` rotation around the
  `getTextWidth` calls).  Not analysed further this wave.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8118
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
```

Scratch (not committed), `/dev/shm/w8118/`: `cc_gun.sh`,
`cc_gun_inst.sh`, `score_gun.sh`, `score_gen.sh`, `score_sl.sh`,
`Gun_swap.cpp`, `Generator_head.cpp`, `sweep_gen*.py`, `gen_p3.cpp`,
`gen_t5.cpp`, `sweep_sl*.py`, `w_lh_before2.cpp`, `sl_*.txt`.

## 6. Files changed

`_work/reconstructions/long-tail-wave135-w8118.md` only.
Gate: byte 4506 / linked 4506, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
