# LevelScript::process — wave 175 (w11901): five extra expect hints

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`,
57,232 B). Base `master 5f599a137` (wave 174, raw 53,673). Worktree
`wt/w11901`, branch `wave175-w11901`.

## Result

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 53,673 | **53,615** |
| instruction-index alignment | 18,226 | 18,486 |
| size | 57,232 | **57,232** |
| whole-`.text` diff | 80,439 | **80,381** |
| `.ARM.extab` diff | 750 | 748 |
| `.rel.plt` diff | 0 | 0 |

`check` is green: byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370, `RATCHET PASS`.

## Shape

Five `__builtin_expect` hints on the group-8 escort/state-1/camera
branches:

```cpp
if (__builtin_expect((player->isDockedToDockingPoint() ||
    m_nScriptTime >= 60001), 1)) { ... }
if (__builtin_expect(((*enemies)[i]->shipGroup == 8), 0)) { ... }
if (__builtin_expect((m_nState == 1), 1)) { ... }
if (__builtin_expect((m_nScriptTimerA >= 6001), 0)) { ... }
} else if (__builtin_expect((m_nScriptTimerA < 7000), 0)) { ... }
```

The hints are semantics-preserving and use the same-line `// lint: arm_builtin`
waiver. The aligned count rises by 260 because the hints change the block
layout; raw is the primary metric.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11901
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
cmake --build cmake-build-match --target check -j2
```
