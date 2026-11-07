# MGame::OnTouchEnd — wave 126 (w7607): report sync + branch-hint sweep

Target `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`), 8080 B.
Worktree `/home/debian/GalaxyOnFire2/wt/w7607`, base `bf365a964` (wave-125 final).
Scratch: `/dev/shm/w7607` (not committed).

## Result

| | checked-in report (stale) | fresh build |
|---|---:|---:|
| fuzzy | 81.3 % | **81.9 %** |
| size | 8080 B | 8080 B |
| `.text` differing bytes | 85,636 | **85,552** |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.ARM.extab` differing bytes | 1,436 | **1,438** |
| `.rel.plt` differing bytes | 0 | **0** |
| `.rodata` differing bytes | 0 | **0** |

Full `check`: byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact 4524/4524,
data_exact 843/843, imports 207, lint **CLEAN**, drift **0**, symbol parity 5370/5370,
avg **99.94 %**, `RATCHET PASS`.

## Why this pass is a sync, not a source win

The wave-125 `bool refinedAny` change (commit `bf365a964`, note
`MGame-OnTouchEnd-wave125-w7514.md`) moved `MGame::OnTouchEnd` 78.8 -> 79.2 on its own base,
but the checked-in `report.json` and both docs were left at the pre-`bool` wave-125 snapshot
(81.3 / 85,636 / 1,436).  A clean rebuild of the committed source is **81.9 %** with
`.text` 85,552 and `.ARM.extab` 1,438, all other sections unchanged.  Running the `verify`
target re-synced `report.json`; this note and the docs are updated to the real numbers.

No source edit from this pass was kept: every candidate below either lost fuzzy score,
changed the 8080 B size (which shifts every later `.text` symbol and collapses `byte_exact`
to 4486), or broke `.ARM.exidx`/`.rel.plt` byte-identity.

## Variants measured (base = committed source, 81.9 % / 8080 B)

`pct / size / PLT / exidx / extab` = fuzzy %, body size, `.rel.plt` differing bytes,
`.ARM.exidx` differing bytes, `.ARM.extab` differing bytes.  All builds were linked with
the normal gold + strip pipeline.

| variant | pct | size | PLT | exidx | extab |
|---|---:|---:|---:|---:|---:|
| **committed source (`bool refinedAny`)** | **81.9** | **8080** | **0** | **0** | **1438** |
| `char refinedAny` (pre-wave-125-w7514) | 81.3 | 8080 | 0 | 0 | 1436 |
| `__builtin_expect(hudTouchFlags == 0, 1)` | 81.9 | 8080 | 0 | 0 | 1438 |
| `!this->hudTouchFlags` / `== 0u` / `0 == ...` | 81.9 | 8080 | 0 | 0 | 1438 |
| `weaponSlot != -1` + `wingmanCommand != -1` | 81.9 | 8080 | 0 | 0 | 1438 |
| separate `if (hudTouchFlags == 0)` (outside chain) | 79.0 | 8076 | 0 | — | — |
| separate `if (hudMenuOpen && hudTouchFlags == 0)` | 79.0 | 8080 | 0 | — | — |
| `if (hudTouchFlags == 0 && hudMenuOpen)` | 79.0 | 8084 | 0 | — | — |
| `hudTouchFlags == 0` moved before weapon/wingman | 80.5 | 8084 | 0 | — | — |
| `__builtin_expect(hudTouchFlags == 0, 0)` | 78.1 | 8076 | 0 | 24 | 1468 |
| `__builtin_expect(hudTouchFlags == 0, 0)` + no campaign temp | 78.2 | 8080 | 0 | — | — |
| `__builtin_expect(hudTouchFlags == 0, 0)` + `char refinedAny` | 77.6 | 8076 | 0 | — | — |
| `__builtin_expect(cutsceneActive, 1)` | 73.3 | 8052 | 0 | — | — |
| `__builtin_expect(cutsceneActive, 0)` | 67.0 | 8012 | 0 | — | — |
| `__builtin_expect(failed, 0)` | 71.7 | 8064 | 0 | — | — |
| `__builtin_expect(failed, 1)` | 53.4 | 7820 | 0 | — | — |
| `__builtin_expect(hudMenuOpen, 1)` | 74.4 | 8068 | 0 | — | — |
| remove `Mission *campaign` temp (separate calls) | 80.8 | 8084 | 0 | — | — |
| faithful mining `if (amount != 0)` / `String(-amount)` | 74.3 | 8064 | 0 | — | — |
| `int count = -amount; if (count != 0)` | 74.3 | 8064 | 0 | — | — |
| `if (amount != 0) { int count = -amount; ... }` | 74.3 | 8064 | 0 | — | — |
| `String(0 - amount)` | 74.3 | 8064 | 0 | — | — |
| `if (amount < 0)` | 72.0 | 8084 | 0 | — | — |
| named `String separator("t ")` | 81.0 | 8080 | 0 | — | — |
| named `String line = ...; report += line;` | 77.4 | 8092 | 0 | — | — |
| parenthesised `String("t ") + getText(...)` | 79.8 | 8072 | 0 | — | — |

## Evidence / remaining root cause

The committed source already matches the original's mining-block counter direction
(`sub.w r9,r9,#1` then `rsb r1,r9,#0`) and the `bool refinedAny` flag test
(`lsls r0,r0,#30`? no — `lsls r0,r0,#31`, bit 0).  The residual is still cold-block
address placement:

* original shared canary return block at `0x17b004` (offset `0xec0`);
* ours at `0x17b992` (offset `0x184e`).

Every `b.w 0xec0` in the original is a `b.w 0x184e` in ours, and difflib counts each as a
structural difference.  The original lays the cold `hudTouchFlags == 0` block at `0x17b01a`
immediately after the return; ours keeps it hot at `0x17aebe` (fallthrough of the
`hudTouchFlags == 0` test).  The `__builtin_expect(..., 0)` hint does move the block cold,
but it also moves the return block to `0x17af80`, which turns the early `b.w`+`nop`
(6 B) into `b.n` (2 B) and shrinks the body to 8076 B; that shifts every later `.text`
symbol and collapses `byte_exact` to 4486.  No 8080 B spelling of the hint was found.

The faithful mining `if (amount != 0)` / `String(-amount)` spelling is semantically right
but lets clang 7 canonicalise the down-counter into an up-counter
(`add.w r8,r8,#1` / `mov r1,r8` instead of `sub.w r9,r9,#1` / `rsb r1,r9,#0`), shrinking
the body 16 B.  The committed `int count = -amount; if (count > 0)` spelling keeps the
down-counter and `rsb`; its only mismatch is `cmp r9,#-1; bgt` vs the original
`cmp r9,#0; beq`, which is size-neutral.

## Files

- `report.json` — re-synced from the fresh `verify` target.
- `docs/IDENTICALITY.md`, `docs/VALIDATION.md` — wave-126 numbers.
- `src/game/menu/MGame.cpp` — unchanged (committed wave-125 source is the 81.9 % build).
