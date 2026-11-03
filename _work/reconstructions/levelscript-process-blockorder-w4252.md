# LevelScript::process — mission-125 block order + anchor removal (w4252)

Status: code change landed, 2026-11-03. Branch `wave92-w4252`, base master
`fefc4e9f` (wave-92 head; the task brief's `b379d00d` is the wave-92 base).
Target: `_ZN11LevelScript7processEi` (`LevelScript::process`), original
0x137d18, 57232 B (0xdf90), 20169 instructions.

This pass replaces the wave-92 pass-3 `break; } if (m_nState == 3)` join with a
different source shape that reproduces the **original block order** of the
mission-125 state machine.  It keeps `.rel.plt` byte-identical (0 differing
bytes), makes the function naturally 57232 B, and lets the 99-store `volatile`
size anchor be deleted.  The target's raw fuzzy score rises 42.849334 ->
51.997113.

## Result

```
                         before            after
raw fuzzy (autojunk=0)   42.849334         51.997113   (+9.147779)
official rounded pct     42.8              52.0
orig / ours size         57232 / 57232     57232 / 57232  (size-exact)
orig / ours insns        20169 / 20079     20169 / 20014
byte_exact               4499              4499
linked_exact             4499              4499
size-exact               4524/4524         4524/4524
stub_zero_size           0                 0
missing/wrong_type/extra 0/0/0             0/0/0
data_exact               843/843           843/843
imports                  207               207
.text                    0x1337c8          0x1337c8
.rodata                  0x41b67           0x41b67
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26298           0x26298
.rel.plt diff            0 bytes           0 bytes
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

`cmake --build cmake-build-match --target check` passes; `drift` reports 0
fields across 0 classes.  No baseline was updated.

## 1. The original block order (disassembly evidence)

The mission-125 case body is split across the function.  The relevant blocks
(ELF addresses, function base 0x137d18) are:

| block | original address | original function offset |
|---|---:|---:|
| case-125 state 0 (`messages[1]->isTriggered` + `Route`) | 0x139f9c | +0x2284 |
| case-125 state 1 (`player->getRoute` / waypoint) | 0x13b43e | +0x3726 |
| case-125 state 2 (`messages[2]->isOver` + pirate spawn) | 0x13b518 | +0x3800 |
| mission-0 case 9 (`switchPlanetForIntro` / `switchSkyboxForIntro`) | 0x13d1de | +0x54c6 |
| case-125 state 3 (`moveForward` loop / `resetGunDelay`) | 0x13dcb2 | +0x5f9a |
| case-125 state >= 4 (`deleteHackingGame`) | 0x13de44 | +0x612c |

So the original order is

```
[125 state0/1/2] [mission-0 case9] [125 state3] [125 state>=4]
```

with `deleteHackingGame` (0x13ded4) *after* the five PLT symbols
(`switchPlanetForIntro` 0x13d1ec, `switchSkyboxForIntro` 0x13d1f4,
`ParticleSystemManager::reset` 0x13d212/0x13d21c, `startSmokeEmission`,
`PlayerStation::setVisible`).

The wave-92 pass-3 join produced

```
[mission-0 case9] [125 state2] [125 state3] [125 state>=4]
```

which also puts `deleteHackingGame` last (hence `.rel.plt` = 0) but tail-merges
420 B and needs the `volatile` anchor.

## 2. The source change

`src/game/world/LevelScript.cpp`, mission-125 case.  The state checks are now
three independent `if` statements instead of one `else if` chain:

```diff
-                } else if (m_nState == 2) {
+                }
+                if (m_nState == 2) {
                     ...
-                } else if (m_nState == 3) {
+                }
+                if (m_nState == 3) {
                     ...
-                } else if (m_nState >= 4 && m_nState <= 6 && ...) {
+                }
+                if (m_nState >= 4 && m_nState <= 6 && ...) {
                     ...
                 }
                 break;
