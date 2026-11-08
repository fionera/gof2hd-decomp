# `Layout::Layout` C1/C2 — wave 164 / worker w11006

## Second-half store-order relocations: raw **5,705 -> 5,698** (-7),
## aligned **2,082 -> 2,107** (+25), size **7,072 B** exact.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-163 HEAD `b3d3a1f68`: raw **5,705**, aligned **2,082**,
7,072 B.  Exit: raw **5,698**, aligned **2,107**, size 7,072.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.

Scope: the second half of the constructor (fields `0x60..end`), i.e. the
tail from `field_0x1e8` on.  The first half is owned by another worker.

## 1. Landed change: four single-statement relocations

All four are pure statement moves (no expression/constant/type edits).
Measured individually from the 5,705 baseline:

| moved statement              | from                          | to                            | single raw | single aligned |
|------------------------------|-------------------------------|-------------------------------|-----------:|---------------:|
| `field_0x2cc`                | before `int t_field_0x2c8`    | after `field_0x2c8 = ...`     | 5,704      | 2,085          |
| `ringNearMidThreshold` (0xec)| before `field_0xcc`           | after `field_0xcc`            | 5,704      | 2,082          |
| `field_0x18c`                | before `field_0x19c`          | after `field_0x19c`           | 5,704      | 2,078          |
| `field_0x1d4`                | before `field_0x1c0`          | after `field_0x1c0`           | 5,701      | 2,108          |

Combined: **5,698 / 2,107 / 7,072**.  The combination is super-additive on
raw (each single move is only -1..-4) and the aligned regression is
concentrated in the `field_0x1d4` move (+26).  Dropping `field_0x1d4` gives
5,702 / 2,081 (both metrics better than baseline but 4 raw bytes worse than
the full set); raw is the byte-identicality ground truth, so the full set is
landed.

## 2. Original tail structure (what the residual diff is)

The original tail `d21e2..d2800` is a **multi-way device dispatch**, not a
per-field ternary cascade like our source:

```
d21e2  field_0x1e8, field_0x1e4
d2202  beq d22b8
  d2204..d22ae  path A (retina): 0x1fc 0x1ec 0x27c/0x280 0x3e0/0x3e4
                0x3e8 0x3f4/0x3f8 + vst1 (0x204,0x208) (0x214,0x218)
                (0x224,0x228) (0x23c,0x240) (0x24c,0x250) (0x25c,0x260)
                (0x26c,0x270)
  d22b2  beq.w d24de / b d2530
d22b8  cmp r9; beq d240e
  d22c0..d240c  path B (n9): 0x1fc 0x1ec 0x224 0x228 0x22c 0x230 0x234
                0x238 0x24c/0x250 0x20c 0x218 0x220 0x204/0x208 0x210/0x214
                0x21c 0x23c/0x240 0x278 0x264/0x268 0x25c 0x260 0x274 0x27c
d240e  path C (else): same 0x204..0x270 vst1 groups
d2498  shared: 0x280 0x3e4 (scrollBarHandle) 0x3e0 (scrollBarInset)
d24ba..d2530  shared reward box: 0x3e8 0x3f4 0x3f8 0x3ec 0x3f0
d2548..d2800  final fields 0x2bc 0x3fc 0x200 0x1f0/0x1f4 0x1f8 0x284
                0x285 0x286 0x288/0x28c 0x290 0x2f4 0x2f8 0x2fc/0x300/0x304
                0x308 0x30c 0x314 0x318 0x31c 0x320
```

Our build has the same 5-block shape but a 2-way tail dispatch and the
`0x204..0x238` fields in a single block.  This is where most of the tail
diff comes from (biggest regions: `d22cd..d23a4` 215 B, `d21ea..d2266`
124 B, `d2522..d259a` 120 B, `d23db..d244f` 116 B).

### `field_0x1e4` is `(int)(float)` in the original

The original tail starts

```
d21e2  vcvt.s32.f32 s0, s0        ; s0 = 5.625f (HD) / 6.0f (n9) / 8.0f or 4.0f
d21fa  str.w  r1, [r0, #0x1e8]    ; field_0x1e8
d21fe  vstr   s0, [r0, #484]      ; field_0x1e4  (0x1e4)
```

so `field_0x1e8` and `field_0x1e4` are **two separate stores**, and
`field_0x1e4` is the int result of a runtime float->int conversion
(HD = `4 * 1.40625f` = 5.625f, n9 = 6.0f, retina = 8.0f, else = 4.0f).
Our source has plain int constants and the compiler fuses the two stores
into `strd r8, r1, [r0, #484]`.

Rewriting `field_0x1e4` as
`(int)(Globals::iPadHD ? 4 * 1.40625f : (Globals::n9 ? 6.0f : (Globals::retinaDisplay ? 8.0f : 4.0f)))`
does reproduce `vcvt.s32.f32` + `vstr s0`, but costs **+16 B**
(5,813 / 2,447 / **7,088**) and is a raw regression, so it is not landed.
Swapping the two statements (`0x1e8` first) is worse still
(6,019 / 2,408 / 6,984).

