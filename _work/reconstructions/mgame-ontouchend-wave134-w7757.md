# MGame::OnTouchEnd — wave 134 (w7757): PredBB / branch-probability levers

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7757`,
base `ec0acea1e` (pristine source restored; **no source change landed**). Scratch
`/dev/shm/w7757`. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

Baseline (verified this wave, full `check`):

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS          lint CLEAN   drift 0
.text 85,143   .ARM.exidx 0   .rel.plt 0   .ARM.extab 1,416   .note.gnu.build-id 20
raw fuzzy (difflib, autojunk off): 81.9607 %   size 8080/8080   insns 2775/2774
```

## 1. Where the ownership is actually decided

Two independent stages matter, and wave 133 only described the first.

**(a) BranchFolder (CFO) survivor choice.** `BranchFolder::CreateCommonTailOnlyBlock`
(release_70 `BranchFolding.cpp:784`) picks the survivor of the two `initHudMenu`
bodies.  `PredBB` is `&*std::prev(I)` where `I` is the join block
(`BranchFolding.cpp:1123`), i.e. **the block immediately before the join block in
the machine-function list**, not a CFG predecessor.

In the PEI dump (base):

* join block = `bb.463` (`hudTouchFlags == 0` test, `%ir-block.2567`);
* `PredBB = prev(bb.463) = bb.462` = the **wingmanCommand body tail**
  (`%ir-block.2558`, `hudMenuOpen=0; closeHudMenu; resumeSounds; b bb.471`);
* `SameTails = {bb.426 (0x200 body, %ir-block.2426), bb.430 (0x400 body, %ir-block.2448)}`
  in that order (higher block number first, `ComputeSameTails` pushes
  `CurMPIter` then `I`);
* both bodies are `ldrd r0,r2,[fp,#116]; movs r1,#N` → `EstimateRuntime` = 2+1 = 3
  for both, so the `t <= TimeEstimate` tie-break keeps the **last** entry =
  `bb.426` = the **0x200 body**.

CFO output confirms it: `bb.404` (0x200 body) is split and the shared-tail block
`bb.405` is inserted right after it; `bb.435` (0x400 body) branches to `bb.405`.
So the CFO survivor is the 0x200 body.

**(b) MachineBlockPlacement re-layout.** MBP walks the post-CFO block list
`bb.402 (hudMenuOpen test) -> bb.403 (0x200 test) -> bb.404 (0x200 body) ->
bb.405 (shared blx) -> bb.406 (join test) -> bb.407 (flags==0 body) -> ... ->
bb.432 (0x800 test) -> bb.433 (0x800 body) -> bb.434 (0x400 test) ->
bb.435 (0x400 body)`.

* `bb.356` (0x200 body) → `bb.357` (shared blx): `hasBetterLayoutPredecessor`
  returns **false**, so it chains.
* `bb.357` (shared blx) → `bb.359` (join test): returns **true** (join test has
  other unplaced preds), so the chain stops.
* `bb.358` (0x800 body) → `bb.359` (join test): chains. That is why our join
  test sits after the 0x800 body (`0x17aeb4`) and the `hudTouchFlags==0` body is
  hot, whereas the original has the join test after the shared blx
  (`0x17afe2`) and the `hudTouchFlags==0` body cold at `0x17b01a`.

`hasBetterLayoutPredecessor` (`MBP.cpp:1276`) with no profile data uses
`HotProb = StaticLikelyProb = 80%` and rejects `BB→Succ` when any other
unplaced predecessor `Pred` satisfies

```
freq(Pred) * 0.8 >= freq(BB) * 0.2        (i.e. freq(Pred) >= freq(BB)/4)
```

With the 50/50 dispatch chain, `freq(0x400 body) = freq(0x200 body)/4` exactly.
The comparison is on integer `BlockFrequency` values (floating-point propagation
then `convertFloatingToInteger` scales so the minimum block is 8), so the
`>=` is decided by rounding: at exactly 1/4 the 0x200 body still wins.

## 2. The lever that *does* flip the ownership: branch probability

Biasing the **0x800 test** to be unlikely-taken with
`__builtin_expect((this->hudTouchFlags & 0x800) != 0, 0)` raises
`freq(0x400 body)/freq(0x200 body)` above 1/4. `hasBetterLayoutPredecessor`
then returns true for `0x200 body → shared blx`, the 0x200 body is not chained,
and the 0x400 body becomes the layout predecessor of the shared tail:

```
17adc2: ldrd r0, r2, [fp, #116]      ; 0x200 body
17adc6: movs r1, #1
17adc8: b.w  17ae96                  ; branches forward to the shared blx
...
17ae90: ldrd r0, r2, [fp, #116]      ; 0x400 body
17ae94: movs r1, #2
17ae96: blx  Hud::initHudMenu        ; shared tail
17ae9a: ldr.w r0, [fp, #248]         ; join test, falls through
17ae9e: cmp  r0, #0
17aea0: bne.w ...
```

