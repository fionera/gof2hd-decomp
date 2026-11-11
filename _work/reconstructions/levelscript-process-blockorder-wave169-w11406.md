# LevelScript::process — wave 169 / w11406: outer mission-dispatch block order
# and the per-mission switch arms (block-order census; no source win)

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
size-exact.  Worktree `wt/w11406`, branch `wave169-w11406`, base master
`ec3ceb07c` (wave 168).  Scratch `/dev/shm/w11406/`.

## Baseline (HEAD, tree left clean)

```
process raw diffbytes    54,015
process aligned insns    17,788
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20715 / 20830
.text diffbytes          82,327
.ARM.extab diffbytes     668
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
lint CLEAN / RATCHET PASS
```

## 1. Method

A reachable-CFG walker (`/dev/shm/w11406/cfg.py`, capstone 5.0.7) decoded the
function from its entry, following `b`/`bcc`/`cbz`/`cbnz`/`tbh`/`tbb`
successors and stopping at `pop {...,pc}`/`bx lr`, so literal pools are not
mistaken for code.  It produced **1,394 orig / 1,356 ours basic blocks**
(19,245 / 19,128 reachable instructions; the rest of objdump's 20,715 / 20,830
is literal-pool data and unreachable cold code).  Blocks were normalised
(registers -> `R`, immediates/branch targets -> `I`, symbol names stripped)
and paired by unique signature; 276 pairs are unique.  The physical-order
inversions below are from that pairing, cross-checked by decoding every
`tbh` table from its actual bytes (base = `tbh+4`).

## 2. Outer mission-dispatch physical order differs

The 20-value linear compare chain at `0x1387e2..0x13885e` is byte-for-byte the
same order in both binaries (14,16,21,24,29,40,41,42,56,80,91,92,94,102,105,
114,125,154,157,158) and the two range prongs (`131..145` @`0x138a8a`,
`64..73` @`0x138c3e`) are at the same addresses.  The **mission-specific
bodies before the switch guard**, however, are laid out in a different
physical order:

| chain slot | original | ours |
|---|---|---|
| 1 | mission 5 (0x13812a) | mission 81 (0x138124) |
| 2 | mission 81 (0x1381b2) | mission 5 (0x13824c) |
| 3 | mission 26 (0x1382dc) | mission 78 (0x1382cc) |
| 4 | mission 78 (0x13832c) | mission 26 (0x1384c2) |
| 5 | mission 50 (0x1383e4) | mission 50 (0x13874a) |
| 6 | mission 89 (0x138574) | mission 51 (0x138758) |
| 7 | mission 51 (0x13876c) | mission 89 (0x138550) |
| guard | 0x13879e | 0x138786 |

So the original order is **5, 81, 26, 78, 50, 89, 51**; ours is
**81, 5, 78, 26, 50, 51, 89**.  The source order (outer chain 0,1,81,78,89,
106,95..., then active tail, then pre-switch specials 5,26,50,51) is the same
as ours; the difference is MachineBlockPlacement, not source order.

The active-tail fallthrough confirms the original layout: original
`0x137f82 beq 0x13812a` and the tail body ends at `0x138128` falling into
`0x13812a` (mission 5); ours falls into `0x138124` (mission 81).  The
mission-50 `goto wormholeExitBody` is at `0x1383e4` in the original (before
mission 89) but at `0x13874a` in ours (after mission 26/89).

## 3. Mission-78 switch: body order differs (tbh @0x138384 / 0x138326)

Decoded 12-entry table (states 1..12; base `tbh+4`):

```
orig  s1=1383a0 s2=13ba5c s3=13b8dc s4=13baba s5=13b8c0 s6=13bb7c
      s7=13bc18 s8=13baf2 s9=13bc34 s10=13b948 s11=13bd2c s12=13b7f2
ours  s1=138342 s2=13b80e s3=13b788 s4=13b7d6 s5=13b660 s6=13b8ec
      s7=13b98a s8=13b862 s9=13b9a4 s10=13b682 s11=13ba98 s12=13b598
```

Physical order:

