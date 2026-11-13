# `Status::missionCompleted` — wave 191b (w13640): TBH provenance + case-166 body allocation

Worktree `wt/w13640`, branch `wave191b-w13640`, base master `57ce1a4c8`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13640`.

**Status: research note, no `src/` change landed.**  Baseline re-confirmed
on the committed source: raw **71**, aligned **38**, size **1,432/1,432**
exact, 527/527 instructions.  Whole-file section deltas: `.text`
**71,197**, `.ARM.extab` **577**, `.ARM.exidx` **0**, `.rodata` **0**,
`.rel.plt` **0** (sizes and VMAs identical for all 27 sections).  `check`
re-run in this worktree: `RATCHET PASS` (byte_exact 4508, linked_exact
4508, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370).  Tree clean.

## 0. Per-region diff map (re-measured)

| region | diff bytes | VMA |
|---|---:|---|
| 40-entry TBH table | 24 | `abff2..ac013` |
| production-good body (source `case 158`) | 46 | `ac266..ac2a5` |
| literal pool (`Globals::status` GOT offset) | 1 | `ac4c4` |

The literal-pool byte is **not** independent: `ldr r0,[pc,#568]` at
`ac288` is a 2-byte Thumb encoding, and `add r0,pc` sits at `ac28a`
(orig) / `ac28e` (ours).  Both resolve to the same GOT slot
`0x20f15c`; the 4-byte difference is exactly the extra instruction our
body carries before it.  So the whole residual is TBH + body.

## 1. TBH provenance: the first group is *not* a weight, it is a shared body

Decoding both TBHs (`base = abfe6`, `target = base + 2*entry`) gives the
original body order by VMA:

```
167/174, 150,151,152,153,154,155, 158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172, default
```

Ours is `167/174, 150..155, 156,157,158,160,162,...,189` (ascending
value).  **The two agree on the first two groups.**  The reason is
visible in `SelectionDAGBuilder::buildJumpTable`:

```cpp
JTProbs[Clusters[I].MBB] += Clusters[I].Prob;
```

`case 167: case 174:` share one MBB, so that destination's probability is
`2/40` while every single-value case is `1/40`.  `MachineBlockPlacement`
picks the highest-frequency successor first, so the shared body is
placed first *with no profile data at all*.  This is confirmed by our own
unweighted build, which already places `167/174` first.

The remaining delta is the split of the other 16 bodies into
`{158,162,166,170,184,189}` before `{156,157,160,163,164,165,168,169,171,172}`.
Within each group the order is ascending value, i.e. the MBP worklist
order, so the two groups must have **distinct block frequencies**.  A
flat `!prof` vector reproduces it exactly (see §3); no source-level
construct produces a 3-level flat vector (w9920 §0.2/§0.3).

## 2. The 46-byte body: original vs ours

Original (`ac260..ac2a5`, body of original type 166):

```
ac260 mov r0,sl ; blx getProductionGoodIndex ; mov r9,r0   index -> r9
ac268 mov r0,sl ; blx getProductionGoodAmount; mov r5,r0   amount -> r5
ac270 ldr r0,[r6,#400]; blx getCargo         ; mov r2,r0   cargo -> r2
ac27a mov r0,r9 ; mov r1,r5 ; blx isInList
ac282 cmp r0,#0 ; bne.w ac49e
ac288 ldr r0,[pc,#568] ; add r0,pc ; ldr r0,[r0] ; ldr r0,[r0]
ac290 ldr r5,[r0,#400]                                     ship -> r5
ac294 mov r0,sl ; blx getProductionGoodIndex ; mov r1,r0   re-query
ac29c mov r0,r5 ; movs r2,#1 ; blx hasEquipment
ac2a4 b.n ac458                    <-- tail-merged with case-8's cbnz test
```

`this` stays in `r6` for the whole body; the `hasEquipment` tail is a
single `b.n` into the shared `cbnz r0, ac49e` block at `ac458`.

Ours (committed source, `index,cargo,amount` + `!hasEquipment(index,1)`):

```
ac266 mov r5,r0        index -> r5
ac268 ldr r0,[r6,#400]; blx getCargo ; mov r6,r0   cargo -> r6 (clobbers this)
ac272 mov r0,sl ; blx getProductionGoodAmount ; mov r1,r0
ac27a mov r0,r5 ; mov r2,r6 ; blx isInList
ac29c ldr r6,[sp,#16]  <-- this reloaded
ac29e cmp r0,#0 ; bne.w ac45a ; b.n ac49e
```

### 2.1 The allocator *can* produce the original allocation

`-print-machineinstrs` (scratch `mi_vrr_new.txt`) shows that the
**case-8 body in our own build already uses the original's exact
assignment**:

```
bb.31 (case 8): renamable $r9 = COPY $r0    ; index
                renamable $r5 = COPY $r0    ; amount
                renamable $r2 = COPY $r0    ; cargo
                renamable $r0 = t2LDRi12 renamable $r6, 400   ; this->ship
```

The case-189 body (also byte-identical) uses the *other* assignment
(`index->r5`, `amount->r6`, `ldr r6,[sp,#16]`).  So the same function
contains both allocations; the case-166 body is the only one that gets
the "wrong" one.  The difference is not the `hasEquipment` block
(removing it still gives `r5/r6`), it is the greedy allocator's
whole-function live-range order (LLVM 7 `RAGreedy::enqueue`: local ranges
are ordered by `beginIndex().getInstrDistance(lastIndex)`, so
*earlier-starting* ranges are allocated first, and `this` is a single
global range in `r6` that the allocator may split).

### 2.2 Measured body shapes (all semantic-equivalent; size must stay 1432)

`raw / aligned / size` from `tools/fn_diff.py`; `bm` = differing bytes
of the 70-byte body at `ac260` (only meaningful when size is 1432).

| shape | raw | al | size | bm |
|---|---:|---:|---:|---:|
| committed `index,cargo,amount` + `!hasEquipment(index,1)` | **71** | **38** | **1432** | 46 |
| `index,cargo,amount` + `hasEquipment(re-query,1)` positive | 384 | 43 | 1432 | **21** |
| `index,amount,cargo` + `!hasEquipment(index,1)` | 412 | 46 | 1432 | 54 |
| `cargo,index,amount` + re-query positive | 390 | 44 | 1432 | 27 |
| `index,amount,cargo` + re-query positive (`iac`) | 682 | 145 | 1436 | – |
| `index,amount,cargo` + re-query negative | 660 | 143 | 1440 | – |
| `index,amount,cargo` + `hasEquipment(index,1)` positive | 672 | 168 | 1428 | – |
| `amount,index,cargo` + re-query positive | 686 | 144 | 1436 | – |
| `index,cargo,amount` + re-query negative | 628 | 143 | 1440 | – |
| `index,cargo,amount` + `hasEquipment(index,1)` positive | 664 | 153 | 1424 | – |
| `hasEquipment` first, then `isInList` | 670 | 143 | 1396 | – |
| `Ship *s = ship` / `bool ok` / `||` / `else if` / nested guard | 678–694 | 139–146 | 1436/1456 | – |
| `(void)amount;` / `amount>=0?1:1` (extend amount's range) | 660–682 | 143–145 | 1436/1440 | – |

A 60-variant `__builtin_expect` sweep (order × isInList-expect ×
hasEquipment-expect × re-query × polarity × outer-guard-expect) produced
**no size-exact result below raw 71**; every 1432 hit is the
`!hasEquipment(index,1)` family at raw 412.

Moving the whole `case 158:` block before each other case label (28
positions) gives 682/1436 for positions after `153`, and 722–743/1432
for positions before `153` (the layout itself changes there); none beats
71.  Re-ordering all case blocks into the original body order gives
1132/203/**1380**.

## 3. Relabel + flat `!prof` weights: raw 47, TBH byte-identical

The case labels in the committed source are a permutation of the
original's (w13618 §1).  Relabelling the bodies to the original types
(`Status.relabel.cpp`) and injecting the flat switch weights

```
default 1, {167,174,8,0,11,13,14}=4, group2=3, group3=2
```

on the optimized IR gives **raw 47, aligned 19, size 1432/1432,
527/527 instructions, TBH byte-identical**, and the remaining diff is
exactly the 46-byte body + the 1-byte literal.  This is the same result
w9920/w13618 reported (raw 558 before the relabel).

Relabel alone (no weights) is **904/342/1424** — a size regression, so
gate-fatal.  The weights are a `!prof` `branch_weights` vector on the
`switch` terminator; clang 7.0.2 cannot emit it from source
(`handleSwitchExpect` is one-hot 2000:1).  A checked-in profile is the
only route, and both profile flavours have problems:

| route | raw | al | size |
|---|---:|---:|---:|
| `!prof` injection (not source-expressible) | **47** | **19** | **1432** |
| sample profile `-fprofile-sample-use`, samples 3/3/3/2/1, relabelled source, `-mllvm -max-uses-for-sinking=0` | 1317 | 839 | 1432 |
| sample profile, unrelabelled source (w10020) | 1326 | 559 | 1440 |
| full `-fprofile-instr-use` (w9820) | – | – | body shrinks 15 insns |

The sample-profile loader on the relabelled source *does* keep the size
at 1432, but its propagated switch vector is
`[1,1,1,1,1,4,1,1,1,1,3,1,1,3,1,1,3,1,1,2,1,1,1,1,1,1,2,1,1,1]`
(scratch `sp.ll`), not the flat plateau: cases whose body is a
conditional `return m` get their samples attributed to the return edge,
so they stay at weight 1.  That is why its raw is 1317 and not 47.

## 4. Conclusion

* TBH (24 B): the first group (`167/174`) is explained by the shared
  body doubling the jump-table destination probability; the remaining
  `group2 > group3` split needs a 3-level flat `!prof` vector, i.e. a
  profile.  Not source-expressible.
* Body (46 B): pure greedy-allocator live-range ordering.  The original
  assignment (`index->r9, amount->r5, cargo->r2, this->r6`) is produced
  by the *case-8* body in the same function; no case-166 spelling tried
  reproduces it at 1432 B.
* Literal (1 B) is downstream of the body's instruction offsets.

No size-exact shape below raw 71 was found, so `src/` is left unchanged.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13640
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=71 aligned=38 size=1432/1432
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13640/`: `Status.base.cpp`,
`Status.relabel.cpp`, `measure.sh`, `regions.py`, `tbh.py`,
`sweep_body.py`, `sweep_big.py`, `sweep_combo.py`, `sweep_exp.py`,
`sweep_amt.py`, `sweep_pos3.py`, `reorder_cases.py`, `inject2.py`,
`cc_mi.sh`, `mi_vrr_new.txt` (post-RA MIR of the committed build),
`sp.py` + `sp/status.prof` (sample-profile replay), `sp.ll`.
