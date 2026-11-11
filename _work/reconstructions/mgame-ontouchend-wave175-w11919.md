# MGame::OnTouchEnd — wave 175 (w11919): the `bool hasWon` lever moves the epilogue

Status: analysis only, no code win landed, 2026-11-11. Worktree
`/home/debian/GalaxyOnFire2/wt/w11919`, branch `wave175-w11919`, base `5f599a137`.
Target `_ZN5MGame10OnTouchEndEiiPv`, original 8,080 B @ `0x17a144`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w11919`.

Entry/exit state (src left at base): raw **3,771**, aligned **1,453**, size
**8,080/8,080**, epilogue `popeq` `0x17ba32`, `.rel.plt` byte-identical (0),
gate byte_exact **4507**, linked_exact **4507**, size-exact 4524, stub/missing/
wrong_type/extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
**RATCHET PASS**.

## 1. Result summary

The epilogue **can** be moved to the original address `0x17b014`, and the whole
hot tail `0x17af5a..0x17b02e` then matches the original instruction for
instruction — but only via a `Mission::hasWon()` return-type change that makes
the function **8,092 B (+12)**, and a size change is gate-fatal (see §5).
No size-exact shape was found. `src/` is left at the base.

| shape | raw | aligned | size | epilogue | `.rel.plt` |
|---|---:|---:|---:|---:|---:|
| base (uchar `won`/`hasWon`) | **3,771** | **1,453** | 8,080 | `0x17ba32` | 0 |
| `bool won` + `bool hasWon` | 6,042 | 1,493 | 8,076 | `0x17aff8` | 0 |
| … + campaign-first dispatch (`C1`) | 3,907 | 1,352 | 8,092 | **`0x17b014`** | 0 |
| … + `C1` + RS4 store order | **3,896** | **1,350** | 8,092 | **`0x17b014`** | 0 |
| … + `C1` + original campaign-chain order | 4,025 | **804** | 8,092 | **`0x17b014`** | 0 |
| … + `C1` + `expect(hudTouchFlags==0,0)` | 4,216 | 1,400 | 8,100 | `0x17b018` | 0 |
| … + `C1` + `expect(hudMenuOpen,1)` | 6,047 | 1,700 | 8,084 | `0x17afa0` | **13** |

(The `C1` campaign-first form is wave-173's reorder: `if (isCampaignMission()) …
else if (!isInstantActionMission()) … else { …; return; }`.)

## 2. The lever: `Mission::won` / `Mission::hasWon()` must be `bool`

`Mission::hasWon()` currently returns `unsigned char` (`src/game/mission/Mission.h:125`),
so the `won` computation

```cpp
bool won = Globals::status->getMission()->hasWon() ||
           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon() == true;
```

lowers the second test to `icmp eq i8 %x, 1`. `BranchProbabilityInfo::calcZeroHeuristics`
does not handle `icmp eq X, 1` (only `0`, `1` with `slt`, `-1`), so that branch is
**50/50** and the code is `cmp r0,#1; bne.w` (ours `0x17ade2`). The original is
`cmp r0,#0; beq.w` (`0x17adda`).

Changing the field to `bool won;` and the method to `bool hasWon()` makes the call
return `i1`; the `||` becomes a branch on the i1 value, which emits `cmp r0,#0` and
is still 50/50 (no `icmp` heuristic applies). The callee body stays
`ldrb r0,[r0,#5]; bx lr` — **4 bytes, byte-exact** — as long as `won` is `bool`
(with `uint8_t won` the `uint8_t -> bool` conversion grows `hasWon` to **10 bytes**,
which shifts `.text` and costs ~1,433 byte-exact functions).

This one change re-lays the whole hot tail. Per-region same-address diff
(`0x17a144 + off`):

| region | base | `bool` + `C1` |
|---|---:|---:|
| `0x000..0xec0` (hot, up to the epilogue) | 667 | **120** |
| `0xec0..0x1f80` (cold, after the epilogue) | 3,104 | 3,768 |

The hot region becomes essentially exact — `0x17af5a..0x17b02e` is instruction-for-
instruction identical to the original apart from the deliberate campaign/instant
swap at `0x17af7e..0x17afbe`:

