# `Layout::Layout` C1/C2 — wave 133 / worker w7752
## Full SLP store-group census + one-group-at-a-time sweep: 5-byte `.text` win

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-133 master `ec0acea1e` (7072 B, `45.028890217175 %` unrounded,
target-function differing bytes **5,789**, `.text` differing bytes **85,143**).

## Result — landed: 5 group-contiguity reorders (7072 B, diff bytes 5,789 → 5,784)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| unrounded `difflib` ratio | 45.028890217175 % | **44.958150657633 %** |
| target-function differing bytes | 5,789 | **5,784** (−5) |
| `.text` differing bytes | 85,143 | **85,138** (−5) |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
The fuzzy ratio dips 0.07 points (the reorder changes instruction scheduling), but the actual
differing-byte count and `.text` both fall by 5. Section sizes unchanged.

## 1. Full SLP store-group census (base-register-tracked, `/dev/shm/w7752/stores.py`)

Original `vst1.32` destination offsets, in instruction order (all arms of the ctor):

```
HD arm:    0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c
           0xc0 0xb0 0xec 0x294 0x2a4 0x108 0x118 0x128
           0x140 0x150 0x160 0x170 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0
non-HD:    0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x5c 0x7c
           0xb0 0xc0 0xec 0x294 0x2a4 0x118 0x108 0x128
           0x140 0xb0 0xc0 0xec 0x2a0 0x108 0x13c 0x16c 0x180
           0x1c0 0x1b0 0x1d0 0x1c0 0x1b0 0x1d0
           0x204 0x214 0x224 0x23c 0x24c 0x25c 0x26c 0x23c 0x264
           0x204 0x214 0x224 0x23c 0x24c 0x25c 0x26c
```

HEAD (wave-133 master):

```
HD arm:    0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c
           0xb4 0xc4 0x294 0x2a4 0xdc 0x118 0x108 0x128
           0x148 0x158 0x168 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0
non-HD:    0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x5c 0x7c
           0xb0 0xc0 0x294 0x2a4 0xdc 0x118 0x108 0x128
           0xb0 0x294 0x16c 0x16c 0x16c 0x1b0 0x1c0 0x1b0 0x1d0 0x1c0
           0x208 0x218 0x228 0x240 0x250 0x260 0x270
           0x208 0x218 0x228 0x240 0x250 0x260 0x270 0x240 0x264
```

The first nine HD groups already match (w7730). The residual is the HD-arm
`0xb0/0xc0/0xec` window, the tail alignment `0x140/0x150/0x160/0x170` vs
`0x148/0x158/0x168`, and the whole non-HD tail.

`vstr` census (single-float stores): original 27, ours 28. Ours has extra `vstr` at
`0x20, 0x10c, 0x124, 0x148, 0x1f4, 0x204, 0x3e8`; the original has `vstr` at
`0x138, 0x158, 0x15c, 0x170, 0x1e4, 0x3f8` instead. Same tail-alignment story.

## 2. One-group-at-a-time contiguity sweep (all real `link_so` builds)

Each group's 4 member stores were made contiguous at the position of its earliest field
(source order preserved; the intra-group order is a scheduler no-op). Result for every
group whose 4 members have a single top-level assignment:

| group | size | ratio % | diff bytes |
|---|---:|---:|---:|
| **HEAD** | **7072** | **45.029** | **5,789** |
| `0x14` | 7052 | 39.322 | 5,997 |
| `0x4` | 7052 | 24.522 | 5,967 |
| `0x24` | 7060 | 23.555 | 5,999 |
| `0x34` | 7084 | 39.546 | 5,953 |
| `0x44`/`0x54`/`0x64`/`0x7c` | 7072 | 45.029 | 5,789 (no-op) |
| `0xb0` | 6992 | 43.118 | 5,968 |
| `0xc0` | 7072 | 45.029 | 5,789 (already landed) |
| `0xec` | 7016 | 42.640 | 6,007 |
| `0x294` | 7064 | **48.107** | 5,916 |
| `0x2a4` | 7072 | 45.029 | **5,788** |
| `0x108` | 7092 | 42.883 | 5,861 |
| `0x118` | 7028 | 41.196 | 5,839 |
| `0x140` | 7004 | 37.908 | 5,951 |
| `0x150` | 7024 | 44.951 | 5,840 |
| `0x160` | 7072 | 45.029 | 5,789 (no-op) |
| `0x170` | 7080 | 45.281 | 5,850 |
| `0x180` | 7104 | 44.043 | 5,932 |
| `0x1a0` | 7116 | 43.263 | 5,955 |
| `0x1b0` | 7072 | 45.029 | 5,789 (no-op) |
| `0x1d0` | 7072 | 44.152 | 5,829 |
| `0x224` | 7140 | 43.969 | 5,878 |
| `0x24c` | 7072 | 44.958 | **5,788** |
| `0x264` | 7072 | 44.320 | **5,788** |
| `0x26c` | 7080 | 45.281 | 5,840 |
| `0x16c` | 7072 | 45.029 | **5,788** |
| `0x13c` | 7012 | 39.527 | 5,967 |
| `0x2a0` | 7000 | 43.964 | 5,931 |
| `0x5c` | 7072 | 45.029 | 5,789 (no-op) |