### Original vst1 pairs (contiguous 8-byte constant groups)

```
(0x1c0,0x1c4) (0x1d0,0x1d4) (0x204,0x208) (0x214,0x218) (0x224,0x228)
(0x23c,0x240) (0x24c,0x250) (0x25c,0x260) (0x26c,0x270)
```

Ours emits `(0x208,0x20c) (0x218,0x21c) (0x228,0x22c) (0x240,0x244)
(0x250,0x254) (0x260,0x264) (0x270,0x274)` — shifted by 4.  The original
also packs the `retina` booleans with `vdup.16/vmovl.u16/vshl.i32/vbsl`
and stores single lanes with `vst1.32 {d26[1]}` for 0x20c/0x218/0x220;
ours uses plain `movs`/`str`.

### Original `stm.w` groups

* n9 block: `stm.w r12!, {r1,r2,r3,r6}` for `0x2c0..0x2cc`.
* tail: `stm.w r12!, {r2,r3,r5}` for `0x2fc,0x300,0x304`.

Our `field_0x94` sits between `field_0x300` and `field_0x304` (the w10807
placement) and breaks the tail `stm`.  Moving `field_0x94` elsewhere makes
the `stm` form but changes the size: a full sweep of all 205 top-level
positions found the current position is the best size-exact raw
(5,698); the best non-current size-exact is 5,708 (position 149).  The
5,681 positions reported by wave 162 are not size-exact (7,064).

### Reward-box shared tail order

The original's shared reward-box block stores
`0x280, 0x3e4 (scrollBarHandle), 0x3e0 (scrollBarInset), 0x3e8,
0x3f4 (rewardBoxY), 0x3f8 (rewardBoxY2), 0x3ec (rewardBoxHeight),
0x3f0 (rewardBoxX)`.

Note `rewardBoxY2 = (rewardBoxY + 17) << retina` is computed from the
**new** `rewardBoxY` in the original, while our source computes it from the
old (uninitialised) value because the `rewardBoxY2` if/else precedes the
`rewardBoxY` assignment.  Swapping them is semantically correct but gives
5,756 / 2,082 / **7,080** (+8 B), not size-exact.  Reordering the whole
reward-box block to the original order is worse (5,965 / 2,395 / 6,956).

## 3. Shapes measured and not landed (all real `link_so`)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline HEAD | 5,705 | 2,082 | 7,072 |
| **landed 4 relocations** | **5,698** | **2,107** | **7,072** |
| 3 relocations (drop `0x1d4`) | 5,702 | 2,081 | 7,072 |
| aligned-greedy (wave-162 scratch) | 5,703 | 2,078 | 7,072 |
| `field_0x1e4` float cast | 5,813 | 2,447 | 7,088 |
| `field_0x1e8 = (n9\|\|iPadHD) ? 1 : retina` | 5,878 | 2,294 | 7,024 |
| `0x1e4`/`0x1e8` swap | 6,019 | 2,408 | 6,984 |
| `rewardBoxY` before `rewardBoxY2` | 5,756 | 2,082 | 7,080 |
| reward-box block to original order | 5,965 | 2,395 | 6,956 |
| `0x280,0x3e4,0x3e0,0x285` reorder | 5,764 | 2,166 | 7,080 |
| `0x1f4`/`0x1f0` swap | 5,748 | 2,077 | 7,080 |
| `0x28c`/`0x288` swap | 5,728 | 2,107 | 7,072 |
| `0x285` next to `0x284` | 5,719 | 2,109 | 7,072 |
| `0x2c0` before `0x2c4` | 5,964 | 2,269 | 7,056 |
| `0x2c0` after `0x2c4` | 5,949 | 2,263 | 7,040 |
| `0x2c0` after `0x2cc` | 5,883 | 2,255 | 7,056 |
| `0x204` next to `0x208` | 5,967 | 2,355 | 7,024 |
| `0x214` next to `0x218` | 5,893 | 2,229 | 7,064 |
| `0x238_blk16.b` next to `.c` | 5,967 | 2,370 | 7,000 |
| `0x1d4` after `0x1d0` | 5,872 | 2,498 | 7,080 |
| `0x1c4` next to `0x1c0` | 5,900 | 2,155 | 7,040 |
| 0x109..0x128 ascending field order | 5,794 | 2,187 | 7,108 |
| full second-half memory-order sort | 6,030 | 2,648 | 6,740 |

Adjacent-swap greedy from the landed state finds no further raw
improvement (local optimum).  `field_0x94` full placement sweep: current
position is the best size-exact raw.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11006
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j8
```

Scratch `/dev/shm/w11006/`: `sweep.py` (`build_measure`), `stmts2.py`
(statement splitter for lines 45..311), `reord.py`/`reord2.py` (block
reorders), `sweep94b.py`/`sweep94c.py` (`field_0x94` placement sweeps),
`scan_adj.py` (Pareto adjacent-swap scan), `rawregions.py` (raw-diff
regions), `w4.cpp` (landed source).
