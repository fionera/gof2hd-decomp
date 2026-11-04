# Globals::Globals C1/C2 (80.5%, 612 B) — w5106 (wave 101)

Status: research note, 2026-11-04. Worktree `wt/wf106`, branch `wave101-wf106`,
base `3856949ab`. **No source change landed** — the w3808..w5008 baseline source
order is still the best measured shape (80.5%). This commit contains only this
note. Everything below is self-contained; no transcript is required.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182 matched,
  `2·182/452 = 0.8053097345132744`).
* Gate unchanged: **byte_exact 4500 / linked_exact 4500 / size-exact 4524,
  stubs 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
  lint CLEAN, drift 0, `RATCHET PASS`**.
* Region split of the baseline: **early `0:96` = 95/96, late `96:186` = 49/90,
  pool `186:` = 38/39**. The single early miss is the original's
  `movw r6,#583` at normalised index 34 (we emit the `shipTemplate` store
  there and the `movw` at index 139).
* This pass re-derived the annotated original instruction stream from the
  shipped `.so` (store order, GOT-pool order, per-store widths) and ran four
  new families of experiments:
  1. **quality-block position sweep with the `movw #583` index recorded** —
     the hoist (`movw` at index 34) is reproduced at late positions 12/13/14
     but costs 7.5 / 8.8 / 8.4 points;
  2. **moving `status` out of the early block** — the prologue hoist set
     changes from `{Canvas, lastSpace, lastStation, lcmf, lcmfc, options,
     status, recordHandler}` to `{recordHandler, lastSpace, lastStation,
     lcmf, lcmfc, options, generator}` (i.e. `Canvas` is *dropped* and
     `generator` *added*), pool drops 38 → 35;
  3. **settings-tail reorder to the original machine order** (`int31,
     sec_slot1, sec_flag13, flag3f, int3b, flag4e, short4c, int48,
     sec_slot0`) — 79.2, late 46/90 (the early block stays 95/96);
  4. **a 96/96 early block is reachable** — with `recordHandler` moved to the
     end of the early block and `status` moved to the late block, plus the
     quality block at late position 12, the early region is **96/96** and the
     `movw #583` is at index 34, but the pool falls to 31/39 and the score is
     **75.2** (hill-climbed late block: **78.3**, the same basin w4908 called
     "WIN early + WIN late").
* A 500-eval simulated annealing over the full 59-token order (swap moves,
  T = 3.0 → 0.15) and two complete swap hill-climbs of the 19-token late block
  (from 74.3 → 77.4 and from 75.2 → 78.3) found nothing above **80.5**.
* Conclusion unchanged and now sharper: the three remaining deltas
  (`movw r6,#583` hoist, the two Thumb-16→Thumb-32 `ip` promotions for
  `&Globals::imageFactory`, and the late-block store schedule) are one coupled
  register-allocation outcome. The 80.5 basin and the 96/96-early basin are
  mutually exclusive at the source level.

