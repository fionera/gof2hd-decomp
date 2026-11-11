# Status::missionCompleted — case-order / local-order sweep, wave 177 (w12118)

Status: **analysis only, no `src/` change landed** (no size-exact raw win
found).  Worktree `wt/w12118`, branch `wave177-w12118`, base master
`9445ac5fb` (wave 176).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12118`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN6Status16missionCompletedEbbx` (1,432 B).  Entry state (wave-176
landed shape, `case 158` locals `hasSpecial; j; hasGood`): **raw 721 /
aligned 327 / 1,432 B**, insn 529/527.  Gate on entry: byte_exact 4507,
linked_exact 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Read first: `status-432-producers-wave176-w12016.md`,
`status-432-producers-wave175-w11918.md`,
`status-432-producers-wave173-w11819.md`,
`status-switch-nested-wave161-w10715.md`.

## 0. Bottom line

1. **No size-exact raw win was found.**  Every local declaration order,
   statement order, `&&`/`||` operand order, case order, switch-expression
   variant and nested-switch shape tried this wave is either raw >= 721 or
   not 1,432 B.  The raw landscape is flat at 721: 382 of the 462
   single-case moves measure exactly 721, the rest 722/723 or much worse.
2. **The one measured improvement is aligned-only:** declaring `amount`
   before `index` inside `case 166` (and only there) gives **raw 721
   unchanged, aligned 327 -> 326**, size 1,432 B.  It was not landed because
   this wave's instruction is to land only *raw* wins; it is recorded here
   for a future aligned pass.
3. **The residual is entirely block order.**  The first 0x248 bytes of the
   function (through `case 155`, the low switch and the `isCampaignMission`
   guard) are byte-identical.  The original lays the 150..189 jump-table
   targets out **hot-then-cold**:
   `150..155, 158, 162, 166, 170, 184, 189, 156, 157, 160, 163, 164, 165,
   168, 169, 171, 172, default`;
   our build lays them out **ascending by case value**
   (`150..155, 156, 157, 158, 160, 162, ...`).  Source case order does not
   move the jump-table blocks (verified again this wave: moving `156` to the
   end of the source still emits the identical ascending layout), because
   `buildJumpTable` iterates `sortAndRangeify`-sorted clusters.
4. **The nested-default shape reaches the original order but not the
   original size.**  Moving the hot cases into the outer switch and the cold
   cases into its `default:` switch, with
   `switch (__builtin_expect(mtype, 167))`, reproduces the original
   hot-then-cold order (wave-161 shape), but the continue/default block is
   placed second (`ac05c`) instead of last (`ac45a`), so the function is
   1,392 B (40 B short).  Every `-mllvm -likely/unlikely-branch-weight`
   pair and inner-`expect` cell tried this wave either keeps the 1,392 B
   shape or lands at 1,376-1,444 B; none is 1,432 B with raw < 721.

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.  All shapes keep the
case bodies verbatim unless noted.

### 1a. `case 158` declaration orders

All 24 permutations of `{equipment, hasSpecial, j, hasGood}` were tried
(equipment must precede the loop).  12 are raw 721 / size 1,432; 12 are raw
722.  The best aligned among the raw-721 shapes is the landed wave-176
order (`hasSpecial; j; hasGood`, aligned 327).  Representative rows:

| order | raw | aligned | size |
|---|---:|---:|---:|
| **base `equipment; hasSpecial; j; hasGood`** | **721** | **327** | **1432** |
| `equipment; hasSpecial; hasGood; j` | 722 | 323 | 1432 |
| `equipment; hasGood; hasSpecial; j` | 722 | 320 | 1432 |
| `equipment; hasGood; j; hasSpecial` | 722 | 322 | 1432 |
| `equipment; j; hasSpecial; hasGood` | 721 | 329 | 1432 |
| `hasSpecial; equipment; j; hasGood` | 721 | 327 | 1432 |
| `hasSpecial; j; equipment; hasGood` | 721 | 327 | 1432 |
| `hasSpecial; j; hasGood; equipment` | 721 | 327 | 1432 |
| `int hasSpecial/hasGood` (instead of `bool`) | 897 | 346 | 1428 |
| `hasGood & hasSpecial` / `!!` / `== true` | 721 | 327 | 1432 |

