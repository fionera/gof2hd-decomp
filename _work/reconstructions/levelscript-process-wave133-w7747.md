# LevelScript::process — wave 133 / w7747 (case-entry displacement map; cold-block hints)

Status: **analysis only, no code change landed, 2026-11-07**. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**. Owned TU: `src/game/world/LevelScript.cpp`. Base master
`ec0acea1e` (wave-133 docs sync). Scratch helpers in `/dev/shm/w7747/`
(not tracked): `an.py`, `blocks3.py`, `casemap.py`, `measure.py`,
`measure2.py`, `sweep_expect.py`, `sweep125.py`, `try.sh`.

## Result

```
                         before (master)   after (no code change)
raw ratio (measure.py)    58.9              58.9
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        54465             54465
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## New decode — per-case entry displacement map

w7726 decoded the block *order* (monotonic, no inversions). This pass decoded
the **case-entry addresses** from both linear dispatch chains
(`0x1387c6` orig / `0x1387be` ours) and computed the displacement at every
`case` entry. This is the cleanest way to see which case bodies are larger /
smaller and where the cold blocks went.

| case | orig (rel) | ours (rel) | displacement |
|---|---:|---:|---:|
| 29 | 0x1f8a | 0x1d30 | -0x25a |
| 41 | 0x203e | 0x1dfe | -0x240 |
| 80 | 0x211c | 0x1efc | -0x220 |
| 56 | 0x21fc | 0x1fe8 | -0x214 |
| 125 | 0x2284 | 0x202e | -0x256 |
| 92 | 0x22e6 | 0x243e | **+0x158** |
| 157 | 0x25f0 | 0x271c | **+0x12c** |
| 40 | 0x27c0 | 0x28ea | **+0x12a** |
| 102 | 0x2940 | 0x2952 | +0x12 |
| 154 | 0x2a56 | 0x29aa | -0xac |
| 24 | 0x2c18 | 0x2b20 | -0xf8 |
| 14 | 0x2c9a | 0x2ba4 | -0xf6 |
| 21 | 0x2e0c | 0x2ce6 | -0x126 |
| 42 | 0x2e92 | 0x2d6c | -0x126 |
| 91 | 0x2f2a | 0x2e0a | -0x120 |
| 94 | 0x2f94 | 0x2e2c | -0x168 |
| 105 | 0x318c | 0x305c | -0x130 |
| 114 | 0x32c6 | 0x315e | -0x168 |
| 16 | 0x32fc | 0x3194 | -0x168 |

Differencing consecutive entries gives the **per-case body size delta**
(ours - orig). The two large ones:

| case body | delta (ours - orig) | why |
|---|---:|---|
| case 125 | **+0x3ae (942 B)** | its state-1..8 chain is placed *inline* in ours, *far* in orig |
| case 40 | **-0x118 (280 B)** | its state-1..4 chain is placed *far* in ours, *inline* in orig |

Everything else is within ±0x50. So the layout difference is not a global
permutation; it is a **cold-block placement swap between case 40 and case 125**.

## The largest cold-block displacement — case 125

```
case 125 state-0 body   orig 0x139fba (rel 0x22a2)   ours 0x139d58 (rel 0x2040)
case 125 cold chain     orig 0x13b43e (rel 0x3726)   ours 0x139d9e (rel 0x2086)
                        displacement -0x16a0 (5792 B)
