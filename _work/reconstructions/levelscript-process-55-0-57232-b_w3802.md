# LevelScript::process (55.0%, 57232 B) — pass B (w3802)

Status: research note, 2026-11-03. Branch `wave88-w3802`, base `41804126`.
No source change landed: every source-shape variant tried either kept the
score at 55.0 or regressed it, and several changed the function size (which
breaks the size-exact/`byte_exact` gate). Tree left clean.

## Baseline / gate

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN11LevelScript7processEi`: **55.0% fuzzy, 57232 B (size-exact)**, not
`byte`/`linked` exact. `.text 0x1337c8`, lint CLEAN, drift 0.

## What the function actually is

The reconstruction is **semantically complete**. Comparing the normalized
instruction streams (pools excluded):

* 19463 orig instructions vs 19266 ours.
* **Bag-of-instructions intersection 18405 / 19463 = 94.6%** (registers,
  immediates and branch targets normalized). The two bodies are essentially
  a permutation of the same instruction multiset.
* Sequence ratio 55.8%; with register names normalized 63.9%.
* All FP instruction counts match exactly (`vmul.f64` 12/12, `vcmp`/`vcvt`
  families identical), so there is no wrong-width arithmetic or missing
  Mission-case body.

The 45% sequence loss is therefore **register allocation + block layout +
scheduling**, not missing/extra logic. The function is a worst case for the
fuzzy metric because one register choice propagates to hundreds of
instructions.

## First divergence and the dominant root cause

The first structural difference is in the prologue, at the first
`Globals::status` use:

```
orig:  ldr r4,[r0]        ; r4 = &Globals::status
       ldr r0,[r4]        ; Status* (reloaded every call)
       blx  getCurrentCampaignMission
ours:  ldr.w r9,[r0]      ; r9 = &Globals::status
       ldr.w r4,[r9]      ; r4 = Status* (baseline local keeps it live)
       blx  getCurrentCampaignMission
```

The original keeps the **address** of `Globals::status` in `r4` and reloads
the `Status*` value into `r0` at every call (the decompiler's `piVar46` /
`*piVar46` pattern). Ours keeps the `Status*` value in `r4` because of the
`Status *status = Globals::status;` local, and puts the address in `r9`.
That single `r4`/`r9` swap changes every later use of the status pointer.

Removing the local makes the access pattern right (`ldr r0,[r9]` then
`blx`), but the allocator then still picks `r9` for the address and the
function grows by 8 bytes (57240), so the gate would need a compensating
16-byte shrink. Fuzzy: **54.9**.

The original also reuses `r9` as a short-lived float scratch in the mission-0
state-0 block (`mov r9,r2`), which is consistent with `r9` not being the
long-lived status-address register. This is a greedy-allocator tie-break,
not a source-level semantic difference.

## Experiments (all measured with the official verifier)

| variant | size | fuzzy | note |
|---|---|---|---|
| baseline | 57232 | **55.0** | |
| drop `Status *status` local, use `Globals::status->` | 57240 | 54.9 | +8 B, address still in r9 |
| `Status *&status = Globals::status` | 57240 | 54.9 | +8 B |
| `Status *const status` | 57232 | 55.0 | neutral |
| `Status *volatile status` | 57232 | 50.8 | worse |
| `enemies` initialized in declaration | 57232 | 55.0 | neutral |
| `Level *level = m_pLevel` for the three prologue calls | 57232 | 55.0 | neutral |
| `Level *level` for every `m_pLevel->` in process | 56960 | 44.7 | -272 B, worse |
| `player->rotate(...)` instead of `m_pLevel->getPlayer()->rotate(...)` | 57232 | 54.0 | see below |
| reorder mission-78 switch bodies to original block order | 57232 | 55.0 | neutral |

### The `player->rotate` trap

The original's mission-1 rotate uses the cached player:
`ldr r0,[sp,#84]` (player) followed by `blx PlayerEgo::rotate`. Ours calls
`Level::getPlayer()` again (one extra call; orig 23 `getPlayer` calls vs
ours 24). Replacing it with `player->rotate(...)` removes the extra call, but
the compiler then swaps the two stack slots:

```
baseline:  player -> sp+84, this -> sp+88   (matches original)
playerrot: player -> sp+88, this -> sp+84   (mismatch)
```

and the fuzzy drops to 54.0. The extra call is currently load-bearing for the
stack layout. Any future pass that removes it must also restore the
`player @ sp+84 / this @ sp+88` assignment (e.g. by finding the original
declaration order / live-range shape).

### Switch body order is not the issue

The mission-78 (`0x4e`) 12-way `tbh` at `0x138384` places its bodies in the
order **1, 12, 5, 3, 10, 2, 4, 8, 6, 7, 9, 11**, while our source lists them
1,2,3,4,5,6,7,8,9,10,12,11. Reordering the source to the original body order
is score-neutral (55.0), i.e. clang's block placement for this switch is not
driven by source case order. The same was observed for the 131..145 group
(original body order 131,135,145,142,144,139; source 131,145,135,142,144,139).

## Most important remaining root cause

**Global greedy register allocation / stack-slot assignment**, driven by
subtle source-shape differences that are not semantic. The body is already
the right instruction multiset; the first `r4`/`r9` choice and the
`player`/`this` stack-slot order are the two measured anchors. Matching the
original's allocator choices (or finding the exact original local
declaration/lifetime shape) is the only route to a large fuzzy jump. Flag
work is settled (see `worst_function_flag_sensitivity_w3751.md`); this is a
source-shape problem.
