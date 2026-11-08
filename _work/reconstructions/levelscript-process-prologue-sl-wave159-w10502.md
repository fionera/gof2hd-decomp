# LevelScript::process — wave 159 / w10502: size-exact state-6 placement
# (this=sl shapes measured; all cost +40..+56 B, so the gate-green win landed is
#  the state-6 block move, raw 55,094 -> 55,062, aligned 17,129 -> 17,055)

Target `_ZN11LevelScript7processEi`, original `0x137d18`, **57,232 B (0xdf90)**,
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w10502`,
base master `cfde1f5f2` (wave 158).  Scratch `/dev/shm/w10502/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,094            55,062            (-32)
process aligned insns    17,129            17,055            (-74)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## 1. Landed source change

Mission-106 state-6 block moved out of the state-1/2/3 `else if` chain and
placed as a **separate `if` after the state-4/5 chain** (order 1,2,3,4,5,6):

```cpp
} else if (m_nState == 3 && ...) {
    m_nState = 4;
}
if (m_nState == 4 && ...) { ... m_nState = 5; ... }
else if (m_nState == 5) { ... m_nState++; }
if (m_nState == 6 && ((RadioMessage *) ((*messages)[5]))->isOver()) {
    ... status stores / nextCampaignMission / departStation ...
}
```

This is a pure block-placement change (no semantic change).  It keeps the
function size exact at 57,232 B and the prologue unchanged (`this`=fp,
canary=sl, `&m_pLevel`=r4, delta=r6, messages=r5, enemies=r8).

Other size-exact placements measured (all `this`=fp):

| placement | size | raw | aligned |
|---|---:|---:|---:|
| HEAD (state-6 after state-3) | 57,232 | 55,094 | 17,129 |
| **state-6 separate after state-5 (landed)** | **57,232** | **55,062** | **17,055** |
| state-6 between state-4 and state-5 (`else if`) | 57,232 | 55,103 | 17,051 |
| state-6 at end of mission-106 block | 57,232 | 55,088 | 17,051 |
| state-6 as `else if` after state-5 | 57,224 | 55,214 | 17,048 |

## 2. The `this=sl` shapes (measured, not landed: +40..+56 B)

w10403's shape was reproduced: `Level *self = m_pLevel;` used for the first 14
`m_pLevel->` uses (after the first `getMessages` call), state-6 moved after the
state-4/5 chain, and `__builtin_expect(i < 3, 1)` on the three mission-0 intro
loops.  That gives size 57,232, raw **55,331**, aligned **21,566**, map
`this=sl, canary=r9, delta=r5, &m_pLevel=r4, messages=r6, enemies=fp` — i.e. it
reproduces w10403's 55,334/21,573 to within the alias range.

The `self` alias is the only lever found that flips `this` to `sl`, but it
costs +40..+56 B because it keeps the `Level*` value live in `r9` across the
`getPlayer`/`getEnemies` calls and spills `&m_pLevel`/`&Globals::status`.
Measured size-exact-ish `this=sl` shapes with the *ideal* map
(`this=sl, canary=fp, &m_pLevel=r4, delta=r6, messages=r5, enemies=r8`):

| variant | size | raw | aligned |
|---|---:|---:|---:|
| self alias n=3 + state-6 after state-5 | 57,288 | **54,876** | 20,602 |
| self alias n=3 + state-6 after state-5 (decl after getPlayer, n=2) | 57,288 | 54,868 | 20,601 |
| self alias n=5 + state-6 after state-5 | 57,272 | 55,237 | 20,575 |
| self alias n=5 + state-6 between state-4/5 (clean) | 57,272 | 55,220 | 20,574 |
| self alias n=5 + state-6 between state-4/5 (dead duplicate state-5 arm) | 57,232 | 55,358 | 20,566 |

The last row is the only size-exact `this=sl` shape found, but it relies on a
dead `else if (m_nState == 5) {}` branch (kept only for codegen), so it was not
landed.  A clean `this=sl` shape needs a -40/-56 B companion; none was found
in this wave.

## 3. Why the aligned metric moves so little

`LevelScript::process` is 20,807 instructions; the prologue is ~60.  The
aligned diff is dominated by the case-106 and case-64 structural blocks, not
by the prologue register map.  Flipping `this` fp->sl changes thousands of
`regalloc:` operand pairs but the greedy `difflib` alignment on the 57 KB
stream barely moves (baseline 17,129 vs `this=sl` 20,5xx).  The state-6 block
move gives a small but real -74 aligned.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10502/fdiff.py _ZN11LevelScript7processEi        # 55,062 / 57,232
python3 /dev/shm/w10502/align_diff.py _ZN11LevelScript7processEi  # 17,055
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

Scratch harnesses: `/dev/shm/w10502/{sweep_self,sweep2,variant,combo,combo2,place6,prologue,align_regions,chunkdiff}.py`.

## 5. Open questions

1. A clean -40/-56 B companion for the `this=sl` self-alias shape would land
   `54,876/20,602` at size 57,232 (best raw measured this wave).
2. The aligned metric is dominated by the case-106/case-64 block placement;
   the prologue/`this` register map is nearly invisible to it.  Future
   aligned work should target those blocks, not the prologue.