```
ours(C1) 17afd8 ldrd r0,r2,[fp,#116] ; movs r1,#2
         17afde blx  Hud::initHudMenu          <- shared tail, 0x400 arm falls through
         17afe2 ldr  r0,[fp,#248] ; cbz r0,17b01a  <- hudTouchFlags == 0 test
         17afe8 final isAutoPilot block
         17b004 epilogue ; 17b014 popeq
         17b01a hudTouchFlags == 0 body
```

i.e. exactly the layout wave-173/wave-174 said was unreachable: the `0x400` arm owns
the merged `blx`, the test falls through to the final block, the epilogue sits at
`0x17b014`, and the `hudTouchFlags == 0` body is demoted after it.

## 3. Why it cannot land yet: the +12 B

`bool` + `C1` is 8,092 B (+12). The extra bytes are **not** one insertion; they are
a net of cold-block deltas after the epilogue (difflib alignment of the two
instruction streams, `align2.py`):

| site | orig | ours | delta |
|---|---:|---:|---:|
| `0x17b0b2` | 12 B | 72 B | +60 |
| `0x17b13e` | 4 B | 60 B | +56 |
| `0x17b14a` | 2 B | 540 B | +538 |
| `0x17b1e2` | 402 B | 2 B | −400 |
| `0x17b3b4` | 44 B | 2 B | −42 |
| `0x17b41c` | 146 B | 2 B | −144 |
| `0x17b556` | 326 B | 4 B | −322 |
| `0x17b702` | 6 B | 314 B | +308 |
| `0x17b876` | 0 B | 160 B | +160 |
| `0x17ba2e` | 162 B | 2 B | −160 |
| `0x17bb3a` | 114 B | 0 B | −114 |
| `0x17bc1c` | 30 B | 134 B | +104 |
| `0x17bcbc` | 4 B | 136 B | +132 |
| `0x17bd06` | 2 B | 66 B | +64 |
| `0x17bffa` (literal island) | 0 B | 18 B | +18 |
| … | | | **net +12** |

The most concrete artefact is an **18-byte literal-pool island at `0x17bffa`** that
the original does not have (ours: `nop; bx r4; 4 words` between the landing pad
`b.n 17c00e` and the next pad; original falls straight through). It is emitted by
the ARM constant-island pass because the cold code moved far enough from the main
pool at `0x17c0a4`. The camp0 (`bool` only, 8,076 B) build has no island.

Sweeps that did **not** remove the +12 (all stay 8,092 B, epilogue `0x17b014`):

* all 24 permutations of the `weaponSlot >= 0` body (A-first);
* all 120 permutations of the `hudTouchFlags == 0` body with `closeHudMenu` first;
* all 5 campaign-dispatch orders (only two distinct layouts: campaign-first 8,092 /
  instant-first 8,076; the nested forms `n1`/`n2`/`n3` canonicalise to those);
* the original campaign mission-id chain order (`15,22,43,42,65,81,74,95,96,100,
  110,120,126,127,134,144,155,161,162`, with `96`/`127` split) — aligned **804**,
  size still 8,092;
* `count > 0` ore guard, `closeHudMenu`-first weaponSlot/wingman bodies, RS4 store
  order, `expect(hudTouchFlags==0,0)` (8,100, epilogue `0x17b018`),
  `expect(hudMenuOpen,1)` (8,084, epilogue `0x17afa0`, `.rel.plt` 13).

## 4. Why the size is sacred here

`tools/verify/verify.py` compares each function's own bytes (`be = ob == ub`), but
PC-relative literal pools encode `target - (pc + 4)`. Moving a function changes
those words, so **any size change in OnTouchEnd shifts every later function and
breaks its byte-exactness even when the instructions are identical**.

Measured with `bool won` + `bool hasWon` + the *base* MGame (OnTouchEnd 8,076 B):
`check` reports `byte_exact 4507 -> 4487` (linked_exact stays 4507, avg 99.93,
data 843/843, imports 207, lint CLEAN). The 20 functions are exactly the ones
after OnTouchEnd in `.text` and all have `pct 100.0` — pure address-shift:

```
_Z14ArraySetLengthIPN11AbyssEngine4MeshEEvjR5ArrayIT_E
_ZN11AMeshMerger6renderEv / D1Ev / D2Ev
_ZN16SimpleMeshMergerC1E… / C2E…
_ZN5MGame10OnRender2DEv / OnRender3DEv / OnTouchEndEiiPv / successCheckEv
_ZN5MGame12switchCameraEi / dialogueEventEv / gameOverCheckEv
_ZN5MGame14startJumpSceneEv / updateJumpSceneEv / setCinematicModeEb
_ZN5MGame8OnResumeEv / OnReleaseEv / OnSuspendEv / dockEventEii / nextCamIdEi
```

