# MGame::OnTouchEnd — wave 133 (w7734): CFO vs MBP tail-merge ownership

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7734`,
base `144fc16b0`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w7734`. **No source change landed**: every shape that flips the
`initHudMenu` tail merge changes the emitted dispatch-test order or the function
size, and none beats the base.

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

Baseline (verified this wave, full `check`):

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS          lint CLEAN   drift 0
.text 85,244   .ARM.extab 1,416   .note.gnu.build-id 20
.ARM.exidx 0   .rel.plt 0   .rodata/.data/.bss 0
raw fuzzy (difflib, autojunk off): 81.9607 %   size 8080/8080   insns 2775/2774
```

## 1. The merge is BranchFolder (CFO), not MachineBlockPlacement

`-mllvm -print-after-all` on the real NDK clang 7.0.2 shows the pass order for
`OnTouchEnd`: `Prologue/Epilogue Insertion` → **`Control Flow Optimizer`** →
`Tail Duplication` → ... → `Branch Probability Basic Block Placement` (MBP).

* Before CFO the function has **4** `initHudMenu` calls.
* After CFO it has **3**: the two `movs r1,#1` / `movs r1,#2` bodies are
  tail-merged and share one `blx Hud::initHudMenu`.
* The MBP dump still has **3**, so MBP does not merge anything.

"Control Flow Optimizer" is `BranchFolderPass` (`BranchFolding.cpp`). The
survivor is chosen in `BranchFolder::CreateCommonTailOnlyBlock` (release_70,
lines 784–830):

```cpp
commonTailIndex = 0; unsigned TimeEstimate = ~0U;
for (i = 0; i != SameTails.size(); ++i) {
  if (SameTails[i].getBlock() == PredBB) { commonTailIndex = i; break; }   // (1)
  unsigned t = EstimateRuntime(SameTails[i].getBlock()->begin(),
                               SameTails[i].getTailStartPos());
  if (t <= TimeEstimate) { TimeEstimate = t; commonTailIndex = i; }        // (2)
}
```

* `PredBB = &*std::prev(I)` where `I` is the join block being optimized
  (`BranchFolding.cpp` ~1114). It is simply the **previous block in the
  machine-function block list**, whether or not it branches to `I`.
* `SameTails` is built by `ComputeSameTails` walking `MergePotentials` from the
  **highest** block number down, pushing `CurMPIter` then `I`. For a pair this
  is `[higher-numbered body, lower-numbered body]`.
* `EstimateRuntime` is 10 per call, 2 per load/store, 1 otherwise. The two
  bodies are `ldr r0,[..,#116]; ldr r2,[..,#120]; movs r1,#N` → both 5.
* Therefore, on a tie the **last** entry wins, i.e. the **lower-numbered**
  (earlier) body.

In the base build:

* join block = the `hudTouchFlags == 0` test (bb.463 in the PEI MIR);
* `PredBB = prev(bb.463) = bb.462` = the wingman-body tail (`closeHudMenu;
  resumeSounds; b join`), which is **not** one of the two tails;
* the two bodies are bb.426 (`initHudMenu(1)`, lower number) and bb.430
  (`initHudMenu(2)`, higher number), both estimate 5;
* tie → last entry = bb.426 = the **0x200 body**.

The CFO output confirms it: the shared-tail block bb.405 is inserted directly
after bb.404 (the 0x200 body); the 0x400 body bb.435 branches to bb.405.

## 2. MBP then re-chains the shared tail to the earliest body

Even when CFO is forced to split the tail from the 0x400 body, MBP re-chains it
to the 0x200 body. `MachineBlockPlacement` walks the block list in order and
chains the first body it sees whose only successor is the shared tail. The
0x200 body is always earlier (it follows the first dispatch test), so it wins
the final layout.

This was verified with a `goto` shape that puts the 0x400 body before the 0x200
body in the IR:

```cpp
if (this->hudTouchFlags & 0x200) goto hbMenu1;
else if (this->hudTouchFlags & 0x800) { ... }
else if (this->hudTouchFlags & 0x400) goto hbMenu2;
...
hbMenu2: this->hud->initHudMenu(2, this->level);   // 0x400 body first
hbMenu1: this->hud->initHudMenu(1, this->level);   // 0x200 body second
```

The CFO dump then shows the shared tail (bb.463) inserted after the 0x400 body
(bb.462), with the 0x200 body (bb.461) branching to it — i.e. CFO chose the
0x400 body. But the MBP dump still places the 0x200 body (bb.356) immediately
before the shared tail (bb.364) and the 0x400 body (bb.363) elsewhere. The
final `.so` therefore still has `movs r1,#1; blx` at `0x17adc4` and
`movs r1,#2; b 0x17adca` at `0x17cb24`.

**Conclusion:** to move the shared `blx` to the 0x400 body, the 0x400 body must
be earlier than the 0x200 body in the block list at **MBP** time, not just at
CFO time. Since the block list is the IR order and the dispatch chain emits the
0x200 test/body first, this requires the 0x400 case to be first in the source —
which changes the emitted `lsls` test order.

