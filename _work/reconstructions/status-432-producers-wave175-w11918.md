# Status::missionCompleted — grouped-weight source producers, wave 175 (w11918)

Status: **small aligned win landed** (raw unchanged 721, aligned 333 -> 329,
size 1,432 B).  Worktree `wt/w11918`, branch `wave175-w11918`, base master
`5f599a137` (wave 174).  Scratch `/home/debian/GalaxyOnFire2/scratch/w11918`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN6Status16missionCompletedEbbx` (1432 B).  Entry state (wave-174
nested-switch shape): **raw 721 / aligned 333 / 1432 B**, insn 529/527.
Gate on entry: byte_exact 4507, linked_exact 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

Read first: `status-432-producers-wave173-w11819.md`,
`status-432-producers-wave172-w11717.md`,
`status-missioncompleted-two-switch-order-w4103.md`.

## 0. Bottom line

1. **The grouped-weight producer is still not found.**  The exact original
   body order (`167/174 8 11 13 14 150..155 158 162 166 170 184 189
   156..172 default`) is reachable at 1,432 B only by injecting `!prof`
   weights; no source-only shape tried this wave is both 1,432 B and a raw
   win.
2. **The 4-level `__builtin_expect` nest produces the target order** (A then
   B) but is **1,364 B**, not 1,432 B, and the `default` block is placed
   second.  With the default `LikelyBranchWeight=2000` the fold products
   overflow (`i32 0, i32 -1521551046, ...`) and the order scrambles; with
   `-mllvm -likely-branch-weight=3` the weights are clean
   (`1, 3960, 1320, 495, 165x4, 33, 11x11, 1x10`) and the order is
   `167/174, default, 150..155,158,162,166,170,184,189, 156..172`.
3. **The landed win is a case-order move inside the nested switch**:
   `case 158` moves from last to after `case 162`.  Raw stays **721**,
   aligned drops **333 -> 329**, size stays **1,432 B**, and the whole
   `.text` differing-byte count is unchanged at **80,439**.  (Moving 158 to
   the original body position after `case 155` instead gives 722/320; the
   after-162 position is the only raw-neutral/least-aligned placement.)
4. **Two sequential switches still cost +72 B.**  Re-measured on the current
   tree: 1,504 B / raw 1,229, so the exact body order alone is not enough.
   Multi-switch partitions are 1,464-1,504 B.

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.  All source shapes
keep the 29 case bodies verbatim; only the dispatch wrapper / case order
changes.

| shape | raw | aligned | size | body order |
|---|---:|---:|---:|---|
| **base (wave-174 nested switch, 158 last)** | **721** | **333** | **1432** | ascending |
| **landed: 158 after 162** | **721** | **329** | **1432** | ascending |
| 158 after 155 (original body order) | 722 | 320 | 1432 | ascending |
| 158 at any of positions 7..20 | 721 | 329 | 1432 | ascending |
| single switch, cases in original order | 722 | 320 | 1432 | ascending |
| IR `!prof` `{d1,p2,low5,a4,b3}` (base case order) | 588 | 60 | 1432 | original (A then B) |
| IR `!prof` thirds `4x10/3x10/2x9/1` (wave 170) | 637 | 59 | 1432 | original except 150/151 |
| IR `!prof` grouped (wave 166/172 cell) | 558 | 47 | 1432 | original |
| 4-level expect nest, L=2000 | 1305 | 554 | 1384 | 167/174, default, ascending |
| 4-level expect nest, L=3 | 1309 | 469 | 1364 | 167/174, default, A, B |
| 4-level expect nest, L=2..12 | 1309-1329 | 434-482 | 1364-1388 | same |
| outer expect `switch(expect(t,C))`, outer=PLA inner=B | 1270-1339 | 458-682 | 1352-1452 | default, 167/174, A, B |
| outer expect, outer=PL inner=AB | 1303-1346 | 540-720 | 1352-1420 | default/167, ascending |
| two sequential switches (P+low+A / B) | 1229 | 342 | 1504 | original |
| three switches (P+low / A / B) | 1285 | 369 | 1500 | original |
| four switches (P / low / A / B) | 1285 | 369 | 1500 | original |
| two switches (P+low / A+B) | 1253 | 463 | 1464 | P,low,A+B |
| if-chain with group switches (`expect(_,0)`) | 1356 | 512 | 572 | tail-merged |
| if-chain with group switches (`expect(_,1)`) | 1343 | 499 | 720 | tail-merged |
| single-case if-chain, 2-4 arms | 1318-1399 | 738-776 | 1440-1480 | scrambled |
| `switch((short)mtype)` on base | 1095 | 365 | 1432 | ascending |
| `switch((unsigned char)mtype)` on base | 1196 | 358 | 1436 | ascending |
| loop-condition `expect(...,1)` / `expect(hasWon,0)` | 721 | 333 | 1432 | neutral |
| loop-condition `expect(...,0)` | 1349 | 596 | 1400 | moved |
| `expect(hasWon,1)` / `expect(m==0,0/1)` | 1265-1340 | 501-616 | 1364-1396 | moved |

## 2. Why the 4-level nest is not 1,432 B

The 4-level nest

```cpp
switch (__builtin_expect(mtype, 167)) {          // P
  case 167: case 174: ...
  default:
    switch (__builtin_expect(mtype, 8)) {        // low
      case 8: case 0: case 11: case 13: case 14: ...
      default:
        switch (__builtin_expect(mtype, 150)) {  // A
          case 150..189 ...
          default:
            switch (mtype) { case 156..172 ... } // B
        }
    }
}
```

folds to one switch with clean graded weights at `-mllvm
-likely-branch-weight=3` (the default 2000 overflows the 32-bit `!prof`
vector).  The resulting body order is the original A-then-B order, but the
outer `default:` block (the inner dispatch) is placed second, not last, and
the function is 1,364 B: the nested `__builtin_expect` calls change register
allocation (zero register moves from `fp` to `sl`) and shorten branches.
The original keeps `fp` as the zero register, which the plain nested shape
(wave 174) preserves.  This is why the weighted nest cannot be padded to
1,432 B without a non-natural anchor.

## 3. The landed change

`src/game/mission/Status.cpp`, inside the nested `default:` switch: move the
`case 158:` block from the end of the inner switch to after `case 162:`.
This is a pure case-order move (the bodies are unchanged).  It is the only
placement that keeps raw at 721 and lowers the instruction-index alignment
from 333 to 329; `case 158` after `case 155` (the original body position)
gives 722/320, and leaving it last gives 721/333.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11918
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11918/`: `Status_base.cpp`
(wave-174 source), `cc.sh` (exact TU compile, optional `-mllvm` flags),
`fastlink.sh` + `linkcmd.txt` (extracted gold+strip link), `gen2.py`
(single/nested switch generator), `gen4.py` (4-level expect nest),
`genchain.py` / `genchain2.py` (if-chain shapes), `gentwo.py` /
`genmulti.py` (multi-switch shapes), `decode3.py` (TBH body-order decoder),
`inject.py` (IR `!prof` injection), `textdiff.py` (section byte diff).

## 5. Files changed

* `src/game/mission/Status.cpp` — `case 158` moved after `case 162`.
* `docs/IDENTICALITY.md` — aligned total 26,185 -> 26,181, Status 329.
* `_work/reconstructions/status-432-producers-wave175-w11918.md` — this note.
