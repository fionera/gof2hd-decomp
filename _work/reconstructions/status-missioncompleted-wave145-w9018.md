# Wave 145 (w9018): `Status::missionCompleted` — the helper-group-id shape is
# measured and loses (+52 B); the 558 layout is reachable only by a *group-uniform*
# `!prof` vector, and every source producer of one costs extra dispatch code.

Status: **note-only** (no `src/` change landed).  Gate on the unchanged tree:
`byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0.  Worktree `wt/w9018`, base master `eb13ada9b`
(wave 144).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**,
baseline raw same-address diff **722 B**.  Metric: raw same-address diff bytes
(`/dev/shm/w9018/fdiff.py`), not the PYTHONHASHSEED-dependent fuzzy %.

## 0. New results

### 0.1 The 558 layout is a *group-uniform* `!prof` vector, re-confirmed
Injecting `branch_weights` into the real optimised IR switch (29 cases +
default, case order = source order) and re-linking with the checked-in gold
command reproduces the original block order and **558 / 1432**.  A fresh sweep
of the `(default, A, low, B, C)` group vector (A = 167/174, low = 8/0/11/13/14,
B = 150..189 minus C, C = 156..172) on the real TU:

| vector | raw diff | size |
|---|---:|---:|
| `1,3,4,3,2` | **558** | 1432 |
| `1,4,4,3,2` | **558** | 1432 |
| `1,5,4,3,2` | **558** | 1432 |
| `1,6,5,4,3` | **558** | 1432 |
| `1,2,2,2,1` | 1254 | **1372** |
| `1,3,3,3,1` | 1245 | **1380** |
| `1,8,4,2,1` | 1174 | **1444** |
| `1,4,2,1,1` | 1295 | **1412** |

So the load-bearing shape is `low >= A >= 3`, `B = 3`, `C = 2`, `default = 1`
(or a common scale, e.g. `6,5,4,3`); a *strictly descending geometric* vector
(`8,4,2,1`) is **not** in the class.  The 558 residual is entirely within-block:
the first **664 B (0x298)** of the function are byte-identical; the diffs start
just before the 162 body (`0x298..`) and the big run is `0x363..0x43b` (216 B).

### 0.2 The helper-group-id shape (the wave-145 hypothesis) is measured: +52 B
Shape tried (real `Status.cpp`, real gold link):

```cpp
static int missionGroup(int t) {           // inline: 0 = A, 1 = low, 2 = B, 3 = C
    if (t == 167 || t == 174) return 0;
    if (t == 8 || t == 0 || t == 11 || t == 13 || t == 14) return 1;
    if (t == 156 || t == 157 || t == 160 || t == 163 || t == 164 ||
        t == 165 || t == 168 || t == 169 || t == 171 || t == 172) return 3;
    return 2;
}
int t = m->getType();
int g = missionGroup(t);
switch (g) { case 0: <A body>; break;
             case 1: switch (t) { <low> } break;
             case 2: switch (t) { <B> }   break;
             default: switch (t) { <C> }  break; }
```

| shape | raw diff | size |
|---|---:|---:|
| `switch (g)` + per-group `switch (t)` | 1212 | **1484** |
| `if (__builtin_expect(g==0,1)) ... else if ...` + per-group `switch (t)` | 1188 | **1484** |
| master | **722** | 1432 |

The group body cannot be a single block (the low/B/C groups have per-type
bodies), so the helper shape necessarily emits a **second `switch (t)` per
group**: +52 B.  `-mllvm -likely-branch-weight=3` is a no-op on both (the
`__builtin_expect(...,1)` weight only changes the arm probabilities, not the
per-group dispatch).

### 0.3 Nested-switch producers of a group weight are still too steep
`FoldValueComparisonIntoPredecessors` scales a folded predecessor case by
`ValidTotalSuccWeight` = the *sum of the successor switch's weights*.  With the
switch as the predecessor's **default** (`PredDefault == BB`), a 10-case inner
switch therefore gives `11:1` (w144 §0.3).  Measured on the real TU:

| shape | raw diff | size |
|---|---:|---:|
| nested `switch(t){A+low+B; default: switch(t){C}}`, no expect | **722** | 1432 |
| same + `switch (__builtin_expect(t,167))`, L=1..4 | 1260 | **1392** |
| master + `-mllvm -likely-branch-weight=1..7` | **722** | 1432 |
| master with the 5 in-body `__builtin_expect(...,1)` removed | 894 | **1428** |
| master + `__builtin_expect(m->hasWon(),0)`, L=1..4 | **722** | 1432 |

The plain nested switch is folded away with **no** weights (neither side has
`!prof`), so the layout does not move.  Seeding it with a switch-expect makes
the outer jump table disappear (1392 B).  A `case a ... b:` range is expanded
to individual IR cases and, when it is the predecessor's *non-default* arm,
`PredDefault != BB` so no scaling is applied at all.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9018/fdiff.py _ZN6Status16missionCompletedEbbx   # 722 / 1432

# reproduce 558: inject weights into the real switch IR, recompile, relink
python3 /dev/shm/w9018/inject.py status.ll si.ll 1,3,3,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2
bash /dev/shm/w9018/cc_inj.sh /dev/shm/w9018/si.ll
bash /dev/shm/w9018/relink2.sh        # runs the checked-in gold link directly
```

Scratch (`/dev/shm/w9018/`): `fdiff.py` (raw-diff scorer), `branges.py`
(diff-run lister), `tbh.py` (decode both `tbh` tables), `inject.py` (insert
`!prof` into the real switch), `cc.sh` / `cc_inj.sh` / `relink.sh` /
`relink2.sh` (exact TU compile + checked-in gold link), `gen.py` (group
vector), `vg.py` (helper-group-id / if-chain source generator), `v2.py`
(nested C switch), `vnoexp.py`, `vhw.py`.

## 2. Open items

1. A source producer of a **group-uniform** vector with a *shallow* ratio is
   still missing.  The only fold lever is `ValidTotalSuccWeight`, so the inner
   switch must have 1..5 contributing successors while still covering the whole
   cold group.  `case a ... b:` does not help (`PredDefault != BB` ⇒ no
   scaling); a 1..5-case inner switch leaves the rest of the group hot.
2. The helper-group-id shape is ruled out (+52 B from the second per-group
   `switch (t)`), unless the group bodies can be reached without a second
   dispatch (they cannot: low/B/C have per-type bodies).
3. The residual 558 B (within-block, from `0x298`) is independent of the
   layout and remains the only source-reachable part once the weights match.
