# MGame::OnTouchEnd (8080 B) — wf307 (wave 103): the residual is MBP's
# post-layout tail merge, and it is weight-sensitive, not order-sensitive

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA
0x17a144, 8080 B).  Baseline = the wave-102 faithful shape (`908f1ae6d`):
**74.8 %**, 8080 B, gate-green (byte 4500 / linked 4500 / size 4524 / data
843 / imports 207 / lint CLEAN / drift 0).  **No source change landed this
pass.**  This note records the new evidence so the next pass does not
re-derive it.

## 1. What is left

The instruction stream is identical to the original from the function entry
through `0x17ada8` (1098 instructions); every earlier difference is only a
branch target or a literal-pool offset.  The first real difference is the
camera-dispatch `beq` at `0x17adae` (`beq.n` → `beq.w`), and from there the
whole tail cluster is laid out differently:

```
original                                     ours (wave-102 shape)
17adc0  0x200 arm: ldrd; movs r1,#1         17adc4  0x200 arm: ldrd; movs r1,#1
17adc6  b 17afde                            17adca  blx initHudMenu
17adc8  campaign chain ...                  17adce  b.n 17aeb2
17ae8c  0x800 arm: ...; b 17afe2            17ae94  0x800 arm: ... (falls through)
17aeac  maneuver                            17aeb2  test hudTouchFlags==0
17aebc  menu-touch ...                      17aebc  hud0 body
17afd8  0x400 arm: ldrd; movs r1,#2         17aef8  maneuver
17afde  blx initHudMenu   <-- shared        17af0a  menu-touch ...
17afe2  test hudTouchFlags==0               17afa8  0x400 arm: ldrd; movs r1,#2
17afe8  isAutoPilot tail                    17afae  b.n 17adca   <-- shared blx
17b004  epilogue                            17b95a  isAutoPilot tail
17b016  __stack_chk_fail                    17b976  epilogue
17b01a  hud0 body                           17b988  __stack_chk_fail
```

So the original has `[0x400 arm][shared blx][test][isAutoPilot][epilogue]`
with the hud0 body last; ours has `[0x200 arm][shared blx][campaign]
[0x800 arm][test][hud0 body]...` with the `[isAutoPilot][epilogue]` pair at
the very end.  Both functions are exactly 8080 B and the instruction
multiset is essentially equal (orig 2865 insns / ours 2772 in the object,
2863 in the linked `.so`).

## 2. Mechanism: MBP runs a *second* BranchFolder and then re-lays-out

The tail merge is not the earlier `Control Flow Optimizer` pass.  In LLVM 7
`MachineBlockPlacement::runOnMachineFunction`
(`lib/CodeGen/MachineBlockPlacement.cpp:2812`) does, after the first
`buildCFGChains()`:

```cpp
unsigned TailMergeSize = TailDupSize + 1;      // optsize -> TailDupSize = 1
BranchFolder BF(true, false, *MBFI, *MBPI, TailMergeSize);
if (BF.OptimizeFunction(MF, ..., /*AfterBlockPlacement=*/true)) {
  BlockToChain.clear(); ComputedEdges.clear();
  buildCFGChains();                            // <-- redo the layout
}
```

`-Oz` sets `optForSize()`, so `TailMergeSize == 2`: two `blx initHudMenu`
tails (the `0x200` arm and the `0x400` arm) are merged into one
common-tail block, and then the *whole function is laid out again*.
The clang 7.0.2 MIR confirms the merged block:

```
$ clang++ ... -mllvm -print-machineinstrs -c src/game/menu/MGame.cpp
# After Branch Probability Basic Block Placement:
bb.342 successors: %bb.346(100%)     $r1 = t2MOVi 1            ; 0x200 arm
bb.346 successors: %bb.347(100%)     tBL @_ZN3Hud11initHudMenu  ; shared
                                     t2B %bb.347
bb.345 successors: %bb.346(100%)     $r1 = t2MOVi 2            ; 0x400 arm
                                     t2B %bb.346
```

The second layout places `bb.346` immediately after `bb.342` (the `0x200`
arm) in our build, and after `bb.345` (the `0x400` arm) in the original.
Which arm owns the shared `blx` is decided by `BranchFolder`'s
`CreateCommonTailOnlyBlock` (`EstimateRuntime` tie-break on the block
number), and where the merged block lands is decided by the second
`buildCFGChains`.

## 3. Why the second layout differs: HBLP on the shared-blx -> test edge

`hasBetterLayoutPredecessor` rejects the edge `shared-blx -> test` when any
unplaced predecessor of the test has a hot enough edge:

