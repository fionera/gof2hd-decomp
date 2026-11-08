# Small tail wave 147 (w9316): SpaceLounge clamp landed; Gun / Generator /
# MenuTouchWindow / ModStation re-decoded

Status: **one landed `src/` change** (`SpaceLounge::drawLounge`). Worktree
`wt/w9316`, base `9c9744ca5` (wave 146). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave146-w9215.md`, `near-miss-trio-wave146-w9216.md`,
`near-miss-trio-wave145-w9019.md`, `SpaceLounge-drawLounge-wave123-w7315.md`.

Gate before/after (source pristine -> this pass): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.rel.plt` unchanged (4 differing bytes:
the `setVisible`/`deleteHackingGame` swap). `.ARM.extab` 1,393 unchanged.
`.text` differing bytes **85,928 -> 85,929** (the landed change is +1 raw
byte; see the METRIC-ONLY caveat in §1).

## 0. Residual classification (raw same-address bytes / aligned instructions)

| target | size | raw before | raw after | aligned before | aligned after |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` | 1984 B | 463 | 463 | 57 | 57 |
| `Generator::createMission` | 1996 B | 445 | 445 | 59 | 59 |
| `SpaceLounge::drawLounge` | 1948 B | 175 | **176** | 57 | **42** |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 | 28 | 16 | 16 |
| `ModStation::ModStation` (C1/C2) | 1008 B | 43 | 43 | 12 | 12 |

All five are size-exact.  The aligned metric is
`_work/reconstructions/align_diff.py` (branch targets normalised to
instruction indices).

## 1. SpaceLounge — LANDED: `pad + pad` in the clamp (METRIC-ONLY)

The wave-145/146 notes left the `short pad3` sign-extend workaround in the
right-edge clamp.  The original clamp is the 2-instruction
`itt gt; addgt.w r1, r6, sl; subgt r5, r0, r1`, i.e. it reuses the int
`pad * 2` that is already live in `sl` for `top`.  Writing the clamp as
`width + pad + pad` (instead of `width + pad3 * 2`) removes the workaround and
lets the int `pad + pad` value stay in play.

Landed change (one expression, `short pad3` removed):

```cpp
x = Globals::w - layout->buttonInsetX - (width + pad + pad);
```

Measured (all 1948 B):

| variant | raw | aligned | note |
|---|---:|---:|---|
| HEAD (`short pad3 * 2`) | **175** | **57** | sign-extend workaround |
| **`width + pad + pad` (LANDED)** | **176** | **42** | clamp no longer sign-extends |
| `width + pad * 2` | 445 | 54 | |
| `width + pad << 1` / `2 * pad` | 445 | 54 | |
| `width + (pad + pad)` / `(width + pad) + pad` | 176 | 42 | same IR |
| `width + pad + pad + width` | 445 | 54 | reassociates differently |
| `int pad2 = pad + pad` at top, reused | 445 | 54 | |
| `int pad3 = pad` (int, not short) | 445 | 54 | clamp matches, RA rotates |
| `short pad3 + pad3` | 497 | 74 | 1952 B |

**METRIC-ONLY caveat.**  The landed form is semantically identical to the
original clamp but the backend reassociates it to
`(w - inset) - width - pad * 2` (`movgt r1,#0; subgt.w r1,r1,fp,lsl#1;
subgt r1,r1,r6; addgt.w r9,r1,r0`) instead of the witnessed
`(w - inset) - (width + sl)`.  The source spelling `width + pad + pad` is
arguably the faithful one (it is the same int value as `top`), but the
instruction order is not the original's.  The coordinator may keep the
175-byte `short pad3` form if raw bytes are preferred; the structural
instruction-index metric improves 57 -> 42.

The remaining SpaceLounge residual is still the `pad`/`x` register-priority
tension documented in `SpaceLounge-drawLounge-wave123-w7315.md` §3
(`pad -> r9`, `x -> r5` is a weight-order impossibility for this MIR).
`int pad3` matches the original clamp instruction exactly but costs 445 raw
because `pad` stays in `fp` and `x` in `r9`, adding `mov r5,fp` copies.

## 2. Gun — the r4/r5 `i*3` swap is the k-loop sret-address hoist

Re-confirmed on this base.  The first divergence is `add.w r4, fp, fp, lsl #1`
(original) vs `add.w r5, ...` (ours) at `0x153198`; the scatter k-loop body
recomputes `add r4, sp, #104` (the `Player::getPosition` sret temp) each
iteration in the original (`0x153444`), while HEAD hoists it into both
predecessors of the scatter block (`0x153198`, `0x153260`).  Because `r4` is
then live across the k-loop, the `i*3` index is forced into `r5`, which
rotates the `sp+44`/`sp+104` StackSlotColoring pair and shifts every branch
target by 2 bytes.

