# StarMap::draw — 15-byte residual solved: direct-form loop + unlikely branch hint (w4225)

Status: **landed**, 2026-11-03 (wave 92). Worktree `wt/w4225`, branch
`wave92-w4225`, base master `b379d00d`. Every number below was measured in this
worktree against `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
with the tree's NDK r18b clang 7.0.2 and the official `tools/verify/verify.py`
(`autojunk=False`). Scratch drivers live in `/dev/shm/w4225` (`fdiff.py`,
`tryflag2.sh`, `ccobj.sh`, `cc.sh`, `dump.sh`, `base_*.txt`, `direct_*.txt`,
`orig_draw.txt`, `ours_draw.txt`, `pa_draw.txt`, `vgt_draw.txt`).

Build note (git worktree): configure with
`-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
-DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`.
`FN=... verify-fn` is broken here (`--show` gets no argument); call
`tools/verify/verify.py --show <mangled>` directly, or use the raw byte differ
`/dev/shm/w4225/fdiff.py`.

## Gate (before -> after this pass)

| metric | before | after |
|---|---:|---:|
| byte_exact | 4498 / 4524 | **4499 / 4524** |
| linked_exact | 4498 / 4524 | **4499 / 4524** |
| size-exact | 4524 / 4524 | 4524 / 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 |
| data_exact | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70` exact, `.ARM.extab 0x26290` (-8).
`_ZN7StarMap4drawEv` is now **2644 B, 0 differing bytes** (was 15 bytes / 10 runs).

## 1. The landed source change

`src/game/world/StarMap.cpp`, second loop of `StarMap::draw`:

```diff
-        Array<SolarSystem *> &systems = *this->systems;
-        for (uint32_t i = 0; i < systems.size(); i++) {
+        for (uint32_t i = 0; i < this->systems->size(); i++) {
             Array<bool> *visible = Globals::status->getSystemVisibilities();
-            if (visible == 0 || i >= this->systemPositions->size() || visible->size() <= i || !(*visible)[i])
+            if (visible == 0 ||
+                __builtin_expect(i >= this->systemPositions->size(), 0) || // lint: arm_builtin
+                visible->size() <= i || !(*visible)[i])
                 continue;
```

Two independent source facts are encoded here:

1. the loop bound **re-reads** `this->systems` (`this->systems->size()`), and
2. the `i >= this->systemPositions->size()` continue edge is an **unlikely
   branch** (1:7), expressed with the file's existing
   `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` override.

Both are required; neither alone is enough (see §4/§5).

## 2. The 15-byte residual (before)

`_ZN7StarMap4drawEv` @ `0xc90f4`, 2644 B, **15 differing bytes / 10 runs**.
Re-measured this pass (w4204 reported 8 runs; the raw byte differ reports 10).

```
+0x247 (1B)  c933b  add.w r8, sp, #136      vs add.w fp, sp, #136
+0x24c (1B)  c9341  mov   r1, r8            vs mov   r1, fp
+0x26c (1B)  c9361  ldr.w fp, [sp, #16]     vs ldr.w r8, [r4, #84]
+0x26e..0x26f (2B)  c9363  (imm/reg of the same ldr)
+0x2b0 (1B)  c93a5  mov   r1, r8            vs mov   r1, fp
+0x2be..0x2bf (2B)  c93b2  mov   r0, fp      vs ldr   r0, [sp, #16]
+0x3e2..0x3e3 (2B)  c94d6  ldr   r0, [r4,#84] vs ldr.w r0, [r8]
+0x3e5 (1B)  c94d9  ldr   r0, [r0, #0]     vs (folded)
+0x8d0..0x8d1 (2B)  c99c4  mov   r0, fp      vs ldr   r0, [sp, #16]
+0x972..0x973 (2B)  c9a66  mov   r0, fp      vs ldr   r0, [sp, #16]
```

Classification (no encoding / scheduling / semantic difference):

