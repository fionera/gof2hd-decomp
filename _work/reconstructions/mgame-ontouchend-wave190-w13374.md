# MGame::OnTouchEnd — wave 190 (w13374): finish-block + 96/127 split re-check

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13374`, branch
`wave190-w13374`, base master `0f54626b3`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13374`.

Prior notes: `mgame-ontouchend-wave189-w13318.md` (full-body pass),
`mgame-ontouchend-wave189-w13323.md` (96/127 split analysis),
`mgame-ontouchend-wave189-w13344.md` (split + compensation search),
`mgame-ontouchend-wave189-w13357.md` (escort-block reorder, raw 2,920 -> 2,913).

## Result

**No size-exact raw win.** `src/` is unchanged. The base is a local raw minimum
under every sweep tried (adjacent transpositions, finish-block permutations,
escort-block permutations, chain re-orders, condition spellings). The note records
the measured shapes and the structural findings so the next pass does not repeat
them.

| metric | base |
|---|---:|
| raw same-address diff bytes | 2,913 |
| aligned (instruction) diff | 1,706 |
| function size | 8,080 / 8,080 |
| `.text` differing bytes | 73,516 |
| `.ARM.extab` size / diff | 156,312 / 610 |
| `.rel.plt` diff | 0 (byte-identical) |
| build-id diff | 20 |
| file size | 2,187,328 exact |

Gate after restoring base (`check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 headers.

## 1. The `!keepObjectives` block already emits the original order

The original finish block at `0x17b26c` is emitted as

```
removeObjectives(); setMission(empty); setRoute(nullptr);
if (goingToWaypoint()) setAutoPilot(nullptr);
removeRoute(); setPlayerRoute(nullptr);
```

i.e. **A C B F D E**. Our source is written A F C B D E (the w13318 win), but the
compiler's scheduler already hoists `setMission`/`setRoute` above the
`goingToWaypoint` guard, so the emitted bytes at `0x17b3da` are exactly A C B F D E
and differ from the original only by the block's address (ours `0x17b3da` vs
original `0x17b26c`, +0x16e). Rewriting the source to A C B F D E therefore does
**not** change the finish block; it only perturbs the 42-block/keepObjectives
layout and measures **raw 2,922** (`.text` 73,525), worse than the 2,913 base.

The failed-mission block at `0x17b89c` is already at exactly the original address
and byte-identical (A C B F D E), confirming the emitted order.

## 2. Every finish-block source permutation is raw-neutral or worse

120 random permutations of the six independent statements
(`removeObjectives`, guard, `setMission`, `setRoute`, `removeRoute`,
`setPlayerRoute`) on the current base: no permutation beats raw 2,913; the landed
order is the best. This reproduces w13318's 720-permutation result on the newer
escort-reordered base.

## 3. Escort block: the base order is a local minimum

200 random permutations of the six independent escort statements (with the
`Mission`/`Item` declarations first) all measure raw >= 2,913. The original emitted
order is `field_0x1e0 = killCountB; makeItem; setEquipment; new Mission;
setMission; setFreelanceMission; active = 0; module(2)` (from `0x17b2cc`); writing
that source order measures **raw 2,922, `.ARM.extab` 612** (worse). The landed
w13357 order remains the raw best.

## 4. 96/127 split: structurally real, size +4/+8, raw >= 3,900

Confirmed against the binary: the original has two separate tests, 96 at
`0x17b71c` and 127 at `0x17bc34`, each with its own `isCampaignMission()` call;
127's `beq.w 0x17b722` tail-merges into the shared 96 body. Splitting our merged
`(== 96 || == 127)` predicate therefore costs the duplicated
`mov/blx isCampaignMission` (8 B).

Measured on the current base:

| variant | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| base (merged) | **2,913** | 1,706 | 8,080 | 610 |
| split adjacent to 96 | 3,914 | – | 8,084 | 1,321 |
| split after 126 (original position) | ~3,880 | – | 8,088 | – |
| original chain order + split | 4,749 | – | 8,100 | 1,328 |
| original chain order + split + instant-first won block | 5,781 | – | **8,080** | **609** |

The last row is the only size-exact split reconstruction found; it also improves
`.ARM.extab` 610 -> 609, but `.text` rises 73,516 -> 76,384 and raw to 5,781, so it
is a net loss. The split's raw cost is the ~1,000-byte block-placement shift, not
the 8-byte test; no single compensation recovers it.

## 5. Other measured probes (all reverted)

- `count != 0` / `amount != 0` / `~amount+1` / `0-amount` / `(int)(0u-(unsigned)amount)`
  emit the original `cmp.w r9,#0; beq.n` but canonicalise the decrementing loop
  counter and flip the global `sl`/`r9` allocation: size **8,088**, raw 6,592.
  `count > 0`, `amount < 0`, `count >= 0 && count != 0`, `(count != 0) & (count >= 0)`
  are size-exact but raw 2,915. `amount <= 0`, `amount < 1`, `0 >= amount` are
  byte-identical to the base.
- Mining-block `String` expression variants (named prefix, named `t`, named text,
  split statement, `report = report + ...`, `getText(refined + 1274)`,
  `const char *t`) either are byte-identical (`getText` commute, `const char *`)
  or blow up the layout (raw 3,231-7,595). The 22-byte residual at `+0x4a4` is a
  pure scheduler hoist of `add.w r8, sp, #80` plus a `mov r0,r8`/`movs r2,#0`
  swap; no source shape found that reproduces the original schedule.
- Block-100 original body order (`HP,Shield,Armor,Gamma,sw=0,init=0,depart,module`)
  is a genuine fidelity fix but measures **size 8,048 (-32), raw 3,648** — the
  layout shifts, so it is size-fatal on the merged base and cannot be used as the
  split compensation.
- `95` switch-first, `96` Armor/Shield swap, `tail_best` back-to-station order,
  removing the `__builtin_expect` on 162, and `else if` for the escort block all
  measure raw >= 2,915 or size-fatal.
- Full adjacent-transposition sweep of all 311 same-indent statement pairs in
  `OnTouchEnd`: no hit below raw 2,913.

- The original body order (HP, Shield, Armor, Gamma) applied block-by-block to
  the size-exact original-order+split+instant-first variant (65/81/95/96/127/100/
  110/126/161/162) does not converge: the first three fixes keep size 8,080 but
  raw stays 5,786-5,790; the 96/127 and later fixes grow the function to 8,120-8,152.
  The size-exact 8,080 of the un-corrected variant is a layout coincidence, not a
  faithful reconstruction.

## Conclusion

The current base is a strong local raw minimum. The remaining 2,913 diff bytes are
dominated by block placement from `+0xe3e` (the won-block preamble and the campaign
chain), not by any local statement order. The next pass should either (a) fully
reconstruct the original won-block `instantAction`-first structure together with
the original chain order and body sizes so the block addresses align, or (b) leave
this function and spend the pass elsewhere. The finish block and the 96/127 split
are exhausted as independent levers.

## Current-master status (wave 190 harvest)

The measurements above are from base `0f54626b3`. Current master after the
wave-190 harvest is raw **2,913**, aligned 1,706, size 8,080/8,080; `.text`
73,274, `.ARM.extab` 606, total 73,900. No source change was landed from this
note; it is a negative-result record so the next MGame pass does not repeat the
finish-block/escort permutations or the 96/127 split search.
