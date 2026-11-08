# Small tail wave 149 (w9516): Gun / Generator / MenuTouchWindow / ModStation —
# new hypotheses on the four residuals (one landed MenuTouchWindow variant)

Status: **one landed `src/` change** (`MenuTouchWindow::OnTouchBegin`, metric-only).
Worktree `wt/w9516`, base `fece77a64` (wave 148). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave148-w9417.md`, `small-tail-wave147-w9316.md`,
`near-miss-trio-wave146-w9216.md`.

Gate before/after: byte_exact **4507 -> 4507**, linked_exact **4507 -> 4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
RATCHET PASS.  `.text` size unchanged (the landed change is size-neutral).

## 0. Residual classification (raw same-address bytes / aligned instructions)

| target | size | raw before | raw after | aligned before | aligned after |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` | 1984 B | 463 | 463 | 57 | 57 |
| `Generator::createMission` | 1996 B | 445 | 445 | 59 | 59 |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 | **29** | 16 | **15** |
| `ModStation::ModStation` (C1) | 1008 B | 43 | 43 | 12 | 12 |

Aligned metric = `_work/reconstructions/align_diff.py` (branch targets
normalised to instruction indices).  All four remain size-exact.

## 1. Gun — the r4/r5 `i*3` swap is the k-loop rotation, not the sret address

Re-confirmed the first divergence is `add.w r4, fp, fp, lsl #1` (original)
vs `add.w r5, ...` (ours) at function offset `0xdc`, and the k-loop
(`for (unsigned k = 0; k < this->enemies->size(); ++k)` in the `scatter`
tail) is **rotated** in the original (preheader `movs r5,#0; b.n <cond>`;
body at `0x153442`; condition at `0x1534fc`; `bcc.n 0x153442`) but
**top-tested** in ours (`movs r5,#0; ldr r0,[r9,#180]; ... cmp; bcs exit`;
body at `0x15344c`).  Because the original body recomputes
`add r4, sp, #104` (the `Player::getPosition` sret temp) inside the loop,
`r4` is live and the `i*3` index is forced into `r5`; ours hoists the
address into both predecessors and keeps `i*3` in `r5`, swapping the
`sp+44`/`sp+104` StackSlotColoring pair.

New probes this wave (all reverted; all 1984 B unless noted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **463** | **57** | 1984 |
| `Vector enemyPos = enemy->getPosition(); *td = enemyPos;` | 463 | 57 | 1984 |
| explicit do/while rotation (`if(size) do{...}while(k<size)`) | 1778 | 308 | 1952 |
| drop `__builtin_expect(distance < magnitude, 0)` | 1810 | 257 | 1908 |
| `__builtin_expect(..., 1)` | 1810 | 257 | 1908 |
| `__builtin_expect(distance >= magnitude, 0)` | 463 | 57 | 1984 |

The explicit-temp form is IR-identical.  The do/while and the expect
removal both change the loop rotation but shrink the function, so they
cannot compose.  The rotation is a BlockPlacement/LoopRotate outcome of
the cold `distance < magnitude` branch weight; no source spelling tested
keeps 1984 B *and* rotates.

## 2. Generator — `rem` still refuses to spill; every new spelling is 2000 B

Re-confirmed HEAD algebraic `bonus -= rem * 2` = **445 raw / 59 aligned /
1996 B**; faithful `bonus = rawBonus - rem` = **304 / 112 / 2000 B**
(frame 104, `mov r9,r1` + `subne.w r5,r6,r9`; `agent` spills to a new
slot).  Original spills `rem` to the dead `[sp,#32]` and keeps `agent`
in `r9` (frame 96).

New probes this wave (all reverted; all 2000 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD algebraic | **445** | **59** |
| faithful (re-confirmed) | 304 | 112 |
| function-scope `int rem;` then assign | 304 | 112 |
| `unsigned rem` / `long rem` / `(int)rem` casts | 304/306 | 112 |
| `bonus = rawBonus; bonus -= rem;` two statements | 304 | 112 |
| `bonus = rawBonus + -rem` | 304 | 112 |
| `volatile int rem` | 702 | 91 |
| `__builtin_expect(bonus % 50 != 0, 0/1)` | 304 | 112 |
| cache `agent->getImageParts()` before bonus block | 1150 | 251 (1996 B) |
| move `getImageParts()` to just after bonus block | 1142 | 165 (1996 B) |

Caching `getImageParts()` does keep the frame at 96 (1996 B) but moves
the call and re-colours the whole function (raw 1142-1150).  The
allocator still prefers `rem` (short live range, high spill weight) over
`agent` (long live range); no spelling found that spills `rem` into the
existing dead slot.

## 3. MenuTouchWindow — LANDED: A/B direct, C via the reference (METRIC-ONLY)

Re-confirmed the case-15 block.  The original reloads `Globals::layout`
from `[sp,#16]` for all four accesses; the 26 B cluster is the cached
`Layout *` live range across the first `GetImage2DWidth` call.

New probes this wave (all 1896 B unless noted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (A,B ref; C,D direct) | **28** | **16** | 1896 |
| **A,B direct; C ref; D direct (LANDED)** | **29** | **15** | 1896 |
| A direct; B,C,D ref | 287 | 74 | 1892 |
| A,B direct; C,D ref | 314 | 74 | 1888 |
| all four direct | 358 | 76 | 1892 |
| all four direct + `Layout *&` reference | 358 | 76 | 1892 |

Making B direct alone (or all four direct) re-colours `&Globals::Canvas`
from `fp` to `r5` and shrinks the function by 4 B (`ldr.w fp,[r1]` 4 B
becomes `ldr r5,[r1]` 2 B plus two `ldr.w`/`ldr` swaps), so it cannot be
landed at 1896 B.  The landed variant keeps the reference live across the
first call (C) while making A and B direct; it is size-neutral and the
instruction-index alignment improves 16 -> 15 (raw 28 -> 29).  It is
**metric-only**: the original's C is a fresh load, ours still uses the
cached `r6`.

## 4. ModStation — literal-pool order + pre-RA copy, unchanged

Re-confirmed 43 raw / 12 aligned / 1008 B.  Two artefacts:
1. literal-pool order: original `YawIPad` then `Yaw`; ours `Yaw` then
   `YawIPad` (the `select` materialisation order);
2. `str.w r8,[sp,#184]` / `mov r4,r2` hoisted above the yaw load in ours,
   after it in the original.

New probe this wave (reverted): computing `yawBits` before `pitchBits`
(and `pitch` before `yaw`) = **744 raw / 40 aligned / 1008 B**.  The
literal order and the copy placement are backend choices not exposed by
source order.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9516
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9516/fdiff.py <mangled>
python3 /dev/shm/w9516/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w9516/`: `fdiff.py`, `align_diff.py`,
`sd.py`, `m.sh`, `sweep_gen*.py`, `sweep_gun*.py`, `sweep_mtw.py`,
`base/` (pristine sources).

## 6. Files changed

* `src/game/ui/MenuTouchWindow.cpp` — case-15 `OnTouchBegin`: A
  (`buttonInsetX`) and B (`field_0xc_leftMargin`) now use
  `Globals::layout->` directly, C (`field_0x20_top`) keeps the local
  reference.  Size 1896 unchanged; raw 28 -> 29, aligned 16 -> 15.
* `_work/reconstructions/small-tail-wave149-w9516.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.
