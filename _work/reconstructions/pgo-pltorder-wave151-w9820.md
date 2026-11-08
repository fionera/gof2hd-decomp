# Status::missionCompleted PGO: a shifted-rank profile restores the canonical
# call order (and a byte-identical `.rel.plt`) but not the 1432 B size
# (wave 151, w9820)

Status: **harness + note, no `src/` / `CMakeLists.txt` change**. Worktree
`wt/w9820`, base master `9d491d46a` (wave 151). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w9820`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target: `_ZN6Status16missionCompletedEbbx` (original `0xabf38`, **1432 B**).
Baseline: raw same-address diff **722**, instruction-index aligned diff **320**.

Mission (w9720 follow-up): the 1432 B rank-profile build loses 14 downstream
functions because its call-site order permutes five `.rel.plt` slots. Find a
profile whose first-occurrence call order is the canonical
`[getClientImage, isInList(i), getIngredientList, getQuantityList,
isInList(ii), setWon]` (`[0,1,2,3,4,5]`).

## 0. Bottom line

1. **NEW: a *shifted* rank profile restores the canonical call order and a
   byte-identical `.rel.plt`.** The lever is the switch-case counter value:
   instead of `rank-1` (w9515/w9620), use `rank + off` with `off >= 5`
   (`base=12, K=1, off=10, k4=24, c28=28, latch=0`). The five permuted
   `.rel.plt` entries (offsets `0x3628..0x3658`) become **0 diffs**.
   The reason: the original's shared tails (`isInList(ii)` at `+0x338`,
   `setWon` at `+0x574`) are cold blocks at the *end* of the function; the
   rank profile makes case 8 hot, its body falls through to the merged
   `isInList(ii)` tail, and the tail is placed early. Raising the case
   counters makes the tails cold and moves them back to the end.

2. **But the shifted profile no longer reproduces the original's switch-case
   order and does not hit 1432 B.** `off=10` gives size **1388** (raw 1351,
   aligned 570) and the tbh order becomes
   `[9,10,12,8,13,14,167,174,11,150,...,172]`, not the original's
   `[167,174,8,11,13,14,150,...,172]`. With `-mllvm
   -max-uses-for-sinking=0` the eager `rsbs/sbcs` entry is restored and the
   canonical order survives, but size is **1404** (raw 1301, aligned 481).

3. **No profile found gives size 1432 + canonical order.** 500+ measured
   points (grids over `base`, `off`, `k4`, `c28`, `latch`; see §3) give
   canonical-order sizes 1368..1412 only; every size >= 1424 point has the
   old `[4,1,5,2,3,0]` order. Because `-ffunction-sections` concatenates
   `.text`, a non-1432 target shifts every later function and `byte_exact`
   collapses: the 1388 B build gives `byte_exact 2327` / `linked_exact 4507`,
   the 1404 B eager build gives `2469` / `4507`.

4. **Root cause of the size gap: a full profile generates 15 fewer
   instructions than the original.** Instruction counts: original **527**,
   unprofiled canonical build **529**, shifted profile **514**. The original
   is essentially the *unprofiled* codegen with only the switch reordered,
   which is the `handleSwitchExpect` (switch-only weights) hypothesis, not a
   full `-fprofile-instr-use` profile. A full profile also changes the
   non-switch block layout and shrinks the function, so matching the call
   order alone cannot land it.

## 1. Measurements

`raw` / `aligned` / `size` are the target; `order` is the first-occurrence
order of the six callees; `relplt` is the number of differing 8-byte
`.rel.plt` entries vs the original.

| variant | raw | aligned | size | eager | order | relplt | byte_exact |
|---|---:|---:|---:|---|---|---:|---:|
| master (no profile) | **722** | **320** | **1432** | yes | `[0,1,2,3,4,5]` | 0 | **4507** |
| rank `rank-1`, `k4=12, k=23=0` (w9620) | 1367 | 636 | 1432 | no | `[4,1,5,2,3,0]` | 5 | 4493 |
| shifted `off=10, k4=24` | 1351 | 570 | 1388 | no | `[0,1,2,3,4,5]` | **0** | 2327 |
| shifted `off=10, k4=24` + `-max-uses-for-sinking=0` | 1301 | 481 | 1404 | **yes** | `[0,1,2,3,4,5]` | **0** | 2469 |

The `.rel.plt` diff of the rank build is exactly the five entries
`0x3628, 0x3638, 0x3640, 0x3648, 0x3658`; the shifted build has none.

## 2. Mechanism: why `off` moves the shared tails

The frontend IR tail-merges the identical `isInList(ii)` tails of case 8
(block 90) and case 172 (block 396). The merged tail has two predecessors.
MachineBlockPlacement places it after the hotter predecessor:

- rank profile: case 8 is rank 26 (hot), case 172 rank 1 (cold) -> the tail
  is placed right after case 8's body, before case 13/14/162/166. First
  `isInList(ii)` is therefore the shared tail, i.e. call order `[4,...]`.
- shifted profile (`rank+10`): the case blocks are all hot relative to the
  loop's non-case counters, the exit/return tails become cold, and the
  shared `isInList(ii)` and `setWon` tails are placed at the end. First
  `isInList(ii)` is then case 166's own copy, after `getIngredientList`.

The `setWon` tail behaves the same way: in the original it is at `0xac4ac`
(shared by cases 150/151/153/154/155/167/174/...), in the rank build it is
inlined early, in the shifted build it is back at the end (`0xac370`).

## 3. Search

`pgo_shift_w9820.py` builds the shifted vector
`case_ctr = rank*K + off`, `latch(6,7,12)=latch`, `c28`, `k4`, `k23=0`, then
compiles `Status.cpp` with `-fprofile-instr-use` + section-prefix off, links
with gold, and reports size/raw/aligned/eager/call-order/`.rel.plt` diffs.

Measured grids (all with `K=1`):

- `base in {0,4,8,12,16}`, `off in {0,5,10,15,20,25,30}`, `k4 in {12,24,30}`:
  105 points. Canonical order appears for `off>=5`; size 1372..1404.
- `base=12`, `off=10`, `k4 in {12,20,24,28}`, `c28 in {0,8,16,28,40}`,
  `latch in {0,4,12,24}`: 80 points. `latch=12` hits 1432 but order
  `[1,5,2,3,4,0]`; canonical order only at `latch=0`, size 1388..1400.
- `base in {8,12,16}`, `off in {0,5,10,15}`, `k4 in {12,16,20,24,28,32}`,
  `latch in {4,8,12}`: 216 points. No size-1432 canonical-order point.
- 119-point random grid over `base in {0,2,4,6,20,24,28,32,40}`,
  `off in {5,8,10,12,15,20}`, `k4 in {12..40}`, `latch in {0,2,4,6,8,10,12}`,
  `c28 in {0,8,16,28,40}`. Canonical-order sizes 1368..1412; every
  size>=1424 point has order `[4,1,5,2,3,0]`.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9820
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# canonical-order build (size 1388, .rel.plt identical):
python3 _work/reconstructions/pgo_shift_w9820.py shift 12 10 24 28 0
# eager entry + canonical order (size 1404):
python3 _work/reconstructions/pgo_shift_w9820.py shift 12 10 24 28 0 --extra \
  -mllvm -max-uses-for-sinking=0
# the rank build for comparison (size 1432, 5 .rel.plt diffs):
python3 _work/reconstructions/pgo_combine_w9620.py run 12 28 0 23=0
python3 _work/reconstructions/pgo_shift_w9820.py order \
  /dev/shm/w9820/run/run_12_28_0.so /dev/shm/w9820/run/shift.so
```

`pgo_shift_w9820.py order <so>` prints the call order and the `.rel.plt`
diff count for any linked `.so`.

## 5. Open questions

1. The shifted profile proves the `.rel.plt` permutation is fixable, but the
   full-profile codegen is 15 instructions smaller than the original. The
   original's 527-instruction body is the unprofiled 529-instruction body
   with the switch reordered, which points at `handleSwitchExpect`
   (switch-only `!prof`) rather than a full TU profile. Reproducing that in
   source (e.g. `switch (__builtin_expect(...))`) is the next lever.
2. If a switch-only weighting mechanism is found, the per-case rank order
   and the eager entry should both survive at 1432 B, and the `.rel.plt`
   should stay canonical. That is the only path to `byte_exact 4507` with a
   non-baseline target.
3. The 1388 B shifted build is otherwise a useful oracle: it has the
   original's call order and `.rel.plt`, so any future 1432 B variant can be
   checked against it for the tail placement.