```

The two functions have the **same CFG here**: state 0 hot fallthrough
(`bne`/`cbnz` to the cold chain), state-1 test first in the cold chain, then
state 2. The only difference is *where* MBP puts the cold chain. In the
original it is parked after case 92's body (0x3726); in ours it is parked
immediately after the case-125 hot body (0x2086).

## Experiments (all reverted; each measured on pristine master)

Baseline: size 57232, `measure.py` pct 58.9, `measure2.py` diffbytes 54465,
normalized diff count (`ndiff`) 12955.

### 1. `__builtin_expect(m_nState == 0, 1)` sweep over all 9 state-0 tests

The nine `if (m_nState == 0 && ...)` sites are at source lines
1904 (case 40), 2091 (case 42), 2212 (case 64), 2572 (case 80), 2779 (case 91),
2881 (case 92), 3238 (case 102), 3895 (case 125), 4450 (case 154).

| site | size | diffbytes | pct |
|---|---:|---:|---:|
| none (baseline) | 57232 | **54465** | 58.9 |
| 1904 case 40 | 57292 | size-diff | 50.3 |
| 2091 case 42 | 57232 | 54570 | 57.7 |
| 2212 case 64 | 57240 | size-diff | 58.1 |
| 2572 case 80 | 57248 | size-diff | 55.9 |
| 2779 case 91 | 57224 | size-diff | 56.6 |
| 2881 case 92 | 57232 | **54465** | 58.9 |
| 3238 case 102 | 57480 | size-diff | 34.9 |
| 3895 case 125 | 57232 | 54466 | 58.8 |
| 4450 case 154 | 57396 | size-diff | 42.9 |

Combinations of the 40/91/102/154 family are all size-changing
(`40+91` 57288, `40+102` 57240, `40+154` 57384, `91+102` 57476,
`91+154` 57404, `102+154` 57352, all four 57388). The only size-neutral sites
(92, 125) are exactly neutral / +1 diffbyte. **No win.**

### 2. Fallthrough shape: compound `&&` vs nested `if` vs `else if`

Converting `if (m_nState == 0 && cond)` to nested
`if (m_nState == 0) { if (cond) ... }` for cases 40/91/102/154 together:
size **57160 (-72)**, ndiff **14867** (worse), pct 53.9.
Case 40 alone: 57224 (-8), ndiff 13062. Case 125 alone: size-neutral but
diffbytes 54465 (identical).

Converting case 125's independent `if (m_nState == 1)` to
`else if (m_nState == 1)` (the faithful CFG per w7726 finding 4):
size **57256 (+24)**, ndiff 13749. The compiler then makes state 1 hot and
state 0 cold — the opposite of the original.

### 3. Combined cold hint + faithful `else if` for case 125

`if (__builtin_expect(m_nState == 0, 1) && cond) { ... } else if (m_nState == 1)`:
size **57248 (+16)**, ndiff **13751** (worse than baseline 12955).
This *does* move the cold chain from rel 0x2086 to **rel 0x440e**
(0x13c126), closing most of the -0x16a0 gap toward orig 0x3726, but the global
re-roll costs more than the local fix gains. A -16 B companion was not found
in the time box (the w7723 case-154 state-2 tail fix measures -32 B on this
base, overshooting; adding its `setSpeed`/timer statements gives -36 B).

### 4. `__builtin_expect(m_nState == 2, 1)` on case 125's state-2 test

The original tests state 2 first (`cmp r0,#2; bne cold`); ours tests state 3
first (`cmp r0,#3; beq; cmp r0,#2; bne`). Forcing state 2 hot: size **57348
(+116)**, ndiff **17949** — much worse.

## Conclusion / open questions

The largest MBP displacement is case 125's state-1..8 cold chain (-0x16a0),
and the faithful `else if` + branch-weight hint moves it to -0xce8, but every
source shape that moves it also re-rolls the whole function and costs more
normalized diff than it saves. The missing piece is a **size-neutral branch
weight** for the case-125 state-0 condition: `__builtin_expect` alone is
size-neutral but does not move the cold chain; `else if` moves it but is +16 B.

Next wave: find a -16 B faithful companion for the case-125
`else if` + `__builtin_expect` shape (the w7723 case-154 tail family is the
closest candidate but measures -32 B alone here), then re-measure diffbytes.
Also worth testing: a `switch (m_nState)` for case 125's state 2..8 chain
(original uses linear compares, so this is a long shot) and the case-40
cold-chain placement (the mirror image, -0x118 body delta).

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4506
python3 /dev/shm/w7747/measure.py                         # pct 58.9, ndiff 12955
python3 /dev/shm/w7747/measure2.py                        # diffbytes 54465
python3 /dev/shm/w7747/casemap.py                         # case-entry displacement map
python3 /dev/shm/w7747/sweep_expect.py                    # __builtin_expect sweep
python3 /dev/shm/w7747/sweep125.py                        # case-125 shape sweep
```
