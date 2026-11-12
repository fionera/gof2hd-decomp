# Radar::draw wave 190 (w13456 pass 3) — music-tail play id swap
# raw 6,156 -> 6,155 (-1), size-exact, `.rel.plt` identical

**Status:** landed, 2026-11-12. Worktree `wt/w13456`, branch `wave190-w13456`.
Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`, 9,908 B.
Read with `radar-draw-wave190-w13456.md` / `-pass2.md` (the 6,156 base) and
`radar-draw-wave190-w13404-music.md`.

## 0. The change

In the music-selection tail, swap the two `play` calls in different branches at the
same brace depth:

```cpp
             } else if (__builtin_expect(this->field_0x1bc < 3, 0)) {
                 ...
                 } else {
-                    Globals::sound->play(140, nullptr, nullptr, 0.0f);
+                    Globals::sound->play(151, nullptr, nullptr, 0.0f);
                 }
             } else if (__builtin_expect(this->field_0x1bc < 5, 0)) {
                 if (__builtin_expect(alwaysEnemySeen, 1)) {
-                    Globals::sound->play(151, nullptr, nullptr, 0.0f);
+                    Globals::sound->play(140, nullptr, nullptr, 0.0f);
```

The -1 is in the `12f984` branch immediate and the `130562` tail byte; function size
stays 9,908. Found by the extended-window (40-line) same-depth statement-swap sweep
over lines 1050-1353, `/home/debian/GalaxyOnFire2/scratch/w13456/sweep2.py` (log
`sweep2.log`); reproducer `swap6155.py`. This is the same cell the pass-1 note listed
as a -1 candidate on the 6,265 base; it survives on top of the two asteroid-block
wins.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,156 | **6,155 (-1)** |
| instruction-index alignment | 2,021 | 2,021 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes (whole `.so`) | 72,148 | **72,147 (-1)** |
| `.ARM.extab` differing bytes | 584 | 584 |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 72,752 | **72,751 (-1)** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 1. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6155 aligned= 2021 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
