# PlayerEgo::calcCollision — wave 111 (w5911): check-shape sweep, no new best

**Status:** research note, 2026-11-05. Worktree `wt/w5911`, branch
`wave111-w5911`, base `8eea060cc`. **No source change landed** — the
checked-in 87.6436781609 % / 968 B shape is still the best measured.
This note extends the wave-107/108/109/110 fixed-point notes
(`playerego-calccollision-wave107-w5513.md`,
`-wave108-w5614.md`, `-wave109-w5711.md`,
`-wave110-w5811.md`).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions, 87.6436781609 % exact).

## TL;DR

* The landed two-anchor body still scores **87.6436781609 %** exactly
  (968 B, 347/349 normalised instructions).  ~150 new builds this wave
  (cheap always-false checks, early-outs, surface/collision/impact
  declaration forms, loop forms, dock/i weight perturbations) produce
  nothing above it.
* **New measurement: the content-only (branch-target-masked) ratio.**
  Masking intra-function branch targets before the
  `SequenceMatcher` separates the two failure modes.  The landed body
  is **92.8161 %** content-identical to the original; the `le/le`
  two-check body is **95.1289 %** content-identical but loses more to
  literal branch offsets because it is 16 B too long (984 B).
* **New fixed point: `ge_noguard`.**  A *single* always-true check
  `if (objects->size() >= i) continue;` after `outerCollide`, plus the
  **landed** bottom guard (`if (object->field_0x71)` without `i == 0`),
  reproduces the original's **exact latch address `0x9f38c`** and the
  original **968 B** size, with `this=r5`, `surface=r6`,
  `&__stack_chk_guard` spilled at def — but swaps the last two
  callee-saved roles: **`i=r9`, `dock=fp`** instead of the original's
  `i=fp`, `dock=r9`.  Score 87.2832369942 %, 968 B, 347/345.
* The right register file (`this=r5`, `surface=r6`, `i=fp`, `dock=r9`,
  guard spilled at def) is reproduced only by **two** size-based
  always-false checks (`objects->size() <= i` at the landed A1+A2
  positions) — 85.6734 %, 984 B.  Removing either check, or the
  `i == 0` bottom guard, moves `i` to `r9` or `sl`.

## 1. Exact scores (unrounded)

```
landed (A1+A2, guard w/o i==0)     pct=87.6436781609  size=968  lens=347/349
ge_noguard (single ge, no i==0)    pct=87.2832369942  size=968  lens=347/345
le/le + real guard                 pct=85.6733524355  size=984  lens=347/351
lele_noguard                       pct=85.4676258993  size=976  lens=347/348
eq/le + real guard                 pct=85.2224184741  size=972  lens=347/347
ge + real guard                    pct=83.4782608696  size=968  lens=347/343
natural (no checks, real guard)    pct=59.7997138770  size=996  lens=347/352
```

## 2. Content-only (branch-target-masked) ratio

`SequenceMatcher` over the normalised streams with `loc_XXX` and
`0xNNN` operands replaced by `#` (so branch displacement and pool
offsets do not count):

| body | masked % | size | content diffs vs original |
|---|---:|---:|---|
| landed | **92.8161** | 968 | missing `str.w r9,[sp,#20]`@def, A1 (4), A2 (3), collision-arm shuffle (4), `cbz` vs `cmp/beq.w`, latch (2) |
| le/le + real guard | **95.1289** | 984 | pool offsets (5), A1 (3), A2 (3), 2 late `str.w r9,[sp,#20]`, collision-arm shuffle (4), `nop` |
| ge_noguard | 90.1734 | 968 | `i`/`dock` register swap (many), `ge` check (3), missing `i == 0` guard (2), pool offset |
| natural | 68.0973 | 996 | `this=sl`, `surface=r5`, guard in `r6`, `distance` spilled, rotated loop |

The `le/le` shape is the closest in *content* but pays 16 B, which
shifts every literal branch target; the landed shape is farther in
content but keeps the 968 B size, so its branch targets still match
the original's absolute offsets in the prologue and the tail.

