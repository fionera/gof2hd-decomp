# Radar::draw pass 10 — cold-tail placement is a source-shape lever, but
# the first `otherLock` chain is a *global* MachineBlockPlacement artifact (w4271)

**Status:** research note, 2026-11-03. Worktree `wt/w4271`, branch `wave92-w4271`,
base `b379d00d`. **No source change landed**: no variant that keeps
`Radar::draw` at 9908 bytes beats the baseline's verifier ratio
(0.739255). This commit contains this note only.

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

(The wave-92 brief's `byte 4498` is stale; the current tree is 4499.)

`_ZN5Radar4drawEP6PlayerP3Hudi`:

```
osize 9908  usize 9908  byte False linked False
official ratio 0.739255 (73.9255 %)  normalized stream 3492 (orig) / 3488 (ours)
```

`Radar.cpp` carries the scoped `-mllvm -likely-branch-weight=80
-mllvm -unlikely-branch-weight=1` override (`CMakeLists.txt:86-87`).

## 1. The lever: the positive-OR spelling moves the cold `otherLock` block
##    into the original's cold tail

The current source spells the first `otherLock` as

```cpp
int otherLock;
if (__builtin_expect(this->lockedAsteroid == nullptr && this->lockedGasCloud == nullptr && // lint: arm_builtin
                     this->field_0x1c == nullptr,
                     0)) {
    otherLock = this->field_0x14 != 0;
} else {
    otherLock = 1;
}
```

Replacing it with the semantically identical **positive-OR** form

```cpp
int otherLock;
if (this->lockedAsteroid != nullptr || this->lockedGasCloud != nullptr ||
    this->field_0x1c != nullptr) {
    otherLock = 1;
} else {
    otherLock = this->field_0x14 != 0;
}
```

(no `__builtin_expect`) moves the cold `otherLock = field_0x14 != 0` block
from its far-away home (baseline `0x2480`) into the cold tail, immediately
before the `lockedStation` and `missionBlocksPlanets` cold blocks — exactly
the original's placement. Measured region map (function-relative offsets;
original base `0x12e050`):

| block | original | baseline | **positive-OR** |
|---|---:|---:|---:|
| `otherLock = f14 != 0` cold | `0xb72` | **`0x2480`** | **`0xb3a`** |
| `otherLock = lockedStation != 0` cold | `0xb7e` | `0xb3a` | `0xb4c` |
| `missionBlocksPlanets` cold | `0xb8a` | `0xb46` | `0xb58` |
| `planetLockTimer = 0; f18 = 0` | `0xbcc` | `0xbda` | `0xbea` |
| `enemies:` | `0xbd4` | `0xbe6` | `0xbf6` |

So the cold-tail *order* is now `f14`, `lockedStation`, `missionBlocksPlanets`,
`f14==0`, `enemies:` — the original's order. The whole tail is shifted by
`-0x38` because the first `otherLock` hot chain is still `0x24` bytes longer
than the original's.

**But the ratio drops**: `usize 9904`, ratio **0.721697** (72.1697 %),
stream 3492/3486. The 4-byte shrink and the lower score are both caused by
the same thing: the hot chain. The cold-tail placement alone is not a win.

## 2. The hot chain is the real residual — and it is not a source shape

Original first `otherLock` chain (`0x2ac..0x2c4`, 10 instructions):

```
2ac  mov   r1, r4
2ae  ldr.w r0, [r1, #12]!      ; &lockedAsteroid, load
2b2  str   r1, [sp, #124]
2b4  cbnz  r0, 0x2c2           ; asteroid != 0 -> done
2b6  ldr   r0, [r4, #56]       ; lockedGasCloud
2b8  cbnz  r0, 0x2c2           ; gas != 0 -> done
2ba  ldr   r0, [r4, #28]       ; field_0x1c
2bc  cmp   r0, #0
2be  beq.w 0xb72               ; f1c == 0 -> cold
2c2  movs  r5, #1              ; done: otherLock = 1
2c4  ldr.w r0, [r4, #312]
```

Baseline (and every no-`expect`/positive-OR spelling), 14 instructions plus
an inline pool:

```
2ac  mov   r1, r4
2ae  ldr.w r0, [r1, #12]!
2b2  str   r1, [sp, #124]
2b4  cbz   r0, 0x2d4           ; asteroid == 0 -> continue
2b6  mov.w r9, #1              ; done (r9, not r5)
2ba  ldr   r5, [sp, #156]
2bc  b.n   0x2e8
2be  nop
2c0  <5-word constant pool>
2d4  ldr   r0, [r4, #56]
2d6  ldr   r5, [sp, #156]
2d8  cmp   r0, #0
2da  itt   eq
2dc  ldreq r0, [r4, #28]
2de  cmpeq r0, #0
2e0  beq.w 0x2480              ; cold (far)
2e4  mov.w r9, #1
2e8  ldr.w r0, [r4, #312]
```

