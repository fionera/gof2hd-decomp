# `Status::missionCompleted` — wave 191b (w13604): case-158 tail-merge / register-allocation sweep

Worktree `wt/w13604`, branch `wave191b-w13604`, base master `49a44a262`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13604`.

**Status: research note, no `src/` change landed.**  Baseline re-confirmed
on the committed source: raw **71**, aligned **38**, size
**1,432/1,432** exact, 527/527 instructions.  `.text` differing bytes
71,202, `.ARM.extab` 577, `.ARM.exidx` 0, `.rodata` 0.  Tree clean,
`check` green.

## 0. Per-region diff map (re-measured)

Byte-diff runs inside `abf38..ac4d0` (offsets relative to `abf38`):

| region | diff bytes | VMA |
|---|---:|---|
| 40-entry TBH table | 24 | `abfe6..ac034` |
| case-158 body | 46 | `ac266..ac2a5` |
| literal pool entry (`Globals::status` GOT offset) | 1 | `ac4c4` |

Everything else (prologue, dispatch `sub/cmp/bhi/tbh`, all other case
bodies, latch/return blocks) is byte-identical.  The 24 TBH bytes are
the only remaining dispatch delta: the table is a value→address
permutation in the original and the identity in ours (see §3).

## 1. The case-158 register allocation is the crux

Original `case 158` (`ac260..ac2a5`), 70 B:

```
mov r0,sl ; blx getProductionGoodIndex ; mov r9,r0      ; index -> r9
mov r0,sl ; blx getProductionGoodAmount; mov r5,r0      ; amount -> r5
ldr r0,[r6,#400]; blx getCargo; mov r2,r0               ; cargo -> r2
mov r0,r9 ; mov r1,r5 ; blx isInList
cmp r0,#0 ; bne.w ac49e
ldr r0,[pc,#568]; add r0,pc; ldr r0,[r0]; ldr r0,[r0]
ldr.w r5,[r0,#400]                                      ; ship -> r5
mov r0,sl ; blx getProductionGoodIndex ; mov r1,r0      ; re-query
mov r0,r5 ; movs r2,#1 ; blx hasEquipment
b.n ac458                                               ; shared cbnz block
```

`this` stays in `r6` (never clobbered), `index`→`r9`, `amount`→`r5`,
`cargo`→`r2`.  The `hasEquipment` tail is a **single** `b.n ac458`,
where `ac458` is the shared `cbnz r0, ac49e` test block also used by
the case-8 and case-189 `isInList` tails.

Our committed device (`index, cargo, amount` + `hasEquipment(index,1)`,
negated) is 70 B but differs in 46 bytes: `index`→`r5`, `amount`→`r1`,
`cargo`→`r6`, `this` reloaded from `[sp,#16]` at `ac29c`, and the tail
is `cmp r0,#0; bne.w ac45a; b.n ac49e` (no tail merge).

The faithful call order (`index, amount, cargo`, re-query) reproduces
the original's `r9/r5/r2` *prefix* only if `this` also stays in `r6`.
In our build clang instead puts `amount` in `r6`, spills `this`, and
emits `mov r1,r6` + a stack reload — the body is **1,436 B** (raw 682).
The +4 B is exactly `mov r1,r6` (2 B) plus the un-merged tail
`cmp r0,#0` (2 B).

### 1.1 Why the tail does not merge into `ac458`

`-print-machineinstrs` (scratch `fp_mi.txt`, 88 MB) on the faithful
`iac`+re-query build shows the branch folder (`After Control Flow
Optimizer`, line 838195) merging the case-158 `hasEquipment` block
`bb.66` into `bb.57` (the case-156 `hasGood && hasSpecial` tail), not
into `bb.103` (the shared `ac458` `cmp r0,#0; bne` block):

```
bb.66: ... hasEquipment; cmp r0,#0; b bb.57
bb.57: ldr r6,[sp,#16]; bne bb.87; b bb.104      ; case-156 tail
bb.103: cmp r0,#0; bne bb.87                      ; ac458, fallthrough bb.104
```

The common suffix with `bb.57` (`ldr r6; bne; b`) is 3 instructions and
`bb.57` precedes `bb.103` in layout order, so the branch folder picks
it first.  The original must have had no `ldr r6` in the tail (because
`this` was never clobbered), which makes the suffix `cmp; bne; b` and
lets `ac458` win.  So the tail-merge failure is a *consequence* of the
register allocation, not an independent lever.

## 2. Fresh-angle measurements (all semantic-equivalent)

`raw/aligned/size` from `tools/fn_diff.py`; size-exact means 1,432.

### (a) case-158 shape / tail

| shape | raw | al | size |
|---|---:|---:|---|
| committed: `index,cargo,amount` + `hasEquipment(index,1)` negated | **71** | 38 | 1432 |
| `index,amount,cargo` + re-query, positive (`iac`) | 682 | 145 | **1436** |
| `index,amount,cargo` + re-query, negative | 660 | 143 | 1440 |
| `index,cargo,amount` + re-query, positive (`ica`) | 384 | 43 | 1432 |
| `index,cargo,amount` + re-query, negative | 628 | 143 | 1440 |
| `amount,cargo,index` + `hasEquipment(index,1)` negated | 72 | 39 | 1432 |
| `amount,cargo,index` + re-query, positive | 90 | 32 | 1432 |
| `cargo,amount,index` + re-query, positive | 94 | 35 | 1432 |
| `index,amount,cargo` + `hasEquipment(index,1)` positive | 672 | 168 | 1428 |
| `index,amount,cargo` + `ship->hasEquipment(...)` re-query | 416 | 125 | 1428 |
| `index,amount,cargo` + `ship->hasEquipment(index,1)` | 666 | 150 | 1420 |
| swap the two `if`s (hasEquipment first), `ica`+re-query | 718 | 199 | 1436 |

Notable: **swapping the two `if`s** gives the original's
`index`→`r9`, `amount`→`r5` allocation (disassembly:
`mov r9,r0` / `mov r5,r0`, `ship`→`r4`, `m`→`fp`), but `cargo` then
lands in `r6` (live across the `hasEquipment` call) and the body is
1,436 B.  **Pre-computing the re-query** (`int index2 =
m->getProductionGoodIndex();` before the `isInList`) also gives
`index`→`r9` (`amount`→`r4`), size 1,436.  No size-exact shape with
`index`→`r9` was found.

### (b) declaration-order sweep

Assignment order is what matters; declaration order is inert (all six
declaration permutations of `{index,amount,cargo}` with the same
assignment order produce identical code).  With `hasEquipment(index,1)`
negated and size 1,432:

| assignment order | raw |
|---|---:|
| index, cargo, amount | **71** |
| amount, cargo, index | 72 |
| index, amount, cargo | 412 |
| cargo, index, amount | 412 |
| amount, index, cargo | 420 |
| cargo, amount, index | 420 |

### (c) block relocation (faithful `iac`+re-query shape)

Moving the whole `case 158:` block before another case: 1,436 B for
every position except before 8/150/151/152/167 (1,432 B, raw 722) and
before 174 (1,432 B, raw 1,206).  No position beats raw 71.

### (d) per-TU LLVM flag sweep (via `ft3.sh`)

No flag produced a size-exact body below raw 71.  Highlights:
`-disable-postra-machine-sink` → raw 142; `-regalloc=basic` → 1,356 B;
`pbqp` → 1,384 B; `fast` → 1,960 B; `-tail-merge-threshold=3` →
1,552 B; `-tail-merge-size=10` → 1,436 B; `-enable-tail-merge=false`
→ 1,636 B; `-disable-branch-fold` → 1,436 B.  `-disable-postra-machine-licm`,
`-enable-earlycse-memssa=false`, `-likely-branch-weight`,
`+cheap-predicable-cpsr`, `-regalloc=greedy`, `-regalloc-csr-first-time-cost`,
`-join-liveintervals=false`, `-split-spill-mode=*`,
`-enable-deferred-spilling`, `-optimize-regalloc=false`,
`-twoaddr-reschedule=false`, `-disable-early-taildup`,
`-branch-fold-placement=false` — all raw 71 unchanged.

## 3. TBH (24 B) and literal pool (1 B)

The TBH table is a value→address permutation in the original because
the original's *body order* differs from ours.  Original body order
(by VMA): `150 151 152 153 154 155 158 162 166 170 184 189 156 157 160
163 164 165 168 169 171 172`; ours: `150 151 152 153 154 155 156 157
158 160 162 163 164 165 166 168 169 170 171 172 184 189`.  MBP lays
bodies out in jump-table-successor (value) order, so a direct switch
always yields the identity table.  Reproducing the permutation requires
a case-value permutation, i.e. the old `.rodata` rank helper, which
measured raw 74 (w13569) — worse than the current direct-switch 71.
Source reorder to the original body order keeps the identity TBH and
raises raw to 110 (w13569 §3).

The single literal-pool byte at `0x58c` is the `Globals::status`
GOT-relative offset: original `0x00163ece`, ours `0x00163aca`.  The
4-byte difference is a `.got`/`.rel.dyn` entry-order artifact (the
`Globals::status` GOT slot is 4 B earlier in our link), not a
source-level PC-relative alignment; it is a consequence of the
case-158 body size and cannot be fixed independently.

## 4. Conclusion

The remaining 71 raw bytes are 24 TBH (needs the rank table, which
costs more than it saves) + 46 case-158 (pure greedy-allocator
register assignment: `r5/r1/r6` + `this` spill vs the original's
`r9/r5/r2` + `r6`) + 1 literal-pool byte (GOT layout).  The
tail-merge failure (`cmp; b.n ac1d4` instead of `b.n ac458`) is a
downstream effect of the `this` spill, not an independent lever.
No size-exact shape below raw 71 was found; `src/` is left unchanged.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13604
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=71 aligned=38 size=1432/1432
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13604/`: `Status.base.cpp`,
`ft3.sh`, `cc.sh`, `linkcmd.sh`, `diffmap.py`, `vsearch4.py`,
`bigsweep.py`, `sweep_decl.py`, `relocate.py`, `fp_mi.txt` (88 MB
`-print-machineinstrs` of the faithful `iac`+re-query build).
