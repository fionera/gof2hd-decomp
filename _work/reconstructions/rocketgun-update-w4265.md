# `RocketGun::update(int)` — pass 7 (w4265, wave 92)

**Status:** research note, 2026-11-03. Worktree `wt/w4265`, branch `wave92-w4265`,
base `b379d00d`. **No source change landed**: every faithful source shape tried
this pass scores <= 86.9 %. Tree clean, gate unchanged.

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**.

Earlier notes: `rocketgun-update-w4248.md` (pass 6), `rocketgun-update-w4216.md`
(pass 5), `rocketgun-update-w4111.md` (pass 4), `rocketgun-update-w4011.md`
(pass 3), `rocketgun-update-w3831.md` (pass 2),
`rocketgun-update-86-9-1908-b_w3811.md`.

This pass contributes (a) the exact pre-RA live intervals and the greedy
eviction arithmetic, (b) the precise **frame-slot** table for both binaries and
the demonstration that the residual `strd` is a *slot-adjacency* consequence,
(c) the `s`/`c` spill asymmetry, and (d) a ~40-variant grid on the
`elapsed`-copy / anchor / `s`-`c` axes. None of it moves the score.

---

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

(`byte`/`linked` are **4499** here, not the 4498 quoted in the wave-92 prompt;
the ratchet baseline agrees with the fresh build.)

`verify.py --show _ZN9RocketGun6updateEi`: 1908 B both sides, 635 vs 636
instructions, **86.9 %**.

The opcode sequences of the two functions are otherwise the same function:
every difference is a register name, a frame offset, or one of the four
source-level anchors described in §3. There is no block-order, scheduling or
semantic difference.

---

## 2. The single root cause: one `shouldEvict` in `RegAllocGreedy`

`elapsed` is vreg `%29`; its pre-RA interval is

```
%29 [16r,9648B:0)[9840B,15104B:0)  0@16r weight:9.400839e-04
```