New probes this wave (all reverted; all 1984 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD | **463** | **57** |
| `__builtin_expect(distance < magnitude, 1)` | 1810 | 257 (1908 B) |
| `__builtin_expect(distance >= magnitude, 0/1)` | 463/1810 | 57/257 |
| expect on `enemy->isAsteroid()` (weight 0) | 1807 | 352 (1980 B) |
| expect on `mine` branch (weight 0) | 1019 | 306 (1988 B) |
| expect on `scatter` (weight 0) | 1715 | 347 (1936 B) |
| expect on `explodeOnContact` (weight 0) | 673 | 154 |
| expect on outer `mine` (weight 0) | 1681 | 212 |
| expect on `!enemy->isActive()` (weight 0) | 680 | 125 |
| expect on `k < enemies->size()` (weight 1/0) | 1764/1677 | 355/303 |
| expect on `i < positions.count` / `e < enemies->size()` | 463 | 57 |
| `const Vector &position`/`velocity` | 1732 | 391 (1920 B) |
| `const Vector &position` only | 1734 | 326 (1912 B) |
| `const Vector &velocity` only | 1557 | 333 (1968 B) |
| `Vector enemyPosition` at function scope | 1734 | 367 (1952 B) |
| `Vector position/velocity` at function scope | 1798 | 335 (1952 B) |

No source shape keeps the 1984 B size and prevents the hoist.  The
`__builtin_expect` weights (the w9214 `!prof` lever) change the block layout
and shrink the body, so they cannot compose.

## 3. Generator — the faithful bonus form is 2000 B because `agent` spills

Re-confirmed.  The original bonus block is the faithful operand form:

```
509 adds r5, r1, r6       ; bonus = rem + rawBonus
510 str r1, [sp, #32]     ; spill rem
511 movs r1, #50
512 mov r0, r5
513 blx  __aeabi_idivmod
514 ldr r0, [sp, #32]     ; reload rem
515 cmp  r1, #0
516 it   ne
517 subne r5, r6, r0      ; bonus = rawBonus - rem
518 str r5, [sp, #16]
```

HEAD uses the algebraic `bonus -= rem * 2` (445 raw / 1996 B).  The faithful
`bonus = rawBonus - rem` is 304 raw but **2000 B**: the allocator keeps `rem`
in `r9` across the second `idivmod` and spills `agent` to a new stack slot
(frame 96 -> 104).  The original keeps `agent` in `r9` and spills `rem` into
the dead `[sp,#32]` slot (later reused for the `Mission *`), so its frame is
96.  The extra instruction is `mov r9, r1` (rem) plus the 4-byte
`subne.w r5, r6, r9` vs the original's 2-byte `subne r5, r6, r0`.

New probes this wave (all 2000 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD algebraic | **445** | **59** (1996 B) |
| faithful `bonus = rawBonus - rem` | **304** | **112** |
| `const int rem` / `int rem; rem = ...` | 304 | 112 |
| `long` / `long long` / `short` rem | 304 | 112 |
| truthy `if (bonus % 50)` / extra parens | 304 | 112 |
| `int b = rawBonus - rem; bonus = b;` | 304 | 112 |
| `__builtin_expect(bonus % 50 != 0, 1/0)` | 304 | 112/113 |
| `int *remp = &rem; ... rawBonus - *remp` | 304 | 112 |
| `volatile int rem` | 702 | 91 |
| `*(volatile int *)&rem` read | 706 | 88 |
| `bonus -= rem; bonus -= rem; bonus += rem` | 596 | 61 (1992 B) |
| `if (agent == 0) bonus = 0;` after the block | 573 | 55 (2004 B) |

The faithful form cannot be landed at 1996 B: a +4 B size shifts every
following function and `check` drops byte_exact 4507 -> 4412.  The remaining
lever is to make `rem` (not `agent`) spill; every spelling tested still keeps
`rem` in `r9`.

## 4. MenuTouchWindow — the 26 B cluster is the `Layout*` live range

Re-confirmed on this base: HEAD = **28 raw / 16 aligned / 1896 B**.  The
original's case-15 block uses `Globals::layout` directly for all four
accesses (A `buttonInsetX`, B `leftMargin`, C `top`, D `buttonInsetX`),
reloading the global after the first `GetImage2DWidth` call.  HEAD's inner
`Layout &layout = *Globals::layout;` keeps the value in `r6` across the call,
so B (`leftMargin`) comes from the cached `r6` instead of the fresh load.
The other clusters are pure RA: `ip` vs `r4` for `this->listEntryWidth`
(0x154) in case 8, and `r5` vs `r4` for `buttonInsetX` in the third if.

New probes this wave (all reverted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **28** | **16** | 1896 |
| A direct, B cached | 28 | 16 | 1896 (CSE'd with the reference) |
| A cached, B direct | 358 | 76 | 1892 |
| all four direct (inner local removed) | 358 | 76 | 1892 |
| direct + tail uses local `layout->` | 1723 | 443 | 1896 |
| HEAD + tail local `layout->` | 1736 | 452 | 1900 |
| direct + `PaintCanvas *canvas = Globals::Canvas` | 312 | 40 | 1892 |
| HEAD + `PaintCanvas *canvas` local | 49 | 69 | 1896 |
| `PaintCanvas **canvasAddr = &Globals::Canvas` | 358 | 76 | 1892 |
| `Layout **layoutAddr = &Globals::layout` | 358 | 76 | 1892 |

Every shape that makes B direct loses 4 B: with `r6` (the cached layout
value) no longer live across the call, the allocator moves
`&Globals::Canvas` from `fp` (4-byte `ldr.w`) to `r5` (2-byte `ldr`), and
`&Globals::layout` takes `fp`.  The original keeps `fp = &Globals::Canvas`
and `[sp,#16] = &Globals::layout`, exactly like HEAD.  The 4-byte shrink is
therefore the direct-B shape's own RA consequence, not a source-size change
we can absorb.

## 5. ModStation — literal-pool order + pre-RA copy, unchanged

Re-confirmed 43 raw / 12 aligned / 1008 B.  The two clusters are exactly the
wave-146 w9216 findings:

1. literal-pool order: original `Pitch, YawIPad, Yaw` (`0x128c16`,
   `0x128c38`), ours `Pitch, Yaw, YawIPad` (`0x128c3a`, `0x128c0c`);
2. the pre-RA copy `str.w r8,[sp,#184]` / `mov r4,r2` is hoisted above the
   yaw load (`ldr.w r3,[r0,r8,lsl#2]`) in ours, after it in the original.

New probes this wave (all 1008 B, all 43 raw / 12 aligned):

| variant | raw |
|---|---:|
| HEAD (`far = pitch`) | **43** |
| `__builtin_expect(Globals::iPad, 0/1)` in the yaw select | 43 |
| `!Globals::iPad ? Yaw : YawIPad` | 43 |
| `if (iPad) yawTable = YawIPad; else yawTable = Yaw;` | 43 |
| `float farYaw = (...)[race];` before the far call | 43 |
| `const float farYaw` / `const float *farYawTable` | 43 |
| `float farYaw = *(const volatile float *)&(...)[race]` | 337 |

The literal order is the backend's `select` materialisation order and does
not follow the ternary operand order; the pre-RA copy is not the machine
scheduler (`-enable-misched=false` / `-enable-post-misched=false` leave it
unchanged, per w9216).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9316
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9316/fdiff.py _ZN11SpaceLounge10drawLoungeEv   # 176
python3 /dev/shm/w9316/align_diff.py _ZN11SpaceLounge10drawLoungeEv  # 42
```

Scratch (not committed), `/dev/shm/w9316/`: `fdiff.py`, `align_diff.py`
(OURS pointed at this worktree), `sd.py` (side-by-side instruction diff),
`dumpfn.py`, `words.py`, `m.sh`, `*_sweep*.py`, `*.cpp.base`.

## 7. Files changed

* `src/game/menu/SpaceLounge.cpp` — right-edge clamp `width + pad3 * 2`
  (short sign-extend) -> `width + pad + pad`; `short pad3` removed.  Raw
  175 -> 176, aligned 57 -> 42, size 1948 unchanged.  **METRIC-ONLY** on the
  instruction-order axis (see §1).
* `_work/reconstructions/small-tail-wave147-w9316.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.  `.rel.plt` 4 differing bytes (unchanged).
