# MGame::OnTouchEnd — wave 139 (w8414): the wingman-body -> flags-test join is real but does not move the join test

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w8414`,
base `bf123bd47`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w8414`. **No source change landed** (base restored; full gate green).
This wave tested the wave-138 open question: does making the wingman body fall
through to the `flags == 0` test (the "join" the original has at `0x17bf4a: b.w
0x17afe2`) move the join test / flip the BranchFolder survivor?

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

Baseline (re-built this wave):

```
raw same-address diff 4363 / 8080   size 8080/8080
survivor (movs r1,#N before blx initHudMenu): 1  (0x200 body owns the shared tail)
join test (ldr [fp,#248]; cmp #0): 0x17aeb4
```

## 1. Ground truth re-confirmed (original)

```
17adc0: ldrd r0,r2,[fp,#116] ; movs r1,#1 ; b.n 17afde   ; body 0x200 -> shared tail
17afd8: ldrd r0,r2,[fp,#116] ; movs r1,#2                ; body 0x400
17afde: blx Hud::initHudMenu                              ; shared tail (0x400 falls through)
17afe2: ldr.w r0,[fp,#248] ; cbz r0, 17b01a               ; join test: flags == 0
17afe8: ... epilogue (isAutoPilot) ...
17b01a: flags body
...
17bf4a: b.w 17afe2                                       ; wingman body -> join test
```

So the original **does** have the wingman body as a predecessor of the flags test
(`b.w 17afe2`), and the join test is laid out immediately after the shared tail
(owned by body 0x400). The base has the wingman body branching to the epilogue
(`b.n 17b976`) instead.

## 2. The exact LLVM 7 tie-break (BranchFolding.cpp, release/7.x)

Downloaded the real source (`/dev/shm/w8414/BF.cpp`) this wave. The relevant facts:

* `MergePotentialsElt::operator<` sorts by **hash, then by `getBlock()->getNumber()`**
  (i.e. block-list order). So body 0x200 (lower number) sorts before body 0x400.
* `ComputeSameTails` walks `MergePotentials` **from the back**, so
  `SameTails = [body400, body200]`.
* `CreateCommonTailOnlyBlock`:
  ```cpp
  for (i...) {
      if (SameTails[i].getBlock() == PredBB) { commonTailIndex = i; break; }   // early break
      unsigned t = EstimateRuntime(...);
      if (t <= TimeEstimate) { TimeEstimate = t; commonTailIndex = i; }         // tie -> last
  }
  ```
  `PredBB = &*std::prev(I)` where `I` is the join-test block: the **block-list
  predecessor**, not a CFG predecessor.
* `EstimateRuntime` counts `mayLoad/mayStore` as 2, calls as 10, else 1. Both
  0x200 and 0x400 prefixes are `ldrd; movs` = 3, a **tie**, so the estimate loop
  keeps the last entry = body 0x200.

Therefore the only way to get body 0x400 is `PredBB == body400`, i.e. the join
test block must be emitted **immediately after the body-0x400 block** in the IR.

## 3. Variants measured this wave (all reverted)

`survivor` = `movs r1,#N` immediately before the shared `blx initHudMenu`.

| variant | raw diff | size | survivor | join test |
|---|---:|---:|---|---|
| base | **4363** | 8080 | 0x200 | 0x17aeb4 |
| **H** wingman body falls through to `flags==0` (`else { if (wingman){} if (flags==0){} }`) | 4620 | 8068 | 0x200 | 0x17aeb4 |
| E same but flags as `else if` of wingman | 4655 | 8064 | 0x200 | 0x17aeb4 |
| B `flags==0` as `else` of the 0x400 case, rest of chain nested | 4760 | 8084 | 0x200 | (join is epilogue) |
| C `flags==0` as last `else if` of the dispatch chain | 4669 | 8084 | 0x200 | (join is epilogue) |
| D `flags==0` checked first in the second chain | 5501 | 8052 | 0x200 | — |
| HX H + `hudMenuOpen = 1;` before `initHudMenu(1)` (estimate bump) | 4580 | 8072 | 0x200 | 0x17aeba |
| HX3 H + `hudMenuOpen = 0;` before `initHudMenu(1)` | 4580 | 8072 | 0x200 | 0x17aeba |
| SWAP 0x200/0x400 cases swapped (wave-135 shape) | 4366 | 8080 | **0x400** | 0x17aeb4 |
| SWAPH SWAP + H | 4623 | 8068 | **0x400** | 0x17aeb4 |

