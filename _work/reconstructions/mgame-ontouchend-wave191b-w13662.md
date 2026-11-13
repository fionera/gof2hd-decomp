# MGame::OnTouchEnd — wave191b w13662: mining-report `!= 0` semantics + faithful store orders

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13662`, branch
`wave191b-w13662`, base master `c43220fb8`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13662`.

Prior notes: `mgame-ontouchend-wave191b-w13626.md` (arm-81 body order, 96/127 split,
campaign-first conditions, mining `count >= 0` bug), `-w13622.md` (diff clusters,
arm-134 store swap), `-w13639.md` (arm-126/95/161 store orders), `-w13611.md`
(diff-cluster map).

## Result (landed)

Three changes, all size-exact 8,080:

1. **Mining-report predicate fixed.** The binary's report block at `0x17a5ca` is
   `cmp.w r9,#0; beq.n skip` — it skips the `"0 t ..."` line when `count == 0`.
   The source had `if (count >= 0)`, which emits `cmp.w r9,#0; bgt.n skip`
   (`amount` is `r9`; `count = -amount`), so it printed a zero line for every ore
   with no cargo. Changed to `if (count > 0)`. Since `amount` starts at 0 and is
   only decremented, `count >= 0` always holds, so `count > 0` is exactly the
   binary's `count != 0` predicate.
2. **Arm 126 faithful store order.** `Globals::switch_to_target_setting = 1;`
   moved to the head of the arm, matching the original's `strb` before the four
   `Status` getter stores (w13639 evidence). Pure independent stores.
3. **Cinematic faithful store order.** `setCinematicMode(true);` moved before
   `this->hudTouchFlags = 0;`, matching the original (w13626 evidence).
   `setCinematicMode` does not read `hudTouchFlags`.

| | raw | aligned | size | insn | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| before | **2,878** | 1,572 | 8,080/8,080 | 2,862/2,865 | 156,312 (577 diff) |
| after  | **2,884** | **1,545** | 8,080/8,080 | 2,862/2,865 | 156,312 (577 diff) |

Net: raw +6, aligned **-27**, size exact, `.ARM.extab` content unchanged.
`.text` differing bytes 71,212 -> 71,216 (+4); `.ARM.exidx`/`.rodata` unchanged.

`check` green: byte_exact 4508/4524, linked_exact 4508/4524, size-exact 4524/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370. `.rel.plt` unchanged.

## Why not `count != 0`

The literal binary predicate is `count != 0`, but every `!= 0` spelling flips
LLVM's induction variable from the decrementing `amount` (`r9`) to a positive
counter (`r8`) and costs **+8 B** (8,088), because the loop-exit block can no
longer use `ldrd sl,r8,[sp,#24]` and needs an extra `ldr.w r9,[sp,#24]`:

| condition | raw | aligned | size |
|---|---:|---:|---:|
| `count >= 0` (base) | **2,878** | 1,572 | 8,080 |
| `count != 0` | 6,637 | 2,188 | 8,088 |
| `count` / `amount != 0` / `amount` | 6,637 | 2,188 | 8,088 |
| `0 != count` / `-amount != 0` / `!!count` | 6,637 | 2,188 | 8,088 |
| `count != 0u` / `count != 0L` / `(unsigned)count != 0` | 6,637 | 2,188 | 8,088 |
| `unsigned count = -amount; count != 0` | 6,637 | 2,188 | 8,088 |
| `long long count = -amount; count != 0` | 6,637 | 2,188 | 8,088 |
| `short count = -amount; count != 0` | 4,641 | 1,829 | 8,092 |
| `count != 0` + `int refinedAny` | 6,207 | 2,002 | 8,076 |
| `count != 0 && count >= 0` | 2,880 | 1,573 | 8,080 |
| `count > 0` | 2,880 | 1,573 | 8,080 |
| `amount < 0` / `count >= 1` | 2,880 | 1,573 | 8,080 |
| `count != 0 && amount <= 0` | 2,880 | 1,573 | 8,080 |

`count > 0` is the size-exact form that keeps the original's negative counter
(`sub.w r9,r9,#1`, `rsb r1,r9,#0`) and register allocation. It emits
`cmp.w r9,#-1; bgt.n` instead of the binary's `cmp.w r9,#0; beq.n`; the two
extra differing bytes are the `#-1` immediate. The branch byte was already a
diff in the base, so the cost is exactly +2 raw.

### Exhaustive in-block loop/store-order sweep

All 24 permutations of the mining loop body
(`removeCargo`, `addCargo`, `amount--`, `refinedAny = true`) were built and
measured for both `count != 0` and `count > 0`:

* `count != 0`: sizes 8,008 / 8,048 / 8,088 — **no size-8,080 permutation**.
* `count > 0`: best size-exact result is 2,880 / 1,573 (the base order); every
  other size-exact permutation is >= 2,911 raw.

Declaration-order permutations (`String report` / `Ship *ship` / `bool refinedAny`),
`refinedAny` type (`bool`/`int`/`char`/`short`/`long`), `refinedAny = 1`, the
`fee` computation shape, the outer-loop form, and 40+ report-expression shapes
(`String line` splits, named `unit`, `const char *unit`, groupings, `report = report + ...`)
were also measured. None gives a size-8,080 `!= 0` form, and none beats
2,880 raw for `count > 0`.

## Compensation: faithful store orders

The semantic fix costs +1 aligned; the two faithful store orders recover it and
more. Measured on the current base:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,878** | 1,572 | 8,080 |
| mining `count > 0` only | 2,880 | 1,573 | 8,080 |
| arm 126 `switch=1` first | 2,881 | 1,550 | 8,080 |
| cinematic swap | 2,879 | 1,566 | 8,080 |
| arm 126 + cinematic | 2,882 | **1,544** | 8,080 |
| **all three (landed)** | **2,884** | **1,545** | 8,080 |

Arm 95 `switch=1` first (2,918 / 1,564) and arm 161 `switch=0` before
`initStream=0` (2,880 / 1,574) were re-measured but are worse on aligned.

## Conclusion

The mining-report zero-line bug is fixed with the size-exact `count > 0` form.
The literal `count != 0` is size-fatal (+8) because it flips the loop counter
sign; that is a clang induction-variable canonicalization, not a source-shape
choice (all `!= 0` spellings tested flip). The aligned residual is recovered
with two faithful independent-store reorderings (arm 126, cinematic block),
landing aligned 1,545 (-27) at raw 2,884 (+6), size exact and `.ARM.extab`
unchanged.

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13662`: `measure.sh` (build +
`tools/fn_diff.py` + extab), `sweep*.py` (condition/loop/report/type sweeps),
`sweep_cond.py` (campaign-first arms), `sweep_arms.py` (126/161/95),
`sweep_cin.py`, `sweep_combo.py` (final combinations), `clusters.py` (raw diff
clusters), `adiff.py` / `widthdiff.py` (instruction alignment / width diff).
