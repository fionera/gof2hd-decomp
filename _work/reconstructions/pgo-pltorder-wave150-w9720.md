# Status::missionCompleted PGO: the 14 downstream diffs are a `.rel.plt`
# permutation, not a literal-pool artefact (wave 150, w9720)

Status: **harness + note, no `src/` / `CMakeLists.txt` change**. Worktree
`wt/w9720`, base master `db9658a51` (wave 150). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w9720`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target: `_ZN6Status16missionCompletedEbbx` (original `0xabf38`, **1432 B**).
Baseline: raw same-address diff **722**, instruction-index aligned diff **320**.

Mission (w9620 follow-up): find a clean 1432 B profile whose downstream
literal pools stay exact.  w9620 had found that a 1432 B rank-profile hit
(`base=12,c28=28,latch=0`, `k=23=0`) still gives `byte_exact 4493` because 14
downstream functions differ by one byte.

## 0. Bottom line

1. **NEW: the 14 downstream diffs are a 5-entry `.rel.plt` permutation.**
   The profiled target's call sites appear in a different order in its
   `.text` section, so gold allocates five PLT slots in a different order.
   The five permuted slots are

   `Mission::getClientImage`, `Item::isInList(int,Array*)`,
   `BluePrint::getIngredientList`, `BluePrint::getQuantityList`,
   `Item::isInList(int,int,Array*)`, `Mission::setWon`.

   `.rel.plt` differs in exactly 5 entries (offsets `0x3628..0x3658`); `.plt`
   and `.got` bytes are identical, `.text` differs only in the target plus
   the `blx`/GOT-literal bytes of the 14 callers.  The canonical build's
   `.rel.plt` is byte-identical to the original, so the original's call order
   is the canonical order.

2. **The first-occurrence order of those six callees is the whole story.**
   Original / canonical: `[getClientImage, isInList(i), getIngredientList,
   getQuantityList, isInList(ii), setWon]` (`[0,1,2,3,4,5]`).  The 1428 B
   rank profile gives `[4,1,5,2,3,0]` — `getClientImage` is last because the
   profiled entry sinks the `shortMission`/`longMission` block to the end.
   `-max-uses-for-sinking=0` restores the eager `rsbs/sbcs` entry but **not**
   the call order: still `[4,1,5,2,3,0]`, 1404 B.  So "eager entry" (the
   w9620 `rsbs/sbcs` test) is necessary but not sufficient.

3. **The entry `isCampaignMission()` branch weight underflows to
   `{-1, 1}`.**  In the `-fprofile-instr-use` IR the branch is
   `!{!"branch_weights", i32 -1, i32 1}`.  The switch block's count is
   derived from **counter 4** (loop body count), while the `&&`-chain
   counters 8/9/10/11 are all `base=12`, so `count(switch) - count(chain)`
   wraps and the true (switch) edge becomes effectively unreachable-hot.
   This is why the chain is outlined and placed after the switch.  Setting
   `counter 4 = 300` gives `{277, 13}`; setting `counter 4 = 100,
   counter 11 = 50` flips the chain to the fallthrough and yields
   `[0,4,1,2,3,5]` (`getClientImage` first) — but the function shrinks to
   1376 B.

4. **No `-mllvm` sink threshold gives 1432 B + the canonical call order.**
   `max-uses-for-sinking=0/1`, `sink-freq-percent-threshold=0/50/100` all
   leave the order `[4,1,5,2,3,0]`; the best aligned diff is
   `sink-freq-percent-threshold=0` at 511 (vs baseline 320), size 1424.
   A `base=0..40` sweep with sinking disabled never reaches 1432 B
   (1400-1424), and a `c28 x latch` sweep at `base=12, sink=0` never reaches
   1432 B either.

5. **The remaining gap is the shared-tail placement, not the switch table.**
   The rank profile reproduces both `tbh` tables in *relative* order, but the
   original's `isInList(ii)` call lives in a shared tail at `+0x338`
   (case 8 falls through to it via `b.n ac454`), while the profiled build
   places that tail early.  Matching `[0,1,2,3,4,5]` therefore needs the
   original's *shared-tail* block placement as well as the case order — a
   stronger constraint than the `tbh` rank match.

## 1. Measurements

`raw` / `aligned` / `size` are the target; `plt` is the first-occurrence
order of the six callees (`[0,1,2,3,4,5]` = canonical).

| variant | raw | aligned | size | eager | plt |
|---|---:|---:|---:|---|---|
| master | **722** | **320** | **1432** | yes | `[0,1,2,3,4,5]` |
| rank `12,28,0` | 1349 | 654 | 1428 | no | `[4,1,5,2,3,0]` |
| rank `12,28,0` + `max-uses-for-sinking=0` | 1329 | 635 | 1404 | yes | `[4,1,5,2,3,0]` |
| rank `12,28,0` + `max-uses-for-sinking=1` | 1380 | **519** | 1424 | no | `[4,1,5,2,3,0]` |
| rank `12,28,0` + `sink-freq-percent-threshold=0` | 1342 | **511** | 1424 | no | `[4,1,5,2,3,0]` |
| rank `12,28,0`, `k=23=0` (1432 B hit) | 1367 | 636 | **1432** | no | `[4,1,5,2,3,0]` |
| rank + `counter4=100, counter11=50` | 1315 | 531 | 1376 | no | `[0,4,1,2,3,5]` |
| rank + `counter4=300` | 1329 | 543 | 1380 | no | `[0,4,1,2,3,5]` |

The `k=23=0` 1432 B hit is the only size-1432 vector measured this wave; its
`byte_exact` is 4493 (14 downstream diffs), unchanged from w9620.

## 2. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9720
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# the canonical build's call order (== original's):
python3 _work/reconstructions/pgo_pltorder_w9720.py order cmake-build-match/libgof2hdaa.so
# sink-threshold matrix (raw/aligned/size/plt):
python3 _work/reconstructions/pgo_pltorder_w9720.py thresholds
# the counter-4 / counter-11 entry-weight lever:
python3 _work/reconstructions/pgo_pltorder_w9720.py sweep4 100 50
# the 1432 B rank hit and its 4493 gate:
python3 _work/reconstructions/pgo_combine_w9620.py run 12 28 0 23=0
python3 _work/reconstructions/pgo_combine_w9620.py verify /dev/shm/w9620/run/run_12_28_0.so
```

