# MGame::OnTouchEnd — wave 144 (w8910): cargo deferral via the `failed` branch weight

Status: analysis-only, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w8910`,
base `f5093474f` (wave 143). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w8910`. **No source change landed** (base restored; full `check`
green: byte 4507, linked 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN,
RATCHET PASS). Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

Baseline re-measured: **8080/8080, raw same-address diff 4399**.

## 1. Corrected frequency analysis (supersedes wave-143 note)

The wave-143 "cargo 0.00030 vs campaign entry 0.00028" margin is **wrong**.  Read
the real MachineBlockPlacement MIR (`/dev/shm/w8910/ote.bpp.txt`) and propagate
the successor probabilities:

```
bb.179  won first hasWon       F179
bb.180  won second hasWon      F179*0.375
bb.181  keepObjectives hasWon  F179*0.8125
bb.205  mission-selection hW   F179*0.8125
bb.208  isInstantActionMission F179*0.8125
bb.211  isCampaignMission      F179*0.3047   (0.375 false edge of bb.208)
bb.229  freelance              F179*0.1523 = 0.000221
bb.218  cargo                  bb.198*0.5  = 0.000302
bb.230  campaign entry         bb.211*0.5 + bb.212 = 0.000441
```

At the `selectBestCandidateBlock()` dead end after bb.217 the worklist holds
**cargo (0.000302)** and **freelance (0.000221)**; bb.230 is still blocked by its
unscheduled predecessor bb.229.  Cargo wins, so it is placed before the campaign
chain.  **The lever is bb.229's frequency, not bb.230's.**  Raise bb.229 above
0.000302 and the order becomes freelance -> campaign entry -> campaign chain ->
... -> cargo, which is the original's order (freelance `0x17b0b6`, campaign
entry `0x17b0e2`, cargo `0x17b374`).

bb.229 = F179 * 0.8125 * P(208->211) * 0.5.  Need P(208->211) > 0.5125.

## 2. Why `__builtin_expect` is not needed: the zero heuristic

`BranchProbabilityInfo::calcZeroHeuristics` (LLVM 7.0.1, fetched to
`/dev/shm/w8910/bpi_src.cpp`) gives `ZH_TAKEN_WEIGHT=20`, `ZH_NONTAKEN_WEIGHT=12`
(20/32 = 62.5 %, 12/32 = 37.5 %) for `icmp eq/ne X, 0`.  `isInstantActionMission()`
returns `i8`, so `if (isInstantActionMission())` is `icmp ne i8 %x, 0` and the
campaign/freelance edge (X==0) is **37.5 %**.  A comparison to a **non-zero**
constant (`== 1`, `!= 1`, `== 2`, `> 1`) is *not* handled by the zero heuristic
and falls back to **50 %** — just short of the 51.25 % needed (cargo 0.000302 vs
bb.229 0.000295).  `X < 1` is canonicalised to the `isOne() && SLT` case and
stays 37.5 %.

## 3. Measured variants (all reverted; base restored)

| variant | size | raw | cargo addr | note |
|---|---:|---:|---|---|
| base | 8080 | **4399** | `0x17b084` | reference |
| `inst_eq1` (`isInstantActionMission() == 1`) | 8080 | 4689 | `0x17b0b8` | 50 % — just short |
| `inst_ne1` / `inst_eq2` / `inst_gt1` | 8080 | 4688-4690 | `0x17b0b8` | same 50 % |
| `inst_lt1` (`< 1`) | 8076 | 4756 | `0x17b33a` | **deferred** but -4 B |
| `inst_expect0` | 8076 | 6054 | `0x17b436` | deferred, whole fn re-laid |
| `won2 bare` | 8068 | 6023 | `0x17b110` | -12 B, re-lays first half |
| `bare + inst_eq1` | 8080 | 6035 | `0x17b244` | **deferred**, first half re-laid |
| **`swap_int`** | **8080** | **4691** | **`0x17b34a`** | **deferred**, first half exact |
| `swap_int + inst_eq1` | 8068 | 5984 | `0x17b458` | deferred |
| `swap_int + won2 bare` | 8068 | 5888 | `0x17b3a2` | t12/cargo/cmp65 within 4 B of orig |

`swap_int` is the only 8080-B variant that defers cargo **and** keeps the first
half (0x17a144..0x17ad00) byte-exact.  It is a semantically-equivalent reshape of
the `if (failed)` test:

```cpp
-                bool failed = Globals::status->getMission()->hasFailed();
-                if (failed) { /* failed body */ } else { /* won body */ }
+                int failed = Globals::status->getMission()->hasFailed();
+                if (failed != 1) { /* won body */ } else { /* failed body */ }
```

`int` + `!= 1` avoids the zero heuristic (50 % won instead of 37.5 %), raising
bb.229 by 33 % (0.000221 -> 0.000295... but combined with the `!failed` true
edge it lands above cargo).  Cargo moves to `0x17b34a`, i.e. **after** the
`==15` test (`0x17b0ee`) and before `==65` (`0x17b3c8`) — the original's order.
Raw diff 4691 (+292 vs base): the campaign region is unchanged (~240/window,
because the type12 block is still misplaced at `0x17b03a` vs orig `0x17b096`),
and the **tail loses the wave-143 join-test win** (`0x17bf00` 36 -> 242,
`0x17c000` 17 -> 107).  The join test itself is still `cbz` at `0x17b93a`
(2 B earlier than base `0x17b93c`), so the tail loss is a downstream shift, not
a polarity regression.

**`swap_int` is a LAYOUT reshape, not the witnessed source.**  The original at
`0x17ab38` is `beq.w 17adc8` (won branch is the `else` of `if (failed)`), which
matches the base's `bool failed; if (failed)`.  The coordinator may prefer the
faithful base form at 4399.

## 4. Open questions

1. The type12 block is the other big misplacement: base `0x17af8a`, original
   `0x17b096`.  Fixing cargo alone cannot fix the ~240/window campaign diff
   because the type12 chain is placed before the mission-selection chain in the
   base and after it in the original.  A source shape that flips the type12
   chain behind bb.205 is the next lever.
2. The tail loss of `swap_int` (`0x17bf00` +206) needs a compensating shape; the
   wave-143 flat `hudTouchFlags == 0` shape is already in the base and is not
   preserved by the `int failed` reshape.

## 5. Reproduction

```
cp /dev/shm/w8910/MGame.base.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8910/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4399 / 8080
python3 /dev/shm/w8910/batch5.py    # == 0/1/2, >0/1, <1 sweep
python3 /dev/shm/w8910/batch6.py    # won2 bare + inst variants
python3 /dev/shm/w8910/batch8.py    # failed-branch swap sweep
```

Scratch: `/dev/shm/w8910/{fdiff.py,windows.py,batch*.py,swap.py,ote.bpp.txt,
ote.irfull.txt,bpi_src.cpp,MGame.base.cpp}`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
