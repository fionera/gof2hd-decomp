# Radar::draw pass (wave125, w7511) — first-literal-pool anchor search, no source win

**Status:** research note, 2026-11-06. Worktree `wt/w7511`, based on current
`master` (`02513de33`, wave-124 final). **No source change landed**; this
commit contains this note only. The target remains the pristine baseline
`src/game/weapons/Radar.cpp`.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.268817 %** (official `verify.py` display 75.3), `M = 342` positional
matches. Gate at start and end: `RATCHET PASS`, byte **4506**, linked
**4506**, data 843/843, imports 207, lint CLEAN, drift 0.

Section state of the pristine baseline (this worktree, measured this wave):

| section | size | base | differing bytes |
|---|---:|---|---:|
| `.text` | 1,259,464 | `0x69440` | **85,975** |
| `.rodata` | 269,159 | `0x1c9e10` | 0 |
| `.data` | 10,604 | `0x214000` | 0 |
| `.ARM.exidx` | 28,528 | `0x19cc08` | **0** (byte-identical) |
| `.ARM.extab` | 156,312 | `0x1a3b78` | 1,474 (size/base exact) |
| `.rel.plt` | 29,392 | `0x5751c` | **0** (byte-identical) |
| `.got` | 17,164 | `0x20fcf4` | 0 |

Read with `radar-draw-wave119-w6967.md`, `radar-draw-wave121-w7105.md`,
`radar-draw-wave122-w7209.md`, `radar-draw-wave123-w7307.md` and
`radar-draw-wave124-w7413.md`. This wave re-derives the first-island
placement on the *current* (wave-124) body and searches for a size-neutral
anchor that lands the pool at the original `0x12e340`.

