# LevelScript::process — wave 191b / w13599: tail sweep summary

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13599`, branch
`wave191b-w13599`, base master `d6348b79b` (raw **51,525**, aligned 22,801,
`.text` 71,208, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Final state (three landed passes)

```
process raw diffbytes    51,525 -> 51,519   (-6)
orig / ours size         57,232 / 57,232    (exact)
.text diffbytes          71,208 -> 71,202   (-6)
.ARM.extab               577                (unchanged, size/base exact)
.ARM.exidx               0                  (byte-identical)
.rodata                  0                  (byte-identical)
.rel.plt                 md5 289b2a06…      (byte-identical)
byte_exact               4508/4524
linked_exact             4508/4524
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN ✓
drift                    0
symbol parity            5370/5370
RATCHET                  PASS
```

Landed changes (each committed separately with its own note):

1. `levelscript-process-tail-case102-w13599.md` — case-102 wake block:
   `enemy->awake()` hoisted to the front of the group (-3 raw).
2. `levelscript-process-tail-case105-timer-w13599.md` — case-105
   supernova-expansion block: `m_nScriptTimerA = 0` before the player
   `setDirection` (-2 raw).
3. `levelscript-process-tail-case105-timer2-w13599.md` — same block:
   `m_nScriptTimerA = 0` before the local `Vector dir = getDirection()` read
   (-1 raw).

## Sweep coverage on this base

All sweeps below used the exact NDK r18b `-Oz` compile command from
`compile_commands.json` plus a full gold link, and every reported candidate was
size-exact and `.rel.plt`-identical.

* d1..d6 statement transpositions over the tail (lines 2600-5200).  Only the
  producer/consumer cells already rejected by the w13561 note remained
  (`L3235/3237`, `L3025/3028`, `L4614/4617`, `L3460/3464`, `L4964/4965`,
  `L4855/4856`), plus the two case-105 timer cells landed here.
* Cross-control-boundary moves (simple statement ↔ adjacent `if`/`for`/`while`
  block, both directions; wider "past block + following statement" moves;
  adjacent control-block swaps).  Only the w13524-rejected
  `if (timer < 75001) break;` ↔ `if (m_nState == 2) m_nState = 3;` swap.
* Full statement permutations of 3/4/5-statement windows with strict
  disjoint-identifier and write/read-disjoint filters, per case block
  (157, 92, 154, 105, 70, 102, 80, 158).  No win.
* `__builtin_expect` whole-condition wrap/flip sweep (588 cells).  No win.
* `&&`/`||` operand swaps (60 cells) and comparison-operand flips (394 cells).
  No win.
* `else if` arm swaps in every large tail case.  No win.
* Top-of-function declaration permutations (24) and case-157 index
  initialization permutations (36).  Current order is already best.

## Open structural findings (not landable alone)

* case-154 state 4 (`~0x1431e6`): our source assigns
  `m_dirVec = player->geometry->getRightVector()` and then immediately
  overwrites it with `GetUpVector()` before the `-0.05f` term, so both adds use
  the up vector.  The original adds the right-vector term.  Fixing it is
  size-neutral but measures 51,525 (+6 raw) on this base.
* case-157 state 4 restore loop (`~0x141a70`): the original caches the enemy
  pointer in `r6` (`setVisible`/`setActive`/`setAIDisabled`/field checks all use
  it); our source re-indexes `(*enemies)[i]` every time.  Introducing
  `KIPlayer *ship = (*enemies)[i];` is semantically identical and shrinks the
  function by 32 B (size 57,200), so it needs a compensating +32 B reshape
  before it can land.  Every partial-cache subset is either size-inexact or
  raw-neutral (best size-exact subsets measure 51,519).
