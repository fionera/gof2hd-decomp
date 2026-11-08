# Status::missionCompleted — switch-profile: fold overflow and the nested-default order (wave 161, w10715)

Status: **analysis only, no `src/` change**. Worktree `wt/w10715`, base master
`01c7f92d6` (wave 160). Scratch `/dev/shm/w10715`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (source restored): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target `_ZN6Status16missionCompletedEbbx` (1432 B). Baseline **raw 722 /
aligned 320 / 1432 B**. Plateau (IR `!prof` injection, default 1 / 4 / 3 / 2)
**raw 558 / aligned 47 / 1432 B**.

## 0. Bottom line

No source-only shape tried this wave is both size-exact (1432 B) and a raw win.
The one new structural result: a **nested-default dispatch reproduces the
original block order exactly** (G1, G2, G3) but costs 40 B (1392 B), so it is
gate-fatal.  The if-chain fold cannot produce the flat 4/3/2 vector because
`likely-branch-weight=2000` makes the folded vector steep: after `FitWeights`
only **3 of 24** main-table weights survive non-zero.

## 1. Why the if-chain fold cannot reach 4/3/2

`LowerExpectIntrinsic` is one-hot (two levels).  The only multi-level producer
is `SimplifyCFG::FoldValueComparisonIntoPredecessors`: for an `if (t==c) goto`
chain with `__builtin_expect(cond,1)` the new case weight is
`LikelyBranchWeight * total_successor_weight` and every existing weight is
scaled by `UnlikelyBranchWeight`.  With the default 2000:1 this grows
~2000x per arm, so `FitWeights` (`Offset = 32 - clz64(max)`) shifts the whole
vector and the tail collapses to 0.

Exact replay of the fold (uint64 + `FitWeights` after every fold) for a
29-arm all-`expect(...,1)` chain:

```
i32 0, 2402543029, 1200663, 592, 0, 0, ...   (default, then source order)
```

Only `167`, `174`, `8` are non-zero; every other main-table case ties at 0 and
is laid out ascending.  This is exactly the scrambled order measured on the
built chain variant (167/174, 165, 162, 189, 153, ...).  A *flat* vector would
need `likely-branch-weight` ~ 1, which is a per-TU flag (out of scope).

## 2. Measured source shapes (all `src/` reverted; size is the blocker)

| shape | raw | aligned | size | order |
|---|---:|---:|---:|---|
| baseline `switch (m->getType())` | 722 | 320 | 1432 | ascending, 167/174 hoisted |
| 29-arm `if (t==c) goto` chain, no expect | 722 | 320 | 1432 | folds back to baseline |
| same chain, `expect(...,1)` on every arm | 1287 | 786 | 1400 | scrambled (fold overflow) |
| same chain, `expect(...,1)` on first 3 arms | 1297 | 782 | 1404 | 167/174/150 then ascending |
| range split `if (t>=150) switch else switch` | 1242 | 465 | 1372 | ascending |
| range split + one-hot `expect(t,167)` | 1226 | 540 | 1376 | ascending |
| **nested G3 default switch + one-hot `expect(t,167)`** | **1260** | **503** | **1392** | **original order** |

### 2a. The nested-default result (new)

Source shape:

```cpp
int mtype = m->getType();
switch (__builtin_expect(mtype, 167)) {
    case 167: case 174: ...            // G1
    case 150: ... case 189: ...        // G2 (167/174/8/0/11/13/14 in the main table)
    default:
        switch (mtype) {               // G3 = {156,157,160,163,164,165,168,169,171,172}
            case 156: ... case 172: ...
        }
        break;
}
```

The one-hot hint makes `167` hot, and the nested-default block is placed after
the main cases, so the machine order becomes exactly

```
167/174, 150,151,152,153,154,155,158, 162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172
```

i.e. the original order.  The nested switch is **merged back into the single
40-entry TBH table** (one `tbh` only), so the -40 B is *not* an extra table:
the one-hot hint leaves the default weight at 1, tied with the ordinary cases,
and `MachineBlockPlacement` then places the default block (the nested switch)
second (at `ac05c`) instead of last (original `ac45a`).  Every other body is
placed exactly in the original order.  Without the one-hot hint the compiler
merges the nested switch back into the main switch and the build is
byte-identical to the baseline (722/320/1432).

## 3. Conclusion / open question

The original order is reachable from source (nested default + one-hot), but the
extra table costs 40 B.  A future pass could look for a nested split whose
second table is exactly 7 entries (matching the original's low table) so the
total returns to 1432 B; the G3 set spans 156..172 (17 values), so a different
partition (e.g. move only the 156/157/160 tail) would be needed.

## 4. Files changed

* `_work/reconstructions/status-switch-nested-wave161-w10715.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