| bytes | site | class | cause |
|---:|---|---|---|
| 2 | `c933b`, `c9341` | RA | `&Vector temp` (sp+136) is r8 in the original, fp in ours |
| 3 | `c9361` | RA + source shape | original keeps `&this->scratchVector` (this+0x78) in fp; ours hoists `this->systems` (this+0x54) into r8 |
| 2 | `c93a5`, `c93b2` | RA | `Vector::operator=` source is r8/fp, destination is fp vs `[sp,#16]` |
| 3 | `c94d6` | source shape | original re-reads `this->systems` and `->size()` every iteration; ours reads the hoisted `[r8]` |
| 4 | `c99c4`, `c9a66` | RA | `&this->scratchVector` is fp in the original, spilled to `[sp,#16]` in ours (two animated-route copies) |

The original second-loop allocation:

```
r8  = &Vector temp (sp+136)
fp  = &this->scratchVector (this+0x78)
sl  = 0 (high word of the int64 101), later Canvas, then 0 again
r10/sl is shared: zero -> Canvas -> zero inside one iteration
[sp,#20]   = Globals::Canvas value (loaded once at entry, re-loaded into sl per iteration)
```

Ours (base source, `Array<SolarSystem *> &systems = *this->systems;` bound before the loop):

```
fp  = &Vector temp (sp+136)
r8  = this->systems (hoisted once)
sl  = 0
[sp,#16] = &this->scratchVector (spilled)
```

## 3. The direct loop form is faithful

`for (uint32_t i = 0; i < this->systems->size(); i++)` (no `Array& systems`
local) produces a **byte-identical loop header and a byte-identical pulse
block**:

```
c94d6: ldr r0, [r4, #84]     ; this->systems   (identical)
c94d8: ldr r0, [r0, #0]      ; ->size()
c94da: cmp r6, r0
c94dc: bcc.w c9376
```

and the whole pulse block (`c9454..c94ba`, `ldr.w sl,[sp,#20]` ...
`mov.w sl,#0` ... `blx DrawLine`) is identical.

The direct form is also corroborated by the rest of the TU: `StarMap::init`
(2220 B, byte-exact) uses the same `for (uint32_t i = 0; i < this->systems->size(); i++)`
spelling (src/game/world/StarMap.cpp:310, 334).

But the direct form alone is **2636 B (-8)** and 1656 raw diff bytes:
`Globals::Canvas` is hoisted into sl, the 64-bit zero moves to fp, and
`&this->scratchVector` stays spilled. The 8 missing bytes are exactly the two
in-loop Canvas reload instructions (`ldr.w sl,[sp,#20]` + `mov.w sl,#0`) that
the direct form hoists away.

### 3.1 Root cause: post-RA MachineLICM hoists the `%stack.21` Canvas reload

`%stack.21` is the spill slot holding the `Globals::Canvas` value (stored once in
`bb.7` at `t2STRi12 renamable $r0, %stack.21, 0`). In the pulse block the value
is reloaded with `renamable $r10 = t2LDRi12 %stack.21, 0`.

* Base form (`base_phys.txt`, after Virtual Register Rewriter): the Canvas
  reload is `renamable $r10 = t2LDRi12 %stack.21, 0` (line 678) and the 64-bit
  zero is `renamable $r10 = t2MOVi 0` (line 700). Both live in r10 (=sl), so
  `PhysRegClobbers.test(r10)` is true and **post-RA MachineLICM refuses to
  hoist** the reload.
* Direct form (`direct_phys.txt`): the Canvas reload is also
  `renamable $r10 = t2LDRi12 %stack.21, 0` (line 677), but the zero lives in
  **r11 (=fp)** (`renamable $r11 = t2MOVi 0` in the second-loop preheader,
  line 2832). r10 is therefore not clobbered in the loop, and post-RA
  MachineLICM **hoists** the reload into the second-loop preheader
  (`direct_mlicm.txt` line 240: `renamable $r10 = t2LDRi12 %stack.21, 0` in
  `bb.9`, no debug location = hoisted).

`MachineLICM.cpp` post-RA path (`HoistRegionPostRA`, LLVM 7.0.2):

```cpp
if ((!HasNonInvariantUse && IsLICMCandidate(*MI)) ||
    (TII->isLoadFromStackSlot(*MI, FI) && MFI->isSpillSlotObjectIndex(FI)))
  Candidates.push_back(CandidateInfo(MI, Def, FI));
...
if (!PhysRegClobbers.test(Def) && !TermRegs.test(Def)) { ... HoistPostRA(...); }
```

