# `RocketGun::update(int)` — wave 107 (w5512)

**Status:** source change landed, 2026-11-04. Worktree `wt/w5512`, branch
`wave107-w5512`, base `f857b5171`.

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**.

| | pct | size | insns | gate |
|---|---:|---:|---:|---|
| before (baseline) | 86.9 | 1908 | 636 | PASS |
| **after (this pass)** | **88.3** | **1908** | 636 | **PASS** |

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
drift: 0 field(s) across 0 class(es)
lint:  CLEAN
```

Sections unchanged by this pass: `.text` 0x1337c8 (base-exact), `.ARM.extab`
0x26294 (still the known 4 B short vs 0x26298), 28 sections (the known
`.missioncase` delta). No new section/`extab` delta.

---

## 1. Baseline / reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi
```

Per-variant scoring used a scratch helper (`try.sh <abs path>` = copy into
`src/game/weapons/RocketGun.cpp`, `ninja link_so`, then the single-symbol
`score.py`), ~0.65 s/variant; size/insn screening used the same NDK clang
`-c` + `objdump` (~0.08 s/variant).

---

## 2. What was already known (w5315 / w106-w5412)

The body is semantically faithful; the residual is codegen only. The previous
best was the **baseline** (86.9 %, 1908 B), which carries two `volatile`
anchors (`unused`, `padx`) and a cached `trailMatrices` local.

The w5512 handoff's `k_novel` shape — remove both anchors and the trail local,
and force a real `Vector` copy in the cluster block — reaches **88.4 % /
1912 B / 637 insns**, and its *stack slot map matches the original exactly*:

```
k_novel:  dt@24, this@20, elapsed@16, s@8, c@12, guard@4
original: dt@24, this@20, elapsed@16, s@8, c@12, guard@4
baseline: dt@24, this@20, trail@16, elapsed@12, c@8, guard@4
```

But 1912 B fails the gate: `byte_exact` drops 4500 → 4495 because
`RocketGun::update` grows 4 B and the five following symbols
(`seekEnemy`, `MarqueeImage::{C1,C2,draw,draw}`) shift by 4, changing the
absolute addresses in their literal pools. `.text` size itself is unchanged
(padding absorbs the growth), so only `byte_exact` moves; `linked_exact`
stays 4500.

`k_novel` source shape (scratch `k_novel.cpp`):

```cpp
const float phase = (float) ((uint32_t) (total * (int) i) / n);
const int life = gun->lifetimes[i];                 // after phase
...
struct V3 { float x, y, z; };
V3 crossBuf;
Vector &cross = *(Vector *) &crossBuf;
cross = VectorCross(this->gun->velocities.data()[i],
                    this->gun->upVectors.data()[i]);
this->gun->positions.data()[i] += VectorNormalize(cross) * (s + s) * dt;
```

plus `this->trailMatrices` used directly everywhere (no local) and `life`
declared after `phase`.

### Why `k_novel` is +4 B

Byte-level diff vs the original (same normalised instruction stream otherwise):

| delta | bytes |
|---|---:|
| `blx Vectora` + `add r0,sp,#104` + `mov r1,r5` (the copy) | +8 |
| extra `ldr r5,[sp,#16]` in the `z==50000` branch | +2 |
| original has a `nop` after `b.n` (alignment) that we lack | −2 |
| `add r5,sp,#40` / `add r4,sp,#116` (short) vs original `add.w r9,#40` / `add.w r8,#116` | −4 |
| **net** | **+4** |

So the copy is what keeps the register pressure high enough to reproduce the
original's slot map; removing it (NRVO, `Vector cross = VectorCross(...)`,
`VectorNormalize(VectorCross(...))`, `const Vector &cross = ...`) collapses
the whole allocation to **1872 B / 622 insns / 55–57 %**.

---

## 3. The new lever: reorder one independent statement to recover 4 B

The 4 B can be recovered *without* touching the cluster block or the slot map
by reordering one pair of independent statements so the compiler picks the
short `add rX, sp, #imm` forms / low stack slots. Two families were found:

**(a) launch block — hoist the heading read** (landed):

```cpp
Vector heading = this->gun->velocities.data()[this->gun->currentProjectile];
PaintCanvas *canvas = Globals::Canvas;
Matrix &local = *(Matrix *) canvas->TransformGetLocal(this->transform);
const Vector &shot = this->gun->positions.data()[this->gun->currentProjectile];
MatrixSetTranslation(local, shot.x, shot.y, shot.z);
```

**(b) muzzle block — compute the barrel offset first**:

```cpp
Vector barrel = this->gun->offset;
barrel.z -= 100.0f;
PlayerEgo *player = this->level->getPlayer();
Vector muzzle = player->getPosition();
```

Both give **88.3 % / 1908 B / 636 insns** and pass the gate. (a) was kept
because it leaves the (larger) muzzle block byte-for-byte in the original
order.

All 1908 B variants found *require* the `buf` cluster form (the `V3` buffer +
`Vector &` copy) and the trail-local removal; `nrvo`/`named`/`ref`/`twonorm`/
`fields` forms are 1872–1920 B.

### Measured table (this pass)

| variant | pct | size | insns | note |
|---|---:|---:|---:|---|
| baseline | 86.9 | 1908 | 636 | anchors + trail local + nrvo |
| `k_novel` | 88.4 | 1912 | 637 | best raw, **gate fails** (byte 4495) |
| **`lb_h2` (landed)** | **88.3** | **1908** | 636 | launch-block heading hoist |
| `mz_v1` | 88.3 | 1908 | 636 | muzzle-block barrel first |
| `z_z1` | 88.0 | 1908 | 636 | `trailTimers=0` first in z==50000 branch |
| `cp_fields2` / `l_fields` | 85.9 | 1908 | 636 | field-by-field copy |
| `cc2_copy` (`Vector copy = cross`) | 84.1 | 1908 | 635 | real copy, different allocation |
| `n3_inline` (NRVO, no trail) | 57.1 | 1872 | 622 | collapses |
| `z_inline_chain` | 55.5 | 1872 | 622 | collapses |
| `defassign` (`Vector cross; cross = ...`) | 84.0 | 1920 | 640 | zero-init not elided |

Systematic sweeps: all 486 valid orderings of the 10 muzzle-block statements
were compiled — **72 give 1908 B, all start with the barrel statement**; the
best is 88.3. All 6 valid orderings of the launch-block prefix that give
1908 B were scored; the best is 88.3. Combining both families yields no
1908 B variant.

### Remaining diff (121 normalised lines vs 224 for the baseline)

1. `mov r5, r1` (elapsed) vs original `mov r4, r1`; `player` takes the other
   register. This is the only register permutation left.
2. The `Vectora` copy in the cluster block (3 instructions) — the original
   feeds `VectorCross`'s sret straight into `VectorNormalize`.
3. The extra `ldr r5,[sp,#16]` in the `z==50000` branch (the original
   tail-merges the reload into a shared block).
4. `nop`/`add.w fp,#12`/`vcvt` scheduling and literal-pool slotting.

---

## 4. Proven vs hypothesis

**Proven (measured):**

* `k_novel`'s stack slot map is byte-for-byte the original's; its only
  structural excess is the `Vectora` copy.
* Removing the copy always collapses the allocation (1872–1880 B, 55–57 %),
  with or without the trail local, anchors, two-named-locals, reference
  binding, or in-place normalize.
* Reordering one independent statement (launch-block `heading`, or
  muzzle-block `barrel`) recovers exactly the 4 B and keeps 88.3 %.
* 1908 B is only reachable with the `buf` cluster form + no trail local.
* The gate (`byte_exact >= 4500`) requires the function to stay 1908 B;
  1912 B costs exactly 5 `byte_exact` symbols (`linked_exact` unaffected).

**Hypothesis (unproven):**

* The original source has no `Vectora` copy and `elapsed` in `r4`; the
  original's extra register pressure must come from a construct we have not
  found (the copy is our stand-in). With the copy removed our clang 7.0.2
  picks a 1872 B allocation, so the original source differs somewhere else
  as well.
* The `elapsed`/`player` swap is decided by the greedy allocator's first
  eviction (`%250`, the LICM-hoisted address of the shared `Vector` temp,
  weight 7.9e-3 > elapsed's 8.7e-4); no source-level lever tried so far
  (declaration order, anchors, extra uses, loop forms) changes it.

---

## 5. Scratch

`/dev/shm/w5512`: `base.cpp`, `k_novel.cpp`, `lb_h2.cpp`, `mz_v1.cpp`,
`z_z1.cpp`, `orig.asm`, `kn.asm`, `lbh2.asm`, `mz1.asm`, `zz1.asm`,
`try.sh`, `compile.sh`, `dump.sh`, `ndiff.py`, `score.py`,
`mzall.py`/`mzall_orders.txt` (muzzle-order sweep), `lbsearch.py`,
`comb2.py`, `final.py`. None of it is in the repo.
