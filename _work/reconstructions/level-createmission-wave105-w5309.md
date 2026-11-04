# w5309: `Level::createMission` (92.028388 %, 17172 B) — wave-105:
# the in-loop `cbz` selector is reachable only via a duplicated first-vector body

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w5309`, branch `wave105-w5309`,
base `835748c18`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).
**No source change is committed**: every variant below is neutral or a regression.
The deliverable is this note.

## Baseline (unchanged, gate green before and after)

```text
_ZN5Level13createMissionEv   fuzzy 92.028388 %   size 17172 B   @0xaffd8
                             orig 6055 insns / ours 6063 insns
gate: byte 4500/4524, linked 4500/4524, size 4524/4524, stubs 0, missing 0,
      wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
      RATCHET PASS
```

`cmake --build cmake-build-match --target check -j6` passes before and after.
The working tree is clean; `report.json` is not modified/committed.

## Result in one line

The original's alienGuns table select is an **in-loop branchy `cbz`**:

```text
b32ee ldr   r0, [sp, #64]      ; bigAlienWave
b32f0 cbz   r0, b32f8
b32f2 ldr   r1, [pc, #732]     ; kAlienStationTurrets
b32f4 add   r1, pc
b32f6 b.n   b32fc
b32f8 ldr   r1, [pc, #728]     ; kAlienOutpostTurrets
b32fa add   r1, pc
b32fc add.w r0, r1, r9         ; r9 = k*24
```

The current `(bigAlienWave + (i < 0))` anchor keeps the select in the loop but
lowers it **branchless** (`cmp.w r1, r2, asr #31; it ne; movne`). I found a
source shape that reproduces the original's branchy `cbz` exactly: **duplicate
the first-vector computation in an `if (bigAlienWave) … else …`** and share the
second vector after the merge. Clang then tail-merges the two branches into a
single table-select and emits the original `cbz`. This is also what the Unity
oracle shows (`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/
native/game/Level.c` ~lines 3046-3058: the first vector is computed in both
arms, `puVar15` is assigned per arm, and the second vector uses `puVar15`).

The catch: the duplicated body moves the `Vector` argument temporary from
`sp+200` (original and current base) to `sp+212`, swapping it with the
`MatrixRotateVector` result temporary. That 12-byte slot swap cascades into the
loop's callee-saved register assignment and costs ~3.4 pp, so the branchy
selector is not landable on its own.

## 1. The branchy-selector shape (not landed)

```cpp
const float (*t)[6];
if (bigAlienWave) {
    t = kAlienStationTurrets;
    v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
    gun->geometry->setRotation(v);
} else {
    t = kAlienOutpostTurrets;
    v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
    gun->geometry->setRotation(v);
}
v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
```

Linked disassembly (ours, `dup-first`):

```text
32e8 ldr   r0, [sp, #64]       ; bigAlienWave
32ea cbz   r0, 0x32f2          ; <-- original shape, exact
32ec ldr   r6, [pc, #728]
32ee add   r6, pc
32f0 b.n   0x32f6
32f2 ldr   r6, [pc, #728]
32f4 add   r6, pc
32f6 add.w r0, r6, r9
32fa add.w r8, sp, #312        ; rotation
32fe ldr   r2, [r0, #16]       ; t[4]
3300 ldr   r1, [r0, #12]       ; t[3]
3302 ldr   r0, [r0, #20]       ; t[5]
3304 str   r2, [sp, #216]      ; <-- vector arg at sp+212..220
3306 mov   r2, r4
3308 str   r1, [sp, #212]
330a mov   r1, r8
330c str   r0, [sp, #220]
330e mov   r0, sl              ; result temp = sp+200
3310 blx   MatrixRotateVector
```

Original (same region):

```text
b32ee ldr   r0, [sp, #64]
b32f0 cbz   r0, b32f8
...
b32fc add.w r0, r1, r9
b3300 mov   sl, r1
b3302 add   r4, sp, #312
b3304 ldr   r1, [r0, #8]       ; t[2]
b3306 ldr   r2, [r0, #16]      ; t[4]
b3308 str   r1, [sp, #116]
b330a str   r2, [sp, #204]
b330c add   r2, sp, #200       ; <-- vector arg at sp+200..208
b330e ldr   r1, [r0, #12]      ; t[3]
b3310 ldr.w r8, [r0, #4]       ; t[1]
b3314 ldr   r0, [r0, #20]      ; t[5]
b3316 str   r1, [sp, #200]
b3318 mov   r1, r4
b331a str   r0, [sp, #208]
b331c mov   r0, r5              ; result temp = sp+212
b331e blx   MatrixRotateVector
```

So the selector is exact but the two 12-byte temporaries are swapped
(`arg/result` = `sp+212/sp+200` ours vs `sp+200/sp+212` original), and the six
float loads are scheduled differently (`t[4],t[3],t[5]` vs
`t[2],t[4],t[3],t[1],t[5]`).

## 2. Measurements (robust harness, official verifier normalisation)

`pct` is the unrounded `difflib.SequenceMatcher(autojunk=False).ratio()*100`
(same normalisation as `verify.py`; the report row rounds to 92.0).

| variant | size | ours insns | pct |
|---|---:|---:|---:|
| **baseline (`i < 0` anchor, branchless)** | **17172** | **6063** | **92.028388** |
| `dup-first` (branchy `cbz`, shared second vector) | 17156 | 6062 | 88.635801 |
| `dup-first` + named `Vector dir` inside each arm | 17156 | 6062 | 88.487249 |
| `dup-both` (both vectors duplicated per arm) | 17156 | 6064 | 89.660863 |
| `dup-first` + `Vector dir` before the arms, reused | 17212 | 6084 | 82.164923 |
| `dup-first` + `float dir[3]` before the arms, reused | 17156 | 6062 | 82.413139 |
| `dup-first` + all 24 declaration permutations | 17148-17156 | 6062 | 88.37-88.64 |
| `&kAlienStationTurrets[k]` + `t[k]` (wrong: row 2k) | 17172 | 6061 | 92.076593 |
| `volatile int bw = bigAlienWave; bw ? A : B` | 17172 | 6063 | 92.011883 |
| `bigAlienWave ? A : B` (plain, hoisted out of loop) | 17140 | 6058 | 88.219269 |
| `switch` / `if-else` / `default-then-else` selector | 17140 | 6058 | 88.219269 |
| `const float *row = bigAlienWave ? A[k] : B[k]` (correct 1-D) | 17148 | 6054 | 83.640268 |
| `const float (*t)[6] = bigAlienWave ? A : B; row = t[k]` | 17148 | 6060 | 88.171688 |
| `volatile` table pointer (`const float (*volatile t)[6]`) | 17180 | 6076 | 87.099167 |
| `volatile` condition copy (`volatile int bw`) | 17172 | 6063 | 92.011883 |

The `&A[k]` + `t[k]` row is the only variant that scores *above* the baseline
(92.076593) while keeping size exact, but it is **semantically wrong**: `t`
already points at row `k`, so `t[k]` selects row `2k`. The original's
`add.w r0, r1, r9` with `r9 = k*24` proves it selects row `k`.

## 3. Other probes (all neutral or regressions)

* **`raid` / status-pointer stack-slot swap.** The original has `raid` at
  `sp+84` and the cached `Globals::status` spill at `sp+88`; ours is exactly
  swapped (`sp+88` / `sp+84`). This accounts for ~18 one-instruction diffs
  (`0x4e8`…`0xa72`, `0x2a32`, `0x377e`). Changing `raid` to `int`, `unsigned`,
  `unsigned char`, `short`, `long`, `volatile bool`, `bool[2]`, `bool[4]`, or
  moving the declaration to the top of the function does **not** swap the
  slots; `bool` is the best type (92.028388; every other type 91.86 or lower).
  The swap is a register-allocator spill-slot ordering artifact.
* **Merged station predicate** (`0x2e2c`): original emits
  `subs; mov; it ne; movne; ldr; eor; orrs; bne` (`(i != first) | !alienWave`
  in one `orrs`); ours emits two branches. `&&`→`&`, `!= 0`, `!(… || …)`,
  `i - first == 0`, and the same on the `pirateFortress` arm are all neutral
  (clang canonicalises to the same IR).
* **Convoy offset if-chain** (`0x1444`): original tests `k == 2`, `k == 1`,
  then `cbnz` (source order 2,1,0); ours canonicalises to 0,1,2. `unsigned k`,
  `switch (k)` with cases 2,1,0, and a `default`-before-0 if-chain are all
  neutral.
* **`case 9` crate stores** (`0x1964`): original is `str cargo; strb
  crateCaptured`; ours is `strb; str`. Swapping the two source statements does
  not change the emitted order (clang schedules them), and loses 0.016 pp.
* **`case 4` hunter loop** (`0x41ec`): original reloads `enemies->size()` each
  iteration; the landed `unsigned n` local caches it. The reload variant is
  17164 B / 91.191282 (already documented in wave-102).
* **Call stream**: all 809 `bl`/`blx` sites match in order and target; only
  six calls use `r4` instead of `r5`. The logic/call order is therefore
  correct; the residual is pure codegen.

## 4. Remaining diff (baseline)

`difflib` opcodes: 479 original instructions and 487 ours are non-equal out of
6055/6063. Largest clusters (offset = real address - 0xaffd8):

| offset | orig/ours insns | region |
|---|---:|---|
| `0x32ee` | 16 / 16 | alienGuns table selector + first vector |
| `0x397c` | 12 / 12 | raid loop bound association / register roles |
| `0x2e2c` | 8 / 5 | merged station predicate (`orrs`) |
| `0x1444` | 8 / 8 | convoy offset if-chain order |
| `0x3e02` | 7 / 7 | blueprint slot sum association |
| `0x3c94` | 7 / 7 | extraAliens spawn jitter (register vs stack) |
| `0x3334` | 7 / 7 | alienGuns second vector t[0..2] |
| `0x2f54` | 6 / 1 | pirateFortress predicate tail |

The rest are one- or two-instruction register/slot differences.

## 5. Next hypothesis

The original source almost certainly duplicates the first-vector computation
(oracle-supported) and shares the second vector after the merge. To land it,
the `Vector` argument temporary must stay at `sp+200` while the
`MatrixRotateVector` result stays at `sp+212`. The likely missing detail is that
the original reuses a **single named `Vector` local** for the argument across
both arms and the second vector (the oracle's `local_124`), but every named
spelling tried here either adds the default-constructor zero stores
(`Vector dir;` → 82.16) or reorders the slot the wrong way. A future pass should
try:

1. A named `Vector dir` with a *non-zeroing* construction (e.g. assign the
   first arm's values into a `Vector` that is already live), or
2. A `float dir[3]`/`Vector` declared in the *outer* scope with a lifetime that
   starts before the `if` and ends after the second `MatrixRotateVector`, so
   StackSlotColoring keeps it at `sp+200`, or
3. Reproducing the original's load order `t[2],t[4],t[3],t[1],t[5],t[0]` (the
   reversed pairs are why the original does not form `ldrd`; ours pairs
   `t[1],t[2]` and `t[3],t[4]`).

Until then the `(bigAlienWave + (i < 0))` anchor is the best size-neutral
shape: it keeps the select in the loop and the vector slot at `sp+200`, at the
cost of a branchless `it ne; movne` instead of the original `cbz`.

## 6. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w5309
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# fast single-function score (imports tools/verify/verify.py; unrounded ratio)
python3 /dev/shm/w5309/score1.py _ZN5Level13createMissionEv

# exact side-by-side
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level13createMissionEv
```

Scratch (not committed): `/dev/shm/w5309/` (`Level.base.cpp`, `score1.py`,
`dis.py`, `region.py`, `try.py`, `batch2.py`, `fnrep2.py`, `v*.py`,
`diff.txt`, `diff_dup.txt`, `show.txt`).

## 7. Gate counts (before → after)

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
