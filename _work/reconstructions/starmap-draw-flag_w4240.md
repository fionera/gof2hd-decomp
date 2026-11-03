# StarMap::draw — close the 15-byte residual; the last branch-weight override is irreducible (w4240)

Status: code change landed, 2026-11-03 (wave 92). Worktree `wt/w4240`, branch
`wave92-w4240`, base master `b379d00d` (HEAD at start: `34e76bbe`, the HangarWindow
`.ARM.extab` pass). Every number below was measured in this worktree against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's
NDK r18b clang 7.0.2 and the official `tools/verify/verify.py` (`autojunk=False`).
Scratch drivers live in `/dev/shm/w4240` (`measure.py`, `objdiff.py`, `cc.sh`,
`filter_ir.py`, `greedy_direct.txt`, `virt_direct.txt`, `cur_all.txt`, `dir_all.txt`).

Build note (git worktree): configure with
`-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
-DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`.
`FN=... verify-fn` is broken here (`--show` receives no argument); call
`tools/verify/verify.py --original ... --ours ... --objdump ... --show <mangled>` directly.

## TL;DR

1. **The 15-byte `StarMap::draw` residual is closed.** The faithful source is the
   direct loop bound `this->systems->size()` (re-read every iteration) plus an
   unlikely hint on the second loop's bounds check:
   `__builtin_expect(i >= this->systemPositions->size(), 0)`. `draw` is now
   **2644 B, 0 structural diff in the official linked verifier** (was 15 bytes / 10 runs).
   The object-level oracle still reports 99.9% because of a relocation-encoding
   artifact (`blxgt` vs `blgt` for the `drawOnScreenInfo` call); the linked
   instruction is identical. Gate:
   byte_exact 4498 -> **4499**, linked_exact 4498 -> **4499**.
2. **The per-file `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1`
   on `StarMap.cpp` is irreducible with clang 7.0.2.** It is load-bearing for
   *two* sites now: `drawOnScreenInfo`'s `getStationEnumIndex(93) >= 0` branch
   (needs exactly 7:1) and `draw`'s new bounds hint (needs exactly 1:7).
   `__builtin_expect` is the only source-level weight mechanism and its default
   is 2000:1; `__builtin_expect_with_probability` does not exist in this clang.
   No source shape, weight heuristic, or global-flag setting reproduces the
   original. Keep the flag and document it.

## Gate (before -> after this pass)

| metric | before | after |
|---|---:|---:|
| byte_exact | 4498 / 4524 | **4499 / 4524** |
| linked_exact | 4498 / 4524 | **4499 / 4524** |
| size-exact | 4524 / 4524 | 4524 / 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 |
| data_exact | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| avg_match | 99.9 | 99.9 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

Section sizes (ours == original): `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`. The HangarWindow pass (`34e76bbe`)
already made `.ARM.extab` size-exact; the task brief's `0x26290 (-8)` was stale.
`_ZN7StarMap4drawEv` and `_ZN7StarMap16drawOnScreenInfoEib` are both 0-diff in
`verify --show`.

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

Two independent source facts are encoded:

1. the loop bound **re-reads** `this->systems` (`this->systems->size()`),
   corroborated by `StarMap::init` (lines 310, 334) which uses the same spelling;
2. the `i >= this->systemPositions->size()` continue edge is an **unlikely
   branch** (1:7), expressed with the file's existing
   `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` override.

Both are required. The direct bound alone gives 2636 B / 93.3%; the hint alone
on the `Array& systems` form gives 2644 B / 98.9%; together they give 2644 B /
0 diff.

## 2. The 15-byte residual (before)

