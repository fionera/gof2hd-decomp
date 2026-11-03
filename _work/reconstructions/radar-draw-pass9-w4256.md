# Radar::draw pass 9 — the landmark-body placement lever, and why the
# single-autopilot source is still 168 bytes short (w4256)

**Status:** research note, 2026-11-03. Worktree `wt/w4256`, branch `wave92-w4256`,
base `b379d00d`. **No source change landed** — no variant that keeps
`Radar::draw` at 9908 bytes beats the baseline's verifier ratio. This commit
contains this note only.

## 0. Baseline (measured before and after this pass)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes of both the original and our link (measured with `tools/verify/elf.py`):

| section | original | ours |
|---|---:|---:|
| `.text` | 0x1337c8 | 0x1337c8 |
| `.rodata` | 0x41b67 | 0x41b67 |
| `.ARM.exidx` | 0x6f70 | 0x6f70 |
| `.ARM.extab` | 0x26298 | 0x26298 |

(The wave-92 brief's `.ARM.extab 0x26290 (-8)` is stale: the current build is
**exact** at 0x26298.)

`_ZN5Radar4drawEP6PlayerP3Hudi`:

```
osize 9908  usize 9908  official ratio 0.739255 (73.9 %)  byte False linked False
normalized stream 3492 (original) / 3488 (ours)
```

`Radar.cpp` carries the scoped `-mllvm -likely-branch-weight=80
-mllvm -unlikely-branch-weight=1` override (`CMakeLists.txt:86-87`).

## 1. Method / harness

* Official verifier API, `difflib.SequenceMatcher(..., autojunk=False)`, exactly
  as `tools/verify/verify.py` scores it — not the CLI's rounded `--show`.
* `verify-fn` is broken in this worktree (`FN` is not forwarded, `--show` gets an
  empty name); every measurement below used a direct call into
  `tools/verify/verify.py` (`/dev/shm/w4256/measure.py`, plus `measure2.py`
  which also prints the un-rounded ratio).
* Ghidra: the project's function at `0x12e050` is **truncated**. Verified with a
  headless script this pass:

  ```
  PROGRAM android_2.0.16_libgof2hdaa.so
  IMAGEBASE 00010000
  FUNC FUN_0012e050 entry=0012e050 body=0012e050..0012e061
  ```

  i.e. Ghidra thinks the whole of `Radar::draw` is 0x12 bytes long, so its
  decompiler is useless here. All structural evidence below comes from
  `arm-linux-androideabi-objdump -d` on the original `.so`.

## 2. Structural map (the new evidence this pass)

`Radar::draw` is `0x12e050..0x130704` (9908 B) in the original. Three builds are
compared: the landed **baseline** (9908 B, duplicate campaign autopilot block
present), the semantically correct **delcamp** source (9740 B, duplicate removed),
and **vB** = delcamp with the `__builtin_expect(this->onScreen, 1)` hint removed
(9740 B).

### 2.1 Region boundaries

| region | original | baseline | delcamp | vB |
|---|---:|---:|---:|---:|
| `otherLock` #1 hot chain | `12e2fc` | `12e2fc` | `12e2fc` | `12e2fc` |
| landmark preamble | `12e314` | `12e338` | `12e33c` | `12e33c` |
| `otherLock` recompute | `12e376` | `12e376` | `12e382` | `12e37a` |
| campaign block (`campaign >= 2` test) | `12e38c` | `12e38c` | `12e39a` | `12e37c` |
| campaign-loop exit `bcs.w` | `12e3dc` → **`12e748`** | `12e3dc` → **`12e72c`** | `12e3e6` → **`12e758`** | `12e3c8` → **`12eaec`** |
| top-level autopilot body | `12e6ee` | `12e6d2` | `12e6f4` | `12ea78` |
| duplicate campaign autopilot | — | `12e72c` | — | — |
| planet-lock tail | **`12e748`** | **`12ec08`** | `12e758` | `12eaec` |
| landmark body setup | `12e7a8` | `12e780` | `12f44e` | `12e6ee` |
| landmark loop header | **`12e7d4`** (top) | `12eb04` (bottom) | `12f7c4` (bottom) | **`12e710`** (top) |
| `KIPlayer::isVisible` | `12e7f4` | `12e7c4` | `12f488` | `12e72c` |
| station-lock code | `12eb42` | `12eb10` | `12f7da` | `12f7da` |
| cold `otherLock = f14 != 0` | **`12ebc2`** | **`1304d0`** | `13044a` | `13044a` |
| cold `otherLock = lockedStation` | `12ebce` | `12eb8a` | `12f8b4` | `12f8bc` |
| cold `missionBlocksPlanets` | `12ebda` | `12eb96` | `12f8c0` | `12f8c8` |
| `planetLockTimer = 0; f18 = 0` | **`12ec1c`** (separate) | `12ec2a` (merged) | `12e79c` (merged) | `12eb30` (merged) |
| `enemies:` | `12ec24` | `12ec36` | `12e7a4` | `12eb3a` |