```

plus deletion of the 99-store `volatile int *pad = &Globals::switch_to_target_setting;`
size anchor that wave-92 pass 3 added before the epilogue.

The state-0/state-1 pair stays an `else if` chain (`if (m_nState == 0) ... else
if (m_nState == 1) ...`); only the state-2, state-3 and state->=4 tests are
split out.

## 3. Why this reproduces the original order

The layout is a MachineBlockPlacement consequence, not a source-order one.  In
the joined (single-chain) build clang places the whole mission-125 chain *after*
the mission-0 case-9 block, and the `break` after state 2 makes the state-2 body
a tail branch, which lets clang place `deleteHackingGame` early in the object
(first relocation at 0x543c, before the five at 0x61b2..0x677e).

Splitting the state-2/3/>=4 tests into independent `if`s changes the CFG shape
enough that clang lays the blocks out as:

| block | ours (final) | original |
|---|---:|---:|
| case-125 state 0/1 check | 0x139d06 | 0x139f9c |
| case-125 state 2 check | 0x13caf0 | 0x13b518 |
| mission-0 case 9 | 0x13dbf8 | 0x13d1de |
| case-125 state 3 check | 0x13e202 | 0x13dcb2 |
| case-125 state >= 4 check | 0x13e3e4 | 0x13de44 |
| `deleteHackingGame` call | 0x13e478 | 0x13ded4 |

i.e. the same relative order `[125 state0/1/2] [mission-0 case9] [125 state3]
[125 state>=4]`.  The function is exactly 57232 B without any anchor, so every
later `.text` address is unchanged and `byte_exact` stays 4499.

The object relocation table now lists the six PLT symbols in the original
first-reference order:

```
00005ee0  StarSystem::switchPlanetForIntro
00005ee8  Level::switchSkyboxForIntro
00005f06  ParticleSystemManager::reset
00005f10  ParticleSystemManager::reset
000061e0  PlayerEgo::startSmokeEmission
000064a2  PlayerStation::setVisible(bool)
00006760  PlayerEgo::deleteHackingGame
```

`readelf -rW` on the linked `.so` gives `.rel.plt` diff = 0 bytes.

## 4. Variant sweep (official verifier, raw difflib ratio, autojunk off)

Separators are `state0|state1`, `state1|state2`, `state2|state3`,
`state3|state>=4`; `else` = `} else if (...)`, `if` = `}` + separate `if`.
All variants start from the pre-join source with the anchor removed.

| variant | size | rel.plt | raw % |
|---|---:|---:|---:|
| else else else else (single chain) | 56812 | 0 | 42.950975 |
| else else else if | 57100 | 0 | 40.554481 |
| else else if else | 57132 | 12 | 47.496637 |
| else else if if | 57148 | 12 | 47.382055 |
| else if else else | 57240 | 12 | 54.430853 |
| else if else if | 57284 | 12 | 47.569263 |
| else if if else | 57224 | 0 | 51.865124 |
| **else if if if (landed)** | **57232** | **0** | **51.997113** |
| if else else else | 57240 | 12 | 54.730285 |
| if else else if | 57360 | 12 | 47.314019 |
| if else if else | 57148 | 12 | 46.766962 |
| if else if if | 57172 | 12 | 46.732091 |
| if if else else | 57232 | 12 | 53.952909 |
| if if else if | 57260 | 12 | 47.553144 |
| if if if else | 57200 | 0 | 46.860529 |
| if if if if | 57200 | 0 | 46.847924 |

Only `else if if if` is simultaneously size-exact (57232) and `.rel.plt`-exact
(0) with a score above the landed 42.8.  The closest higher-scoring shape,
`else if else else` (54.43), keeps the original decompiled if/else structure but
leaves `deleteHackingGame` before the five (rel.plt 12) and is 8 B too large.

## 5. What remains

* The absolute block addresses still differ from the original (e.g. state 0 at
  0x139d06 vs 0x139f9c; mission-0 case 9 at 0x13dbf8 vs 0x13d1de).  The
  relative order now matches, but the gaps are occupied by different cold
  blocks.  The largest residual is the mission-0 state machine's block order,
  not mission 125.
* The state-2 body now falls through into the state-3 test (state is
  incremented to 3 in the same frame), whereas the original branches to the
  shared `m_nState++`/tail block at 0x144a0c.  This is the price of the
  independent-`if` shape that fixes `.rel.plt`; adding the `break` back rotates
  `.rel.plt` to 12 again (measured).
* `Globals::status` r4-vs-r9 prologue allocation and the mission-1
  `player->rotate` stack-slot swap from the earlier notes are untouched.

## 6. Reproduction

```
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8     # RATCHET PASS
cmake --build cmake-build-match --target drift -j8     # 0 fields
readelf -rW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/LevelScript.cpp.obj \
  | grep -E 'switchPlanetForIntro|switchSkyboxForIntro|ParticleSystemManager5reset|startSmokeEmission|PlayerStation10setVisible|deleteHackingGame'
```

Section byte-diff helper (reads both ELFs directly; `.rel.plt` must report 0):

```python
import sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32
O = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
U = 'cmake-build-match/libgof2hdaa.so'
def sec(p, n):
    e = Elf32(p); s = next(x for x in e.sections if x['name'] == n)
    return e.data[s['offset']:s['offset'] + s['size']]
for n in ('.rel.plt', '.text', '.rodata', '.ARM.exidx', '.ARM.extab'):
    a, b = sec(O, n), sec(U, n)
    print(n, len(a), len(b), sum(x != y for x, y in zip(a, b)) + abs(len(a) - len(b)))
```
