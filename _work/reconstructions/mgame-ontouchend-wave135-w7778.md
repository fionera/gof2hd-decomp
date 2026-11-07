# MGame::OnTouchEnd — wave 135 (w7778): moving the wingman body off the join test's PredBB

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7778`,
base `bbffef482`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w7778`. **No source change landed**: every shape that moves the wingman body
either folds back to the base bytes or re-lays-out the function and loses size/score.

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

Baseline (verified this wave, `link_so` + per-function score + section diff):

```
score 82.0000 %   size 8080/8080   ninstr 2775/2774
.text 85,022   .ARM.extab 1,416   .note.gnu.build-id 20
.rodata/.data/.bss/.ARM.exidx/.rel.plt 0
```

## 1. Ground truth re-confirmed (objdump + MIR)

Original hud cluster:

```
17ad4c  ldr.w r0,[fp,#248]; tst #512; bne 17adc0     ; 0x200 test first
17ad56  lsls #20; bmi 17ae8c                          ; 0x800
17ad5c  lsls #21; bmi 17afd8                          ; 0x400
...
17adc0  ldrd r0,r2,[fp,#116]; movs r1,#1; b 17afde    ; 0x200 body, forward branch
17afd8  ldrd r0,r2,[fp,#116]; movs r1,#2              ; 0x400 body
17afde  blx Hud::initHudMenu                          ; shared tail (0x400 falls through)
17afe2  ldr flags; cbz 17b01a                         ; join test: flags == 0
```

So the original has **dispatch order 0x200, 0x800, 0x400** *and* the **0x400 body owns
the shared `blx`**.

## 2. BranchFolder (CFO) facts, re-measured from the real NDK clang 7.0.2

`-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN5MGame10OnTouchEndEiiPv`:

* Pass order is **Control Flow Optimizer (BranchFolder) at dump 681236, then
  “Branch Probability Basic Block Placement” (MBP) at dump 745923**.  BranchFolder runs
  *before* MBP, so the `PredBB` it sees is the **IR/ISel block order**, not the MBP
  layout.  (The wave-134 note's “PredBB is fixed by the first MachineBlockPlacement
  pass” is wrong.)
* Last IR before ISel: the join test (`flags == 0`) is block **2561**; its block-list
  predecessor is block **2552** = the big `wingmanCommand >= 0` body tail
  (`hudMenuOpen=0; closeHudMenu; pauseOpen=0; resumeSounds; br end`).
* The two `initHudMenu` bodies are block **2420** (menu 1) and block **2442** (menu 2).
* Pre-CFO MIR: join test `bb.463` (ir-block 2567), predecessor `bb.462`
  (ir-block 2558, wingman-body tail).  `SameTails` = `[bb.430 (0x400 body),
  bb.426 (0x200 body)]`.
* The common tail is only the `blx` (the `ldrd; movs r1,#N` prefixes differ), so
  `tailIsWholeBlock()` is **false** for both.  `CreateCommonTailOnlyBlock` is therefore
  reached and its `if (t <= TimeEstimate)` tie-break keeps the **last** entry
  (`SameTails[1]` = bb.426 = 0x200 body).  Both prefixes estimate 2 (load) + 2 (load)
  + 1 (movs) = 5, so it is a pure tie.
* CFO dump confirms the outcome: shared tail `bb.405` is split out of `bb.404`
  (0x200 body); `bb.435` (0x400 body) branches to it.

To flip the owner you need either `PredBB == bb.430` (0x400 body) or `SameTails` order
reversed (0x400 body emitted *before* 0x200 body in the IR).

## 3. Variants measured this wave (all reverted; base restored)

