# `RocketGun::update(int)` — wave 114 (w6216)

**Status:** dead end, no source change landed. Worktree `wt/w6216`, branch
`wave114-w6216`, base `d8557f7e2`.

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**; ours
**1908 B / 636 insns**, exact `SequenceMatcher` ratio **93.784422 %**
(the gate reports 93.8).

| | pct | size | insns | gate |
|---|---:|---:|---:|---|
| before (wave-113 shape) | **93.8** | 1908 | 636 | PASS |
| best variant this wave | 93.8 (tie, byte-identical code) | 1908 | 636 | PASS |

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint:   CLEAN ✓
drift:  0 field(s) across 0 class(es)
```

No section/extab change: **27 sections**, file size **2,187,328 B**,
`.text` `0x1337c8` @ `0x69440`, `.ARM.exidx` `0x6f70`, `.ARM.extab`
**0x26298** (all identical to the original). Only this note is added.

---

## 1. Residual (unchanged from wave 113)

The normalised diff is still the single `elapsed` r4↔r5 colouring plus the
three tuned-shape deltas that cancel around it:

1. `mov r4, r1` (orig) vs `mov r5, r1` (ours) at entry; `player` then takes
   the other register (`mov r5, r0` / `mov r4, r0`), and everything
   downstream follows (`strd r4/r5, sl, [sp,#16]`, three `ldr r4/r5,
   [sp,#16]`, `subs r0, r1, r4/r5`, `asrs/lsrs`, `mov r1, r4/r5` ×2).
2. Muzzle block is 4 B shorter in ours (narrow `add r4, sp, #116` instead
   of `add.w r8, sp, #116` + hoisted `ldr.w r6, [sl,#24]`).
3. `dt` conversion: ours does `vmov s0, r5; vcvt.f32.s32 s16, s0` early
   and `vmov r0, s16` at the preheader; the original does
   `vmov s0, r4; vcvt.f32.s32 s0, s0; vmov r0, s0` at the preheader.
   (2)+(3) cancel exactly, so the pre-pool size is equal.
4. `hitSmall` z-arm stores `(*trailTimers)[cur] = 0` before `resetSystem`
   (ours) vs after (orig) — +4 B when made faithful.
5. Cluster block: the `V3 crossBuf` two-step copy emits an extra
   `blx VectoraSERKS1_` + `add r0, sp, #104` (6 B); the original is the
   plain `Vector cross = VectorNormalize(VectorCross(...))` shape.
6. Loop tail: ours `add.w fp, fp, #12` then `adds r4, #60`; orig the
   reverse order (`bcc.w` target off by 8).

The wave-113 conclusion still holds: the faithful source shape (dt late,
z-arm store after `resetSystem`, plain cluster) is **exactly** the original
once `elapsed` sits in `r4`; all size deltas cancel around that one flip.

## 2. New finding: the *second-round* r4 occupants

Wave 113 traced the first-round eviction chain (`%249` evicts `%28` from
`$r4` because `7.92e-03 > 8.70e-04`). This wave found the second half of
the blocker, which survives even if the first-round eviction is avoided:

After `%28` is split into `%409` (early, `[16r,9192r)`) and `%408`
(late), `%409` is queued for a second round. Before it is re-selected at
`ra701.txt:17004`, **seven small early-block temporaries have already been
assigned `$r4` and are never unassigned**:

```
%38  [528r,1472r)   w=3.00e-03
%70  [1520r,1744r)  w=2.43e-03
%80  [2064r,2992r)  w=1.18e-03
%109 [3024r,3424r)  w=1.97e-03
%118 [3568r,3680r)  w=1.85e-03
%126 [3840r,3968r)  w=2.37e-03
%170 [5024r,5184r)  w=4.36e-04
```

All seven live entirely inside `%409`'s `[16r,9192r)` range, so they
interfere with it. `%105` (`[2832r,4096r)`) is the only one that gets
reassigned (`can reassign: %105 from $r4 to $r0`, `ra701.txt:15045`).
When `%409` is finally re-selected, `$r4` interferes and the allocator
takes the next free register, `$r5`.

So a source shape must defeat **both** the first-round LEA eviction and
this second-round crowding of the entry/muzzle region. Note that these
temporaries are exactly the ones the *wave-113 tuned shape* keeps alive
(they are the vector math of the muzzle-flash block), which is why every
"faithful" variant that removes the `crossBuf` copy collapses the whole
allocation instead (`cF` → `elapsed → r9`, 1832 B, 61.2 %).

## 3. Variants measured this wave

Full link + `sodiff`-style score (`/dev/shm/w6216/try.sh`); all ≤ 93.8.

| variant | pct | size | note |
|---|---:|---:|---|
| **baseline** | **93.8** | 1908 | `mov sl,r0 / mov r5,r1` |
| `plaincluster` (`Vector cross = VectorNormalize(VectorCross(..))`) | 61.2 | 1832 | call stream exact |
| `plain_norm` (same, named `cross`) | 61.2 | 1832 | |
| `twostep` (`cross = VectorCross(..); cross = VectorNormalize(cross);`) | 60.0 | 1880 | |
| `twostep_ref` (named `norm`) | 55.5 | 1872 | |
| `zfaith` (store after `resetSystem`) | 88.8 | 1912 | |
| `dtlate` (dt just before the loop) | 88.9 | 1908 | |
| `crossBuf + dtlate` | 88.9 | 1908 | |
| `crossBuf + zfaith` | 88.8 | 1912 | |
| `crossBuf + zfaith + dtlate` | 88.4 | 1912 | |
| `swap_plus` (`velocities*dt*0.5f + positions`) | 49.1 | 1848 | |

Fast entry-register probe (single-TU compile, no link; `(this, elapsed)`):

| variant | entry regs |
|---|---|
| baseline | `('sl','r5')` |
| `__builtin_expect(cond, 0)` / plain `if (cond)` / no first block | `('sl','r6')` |
| `__builtin_expect` on `gun->active` | `('fp','?')` |
| `__builtin_expect` on `ahead.z == 50000` | `('r4','?')` |
| `__builtin_expect` on the `trailMatrices` block | `('r4','r5')` |
| `__builtin_expect` on the fade condition | `('r6','r5')` |
| drop the line-336 `elapsed` use | `('r9','r8')` |
| drop the line-275 `elapsed` use | `('sl','r9')` |
| `dt` folded into `s`/`c` (removes `%245` from the loop) | `('r9','r6')` |
| `dt` folded into `(s+s)*dt` / `(c+c)*dt` | `('r9','r6')` |
| `PlayerEgo *&player` | `('sl','r8')` |
| `ahead` reused for `barrel` | `('r5','?')` |
| `if (gun->active == 0) goto fade;` | `('sl','r5')` |
| `if (this->trailMatrices != nullptr) {...}` inverted | `('r9','r5')` |
| `if (elapsed < 0) return;` in the loop | `('r9','r5')` |

Exhaustive/random searches this wave — **all 0 hits for `elapsed → r4`**:

* `gen9.py`: 1936 combinations of 24 mutations (fade-block forms,
  `fadeTimer` forms, `dt` forms, `ahead` forms, loop counter types,
  `this->gun` vs `gun`, launch-block orders).
* `gen10.py`: 5211 combinations of 31 mutations (muzzle-block statement
  reorderings, `Vector` ctor forms, `scale` algebra, `hitSmall` statement
  order, `currentProjectile` locals, loop-body reorderings, fade-block
  forms).

Every "93.8" tie above is byte-identical code to the baseline.

## 4. Why the obvious levers fail

* `elapsed`'s spill weight is `UseDefFreq/(Size+100)` with `Size ≈ 14848`
  (its live range spans the whole function because of the line-336
  `(*trailTimers)[i] - elapsed` use). To exceed `%249`'s `7.9e-03` it would
  need `UseDefFreq ≈ 119`; it has ≈ 13. Aliasing it (`const int ms =
  elapsed;`, `const int elapsedSaved = elapsed;` before/in the loop) is
  coalesced away and changes nothing.
* Removing `dt` from the loop entirely (`(s+s)*dt` folded into `s`) does
  free `%245`, but the allocator then picks `('r9','r6')` — `this` loses
  `sl`, so the shape is worse, not better.
* `-mllvm -disable-machine-licm` does give `mov r4, r1` (wave 113) but
  restructures the loop (matrix IV spilled at `[sp,#24]`, 1892 B, 48.7 %).
  The LEA hoist cannot be suppressed from source: `t2ADDri %stack.N, 0` is
  trivially rematerialisable and `MachineLICM` hoists it unconditionally.

## 5. Reproduction

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

RA trace: compile the single TU with
`/dev/shm/wf213/llvm701build/bin/clang++` (LLVM 7.0.1; the NDK 7.0.2
clang rejects `-mllvm -debug-only=regalloc`) and the repo's TU flags, then
grep for `assigning %409`, `should evict`, `unassigning %38` …
Scratch for this wave lived in `/dev/shm/w6216` (`base.cpp`, `try.sh`,
`score.py`, `search3.py`, `gen9.py`, `gen10.py`, `ra701.txt`); none of it
is in the repo.
