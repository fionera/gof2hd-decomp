# Layout::Layout C1/C2 — body-form and order sweeps (w4001, pass 7)

Status: analysis-only pass (no code change). Worktree `wt/w4001`, branch
`wave90-w4001`, base `360c7467`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
This note continues `layout-layout-c1-c2-midtail-vstr-w4001.md` (pass 6) and
records the body-form / statement-order / expression-form sweeps run after it.

## Result

`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` stay at **47.7% / 7072 B**. Gate unchanged:

| metric | before | after |
|---|---:|---:|
| `byte_exact` | 4498 / 4524 | **4498 / 4524** |
| `linked_exact` | 4498 / 4524 | **4498 / 4524** |
| `size_exact` | 4524 / 4524 | **4524 / 4524** |
| `data_exact` | 843 / 843 | **843 / 843** |
| imports | 207 | **207** |
| `.text` / `.rodata` | `0x1337c8` / `0x41b67` | **unchanged** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

`RATCHET PASS`. No size-exact candidate above 47.7 was found.

## 1. Measured body forms (official verifier, `autojunk=False`)

All variants keep the 204 assignments and the 32-combination value table
(`_work/reconstructions/layout_ctor/lay_orig.json`, 0 bad combos).

| variant | size | pct |
|---|---:|---:|
| current `src/game/ui/Layout.cpp` (13 if/else, hill-climbed order) | **7072** | **47.7** |
| all-ternary body (w3821, all-ternary order) | 6800 | 50.8 |
| all-ternary body, current statement order | 7056 | 45.0 |
| all-ternary body + the same 13 if/else conversions (all-ternary order) | 6884 | 30.9 |
| current body, iOS 1.1.4 source order | 6880 | 37.4 |
| current body, default-path store order | 6824 | 28.7 |
| current body + `field_0x98` if/else | 7016 | 47.8 |
| current body + `field_0x98` if/else + `field_0xa0` if/else | 7072 | 43.2 |
| iOS order + every HD arm written `ipad * 1.40625f` | 6692 | 33.8 |

The `field_0x98` if/else form is the only measured variant that beats 47.7
(47.8), but it is 7016 B, so it is not landable: a 56 B real-code construct
would have to be added without disturbing the instruction stream, and every
tested construct that reaches 7072 loses more than 4 points (§4).

## 2. Exhaustive single if/else conversion sweep on the all-ternary body

Starting from the 6800 B all-ternary body, each of the 204 assignments was
converted to the equivalent `if/else-if` chain (parser: `search/tern.py`,
generator: `search/mkvariant.py`). Sizes:

* range **6744 .. 6968 B**; no single conversion reaches 7072.
* largest: `field_0x1e8` +168 B (6968, 36.0), `rewardBoxY` +120 B (6920,
  42.2), `field_0x90` +116 B (6916, 41.3).
* 103 of 204 conversions stay at 6800 B; the rest mostly shrink.

So the original's +272 B cannot be a single if/else conversion.

## 3. Pair and triple conversion sweeps

All pairs of the 18 conversions with positive size delta: **no pair reaches
7072** (max 7016). All 816 triples of those 18 fields: exactly two reach 7072:

| triple | size | pct |
|---|---:|---:|
| `rewardBoxY, field_0x90, field_0x74` | 7072 | 40.7 |
| `rewardBoxY, field_0x12c, field_0x74` | 7072 | 37.5 |

Both are worse than 47.7. The conversions that add the most bytes are also the
ones that damage the instruction-stream alignment most.

## 4. Statement-order search

A 32-way parallel simulated-annealing / move search over the 204 statement
units (swap, move, reverse, block move; `search/sa.py`) was run on both the
current body and the `field_0x98` if/else body:

* current body: 60 rounds x 32 = 1920 evaluations; best size-7072 result
  stayed **47.7** (no improving move found).
* `field_0x98` if/else body: 120 rounds x 32 = 3840 evaluations; best
  size-7072 result **42.0** (it starts at 7016/47.8 and cannot recover the
  score after growing to 7072).

The current order is a strong local optimum of the fuzzy metric, consistent
with the earlier exhaustive adjacent-swap / pair / four-way sweeps in
`layout-layout-c1-c2-source-shape-w3905.md`.

## 5. Literal-pool content: where the 272 B lives

