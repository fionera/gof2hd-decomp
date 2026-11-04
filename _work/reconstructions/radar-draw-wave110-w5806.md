# Radar::draw pass (wave110, w5806) — expect-polarity sweep, vpush width, and the OR-chain shape

**Status:** research note, 2026-11-04. Worktree `wt/w5806`, branch `wave110-w5806`,
base `390f58052`. No source change landed (no variant beat the baseline); this
commit contains this note only.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.

## 0. Baseline and gate (unchanged source)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 byte False linked False
  raw 74.029509 %  norm 3492/3489
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Per-variant scores use the official verifier API
(`/dev/shm/w5806/measure.py`, `difflib.SequenceMatcher(None, on, un,
autojunk=False).ratio()` at six decimals). The rounded report score stays 74.0
for everything in 73.95..74.04.

## 1. New result: full `__builtin_expect` polarity sweep on the first `otherLock` chain

The landed source is

```cpp
int otherLock;
if (__builtin_expect(this->lockedAsteroid == nullptr &&
                     __builtin_expect(this->lockedGasCloud == nullptr, 1) &&
                     this->field_0x1c == nullptr, 0)) {
    otherLock = this->field_0x14 != 0;
} else {
    otherLock = 1;
}
```

All 4^4 = 256 combinations of {none, 0, 1} hint on the four positions
(`lockedAsteroid`, `lockedGasCloud`, `field_0x1c`, whole condition) were built
and measured. The landed `(n, 1, n, 0)` is the unique maximum; `(n,1,1,0)` and
`(n,1,0,0)` are bit-identical to it (the third hint is dead). The next best is
`(n,n,n,0)` / `(n,n,1,0)` at **74.000859 %** (9908 B). Every hint on the first
term (`a=0` or `a=1`) costs >= 1.2 points. No combo reaches 74.03.

Raw table (top 8, `a b c w`):

| hints | raw % | norm |
|---|---:|---:|
| `n 1 n 0` (landed) | **74.029509** | 3492/3489 |
| `n 1 1 0` / `n 1 0 0` | 74.029509 | 3492/3489 |
| `n n n 0` / `n n 1 0` | 74.000859 | 3492/3489 |
| `1 n 1 n` / `1 1 n n` | 72.836676 | 3492/3488 |
| `n n n n` (plain) | 72.797593 | 3492/3489 |
| `n 1 1 n` | 72.722063 | 3492/3488 |
| `n 0 1 n` | 72.292264 | 3492/3488 |
| `0 0 n n` (worst) | 60.481100 | 3492/3492 |

## 2. The original's first `otherLock` chain is the positive-OR shape

Original `0x2b4..0x2c2` (function-relative):

```
2b4 cbnz r0, 0x2c2       ; lockedAsteroid != null
2b6 ldr  r0, [r4,#56]    ; lockedGasCloud
2b8 cbnz r0, 0x2c2
2ba ldr  r0, [r4,#28]    ; field_0x1c
2bc cmp  r0, #0
2be beq.w 0xb72          ; all null -> cold block 1
2c2 movs r5, #1          ; otherLock = 1
```

This is exactly the shape our **second** `otherLock` chain already produces
(`otherLock = lockedAsteroid != nullptr || lockedGasCloud != nullptr ||
field_0x1c != nullptr || lockedStation != nullptr;` at `0x326..0x33a`). The
landed first chain instead produces a split/if-converted form:

```
2b4 cbz  r0, 0x2d4
2b6 mov.w r9, #1
2ba ldr  r5, [sp,#156]
2bc b.n  0x2e8
2c0 ..0x2d2  literal pool (5 words)
2d4 ldr  r0, [r4,#56]
2d6 ldr  r5, [sp,#156]
2d8 cmp  r0, #0
2da itt  eq
2dc ldreq r0, [r4,#28]
2de cmpeq r0, #0
2e0 beq.w 0xb3c
2e4 mov.w r9, #1
```

Spellings tried this wave to force the `cbnz` shape (all measured):

| first-chain spelling | usize | raw % | 0x2b4 |
|---|---:|---:|---|
| landed nested-expect negative-AND | 9908 | **74.029509** | `cbz` |
| plain negative-AND / positive-OR / `!(a||b||c)` | 9908 | 72.797593 | `cbz` |
| `int o = a||b||c||d;` (4-term OR) | 9904 | 70.549419 | `cmp/itt` |
| `int o = a||b||c; if (d) o = 1;` | 9908 | 65.415473 | `cbnz 0x2ba` |
| `int o = a||b||c; o |= d;` | 9908 | 65.415473 | `cbnz 0x2ba` |
| `bool o = a||b||c||d;` | 9904 | 67.824340 | `cmp/itt` |
| `int o = (a||b||c) ? 1 : (d?1:0);` | 9908 | 72.797593 | `cbz` |
| `int o = !!((a||b||c||d));` | 9908 | 71.248568 | `cmp/itt` |
| `int o = (a||b||c) + d;` | 9908 | 70.542747 | `ldr` |

