# Status::missionCompleted — switch-profile levers (wave 160, w10615)

Status: **analysis only, no `src/` / `CMakeLists.txt` change**. Worktree
`wt/w10615`, base master `08af4633b` (wave 159). Scratch `/dev/shm/w10615`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target: `_ZN6Status16missionCompletedEbbx` (1432 B).
Baseline: **raw 722 / aligned 320 / 1432 B**.
Reference plateau (post-optimisation IR `!prof` injection): **raw 558 /
aligned 47 / 1432 B**.

Read first: `status-profile-plateau-wave159-w10513.md`,
`pgo-profile-sweep-wave159-w10514.md`, `status-source-plateau-wave158-w10413.md`.

## 0. Bottom line

1. **No source lever tried this wave beats 722/320 at 1432 B.**  The plateau
   is still IR-injection-only.
2. **NEW: the 1432-B weight cell is an *arithmetic progression*, not a
   geometric one.**  Injection sweeps show `4/3/2`, `5/4/3`, `6/5/4`,
   `8/6/4`, `17/13/9`, `20/15/10`, `100/99/98` all give **558/47/1432**,
   while `4/2/1`, `3/2/1`, `5/3/2`, `7/5/3`, `9/6/3`, `10/7/4`,
   `4/3/1`, `4/4/2`, `4/2/2` and a full 29-level descending rank all miss
   the size.  A *nearly flat* strictly-decreasing geometric vector
   (ratio 0.99) does hit **561/51/1432**; steeper geometric vectors
   (ratio 2, 0.95, 0.8) do not.  So the original profile is a coarse
   three-level (or nearly-flat) sample profile, and the if-chain fold's
   geometric weights are the wrong shape.
3. **Lever (a) is a no-op.**  `int mtype = m->getType(); switch (mtype)`
   (and `unsigned`) is byte-identical to the direct switch.  `short` keeps
   the size but is worse (1093/352); `unsigned char` changes the size.
4. **Lever (b): an if/else-if chain folds back to the *identical* switch**
   (722/320/1432) when it has no `__builtin_expect`; with expects it folds
   to a single switch with geometric weights at 1348/1400 B, not 1432.
5. **Lever (c): an enclosing `if (__builtin_expect(mtype >= 0, 1))` adds
   4 B (1436).**  `__builtin_expect` on the loop condition or on
   `m->hasWon()` is a no-op (identical body).
6. **Lever (d): a 40-byte `static const unsigned char` rank table read to
   re-map the type into source order produces source-order blocks but
   changes the dispatch and the size (1356 B).**
7. **One-hot `switch (__builtin_expect(t, K))` can hit 1432 only for
   special `K`/weight pairs**, and the best of those is
   `-mllvm -likely-branch-weight=3 -mllvm -unlikely-branch-weight=2`,
   `K=157`: **raw 841 / aligned 283 / 1432 B** — aligned better than
   baseline but raw worse.  No one-hot pair improves *raw*.

## 1. Baseline and jump-table order

`raw` = same-address differing bytes (`/dev/shm/w10615/fdiff.py`);
`aligned` = instruction-index alignment (`align_diff.py`); `size` =
function bytes.  The two TBH tables are at `0xabfe2` (40 entries,
values 150..189) and `0xac04e` (7 entries, values 8..14).

Table 0 body order (target addresses sorted):

```
baseline: ac036 ac110 ac116 ac11c ac150 ac156 ac16e ac180 ac1ac ac1e0
          ac236 ac268 ac2ce ac2ee ac2f2 ac2fa ac366 ac37e ac382 ac38e
          ac3aa ac3f4 ac424 ac45a
        = 167/174, 150,151,152,153,154,155,158, 156,157,160,162,163,
          164,165,166,168,169,170,171,172,184,189, default
original: ac036 ac110 ac116 ac11c ac150 ac156 ac16e ac180 ac1dc ac242
          ac2a6 ac2b4 ac2ee ac32a ac354 ac388 ac3b6 ac3d2 ac3d6 ac3de
          ac3fc ac400 ac41a ac45a
        = 167/174, 150,151,152,153,154,155,158,162,166,170,184,189,
          156,157,160,163,164,165,168,169,171,172, default
```

