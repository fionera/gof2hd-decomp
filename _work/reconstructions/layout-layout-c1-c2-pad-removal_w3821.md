# Layout::Layout C1/C2 — pad removal, size-exact 7072 B body (w3821)

Status: landed pass. Branch `wave89-w3821`, based on master `5710f6d0`.

## Result

| | before (w3801) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` / `C2` fuzzy | 50.8% | **47.7%** |
| C1/C2 body size | 6800 B + 272 B `layoutCtorPad` | **7072 B, no pad** |
| verify `size_match` | 4522 / 4524 | **4524 / 4524** |
| `byte_exact` | 4498 | **4498** |
| `linked_exact` | 4498 | **4498** |
| `.text` | `0x1337c8` @ `0x69440` | **`0x1337c8` @ `0x69440`** |
| `.ARM.exidx` | `0x6f78` @ `0x19cc08` (+8) | **`0x6f70` @ `0x19cc08` (exact)** |
| `.ARM.extab` | `0x2628c` @ `0x1a3b80` (-12, +8 base) | **`0x2628c` @ `0x1a3b78` (-12, base exact)** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

`RATCHET PASS` (byte/linked unchanged; `avg_match` 99.91). No baseline update needed.

## What changed

`src/game/ui/Layout.cpp` now carries the earlier if/else-mixed ctor body
(the `41804126` tree, 7072 B) and the 272-byte `layoutCtorPad` helper is
deleted. The body is size-exact, so `.text` stays at `0x1337c8` without an
anchor and `.ARM.exidx` loses the pad's extra `EXIDX_CANTUNWIND` entry
(3566 entries, CANTUNWIND 272 — both exactly the original's).

The fuzzy score drops 50.8 -> 47.7 because the pad had moved the all-ternary
body (6800 B) to the exact size; the score was never a property of the pad.
The if/else body is the best *size-exact* shape measured so far.

## Branch-tree reconstruction attempt (this pass)

The original is a tail-merged branch tree (ground truth, ELF `0xd1028`):

* `d103e beq.w d126c` — top-level `if (iPadHD)`; HD block `d1042..d1268`
  (29 stores, 123 fields, `vst1.32` pairs + `stmia`), then `b.w d21e2`.
* `d127a beq.w d1570` — nested `if (n9)`; n9 block `d127e..d12fc`
  (20 fields), then `b.n d1860`.
* `d1570..d15d8` — retina conditional-move block, falling into the
  default store block at `d15da` (28 fields), then `b.n d1860`.
* `d1860..d215c` — mid block shared by n9 and non-n9 (86 fields), then
  `b.n d21e2`.
* `d21e2..d2bd0` — common tail (68 fields).

The first nine HD-block stores are reproduced exactly by the current source
(`0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c`), then the schedule diverges.
The original HD block is 0x226 B / 29 stores; ours is 0x236 B / 27 stores.

Explicit branch-tree sources were generated and measured (all with the
same 204 correct per-field values):

| variant | size | pct |
|---|---:|---:|
| if/else body, no pad (landed) | **7072** | **47.7** |
| all-ternary body + pad (w3801) | 6800 + 272 | 50.8 |
| all-ternary, hill-climbed order | 6800 | 50.8 |
| all-ternary, iOS source order | 6848 | 34.7 |
| all-if, iOS order | 6784 | 28.4 |
| all-ternary, HD-mode store order | 6704 | 33.0 |
| all-ternary, n9-mode store order | 6652 | 28.4 |
| all-ternary, retina/default store order | 6780 | 21.9 |
| explicit `if (iPadHD) {HD} else {n9/ret + mid} tail` tree | 6552 | 27.9 |
| `if (iPadHD) {HD constants} else {ternaries}` | 6648 | 19.3 |
| all-ternary + the 13 if/else fields restored | 6896 | 42.8 |
| all-ternary + `field_0x2d4` if/else only | 6808 | 50.4 |
| all-ternary + `field_0x9c` if/else only | 6776 | 50.3 |

So the machine store order is *not* the source order (the -Oz scheduler
permutes it), and forcing the branch tree explicitly makes the body smaller,
not larger. The 272-byte gap is not a missing source construct that can be
re-added by hand; it is a codegen difference (the original uses more 16-bit
flag/branch code and more `strd`/`vst1.32` pairs, fewer `str.w`).

## Remaining root cause

The single remaining cause is the original's **store schedule / register
allocation inside the tail-merged blocks**, not missing code. The values are
bit-exact for all 32 flag combinations (`compare_combos.py`: 0 bad combos,
807 bytes / 202 words), and the literal-pool content is right; only the
instruction selection (pairing into `vst1.32`/`strd` vs scalar `str.w`) and
the block-local ordering differ. Recovering the original source order is
under-determined from the binary alone: the -Oz scheduler reorders stores
within each basic block, and the iOS 1.1.4 source order (the only witness)
scores worse on the fuzzy metric than the hill-climbed order.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 _work/reconstructions/layout_ctor/score_variant.py src/game/ui/Layout.cpp cur
# -> _ZN6LayoutC1Ev / C2: osize 7072 usize 7072 pct 47.7
```
