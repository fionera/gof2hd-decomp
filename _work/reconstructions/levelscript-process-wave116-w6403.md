# LevelScript::process — wave 116 / w6403 (mission-40 state-6 epilogue statement order)

Status: **code change landed, 2026-11-05**. Branch `wave116-w6403`, base master
`d672d349f` (wave-115 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.053282         58.081687      (+0.028405)
orig / ours insns        20169 / 20032     20169 / 20033
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4501              4501
linked_exact             4501              4501
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.ARM.extab               0x26294           0x26294   (unchanged)
```

Raw number = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6403/fast.py`). All 27 section headers, the exact file size and
`.ARM.extab` are unchanged.

## What was applied (one size-neutral statement move)

`src/game/world/LevelScript.cpp`, the mission-40 `m_nState == 6` teardown epilogue
(source ~line 3500):

```c++
// before (master)                 // after (landed)
m_bCinematicBreak = 0;            m_bCinematicBreak = 0;
m_nScriptTimerA = 0;              m_nState++;
m_nState++;                       m_nScriptTimerA = 0;
```

## Ground truth: the original's order

The block is at original `0x8bae..0x8bce` (mission-40 case, `m_nState == 6`,
`m_nScriptTimerA >= 4001` arm).  The preceding calls are `resetCamera` (`0x8bae`)
and `LODManager::forceUpdate` (`0x8bba`); the epilogue then is:

```
0x8bbe  ldr      r0, [r4, #28]      ; r4 = this
0x8bc0  strb.w   sl, [r4, #17]      ; m_bCinematicBreak = 0
0x8bc4  adds     r0, #1
0x8bc6  str      r0, [r4, #28]      ; m_nState++
0x8bc8  ldr      r0, [sp, #80]
0x8bca  strd     sl, sl, [r0]       ; m_nScriptTimerA = 0
0x8bce  b.w      0xd11e
```

i.e. `m_bCinematicBreak = 0; m_nState++; m_nScriptTimerA = 0;` — exactly the landed
order.  Master had `m_nScriptTimerA = 0` before `m_nState++`, which is a faithful
semantic but a different statement order (and a different register allocation for the
three stores).

## Measurements (all 57232 B, size-exact unless noted)

### All six permutations of the three epilogue statements

| order | % |
|---|---:|
| `bCinematic, timerA, state` (master) | 58.053282 |
| **`bCinematic, state, timerA` (landed, faithful)** | **58.081687** |
| `timerA, bCinematic, state` | 58.043332 |
| `timerA, state, bCinematic` | 58.043332 |
| `state, bCinematic, timerA` | 58.081687 |
| `state, timerA, bCinematic` | 58.076713 |

Two permutations tie at 58.081687; the faithful one (`bCinematic, state, timerA`) was
landed.  The win is register allocation / store scheduling in the surrounding block,
not the statement semantics (the three writes are independent).

### How it was found

A random sample of 110 non-adjacent 3-statement rotations (`a,b,c -> c,a,b`) over the
whole function body (lines 606-5290, candidates restricted to three consecutive
same-indent `;` statements).  Only this site beat the baseline; the next best rotation
scored 58.009950.  A separate random sample of 120 adjacent-statement swaps produced
nothing above baseline (best neutral 58.053282), consistent with the wave-115
adjacent-statement sweeps.

## Other findings this pass (not landed, but recorded)

### The pre-switch mission-specials chain is a *separate* `if`, not an `else if`

The original's shared "active" tail (`m_bStartSequence != 0`) has every one of its
early exits branching to `0x412` (`orig 0x26a`, `0x2c8`, `0x2d6`, `0x2e4`, `0x376`),
and the tail body's fallthrough also reaches `0x412`.  `0x412` is the first check of
the pre-switch mission-specials chain (`cmp r0, #5`), so the chain runs for all
missions, i.e. the source is

```c++
if (m_bStartSequence != 0) { ... }
if (mission == 5) { ... } else if (mission == 26 && inAlienOrbit()) { ... }
else if (mission == 50 && messages) { ... } else if (mission == 51 && messages) { ... }
```

with the arms ordered **5, 26, 50, 51** (original checks at `0x412`, `0x5c4`, `0x6cc`,
`0xa54`).  Our source currently has `else if` attached to the active tail with arms
ordered 26, 5, 50, 51.  Measured:

| variant | % | size |
|---|---:|---:|
| master baseline | 58.053282 | 57232 |
| swap 26/5 arms, keep `else if` | 57.968707 | 57232 |
| separate `if`, 26 first | 56.352051 | 57476 |
| separate `if`, 5 first | 56.372098 | 57492 |
| separate `if`, 5 first + m78 epilogue | 51.470076 | 57200 |
| separate `if`, 26 first + m78 epilogue | 50.928184 | 57460 |

The separate-`if` form is the faithful CFG but is +244..+260 B, so it cannot be landed
without removing that much code elsewhere; it also scores lower because the size change
shifts the whole function.  The mission-50 arm in the original evaluates
`mission == 50` *before* the `messages` null test (`0x6d2` then `0x6d8`), while the
mission-51 arm tests `messages` first (`0x5a` then `0x64`); swapping the `&&` operand
order in the source is a no-op for clang (measured neutral, 58.053282).

### Missing mission-78 camera drift statement

The original has one more `0.8f` literal-pool use than our build (3 vs 2; the other
two are the mission-0 state-6 translate at `orig 0x6b74` / ours `0x296e` and the
planet-scale multiply at `orig 0xde06` / ours `0xde00`).  The missing one is at
`orig 0x4b74..0x4ba0`, the mission-78 (`0x4e`) switch epilogue:

```
0x4b74  ldr.w r0, [sl, #28]        ; m_nState
0x4b78  subs  r0, #5
0x4b7a  cmp   r0, #3
0x4b7c  bhi.n 0x4ba2
0x4b7e  ...                        ; (float)(-delta) * 0.8f
0x4b9e  blx   TargetFollowCamera::translate(fff)
0x4ba2  ldrb.w r5, [sl, #17]       ; return m_bCinematicBreak != 0
```

i.e. `if (m_nState >= 5 && m_nState <= 8) m_pCamera->translate((float)-delta * 0.8f, 0, 0);`
after the mission-78 `switch`.  Our mission-78 block has no such statement.  Adding it
after the switch (or before it) changes the switch's `default` target from `0x264`
(the shared tail) to the new epilogue and re-places the whole mission-78 block:

| placement | % | size |
|---|---:|---:|
| none (master) | 58.053282 | 57232 |
| after the switch | 51.470076 | 57200 |
| before the switch | 57.913714 | 57280 |
| in the mission-0 epilogue (before `m_nState == 16`) | 56.542498 | 57272 |

Not landed: the size and the block placement both regress.  This is a real content
gap that a future pass should revisit together with the mission-78 block structure.

### Mission-78 `setVisible` call order

The original's `m_nScriptTimerA >= 2151` arm calls
`enemies[0]->setVisible(false); enemies[1]->setVisible(false); landmarks[0]->setVisible(false);`
(`orig 0x4b32/0x4b3c/0x4b4a`).  Our source has `enemies[1]` before `enemies[0]`.
Swapping to the faithful order measures 58.048307 (slightly *worse* than master), so it
was not landed.

## Proven vs hypothesis

Proven (measured, deterministic):

* The original's mission-40 state-6 epilogue order is
  `m_bCinematicBreak = 0; m_nState++; m_nScriptTimerA = 0;` (disasm `0x8bbe..0x8bca`).
* Landing that order is size-neutral (57232 B) and raises the raw ratio by +0.028405;
  the gate is PASS with byte 4501 / linked 4501 / lint CLEAN / drift 0.

Hypothesis / not established:

* The remaining ~42 % sequence loss is still the global register allocation: the
  original spills `this` to `sp+88` (328 refs) and reuses `sl`/`fp` as scratch
  (`[sl,#…]` 335 refs, `[fp,#…]` 133 refs), while our build keeps `this` in `sl`
  (`[sl,#…]` 739 refs, `[fp,#…]` 45 refs) and uses `r9` for `Globals::status`
  (75 refs vs 25).  The per-window difflib map is 0.05-0.86 matched with a mean
  displacement of -20..-830 instructions across the body; no source-level lever
  found in this pass moves it.
* The `else if` -> separate-`if` change for the pre-switch chain is the faithful CFG
  but needs a compensating -244 B elsewhere before it can be landed.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
```