## 1. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w7511
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
cmake --build cmake-build-match --target diff -j6         # SYMBOL PARITY 5370/5370
```

Per-variant loop (≈1 s each): edit `Radar.cpp`, `cmake --build
cmake-build-match --target link_so -j6`, then score the single symbol with a
scratch `difflib` harness that mirrors `tools/verify/verify.py`'s
`disassemble`/`reconcile` (identical 75.268817 % on the baseline) and read
the first `ldr [pc,#…]` island address from the linked `.so`.

## 2. Baseline first-island placement (re-measured)

The first constant island is placed by `ARMConstantIslands::findAvailableWater`
after the **last water block in `[0x12e068, 0x12e464]`** (the first CP user is
`__stack_chk_guard` at `0x12e064`, Thumb-1 imm8 range 1020).

Baseline water blocks in that window (objdump, current `.so`):

```
0x12e1a6  b.n 0x12e1bc
0x12e2de  b.n 0x12e2fc     ; route onScreen arm
0x12e30c  b.n 0x12e338     ; otherLock=1 block tail
(next: 0x12e49a is outside the window)
```

So the pool lands at `0x12e310` (function-relative `0x2c0`).

Original water blocks in the same window:

```
0x12e1a6  b.n 0x12e1bc
0x12e2de  b.n 0x12e2fc
0x12e33c  b.n 0x12e376     ; 109/110 skip1 tail
(next: 0x12e4a6)
```

So the original pool lands at `0x12e340` (function-relative `0x2f0`).

The original's 109/110 skip is a **separate block placed between the 110
check and the 111 check**, and it branches over the 111 check to the merge
point:

```
12e328: beq.n  12e338      ; 109 -> skip1
12e336: bne.n  12e354      ; !=110 -> check 111
12e338: movs   r1, #0      ; skip1: landmarkHit = 0
12e33a: mov    r5, r6      ; status copy
12e33c: b.n    12e376      ; WATER
12e33e: nop
12e340: <pool>
12e354: ...111 check...
12e372: movs   r1, #0      ; skip2 (landmarks==null / 111&&campaign>93)
12e376: merge
```

The baseline instead has a **single** fall-through skip after all checks
(`12e376: movs r1,#0`), and its `otherLock` chain is inverted
(`cbz r0, 12e324; mov.w r9,#1; ldr r5,[sp,#156]; b.n 12e338`), so the
`b.n` at `0x12e30c` wins the water scan.

## 3. What must change to move the pool

Two independent conditions:

1. **No water block at `0x12e30c`.** The `otherLock` chain must be the
   original's straight `cbnz/cbnz/cmp+beq` form (cold `otherLock =
   field_0x14 != 0` out of line), not the inverted `cbz` + `b.n` form.
2. **A water block at ≈`0x12e33c`.** The 109/110 skip must be a separate
   block that branches to the merge point, and it must be placed *before*
   the 111 check.

Either alone is not enough: a straight chain with a single fall-through skip
puts the pool at the route water (`0x12e2e0`), and a separate skip with the
inverted chain puts it at `0x12e364`–`0x12e378`.

## 4. Variants measured this wave (pristine wave-124 body)

`pool` is the absolute first-island address. All rows are single edits on the
pristine baseline unless noted.

| variant | pool | size | raw % |
|---|---|---:|---:|
| **baseline** | `0x12e310` | 9908 | **75.268817** |
| guard `or` (`!(109\|\|110) && (111!= \|\| <=93)`) | `0x12e398` | 9912 | 65.118945 |
| guard `and1` (`109!= && 110!= && !(111&&>93)`) | `0x12e310` | 9908 | 75.268817 |
| guard `nestedneg` (`!(109\|\|110) && !(111&&>93)`) | `0x12e310` | 9908 | 75.268817 |
| nested `if (landmarks) { if (!(...)) }` | `0x12e310` | 9908 | 75.268817 |
| `if/else-if` with `landmarkHit=false` arms | `0x12e310` | 9908 | 75.268817 |
| `goto skip` / `do { … break; } while (0)` | `0x12e310` | 9908 | 75.268817 |
| `Status *status` local (straight chain) | `0x12e2e0` | 9900 | 73.924269 |
| `Status *status` + `or` guard | `0x12e390` | 9904 | 65.242329 |
| `bool skip = A==109 \|\| A==110` nested | `0x12e374` | 9880 | 71.354765 |
| `bool skip` + `expect(109,0)` | `0x12e364` | 9884 | 71.775004 |
| `bool skip` + `expect(109,1) + expect(110,1)` | `0x12e33c` | 9908 | 56.897293 |
| `Status *status` + `bool skip` + `expect(109,1)+expect(110,1)` | **`0x12e340`** | 9884 | 57.462366 |
| `Status *status` + `bool skip` + `expect(109,1)+expect(110,1)+expect(111,0)+expect(camp,0)` | **`0x12e340`** | 9884 | 57.462366 |

Guard-expect grids (81–243 combinations each) on the plain guard, the
`or` guard, the `and` guards and the `bool skip` form found **no** shape
with pool `0x12e340`, size 9908 and a score above the baseline. The only
`0x12e340` rows are the `bool skip` + `expect(109/110,1)` family, and every
one of them re-rolls the whole function (see §5).

## 5. Why the `0x12e340` shape is not a win

`Status *status` + `bool skip` + `expect(109,1)+expect(110,1)` reproduces the
original's water block and lands the pool exactly at `0x12e340`:

```
12e32c: cmp   r0, #109
12e32e: bne.w 0x1300b6       ; !=109 -> far check
12e332: movs  r5, #0         ; skip1
12e334: ldr   r1, [sp, #156]
12e336: ldr   r0, [sp, #128]
12e338: and.w r5, r5, #1
12e33c: ldr   r0, [r0]
12e33e: b.n   0x12e358       ; WATER
12e340: <pool>
```

But the `bool skip` live value plus the two `expect` weights re-roll the
*whole* function's register allocation and block layout:

* `vpush {d8-d11}` (baseline) vs `{d8-d13}` (original) is unchanged, but the
  frame grows `sub sp,#304` vs 296, and `fp`→`sl` for the campaign value.
* The body shrinks 9908 → **9884 B**, which shifts every downstream
  function and breaks `.ARM.exidx` byte-identity (1,119 differing bytes)
  unless the size is padded back to 9908.
* The `difflib` score drops 75.268817 → **57.462366 %** (`M` 342 → 30), and
  the `.text` differing bytes rise 85,975 → 394,898.

The closest **size-9908** shape is `Status *status` + `bool skip` +
`expect(109,1)+expect(110,1)` with the guard using `Globals::status`
directly: pool `0x12e33c` (4 B short of `0x12e340`), size 9908, raw
**56.897293 %**, `.text` differing bytes **87,499** (worse than 85,975),
`.ARM.extab` differing bytes **1,524** (worse than 1,474), `.ARM.exidx`
still 0. It is a strict regression on every metric except the pool offset.

## 6. Remaining root cause

The first island is a pure block-layout fixed point:

1. It is placed after the last water block within 1020 B of `0x12e064`.
2. The baseline's last such water block is the inverted `otherLock` chain
   tail at `0x12e30c`; the original's is the 109/110 skip1 tail at
   `0x12e33c`.
3. Getting the skip1 tail requires a straight `otherLock` chain **and** a
   separately laid-out 109/110 skip before the 111 check. The only source
   shape found that does both is a `bool skip` live across the two
   `getIndex()` calls plus `__builtin_expect(…, 1)` on both 109 and 110;
   that extra live bool changes the register pressure enough to re-roll
   the whole function.
4. A size-neutral companion anchor that keeps the pool at `0x12e340`
   without that re-roll was not found. The wave-124 body is the best
   `9908 B` shape; the pool stays at `0x12e310`.

## 7. Artifacts

Scratch (not committed, `/dev/shm/w7511`): `Radar.base.cpp` (pristine
source), `score.py`, `sectdiff.py`, `water.py`, `dump.py`, `h.py`,
`diffops.py`, and the variant drivers `v0.py`–`v26.py`, `s1.py`–`s7.py`.
The analysis is self-contained in this note.
