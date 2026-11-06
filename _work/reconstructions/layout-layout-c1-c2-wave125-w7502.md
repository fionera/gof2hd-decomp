# `Layout::Layout` C1/C2 — wave 125 / worker w7502

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, end `0xd2bc8`.
Source `src/game/ui/Layout.cpp` (constructor only).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Baseline = wave-124 master `02513de33`.

Assignment focus: SLP/vectorisation and literal-pool placement — reproduce the
original's 4-word `vst1.32` windows and literal-pool order **without changing
the 7072-byte size**.  Wave 124 had found that making `0xec,0xf0,0xf4,0xf8`
contiguous forms the missing `0xec` SLP group and matches the original's first
four pool words, but shrinks the body to 6952 B.

## Result — no metric win (readability refactor landed)

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (bytes identical) |
| unrounded `difflib` ratio | 48.934051 % | **48.934051 %** |
| `report.json` pct (1 dp) | 48.9 | 48.9 |
| instructions | 2529 / 2490 | 2529 / 2490 |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
Section deltas vs the original: **none** — `.text` `0x1337c8`, `.rel.plt`
`0x72d0`, `.ARM.exidx` `0x6f70`, `.ARM.extab` `0x26298` all at the same
address and size as the original, exactly as at wave 124.

The landed source change is a readability-only refactor: the artificial
`int v138;` temporary that fed `this->field_0x138` is gone and each branch of
the `iPadHD / n9 / iPadLarge / retinaDisplay / else` chain now assigns
`this->field_0x138` directly.  The generated function is **byte-identical**
(sha256 of the 7072-byte body `594fc57e…3ab4` before and after), so this is a
zero-risk cleanup, not a codegen change.

## 1. Why the `0xec..0xf8` variant cannot reach 7072 B

Re-measured on the current baseline (wave-124 quoted 6952 B against the older
`field_0x158 = 14` body; with the committed `this->field_0x154 - 1` arm the
same source shape is **6960 B**):

| variant | size | unrounded % | pool head `0.325` at |
|---|---:|---:|---:|
| baseline (HEAD) | **7072** | **48.934051** | `0x1520` (`.text` island) |
| `0xec,0xf0,0xf4,0xf8` contiguous (`ec`) | 6960 | 46.848655 | `0x186c` (pool head) |

The `ec` body is 112 B short.  It *does* put `0.325f, 0.65f, 100.0f, 50.0f`
at the head of the literal pool exactly like the original (`0xd2854`), and its
first pool words then align with the original's for 4 words — but the body is
112 B smaller, so `.text` shifts and `byte_exact` collapses (4506 → 2455,
`RATCHET FAIL`).  7072 B is a hard wall.

## 2. Pool alignment of the baseline is already 72.5 %

Dumping the two pools word-for-word (original `0x182c..0x1ba0`, 221 words;
ours `0x18a8..0x1ba0`, 190 words) and running `difflib` over the word lists
gives ratio **0.725**.  The opcode-level differences are small and specific:

```
orig[ 0.. 2]  0.325 0.65                     <- ours: absent (.text island 0x1520)
orig[ 2.. 4]  100   50                       <- ours[0..2]  identical
orig[ 4.. 8]  4 PIC deltas                   <- ours[2..7]  5 deltas
orig[ 8..10]  281.25 400                     <- ours[7..9]  identical
orig[10..11]  PIC delta                      <- ours[9..10] identical
orig[11..13]  200 150                        <- ours[10..12] identical
orig[13..15]  2 PIC deltas                   <- ours: 0xbf00bf00 (1 word)
orig[15..23]  4 22 25 12 11 11 10 270        <- ours[13..21] identical
orig[23..27]  60 120 360 146                 <- ours: 60/120 are `movs` immediates
...
orig[215..221] 200 100 150 144 150 76        <- ours[178..184] + 2 trailing deltas
```

So the baseline pool is already the original pool minus the `0.325/0.65`
head pair, two `add rX,pc` deltas, the `60/120` pair and one `15`, plus a few
extra words (`-2`, `0`, `82`, `41`, two trailing deltas).  The `ec` variant
fixes the head pair but loses 13 pool words overall (177 vs 190 vs 221), so it
is a net pool regression.

## 3. Variants measured this wave (real `link_so` builds)

All sizes are the linked `.dynsym` size; `%` is the exact unrounded ratio.