```
PredEdgeFreq * HotProb >= CandidateEdgeFreq * HotProb.getCompl()
HotProb = StaticLikelyProb/100 = 80/100
```

The test's predecessors are the `0x800` arm, `UseKhadorDrive`, the wingman
tail and the shared blx.  With the flag chain being 50/50 per bit
(`calcZeroHeuristics` bails out for `icmp eq (and x, 2^k), 0`) and
`hudTouchFlags == 0` = 37.5 % (`ZH_TAKEN_WEIGHT 20 / ZH_NONTAKEN_WEIGHT 12`):

```
freq(0x200 arm)   = 0.500 * freq(0x200 test)
freq(0x400 arm)   = 0.125 * freq(0x200 test)
freq(shared blx)  = 0.625 * freq(0x200 test)   (both arms branch to it)
freq(0x800 arm)   = 0.250 * freq(0x200 test)
=> freq(0x800 arm) / freq(shared blx) = 0.40  >= 0.25  -> CONFLICT
```

so the shared-blx -> test edge is rejected and the test is placed after the
`0x800` arm instead.  (This reproduces the handoff's `CONFLICT pred
bb.345 predfreq=927228533 candfreq=2318071333` from the llc 7.0.0 trace.)
The ratio is exactly 0.40, not a ±1 rounding case, so **no statement-order
change can flip it** — only a change in the branch weights or in the IR
block order can.

## 4. Measurements (official verifier, `try.sh`)

| variant | size | fuzzy | note |
|---|---|---|---|
| wave-102 faithful (current tree) | 8080 | **74.8** | gate-green |
| 144 new structural variants (`/dev/shm/wf307/g5`) | 8032–8088 | — | none changes the tail |
| 300 wave-102 `v3` variants | 8076–8084 | — | 4 layout classes, none matches |
| oracle camera-first (`if (!hudMenuOpen) { camera } else { flags }`) | 8080 | — | object-identical |
| hud0 test moved before weaponSlot | 8048–8052 | — | tail moves but size breaks |
| `__builtin_expect(flags & 0x800, 1)` | 8036 | — | tail 0x181a -> 0xd6e |
| `__builtin_expect(flags & 0x800, 0)` | 8076 | — | tail 0x181a -> 0x15c6 |
| `__builtin_expect(flags == 0, 0)` | 8084 | — | tail 0x181a -> 0xd76 |
| tuned101 (`24c173bc7` shape) | 8080 | 78.4 | not faithful (see §5) |

The four achievable layout classes over the 444 statement-order variants are:

```
120 x  size 8076  test 0x17ec  tail 0x17f6   (else-if wingman + flattened hud0)
 75 x  size 8084  test None    tail 0x17f0
 75 x  size 8080  test 0xd6e   tail 0x181a   (nested; == current)
 30 x  size 8080  test 0x17ec  tail 0x17f8
```

The original is `test 0xe9e / tail 0xea8`.  No statement order reaches it.
The `__builtin_expect` rows show the tail *is* movable, but only by changing
the branch weights (and every such variant loses size-exactness).

## 5. The 78.4 % tuned shape is content-wrong

`/dev/shm/wf307/MGame_tuned101.cpp` (git `24c173bc7`) scores 78.4 % at
8080 B but differs from the binary in five proven places:

1. `amount--` / `count = -amount; if (count > 0)` — the wave-103 pass
   (`908f1ae6d`) proved `amount++` / `if (amount > 0)` / `String(amount)`
   reproduces `cmp.w r9,#0; beq` and the whole report block byte-for-byte.
2. `mission = ... : campaign` — reuses the `campaign` local, dropping the
   third `getCampaignMission()` call the binary makes at `0x17af78`.
3. `isInstantActionMission` after the campaign/freelance dispatch — the
   binary calls it first at `0x17af7e` (`cbz` -> `0x17afa6`).
4. weaponSlot tail order `resumeSounds()` first — the binary is
   `pauseOpen=0; hudMenuOpen=0; resumeSounds(); closeHudMenu()`.
5. `else if (wingmanCommand >= 0)` + flattened `if (hudTouchFlags == 0)` —
   the binary's weaponSlot tail at `0x17ba24` does `b.w 0x17afe8`, i.e. it
   *skips* the test; the flattened form sends it through the test.

So the current tree is the faithful shape and 74.8 % is the honest faithful
number; 78.4 % is the tuned number.

## 6. Exact commands

```
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
# fast single-TU harness (exact NDK r18b flags, ~0.5 s):
/dev/shm/wf307/cc.sh /dev/shm/wf307/<variant>.cpp /dev/shm/wf307/<variant>.o
python3 /dev/shm/wf307/tailpos.py /dev/shm/wf307/<variant>.o
# official row (~21 s):
/dev/shm/wf307/try.sh /dev/shm/wf307/<variant>.cpp
# clang 7.0.2 MIR (tail-merge evidence):
.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  <exact MGame.cpp flags> -mllvm -print-machineinstrs -c src/game/menu/MGame.cpp
```

`tailpos.py` prints the byte offset of the `hudTouchFlags == 0` test and of
the tail `isAutoPilot` call; the original is `test 0xe9e / tail 0xea8`.

## 7. Conclusion / next lever

The residual is not a source-shape problem that statement reordering can
fix: the faithful CFG gives `freq(0x800 arm) = 0.40 * freq(shared blx)`,
comfortably above the 0.25 HBLP threshold, so the test can never follow the
shared `blx`.  To reproduce the original layout one of these must be true
and has not been found yet:

* the original's `hudTouchFlags & 0x800` branch weight is not 50/50 (e.g. a
  different condition spelling that defeats `calcZeroHeuristics`), or
