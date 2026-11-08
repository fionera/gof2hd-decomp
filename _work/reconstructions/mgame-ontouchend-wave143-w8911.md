# MGame::OnTouchEnd — wave 143 (w8911): hunk classification + survivor/branch-width probes

Status: analysis-only (source unchanged from the wave-143 flat shape), 2026-11-08.
Worktree `/home/debian/GalaxyOnFire2/wt/w8911`, base `f5093474f` (wave 143 docs).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w8911`.
Full `check` green: byte **4507**, linked **4507**, size **4524/4524**, data **843/843**,
imports **207**, lint **CLEAN**, drift 0, RATCHET PASS.

Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.
Baseline reproduced exactly: **raw same-address diff 4399, size 8080/8080**.

## 1. Classification of the remaining 4399 diff bytes

Instruction-level alignment (normalised mnemonics, branch targets erased) gives the
per-region address delta `orig_addr - ours_addr` (dominant value per 0x100 window):

| orig offsets | delta | what |
|---|---:|---|
| `0x0000..0x0bff` | **0** | pause/menu/cutscene/star-map hot path — byte-identical |
| `0x0c00` | **+12** | `hudMenuOpen` chain tail + cameraMode switch + `initHudMenu(1)` |
| `0x0d00` | **+14** | `dockToStream` / `setDirection` / `setPosition` block (the "0x17ae44 window") |
| `0x0e00` | +138 | failed-mission block moved |
| `0x0f00..0x1000` | +80 | campaign chain |
| `0x1100` | +26 | campaign chain |
| `0x1200..0x1300` | +164 | campaign chain |
| `0x1500` | -148 | campaign chain |
| `0x1600..0x1700` | -530/-528 | campaign chain |
| `0x1800` | -232/-442/+72 | campaign chain |
| `0x1900..0x1a00` | +72/+80 | campaign chain |
| `0x1b00` | +56 | campaign chain tail |
| `0x1c00..0x1d00` | +2/0/-2/+28 | join test / hud flags / epilogue |
| `0x1e00..0x1f00` | 0 | stack check / literal pool |

So ~1000 B of the 4399 are the single `+12` shift of the whole `0x0c00..0x0d00`
window (the dockToStream block is byte-identical but displaced); the remaining
~3400 B are the campaign-chain block moves (`+138/+80/+164/-148/-530/...`).

## 2. Exact `+12` ledger at `0x17ae44`

`0x17ae44` is `add r2, sp, #152` in the original; ours has it at `0x17ae50`.
The shift is accumulated from four branch-width deltas before the block:

| site (orig) | orig | ours | Δ |
|---|---|---|---:|
| `0x17ada6` `b.n 17afe2` (join test) | 2 B | `b.w 17b93c` | +2 |
| `0x17adae` `beq.n 17aeac` (maneuver) | 2 B | `beq.w 17aeba` | +2 |
| `0x17adbe` `b.n 17afe8` (hud flags body) | 2 B | `b.w 17b942` | +2 |
| `0x17adc0` `initHudMenu(1)` block | 8 B (`ldrd;movs;b.n`) | 14 B (`ldrd;movs;blx;b.w`) | +6 |

All four are pure distance problems: the original's join-test/epilogue group sits at
`0x17afe2..0x17b01a`, ours at `0x17b93c..0x17b974` (after the whole campaign chain).

## 3. The survivor mechanism (new detail)

The `initHudMenu(1)`/`initHudMenu(2)` shared `blx` is **not** BranchFolding before
placement. `-mllvm -print-after-all` shows four separate calls through Tail
Duplication; the merge appears only after **Branch Probability Basic Block
Placement**, which runs a post-placement `BranchFolder` (`TailMergeSize =
TailDupSize+1 = 2` under `-Oz`). It splits the common tail (`blx initHudMenu`) into a
new block (`bb.354 %ir-block.2569`) with `CreateCommonTailOnlyBlock`.

`CreateCommonTailOnlyBlock` picks the block with the smallest `EstimateRuntime` of
the prefix before the shared tail (`ldrd`=2 + `movs`=1 = 3 for both), tie-broken by
`t <= TimeEstimate` -> **last in `SameTails` order** -> the earliest predecessor in
the join-test predecessor list. Both `initHudMenu` blocks have identical prefixes, so
the survivor is decided purely by predecessor order, not by source size.