### 1b. Local declaration / statement orders in the other bodies

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base | 721 | 327 | 1432 |
| **`case 166`: `amount` before `index`** | **721** | **326** | **1432** |
| `case 166`: `const int` / inline / `unsigned` | 721 | 327 | 1432 |
| `case 166`: `amount` before `index` (global replace, also 8/172) | 726 | 332 | 1432 |
| `case 8`: `amount` before `index` | 727 | 329 | 1432 |
| `case 172`: `amount` before `index` | 1070 | 319 | 1428 |
| `case 162`: `I,Q,C` / `I,C,Q` / `C,I,Q` | 721 | 327 | 1432 |
| `case 162`: `Q,I,C` / `Q,C,I` / `C,Q,I` | 865 | 307 | 1428 |
| `case 152/157/189/163/8`: hoist `unsigned j` before the array | 721 | 327 | 1432 |
| `case 8`: swap the campaign block and the `atStation` block | 1280 | 432 | 1440 |
| `case 160`: swap the two `if` blocks | 1366 | 371 | 1448 |
| `extra > 2000` -> `extra >= 2001`, `extra > 10000` -> `>= 10001` | 721 | 327 | 1432 |

### 1c. Condition / loop / guard orders

All 60 distinct `&&` permutations of the `!isCampaignMission() &&
getAgent()==0 && getClientImage()==0 && getTargetStation()==0 &&
getTargetStation()==0` guard were measured.  The original left-to-right
order is the unique best: **721**; every other permutation is 723-730.
Other rows:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base | 721 | 327 | 1432 |
| `notAtStation = atStation == false` | 721 | 327 | 1432 |
| `hasWon() != 0`, `!m`, `i = i + 1`, `++i` | 721 | 327 | 1432 |
| swap `hasWon()` / `m == 0` blocks | 738 | 330 | 1432 |
| `case 160`: swap `!atStation && ... && extra > 10000` operands | 723 | 327 | 1432 |
| `case 8` pending-product `&&` operand swaps | 800-827 | 334 | 1436 |
| `case 152/157/189` equipment `&&` operand swaps | 867-1183 | 350-429 | 1440-1476 |
| `case 184` `||` operand swap | (not 1432) | | |

### 1d. Case order, switch expression and outer-switch variants

* Every single-case move (462) was re-measured on the wave-176 base:
  raw distribution `{721: 382, 722: 22, 723: 26, 858..1180: 32}`.  No move
  is below 721.  Moving the cold cases to the end of the source
  (`end-156`, `end-157`, ...) keeps raw 721 and the identical ascending
  machine layout (aligned 331 for most; `end-172` 721/327).
* `switch (mtype - 150)`, `switch ((unsigned)(mtype - 150))`,
  `switch (mtype - 8)`, outer `switch (mtype - 8/150/167)`:
  721/327/1432 (compiler normalises).
* `switch ((short)mtype)`: 1328/683/1436.  `switch ((unsigned char)mtype)`:
  1327/680/1432.
* Outer-switch single-case moves (42): best 721; moving `0`/`11` is 738.
* `case 167/174` and `case 0/11` label swaps: 721/327.
* Explicit `default: break;` / `default: continue;` in the inner switch,
  and `default:` first: 721/327.

### 1e. Nested-default / hot-cold shapes and weight flags

