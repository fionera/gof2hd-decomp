# Status::missionCompleted — case-158 locals and switch-shape sweep, wave 176 (w12016)

Status: **small aligned win landed** (raw unchanged 721, aligned 329 -> 327,
size 1,432 B).  Worktree `wt/w12016`, branch `wave176-w12016`, base master
`b21088985` (wave 175).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12016`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN6Status16missionCompletedEbbx` (1,432 B).  Entry state (wave-175
landed shape, `case 158` after `case 162`): **raw 721 / aligned 329 /
1,432 B**, insn 529/527.  Gate on entry: byte_exact 4507, linked_exact 4507,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.

Read first: `status-432-producers-wave175-w11918.md`,
`status-432-producers-wave173-w11819.md`,
`status-432-producers-wave172-w11717.md`,
`status-missioncompleted-two-switch-order-w4103.md`.

## 0. Bottom line

1. **No source-only raw win was found.**  Every case-order permutation,
   `__builtin_expect` cell, and switch-lowering variant tried this wave is
   either raw >= 721 or not 1,432 B.  The original's hot-then-cold block
   order is weight-driven; the plain nested shape (which preserves the
   original `fp` zero register) only reaches it through IR `!prof`
   injection, and every source-level weight hint that reorders the blocks
   also changes register allocation / branch widths and the size.
2. **The landed win is a case-158 local declaration order.**  Inside the
   `case 158` body, declare `hasSpecial` before the loop index `j`:
   ```cpp
   bool hasSpecial = false;
   unsigned j = 0;
   bool hasGood = false;
   ```
   Raw stays **721**, aligned drops **329 -> 327**, size stays **1,432 B**,
   `.text` differing bytes stay **80,166**, `.rel.plt` stays
   byte-identical.  (Wave-175 w11918 landed an aligned-only win the same
   way: raw unchanged, aligned 333 -> 329.)
3. **Correct TBH decode.**  The Thumb-2 `TBH` target is
   `PC + 2 * halfword[PC + 2*Rm]` (the offset is doubled), not
   `PC + halfword`.  With the doubled offset the original table decodes to
   the original hot-then-cold body order; the earlier non-doubled decode in
   some scratch scripts is wrong.

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.  All shapes keep the
29 case bodies verbatim; only the dispatch wrapper / case order / local
declaration order changes.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base (wave-175 landed, `j; hasSpecial; hasGood`) | 721 | 329 | 1432 |
| **landed: `hasSpecial; j; hasGood`** | **721** | **327** | **1432** |
| `hasGood; hasSpecial; j` | 722 | 320 | 1432 |
| `hasSpecial; hasGood; j` | 722 | 323 | 1432 |
| `hasGood; j; hasSpecial` | 722 | 322 | 1432 |
| `j; hasGood; hasSpecial` | 721 | 329 | 1432 |
| `hasSpecial; j; hasGood` + remove 157 expect | 722 | 326 | 1432 |
| remove 157 expect only | 722 | 328 | 1432 |
| remove 163 expect only | 859 | 345 | 1428 |
| remove 157+163 expects | 860 | 348 | 1428 |
| remove 166 expect | 734 | 328 | 1432 |
| remove 184 expect | 887 | 358 | 1428 |
| add expect to case 150 / 151 / 155 condition | 743 | 336 | 1432 |
| add expect to case 158 `hasGood && hasSpecial` | 721 | 329 | 1432 |
| add expect to case 162 condition | 1309 | 501 | 1364 |
| 157 expect value 0 | 1182 | 428 | 1464 |
| flat single switch (all 27 cases, base order) | 721 | 329 | 1432 |
| flat single switch, hot-then-cold source order | 722 | 320 | 1432 |
| nested A/B (hot outer, cold inner default) | 722 | 320 | 1432 |
| two sequential switches | 1229 | 342 | 1504 |
| outer `switch(expect(mtype,C))` cells (7) | 1314-1346 | 540-720 | 1352-1420 |
| inner `switch(expect(mtype,C))` cells (27) | 1312-1357 | 581-685 | 1348-1424 |
| both outer+inner expect cells (102) | 1292-1357 | 520-733 | 1352-1432 |
| only both=14/189 is 1,432 B | 1301 | 729 | 1432 |

