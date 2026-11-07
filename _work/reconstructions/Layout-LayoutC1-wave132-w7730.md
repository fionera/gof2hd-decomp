# `Layout::Layout` C1/C2 — wave 132 / worker w7730
## SLP store-group / literal-pool axis: the `0xc0` group reorder is a real `.text` win

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = current master (wave 132), `49.003189792663 %` unrounded, target-function
differing bytes **5,848**, `.text` differing bytes **85,244**.

## Result — landed: `0xc0` group reorder (fewer differing bytes, same 7072 B)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| unrounded `difflib` ratio | 49.003189792663 % | **45.028890217175 %** |
| target-function differing bytes | 5,848 | **5,789** (−59) |
| `.text` differing bytes | 85,244 | **85,185** (−59) |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
The fuzzy ratio drops because the reorder changes register allocation and moves the end
literal pool by `+0x18`, so every `add rX, pc, #imm` in the body shifts; the *actual*
differing bytes fall. The gate (byte/linked/size/data/imports/lint) is unchanged.

## 1. Store-group census (base-register-tracked, `/dev/shm/w7730/stores.py`)

Original HD-arm `vst1` groups (16-byte, in instruction order):

```
0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c
0xc0 0xb0 0xec 0x294 0x2a4 0x108 0x118
0x140 0x150 0x160 0x170 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0
```

HEAD HD-arm groups:

```
0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c
0xb4 0xc4 0x294 0x2a4 0xdc
0x118 0x108 0x128 0x148 0x158 0x168 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0
```

The first nine groups already match. The residual is the HD arm's `0xb0/0xc0/0xec`
window shift and the tail's `0x140/0x150/0x160/0x170` vs `0x148/0x158/0x168` shift
(same census as w7685, re-confirmed on current master).

## 2. The literal pool is already in the original's order except for `0.325f`/`0.65f`

Original end pool `0xd2854` starts:

```
3ea66666 3f266666 42c80000 42480000   (0.325 0.65 100.0 50.0)
```

HEAD end pool `0xd28d0` starts:

```
42c80000 42480000 ...                 (100.0 50.0 ...)
```

i.e. HEAD is the original's pool **minus the first two words**: the `field_0x90`
`0.325f`/`0.65f` pair is in a `.text` island at `0xd2548`/`0xd254c`, not the end pool.
The original loads it with the same conditional-pointer trick
(`addw r1, pc, #3848` at `0xd1948` → `0xd2854`, `it eq; addeq r1, #4`), so the *code
shape* is identical; only the constant's placement differs. Every remaining pool word
matches word-for-word. This is the cleanest statement of the pool-order residual.

Attempts to force `0.325/0.65` into the end pool all failed to keep 7072 B:

| variant | size | ratio % |
|---|---:|---:|
| `field_0x90` as `if/else` chain | 7072 | 49.003 (byte-identical codegen) |
| swap `field_0x8c` / `field_0x90` source lines | 7048 | 48.053 |
| `0.325/0.65` island stays at `0xd2548` in both | — | — |

## 3. Per-group reorder sweep (all real `link_so` builds)

Each vectorized group's 4 member stores were made contiguous at the position of its
first field. Only the `0xc0` group keeps 7072 B:

| variant | size | ratio % | target diffbytes |
|---|---:|---:|---:|
| **HEAD** | **7072** | **49.003** | **5,848** |
| `0xb0` group contiguous | 7072 | 43.520 | 5,841 |
| **`0xc0` group contiguous (landed)** | **7072** | **45.029** | **5,789** |
| `0xec` group contiguous | 7032 | 43.222 | — |
| `0x294` group contiguous | 7064 | 48.754 | — |
| `0x2a4` group contiguous | 7040 | 45.012 | — |
| `0x108` group contiguous | 7016 | 45.128 | — |
| `0x118` group contiguous | 7072 | 48.625 | 5,877 |
| `0x128` group contiguous | 7064 | 49.023 | — |

`0xc0` + `0x294` is identical to `0xc0` (the `0x294` move is a scheduling no-op once
`0xc0` is applied). `0xc0` + `0xb0` / `0x108` / `0xec` all shrink below 7072.
40 random single-statement moves (seed 7730) produced no other 7072-B variant.

## 4. Landed source change

`field_0xc4`, `field_0xc8`, `field_0xcc` are moved immediately after `field_0xc0`, so
the four stores the original vectorizes as `vst1.32 {d0-d1}, [r0,#0xc0]` are
source-adjacent:

```c++
this->field_0xc0 = ...;
this->field_0xc4 = ...;
this->field_0xc8 = ...;
this->field_0xcc = ...;
```

The body stays 7072 B. The reorder does not by itself change our `vst1` destinations
(HEAD still emits `0xb4`/`0xc4`/`0xdc`), but it changes register allocation (prologue
`stmdb sp!, {r5,r6,r7,r8,r9,sl,fp}` → `{r7,r8,r9,sl,fp}` in the HD arm) and moves the
end pool by `+0x18`, which is where the −59 differing bytes come from.

## 5. Conclusion / open question

The pool order is *already* the original's except for the `0.325f`/`0.65f` pair, and the
code that loads that pair has the original's exact shape. The remaining lever is the
constant's *placement* (`.text` island vs end pool), which is a pool-splitting decision
driven by the cold-block layout, not by the member-store order. Matching the HD-arm
`vst1` windows still costs 12–120 B of body, so the original's extra cold-tail / duplicated
common stores are still the missing +64 B identified by w7685/w7703.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7730
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4506

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w7730/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w7730/ours.dis
python3 /dev/shm/w7730/stores.py /dev/shm/w7730/orig.dis /dev/shm/w7730/ours.dis
python3 /dev/shm/w7730/score2.py        # 7072 / 45.029 / diffbytes 5789
python3 /dev/shm/w7730/textdiff.py cmake-build-match/libgof2hdaa.so  # 85,185
```

Scratch harnesses (not committed, `/dev/shm/w7730/`): `stores.py` (base-register-tracked
store census), `score2.py` (exact single-function ratio + same-address raw-byte diff),
`textdiff.py` (whole-`.text` differing bytes), `pool.py` (literal-pool word dump),
`group.py` / `reorder.py` / `trymod.py` (source-reorder harnesses), `randsearch.py`,
`permsearch2.py`, `Layout.base.cpp`, `base.so`.
