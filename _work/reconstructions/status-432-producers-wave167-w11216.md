# Status::missionCompleted — three more source-producer classes for the 4/3/2 weights (wave 167, w11216)

Status: **analysis only, no `src/` win landed**. Worktree `wt/w11216`,
base master `d00057e6d` (wave 166). Scratch `/dev/shm/w11216`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit: **RATCHET PASS** (baseline rebuild), `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.  Baseline target: **raw 722 / aligned 320 / 1432 B**
(reproduced after every experiment).

Read first: `status-432-switchweights-wave166-w11116.md`,
`build-provenance-round6-branch-weights.md`,
`status-missioncompleted-wave121-w7113.md`.

## 0. Bottom line

Three further producer classes were implemented and measured.  **None is
size-exact (1432 B) and a raw win.**  The new facts:

1. **Duplicated case labels in the jump-table gap slots do move the bodies
   into the original group order**, but the extra labels change the IR switch
   (more cases), which changes instruction selection / register allocation
   and the function size.  The exact group order is reached (G1, G2, G3,
   default) at 13-14 extra labels, but the function shrinks to 1372 B and the
   prologue register allocation changes (`this` moves from r6 to fp), so raw
   is 1287-1288 (worse than 722).  With <= 5 extra labels the size stays 1432
   and the RA is untouched, but the layout is only partially reordered and
   raw is 946-1042.
2. **`switch` over a `(type, flag)` pair** (`t + (atStation ? 40 : 0)` with
   every body duplicated for both flag values) doubles the jump table:
   **raw 1447 / aligned 436 / 1560 B**.  Not size-exact.
3. **A hand-written if-chain over the precomputed rank** (the wave-117
   `kMissionCase` ranks, 29 equality tests, then `switch (rank)`) is
   **raw 1455 / aligned 411 / 1548 B**.  Not size-exact; the chain is not
   folded back into a switch because the rank values are non-monotonic.
4. The known **rank-table dispatch** (wave 117, fixed-address table in the
   `.rodata` gap) is still the only source-level shape that reproduces the
   original body order at 1432 B (92.45 %).  A local `static const` table in
   `Status.cpp` is not equivalent: **raw 1198 / aligned 180 / 1440 B**
   (PC-relative table load, +8 B).

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.  All shapes keep the
29 case bodies verbatim; only the dispatch wrapper changes.

| shape | raw | aligned | size | RA | body order |
|---|---:|---:|---:|---|---|
| **baseline `switch (m->getType())`** | **722** | **320** | **1432** | r6 | ascending after 167/174 |
| +1 gap label (`case 159` -> body 150) | 946 | 379 | 1432 | r6 | 150 first, then ascending |
| +2 gap labels | 951 | 387 | 1432 | r6 | |
| +4 gap labels | 1013 | 425 | 1432 | r6 | |
| +5 gap labels | 1042 | 421 | 1432 | r6 | |
| +7 gap labels (G2 bodies 150..158 weight 2) | 1066 | 468 | 1432 | r6 | |
| +13 gap labels (G1 weight 3, all G2 weight 2) | 1287 | 370 | 1372 | fp | **G1, G2, G3, default** |
| +14 gap labels (G1 weight 4, all G2 weight 2) | 1288 | 370 | 1372 | fp | **G1, G2, G3, default** |
| `switch (t + (atStation ? 40 : 0))`, bodies duplicated | 1447 | 436 | 1560 | - | |
| if-chain over rank (29 eq tests) + `switch (rank)` | 1455 | 411 | 1548 | - | |
| local `static const` rank table + `switch` | 1198 | 180 | 1440 | - | source order |
| (reference) IR `!prof` injection 4/3/2 | 558 | 47 | 1432 | r6 | source order |

## 2. Why the gap-label producer cannot land

The 150..189 table has 16 default slots (159, 161, 173, 175-183, 185-188).
Adding `case <gap>:` to an existing body raises that body's successor weight
without changing the table size.  The weights needed are G1 > G2 > G3
(4/3/2 or the milder plateau cells).  G1 already has weight 2 (167+174), so
G1 needs >= 3 and every G2 body needs >= 2: 1 + 12 = 13 extra labels.  At
that point the IR switch has 42 cases instead of 29, and:

* the SelectionDAG block order changes, so `Greedy` renames the virtual
  registers and the prologue becomes `mov fp, r0` / `mov.w sl, #0` instead
  of the original `mov r6, r0` / `mov.w fp, #0`;
* branch relaxation shortens ~30 branches, so the function is 1372 B, not
  1432 B.

The layout is then exactly the original group order, but the RA/size delta
costs more raw bytes (1287) than the layout saves.  With <= 5 labels the RA
and size are untouched, but the layout is only partially reordered (raw
946-1042).  The `!prof` injection keeps the IR switch unchanged (same cases,
same RA, same 1432 B) and only reweights the edges, which is why it reaches
558.

## 3. Why `(type, flag)` and the if-chain fail

* `(type, flag)` duplicates every body for the two flag values, so the jump
  table doubles (80 entries) and the function grows by 128 B.  The body
  weights are uniform (each body gets 2 labels), so the layout is still
  ascending.
* The 29-test rank if-chain is not folded by `SimplifyCFG` because the rank
  values are non-monotonic; it stays as 29 compares plus a 24-entry rank
  switch, adding 116 B.  The rank table's 192-byte `.rodata` load is cheaper
  than the equivalent if-chain.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11216
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
```

Scratch (`/dev/shm/w11216/`): `Status.base.cpp` (baseline source),
`cc.sh` (single-TU compile), `measure.sh` (rebuild + fn_diff),
`gen.py` (gap-label generator), `genflag.py` (`(type,flag)` generator),
`genif.py` (rank if-chain generator), `genrank.py` (local rank table
generator), `sweep.py` (gap-label sweep), `orig.dis` / `ours.dis`,
`mir.txt` (MIR dump).

## 5. Files changed

* `_work/reconstructions/status-432-producers-wave167-w11216.md` -- this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
