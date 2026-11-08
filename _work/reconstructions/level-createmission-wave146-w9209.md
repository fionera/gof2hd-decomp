# w9209 (wave-146): `Level::createMission` — threshold-local lead

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9209`, based on master
`31297fadc` (wave 145). Continues `level-createmission-wave145-w9013.md`,
`…-wave143-w8913.md`, `…-wave142-w8713.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w9209/fdiff.py`).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5592
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. Threshold-constant table (original vs ours)

All threshold *values* are identical. The only literal-pool difference is the
**order** in which the convoy-offset constants are emitted. The table below is
the complete literal-pool set consumed by the convoy `switch (k)` block
(`Level.cpp:2192..2218`, `for (int k = 0; k < 4; k++)`).

| case | field | source value | bits | orig pool | ours pool |
|---|---|---|---:|---|---|
| 0 | `offset[0]` | 20000.0f  | `0x469c4000` | `0b15c0` | `0b15c4` |
| 0 | `offset[1]` | -3000.0f  | `0xc53b8000` | `0b15bc` | `0b15c0` |
| 0 | `offset[2]` | -50000.0f | `0xc7435000` | `0b15c4` | `0b15c8` |
| 1 | `offset[0]` | 10000.0f  | `0x461c4000` | `0b15cc` | `0b15d8` |
| 1 | `offset[1]` | -7000.0f  | `0xc5dac000` | `0b15c8` | `0b15d4` |
| 1 | `offset[2]` | -30000.0f | `0xc6ea6000` | `0b15d0` | `0b15dc` |
| 2 | `offset[0]` | 25000.0f  | `0x46c35000` | `0b15d4` | `0b15cc` |
| 2 | `offset[1]` | 0.0f      | `0x00000000` | (sl=0)   | (sl=0)   |
| 2 | `offset[2]` | -36000.0f | `0xc70ca000` | `0b15d8` | `0b15d0` |
| 3 | `offset[0]` | 12000.0f  | `0x463b8000` | `0b15e0` | `0b15e0` |
| 3 | `offset[1]` | 6000.0f   | `0x45bb8000` | `0b15dc` | `0b15e8` |
| 3 | `offset[2]` | -45000.0f | `0xc72fc800` | `0b15e4` | `0b15e4` |

Pool emission order:

```text
orig: -3000, 20000, -50000, -7000, 10000, -30000, 25000, -36000, 6000, 12000, -45000
ours: -3000, 20000, -50000, 25000, -36000, -7000, 10000, -30000, 12000, -45000, 6000
```

So orig groups the bodies **case0, case1, case2, default**; ours groups them
**case0, case2, case1, default**. Within the default body orig emits
`offset[1], offset[0], offset[2]`; ours emits `offset[0], offset[2], offset[1]`.

## 2. Compare/branch shape that consumes them

Both builds dispatch with the same chain (`r1 = k & 0x7fffffff`):

```text
cmp r1,#2 ; beq case2
cmp r1,#1 ; beq case1
cbnz r1, default
fallthrough case0
```

The only difference is the **physical layout of the case bodies**:

```text
orig  b1420 case0 | b142c case1 | b1438 case2 | b1444 default | b144e shared str [sp,#0x17c]
ours  b1420 case0 | b142c case2 | b1438 case1 | b1442 shared str [sp,#0x17c]
                                               b1444 b b1452 (skip default)
                                               b1446 default
```

Because orig's default body ends with `offset[2]` it **falls through** into the
shared `str [sp,#0x17c]` tail-merge block. Ours ends the default with
`offset[1]`, so the shared store is placed before the default and an extra
`b.n` (+2 B) is needed to skip it. That `b.n` is the first length-changing
divergence in the whole function (`b1444`), and it shifts every downstream
branch target and literal-pool pointer by +2. This is the dominant source of
the 5,592 raw diff bytes (the rest of the function is largely byte-identical
but shifted).

## 3. Threshold blocks that are already byte-identical

* **Raid threshold** (`Level.cpp:1572..1576`, `security == 0 ? 90 : 1 ? 65 :
  2 ? 35 : 10`, `b04b4..b04d8`): constants `90=0x5a`, `65=0x41`, `35=0x23`,
  `10=0x0a` are immediates and identical. Only the `raid` spill slot differs:
  orig `[sp,#0x54]` (`9015`/`9115`), ours `[sp,#0x58]` (`9016`/`9116`); and the
  `Globals::rnd` (`fp`) prologue spill is swapped the other way
  (orig `str.w fp,[sp,#0x58]`, ours `[sp,#0x54]`). ~16 one-byte diffs.
* **Alien-chance threshold** (`Level.cpp:1691..1693`, mission `>=101 && <=144`,
  `b07d6..b0804`): `101=0x65`, `144=0x90`, divisor `144.0f=0x43100000`
  (pool `0b0a78`), `15.0f` (`vmov.f32 s2,#46`), `5.0f` (`vmov.f32 s18,#20`).
  **Byte-identical.**
* **Alien-wave thresholds** (`Level.cpp:1846..1856`, `nextInt(100) <= 29`,
  mission `> 103`, `> 140`, `b0eb0..b0f3c`): `29=0x1d` (twice), `103=0x67`,
  `140=0x8c`. **Byte-identical** except the +2 branch-target shift from §2.
* **Station-index switch** (cases 10/100/101/108 and 102/103/104,
  `b08d0..b0960`): identical except the same `raid` spill slot swap.

## 4. Source variants tried (all size-exact unless noted)

All variants touch only the convoy `switch` default body / case order.

| variant | size | raw diff |
|---|---:|---:|
| **HEAD default order `[2,0,1]` (best, unchanged)** | **17172** | **5592** |
| default `[2,1,0]` | 17172 | 5595 |
| default `[0,2,1]` | 17172 | 5598 |
| default `[1,2,0]` | 17172 | 5599 |
| default `[0,1,2]` (natural) | 17164 | 10726 (clang builds a vector lookup table) |
| default `[1,0,2]` | 17188 | 11305 (clang hoists `offset[2]` into s16/s18/s20/s22) |
| `case 3:` instead of `default:`, `[2,0,1]` | 17172 | 5602 |
| `case 3:` natural `[0,1,2]` | 17124 | 11576 |
| if/else chain, `[2,0,1]` | 17172 | 5592 |
| if/else chain, natural `[0,1,2]` | 17224 | 11705 |
| `Vector offset` + `.x/.y/.z`, natural | 17168 | 5751 |
| `Vector offset`, `[2,0,1]` | 17176 | 11024 |
| `unsigned k` / `switch((unsigned)k)` + natural | 17164 | 10726 |
| `for (int i=10;i<14;i++) { int k=i-10; ... }` + natural | 17172 | 7447 |
| source case-label order (all 24 permutations) | 17172 | 5592/5593/5598 |
| `int raid` instead of `bool raid` | 17172 | 5819 |
| `bool raid` declared before `security` / `bool raid=false` | 17172 | 5592 |

**Conclusion of the sweep:** the original's `case0,case1,case2,default` layout
with a tail-merged `offset[2]` store is not reachable from any tested source
spelling of this `switch`. Every spelling that makes the default end with
`offset[2]` at the IR level either (a) lets `SimplifyCFG` convert the switch to
a dense vector lookup table (`17164`) or (b) lets it hoist `offset[2]` into FP
registers (`17188`). The current `[2,0,1]` order is the best compromise (5592).

## 5. Gate counts (before -> after)

No source change landed; gate re-run is identical.

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
_ZN5Level13createMissionEv raw diff 5592 -> 5592
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9209/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9209/diffmap.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w9209/{Level.base.cpp,fdiff.py,diffmap.py,align.py,litpool.py,
seqdiff.py,sweep*.py,orig.asm,ours.asm,createMission.mbp.txt,mir_all.txt}`.
