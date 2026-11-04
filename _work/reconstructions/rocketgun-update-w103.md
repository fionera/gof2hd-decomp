# `RocketGun::update(int)` — pass 14 (w103, wave 103)

**Status:** research note, 2026-11-04. Worktree `wt/wf310`, branch `wave103-wf310`,
base `1d2b4398e`. **No source change landed**: ~90 new source shapes this pass all
score <= 86.9 %. Tree clean, gate unchanged (byte 4500, linked 4500, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0).

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**.

Earlier notes: `rocketgun-update-w4910.md` (pass 13, the `shouldEvict` arithmetic and
the register-normalised diff), `-w4812` (pass 11+12, slot-weight model), `-w4710`
(pass 10, frame-slot map), `-w4512`, `-w4412`.

This pass contributes one new hard result: the **exact StackSlotColoring trace** for
the function, which shows the frame layout is a pure function of six spill-slot
weights and that the residual is a *slot-count* problem, not a register-allocation
one (section 2). It also records ~90 new source shapes (section 4), including the
first ones that reach the original's **635-instruction** count without matching.

---

## 1. Baseline (NDK r18b clang 7.0.2, this worktree)

```
cmake --build cmake-build-match --target check -j8
```
```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN9RocketGun6updateEi`: 1908 B both sides, **635 vs 636** instructions, **86.9 %**.

---

## 2. The StackSlotColoring trace (new, exact)

```
/dev/shm/wf310/cc7.sh -mllvm -debug-only=stack-slot-coloring
```

```
********** Function: _ZN9RocketGun6updateEi
Spill slot intervals:
SS#32 [16r,9648B:0)[9840B,15104B:0)  0@x weight:1.131250e+01   <- elapsed
SS#33 [72r,9680r:0)[9840B,15104B:0)  0@x weight:2.000000e+00   <- (low-weight)
SS#34 [9296r,9648B:0)[9840B,15104B:0) 0@x weight:4.774219e+01  <- dt
SS#35 [9248r,9648B:0)[9840B,15104B:0) 0@x weight:3.248438e+01  <- trailMatrices local
SS#36 [1900r,9648B:0)[9840B,15104B:0) 0@x weight:3.674219e+01  <- this
SS#37 [12032r,12704r:0)              0@x weight:7.265625e+00   <- padx anchor

Color spill slot intervals:
Assigning fi#34 to fi#32
Assigning fi#36 to fi#33
Assigning fi#35 to fi#34
Assigning fi#32 to fi#35
Assigning fi#37 to fi#36
Assigning fi#33 to fi#37
```

The colouring order is **exactly weight-descending** (`34, 36, 35, 32, 37, 33` =
47.74, 36.74, 32.48, 11.31, 7.27, 2.00) and each slot gets the next free FI number
`32, 33, 34, ...`. PEI then maps `FI(32)->[sp,#24]`, `FI(33)->20`, `FI(34)->16`,
`FI(35)->12`, `FI(36)->8`, `FI(37)->4`. So the frame layout is *fully determined* by
the six weights:

| `[sp,#N]` | original | ours | ours weight |
|---|---|---|---|
| 4  | stack guard | stack guard | 2.00 |
| 8  | **`s` (Sinf result)** | `c` (Cosf result) | 7.27 |
| 12 | **`c`** | **`elapsed`** | 11.31 |
| 16 | **`elapsed`** | **`trailMatrices` local** | 32.48 |
| 20 | `this` | `this` | 36.74 |
| 24 | `dt` | `dt` | 47.74 |
| 28 | — | `volatile int unused` (fixed object, not a spill slot) | — |

Consequences, stated as requirements rather than hypotheses:

1. `elapsed` can only reach `[sp,#16]` if its weight exceeds the `trailMatrices`
   local's (11.31 < 32.48 today). The local's weight is dominated by its single hot
   use in `for.body` (block freq 31.0), so it cannot be made cold without changing
   the code (see section 4).
2. `strd r4, sl, [sp,#16]` is then automatic: `elapsed` and `this` become adjacent
   and are spilled at the same program point.
3. The original has **five** spill slots (`dt, this, elapsed, c, s`); ours has six.
   The sixth (`trailMatrices`, 32.48) is exactly the one that pushes `elapsed` down.
   The original also spills `s`, which ours keeps in `r6`; that is a *consequence* of
   the same layout (with `elapsed` at 16, `c` moves to 12 and `s` to 8).

This supersedes the pass-13 statement that the residual is a "closed register
fixed point": it is a **slot-count fixed point**. The register permutation
(`elapsed r4/r5`, `player r5/r4`, cursor `r4/r9`, `s` spilled/`r6`) is downstream of
the frame layout, not the cause of it.

---

## 3. Why the faithful (member-only) shape loses

The wave-103 oracle
(`_work/oracles/.../game/RocketGun.c`, `RocketGun::update`) shows the original body
uses `*(int *)(this + 0xd8)` directly — there is **no `trailMatrices` local** — and
the original disassembly confirms it (`ldr.w r0, [sl, #216]` at `15f32c`, `15f370`,
`15f39a`). Removing our local is therefore the *correct* source shape, but it
changes `this`'s register assignment (`this` moves out of `sl` into `r4`/`fp`) and
drops the score to 57.6 %. The local is currently load-bearing for `this`'s
allocation, and it is simultaneously the slot that blocks `elapsed` from 16.

---

## 4. Source variants tried this pass (~90, all <= 86.9)

Baseline **86.9**, 636 insns. Every row measured with the official single-symbol
verifier (`/dev/shm/wf310/try.sh <variant>.cpp`).

### 4.1 `trailMatrices` local placement / use subsets

