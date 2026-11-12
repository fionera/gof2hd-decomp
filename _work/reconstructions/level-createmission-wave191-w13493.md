# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191 / w13493

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191-w13493`, base master `f325e2d87`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Re-tests the two w13487 case-2 candidates on the newer master shape
(`int coords[6]` with separate assignments, from w13469 / `e77b9ad0b`).

```text
                                     size            raw    aligned  insn
base f325e2d87                       17172/17172     2966   1125     6210/6197
landed (faithful pair 0_2)           17172/17172     2964   1124     6210/6197
```

## 1. Candidate results on the current shape

| candidate | source shape | raw | aligned |
|---|---|---|---|
| base | zeros 1,2,3,5; results 0,4 | **2966** | 1125 |
| (1) local `AERandom *rnd` | `rnd` local, results 0,4 | 2987 | 1118 |
| (2) pair `3_4` | zeros 0,1,2,5; results 3,4 | 2966 | 1126 |
| (2') pair `0_2` (faithful) | zeros 1,3,4,5; results 0,2 | **2964** | 1124 |

The w13487 `rnd` local was a −2 raw win only on the old aggregate-initializer
shape; with the separate assignments the current code already loads
`Globals::rnd` once per call site and the local costs **+21 raw** (2,966 ->
2,987), so it is rejected. The old `3_4` crutch is neutral on the new shape
(2,966, aligned 1,126).

## 2. Landed: faithful coords pair `0_2` (−2 raw)

The original disassembly (`0xb16d6..0xb1720`) stores the two `nextInt(20000)`
results to `coords[0]` (`sp+0x138`) and `coords[2]` (`sp+0x140`), with zeros
at 1,3,4,5 — the binary's aggregate is `{res1, 0, res2, 0, 0, 0}`. The
current master source put the second result in `coords[4]`, which is both
semantically wrong and 2 raw bytes worse. Change:

```cpp
int coords[6];
coords[1] = 0;
coords[3] = 0;
coords[4] = 0;
coords[5] = 0;
coords[0] = (int) (float) (-20000 - Globals::rnd->nextInt(20000));
coords[2] = (int) (float) (-20000 - Globals::rnd->nextInt(20000));
```

Raw **2,966 -> 2,964** (−2), aligned **1,125 -> 1,124** (−1), size exact
17,172 B. `.text` differing bytes **71,811 -> 71,809** (−2); `.ARM.extab`
575 unchanged; `.note.gnu.build-id` 20 unchanged; total
(text+extab+build-id) **72,406 -> 72,404** (−2). `.rel.plt` byte-identical,
27/27 headers, file size 2,187,328 exact. `RATCHET PASS`
(byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0).

## 3. Slot-pair sweep (separate assignments, zeros before calls)

`glob` = direct `Globals::rnd`; `rnd` = local pointer. All size 17,172
unless noted.

| pair | glob raw/aligned | rnd raw/aligned |
|---|---|---|
| `0_2` | **2964 / 1124** | 2987 / 1117 |
| `0_4` (base) | 2966 / 1125 | 2987 / 1118 |
| `0_3` | 2966 / 1123 | 2987 / 1123 |
| `2_3` | 2966 / 1124 | 2987 / 1124 |
| `3_4` | 2966 / 1126 | 2987 / 1119 |
| `2_4` | 2966 / 1126 | 2987 / 1119 |
| `3_5` | 2966 / 1126 | 2987 / 1119 |
| `0_5` | 6444 / 1167 (17,164) | 9376 / 1168 |

## 4. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13493
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=2964 aligned=1124 size=17172/17172`, `.text` diff 71,809,
`.ARM.extab` diff 575, `.rel.plt` diff 0, `RATCHET PASS`.
