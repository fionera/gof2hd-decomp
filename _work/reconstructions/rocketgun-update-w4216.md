# `RocketGun::update(int)` — pass 5 (w4216, wave 92)

**Status:** research note, 2026-11-03. Worktree `wt/w4216`, branch `wave92-w4216`,
base `b379d00d`. **No source change landed**: every faithful source shape tried
this pass scores <= 86.9 %. Tree clean, gate unchanged (byte 4498 / linked 4498,
size-exact 4524/4524, `.text 0x1337c8`, imports 207, lint `CLEAN ✓`, drift 0).

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**. See
`rocketgun-update-w4111.md` (pass 4), `rocketgun-update-w4011.md` (pass 3),
`rocketgun-update-w3831.md` (pass 2) and
`rocketgun-update-86-9-1908-b_w3811.md` for the earlier analysis.

This pass adds four things the earlier notes did not have:

1. a **per-use spill-weight dump** (`WDBG`) that shows exactly which uses make
   `%251` (the hoisted `&stack.4` sret address) ~9x heavier than `elapsed`;
2. a **correction to pass 4**: the `volatile int unused` anchor is *not* what
   keeps `this` in `sl` — the `Array<Matrix> *trailMatrices` local is;
3. a **48-variant source grid** (anchor × trailMatrices-local × hasGeometry
   expect × loop expect) measured with the fast oracle;
4. a **weight-knob sweep** (`VRW`) that isolates the target: `elapsed` must be
   in `r4` **and** the `i*60` cursor must be in `r4`; either one alone is not
   enough (elapsed-in-r4 alone scores 76.6, below the 86.9 baseline).

## 1. Baseline (NDK r18b clang 7.0.2, this worktree)

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`verify.py --show _ZN9RocketGun6updateEi`: 1908 B both sides, 635 vs 636
instructions, **86.9 %**. Every other `RocketGun` function is **byte-exact 100 %** (constructor
216 B, `setRadar` 608 B, `seekEnemy` 332 B, `~RocketGun` 88 B, …), so the TU's
source style is already faithful; only `update` is off.

The residual is one allocator colouring permutation (established in passes 2-4,
re-confirmed here):

| value | original | ours (baseline) |
| --- | --- | --- |
| `elapsed` | `r4` until launch, then `[sp,#16]` | `r5` until launch, then `[sp,#12]` |
| `this` | `sl`, spilled `[sp,#20]` | `sl`, spilled `[sp,#20]` |
| `s` (`Sinf` result) | `[sp,#8]` | kept in `r6` |
| `c` (`Cosf` result) | `[sp,#12]` | `[sp,#8]` |
| `trailMatrices` local | none (6x `ldr [sl,#216]`) | `[sp,#16]` |
| `unused` anchor | none | `[sp,#28]` |
| `i*60` trail cursor | `r4` (`adds r4,#60`), saved in `r9` | `r9` (`add.w r9,r9,#60`) |
| `i*12` | `fp` | `fp` |
| `i` | `r8` | `r8` |

The launch-block spill is therefore the original's single
`strd r4, sl, [sp,#16]` (elapsed and `this` in adjacent slots) versus ours
`str r5,[sp,#12]` + `str.w sl,[sp,#20]`. Everything else in the loop body is
instruction-for-instruction identical.

## 2. RA trace: the eviction cascade (self-built LLVM 7.0.0 = NDK clang)

`-mllvm -debug-only=regalloc -mllvm -filter-print-funcs=_ZN9RocketGun6updateEi`
(the self-built tree reproduces the NDK baseline exactly: 625 self-counted
insns, `elapsed` `r5`, cursor `r9`, frame 160). Greedy allocation order is
longest-interval-first; the relevant decisions are:

