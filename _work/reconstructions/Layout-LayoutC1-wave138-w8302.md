# `Layout::Layout` C1/C2 — wave 138 / worker w8302
## `0xdc` strd-vs-vst1 and `0x140..0x1d0` tail group: store-group-boundary reconstruction (no landed change)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-137 master `96b069792` (7072 B, target-function differing bytes **5,757**,
unrounded `difflib` fuzzy **43.6 %**).

## Result — no source win landed; baseline re-confirmed

| metric | HEAD | best candidate |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | 7072 (ties only, no better) |
| target-function differing bytes | **5,757** | 5,757 (8 float-line permutations) |
| unrounded `difflib` fuzzy | **43.6 %** | 43.6 % |
| `.text` size | `0x1337c8` | unchanged |
| byte_exact / linked_exact | 4507 / 4507 | 4507 / 4507 |

`cmake --build cmake-build-match --target check -j6` → **RATCHET PASS**
(byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN, drift 0).

## 1. Ground truth — the two store groups in the original HD arm

Original HD arm (`d11c0..d11de`), base-register tracked:

```
d11c0  stmia.w fp, {r3, r6, r8}   ; fp = r0+0xd0 -> 0xd0, 0xd4, 0xd8   (3 int stores)
d11c8  strd    r2, r4, [r0, #220] ; 0xdc = 14.0625f, 0xe0 = 1.40625f
d11ce  strd    r1, r4, [r0, #228] ; 0xe4 = 28.125f,  0xe8 = 1.40625f
```

Ours (HD arm, object offsets `0x15a`/`0xdc`):

```
d1182  add.w  r3, r0, #220 ; 0xdc
d1186  vst1.32 {d2-d3}, [r3]          ; one 16-byte store of a 4-word literal vector
```

The original's literal pool has the four values as **scalar** words (`d14b8` = 28.125,
`d14bc` = 14.0625, `d1444` = 1.40625); ours has a 16-byte vector literal at `d1488`.
So the original's `0xdc..0xe8` group was *not* SLP-vectorized; the backend merged the
four i32 stores into two `strd` pairs. This is exactly what `-fno-slp-vectorize` emits
for four unconditional float-constant stores.

The group boundaries are coupled to the preceding `0xb0/0xc0/0xd0` groups:

| group | original | ours |
|---|---|---|
| `0xb0` | `vst1 0xb0..0xbc` | scalar `str 0xb0`, then `vst1 0xb4..0xc0` |
| `0xc0` | `vst1 0xc0..0xcc` | `vst1 0xc4..0xd0` |
| `0xd0` | `stmia 0xd0,0xd4,0xd8` | `strd 0xd4,0xd8` (0xd0 folded into the `0xc4` vector) |
| `0xdc` | `strd (0xdc,0xe0)`, `strd (0xe4,0xe8)` | `vst1 0xdc..0xe8` |

So the whole `0xb0..0xe8` window is shifted by one 4-byte slot in ours. The original's
groups are 16-byte aligned (`0xb0`, `0xc0`, `0xd0`); ours start at `0xb4`, `0xc4`, `0xd4`.

## 2. Tail group ground truth

Original HD arm vectorizes the 40-int run `0x140..0x1dc` as ten 16-byte `vst1` groups:

```
0x140 0x150 0x160 0x170 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0   (orig)
0x148 0x158 0x168 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0        (ours)
```

Ours starts at `0x148` because `field_0x140` (source line 295) and `field_0x144`
(line 285) are stored in the *common* arm, so the HD arm's vectorizable run begins
at `0x148`. Moving both into the tail source order **does** flip the groups to
`0x140/0x150/0x160` (verified by disassembly), but removes two scalar stores from
the common arm and shrinks the body to 7032 B.

## 3. New micro-experiments — what makes the `0xdc` group flip to `strd`

All built with the exact NDK r18b clang 7.0.2 `-Oz` flags into `/dev/shm/w8302/t/`.

| # | source shape | `0xdc` group |
|---|---|---|
| t2 | four unconditional `float` constant stores | `vst1` (one 16-byte vector) |
| t7 | `float,int,float,int` fields, ints assigned `0x3fb40000` | **two `strd`** (matches orig) |
| t12 | four float stores **inside `if (hd)`**, interleaved with int stores | **two `strd`** |
| t14 | t12 + the full `0x2b4..0x114` int store set | **two `strd`** |
| t17 | t14 + the full `0x140..0x1dc` tail | **two `strd`** |
| t15/t16/t18 | assignments in the **common** section, `hd ? a : b` ternaries, later if/else | four `vstr` (arms merged into conditional moves) |
| real `Layout.cpp` | common section, sunk into the HD arm | `vst1` |

Key finding: the choice is **not** reachable by permuting the four float statements
alone. It depends on the *pre-SLP store order* of the surrounding int stores and on
whether the compiler sinks the common assignments into a standalone HD block.

The real function's pre-SLP IR (from `-fno-slp-vectorize -S -emit-llvm`) has the four
float stores at IR lines 1587/1597/1603/1615, interleaved with int stores:

```
0xe4, 0xf8, 0x29c, 0x294, 0x2a0, 0xdc, 0x298, 0x2a8, 0xe0, 0x2b0, 0x2ac, 0x2a4, 0x2b4, 0xec, 0xe8
```

SLP first vectorizes the int runs (`0x294..0x2a0`, `0x2a4..0x2b0`), which makes the
four float stores adjacent in the remaining store list, and a later SLP step merges
them into `<4 x float>`. In the micro-tests t12/t14/t17 the same int stores either
stay scalar or are grouped differently, so the float chain never forms and the backend
emits `strd`.

## 4. Source-shape sweeps (every entry a real `link_so` build, symbol size checked)

`osize` always 7072; `usize` is our symbol size (7072 required for a landing).

| variant | change | usize | diff bytes | fuzzy |
|---|---|---:|---:|---:|
| HEAD | — | **7072** | **5757** | **43.6** |
| v1 | move `field_0xb0` after `field_0xb8` | 7056 | 5884 | 42.9 |
| v3 | move `field_0x2a8` before `ringRadiusScale` | 7072 | 5758 | 43.5 |
| v6 | swap `field_0xdc` / `animSpeedScale` | 7088 | 5813 | 43.8 |
| v7 | swap `driftSpeedDivisor` / `ringRadiusScale` | 7088 | 5813 | 43.8 |
| v10 | move `field_0x2a8` before `field_0xdc` | 7072 | 5794 | 43.9 |
| v11 | move all four float lines after `field_0x2b8` | 7024 | 5895 | 39.8 |
| v12 | move all four float lines before `field_0xf8` | 7032 | 5872 | 43.5 |
| v13 | move all four float lines after `field_0x114` | 6920 | 5872 | 41.8 |
| v14 | move all four float lines after `field_0x12c` | 6984 | 5905 | 40.9 |
| v15 | move all four float lines after `field_0x10c` | 6912 | 5873 | 36.2 |

All **24 permutations** of the four float statements at their current source positions
(88, 93, 96, 102) were built: 8 give a codegen-identical tie at **7072 / 5757 / 43.6**,
the other 16 give 7080/7088 with 5813–5819. None flips the `0xdc` group.

All **contiguous** placements of the four float statements give `usize = 7032`
(diff 5857–5874) — SLP always vectorizes a contiguous 4-chain.

Tail variants (move `field_0x140`/`field_0x144` into the tail source order):

| variant | usize | diff bytes | fuzzy |
|---|---:|---:|---:|
| t1 `0x140` before `0x14c` | 7048 | 5944 | 42.0 |
| t2 `0x144` before `0x14c` | 7056 | 5946 | 42.4 |
| t3 both before `0x14c` | 7032 | 5904 | 42.1 |
| t4 both before `0x148` | 7032 | 5927 | 42.1 |
| t5 both after `0x13c` | 7032 | 5904 | 42.1 |
| t6 `0x144` before `0x148` | 7056 | 5940 | 42.4 |
| t7 `0x140` before `0x148` | 7056 | 5976 | 42.1 |

t3/t4/t5 flip the HD groups to `0x140/0x150/0x160` (verified) but need +40 B of
compensating code elsewhere to keep 7072 B.

## 5. Conclusions

* The `0xdc` group is not a local permutation problem: it is a consequence of the
  whole `0xb0..0xe8` store-chain layout. The original's HD arm keeps the groups
  16-byte aligned (`0xb0`, `0xc0`, `0xd0`); ours is shifted one slot (`0xb4`, `0xc4`,
  `0xd4`) because `field_0xb0` is assigned far from `0xb4/0xb8/0xbc` and the SLP
  chain builder leaves it scalar.
* Moving `field_0xb0` after `field_0xb8` fixes the `0xb0/0xc0` boundaries but costs
  16 B and worsens the raw diff; the `0xdc` group still vectorizes.
* The tail flip is understood and cheap to trigger (`0x140`/`0x144` into the tail),
  but it removes two common-arm scalar stores and shrinks the body by 40 B. The
  missing bytes are the known "common arm too big / tail too small" residual, not
  this group.
* No source change improved on 7072 B / 5,757 diff bytes. HEAD is kept.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8302
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
python3 /dev/shm/w8302/score.py                            # 7072/7072, 5757 diff bytes, 43.6 %
```

Scratch harnesses (not committed, `/dev/shm/w8302/`): `score.py` (symbol-size + raw
diff + verify fuzzy via `tools/verify`), `variants.py`/`perm.py`/`tail.py`/`contig.py`/
`region2.py` (source-reorder harnesses), `Layout.base.cpp` (pristine wave-137 body),
`t/` (micro-experiments t2..t19).

## 7. Open questions

* The real function's SLP merges the float group only after the intervening int runs
  are vectorized. A source shape that keeps one of those int runs scalar (e.g. by
  breaking its 4-chain) should keep the floats split into `strd` pairs. The
  `0x294/0x298/0x29c/0x2a0` and `0x2a4/0x2a8/0x2ac/0x2b0` runs are the levers.
* The tail needs the +40 B common-arm/tail redistribution (wave-135 residual) before
  the `0x140` group flip can be landed at 7072 B.