```
orig  1, 12, 5, 3, 10, 8, 2, 4, 6, 7, 9, 11
ours  1, 12, 5, 10, 3, 4, 2, 8, 6, 7, 9, 11
```

The `3/10` pair and the `8/2/4` cluster are swapped.  Reordering the C++
`case` labels (tested `1,12,5,3,10,8,2,4,6,7,9,11` and three neighbours) is a
codegen no-op: all four source orders lower to the same physical
`1,12,5,10,3,4,2,8,6,7,9,11` and the same 54,015 raw / 57,232 B.  The order
is fixed by the shared `break` target at the mission-78 switch exit
(`0x13c88c`), not by source case order.

## 4. Case-145 switch: original splits the dispatch

Original `0x1393a2`:

```
1393a2 ldr r4,[r5,#28]      ; m_nState
1393a8 cmp r4,#1
1393aa beq 1393fe          ; state 1 body
1393ac cbz r4,1393d4       ; state 0 body (loop condition)
1393ae b.w 13cacc          ; states 2..5 -> tbh
...
13cacc subs r0,r4,#2; cmp r0,#3; bhi 144e36; tbh   ; 4 entries: 2,3,4,5
```

Ours is a single 6-entry `tbh` at `0x1392d8` (`cmp r4,#5; bhi; tbh` states
0..5).  The original's state 0 body falls through into state 1 at `0x1393fe`;
the split is a switch-lowering decision (compare cluster 0/1 + table 2..5),
not a source-order one.  Converting the source to an if/else chain with a
`goto` for the state-1 `break` canonicalises back to the same 6-entry table
(54,015 raw / 57,232 B).  Splitting it into two `switch` statements is
**+24 B** (57,256) and raw-worse (54,924).

## 5. Case-154 dense state chain: original is in source order

`tbh` @`0x13e8f0` (orig) / `0x13a890` (ours), states 1..10:

```
orig  s1=13e908 s2=142f34 s3=1430d6 s4=143138 s5=1432c2 s6=14330e
      s7=14349c s8=1434c0 s9=1434f2 s10=14354c
ours  s1=13a8a8 s2=143070 s3=142f44 s4=143278 s5=142f6e s6=143440
      s7=1435d0 s8=1435f4 s9=143626 s10=142fb6
```

Physical order:

```
orig  1, 2, 3, 4, 5, 6, 7, 8, 9, 10   (ascending)
ours  1, 3, 5, 10, 2, 4, 6, 7, 8, 9
```

The source else-if chain is ascending in both; the reorder is a
MachineBlockPlacement outcome (expect weights in this arm were already swept
by the wave-168 expect pass).  All other per-mission `tbh` tables match:

| switch | orig physical order | ours |
|---|---|---|
| mission 89 (0x13866a/0x138648) | 1,5,3,2,4 | same |
| case 73 (0x13983e/0x139780) | 0,1,2,3 | same |
| case 92 (0x13a024/0x13a1d0) | 1,2,3,4,5,6,7 | same |
| case 94 (0x13acb6/0x13acc6) | 0,2,1,3 | same |
| case 105 (0x13aeec/0x13af20) | 1..10 | same |
| case 157 (0x13e81e/0x13a80a) | 1,3,4,5,2,6 | same |
| case 80 (0x143d28/0x143be0) | 8,9,10,11,12 | same |
| mission 0 (0x139b22/0x139054) | 6,10,7,8,9,11,12,13,15,14 | 9,15,6,7,8,10,11,12,13,14 (owned) |

## 6. Measured variants (full gold link, `-j2`)

| # | shape | raw | aligned | size | note |
|---|---|---:|---:|---:|---|
| 0 | HEAD | **54,015** | **17,788** | **57,232** | baseline |
| A | mission-78 exit `if` moved after switch (variant B of w11306) | 55,544 | 20,321 | 57,264 (+32) | 7 mission-78 dir. mismatches fixed |
| B | A + mission-0 case-11 `enableFillScreen` removed | 55,201 | 20,320 | 57,240 (+8) | **needs -8 B companion** |
| C | B + case-12 dead `m_nState = 13` removed | 55,201 | 20,320 | 57,240 (+8) | dead store is free |
| D | case-145 split into two switches | 54,924 | 18,105 | 57,256 (+24) | no |
| E | case-145 if/else + `goto` | 54,015 | 17,788 | 57,232 | canonicalises to same table |
| F | mission-5 `else if` -> `if` | 55,335 | 18,253 | 57,204 (-28) | no |
| G | move mission 89 into pre-switch specials | 55,526 | 19,718 | 57,316 (+84) | no |
| H | move mission 50 before mission 89 | 55,787 | 19,915 | 57,500 (+268) | no |
| I | swap mission 5 and mission 81 | 55,552 | 20,786 | 57,284 (+52) | no |