The original's final literal pool (`0xd2854..0xd2bc8`, 221 words) was compared
with the all-ternary body's pool (`0xd2804..0xd2ab8`, 173 words; the pass-6
note's `0xd27f4` start is 16 B early — `d27f4..d2802` is a cold block, and the
pool proper starts at the `0x0000 0x42800000 0x0000 0x42000000 0x3ea66666`
run at `d2804`). The original has 48 more words (65 original-only vs 17
all-ternary-only). The extra words are **not** new float constants; they are
small integer constants and GOT-relative pointers used repeatedly by the
outlined HD/n9/retina branches:

```
orig-only (sample): 00000004 x3, 00000016 x2, 0000000b, 0000000a x2,
  00000078, 0000012c, 00000258, 00000008 x2, 00000006, 0000000d,
  00000002 x3, ffffffff, 00000001 x4, 00000013 x3, 000000f3, 0000001d,
  00000010, 00000003, 0000000f, 00000012, fffffffe, 00000000 x2, ...
  0013df2e, 0013de3c, 0013ddb4, 0013db84  (GOT-relative pointers)
```

The original materialises these repeated constants with 16-bit
`ldr rX,[pc,#imm]` from the pool (2 B code + 4 B pool) instead of
`movs`/`movw` (2/4 B code). That is why the original is *smaller in code*
(6188 B vs 6296 B) but *larger in pool* (884 B vs 776 B); against the
all-ternary body the split is 6188+884 vs 6108+692. The 272 B is a code/pool
repartition driven by the compiler's outlined-branch schedule, not a missing
statement.

## 6. HD-block scheduling is the remaining frontier

Per-region fuzzy match of the current body against the original (instruction
ranges from the original's block map):

| region | original insns | matched | pct |
|---|---:|---:|---:|
| HD block `d1042..d1268` | 164 | 42 | 25.6 |
| n9 block `d127e..d12fc` | 39 | 21 | 53.8 |
| retina/default `d1570..d1830` | 260 | 156 | 60.0 |
| mid `d1860..d215c` | 742 | 214 | 28.8 |
| tail `d21e2..d2bd0` | 964 | 476 | 49.4 |

The iOS-order body matches the **n9 block 94.9%** (37/39) but the HD block only
3.0%: the iOS source order is right for the n9 region, and the HD block is the
dominant loss. The original's HD block stores the same fields in the same
pair-store order as our current body (`0x14/0x18, 0x4/0x8, 0x24/0x28, ...`),
but schedules the interleaved scalar stores differently (e.g. the original
stores `0xac` before `0x8c/0x90`, and hoists `0x74/0x78` before `0x2c0/0x2c4`).
That is a register-allocation / MachineScheduler difference, not a value or
statement difference: all 32 flag combinations still store the same 807 bytes.

The original prologue also saves one fewer callee-saved register
(`stmdb sp!, {r6,r7,r8,r9,sl,fp}` vs our `{r5,r6,r7,r8,r9,sl,fp}`), which
shifts the whole register assignment from the first instruction onward.

## 7. Reproduction

```sh
cmake --build cmake-build-match --target check -j8        # RATCHET PASS

# variant scorer (w4001 scratch)
python3 /dev/shm/w4001/scorep.py <Layout.cpp> <tag>
python3 /dev/shm/w4001/regions.py <so>                    # per-region match
python3 /dev/shm/w4001/side.py <so> <from> <to>           # side-by-side

# body-form sweeps
python3 /dev/shm/w4001/search/mkvariant.py <out.cpp> <idx,...>   # all-ternary base
python3 /dev/shm/w4001/search/conv_field.py <in> <out> <field>   # current base
python3 /dev/shm/w4001/search/sa.py <base.cpp> <tag> <rounds> <seed>
```

## 8. Remaining work

* Recover the exact Android 2.0.16 statement order for the HD block (the
  scheduler's pair-store schedule is reproducible only from the true source
  order plus the true expression forms). The iOS order is the best witness but
  reproduces only the n9 block.
* Find a 56 B real-code construct for the `field_0x98` if/else body (7016 B,
  47.8%) that does not perturb the hot path. Every hot-path test tried so far
  (a 5-flag `if` with 1-4 redundant stores) overshoots to 7104-7208 B and
  collapses the score to 23.3-23.9.
* The all-ternary body remains the highest fuzzy score (50.8) but is 272 B
  short; any landable candidate must be exactly 7072 B and beat 47.7.
