# `Level::createShip` — wave 188 (w13218): base 1,202 is a strong local optimum

Worktree `wt/w13218`, branch `wave188-w13218`, base master `ce16f45f1`
(wave 187).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13218`.

**Status: no code win landed; note only.**  Base is raw **1,202**,
aligned 484, size 2,764/2,764, insns 1,116/1,117.

## 0. Bottom line

Every variant measured this wave lands at **>= 1,202**.  The first
textual divergence is the stack-slot permutation of the six coordinate
locals (`+0x2c`):

| value | original slot | ours slot |
|---|---:|---:|
| `wpx` | 44 | 40 |
| `wpy` | 48 | 36 |
| `wpz` | 40 | 48 |
| first `nextInt` (jitter3) | 36 | 44 |
| second `nextInt` (jitter2) | 32 | 32 |
| third `nextInt` (jitter1) | 28 | 28 |

The original allocates the six slots in declaration order
(`wpy, wpx, wpz, jitter3, jitter2, jitter1` = 48,44,40,36,32,28); clang 7
here interleaves `jitter3` between `wpz` and `wpx`.  Downstream the
original keeps `ship` in `r9`, `fighter` in `sl`, `x` in `fp`; ours
coalesces `ship`/`fighter`/`fixed` into `fp` and keeps `x` in `r4`, so
the tail differs even where the instruction stream is otherwise the
same shape.

## 1. Sweeps measured this wave (all size 2,764 unless noted)

| variant | raw | aligned | insn |
|---|---:|---:|---:|
| base (`int wpz, wpx, wpy;`, `jitter3, jitter2, jitter1`) | **1,202** | 484 | 1116/1117 |
| natural jitter pairing (first `nextInt` -> `x`) | 1,204 | **399** | **1117/1117** |
| natural + `hitpoints` split statement | 1,204 | 399 | 1117/1117 |
| natural + `y`/`x` declaration swap | 1,204 | 399 | 1117/1117 |
| natural + both | 1,204 | 399 | 1117/1117 |
| natural + jitter declarations before the waypoint block | 1,204 | 399 | 1117/1117 |
| natural + `empPoints` `> 20 ? 20 : getLevel()` | 1,211 | 397 | 1117/1117 |
| `empPoints` `> 20 ? 20 : getLevel()` only | 1,209 | 482 | 1116/1117 |
| `int wpy, wpx, wpz;` | 2,423 | 456 | 1118/1117 (size 2,768) |
| `int wpy, wpx, wpz;` + `> 20` empPoints | 2,423 | 454 | 1118/1117 (size 2,768) |
| `int wpx, wpy, wpz;` | 2,423 | 455 | 1118/1117 (size 2,768) |
| jitter declarations before the waypoint block | 1,202 | 484 | 1116/1117 |
| jitters moved just before `z` | 2,611 | 1,151 | 1107/1117 (size 2,740) |
| jitters moved after the hitpoints switch | 2,459 | 492 | 1097/1117 (size 2,760) |
| jitters moved after `empPoints` | 2,565 | 784 | 1126/1117 (size 2,804) |
| `empPoints = level * 5 + 40` (reuse the clamped local) | 2,491 | 530 | 1107/1117 (size 2,748) |

The natural pairing is structurally much closer (aligned 399 vs 484,
insn 1117/1117) but two raw bytes worse because the slot permutation
moves.  It is not a gate win; it is the best starting point for a future
pass that can also fix the `ship`/`fixed` coalescing.

## 2. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13218
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1202 aligned=484 size=2764/2764 insn=1116/1117
```

Scratch: `Level_base.cpp`, `cs_sweep.py`, `cs_sweep2.py`,
`cs_sweep3.py`, `cs_show.txt` (side-by-side disassembly).

## 3. Files changed

* this note (no `src/` change; `Level.cpp` left at the wave-187 base).