Variant B is the closest structural win: it reproduces the original
mission-78 switch-exit block (case 8 falls into `subs #5; cmp #3; bhi`) and
removes the spurious mission-0 case-11 `enableFillScreen`, but is +8 B on this
base.  No -8 B companion was found in the time-box; the mission-78 arm is
already *missing* five `TargetFollowCamera::translate` calls relative to the
original (see w11306), so the +8 is not a simple extra statement.

## 7. Block-order inversion list (unique-signature pairs)

The matcher reports 37 physical-order inversions between matched blocks.
The largest, by source region:

```
orig 0x13814a -> ours 0x138264   then orig 0x13821e -> ours 0x13818e   (mission 5/81)
orig 0x138312 -> ours 0x1384f8   then orig 0x1383a0 -> ours 0x138342   (mission 26/78)
orig 0x13b7e0 -> ours 0x142d86   then orig 0x13b7f2 -> ours 0x13b598   (mission-78 s12)
orig 0x13b900 -> ours 0x13b7ac   then orig 0x13ba42 -> ours 0x13b76e   (mission-78 s3/s2)
orig 0x13baa6 -> ours 0x13b84e   then orig 0x13bacc -> ours 0x13b7e8   (mission-78 s4/s8)
orig 0x13d0fe -> ours 0x13d0fe   then orig 0x13d1de -> ours 0x13ce36   (mission-0 case 9)
orig 0x13e790 -> ours 0x13da46   then orig 0x13e908 -> ours 0x13a8a8   (case 154 s1)
orig 0x142e60 -> ours 0x142e98   then orig 0x142f10 -> ours 0x142e1e   (case 154 s2/s3)
orig 0x1434ac -> ours 0x1435e0   then orig 0x14355a -> ours 0x142fc4   (case 154 s7/s10)
orig 0x143782 -> ours 0x145b30   then orig 0x1438a4 -> ours 0x143760   (case 154 tail)
```

The full 37-pair list is reproducible with `/dev/shm/w11406/match.py`
(scratch, not tracked); the addresses above are the ones that map to the
regions owned by this pass.


## 7b. The PLT-order anchor is an 8-byte lever

The `if (delta == 0x7d) ...setStanding(0, 100)` anchor in the case-41/42
chain is exactly **8 B**: removing it alone gives 57,224 B (raw 54,607).
That is the size of the variant-B overhang (+8), so `variant B + case-11 fix
+ anchor removal` is size-exact at **57,232 B**, but its raw is 55,365 and
`.rel.plt` differs by 51 bytes (the anchor is tuned for the HEAD layout, not
the variant-B layout).  Making the anchor unconditional keeps size 57,232
but `.rel.plt` differs by 52 bytes and raw is 55,403.  So the anchor is a
real size lever, but it needs a relocation-order retune and variant B is
still raw-worse than HEAD; not landed.

## 8. Conclusion

The outer mission-dispatch compare chain is matched; the remaining outer
block-order difference is the physical interleaving of the mission
5/81/26/78/50/89/51 bodies.  The per-mission switch arms that are *not*
already owned by the mission-0 / mission-78 workers are structurally matched
except case 145's split dispatch and case 154's dense-chain order; both are
switch-lowering/MachineBlockPlacement outcomes, not source-order ones.
No size-neutral raw win was found.  Tree left clean at the entry baseline.

## Files changed

* `_work/reconstructions/levelscript-process-blockorder-wave169-w11406.md` —
  this note.
* No `src/`, `CMakeLists.txt`, baseline or allowlist change.