| variant | pct | insns |
|---|---|---|
| baseline (local before loop; used for `==nullptr`, `!=nullptr`, 1st MST) | **86.9** | 636 |
| all 32 subsets of the 5 `trailMatrices` uses (local vs member) | best 86.9, next 85.7 | 635-662 |
| local declared at top of `for.body` | 59.7 | 631 |
| local declared at top of `if (gun->active != 0)` | 68.5 | 633 |
| local declared in the `else` branch | 57.6 | 626 |
| second local for the trail block only | 48.9 | 632 |
| second local for the 1st MST only | 66.3 | 632 |
| reassign the local inside the loop | 54.3 | 665 |
| `trailMatrices->data()[i]` for the 1st MST | 85.3 | 636 |
| `*(trailMatrices->data() + i)` | 85.3 | 636 |

### 4.2 Declaration order / anchors

| variant | pct | insns |
|---|---|---|
| baseline | **86.9** | 636 |
| `dt` after the trail local | 86.9 | 636 |
| `unused` before `dt` | 86.9 | 636 |
| `unused` hoisted above the `hitSmall` block | 85.3 | 636 |
| `dt` hoisted above the `hitSmall` block | 57.3 | 624 |
| trail local hoisted above `hitSmall` | 81.1 | **635** |
| all three hoisted above `hitSmall` | 81.7 | **635** |
| `dt` + `unused` hoisted above `hitSmall` | 80.9 | 636 |
| `volatile long long unused` | 75.9 | 638 |
| `volatile float unused` | 84.6 | 637 |
| `volatile unsigned unused` | 86.9 | 636 |
| `volatile float padx` | 86.9 | 636 |
| `volatile int padx = elapsed` | 72.6 | 633 |
| two `padx` anchors | 76.9 | 636 |

### 4.3 The E/P/U/L matrix (16 combinations)

`E` = `__builtin_expect` on the outer `hasGeometry && delayActive` test,
`P` = `volatile int padx`, `U` = `volatile int unused`, `L` = `trailMatrices` local.

| E | P | U | L | pct | insns |
|---|---|---|---|---|---|
| 0 | 0 | 0 | 0 | **86.9** | 636 |
| 1 | 0 | 0 | 0 | 84.6 | **635** |
| 1 | 1 | 0 | 0 | 84.6 | **635** |
| 1 | 0 | 1 | 0 | 83.5 | **635** |
| 1 | 1 | 1 | 0 | 83.5 | **635** |
| 0 | 1 | 0 | 0 | 86.7 | 636 |
| 0 | 0 | 1 | 0 | 85.8 | 636 |
| 0 | 1 | 1 | 0 | 85.6 | 636 |
| any with L=1 (member) | | | | 57.4-61.6 | 622-657 |

The rows that reach 635 instructions **lose** score: they remove the `strd`
opportunity in a different way (the `expect`/`padx`/`unused` anchors also steer the
entry-block allocation), so instruction count alone is not a useful target here.

### 4.4 Loop / expression shapes

| variant | pct |
|---|---|
| `size_t i`, `unsigned i`, `i != count`, `++i` vs `i += 1` | 86.9 |
| `int i` with `(int) count` | 86.9 |
| `const uint32_t count` hoisted | 74.9 |
| `dt` recomputed inside the loop | 86.9 |
| `volatile float dt` | 53.6 |
| extra hot-block use of `elapsed` in the fade block (`+ 0 * elapsed`) | 86.9 |
| `const int e = elapsed;` alias, all uses renamed | 86.9 |
| `RocketGun &self = *this;` | 86.9 |
| `Gun *loopGun = this->gun;` hoisted | 45.5 |
| `Array<int> *trailTimers` hoisted | 55.2 |
| `ParticleSystemManager *pm` hoisted | 57.1 |
| `static_cast<float>(elapsed)` | 86.9 |
| non-const `float dt` | 86.9 |
| `Vector ahead;` / `Vector ahead(0,0,0)` | 86.9 |
| `Vector ahead` declared after `Gun &g` | 85.4 |
| `2.0f * s` / `s * 2.0f` / `2.0f * c` | 86.9 |
| `continue` instead of `goto fade` | 86.7 |

---

## 5. Conclusion

The residual is a **slot-count fixed point**:

1. the `trailMatrices` local (weight 32.48) must disappear, because with it
   `elapsed` (11.31) can never reach `[sp,#16]` and `strd` never fires;
2. removing it changes `this`'s register assignment and costs ~29 points;
3. the original also spills `s`, which follows from (1).

The two requirements pull against each other in every source shape tried so far
(~590 across passes 2-14). The remaining lever is a source shape that (a) keeps
`this` in `sl` while (b) not materialising a `trailMatrices` stack slot and
(c) raises register pressure in the cluster block enough to spill `s`. The oracle
proves the *source* shape for (b) (direct member access), but the codegen does not
follow from it under NDK r18b clang 7.0.2 at `-Oz`.

**Reported both ways, as requested:**

* faithful shape (member-only, no anchors): **61.6 %**, 1868 B, 657 insns;
* tuned shape (current `src/`): **86.9 %**, 1908 B, 636 insns.

---

## 6. Reproduction

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

# stack-slot-colouring weights (self-built clang 7 at /tmp/llvm_android_702b)
/dev/shm/wf310/cc7.sh -mllvm -debug-only=stack-slot-coloring
```

Scratch is `/dev/shm/wf310` (`base.cpp`, `cc7.sh`, `try.sh`, `score.py`,
`w103/` with the ~90 variants of this pass, `ssc_cur.log`, `ra_cur.log`,
`orig_update.asm`, `base_update.asm`); none of it is in the repo.
