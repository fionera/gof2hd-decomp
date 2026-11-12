# MGame::OnTouchEnd — wave 189 (w13323): 96/127 chain split, `count` branch, and body order

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13323`, branch
`wave189-w13323`, base master `6854ca93a`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13323`.

## Result

No size-exact raw win was found. **`src/` is unchanged (base).** The note records the
measurements so the next pass does not repeat them.

| metric | base |
|---|---:|
| raw same-address diff bytes | 2,928 |
| aligned (instruction) diff | 1,670 |
| function size | 8,080 / 8,080 |
| insn count | 2,861 / 2,865 |
| `.text` differing bytes | 74,017 |
| `.ARM.extab` size / diff | 156,312 / 639 |
| `.rel.plt` diff | 0 |
| build-id diff | 20 |
| total differing bytes | 74,676 |
| file size | 2,187,328 exact |

Gate after restoring base (`check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 headers.

## 1. Splitting the `96 || 127` block (the w13216 target)

The original keeps `== 127` as a separate chain block between 126 and 134 whose
`beq.w 17b722` jumps into the shared 96 body. Our source merges it as
`(getCurrentCampaignMission() == 96 || ... == 127)`. Four placements were built and
measured (size-exact means 8,080):

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base (merged) | 2,928 | 1,670 | 8,080 | 2861/2865 |
| split adjacent to 96, current order | 3,929 | 1,683 | 8,084 | 2862/2865 |
| split after 126, current order | 3,889 | 1,658 | 8,088 | 2863/2865 |
| split after 126, original order | 3,987 | 1,185 | 8,092 | 2865/2865 |
| split adjacent + `count != 0` | 6,173 | 1,967 | **8,080** | 2861/2865 |
| split after 126 + `count != 0` | 6,536 | 1,981 | 8,084 | 2862/2865 |

The split always shifts the chain body by several bytes, so raw rises by ~1,000 even
when the final size is exact. The adjacent split alone is +4 B (not +8): the merged
96 test shrinks by ~20 B while the 127 test block adds 22 B, and the two bodies
tail-merge. The +4 is the extra `isCampaignMission()` call, as expected.

The original chain order (15,22,43,42,65,81,74,95,96,100,110,120,126,127,134,144,
155,161,162) gives the best **aligned** count seen for this function (1,185 vs 1,670)
but is +12 B in our body shapes, so it is not landable without the body work below.

## 2. `if (count >= 0)` is the wrong source condition

Original at `0x17a5ca` is `cmp.w r9,#0; beq.n 17a652` (skip when `amount == 0`).
Our `if (count >= 0)` emits `cmp.w r9,#0; bgt.n` (never skips, because
`count = -amount` and `amount <= 0`). `count >= 0` is always true, so the
reconstruction is behaviourally wrong: it appends a `"0t ..."` line for every ore
that was not refined.

`count != 0` (equivalently `amount != 0`, `if (count)`, `if (amount)`, or
`if (count == 0) continue;`) produces the original `beq.n` **and** the original
`+0x4a4` schedule (`add.w r8, sp, #80` after `ldr r1,[pc]; add r1,pc` instead of
before the first `operator+`). But it also flips the global register allocation:
the prologue becomes `mov r9, fp; ldr sl, [r0]` instead of the original
`mov sl, fp; ldr r9, [r0]`, and the function grows to 8,088 B (raw 6,591,
aligned 2,175). The swap is a graph-colouring consequence of the changed branch
probability, not a local effect.

`if (count > 0)` is size-exact (8,080) and semantically correct, but emits
`cmp.w r9,#-1; bgt.n` and leaves the schedule hoist: raw 2,930 (+2), aligned 1,671.
Not a raw win, but it is the only size-exact, behaviour-correct form found.

### Allocator-flip search

To test whether the `sl`/`r9` swap could be perturbed back, a full sweep of every
adjacent same-indentation simple-statement transposition (721 candidates) was run
on top of the size-exact `split adjacent + count != 0` variant. Every candidate kept
size 8,080 and the swapped prologue (`46 d9` at `0x17a154`); none produced the
original `46 da`. A second 721-candidate sweep on plain `count != 0` also produced
no flip. The swap is robust to single transpositions.

## 3. Chain-block body order (new finding)

The original getter/store order is **the same in every campaign block**:

```
HP -> field_64, Shield -> field_5c, Armor -> field_60, Gamma -> field_68
```

with `switch_to_target_setting` / `Level::initStreamOutPosition` / `departStation`
placed around that run per block (e.g. 65 has `sw` first, then the four getters,
then `init`, then `new FileRead`; 100 has the four getters, then `sw`, `init`,
`depart`, `setmod`; 110 has the getters, then `sw`, `init`, `depart`, `setmod`,
`enterSpaceLounge`).

Our source differs from this order in blocks 65, 81, 95, 96, 100, 110, 126, 161 and
162. Reordering all of them to the original order (and reordering block 100
completely, including moving `SetCurrentApplicationModule(5)` after `departStation`)
gives:

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base | 2,928 | 1,670 | 8,080 | 2861/2865 |
| block 100 reorder only | 3,663 | 1,585 | 8,048 | 2848/2865 |
| uniform getter reorder (all blocks) | 3,915 | 1,501 | 8,084 | 2864/2865 |
| block 100 + uniform getters | 3,914 | 1,498 | 8,052 | 2851/2865 |
| explicit body reorder of 65/81/95/96/100/110/126/161/162 | 3,914 | 1,498 | 8,052 | 2851/2865 |

The aligned count drops 1670 -> 1498, i.e. the body order is a genuine fidelity
improvement, but the size falls 28 B short of 8,080. The 100-block reorder alone is
-32 B: its reordered form tail-merges the `active = 0; return` path with the
134/162 tail exactly like the original (`b.n 17bcac`), whereas the base 100 block
emits its own inline tail. That -32 must be compensated by +28 elsewhere (the
remaining blocks' tail-merging/placement differs from the original) before the body
reorder can be landed size-exactly.

## 4. Other negative results

- `won`/`instantAction` branch restructure to the original shape
  (`if (!instantAction) { if (campaign) ... else ... } else { ... }`) is +4 B
  (8,084, raw 4,703). The original also does `setFreelanceMission` before
  `showMissionRewardMessage`; swapping those alone is raw 2,930 (+2).
- 134-block `switch`/`field_5c` swap is size-exact and raw-neutral (2,928) but drops
  aligned to 1,559. The w13216 note called this a difflib artifact; the measurement
  is reproducible but raw does not move.
- The `+0x4a4` schedule difference is caused by the `count` branch, not by the
  string-expression shape: `String("t ", false)`, a `static const char[]`, a
  `const char *` variable, and `getText` in a named temporary all leave the base
  schedule and raw 2,928 (or worse) unchanged.

## Conclusion

The split is a real structural match but is not a raw win in any placement: the
chain body is still ~1,000 raw bytes away from the original, and the only way to
realise the split is to re-derive the block bodies/tails (the -28 B body-order
result above) so that the original chain order becomes size-exact. `src/` is left
at the base.

## Harvest on wave-189 master (`02734d43c`)

Note-only: raw stays 2,928 / 1,670 / 8,080.  Current master is `.text` 73,981
and total 74,616.  The 96/127 split costs +8 B and the count-branch
compensations are recorded above; the next MGame pass should try a different
compensation shape (or the split on the current body).
