# `Layout::Layout` C1/C2 — wave 158 / worker w10407
## Two pure statement moves on the w10307 base: `field_0x110_strip58`
## moved up two positions and `rewardBoxY` moved after the `rewardBoxY2`
## chain.  Raw **5,753 -> 5,716**, aligned **1,984 -> 1,979**, size
## **7,072 B** (exact).  Gate green.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-157 HEAD `9e4795206` (w10307 landed): raw same-address
diff **5,753**, aligned instruction diff **1,984**, size 7,072.
Exit: raw **5,716**, aligned **1,979**, size 7,072.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, `RATCHET PASS`.

## 1. Landed source order (net, two moves)

Only two pure statement relocations; no expression, constant or type was
edited:

1. `field_0x110_strip58` moved from between `field_0x11c` and
   `field_0x118` up to between `field_0x12c` and `field_0x2b8`.
2. `rewardBoxY` moved from before the `rewardBoxY2` if/else chain to
   after it (the chain still reads `this->rewardBoxY` in its else arm).

The resulting `0x108..0x12c` region is:

```
field_0x108
field_0x12c
field_0x110_strip58
field_0x2b8
field_0x11c
field_0x118
...
```

and the reward-box tail is:

```
footerButtonOffset
if (...) rewardBoxY2 = ...
rewardBoxY
field_0x1f8
...
```

The `field_0x15c` / `field_0x158` order is unchanged from w10307 (the
intermediate 101/102 swap was reverted by the final composition step).

## 2. Search (fresh exhaustive adjacent-statement sweeps)

The w10307 base is new (composed SLP chunk fix + four moves), so all
previous adjacent-swap sweeps were re-run from scratch.  Statements were
split at top-level `;` / `}` boundaries (206 statements, 204 adjacent
pairs); each variant was compiled with the exact NDK r18b clang 7.0.2
flags, gold-linked, stripped, and measured with both `fdiff` and
`align_diff`.

Round-by-round best strict improvements (size 7,072 throughout):

| base | move | raw | aligned |
|---|---|---:|---:|
| w10307 HEAD | — | 5,753 | 1,984 |
| w10307 | swap `field_0x158`/`field_0x15c` (101/102) | 5,752 | 1,984 |
| +101/102 | swap `rewardBoxY`/`rewardBoxY2` chain (184/185) | 5,729 | 1,991 |
| +101/102+184/185 | swap `field_0x11c`/`field_0x110_strip58` (85/86) | 5,717 | 1,982 |
| +101/102+184/185+85/86 | swap `field_0x2b8`/`field_0x110_strip58` (84/85) **and revert 101/102** | **5,716** | **1,979** |

The last composition is the landed shape.  The 101/102 swap is a
one-byte raw win alone but costs one byte in the final combination, so
it is not present.

Targeted move sweeps (remove statement `i`, re-insert at `j`) over the
`0x150..0x1d0` run (statements 98..133, window +/-4), the
`0x2c4..0x2d8` group (30..36, full) and the `0x7c..0x94` group (39..46,
full) found no further strict dominator.  The best raw-only variants
were `field_0x2f4`/`field_0x2f8` (5,708 / 2,008) and
`field_0x19c`/`field_0x18c` (5,709 / 1,983); the best aligned-only
variants were 1,978 (`field_0x1c8`/`field_0x1d8` move, raw 5,738, and
`field_0x80_touchMargin`/`field_0x7c` swap, raw 5,770).

## 3. Store / pool tables (final build)

Instruction counts and `vst1.32` offsets (function-relative, emission
order), measured with `objdump`:

| build | size | `vst1.32` | `vld1.64` | `strd` |
|---|---:|---:|---:|---:|
| ORIGINAL | 7,072 | **81** | **86** | **62** |
| final | 7,072 | **70** | **77** | **49** |

```
ORIG vst1.32: 7c 88 96 a4 b2 c0 ce dc ea 130 13e 14c 15a 168 176 184
              1c0 1ce 1d6 1de 1e6 1ee 1f6 1fe 206 20e 216 2a2 2a8 2b0
              2b8 2c0 2c8 2d0 678 8a4 a50 a58 a60 a68 a70 a78 a80 a88
              a90 be0 bf4 c04 c0c c14 c1c c24 cfe e52 ec4 111c 1128
              1130 11a6 11ae 11b6 1256 125e 1266 126e 1276 127e 1286
              1358 1360 1368 13a0 13a8 13b0 143c 1444 144c 1454 145c
              1464 146c
OURS vst1.32: 78 84 92 a0 ae bc ca d8 e6 12a 138 146 154 162 1ae 1c0
              1ce 1d6 1de 1e6 1ee 1f6 1fe 206 23a 23e 2b2 2b8 2c0 2c8
              2d0 2d8 2e0 682 8c0 a70 a78 a80 a88 a90 a98 aa0 aa8 b94
              bba f48 f6a fba 1204 1214 1266 126e 1276 12ee 12f6 12fe
              1306 130e 1316 131e 1384 138c 1394 139c 13a4 13ac 13b4
              14b6 14be 14c6
```

The final literal pool starts at function-relative `0x18d0` (180 words);
the original's starts at `0x1854` (211 words) by the same
last-code-before-pool heuristic.  The two moves trade 40 B of code for
10 pool words; the remaining 11 `vst1.32` / 9 `vld1.64` deficit is the
same structural residue documented in the w10308 note (0xec, 0x140,
0x204 group bases; device-major `0x04..0x1e0` rewrite).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10407
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10407/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5716
python3 /dev/shm/w10407/align_diff.py _ZN6LayoutC1Ev   # 1979
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10407/`: `split.py` (statement splitter),
`sweep_adj.py` (exhaustive adjacent-swap sweep), `sweep_move.py`
(remove/re-insert move sweep), `measure.py` (combined raw + aligned
measurement), `stores.py` (store census), `Layout.base*.cpp` (base
snapshots), `sweep*_results.txt`, `move_*_results.txt`.