Table 1 is byte-identical in baseline and original
(`ac060 ac0c4 ac0ce ac0dc ac45a` = 8, 11, 13, 14, default).

The original order is exactly the source order; the baseline is roughly
ascending with `167/174` hoisted and `158` before `156/157`.

## 2. Weight-vector sensitivity (IR injection, `pgo_switchweight_w9920.py`)

All vectors are `default=1` unless stated; groups are
G1={167,174,8,0,11,13,14}, G2={150,151,152,153,154,155,158,162,166,170,
184,189}, G3={156,157,160,163,164,165,168,169,171,172}.

| vector | raw | aligned | size |
|---|---:|---:|---:|
| **4/3/2** | **558** | **47** | **1432** |
| 5/4/3 | 558 | 47 | 1432 |
| 6/5/4 | 558 | 47 | 1432 |
| 8/6/4 | 558 | 47 | 1432 |
| 12/9/6 | 558 | 47 | 1432 |
| 17/13/9 | 558 | 47 | 1432 |
| 20/15/10 | 558 | 47 | 1432 |
| 100/99/98 | 558 | 47 | 1432 |
| 4/3/2, default 2/4/8 | 558 | 47 | 1432 |
| 4/2/1 | 1174 | 200 | 1444 |
| 3/2/1 | 1055 | 191 | 1444 |
| 5/3/2 | 1055 | 191 | 1444 |
| 7/5/3 | 1055 | 191 | 1444 |
| 9/6/3 | 1055 | 191 | 1444 |
| 10/7/4 | 1055 | 191 | 1444 |
| 4/3/1 | 1265 | 342 | 1380 |
| 4/3/3 | 722 | 320 | 1432 (baseline layout) |
| 4/4/2 | 1252 | 332 | 1372 |
| 4/2/2 | 1151 | 365 | 1428 |
| 2/1/1 | 1295 | 464 | 1412 |
| 29-level descending rank | 1057 | 248 | 1444 |
| 29-level ascending rank | 1171 | 593 | 1444 |
| geometric ratio 0.99 | 561 | 51 | 1432 |
| geometric ratio 0.95 | 1057 | 195 | 1444 |
| geometric ratio 0.8 | 1310 | 334 | 1420 |
| geometric ratio 0.5 | 1269 | 509 | 1380 |
| geometric ratio 2 | 1283 | 448 | 1388 |
| 4 levels (5,4,3,2 by 7s) | 1037 | 249 | 1440 |

**Interpretation.**  The 1432 cell accepts a *three-level arithmetic
progression* with small gaps (the ratio between adjacent levels must be
roughly <= 1.5) and a nearly-flat strictly-decreasing vector, but not a
steep geometric sequence.  This is why the if-chain fold (which always
produces geometric weights) cannot land on 1432 even when its size is
close.

## 3. Lever (a): switch on a local computed from `getType()`

Source: `int mtype = m->getType(); switch (mtype) { ... }` (same bodies).

| variant | raw | aligned | size | table-0 body order |
|---|---:|---:|---:|---|
| `int` local | 722 | 320 | 1432 | identical to baseline |
| `unsigned` local | 722 | 320 | 1432 | identical to baseline |
| `short` local | 1093 | 352 | 1432 | same structure, offsets +2 |
| `unsigned char` local | 1191 | 345 | 1436 | same structure, offsets +4 |

The `int`/`unsigned` forms are byte-identical; the compiler canonicalises
the local away.

## 4. Lever (b): if/else chain instead of the switch