With `uint8_t won` (hasWon 10 B) the shift is larger and `byte_exact` collapses to
**3,074**.

So the correct next step is to make OnTouchEnd **exactly 8,080 B** with
`bool won` + `bool hasWon` + `C1`; the +12 must be found in the cold block order,
not compensated elsewhere.

## 5. Other measured shapes (base source, no `bool`)

| shape | raw | aligned | size | epilogue | relplt |
|---|---:|---:|---:|---:|---:|
| base | **3,771** | **1,453** | 8,080 | `0x17ba32` | 0 |
| `if (hudMenuOpen == 1 / != 0 / == true)` | 3,771 | 1,453 | 8,080 | `0x17ba32` | 0 |
| `switch` + explicit `default:` / if-else-if rewrite | 3,771 | 1,453 | 8,080 | `0x17ba32` | 0 |
| `!hudTouchFlags` / `<= 0` / `expect(==0,1)` | 3,771 | 1,453 | 8,080 | `0x17ba32` | 0 |
| `expect(hudMenuOpen,1)` | 5,962 | 1,807 | 8,068 | `0x17afa0` | 13 |
| `expect(hudMenuOpen,0)` | 6,250 | 1,798 | 8,048 | `0x17aefc` | 12 |
| `expect(hudTouchFlags==0,0)` | 5,918 | 1,602 | 8,076 | `0x17af90` | 0 |
| final `isAutoPilot` block moved before `if (hudMenuOpen)` | 6,272 | 1,960 | 8,020 | `0x17afba` | 0 |
| campaign-first (`C1`) alone | 6,070 | 2,292 | 8,084 | `0x17af92` | 0 |
| `won != false` / `won != 0` / bare `won` | 6,101 | 2,275 | 8,068 | `0x17af92` | 0 |
| 24 weaponSlot permutations | best 3,771 | 1,521 | 8,080 | `0x17ba32` | 0 |
| 24 wingman permutations | base best 3,771 | 1,453 | 8,080 | `0x17ba32` | 0 |

Note: `won != false` / `won != 0` / bare `won` change the branch weight from the
50/50 `icmp eq i8,1` to `calcZeroHeuristics`' 62.5/37.5 `icmp ne i8,0`, re-laying
the whole tail (raw 6,101, size 8,068) — the `== true` spelling is load-bearing
while `hasWon` returns `unsigned char`.

The `weaponSlot` closeHudMenu-first order (wave-174's `RS4 + DACB`) re-measures at
**3,771 / 1,521 / 8,080 / `0x17ba32`** on this base, i.e. raw is *not* lower than
base (wave-174 reported 3,770 on its own base); it is not a win.

## 6. Reproduction

```
ulimit -v 12000000
# bool lever (does NOT land: size 8,076 -> byte_exact 4507 -> 4487)
sed -i 's/^    uint8_t won;/    bool won;/' src/game/mission/Mission.h
sed -i 's/^    unsigned char hasWon();/    bool hasWon();/' src/game/mission/Mission.h
sed -i 's/^unsigned char Mission::hasWon() { return this->won; }/bool Mission::hasWon() { return this->won; }/' src/game/mission/Mission.cpp
# + campaign-first dispatch (C1) -> size 8,092, epilogue 0x17b014
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
cmake --build cmake-build-match --target check -j2
```

Scratch helpers (all under `/home/debian/GalaxyOnFire2/scratch/w11919`):
`fast.py` (single-variant build+measure: raw/aligned/size/epilogue/relplt),
`s3.py` (campaign-first), `s4.py` (campaign × htf store-order matrix),
`s6.py` (campaign-first + original mission-id chain order), `s7.py`/`s13.py`
(weaponSlot permutations), `s14.py`/`s16.py` (hudTouchFlags permutations),
`s15.py` (wingman permutations), `align2.py` (per-opcode byte-delta accounting),
`cfg.py` (pre-MBP CFG + block-frequency propagation from `prembp.txt`),
`ranges.py` (same-address diff runs), `relplt.py` (`.rel.plt` order check).
