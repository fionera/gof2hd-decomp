# `Layout::Layout` C1/C2 — wave 190 / worker w13390

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave190-w13390`, base master `bea73c49b` (wave-190; raw **5,256**,
aligned 2,726, 2,520 / 2,552 instructions).

**Landed: one size-exact raw win.**

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,256** | **5,255** (-1) |
| aligned (difflib) | 2,726 | 2,726 |
| size | **7,072/7,072** | **7,072/7,072** |
| instructions | 2,520/2,552 | 2,520/2,552 |
| whole-`.text` diff | 73,250 | **73,249** |
| `.ARM.extab` diff | 606 | 606 (size/base exact `0x1a3b78`, 156,312 B) |

## Landed change

The `field_0x1cc` device chain is written as one ternary instead of an
if/else chain:

```cpp
-    if (Globals::iPadHD)
-        this->field_0x1cc = 36 * 1.40625f;
-    else if (Globals::n9)
-        this->field_0x1cc = 54;
-    else if (Globals::retinaDisplay)
-        this->field_0x1cc = 72;
-    else
-        this->field_0x1cc = 36;
+    this->field_0x1cc = Globals::iPadHD ? 36 * 1.40625f : (Globals::n9 ? 54 : (Globals::retinaDisplay ? 72 : 36));
```

The values are unchanged (verified by the w13385 Unicorn oracle over all 32
flag combinations: zero value diffs).  The chain and the ternary compile to
the same 7,072 B body; the ternary removes one byte of the raw same-address
diff.  This is the same change independently landed by w13372
(`cf8bf10c5`), rebased here onto `bea73c49b`.

## Searches re-run on the current body (no further win)

* `field_0x280` chain -> ternary (`iPadHD ? 0 : (n9 ? 4 : (retina ? 5 : 0))`)
  is compile-proxy-neutral (5,252) and linked-raw-neutral (5,255).
* `field_0x190`, `field_0x20c`, `orbitInfoTextX`, `orbitInfoStationY`,
  `field_0x218`, `field_0x260`, `rewardBoxY2` chain -> ternary are all
  size-fatal or raw-worse:
  `0x190` 5,369/7,088; `0x20c` 5,520/7,088; `orbitInfoTextX` 5,520/7,088;
  `orbitInfoStationY` 5,347/7,048; `0x218` 5,347/7,048;
  `0x260` 5,845/7,072; `rewardBoxY2` 5,471/6,960.
* Exhaustive K=6 window permutations over the early `0x24..0x78` block
  (16,560 compiles, compile-only proxy) found no size-exact variant below
  the base: best 5,253 (= base proxy), next 5,254.
* An early-block if/else chain (HD/n9/else), a four-way chain
  (HD/n9/retina/default), and a full member-initializer list all shrink the
  body (6,828 / 6,960 / 6,884 B), so none is size-exact.

## Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact 4524/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical; 27/27 headers; file size 2,187,328 exact.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13390
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5255 aligned=2726 size=7072/7072 insn=2520/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13390/`
(`proxy.py`, `fastproxy.py`, `search6.py`, `search6.out`, `try_chains.py`,
`gen_four.py`, `gen_initlist.py`, `cmpvals.py`).