| variant | size | % |
|---|---:|---:|
| **baseline (HEAD)** | **7072** | **48.934051** |
| `ec` (`0xec..0xf8` contiguous) | 6960 | 46.848655 |
| `ec` + every single 4-word SLP window (22 windows) | 6896–7008 | ≤ 47.0 |
| `ec` + every pair of SLP windows (231 pairs) | 6904–6960 | ≤ 47.0 |
| `ec` + one statement moved to the end (42 group members) | 6824–7008 | — |
| `ec` + 150 random single-statement moves | none hit 7072 | — |
| per-field `if/else` chain for every ternary (current order) | 6984 | 20.146448 |
| all 11 constructor `if/else` blocks → ternaries | 7112 | 44.859813 |
| block 3 (`field_0x190`) → ternary | 7072 | 48.854353 |
| block 8 (`field_0x218`) → ternary | 7072 | 48.854353 |
| block 9 (`orbitInfoStationY`) → ternary | 7072 | 48.934051 |
| every subset of convertible blocks (128 combos) | 7072–7180 | ≤ 48.934051 |
| `field_0x130/0x134/0x138` block → 3 ternaries | 7088 | 48.616365 |
| `field_0x130` ternary only | 7012 | — |
| `field_0x130` ternary + `0x134` block + `0x138` ternary | 7088 | — |
| `field_0x130` ternary + `0x134` block + `0x138` block | 7096 | — |
| `field_0x130/0x134/0x138` block with direct member stores | **7072** | **48.934051** (identical bytes) |
| `v138` block moved to item 0/10/50/80/120/200/240/260 | 6896–7036 | — |
| `field_0x90` written as `(retina\|\|iPad\|\|n9) ? 0.325f : 0.65f` | 7072 | 48.934051 (identical) |
| `field_0x90` n9-first / retina-first chains | 7072 | 48.924303 |
| `field_0x90` as a local `float` | 6968 | 20.661986 |
| every adjacent statement swap (265 swaps) | 6956–7172 | best 48.934051 (neutral) |
| first 30 statements sorted ascending by offset | 7068 | 24.760383 |
| first 8 statements in original HD order `14 18 1c 20 4 8 c 10` | 7072 | 45.795138 |
| first 8 statements ascending `4 8 c 10 14 18 1c 20` | 7060 | 24.705648 |
| `0x90/0x9c` moved to the front | 6780 | — |
| `field_0x2f4` moved to the front | 7040 | 30.744723 |
| `field_0x2f8` moved to the front | 7048 | 30.129353 |
| `rewardBoxWidth` moved next to `field_0x90` | 7032–7052 | — |
| HD-branch hybrid (`if (iPadHD) {…} else {…}` split, w7402 `gen_hybrid.py`) | 5224 | 14.266304 |

No variant at 7072 B beats 48.934051, and no variant puts `0.325/0.65` in the
main pool while staying at 7072 B.  The only 7072-B shapes found are
codegen-identical to the baseline.

## 4. Original block layout (for the next wave)

Re-derived from the binary this wave (all offsets function-relative):

```
0x0000..0x0019  prologue, load Globals::iPadHD into sl, `beq 0x244`
0x001a..0x0240  HD branch (163 insns, 42 stores, 137 words)
                ends `b.w 0x11ba`  -> skips common1
0x0244..0x02d4  n9 branch (45 insns, 9 stores, 34 words), `b.n 0x838`
0x02d8..0x04b2  HD/n9 literal pool (includes the Globals::iPadHD GOT delta)
0x04b4..0x0546  n9 literal pool
0x0548..0x0836  else branch (retina / iPadLarge / iPad / default),
                278 insns, 38 stores, 49 words, falls through to 0x838
0x0838..0x11b8  common1 (785 insns, 119 stores, 231 words) — tree of
                per-flag if/else groups, only reached by the non-HD paths
0x11ba..0x1826  common2 (528 insns, 92 stores, 173 words) — reached by all
                paths; contains the backward `b.w` at 0x17fc/0x180a/0x1818/0x1826
0x1828..0x1ba0  main literal pool (221 words, starts `0.325 0.65 100 50`)
```

The HD branch and the common2 block are the two regions the current field-major
body matches best; the missing SLP groups all live in common1.

## 5. Remaining root cause

The original is a **config-major** constructor: a single `if (iPadHD)` at the
top, an `else if (n9)`, an `else`, then a shared tree (`common1`) that is only
executed on the non-HD paths and a final `common2` reached by every path.  That
shape is what makes clang 7.0.2 emit 16-byte-aligned `vst1.32` windows and the
`0.325/0.65` pool head.  The current field-major ternary body is a strong local
optimum of the fuzzy metric: 265 adjacent swaps, 128 block-shape combinations,
a 265-point "move to front" sweep and 150 random moves on top of `ec` all fail
to beat 48.934051 at 7072 B, and every config-major reconstruction measured
(wave 121–125) lands at 5.2–7.1 KB with 13–34 %.

Reproducing the original's `common1` grouped-store tree (and clang's outline
decisions inside it) is the remaining work; a faithful per-field body is
~400 B short and the naive full tree is ~4.7 KB over.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7502
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7502/lib.py                            # 7072 / 48.934051
```

Scratch harnesses (not committed, `/dev/shm/w7502/`): `lib.py` (exact score),
`lsize.py` (build + linked size), `reord.py` (item parser / reorder),
`win.py` (make a field window contiguous), `blk2tern2.py` (if/else ↔ ternary),
`searchec.py` / `combo_blocks.py` / `swap_sweep.py` / `rand_ec.py` / `find325.py`.
