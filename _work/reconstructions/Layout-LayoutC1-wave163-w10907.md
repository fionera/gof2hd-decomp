# `Layout::Layout` C1/C2 — wave 163 / worker w10907
## 0x8c..0xa8 contiguous-run hypothesis: source shapes found that hold
## 7,072 B, but all are raw regressions.  Baseline stays raw **5,705**,
## aligned **2,082**, size **7,072**.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-162 HEAD `e51db2fc7`: raw **5,705**, aligned **2,082**,
7,072 B.  Exit: unchanged (no source change landed).

## 1. The original 8-store run (confirmed from objdump)

Original HD block (`d1118..d1148`):

```
d1118: strd  r6,r5, [r0,#0x74]     ; 0x74=161, 0x78=369
d1122: strd  r3,r6, [r0,#0x2c0]    ; 0x2c0=-1, 0x2c4=126
d1128: str.w  r3,   [r0,#0x2d8]    ; 141
d1132: str.w  r3,   [r0,#0xac]     ; 85
d1136: strd  r6,r4, [r0,#0x8c]     ; 0x8c=45, 0x90=0.46222222f
d113a: add.w r4, r0, #0x94
d113e: stmia.w r4, {r5,r8,ip,lr}   ; 0x94=14, 0x98=590, 0x9c=70, 0xa0=35
d1142: movs  r6, #28
d1144: mov.w r8, #52
d1148: strd  r9,r3, [r0,#0xa4]     ; 0xa4=7, 0xa8=85
```

So the emitted source order is `0xac, 0x8c, 0x90, 0x94, 0x98, 0x9c,
0xa0, 0xa4, 0xa8` — the 8-store run `0x8c..0xa8` is contiguous and is
`strd + stmia(4) + strd`.  `0xac` is emitted immediately before it and
`0xa8` reuses the `0xac` value register (`r3=85`).

Our baseline source instead has `eventBannerScale` (0x1e0) between
0x90 and 0x9c, `0x98` up at line 45, `0xac` after 0x9c, and
`0xa0/0xa8/0xa4` later; the run is therefore not contiguous.

## 2. Measured contiguous-run shapes

`int v` (used by `field_0xa8 = (int)(float)v`) and `eventBannerScale`
must be moved out of the run.  Harness: `/dev/shm/w10907/exp.py`
(chunk-permutation builder, exact NDK r18b `-Oz` object compile,
0.14 s/trial) and `exper.py` (size + code/pool split + store census).

| shape | size | raw | aligned |
|---|---:|---:|---:|
| baseline | 7,072 | **5,705** | **2,082** |
| naive contiguous run, `eventBannerScale` at tail | 6,944 | 5,901 | 2,649 |
| run, `0xac` after 0x9c | 6,984 | — | — |
| run, `0xa8` before `0xa4`, `eventBannerScale` after chunk 252 | **7,072** | 5,871 | 2,613 |
| 0x140 group only (`0x140,0x144,0x148,0x14c` after 0x128) | 7,036 | — | — |
| 0x140 group + `eventBannerScale` after chunk 126 | **7,072** | 5,897 | 2,585 |
| run + 0x140 group | 7,008 | — | — |

A full sweep of `eventBannerScale` over all 214 valid statement
positions (266 chunks minus the run members) found exactly **one**
7,072-B hit for the original run order (`0xa4` before `0xa8`): none.
The only 7,072-B hit with `0xa8` before `0xa4` is the chunk-252
variant above.  The 0x140-group sweep found exactly one 7,072-B hit
(chunk 126).

## 3. Why the contiguous run alone cannot hold 7,072

The original's HD block is **550 B** (`0xd1042..0xd1268`); ours is
**564 B**.  The naive contiguous run shrinks our HD block to ~562 B
and the whole function to 6,944 B.  The original's *literal pools are
interleaved with code* (the first `vld1.64` at `d1046` reads `d1308`,
inside the code range), so the 128-B "code/pool split" seen by w10806
is really 128 B of extra vector-constant materialisation spread over
several pools.  The original has 81 `vst1.32` / 86 `vld1.64` / 62
`strd`; our baseline has 70 / 77 / 44.  The contiguous run raises the
`stmia` count (4 -> 6) but does **not** recover the 11 missing
`vst1.32` groups, so the function stays 128 B short.

Holding 7,072 B requires moving `eventBannerScale` to a position that
re-inflates the code by exactly the lost amount; the two such
positions found (252 with the run, 126 without) both re-lay the HD
block in a way that increases the same-address diff.

## 4. Conclusion / open questions for wave 164

* The run is real and its source order is `0xac,0x8c,0x90,0x94,0x98,
  0x9c,0xa0,0xa4,0xa8`, but making it contiguous is a **raw
  regression** at any 7,072-B shape found (best 5,871 vs 5,705).
  The baseline's broken run is locally optimal for the raw metric.
* The missing 11 `vst1.32` groups (and 18 `strd`) are the real
  residual.  They are the `0x108/0x118/0x128/0x140/0x150/...` groups
  in the common block; the original groups them in ascending order
  with `0x138/0x13c` last.  A source order that reproduces *all* of
  the original's vector groups while keeping the baseline HD block
  is the next lever — but the whole-region reorder measured by w10806
  (6,796 B) is far too small, so it must be paired with a +276-B
  companion.
* Scratch `/dev/shm/w10907/`: `chunks.py` (constructor statement
  parser), `mk.py`/`mkperm.py`/`exp.py` (permutation + size harness),
  `exper.py` (size/code/pool/store census), `sweep*.py` (eventBanner
  position sweeps), `sA..sG.json` (shape specs), `vA.cpp`/`v.cpp`
  (built variants), `orig.s`/`ours.s` (full disassembly).
