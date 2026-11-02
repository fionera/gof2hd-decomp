# Layout::Layout() — the missing 272 bytes (w3647)

Status: research note (2026-11-02). Worktree `wt/w3647`, branch `wave87-w3647`,
HEAD `c5514e35`. Every number below was measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the repo's NDK r18b clang 7.0.2
`-Oz` build. Scratch commands are reproducible with the two helper scripts committed
next to this note (`layout_ctor/score_variant.py`, `layout_ctor/compare_combos.py`).

## TL;DR

* The 272-byte gap is real **only for the held all-ternary schedule** (`30f1280d`):
  `Layout::C1` is 6800 B there, against the original 7072 B.
* It is **not a missing field store**. All 32 device-flag combinations store exactly
  the same 807 bytes / 202 words as the original (0 bad combinations).
* It is a **codegen width/schedule difference**. The original has 100 more 16-bit
  instructions and 14 more 32-bit instructions than the all-ternary body, and uses
  81 `vst1.32` + 62 `strd` vs 70 + 54, with 103 `str.w` vs 169. The extra instructions
  account for 256 B; the remaining 16 B is padding/alignment.
* Current HEAD is already size-exact (7072 B, 46.8 %) with a different field order /
  if-else mix; it does **not** use a pad. The held pad is a separate 272-byte local
  function, not part of `C1`; it makes linked `.text` exact but leaves `C1` at 6800 B
  and adds 8 B to `.ARM.exidx`.
* No tested variant is a strict gate improvement. The best size-exact fuzzy score
  remains current HEAD 46.8 %; the best fuzzy score (50.8 %) is the 6800 B
  all-ternary body.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Sections at HEAD:

| section | original | HEAD |
|---|---|---|
| `.text` | `0x1337c8` @ `0x69440` | `0x1337c8` @ `0x69440` |
| `.ARM.exidx` | `0x6f70` @ `0x19cc08` | `0x6f70` @ `0x19cc08` |
| `.ARM.extab` | `0x26298` @ `0x1a3b78` | `0x26290` @ `0x1a3b78` (-8) |
| `.rodata` | `0x41b67` @ `0x1c9e10` | `0x41b67` @ `0x1c9e10` |

`report.json` at HEAD: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` are
`osize=usize=7072`, `pct=46.8`, `byte=false`, `linked=false`.

## 2. The held 272-byte experiment

`30f1280d` is **not** an ancestor of HEAD:

```sh
git merge-base --is-ancestor 30f1280d HEAD; echo $?   # 1
git log --oneline --all -- src/game/ui/Layout.cpp
# 30f1280d (wave87-w3600) Layout::Layout pass: 508 all-ternary store schedule
#          + 272B ctor size anchor (46.8 -> 50.8%, 6800B ctor; ...)
```

Applying that source to the current tree and rebuilding:

```sh
git show 30f1280d:src/game/ui/Layout.cpp > src/game/ui/Layout.cpp
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target verify  -j8
```

Measured:

* `_ZN6LayoutC1Ev` / `C2`: `usize=6800`, `pct=50.8`, `osize=7072`.
* `_ZL13layoutCtorPadPVi` is a **separate local function**, size **272 B**
  (`readelf -sW`; `nm -S` shows `00000110` bytes).
* `.text` `0x1337c8` @ `0x69440` — exact.
* `.ARM.exidx` `0x6f78` @ `0x19cc08` — **+8** vs the original.
* `.ARM.extab` `0x26290` @ `0x1a3b80` — **-8** size, **+8** base vs the original.
* Gate: `byte 4496  linked 4496` (C1 was already not linked; the pad does not change
  the count).

So the pad fills the linked `.text` gap but does not make `C1` size-exact, and it
costs one `EXIDX_CANTUNWIND` entry. Restore HEAD afterwards:

```sh
git checkout -- src/game/ui/Layout.cpp
cmake --build cmake-build-match --target check -j8
```

## 3. Semantic proof: no missing store

`_work/reconstructions/layout_ctor/emu.py` executes `Layout::C1` for all 32
combinations of the five `Globals` flags and records every byte written to `this`.
The original and a candidate `.so` are compared byte-for-byte:

```sh
python3 _work/reconstructions/layout_ctor/score_variant.py \
    src/game/ui/Layout.cpp current
PYTHONPATH=tools/verify python3 \
    _work/reconstructions/layout_ctor/compare_combos.py /tmp/layout_current.so
