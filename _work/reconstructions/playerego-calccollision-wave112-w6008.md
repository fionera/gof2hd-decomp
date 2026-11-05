# PlayerEgo::calcCollision — wave 112 (w6008): RA interval audit, no new best

**Status:** research note, 2026-11-05. Worktree `wt/w6008`, branch
`wave112-w6008`, base `5d98bd391`. **No source change landed** — the
checked-in 87.6436781609 % / 968 B shape is still the best measured.
This note extends the wave-107/108/109/110/111 fixed-point notes
(`playerego-calccollision-wave107-w5513.md` … `-wave111-w5911.md`).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions, 87.6436781609 % exact).

## TL;DR

* The landed two-anchor body still scores **87.6436781609 %** exactly
  (968 B, 347/349 normalised instructions).  ~180 new builds this wave
  (full 8×8 check-form cross product, check-position sweep, early-out
  wormhole form, `hit` form, loop-form variants, top-guard
  permutations, `distance`/`strength` hoists) produce nothing above it.
* **New: RAGreedy interval audit of the `ge` shape.**  The allocator's
  queue priority is `(1u<<29) + LI->getSize()` for global ranges
  (`rag.cpp:680`), and a range that is live out of the last MIR block
  is extended to the end of the function (`8096B`).  In the `ge` shape
  the preheader pointers are all "live to end" and therefore ordered
  by their def point: `moveDirection`(352r) > `moveTarget`(368r) >
  `dock`(384r) > `i`(400r) > `distance`(2704r, a *local* 1360-slot
  range).  `dock` (7712) beats `i` (7488) and both beat `distance`
  (1360), so the `ge` shape gets `dock=r9, i=sl(r10), distance=fp(r11)`
  — exactly the observed file.  The original's `distance=sl, i=fp`
  order needs `distance` to be a *global* range longer than `i`'s,
  which no source spelling tried so far produces.
* **New fixed point: `A1s+A2`.**  `if (i >= objects->size()) continue;`
  at A1 (always false, but the continue edge is the taken branch) plus
  the landed A2 reproduces the original's **un-rotated latch**
  (`0x2c8`, vs original `0x2c0`) **and** the original register file
  (`dock=r9, i=fp, this=r5, surface=r6`) — but is **976 B** (8 too
  long), so it scores 85.4676 %.  Removing A2 (`A1s` alone) restores
  968 B but flips `dock`/`i` (`dock=fp, i=r9`, 87.2832 %).
* The full 8×8 A1×A2 form cross product (`gt1`, `ge`, `le`, `eq`,
  `lt`, `ne`, `gt`, `le1`) confirms the landed pair is the unique
  87.6437 % point; every `gt1/*` pair is codegen-identical to the
  landed body, every `ge`/`le`-direction A1 is 976–980 B.

## 1. Exact scores (unrounded, this wave)

```
landed (A1+A2, guard w/o i==0)     pct=87.6436781609  size=968  lens=347/349
A1s only (i >= size, landed guard) pct=87.2832369942  size=968  lens=347/345
ge_landedguard                     pct=87.2832369942  size=968  lens=347/345
wrap (i < size wrapper)            pct=87.2832369942  size=968  lens=347/345
lele (size<=i at A1+A2, landed g.) pct=85.4676258993  size=976  lens=347/348
A1s+A2                             pct=85.4676258993  size=976  lens=347/348
A1s+A2+real guard                  pct=85.6733524355  size=984  lens=347/351
landed + real guard                pct=84.9785407725  size=976  lens=347/352
ge_guard (ge + real guard)         pct=83.4782608696  size=968  lens=347/343
natural (no checks, real guard)    pct=59.7997138770  size=996  lens=347/352
natnobrace (no surface braces)     pct=64.0804597701  size=968  lens=347/349
```

## 2. RAGreedy interval audit (new)

`/tmp/llvm_ndk_build/bin/llc -run-pass=greedy -debug-only=regalloc` on
the MIR extracted at `-stop-before=greedy` (see §5) gives, for the
`ge` shape:

```
%0  [384r,8096B)   &dockOffsetVec      size 7712
%1  [368r,8096B)   &moveTarget         size 7728
%2  [352r,8096B)   &moveDirection      size 7744
%17 [400r,8096B)   dead i copy         size 7696
%57 [2704r,4064r)  distance (local)    size 1360
%173 [608r,624B:1)[624B,8064r:2)[8064r,8096B)  i  size 7488
```

`enqueue()` (`rag.cpp:680`) gives global ranges
`Prio = (1u<<29) + Size`, and local ranges
`Prio = beginIndex().getInstrDistance(Indexes->getLastIndex())`
(≈ 5392 for `distance`), so **every** "live to end" preheader pointer
outranks `i`, and `i` outranks `distance`.  The queue order observed
in the debug log is

```
%10 objects -> r4, %9 this -> r5, %2 moveDirection -> r6 (evicted),
%1 moveTarget -> r6 (evicted) -> r9, %0 dock -> r6 (evicted) -> r10,
%75/%173 i -> r11
```

i.e. `dock` and `i` are the two "mutually exclusive" attractors for
`r9`; whichever has the larger interval at enqueue time takes it, and
the loser lands in `r10`/`r11`.  The landed body's *anchor blocks*
change the MIR block order enough to make `dock` (7904) beat `i`
(7680), which is why it gets the original register file — at the cost
of the rotated latch.

For `distance` to take `r10` (as in the original), it would have to be
a global range with size > `i`'s 7488.  Its def is at 2704r and its
last use at 4064r; even extending it to the end of the function gives
5392 < 7488.  The only way is to make `distance` live across the loop
back edge (declare it outside the loop and use it after the loop),
which was measured: it adds a preheader `mov #0`, changes the frame
from 112 to 112+ and moves `this`/`objects` off `r5`/`r4`
(`landed distpre+use` = 67.8 %, 992 B, `this=r4 objects=r5`).

## 3. Check-form cross product (new, exhaustive)

For each pair `(A1 form, A2 form)` with the landed (no `i==0`) bottom
guard, splicing the form at the landed A1/A2 positions:

| A1 \ A2 | gt1 | ge | le | eq | lt | ne | gt | le1 |
|---|---|---|---|---|---|---|---|---|
| **gt1** | 84.8(972) | **87.6(968)** | **87.6(968)** | **87.6(968)** | **87.6(968)** | **87.6(968)** | **87.6(968)** | **87.6(968)** |
| ge | 85.5(980) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) |
| le | 85.5(980) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) |
| eq | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) |
| lt | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) |
| gt | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) | 87.4(968) |
| ne | 85.5(980) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) |
| le1 | 85.5(980) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) | 85.5(976) |

`gt1` = `i > objects->size() - 1` (landed A1), `ge` = `i >= size`,
`le` = `size <= i`, `eq` = `size == i`, `lt` = `size < i`,
`ne` = `size != i`, `gt` = `size > i`, `le1` = `i <= size`.
The landed `gt1/gt1` and every `gt1/*` pair are codegen-identical
(same 968 B, same latch `0xc6`); the `eq`/`lt`/`gt` A1 rows are also
968 B but land the latch at `0xc4`.

## 4. Other measured dead ends this wave