`%stack.21` is a spill slot, so the reload is always a candidate; the only
guard that fires is `PhysRegClobbers.test(Def)`.

Consequence: the direct form's preheader is
`ldr.w sl,[sp,#20]` (Canvas) + `mov.w fp,#0` (zero), whereas the original is
`ldr.w fp,[sp,#16]` (&scratchVector) + `mov.w sl,#0` (zero).

### 3.2 Pre-RA IR: the base form's `systems` load is what pins r8

`base_early_before.txt` (Before Early Machine LICM) line 247:

```
%12:gpr = t2LDRi12 %90:gprnopc, 84   ; this->systems, in the preheader bb.7
```

`direct_early_before.txt` line 522 (`bb.28`, the loop header):

```
%161:gpr = t2LDRi12 %89:gprnopc, 84  ; this->systems
%162:rgpr = t2LDRi12 killed %161:gpr, 0  ; ->size()
```

So the base form's `Array& systems` local is hoisted at IR level and pins r8;
the direct form has no such value and r8 goes to `&temp`. The remaining
difference is then purely the zero-vs-`&scratchVector` choice for fp.

## 4. The branch-weight witness: `i >= systemPositions->size()` is 1:7

The direct form's spill placement keeps `&this->scratchVector` in
`[sp,#16]` and gives fp to the zero. The original gives fp to
`&this->scratchVector` and rematerialises the zero in sl. The allocator's
choice is driven by the **block frequencies** of the second loop, which are
computed from the branch probabilities.

The natural heuristic weights of the four `continue` checks are not the
original's. The original's frequencies require the
`i >= this->systemPositions->size()` continue edge to be **1:7** (unlikely).
The file's existing per-file override

```
-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1
```

turns `__builtin_expect(cond, 0)` into exactly that 1:7 weight. Adding the
hint to the second loop's bounds check flips the spill placement to the
original's:

```
c9338: add.w r8, sp, #136     ; &temp
c9360: ldr.w fp, [sp, #16]    ; &scratchVector  (hoisted reload)
c936a: mov.w sl, #0           ; zero
c9372: movs r6, #0
c9374: b.n c94d6
```

and the official verifier reports **2644 B, 0 differing bytes**.

### 4.1 Equivalence of the three sub-condition placements

The 1:7 weight can be attached to any of the three "continue" sub-conditions
that dominate the loop frequency; all three produce the same machine code
(0 diff):

| hint | size | diff |
|---|---:|---:|
| `__builtin_expect(i >= this->systemPositions->size(), 0)` (landed) | 2644 | **0** |
| `__builtin_expect(visible == 0, 0)` | 2644 | **0** |
| `__builtin_expect(visible->size() <= i, 0)` | 2644 | **0** |
| `__builtin_expect(!(*visible)[i], 0)` | 2644 | 2 |
| `__builtin_expect(whole 4-term condition, 0)` | 2644 | 40 |
| `__builtin_expect(whole 4-term condition, 1)` | 2636 | 1714 |

The landed site is `i >= this->systemPositions->size()` because it is the one
check **unique to the second loop's condition chain** (the first loop's
condition is `visible == 0 || visible->size() <= i || !(*visible)[i]`). It is
therefore the minimal single-site hint consistent with the rest of the TU.

Adding the same hint to the first loop's `visible == 0` check *changes* the
second loop's allocation (2636 B, 1656 diff), so the hint is second-loop-only.
The IR metadata confirms the weight: `!{!"branch_weights", i32 7, i32 1}` on
`br i1 %230, label %231, label %307` (the `i < size` branch is 7, the
`i >= size` continue is 1).

### 4.2 Why the hint is not an arbitrary anchor

The original binary proves the branch weight: only the 1:7 weight reproduces
the original's register allocation, and the same per-file 7:1 override is
already load-bearing for `drawOnScreenInfo` (w4204 §2). The source already
uses `__builtin_expect` in this file at lines 696 and 975, so the hint is
written in the file's established idiom (`// lint: arm_builtin` waiver,
same-line).

## 5. Variants measured

