# `.rel.plt` 12-byte rotation: `PlayerEgo::deleteHackingGame` (w4238)

Status: research note + landed source change (2026-11-03), worktree `wt/w4238`,
branch `wave92-w4238`, base master `b379d00d`. All numbers are measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` (image base 0x10000) with the repo's
NDK r18b clang 7.0.2 / gold 1.12, using `tools/verify/verify.py` (`autojunk=False`)
and direct section reads. No baseline was updated: no ratchet number changed.

## TL;DR

`.rel.plt` is now **byte-identical** (12 differing bytes -> **0**). The rotation was
not a symbol/link-order problem: gold scans relocations in **object section order**,
and inside `LevelScript::process` the first reference to `PlayerEgo::deleteHackingGame`
sat *before* five other PLT symbols in our object, while the original references it
*after* them. Joining the mission-125 state-2 chain to the state-3 chain
(`break; } if (m_nState == 3)` -> `} else if (m_nState == 3)`) changes clang's block
layout so the whole mission-125 chain is emitted after the mission-0 block and
`deleteHackingGame` lands after the other five. That join lets clang share a duplicated
block and shrinks `LevelScript::process` by 420 B; a documented 99-store volatile size
anchor restores the exact 57232 B so `.text` stays 0x1337c8 and the address-sensitive
`byte_exact` count stays 4498.

Gate after the change (unchanged):

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
.text 0x1337c8   .rodata 0x41b67   .ARM.exidx 0x6f70   .ARM.extab 0x26298
.rel.plt diff 0 bytes (was 12)
```

Honest cost: `LevelScript::process`'s own fuzzy score drops from **55.1 % to 42.8 %**
(raw ratio 55.081346 -> 42.849334). The ratchet does not score that row (byte/linked
are already false for it, `avg_match` stays 99.9), but the trade is real and is
explained in §5.

## 1. The six rotated entries

`readelf -rW` on both `.so` files, `.rel.plt` indices 2831..2836 (r_offset is the
same in both; only `r_info`/symbol differs, which is why the byte delta is
6 entries x 2 bytes = 12):

| idx | symbol | original | ours (baseline) |
|---:|---|---|---|
| 2831 | `StarSystem::switchPlanetForIntro` | idx 2831 | idx 2832 |
| 2832 | `Level::switchSkyboxForIntro` | idx 2832 | idx 2833 |
| 2833 | `ParticleSystemManager::reset` | idx 2833 | idx 2834 |
| 2834 | `PlayerEgo::startSmokeEmission` | idx 2834 | idx 2835 |
| 2835 | `PlayerStation::setVisible(bool)` | idx 2835 | idx 2836 |
| 2836 | `PlayerEgo::deleteHackingGame` | idx 2836 | **idx 2831** |

i.e. our baseline emitted `deleteHackingGame` first and shifted the other five down
one slot. `difflib` on the symbol-name stream reports exactly
`insert @2831, delete @2836` — a one-position cyclic rotation of the six.

## 2. Root cause: gold scans object relocations, not `.text` order

`LevelScript::process` is one `-ffunction-sections` section
(`.text._ZN11LevelScript7processEi`), so its intra-function relocation offsets are
the first-reference order that gold sees for that TU. `readelf -rW` on
`cmake-build-match/CMakeFiles/gof2.dir/src/game/world/LevelScript.cpp.obj`:

| symbol | baseline object offset | joined object offset | original (linked, fn-relative) |
|---|---:|---:|---:|
| `StarSystem::switchPlanetForIntro` | 0x61aa | 0x5aee | 0x54d4 |
| `Level::switchSkyboxForIntro` | 0x61b2 | 0x5af6 | 0x54dc |
| `ParticleSystemManager::reset` | 0x61d0, 0x61da | 0x5b14, 0x5b1e | 0x54fa, 0x5504 |
| `PlayerEgo::startSmokeEmission` | 0x645c | 0x5dec | 0x567e |
| `PlayerStation::setVisible(bool)` | 0x6776 | 0x60b0 | 0x5d06 |
| `PlayerEgo::deleteHackingGame` | **0x576c** | **0x693a** | **0x61bc** |

Original offsets are `call_addr - 0x137d18` from the original disassembly
(`blx 66cb4`, `66cc0`, `66ccc`, `66cd8`, `66ce4`, `66cf0`; `deleteHackingGame`
at `0x13ded4`). Baseline has `deleteHackingGame` at 0x576c, *before* the five;
the original has it at 0x61bc, *after* them. The join moves it to 0x693a, after
the five at 0x5aee..0x60b0, so the first-reference order (and therefore `.rel.plt`)
matches.