In the original the join test's fall-through predecessor (`PredBB`) is the
`initHudMenu(2)` block, so `BranchFolder` splits it and the shared `blx` lands at
`0x17afde` (init1 becomes an 8-B `ldrd;movs;b.n`). In ours `PredBB` is the
weapon-slot block (which falls through the join test), so the tie-break splits
`initHudMenu(1)` and the shared `blx` lands at `0x17adcc`.

## 4. Variants measured (all 8080 B unless noted; base 4399)

| variant | size | raw | note |
|---|---:|---:|---|
| flat (landed) | 8080 | **4399** | baseline |
| swap `0x200`/`0x400` test order | 8080 | 4402 | flips survivor to init2, but common tail still after first branch block; `0x17ae44` still +12 |
| `0x200,0x400,0x800` test order | 8060 | 6078 | survivor init2, `0x17ae44` becomes **-4**, but -20 B and tail re-lays |
| `0x200,0x400,0x800` on nested | 8080 | 4467 | |
| nested (join test inside weapon-slot `else`) | 8080 | 4464 | correct early placement, wrong `cmp;bne` polarity; `0x17ae44` +8 |
| `__builtin_expect(flags==0,0)` nested | 8076 | 6058 | re-lays whole function |
| `__builtin_expect(flags==0,1)` / `!=0,1` nested | 8080 | 4464 | byte-identical to nested |
| `__builtin_expect(flags==0,0)` flat | 8088 | 4767 | |
| `__builtin_expect(flags!=0,1)` flat | 8072 | 4724 | |
| `__builtin_expect(flags&0x400,1)` nested | 8020 | 6044 | |
| `__builtin_expect(flags&0x800,0)` nested | 8072 | 4485 | |
| `int menuId` single call site | 8076 | 4704 | |
| `Level *level = this->level` cache | 8084 | 4802 | |
| nested-if around `0x200` branch | 8080 | 4399 | byte-identical |
| `else { if (wingman) ... else if (flags==0) }` | 8080 | 4734 | |
| `weaponSlot > -1` / `wingmanCommand > -1` | 8080 | 4399 | identical |
| `!flags` / `flags == false` | 8080 | 4399 | identical |
| `hudMenuOpen == 1` / `!= 0` | 8080 | 4399 | identical |
| flags-body statement order swaps | 8080 | 4400 | 1 B worse |
| `goingToWaypoint() == 1` (w8809 METRIC-ONLY) | 8084 | 4715 | flat tail already aligned differently |
| bare `hasWon()` (w8809 shape) | 8068 | 6023 | flat tail not the base tail |
| `amount < 0` / `failed == true` | 8080 | 4399 | identical on flat |

## 5. Conclusion / next lead

The `+12` window is **not** a survivor-choice bug that can be fixed in isolation: the
survivor is downstream of the join-test placement. The original places the
join-test/epilogue group *before* the failed block and campaign chain; ours places it
*after* the whole chain. Flipping the survivor (swap `0x200`/`0x400`) does not move
the common tail because the tail is split from the first branch block, not from the
`PredBB`.

The only shape that moved `0x17ae44` to a smaller delta was the `0x200,0x400,0x800`
test reorder (`-4`), but it costs 20 B and re-lays the tail (raw 6078). A future pass
should look for a **faithful** way to make the join-test block the layout successor
of `initHudMenu(2)` — i.e. move the join-test group ahead of the `failed` block —
without changing the `0x200,0x800,0x400` test order. That is an MBP chain/frequency
problem (same family as the w8809 cargo-block placement), not a statement-order one.

## 6. Reproduction

```
cp /dev/shm/w8911/MGame.flat.cpp src/game/menu/MGame.cpp   # pristine flat
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8911/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4399 / 8080
python3 /dev/shm/w8911/delta.py _ZN5MGame10OnTouchEndEiiPv   # per-region delta table
python3 /dev/shm/w8911/hunks.py _ZN5MGame10OnTouchEndEiiPv   # 234 raw hunks
```

Scratch: `/dev/shm/w8911/{fdiff.py,hunks.py,delta.py,align.py,check_survivor.py,
batch.py,MGame.flat.cpp,MGame.nested.cpp,pa.log,BranchFolding.cpp,
MachineBlockPlacement.cpp,TailDuplicator.cpp,orig.dis,ours.dis,nested.dis}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