### 2.2 What the map says

1. **The original's campaign-loop exit lands directly on the planet-lock tail**
   (`bcs.w 12e748`), and that tail ends with an unconditional `b.n 12ec24`.
   The landmark body is placed immediately after it (`12e7a8`), i.e. the
   landmark loop is laid out **unrotated** — header at the top (`12e7d4`),
   body following, latch (`12ea18: add.w sl,sl,#1; b.n 12e7d4`) at the bottom.
2. **The baseline reproduces the landmark-body placement** (modulo the 0x30
   literal-pool shift: `12e780` vs `12e7a8`) but pays for it with the duplicate
   campaign autopilot block, which is exactly what the campaign-loop exit
   branches into (`bcs.w 12e72c`). The planet-lock tail is pushed to `12ec08`,
   after the cold blocks.
3. **The delcamp source gets the planet-lock tail right** (`bcs.w 12e758`,
   tail falls through into `enemies:` at `12e7a4`) but the landmark body is
   sunk into the middle of the enemies region (`12f44e`, header at the bottom).
4. **vB is the only build that gets *both* the landmark body placement and the
   unrotated landmark-loop layout right** (`12e6ee` / header `12e710`), but it
   then puts the planet-lock tail *after* the landmark body (`12eaec`).

So the three structural facts are individually reachable but never together:

```
original      [campaign][autopilot][planet-lock][landmark body, header top]
baseline      [campaign][autopilot][dup][landmark body, header bottom][cold][planet-lock]
delcamp       [campaign][planet-lock][enemies ... landmark body, header bottom]
vB            [campaign][landmark body, header top][planet-lock][enemies]
```

### 2.3 The 168-byte gap is 68 real instructions

Counting normalized instructions excluding literal-pool words
(`.word` / `.short` / `pool:` lines) in the verifier's own stream:

| build | real instructions | bytes |
|---|---:|---:|
| original | **3447** | 9908 |
| baseline | 3443 | 9908 |
| delcamp | **3379** | 9740 |

The delcamp source is 68 instructions / 168 bytes **short** of the original,
yet its `blx sym:` multiset differs by only four calls:

```
orig  delcamp  callee
  9      8     PaintCanvas::DrawString(uint, String const&, int, int, bool)
 17     16     KIPlayer::isDead()
 13     12     KIPlayer::isDying()
  7      6     FModSound::play(int, Vector*, Vector*, float)
```

Those four are tail-merges (the original keeps the landmark `else`
`DrawString` at `12e9e0` and the `i == 0` one at `12eb36` separate; it keeps
two `play` tails at `1302d2` / `130698`). They account for ~4 instructions, not
68. **The remaining ~64 instructions are not a missing statement** — they are
the difference between the original's unrotated/early landmark body and the
delcamp build's sunk/rotated one, plus the block-placement-driven branch
encodings. This is why the duplicate campaign block "works": its 29
instructions / 84 bytes sit exactly where the original's extra code sits, and
its presence is what pushes the landmark body back in line.

## 3. The `__builtin_expect(this->onScreen, 1)` lever

This is the single strongest block-placement knob found this pass.

| build | `onScreen` hint | usize | ratio | landmark `isVisible` |
|---|---|---:|---:|---:|
| delcamp | `expect(..., 1)` | 9740 | 0.622903 | `12f488` (sunk) |
| delcamp | `expect(..., 0)` | 9712 | 0.608206 | `12e724` |
| **delcamp** | **plain** | **9740** | **0.665126** | **`12e72c`** (early) |
| baseline | `expect(..., 1)` | 9908 | **0.739255** | `12e7c4` |
| baseline | `expect(..., 0)` | 9860 | 0.653763 | `12e712` |
| baseline | plain | 9896 | 0.710213 | `12e714` |

Dropping the hint moves the landmark body from `12f44e` to `12e72c` and is
worth **+4.2 points** on the single-autopilot source (0.6229 → 0.6651) — the
largest single-source-shape swing observed for this function. It is *not*
landable on the baseline, where the hint is worth −2.9 points.

The original's own layout is consistent with the hint being present
(`12e7f4 isVisible` is a fallthrough off `12e80e cmp r0,#0`), so the hint is
correct; the placement difference must come from something else.