Two independent codegen differences:

1. **The last two tests are if-converted** (`itt eq; ldreq; cmpeq`) instead
   of two separate `cbnz`. The `done` block is placed *after the first test*
   (fallthrough) instead of after the whole chain, and the `b.n 0x2e8` it
   emits is what makes clang drop the first constant island at `0x2be`
   (original: `0x2ee`). The pool shift is therefore a *consequence* of the
   block placement, exactly as w4223 §12 concluded.
2. **`otherLock` lives in `r9`, not `r5`.** `mov.w r9,#1` is 4 bytes;
   `movs r5,#1` is 2 bytes. The original's cold block is `ldr r5,[r4,#20];
   cmp r5,#0; it ne; movne r5,#1`; ours is `ldr.w r9,[r4,#20]; cmp.w r9,#0;
   it ne; movne.w r9,#1`, so even the relocated cold block does not match.

### 2.1 The IR is identical for the two `otherLock` chains

This is the key new evidence. The **second** `otherLock` is already a plain
4-operand OR in the source:

```cpp
otherLock = this->lockedAsteroid != nullptr || this->lockedGasCloud != nullptr ||
            this->field_0x1c != nullptr || this->lockedStation != nullptr;
```

and it compiles to the original's `cbnz/cbnz/cmp/beq` chain in *our* build
(`0x326..0x338`). If the first `otherLock` is written the same way
(`int otherLock = a || b || c || d;`, the `or4decl` variant), the two chains
have **byte-for-byte identical IR shape** (`-emit-llvm -S`, `-Oz`, NDK r18b
clang 7.0.2):

```
; first otherLock (blocks 251..267)
251: %253 = load asteroid; %254 = icmp eq null; br %254, label %255, label %267
255: %257 = load gas;      %258 = icmp eq null; br %258, label %259, label %267
259: %261 = load f1c;      %262 = icmp eq null; br %262, label %263, label %267
263: %265 = load f14;      %266 = icmp ne null; br label %267
267: %268 = phi i1 [ true, %259 ], [ true, %255 ], [ true, %251 ], [ %266, %263 ]

; second otherLock (blocks 671..687)
671: %673 = load asteroid; %674 = icmp eq null; br %674, label %675, label %687
675: %677 = load gas;      %678 = icmp eq null; br %678, label %679, label %687
679: %681 = load f1c;      %682 = icmp eq null; br %682, label %683, label %687
683: %685 = load lockedStation; %686 = icmp ne null; br label %687
687: %688 = phi i1 [ true, %679 ], [ true, %675 ], [ true, %671 ], [ %686, %683 ]
```

No `!prof` on either chain in this spelling. Yet the backend lays the first
chain out `cbz + IT` (done early) and the second `cbnz/cbnz/cmp/beq` (done
after the chain). **The difference is a global MachineBlockPlacement /
IfConverter decision, not a local source shape.** That is why 17 `otherLock`
spellings (w4223 §12.1) and the 30-spelling permutation sweep in §4 below
all collapse to the same two machine layouts.

### 2.2 `__builtin_expect(...,0)` is what pins the cold block far away

Removing the hint (any of the positive-OR / plain-`&&` spellings) moves the
cold block to the tail but does **not** shorten the hot chain. Keeping the
hint pins the cold block at `0x2480` but does not shorten the hot chain
either. The two effects are orthogonal:

| source | `expect` | usize | ratio | cold `f14` block | hot chain |
|---|---|---:|---:|---:|---|
| baseline `&&` | `0` | 9908 | **0.739255** | `0x2480` (far) | `cbz`+IT |
| baseline `&&` | none | 9904 | 0.721697 | `0xb3a` (tail) | `cbz`+IT |
| positive OR | none | 9904 | 0.721697 | `0xb3a` (tail) | `cbz`+IT |
| positive OR | `1` | 9908 | 0.739255 | `0x2480` (far) | `cbz`+IT |
| positive OR | `0` | 9912 | 0.707825 | far | `cmp`+IT |

The `expect(...,1)` rows are byte-identical to the baseline (SHA-1 of the
extracted body), i.e. the positive-OR spelling is a no-op unless the hint is
dropped.

## 3. A `cbnz` chain *is* reachable — but only for the second `otherLock`

Reordering the three conditions in the positive-OR form changes which chain
gets the `cbnz` layout. The sweep of all six permutations (`A` = asteroid,
`B` = gas cloud, `C` = `field_0x1c`), positive-OR form, no `expect`:

