# LevelScript::process — wave 189 / w13310: permutation / 2-opt / 3-opt sweep (no win)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13310`, branch `wave189-w13310`, base master
`ad42660c5` (wave-188 harvest).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13310`.

**Result: no size-exact, `.rel.plt`-identical raw win exists in the swept
permutation space.  `src/` is unchanged; this note is the only commit.**

```
                         base (HEAD)
process raw diffbytes    51,917
process aligned insns    20,935
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726
.text diffbytes          74,047
.ARM.extab diffbytes     639            (size/base exact 156,312 B @ 0x1a3b78)
.rel.plt                 md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
.rodata / .data / .bss   0 / 0 / 0
file size                2,187,328      (exact)
byte_exact               4507
linked_exact             4507
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN ✓
drift                    0
symbol parity            5370/5370
RATCHET                  PASS
```

## 1. Harness

The w13105/w13205 statement harness was rebuilt in
`/home/debian/GalaxyOnFire2/scratch/w13310/`:

* `stmts.py` — character-level split of the `process` body into movable
  single-line statements (2,843 statements in 784 contiguous runs).
* `model.py` — prefix-aware read/write/call access-chain model.
* `gen.py` — independent single-statement moves (`down`/`up`) within a run.
* `eval.py` — copies the TU, compiles with the exact
  `cmake-build-match/compile_commands.json` command (NDK r18b clang 7.0.2,
  `-Oz`, `-mllvm -disable-postra-machine-sink`), runs the exact gold link
  (`linkonly.txt`, no strip), and reads the function bytes with a minimal
  ELF32 reader (`elfread.py`).  `elfread.raw_diff` is the same same-address
  byte-diff metric as `tools/fn_diff.py` (base 51,917 verified both ways).
* `group.py` — permutations of K consecutive pairwise-independent statements.
* `sweep.py`, `adj_same.py`, `same_recv_dist.py`, `opt2.py`, `opt3.py` — the
  sweep drivers.  4 workers, `ulimit -v 12000000`, ~0.4 s/candidate.

A latent bug in the w13205 `eval.apply_moves` was fixed: the `pos` map was
built once and never updated, so multi-move candidates were applied at stale
positions.  It now recomputes `order.index(...)` per move.

## 2. Sweeps and results

All candidates below were re-measured on the current 51,917 body.  Every
size-exact candidate scored **exactly 51,917** (not one moved the raw metric
by a byte).

| sweep | candidates | size-exact | raw min |
|---|---:|---:|---:|
| independent adjacent (`d=1`, deduped) | 1,712 | 1,637 | 51,917 |
| independent `d=11..40` | 2,530 | 2,378 | 51,917 |
| same-receiver adjacent (`d=1`) | 417 | 414 | 51,917 |
| same-receiver `d=2..10` | 2,384 | 2,269 | 51,917 |
| same-receiver `d=11..40` | 394 | 334 | 51,917 |
| group reverse, K=4 | 811 | 758 | 51,917 |
| group reverse, K=5 | 600 | 562 | 51,917 |
| group reverse, K=6 | 468 | 438 | 51,917 |
| group reverse, K=7 | 375 | 351 | 51,917 |
| group reverse, K=8 | 299 | 279 | 51,917 |

Group windows are contiguous K-statement runs whose members are pairwise
independent under `model.conflict`; the reverse permutation is the one
multi-move order not already covered by the single-move sweeps (a rotation is
a single move and is included in the `d` sweeps).

### 2-opt / 3-opt

Because every single move is neutral, the best single moves are all ties at
51,917.  Sixty random disjoint **pairs** and sixty random disjoint **triples**
of neutral moves were applied together (both from the `d=11..40` winner set)
and each was compiled and linked:

* 60/60 pairs: size 57,232, raw 51,917, **function `.text` byte-identical to
  base**.
* 60/60 triples: size 57,232, raw 51,917, **function `.text` byte-identical to
  base**.

Three representative single moves were also checked byte-for-byte against the
base body (one independent, two same-receiver-adjacent): all `.text`-identical.
So the neutral moves are not merely equal-raw, they are codegen no-ops, and
their compositions are no-ops too.

## 3. Conclusion

The current 51,917 body is a fixed point of the statement-permutation space
covered here: independent adjacent swaps, independent moves out to distance
40, same-receiver moves out to distance 40, K=4..8 group reversals, and
2-opt/3-opt compositions of the neutral winners.  The residual is not
reachable by reordering statements; it is instruction-selection /
register-allocation / branch-layout (the axes w13201/w13202/w13203 and
w1310x are exploring).  No source change was landed.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13310
ulimit -v 12000000
cmake --preset default -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51917 aligned= 20935 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch artifacts (`stmts.py`, `model.py`, `gen.py`, `eval.py`, `elfread.py`,
`group.py`, `sweep.py`, `adj_same.py`, `same_recv_dist.py`, `opt2.py`,
`opt3.py`, `linkonly.txt`, `res_*.jsonl`, `cand/`) live in
`/home/debian/GalaxyOnFire2/scratch/w13310/`; they are scratch, not tracked.

## Harvest on wave-189 master (`8225c71a8`)

Note-only: no size-exact win.  The sweep was run on the 51,917 body; current
master is 51,874 (w13309/w13208/w13205 harvests), so a fresh K=4..8 / 2-opt
sweep on the current body is still open (w13332 is doing it).