## 3. The swap shape flips both stages but breaks the test order

Swapping the 0x200 and 0x400 cases in the source (`if (flags & 0x400)` first)
puts the 0x400 body first in the IR. Both CFO and MBP then pick it: the final
`.so` has `movs r1,#2; blx; b.n` at `0x17adc4` and `movs r1,#1; b 0x17adca` at
`0x17afaa` — exactly the original's ownership. But the emitted dispatch is now
`tst #1024` (0x400), `lsls #20` (0x800), `lsls #22` (0x200), whereas the
original is `tst #512` (0x200), `lsls #20` (0x800), `lsls #21` (0x400). Score
81.9, `.text` 85,247, size 8080, `.rel.plt` 0 — a near-tie with the base.

## 4. Variants measured this wave (all reverted; base = 82.0 / 8080 / 85,244 / PLT 0)

| variant | pct | size | `.text` | `.rel.plt` | `.ARM.exidx` |
|---|---:|---:|---:|---:|---:|
| base | **82.0** | 8080 | 85,244 | 0 | 0 |
| swap 0x200/0x400 case order | 81.9 | 8080 | 85,247 | 0 | 0 |
| swap + separate `if (flags == 0)` | 79.0 | 8076 | 104,896 | 0 | 24 |
| swap + bare `hasFailed()` | 72.6 | 8088 | 209,911 | 6 | 3,586 |
| `__builtin_expect(hudMenuOpen, 1)` | 74.4 | 8068 | 163,031 | 26 | 334 |
| `__builtin_expect(hudMenuOpen, 0)` | 73.2 | 8040 | 213,125 | 22 | 3,552 |
| `__builtin_expect(flags == 0, 0)` | 78.2 | 8076 | 106,246 | 0 | 24 |
| `__builtin_expect(flags == 0, 1)` | 82.0 | 8080 | 85,244 | 0 | 0 |
| local `int flags = hudTouchFlags` | 82.0 | 8080 | 85,244 | 0 | 0 |
| `hudMenuOpen != 0` / `== 1` | 82.0 | 8080 | 85,244 | 0 | 0 |
| case order 0x200,0x400,0x800 | 79.1 | 8076 | 104,739 | 2 | 24 |
| case order 0x800,0x200,0x400 | 78.0 | 8064 | 163,887 | 4 | 346 |
| 0x400 case moved to end of chain | 77.5 | 8080 | 85,563 | 27 | 0 |
| goto hbMenu1→hbMenu2 after wingman | 79.1 | 8076 | 104,893 | 0 | 24 |
| goto hbMenu2→hbMenu1 after wingman | 79.1 | 8076 | 104,893 | 0 | 24 |
| goto bodies before dispatch | 82.0 | 8080 | 85,252 | 0 | 0 |
| goto bodies before weaponSlot | 82.0 | 8080 | 85,244 | 0 | 0 |

The last two are byte-identical to the base (the compiler folds the `goto`
labels back into the if/else order); the "bodies before dispatch" variant is
8 bytes worse in `.text` and still leaves the 0x200 body owning the `blx`.

## 5. Remaining attack

1. Find a source shape where the **0x400 body is earlier than the 0x200 body in
   the block list at MBP time** *and* the dispatch still emits
   `tst #512` (0x200), `lsls #20` (0x800), `lsls #21` (0x400). The swap shape
   proves the ownership flip is worth roughly the same as the test-order break;
   fixing both should be a large win.
2. The `hudTouchFlags == 0` test is the join block. Its layout predecessor in
   the IR is the wingman-body tail. If the 0x400 body could be emitted between
   the wingman body and the test (e.g. a real label that clang does not fold),
   `PredBB` would be the 0x400 body and CFO would pick it directly.
3. The `separate if` shape gives the original `ldr; cbz` test polarity at
   8076 B. Adding the two missing Thumb16 instructions (see w7713 §4: the
   `campaign` temp removes a `getCampaignMission` call, the merged 96/127
   removes an `isCampaignMission` call, and the mission-selection `mov r1,r0`
   is missing) would make it 8080 B; if the tail ownership can then be flipped
   without re-laying-out the function, this is the most promising route.

## 6. Reproduction

```
cp /dev/shm/w7734/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7734/fastscore.py        # 82.0 / 8080 / 2775:2774
python3 /dev/shm/w7734/secdiff.py          # .text 85,244 / .rel.plt 0
```

CFO/MBP MIR dumps (real NDK clang, filtered to OnTouchEnd):

```
/dev/shm/w7734/pa_filtered.sh              # -mllvm -print-after-all
                                           # -mllvm -filter-print-funcs=_ZN5MGame10OnTouchEndEiiPv
grep -n "IR Dump After Control Flow Optimizer" /dev/shm/w7734/pa_filt.log
```

The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`; the
original 0x200/0x400 bodies are at `0x17adc0` (`ldrd; movs r1,#1; b 0x17afde`)
and `0x17afd8` (`ldrd; movs r1,#2; fallthrough 0x17afde`).
