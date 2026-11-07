# MGame::OnTouchEnd — wave 138 (w8314): the join-test move is `OptimizeBlock`'s predecessor walk

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w8314`,
base `96b069792`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w8314`. **No source change landed** (base is byte-identical to HEAD and the
full gate is green). This note pins the *second* half of the survivor mechanism
(wave 136 pinned the `CreateCommonTailOnlyBlock` half) and falsifies the
"reorder the bodies with `goto`" family.

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

Baseline (verified this wave, full `check`):

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
size 8080/8080   raw fuzzy 82.0000 %   struct diffs 1688/2775
```

## 1. Ground truth (original disasm, objdump)

The 0x200 / 0x400 bodies and the shared `blx` / join test:

```
17ad52: ldr.w r0,[fp,#248]        ; hudTouchFlags
17ad56: tst.w r0,#512             ; 0x200
17ad5a: bne.n 17adc0              ; -> body 0x200
17ad5c: lsls r1,r0,#20            ; 0x800
17ad5e: bmi.w 17ae8c              ; -> body 0x800
17ad62: lsls r1,r0,#21            ; 0x400
17ad64: bmi.w 17afd8              ; -> body 0x400
... (0x2000,0x4000,0x8000,0x10000,0x20000,0x40000,0x80000,0x100000,0x1000) ...
17adb8: blx UseKhadorDrive ; b.n 17afe2

17adc0: ldrd r0,r2,[fp,#116]      ; body 0x200
17adc4: movs r1,#1
17adc6: b.n 17afde                ; branch to shared tail

17afd8: ldrd r0,r2,[fp,#116]      ; body 0x400
17afdc: movs r1,#2
17afde: blx initHudMenu           ; shared tail
17afe2: ldr.w r0,[fp,#248]        ; join test: hudTouchFlags == 0
17afe6: cbz r0, 17b01a            ; -> flags body
17afe8: ...                       ; epilogue (isAutoPilot)
```

So in the original the **0x400 body owns the shared `blx`** and the join test
falls through from it. In our base the **0x200 body owns the `blx`**:

```
ours 17adc4: ldrd r0,r2,[fp,#116] ; body 0x200
ours 17adc8: movs r1,#1
ours 17adca: blx initHudMenu      ; shared tail (owned by 0x200)
ours 17adce: b.n 17aeb4           ; -> join test far away
```

Test order is identical in both (`0x200, 0x800, 0x400, 0x2000, ...`), so the source
order of the dispatch chain is already right.

## 2. The full mechanism (two passes, not one)

Wave 136 proved the *second* half: at the CFO pass `CreateCommonTailOnlyBlock`
takes the `SameTails[i].getBlock() == PredBB` early break, so the block that is the
join test's **list predecessor** at tail-merge time becomes the survivor.

This wave pins the *first* half — how the join test gets that predecessor. It is
`BranchFolder::OptimizeBlock` (BranchFolding.cpp, LLVM 7), the block:

```cpp
if (!PrevBB.canFallThrough()) {
  bool CurFallsThru = MBB->canFallThrough();
  for (MachineBasicBlock *PredBB : MBB->predecessors()) {
    if (PredBB != MBB && !PredBB->canFallThrough() &&
        !TII->analyzeBranch(*PredBB, PredTBB, PredFBB, PredCond, true) &&
        (!CurFallsThru || !CurTBB || !CurFBB) &&
        (!CurFallsThru || MBB->getNumber() >= PredBB->getNumber())) {
      if (CurFallsThru) { ...insert unconditional branch to NextBB... }
      MBB->moveAfter(PredBB);
      goto ReoptimizeBlock;
    }
  }
}
```

`MBB` is the join-test block; `PrevBB` is the wingman tail (`b` to the final tail,
`canFallThrough()==false`). The join test ends with `t2Bcc epilogue; t2B flagsbody`;
`analyzeBranch` (AllowModify=true) deletes the redundant `t2B` because `flagsbody`
is the next block, so `CurFBB==null` and `(!CurFallsThru || !CurTBB || !CurFBB)`
is satisfied. The walk then moves the join test **after the first predecessor that
cannot fall through**.

Predecessor order at that point is the IR block order (verified against the `.ll`:
blocks 1988 body0x200, 1995 body0x800, 2000 body0x400, 2034 test0x1000, 2037
UseKhadorDrive):

```
base PEI dump: bb.463 (join test) predecessors = bb.426(0x200) bb.428(0x800) bb.430(0x400) bb.447 bb.448
```

so the first non-fall-through predecessor is **body 0x200** → the join test is moved
right after it → in the next `while` iteration `TailMergeBlocks` sees `PredBB =
body 0x200` → early break → 0x200 survives. That is exactly the base CFO dump:

```
bb.403 (%ir-block.2419)  test 0x200
bb.404 (%ir-block.2426)  body 0x200 (ldr,ldr,movs r1,#1)   ; no blx
bb.405 (%ir-block.2567)  shared blx initHudMenu            ; split off
bb.406 (%ir-block.2567)  join test (ldr 248; cmp 0; bcc)
```

The original needs the walk to skip body 0x200 and body 0x800 and land on body
0x400. Nothing in the source spelling of the *bodies* can do that (wave 136
already falsified the `EstimateRuntime` lever: `hudMenuOpen=1`, `pauseSounds()`
etc. still leave body 0x200 as `PredBB`), because the early break fires before the
estimate loop.

## 3. Variants measured this wave (all reverted; base restored)

`survivor` = the `movs r1,#N` immediately preceding the shared `blx initHudMenu`
(N=1 → 0x200 owns, N=2 → 0x400 owns).

| variant | size | pct | survivor | struct diffs |
|---|---:|---:|---|---:|
| base | 8080 | **82.00** | 0x200 | 1688 |
| nested `else { if (0x2000)... }` for 0x400 | 8080 | 82.00 | 0x200 | 1688 (byte-identical) |
| `goto body400` moved after the chain | 8080 | 82.00 | 0x200 | 1688 (byte-identical) |
| `goto body200` + `goto body400` to end, 400 first | 8080 | 82.00 | 0x200 | 1688 (byte-identical) |
| `goto body200` + `goto body400` to end, 200 first | 8080 | 82.00 | 0x200 | 1688 (byte-identical) |
| `goto body200` + `goto body800` moved after body400 | 8080 | 82.00 | 0x200 | 1688 (byte-identical) |
| `weaponSlot != -1` | 8080 | 82.00 | 0x200 | 1688 (byte-identical) |
| `wingmanCommand != -1` | 8080 | 82.00 | 0x200 | 1688 (byte-identical) |
| `!hudTouchFlags` | 8080 | 82.00 | 0x200 | 1688 (byte-identical) |
| `hudMenuOpen != 0` / `== 1` / `!!` | 8080 | 82.00 | 0x200 | 1688 (byte-identical) |
| empty `else if (false)` after 0x200 | 8080 | 82.00 | 0x200 | 1688 (byte-identical) |
| nested `else { if (0x800)... }` | 8080 | 82.00 | 0x200 | 1688 (byte-identical) |
| **swap 0x200/0x400 cases** (test 0x400,0x800,0x200) | 8080 | 81.90 | **0x400** | 1691 |
| **swap 0x800/0x400 cases** (test 0x200,0x400,0x800) | 8076 | 79.10 | **0x400** | 1712 |
| 0x400 case first (test 0x400,0x200,0x800) | 8076 | 79.00 | 0x200 | 1714 |
| swap 0x800/0x200 (test 0x800,0x200,0x400) | 8068 | 80.50 | 0x200 | 1647 |
| `useCloak()` before `resumeSounds()` in 0x800 | 8080 | 81.90 | 0x200 | 1687 |
| separate `if (flags==0)` after chain | 8076 | 79.10 | 0x200 | 1722 |
| flags==0 first in weaponSlot chain | 8052 | 75.30 | 0x200 | 1984 |
| `_bc8` block moved out of flags body | 8084 | 79.90 | 0x200 | 1725 |
| wingman tail `pauseOpen`/`closeHudMenu` reorder | 8080 | 81.80 | 0x200 | 1688 |

Key observations:

* **`goto` is fully normalised away.** Every `goto`-to-label shape that keeps the
  test order compiles to the exact same bytes as the base (survivor unchanged).
  So the "move the body physically last" idea cannot be expressed with `goto`.
* **The survivor flips exactly when the 0x400 case is not the third case.** With
  source order `0x400,0x800,0x200` or `0x200,0x400,0x800` the 0x400 body becomes
  the join test's first non-fall-through predecessor and survives — but the test
  order is then wrong, and the score drops (81.9 / 79.1).
* **The survivor flip alone is not a win.** `swap200_400` has the correct survivor
  and size 8080 but scores 81.9 % because the dispatch-test order is wrong; the
  base has the wrong survivor but the right test order and scores 82.0 %. They are
  roughly equivalent under the fuzzy metric.

## 4. Open question for the next wave

The original needs **test order `0x200,0x800,0x400` AND survivor `0x400`**. In the
current source those two are coupled: the survivor is the first non-fall-through
predecessor, which is the first body in IR order, which is the first test in source
order. To decouple them we need a source shape where body 0x400 is emitted before
body 0x200 in the IR **while the tests stay in 0x200/0x800/0x400 order**. `goto`
does not do it (normalised). Candidate next ideas:

1. Put the `initHudMenu(2)` call behind a helper variable that is set in the 0x400
   arm but called after the chain — this would however break the shared-tail
   structure unless the call is still textually in the arm.
2. Force the IR block order with a construct the IR optimiser will not reorder
   (e.g. a `switch` on `hudTouchFlags & 0x600` with `case 0x200/0x400/0x0`, or a
   `do { } while (0)` wrapper) and check the `.ll` block order before building.
3. Alternatively accept the `swap200_400` shape and try to recover the ~0.1 % by
   fixing the two known content deltas (`cmp r4,#0` vs `#1` = `hasFailed()==true`
   spelling; `cmp.w r9,#0` vs `#-1` = mining `count > 0` vs `amount != 0`), which
   wave 132 measured as 12 B short on their own.

## 5. Reproduction

```
cp /dev/shm/w8314/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8314/score.py            # 82.0000 / 8080 / 1688
/dev/shm/w8314/check.sh base               # prints survivor = 0,1,3
# CFO block order (slow, ~40 s, -mllvm -print-after-all):
/dev/shm/w8314/cfo.sh                      # bb.403 test, bb.404 body200, bb.405 tail, bb.406 join
```

Variant runners: `/dev/shm/w8314/variants*.py`, `batch.py`, `extract_cfo.py`,
`extract_full.py`, `extract_blocks.py`.

The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`; the original
0x200/0x400 bodies are at `0x17adc0` (`ldrd; movs r1,#1; b 0x17afde`) and
`0x17afd8` (`ldrd; movs r1,#2; fallthrough 0x17afde`), shared tail `0x17afde`,
join test `0x17afe2`.
