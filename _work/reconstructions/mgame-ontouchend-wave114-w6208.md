# MGame::OnTouchEnd (8080 B) — w6208 (wave 114): tail-merge host arm,
# `96||127` placement, and why the residual gap is layout-only

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Base: master `d8557f7e2`, branch
`wave114-w6208`.

## Result

**No gate-safe improvement found.**  HEAD stays at **8080 B /
78.9189 % / `.rel.plt` diff 0 / RATCHET PASS** (byte 4500, linked 4500,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
27 sections, file 2,187,328 B, `.ARM.extab` 0x26298).  This note records
four new, decisive measurements that close off the remaining levers.

## 1. The `96 || 127` source form is correct; the original's `cmp #127`
##    is a *layout* artefact, not a separate source case

The wave-113 note claimed the original has "an extra `cmp r0,#127` /
`cmp r0,#96` branch pair in the `==96||==127` arm".  It does not.  The
original's campaign-mission compare order (by address) is

```
15 22 43 42 65 81 74 95 96 100 110 120 126 127 134 144 155 161 162
```

i.e. `cmp r0,#127` sits at 0x17bc34 — **between `126` (0x17bbbc) and
`134` (0x17bc4c)** — and `beq.w 17b722` jumps *back* to the shared
case-96 body at 0x17b722.  HEAD (`96 || 127`) emits exactly the same
instruction sequence; it just lays the `127` block out at 0x17ba1c
(right after the `96` block at 0x17b848) instead of after `126`.

A faithful separate `== 127` case placed between `126` and `134` (both a
duplicated body and a `goto campaign96` label form) reproduces the
original's compare order exactly —

```
17b848  cmp r0,#96 ; bne.w 17b9a2
17b84e  <case 96 body>
17bc7a  cmp r0,#127 ; beq.w 17b84e
```

— but the function grows to **8100 B** (two extra shared-body edges keep
one more block alive at -Oz) and the fuzzy score falls to **75.1620 %**.
So the `||` spelling in HEAD is the right one; the `127` position is a
block-placement difference, not a source difference.

## 2. The merged `blx initHudMenu` host arm is a `t <=` tie-break

`BranchFolder::CreateCommonTailOnlyBlock` picks `PredBB` if it is in
`SameTails`, otherwise the **last** entry with the smallest
`EstimateRuntime`.  `ComputeSameTails` walks `MergePotentials` from the
**highest** MBB number down, so `SameTails[0]` is the highest-numbered
arm and the `t <= TimeEstimate` update makes the **lowest-numbered** arm
win.

Both `initHudMenu` arms have a 3-cycle prefix (`ldrd` = mayLoad → 2,
`movs` → 1), so the tie-break is purely the MBB number.  The `0x200` arm
is first in the dispatch chain (smallest MBB) and therefore hosts the
merged tail in HEAD (`0x17adc4 ldrd; movs r1,#1; 0x17adca blx`), while
the original hosts it on the `0x400` arm (`0x17afd8 ldrd; movs r1,#2`
falls through to `0x17afde blx`).

Confirmed empirically: moving the `0x400` arm to the front of the
dispatch chain (`gen7` permutation `2,1,0,3,…`) flips the host to
`0x17adc6 ldrd; movs r1,#2; 0x17adcc blx` and scores **78.8108 %**.  The
dispatch-chain compare order then no longer matches the original
(`tst #0x400` first instead of `tst #0x200`), so this is not usable.

The `PredBB` route is closed too: in the first placement the
`hudTouchFlags == 0` test block's layout predecessor is the wingman-body
end (`bb.422`), not either arm, and `hasBetterLayoutPredecessor` rejects
the `0x400 arm → test` edge because `freq(0x200 arm) = 0.049` is far
above `freq(0x400 arm)/4 = 0.0033`.

## 3. `hasBetterLayoutPredecessor` arithmetic for the test → tail edge

Propagating the MIR edge probabilities from the entry (2 iterations,
`mirn.txt`, post-placement tail merge disabled) gives

| block | freq |
|---|---:|
| `0x200` arm (`bb.391`) | 0.04888 |
| `0x800` arm (`bb.415`) | 0.02528 |
| `0x400` arm (`bb.417`) | 0.01306 |
| wingman-body end (`bb.422`) | 0.01397 |
| `hudTouchFlags == 0` test (`bb.450`) | 0.10500 |
| tail `isAutoPilot && !wasAutoPilot` (`bb.458`) | 0.17185 |
| hud0 body (`bb.451`) | 0.04049 |

For the candidate edge `test → tail`:
`CandidateEdgeFreq = 0.105 * 0.625 = 0.0656`; the hud0-body predecessor
of the tail has `0.0405 * 0.8 = 0.0324 >= 0.0656 * 0.2 = 0.0131`, so the
edge is **always** rejected and the test falls through to the hud0 body
(37.5 %), exactly as HEAD does.  The original has the test falling
through to the tail, which therefore cannot be reached from this CFG
shape by any statement reordering that leaves the edges intact.

## 4. The `-print-machineinstrs` block order is not the final layout