```
selectOrSplit rGPR:%29  [16r,9648B:0)[9840B,15104B:0)  weight:9.400839e-04
assigning %29 to $r4                       # elapsed -> r4 (first, size 14894)

selectOrSplit GPR:%28   [32r,9648B:0)[9840B,15104B:0)  weight:1.479491e-02
assigning %28 to $r5                       # this -> r5

selectOrSplit GPR:%247  [9296r,9648B:0)[9840B,15104B:0) weight:8.627993e-03
assigning %247 to $r6

selectOrSplit GPRnopc:%251 [9312r,9648B:0)[9840B,15104B:0) weight:8.566814e-03
should evict: %29 ... weight:9.400839e-04
evicting $r4 interference: Cascade 1
unassigning %29 from $r4
assigning %251 to $r4                      # hoisted &stack.4 takes r4
queuing new interval: %29 ...              # elapsed re-queued

selectOrSplit rGPR:%29 ...
assigning %29 to $r8

selectOrSplit GPRnopc:%252 [9328r,9648B:0)[9840B,15104B:0) weight:9.612001e-03
should evict: %251 ... weight:8.566814e-03
evicting $r4 interference: Cascade 2
assigning %252 to $r4                      # hoisted &stack.3 takes r4
assigning %251 to $r9                      # &stack.4 -> r9

selectOrSplit GPRnopc:%255 ... weight:1.007480e-02
should evict: %247 ... weight:8.627993e-03
evicting $r6 interference: Cascade 3
assigning %255 to $r6                      # hoisted &stack.7 takes r6
assigning %247 to $r10                     # -> sl

selectOrSplit GPRnopc:%256 ... weight:3.753831e-03
assigning %256 to $r11                     # hoisted &stack.1 (ahead) -> fp

selectOrSplit rGPR:%385 [9472r,9504B:0)[9504B,9648B:1)[9840B,14960r:1)[14960r,15104B:2)
              weight:1.493983e-02          # the i*60 cursor, processed last
should evict: %252 ... weight:9.612001e-03
should evict: %251 ... weight:8.566814e-03
evicting $r9 interference: Cascade 11
assigning %385 to $r9                      # cursor -> r9 (r4 held by %252)
```

`this` (`%28`) is later evicted from `r5` by a rematerialised `%252` address
(`%413`, Cascade 20) and split for `$r10` (= `sl`), which is why `this` ends up
in `sl` in both binaries. The only decision that differs from the original is
step 4: `%251` evicts `elapsed` from `r4`.

## 3. New: per-use spill weights (`WDBG`)

The patched `CalcSpillWeights.cpp` in the self-built tree can print every use
that contributes to an interval's weight
(`LiveIntervals::getSpillWeight = (isDef+isUse) * blockFreq/entryFreq`).
For the baseline source:

`%29` (`elapsed`), size 14894:

| block | use | weight |
| --- | --- | --- |
| bb.0 | `%29 = COPY $r1` | 1.0 |
| bb.0 | `$r1 = COPY %29` (Gun::update arg) | 1.0 |
| bb.16 | `$r1 = COPY %29` (recursive update, ×2) | 0.1953125 each |
| bb.32 | `%234 = t2SUBrr %11, %29` (`fadeTimer - elapsed`) | 0.1571655 |
| bb.34 | `%239 = COPY %29` (`(float) elapsed`) | 1.0 |
| bb.34 | `t2STRi12 %29, %stack.12.ref.tmp47` (**`unused` anchor**) | 1.0 |
| bb.55 | `%373 = t2SUBrr %24, %29` (`trailTimers[i] - elapsed`, in-loop) | **9.6875** |
| | **total** | **14.23** |

`%251` (hoisted `t2ADDri %stack.4.ref.tmp, 0`), size 5598:

| block | use | weight |
| --- | --- | --- |
| bb.34 (preheader) | `%251 = t2ADDri %stack.4.ref.tmp, 0` | 1.0 |
| bb.39 (loop body) | `$r0 = COPY %251` | 31.0 |
| bb.39 (loop body) | `$r1 = COPY %251` | 31.0 |
| bb.51 | `$r0 = COPY %251` | 12.10938 |
| bb.51 | `$r1 = COPY %251` | 12.10938 |
| bb.49 | `$r0/$r1 = COPY %251` (×4) | 3.632812 each |
| | **raw sum** | **101.75** (the `visited` de-dup in `weightCalcHelper` drops it to ~96, which is the 8.566814e-03 in the RA trace) |

`isRematerializable` halves that (`totalWeight *= 0.5`, `CalcSpillWeights.cpp`),
giving `%251` weight 8.57e-3 — about 9x `elapsed`'s 9.40e-4. The loop body
frequency is `268435456 / 8388608 = 32` (LLVM's default loop heuristic,
`BranchProbability(124,128)` = 96.875 % back-edge), which is what makes the two
`$r0/$r1 = COPY %251` uses in bb.39 dominate.

Consequence: **no source shape that only reorders/renames the existing
expressions can flip this comparison.** `elapsed` has one in-loop use at
frequency 9.69; `%251` has two at frequency 31 plus six more. To make
`elapsed` win, either its live interval must end before the loop (so its
`size` shrinks from 14894 to ~1300 and its weight rises ~11x), or `%251` must
not be hoisted / must be given a much lower loop frequency.

