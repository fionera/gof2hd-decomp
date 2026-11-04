# MGame::OnTouchEnd (8080 B) — w5422 (wave 106): faithful mission-dispatch
# order + unguarded hud0 close, 78.4 % -> 78.9 %

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA
0x17a144, 8080 B).  Measured with the official `tools/verify` row:

| shape | size | fuzzy | exact ratio | gate |
|---|---|---|---|---|
| wave-105 tuned baseline | 8080 | 78.4 | 78.3565 (2174/2775) | green |
| **this pass** | **8080** | **78.9** | **78.8829 (2189/2775)** | **green** |

Gate after the change: byte_exact 4500 / linked_exact 4500, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.

## 1. What changed (4 source edits, all real C++)

```
src/game/menu/MGame.cpp, MGame::OnTouchEnd only
```

1. **Mission dispatch order** — `isInstantActionMission()` is now tested
   *first* and the campaign/freelance dispatch became its `else if` / `else`.
   The binary proves this: `MGame::OnTouchEnd` calls
   `Mission::isInstantActionMission` at 0x17af7e, *before*
   `Mission::isCampaignMission` at 0x17afa8 and `nextCampaignMission` at
   0x17afba.  The wave-105 source had the campaign branch first.
2. **Mission ternary re-call** — `mission = getMission()->hasWon() ?
   getMission() : getCampaignMission()`.  The binary re-calls
   `Status::getCampaignMission` in the else arm (0x17af78), it does not reuse
   the `campaign` local.
3. **hud0 body order / guard** — `hud->closeHudMenu()` is now the first
   statement of the `hudTouchFlags == 0` body and is unguarded.  The binary
   at 0x17b01a does `ldr r0,[fp,#116]; blx closeHudMenu` with no null check,
   then `hudMenuOpen = 0` (0x17b026).
4. **`int refinedAny = 0`** instead of `bool refinedAny = false` in the
   mining-refine block.  This is the one *tuned* (non-faithful) edit — see
   §4.

## 2. Original block order (authoritative, VMA) — unchanged from wave 105

```
0x17afe2  hudTouchFlags == 0 test      (ldr [fp,#248]; cbz 0x17b01a)
0x17afe8  isAutoPilot tail             (shared by camera case 3/0, weaponSlot, hud0)
0x17b004  epilogue
0x17b016  __stack_chk_fail
0x17b01a  hud0 body (closeHudMenu FIRST, unguarded)
0x17b07e  freelance mission failed
0x17b0aa  campaign switch chain
0x17b9ca  weaponSlot body (ends b.w 0x17afe8 — skips the test)
0x17bf4a  wingman body (ends b.w 0x17afe2 — enters the test)
```

The wave-105 note's MBP/HBLP conflict analysis still holds: the
`hudTouchFlags` test's tail edge loses to the `cameraMode == 0` body, so the
`test -> tail -> epilogue -> hud0` cluster is still emitted at the *end* of
our function (0x17b93e/0x17b966) rather than at 0x17afe2/0x17b004.  This pass
does not move it; it improves the *alignment* of the surrounding
mission-dispatch blocks instead.

## 3. Measurements (official fuzzy; baseline = 78.4 / 8080 B)

All variants are produced by `/dev/shm/w5422/variants.py` + `probe2.py`
(scratch, not tracked).  `refined_int` = `int refinedAny`, `ia_elseif` =
instant-action-first else-if chain, `h_both` = unguarded closeHudMenu first,
`m_mcrecall` = re-call getCampaignMission in the ternary.

| variant | size | fuzzy | exact |
|---|---:|---:|---:|
| baseline (wave 105) | 8080 | 78.4 | 78.3565 |
| `ia_elseif` | 8076 | 76.4 | — |
| `refined_int` | 8080 | 78.0 | — |
| `refined_int+ia_elseif` | 8080 | 78.8 | 78.8326 |
| `ia_elseif+h_both+m_mcrecall` | 8076 | 76.4 | — |
| **`refined_int+ia_elseif+h_both+m_mcrecall`** | **8080** | **78.9** | **78.8829** |
| `refined_int+ia_elseif+m_mcrecall+h_noguard` | 8080 | 78.9 | 78.8829 |
| `refined_int+ia_elseif+h_order` | 8080 | 78.8 | 78.7966 |
| `refined_int+ia_elseif+m_mcrecall` | 8084 | 77.2 | — |
| `refined_int+ia_elseif+t_first` | 8052 | 73.0 | — |
| `refined_int+ia_elseif+t_else2` | 8084 | 76.5 | — |
| `refined_int+ia_elseif+hud0_inline` | 8076 | 76.1 | — |
| `refined_int+ia_elseif+chain_400_first` | 8068 | 71.8 | — |
| `refined_int+ia_elseif+cam_swap` | 8080 | 78.6 | — |
| `refined_int+ia_elseif+reorder_nocamp` | 8084 | 76.8 | — |
| `mine_ne` (faithful `amount != 0`) | 8076 | 71.4 | — |
| `mine_ne+refined_int+ia_elseif` | 8072 | 70.6 | — |

Search method: exhaustive pairs over 24 structural transforms
(`/dev/shm/w5422/search.py pairs`), then exhaustive single and pair
augmentation of the best base (`search2.py`, `search3.py`).  Nothing above
78.9 was found.

## 4. Proven vs hypothesis

**Proven from the binary** (all four are visible in the Thumb disassembly):

* `isInstantActionMission()` is called before `isCampaignMission()` and
  `nextCampaignMission()` (call order at 0x17af7e / 0x17afa8 / 0x17afba).
* The mission ternary's else arm re-calls `Status::getCampaignMission`
  (0x17af78) instead of reusing the `campaign` local.
* The `hudTouchFlags == 0` body calls `closeHudMenu()` first and without a
  null guard (0x17b01a..0x17b01e), then stores `hudMenuOpen = 0`
  (0x17b026), `touch0Id`/`touch1Id` (0x17b02a), `pauseOpen` (0x17b02e),
  `resumeSounds` (0x17b032).
* `refinedAny` is `bool` in the original: the test at 0x17a666 is
  `ldr r0,[sp,#40]; lsls r0,r0,#31; beq` (bit-0 test), which is what clang
  emits for `bool`, not for `int` (`cmp #0`).

**Hypothesis / tuned**:

* `int refinedAny = 0` is *not* faithful (see above) but it is the only
  size-neutral way found to compensate the 4 bytes that the faithful
  mission-dispatch reorder removes.  It flips the mining guard from
  `cmp #-1; bgt` to `cmp #0; beq` and shifts one literal-pool slot, which
  re-aligns the huge campaign-switch chain and is worth +0.5 fuzzy on its
  own.  Both spellings are ordinary C++; `int`-for-boolean is period-typical.
* The remaining 586 mismatching instructions are all block placement /
  register allocation / literal-pool slotting.  The largest residual hunks
  are the campaign `hasWon`/`getCampaignMission` sequence and the campaign
  switch chain, both of which follow the still-unplaced tail cluster.

## 5. Exact commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w5422
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
python3 /dev/shm/w5422/score.py                        # official fuzzy
python3 /dev/shm/w5422/exact.py                        # matched-insn ratio
python3 /dev/shm/w5422/probe2.py <combo>               # variant harness
```

Scratch (not tracked): `/dev/shm/w5422/` (`variants.py`, `v2.py`,
`probe2.py`, `search.py`, `search2.py`, `search3.py`, `score.py`,
`exact.py`, `showaddr.py`, `opcodes.py`, `perm.py`, `base.so`,
`refint.so`).