The `.rel.plt` diff is visible directly:

```sh
python3 - <<'EOF'
import struct
def sec(p,n):
    d=open(p,'rb').read(); o,=struct.unpack_from('<I',d,0x20)
    es,en,si=struct.unpack_from('<HHH',d,0x2e)
    L=[struct.unpack_from('<IIIIII',d,o+i*es) for i in range(en)]
    st=d[L[si][4]:L[si][4]+L[si][5]]
    for nm,ty,fl,ad,of,sz in L:
        if st[nm:st.find(b'\0',nm)].decode()==n: return d,of,sz
a=sec('cmake-build-match/libgof2hdaa.so','.rel.plt')
b=sec('/dev/shm/w9620/run/run_12_28_0.so','.rel.plt')
print([hex(i) for i in range(0,a[2],8) if a[0][a[1]+i:a[1]+i+8]!=b[0][b[1]+i:b[1]+i+8]])
EOF
# -> ['0x3628', '0x3638', '0x3640', '0x3648', '0x3658']
```

## 3. Open questions

1. A profile whose **shared-tail** blocks land where the original's do (the
   `isInList(ii)` tail at `+0x338`, the `setWon` tail at `+0x574`) would
   restore `[0,1,2,3,4,5]` at 1432 B.  The lever is the loop-body counter 4
   plus the `&&`-chain counters 8/9/10/11; the search this wave only covered
   a coarse grid and always lost 40-70 B of size.
2. The `{-1,1}` underflow is a clang-7 PGO artefact of a block with no
   counter (the switch block).  A hand-authored `.profdata` cannot remove
   it, because the weight is recomputed from the counters at use time; only
   making `count(switch) > count(chain)` avoids the wrap.
3. `-mllvm -max-uses-for-sinking=1` and
   `-mllvm -sink-freq-percent-threshold=0` are the best *aligned* results
   (519 / 511 vs baseline 320) and are worth revisiting once the target's
   own block order is fixed.
