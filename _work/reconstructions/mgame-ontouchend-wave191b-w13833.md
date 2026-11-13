# MGame::OnTouchEnd (8,080 B) — w13833 (wave 191b): switch-store placement sweep

Target: `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`, 8,080 B).
Branch `wave191b-w13833`, base master `da998189d`.

**Landed win (size-exact):** move the `95`-clause `switch_to_target_setting = 1`
store from before `field_68` to after it.  raw **3,665 → 3,660** (−5 bytes),
aligned 545 unchanged, size 8,080/8,080, insn 2,865/2,865, `.text` **76,629 →
76,624**, `.ARM.extab` 623 unchanged, `.rel.plt` byte-identical.

## 0. Baseline (pristine tree, rebuilt this pass)

```
_ZN5MGame10OnTouchEndEiiPv   raw=3665  aligned=545  size=8080/8080  insn=2865/2865
```

Section deltas (`scratch/w13833/secdiff.py`, run from repo root):

| section | delta |
|---|---:|
| `.text` | 76,629 |
| `.ARM.extab` | 623 |
| `.ARM.exidx` / `.rodata` / `.data` / `.rel.plt` / `.dynsym` / `.dynstr` / `.hash` / `.got` / `.bss` | 0 |

After the landed change: `.text` 76,624, everything else unchanged.

## 1. The landed change (95-clause store placement)

Source `95` body (lines 1589–1599) is the only campaign-chain body whose
statement order still differs from the original.  Original order at
`17b6a8..17b702`:

```
switch_to_target_setting = 1;      <- first
field_64 = getHitpoints();
field_5c = getShieldHP();
field_60 = getArmorHP();
field_68 = getGammaHP();
initStreamOutPosition = 1;
departStation(galaxy->getStation(10));
active = 0;
SetCurrentApplicationModule(2);
return;
```

Pristine source had the store between `field_60` and `field_68`.  Placement
sweep of that one store inside the body (all other statements fixed):

| placement | raw | aligned | size |
|---|---:|---:|---:|
| before `field_64` (first) | 4,071 | 733 | 8,084 FATAL |
| after `field_64` | 4,072 | 733 | 8,084 FATAL |
| after `field_5c` | 4,073 | 733 | 8,084 FATAL |
| **after `field_68` (landed)** | **3,660** | **545** | **8,080** |
| after `initStreamOutPosition` | 3,661 | 545 | 8,080 |
| after `departStation` | 3,850 | 616 | 8,092 FATAL |
| after `active = 0` | 4,052 | 608 | 8,100 FATAL |
| after `SetCurrentApplicationModule` | 4,087 | 608 | 8,108 FATAL |
| after `return` | 4,003 | 594 | 8,068 FATAL |

Only "after `field_68`" and "after `initStreamOutPosition`" stay size-exact;
the former is 1 raw byte better.  Note the original's literal order ("store
first") is **+4 B fatal** in our compile, so the original source almost
certainly did *not* put the store first — the reordering is a codegen artefact
of the surrounding block layout, not a source-order lever.

Other single-store moves inside the campaign chain (all size-exact, from the
same sweep) for reference:

| body (line) | move | raw | aligned |
|---|---|---:|---:|
| 100 (1648) | to first | 3,638 | 556 |
| 100 (1648) | after `field_64` | 3,634 | 634 |
| 120 (1666) | after `initStreamOutPosition` | 3,657 | 698 |
| 134 (1688) | after `field_60` | 3,658 | 557 |
| 65 (1600) | after `field_60` | 3,663 | 556 |
| 81 (1613) | after `field_64` | 3,667 | 556 |
| 96/127 (1677) | after `field_64` | 3,666 | 556 |
| 144 (1710) | after `field_64` | 3,665 | 547 |
| 161 (1723) | after `initStreamOutPosition` | 3,666 | 547 |

Multi-move combos (A = landed 95 move, B = 65 pos3, C = 100 pos1, D = 120 pos5,
E = 134 pos3), all size-exact:

| combo | raw | aligned |
|---|---:|---:|
| A | 3,660 | **545** |
| B | 3,663 | 556 |
| C | 3,634 | 634 |
| D | 3,657 | 698 |
| E | 3,658 | 557 |
| AC | 3,629 | 634 |
| ABC | 3,630 | 645 |
| ABCD | 3,622 | 667 |
| ABCDE | 3,615 | 762 |
| CE | 3,627 | 646 |
| CD | 3,626 | 656 |
| CEA | 3,622 | 646 |

A greedy sweep of "A + every other size-exact single-store move" (106 builds)
found **no combo that keeps aligned ≤ 545**: the lowest-raw aligned-preserving
result is A alone (3,660/545).  Everything that lowers raw further raises
aligned (C-family) or breaks size.

## 2. Campaign-chain order (ours vs original)

Test-site addresses (`cmp r0,#imm`), ours (A applied) vs original:

| case | ours | original |
|---|---:|---:|
| 95 | 17b3b6 | 17b6a4 |
| 65 | 17b4a2 | 17b428 |
| 81 | 17b530 | 17b4b6 |
| 74 | 17b5dc | 17b562 |
| 96 | 17b704 | 17b71c |
| 127 | 17b710 | 17bc34 |
| 100 | 17b934 | 17b94e |
| 110 | 17b9ee | 17ba3a |
| 120 | 17baea | 17bad8 |
| 126 | 17bbd4 | 17bbbc |
| 134 | 17bc4e | 17bc4c |
| 144 | 17bd08 | 17bd10 |
| 155 | 17bd8c | 17bd94 |
| 161 | 17bdca | 17bdcc |
| 162 | 17be40 | 17be48 |

