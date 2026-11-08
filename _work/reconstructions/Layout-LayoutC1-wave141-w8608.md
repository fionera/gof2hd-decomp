# `Layout::Layout` C1/C2 — wave 141 / worker w8608
## `0xdc` strd-vs-vst1 flip **landed** at 7072 B: raw same-address diff 5,759 → **5,752**

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
image base `0x10000`.  Baseline = wave-140 master `e18e0c2a4` (7072 B, target-function
same-address differing bytes **5,759**).

## Result — flip landed, raw diff improved by 7 bytes

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| `_ZN6LayoutC2Ev` size | 7072 | **7072** |
| target-function same-address diff bytes | 5,759 | **5,752** |
| `0xdc` store form | one `vst1.32 {d2-d3}` | **two `strd` pairs** (matches original) |
| `.text` size | `0x1337c8` | `0x1337c8` |
| byte_exact / linked_exact | 4507 / 4507 | **4507 / 4507** |
| size_exact / data_exact | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |
| symbol parity | 5370/5370 | 5370/5370 |

`cmake --build cmake-build-match --target check -j6` → **RATCHET PASS**.

## 1. The winning source shape

The four float stores are made **contiguous in source with one non-vectorizable int store
interleaved** (`field_0x2b8` between `0xdc` and `animSpeedScale`), placed immediately after
`field_0xcc`:

```
field_0xcc
field_0xdc            (0xdc)
field_0x2b8           (0x2b8)   <- scalar int store, breaks the 4-chain
animSpeedScale        (0xe0)
driftSpeedDivisor     (0xe4)
ringRadiusScale       (0xe8)
field_0xf0
field_0xf8
...
```

SLP then declines the 4-wide `<4 x float>` bundle and the backend emits the original's

```
strd r2, r4, [r0, #220]   ; 0xdc = 14.0625f, 0xe0 = 1.40625f
strd r1, r4, [r0, #228]   ; 0xe4 = 28.125f,  0xe8 = 1.40625f
```

verified with a base-register-tracked disassembly check (`strd ... [#220]` present).

## 2. The companion reorder that recovers the 85-byte flip cost

The flip alone (`run dc,field_0x2b8,e0,e4,e8 after field_0xcc`) is **7072 B / 5,917 diff**
(+158 vs HEAD).  The remaining 165 bytes were recovered by a **random local search over the
statement order of the whole `0xcc..0x168` region** (lines 85..169), constrained to
`size == 7072` **and** the `strd` pattern.  The winning permutation moves:

* `field_0x138 = 94` (the iPadLarge branch body) out to just after `field_0x204`;
* `field_0x138 = 70` (the retina branch body) into the `n9` branch of the `field_0x190`
  if/else chain;
* `field_0x168` before `field_0x2bc`;
* `field_0x108` before `field_0x110_strip58`;
* `oreLabelYOffset` after `field_0x118`.

Search history (all real `link_so` builds, `size`/`diff`):

| stage | size | diff | `0xdc` |
|---|---:|---:|---|
| HEAD | 7072 | 5,759 | `vst1` |
| flip run only | 7072 | 5,917 | **strd** |
| + random 1-3 moves, block 85..115 | 7072 | 5,811 | **strd** |
| + random moves, block 85..169 | 7072 | 5,783 | **strd** |
| + random moves, block 85..169 (seed 23) | 7072 | **5,752** | **strd** |

A semantically-clean variant that keeps the `field_0x130/0x134/0x138` if/else intact and only
reorders the surrounding statements bottoms out at **5,778** (block 85..169 with 116..135
locked) — still above baseline.  The landed 5,752 variant therefore relocates the two
`field_0x138` branch bodies; see §4.

## 3. Region histogram (same-address diff bytes)

| region | HEAD | landed |
|---|---:|---:|
| arms `0x000..0x2e0` | 640 | 682 |
| literal pool `0x2e0..0x540` | 218 | 209 |
| middle/common `0x540..0x1880` | 4,590 | 4,613 |
| all-device tail `0x1880..0x1ba0` | 311 | 307 |
| **total** | **5,759** | **5,752** |

The flip still costs ~42 bytes in the arms (the two `strd` + literal words vs one `vst1` +
vector literal), but the companion reorder now wins it back in the pool/middle/tail.

The original's HD arm ends at `d1268` (`b.w d21e2`, common arm at offset `0x11ba`); ours ends
at `d1274` (`b.w d229e`, offset `0x1276`) — the common arm is still **186 B late**, i.e. our
arms are 186 B too long / common+tail 186 B too short.  That is the wave-135 residual and the
next big lever, not the `0xdc` group.

## 4. Caveat / open question

`field_0x138` is **not** a clean per-device if/else in the original: it is stored only three
times in the whole function —

```
d1244  strd r1, r3, [r0, #312]   ; 0x138 = 66 (HD path; 47*1.40625 truncated)
d1abe  strd r1, r2, [r0, #312]   ; 0x138 = 52 (n9 path)
d1d22  vstr s2,     [r0, #312]   ; computed int (vcvt.s32.f32) on the default path
```

so the original's branch structure for `0x130/0x134/0x138` is *not* our 5-way if/else (ours
emits 5 stores).  The landed permutation moves the `94`/`70` branch bodies to positions that
are byte-closer but semantically odd; a future pass that reconstructs the real
`0x130/0x134/0x138` structure (likely a default assignment plus a shared computed store) should
replace this.  The *flip mechanism itself* (§1) is independent of this and is the durable
result.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8608
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8608/fdiff.py _ZN6LayoutC1Ev          # 7072/7072, diff 5752
```

Scratch harnesses (not committed, `/dev/shm/w8608/`): `fdiff.py` (size + raw same-address
diff), `lib.py` (build/score/permute/`dc_kind` base-register-tracked flip detector),
`stores.py` (base-register-tracked store sequence dump), `region.py` (region histogram),
`exp*.py` / `rand*.py` (the sweeps above), `Layout.base.cpp` (pristine wave-140 body).

## 6. Open questions

* The 186 B arms-too-long / common-too-short shift is the dominant residual; aligning the
  common arm would make the `0xdc` flip diff-cheap.
* Reconstruct the real `0x130/0x134/0x138` structure (3 original stores, not 5) so the
  `field_0x138` relocation in §2 can be replaced by a semantically-clean shape.
* The `0x294`/`0x2a4` SLP runs are still the coupling lever for any further float-group move.