All variants were compiled with the tree's flags (including
`-mllvm -likely-branch-weight=7`) and measured with `fdiff.py`. `size`/`diff`
are the function size and raw differing-byte count; `preheader` lists the
second-loop preheader `mov`/`ldr` sequence.

### 5.1 Loop-bound spelling (all re-read `this->systems`)

| variant | size | diff | preheader |
|---|---:|---:|---|
| `this->systems->size()` (direct) | 2636 | 1656 | sl=Canvas, fp=0 |
| `this->systems->count` | 2636 | 1656 | sl=Canvas, fp=0 |
| `(*this->systems).size()` | 2636 | 1656 | sl=Canvas, fp=0 |
| `(this->systems)->size()` | 2636 | 1656 | sl=Canvas, fp=0 |
| `(int) this->systems->size()` | 2636 | 1656 | sl=Canvas, fp=0 |
| `size_t i` | 2636 | 1656 | sl=Canvas, fp=0 |

### 5.2 `Vector` local forms (direct base)

| variant | size | diff | preheader |
|---|---:|---:|---|
| `Vector &scratch = this->scratchVector;` before loop 2 | 2636 | 1656 | sl=Canvas, fp=0 |
| `Vector *scratch = &this->scratchVector;` before loop 2 | 2636 | 1656 | sl=Canvas, fp=0 |
| same, declared before loop 1 / at function top | 2636 | 1656 | sl=Canvas, fp=0 |
| same, reads only (`.z`, `.x`, `.y`) | 2636 | 1656 | sl=Canvas, fp=0 |
| same, assignment + reads | 2636 | 1656 | sl=Canvas, fp=0 |
| both `scratchVector` and `scratchVector2` refs | 2636 | 1656 | sl=Canvas, fp=0 |
| function-top `Vector *` / `Vector &` used everywhere | 2436 | 2164 | (no `[sp,#16]`) |

The compiler folds the reference/pointer into `this->scratchVector` at every
use and spills the address; it never keeps it in fp.

### 5.3 Loop-invariant locals that DO take a callee-saved register

