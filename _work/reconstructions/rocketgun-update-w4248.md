# `RocketGun::update(int)` — pass 6 (w4248, wave 92)

**Status:** research note, 2026-11-03. Worktree `wt/w4248`, branch `wave92-w4248`,
base `b379d00d`. **No source change landed**: every faithful source shape tried
this pass scores <= 86.9 %. Tree clean, gate unchanged.

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**. See
`rocketgun-update-w4216.md` (pass 5), `rocketgun-update-w4111.md` (pass 4),
`rocketgun-update-w4011.md` (pass 3), `rocketgun-update-w3831.md` (pass 2) and
`rocketgun-update-86-9-1908-b_w3811.md`.

This pass adds one new piece of evidence the earlier notes do not have — the
**`strd` is a post-RA `ARMLoadStoreOptimizer` merge, not a pre-RA pair** — and
a fresh ~70-variant grid aimed at the handoff's "make the fade reload a genuine
spill reload" angle. It also re-confirms the gate and the RA cascade.

## 1. Baseline (NDK r18b clang 7.0.2, this worktree)

```
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

(`byte`/`linked` are 4499 here, not the 4498 quoted in the wave-92 prompt: the
base already contains `8f90656c StarMap::draw`, which raised the count. The
ratchet baseline agrees with the fresh build, so 4499 is the real current
number.)

`verify.py --show _ZN9RocketGun6updateEi`: 1908 B both sides, 635 vs 636
instructions, **86.9 %**. Every other `RocketGun` function is byte-exact, so the
TU's source style is already faithful; only `update` is off.

Frame slots / registers (re-confirmed):

| value | original | ours |
| --- | --- | --- |
| `elapsed` | `r4` until launch, then `[sp,#16]` | `r5` until preheader, then `[sp,#12]` |
| `this` | `sl`, spilled `[sp,#20]` | `sl`, spilled `[sp,#20]` |
| `player` | `r5` | `r4` |
| `s` (`Sinf` result) | `[sp,#8]` | kept in `r6` |
| `c` (`Cosf` result) | `[sp,#12]` | `[sp,#8]` |
| `trailMatrices` local | none (6x `ldr [sl,#216]`) | `[sp,#16]` |
| `unused` anchor | none | `[sp,#28]` |
| `dt` | `[sp,#24]` | `[sp,#24]` |
| `i*60` trail cursor | `r4` (`adds r4,#60`), saved in `r9` | `r9` (`add.w r9,r9,#60`) |
| `i*12` | `fp` | `fp` |
| `i` | `r8` | `r8` |

The loop body is otherwise instruction-for-instruction identical.

## 2. New: the `strd` is a post-RA load/store merge

The original's launch-block spill is one instruction:

```
15edea:  e9cd 4a04   strd  r4, sl, [sp, #16]      ; Rt=r4, Rt2=r10, imm=4 -> offset 16
```

Ours is two:

```
15edda:  9503        str   r5, [sp, #12]
15eddc:  f8cd a014   str.w sl, [sp, #20]
```

`ARMBaseInstrInfo::storeRegToStackSlot` only emits `STRD` for the 8-byte
`GPRPairRegClass` (a `long long`/`double` in two GPRs), so `strd r4, sl` cannot
come from the spiller. It comes from `ARMLoadStoreOptimizer`:

* `ARMTargetMachine.cpp` adds `createARMLoadStoreOptimizationPass()` both in
  `addPreRegAlloc()` (with `pre-register alloc = true`) and again in
  `addPreSched2()` (post-RA).
* The pre-RA instance merges two adjacent `STRi12`s into `STRD` **and adds
  `RegPairEven`/`RegPairOdd` allocation hints** (`ARMLoadStoreOptimizer.cpp`
  ~line 2309), so a pre-RA merge would force *consecutive* registers.
* The original's pair is `r4`/`r10` — non-consecutive. Therefore the merge
  happened **post-RA**, where no hint is added.

Consequence: the merge only fires when the two spills are (a) adjacent in the
instruction stream and (b) to **adjacent frame slots**. The original has
`elapsed` at `[sp,#16]` and `this` at `[sp,#20]`, so the merge fires. Ours has
`elapsed` at `[sp,#12]` and `this` at `[sp,#20]`, so it cannot fire. This is the
source of the 635-vs-636 instruction-count difference, and it is a *downstream
consequence* of the register/slot colouring, not an independent knob: any
variant that moves `elapsed` to `[sp,#16]` also changes the frame/colouring.

## 3. RA cascade (re-confirmed with `-debug-only=regalloc`)

Self-built LLVM 7.0.0 at `/tmp/llvm_android_702b/build2/bin/clang++` reproduces
the NDK baseline exactly (636 insns, `elapsed` r5, cursor r9, frame 160).

```
selectOrSplit rGPR:%29  [16r,9648B:0)[9840B,15104B:0)  weight:9.400839e-04
assigning %29 to $r4                       # elapsed -> r4 (first, size 14894)
selectOrSplit GPR:%28   ... weight:1.479491e-02
assigning %28 to $r5                       # this -> r5
selectOrSplit GPR:%247  ... weight:8.627993e-03
assigning %247 to $r6                      # dt -> r6
selectOrSplit GPRnopc:%251 [9312r,...)  weight:8.566814e-03   # &stack.4 (sret)
should evict: %29 ... weight:9.400839e-04
evicting $r4 interference: Cascade 1
assigning %251 to $r4
selectOrSplit GPRnopc:%252 ... weight:9.612001e-03            # &stack.3
evicting $r4 interference: Cascade 2
assigning %252 to $r4 ; assigning %251 to $r9
...
selectOrSplit rGPR:%385 [9472r,...) weight:1.493983e-02       # i*60 cursor
should evict: %252 ... / should evict: %251 ...
evicting $r9 interference: Cascade 11
assigning %385 to $r9
```

`this` (`%28`) is later evicted from `r5` by a rematerialised sret address
`%413` (weight 8.5e-2, Cascade 20), which is why it ends in `sl` in both
binaries. The single decision that differs from the original is step 4:
`%251` evicts `elapsed` from `r4`.

Weight formula (`CalcSpillWeights.h`):
`weight = UseDefFreq / (Size + 25*InstrDist)`. For `%29`, `UseDefFreq ~= 14.23`
and `Size = 14894`, giving `9.4e-4`. To beat `%251`'s `8.57e-3` the interval
must shrink to ~1560 slots (or the use frequency must ~9x). The only in-loop
use of `elapsed` is `trailTimers[i] - elapsed` in the fade block (block
frequency 9.69); it is what keeps the second range `[9840B,15104B)` alive.

## 4. Variant grid tried this pass

Fast oracle: compile one TU with the exact NDK flag set, replace the RocketGun
object in the link, and score only this function (~0.6 s/variant). Scripts in
`/dev/shm/w4248`: `cc.sh`, `check.sh`, `gen*.py`, `base.cpp` (pristine copy).

All of the following scored **86.9 %** (i.e. identical code, the copy coalesced
away): a plain `const int frameTime = elapsed;` copy before the loop / before
`dt` / at the top of the function / after the launch block / inside the loop /
inside the fade block / inside the fade condition; the same copy used in the
launch block's recursive `this->update(...)` and/or the fade block; a
`const int &` reference; a one-element `int[1]`; a one-field struct; `(int)`
casts; `elapsed ^ 0`, `elapsed << 0`, `__builtin_bswap32(__builtin_bswap32(
elapsed))`; `(*timers)[i] -= elapsed`; non-const `s`/`c`; swapping the `Sinf`
and `Cosf` computations; `s * 2.0f` / `c * 2.0f`; `dt` declared `float` /
non-const / inside the loop; `i` as `int` / `unsigned` / `size_t` / `i++`;
`this->gun->update(elapsed)` instead of `Gun &g`; removing the `gun = this->gun`
member assignment; all 6 permutations of the `dt`/`unused`/`trailMatrices`
declarations; `trailMatrices` as `auto` / `*const`; `goto`-free restructure of
the `ahead.z == 50000.0f` block.

Variants that changed the code but did **not** improve the score:

| variant | pct | note |
| --- | --- | --- |
| baseline | **86.9** | 635/636 |
| no `unused` anchor | 85.8 | 635/636 |
| no `padx` anchor | 86.7 | 635/636 |
| `trailMatrices` local removed | 57.6 | 626 insns, `this` -> fp |
| `trailMatrices` local moved into loop | 59.7 | `elapsed` -> r4, `this` -> r9 |
| `trailMatrices` used for `data()[i]` too | 85.7 | |
| `volatile int frameTime = elapsed` + fade | 48.2 | 642 insns |
| `volatile int unused` used in fade | 52.9 | |
| `volatile float s` / `volatile float c` | 76.1 / 83.8 | |
| `volatile float s, c` | 76.4 | |
| `volatile int pad12` added (any position) | 75.6 | frame 1912 |
| second volatile anchor | 75.6 | frame 1912 |
| `volatile long long` anchor | 75.9 | frame 1912 |
| `volatile double` anchor | 75.4 | frame 1916 |
| `elapsed` via `volatile const int *` | 52.0 | |
| `s`/`c` swapped | 45.2 | |
| `dt` as `double` | 84.6 | |
| count cached in a local | 74.9 | |
| `ahead` re-declared inside the loop | 73.2 | |
| `(dt * 0.5f)` (one `operator*`) | 53.7 | 1808 B, 619 insns |
| fade condition reordered | 86.4 | |
| `timers[i] > 0` first | 67.1 | |

Per-TU LLVM flags (added to the RocketGun compile only):

| flag | pct |
| --- | --- |
| `-likely-branch-weight=3` / `=5` | 84.6 (635/635) |
| `-likely-branch-weight=7..63` | 86.9 |
| `-unlikely-branch-weight=3` | 84.6 |
| `-unlikely-branch-weight=5..63` | 70-71 |
| `-split-spill-mode=size` | 86.9 |
| `-tail-dup-size=0` | 86.9 |
| `-disable-block-placement` | 75.0 |

No flag or source shape moved the cursor to `r4` while keeping `this` in `sl`.

## 5. Why the "genuine spill reload" angle does not bite

The fade reload is already `ldr r2, [sp,#12]` (ours) / `ldr r2, [sp,#16]`
(original) at the use — both are genuine spill reloads in the final code. The
loop-spanning range `[9840B,15104B)` is the **pre-RA** live interval, and its
size is what feeds the weight formula; the spiller's reload placement does not
change it. To make `elapsed` die before the loop in the pre-RA IR, the in-loop
`trailTimers[i] - elapsed` use would have to refer to a *different* SSA value.
Every C++ copy of `elapsed` is coalesced back into `%29` (table above), and
forcing a real memory round-trip (`volatile`, array, struct, pointer) adds
instructions and shifts the whole frame, dropping to 48-53 %.

## 6. Root cause / what remains

The residual is one `RegAllocGreedy::shouldEvict` comparison:

* `%29` (`elapsed`, size 14894, weight 9.4e-4) is allocated first and takes r4;
* `%251` (hoisted `t2ADDri %stack.4, 0`, weight 8.57e-3) evicts it and takes r4;
* `%252` evicts `%251`; the `i*60` cursor is processed last and can only take r9
  because evicting `%252` from r4 is "cost too high";
* the original keeps `elapsed` in r4 until it dies at the `dt` conversion, so r4
  is free for the cursor, and the post-RA load/store optimizer merges the
  `elapsed`+`this` spills into one `strd`.

To match, a source shape must make `%29`'s interval end before the preheader
(`%251` is defined at 9312; the last pre-loop use is the anchor at 9232) **and**
keep `this` in `sl` **and** keep the 160-byte frame. The pre-RA interval is
`[16,9648) U [9840,15104)`; the second range is the in-loop fade use, and it
cannot be split off by any source-level copy because the coalescer merges every
copy of a value that is live-in to the loop. No source-level way was found to
break that coalescing without adding instructions.

One avenue remains untried: a source shape in which the five MachineLICM-hoisted
`t2ADDri %stack.N, 0` vregs are *not* stack-colour-merged with the second
cluster's temporaries (the `-no-stack-coloring` experiment shows this is the
right lever, but it changes the frame to 592 and scores 48.4 %).

## 7. Reproduction

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

# RA trace (self-built LLVM 7.0.0)
/tmp/llvm_android_702b/build2/bin/clang++ ... -mllvm -debug-only=regalloc \
  -mllvm -filter-print-funcs=_ZN9RocketGun6updateEi -c src/game/weapons/RocketGun.cpp

# fast single-function oracle used for the grid
/dev/shm/w4248/check.sh <variant.cpp> <tag>
```

Scratch lives in `/dev/shm/w4248` (`base.cpp`, `cc.sh`, `check.sh`,
`gen1.py`..`gen17.py`, `v_*.cpp`, `ra_base.log`, `base_show.txt`); none of it is
in the repo.