```

```
bad combos 0 of 32 bytes 807 words 202
```

Every field value and every byte is present. The 272 bytes cannot be a missing
assignment; the difference must be in how the same stores are encoded.

## 4. Where the 272 bytes are

Instruction and store statistics for `_ZN6LayoutC1Ev` (objdump; a "16-bit" row is
one 2-byte encoding, a "32-bit" row is one 4-byte encoding):

| variant | size | insns | 16-bit | 32-bit | `vst1.32` | `strd` | `vstr` | `str.w` | `str` |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| original | 7072 | 2537 | 1568 | 969 | 81 | 62 | 27 | 103 | 18 |
| current HEAD | 7072 | 2509 | 1514 | 995 | 70 | 44 | 26 | 174 | 21 |
| all-ternary (`30f1280d`) | 6800 | 2423 | 1468 | 955 | 70 | 54 | 27 | 169 | 19 |

Width accounting:

```
original    1568*2 + 969*4 = 7012 B + 60 B padding = 7072
all-ternary 1468*2 + 955*4 = 6756 B + 44 B padding = 6800
difference   100*2 +  14*4 =  256 B + 16 B padding =  272
current     1514*2 + 995*4 = 7008 B + 64 B padding = 7072
```

The original is **less if-converted**: it has 100 more 16-bit flag/branch
instructions and 14 more 32-bit pair/vector stores, while using 66 fewer 32-bit
`str.w`. The all-ternary body compresses the flag logic into conditional moves and
single `str.w` stores. Current HEAD reaches 7072 B by a different width mix
(+46 16-bit, +40 32-bit vs the all-ternary body), which is why it is size-exact but
scores lower.

Store-order agreement with the original's default-path store order (204 stores,
`_work/reconstructions/layout_ctor/order_default.txt`):

| variant | same position | LCS | LCS % |
|---|---:|---:|---:|
| current HEAD | 61 / 204 | 123 / 204 | 60.3 % |
| all-ternary (`30f1280d`) | 66 / 204 | 141 / 204 | 69.1 % |

The better store order of the all-ternary body correlates with its higher fuzzy
score, but that order is also the one that emits only 6800 B.

## 5. Control-flow evidence

The original `C1` disassembly is a tail-merged branch tree, not a flat sequence of
per-field ternaries:

* `d103e: beq.w d126c` — top-level `if (iPadHD)`. The iPadHD block runs
  `d1042..d1268`, then `b.w d21e2` (common tail).
* `d127a: beq.w d1570` — nested `if (n9)`. The n9 block runs `d127e..d12fc`,
  then `b.n d1860` (common tail).
* `d1570..d15d8` — retina conditional-move block (`it eq` / `moveq`), falling
  through to the default store block.
* `d15da` — first default-path store (`strd r1, r4, [r0, #4]`).

The all-ternary schedule produces the default-path values with conditional moves;
it does not reproduce the iPadHD/n9 branch blocks or the original's pair-store
schedule, hence the 272-byte shortfall.

## 6. A/B table of source variants

All variants were scored with
`_work/reconstructions/layout_ctor/score_variant.py` (compile the candidate
`Layout.cpp` with the exact build flags, rename `.text` to `.text.tuend.o107`,
gold-link replacing `o107_Layout.cpp.obj`, strip, score with `tools/verify`).

| variant | C1 size | pct | note |
|---|---:|---:|---|
| current HEAD | 7072 | 46.8 | size-exact; `.ARM.extab` -8 |
| held `30f1280d` all-ternary | 6800 | 50.8 | +272 B separate pad; exidx +8 |
| current order, pure all-ternary | 7040 | 45.1 | all if/else converted to ternaries |
| current order, `field_0x2f4` -> ternary | 7048 | 47.0 | 24 B short |
| current order, `field_0x2f4` + `rewardBoxY2` -> ternary | 7032 | 47.1 | 40 B short |
| all-ternary reordered to original store order | 6780 | 21.9 | |
| current expressions reordered to original store order | 6784 | 21.1 | |
| `gen.py` chain (original order) | 6488 | 17.6 | shortest if/else chains |
| `gen.py` hdblock | 6472 | 35.6 | |
| all-if (current order) | 7040 | 19.6 | every ternary expanded |
| all-if (all-ternary order) | 6744 | 26.2 | |
| parent `2efa0c83` (32 ifs) | 7072 | 46.6 | before the `25ac7c1e` if/else pass |

No size-exact variant beats 46.8 %. The best fuzzy variants are size-short; the
best size-exact variants are the current HEAD and its parent.

## 7. Conclusion — proven vs hypothesis

**Proven**

* Current HEAD `C1` is 7072 B (size-exact) and 46.8 %; it does not use the pad.
* The held all-ternary `C1` is 6800 B and 50.8 %; its 272-byte pad is a separate
  local function, so `C1` itself remains 6800 B.
* All 32 flag combinations store identical bytes/words; no field store is missing.
* The 272 bytes are 100 extra 16-bit + 14 extra 32-bit instructions (256 B) plus
  16 B of padding/alignment; the original uses more pair/vector stores and fewer
  `str.w`.
* The original control flow is a tail-merged `iPadHD` / `n9` / retina / default
  branch tree.
* The pad makes `.text` exact but costs `.ARM.exidx` +8 and shifts `.ARM.extab`.

**Hypothesis**

* The 272 bytes are the original's less-if-converted branch/flag-test blocks and
  paired stores. Reproducing the exact branch/if-conversion mix (rather than adding
  a pad) is the remaining work.
* Current HEAD's size-exactness is a different width mix, not the original's; a
  future pass should aim for the original's 1568/969 16/32-bit split, not merely
  7072 B.

## 8. Reproduction

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8

# any candidate Layout.cpp
python3 _work/reconstructions/layout_ctor/score_variant.py \
    src/game/ui/Layout.cpp myvariant

# semantic equality for the current source
python3 _work/reconstructions/layout_ctor/score_variant.py \
    src/game/ui/Layout.cpp current
PYTHONPATH=tools/verify python3 \
    _work/reconstructions/layout_ctor/compare_combos.py /tmp/layout_current.so

# held all-ternary experiment
git show 30f1280d:src/game/ui/Layout.cpp > src/game/ui/Layout.cpp
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target verify  -j8
git checkout -- src/game/ui/Layout.cpp
cmake --build cmake-build-match --target check -j8
```
