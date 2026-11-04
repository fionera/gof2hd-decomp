# Globals::Globals C1/C2 (80.5%, 612 B) — w5314 (wave 105)

Status: research note, 2026-11-04. Worktree `wt/w5314`, branch `wave105-w5314`,
base `835748c18`. **No source change landed** — the w3808..w5106 baseline source
order is still the best measured shape (80.5%). This commit contains only this
note. Everything below is self-contained and was re-derived from the shipped
`.so` this pass with `objdump` (no Ghidra import needed for a 186-instruction
leaf constructor).

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182 matched,
  `2·182/452 = 0.8053097345132744`).
* Gate unchanged: **byte_exact 4500 / linked_exact 4500 / size-exact 4524,
  data 843/843, imports 207, lint CLEAN, drift 0**.
* This pass re-derived, instruction by instruction, **the original's complete
  56-store order and 31-word GOT pool order** (section 2) and established the
  exact remaining delta (section 3):
  1. `movw r6,#583` sits at normalised index **34** in the original (ours:
     `movw r0,#583` at 139);
  2. the `imageFactory` GOT slot is reached through `ldr.w ip,[r0]` /
     `str.w r2,[ip]` (32-bit forms) with a 55-instruction live range; ours is
     `ldr r0,[pc]` / `str r2,[r0]` (16-bit), which is exactly the 4-byte
     shortfall that produces our two trailing `nop`s;
  3. the late zero-init block is scheduled differently.
* New measurements this pass (all negative, section 4): the full 60-token order
  is a local optimum under **insertion** moves as well as swaps (early block,
  settings tail, late block each independently hill-climbed to 80.5); a
  24-worker × 500-eval simulated anneal over the whole order returned the
  baseline; the original's machine store order as source scores 46.9–54.9.
* The single most useful new datum: **`status` is the *only* pool difference.**
  Our pool is the original's with `status` inserted at slot 6. Every attempt to
  move `status` to its true late slot (pool slot 26) loses 2.2–3.1 points of
  body match because the prologue hoist set then changes from
  `{Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler}`
  to `{recordHandler, lastSpace, lastStation, lcmf, lcmfc, options, generator}`
  (Canvas drops out, generator comes in).