| order | usize | ratio | first chain | second chain |
|---|---:|---:|---|---|
| `ABC` | 9904 | 0.721697 | `cbz`+IT | `cbnz/cbnz/cmp/beq` |
| `ACB` | 9904 | 0.721697 | `cbz`+IT | `cbnz/cbnz/cmp/beq` |
| `BAC` | 9892 | 0.713138 | `cbz`+IT (gas first) | **`ldr.w r0,[r1,#12]!` + `cbnz/cbnz/cmp/beq`** |
| `BCA` | 9892 | 0.713138 | `cbz`+IT (gas first) | same as `BAC` |
| `CAB` | 9928 | 0.668481 | `cbz`+IT | `cbnz/cbnz/cmp/beq` |
| `CBA` | 9928 | 0.668481 | `cbz`+IT | `cbnz/cbnz/cmp/beq` |

The `BAC`/`BCA` rows are notable: the **second** chain now uses the
write-back `ldr.w r0,[r1,#12]!` + `str r1,[sp,#108]` idiom and the full
`cbnz/cbnz/cmp/beq` layout — i.e. it reproduces the original's chain shape
verbatim. The first chain still does not. This is further proof that the
layout choice is global: the same chain source produces the original shape
at one site and the `cbz`+IT shape at the other.

## 4. The "cold tail without the duplicate campaign block" variants

The w4223/w4256 `delcamp` source removes the duplicate `if (!alienOrbit)`
autopilot block after the planet loop. Combining `delcamp` with the
cold-tail lever gives the best semantically-correct scores seen so far, but
none is size-exact (the original is 9908):

| source | `onScreen` | usize | ratio |
|---|---|---:|---:|
| baseline (duplicate present) | `expect 1` | 9908 | **0.739255** |
| delcamp | `expect 1` | 9740 | 0.622903 |
| delcamp | plain | 9740 | 0.665126 |
| delcamp + positive OR | plain | 9728 | 0.672924 |
| **delcamp + `or4decl`** | **plain** | **9816** | **0.683490** |
| delcamp + `or4decl` | `expect 1` | 9824 | 0.634527 |
| delcamp + A+B+C (w4105) | `expect 1` | 9784 | 0.610125 |

So the cold-tail lever is worth **+1.8 points** on the single-autopilot
`delcamp` source (0.665126 → 0.683490), which is a real source-shape win, but
the result is still 92 bytes short of the original and 5.6 points below the
baseline's local optimum. It cannot land under the size-exact gate.

## 5. Swapping the duplicate block and the planet-lock tail

The w4256 note's blocker is that the campaign-loop exit (`bcs.w`) lands on
the duplicate autopilot block in the baseline, but on the planet-lock tail
in the original. Moving the planet-lock tail *before* the duplicate block in
the source (same statements, same semantics) fixes that:

```
swap variant:  campaign-loop exit bcs.w 0x6fa -> planet-lock tail 0x6fa..0x744
               (original: bcs.w 0x6f8 -> planet-lock tail 0x6f8..0x748)
```

and the normalized stream length becomes **3492/3492 — exactly the
original's instruction count**. But the duplicate block then sits between
the planet-lock tail and the landmark body (`0x746..0x796`), pushing the
landmark body to `0x798` (original `0x758`), and the function grows to
**9948 bytes / 0.701890**. Removing the duplicate shrinks it further. This
is the clearest demonstration that the duplicate block is load-bearing only
because it occupies the slot the original's *landmark body* occupies.

## 6. Everything else tried this pass (all `usize 9908` unless noted)