## 4. Correction to pass 4: which anchor keeps `this` in `sl`

Pass 4 claimed the `volatile int unused = elapsed` anchor is what puts `this`
in `sl`. That is wrong. Removing the anchors one at a time (fast oracle,
`an.sh`; official scores from `score4.sh`):

| variant | self insns | `this` | `elapsed` | cursor | official |
| --- | --- | --- | --- | --- | --- |
| baseline | 625 | `sl` | `r5` | `r9` (`add.w`) | **86.9** |
| no `unused` anchor | 624 | `sl` | `r5` | `r9` | 85.8 |
| no `trailMatrices` local | 614 | `fp` | spilled (`r1`→stack) | `r0` (spilled) | 57.6 |
| no both | 613 | `fp` | spilled | `r0` (spilled) | 57.5 |
| no `__builtin_expect` on hasGeometry | 625 | `sl` | `r6` | `r9` | 84.6 (635 insns) |

So it is the **`Array<Matrix> *trailMatrices = this->trailMatrices;` local**
(whose load `ldr r0,[sl,#216]` sits in the preheader and whose value is spilled
to `[sp,#16]`) that keeps `this` in `sl`; the `unused` anchor is incidental.
The original has neither construct (it reloads `[sl,#216]` six times), so the
faithful source is the "no both" shape, which scores 57.5 %.

## 5. 48-variant source grid

Generated by `/dev/shm/w4216/gen.py`: `anchor ∈ {present, absent}` ×
`trailMatrices local ∈ {present, absent}` × `hasGeometry expect ∈ {1, none}` ×
`loop expect ∈ {none, 0, 1}`, compiled with the self-built clang and classified
by the entry `mov` / cursor `add`. Full result:

```
anchor tm expect loop  insns this elapsed cursor  frame
  0     0   1     0     625   sl   r5     32:r9   160     <- baseline
  0     0   1     1     633   fp   sl     32:r8   168
  0     0   1     2     625   sl   r5     32:r9   160
  0     0   0     0     625   sl   r6     32:r9   160
  0     0   0     1     633   fp   sl     32:r8   160
  0     0   0     2     625   sl   r5     32:r9   160
  0     1   1     0     614   fp   ?      16:r0   160
  0     1   1     1     632   fp   sl     32:r8   160
  0     1   1     2     644   r4   ?      16:r5   160
  0     1   0     0     647   r4   r6     16:r5   160
  0     1   0     1     628   fp   sl     32:r8   160
  0     1   0     2     644   r4   ?      16:r5   160
  1     0   1     0     624   sl   r5     32:r9   160
  1     0   1     1     632   fp   sl     32:r8   168
  1     0   1     2     624   sl   r5     32:r9   160
  1     0   0     0     624   sl   r6     32:r9   160
  1     0   0     1     632   fp   sl     32:r8   160
  1     0   0     2     624   sl   r5     32:r9   160
  1     1   1     0     613   fp   ?      16:r0   160
  1     1   1     1     631   fp   sl     32:r8   160
  1     1   1     2     643   r4   ?      16:r5   160
  1     1   0     0     646   r4   r6     16:r5   160
  1     1   0     1     627   fp   sl     32:r8   160
  1     1   0     2     643   r4   ?      16:r5   160
```
Columns: `anchor`/`tm` 0 = construct present, 1 = removed; `expect` 0 =
`__builtin_expect(...,1)` present on the hasGeometry check, 1 = removed;
`loop` 0/1/2 = no loop hint / `__builtin_expect(...,0)` /
`__builtin_expect(...,1)`.

**No combination puts `elapsed` in `r4`.** The closest to the original's shape
is the baseline itself. Note that the loop-expect-0 variants (which drop the
loop frequency to ~1 and should make `elapsed` win the weight comparison) move
`this` to `fp` and `elapsed` to `sl` — the whole entry-block colouring changes,
not just the loop.

## 6. Weight-knob experiments (mechanism proof, not landable)

The self-built tree has a `VRW=<vreg>:<factor>` hook in `CalcSpillWeights.cpp`
that multiplies one interval's weight. `%29` = `2147483677`:

| factor | insns | `this` | `elapsed` | cursor | official |
| --- | --- | --- | --- | --- | --- |
| 1 (baseline) | 625 | `sl` | `r5` | `r9` | 86.9 |
| 2-8 | 625 | `sl` | `r5` | `r9` | - |
| **12-15** | 625 | `sl` | **`r4`** | `r9` | **76.6** |
| 25 | 652 | `r4` | ? | `r0` | - |
| 50-1000 | 628 | `r5` | `r4` | `r0` | - |

At factor 12 `elapsed` finally outranks `%251` and keeps `r4` — but the cursor
is still `r9`, and the score *drops* to 76.6 (the `s`/`c`/`trailMatrices`
slots shift). So **`elapsed` -> `r4` alone is necessary but not sufficient**;
the cursor must also take `r4` after `elapsed` dies at the preheader.

`NOEVICT=2147483677` (skip the eviction of `%29`) gives `elapsed` `r4`,
`this` `r5`, cursor spilled (`r0`), 620 insns, official 47.7 — same conclusion.

## 7. MachineLICM experiments

The five hoisted sret addresses are `t2ADDri %stack.N, 0` (ARM
`T2I_bin_ii12rs` sets `isReMaterializable = 1` on the shifted-imm form,
`ARMInstrThumb2.td:769`), so `MachineLICM::IsProfitableToHoist` returns true
unconditionally (`MachineLICM.cpp:1206`). They are spliced into the preheader
at 9312-9440, while the induction-variable PHI copies land at 9472-9488
(`Eliminate PHI nodes` runs after `Early Machine Loop Invariant Code Motion`),
which is the fixed ~160-slot handicap.

A patched `MachineLICM.cpp` that refuses to hoist any instruction carrying a
frame-index operand (`NOFI_HOIST=1`) produces 623 insns with `this` in `r8`,
`elapsed` in `fp` and the cursor in `r0` — worse, not better. Refusing the
hoist alone is therefore also not sufficient; it changes the whole entry/loop
colouring.

`-mllvm -disable-machine-licm` (per-TU) gives `elapsed` `r4` but 666 insns and
48.9 %. `-mllvm -no-stack-coloring` gives the cursor `r4` (`adds r4,#60`) but
frame 592 and 48.4 % (it also breaks the other RocketGun functions).

## 8. Root cause / what remains

The residual is one `shouldEvict` comparison in `RegAllocGreedy`:

* `%29` (`elapsed`) is allocated first and takes `r4`;
* `%251` (`&stack.4`, weight 8.57e-3) evicts it (elapsed weight 9.40e-4) and
  takes `r4`; `%252` then evicts `%251`, which lands in `r9`;
* the `i*60` cursor is processed last, finds `r4`/`r9` occupied and evicts
  `%251` from `r9`;
* the original keeps `elapsed` in `r4` through the preheader (spilling it
  together with `this` as `strd r4, sl, [sp,#16]`), which frees `r4` for the
  cursor and produces the `mov r9,r4` / `mov r4,r9` save/restore pair around
  the cluster-rocket block.

For a source shape to match, it must make `elapsed`'s pre-spill live interval
end before the loop (raising its weight above `%251`'s) **without** moving
`this` out of `sl` and **without** changing the frame. The per-use dump shows
the only in-loop `elapsed` use is `trailTimers[i] - elapsed` (bb.55, weight
9.69); the original reloads that value from `[sp,#16]`, i.e. in the original
IR it is a *spill reload*, not a use of the loop-spanning value. No C++ source
form tried so far (48-combination grid, ~100 variants from passes 2-4,
volatile/pointer copies, `(int) dt`, `int i`, cached count, while/do-while,
named temporaries, `this->gun->`, `Matrix &trail`, …) produces that IR shape
while keeping the rest of the colouring.

## 9. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j8

# side-by-side
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi

# RA trace (self-built LLVM 7.0.0 at /tmp/llvm_android_702b/build2/bin/clang++)
clang++ ... -mllvm -debug-only=regalloc \
  -mllvm -filter-print-funcs=_ZN9RocketGun6updateEi -c RocketGun.cpp

# per-use spill weights (patched CalcSpillWeights, WDBG=1)
WDBG=1 clang++ ... -c RocketGun.cpp

# MachineLICM hoist set
clang++ ... -mllvm -debug-only=machinelicm \
  -mllvm -filter-print-funcs=_ZN9RocketGun6updateEi -c RocketGun.cpp
```

Scratch used this pass lives in `/dev/shm/w4216` (`an.sh`, `gen.py`,
`score4.sh`, `ra_cur.log`, variant `.cpp`s); none of it is in the repo.