## 4. Variant table (all measured this pass, official verifier)

Every row is the landed `Radar.cpp` plus the listed edit, compiled with the
80:1 override. `usize 9908` is required for a gate-neutral landing.

| variant | usize | ratio |
|---|---:|---:|
| **baseline (landed)** | **9908** | **0.739255** |
| delcamp (delete campaign `if (!alienOrbit)` autopilot) | 9740 | 0.622903 |
| delcamp + `expect(onScreen, 0)` | 9712 | 0.608206 |
| delcamp + `onScreen` plain | 9740 | 0.665126 |
| delcamp + `uint32_t i` loop | 9752 | 0.622543 |
| delcamp + `expect(field_0x14 == 0, 0)` on the planet-lock `else if` | 9744 | 0.622614 |
| baseline + `expect(onScreen, 0)` | 9860 | 0.653763 |
| baseline + `onScreen` plain | 9896 | 0.710213 |
| baseline + hoisted `int nLandmarks = landmarks->size()` | 9912 | 0.707045 |
| baseline + `!data[i]` instead of `data[i] == nullptr` | 9908 | 0.739255 |
| baseline + `otherLock` gas-first `&&` chain | 9896 | 0.708632 |
| baseline + positive `otherLock` form, `expect(...,1)`, gas-first | 9896 | 0.708632 |
| baseline + positive `otherLock` form, `expect(...,1)`, asteroid-first | 9908 | 0.739255 |
| baseline + `expect(field_0x14 == 0, 0)` on the planet-lock `else if` | 9920 | 0.718786 |
| baseline + swapped planet-lock `else if` / `else` arms | 9908 | 0.739255 |
| baseline + `else { planetLockTimer = 0; if (f14 == 0) f18 = 0; }` | 9908 | 0.739255 |
| baseline + `goto enemies;` in the planet-lock `else` | 9908 | 0.739255 |
| baseline + `goto enemies;` in both planet-lock arms | 9908 | 0.739255 |
| baseline + planet-lock block moved *before* the autopilot block | 9948 | 0.701890 |
| baseline + `float cx/cy` in the enemy in-sight test | 9948 | 0.704064 |
| baseline + distance-first in-sight (pass-8 "C") | 9924 | 0.711894 |
| baseline + distance-first with inline `(int)crosshairPos.x` | 9948 | 0.621571 |
| baseline + `VectorLength` first, then crosshair, inline casts | 9928 | 0.625233 |
| baseline + `float cx/cy` + distance-first, interleaved | 9952 | 0.631367 |
| baseline + `float cx/cy` + distance-first, grouped | 9924 | 0.711321 |

Nothing reaches 0.739255 except exact no-ops.

## 5. LLVM flag sweep (all appended to the 80:1 override)

Baseline source:

| flag | usize | ratio |
|---|---:|---:|
| `-force-loop-cold-block` | 9992 | 0.536 |
| `-loop-to-cold-block-ratio=1/3/10` | 9908 | 0.739255 |
| `-block-placement-exit-block-bias=1/5` | 9908 | 0.739255 |
| `-rotation-max-header-size=1/4` | 9908 | 0.739255 |
| `-cold-branch-ratio=1/50/90` | 9908 | 0.739255 |
| `-branch-fold-placement` | 9908 | 0.739255 |
| `-tail-dup-placement` | 9908 | 0.739255 |
| `-tail-dup-placement-threshold=0/4` | 9908 | 0.739255 |
| `-tail-dup-placement-penalty=0` | 9908 | 0.739255 |
| `-disable-early-ifcvt` | 9908 | 0.739255 |
| `-disable-ifcvt-triangle-rev` | 9908 | 0.739255 |
| `-ifcvt-limit=0` | 9920 | 0.690 |
| `-disable-ifcvt-diamond` | 9932 | 0.706 |
| `-disable-machine-licm` | 10208 | 0.299 |

Delcamp source:

| flag | usize | ratio |
|---|---:|---:|
| `-enable-misched` | 9784 | 0.472 |
| `-arm-no-restrict-it` | 9740 | 0.622903 |
| both | 9784 | 0.472 |

vB source (delcamp + `onScreen` plain):

| flag | usize | ratio |
|---|---:|---:|
| `-tail-merge-size=10` | 9740 | 0.665126 |
| `-enable-misched` | 9780 | 0.512 |
| `-disable-branch-fold` | 9748 | 0.664 |
| `-disable-early-taildup` | 9744 | 0.664 |

