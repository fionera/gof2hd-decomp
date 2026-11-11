# Status::missionCompleted — if-chain/rank and group-index producers (wave 168, w11317)

Status: **analysis only, no `src/` win landed**.  Worktree `wt/w11317`,
branch `wave168-w11317`, base master `631c26f0b`.  Scratch `/dev/shm/w11317`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry/exit: baseline reproduced, target **raw 722 / aligned 320 /
1432 B**, `insn 529/527`.

Read first: `status-432-producers-wave167-w11216.md`,
`status-432-switchweights-wave166-w11116.md`,
`status-432-progression-wave163-w10915.md`,
`status-missioncompleted-wave117-w6513.md`.

## 0. Bottom line

Two further producer classes were implemented and measured: a hand-written
if-chain over the switch value / a precomputed rank (with split, i.e.
duplicated, `167`/`174` bodies), and a `switch` over a precomputed group
index with duplicated per-group bodies.  **None is size-exact (1432 B) and a
raw win.**  The new facts:

1. **A plain `if/else if` chain over `m->getType()` in source order folds
   back to the byte-identical switch** (`722/320/1432`).  clang's
   `FoldValueComparisonIntoPredecessors` normalises the plain chain's branch
   weights, so the chain is a no-op — the source order is *not* preserved.
2. **`__builtin_expect(cond, 1)` on every condition also folds to uniform
   weights** (every case 2000), so the body order is still ascending; the
   function shrinks to 1348 B.  `__builtin_expect(cond, 0)` on every
   condition is a complete no-op (`722/320/1432`).
3. **A chain with `__builtin_expect` on a single case does produce
   geometric branch weights** and *does* move the bodies into the original
   `G2` then `G3` order.  The IR weights are
   `0, -98566144, -98566144, -100663296, 1048576, 524288, ... 2, 1, 0`
   (default, then 167,174,150,151,...,172,8): `2^i` halving, which is the
   "too steep" ratio the wave-166 note already identified.  It reorders the
   bodies but shrinks the function (1396-1428 B) and moves the switch's
   default block (the low-type switch) *after* `G3` instead of second, so
   the whole body layout shifts and raw is worse than baseline.
4. **`gen9` (chain over the high types followed by a separate low-type
   `switch`) reproduces the original `G2`/`G3` body order exactly** at
   1428 B — the closest structural match found — but the low-type bodies
   are interleaved with the high bodies (`13`/`14` land at `ac424`), so the
   same-address raw diff is 1270, worse than the 722 baseline.
5. **A two-level group switch** (ternary group index + `switch (group)` with
   duplicated per-group bodies) is 1472 B / raw 1241; adding
   `__builtin_expect(group, 1)` gives 1388 B / raw 1315.
6. Boosting the default edge with `__builtin_expect(cond, 0)` on the last
   chain branch reallocates the fixed weight budget and underflows all
   earlier cases to 0, reverting the layout to ascending order.

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.  All shapes keep the
case bodies verbatim; only the dispatch wrapper changes.

| shape | raw | aligned | size | body order |
|---|---:|---:|---:|---|
| **baseline `switch (m->getType())`** | **722** | **320** | **1432** | ascending, 167/174 peeled |
| plain `if/else if` chain over `type`, source order | 722 | 320 | 1432 | identical to baseline |
| independent `if` chain + `continue` per body | 1472 | 695 | 1520 | source order, extra branches |
| chain with `__builtin_expect(_,1)` on every condition | 1321 | 657 | 1348 | ascending (uniform 2000) |
| chain with `__builtin_expect(_,0)` on every condition | 722 | 320 | 1432 | identical to baseline |
| `switch(group)` (ternary group index), duplicated bodies | 1241 | 341 | 1472 | G1, G2, G3 |
| `switch(expect(group,1))`, duplicated bodies | 1315 | 798 | 1388 | G1, G2, G3 |
| chain order 167/174,G2,G3,G1low, expect on 167/174+G2 | 1320 | 744 | 1388 | partially reordered |
| split 167/174, expect on 167,174,G2 | 1331 | 744 | 1392 | partially reordered |
| grouped, expect only on 150 (`specC`) | 1325 | 764 | 1396 | **G2, G3**, default after G3 |
| grouped, expect 150 + expect(_,0) on 8 | 1331 | 744 | 1392 | ascending (budget underflow) |
| split, expect(_,0) on 167 + expect(_,1) on 150 | 1320 | 772 | 1416 | mixed |
| `if (type>=150&&type<=189) { chain } else { low }` | 1304 | 453 | 1372 | ascending |
| same + expect 167/174+150 | 1274 | 468 | 1384 | ascending |
| `rank = type-150` + chain over rank | 1323 | 490 | 1364 | ascending |
| `if (type<150) { low } else { chain }` | 1316 | 542 | 1424 | ascending |
| `if (type<150) { low; continue }` then chain | 1316 | 542 | 1424 | ascending |
| **chain over high types, then separate low `switch`** | 1189 | 597 | 1408 | G2, G3 |
| **same + expect on 150 (`gen9`)** | **1270** | **652** | **1428** | **G2, G3 exact**, default misplaced |

## 2. What the body order needs (reconfirmed)

Original first-table body order (by address):

```
167/174, 150 151 152 153 154 155 158 162 166 170 184 189,
156 157 160 163 164 165 168 169 171 172, default(latch)
```

i.e. `G2` (12 bodies) then `G3` (10 bodies).  The low-type bodies
(`8, 0/11, 13, 14`) are emitted by the second `tbh` (`sub #8; cmp #6`)
*before* the `150..189` bodies, at `ac060..ac110`, in both the original and
the baseline.  So only the first table's successor order differs.

A chain gives the right order because its folded `!prof` weights decrease
along the chain (`G2 > G3`), but the same weights also make the switch's
default successor (the low-type switch) the *lowest*-weight successor, so
MBP places it after `G3` rather than second.  The original's default sits
second because with the PGO 4/3/2 vector the low-type switch block is placed
by the entry/compare chain, not by the switch edge weight; no source shape
tried here reproduces that.

`gen9`'s first table (decoded from the TBH) is exactly the original order:

```
167/174 ac034, 150 ac0c2, 151 ac134, 152 ac13a, 153 ac172, 154 ac178,
155 ac184, 158 ac18a, 162 ac1ec, 166 ac256, 170 ac2ba, 184 ac2c8,
189 ac302, 156 ac342, 157 ac36c, 160 ac3aa, 163 ac3d6, 164 ac3f4,
165 ac3fc, default ac460, 168 ac466, 169 ac47e, 171 ac482, 172 ac48c
```

but the high bodies start 78 B earlier than the original (`ac0c2` vs
`ac110`) and the low bodies are split (`8` at `ac062`, `0/11` at `ac0da`,
`13` at `ac424`, `14` at `ac430`), so the same-address diff is large.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11317
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
```

Scratch (`/dev/shm/w11317/`): `Status.base.cpp` (baseline source),
`gen.py`..`gen9.py` (chain / rank / group-switch generators), `gen2.py`
(group switch), `m.sh` (compile one TU + relink + fn_diff), `sweep*.py`,
`spec*.txt`, `status.ll`..`status3.ll` (IR weight dumps).

## 4. Files changed

* `_work/reconstructions/status-432-producers-wave168-w11317.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
