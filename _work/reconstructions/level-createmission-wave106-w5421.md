# w5421: `Level::createMission` (92.028388 %, 17172 B) — wave-106:
# selector/raid-slot/merged-condition sweep; no size-preserving improvement

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w5421`, branch `wave106-w5421`,
base `486f8a119`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).
**No `src/` change is committed**: every variant measured below is neutral or a
regression. The deliverable is this note. The worktree is clean.

## Baseline (unchanged, gate green before and after)

```text
_ZN5Level13createMissionEv   fuzzy 92.028388 %   size 17172 B   @0xaffd8
                             orig 6055 insns / ours 6063 insns
gate: byte 4500/4524, linked 4500/4524, size 4524/4524, stubs 0, missing 0,
      wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
      RATCHET PASS
```

`cmake --build cmake-build-match --target check -j6` passes before and after.
`report.json` is not modified/committed.

## Method

Every row below is a fresh `cmake --build cmake-build-match --target link_so`
followed by `score2.py` (the official verifier's disassembly + reconcile +
`difflib.SequenceMatcher(autojunk=False)` normalisation, unrounded). `sp84` /
`sp88` are the reference counts at `[sp,#84]` / `[sp,#88]` in the fresh
`createMission` disassembly: the original has `raid` at `sp+84` (17 refs) and
the `__stack_chk_guard` address spill at `sp+88` (2 refs); the baseline is the
mirror image (`sp84=2`, `sp88=17`).

## 1. Selector sweep (the in-loop table select)

The original's select is a branchy in-loop `ldr bigAlienWave; cbz`, followed by
a shared body. The baseline's `(bigAlienWave + (i < 0)) ? A : B` anchor keeps
the select in the loop but lowers it branchless (`cmp.w r1,r2,asr #31; it ne;
movne`). Every plain-condition spelling is LICM-hoisted out of the loop and
loses ~3.8 pp; every shape that produces the real branch also changes the
block layout and loses more than the `cbz` gains.

| variant | size | ours insns | pct |
|---|---:|---:|---:|
| **baseline (`(bigAlienWave + (i < 0)) ? A : B`)** | **17172** | **6063** | **92.028388** |
| `Vector(...)` parens / `(bool)` cast / `> 0` / `!= 0` / `-` / `^` / `i >> 31` / `(i<0)*1` / `(i<0)+0` / `(i<0)|0` / `(i<0)!=0` / `i < 0L` / `((i&0x80000000)!=0)` | 17172 | 6063 | 92.028388 |
| `(bigAlienWave == 1)` / `!= 0` / `if-else` selecting only `t` / `goto` / `?:`-void / `if` assigning both arms | 17140 | 6058 | 88.219269 (hoisted) |
| `(bigAlienWave != 0) \|\| (i < 0)` | 17180 | 6067 | 89.638673 |
| `(bigAlienWave) \| (i < 0)` | 17172 | 6063 | 90.823568 |
| `(bigAlienWave != 0) && (i >= 0)` | 17180 | 6066 | 89.613068 |
| `(i < 0) \|\| bigAlienWave` | 17180 | 6067 | 89.638673 |
| `(bigAlienWave + (i == -1))` | 17164 | 6064 | 90.898589 |
| `(bigAlienWave + (i & 0x80000000))` | 17172 | 6063 | 90.823568 |
| `(bigAlienWave + (-i < 0))` | 17180 | 6065 | 89.240924 |
| `(bigAlienWave + (i != (i & 0x7fffffff)))` | 17172 | 6065 | 91.980198 |
| `(bigAlienWave + (i < 0u))` | 17140 | 6058 | 88.219269 (hoisted) |
| `dup-first` (first vector + setRotation in both arms) | 17156 | 6062 | 88.635801 |
| `dup-first` + parens / `==1` / `t` declared outside | 17156 | 6062 | 88.635801 |
| `dup-first` + named `Vector tmp` per arm | 17156 | 6062 | 88.668812 |
| `dup-first` + direct `v =` in each arm | 17148 | 6062 | 88.701824 |
| `dup-first` + shared `v` after the arms | 17140 | 6058 | 88.219269 |
| `dup-anchor` (`if (bigAlienWave + (i<0))` with duplicated body) | 17148 | 6061 | 87.355563 |
| `dup-both` (both vectors duplicated per arm) | 17156 | 6064 | 89.660863 |
| named `Vector dir(t[k][3],…)` / brace-init per vector | 17164 | 6059 | 88.657751 |
| named first `Vector dir`, inline second | 17156 | 6055 | 88.703551 |
| named `float f0,f1,f2` before the first vector | 17172 | 6063 | 92.028388 |
| named `float f0,f1,f2` before the second vector | 17172 | 6063 | 92.028388 |
| `const float *t = … ? &A[k][0] : &B[k][0]` then `t[3..5]` | 17156 | 6057 | 83.603038 |
| `const float (*t)[6] = … ? &A[k] : &B[k]` then `t[k][3..5]` | 17172 | 6062 | 90.831064 |

The `dup-first` shape does reproduce the original `cbz` verbatim, but the added
branch re-orders the whole function (a 208-instruction `difflib` replace at the
alienGuns-loop tail) and costs 3.4 pp. The baseline anchor is the best
size-preserving shape found.

## 2. `raid`/guard stack-slot swap (`sp+84` ↔ `sp+88`)

The wave-90 note established the mechanism (StackSlotColoring by spill weight:
ours `raid`=1.197958 vs guard=1.193760, a 0.0042 margin) and that every
size-preserving variant leaves the slot at 84. This pass re-confirmed that and
found two more slot-flipping spellings; both change the code far more than the
~18-instruction swap is worth.

| variant | size | ours insns | pct | sp84/sp88 |
|---|---:|---:|---:|---:|
| **baseline** | **17172** | **6063** | **92.028388** | **2/17** |
| `int raid` | 17172 | 6063 | 91.863344 | 2/17 |
| `unsigned security` | 17172 | 6062 | 91.903937 | 2/17 |
| `unsigned raidRoll` | 17172 | 6063 | 91.995379 | 2/17 |
| `bool raid = roll<…; if (intro) raid=false;` (`decl_init`) | 17236 | 6074 | 83.667244 | **16/2** |
| `switch (stationIndex)` instead of `switch (getIndex())` | 17212 | 6079 | 82.973463 | **4/2** |
| `switch (stationIndex)` + `if (stationIndex == 108)` | 17204 | 6057 | 67.850066 | **4/2** |

`decl_init` flips the slot but the CFG change (unconditional raid computation)
costs 8.4 pp. The `switch (stationIndex)` variants flip it but reorder the
mission-case blocks.

## 3. `b2e04` merged station predicate (`(i != first) | !alienWave`)

The original materialises the negation as a value and emits one `orrs` +
`bne`; the baseline emits two short-circuit branches. `(i==first) & alienWave`
with `bool` operands canonicalises back to the baseline IR. Casting to `int`
or `unsigned` does produce the original `orrs`, but it also re-orders the whole
function: the alienGuns loop moves from `0xb32a4` to `0xb1d8c` and the
pirateFortress block from `0xb3398` to `0xb375c`, collapsing the fuzzy score.

| variant | size | ours insns | pct |
|---|---:|---:|---:|
| **baseline (`&&`)** | **17172** | **6063** | **92.028388** |
| `(i==first) & (alienWave != 0)` | 17172 | 6063 | 92.028388 (same IR) |
| `(bool)(i==first) & (bool)alienWave` | 17172 | 6063 | 92.028388 (same IR) |
| `(int)(i==first) & alienWave` | 17180 | 6065 | 74.389439 |
| `(unsigned)(i==first) & (unsigned)alienWave` | 17180 | 6065 | 74.389439 |
| `(i==first) & (alienWave & 1)` | 17180 | 6065 | 74.389439 |
| `(i==first) * alienWave` | 17172 | 6063 | 90.642020 |
| `(i==first) & (alienWave == 1)` | 17172 | 6063 | 90.642020 |
| `!(i != first \| !alienWave)` | 17164 | 6057 | 84.230515 |
| `(int)(i==first) & alienWave` + inline hunter-loop bound | 17172 | 6060 | 74.651259 |

The `int`-cast variant is size 17180; pairing it with the -8-byte inline
hunter-loop bound restores size 17172 but the block re-ordering persists
(74.65). The re-ordering, not the size, is the cost.

## 4. Hunter-loop bound (`case4Tail`)

The landed `unsigned n = enemies->size()` cache is a size-tuning hack (the
original reloads `size()` each iteration). Re-confirmed as the best of the
family.

| variant | size | ours insns | pct |
|---|---:|---:|---:|
| **`unsigned n = enemies->size()` (landed)** | **17172** | **6063** | **92.028388** |
| `int n` / `int i` / `unsigned n` + `int i` | 17172 | 6063 | 92.028388 |
| `i <= n - 1` | 17172 | 6063 | 92.011883 |
| `unsigned n = hunterCount` | 17164 | 6060 | 91.159719 |
| `int n = hunterCount` | 17164 | 6060 | 91.176228 |
| inline `i < enemies->size()` (`unsigned` or `int`) | 17164 | 6058 | 91.191282 |

## 5. Other probes (all neutral)

* `Vector v`/`base`/`Matrix rotation` declaration order permutations:
  `swap_v_base` 91.863344, `rot_after` 91.846839, `base_before_rot` 91.780822.
* `const KIPlayer *gun`, `const Vector base`, `Matrix rotation = Matrix()`,
  `Vector v = Vector(0,0,0)`: all 92.028388.
* `if (!nameLeader) return;` / `if (!mission) return;`: 92.028388.
* Extra `return;` after the nameLeader store: 17148, 6053, 88.553023.
* Swapping the `==68` / `==33` campaign checks: 91.995379.
* Merging them into `cm == 68 || cm == 33`: 17172, 6057, 88.061427.
* `unsigned stationIndex`: 92.028388.
* `float offset[3]` → named `Vector`: zero stores, already documented.

## 6. Remaining diff (baseline)

`difflib` opcodes: 478 original instructions and 486 ours are non-equal out of
6055/6063. 167 are single-instruction replaces; the largest clusters are:

| offset | orig/ours insns | region |
|---|---:|---|
| `0x32c6` | 14 / 16 | alienGuns table selector + first vector |
| `0x397c` | 12 / 12 | raid loop bound association / register roles |
| `0x2e04` | 8 / 5 | merged station predicate (`orrs`) |
| `0x1408` | 6 / 5 | convoy offset if-chain order |
| `0x3dd8` | 2 / 7 | blueprint slot sum association |
| `0x3c48` | 1 / 7 | extraAliens spawn jitter (VFP regs vs stack) |
| `0x330c` | 1 / 7 | alienGuns second vector `t[0..2]` |

The `[sp,#84]/[sp,#88]` swap alone accounts for 18 of the single-instruction
replaces; the `[sp,#40]/[sp,#44]/[sp,#48]` sum-slot rotation accounts for ~6
more.

## 7. Proven vs hypothesis

Proven:
- The baseline is a local optimum for every size-preserving spelling tried:
  selector, vector construction, declaration order, `raid`/`security`/
  `raidRoll` types, hunter-loop bound, and the `b2e04` condition.
- The `int`/`unsigned`-cast `b2e04` spelling reproduces the original `orrs`
  instruction exactly, but re-orders the alienGuns and pirateFortress blocks
  and loses 17.6 pp. The block ordering, not the condition, is the blocker.
- `decl_init` and `switch (stationIndex)` flip the `raid`/guard slot, but both
  change the CFG enough to lose more than the ~18-instruction swap gains.

Hypothesis:
- The original's block layout is consistent with the baseline's, not with the
  `orrs` variant's. The original therefore reaches the `orrs` form through an
  IR shape whose branch-probability estimate differs from the `int`-cast
  variant's (the `orrs` is a single `bne`, which LLVM's BPI weights at 50 %,
  while the baseline's two short-circuit branches weight the else edge at
  ~75 %; the original must carry different weights). A `!prof` / branch-weight
  shape that keeps the blocks in place is the remaining lever.
- The `raid`/guard slot swap is downstream of that same branch-probability
  difference: the wave-90 note's epilogue-frequency margin is 0.0042.

## 8. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w5421
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# fast single-function score (official verifier normalisation, unrounded)
python3 /dev/shm/w5421/score2.py _ZN5Level13createMissionEv

# exact side-by-side
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level13createMissionEv
```

Scratch (not committed): `/dev/shm/w5421/` (`Level.base.cpp`, `score1.py`,
`score2.py`, `dstat.py`, `dstat2.py`, `trie.py`, `trie2.py`, `batch1..24.py`,
`show_base.txt`, `dstat*.txt`).

## 9. Gate counts (before → after)

```text
byte_exact      4500 → 4500   (4500/4524)
linked_exact    4500 → 4500   (4500/4524)
size_exact      4524 → 4524
stub_zero_size     0 → 0
missing            0 → 0
wrong_type         0 → 0
extra              0 → 0
data_exact       843 → 843
imports          207 → 207
lint            CLEAN   → CLEAN
drift           0       → 0
RATCHET         PASS    → PASS
```