i.e. live from the entry def to the end of the preheader, and again across the
whole loop (the loop's fade block reads it). `RAGreedy` processes intervals
largest-first (`ENQ ... size=14894 prio=0xE0003A2E`), so `%29` is allocated
first and takes `r4`. The cascade then is:

```
selectOrSplit rGPR:%29  [16r,9648B)[9840B,15104B)  weight:9.400839e-04
assigning %29 to $r4
selectOrSplit GPR:%28   [32r,9648B)[9840B,15104B)  weight:1.479491e-02
assigning %28 to $r5                       # this
selectOrSplit GPR:%247  [9296r,9648B)[9840B,15104B) weight:8.627993e-03
assigning %247 to $r6                      # dt
selectOrSplit GPRnopc:%251 [9312r,9648B)[9840B,15104B) weight:8.566814e-03
should evict: %29 ... weight: 9.400839e-04
evicting $r4 interference: Cascade 1
assigning %251 to $r4
selectOrSplit rGPR:%29 ...                 # requeued
assigning %29 to $r8
...
selectOrSplit rGPR:%386 [9488r,...) weight:2.805497e-02   # i*60 cursor
evicting $r8 interference: Cascade 4
unassigning %29 from $r8
assigning %386 to $r8
```

`%251` is `t2ADDri %stack.4.ref.tmp, 0` (the hoisted address of the
`forward`/loop-temporary stack slot), `%252` is `&stack.3.barrel`, `%255` is
`&stack.7.tmp`, `%256` is `&stack.1.ahead`, `%302` is `&stack.6.up`. They are
all `isReMaterializable` `t2ADDri` with a frame-index operand, so
`MachineLICM::IsProfitableToHoist` returns unconditionally at
`lib/CodeGen/MachineLICM.cpp:1206`:

```cpp
  // Rematerializable instructions should always be hoisted since the register
  // allocator can just pull them down again when needed.
  if (TII->isTriviallyReMaterializable(MI, AA)) { ... return true; }
```

They are hoisted into the preheader (`def 9312..9440`), are live across the
whole loop, and therefore interfere with `%29`.

`shouldEvict` (`lib/CodeGen/RegAllocGreedy.cpp:898`) is a plain strict weight
comparison:

```cpp
  if (A.weight > B.weight) { ... return true; }   // evict B
```

### 2.1 Why `elapsed` can never win on weight

`normalizeSpillWeight` (`include/llvm/CodeGen/CalcSpillWeights.h:35`) is

```
weight = UseDefFreq / (Size + 25 * SlotIndex::InstrDist)      // InstrDist == 4
```

Solving the two printed weights (`Size` from the interval dump):

| vreg | Size | weight | implied UseDefFreq |
| --- | --- | --- | --- |
| `%29` elapsed | 14894 | 9.400839e-04 | 14.10 |
| `%251` &ref.tmp | 5662 | 8.566814e-03 | 49.36 |

`elapsed`'s UseDefFreq is ~14: one use in the loop body (block freq 9.6875),
plus a handful at freq 1. `%251`'s is ~49 because it is passed by address to
~8 calls inside the loop body. To beat `%251` **without shortening the
interval**, `elapsed` would need

```
UseDefFreq > 8.566814e-03 * (14894 + 100) = 128.4
```

i.e. ~13 uses at loop frequency. No plausible source shape does that.

The only way out is therefore **structural**: make `%29`'s interval end before
the first hoisted-address def (9296/9312). The last pre-loop use is the
`dt` conversion at 9216 and the `unused` anchor store at 9232, so an interval
ending there would clear all of `%247` (9296), `%251` (9312), `%252` (9328),
`%255` (9360), `%256` (9376).

That requires the loop's fade use (`(*this->trailTimers)[i] - elapsed`) to be a
*different SSA value*. Every C++ copy of `elapsed` is coalesced straight back
into `%29` (verified: `int/const int/volatile int/int&/const int&` copies at
four positions, `elapsed^0`, `elapsed<<0`, an `int[1]`, a one-field struct, and
a phi built by assigning the same value on both sides of the launch `if` all
leave the interval byte-identical). A real memory round-trip (`volatile`,
array, `&elapsed`) either coalesces away or adds instructions and drops to
48-53 %.

---

## 3. Frame-slot table and the `strd`

Both binaries have a 160-byte frame. The slot contents differ:

| slot | original | ours |
| --- | --- | --- |
| `[sp,#4]` | stack-guard copy | stack-guard copy |
| `[sp,#8]` | `s` (`Sinf` result) | `c` (`Cosf` result) |
| `[sp,#12]` | `c` (`Cosf` result) | **`elapsed`** |
| `[sp,#16]` | **`elapsed`** | `trailMatrices` local |
| `[sp,#20]` | `this` | `this` |
| `[sp,#24]` | `dt` | `dt` |
| `[sp,#28]` | — | `unused` / `padx` anchors |
| `[sp,#144]` | `ahead` | `ahead` |
| `[sp,#156]` | stack-guard | stack-guard |

The launch-block spill is

```
original:  15edea:  e9cd 4a04   strd  r4, sl, [sp, #16]      ; elapsed, this
ours:      15edda:  9503        str   r5, [sp, #12]          ; elapsed
           15eddc:  f8cd a014   str.w sl, [sp, #20]          ; this
```

`strd` here is **not** the spiller: `ARMBaseInstrInfo::storeRegToStackSlot`
only emits `STRD` for the 8-byte `GPRPairRegClass`. It is the post-RA
`ARMLoadStoreOptimizer` (`addPreSched2`) merging two adjacent `STRi12`s. The
pre-RA instance would have added a `RegPairEven` hint and forced consecutive
registers; the original's pair is `r4`/`r10`, so the merge is post-RA and
therefore needs **adjacent frame slots**. Ours spills to 12 and 20, so no merge
and one extra instruction (636 vs 635).

This is a *downstream consequence* of the register colouring: if `elapsed`
kept `r4` the slot assignment would also move (see §4).

### 3.1 The `s`/`c` asymmetry

In the cluster branch the original spills **both** trig results:

```
15f274:  str  r0, [sp, #8]     ; s = Sinf(...)
15f29c:  str  r0, [sp, #12]    ; c = Cosf(...)
...
15f2be:  ldr  r0, [sp, #8]     ; reload s for (s+s)*dt
15f2f4:  ldr  r0, [sp, #12]    ; reload c for (c+c)*dt
```

Ours keeps `s` in `r6` and spills only `c`:

```
        blx  Sinf
        mov  r6, r0            ; s stays in r6
        ...
        blx  Cosf
        str  r0, [sp, #8]      ; c -> [sp,#8]
        ...
        vmov s0, r6            ; s still in r6
```

Because ours has one more free register in that block (r4 is free: `player`
died and `elapsed` is in r5), `s` never spills. That is why `c` lands on slot 8
instead of 12 and why `elapsed` lands on 12 instead of 16. It is the same
root cause as §2 seen from the stack-slot side.

### 3.2 The three source-level anchors are visible in the code

The current source carries three non-original constructs; each is an extra
instruction relative to the original:

```
ours  15edda:  str   r5, [sp, #12]        \  the two spills that the original
      15eddc:  str.w sl, [sp, #20]        /  does with one strd at 15edea
ours  15ef6c:  str   r4, [sp, #28]        ; volatile int padx = 0   (launch branch)
ours  preheader:
      15f170:  str   r5, [sp, #28]        ; volatile int unused = elapsed
      15f172:  ldr.w r0, [sl, #216]       \  Array<Matrix> *trailMatrices =
      15f17a:  str   r0, [sp, #16]        /  this->trailMatrices
```

The original preheader is exactly

```
15f168:  movs r4, #0            ; i*60 cursor
15f16a:  vcvt.f32.s32 s0, s0    ; dt
15f16e:  mov.w fp, #0
15f172:  mov.w r8, #0
15f176:  vmov r0, s0
15f17a:  str  r0, [sp, #24]     ; dt
15f17c:  b.n  15f414
```

with no anchor store and no `trailMatrices` local. The original re-reads
`this->trailMatrices` (`ldr [sl,#216]`) at each use in the loop (6 loads); ours
caches it in `[sp,#16]`.

---

## 4. New evidence this pass (not in earlier notes)

### 4.1 iOS 1.1.4 oracle

`/home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD` (Mach-O armv7) contains
`RocketGun::update` at **0x1ac0d8-0x1ac890**. It is the same source:

* `str r1,[sp,#0x40]` at `0x1ac102`, immediately after `ldr r0,[r6,#8]` and
  before `bl Gun::update` — `elapsed` is spilled at the top of the launch
  block, exactly like the Android original.
* `str r6,[sp,#0x48]` stores `this`.
* `elapsed` is re-read from `[sp,#0x40]` at `0x1ac488/0x1ac48a` (recursive
  `update`), `0x1ac550` (fade), `0x1ac57a` (loop preheader) and `0x1ac834`
  (loop trail-timer fade).
* `dt` is converted at `0x1ac58a` (`vmov d16,r1,r1`) and stored to `[sp,#0x4c]`.
* The loop cursor is `[sp,#0x44]` (`adds #0x3c`), `i*12` is `[sp,#0x50]`
  (`adds #0xc`), `i` is `r8`; `trailMatrices` is re-loaded from `[r6,#0xd8]`
  at every use — **no local**.

So the original source keeps `elapsed` in a stack slot across the loop and
re-reads it. The Android original's `r4`-then-spill is purely RA, not a source
local. The iOS build also confirms there is no `trailMatrices` local and no
anchor in the source.

### 4.2 `-join-liveintervals=false` proves `r4` is reachable

Compiling the *current* source with `-mllvm -join-liveintervals=false` emits
`mov r4, r1` in the prologue (elapsed in `r4`) — so the r4 colouring is
reachable from this IR. But disabling copy joining breaks every other copy in
the function (638 insns, 52.2 %), so it is not a usable lever.

### 4.3 `NOFI_HOIST=1` (no frame-index LICM hoisting) gives 56.9 %

Disabling only the frame-index hoist removes the interference but the RA then
colours the in-loop addresses differently and the function degrades badly. The
hoisted addresses are therefore *needed* for the current match; the original
must have hoisted them too and still kept `elapsed` in `r4`.

### 4.4 `noboth` (faithful: no anchors, no trail local) gives 57.5 %

`noboth` (`this->trailMatrices` used directly, no `unused`, no `padx`) is the
faithful shape; it produces 624 insns and **57.5 %**. Its RA trace is

```
%28 elapsed [16r,9616B)[9808B,15120B) weight:8.731315e-04 -> r4
%27 this    [32r,9616B)[9808B,15120B) weight:1.654269e-02 -> r5
%246 dt     [9264r,9616B)[9808B,15120B) weight:8.086808e-03 -> r6
%250 &ref.tmp weight:8.024683e-03 -> evicts %28 from r4
```

so `elapsed` *does* get `r4` here. `this` then ends in `r11` (fp) instead of
`r10` (sl) because `dt` (`%246`) keeps `r10`; in the current base a later
short-lived vreg (`%307`, weight 1.83e-2) evicts `dt` from `r10`, freeing it
for `this`. That eviction does not happen in `noboth` (`%294`, weight
6.85e-3, is too light). So the two anchors are effectively buying the
`dt`-eviction that puts `this` in `sl`, at the cost of moving `elapsed` to
`r5`.

---

## 5. Variant grid tried this pass

Fast oracle: one TU compiled with the exact NDK flags, object swapped into the
link, single-function score (~0.6 s/variant). Scratch: `/dev/shm/w4265`.

All of these scored **86.9 %** (identical code; the copy coalesced):

* `int/const int/unsigned/long frameTime = elapsed;` at 5 positions (top,
  after `Gun &g`, after `gun = this->gun`, before the launch `if`, before the
  fade `if`), with the loop fade and/or `dt` switched to the copy;
* a phi built by assigning `frameTime = elapsed` on both sides of the launch
  `if`;
* `const int *elapsedPtr = &elapsed;` used in the loop;
* `while` loop instead of `for`;
* `dt` moved to 5 positions (incl. top and inside the loop);
* `s`/`c` declared at function scope (`float s, c;` assigned in the loop);
* `gun = this->gun;` moved after the launch `if`, or replaced by a `Gun *g2`;
* `this->gun` used directly in the fade block;
* `unused` moved after the `trailMatrices` local, or typed `unsigned`/`long`.

Variants that changed the code but did not improve:

| variant | pct | note |
| --- | --- | --- |
| baseline | **86.9** | 635/636 |
| `padx` removed | 86.7 | |
| `unused` removed | 85.8 | |
| `unused` at top / after `Gun &g` / before launch `if` / before fade `if` | 83.4 / 84.8 / 85.3 / 85.0 | |
| `unused` as `float`, `long`, `unsigned` | 84.6 / 86.9 / 86.9 | |
| `unused` after the loop / inside the loop | 85.0 / 70.0 | |
| `trailMatrices` local before launch `if` | 81.1 | 635/635 insns |
| `trailMatrices` local before fade `if` | 81.1 | 635/635 insns |
| `trailMatrices` local removed (`notrail`) | 57.6 | 626 insns |
| `Array<Matrix> *volatile trailMatrices` | 57.0 | 628 insns |
| `dt` at top / before launch `if` / inside loop | 82.1 / 81.7 / 86.9 | |
| loop fade uses `(int)dt` | 52.9 | 643 insns |
| `wasFiring` after the launch `if` | 84.0 | |
| `swapupd` (swap the two `positions +=` statements) | 56.6 | 640 insns |
| `VectorNormalize(cross)` hoisted to a local | 85.1 | |
| `volatile float s, c` at function scope | 75.5 | 640 insns |
| `-mllvm -join-liveintervals=false` | 52.2 | 638 insns, elapsed -> r4 |
| `-mllvm -disable-machine-licm` | 48.9 | 666 insns |
| `-mllvm -enable-local-reassign=true` | 86.9 | |
| `-mllvm -split-spill-mode=size` | 86.9 | |

---

## 6. What remains

The residual is one `RegAllocGreedy::shouldEvict` comparison
(`%29` 9.400839e-04 vs `%251` 8.566814e-03) plus its downstream slot and
`s`/`c` consequences. To match, a source shape must do **all** of:

1. make `elapsed`'s pre-RA interval end before 9296 — i.e. the loop's fade use
   must be a *different* SSA value that survives copy coalescing (no such
   source shape found; every copy of `elapsed` is joined back into `%29`);
2. keep `this` in `sl` (r10), which currently needs the `dt`-eviction that the
   two volatile anchors buy;
3. keep the 160-byte frame;
4. keep `s` and `c` both spilled so `elapsed` lands on slot 16 next to `this`
   on 20 (needed for the post-RA `strd` merge and the 635-instruction count).

(1) and (2)/(4) currently pull in opposite directions: the anchors that put
`this` in `sl` also raise the preheader pressure that evicts `elapsed` from
`r4`.

One avenue remains untried: a source shape in which the five
MachineLICM-hoisted `t2ADDri %stack.N, 0` vregs are not stack-colour-merged
with the second cluster's temporaries. `-no-stack-coloring` shows this is the
right lever but changes the frame to 592 and scores 48.4 %.

---

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

# fast single-function oracle
/dev/shm/w4265/check.sh <variant.cpp> <tag>
```

Scratch lives in `/dev/shm/w4265` (`base.cpp`, `noboth.cpp`, `cc.sh`,
`check.sh`, `checkself.sh`, `fastscore.py`, `gen_search.py`, `ra_cur.log`,
`ra_noboth2.log`, `ra_nojoin.log`, `ORIG.asm`, `OURS.asm`, `ios_update.txt`);
none of it is in the repo.
