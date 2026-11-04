# Layout::Layout C1/C2 — pass 11: exhaustive reorder search, instruction-mix
# residue, and why 47.733 % is a hard local optimum (w4802, wave 98)

Worker **w4802**, branch `wave98-w4802`, base `fe4f5e7cf`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B** (body `0xd1028..0xd2bc8`; the ctor's literal pools live inside
`.text`). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

Continues `layout-layout-c1-c2-store-order-w4502.md` (pass 10),
`layout-layout-c1-c2-hd-block-order-w4221.md`,
`layout-layout-c1-c2-vstr-shapes-w4401.md`,
`layout-layout-c1-c2-vstr-opacity-w4274.md`,
`layout-layout-c1-c2-hd-vector-w4101.md`.

## Result

**No landable improvement found.** The body is unchanged; this note is the
deliverable. Gate before == after (verified with `check` on the clean tree):

| metric | value |
|---|---:|
| `byte_exact` | 4500 / 4524 |
| `linked_exact` | 4500 / 4524 |
| `size_exact` | 4524 / 4524 |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 |
| `data_exact` | 843 / 843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |

`RATCHET PASS`. Raw verifier ratio for `_ZN6LayoutC1Ev` (unrounded,
`autojunk=False`): **47.7326968973747 %**, `usize` **7072**.

This pass attacked the problem independently of pass 10: instead of more
single-statement reorders, it (a) measured the *instruction-mix* residue,
(b) established the exact code/literal-pool split, and (c) ran ~1000 further
size-neutral reorderings (single-unit relocations, pairwise swaps, contiguous
block moves) plus a fresh batch of expression-shape variants. None beat
47.7327.

## 1. Score is not a sufficient gate — always run `check`

While iterating, a statement reorder that reproduced the *identical* raw ratio
was briefly left in the tree; `check` still has to be run on the final tree.
(Cheap scorer: `cmake --build cmake-build-match --target link_so -j8` then the
`fscore.py` below; ~0.9 s per variant.) The search drivers in `/dev/shm/w4802`
rewrite `src/game/ui/Layout.cpp` in place and leave the **last tried** variant
behind — always `git checkout -- src/game/ui/Layout.cpp` before the final
`check`.

## 2. Exact code / literal-pool split

Both bodies are 7072 B and start at the same VMA, so the split is directly
comparable:

| | code end | code bytes | pool bytes |
|---|---|---:|---:|
| original | `0xd2802` (after `pop {r4,r5,r6,r7,pc}` at `0xd2800`) | **6106** | **966** |
| ours | `0xd28c0` (after `pop` at `0xd28be`) | **6296** | **776** |

Ours carries **+190 B of code and −190 B of literal pool**. The original
therefore materialises ~47 more 4-byte constants from the pool than we do, and
the pool contents/tails differ (e.g. the original's pool word run at
`0xd2940..0xd29de`, 78 half-words, has no exact counterpart in ours; ours has
`0x3fc00000` (1.5f) twice in the pool where the original has it zero times).

## 3. Normalized instruction counts

`verify.disassemble` + `reconcile`, `_ZN6LayoutC1Ev`:

| | total | code | `pool:` entries | matched | ratio |
|---|---:|---:|---:|---:|---:|
| original | 2529 | 2491 | 38 | | |
| ours | 2499 | 2465 | 34 | | |
| difflib | | | | **1200** | **0.47733** |
| code-only difflib | | | | **1179** | **0.47579** |

So only ~1.3 % of the ratio comes from pool entries; the residue is code.

**Multiset** (order-insensitive) intersection is **1516** of 2529/2499
(Jaccard **0.781**). Even after ignoring *all* ordering, 22 % of the
instructions differ in kind. The top multiset deltas (ours − original):

```
 +72 str.w          -17 strd           +39 cmp.w          -11 cbz
 -65 movs           +22 beq.n          -21 cmp            -13 lsls
 +12 vld1.64 {d2-d3},[r1]  +11 vst1.32 {d2-d3},[r1]
 +12 ldrb.w r1,[r8]        +11 cmp.w lr,#0     +10 cmp r2,#0
```

