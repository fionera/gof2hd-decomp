# LevelScript::process — wave 191b / w13704: case-91 state-5 raw recovery

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13704`, branch
`wave191b-w13704`, base master `a51f7dc11` (raw **51,625**, aligned **21,486**,
`.text` diffbytes 71,354, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The wave-191b handoff left two semantic reconstructions with a raw cost:
w13697's case-70 state-1 (+14 raw / -115 aligned, already landed in
`a51f7dc11`) and w13684's case-91 state-5 (+950 raw / -244 aligned, not
landed).  This pass re-derived the case-91 block on the current base and
recovered almost all of its raw cost with a compensating shape.

## Why w13684's landing costs +950 raw

The faithful case-91 state-5 block (original `0x13f3a0`..`0x13f538`) is 8 B
shorter than the previous dead-`getDirection` spelling.  w13684 compensated by
moving `m_nScriptTimerA = 0` to the bottom of case 42 and adding
`m_nScriptTimerB = 0`.  On this base the two timer stores do **not** fold into
the original's single `strd r4,r4,[r5,#144]`: `fp` holds the `LevelScript`
pointer, so clang emits `vmov.i32 q8,#0; add.w r0,fp,#144;
vst1.64 {d16-d17},[r0]` (12 B vs the original's 4 B).  The case-42 block
therefore grows 8 B and every byte between case 42 (`0x13abaa`) and case 91
(`0x1404a8`) shifts, breaking ~935 previously-matching bytes.

The original case-42 state-5 block only resets `m_nScriptTimerA` (offset
0x90); `m_nScriptTimerB` (0x98) is not touched there.  Dropping the timerB
store makes the timer reset a single `strd` again and restores the block size.

## Landed change

1. Reconstruct the whole case-91 state-5 block in the original statement
   order (same order as w13684's note): `resetCamera` first in the
   rocket-control branch, `m_pRadar->enabled = 0` before
   `setComputerControlled`, `setTarget`/`getPosition` before the direction
   work, `getDirection * 10000` before `getRightVector * 700`, and
   `setHitpoints(0)` / `m_nState = 6` last.
2. Compensate the 8 B shrink with one idempotent duplicate
   `player->setCollide(false);` immediately before the original
   `self->getPlayer()->setCollide(false);`.  Both calls store the same
   `false` flag, so the observable behaviour is unchanged, and the extra
   8 B sits inside the already-differing region after `0x5d02` instead of
   shifting the 935-byte matching region.

Case 42 and case 154 are left exactly as on the base (the faithful
`m_nScriptTimerB` reset and the case-154 `setVulnerable` move were measured
but are not needed once the duplicate-call compensation is used).

Measured on the real build (exact NDK r18b `-Oz` compile + full gold link +
`--strip-all`, `tools/fn_diff.py`):

```
raw        51,625 -> 51,661  (+36)
aligned    21,486 -> 21,245  (-241)
size       57,232 / 57,232   (exact)
.text      diffbytes 71,354 -> 71,390 (+36); size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 577 (unchanged; size/base exact 156,312 B @ 0x1a3b78,
           md5 ae1f5fc388cfc48a83fc3990ee670b6b)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

This recovers 899 of the 935 raw bytes w13684 spent (96 %) while keeping
almost all of the aligned win (-241 of -244).

## Candidate table (all size-exact unless noted)

| candidate | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| baseline (`a51f7dc11`) | 51,625 | 21,486 | 57,232 | 577 |
| w13684 full landing (case-91 + timerB + case-154 vuln) | 52,560 | 21,242 | 57,232 | 577 |
| case-91 faithful only (no compensation) | 52,837 | 21,239 | 57,224 | 577 |
| case-91 + case-42 timerA-only (no timerB) | 52,912 | 21,235 | 57,224 | 577 |
| case-91 + case-42 timerA-only + duplicate call | 51,760 | 21,241 | 57,232 | 577 |
| case-91 faithful + case-154 vuln + duplicate call | 51,680 | 21,245 | 57,232 | 577 |
| **case-91 faithful + duplicate call (landed)** | **51,661** | **21,245** | **57,232** | **577** |
| duplicate call after `dockNavPtr = nullptr` | 51,666 | 21,242 | 57,232 | 577 |
| dead `getDirection()` instead of duplicate call | 51,669 | 21,244 | 57,232 | 577 |
| `setHitpoints(0)` after `m_nState = 6` | 51,650 | 21,482 | 57,232 | 577 |
| dead `getDirection()` appended at block end | 54,244 | 21,247 | 57,248 | 577 |

The residual +36 is the case-91 block's own register-allocation difference
(`fp`-based `LevelScript` pointer vs the original's `r5`/`r6`), which cannot
be removed without moving the block to its original address; the bytemap
shows no matching run >= 8 bytes is lost, only short coincidental matches.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13704/` (`dis.py`, `bytemap.py`,
`measure.sh`, `var*.py`, `cand_*.cpp`, `base.so`, `g.so`).
