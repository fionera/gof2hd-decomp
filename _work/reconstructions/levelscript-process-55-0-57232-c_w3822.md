# LevelScript::process (55.0%, 57232 B) — pass C (w3822)

Status: research note, 2026-11-03. Branch `wave89-w3822`, base `5710f6d0`.
**No source change landed**: the baseline is locally optimal. Every
size-exact source-shape variant tried either reproduces the *identical*
instruction stream or lowers the raw fuzzy ratio. Tree left clean.

## Baseline / gate

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN11LevelScript7processEi`: **55.0% fuzzy, 57232 B (size-exact)**, not
`byte`/`linked` exact. `.text 0x1337c8`, lint CLEAN, drift 0.

Raw (unrounded) similarity: **54.9966%**, LCS matches **11037** of
20169/19968 instructions. The official `verify` score rounds this to 55.0.
Any candidate must beat 54.9966 raw to be a real gain; none did.

## What was re-verified this pass

The wave-88 body-completeness result still holds: 20169 orig vs 19968 ours
instructions, bag-of-instructions intersection 16790 (83.25% of orig;
94.6% after normalising registers/immediates as in the wave-88 note).

The two wave-88 anchors were re-measured:

* Cached `Status *status` local. The original has **no** local: it keeps
  `&Globals::status` in `r4` and reloads `[r4]` into `r0` at every call.
  Removing the local does produce the reload pattern (`ldr.w r0,[r9]`),
  but the allocator still puts the *address* in `r9`, not `r4`, and the
  function grows to 57240 B. `-mllvm -regalloc=basic` does give
  `r4 = &Globals::status`, but raw similarity is 54.9844 (worse), so the
  `r4`/`r9` swap is not the dominant term.
* `player` stack slot. Baseline keeps `player @ sp+84`, `this @ sp+88`,
  matching the original. Replacing the mission-1 `m_pLevel->getPlayer()`
  re-fetch with the cached `player` removes the extra `Level::getPlayer`
  call but moves `player` to `sp+88` / `this` to `sp+84` and drops the
  score to 54.0. Declaration-order and `const`/`volatile` variants of
  `player` do not recover the original slot.

## Experiments (raw ratio; baseline 54.9966)

| variant | size | raw % | note |
|---|---:|---:|---|
| baseline | 57232 | **54.9966** | |
| drop `Status *status`, use `Globals::status->` | 57240 | 54.98 | +8 B |
| `Status **statusPtr = &Globals::status` (decl first or after) | 57240 | 54.98 | +8 B |
| `Status *&status = Globals::status` | 57240 | 54.98 | +8 B |
| `Level **levelPtr = &m_pLevel` for the 3 prologue calls | 57232 | 54.9966 | identical stream |
| drop `Status` local + drop `setStanding` PLT anchor | 57232 | 54.9894 | size-exact, same 55.0 |
| drop `setStanding` anchor only | 57224 | — | loses size-exact |
| drop `setRocketControl` anchor only | 57208 | 54.9 | |
| drop `autoTurret` anchor only | 57216 | 54.0 | |
| drop `switchSunForSupernovaIntro` anchor only | 57224 | 55.0 | breaks PLT order |
| drop all four PLT anchors | 57184 | 54.9 | breaks PLT order badly |
| `player->rotate(...)` in mission 1 | 57232 | 54.0 | slot swap |
| mission-0 state bodies reordered to orig order `6,10,7,8,9,11,12,13,15,14` | 57232 | **54.9966** | *identical* stream |
| swap mission cases 157/125 (orig body order) | 57232 | 54.9917 | bag 16793 |
| swap 131-145 sub-order to `131,135,145,142,144,139` | 57232 | 54.7176 | |
| regroup 131-145 + 64-73 ranges contiguously | 57236 | 54.5808 | |
| mission-0 branch restructured as the `else` of `if (mission != 0)` | 57320 | 50.5645 | +88 B |
| `-mllvm -regalloc=basic` (diagnostic only) | 57232 | 54.9844 | r4 prologue, same 55.0 |

## Two structural facts established

1. **Switch-case source order is provably neutral here.** Reordering the
   mission-0 `switch (m_nState)` bodies to the original's tbh target order
   produces a *byte-identical* instruction stream (raw 54.996637, LCS
   11037, same size). Clang 7.0.2 lowers the switch to a `tbh` and
   MachineBlockPlacement ignores source case order. This explains why the
   wave-88 mission-78 and 131-145 reorders were neutral.
2. **The four never-taken PLT anchors are load-bearing for `.plt`.** The
   original `.plt` order `setRocketControl` (2813) < `setStanding` (2817)
   < `setRotationAroundTarget` (2818) < `autoTurretIsEnabled` (2822) <
   `setAutoTurret` (2823) < `freeMissionLock` (2829) <
   `switchSunForSupernovaIntro` (2830) < `setDockingState` (2831) is
   reproduced *only* with the anchors. Removing any one moves the first
   natural reference to a later block and perturbs the order (e.g. no
   `switchSun` anchor puts `setDockingState` at 2830 and `switchSun` at
   2831). Any future removal must first move the corresponding natural
   call into the right block.

## Root cause (unchanged, now quantified)

The body is a permutation of the original instruction multiset; the loss
is **global greedy register allocation + MachineBlockPlacement**, not
missing logic. The first divergence is the `Globals::status` address
register (`r4` orig vs `r9` ours). The address interval is materialised at
the same instruction (after `getEnemies()`), so this is a greedy-allocator
priority tie-break (the `this+0x18` interval occupies `r4` at that point
in our build), not a source-declaration difference. The second anchor is
the `player`/`this` stack-slot colouring, which the extra mission-1
`getPlayer()` call currently holds in place.

The remaining lever is the *CFG shape* (if/else vs switch, shared-block
structure), which is what drives MachineBlockPlacement. Source-level case
reordering cannot move blocks; only changing the branch structure can.
The wave-88 note's `goto wormholeExitBody` and per-mission `else if`
chains are the places to look next.

## Recommendation for the next pass

Do **not** re-run flag matrices (settled). Do not remove a PLT anchor
without simultaneously relocating its first natural reference. The only
source changes that can move the needle are CFG rewrites that change
MachineBlockPlacement (e.g. converting a `switch` to an equivalent
if/else chain in a block whose original layout differs), and even those
must be measured raw, not rounded.