* the original's IR block order makes the `0x800` arm and the shared blx
  land in the *same* chain in the first `buildCFGChains` (so
  `BlockToChain[Pred] == &Chain` skips the conflict), or
* the original's `hudTouchFlags`/`hudMenuOpen` types differ in a way that
  changes `BranchProbabilityInfo` (not reproduced by `int`/`unsigned`/
  `bool`/`uint8_t` swaps tried so far).

Do not `--update-baseline`; the gate is green and this pass is research only.

## 8. wave-103b: re-verified, no faithful improvement; the score driver is the
##    `[isAutoPilot][epilogue]` placement, and the test edge has *several*
##    independent HBLP conflicts

Re-ran the gate on this worktree first (`check`): **RATCHET PASS**, byte 4500 /
linked 4500 / size 4524 / data 843 / imports 207 / lint CLEAN / drift 0.
`MGame::OnTouchEnd` = **74.8 %**, 8080 B, `byte=False`, `linked=False`.
Nothing in this section changed `src/`; all numbers below are measured.

### 8.1 The fuzzy gap is one edge, not the whole tail

The function has the *same* control-flow skeleton in both builds: 21 `b.w`
sites target the epilogue and 8 target the `isAutoPilot` tail in the original
**and** in ours.  What differs is where those two blocks land:

```
                         original            ours
isAutoPilot tail         0x17afe8 (0xea4)   0x17b95a (0x1816)
epilogue                 0x17b004 (0xec0)   0x17b976 (0x1832)
hudTouchFlags == 0 test  0x17afe2 (0xe9e)   0x17aeb2 (0xd6e)
```

Because the original chains `[test][isAutoPilot][epilogue][stack_chk_fail]`
while ours chains `[test][hud0]` and puts `[isAutoPilot][epilogue]` at the very
end, **all 29 of those branch sites resolve to a different function-relative
offset**, and difflib's alignment then cascades through the rest of the tail.
That is why the score is 74.8 % even though the instruction stream is identical
for the first 1098 instructions.  Fixing the single `test -> isAutoPilot` edge
would move the epilogue early and recover essentially the whole gap.

### 8.2 No variant reaches `test -> isAutoPilot` (the earlier "WIN" was a false positive)

`checktail.py`/a `cbz`-only check reports "WIN" for 48 `g5`/`g6` variants and
120 `v3` variants, but that only proves the test block ends in `cbz`, not that
its fall-through is the `isAutoPilot` tail.  A stricter check (`ldr.w r0,[fp,#248]`
-> `cbz` -> within 4 insns `blx isAutoPilot`) finds **zero** WINs across all
444 variants.  The `g5`/`g6` WINs are all `testpos in {before, between}`, where
the `cbz` falls through into the weaponSlot/wingman block instead.

### 8.3 The `test -> isAutoPilot` edge is blocked by several preds, not just one

LLVM 7 `hasBetterLayoutPredecessor` rejects edge `test -> isAutoPilot` when any
unplaced predecessor `P` of the join satisfies
`freq(P->join) * 80/100 >= freq(test->join) * 20/100`, i.e.
`freq(P->join) >= 0.25 * 0.625 * freq(test)`.

Parsing the real clang 7.0.2 post-MBP MIR CFG (`post_mbp.txt`:
`bb.NNN (%ir-block.MMM)` + `successors: %bb.X(0x...)`) and propagating
frequencies from the entry (entry = 16384, one forward pass in layout order;
approximate but order-correct) gives