The groups whose members are inside `if`/`else` chains (`0x2c8`, `0x128`, `0x190`,
`0x1c0`, `0x204`, `0x214`, `0x23c`, `0x25c`) cannot be moved as a single 4-line block
and were left alone.

## 3. Combining the zero-delta groups

The size deltas are not additive. Exhaustive subset search over the 8 primitives
`{0x170, 0x26c, 0x294, 0x2a4, 0x24c, 0x264, 0x16c, 0x1d0}` (256 real builds) found the
minimum target-function diff bytes at **5,784**:

```
{0x26c, 0x2a4, 0x24c, 0x264, 0x16c}  →  7072 B, ratio 44.958, diff bytes 5,784
```

This is the landed variant. The five moves are pure member-store reorders (see the
source diff); no expression changed.

## 4. NEW finding — the `0x294` group reorder matches the original's register allocation

`0x294` contiguous **alone** is the only single move that reproduces the original's
prologue:

| binary | prologue at `0xd102c` |
|---|---|
| original | `e92d 0fc0  stmdb sp!, {r6, r7, r8, r9, sl, fp}` |
| HEAD | `e92d 0f80  stmdb sp!, {r7, r8, r9, sl, fp}` |
| `0x294` contiguous | `e92d 0fc0  stmdb sp!, {r6, r7, r8, r9, sl, fp}` ✓ |

The original's decompile (`layout-orig-ghidra-decompile.txt` lines 757-760) stores the
`0x294` region as four 8-byte chunks (`0x294, 0x29c, 0x2a4, 0x2ac`), so the source order
is ascending and the four individual stores are source-adjacent. The reorder changes
register allocation (r6 becomes live) and raises the fuzzy ratio to **48.107 %**.
It costs 8 B of body (7064 B) and raises diff bytes to 5,916.

Adding one +8 B group (`0x170` or `0x26c`) restores 7072 B with the matching prologue:

| variant | size | ratio % | diff bytes |
|---|---:|---:|---:|
| `0x294` + `0x26c` | 7072 | **47.680** | 5,838 |
| `0x294` + `0x170` | 7072 | **47.998** | 5,887 |
| `0x294` + `0x26c` + `{0x2a4,0x24c,0x264,0x16c}` | 7072 | 47.680 | 5,836 |

These are the largest structural wins found (ratio 45.0 → 47.7–48.0, matching prologue),
but they regress the same-address diff-byte count by ~50 because the literal pool shifts
by 8 B. They are recorded here as the next lever if the project decides the fuzzy ratio
outranks the raw diff-byte count.

## 5. Why the tail groups cannot be matched by reordering alone

Making `0x140`/`0x150`/`0x160`/`0x170` contiguous always shrinks the body
(7004/7024/7072/7080 B) because the original's extra ~64 B of duplicated common-arm
stores is absent. The original decompile shows the HD arm writing the tail as 8-byte
chunks (`*(undefined8 *)(this + 0x140) = uVar27; *(undefined8 *)(this + 0x148) = uVar28;`
then `0x150`, `0x154`, `0x158`, `0x15c`, ...) and repeating `0x150/0x154/0x160/0x164/
0x170/0x174` in the branch arms — that is the missing code, not a reorder.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7752
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4506

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > ours.dis
python3 /dev/shm/w7752/stores.py orig.dis ours.dis   # base-register-tracked store census
python3 /dev/shm/w7752/score.py                       # 7072 / 44.958 / diffbytes 5784
python3 /dev/shm/w7752/textdiff.py cmake-build-match/libgof2hdaa.so  # .text 85,138
```

Scratch harnesses (not committed, `/dev/shm/w7752/`): `score.py` (exact single-function
ratio + same-address raw-byte diff), `stores.py`/`stores2.py` (base-register-tracked
`vst1` census), `fields.py`/`mapgroups.py` (field→offset and group→source-line maps),
`reorder.py`/`move.py`/`multi.py` (source-reorder harnesses), `sweep.py`/`rand.py`
(group and random-move sweeps), `dbmap.py` (diff-byte address histogram),
`textdiff.py` (whole-`.text` differing bytes), `Layout.head.cpp` (pristine body).
