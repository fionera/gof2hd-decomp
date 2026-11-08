# Status::missionCompleted — 4/3/2 weight progression from source (round 3, wave 163, w10915)

Status: **analysis only, no `src/` win landed**. Worktree `wt/w10915`,
base master `e51db2fc7` (wave 162). Scratch `/dev/shm/w10915`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target `_ZN6Status16missionCompletedEbbx` (1432 B).  Baseline **raw 722 /
aligned 320 / 1432 B**.  Reference plateau (IR `!prof` injection,
default 1 / G1 4 / G2 3 / G3 2) **raw 558 / aligned 47 / 1432 B**.

Read first: `wave162-merge-tests.md`,
`status-switch-nested2-wave162-w10815.md`,
`status-switch-nested-wave161-w10715.md`,
`status-switch-profile-wave160-w10615.md`.

## 0. Bottom line

No source-only shape tried this wave is size-exact (1432 B) **and** a raw
win.  New measured facts:

1. **The 1432-B cell needs a strictly decreasing vector whose last group is
   `2` (strictly above `default 1`) with mild ratios.**  Injection sweep:
   `4/3/2`, `5/4/3`, `8/6/4` -> **558/47/1432**; `3/3/3` and `2/2/2` ->
   **722/320/1432** (i.e. the *baseline* layout, not source order);
   `3/3/2` and `4/4/2` -> 1372; `3/2/1` -> 1444; `2/2/1` -> 1372.
   So a flat or equal vector is a no-op, and `G3` must be >= 2.
2. **The nested-switch merge can only produce geometric vectors** with
   ratio = inner group size (11..13), which is far too steep.  A 4-level
   nesting is the closest source shape: **1400 B** (n4) and **1412 B**
   (best, see below), but the loop-latch `default` block lands in the
   middle of the table, not last.
3. **The literal "3-case outer switch + `__builtin_expect(outer,1)`"
   shape (a computed coarse class) is 1500 B** and its table order is
   `G2, G1, G3, default` -- the compiler folds the two switches together
   and the `!prof` is lost.
4. **A value-range `if (type <= X) ... else if` tree is worse**: the
   if-tree is folded back to a switch with no `!prof` and the function
   shrinks (1092..1364 B).

## 1. Injection cell map (baseline IR, `pgo_switchweight_w9920.py score`)

Vector = `[default, G1(7), G2(12), G3(10)]` in the switch's case order
(`167,174,8,0,11,13,14,150..189,156..172`).

| vector | raw | aligned | size |
|---|---:|---:|---:|
| **4/3/2** | **558** | **47** | **1432** |
| **5/4/3** | **558** | **47** | **1432** |
| **8/6/4** | **558** | **47** | **1432** |
| 2/2/2 | 722 | 320 | 1432 (baseline) |
| 3/3/3 | 722 | 320 | 1432 (baseline) |
| 3/3/2 | 1254 | 316 | 1372 |
| 4/4/2 | 1252 | 332 | 1372 |
| 2/2/1 | 1254 | 316 | 1372 |
| 3/2/1 | 1055 | 191 | 1444 |

This is the key new constraint: the `1432` cell is not "any decreasing
vector"; it needs `G3 >= 2` and a *strict* decrease `G1 > G2 > G3`.

## 2. Source shapes measured (all `src/` reverted afterwards)

`raw` = same-address differing bytes; `aligned` = instruction-index
alignment; `size` = function bytes.  Every shape keeps the 29 case bodies
verbatim; only the dispatch wrapper changes.