```
test (bb.347)            freq  41.0
isAutoPilot join (bb.348) freq 93.1
threshold = 0.25*0.625*41.0 = 6.4

pred edge freqs of the join:
  weaponSlot tail (bb.360)  19.4   CONFLICT
  camera-dispatch pred (bb.340) 10.9 CONFLICT
  freeCam/cam3 (bb.341)     10.9   CONFLICT
  maneuver (bb.344)         10.9   CONFLICT
  hud0 body (bb.351)         5.8   ok
  hud0 _bc8 (bb.352)         3.6   ok
  hud0 resetGunDelay (bb.353) 6.0  ok
```

So even if the camera dispatch were removed, the **weaponSlot tail** alone
(19.4, ~3x the threshold) still blocks the edge.  This is a second, independent
structural conflict on top of the two in §2/§3 (`0x200 -> shared` 1-unit
near-miss, `shared -> test` blocked by the `0x800` arm).

### 8.4 The instrumented llc is *not* a faithful 7.0.2 model

`llc` 7.0.0 on the exact `-Oz` IR (`MGame_oz.ll`, attributes
`minsize optsize sspstrong`) still emits a **7608 B / 2532-insn** function
(`sub sp,#0xbc`) where clang 7.0.2 emits **8080 B / 2772-insn**
(`sub sp,#0xcc`).  `-float-abi=softfp` is rejected by llc and `default`/`soft`
both give the small frame, so the MBP block order printed by the instrumented
llc (`FINAL:` etc.) does **not** match the `.so`.  Use the real clang 7.0.2
`-mllvm -print-machineinstrs` MIR for CFG/branch-probability ground truth
(`post_mbp.txt`) and treat llc's layout trace as indicative only.

### 8.5 Spelling variants are object-identical

All of these leave the object byte-identical to the current tree
(`test 0xd6e / tail 0x181a`), i.e. they are not levers:

```
if (this->hudMenuOpen != 0) / == 1 / == true
if (!this->hudTouchFlags) / == 0U / 0 == this->hudTouchFlags
```

`switch (cameraMode) { case 0: ...; case 3: ...; }` (case order swapped) and
`if (cameraMode == 0) ... else if (cameraMode == 3) ...` both move the test to
`0xd74 / tail 0x181a` and score **74.7 %** (slightly worse than 74.8 %).

### 8.6 Official-verifier measurements this session

| variant | osize | usize | fuzzy |
|---|---|---|---|
| current tree (wave-102 faithful) | 8080 | 8080 | **74.8** |
| `v_t101faith14` (tuned101 + 3 faithful fixes) | 8080 | 8080 | 76.9 |
| `g6/h0024` (test before weaponSlot) | 8080 | 8048 | 70.9 |
| `g6/h0031` (same, else-if form) | 8080 | 8052 | 70.8 |
| `g6/h0048` (test between weaponSlot and wingman) | 8080 | 8032 | 71.6 |
| `q_camif03` (oracle camera if/else-if, 0 first) | 8080 | 8080 | 74.7 |

**`v_t101faith14` is not faithful.**  Its hud0 body is
`hudMenuOpen = 0; if (hud != nullptr) closeHudMenu(); touch0Id = ...` — the
binary is `closeHudMenu(); hudMenuOpen = 0; touch0Id = ...` with no null check
(verified at `0x17b01a` in the original and `0x17b96c` in the variant's `.so`).
It also keeps the flattened `else if (wingmanCommand >= 0)` + `if (hudTouchFlags
== 0)` shape (content diff #5) and the reused `campaign` local (diff #2).  So
76.9 % is a tuned number, not an honest faithful one.

### 8.7 Exact commands used

```
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
/dev/shm/wf307/cc.sh /dev/shm/wf307/<v>.cpp /dev/shm/wf307/<v>.o   # 0.6 s
python3 /dev/shm/wf307/tailpos.py /dev/shm/wf307/<v>.o
python3 /dev/shm/wf307/wincheck2.py /dev/shm/wf307/<v>.o
/dev/shm/wf307/try.sh /dev/shm/wf307/<v>.cpp                # official row, ~21 s
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5MGame10OnTouchEndEiiPv
```

### 8.8 Conclusion

No faithful source shape beats 74.8 %.  The residual is a single MBP chain
decision (`test -> isAutoPilot`) that is blocked by at least five independent
predicate frequencies, and every shape that unblocks it changes the source
content (flat/`before`/`between` hud0 placement) or the branch weights
(`__builtin_expect`, which also loses size exactness).  Keep the wave-102
faithful shape; the 78.4 % tuned shape and the 76.9 % `v_t101faith14` shape are
both content-wrong.
