# LevelScript::process — wave 133 / w7726 (full block-order decode; MBP cold-block hypothesis)

Status: **analysis only, no code change landed, 2026-11-07**. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 normalised instructions. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `144fc16b0` (wave-132 head).
Scratch helpers in `/dev/shm/w7726/` (not tracked): `analyze.py`, `blocks.py`,
`blockmap.py`, `disp.py`, `callblocks.py`, `score.py`, `textdiff.py`, `sweep*.py`.

## Result

```
                         before (master)   after (no code change)
raw ratio (this pass)    59.2              59.2
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process insns            20169 / 20047     20169 / 20047
whole .text diffbytes    85244             85244
.ARM.extab diffbytes     1416              1416
.rodata / .data          0 / 0             0 / 0
.rel.plt diffbytes       0                 0
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## New hypothesis tested: the global MachineBlockPlacement permutation

Prior waves compared only the *switch case bodies*. This pass decoded the **full
block order** of both functions (all 1986/1984 basic blocks, not just the tbh
bodies) and asked whether the original is a permutation of ours.

Method (`analyze.py` + `blocks.py`): reuse `tools/verify/verify.py`'s
`disassemble()` (pool/tbh masking, `reconcile()`), split both streams into basic
blocks at branch targets / post-terminator, then map each original block to the
ours block that receives most of its aligned instructions (`difflib` opcodes).

### Finding 1 — the block order is monotonic, not permuted

Across the 1391 blocks with any aligned instruction there are **zero order
inversions**. The original is not a reordering of our block sequence; the
addresses differ only because block *sizes* differ. So a pure "permute the
cases" source change cannot be the lever.

### Finding 2 — the switch case-body order is identical (confirmed for the
linear compare chain, not just the tbh)

Decoded the linear dispatch at `0x1387c6` and both `tbh` tables. Address order of
the case entries is identical on both sides:

```
158, [131 64 135 145 142 144 139 69 73 67 70], 29, 41, 80, 56, 125, 92,
157, 40, 102, 154, 24, 14, 21, 42, 91, 94, 105, 114, 16
```

The tbh sub-order is also identical: `131,135,145,142,144,139` and
`64,69,73,67,70`. Only the absolute addresses differ (ours is ~0x160-0x250
earlier in the 0x1f8a..0x32fc range).

### Finding 3 — the *cold state sub-blocks* are placed differently

The real MBP difference is inside the `if (m_nState == 0 && cond) ... else if`
state machines. The original keeps **state 0 hot** (fallthrough after the entry)
and throws the rest of the `else if` chain far away as cold code; ours keeps
**state 1/2 hot** and makes state 0 cold.

Concrete displacements (function offsets):

| block | original | ours | displacement |
|---|---:|---:|---:|
| case 125 state 0 body | `0x22ac` | `0x2050` | -0x25c |
| case 125 state 1 body (`getText`/`setName`) | `0x376e` | `0x20d4` | **-0x169a** |
| case 91 state 0 body | `0x2f4a` | `0x2e2c` | -0x11e |
| case 91 state 1..6 chain | `0x6ed0` | `0x4c40` | **-0x2290** |
| case 40 state-0 test (polarity) | `bne.w cold` (hot) | `beq.w cold` (cold) | — |
| case 91 state-0 test (polarity) | `bne.w cold` (hot) | `beq.w cold` (cold) | — |
| case 102 state-0 test | `bne.w cold` (hot) | `beq.w cold` (cold) | — |
| case 154 state-0 test | `bne.w cold` (hot) | `beq.w cold` (cold) | — |

This is the same 40/91/102/154 polarity family recorded in wave 130 (w7668),
but the *cause* is the global MBP frequency model, not the source condition
order: the source already spells `m_nState == 0 && cond` (state tested first),
which is what the original entry does.

## Finding 4 — a genuine semantic bug: case 125 state 0/1 must be `else if`

Original `0x139f9c..0x139ffa`:

```
139f9c  ldr r0,[sp,#88] ; ldr r1,[r0,#28]        r1 = m_nState
139fa0  cmp r1,#0
139fa2  bne.w 13b43e                             if (state != 0) -> state-1 check
...     isTriggered() + state-0 body ...
139fbc  adds r1,#1 ; str r1,[r2,#28]             m_nState++ (now 1)
139ffa  b.w 13b518                               -> STATE-2 check (skips state 1)
13b43e  cmp r1,#1 ; bne 13b518                   state-1 check reuses the OLD r1
```

After the state-0 body the original branches **straight to the state-2 test**,
and the state-1 test reuses the pre-body `m_nState` in `r1`. That is exactly the
CFG of `if (m_nState == 0 && cond) { ... } else if (m_nState == 1) { ... }`.

Our source (line 3899/3907) uses two **independent** `if`s:

```cpp
if (m_nState == 0 && isTriggered()) { ... m_nState++; ... }
if (m_nState == 1) { ... }
```

so after the state-0 body (which sets `m_nState = 1`) our code **runs state 1 in
the same frame** — a real mission-125 progression bug. The faithful shape is
`} else if (m_nState == 1) {`.

Measured cost of the faithful `else if` shape (each on pristine master):

| variant | size | delta | raw % |
|---|---:|---:|---:|
| master (independent `if`s) | 57232 | 0 | 59.2 |
| `else if (m_nState == 1)` | 57256 | **+24** | 57.5 |
| nested `if (state==0) { if(cond) ... } else if` | 57248 | +16 | 57.5 |
| `__builtin_expect(state==0,1)` + `else if` | 57248 | +16 | 57.4 |
| nested `if (state==0) { if(cond) ... }` + independent `if` | 57232 | 0 | 59.2 (identical to master) |

The `else if` is semantically correct but is not landable at 57232 B without a
-24 B faithful companion. Note that the `else if` build also *re-lays out* the
whole function: state 1 becomes hot and state 0/2 cold, the opposite of the
original, so its raw score drops even though the CFG is right.

## Finding 5 — `__builtin_expect` is the only MBP lever, but it is not size-neutral

`__builtin_expect(m_nState == 0, 1)` on the `if (m_nState == 0 && ...)` of each
case makes state 0 the hot fallthrough and reproduces the original polarity.
It is already an accepted tool in this tree (`// lint: arm_builtin anchor:`).
Measured deltas on pristine master (line numbers in `LevelScript.cpp`):

| case | line | size | delta | raw % |
|---|---:|---:|---:|---:|
| 40 | 1904 | 57300 | +68 | 50.3 |
| 42 | 2091 | 57224 | -8 | 58.2 |
| 64 | 2212 | 57240 | +8 | 58.3 |
| 80 | 2574 | 57252 | +20 | 56.2 |
| 91 | 2781 | 57228 | **-4** | 57.2 |
| 92 | 2883 | 57232 | 0 | 59.2 |
| 102 | 3240 | 57500 | +268 | 34.9 |
| 125 | 3897 | 57232 | 0 | 59.1 |
| 154 | 4453 | 57432 | +200 | 42.9 |

The deltas are **not additive** (they re-roll the global layout): e.g.
`{42,64}` = +8 (not 0), `{91,64}` = +12, `{91,42,64}` = +16, `{125,92}` = 0.
The two size-neutral single edits (`92`, `125`) both leave `.text` at 85245
(+1 vs the 85244 baseline), i.e. no win.

**Best lead:** case 91 `__builtin_expect(m_nState == 0, 1)` is a genuine
faithfulness fix (orig `bne.w cold`, ours `beq.w cold`) and makes the entry
instruction-faithful, but it is **-4 B**. It needs a +4 B faithful companion
that was not found in the time box. The case-91 state-0 body then matches the
original instruction-for-instruction apart from `r4`/`r5`/`r8` colouring.

## Open questions for the next wave

1. Find a +4 B faithful statement to pair with the case-91 `__builtin_expect`
   (the state-0 body itself is instruction-faithful, so the +4 is elsewhere).
2. The case-125 `else if` (+24 B) is a real bug fix; a -24 B companion (or a
   different shape that keeps state 0 hot *and* skips state 1) would land it.
3. The remaining `else if` chains (case 91 state 1..6, case 16 state 1..6) are
   placed hot in ours / cold in orig; a *single* global frequency fix may move
   all of them at once. `__builtin_expect` on the individual arms is too
   disruptive (+68/+268/+200) — a source-level grouping change (e.g. moving a
   whole state machine into a helper) is the untested alternative.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4506
python3 /dev/shm/w7726/score.py                           # 59.2, 57232/57232
python3 /dev/shm/w7726/textdiff.py                        # .text 85244
python3 /dev/shm/w7726/sweep.py                           # expect deltas
```
