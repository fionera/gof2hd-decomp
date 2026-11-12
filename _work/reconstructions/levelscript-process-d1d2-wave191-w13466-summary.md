# LevelScript::process — wave 191 / w13466: full d1/d2 sweep summary

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13466`, branch
`wave191-w13466`, base master `f325e2d87` (raw **51,679**, aligned 22,003,
`.text` 71,811, `.ARM.extab` content diff 575, `.rel.plt` byte-identical).

## Sweep

`stmts.py` parses the whole body (lines 605-5373) into same-indent
straight-line runs of complete single statements.  Barriers: braces, control
headers (`if`/`else`/`for`/`while`/`switch`/`case`/`default`/`do`/`try`/
`catch`), labels, `return`/`break`/`continue`/`goto`/`throw`, and the
unbraced body of a control header (which starts its own run).  For every run
the harness compiles the exact NDK r18b `-Oz` TU and gold-links a full
`libgof2hdaa.so`, then measures the raw same-address diff:

* `d=1`: swap each adjacent pair (`[B,A]`);
* `d=2`: both rotations of each triple (`[B,C,A]` and `[C,A,B]`).

**5,430 candidates**, 4 workers, 1.4 s/candidate idle (0.9 s under fleet
load).  Result: 28 size-exact cells below the 51,679 base.  The cells already
documented in the wave-189/190 notes are all read-after-write rotations and
were re-found as expected.

## Landed (8 commits, raw 51,679 -> 51,657, -22)

| source lines | move | raw |
|---|---|---:|
| 993-997 | mission-81 arrival `setPosition` after `SetAnimationState(3)` | -4 |
| 5135-5138 | mission-158 exit `SetAnimationState(1)` after timer/radar stores | -6 |
| 1069-1074 | mission-81 case-1 camera `setTarget` after transform anim | -2 |
| 1736-1742 | landmark-3 escort `setRoute` hoist | -2 |
| 4091-4094 | guard `awake()` after `setPosition` | -2 |
| 2956-2957 | intro `enemies[2]` before `enemies[0]` park | -1 |
| 3722-3725 | cinematic flag/radar stores after camera `setTarget` | -1 |
| 5119-5123 | explosion `start` after sound/rumble | -1 |
| 997-999 | mission-81 arrival state/timer before `SetAnimationState(1)` | -3 |

All are size-exact (57,232/57,232), `.rel.plt` byte-identical (29,392 B),
`.ARM.extab` size/base exact at 0x1a3b78 with content diff unchanged at 575,
`.ARM.exidx` byte-identical, `.rodata` 0 diff, `.data` identical.

## Rejected size-exact cells

Read-after-write / observable-order cells (documented in wave 189/190 and
re-found here):

* `m_tempVec` producer/consumer rotations: 889-891, 1237-1240, 4542-4544,
  4604-4606, 4855-4856, 4964-4966.
* `camPos +=` before `setPosition(camPos)` / `routePoints`: 1728-1730.
* two `Globals::rnd->nextInt` calls swapped (RNG order): 2411-2413.

`.rel.plt`-fatal:

* 829-830 `particleRenderBoolPtr->reset()` across `switchSkyboxForIntro()`
  (raw -1, but `.rel.plt` differs in 4 bytes) — same first-reference-order
  family as the wave-189 828/829 `switchPlanetForIntro` cell.
* 828-830 `d2up` variant of the same.

Local re-sweep of every landed block (218 candidates) found only the
documented unsafe 1728-1730 cell; the landed arrangements are locally
optimal.

## Final state

```
raw        51,679 -> 51,657  (-22)
aligned    22,003 -> 22,079
size       57,232 / 57,232   (exact)
.text      71,811 -> 71,789  (-22)
.ARM.extab 156,312 B, content diff 575 (unchanged)
.rel.plt   29,392 B, byte-identical
check      RATCHET PASS
```