The `cbnz` shape is reachable (`v6`/`v14` above) but the target is `0x2ba`,
not `0x2c2`, and the score falls to 65.4 %. The difference from the second
chain is not the source spelling: both are `(a||b||c) ? 1 : (d != 0)`. It is
the *use* of the result (`notOtherLock = !otherLock` on the stack vs. the
later `!otherLock` in the planet tail) and the register the result lands in
(`r9` vs `r0`). The original has `otherLock` in `r5` and the status pointer in
`r6`; ours has `otherLock` in `r9` and the status pointer in `r5`.

## 3. `vpush {d8-d13}`: distance-first in-sight is the cause, but it moves the frame

Original prologue: `vpush {d8-d13}` + `sub sp,#296` + `str r3,[sp,#100]`.
Ours: `vpush {d8-d11}` + `sub sp,#296` + `str r3,[sp,#112]`.

The original's enemy in-sight block (`0xef4c..0xefc0`) loads
`PlayerEgo::crosshairPos.x/.y` into `s26`/`s24` **before** the
`Vector::operator-` call, converts them to int after it, and only then tests
the 60000-unit distance and the four screen bounds. That keeps two extra
callee-saved floats live across the call, which is why it saves `d8-d13`.

The faithful source shape is therefore

```cpp
float cx = PlayerEgo::crosshairPos.x;
float cy = PlayerEgo::crosshairPos.y;
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f) {
    inSight = this->screenX > (int)cx - this->field_0x124 &&
              this->screenY > (int)cy - this->field_0x124 &&
              this->screenX < (int)cx + this->field_0x124 &&
              this->screenY < (int)cy + this->field_0x124;
}
enemy->field_0x73 = inSight;
```

Measured (all on the pristine baseline):

| in-sight shape | usize | frame | vpush | raw % |
|---|---:|---|---|---:|
| landed (bounds-first, int cx/cy) | 9908 | 296 | d8-d11 | **74.029509** |
| `float cx/cy`, distance-first, nested if | 9936 | 304 | **d8-d13** | 62.358179 |
| `float cx/cy`, distance-first, `inSight = &&` | 9928 | 296 | **d8-d13** | 64.474625 |
| `float cx/cy`, bounds-first | 9908 | 296 | d8-d11 | 70.422535 |
| direct `(int)crosshairPos.x`, distance-first | 9984 | 304 | d8-d11 | 62.029483 |
| `float dist` local, bounds-first | 9908 | 296 | d8-d11 | 63.115693 |

The `inSight = &&` variant (G1) matches the original's `vpush {d8-d13}` **and**
`sub sp,#296` exactly, and its in-sight body is instruction-for-instruction
close to the original (branchy bounds, final `it lt; movlt`). But the first
stack slot is `[sp,#108]` instead of `[sp,#100]` (8 B of extra locals), the
body is 20 B larger (9928 B), and the landmark body / cold block 1 / enemies
all shift (`0x736` / `0x25ba` / `0xbe8` vs. original `0x758` / `0xb72` /
`0xbd4`), which costs ~10 points. Combining it with the plain `otherLock`
(which puts cold block 1 back at `0xb3c`) does not help: 64.399028 %.

## 4. Call multiset (re-confirmed) and the missing `DrawString`

Baseline vs original call counts:

```
DrawString           9 vs 8     (+1 original)
isDead              17 vs 16    (+1 original)
isDying             13 vs 12    (+1 original)
getStations          2 vs 3
getStationEnumIndex  1 vs 2
getPlayer           29 vs 32
getSystem            5 vs 7
goingToPlanet        1 vs 2
isDockingToPlanet    4 vs 5
getAutoPilotTarget   1 vs 2
```

The 10-call surplus is exactly the post-planet-loop `if (!alienOrbit)`
recompute. `nodup` (remove it) gives **342 vs 339** calls: the three remaining
misses are the faithful 6-call asteroid predicate (one `isDead` + one
`isDying`) and one `DrawString`.

The missing `DrawString` is the **landmark else-branch lock label** (the
`else` of `if (i == 0 && !alienOrbit)` in the `i <= 1` landmark block, source
lines ~470-478). The original has 9 `DrawString` sites at `0x28a, 0x5a4,
0x90a, 0x990, 0xa5e, 0xae6, 0xfda, 0x16e6, 0x1a24`; the pristine baseline has
8 at `0x28a, 0x5a4, 0x908, 0xa1a, 0xaa2, 0x1000, 0x16e6, 0x1a68`. The
gas-cloud site **is** present and lands at the same address as the original
(`0x16e6`); the four landmark sites are not all separate: the original's
else-branch call at `0x990` is tail-merged by clang with the `i == 0 &&
!alienOrbit` branch's lock-label call, so both branches reach the single
`blx` at `0xaa2` (the else branch does `b.n 0xaa2` at `0x994`). This is a
codegen/tail-merge divergence, not a missing statement: the source already has
both calls.

