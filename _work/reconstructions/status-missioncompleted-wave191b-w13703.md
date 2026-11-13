# `Status::missionCompleted` — wave 191b (w13703): body-spelling + TBH-weight re-sweep

Worktree `wt/w13703`, branch `wave191b-w13703`, base master `21aadcc74`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13703`.

**Status: research note, no `src/` change landed.**  Baseline re-confirmed on
the committed source: raw **71**, aligned **38**, size **1,432/1,432** exact,
527/527 instructions.  Whole-file section deltas: `.text` **71,339**,
`.ARM.extab` **577**, `.ARM.exidx` **0**, `.rodata` **0**, `.rel.plt` **0**;
all 27 section sizes/VMAs identical.  `check` re-run in this worktree:
`RATCHET PASS` (byte_exact 4508, linked_exact 4508, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).
Tree clean.

## 0. Residual map (re-measured)

Byte-diff runs inside the 1,432-byte function:

| region | diff bytes | VMA |
|---|---:|---|
| 40-entry TBH table | 24 | `abff2..ac036` |
| production-good body (source `case 158`) | 46 | `ac266..ac2a5` |
| literal pool (`Globals::status` GOT offset) | 1 | `ac4c4` |

The literal at `ac4c4` is `0x00163ece` (orig) vs `0x00163eca` (ours): the
`Globals::status` GOT slot resolves to `0x20f15c` vs `0x20f158`, i.e. a
4-byte GOT-layout difference, not a body-layout consequence.  It is not
independently addressable from this TU.

The body diff is exactly 46 bytes inside a **same-size 70-byte body**
(`ac260..ac2a6`): the first two instructions (`mov r0,sl`,
`blx getProductionGoodIndex`) are already byte-identical, and the whole
`Globals::status->ship->hasEquipment` block from `ac288` onward matches
the original's instruction *shape* except for the final test.

## 1. Original body vs committed body

Original (`ac260..ac2a5`, body of original type 166):

```
ac260 mov r0,sl ; blx getProductionGoodIndex ; mov r9,r0    index -> r9
ac268 mov r0,sl ; blx getProductionGoodAmount; mov r5,r0    amount -> r5
ac270 ldr r0,[r6,#400]; blx getCargo         ; mov r2,r0    cargo -> r2
ac27a mov r0,r9 ; mov r1,r5 ; blx isInList
ac282 cmp r0,#0 ; bne.w ac49e
ac288 ldr r0,[pc,#568]; add r0,pc; ldr r0,[r0]; ldr r0,[r0]
ac290 ldr r5,[r0,#400]                                      ship -> r5
ac294 mov r0,sl ; blx getProductionGoodIndex ; mov r1,r0    re-query
ac29c mov r0,r5 ; movs r2,#1 ; blx hasEquipment
ac2a4 b.n ac458                     <-- shared `cbnz r0, ac49e` test
```

Committed (`index, cargo, amount` + `!hasEquipment(index,1)`):

```
ac260 mov r0,sl ; blx getProductionGoodIndex ; mov r5,r0    index -> r5
ac268 ldr r0,[r6,#400]; blx getCargo ; mov r6,r0            cargo -> r6 (clobbers this)
ac272 mov r0,sl ; blx getProductionGoodAmount ; mov r1,r0
ac27a mov r0,r5 ; mov r2,r6 ; blx isInList
ac282 cmp r0,#0 ; bne.w ac49e
ac288 ldr r0,[pc,#568]; mov r1,r5; movs r2,#1; add r0,pc; ldr r0,[r0]; ldr r0,[r0]; ldr r0,[r0,#400]
ac298 blx hasEquipment
ac29c ldr r6,[sp,#16]               <-- this reload
ac29e cmp r0,#0 ; bne.w ac45a ; b.n ac49e
```

The original keeps `this` in `r6`, puts `index` in `r9`, `amount` in
`r5`, `cargo` in `r2`, and re-queries `getProductionGoodIndex()` for
`hasEquipment`.  The committed body trades that allocation for a
`this` reload and a negated tail, which is the only shape that keeps
1,432 B.

## 2. Body-spelling sweep (~100 variants; `raw / aligned / size`)

All measured with `tools/fn_diff.py` on `case 158` only; everything else
untouched.  `.ARM.extab` content stayed **577** on every candidate
measured (size/base unchanged), so none is an extab trade.

| shape | raw | al | size |
|---|---:|---:|---:|
| committed `i,c,a` + `!hasEquipment(index,1)` | **71** | **38** | **1432** |
| `a,c,i` + `!hasEquipment(index,1)` | 72 | 39 | 1432 |
| `c,i,a` + `!hasEquipment(index,1)` | 77 | 35 | 1432 |
| `c,a,i` + `!hasEquipment(index,1)` | 77 | 40 | 1432 |
| `i,a,c` + `idxr` + positive `hasEquipment(index,1)` | 77 | 39 | 1432 |
| `i,a,c` + `!hasEquipment(index,1)` | 412 | 46 | 1432 |
| `i,c,a` + positive re-query (`pos_glob_ica`) | 384 | 43 | **1432** |
| `i,a,c` + positive re-query (`faithful`) | 682 | 145 | **1436** |
| `i,c,a` + `!re-query` | 628 | 143 | **1440** |
| `i,a,c` + `int index2` re-query local | 414 | 51 | **1432** |
| `i,c,a` + positive `hasEquipment(index,1)` | 664 | 153 | **1424** |
| `i,a,c` + `Ship *s = ship` + positive re-query | 694 | 139 | **1436** |
| `i,a,c` + `ship->hasEquipment(re-query)` | 416 | 125 | **1428** |
| `i,a,c` + `__builtin_expect(isInList,0/1)` | 682/660 | 145/143 | 1436/1440 |
| `i,a,c` + `__builtin_expect(hasEquipment,0/1)` | 682/660 | 145/143 | 1436/1440 |
| `i,a,c` + `unsigned`/`long`/`const` locals | 682 | 145 | 1436 |
| `i,a,c` + nested-if / `&&` / `||` / `else if` | 678–682 | 143–145 | 1436 |
| inverted `!isInList` nested / `&&` | 678 | 143 | 1436 |
| `i,a,c` + `decl_first` (declare then assign) | 660 | 143 | 1440 |

The committed spelling is a strict local optimum: every faithful
`i,a,c` + re-query shape is **+4 B** (1,436) because RAGreedy keeps
`amount` in `r6` and spills `this` to `r1` (`mov r1,r6`), or **+8 B**
when the tail is negated.  The `i,c,a` + positive re-query shape
(`pos_glob_ica`) is size-exact and reproduces the original's whole
`ac288..ac2a0` hasEquipment block byte-for-byte, but its prefix still
uses `r5/r6` for `index/cargo` and its tail is
`cmp r0,#0; b.n ac1d4` (4 B) instead of `b.n ac458` (2 B), so raw is
384, not < 71.

Also re-tested and inert: swapping `case 167/174`, swapping
`case 0/11`, moving the `167/174` block before `case 150` — all
raw 71 / size 1432.

## 3. TBH-weight re-sweep (relabelled source)

The body order must be the original tiered order
`167/174, 8,11,13,14, 150..155, 158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172`.  MBP places jump-table
successors in ascending value order, so the tier split needs edge
probabilities.  Re-measured (all on the relabelled source):

| variant | raw | al | size | notes |
|---|---:|---:|---:|---|
| committed (unrelabelled) | **71** | **38** | **1432** | baseline |
| relabel only / 3-level nest, no expect | 904 | 342 | 1424 | ascending order |
| nested inner `__builtin_expect(mtype,156)` | 1288 | 386 | 1368 | tier order, 64 B shrink |
| nested inner expect + `cargo2` + re-query | 1292 | 377 | 1428 | 1 insn short |
| 3-level nest, middle expect 150 | 1343 | 606 | 1380 | low-range tested first |
| 3-level nest, outer expect 167 | 1301 | 518 | 1420 | **exact tier order**, low group `tbb`, 13/14 late |
| single switch, committed labels, expect 8 | 1236 | 531 | 1400 | best single-switch expect |
| single switch, committed labels, expect 167 | 1284 | 485 | 1424 | |
| flat `!prof` injection (w13640) | **47** | 19 | **1432** | TBH exact; not source-expressible |

The outer-expect-167 variant (`gen5`) does produce the exact original
body order for the 16 main bodies, but the extreme 2000:1 weight moves
the low group (13/14) after 189, which turns the low table into `tbb`
and shortens the branch encodings; size is 1,420 and raw 1,301.  The
mild flat plateau (default 1, low 4, tier2 3, tier3 2) is the only
weight vector known to give both the tier order and 1,432 B, and clang
7's `LowerExpectIntrinsic::handleSwitchExpect` is one-hot 2000:1.

## 4. Conclusion

* TBH (24 B): needs the original tiered body order.  Reachable from
  source only through one-hot `__builtin_expect`, which perturbs the
  whole-function register allocation (raw 1,236–1,343) and does not
  keep 1,432 B.
* Body (46 B): the committed `i,c,a` + negated `hasEquipment(index,1)`
  shape is the unique size-exact local optimum at raw 71.  The faithful
  `i,a,c` + re-query shape is 1,436 B (this-spill `mov r1,r6`).
* Literal (1 B): `Globals::status` GOT slot differs by 4
  (`0x20f158` vs `0x20f15c`), a link/GOT-layout delta.

No size-exact shape below raw 71 was found, so `src/` is left unchanged.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13703
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=71 aligned=38 size=1432/1432
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13703/`: `fn_base.txt`,
`Status.base.cpp`, `gen2.py`/`gen3.py`/`gen4.py`/`gen5.py` (relabel /
nested-switch builders), `gen158.py`, `sweep158.py`, `sweep2.py`–`sweep10.py`
(body-spelling sweeps), `installfn.py`, `build.sh`, `decode.py` (TBH
decoder), `adiff.py` (instruction aligner), `measure_extab.py`,
`orig.dis`/`cand*.dis`, `LowerExpectIntrinsic.cpp`, `SimplifyCFG.cpp`,
`MBP.cpp`, `RAGreedy.cpp`, `SDB.cpp`.