Every single-case move (462 variants), every pair swap (231), every outer
group permutation (120), and every low-group partition (32) was also swept
on the base and on the `hasSpecial`-first local order; the best raw is
always 721, and the best aligned among raw-721 shapes is 327.

## 2. The original block order (correct TBH decode)

The dispatch is one `TBH` for `mtype-150` in 0..39 plus a second `TBH` for
`mtype-8` in 0..6.  Decoding the first table with the doubled offset gives
the original body order:

```
167/174, 150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172, default
```

i.e. a **hot group** `{150..155,158,162,166,170,184,189}` followed by a
**cold group** `{156,157,160,163,164,165,168,169,171,172}`.  Our build
always lays the jump-table cases out in ascending value order
(`150,151,...,189`), regardless of source case order: a flat single switch
with the exact hot-then-cold source order still compiles ascending
(raw 722 / aligned 320).  The order is chosen by `MachineBlockPlacement`
from the switch edge weights, not from source order.

The `!prof` weight vector that reproduces the original order (from the
wave-175 analysis) is `{default 1, 167/174 2, low 5, hot 4, cold 3}`; with
the plain nested shape it yields raw 588 / aligned 60 at 1,432 B, but it is
only reachable by IR metadata injection, not from C++.

## 3. Why the source-level weights miss

`switch (__builtin_expect(x, C))` does attach `!prof
!{!"branch_weights", ...}` to the switch, but it boosts **one** case to
2000 and leaves every other case at 1.  That is enough to move that case
to the front, but it also changes the jump-table lowering / branch widths:
the 7 outer cells are 1,352-1,420 B and the 27 inner cells 1,348-1,424 B.
The only outer+inner pair that lands on 1,432 B (`14`,`189`) is raw 1301.
Nesting `__builtin_expect` calls only overrides the inner weight (the outer
value wins), so a graded hot/cold split cannot be expressed this way.

The 4-level expect nest does produce the hot-then-cold order, but it moves
the zero register from `fp` to `sl` and shortens branches, giving 1,364 B
(wave-175 note).  The original keeps `fp` as the zero register, which the
plain nested shape preserves.

## 4. The 157 loop shape (side finding)

The original `case 157` loop is bottom-tested:
```
b.n check ; body ; j++ ; check: cmp j,size ; bcc body ; b.n break
```
Our source's `__builtin_expect(..., 1)` on the loop condition makes it
top-tested (`cmp j,size ; bcs break ; body ; ... ; b.n check`).  Removing
that expect restores the original bottom-tested shape and the loop
instruction stream matches the original byte-for-byte at its own address,
but the block is still placed at the ascending position, so the same-address
raw metric is 722 (one worse than 721) and the win is not landed.  The
expect is also load-bearing for the rest of the function (removing it alone
is 722/328; removing 163's expect is 859/1,428).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12016
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
# -> raw=721 aligned=327 size=1432/1432 insn=529/527
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target diff -j2
cmake --build cmake-build-match --target drift -j2
```

Section check (objdump `-h`: size = field 2, file offset = field 5):
`.text` 80,166 differing bytes (size/base exact); `.ARM.exidx` 0;
`.ARM.extab` 746 (size/base exact); `.rodata` 0; `.data` 0; `.bss` 0;
**`.rel.plt` 0 (byte-identical, 29,392 B)**; file size exact 2,187,328 B.

## 6. Files changed

* `src/game/mission/Status.cpp` — `case 158` locals: `hasSpecial` before
  `unsigned j`.
* `docs/IDENTICALITY.md` — wave-176 authoritative section; aligned total
  26,502 -> 26,500, Status 329 -> 327.
* `_work/reconstructions/status-432-producers-wave176-w12016.md` — this note.

Scratch `/home/debian/GalaxyOnFire2/scratch/w12016/`: `Status_base.cpp`
(wave-175 source), `Status_specialj.cpp` (landed), `Status_flat.cpp` /
`Status_flat_ord.cpp` (single-switch shapes), `flatten.py`,
`reorder_flat.py`, `tbh2.py` (correct doubled-offset TBH decode),
`worddiff.py`, `rm157_163.py`, `rm157_specialj.py`,
`sweep_bodyexpect_val.py`, `sweep_hotexpect.py`, `sweep_expects_remove.py`,
and the wave-175 sweep scripts/logs carried over.
