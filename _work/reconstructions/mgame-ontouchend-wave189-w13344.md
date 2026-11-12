# MGame::OnTouchEnd — wave 189 (w13344): 96/127 split + compensation search

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13344`, branch
`wave189-w13344`, base master `2ae16a5d8`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13344`.

Prior notes: `mgame-ontouchend-wave189-w13323.md` (96/127 split analysis),
`mgame-ontouchend-wave188-w13216.md` (mission-162 transposition, transposition sweeps).
w13318 is doing the full-body/RA pass; this note does not duplicate its cells.

## Result

No size-exact variant with raw < 2,928 was found. **`src/` is unchanged (base).**
The note records the measured source shapes and the exact compensation search so the
next pass does not repeat them.

| metric | base |
|---|---:|
| raw same-address diff bytes | 2,928 |
| aligned (instruction) diff | 1,670 |
| function size | 8,080 / 8,080 |
| insn count | 2,861 / 2,865 |
| `.text` differing bytes | 73,981 |
| `.ARM.extab` size / diff | 156,312 / 639 |
| `.rel.plt` diff | 0 |
| build-id diff | 20 |
| total differing bytes | 74,616 |
| file size | 2,187,328 exact |

Gate after restoring base (`check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 headers.

## 1. The split reproduces exactly as w13323 reported

The original 96 test is at `0x17b70c` (`cmp r0,#96`, `bne.w`), and the separate
127 test is at `0x17bc24` between 126 and 134:

```
17bc24  mov r0, r5
17bc26  blx isCampaignMission
17bc2a  cbz r0, 17bc3a
17bc2c  ldr r0, [r8]
17bc30  blx getCurrentCampaignMission
17bc34  cmp r0, #127
17bc36  beq.w 17b722        ; jump into the shared 96 body
```

Our merged `(== 96 || == 127)` test is 14 B at the 96 slot; the original's separate
127 test is 22 B, so splitting costs +8 B. Measured placements (all reproduce the
w13323 numbers):

| placement | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base (merged 96\|\|127) | 2,928 | 1,670 | 8,080 | 2861/2865 |
| split adjacent to 96 | 3,929 | 1,683 | 8,084 | 2862/2865 |
| split after 126 | 3,889 | 1,658 | 8,088 | 2863/2865 |
| split after 134 | 3,890 | 1,373 | 8,092 | 2865/2865 |
| split after 144 | 3,888 | 1,373 | 8,092 | 2865/2865 |
| split after 155 | 3,888 | 1,659 | 8,088 | 2863/2865 |
| split after 161 | 3,892 | 1,656 | 8,088 | 2864/2865 |

The split is a real structural match (the 127 test is byte-for-byte the original's
`mov/blx/cbz/ldr/blx/cmp/beq.w` shape), but every placement shifts the body and the
literal pool, so raw rises by ~1,000 even when the size is exact.

## 2. Full original chain order + body order (the w13323 "body reorder" target)

The original emitted chain order is
`15,22,43,42,65,81,74,95,96,100,110,120,126,127,134,144,155,161,162`.
Reconstructing the block bodies in the original order (verified against the
disassembly, not just the earlier note) gives:

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| merged, original chain order | 4,273 | 1,307 | 8,084 | 2863/2865 |
| split, original chain order, base bodies | 4,804 | 1,507 | 8,104 | 2866/2865 |
| split, original chain order, original bodies | 4,807 | 1,196 | 8,104 | 2867/2865 |

Key body-order corrections found against the binary (the w13323 note's per-block
recipe is not exact):

- 95: `switch_to_target_setting = 1` is **first**, then HP, Shield, Armor, Gamma.
- 126: `switch_to_target_setting = 1` is **first**, then HP, Shield, Armor, Gamma.
- 161: `switch_to_target_setting = 0` then `initStreamOutPosition = 0` (sw before init).
- 65, 81, 120: `switch_to_target_setting` first, then the four getters.
- 100: four getters (HP, Shield, Armor, Gamma), then `sw=0`, `init=0`,
  `departStation(120)`, `SetCurrentApplicationModule(5)`.
- 110: four getters, then `sw=0`, `init=0`, `departStation(10)`,
  `SetCurrentApplicationModule(5)`, `enterSpaceLounge=1`.
- 162: four getters, then `sw=0`, `init=0`, `departStation(93)`.

With the corrected bodies the aligned count drops 1,670 -> **1,196** (close to the
w13323 note's 1,185), but the function is **8,104 B (+24)**. The residual +24 is
mostly in the tail after 162 (+28 B); the chain block `cmp` addresses are within
±6 B of the original at 126/127/134/144/155/161/162.

## 3. Compensation search

### 3.1 Block-100 statement order

Block 100 has the most slack. Sweeping all 48 combinations of
`{A=SetCurrentApplicationModule(5), B=field_64, C=field_60, D=field_68,
E=field_5c, F=switch_to_target_setting=0, G=departStation(120),
H=initStreamOutPosition=0}` (I/J fixed at the end) on the split bases:

| split base | base order | A after H | original order (B,E,C,D,F,H,G,A) |
|---|---:|---:|---:|
| after 126 (8,088) | 8,088 | 8,084 | 8,060 |
| adjacent (8,084) | 8,084 | 8,076 | 8,052 |

The original block-100 order is a -28 B change on the split base, but it is not
enough to make the split size-exact, and it does not lower raw below the base.

### 3.2 Adjacent-transposition sweeps

Full adjacent-transposition sweeps (256 candidates each, every same-indentation
simple-statement pair in `OnTouchEnd`) were run on:

- `split adjacent` base (8,084): 11 size-8,080 hits. Best raw **3,506**
  (`Globals::status->nextCampaignMission(true);` <-> `Globals::status->setKills(3);`
  in the pause-menu switch), aligned 1,681.
- `split after 126` base (8,088): **no** size-8,080 hit at all (sizes 8,084/8,088/
  8,092/...).
- `split after 126 + block-100 A-after-H` (8,084): 12 size-8,080 hits. Best raw
  **3,555** (same `nextCampaignMission`/`setKills` swap), aligned 1,673.
- full original-order + original-body variant (8,104): no size-8,080 hit; the
  minimum is 8,096 (three hits).

Every size-exact split hit is raw >= 3,506, i.e. above the 2,928 base. The split
itself is the ~1,000-byte raw cost; no single transposition recovers it.

### 3.3 `count` branch

`int count = -amount; if (count >= 0)` is the first residual byte (`bgt.n` where
the original has `beq.n` at `+0x48a`). Confirmed again:

- `count > 0` and `count >= 0 && count != 0` both still emit
  `cmp.w r9,#-1; bgt.n` (the compiler folds the redundant test), size 8,080,
  raw 2,930 (+2).
- `count != 0` emits the original `cmp.w r9,#0; beq.n`, but flips the global
  register allocation and grows the function: 8,088 on base (raw 6,591),
  8,084 on split-after-126 (raw 6,536), 8,080 on split-after-126 + block-100
  A-after-H (raw 6,243). Size-exact but raw-fatal.

### 3.4 Source shape that emits the 127 block without the +8 B

Dropping the redundant `mission->isCampaignMission()` from the 127 condition
(`else if (Globals::status->getCurrentCampaignMission() == 127)`) saves the
`mov/blx/cbz` (8 B) and emits a 14-byte 127 test. It is behaviourally equivalent
for the campaign chain but does not match the original's 22-byte test. Measured:

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| split after 126, no campaign check on 127 | 3,874 | 1,774 | 8,084 | 2861/2865 |
| split after 126 + block-100 A-after-H, no check | 3,607 | 1,769 | **8,080** | 2860/2865 |
| original chain+body order, no check on 127 | 3,962 | **1,036** | **8,080** | 2862/2865 |

The last row is the best structural reconstruction found (aligned 1,036 vs 1,670)
and is size-exact, but its raw is 3,962 (the body/tail layout is still shifted),
so it is not a raw win. A full adjacent-transposition sweep on this variant
(256 candidates) produces 40+ size-8,080 hits; the best is raw **3,961**
(aligned 1,032, `m_nTimeLimit=0` <-> `setMission(Mission::empty)`), still above
the 2,928 base.

## 4. Conclusion

The 96/127 split is a genuine structural match and can be made size-exact
(8,080) with several independent statement/order changes, but every such variant
has raw >= 3,506 against the 2,928 base. The base is a local raw minimum: the
split's ~1,000-byte raw cost is only recoverable by matching the compiler's block
placement/tail-merging in the chain and the post-162 tail, which is the w13318
full-body/RA pass. `src/` is left at the base; no code win was landed.

## Reproduction

Scratch scripts (`/home/debian/GalaxyOnFire2/scratch/w13344/`):
`chain.py` (chain-order permutations), `origbody.py` (original body orders),
`splitpos.py` (127 placement), `vgen.py`/`combo.py` (block-100 orders),
`trans_sweep.py` (adjacent transpositions), `countvar.py` (count branch),
`fn_diff.py` (raw/aligned/size). All builds used `-j2` with `ulimit -v 12000000`.