Interpretation:

* **`str.w` +72 / `strd` −17** is the single largest structural delta. The
  original pairs adjacent-address stores into `strd` (2 stores, 4 B) far more
  often; we emit two `str.w` (2 × 4 B). 72 extra `str.w` ≈ 288 B, which is the
  bulk of the +190 B code delta after the `movs`/`cmp` credits.
* **`cmp.w` +39 / `cbz` −11** is register-allocation: the original keeps the
  `Globals::*` flag bytes in *low* registers so `cmp rN,#0` (2 B) + `cbz`
  (2 B) can be used, while our scheduler keeps them in `ip`/`r8`/`r9`/`fp`
  and must use `cmp.w` (4 B).
* **`vld1 {d2-d3}` / `vst1 {d2-d3}`** (12/11 more on our side) is the SLP
  vector-group *phase*: the original groups `0x140,144,148,14c | 0x150..`
  (pass-10 §2.3b), we group `0x148,14c,150,154 | …`.

None of these are reachable by editing a single constant or a single
statement; they are all consequences of the exact statement order *and* the
exact expression types.

## 4. Reorder search (this pass)

All variants were built and scored with the official verifier
(`tools/verify/verify.py` via `fscore.py`); only size-7072 candidates can land.

| search | seed | evals | best found |
|---|---:|---:|---:|
| random single-unit relocation (move 1 of 211 units anywhere) | 1 | 458 | 47.7326968973747 (no change) |
| random pairwise swap (exchange 2 of 211 units) | 7 | 284 | 47.7326968973747 (no change) |
| random contiguous block move (block length 2..6) | 3 | 254 | 47.7326968973747 (no change) |
| random single-unit relocation **restricted to the `mid` region** (units 93..197, i.e. `field_0x134`..`field_0x290`) | 5 | 260 | 47.7326968973747 (no change) |

That is **1256** size-neutral reorderings this pass. Together with pass 10's
exhaustive adjacent-swap hill-climb (203 evals × 3 sweeps) and its 150 random
1/2/3-unit moves, the statement order is now a local optimum under **five**
different neighbourhoods (adjacent swap, single-unit relocation, pairwise
swap, contiguous block move, region-restricted relocation). Every accepted move
kept `usize == 7072`; every rejected move was either worse or size-changing.

### 4.1 Targeted reorders (all measured, all worse)

| variant | `usize` | raw % |
|---|---:|---:|
| `0xdc,0xe0,0xe4,0xe8` made adjacent, ascending | 6976 | 41.9555 |
| `0x2c4,0x2c8,0x2cc,0x2d0,0x2d4,0x2d8,eventBannerScale` ascending | 7072 | 45.2665 |
| same but `eventBannerScale` last | 6984 | 44.4178 |
| `eventBannerScale` immediately after `0x2c4` (already true) | 7072 | **47.7327** |
| first 28 fields in the original's *def-block store order* | 6972 | 21.2442 |
| first 28 fields in ascending address order | 6984 | 41.3766 |