| shape | raw | aligned | size | table-0 body order |
|---|---:|---:|---:|---|
| baseline `switch (m->getType())` | 722 | 320 | 1432 | ascending, 167/174 hoisted |
| `switch (__builtin_expect(m->getType(), 167))` | 1277 | 632 | 1392 | 167/174, **default**, 150..189, 156..172 |
| nested default (G3) + `expect(mtype,167)` | 1260 | 503 | 1392 | 167/174, **default**, G2, G3 |
| 3-level nesting, no expect (`nested3`) | 722 | 320 | 1432 | **identical to baseline** |
| 3-level, L1 `expect(167)` (`n3`) | 1279 | 499 | 1384 | 167/174, **default**, G2, G3 |
| 3-level, L1 `expect(167)`, L2 `expect(150)` (`n3e`) | 1276 | 480 | 1380 | 167/174, **default**, G2(150 hot), G3 |
| 4-level, L1 `expect(167)` (`n4`) | 1267 | 453 | 1400 | 167/174, G2, **default**, G3 |
| 4-level, L1 `expect(167)`, L4 `expect(172)` (`n4x`) | 1267 | 453 | 1400 | same as n4 |
| 4-level, L1 `expect(167)`, L3 `expect(171)` (`n4y`) | 1303 | 467 | 1400 | 167/174, G2, **default**, G3(172 hot) |
| 4-level, L1 `expect(167)`, L3 `expect(156)` (`n4z`) | 1303 | 474 | 1400 | 167/174, G2, **default**, G3(156 hot) |
| 5-level, L1 `expect(167)` (`n5`) | 1277 | 523 | 1400 | 167/174, G2, **default**, G3 |
| **4-level L1=167, L2=plain, L3=156, L4=172** | **1317** | **478** | **1412** | 167/174, 189, 150..184, 170, 156, **default**, 157..171 |
| coarse class switch + `expect(outer,1)` | 1192 | 486 | 1500 | **G2, G1, G3, default** |
| value-range `if (mtype<=155) ... else if (<=172) ... else` (`if3`) | 1000 | 453 | 1092 | if-chains, no `!prof` |
| value-range `if (mtype<=14) ... else if (<=172) ... else` (`if3b`) | 1258 | 617 | 1364 | switch `{14,13,8,0,11}`, no `!prof` |

### 2a. The 4-level nesting is the closest source shape

`n4` is the only source shape that gets the *group order* `G1, G2, G3`
right (the `default` is the only misplaced block).  Its merged `!prof`
vector is

```
default 1, 167 260000, {174,8,0,11,13,14} 130,
{G2} 10, {G3} 1
```

i.e. `G1=130, G2=10, G3=1`.  The `default` ties with `G3` at 1 and
`MachineBlockPlacement` places it *before* G3 (right after G2).  Injecting
exactly this vector into the baseline IR gives **1384 B**, so the 1400 B
of the source shape is the structural change, not the vector.

The `1412`-B variant (`L1=167, L2=plain, L3=156, L4=172`) has the vector

```
default 1, 167 52234000, {174,8,0,11,13,14} 26117,
{G2} 2009, {G3} 1, 172 2000
```

It puts `172` (2000) last and `G3\{172}` at 1, so the table order becomes
`167/174, 189, 150..184, 170, 156, default, 157..171` -- still not source
order.

## 3. Why the arithmetic progression is still not source-reachable

* `LowerExpectIntrinsic::handleSwitchExpect` is one-hot
  (`Weights[0]` = default, `Weights[case+1]` = 2000, all others 1).
* `SimplifyCFG::FoldValueComparisonIntoPredecessors` merges a nested
  switch into its predecessor with
  `pred_case *= (sum of successor weights)` and
  `inner_case = pred_default * inner_case_weight`.
  With all-plain inner switches the successor weights are all 1, so the
  merged vector is geometric with ratio = number of inner cases
  (`G1 = n2*(n3+1)+1`, `G2 = n3+1`, `G3 = 1`, `default = 1`).
* `FitWeights` only shifts when `max > UINT_MAX`; the source vectors here
  top out at ~5.2e7, so no shift happens and `G3` stays tied with
  `default` at 1.
* To break that tie the innermost switch needs its case weight > its
  default weight, i.e. a `__builtin_expect` on a *case* of the innermost
  switch -- but that boosts only one case, not the whole group.

## 4. Open questions for wave 164

1. The 4-level nest (`n4`, 1400 B) is 32 B short only because the
   loop-latch `default` block is placed before G3.  A shape that makes
   `G3 > default` without changing the code would land it.
2. `FitWeights` is the only integer-level knob that can turn a tied
   `1` into `0`: force `max > 2^32` (two or three `__builtin_expect`
   levels) and check whether the shifted vector breaks the `G3/default`
   tie at 1432 B.  The 3-expect cells tried here (`n3e`, `167/150/156`)
   top out at 4.4e7, so more expect levels are needed.
3. The literal "3-case outer switch" shape folds both switches together
   and loses the `!prof`; a shape that keeps the two switches separate
   (e.g. a non-inlined helper) might keep the coarse weights.

## 5. Files changed

* `_work/reconstructions/status-432-progression-wave163-w10915.md` -- this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.

Scratch (`/dev/shm/w10915/`): `fdiff.py`, `align_diff.py`, `tbl.py`,
`irsw.py`, `ircheck.sh`, `cc_ll.sh`, `measure.py`, `mkvar.py`,
`mknest.py`, `mkif.py`, `sweepnest.py`, `Status.base.cpp`,
`Status.{onehot,nested_default,nested3,n3,n3e,n4,n4x,n4y,n4z,n5,coarse,iftree,if3,if3b,best}.cpp`.