`-rotation-max-header-size` being completely inert is itself evidence: the
landmark loop is **not** rotated in the IR. The header-at-bottom layout of the
baseline and delcamp builds is `MachineBlockPlacement`'s chain ordering, not
`LoopRotate`. `-force-loop-cold-block` (the one flag that does move the layout)
destroys the ratio, so the cold-block classifier is not the lever either.

## 6. The VFP save set (unchanged by every source shape tried)

```
original  12e058  vpush {d8-d13}
baseline  12e058  vpush {d8-d11}
```

The original keeps `PlayerEgo::crosshairPos.x/y` live in `s26`/`s24` across the
enemy in-sight `AEMath::operator-` call and converts them to int only after it:

```
12ef4c: ldr r0,[pc,#688]        ; &crosshairPos
12ef52: strb.w r6,[r8,#115]     ; enemy->field_0x73 = 0
12ef5a: vldr s26,[r0]           ; crosshairPos.x
12ef5e: vldr s24,[r0,#4]        ; crosshairPos.y
12ef64: ldrd r2,r1,[sp,#108]
12ef68: blx  AEMath::operator-
12ef6e: vcvt.s32.f32 s24,s24    ; (int)crosshairPos.y
12ef72: vcvt.s32.f32 s26,s26    ; (int)crosshairPos.x
12ef76: blx  AEMath::VectorLength
```

so the original's in-sight test is **distance-first** and the crosshair values
are `float` locals cast at each use. Every spelling of that shape tried
(§4, the six `in-sight` rows) moves `usize` to 9924-9952 and the ratio to
0.62-0.71; none keeps `usize 9908`.

## 7. Conclusion

* The residual is **block placement**, not missing statements: the delcamp
  source's `blx` multiset differs from the original by exactly the four known
  tail-merges, yet it is 68 real instructions short.
* The three structural properties of the original —
  (a) campaign-loop exit → planet-lock tail, (b) landmark body immediately
  after the planet-lock tail, (c) unrotated landmark-loop header —
  are individually reproducible (delcamp gives (a), baseline gives (b),
  `onScreen`-plain gives (b)+(c)) but **no source shape so far gives all
  three at once**, and only the baseline's duplicate-block crutch reaches
  0.739255.
* `__builtin_expect(this->onScreen, 1)` is the dominant placement knob
  (+4.2 points on the single-autopilot source, −2.9 on the baseline). It is
  semantically right (the original's `12e7f4` is a fallthrough), so it should
  stay.
* No new anchor was added and no flag outside the existing 80:1 override helps.

### What remains

1. Find the source shape that keeps the landmark body at `12e7a8` **and** the
   planet-lock tail at `12e748` with the duplicate block deleted. The blocker
   is that the campaign-loop exit block is chosen by `MachineBlockPlacement`
   from the block that follows the loop; in the original that is the
   planet-lock tail, in the baseline it is the duplicate autopilot block.
2. Recover the ~64 non-call instructions the delcamp build loses. They are
   spread over the landmark body / station-lock region, not concentrated in
   one block, so a single missing statement is unlikely.
3. The `vpush {d8-d13}` / `s24`/`s26` shape (§6) is independent and still
   costs `usize 9908` whenever it is reproduced.

## 8. Evidence / scratch

Scratch lives in `/dev/shm/w4256/` (not committed):

* `measure.py` / `measure2.py` — official-API ratio (measure2 also prints the
  un-rounded ratio).
* `trysrc.py` — copy a variant into `src/game/weapons/Radar.cpp`, rebuild,
  measure, and report the address of the landmark `KIPlayer::isVisible` call.
* `sweep2.sh` — apply one `-mllvm` flag to the Radar override, rebuild, measure.
* `v_base.cpp` (landed baseline), `v_delcamp.cpp`, `vA.cpp`, `vB.cpp`,
  `vC.cpp`, `v_dc_e0.cpp`, `v_baseA.cpp`, `v_baseB.cpp`, `bF.cpp`, `bG.cpp`,
  `o1-o3.cpp`, `p1-p3.cpp`, `q1-q3.cpp`, `s1.cpp`, `s3.cpp`, `c1/c2.cpp`,
  `r1/r2.cpp`, `t1/t2.cpp` — the §4 variants.
* `orig_body.txt`, `base_fn.txt`, `dc_fn.txt`, `vb_fn.txt` — objdump of
  `Radar::draw` for the original / baseline / delcamp / vB builds.
* `blocks.py`, `cnt.py`, `range.py`, `callseg.py`, `seqdiff2.py`,
  `bigdiff.py` — the structural tools used for §2.
* `/home/debian/ghidra_scripts/W4256Info.java` (outside the repo) — the
  headless script that confirmed the Ghidra truncation in §1.