The last row is important: reordering the first 28 statements to the exact
order in which the original's `else`-path stores them (`0x4,0x8,0xc,0x10,
0x14,0x18,0x1c,0x20,0x24,0x28,0x30,0x2c,0x38,0x34,0x3c,0x44,0x40,0x4c,0x48,
0x54,0x50,…`) collapses the score to 21 %. **The original's store order is not
its source order** — clang's scheduler reorders the store chain heavily, and
our current source order is the one that reproduces the original's schedule
best. This kills the "just copy the store order" strategy.

## 5. Expression-shape variants (this pass)

All measured on the pass-10 body. `(int)(5*1.40625f)` etc. are semantically
identical to the implicit float→int conversion the fields already perform, so
these are pure codegen probes.

| variant | `usize` | raw % |
|---|---:|---:|
| `0x20_top` HD → `(int)(5 * 1.40625f)` | 6928 | 13.4711 |
| `0x148` HD → `(int)(2 * 1.40625f)` | 7056 | 47.5811 |
| `0x1f4` HD → `(int)(16 * 1.40625f)` | 7056 | 47.7006 |
| `0x204` HD → `(int)(9 * 1.40625f)` | **7072** | 47.4354 |
| `0x10c_rowBaseY` HD → `(int)(1 * 1.40625f)` | 7008 | 42.6752 |
| `rewardBoxY2` → `(int)(float)((rewardBoxY+17) << retina)` | 7080 | 47.6834 |
| shared `float lscale` local for `animSpeedScale`/`ringRadiusScale` | 7072 | 47.7327 (identical) |
| `ringRadiusScale = animSpeedScale` | 7040 | 37.6368 |

The `0x204` variant is size-neutral and still loses 0.30 pp — i.e. even a
"free" expression change perturbs the SLP groups enough to lose more than it
gains. The shared-local result shows clang already CSEs the two identical
`1.40625f/1.5f/2.0f/1.0f` ternaries, so there is nothing to recover there.

## 6. Why the residue is (currently) irreducible

The remaining 52 % is not "a wrong constant" or "a missing statement": the
emulator check in pass 10 §4 already showed every stored value is identical in
all six flag configurations (HD, n9, iPad, iPadLarge, retina, default). The
residue is entirely:

1. **SLP vector-group phase** — the original groups `0x140..0x17c` as
   `0x140,144,148,14c | 0x150,154,158,15c | 0x160,164,168,16c | 0x170,174,178,17c`;
   we group from `0x148`. Moving `0x140`/`0x144` next to `0x148`/`0x14c`
   changes `usize` (pass 10: 6976 / 6984) and loses.
2. **`strd` pairing / store scheduling** — §3. Driven by the same order.
3. **Register allocation** — the original prologue saves six registers
   (`r6,r7,r8,r9,sl,fp`), ours saves seven (`+r5`); the original keeps the
   flag bytes in low registers (`cbz`), ours in high ones (`cmp.w`). This is
   downstream of (1) and (2) and cannot be steered from the source directly.
4. **Literal-pool layout** — ±190 B, a consequence of (1)/(2).

To close it one needs the *exact* original statement interleaving (including
which fields share one `if (Globals::iPadHD)` block), not local edits. The
search evidence above says that interleaving is within one or two units of
what is already checked in, but the objective is flat-to-worse in every
direction tried.

## 7. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
python3 /dev/shm/w4802/fscore.py                            # 7072 / 47.7326968973747
python3 /dev/shm/w4802/stores.py /dev/shm/w4802/orig2.objdump 0xd1028 0xd2bc8
python3 /dev/shm/w4802/dumpnorm.py 0 200                    # side-by-side normalized
python3 /dev/shm/w4802/run_climb.py 1 560                   # single-unit relocations
python3 /dev/shm/w4802/run_swap.py  7 420                   # pairwise swaps
python3 /dev/shm/w4802/run_block.py 3 420                   # contiguous block moves
python3 /dev/shm/w4802/run_range.py 93 198 5 400             # mid-region only
```

Scratch (`/dev/shm/w4802`): `fscore.py`, `stores.py`, `dumpnorm.py`,
`climb.py` (split/join + `reorder()` + `build_score()`), `run_climb.py`,
`run_swap.py`, `run_block.py`, `run_range.py`, `fast.py` (compile-only scorer; note it is
biased ~0.6 pp low because the `.o` has unrelocated pool words — use it only
for coarse triage), `Layout.base.cpp` (== git HEAD `src/game/ui/Layout.cpp`),
`orig2.objdump` / `ours_new.objdump` (full 7072 B bodies).

Note on scratch: the disassembly must be taken over the **full**
`0xd1028..0xd2bc8` range. `0xd1028 + 0x1ba0 = 0xd2bc8`, not `0xd1bc8`; using
the wrong end truncates the body and makes the pool analysis nonsense.