| shape | raw | aligned | size | order |
|---|---:|---:|---:|---|
| nested hot outer + cold `default:` switch, `expect(mtype,167)` | 1300 | 503 | 1392 | original, default 2nd |
| same, `default: break/continue`, cold cases reversed | 1300 | 503 | 1392 | original, default 2nd |
| same, `if (mtype < 156 || mtype > 172) break;` guard | 1311 | 662 | 1404 | original, default 2nd |
| same, inner `expect(mtype,156/172/999)` | 1296-1318 | 516-703 | 1384-1388 | original, default 2nd |
| same, outer `expect(mtype,158)` | 1275 | 515 | 1352 | scrambled |
| same, outer `expect(mtype,150)` | 1322 | 459 | 1396 | scrambled |
| same, no outer expect (merges back) | 722 | 322 | 1432 | ascending |
| sequential hot switch + cold switch | 1232 | 341 | 1504 | original |
| lookup-table dense-index reorder (`missionOrder[40]`) | 1331 | 462 | 1348 | original |
| `-mllvm -unlikely-branch-weight=0/2/3/5/10` on nested | 1265-1328 | 466-779 | 1376-1420 | |
| `-mllvm -likely-branch-weight=2..100` on nested | 1307-1325 | 367-462 | 1384-1412 | |
| `-mllvm -align-all-nofallthru-blocks=2` on nested | 1293 | 521 | 1424 | |
| outer+inner `expect` pairs, L=2..10, U=1 | 1183-1308 | 385-648 | 1400-1432 | |
| **best 1,432 B expect pair (`co158 ci158 L10 U1`)** | **1183** | **385** | **1432** | shifted head |

The 1,432 B expect-pair rows are not raw wins: the weight metadata changes
the block placement of the *head* (the 167/174 and low-switch blocks), so
the matching `ac110..ac180` prefix is lost.

## 2. Why the raw metric is flat

The same-address diff of the wave-176 base is:

```
abff2-abff6   jump-table entries for 156/157/158
abffa         entry for 160
abffe-ac006   entries for 163..166
ac00a-ac012   entries for 169..172
ac02a, ac034  entries for 182/184
ac068         ldr literal offset (constant-pool address)
ac0bc, ac0c8, ac0da, ac10e   branch targets into the tail
ac180-ac44c   the whole hot/cold tail (block order)
ac450-ac458   tail
ac4c4-ac4cc   constant-pool words
```

The head through `ac180` matches because the blocks before `case 156` have
the same addresses and sizes.  The tail differs because the 22 jump-table
targets are laid out in a different order.  A same-address shift scan over
the tail (`shift -64..64`) shows shift 0 is by far the best (156/848 bytes
match; the next best is 57/810), so no small uniform shift helps either.

A per-block simulation (take each case body from our build and compare it
with the original body at the original address) shows the bodies themselves
are close: `162` 86/102, `158` 58/86, `166` 41/100, `184` 5/48, `189` 5/54.
So block order, not body code, is the dominant residual.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12118
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
# -> raw=721 aligned=327 size=1432/1432 insn=529/527
cmake --build cmake-build-match --target check -j2
```

Section check on the base tree: `.text` 79,591 differing bytes (size/base
exact); `.ARM.exidx` 0; `.ARM.extab` 1,321 (size/base exact); `.rodata`,
`.data`, `.bss`, `.rel.plt` 0 (byte-identical); file size exact 2,187,328 B.

Scratch `/home/debian/GalaxyOnFire2/scratch/w12118/`: `harness.py` /
`sweep.py` (single-TU compile + real gold link + `fn_diff` in ~1 s),
`sweep158.py` / `sweep158b.py` / `sweep158c.py` / `sweep158d.py`
(`case 158` locals), `sweep_locals.py`, `sweep_conds.py`, `sweep_misc.py`,
`sweep_cases.py` (all 462 single-case moves), `sweep_cases2.py`,
`sweep_outercases.py`, `sweep_outerdef.py`, `sweep_shared.py`,
`sweep_expr.py`, `sweep_outerexpr.py`, `sweep_campchain.py` (60 guard
permutations), `sweep_stmts.py`, `sweep_eq.py`, `sweep_am.py`,
`random_search.py`, `mknested.py` / `mknested3.py` / `mktwo.py` /
`mkseq.py` / `mkidx.py`, `sweep_nested.py` / `sweep_nested2.py` /
`sweep_nested_flags.py` / `sweep_both.py` / `sweep_inner.py`,
`tbhmap2.py` (TBH decode), `blocksim.py`, `shift.py`, `analyze.py`.

## 4. Files changed

* `_work/reconstructions/status-432-producers-wave177-w12118.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
