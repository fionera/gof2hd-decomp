# `Status::missionCompleted` — wave 130 (w7672): case-162 loop written the
# original's way (`continue`), and the high-TBH body order is an A-run/B-run
# *frequency* ordering, not arbitrary source order

Status: **source change landed** (case-162 `hasCargo` loop rewritten as
`if (hasCargo) continue; complete = false; break;`).  Worktree `wt/w7672`,
base master `003a9623a` (wave-129 snapshot, byte 4506 / linked 4506).
Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image
base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | before | after |
|---|---:|---:|
| function score | 71.1 % (523 vs 524) | **71.0 %** (523 vs 525) |
| function size | 1432 B | **1432 B** |
| gate byte_exact | 4506 | **4506** |
| gate linked_exact | 4506 | **4506** |
| data_exact | 843/843 | **843/843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |
| `.text` differing bytes | 85,441 | **85,434** |

## 0. Summary

Three concrete results:

1. **The case-162 loop is written the original's way with a `continue`.**
   The original body (`ac21c..ac240`) is

   ```
   ac23a adds r4,#1
   ac23c cmp  r0,#0
   ac23e bne.n ac21c      ; hasCargo -> loop
   ac240 b.n  ac45a       ; else -> latch
   ```

   The landed `if (__builtin_expect(!hasCargo, 1)) { complete = false;
   break; }` emits the opposite polarity (`beq.w latch; b.n loop`).
   Writing the body as

   ```cpp
   for (unsigned j = 0; j < ingredients->size(); j++) {
       if (ship->hasCargo((*ingredients)[j], (*quantities)[j])) {
           continue;
       }
       complete = false;
       break;
   }
   ```

   emits exactly `cmp r0,#0; bne.n loop; b.n latch` — instruction-identical
   to the original — at the same 1432 B.  It costs 0.1 fuzzy (the shifted
   instruction changes the global alignment) but removes **7 differing
   `.text` bytes**, so it is a real step toward byte-identity.  Landed.

2. **The original high-TBH body order is two ascending runs**, not an
   arbitrary "source order":

   ```
   150 151 152 153 154 155 158 162 166 170 184 189   (A run, ascending)
   156 157 160 163 164 165 168 169 171 172           (B run, ascending)
   ```

   (after the peeled `167/174`, the low group `8, 11, 13, 14`, and the
   `0/11` body).  This is exactly what `MachineBlockPlacement` produces when
   the A bodies are *hotter* than the B bodies: every body inside a run ties
   on frequency, so the worklist drains in `buildJumpTable` table order
   (ascending value); the run boundary is a frequency boundary.  The
   wave-129 "MBP fallback = source order" explanation is equivalent here
   only because the source happens to be A-then-B, but the frequency model
   is the one that also explains the ascending order *inside* each run.

3. **Two-switch forms reproduce the exact body order but cannot hit
   1432 B** (new measurements, current base):

   | source shape | size | high-table body order |
   |---|---:|---|
   | `switch(m->id){A} switch(m->id){B}` (no goto) | 1464 | **exact original** |
   | `switch(m->getType()){A} switch(m->getType()){B}` | 1464 | **exact original** |
   | cached `int mtype` + two switches | 1504 | **exact original** |
   | `switch(m->id){A; goto done;} switch(m->id){B} done:` | **1432** | value order (folded) |
   | `switch(m->id){A; goto done;} if(id==-1)goto s; s: switch(m->id){B} done:` | 1432 | value order (folded) |
   | same with a `volatile` guard | 1456 | exact original |
   | nested `default: switch` | 1432 | value order |

   The non-goto form keeps the second switch's 17-entry TBB dispatch
   (~34 B: `ldr.w r0,[m,#12]; subs r0,#156; cmp r0,#16; bhi; tbb; 17-byte
   table`) because the A bodies' `break` reaches it, so the B bodies get a
   second unplaced predecessor and are pushed only after the A run.  The
   `goto done` form lets `SimplifyCFG::FoldValueComparisonIntoPredecessors`
   fold the B cases into the first table, which removes the dispatch and
   restores 1432 B — but then the successors are added in table (value)
   order and the exact order is lost.  A `volatile` guard prevents the fold
   but costs 24 B.

## 1. Case-162 / case-163 exact loop shapes (measured)

| variant | size | case-162 body | case-163 body |
|---|---:|---|---|
| landed base (wave 126 hint) | 1432 | `beq.w latch; b loop` | `bgt.w latch; b loop` |
| case-162 `continue` (landed) | 1432 | `bne.n loop; b latch` **= orig** | `bgt.w latch; b loop` |
| case-163 `for (; j<size; j++) { if (val>=0) break; } if (j>=size) return m;` | 1432 | `beq.w latch; b loop` | `ble.n loop; b latch` **= orig** |
| both | 1428 | **= orig** | **= orig** |

The case-163 `j`-based loop is the faithful shape (the original loads
`field_90`, compares `j` to `size` at the top, and branches `ble.n` back to
the loop while the value is `-1`).  It is also 1432 B on its own, but it
only saves 1 differing `.text` byte (vs 7 for case-162), so case-162 is the
one landed.  Both together are 1428 B, which shifts every later `.text`
address and is fatal for the gate.

The `j`-based *case-162* form (`for (; j<size; j++) { if (!hasCargo)
break; } if (j>=size) return m;`) is 1436 B: the extra live `j` displaces
`this` from `r6` to `r9`, so it is not landable.

## 2. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506

# fast compile-only body-order / size probe (exact project flags):
#   /dev/shm/w7672/cc.sh <in.cpp> <out.o>
#   /dev/shm/w7672/bodies.py <out.o>     # decodes the r1 TBH(s), prints size/order
# single-function score on the linked .so:
#   python3 /dev/shm/w7672/score1.py cmake-build-match/libgof2hdaa.so
# .text differing bytes:
#   python3 /dev/shm/w7672/textdiff.py cmake-build-match/libgof2hdaa.so
```

Scratch (not committed): `/dev/shm/w7672/` — `cc.sh`, `bodies.py`,
`bodies2.py` (r0/r1/r2 TBH), `score1.py`, `textdiff.py`, `gen.py`,
`gen2.py`..`gen7.py` (switch/if-chain/two-switch generators), `base.cpp`,
`v162cont.cpp`, `v163j.cpp`, `v4_id_id.cpp`, `v5_goto_id.cpp`,
`v7_dummy_vol.cpp`.

## 3. Open question

The exact body order is reproducible at 1464 B (two switches, second TBB
dispatch survives) and at 1432 B only with the second switch folded (value
order).  A source shape that keeps the B bodies' second unplaced predecessor
*without* the extra dispatch block would give the exact order at 1432 B.
The remaining candidates are a cheaper non-foldable guard between the two
switches, or a `!prof`/PGO weight set whose tail-merging also lands at
1432 B (wave-125 found 1436/1428, wave-129 1444).
