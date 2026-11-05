# `RocketGun::update(int)` — wave 113 (w6115)

**Status:** dead end, no source change landed. Worktree `wt/w6115`, branch
`wave113-w6115`, base `a979ace18`.

Target `_ZN9RocketGun6updateEi`: original **1908 B / 645 insns**; ours
**1908 B / 646 insns**, exact `SequenceMatcher` ratio **93.784422 %**
(the gate reports 93.8).

| | pct | size | insns | gate |
|---|---:|---:|---:|---|
| before (wave-112 shape) | **93.8** | 1908 | 646 | PASS |
| best variant this wave | 93.8 (tie, byte-identical code) | 1908 | 646 | PASS |

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
drift: 0 field(s) across 0 class(es)
lint:  CLEAN ✓
```

No section/extab change: **27 sections**, file size **2,187,328 B**,
`.ARM.extab` **0x26298** (unchanged; the 0x26294 figure in the wave-112 note
is stale). `.rodata` is 0x41c27 vs the original's 0x41b67 (the known
`Status::missionCompleted` rank-table tail). Only this note is added.

---

## 1. Exact residual (normalised instruction diff)

645 vs 646 instructions. The complete set of differences is:

1. **`elapsed` r4↔r5 at entry** — `mov r4, r1` (orig) vs `mov r5, r1`
   (ours). Everything downstream follows: `mov r5, r0`/`mov r4, r0`
   (`player`), `ldr r0, [r5,#0]`/`[r4,#0]`, `strd r4, sl, [sp,#16]` /
   `strd r5, sl, [sp,#16]`, three `ldr r4,[sp,#16]` / `ldr r5,[sp,#16]`,
   `subs r0, r1, r4` / `r5`, `asrs r0,r6,#15` / `asrs r4,r5,#15`,
   `lsrs r0,r6,#31` / `lsrs r4,r5,#31`, `mov r1, r4` / `mov r1, r5` ×2.
2. **Two early `beq.n` targets**: `15edda` (orig) vs `15edd6` (ours).
3. **Muzzle block, 4 B**: orig `add.w r8, sp, #116` (4 B) + hoisted
   `ldr.w r6, [sl,#24]` (4 B) then `mov r0, r6` (2 B); ours
   `add r4, sp, #116` (2 B) then a later `ldr.w r0, [sl,#24]` (4 B).
   Ours is 4 B shorter because `r4` is free.
4. **`dt` conversion placement**: ours `vmov s0, r5; vcvt.f32.s32 s16, s0`
   early (8 B before the mid-function pool) and `vmov r0, s16` at the
   preheader; orig has `vmov s0, r4; vcvt.f32.s32 s0, s0; vmov r0, s0`
   all at the preheader (`0x15f15c`, after the pool). Total size equal.
5. **Three PC-relative pool offsets differ by 4** (`ldr r1,[pc,#604]`,
   `ldr r0,[pc,#436]`, `vldr s0,[pc,#332]`). Verified: they resolve to the
   **same absolute addresses** `15f054/15f058/15f05c` on both sides — only
   the +4 instruction shift differs, so the pool itself is already correct.
6. **`hitSmall` z-arm store order**: ours stores
   `(*trailTimers)[cur] = 0` *before* `resetSystem` (and reuses
   `r1 = gun`, 2 B shorter); orig stores it *after* (`+4 B`).
7. **Cluster block, 6 B**: ours emits an extra
   `blx VectoraSERKS1_` (`Vector::operator=`) + `add r0, sp, #104`
   (the `V3 crossBuf` two-step construction). The original has none — it
   is the plain `Vector cross = VectorNormalize(VectorCross(v, up));`
   shape (`VectorCross` → `sp+128`, `VectorNormalize` → `sp+40`, no copy).
8. **Loop tail order**: ours `add.w fp, fp, #12` then `adds r4, #60`;
   orig `adds r4, #60` then `add.w fp, fp, #12`. `bcc.w` target off by 8.

Everything else (all 69 calls, their order, the launch block, the
`seekEnemy`/homing/cluster/s/c math, the trail block, the fade block) is
instruction-identical.

## 2. Root cause: `elapsed` is the lightest of six loop-invariant values

The residual is a **greedy-register-allocator artefact**. Traced with
`-mllvm -debug-only=regalloc` on a locally built LLVM 7.0.1
(`/dev/shm/wf213/llvm701build/bin/clang++`; 7.0.2's clang rejects
`-debug-only`, but 7.0.1 reproduces the identical initial allocation and
entry `mov r5, r1`).

Preheader MIR (`bb.34.if.end235`) after MachineLICM:

```
9232B  %245:gpr     = VMOVRS %1:spr            ; dt in a GPR
9248B  %249:gprnopc = t2ADDri %stack.4.ref.tmp ; cross temp
9264B  %250:gprnopc = t2ADDri %stack.3.barrel
9296B  %253:gprnopc = t2ADDri %stack.7.tmp
9312B  %254:gprnopc = t2ADDri %stack.1.ahead
9376B  %302:gprnopc = t2ADDri %stack.6.up
```

Eviction chain:

```
%28 elapsed [16r,9568B)[9760B,15072B) w=8.704e-04 -> $r4
%27 this    [32r,9568B)[9760B,15072B) w=1.647e-02 -> $r5
%245 dt     w=7.981e-03 -> $r6
%249        w=7.920e-03: r0-r3 occupied, "$r8 is available at cost 1",
            "Only trying the first 9 regs."
            should evict: %28 (7.920e-03 > 8.704e-04)
            evicting $r4 interference: Cascade 1   -> %28 -> $r8
%250 evicts %249 from $r4 (Cascade 2) -> %249 -> $r9
%253 evicts %245 from $r6 (Cascade 3) -> %245 -> $r10
%254 -> $r11 ; %302 waits
%389 (loop counter i) evicts %28 from $r8 (Cascade 4) ; %28 waits
...
%414 [13416r,14000r) w=1.903e-02 evicts %27 from $r5 (Cascade 19) -> %27 -> $r9
%436 evicts %27 from $r9 (Cascade 21) ; %27 split -> ends in $r10 (sl)
%409 takes $r5 ; %28 finally lands in $r5
```

Why `r8` is not chosen for `%249`: ARM gives `r8..r11` (and `r12`) a
`CostPerUse = 1` (`ARMRegisterInfo.td:81`), while `r0..r7` are 0. The
allocation order is `[r0 r1 r2 r3 r12 r4 r5 r6 r7 r8 r9 r10 r11 lr]`, so
`getLastCostChange` = 9 and `tryEvict(CostPerUseLimit=1)` only considers
`r0..r7`. `r0-r3` are busy, `r5` holds `this` (heavier), `r6` holds `dt`;
`r4` holds `elapsed` and is the only cheap victim.

**Key counting fact:** the loop needs **six** loop-invariant values live
across it (`%245` dt + the five LEAs) but only **five** callee-saved
registers are available (`r6 r8 r9 r10 r11`; `r4`/`r5` are wanted for the
matrix induction variable and `this`). One must be evicted, and
`elapsed`'s spill weight (`UseDefFreq/(Size+50)`, `Size≈14848`) is by far
the smallest. The original instead rematerialised all five LEAs inside the
loop (its preheader is only `vmov/vldr/vldr/movs/vcvt/mov.w/mov.w/vmov/str/b`),
which is exactly what `RAGreedy` does when it cannot find a register.

Why the LEAs are always in the preheader at all:
`isReallyTriviallyReMaterializableGeneric` (`TargetInstrInfo.cpp:912`)
accepts `t2ADDri %stack.N, 0` (operand 0 is the def, `sp` is a constant
physreg, no vreg uses, no load/store), and
`MachineLICMBase::IsProfitableToHoist` (`MachineLICM.cpp` ~1214) returns
`true` unconditionally for trivially rematerialisable instructions
("the register allocator can just pull them down again when needed").
So no source shape can stop the hoist; only the allocator's downstream
decision matters.

Confirmation: `-mllvm -disable-machine-licm` gives `mov r4, r1`
(`elapsed → r4`) but restructures the whole loop (matrix IV spilled at
`[sp,#24]`, 1892 B, 48.7 %).

## 3. Why the faithful cluster shape scores *lower*

The original cluster block is the plain

```cpp
Vector cross = VectorNormalize(VectorCross(velocities[i], upVectors[i]));
positions[i] += cross * (s + s) * dt;
```

(`VectorCross` sret → `sp+128`, `VectorNormalize` sret → `sp+40`, no
`operator=`). That form (`cF`) makes the **call stream identical to the
original** (69 calls, same order, 646 vs 645 insns) but collapses the RA:
`mov r9, r1` (elapsed → r9), `this` stays in `r5` (never evicted), 1832 B,
61.2 %. The extra `blx VectoraSERKS1_` of the `V3 crossBuf` trick is
**load-bearing**: it adds the `%414`-style pressure that pushes `this` out
of `r5` into `sl`. Removing the copy in any other way (variants `a`, `b`,
`e`, `cFout`, `cFref`, `Vector cross; cross = VectorCross(...)`) all lose
the allocation.

## 4. Variants measured this wave (all ≤ 93.8 %)

| variant | pct | size | note |
|---|---:|---:|---|
| **baseline** | **93.8** | 1908 | `mov sl,r0 / mov r5,r1` |
| `dtlate` (dt just before the loop) | 88.9 | 1908 | pool −8 |
| `zfaith` (store after `resetSystem`) | 88.8 | 1912 | pool +4 |
| `zfaith + dtlate` | 88.4 | 1912 | |
| `cF` (plain `Vector cross = VectorNormalize(VectorCross(...))`) | 61.2 | 1832 | call stream exact |
| `cF + dtlate` | 61.4 | 1832 | |
| `cF + zfaith` | 66.6 | 1840 | |
| `cF + zfaith + dtlate` | 61.9 | 1840 | |
| `c3b` (`Vector cross; cross = VectorCross(..); cross = VectorNormalize(cross);`) | 89.0 | 1920 | entry `mov sl,r0 / mov r5,r1` |
| `c3b + dtlate` / `+zfaith` / `+both` | 84.8 / 85.8 / 86.0 | 1920/1924/1924 | |
| `a` (`Vector cross = VectorCross(..);` + inline normalize) | 56.8 | 1872 | |
| `b` (`Vector cross; cross = VectorCross(..);` + inline normalize) | 89.5 | 1916 | canary moves to `[sp,#8]` |
| `e` (`V3` + `cross = VectorNormalize(VectorCross(..))`) | 59.6 | 1880 | |
| `gen40 a/b/c` (named `nn` normalize result) | 55.5/59.6/59.4 | 1872/1884/1896 | |
| `dt_hit` (dt just before the `hitSmall` block) | 93.8 | 1908 | identical code |
| `dt_active` (dt before the `active` block) | 91.3 | 1908 | |
| `dt_muzzle` (dt before the muzzle block) | 93.3 | 1908 | |
| `dt_top` / `dt_after_gun` | 93.3 / 93.3 | 1908 | |
| `dtimplicit` (`= elapsed`) / `dtstatic` | 93.8 / 93.8 | 1908 | identical code |
| `dtvol` (`volatile float dt`) | 43.8 | 1880 | |
| `inline` (no `dt`; `(float) elapsed` at each use) | 88.9 | 1908 | IR-LICM hoists the vcvt, = `dtlate` |
| `while` loop instead of `for` | 46.4 | 1796 | |
| `crossout` (`Vector cross` at function scope) | 55.1 | 1892 | |
| `cFout` (`cross` at function scope + cF) | 55.2 | 1888 | |
| `scswap` (compute `c` before `s`) | 55.7 | 1812 | |
| `lifeinline` (`total - gun->lifetimes[i]` in `s`) | 93.8 | 1908 | identical code |
| `p5` (`c` uses `gun->` not `this->gun->`) | 93.8 | 1908 | identical code |
| `m_geomlocal` (`AEGeometry *geom` after `forward`) | 93.8 | 1908 | identical code |
| `m_geomfirst` (`geom` before `forward`) | 93.2 | 1908 | |
| `m_upfirst` (`up` before `forward`) | 92.8 | 1908 | |
| `dzm_geomfirst` (`dtlate+zfaith+geomfirst`) | 93.2 | 1916 | |
| `ahead` init forms (`Vector(..)`, `{}`, `Vector ahead;`, paren) | 93.8 | 1908 | identical code |
| `player_top` (`player` declared at function scope) | 93.8 | 1908 | identical code |
| `shadow`/`shadow2` (`elapsedMs` alias used in the fade blocks) | 93.8 | 1908 | identical code |
| 48 valid permutations of the cluster-block statement order | ≤ 71.2 | — | base order is best |
| 120 combos {dt early/late/top} × {z tuned/faith} × 5 muzzle × 4 cluster | ≤ 93.8 | — | base wins |
| 24 combos {cluster base/cF/c3b} × {dt early/late} × {z} × {muzzle base/geomfirst} | ≤ 93.8 | — | base wins |

Every "93.8" tie above is **byte-identical code** to the baseline
(verified with a full-precision `SequenceMatcher` ratio of 93.784422).

## 5. Conclusion / next lever

The remaining gap is purely `elapsed`'s colouring. To land it, a source
shape must either

* give `elapsed` a spill weight above `%249`'s ≈ 7.9e-3 — impossible while
  its live range spans the whole function (`Size ≈ 14848`), or
* remove one of the six loop-invariant values so the five LEAs fit in
  `r6/r8/r9/r10/r11` — i.e. get `dt` (`%245`) out of a callee-saved
  register, or drop one of the loop's stack temporaries, or
* make `%249`'s live range long enough (≈ >3400 slots at 3 uses) that its
  weight falls below `elapsed`'s 8.7e-4 — i.e. hoist the cross temp's
  address out of the loop *before* the preheader.

Note that the faithful shape (`dt` late, `trailTimers` store after
`resetSystem`, no cluster copy) is **exactly** the original once
`elapsed` sits in `r4`: the three pre-pool deltas (`dt` −8, `z` +4,
muzzle +4) cancel, and the cluster fix (−6) is post-pool. So a single
allocation flip should take the function to byte-exact; nothing else is
out of place.

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi
```

RA trace: add `-mllvm -debug-only=regalloc` to the TU compile line and
build with `/dev/shm/wf213/llvm701build/bin/clang++` (7.0.1; the NDK
7.0.2 clang rejects `-debug-only`). Scratch for this wave lived in
`/dev/shm/w6115` (`base.cpp`, `try.sh`, `score.py`, `score2.py`,
`norm.py`, `gen2.py`, `gen32`–`gen42.py`, `search4`–`search7.py`,
`dumpra.sh`, `licm.sh`); none of it is in the repo.