`-mllvm -print-machineinstrs` on HEAD puts the `hudTouchFlags == 0` test
block (`bb.354`, `t2LDRi12 $r11,248; t2CMPri 0; t2Bcc %bb.358`) between
the weaponSlot-body end (`bb.353`) and the tail (`bb.355`) — but the
emitted code has the test at **0x17aeb2** and the weaponSlot body end
falls straight through to the tail at 0x17b970.  The MIR printout order
therefore cannot be used as the layout order for this function; every
"which block is where" claim must be cross-checked against the
disassembly.  (The block count also jumps 471 → 552 at the ARM constant
island pass, so MIR indices are not stable across stages.)

## 5. Measurements (all on top of HEAD)

| variant | size | fuzzy (4 dp) |
|---|---:|---:|
| **HEAD** | **8080** | **78.9189** |
| `gen7` identity (sanity) | 8080 | 78.9189 |
| `gen7` `2,1,0,…` (0x400 arm first, host flips) | 8080 | 78.8108 |
| `gen7` `1,0,2,…` (0x800 arm first) | 8080 | 78.5225 |
| `gen7` `0,2,1,…` (0x800/0x400 swap) | 8064 | 71.9798 |
| `gen7` `0,1,3,4,…,11,2` (0x400 arm last) | 8064 | 74.4907 |
| `gen7` `0,1,2,3,4,5,6,7,9,10,8,11` (0x80000/0x40000 swap) | 8080 | 78.3423 |
| `gen7` `0,1,2,3,4,5,6,8,9,7,10,11` (0x10000/0x8000 swap) | 8080 | 78.8108 |
| `gen7` `0,1,2,3,4,5,7,8,9,6,10,11` (0x10000/0x20000 swap) | 8076 | 75.9279 |
| `gen7` `11,0,…` (UseKhadorDrive first) | 8092 | 73.9287 |
| `gen7` `0,11,1,…` | 8104 | 74.1274 |
| `gen7` `0,1,11,2,…` | 8104 | 75.8683 |
| `gen7` `0,1,2,3,4,5,6,7,8,9,11,10` | 8092 | 74.9865 |
| `gen7` `0,1,2,3,4,5,6,7,8,10,11,9` | 8092 | 75.3467 |
| `gen8` all 8 cond/body/fetch combinations | 8076–8084 | 76.1579 – 78.9189 |
| `gen15` pre-`ca43091c` shape (`pre`) | 8076 | 77.1310 |
| `gen15` `pre_cmfirst` / `cmfirst` | 8076 | 76.3295 |
| `gen15` `pre_keep` | 8080 | 78.9189 |
| `gen16` separate `== 127` case after 96 | 8096 | 74.7795 |
| `gen18` separate `== 127` case after 126 (dup body) | 8100 | 75.1620 |
| `gen20` separate `== 127` case after 126 (`goto`) | 8100 | 75.1620 |
| `gen17` `sep3` (three separate `if`s) | 8080 | 78.9189 |
| `gen17` `sep3_wingfirst` | 8080 | 78.9189 |
| `gen17` `sep3_hudmid` | 8032 | 73.8400 |
| `gen17` `nested_wing_else` | 8084 | 76.6847 |
| `gen17` `hud_else_ws` | 8048 | 72.9666 |
| `gen19` tail before `hudMenuOpen` | 8032 | 60.8851 |
| `gen19` tail before the dispatch | 8032 | 40.0071 |
| `gen19` tail after the hud0 block | 8080 | 78.9189 |
| `gen21` `if (!hudMenuOpen)` as a second top-level `if` | 8076 | 74.7253 |
| `gen22` tail before the hud0 `if` | 8080 | 78.9189 |
| `gen22` `weaponSlot`/`wingmanCommand` declared at function scope | 8080 | 78.9189 |
| `gen23` `127 || 96` (127 check first) | 8080 | 78.8829 |
| `gen23` `96 || 127` without the inner `isCampaignMission()` | 8068 | 73.7070 |

Code-identical (8080 / 78.9189, no source lever): `sep3`, `sep3_wingfirst`,
`swap_tail_hud0`, `decl_top`, `pre_keep`, `gen6` flat, `gen13` if/else and
switch-default, `gen14` switch/if.

## 6. Gate / section state (unchanged)

27 sections (exact), file size 2,187,328 B, `.text` 0x1337c8 (size and
base exact), `.ARM.extab` 0x26298 (exact), `.rel.plt` diff 0, imports
207, lint CLEAN, drift 0, `RATCHET PASS` (byte 4500 / linked 4500,
avg 99.92, size 4524/4524, data 843/843).

```bash
cd /home/debian/GalaxyOnFire2/wt/w6208
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w6208/score.py                      # 8080 / 78.9189
```

Scratch (not tracked): `/dev/shm/w6208/` (`score.py` prints the 4-dp
ratio, `runone.sh <gen.py> "<args>"` restores the base, applies one
generator, rebuilds and prints the score, `mset.py` prints the
normalised instruction multiset diff, `blkseq.py`/`lcsblk.py` align the
basic-block sequences, `freqs.py` propagates the MIR edge probabilities,
`mirf_new.txt` is the current `-print-machineinstrs` dump, `mirn.txt` is
the same with `-branch-fold-placement=false`).