This is **exactly the original's tail structure** (`0x17afd8` 0x400 body →
`0x17afde` shared `blx` → `0x17afe2` join test, all fall-through).  The dispatch
test order also stays `tst #512` (0x200), `lsls #20` (0x800), `lsls #21` (0x400).

The catch: the same bias makes the 0x800 body cold, so MBP moves it to
`0x17bf46` (far end) and the overall fuzzy drops to **73.92 %** (size 8076,
`.ARM.exidx` 24).  The original keeps the 0x800 body at `0x17ae8c` (hot, right
after the campaign chain).  So the branch-probability lever flips the tail but
simultaneously destroys the 0x800 body placement.

`__builtin_expect((hudTouchFlags & 0x200) != 0, 0)` flips the ownership the same
way (0x400 body at `0x17aeaa` owns the `blx`, join test falls through) but makes
the **0x200 body** cold (`0x17bf46`); 77.33 %, size 8076.

## 3. Variants measured this wave (base = 81.9607 / 8080 / 85,143 / exidx 0)

| variant | fuzzy % | size | `.text` | `.ARM.exidx` |
|---|---:|---:|---:|---:|
| base (pristine) | **81.9607** | 8080 | **85,143** | 0 |
| `goto afterHudMenu` after 0x200 body + `hbMenu2:` after chain | 81.9755 | 8080 | 85,151 | 0 |
| swap 0x200/0x400 cases | 81.8526 | 8080 | 85,146 | 0 |
| separate `if (hudTouchFlags == 0)` instead of `else if` | 79.0916 | 8076 | 104,792 | 24 |
| `__builtin_expect((flags & 0x200) != 0, 0)` | 77.3252 | 8076 | 104,706 | 24 |
| `goto hbMenu2` placed before join test (goto from weaponSlot+wingman bodies) | 77.1202 | 8060 | 163,877 | 346 |
| `hudTouchFlags == 0` test moved to front of the weaponSlot chain | 75.2750 | 8052 | 163,354 | 352 |
| `__builtin_expect((flags & 0x800) != 0, 0)` | 73.9232 | 8076 | 104,730 | 24 |
| swap 0x800/0x400 + `__builtin_expect(0x800,0)` | 73.8151 | 8076 | 104,741 | 24 |
| `__builtin_expect((flags & 0x400) != 0, 1)` | 73.6823 | 8016 | 214,304 | 3,562 |

The `goto afterHudMenu` variant is the only one that keeps size 8080 / exidx 0
and is marginally better on the instruction-level fuzzy, but it is 8 bytes worse
on raw `.text`; it is a wash and was not kept.

## 4. Why the rest of the function is still far off

The tail merge is only one of many layout differences.  The diff still has
~1795 structural lines, including large block reorderings driven by literal-pool
placement:

* `orig[1257:1306]` (49 instructions) is a 6-word literal pool
  (`Globals::appManager`, `Globals::status`, `0x45fa0000`, `Globals::sound`,
  `Level::doInstantJump`, `Globals::switch_to_target_setting`) that the original
  emits immediately before the 0x400 body at `0x17afc0`; ours emits the pool
  elsewhere because the 0x400 body is placed differently.
* `orig[1898:1979]` (81 instructions) is the mission-96 `departStation` block;
  it is reordered in ours.
* `orig[1441:1462]`, `orig[1727:1772]`, `orig[2398:2417]` are further
  block reorderings.

So even a perfect tail-merge flip will not make the function byte-exact until
the block/literal-pool order of the whole `pauseOpen` / `cutsceneActive` region
matches.  The branch-probability lever above is the first source shape found
that reproduces the original's `0x400 body -> shared blx -> join test`
fall-through chain while keeping the 0x200/0x800/0x400 dispatch order.

## 5. Reproduction

```
cp /dev/shm/w7757/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7757/rawscore.py      # 81.9607 / 8080 / 2775:2774
python3 /dev/shm/w7757/secdiff.py       # .text 85,143 / .rel.plt 0 / exidx 0
```

MIR dumps (real NDK clang 7.0.2, `-mllvm -print-after-all
-mllvm -filter-print-funcs=_ZN5MGame10OnTouchEndEiiPv`):

```
/dev/shm/w7757/mir.sh
# CFO  = pa.log line 681235 -> cfo_base.mir
# MBP  = pa.log line 745922 -> mbp_base.mir
```

Key MIR facts: PEI `bb.463` join, `PredBB = bb.462` (wingman tail),
`SameTails = {bb.426, bb.430}`; CFO survivor `bb.426` (0x200 body), shared tail
`bb.405`; MBP `bb.356 -> bb.357` chains, `bb.357 -/-> bb.359`,
`bb.358 -> bb.359` chains.

Original authoritative binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(0x200 body `0x17adc0`, 0x400 body `0x17afd8`, shared `blx` `0x17afde`,
join test `0x17afe2`, cold `hudTouchFlags==0` body `0x17b01a`).