## 1. Baseline and gate

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
drift:  0 field(s) across 0 class(es)
```

```text
Globals::Globals()  orig 612B @0xe31e0 | ours 612B @0xe31e0
(length differs: orig 225 vs ours 227 insns)
```

The two extra normalised entries are the two `nop`s that pad our 4-byte-shorter
code body up to the 4-byte-aligned literal pool. `C1` is a plain alias of `C2`;
every experiment below was scored on `C1` (identical stream).

## 2. Ground truth (re-derived this pass, self-contained)

`_ZN7GlobalsC1Ev` @ `0xe31e0`, 612 B. 186 code instructions, then 4 float
words at `0xe33b8` (`0x3f000000, 0x3f000000, 0x3f19999a, 0x3f19999a` =
0.5, 0.5, 0.6, 0.6, loaded by the single `vld1.64 {d16-d17}`), then the 31-word
GOT pool at `0xe33c8`.

Original **store** order (PIC idiom resolved through `.rel.dyn`; `options:N`
are settings-record byte offsets, `hints:N` the secondary object):

```text
Canvas, shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd,
globals, layout, appManager, recordHandler, generator, this+0,
options:0x14 (vst1: 0.5,0.5,0.6,0.6), options:0xc, options:0x0/0x4,
options:0x8, options:0x24/0x28, options:0x2c, options:0xe, options:0x30,
options:0x10, options:0x40, options:0x50, options:0x39, options:0x35,
options:0x31, hints:0x4, hints:0x13, options:0x3f, options:0x3b,
options:0x4e, options:0x4c, options:0x48, hints:0x0, gameLoaded,
gameSaving, lastSpace, initMem, lastStation, lcmfc, lcmf,
options:0x54, options:0x58, ias, iaw, iat, iapn, items, status,
options:0x60, galaxy, achievements, imageFactory, ships, soundRes
```

Original **pool** order (= machine order of the `tLDRLIT_ga_pcrel` nodes; the
first 7 are the prologue hoist):

```text
Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler,
shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, generator, hints, gameLoaded, gameSaving, initMem,
imageFactory, achievements, galaxy, ias, iaw, status, iat, iapn, items, ships
```

The baseline's pool is the original's with `status` inserted at slot 6 (the
baseline hoists 8 GOT words instead of 7).

The original's quality block (indices 121–146) is two independent `if`s, both
if-converted, and the `resWidth` default is materialised in **`r6`**:

```text
124 vcmpe.f32 s2,s0      ; fv vs 1.0
126 vmrs
127 vcmpe.f32 s2,#0.0    ; fv vs 0.0
131 it ge / 132 movwge r6,#830
133 vmrs
134 vcmpe.f32 s2,s0
135 it ls / 136 movwls r6,#415
137 vmrs
138 vcmpe.f32 s2,#0.0
139 str r6,[r1,#84]      ; resWidth
140 it ge / 141 movwge r0,#730
142 vmrs
143 it ls / 144 movwls r0,#365
146 str r0,[r1,#88]      ; resHeight
```

and the default `movw r6,#583` sits at index 34, immediately after
`ldr.w ip,[r6]` (the last use of `r6` for the `lcmfc` address). Our build
emits the identical block with `r0` for both values and the default `movw`
at index 139.

## 3. New measurements this pass

All variants were compiled with the exact repo compile line (NDK r18b clang
7.0.2, `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
-fstack-protector-strong -Oz -g -stdlib=libc++`) and scored with the official
`tools/verify/verify.py` normalisation (`difflib.SequenceMatcher(autojunk=False)`,
`V.reconcile`). The fast single-TU scorer reproduces 80.5 / 227 / 225 exactly.

### 3.1 Quality-block position sweep (with `movw #583` index)

`q` is the index into the 19-token late block at which the quality block is
inserted (`q=0` is the baseline, quality before the late block).

| q | pct | `movw #583` index |
|---:|---:|---:|
| 0–3 | **80.5** | 139 |
| 4–10 | 79.2 | 143 |
| 11 | 77.9 | 147 |
| **12** | 73.0 | **34 (hoist)** |
| **13** | 71.7 | **34 (hoist)** |
| **14** | 72.1 | **34 (hoist)** |
| 15 | 72.6 | 156 |
| 16–17 | 61.1 | 161 |
| 18 | 74.3 | 162 |

The hoist is reproducible, but only in a late-block shape that loses 7.5+
points elsewhere. This confirms w4908's finding with a finer map.

### 3.2 `status` moved out of the early block

Moving `Globals::status = 0;` from the early block to just after
`Globals::items = 0;` (the original's store position):

```text
pct 77.0   early 90/96   late 49/90   pool 35/39
pool: recordHandler, lastSpace, lastStation, lcmf, lcmfc, options,
      generator, shipTemplate, Canvas, gameText, font, ...
```

The prologue hoist set changes from
`{Canvas, lastSpace, lastStation, lcmf, lcmfc, options, status, recordHandler}`
to `{recordHandler, lastSpace, lastStation, lcmf, lcmfc, options, generator}`:
`Canvas` is dropped and `generator` is added, and `recordHandler` moves to
pool slot 0. The pool therefore gets *worse*, not better.

### 3.3 Settings tail reordered to the original machine order

Source order `int31, sec_slot1, sec_flag13, flag3f, int3b, flag4e, short4c,
int48, sec_slot0` (the original's store order for the settings tail):

```text
pct 79.2   early 95/96   late 46/90   pool 38/39
```

The early block is untouched, but the register allocation of the late block
changes and the late matches fall 49 → 46.

### 3.4 A 96/96 early block is reachable

Early block `shipTemplate, Canvas, gameText, font, fontAlien, fontLangSelect,
rnd, globals, tintR, layout, appManager, generator, recordHandler` (i.e.
`recordHandler` last, `status` moved to the late block), baseline settings
block, baseline late block + `status` appended, quality block at `q=12`:

```text
pct 75.2   early 96/96   late 43/90   pool 31/39   movw #583 @34
```

This is the only measured shape with **both** the full early block and the
`movw` hoist. Complete swap hill-climb of the 19-token late block from here:

```text
pct 78.3   q=12
late = lastSpace, gameLoaded, gameSaving, initMem, imageFactory,
       achievements, galaxy, ias, iaw, lastStation, lcmf, lcmfc, items,
       iat, iapn, status, short60, ships, soundRes
```

which is the same basin w4908 reported as "WIN early + WIN late = 78.3".
Moving `recordHandler` to *after* `appManager` instead of after `generator`
gives 74.3 at `q=12` (pool 29/39); its swap hill-climb reaches 77.4.

### 3.5 Other negative results this pass

| variant | pct | note |
|---|---:|---|
| baseline | **80.5** | reference |
| full-order SA, 500 evals, swap moves, T 3.0→0.15 | **80.5** | returned the baseline |
| settings block in struct/address order | 74.3 | early 86, pool 36 |
| source order = original GOT-load order (3 variants) | 67.7–73.5 | prologue hoist changes |
| original late store order as source | 46.9–59.3 | zero register changes to `r3` |
| `imageFactory`/`achievements`/`galaxy` moved to the end of the late block | 59.3–61.9 | `r3` becomes the zero register |
| `flag3f` moved after `sec_flag13` only (partial tail reorder) | 79.2 | — |
| `status` moved after `items` (see 3.2) | 77.0 | pool 35 |

## 4. What is proven vs still hypothesis

**Proven (measured or read off the binary):**

1. The original's exact store order, store widths, stored values and GOT-pool
   order (section 2).
2. The baseline scores 80.5 with early 95/96, late 49/90, pool 38/39; the
   single early miss is the `movw #583` position.
3. The `movw #583` hoist *is* reproducible from source (quality block at late
   position 12/13/14) but costs 7.5–8.8 points.
4. A 96/96 early block is reproducible (section 3.4), but only with a late
   block that scores 75.2–78.3.
5. No ordering tried here (SA, two late-block swap hill-climbs, the new
   structural variants) beats 80.5. Combined with w4908's 8-chain full-order
   SA and w5008's 20 000 three-move permutations, the baseline is a basin
   optimum of the fuzzy metric, not just a single-move local optimum.

**Hypothesis (not proven):**

1. The remaining gap is one coupled register-allocation outcome: the
   `movw r6,#583` hoist, the two Thumb-16→Thumb-32 promotions
   (`ldr.w ip,[r0]` + `str.w r2,[ip]` for `&Globals::imageFactory`) and the
   late-block store schedule all depend on the same live-interval/pressure
   decision. Recovering the `ip` promotions alone would remove the two `nop`s
   (225 vs 227 normalised entries) and is worth ~80.5 → ~80.9; the hoist alone
   ~80.5 → ~81.0.
2. The original's source order is *not* its machine store order (writing the
   machine order as source changes the prologue hoist and the zero register
   and scores 46.9–59.3). The source order that produced the original is
   therefore still unknown; the pool order and the store order are both
   post-scheduling artefacts.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf106
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC1Ev
```

Scratch harness (not tracked, `/dev/shm/wf106`): `fastscore.py` (single-TU
compile + gold link + official score, ~0.4 s/eval), `trybody.py` (splice a
ctor body into the real source), `gen.py` (token → statement body generator),
`regions.py` (early/late/pool match counts), `norm2.py` (indexed normalised
diff), `annot2.py` (annotated disassembly with GOT/store resolution),
`qsweep2.py` (quality-position sweep with the `movw` index), `sa.py`
(full-order SA), `hc2.py` (late-block swap hill-climb).
