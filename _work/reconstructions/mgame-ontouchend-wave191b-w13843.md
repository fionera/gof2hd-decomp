# MGame::OnTouchEnd (8,080 B) — w13843 (wave 191b): cinematic skip-switch
# case order (case 0 first) — first size-exact raw win on this slice

Target: `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`, 8,080 B).
Branch `wave191b-w13843`, base master `c968df2c0`.
Slice: the **cinematic skip switch** (`cinematicSteerActive` branch, orig
`17b61c..17b810`) — deliberately different from w13818/w13819 (non-cinematic
cases + shared tail, note-only) and w13805 (cargo / instant-action, note-only).

**Landed a size-exact raw win.** `src/game/menu/MGame.cpp` changes one thing:
the `switch (Globals::status->getCurrentCampaignMission())` cases are written
`case 0`, `case 1`, then `case 154/157/158` instead of `154/157/158` first.

## 0. Baseline (pristine tree, rebuilt this pass)

```
_ZN5MGame10OnTouchEndEiiPv   raw=3665  aligned=545  size=8080/8080  insn=2865/2865
```
Section deltas vs the original (`scratch/w13843/secdiff.py`):

| section | delta |
|---|---:|
| `.text` | 76,006 |
| `.ARM.extab` | 552 |
| `.note.gnu.build-id` | 20 |
| `.ARM.exidx` / `.rodata` / `.data` / `.rel.plt` | 0 (identical) |

## 1. Result (landed)

| variant | raw | aligned | size | .text delta | .extab |
|---|---:|---:|---:|---:|---:|
| baseline | 3,665 | **545** | 8,080 | 76,006 | 552 |
| **case order `0, 1, 154/157/158`** | **3,548** | 697 | **8,080** | **75,889** | **552** |

The change is a **raw / `.text` win of 117 bytes** with `.ARM.extab` unchanged
and size still exactly 8,080.  The aligned proxy rises 545 -> 697 because the
literal-pool blocks shift (see §3); this is the same trade-off that landed
`HangarWindow::render` w13799 (raw 2,021 -> 1,456, aligned 306 -> 386).

Gate on the landed tree: `check` -> byte_exact **4508**, linked_exact **4508**,
size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint
**CLEAN**, parity **5370/5370**, RATCHET PASS.

## 2. Why the case order matters (clang 7.0.2 `-Oz`)

The original's cinematic switch shape is:

```
17b62e sub.w r1,r0,#154 ; cmp r1,#4 ; bhi 17b64c
17b636 movs r2,#1 ; lsl.w ; tst.w #25 ; beq 17b64c
17b642 skipCutscene ; b 17b810
17b64c cmp r0,#1 ; beq.w 17b7f4      <- case 1 tested FIRST, body at 17b7f4
17b652 cmp r0,#0 ; bne.w 17b810      <- case 0 is the FALLTHROUGH body
17b658 <case 0 body>
```

Baseline ours tested `cmp r0,#0` first and made case 1 the fallthrough body
(`17b6d0 <case 1 body>`, case 0 at `17b7e8`).  The source order in the file was
`154/157/158, 0, 1`.

`SelectionDAGBuilder::lowerWorkItem` (LLVM 7.0.2, `sdb.cpp` in scratch) does:

1. sort clusters by `a.Prob > b.Prob`, tie-break `a.Low < b.Low` (ascending
   value) -> for equal-probability single cases the order is `0, 1`;
2. then "rearrange the case blocks so that the last one falls through":
   scan `I` backwards from `LastCluster`; if `I->Kind == CC_Range &&
   I->MBB == NextMBB`, swap `*I` with `*LastCluster`.

`NextMBB` is the machine block physically after the switch block, i.e. the
**first case body emitted in IR block order**.  With source order
`154,0,1` the 154 body is `NextMBB`, the swap does not fire, and case 1 stays
the fallthrough.  With source order `0,1,154` (or `0,154,1`) the **case 0
body is `NextMBB`**, the swap fires (`0 <-> 1`), case 1 is tested first and
case 0 falls through — exactly the original.

Measured permutation sweep (all size 8,080):

| source case order | raw | aligned |
|---|---:|---:|
| `154,0,1` (baseline) | 3,665 | 545 |
| `0,1,154` (**landed**) | **3,548** | 697 |
| `0,154,1` | 3,548 | 697 |
| `1,0,154` | 3,665 | 545 |
| `154,1,0` | 3,665 | 545 |
| `1,154,0` | 3,665 | 545 |

So **only a case-0-first source order reproduces the original's
`cmp #1` / `cmp #0` sequence**; the earlier note that "source order does not
matter" is only true when the 154 group stays first.  `default:` placement and
dropping `default: break;` are both exact no-ops (same 3,548 / 697 / 8,080).

## 3. Why aligned rises while raw falls

The `ldr rX,[pc,#N] ; movs r1,r1` runs are **literal-pool data** (objdump
disassembles them linearly).  The case-order change moves the cold block that
owns the pool: orig keeps the 35-word pool at `17b7b0` (just before the case 1
body at `17b7f4`); the landed tree emits a 29-word pool at `17b70c` (after the
case 0 body) plus a 40-word pool at `17bb44`.  The 117-byte raw win is real
byte agreement at the same offsets; the aligned proxy counts the relocated
pool words as non-matching "instructions", so it moves the other way.

## 4. Variants that were measured and rejected

| variant | raw | aligned | size |
|---|---:|---:|---:|
| landed `0,1,154` | 3,548 | 697 | 8,080 |
| `0,1,154` + 95 clause after 65 | 3,549 | 731 | 8,080 |
| `0,1,154` + 95 clause after 81 | 3,553 | 733 | 8,080 |
| `0,1,154` + 95 clause after 74 | 4,021 | 756 | **8,076 (fatal)** |
| `0,1,154`, no `default:` | 3,548 | 697 | 8,080 |
| `0,1,154`, `default:` first | 3,548 | 697 | 8,080 |

## 5. Remaining clusters (landed tree)

`aldiff` reports 237 clusters / 697 non-matching (baseline 196 / 545).  The
biggest are the relocated pool blocks:

| n | kind | O block | U block | what |
|---:|---|---|---|---|
| 64 | replace | `O[1608:1609]@17b372` | `U[1608:1672]@17b378` | cargo-loop pool island |
| 46 | replace | `O[1926:1972]@17b6ec` | `U[2001:2003]@17b7ae` | 95-clause tail / 96+127 merge |
| 40 | replace | `O[2322:2323]@17bb38` | `U[2321:2361]@17bb40` | high-case pool block (relocated) |
| 35 | replace | `O[1997:2032]@17b7b0` | `U[2028:2031]@17b7f8` | case-1 pool block (orig site) |
| 32 | delete | `O[2339:2371]@17bb6c` | `U[2377:2377]@17bbc2` | high-case pool block (orig site) |
| 29 | replace | `O[1895:1896]@17b692` | `U[1937:1966]@17b70a` | case-0 pool block (landed site) |

The cinematic switch itself now matches instruction-for-instruction apart from
the position-dependent `beq.w`/`bne.w`/`bhi.n` immediates (the tail moved).
The next productive angle remains the 96/127 clause split (+68 B, gate-fatal
unless recovered) and the global pool/block placement, not local statement
order in this switch.