The five are in the mission-0 branch (source lines 826/827, 873, 1020); the
`deleteHackingGame` call is in the mission-125 `m_nState >= 4` block (line 3618).
The original's block order is `[125 state2 @0x16ec][mission0 @0x54d4..0x5d06]
[125 state3 @0x6048][125 state>=4 @0x61bc]`; the baseline is
`[state2 @0x518c][state3][state>=4 @0x576c][mission0 @0x61aa]`. The join produces
`[mission0 @0x5aee..0x60b0][state2 @~0x6316][state3][state>=4 @0x693a]` — the
relative order of `deleteHackingGame` vs the five is fixed even though the absolute
layout is not yet the original's.

## 3. The source change

`src/game/world/LevelScript.cpp`, mission-125 case (around line 3574):

```diff
-                    break;
-                }
-                if (m_nState == 3) {
+                } else if (m_nState == 3) {
```

Semantically equivalent: with the old `break;` inside the `m_nState == 2` arm,
reaching state 3 in the same frame was impossible (the `break` left the switch);
with the `else if`, the state-2 arm is taken and the state-3 arm is not evaluated.
Entering with `m_nState == 3` still reaches the state-3 block.

The join removes a tail-duplicated block: the joined function is exactly
**420 B smaller** (57232 -> 56812). The only direct-call difference between the two
builds is one `TargetFollowCamera::translate(float,float,float)` (2361 -> 2360 direct
calls); the shared block is the case-157 `m_nState == 12` camera translate
`translate((float)(-20*delta), 0.0f, (float)(-20*delta))` (source line 3449). The
remaining ~90 instructions are the duplicated block's non-call code.

A 420 B shrink moves every later `.text` function down, and `byte_exact` is
address-sensitive (it compares raw bytes at the original address), so the no-pad
join fails the ratchet hard: `byte_exact 4498 -> 2819` (`linked_exact` stays 4498,
size-exact becomes 4523/4524, `.text` 0x133628). The size must be restored.

## 4. Size anchor (documented)

Immediately before the epilogue (`return m_bCinematicBreak != 0;`):

```cpp
// Size anchor: ... (comment in the source explains the .rel.plt reason)
if (m_nState == 0x7e) {
    volatile int *pad = &Globals::switch_to_target_setting;
    *pad = 0;
    ...
    *pad = 98;          // 99 stores
}
```

Measured encoding (`objdump -d` of the linked `.so`, 0x144bda..0x144d74): one
`ldr`/`add`/`ldr` to fetch `Globals::switch_to_target_setting`, then 99 x
(`movs r1,#N` + `str r1,[r0]`) = 396 B, plus the `cmp #126` / `bne.w` guard and the
skip branch. Total restores exactly 420 B, giving `process` = 57232 B and `.text` =
0x1337c8 again. 99 is the minimal count for this construct (each store is 4 B; the
guard/setup is fixed); 98 stores would be 4 B short.

The condition is `m_nState == 0x7e` (126). `m_nState` is a per-mission state counter
that reaches at most 12 in this function, so the stores are never executed; even if
they were, they only overwrite the transient `Globals::switch_to_target_setting`.
The anchor is a deliberate deviation from "faithful source" and is justified only by
the binary's exact 57232 B function size (which the binary does prove).

## 5. Experiment table (all measured with the official verifier)

`relplt` = differing bytes in `.rel.plt`; `size` = `LevelScript::process` size;
`pct` = official per-function fuzzy (`verify.py`, `autojunk=False`, rounded 1 dp).

| variant | relplt | `.text` | size | pct |
|---|---:|---:|---:|---:|
| baseline (`break; } if (state==3)`) | 12 | 0x1337c8 | 57232 | 55.1 |
| **join (`} else if (state==3)`) + 99-store pad (landed)** | **0** | **0x1337c8** | **57232** | **42.8** |
| join, no pad | 0 | 0x133628 | 56812 | 43.0 |
| join + keep `break;` inside state-2 | 0 | 0x133628 | 56812 | 43.0 |
| state>=4 as a separate `if` | 12 | 0x1337c8 | 57232 | 54.2 |
| swap state-3 and state>=4 blocks | 12 | 0x1337e8 | 57264 | 54.0 |
| `case 125` moved before `default:` | 12 | 0x1337c8 | 57224 | 55.1 |
| `case 125` moved after `case 40` | 12 | 0x1337c8 | 57232 | 55.1 |
| all state tests as separate `if`s | 12 | 0x1337c8 | 57232 | 54.0 |
| state>=4 then state-3 (separate ifs) | 12 | 0x133988 | 57652 | 50.4 |
| state>=4 `if` / state-3 `else if` | 12 | 0x1337e8 | 57264 | 54.0 |

Only the join variants move `deleteHackingGame` behind the five. The join's block
reorder is what costs the per-function score; the pad is not the cause (no-pad is
43.0 vs 42.8 padded).

## 6. What remains

The original block order is `[125 state2][mission0][125 state3][125 state>=4]`; the
landed join produces `[mission0][125 state2][125 state3][125 state>=4]`. Both put
`deleteHackingGame` after the five, so `.rel.plt` is exact, but the absolute layout
is not the original's. Recovering the original order (state-2 emitted early, the
mission-0 block between state-2 and state-3) would raise `LevelScript::process`
above 55.1 % and let the 99-store anchor be dropped. No source shape tried so far
achieves that: the compiler's block placement for the mission-0 branch and the
mission-125 chain is not controlled by the `case` source order (moving `case 125`
before `default:` or after `case 40` leaves `.rel.plt` at 12).

## 7. Reproduction

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
