# LevelScript::process — iOS-structure + data-flow pass (wave 189, w13309)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13309`, branch
`wave189-w13309`, base master `247466fde` (wave-188 harvest, raw **51,917**,
`.text` 74,047, `.ARM.extab` 639).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13309`.

Continuation of the wave-183..188 iOS-structure passes (w12704..w13204) and the
wave-182..188 data-flow passes.  This pass re-diffs the iOS 1.1.4 Mach-O and the
original source-layout dumps, checks the operator-overload call census against
the original, and runs a fresh **comparison-operand swap sweep** over the whole
`process` body (a data-flow family not swept before: only one compare site had
ever been measured).

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,917           51,915     (-2)
process aligned insns    20,935           20,935
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          74,047           74,045     (-2)
.ARM.extab diffbytes     639              639        (size/base exact @0x1a3b78, 156,312 B)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
stub_zero_size           0                0
missing / wrong_type     0 / 0            0 / 0
extra                    0                0
RATCHET                  PASS             PASS
```

## Landed changes (2 independent comparison-operand swaps, compose to -2)

| # | source line | before | after |
|---:|---:|---|---|
| 1 | 1325 | `for (unsigned int i = 3; i < enemies->count; i++)` | `for (unsigned int i = 3; enemies->count > i; i++)` |
| 2 | 2581 | `getEmpPoints() < getMaxEmpPoints()` | `getMaxEmpPoints() > getEmpPoints()` |

Both are strict comparisons of pure operands; swapping the operands and the
operator is semantics-preserving (same NaN behaviour for the float-free int
compares, same short-circuit-free evaluation).  Measured on the wave-188 body:
each alone is raw **51,917 -> 51,916** (-1), both together **51,915** (-2),
size 57,232/57,232 exact, `.rel.plt` unchanged.

## Operator-overload call census (new structural evidence)

Direct `bl`/`blx` targets in the process body are directly comparable between
the original and ours (both link every function at its original address and
the `.plt` is byte-identical).  The census shows the two scalar/vector
multiply overloads are the only *structural* call-count mismatch:

| callee | original | ours |
|---|---:|---:|
| `AEMath::operator*(const Vector&, float)` (`0x5ec74`) | 29 | 26 |
| `AEMath::operator*(float, const Vector&)` (`0x62658`) | 1 | 4 |

The original's single `float * Vector` call is at `0x13c2f6`, immediately
after `AEMath::VectorNormalize(player->getPosition())` and before the
`m_tempVec = …` store — i.e. source line 2153
(`m_tempVec = 5000.0f * VectorNormalize(...)`).  The other three
`float * Vector` sites in our source should be `Vector * float`:

* line 2504 `camPos += 3000.0f * dir;` — original `0x139a66` calls `0x5ec74`
  (`Vector,float`) with the scalar in `r2`, i.e. `dir * 3000.0f`;
* line 3821 `geo->translate(-30000.0f * right);` — original `0x142b86` calls
  `0x5ec74` with `r2 = -30000.0f`, i.e. `right * -30000.0f`;
* line 3181 `… - 2400.0f * player->GetDirVector()` — original `0x143b90`
  calls `0x5ec74` with `r2 = 2400.0f`, i.e. `player->GetDirVector() * 2400.0f`.

All 8 combinations of these three corrections were measured: **raw is exactly
51,917 in every case** (size-exact, `.rel.plt` identical).  The call-target
bytes are already mismatched at the shifted addresses, so the correction is
structurally right but metric-neutral; it is recorded here rather than landed
alone.  (The wave-187 w13103 change at line 2504 is therefore the wrong
overload; it is kept only because reverting it is also metric-neutral.)

## Comparison-operand swap sweep (method)

`scratch/w13309/cmp_swap.py` parses every top-level `a < b`, `a > b`,
`a <= b`, `a >= b`, `a == b`, `a != b` in the `process` body (balanced
operand scan, casts and `->` chains handled), rewrites it to
`b <op'> a` (`<`↔`>`, `<=`↔`>=`, `==`/`!=` unchanged), and measures it with
the exact NDK r18b `-Oz` compile + gold link (`scratch/w13309/fast.py`,
~1.5 s/candidate).  595 sites are being measured; the two landed sites are
the first wins found.  Results accumulate in `scratch/w13309/cmp_res.json`.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13309
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51915 aligned= 20935 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```