Original binary order: 65, 81, 74, 95, 96, 100, 110, 120, 126, **127**, 134,
144, 155, 161, 162.  Ours: 95, 65, 81, 74, **96+127 combined**, 100, 110, 120,
126, 134, 144, 155, 161, 162.

### 2.1 `96 || 127` is two separate source clauses in the original

Original `96` test (`17b71c`): `cmp r0,#96; bne.w 17b93c` → falls into the body
at `17b722`.  Original `127` test (`17bc34`, i.e. after `126`):
`cmp r0,#127; beq.w 17b722` — a **backward branch to the same body**.  So the
original emits the body once, in the `96` clause, and the `127` clause is a
`goto`/merged tail.

Ours combines the conditions (`(cur == 96 || cur == 127)`), so both compares are
adjacent at `17b704`/`17b710`.

Splitting the source into two separate clauses with identical bodies does make
clang 7.0.2 tail-merge them correctly (`beq.w` back to the shared body), but the
result is **not size-exact**:

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| split 96/127, 95 still first | 4,827 | 932 | 8,104 FATAL | 2,867 |
| split 96/127 + 95 after 74 (store pos 3) | 4,713 | 992 | 8,100 FATAL | 2,865 |
| split 96/127 + 95 after 74 (store pos 4 = A) | 4,726 | 992 | 8,100 FATAL | 2,865 |

(+20…+24 B; instruction count can be made to match exactly, so the excess is
encoding width / pool padding, not extra blocks.)  Not landable.

### 2.2 Moving the 95 clause to its source-correct position is worse

`95` after `74` (source-correct order) alone: raw **3,751**, aligned **799**,
size 8,080, insn 2,862/2,865.  The chain then starts at `17b3b6` for the `65`
test (original `17b428`), i.e. the whole pre-chain region is 0x72 B short — the
cargo-loop inline literal pool (see §3) accounts for 0x2A of it, the rest is
block-layout drift.  The current "95 hoisted first" arrangement is a local
optimum for the raw/aligned metric.

## 3. Literal-pool placement: cargo loop

Original has an **inline literal pool** at `17b3b6..17b3e0` inside the
freelance-failure cargo loop (`b.n 17b40e` at `17b3b4`, loop body resumes at
`17b3e0`).  Ours has no pool there (`b.n 17b488` at `17b458`, loop body at
`17b45a`); our pool word(s) are shared/hoisted elsewhere.  This is the 0x2A-byte
difference between the type-check block start and the `65` test
(orig `17b374..17b428` = 0xB4, ours `17b418..17b4a2` = 0x8A).  Same story for
the 40-byte inline pool at our `17b37a` (between the freelance block's
`b.n 17b004` and the `95` test) which the original does not have — this is the
`64 insert` cluster in the aligned diff.

These pool placements are not directly controllable from C++ source; they fall
out of block layout.

## 4. Cinematic dispatch (case 154/157/158, 0, 1)

Original test order: `17b64c cmp r0,#1 / beq 17b7f4`, then `17b652 cmp r0,#0`,
body at `17b658` → 154-group, then 1, then 0.  Ours: 154-group, 0, 1.
Reordering the source to `154,1,0` compiles **byte-identically** to pristine
(no lever).  `0,1,154` → raw 3,548 / aligned 697 / 8,080 (aligned worse).
`if`/`else-if` chain spellings (`g,1,0`, `g,0,1`) are byte-identical to
pristine.  Original case-0 body tail `17b692 b.n 17b810` (shared tail); ours
`b.n 17b98e`.

## 5. Freelance reward block order

Original `17b0b6`: `setFreelanceMission(Mission::empty)` →
`showMissionRewardMessage(reward+bonus, false)` → `changeCredits(...)`.
Reordering the source to match gives raw 3,667 (worse by 2 vs pristine), so it
was not landed.

## 6. Diff shape (with the landed change)

`tools/fn_diff.py`: raw 3,660 / aligned 545 / size 8,080 / insn 2,865.
196 aligned clusters, 150 of size 1, 20 clusters ≥ 5.  Largest clusters:

| n | kind | original | ours |
|---:|---|---|---|
| 64 | insert | `17b374` | `17b37a` (inline pool + hoisted 95 block) |
| 46 | replace | `17b6ec` | `17b75a` (95 body) |
| 31 | insert | `17bac6` | `17ba7a` |
| 25 | replace | `17b9d8` | `17b9c0` |
| 22 | replace | `17b3b4` | `17b458` (cargo loop pool) |
| 21 | replace | `17b1e2` | `17b21e` |
| 16 | replace | `17b0c2` | `17b0c6` |
| 14 | delete | `17b910` | `17b922` |

Entry prologue `17a144..17a16e` is byte-identical.

## 7. Scratch

`/home/debian/GalaxyOnFire2/scratch/w13833/`:
`MGame.cpp.base` (pristine), `probe95.py` (95 store placement), `sweepsw.py`
(+`sweepsw.out`, exhaustive single-store placement), `combo3.py` (multi-move),
`combo127.py` (96/127 split + 95 move), `mv95.py`, `greedyA.py`,
`secdiff.py`, `aldiff.py`, `firstmm.py`.

## Harvest measurement (2026-11-13, on master c559cbf96)

On the current tree the 95-clause switch store-after-`field_68` candidate
measures raw **3,548 -> 3,731** (+183), aligned **697 -> 680** (-17), but
size **8,084/8,080** — size-fatal.  Kept as a note.
