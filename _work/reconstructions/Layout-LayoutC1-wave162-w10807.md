# `Layout::Layout` C1/C2 — wave 162 / worker w10807

## `field_0x94` tail relocation: raw **5,715 -> 5,705** (-10),
## aligned **2,050 -> 2,082** (+32), size **7,072 B** exact.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-161 HEAD `29248c1d0`: raw same-address diff **5,715**,
aligned instruction diff **2,050**, size 7,072.
Exit: raw **5,705**, aligned **2,082**, size 7,072.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.
`.text` diff **85,374 -> 85,364** (-10).  `.rel.plt` still byte-identical
(0), `.ARM.extab` 725 unchanged, every section size exact.

## 1. The `0x94..0xa0` stmia: what actually produces it

The original's `stmia.w r4, {r5, r8, ip, lr}` at `0xd113e` is **not** a
4-store merge.  It is the middle third of an **8-store contiguous run**
`0x8c..0xa8` that the post-RA ARMLoadStoreOptimizer groups as
`strd(0x8c,0x90) + stmia(0x94..0xa0) + strd(0xa4,0xa8)`:

```
d1136 strd  r6,r4,[r0,#140]   ; 0x8c,0x90
d113a add.w r4,r0,#148
d113e stmia.w r4,{r5,r8,ip,lr}; 0x94,0x98,0x9c,0xa0
d1148 strd  r9,r3,[r0,#164]   ; 0xa4,0xa8
```

Four consecutive constant int stores are **always** lowered to
`vld1.64 {d16-d17}` + `vst1.32 {d16-d17}` by clang 7.0.2 at `-Oz`, never
to stmia (verified with a standalone harness for 4/8-store functions,
aligned and unaligned bases, literal/struct/memcpy/comma forms).  stmia
only appears for a run of 3 (no vst1 form) or for a run of 8 split 2+4+2
by the load/store optimizer.  `stmia.w r0,{r1,r2,r3,ip}` is also emitted
when the four values are already in general registers (e.g. function
arguments), which is what the original's hoisted-constant register
allocation gives.

## 2. Why making the run contiguous loses 24-136 B

Moving `field_0x94` next to `field_0x98` (the wave-161 hypothesis) makes
the four stores contiguous in source but the compiler still emits four
`str.w` (no stmia) and the function shrinks to **7,048 B** (v1).  Moving
the whole 8-store run `0x8c,0x90,0x94,0x98,0x9c,0xa0,0xa4,0xa8` to the
`0x8c` source position gives **6,936 B** (v7); to the `0x94` tail position
**6,944 B** (v9).  In both, the iPadHD specialization boundary moves and
the run is emitted as `strd+strd` in the common tail, not `strd+stmia+strd`.
The 7,072 B size is tied to the *scattered* source placement.

Sweeps run (all real clang 7.0.2 `-Oz` object compiles):
* 24 permutations of `0x94/0x98/0x9c/0xa0` as a contiguous unit at every
  one of the 205 top-level statement positions: **no** position gives
  7,072 B and **no** position produces a `0x94` stmia.
* memory-order sort of the whole `0x108..0x1e8` region (chains kept as
  units): 6,808 B; the 16-byte groups start at `0x13c` instead of `0x140`.
* the `0x140` group is still blocked as wave-160/161 found: putting
  `0x140/0x144/0x148/0x14c` contiguous creates the group but shifts the
  downstream `0x170..0x1b0` boundaries.

## 3. Landed change

One statement relocation in the tail (no expression/constant/type edit):

```
line 282 (between field_0x1f4 and field_0x1f0)
  -> line 303 (between field_0x300 and field_0x304)
```

This is the lowest raw diff among the 25 size-exact single-statement
placements of `field_0x94` (5,705; next 5,715 at the original position,
5,720 at line 299).  The aligned heuristic rises by 32 because the tail
block shifts; raw same-address bytes are the byte-identicality ground
truth.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10807
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10807/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5705
python3 /dev/shm/w10807/align_diff.py _ZN6LayoutC1Ev   # 2082
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10807/`: `cc.sh` (0.14 s single-TU compile),
`size.sh`, `dis.sh`, `objdiff.py` (object `.text` byte compare),
`sweep_group.py`/`sweep_one.py`/`sweep_link.py` (placement sweeps with
link+fdiff), `moveblocks.py`/`reorder.py`/`sortblocks.py` (source movers),
`t*.cpp` (stmia lowering harnesses), `v1..v9.cpp` (measured variants).