## 4. What the MIR proves

`-mllvm -print-after-all` on variant H (wingman body -> flags join):

* IR block order: `... wingmanTail(2092) -> flagsTest(2094) -> flagsBody ...`.
  `wingmanTail` ends with `br label %2094` and is the **list predecessor** of the
  flags test, so `PrevBB.canFallThrough() == true` and `OptimizeBlock` does **not**
  move the join test. `PredBB = wingmanTail`.
* CFO dump: `bb.463 (join test)` predecessors `bb.404 (0x200), bb.428 (0x800),
  bb.430 (0x400), bb.462 (wingman tail), bb.457, bb.458`; `bb.462` falls through
  into `bb.463` (no terminator). So the estimate loop **is** reached.
* Yet the survivor is still body 0x200. Variant HX/HX3 raise body 0x200's prefix
  to `ldr; ldr; movs; strb; movs` (estimate 8 vs 3 for body 0x400) and the
  survivor is still 0x200. This means the `PredBB` early break is still winning
  (or the merge is being done by a later pass, not the CFO dump above); the
  estimate lever is not reachable from the wingman-join shape alone.

## 5. Consequence / next hypothesis

The wingman body -> flags-test join (variant H) is a **content** match to the
original (`b.w 17afe2` instead of `b.n 17b976`) but it is not the lever that moves
the join test: the join test stays at `0x17aeb4` and the survivor stays 0x200.
The survivor flip still requires the flags-test block to be emitted immediately
after the body-0x400 block **while the flags test remains the last branch of the
dispatch chain** (test order `0x200, 0x800, 0x400, 0x2000, ..., 0x1000, flags==0`).

* Variant B emits the flags test right after body 0x400 (`cmp r0,#0; beq` at
  `0x17ad62`) but makes it the 4th dispatch test — wrong branch order.
* Variant H makes the wingman body join the flags test but leaves the flags test
  emitted after the wingman body — right branch order, wrong list predecessor.

The next wave should look for a source shape that gets **both**: the flags test
block emitted after body 0x400 (so `PredBB == body400`) and reached only from the
0x1000 test's false edge (so the branch order is unchanged). Candidates: a
`switch`/`do{}while(0)` wrapper that clang does not re-order, or a helper
variable that is set in the 0x400 arm and tested after the chain (which would
however break the shared-tail structure unless the call stays textually in the
arm).

## 6. Reproduction

```
cp /dev/shm/w8414/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8414/analyze.py        # diff 4363 / 8080 / survivor 1 @0x17adca / join 0x17aeb4
python3 /dev/shm/w8414/gen3.py H         # wingman-join variant
python3 /dev/shm/w8414/gen3.py HX3       # wingman-join + estimate bump
python3 /dev/shm/w8414/gen3.py SWAP      # 0x200/0x400 swap (survivor 0x400)
/dev/shm/w8414/score.sh                  # build + fdiff + survivor
```

Variant generator `/dev/shm/w8414/gen3.py` (variants `base,B,C,D,E,H,HX,HX2,HX3,SWAP,SWAPH`).
LLVM 7 sources `/dev/shm/w8414/BF.cpp`, `/dev/shm/w8414/MBP.cpp`.

The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`; original
0x200/0x400 bodies at `0x17adc0` / `0x17afd8`, shared tail `0x17afde`, join test
`0x17afe2`, wingman tail `0x17bf4a` (`b.w 0x17afe2`).
