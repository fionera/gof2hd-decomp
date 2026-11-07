# w8312 (wave-138): `Level::createMission` — case-183 offset block branch order (fuzzy 92.34 -> 92.49 %)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8312`, based on master `96b069792`.
Continues `level-createmission-wave137-w8214.md`, `…-wave136-w8112.md`,
`…-wave135-w7776.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; `_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17172 B).
Manual Thumb disassembly (objdump) plus the official `tools/verify/verify.py` fuzzy metric
(`difflib` ratio over the normalised instruction stream, unrounded). Build/flags unchanged
(NDK r18b clang 7.0.2, `-Oz`).

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   official fuzzy 92.3432 %   size 17172/17172 B
                             orig 6055 / ours 6065 insns
.text diff_bytes 85,535 (whole section)
.ARM.extab 1,395   .ARM.exidx/.rodata/.data/.bss exact
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## New hypothesis: steer the `case 183` cold-block placement from the source

The previous waves measured the `case 183` offset `if`-chain with the **positional**
`raw fn differing bytes` metric and concluded that clang "canonicalises" the body order
and that no source order helps (wave 135 §3: "`0123` and `0213` both give 5761").  That
metric is dominated by the literal-pool reorder (a 4-byte shift of every `ldr [pc,#…]`
in the block), so it cannot see a body-order win.  Re-scoring the six valid `else`-last
permutations with the **official** fuzzy metric shows a real win.

The block is the convoy `offset[3]` switch inside `case 183`:

```cpp
float offset[3];
...
if (k == 0)      { offset[0]=20000; offset[1]=-3000; offset[2]=-50000; }
else if (k == 1) { offset[0]=10000; offset[1]=-7000; offset[2]=-30000; }
else if (k == 2) { offset[0]=25000; offset[1]=0;     offset[2]=-36000; }
else             { offset[1]=6000;  offset[2]=-45000; offset[0]=12000; }
```

The four bodies are **mutually exclusive** (`k` is the `for (int k = 0; k < 4; k++)`
convoy index), so the test order is semantically free.  Clang emits the bodies in a
different order than the source, and the literal pool follows the emitted body order;
the pool reorder is what shifts every PC-relative load in the `0xb13b0..0xb1600` block.

### Emitted order / pool per source permutation (official fuzzy)

| source order | emitted body layout | literal pool order | fuzzy |
|---|---|---|---|
| **`0,1,2,else` (baseline)** | 0,2,1,default | b0 b2 b1 b3 | **92.3432** |
| `0,2,1,else` | 0,2,1,default | b0 b2 b1 b3 | 92.3432 |
| `1,0,2,else` | 1,0,2,default | b1 b0 b2 b3 | 92.3597 |
| `1,2,0,else` | 1,0,2,default | b1 b0 b2 b3 | 92.3597 |
| **`2,0,1,else` (landed)** | 2,0,1,default | b2 b0 b1 b3 | **92.4917** |
| `2,1,0,else` | 2,0,1,default | b2 b0 b1 b3 | 92.4917 |

The original's emitted layout is `0,1,2,default` with pool
`b0(-3000,20000,-50000) b1(-7000,10000,-30000) b2(25000,-36000) b3(6000,12000,-45000)`
and dispatch `cmp r1,#2; beq body2; cmp r1,#1; beq body1; cbnz default; body0`.
No source permutation reproduces that exact layout under clang 7.0.2 (the `else if
(k==3)` form does emit `0,1,2,3` but loses the `cbnz` default edge, 6064 vs 6065
instructions, and scores 92.4169).  The `2,0,1,else` rotation keeps the 6065-instruction
shape and scores highest.

Nested `if` forms (`if (k!=0){…} else …`, a nested `else { if … }`) and the
`else if (k == 3)` default were also measured; all are neutral or worse (92.3432 /
92.4169).  Reordering the stores *inside* body 3 (`offset[1],offset[0],offset[2]`)
changes the size to 17188 and drops to 89.86, so the body-3 store order is kept.

## Tail / alien blocks (re-verified this wave, no change)

The wave-137 one-liner tail is kept.  Re-measured with the official metric:

| variant | size | fuzzy |
|---|---:|---:|
| **checked-in tail (latch + one-liner)** | **17172** | **92.3432** |
| faithful `this->enemies->size()` loop condition | 17164 | 91.3578 |
| faithful condition + `Level *self` / `count` / `int` / cast | 17164 | 91.3578 |
| alien `setScaling` after `setPosition`, fresh receiver | 17172 | 92.2937 |
| alien `setScaling` after `setPosition`, `gun` receiver | 17172 | 92.2264 |
| alien `cargo` before `shipGroup` | 17172 | 92.3267 |
| fortress `cargo` before `shipGroup` | 17172 | 92.2937 |
| turret table-select anchors (`|`, `||`, `>>31`, `!`, ternary) | 17172 | 92.18–92.34 |

The faithful `size()` condition is 8 B short because `this` stays in `sl` and the two
original `this` reloads (`ldr.w r8,[sp,#124]` at `0xb40b6`, `ldr r4,[sp,#124]` at
`0xb3232`) disappear; every spelling that keeps the re-read coalesces to the same
allocation.  The `(bigAlienWave + (i < 0))` table-select anchor still if-converts to
`cmp.w r1,r9,asr #31; it ne; movne` instead of the original in-loop `cbz`.

## Landed source

```cpp
// The four offsets are mutually exclusive; testing k == 2
// first lets clang place the cold blocks (and their
// literal pool) in the original's order.
if (k == 2) {
    offset[0] = 25000.0f;
    offset[1] = 0.0f;
    offset[2] = -36000.0f;
} else if (k == 0) {
    offset[0] = 20000.0f;
    offset[1] = -3000.0f;
    offset[2] = -50000.0f;
} else if (k == 1) {
    offset[0] = 10000.0f;
    offset[1] = -7000.0f;
    offset[2] = -30000.0f;
} else {
    offset[1] = 6000.0f;
    offset[2] = -45000.0f;
    offset[0] = 12000.0f;
}
```

Semantics are unchanged (the four cases are disjoint), the function stays at exactly
17,172 B, and the official fuzzy rises **92.3432 -> 92.4917 %** (+0.15 pp).  The
positional `.text` diff rises 85,535 -> 85,538 (+3 B) because the pool rotation moves
three PC-relative load offsets; the instruction-level match is better despite that.

## Gate counts (before -> after)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      85,535 -> 85,538
.ARM.extab diff  1,395 -> 1,395
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8312/measure.py     # exact official fuzzy for the one function
python3 /dev/shm/w8312/textdiff.py    # whole-.text differing bytes
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch: `/dev/shm/w8312/{Level.base.cpp,Level.201.cpp,measure.py,diffregions.py,
calls.py,runvar.py,textdiff.py,orig.asm,ours.asm}`.
