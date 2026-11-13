# `Layout::Layout` C1/C2 — wave 191b / worker w13850

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13850`, base master `4d68f8b10` (raw 5,571 / aligned 2,011).

**Result: one size-exact raw/`.text` win landed (raw -1, aligned unchanged).**

| | before | after |
|---|---:|---:|
| raw | 5,571 | **5,570** |
| aligned | 2,011 | 2,011 |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,516/2,552 | 2,516/2,552 |
| `.text` byte diff | 75,773 | **75,772** |
| `.ARM.extab` byte diff | 544 | 544 |
| `.ARM.exidx` / `.rodata` / `.data` / `.bss` | 0 | 0 |
| `.rel.plt` | identical | identical |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint `CLEAN`, drift 0, parity 5370/5370.
All 27 section sizes/base addresses unchanged; `.rel.plt` byte-identical.

## The win

```
m   field_0x200  -> before the field_0x218 if/else   # 0x200 written before 0x218
```

`field_0x200` is the independent `-18/-26/-26/-13` store that currently sits
between `field_0x214` and `field_0x234`.  Moving it just before the
`field_0x218` device chain recovers one raw byte of the `0x200..0x218` block
(and one `.text` byte) while leaving the instruction count, size and aligned
diff untouched.  Four equivalent placements tie at 5,570 / 2,011 / 7,072:
`m(150,154)`, `m(150,155)`, `mb(150,153)`, `mb(150,154)`.

## Search

* `m`/`mb` sweep of units 147..156 over every position 140..160 (400 builds,
  114 size-exact): `m(150,154)` and its three ties are the only points below
  raw 5,571 at aligned 2,011.
* `m`/`mb` sweep of the `field_0x2a4`/`field_0xf0` cluster (units 73..82 over
  68..86, 360 builds, 79 size-exact): no aligned win.  `m(75,77)` etc. reach
  raw 5,570 but aligned 2,013 (worse); every aligned-2,011 point is raw 5,571.
* Distance-1..4 swap sweep over all 206 ctor units (814 builds, 231
  size-exact): no swap reaches aligned < 2,011.  Best raw is `s(4,7)` =
  5,560 at aligned 2,051 (bad trade); every aligned-2,011 point is raw 5,571.

### Negative / not landed

* **Vector-group contiguity.**  The original SLP-vectorises seven 4-field
  groups (`vst1.32 {d0-d1}`): `0xc0..0xcc`, `0xb0..0xbc`, `0xec..0xf8`,
  `0x294..0x2a0`, `0x2a4..0x2b0`, `0x108..0x114`, `0x118..0x124`, then a
  `0x128..0x134` group.  Making any single group contiguous in source order
  (`[0xc0,0xc4,0xc8,0xcc]`, `[0xb0,...]`, `[0xec,...]`, `[0x294,...]`,
  `[0x108,...]`, `[0x118,...]`) shrinks the body (6,952..7,008 B) and pushes
  aligned to 2,132..2,817.  The `0x2a4` group alone is 7,064 B / 2,298.
* **Full field-order reorder of `0x140..0x1dc`.**  Placing the 40 units in
  ascending field order (the original's `vst1.32` groups `0x140`, `0x150`,
  ..., `0x1d0`) gives 5,921 / 2,508 / **7,016** — 56 B short.
* **`0x94..0xa8` group** is still the original's `stmia.w`; see the w13825
  note for the 6,840 B shrink.
