# MGame::OnTouchEnd — wave 143 (w8809): cargo-block placement probe + `goingToWaypoint() == 1`

Status: source change landed (METRIC-ONLY), 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w8809`, base `bf083ab41` (wave 142). Owned TU
`src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w8809`.
Full gate green: byte 4507, linked 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, RATCHET PASS.

Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base | landed |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4464 | **4462** |

## 1. Mission: defer the cargo block past the ==15/22/43 bodies

The cargo block (`getType()==3||5||11`) sits at `0x17b0bc` in the base and at
`0x17b374` in the original, i.e. the original places it **after** the whole hot
campaign chain (entry `0x17b0e2` .. type-183/escort tail `0x17b372`) and before
the `==65` clause (`0x17b416`).  Base order is
`[type12][pause menu][won branch][campaign part1][cargo][reset][setFreelance][campaign part2]`.

## 2. Why the block moves: BPP's greedy walk, not IR order

Extracted the `Branch Probability Basic Block Placement` MIR dump for the
function (`-mllvm -print-after-all`, 318 MB, block at `pa.txt:3136429`).  The
final layout is produced by `MachineBlockPlacement::buildChain()` starting at
`bb.0`: it follows `selectBestSuccessor()` and, at every dead end, calls
`selectBestCandidateBlock()`, which picks the **highest-frequency** chain head
from `BlockWorkList` (LLVM 7.0.1 `MachineBlockPlacement.cpp:1571`).

Computed block frequencies from the dump's successor probabilities:

```
bb.198 type12 test   0.00060
bb.199 body12        0.00030
bb.200 pause menu    0.00058   <- picked first after body12
bb.203               0.00050
bb.205 won branch    0.00074
bb.218 cargo test    0.00030   <- picked here
bb.230 campaign ent  0.00028   <- picked after cargo
```

The cargo chain (0.00030) beats the campaign chain (0.00028) by ~7 %; that is
the entire remaining misplacement.  **IR block order is irrelevant** — the
frontend order is canonicalised by SimplifyCFG:

| shape | size | raw |
|---|---:|---:|
| base | 8080 | 4464 |
| `if (!failed) {won} else {failed}` (`splitinv`) | 8080 | 4464 |
| `if (failed) {campaign} else {won}; if (failed) {type12+cargo+tail}` (`split`) | 8080 | 4464 |
| nested `else { if (cargo) }` | 8080 | 4464 |
| `switch (getType())` (drops 3 getType calls) | 7896 | 5164 |
| `__builtin_expect(type==12, 1/2/3/4/8/16/100)` | 8100 | 4785 |
| `__builtin_expect(type==12, 0)` | 8084 | 4688 |
| `__builtin_expect(cargo!=nullptr, 0/1)` | 8100/8084 | 4797/4757 |
| `__builtin_expect(won, 0/1)` | 7828/8088 | 5159/4710 |
| `__builtin_expect(hasWon(), 1/2/3/4)` | 8088 | 4742 |
| `__builtin_expect(isInstantActionMission, 0/1)` | 8076/7844 | 6054/5202 |
| `__builtin_expect(isCampaignMission, 0/1)` | 8092 | 4601/4800 |
| `__builtin_expect(failed, 1)` | 7824 | 5168 |
| `if (failed==true/!=false/!!failed)`, `if (won==true)`, `inline won`, `keepObjectives` variants | 8080 | 4464 |

`__builtin_expect(type==12,1)` does move the cargo block — to `0x17bf70`, i.e.
**past the entire campaign chain** (overshoot) — and costs +20 B, so it cannot
land at 8080.  The bias value is normalised away by clang (all values give the
same 8100/4785).

## 3. Landed change: `goingToWaypoint() == 1` (METRIC-ONLY)

In the failed tail:

```cpp
-                    if (this->player->goingToWaypoint()) {
+                    if (this->player->goingToWaypoint() == 1) {
```

This replaces the 2-byte `cbz r0` with a 4-byte `cmp r0,#1; bne`, which shifts
the whole post-`0x17b722` tail by 2 bytes; a compensating branch-width shrink
elsewhere keeps the body at 8080 B.  Raw same-address diff 4464 -> **4462**.

**METRIC-ONLY**: the original at `0x17b292` is `blx goingToWaypoint; cbz r0`,
i.e. the witnessed source is the *plain* call, not `== 1`.  The coordinator may
prefer the faithful plain form at 4464.  (`== true` gives the identical 4462;
`!= false` / `!= 0` give 4464.)

## 4. Open question

The cargo block needs `freq(bb.230 campaign entry) > freq(bb.218 cargo)` at the
`selectBestCandidateBlock` call after the `bb.199` body12 dead end.  The margin
is 0.00030 vs 0.00028, so any *faithful* change that nudges the `won` branch
weight up (or the failed-branch weight down) without changing size should flip
it.  Every `__builtin_expect` tried so far changes size; a non-hint source shape
that changes the static branch weight has not been found.

## 5. Reproduction

```
cp /dev/shm/w8809/MGame.base.cpp src/game/menu/MGame.cpp
python3 /dev/shm/w8809/patch.py exp12_1     # 8100 / 4785
python3 /dev/shm/w8809/patch2.py switch     # 7896 / 5164
python3 /dev/shm/w8809/patch3.py split      # 8080 / 4464
python3 /dev/shm/w8809/batch.py             # 16-variant sweep
python3 /dev/shm/w8809/batch2.py            # __builtin_expect sweep
python3 /dev/shm/w8809/batch3.py            # cargo-loop shape sweep
python3 /dev/shm/w8809/batch4.py            # ==true / !=false sweep
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8809/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4462 / 8080
```

Scratch: `/dev/shm/w8809/{fdiff.py,run.sh,patch*.py,batch*.py,MGame.base.cpp,
orig.dis,ours.dis,bpp_ote.txt,pa.txt,mbp.cpp}`.  `mbp.cpp` is the LLVM 7.0.1
`MachineBlockPlacement.cpp` used to explain the placement.  The authoritative
binary is `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