| variant | pct | size | note |
|---|---:|---:|---|
| `landed + ge` (both checks) | 85.1 | 984 | right regs, latch 0x2d0 |
| `landed` ge before A1 | 85.1 | 984 | identical |
| `ge_guard` + `distance<1000` after matrix | 82.6 | 968 | i=sl |
| `ge_guard` nested top guard | 83.5 | 968 | identical |
| `ge_guard` nested bottom guard | 83.5 | 968 | identical |
| `ge_guard` `field_0x71 != 0` | 83.5 | 968 | identical |
| `ge_guard` `int i` / `size_t i` | 83.5 | 968 | identical |
| `ge_guard` unbraced surface | 51.5 | 996 | |
| `ge` at other positions (steal/visible/hitcam/loop-end) | 57–59 | 988–1008 | rotated |
| `ge@before visible + A2` | 85.9 | 976 | right regs, latch 0x2c6 |
| wormhole arm as early-out (`strength<1 {stop; continue;}`) | 87.4 | 968 | landed base |
| `int hit = outerCollide(...)` | 87.6 | 968 | identical to landed |
| `Vector surface(ctor)` / `surf direct` | 87.6 | 968 | identical |
| `type` local / `stealFlagByte != 0` / `isVisible()==false` | 87.6 | 968 | identical |
| `impact`/`collision` comparison operand swap | 87.1/87.4 | 968 | |
| top-guard permutations (`i,orbit,f71,dock` etc.) | 51.9–85.9 | 968–1020 | |
| `distance`/`strength` declared before loop | 87.6 | 968 | codegen-identical |
| `distance` pre-loop + use after loop | 67.8 | 992 | `this=r4 objects=r5` |
| loop forms (`while`, `do/while`, `++i`, cached `n`, `int i`) | 42–87.6 | 948–996 | `do/while` 77.1 |
| `dockOffsetVec.x == 12345.0f` extra use | 57.8–76.0 | 976–1012 | |

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6008
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
# per-variant: splice body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6008/harness.py /dev/shm/w6008/body_landed.txt   # 87.6 / 968 B
python3 /dev/shm/w6008/diff.py                                     # normalised diff
```

RA interval audit (single function, ~2 s):

```sh
cd cmake-build-match
CMD=$(grep -m1 -A2 "build CMakeFiles/gof2.dir/src/game/ship/PlayerEgo.cpp.obj" \
      build.ninja | grep "FLAGS" | sed 's/^  FLAGS = //')
CC=../.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
$CC $CMD -mllvm -stop-before=greedy \
    -mllvm -filter-print-funcs=_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE \
    -c ../src/game/ship/PlayerEgo.cpp -o /dev/shm/w6008/ge_stop.mir
python3 /dev/shm/w6008/mkmir.py /dev/shm/w6008/ge_stop.mir /dev/shm/w6008/ge_fn.mir
sed -i 's/ target-flags(<unknown>)//g; s/ target-flags(arm-none)//g; \
        s/%stack\.\([0-9]*\)\.<unnamed alloca>/%stack.\1/g' /dev/shm/w6008/ge_fn.mir
/tmp/llvm_ndk_build/bin/llc -run-pass=greedy -debug-only=regalloc \
    /dev/shm/w6008/ge_fn.mir -o /tmp/ge_out.mir 2> /dev/shm/w6008/ge_ra.txt
```

Scratch harness (`/dev/shm/w6008`): `harness.py`, `run.py`, `diff.py`,
`dump.py`, `mkmir.py`, `mir.sh`, `bw.py` … `bw21.py`, the body files
(`body_landed.txt`, `body_ge_guard.txt`, `body_ge_landedguard.txt`,
`body_lele.txt`, `body_wrap.txt`, `body_natural.txt`,
`body_natnobrace.txt`, `body_A1sA2.txt`) and the MIR/RA dumps
(`ge_stop.mir`, `ge_fn.mir`, `ge_ra.txt`, `landed_ra.txt`).

## 6. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0,
`RATCHET PASS`.  27 sections exact, file size 2,187,328 B.  No source
change, so `tools/verify_baseline.json` is untouched.

## 7. Proven vs. hypothesis

**Proven (measured):**

* The landed body's exact score is 87.6436781609 % (347 vs 349
  normalised instructions, 968 B); the full 8×8 A1×A2 form cross
  product and ~180 other variants this wave find no better point.
* `A1s+A2` (`i >= objects->size()` + landed A2) is the only 976 B shape
  that has both the original register file and the original
  un-rotated latch (`0x2c8`); it is 8 B too long.
* Removing A2 from `A1s` restores 968 B but flips `dock`/`i`.
* In the `ge` shape `distance` is a local 1360-slot range and can never
  outrank `i` (7488) at enqueue time; this is why `i=sl, distance=fp`.

**Hypothesis (not proven):**

* The original source is the natural (REA) shape compiled with
  `&__stack_chk_guard` and `&dockOffsetVec` both spilled at their def
  and `distance` promoted to a global range — an RA state no source
  spelling tried in waves 107–112 reproduces without adding a check.