| variant | size | diff | preheader |
|---|---:|---:|---|
| `int alpha = this->alpha;` before loop 2 (used for SetColor) | 2640 | 1716 | sl=0, fp=alpha |
| `PaintCanvas *canvas = Globals::Canvas;` in pulse block | 2636 | 1535 | sl=0, fp=Canvas value |
| `PaintCanvas &canvas = *Globals::Canvas;` in pulse block | 2636 | 1535 | sl=0, fp=Canvas value |
| `PaintCanvas *canvas = Globals::Canvas;` before loop 2 | 2632 | 1776 | sl=0, fp=0 |
| `Array<Vector *> *positions = this->systemPositions;` before loop 2, used everywhere | 2648 | 598 | r8=[r4,#404] |
| `positions` before loop 2, used only in animated route | **2636** | **475** | sl=0, r8=temp, fp=positions |

`alpha` and `canvas` both take fp and push the zero into sl, exactly the
behaviour the original has — but they replace `&scratchVector` with a different
value. `positions` is the best raw diff (475 B) because it also restores the
zero→sl and temp→r8 allocation, but it is wrong in the animated route.

### 5.4 `pos_anim_only` — the most informative near-miss

Source shape:

```cpp
Array<Vector *> *positions = this->systemPositions;
// Routes out of the current system pulse.
...
for (uint32_t i = 0; i < this->systems->size(); i++) { ... }   // unchanged
// Animated planned route.
... uses (*positions)[...] instead of (*this->systemPositions)[...]
```

Result: 2636 B, 475 diff bytes. Instruction-level comparison with the original
(`orig_draw.txt` vs `pa_draw.txt`, 936 vs 934 instructions):

* The pulse block (`c9454..c94ba`) is **instruction-for-instruction identical**.
* The preheader differs only at the `&scratchVector` slot: ours has an extra
  `ldr.w fp,[r4,#404]` (hoisted `positions`) where the original has
  `ldr.w fp,[sp,#16]` (`&scratchVector`), and the zero is in sl in both.
* The second-loop `Vector::operator=` destination is `ldr r0,[sp,#16]` in ours
  vs `mov r0,fp` in the original (2 B).
* The animated route uses `[fp,#4]` instead of `ldr.w rX,[r4,#404]` four times
  (4 x 2 B = the whole 8-byte deficit).

So the original's allocation is exactly `pos_anim_only`'s **except** that fp
holds `&this->scratchVector` rather than a hoisted `systemPositions` load, and
the animated route re-reads `this->systemPositions`.

### 5.5 Other source shapes tried (all non-improving)

* `pos_anim_only` + `alpha` / `canvas` / `Vector&` / `Vector*` / function-top
  scratch: 2636-2640 B, 475-1748 diff.
* `pulse`, `fade`, `hidden`, `visible`, `routes`, `i` declared at function
  scope and reused: 2636 B, 1656 diff (no change) or compile failures.
* `Vector temp;` named at function scope and reused for the `Vector(...)`
  temporaries: 2676-2688 B (extra `operator=`).
* `hidden` operand swap / `!(a >= 0)` / `&` instead of `&&`: 2624-2636 B,
  1658-1840 diff.
* `for`/`while`, `int`/`uint32_t`, `i` declared outside the loop: no change.
* Splitting the 4-term `continue` into 1-4 separate `if (...) continue;`
  statements: 2636 B, 1656 diff (no weight change).
* `Vector *sv[1] = { &this->scratchVector };` / pointer-to-pointer / opaque
  aliases: 2636 B, 1656 diff (clang folds the address).

## 6. Flag sweeps (base source, not landable as source)

| flags | size | diff |
|---|---:|---:|
| none | 2644 | 15 |
| `-mllvm -split-spill-mode=size` | 2644 | 425 |
| `-mllvm -split-spill-mode=speed` | 2644 | 15 |
| `-mllvm -enable-deferred-spilling` | 2644 | 129 (address +4) |
| `-mllvm -disable-spill-hoist` | 2644 | 15 |
| `-mllvm -hoist-cheap-insts` | 2644 | 15 |
| `-mllvm -avoid-speculation=false` | 2644 | 15 |
| `-mllvm -sink-insts-to-avoid-spills` | 2648 | 971 |
| `-mllvm -join-liveintervals=false` | 2744 | 2538 |
| `-mllvm -regalloc=greedy` | 2644 | 15 |

`-split-spill-mode=size` on the direct form gives 2636 B / 1308 diff (same
preheader as direct), so it does not fix the zero/fp choice either.

`-mllvm -likely-branch-weight=7` does not change `draw`'s instruction stream
by itself (w4204 §2.4: identical 895-instruction stream at 7:1 and 2000:1);
the *presence* of a `__builtin_expect` site is what changes the block
frequencies. The flag is load-bearing for `drawOnScreenInfo`'s
`getStationEnumIndex(93) >= 0` branch and for the new `draw` site.

## 7. Conclusion

* The faithful loop shape is `for (uint32_t i = 0; i < this->systems->size(); i++)`
  (re-read), corroborated by `StarMap::init`'s identical spelling.
* The direct form gives a byte-identical loop header **and** a byte-identical
  pulse block, but post-RA MachineLICM hoists the `%stack.21` Canvas reload
  because the 64-bit zero took fp instead of sl.
* The original's block frequencies require the second loop's
  `i >= this->systemPositions->size()` continue edge to be 1:7. The file's
  `-likely-branch-weight=7 -unlikely-branch-weight=1` override plus
  `__builtin_expect(..., 0)` encodes that weight; the allocator then keeps
  `&this->scratchVector` in fp, the zero in sl (rematerialised), and `&temp`
  in r8 — exactly the original.
* `_ZN7StarMap4drawEv` is **2644 B, 0 differing bytes**. Gate:
  byte_exact 4498 -> **4499**, linked_exact 4498 -> **4499**; everything else
  unchanged (data 843/843, imports 207, lint CLEAN, drift 0).
* `.ARM.extab` is still `0x26290` (-8). `StarMap::draw`'s extab record moved
  from `0x1ac6b8` to `0x1ac6b4` and its LSDA prel31 word changed from
  `a824eb7f` to `ac24eb7f` (a +4 compensation for the -4 record shift); the
  record is semantically identical. The remaining -8 is unchanged.