| variant | usize | ratio |
|---|---:|---:|
| baseline | 9908 | **0.739255** |
| `!(x > 24000) && !(x < -24000)` in-range rewrite (§w4223) | 9908 | 0.739255 (neutral) |
| `otherLock` type: `unsigned`/`uint32_t`/`long`/`char`/`short`/`int8_t` | 9908 | 0.739255 (neutral) |
| `marked` as `bool` | 9928 | 0.708901 |
| `notOtherLock`/`notTurretMode` inlined | 9936 | 0.701131 |
| `otherLock` block moved after `landmarkHit` decl | 9908 | 0.739255 (byte-identical) |
| `otherLock` block moved before the route block | 9844 | 0.699785 |
| `(i & 0x7fffffff)==1\|\|...` hint `expect 1` | 9908 | 0.739255 (neutral) |
| `(i & 0x7fffffff)==1\|\|...` hint removed | 9912 | 0.721100 |
| positive OR + `onScreen` plain | 9892 | 0.708698 |
| positive OR + `onScreen` `expect 0` | 9860 | 0.653763 |
| positive OR + in-range rewrite | 9904 | 0.721697 |
| positive OR + distance-first in-sight (C) | 9920 | 0.710952 |
| positive OR + asteroid `dead` predicate (B) | 9924 | 0.711525 |
| positive OR + `-tail-merge-size=10` | 9916 | 0.717640 |
| positive OR + `-disable-ifcvt-triangle` | 9904 | 0.720837 |
| positive OR + `-ifcvt-limit=0` | 9920 | 0.691602 |
| positive OR + `-disable-early-taildup` | **9908** | **0.724276** |
| positive OR + `-disable-branch-fold` | **9908** | 0.719198 |
| positive OR + `-force-loop-cold-block` | 9992 | 0.536195 |
| positive OR + `-enable-misched` | 9976 | 0.539477 |
| `and_plain` + branch-weight sweep `L=1..2000` | 9896-9904 | ≤0.725892 |
| `or4decl` (4-operand direct OR) | 9900 | 0.708381 |
| `or4decl` + `onScreen` plain | 9816 (delcamp) | 0.683490 |
| swap planet tail / duplicate | 9948 | 0.701890 |
| swap + in-range rewrite | 9948 | 0.701890 |

No variant that keeps `usize 9908` beats **0.739255**.

## 7. Conclusion

* The **cold-tail placement is a real, reachable source-shape lever**: the
  positive-OR spelling with no `__builtin_expect` puts the
  `otherLock = field_0x14 != 0` cold block at the head of the cold tail
  (`0xb3a`, original `0xb72`) instead of `0x2480`. It is worth +1.8 points
  on the `delcamp` source.
* It is **not** worth anything on the duplicate-bearing baseline, because
  the first `otherLock` hot chain stays `0x24` bytes long and the pool shift
  dominates.
* The hot chain is a **global MachineBlockPlacement/IfConverter artifact**,
  not a local source shape: the first and second `otherLock` chains have
  identical IR (proved by `-emit-llvm -S`) yet different machine layouts in
  our build. `BAC`/`BCA` condition orders prove the original `cbnz` layout is
  reachable at the second site with the same source.
* The duplicate campaign block is load-bearing only because it occupies the
  slot the original's landmark body occupies; swapping the planet-lock tail
  in front of it reproduces the original's campaign-loop exit and the
  original's instruction count (3492/3492), but grows the function to 9948.
* No source variant with `usize 9908` beats 0.739255, so no code change was
  landed.

### What remains

1. Find the global layout knob that makes the *first* `otherLock` chain use
   the `cbnz/cbnz/cmp/beq` layout while the second already does. The IR is
   identical, so this is a MachineBlockPlacement/IfConverter frequency
   decision; the `!prof` weights and the `-mllvm` block-placement flags
   tried so far do not reach it.
2. Once the first chain is short, the cold block naturally lands at the
   head of the cold tail and the pool shifts to `0x2ee`; then re-check the
   stack-slot swap (`mode` `[sp,#100]` vs `[sp,#112]`) and the
   `vpush {d8-d13}` / `s24`/`s26` shape.
3. The `delcamp + or4decl + onScreen plain` shape (9816 B, 0.683490) is the
   best semantically-correct candidate; it needs ~92 bytes of real
   block-placement code before it can be size-exact.

## 8. Evidence / scratch

Scratch lives in `/dev/shm/w4271/` (not committed):

* `measure.py` — official-API ratio + stream lengths for one symbol.
* `dumpraw.py` — raw objdump of one symbol (original or ours).
* `streamdiff.py` / `streamdiff2.py` — opcode-level diff of the normalized
  streams.
* `variants.py`, `oracle.py`, `perm.py`, `sweep.py`, `exp.py`, `more.py`,
  `type.py`, `type2.py`, `combo.py`, `basecombo.py`, `dc.py`, `abc.py`,
  `swapvar.py`, `move.py`, `flagtest.py`, `wsweep.py` — the variant
  harnesses (each patches `src/game/weapons/Radar.cpp`, rebuilds only
  Radar.cpp + link, measures).
* `Radar.base.cpp` — the landed baseline source.
* `Radar.ll`, `Radar_or4.ll`, `Radar_ap.ll` — `-emit-llvm -S` IR dumps.
* `orig.txt`, `ours.txt`, `ours_or.txt`, `ours_bac.txt`, `ours_swap.txt`,
  `ours_dc_or4.txt` — objdump bodies.
* `Radar.swap.cpp` — the planet-tail/duplicate swap source.
* `CMakeLists.base` — the unmodified CMakeLists (restored after flag tests).