| variant | raw | aligned | size | table-0 body order |
|---|---:|---:|---:|---|
| flat `if/else if` chain (27 arms, no expect) | 722 | 320 | 1432 | identical to baseline (folds back to the same switch) |
| flat chain with `__builtin_expect(cond,1)` each arm | 1237 | 657 | 1348 | `ac030 ac0b8 ac154 ac15a ac160 ac196 ac19c ac1a8 ac1b4 ac1dc ac212 ac26c ac29c ac302 ac322 ac326 ac330 ac392 ac3ac ac3b0 ac3be ac3c8 ac3d6 ca002` (single TBH) |
| `goto`-chain, per-comparison expect, 27 arms | 1287 | 786 | 1400 | `ac032 ac102 ac122 ac18a ac1cc ac1d2 ac20c ac22c ac28c ac292 ac2a0 ac2a6 ac2e0 ac310 ac372 ac37e ac38a ac3c0 ac3ea ac3fe` |
| `goto`-chain, per-comparison expect, grouped `||` | 1287 | 786 | 1400 | same as above |

The no-expect chain is folded by SimplifyCFG into exactly the baseline
switch, so lever (b) alone cannot change the layout.  With expects the
folded switch carries **geometric** weights (LLVM
`FoldValueComparisonIntoPredecessors` multiplies the chain weights), and
the body is 1348/1400 B, not 1432.

With per-TU `-mllvm -likely-branch-weight=L -unlikely-branch-weight=U` on
the goto-chain the folded weights become nearly flat for `L<<U`
(e.g. `L=1,U=2000` gives `1201263, 1200663, 1200063, ...`), but the
if-chain body itself is 1408..1440 B; no `(L,U)` pair lands on 1432.

## 5. Lever (c): `__builtin_expect` on the enclosing guard

| variant | raw | aligned | size | table-0 body order |
|---|---:|---:|---:|---|
| `if (__builtin_expect(mtype >= 0, 1)) { switch }` | 1207 | 351 | 1436 | `ac03c ac116 ac11c ac122 ac156 ac15c ac174 ac186 ac1b2 ac1e6 ac23c ac26e ac2d4 ac2f4 ac2f8 ac300 ac36c ac384 ac388 ac394 ac3b0 ac3f8 ac428 ac45e` |
| `if (mtype >= 0) { switch }` (no expect) | 1310 | 457 | 1416 | `ac03a ac114 ac11a ac120 ac152 ac158 ac170 ac182 ac1ae ac1e0 ac23a ac26c ac2d0 ac2ee ac2f2 ac2fa ac35a ac372 ac376 ac382 ac39c ac3da ac3e4 ac416` |
| `if (__builtin_expect(m->getType() >= 0, 1)) { switch (m->getType()) }` | 1199 | 335 | 1444 | `ac042 ac11c ac122 ac128 ac15c ac162 ac17a ac18c ac1b8 ac1ec ac242 ac274 ac2da ac2fa ac2fe ac306 ac372 ac38a ac38e ac39a ac39b...` |
| `if (__builtin_expect(mtype >= 150, 1)) { switch }` | 992 | 428 | 1172 | `ac032 ac042 ac048 ac04e ac082 ac088 ac0a0 ac0b2 ac0de ac112 ac168 ac19a ac200 ac220 ac224 ac22c ac298 ac2b0 ac2b4 ac2c0 ac2dc ac320 ac34e ac384` |
| expect on loop condition `i < missions->size()` | 722 | 320 | 1432 | identical to baseline |
| expect on `m->hasWon()` | 722 | 320 | 1432 | identical to baseline |
| expect on `m == 0` | 1239 | 497 | 1364 | `ac034 ac0da ac0e0 ac0e6 ac118 ac11e ac136 ac148 ac174 ac1a6 ac208 ac23a ac29e ac2bc ac2c0 ac2c8 ac328 ac340 ac344 ac350 ac36a ac3a8 ac3ae ac3e4` |

The guard always adds code (>= 4 B) and never reorders the table into
source order.

## 6. Lever (d): static const `unsigned char` table read in a loop

Rank table (40 bytes, index `t-150`, values 0..23 in source order,
24 = default), then `switch (g_missionRank[mtype - 150])` for the high
range and the original low switch for 8..14:

| variant | raw | aligned | size | table-0 body order |
|---|---:|---:|---:|---|
| rank table, switch on rank | 1246 | 353 | 1356 | source-order blocks (`abffe ac020 ac0dc ac0e2 ac0e8 ac11a ac120 ac138 ac14a ac1a4 ac200 ac208 ac26c ac27a ac2b4 ac2ee ac324 ac354 ac380 ac39c ac3a0 ac3aa ac3c0 ac3c2 ac3c6 ac3d0 ac40c ...`) |

The rank switch makes the machine layout source-ordered, but the extra
range check + table load changes the dispatch (1356 B) and the 40-byte
table also changes `.rodata` (data_exact would drop).  Size is sacred, so
this is not landable.

## 7. One-hot `switch (__builtin_expect(t, K))`

With the default `2000/1` weights only `K=14` keeps 1432 B
(raw 1295 / aligned 780); `K=167` is 1392, `K=150` is 1404, `K=8` is
1384, `K=0` is 1424, `K=156` is 1388.

With per-TU `-mllvm -likely-branch-weight=L -unlikely-branch-weight=U`
the size-1432 hits found by sweeping all 29 case values are:

| L/U | K | raw | aligned |
|---|---|---:|---:|
| 4/2 | 166 | 1195 | 403 |
| 4/2 | 171 | 1063 | 368 |
| 4/3 | 166 | 1017 | 391 |
| 4/3 | 171 | 1051 | 365 |
| 3/2 | 166 | 1047 | 393 |
| **3/2** | **157** | **841** | **283** |

`3/2, K=157` is the only variant with **aligned < 320**, but its raw is
841 > 722, so it is not a raw win and needs a per-TU compile flag.

`-mllvm -block-placement-exit-block-bias` on the baseline source keeps
aligned at 320 for bias 1/2/200 (raw 722) and 5..50/150 (raw 754); bias
100 gives 1307/702.  No win.

## 8. Why the plateau is not source-producible

* `LowerExpectIntrinsic::handleSwitchExpect` (LLVM 7.0.2) is one-hot:
  exactly one successor gets `LikelyBranchWeight`, every other gets
  `UnlikelyBranchWeight` (two levels).  `__builtin_expect_with_probability`
  does not exist in clang 7 and `__builtin_unpredictable` emits
  `!unpredictable`, not weights.
* The only source producer of more than two levels is the
  `SimplifyCFG::FoldValueComparisonIntoPredecessors` fold of an if-chain,
  and its weights are **geometric** (`w_i = L*U^(i-1)*(L+U)^(n-i)`),
  which the 1432 cell rejects.
* A `switch` on a rank-remapped local makes the *machine layout*
  source-ordered but costs a table load and changes the size.
* Therefore the 4/3/2 plateau (or the nearly-flat 0.99 vector) remains
  reachable only by post-optimisation IR `!prof` injection.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10615
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# plateau reference
W9920_SCRATCH=/dev/shm/w10615/w9920 \
  python3 _work/reconstructions/pgo_switchweight_w9920.py groups 4 4 4 3 2

# weight-vector sensitivity (see scratch sweep scripts)
# /dev/shm/w10615/sweep_weights.py, sweep2.py, sweep3.py, sweep4.py,
# sweep5.py, sweep6.py

# per-TU flag trials
python3 /dev/shm/w10615/build_flags.py \
  -mllvm -likely-branch-weight=3 -mllvm -unlikely-branch-weight=2
```

Scratch (`/dev/shm/w10615/`): `fdiff.py`, `align_diff.py`, `tbh.py`,
`measure.sh`, `gen_variants.py`, `run_variants.py`, `build_flags.py`,
`onehot_sweep.py`, `onehot_sweep2.py`, `sweep_weights.py`..`sweep6.py`,
`Status.baseline.cpp`, `Status.ifchain_expect_each.cpp`,
`Status.g1ifs_switch.cpp`.

## 10. Files changed

* `_work/reconstructions/status-switch-profile-wave160-w10615.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