| variant | pct | size | `.text` | `.rel.plt` | owner of shared `blx` |
|---|---:|---:|---:|---:|---|
| base | **82.0** | 8080 | 85,022 | 0 | 0x200 (`movs r1,#1`) |
| swap 0x200/0x400 cases | 81.9 | 8080 | 85,025 | 0 | **0x400** (`movs r1,#2`) |
| swapAB (wingman `else if` before weaponSlot) | 82.0 | 8080 | 85,022 | 0 | 0x200 — **byte-identical to base** |
| swapBC (wingman `else if` after `flags==0`) | 76.7 | 8036 | shifted | 22 | 0x200 |
| swapAC (`flags==0` first) | 76.3 | 8104 | shifted | 22 | 0x200 |
| dispatch 0x800 last | 77.3 | 8080 | 85,359 | 24 | 0x200 |
| dispatch 0x400 last | 77.5 | 8080 | 85,341 | 27 | 0x200 |
| dispatch 0x200 last | 77.2 | 8080 | 85,359 | 24 | 0x200 |
| `flags==0` after 0x400 case (dispatch chain) | 76.1 | 8084 | shifted | 0 | 0x200 |
| `flags==0` at end of dispatch chain | 80.6 | 8084 | shifted | 27 | 0x200 |
| `flags==0` after 0x200 case | 74.5 | 8084 | shifted | 2 | 0x200 |
| `flags==0` after 0x800 case | 75.9 | 8084 | shifted | 0 | 0x200 |
| separate `if (flags==0)` after chain | 44.5 | 7704 | shifted | 0 | (no shared blx) |
| boolean flag moves wingman body after chain | 80.7 | 8076 | shifted | 0 | 0x200 |
| wingman tail reordered to match weaponSlot tail | 81.8 | 8080 | 85,022 | 0 | 0x200 |
| local-var spellings of `initHudMenu` args (6 shapes) | 82.0 | 8080 | 85,022 | 0 | 0x200 — byte-identical |
| `hudMenuOpen = hudMenuOpen` in 0x200 body | 80.8 | 8084 | shifted | 0 | 0x200 |

### Key negative results

1. **Moving the wingman body in source order is normalised away.**  `swapAB`
   (wingman body before the weaponSlot body) is byte-identical to the base — the IR
   block order from clang is the same and MBP produces the same layout.  Any other
   placement (`swapBC`, boolean flag, `goto`) changes the **merge point** or the block
   frequencies and loses size (8036/8076) and score.
2. **The ownership flip does not move the join test.**  `swap200_400` flips the shared
   `blx` owner to the 0x400 body (verified: `movs r1,#2` immediately before the `blx`)
   but the join test stays at `0x17aeb4`, still immediately after the 0x800 body.  So
   the join test placement is a *separate* MBP decision, not a consequence of the
   BranchFolder owner.
3. **The EstimateRuntime lever is not reachable from source.**  The 0x200/0x400
   prefixes are identical (`ldr; ldr; ADJCALLSTACKDOWN; movs`).  Six local-variable /
   reordered-argument spellings of `initHudMenu` all compile to the base bytes; only a
   real extra instruction (e.g. a self-assignment store) changes the estimate, and that
   costs 4 B and re-lays-out the function.

## 4. Why the join test sits after the 0x800 body (MBP evidence)

Base MBP dump (layout order): `bb.356` (0x200 body) idx201, `bb.357` (shared tail)
idx202, unrelated cold blocks idx203-208, `bb.358` (0x800 body) idx209, `bb.359` (join
test) idx210.  Edge probabilities into the join test: `bb.358` 200 %, `bb.357` 200 %,
`bb.352` (UseKhadorDrive body) 200 %, `bb.351` (0x1000 test) 50 %.  The 0x800 body is a
chain head whose only successor is the join test, so `buildChain` claims the join test
for it; the shared tail (`bb.357`) is already laid out earlier and does not get it.

To move the join test after the shared `blx` one must make `bb.357` claim `bb.359` in
`buildChain` (or remove the 0x800 body as a predecessor).  No source spelling tried
here achieves that: the 0x800 body always remains a chain head with the join test as
its single successor.

## 5. Reproduction

```
cp /dev/shm/w7778/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7778/score_raw.py        # 82.0000 / 8080 / 2775:2774
python3 /dev/shm/w7778/secdiff.py          # .text 85,022 / .ARM.extab 1,416
python3 /dev/shm/w7778/det.py              # shared blx owner + join-test address
```

Variant generators: `/dev/shm/w7778/gen2.py` (dispatch/second-chain permutations),
`gen3.py` (`flags==0` in the dispatch chain), `gen4.py` (body spellings),
`gen5.py` (boolean flag / goto), `gen6.py` (wingman tail order).

The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`; the original
0x200/0x400 bodies are at `0x17adc0` (`ldrd; movs r1,#1; b 0x17afde`) and `0x17afd8`
(`ldrd; movs r1,#2; fallthrough 0x17afde`).
