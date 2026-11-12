# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191 / w13521

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13521`, base master `5c99979c4`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

```text
                                     size            raw    aligned  insn
base 5c99979c4                       17172/17172     2669    865     6206/6197
landed                               17172/17172     2661    863     6206/6197
```

## 1. Result — landed

| metric | before | after |
|---|---:|---:|
| size | 17,172 | 17,172 (exact) |
| raw | 2,669 | **2,661** (-8) |
| aligned | 865 | **863** (-2) |
| insn | 6206/6197 | 6206/6197 |
| `.text` diff | 71,416 | **71,408** (-8) |
| `.ARM.extab` diff | 577 | 577 (unchanged) |
| `.rodata` diff | 485 | 485 |
| build-id diff | 20 | 20 |

`.rel.plt` byte-identical (not listed by `secdiff.py`), 27/27 headers,
`.ARM.exidx` 0, file size 2,187,328 exact. `check`: **RATCHET PASS**
(byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0).

## 2. The change: case-2 escort loop `offset` update in the for-increment

The original's first (escort) loop latch, `0xb187a..0xb1888`, emits:

```
0xb187a  add.w r9, r9, #2000     ; offset += 2000
0xb187e  str.w r9, [sp, #120]    ; spill offset
0xb1882  add.w r8, r8, #1        ; i++
0xb1886  cmp   r8, fp
0xb1888  blt.n 0xb1792
```

The source had `offset += 2000;` as the last statement of the loop body,
which clang scheduled as `add r9; add r8; str r9` — the loop increment was
hoisted above the spill.  Moving the update into the `for` increment
expression

```cpp
for (int i = 0; i < convoyEnd; i++, offset += 2000) {
    ...
    // (offset += 2000 removed from the body)
}
```

puts both updates in the latch block in source order and reproduces the
binary's `add r9; str r9; add r8` sequence, saving the 8 differing bytes
(4 at `0xb187e`, 4 at `0xb1882`).

## 3. Measurements (all raw/aligned; size 17,172 unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 2,669 | 865 | 17,172 |
| **`i++, offset += 2000` in for-increment (landed)** | **2,661** | **863** | **17,172** |
| `offset += 2000, i++` in for-increment | 2,669 | 865 | 17,172 |
| `offset = offset + 2000;` in body | 2,669 | 865 | 17,172 |

## 4. Rejected / neutral experiments this pass

Case-183 switch (`0xb1418..0xb144e`) — all 24 source case-body permutations
measured; only permutations with `case 0` first stay near base
(`0 1 D 2` = 2,669, `0 D 1 2` = 2,669, `0 1 2 D` = 2,670, `0 D 2 1` = 2,670,
`0 2 1 D` = 2,670, `0 2 D 1` = 2,670).  Any order with `case 0` not first
jumps to ~5,358.  The binary's block layout (`case0, case1, case2, default`)
is produced by MachineBlockPlacement after RA; our layout is
`case0, case2, case1, default` and is not reachable by source case ordering.
The `bic r1, r5, #0x80000000` / `cmp 2` / `cmp 1` / `cbnz` branch chain is
byte-identical in both.

Raid block (`0xb3836..0xb3946`) — the binary's order is
`setHitpoints(max*2)`, `setSpeed(3.5)`, `setRotate(3)`, then
`spawnPosition = getPosition()`, x/y/z increments, `setPosition`.  Applying
that order alone is raw 2,677 / aligned 855; with the z-increment moved
before `setPosition` it is raw 2,678 / aligned 825; z-increment alone is
raw 2,670 / aligned 835.  All are raw regressions (the residual is dominated
by `this` living in `r4` in the binary vs `r8` in ours), so none landed.

Case-2 coords zero-init forms: `int coords[6] = {0};` / `{}` /
`{0,0,0,0,0,0}` add an instruction (insn 6207) and blow raw to 9,354;
`coords[1] = coords[3] = coords[4] = coords[5] = 0;` is exactly neutral
(2,669); results-first is 2,693.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13521
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=2661 aligned=863 size=17172/17172`, `.text` diff 71,408,
`.ARM.extab` diff 577, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13521/`
(`variants.py`, `run_off.py`, `run_coords.py`, `run_raid2.py`,
`run_perm2.py`, `secdiff.py`, `sbs2.py`, `hunks.py`, `classify.py`).