`_ZN7StarMap4drawEv` @ `0xc90f4`, 2644 B, **15 differing bytes / 10 runs**:

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
sl  = 0 (high word of the int64 101), later &Globals::Canvas, then 0 again
[sp,#16] = &this->scratchVector (entry spill, reloaded into fp at the preheader)
[sp,#20] = &Globals::Canvas (entry spill, reloaded into sl inside the pulse block)
```

The base source (`Array<SolarSystem *> &systems = *this->systems;`) gives:

```
fp  = &Vector temp (sp+136)
r8  = this->systems (hoisted once)
sl  = 0
[sp,#16] = &this->scratchVector (spilled)
```

## 3. Why the direct form alone is 8 bytes short

`for (uint32_t i = 0; i < this->systems->size(); i++)` restores the
`c94d6: ldr r0,[r4,#84]; ldr r0,[r0,#0]; cmp r6,r0; bcc` header and the whole
pulse block (`c9454..c94ba`), but the function shrinks to **2636 B (-8)**:
`Globals::Canvas` is hoisted into sl and the 64-bit zero moves to fp, so the
two in-loop Canvas reload instructions (`ldr.w sl,[sp,#20]` + `mov.w sl,#0`)
are hoisted away.

Root cause is post-RA MachineLICM. `%stack.21` is the spill slot holding
`&Globals::Canvas`; the pulse block reloads it with
`renamable $r10 = t2LDRi12 %stack.21`. In the base form the 64-bit zero is
also in r10 (=sl), so `PhysRegClobbers.test(r10)` is true and MachineLICM
refuses to hoist the reload. In the direct form the zero is in r11 (=fp), so
r10 is free and the reload is hoisted to the second-loop preheader. The
MachineLICM post-RA guard (`HoistRegionPostRA`, LLVM 7.0.2) is:

```cpp
if ((!HasNonInvariantUse && IsLICMCandidate(*MI)) ||
    (TII->isLoadFromStackSlot(*MI, FI) && MFI->isSpillSlotObjectIndex(FI)))
  Candidates.push_back(CandidateInfo(MI, Def, FI));
...
if (!PhysRegClobbers.test(Def) && !TermRegs.test(Def)) { ... HoistPostRA(...); }
```

The fix is therefore to change the block frequencies of the second loop so the
allocator keeps the zero in sl (rematerialised) and gives fp to
`&this->scratchVector`. The `i >= this->systemPositions->size()` continue edge
must be 1:7; the file's existing 7:1 override plus
`__builtin_expect(..., 0)` encodes exactly that. The resulting preheader is
the original:

```
c9338: add.w r8, sp, #136     ; &temp
c9360: ldr.w fp, [sp, #16]    ; &scratchVector  (hoisted reload)
c936a: mov.w sl, #0           ; zero
c9372: movs r6, #0
c9374: b.n c94d6
```

### 3.1 Equivalent hint sites

The 1:7 weight can be attached to any of the three "continue" sub-conditions
that dominate the loop frequency; all three produce 0 diff:

| hint | size | diff |
|---|---:|---:|
| `__builtin_expect(i >= this->systemPositions->size(), 0)` (landed) | 2644 | **0** |
| `__builtin_expect(visible == 0, 0)` | 2644 | **0** |
| `__builtin_expect(visible->size() <= i, 0)` | 2644 | **0** |
| `__builtin_expect(!(*visible)[i], 0)` | 2644 | 2 |
| `__builtin_expect(whole 4-term condition, 0)` | 2644 | 40 |
| `__builtin_expect(whole 4-term condition, 1)` | 2636 | 1714 |

The landed site is the one check unique to the second loop's condition chain
(the first loop is `visible == 0 || visible->size() <= i || !(*visible)[i]`).
Adding the same hint to the first loop changes the second loop's allocation
(2636 B / 1656 diff), so the hint is second-loop-only. The IR metadata is
`!{!"branch_weights", i32 7, i32 1}` on the `i < size` branch.

## 4. The branch-weight override is irreducible

### 4.1 `drawOnScreenInfo` needs exactly 7:1

`_ZN7StarMap16drawOnScreenInfoEib` @ `0xc8354`, 3488 B. The relevant source is
the campaign-mission 120 arm:

```cpp
} else if (Globals::status->getCurrentCampaignMission() == 120 &&
           (stationMode ? (*this->stations)[index]->getIndex() == 93
                        : (bool) __builtin_expect(
                              (int) (*this->systems)[index]->getStationEnumIndex(93) >= 0, 1))) {
    this->iconBuffer[1] = this->keyImageWanted;
}
```

Original site-1 code (`orig_dosi.dis` `c8534..c8546`):

```
c8534: ldr.w r0,[r9,#84]     ; this->systems
c8538: movs  r1,#93
c853a: ldr   r0,[r0,#4]
c853c: ldr.w r0,[r0,sl,lsl#2]
c8540: blx   getStationEnumIndex
c8544: cmp   r0,#0
c8546: bge.n c8600           ; then block, out of line
```

Weight sweep on the real TU (`-likely-branch-weight=N`, `-unlikely=1`):

| N | `drawOnScreenInfo` | allocation |
|---:|---:|---|
| 1..6 | 0xd98 (3480) | `bge` to then; campaign `[sp,#24]`, freelance r6 |
| **7** | **0xda0 (3488)** | **`bge` to then; campaign r6, freelance `[sp,#20]`** |
| 8..2000 | 0xdac (3500) | `blt` to else; campaign r6, freelance `[sp,#20]` |

Only N=7 reproduces the original size and allocation. The natural
`calcZeroHeuristics` ratio is 20:12 (0.625) and gives 0xd98; the default
`__builtin_expect` ratio is 2000:1 and gives 0xdac. The branch is
`icmp sgt i32 %x, -1` (InstCombine's canonical form of `x >= 0`), so the
zero-compare heuristic is 20:12, not 7:1.

### 4.2 No source-level 7:1 mechanism exists in clang 7.0.2

`LowerExpectIntrinsic.cpp` (LLVM 7.0.2) is the only source of
`!prof` branch weights:

```cpp
static cl::opt<uint32_t> LikelyBranchWeight(
    "likely-branch-weight", cl::Hidden, cl::init(2000), ...);
static cl::opt<uint32_t> UnlikelyBranchWeight(
    "unlikely-branch-weight", cl::Hidden, cl::init(1), ...);
```

`__builtin_expect(x, 1)` always writes `(Likely, Unlikely)` = (2000, 1);
`__builtin_expect(x, 0)` writes (1, 2000). `__builtin_expect_with_probability`
does not exist in this clang (verified: "use of undeclared identifier"). The
other `BranchProbabilityInfo` heuristics are fixed ratios (zero/pointer/float
20:12, loop backedge 124:4 or 62:4, cold call 4:64, invoke 1048575:1, none
1:1); none is 7:1. The `.comment` of the original is stock
`Android (4751641 based on r328903) clang version 7.0.2`, i.e. no PGO/profile
data to explain 7:1.

### 4.3 Source shapes tried for `drawOnScreenInfo` (all 2000:1, no flag)

| variant | size | pct |
|---|---:|---:|
| plain `>= 0` | 3480 | 88.4 |
| `(unsigned) x < 0x80000000u` / `x != -1` / `x > -2` / `x + 1 > 0` / `!(x < 0)` | 3480 | — |
| `(x >= 0) && true` / `(x >= 0) \|\| false` | 3480 | — |
| reversed ternary (`!stationMode ? system : station`) | 3480 | — |
| expect whole `else if` condition | 3508 | 81.6 |
| expect `getCurrentCampaignMission() == 120` | 3520 / 3516 | 79.7 |
| expect `stationMode` | 3492 / 3496 | 85.6 |
| expect around station-arm comparison only | 3488 | 93.8 |
| expect around the ternary | 3492 / 3496 | 85.6 |
| split station/system paths | 3516 / 3524 | — |
| nested `if` | 3500 | — |
| `(bool) __builtin_expect((x >= 0) != 0, 1)` | 3500 | 87.3 |
| `__builtin_expect((x >= 0) != 0, 1)` (no cast) | 3488 | 93.8 |
| `__builtin_expect(!!(x >= 0), 1)` (no cast) | 3488 | 93.8 |
| `__builtin_expect(x >= 0, 0/1)` (no cast) | 3488 | 93.8 |
| expect on the mission-116 branch | 3500 / 3504 | 83.4-87.3 |
| expect on the whole mission-116 condition | 3492 / 3500 | 85.7-87.3 |
| all 24 permutations of the 116/120/125s/125n `else if` arms | best 3488 | 91.9 |

The 3488-byte variants are **not** byte-exact: dropping the `(bool)` cast makes
the ternary type `int`, so `&&` normalises both arms through `!= 0`; the
station arm becomes `subs #93; clz; lsrs` instead of `cmp #93; bne`, and the
allocator swaps `campaign`/`freelance`. The target keeps the station arm as a
direct comparison and campaign in r6. The best reordering (116, 125s, 120,
125n) is 3488 B but 91.9% and still has the wrong block layout.

### 4.4 The flag must stay per-file

Applying `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1`
globally (via `GOF2_MATCH_OPT`) was measured in the w4204/w4218 pass:
byte_exact 4498 -> **3633**, linked_exact 4498 -> 4496. The flag must remain
scoped to `StarMap.cpp`; the other TUs' `__builtin_expect` sites need the
default 2000:1.

### 4.5 The new `draw` hint also depends on the flag

The w4225 solution is not a flag removal: without the per-file override,
`__builtin_expect(..., 0)` at the bounds check becomes 1:2000 instead of 1:7,
and the second loop's allocation changes. The flag is therefore now
load-bearing for both `drawOnScreenInfo` (7:1) and `draw` (1:7).

## 5. Variants measured for the `draw` residual

All compiled with the tree's flags (including the 7:1 override) and measured
with `measure.py` (object-level oracle, `autojunk=False`).

| variant | size | pct |
|---|---:|---:|
| current (`Array& systems`) | 2644 | 98.9 |
| direct `this->systems->size()` | 2636 | 93.3 |
| direct + `syspos` ptr/ref local | 2644 | 98.5 |
| `const Array&` / `Array* const` / `const Array*` | 2644 | 98.9 |
| local `uint32_t n = size()` | 2640 | 93.9 |
| `Vector&`/`Vector*`/`volatile` sv variants (assign / reads / both, in-loop or before loop, spanning both loops) | 2636-2660 | 86-93 |
| named `Vector temp` + copy | 2680 | 75 |
| pulse as `* 0.01f` | 2644 | 97.2 |
| pulse with `101LL`/`(int64_t)`/`(uint64_t)`/`101u`/`101ULL`/`long long` locals/two-step | 2636 | 93.3 |
| `volatile int64_t base = 101` | 2648 | 94.0 |
| `static const int64_t base` / global const base | 2636 | 93.3 |
| reorder routes/scratchVector2/SetColor around the second-loop preheader | 2644 | 97.0-97.6 |
| `while` loop | 2636 | 89.5 |
| `for (int…)` / `size_t` / `unsigned` / `(*systems).size()` / `++i` / `i += 1` | 2636 | 93.2-93.3 |
| Canvas `PaintCanvas*`/`*&`/`volatile` local in loop | 2632-2636 | 90-93 |
| alpha/fade local before SetColor | 2636 | 93.3 |
| `-split-spill-mode=size` | 2636 | 93.7 |
| `-regalloc-csr-first-time-cost` 0..8192 | 2644 | 98.9 (no effect) |
| `-spp-rematerialization-threshold` 0..1000 | 2644 | 98.9 (no effect) |
| `-enable-deferred-spilling` | 2636 | 92.8 |
| `-disable-postra-machine-licm` | — | 92.3 |
| `-disable-machine-licm` | — | 56.4 |
| `-regalloc=basic/pbqp/fast` | — | much worse |

The only source shape that closes the residual is the landed direct bound +
`__builtin_expect(i >= this->systemPositions->size(), 0)`.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
# verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

Single-function oracle (compiles the real TU and compares the function section):

```sh
python3 /dev/shm/w4240/measure.py --src /home/debian/GalaxyOnFire2/wt/w4240/src/game/world/StarMap.cpp --fn _ZN7StarMap4drawEv
# {"osize":2644,"usize":2644,"exact":false,"pct":99.9}
# the only oracle diff is blxgt (orig) vs blgt (ours) on the drawOnScreenInfo call;
# the official linked verifier reports 0 structural diff for StarMap::draw.
python3 /dev/shm/w4240/measure.py --src /home/debian/GalaxyOnFire2/wt/w4240/src/game/world/StarMap.cpp \
    --fn _ZN7StarMap16drawOnScreenInfoEib
# {"osize":3488,"usize":3488,"exact":true,"pct":100.0}
python3 /dev/shm/w4240/measure.py --src /home/debian/GalaxyOnFire2/wt/w4240/src/game/world/StarMap.cpp \
    --fn _ZN7StarMap16drawOnScreenInfoEib --strip-bw
# {"osize":3488,"usize":3500,"exact":false,"pct":87.3}
```

## 7. Evidence locations

Scratch (not committed): `/dev/shm/w4240/` — `measure.py` (object-level oracle),
`objdiff.py` (normalised instruction diff), `filter_ir.py` (extracts the
`StarMap::draw` machine IR from `-print-after-all`), `greedy_direct.txt`
(after Greedy RA, direct form), `virt_direct.txt` (after Virtual Register
Rewriter, direct form), `cur_all.txt` / `dir_all.txt` (filtered `-print-after-all`
for the current/direct forms), `orig_draw.dis`, `orig_dosi.dis`,
`dosi_flag.dis`, `dosi_nostrip.dis`, `draw_official_sbs.txt`.

Related committed notes: `_work/reconstructions/starmap-draw-w4204.md`
(15-byte classification, flag sweep), `_work/reconstructions/branch-weight-overrides_w4218.md`
(why three overrides were removed and StarMap's kept),
`_work/reconstructions/starmap-draw-w4225.md` (the direct-bound + hint solution;
this pass re-derived and landed the same change on top of `34e76bbe` and adds
the flag-irreducibility proof), `_work/reconstructions/starmap-mgame-mtw-modstation-w4119.md`
(direct form 2636, Canvas hoist).

## 8. Conclusion

- The 15-byte `draw` residual was a three-way allocator tie between
  `&Vector temp`, `&this->scratchVector`, and the zero/`&Globals::Canvas`
  value. The faithful direct loop bound plus a 1:7 unlikely hint on the
  second loop's bounds check reproduces the original allocation exactly.
- `StarMap::draw` is now 2644 B / 0 diff; `drawOnScreenInfo` remains 3488 B /
  0 diff with the 7:1 override.
- The per-file `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1`
  on `StarMap.cpp` is irreducible with clang 7.0.2: only `__builtin_expect`
  produces branch weights, its default is 2000:1, and only 7:1 reproduces
  `drawOnScreenInfo`'s 0xda0 allocation. The new `draw` hint needs 1:7, so the
  flag is now load-bearing for two sites. Keep it and document it.
- Gate: byte_exact 4498 -> **4499**, linked_exact 4498 -> **4499**; size
  4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.