## 3. The collision-arm shuffle is a live-interval artifact

Original collision arm (`0x9f2f6`):

```
ldr r1,[r5,#8] ; mov r0,r6 ; blx getPosition     ; surface = geometry->getPosition()
ldr r0,[r8]    ; mov sl,r5 ; mov r5,r6 ; add r6,sp,#96
mov r1,r8 ; mov r2,r5 ; ldr r3,[r0,#88] ; mov r0,r6 ; blx r3
...
mov r6,r5 ; mov r5,sl ; mov r1,r6
```

The sret slot is `r6` and `this`/`surface` are shuffled through `sl`.
In every variant we build the sret slot is `sl` directly
(`add.w sl,sp,#96`), 4 instructions shorter.  In the original the
collision arm is laid out **between** the `distance` definition
(`0x9f1f0`, `vmov sl,s0`) and its last use (`0x9f40a`,
`cmp.w sl,#1000`), so `sl` is inside `distance`'s linear live range;
the allocator therefore cannot use it for the sret pointer and picks
`r6`, forcing the shuffle.  In our 984 B `le/le` body the same block
order holds, but the allocator's interval splitting still lets `sl`
be reused, so the shuffle does not appear.  This is the single
largest remaining content delta (4 instructions) and it is purely an
allocator/layout artifact, not a source-shape difference.

## 4. What was tried this wave (all <= landed)

### 4.1 Cheap always-false checks at the landed A1/A2 positions
(on the natural base)

| form | pct | size | i | dock | latch |
|---|---:|---:|---|---|---|
| `size() <= i` (le/le, = lele) | **85.6734** | 984 | fp | r9 | 0x9f39a |
| `size() != 1` | 85.6734 | 984 | fp | r9 | 0x9f39a |
| `size() - i == 0` | 85.2224 | 972 | fp | r9 | 0x9f190 |
| `size() < i` | 85.2224 | 972 | fp | r9 | 0x9f190 |
| `size() <= 1` | 85.2224 | 972 | fp | r9 | 0x9f190 |
| `size() - 1 < i` | 84.9785 | 976 | fp | r9 | 0x9f192 |
| `size() > 100000u` | 84.5714 | 980 | fp | r9 | 0x9f192 |
| `i > 100000u` (i-only) | 70.6724 | 968 | r9 | fp | 0x9f440 |
| `i == 0xffffffffu` | 67.6218 | 952 | r9 | fp | 0x9f196 |
| `size() == 0` (2 insns) | 69.7013 | 1016 | sl | — | 0x9f3b2 |

Only checks that both load `objects->size()` **and** compare against
`i` reproduce the right `this/surface/dock/i` file.

### 4.2 Single checks / early-outs / loop forms

* Single check at A1 or A2 only: 70.5–82.6 %.
* `if (objects->size() == 0) return;` after the dockedFlag check:
  landed 77.0328 (980 B), natural 63.8968 (960 B).
* Top-of-loop `size()==0 / size()<=i / i>=size / size()<1`:
  codegen-identical to natural (996 B, 59.7997); `size() != 1`
  57.3466.
* Explicit `continue;` at loop end / `++i` / `while` form:
  59.7997 / 59.7997 / 42.6087.
* `if (object != nullptr) { ... }` wrapper instead of
  `if (object == nullptr) continue;`: landed 87.6437 (identical),
  natural 59.7997.
* `i` predeclared outside the loop (± use after loop): 87.2832
  (identical to `ge_noguard`).

### 4.3 Surface / collision / impact declaration forms