## 1. Baseline and gate

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
drift:  0 field(s) across 0 class(es)
```

```text
Globals::Globals()  orig 612B @0xe31e0 | ours 612B @0xe31e0
(length differs: orig 225 vs ours 227 normalised entries)
```

`C1` is a plain alias of `C2`; every measurement below is on `C1` (identical
stream). The two extra normalised entries are the two `nop`s that pad our
4-byte-shorter code body up to the 4-byte-aligned literal pool.

Matched runs of the baseline (`difflib.SequenceMatcher(autojunk=False)`):

```text
34   prefix 0..34 (everything through `ldr.w ip,[r6]`)
61   early pointer block + settings block (orig 35..96)
2+1+1+9+3+1+1+1+1+1+1+3+1+1+5+2+1+1+3+1+1+1+2+1+1+1+2   late-block fragments
14   float-pool halfwords
20+4+4+2   GOT pool words (30 of 31; `status` is the miss)
= 182
```

## 2. Ground truth re-derived this pass (self-contained)

`_ZN7GlobalsC1Ev` @ `0xe31e0`, 612 B. 186 code instructions, then 4 float words
at `0xe33b8` (`0x3f000000, 0x3f000000, 0x3f19999a, 0x3f19999a` = 0.5, 0.5, 0.6,
0.6, loaded by one `vld1.64 {d16-d17}`), then the 31-word GOT pool at
`0xe33c8`.

### 2.1 Original store order (56 stores, machine order)

Resolved through the PIC idiom (`ldr rX,[pc,#N]; add rX,pc; ldr rX,[rX]`) and
the pool table below. `opt:N` are byte offsets into `Globals::options`,
`hints:N` offsets into `Globals::hints`.

```text
Canvas, shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd,
globals, layout, appManager, recordHandler, generator, this+0,
opt:0x14 (vst1: 0.5,0.5,0.6,0.6), opt:0x0c (0x0101),
opt:0x00/0x04 (strd 0.5,0.5), opt:0x08 (0.5),
opt:0x24/0x28 (strd 0.5,1.0), opt:0x2c (0.5), opt:0x0e (1), opt:0x30 (1),
opt:0x10, opt:0x40, opt:0x50, opt:0x39, opt:0x35, opt:0x31,
hints:0x04, hints:0x13, opt:0x3f, opt:0x3b, opt:0x4e, opt:0x4c, opt:0x48,
hints:0x00,
gameLoaded, gameSaving, lastSpace(-1), initMem, lastStation(-1),
lcmfc(0), lcmf(-1), opt:0x54 (resWidth), opt:0x58 (resHeight),
ias, iaw, iat(-1), iapn, items, status, opt:0x60 (0x0100),
galaxy, achievements, imageFactory, ships, this+4 (soundResources)
```

Note the settings tail order (the one the current source gets wrong):

```text
flag39(0x39), int35(0x35), int31(0x31), hints+4, hints+0x13,
flag3f(0x3f), int3b(0x3b), flag4e(0x4e), short4c(0x4c), int48(0x48),
hints+0
```

### 2.2 Original GOT pool order (31 words at 0xe33c8)

```text
Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler,
shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, generator, hints, gameLoaded, gameSaving, initMem,
imageFactory, achievements, galaxy, ias, iaw, status, iat, iapn, items, ships
```

The first 7 are the prologue hoist (one `ldr [pc]` per available low
register). The baseline's pool is this list with `status` inserted at slot 6
(pushing `recordHandler` to 7 and everything else by one) — pool LCS 30/31.
The baseline's pool:

```text
Canvas, lastSpace, lastStation, lcmf, lcmfc, options, status, recordHandler,
shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, generator, hints, gameLoaded, gameSaving, initMem,
imageFactory, achievements, galaxy, ias, iaw, iat, iapn, items, ships
```

### 2.3 The quality block (identical shape, different register)

Original (indices 121–146):

```text
121 vldr  s2,[r1,#68]        ; fv = options->qualityLevel
124 vcmpe.f32 s2,s0          ; fv vs 1.0
126 vmrs
127 vcmpe.f32 s2,#0.0        ; fv vs 0.0
128 ldr.w ip,[r0]            ; &imageFactory  (pool 21)
129 movw  r0,#513             ; resHeight default
130 ldr   r4,[r4]             ; &achievements  (pool 22)
131 it ge / 132 movwge r6,#830
133 vmrs
134 vcmpe.f32 s2,s0
135 it ls / 136 movwls r6,#415
137 vmrs
138 vcmpe.f32 s2,#0.0
139 str   r6,[r1,#84]         ; resWidth
140 it ge / 141 movwge r0,#730
142 vmrs
143 it ls / 144 movwls r0,#365
146 str   r0,[r1,#88]         ; resHeight
```

and the default `movw r6,#583` sits at normalised **index 34**, immediately
after `ldr.w ip,[r6]` (the last use of `r6` as the `lcmfc` GOT address).
Our build emits the identical block with `r0` for both values and the default
`movw` at index 139.

### 2.4 The two 32-bit promotions

Original:

```text
e3324  ldr.w ip,[r0]     ; ip = &Globals::imageFactory   (pool 21)
...
e33a8  str.w r2,[ip]     ; imageFactory = 0   (55 instructions later)
```

Ours:

```text
ldr r0,[pc,#276] ; add r0,pc ; ldr r0,[r0] ; str r2,[r0]
```

`str.w r2,[ip]` is 32-bit only because `ip` is a high register; the allocator
picked `ip` in the original because every low register was live at the store
point. Our code body is 468 B of real instructions + 2 `nop`s = 472 B; the
original is 472 B of real instructions. The 4-byte delta is exactly these two
`.w` promotions.

## 3. Exact remaining diff (baseline)

```text
orig[0:34]   == ours[0:34]        (34)
orig[34]     = movw r6,#583       ours[34:38] = str shipTemplate / ldr / add / ldr
orig[35:96]  == ours[38:99]       (61)
orig[96:98]  == ours[96:98]       (hints+4, hints+0x13)
orig[98:101] = flag3f,int3b,flag4e   ours[99:101] = int3b,flag4e
orig[102]    = int48              ours has it at 104
orig[103]    = hints+0            ours has it at 98
orig[104:]   late block: see 2.1; ours emits
             gameLoaded, gameSaving, initMem, imageFactory(+store),
             achievements(+store), galaxy(+store), ias, lastStation,
             [quality], iat, int48, flag3f, iapn, items, ships, iaw,
             lcmf, soundResources, short60, lcmfc
pool         30/31 (status at slot 6 instead of 26)
```

## 4. What was tried this pass (all negative)

All variants were compiled with the exact repo compile line (NDK r18b clang
7.0.2, `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
-fstack-protector-strong -Oz -g -stdlib=libc++`) and scored with the official
`tools/verify/verify.py` normalisation (`difflib.SequenceMatcher(autojunk=False)`,
`V.reconcile`). A single-TU scorer (compile `Globals.cpp`, disassemble
`.text._ZN7GlobalsC2Ev` from the `.o`, resolve pool words from the
`R_ARM_GOT_PREL` relocations) reproduces 80.5 / 227 / 225 exactly and runs in
~0.3 s/eval.

### 4.1 `status` placement (all 61 positions)

| position | pct |
|---|---:|
| baseline (after `generator`, before `this->field_0x0`) | **80.5** |
| after `generator` +1..+5 | 80.1 |
| after `items` (the original's store slot) | 77.0 |
| before `iat` | 77.4 |
| before `ships` | 77.0 |
| very end | 56.2 |

Moving `status` out of the early block does **not** reproduce the original's
prologue hoist. Measured hoist sets:

```text
baseline (status early):
  Canvas, lastSpace, lastStation, lcmf, lcmfc, options, status, recordHandler
status late:
  recordHandler, lastSpace, lastStation, lcmf, lcmfc, options, generator
status late + Canvas 2nd in the early block:
  Canvas, lastSpace, lastStation, lcmf, lcmfc, options, generator
status late + generator last in the early block:
  Canvas, lastSpace, lastStation, lcmf, lcmfc, options, generator, recordHandler
```

The last one is the closest (78.3): six of the seven hoisted names are right,
but `generator` is inserted at slot 6 and everything shifts by one, so the pool
LCS is again 30/31 — the same as the baseline, at 2.2 points lower body match.

### 4.2 Settings-tail order

Writing the original's machine tail order as source
(`int31, sec_slot1, sec_flag13, flag3f, int3b, flag4e, short4c, int48,
sec_slot0`) makes the settings block match **11/11** (the run grows 61 → 64)
but costs the late block: **79.2** overall. A full insertion hill-climb of the
8-token tail with everything else fixed converges back to the baseline order
at 80.5.

### 4.3 Quality-block placement

Sweep of `QUALITY` over all 61 positions: 80.5 at positions 40–44 (i.e. right
before `initMem`), 79.2 at 51–52, and the `movw r6,#583` hoist only appears at
positions 53–55 (before `ships` / `iaw` / `lcmf`) where the score collapses to
73.0 / 71.7 / 72.1. Confirms w4908's "the hoist costs 7.5–8.8 points".

### 4.4 Independent hill-climbs (insertion moves, not just swaps)

| block optimised (rest fixed) | start | converged |
|---|---:|---:|
| early block (14 tokens) | 80.5 | **80.5** (baseline order) |
| settings tail (8 tokens) | 80.5 | **80.5** (baseline order) |
| late block (19 tokens, incl. QUALITY) | 80.5 | **80.5** (baseline order) |
| late block, settings-tail = original order | 79.2 | **79.2** |

### 4.5 Whole-order simulated annealing (24 workers × 500 evals, T 2.5→0.1)

Top three chains all returned the exact baseline order at **80.5**.

### 4.6 Original machine order as source

| variant | pct |
|---|---:|
| original early order, baseline settings/mid/late | 79.2 |
| original early order + original settings tail | 75.7 |
| original late block in machine store order | 54.9 |
| original late block in pool order | 70.8–71.7 |
| whole function in machine store order | 46.9 |

## 5. Proven vs hypothesis

**Proven (measured or read off the binary):**

1. The original's exact 56-store order, 31-word pool order, per-store widths,
   stored values, and the quality-block shape (section 2).
2. The baseline scores 80.5 with M = 182; the pool is the original's with
   `status` inserted at slot 6.
3. `status` is the *only* pool difference. Moving it late is the only way to
   change that, and it always changes the prologue hoist set for the worse.
4. The `movw r6,#583` hoist and the two `.w` promotions are both reachable
   from source in isolation but each costs more than it gains (section 4.3).
5. The baseline is a basin optimum under insertion moves in every block and
   under a 24-worker SA over the whole order.

**Hypothesis (not proven):**

1. The remaining gap is one coupled register-allocation outcome: the hoist,
   the two `ip` promotions and the late-block store schedule all fall out of
   the same live-interval/pressure decision in the post-RA top-down list
   latency scheduler. The post-RA scheduler preserves the ISel order almost
   exactly for this function (verified with `-mllvm -print-after-all`), so the
   difference must already exist in the *ISel* order — i.e. the original's
   source order is not any order we have tried.
2. The original's early block probably has 12 pointer stores (we have 13 with
   `status`), and its `status` store is in the late block — but the compiler's
   IR-level scheduler produces a different hoist set for every 12-store
   variant tried, so the exact source order that yields
   `{Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler}` is
   still unknown.

## 6. Next hypotheses (ranked)

1. **Find a 12-store early block whose IR-scheduled ISel order starts with
   `Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler`.**
   The IR scheduler currently hoists `recordHandler` first and `generator`
   seventh when `status` is absent. Try reordering only `recordHandler`,
   `generator` and `Canvas` while keeping the settings block fixed; the target
   is the pool prefix, and the body can be re-optimised afterwards.
2. **Make the `imageFactory` store use `ip`.** The only route to removing the
   two `nop`s (worth ~80.5 → ~80.9 on its own). It needs `r0..r7` to be live
   at the store, which in the original is achieved by deferring the store to
   the very end of the block while hoisting the load to just after the first
   `vcmpe`. No source form tried so far separates the load from the store.
3. **Recover the `movw r6,#583` hoist at index 34** (worth ~80.5 → ~81.0).
   w4908 proved it is an ISel-order property: the `t2MOVi16 583` is already at
   machine-IR line 119 right after ISel and neither scheduler moves it. The
   only source lever found is putting the quality block at late position
   53–55, which costs 7.5+ points.
4. If none of the above works, the function is at its fuzzy-metric optimum and
   only a byte-exact scheduler match (different compiler build) would improve
   it.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5314
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC1Ev
```

Annotated original listing (store/load trace with pool indices):
`objdump -d --start-address=0xe31e0 --stop-address=0xe33b8` on the original;
pool word `i` is at `0xe33c8 + 4i` and names the GOT slot
`(add_pc_addr + 4 + word)`.

Fast scorer (scratch, not tracked; ~0.3 s/eval): compile `Globals.cpp` with the
exact compile line to a `.o`, `objdump -dr --section=.text._ZN7GlobalsC2Ev`,
normalise with `tools/verify/verify.py`'s `_pool_bytes` / `normalize`, and map
each pool word to its `R_ARM_GOT_PREL` symbol. It reproduces the official
80.5 / 227 / 225 exactly.