`nodup` + 6-call asteroid + preventing the landmark else-branch tail merge is
therefore the faithful call multiset. Its size would be ~9776 B, still ~130 B
short of 9908, so other non-call code is also missing (the `nodup` build's
landmark loop is sunk after the enemies at `0xf16`, whereas the original keeps
it hot at `0x758`).

## 5. Measurements this wave (all on the pristine baseline)

Baseline = **9908 B / 74.029509 % / 3492/3489**.

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| full expect sweep, best non-landed | 9908 | 74.000859 |
| plain negative-AND | 9908 | 72.797593 |
| positive OR | 9908 | 72.797593 |
| `int o = a||b||c||d` | 9904 | 70.549419 |
| `int o = a||b||c; if (d) o=1` | 9908 | 65.415473 |
| `bool o = a||b||c||d` | 9904 | 67.824340 |
| `tail_before_dup` (tail before recompute) | 9948 | 70.293486 |
| plain `otherLock` + `tail_before_dup` | 9948 | 70.322119 |
| `nodup` | 9756 | 62.314026 |
| `nodup` + 6-call asteroid | 9776 | 62.393533 |
| `nodup` + 6-call asteroid (baseline) | 9928 | 72.154617 |
| G1 distance-first `&&` (vpush d8-d13, frame 296) | 9928 | 64.474625 |
| G1 + plain `otherLock` | 9928 | 64.399028 |
| G1 + 6-call asteroid | 9928 | 64.533638 |
| F1 distance-first nested (vpush d8-d13, frame 304) | 9936 | 62.358179 |
| `float cx/cy` bounds-first | 9908 | 70.422535 |
| `int cx/cy` distance-first nested | 9936 | 62.415625 |
| `float dist` bounds-first | 9908 | 63.115693 |

No variant beat **9908 B / 74.029509 %**.

## 6. Proven vs hypothesis

**Proven (measured):**
* The landed nested-expect `(n,1,n,0)` is the unique maximum of all 256
  `__builtin_expect` polarity combinations on the first `otherLock` chain.
* The original's first chain is the positive-OR `cbnz` shape. The `cbnz` shape
  is reachable from source (`int o = a||b||c; if (d) o = 1;`), but its branch
  target and the resulting block layout differ, and the score falls to 65.4 %.
* The original's `vpush {d8-d13}` is caused by the distance-first in-sight
  block keeping `crosshairPos.x/.y` in `s26`/`s24` across `Vector::operator-`.
  The faithful shape (`float cx/cy`, distance-first, `inSight = &&`) reaches
  `vpush {d8-d13}` and `sub sp,#296`, but the first stack slot is `[sp,#108]`
  (8 B extra) and the block layout shifts, costing ~10 points.
* The baseline's call multiset is 352 vs the original's 342. The 10-call
  surplus is exactly the post-loop recompute; `nodup` gives 339 vs 342, the
  three remaining misses being the 6-call asteroid predicate (2) and the
  landmark else-branch lock-label `DrawString` (1).
* The gas-cloud `DrawString` (source line 998) is present in both bodies and
  lands at the same address (`0x16e6`). The one absent call site is the
  landmark else-branch lock label, tail-merged into the `i == 0 &&
  !alienOrbit` branch's call at `0xaa2`.

**Hypothesis (not proven):**
* The original source is the faithful `nodup` + 6-call asteroid + unmerged
  landmark else-branch `DrawString` shape. Its block placement (landmark loop hot at `0x758`) is
  only reachable if the code between the planet tail and the landmark body is
  the original's inline planet tail + literal pool (~0x5e B), not the 0x52 B
  duplicate recompute. Replacing the recompute with the planet tail
  (`tail_before_dup`) puts the tail at `0x6fa`, but then the recompute (still
  present) pushes the landmark body to `0x798` and the score drops to 70.29 %.
* The remaining ~130 B gap in `nodup` is non-call code (the landmark loop's
  hot placement + the 6-call asteroid predicate's branchy form + the
  unmerged landmark else-branch `DrawString` setup), not another missing
  statement.

## 7. Evidence / scratch

All scratch under `/dev/shm/w5806` (not committed): `Radar.base.cpp` (pristine
baseline source), `measure.py`, `run.sh`, `count.py`, `diffregions.py`,
`diffshow.py`, `layout.py`, `blocks.py`, `shape.py`, `mkdis.py`, `calls.py`,
`callsites2.py`, `callseq.py`, `sweep_exp.py` (256-combo sweep),
`sweep_exp.json`, `batch2.py`..`batch12.py`, `orig.dis`, `ours.dis`,
`orig.blocks`, `ours.blocks`, `base_regions.txt`, `nodup_regions.txt`,
`rea.json`, `ghidra.c`.