| form | pct | size |
|---|---:|---:|
| `Vector surface;` hoisted to function scope (landed base) | 46.9590 | 1028 |
| `Vector surface;` hoisted (natural base) | 44.6629 | 1028 |
| `Vector surface = getPosition();` before the null check (landed) | 51.4045 | 1020 |
| same (natural) | 59.5683 | 964 |
| remove the `{ }` around surface (landed) | 56.8627 | 1000 |
| remove the `{ }` around surface (natural) | 64.0805 | 968 |
| `Vector collision; collision = ...` | 71.8529 | 1028 |
| `Vector collision(...)` | 85.6734 | 984 |
| `Vector collision = Vector(...)` | 85.6734 | 984 |
| `if (collision != Vector{...})` | 85.3868 | 984 |
| `Vector impact; impact = ...` | 71.7514 | 1024 |
| `if (Vector{...} != impact)` | 85.1003 | 984 |
| `Vector collision;` / `Vector impact;` hoisted | 78.7447 / 77.8887 / 77.6836 | 1000/1000/1020 |

The `natural` + no-braces shape is notable: it lands at the original
**968 B** with the original latch address `0x9f38c`, but the register
file is `this=r5, object=r9, dock=sl, surface=r8`, frame 120 B
(69.2529 % masked).

### 4.4 Third check / weight perturbations

* Third `size() <= i` check after the steal arm / before
  `projectCollisionOnSurface` / before `hitCamera` / before the
  getType test: 71.04 / 71.04 / 84.69 / 85.10 %.
* Dock-weight raisers after the `ge` check (`dockOffsetVec.x != .x`,
  `.y != .y`, `VectorLength(dock) > 1e6f`, `< 0.0f`,
  `dockingPointIndex == 0xffff`): 70.30–74.39 %.
* Size-only checks (`size() > 0xffffu`, `size() < 0xffffu`,
  `size() >= 0x10000u`, `size() == 0xffffu`, `size() != 0xffffu`,
  `size() > 100u`, `size() < 100u`): 79.31–82.30 %, all give
  `i=r9`, `dock=fp`.

## 5. Proven vs. hypothesis

**Proven (measured):**

* The landed body's exact score is 87.6436781609 % (347 vs 349
  normalised instructions, 968 B); ~150 new variants this wave plus
  the full wave-110 A1×A2 cross product find no better point.
* `ge_noguard` (single always-true `size() >= i` check + the landed
  bottom guard) is 968 B with the **exact original latch address
  `0x9f38c`** and the exact original prologue except that `i` and
  `dock` swap `r9`/`fp`.  Score 87.2832369942 %.
* The right register file is only reached with **two** size-based
  checks (le/le, 984 B, 85.6734 %) or with `ge_noguard` (968 B, but
  `i`/`dock` swapped).  No variant reaches the right register file at
  968 B.
* Content-only (branch-masked) similarity: landed 92.8161 %, le/le
  95.1289 %, `ge_noguard` 90.1734 %, natural 68.0973 % (natural without the
surface braces 69.2529 %).
* The original's collision-arm register shuffle is explained by the
  collision arm being laid out inside `distance`'s live interval
  (`distance` def `0x9f1f0` … use `0x9f40a`); `sl` is therefore not
  available for the sret pointer.

**Hypothesis (not proven):**

* The original source is the natural (REA/oracle) shape compiled with
  an allocator state that spills the `__stack_chk_guard` pointer and
  keeps `distance` in `sl`; none of the ~150 source spellings tried
  here (nor the ~2000 in waves 107–110) reproduces that state without
  adding a check.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5911
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
# per-variant: splice body into src/game/ship/PlayerEgo.cpp, then
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w5911/score.py            # exact unrounded score
python3 /dev/shm/w5911/measure.py <body>   # score + register file / latch
python3 /dev/shm/w5911/maskdiff.py         # branch-target-masked content diff
```

Scratch harness (`/dev/shm/w5911`): `score.py`, `measure.py`,
`maskdiff.py`, `try.py`, `batch.sh`, the body files
(`body_landed.txt`, `body_natural.txt`, `body_lele.txt`,
`body_ge.txt`, `body_ge_noguard.txt`, `body_natnobrace.txt`) and the
sweep outputs (`w111/sweep1.txt` … `sweep17.txt`).

## 7. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0,
`RATCHET PASS`.  27 sections exact, file size 2,187,328 B.  No source
change, so `tools/verify_baseline.json` is untouched.
