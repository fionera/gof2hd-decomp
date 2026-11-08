# Small tail wave 163 (w10918): new shape sweep, no source win

Status: **no `src/` change landed** (analysis note only).  Worktree
`wt/w10918`, base master `e51db2fc7` (wave 162).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w10918`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.text` diff **85,042** bytes,
`.rel.plt` byte-identical, all section sizes exact.

Metrics: raw same-address diff bytes (`/dev/shm/w10918/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10918/align_diff.py`).

Baseline (master `e51db2fc7`):

| fn | raw | aligned | size |
|---|---:|---:|---:|
| `Gun::calcCharacterCollision` | 463 | 57 | 1984 exact |
| `SpaceLounge::drawLounge` | 175 | 41 | 1948 exact |
| `ModStation::ModStation` C1 | 43 | 12 | 1008 exact |
| `Generator::createMission` | 162 | 64 | 1996 exact |

Every candidate below was built with `cmake --build cmake-build-match
--target link_so -j6` and measured with both scripts; all sizes are exact
unless stated.

## 1. ModStation: pool order vs register pair

The residual is exactly the two yaw-table materialisations: the original
emits `pitch, YawIPad, Yaw` (pool `[Pitch, YawIPad, Yaw]`) and assigns
`r3=YawIPad`, `r4=Yaw`; master emits `pitch, Yaw, YawIPad` (pool
`[Pitch, Yaw, YawIPad]`) with the same `r3/r4` pair.  The two shapes are
the same instruction multiset in a different order, so raw stays 43.

New shapes tried (all 1008 B exact, all **43/12** unless stated):

* `!iPad ? Yaw : YawIPad` (canonicalises to the if-form shape: pool
  `[YawIPad, Yaw]` but `r4=YawIPad`, `r3=Yaw`; semantically inverted).
* `iPad == 0 ? Yaw : YawIPad`, `iPad != 0`, `iPad == 1`, `(iPad & 1)`,
  `static_cast<bool>(iPad)`, `!(iPad == 0)`, `(const float*)` casts.
* Diamond `if (iPad) yaw = YawIPad; else yaw = Yaw;` (with and without
  braces, with `nullptr` init) — identical to master.
* `if (!iPad) yaw = Yaw;` after `yaw = YawIPad` — 688/54, 1000 B.
* Two named table pointers plus a separate selected pointer (`P/Q`) —
  43/12, 1008 B.
* `const`/`volatile`/reference variants of the two bit-loads, and
  `reinterpret_cast<const volatile int*>` — 43/12.
* `volatile int pitchBits/yawBits` — 655/122; `volatile float*` — 43/12.
* Swapping the `pitchTable`/`yawTable` declaration order — 46/13.
* Moving `yawTable` after the `pitchBits` load — 690/52, **1000 B**
  (gate-fatal); swapping the two int loads — 744/40, **1000 B**.
* Far-key rewrite (`yawTable[race]`) — 580/71, **984 B**; a named
  `farYawTable` — 43/12.

Conclusion: the remaining 43 B is the materialisation-order tie; no
source-only shape found that gets both the original pool order and the
original `r3/r4` pair.

## 2. SpaceLounge: clamp shape vs condition sum

Master is 175/41.  The original clamp is
`itt gt; addgt r1,r6,sl; subgt r5,r0,r1` (uses `sl = pad*2`), but master
computes `0 - 2*pad - width + (w-inset)`.

New shapes tried (all 1948 B exact):

* `x = w - inset - (width + pad + pad)` and all condition orders
  (`x+pad+width`, `width+x+pad`, `x+width+pad`, explicit parentheses) —
  **176/42**; the clamp still reassociates to the 4-instruction form.
* `x = w - inset - (width + (pad + pad))` — gets the exact
  `itt/addgt/subgt` clamp and the correct condition sum, but the
  register allocator moves `x` to `r9` and `pad` to `fp` and inserts two
  `mov r5,fp` — **445/54**, 1948 B.
* `(pad + pad) + width`, `width + (pad + pad)` — 445/54.
* `pad + pad + width` — 445/54; `pad + width + pad` — 176/42.
* `(w - inset) - (width + pad + pad)` — 176/42;
  `(w - inset) + (-width - pad - pad)` — 175/41 (master).
* `(w - inset) - width - pad - pad` — 464/64, **1944 B**.
* Float clamp variants (`(int)((float)...)`) — 517-536 raw, 1964-1992 B.
* `const int pad`, moving the `pad` declaration after `top` — 175/41 or
  254/72.
* `pad2 = pad + pad` used consistently in `top`, clamp and `drawBox` —
  1812/237, **1960 B**.

Conclusion: the exact clamp shape costs the `r5/r9` allocation tie; no
size-exact shape reproduces both.

## 3. Generator: bonus fold

Master is 162/64.  The original's exact bonus block is
`adds r5,r1,r6; str r1,[sp,#32]; ...; cmp r1,#0; it ne; subne r5,r6,r0`
(i.e. `bonus = rawBonus - rem`), with `rem` spilled at `[sp,#32]`.

New shapes tried (all 1996 B exact unless stated):

* `bonus = rawBonus - rem` with `volatile rem` — 705/89, **2000 B**.
* `bonus -= rem + rem` — 583/64, **2000 B**.
* `bonus -= rem << 1` — 162/64 (same as master).
* `bonus -= 2 * rem` — 162/64.
* `bonus = bonus - rem - rem` — 583/64, **2000 B**.
* `if ((rawBonus + rem) % 50 != 0)` — 593/62, **2000 B**.
* `const int rem` — 627/59; non-volatile `rem` — 627/59.
* Non-volatile `rem` + `bonus = rawBonus - rem` — 177/116, **2000 B**.
* `volatile int b` local — 690/152, **2004 B**.
* Volatile read of `type` before the `race<4 && type==13` guard —
  1891/454, **2012 B**.

Conclusion: the exact block is 4 B larger; the master shape is the best
size-exact raw result.

## 4. Gun: `i*3` r4/r5 tie and stack-slot swap

Master is 463/57.  The original keeps `i*3` in `r4` and `playerPosition`
at `sp+44`; master keeps `i*3` in `r5` and swaps the two stack slots.

New shapes tried (all 1984 B exact unless stated):

* `reinterpret_cast<Vector *>(data())[i]` — 463/57.
* `*(data() + i)` — 463/57.
* Pointer locals `posData/velData` — 485/61.
* Swapping the `position`/`velocity` declarations — 478/80.
* All 8 permutations of the four locals (`position`, `velocity`,
  `enemyPosition`, `radius`) — best was master 463/57; `radius` first
  changed the size to **1968 B**.
* `size_t i` — 463/57; `int i` — 464/58.
* `for (unsigned i = 0, n = count; i < n; ++i)` — 1677/151, **1988 B**.
* `Vector playerPosition;` hoisted to the loop top — 1593/264,
  **1964 B**.

Conclusion: the `i*3` register and the two stack slots are pure
Greedy-RA ties; no source shape found that moves them.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10918
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w10918/fdiff.py _ZN10ModStationC1Ev
python3 /dev/shm/w10918/align_diff.py _ZN10ModStationC1Ev
```

Scratch (not committed), `/dev/shm/w10918/`: `fdiff.py`,
`align_diff.py`, `textdiff.py`, `modtry*.py`, `sltry*.py`, `gentry*.py`,
`guntry*.py`, `mgtry*.py`, and the `.base.cpp` copies.

## 6. Files changed

* `_work/reconstructions/small-tail-wave163-w10918.md` (this note).
* No `src/` change: every measured candidate regressed raw and/or size.
