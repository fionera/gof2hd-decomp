# Radar::draw pass 16 (wf102) — otherLock IR proof, faithful inSight shape, and the
# 9908-byte local optimum

**Status:** research note, 2026-11-04. Worktree `wt/wf102`, branch `wave101-wf102`,
base `3856949ab`. **No source change landed** (the landed source is byte-identical to the
base): every variant tried is size- or ratio-worse than the 9908 B / 73.868195 % baseline.
This commit contains this note only.

## 0. Baseline (confirmed before and after)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN5Radar4drawEP6PlayerP3Hudi` = `osize 9908 usize 9908 byte False linked False`,
raw verifier ratio **73.868195 %**, normalized stream 3492 (orig) / 3488 (ours).
(`tools/verify/verify.py`'s `score()` rounds to 73.9; the raw ratio is
`difflib.SequenceMatcher(..., autojunk=False).ratio()*100`.)

## 1. The `otherLock` chain: the source IR is already correct; the ARM backend
##    if-converts it (PROVEN)

The original's first chain is a textbook 3-way short-circuit:

```
0002b4  cbnz r0, 0x2c2        ; lockedAsteroid != null
0002b6  ldr  r0, [r4, #56]    ; lockedGasCloud
0002b8  cbnz r0, 0x2c2
0002ba  ldr  r0, [r4, #28]    ; field_0x1c
0002bc  cmp  r0, #0
0002be  beq.w 0xb72           ; all null -> otherLock = field_0x14 != 0 (cold)
0002c2  movs r5, #1           ; otherLock = 1
```

`otherLock` lives in **r5** and the cold block is 12 bytes (`ldr r5,[r4,#20]; cmp;
it ne; movne r5,#1; b`). The base emits a different chain:

```
0002b4  cbz  r0, 0x2d4        ; a == null -> evaluate b/c
0002b6  mov.w r9, #1          ; otherLock = 1   (r9, not r5)
...
0002d4  ldr  r0, [r4, #56]
        cmp  r0, #0
        itt  eq
        ldreq r0, [r4, #28]
        cmpeq r0, #0
        beq.w 0xb3a
        mov.w r9, #1
```

The cold block is 14 bytes (`ldr.w r9; cmp.w r9; it ne; movne.w r9; b`) and the
literal-pool island therefore lands at `0x2c0` instead of `0x2f0`.

**This is not a source-shape error.** The optimized LLVM IR emitted for the
current source already has exactly the original's CFG (produced with
`clang++ ... -S -emit-llvm src/game/weapons/Radar.cpp`):

```llvm
; <label>:251:
  %253 = load %class.KIPlayer*, ... getelementptr i32 3   ; lockedAsteroid
  %254 = icmp eq %class.KIPlayer* %253, null
  br i1 %254, label %255, label %268
; <label>:255:
  %257 = load %class.KIPlayer*, ... i32 14                ; lockedGasCloud
  %258 = icmp eq %class.KIPlayer* %257, null
  br i1 %258, label %259, label %268
; <label>:259:
  %261 = load %class.KIPlayer*, ... i32 7                 ; field_0x1c
  %262 = icmp eq %class.KIPlayer* %261, null
  br i1 %262, label %263, label %268, !prof !7857         ; !7857 = {1, 2000}
; <label>:263:
  %265 = load %class.KIPlayer*, ... i32 5                 ; field_0x14
  %266 = icmp ne %class.KIPlayer* %265, null
  %267 = zext i1 %266 to i32
  br label %268
; <label>:268:
  %269 = phi i32 [ %267, %263 ], [ 1, %251 ], [ 1, %255 ], [ 1, %259 ]
```

That is the original's shape. The difference appears in the ARM backend: the
`ARMIfConverter` predicates block `%259` into block `%255`
(`itt eq; ldreq; cmpeq; beq`), which forces `otherLock` out of r5 into r9 and
moves the pool island. The same IR compiled in isolation (a 4-field struct and
the identical `&&` chain, with or without `__builtin_expect(...,0)`, with
branch weights 80:1 or the default 2000:1) produces the original's `cbnz` chain,
so the if-conversion is a whole-function profitability decision, not a
statement-level one.

Sweep of the three `&&` operands (A = `lockedAsteroid == nullptr`,
B = `lockedGasCloud == nullptr`, C = `field_0x1c == nullptr`):

| order | usize | raw ratio |
|---|---:|---:|
| **A B C** (landed) | **9908** | **73.868195** |
| A C B | 9908 | 73.868195 |
| B A C | 9896 | 70.863206 |
| B C A | 9896 | 70.863206 |
| C A B | 9932 | 66.313377 |
| C B A | 9932 | 66.313377 |

The `__builtin_expect(...,0)` wrapper is load-bearing for the size: removing it
gives 9904 B / 72.169676 % (same `itt eq` chain). Positive-OR / nested-if /
`bool otherLock` spellings were already swept in pass 15 and re-confirmed here
(`bool otherLock` = 9876 B / 67.796123 %).

## 2. The enemy `inSight` block: distance-first is the faithful shape (PROVEN),
##    but it costs 16 bytes

The original's `!onScreen` branch computes the crosshair ints first, then calls
`operator-`/`VectorLength`, then does the four bounds checks:

```
000efc  ldr  r0, [pc, ...]        ; PlayerEgo::crosshairPos
000f0a  vldr s26, [r0]            ; crosshairPos.x
000f0e  vldr s24, [r0, #4]        ; crosshairPos.y
000f18  blx  AEMath::operator-
000f1e  vcvt.s32.f32 s24, s24
000f22  vcvt.s32.f32 s26, s26
000f26  blx  AEMath::VectorLength
000f30  vcmpe.f32 s0, s20         ; < 60000.0f
000f38  bpl  skip
000f3a  vmov r1, s26              ; cx (kept across both calls in s26)
000f3e  ldr.w r0, [r4, #292]      ; field_0x124
000f42  ldr.w r2, [r4, #252]      ; screenX
...                                ; screenX > cx-size && screenX < cx+size
000f54  vmov r1, s24              ; cy (kept in s24)
000f58  ldr.w r2, [r4, #256]      ; screenY
...                                ; screenY > cy-size && screenY < cy+size
```

Because `cx`/`cy` are live across two calls, the original pushes
`vpush {d8-d13}` (s24/s26 = d12/d13). The landed source is bounds-first, so
`cx`/`cy` die before the call and the prologue is `vpush {d8-d11}`.

Reordering the source to distance-first (x-pair then y-pair) reproduces the
original's prologue and the block almost instruction-for-instruction:

```
000008  ed2d 8b0c  vpush {d8-d13}          ; matches original
00000e  931a       str   r3, [sp, #104]    ; original sp#100, landed sp#112
000f54  vmov r2, s26                       ; orig r1, s26  (reg swap)
000f58  ldr.w r0, [r4, #292]
000f5c  ldr.w r1, [r4, #252]               ; orig r2
000f60  subs r3, r2, r0
000f62  cmp  r1, r3
000f64  ble
000f66  ldr  r6, [sp, #144]                ; extra load (orig has movs r6,#0)
000f68  add  r2, r0
...
```

The residual is register allocation (`cx` in r2 vs r1, `inSight` in r0 vs r6,
two extra `ldr r6,[sp,#144]`), not statement order. But the variant is
**9924 B** (+16), which shifts every later block and drops the raw ratio:

| variant | usize | raw ratio | norm |
|---|---:|---:|---:|
| landed (bounds-first) | **9908** | **73.868195** | 3492/3488 |
| distance-first, x-pair then y-pair | 9924 | 71.275225 | 3492/3495 |
| distance-first, interleaved x/y | 9952 | 63.165447 | 3492/3477 |
| distance-first + `int topOk` | 9924 | 71.217976 | 3492/3495 |
| distance-first + `bool marked` | 9944 | 70.074456 | 3492/3492 |
| distance-first + `bool otherLock` | 9896 | 47.990815 | 3492/3476 |

The +16 cannot be recovered by any of the type/order edits tried; the block
layout after `inSight` is what dominates the metric, and the landed bounds-first
shape happens to align it better.

## 3. Landmark hit test: `bool topOk` is already optimal

The original's four checks are all separate branches:

```
000844  vmov r3, s2                 ; left
000848  cmp  r0, r3   ; screenX > left
00084a  ble
00084c  vmov r6, s0                 ; top
000850  add.w r5, r6, r2, lsl #1    ; top + 2*size
000854  cmp  r1, r5   ; screenY < top+2*size
000856  bge
000858  cmp  r1, r6   ; screenY > top
00085a  ble
00085c  add.w r1, r3, r2, lsl #1    ; left + 2*size
000860  cmp  r0, r1   ; screenX < left+2*size
000862  bge
```

The landed source (`bool topOk = top < this->screenY;` then a flat `&&` chain)
emits `itt gt; addgt; cmpgt` for checks 3+4. Inlining `topOk` moves the
if-conversion to checks 1+3. Neither reproduces the all-separate original, and
all 24 orderings of the four conditions were measured:

* `bool topOk` + 24 orders: 73.6 – **73.868195** (ABCD best).
* inlined `topOk` + 24 orders: 73.6 – 73.810888 (ABCD best).
* `int topOk`: 73.810888 (same code as inlined).
* 2+2 and 4-level nested `if` spellings: 73.868195 (same `itt gt` block).

## 4. Type/expression micro-sweep (all size-neutral or worse)

| variant | usize | raw ratio |
|---|---:|---:|
| landed | **9908** | **73.868195** |
| `bool topOk` reversed (`screenY > top`) | 9908 | 73.868195 |
| `const bool topOk` | 9908 | 73.868195 |
| `bool topOk = (top < screenY) != 0` | 9908 | 73.868195 |
| `otherLock = (field_0x14 != 0) ? 1 : 0` | 9908 | 73.868195 |
| `otherLock = (int)(field_0x14 != 0)` | 9908 | 73.868195 |
| `otherLock = !!(field_0x14 != 0)` | 9908 | 73.868195 |
| `int topOk` | 9908 | 73.810888 |
| `int topOk = (top < screenY) != 0` | 9908 | 73.810888 |
| `bool alwaysEnemySeen` | 9908 | 71.422432 |
| `bool marked` (either declaration position) | 9928 | 70.890067 |
| `bool otherLock` | 9876 | 67.796123 |

## 5. Why the base is a local optimum and the faithful shape cannot be landed

The base's block layout is already within ~0x44 bytes of the original
throughout (pass 13/15 notes), and its music `play` tail already lands on the
original's last call addresses. The 26 % instruction mismatch is dominated by:

1. **Stack-slot colouring.** Original: `mode` sp#100, `marked` sp#132,
   `textOffset` sp#128, `stations` sp#116. Landed: `mode` sp#112,
   `textOffset` sp#116, `stations` sp#132, and `marked` is kept in **r10 (sl)**
   rather than a stack slot. Every `sp`-relative operand therefore differs even
   where the opcode sequence matches.
2. **Register allocation.** Original `otherLock` r5, `inSight` r6, `cx` r1;
   landed `otherLock` r9, `inSight` r0, `cx` r3.
3. **Two backend if-conversions** (`otherLock` b&&c, hit-test 3&&4) that the
   original did not take.

The semantically-faithful shape (drop the duplicate `if (!alienOrbit)`
autopilot block after the planet loop, restore the asteroid `dead` predicate,
distance-first `inSight`; the "A+B+C" patch of pass 13) is **9784 B**. It cannot
be landed because `-ffunction-sections` + the fixed link order means any size
change shifts every later function's PC-relative literal-pool load, and
`byte_exact` is a raw-byte count: pass 13 measured byte_exact 4498 -> 2701 for
the 9784 B shape. The 9908 B size is therefore a hard constraint on this
function until the whole `.text` layout is re-solved.

## 6. Conclusion

* The `otherLock` source is correct at the IR level; the remaining difference is
  an ARM backend if-conversion, not a statement error.
* The enemy `inSight` distance-first order is the faithful shape and restores
  the original's `vpush {d8-d13}`, but costs 16 bytes and 2.6 raw points.
* Every size-neutral variant tried is <= 73.868195 %. No variant beats the
  landed 9908 B baseline, so nothing is landed.

## 7. Evidence / scratch (all under `/dev/shm/wf102`, not committed)

* `Radar.base.cpp` — pristine landed source (byte-identical to git HEAD).
* `bm.sh` — `link_so` + measure; `measure.py` / `measure6.py` — official
  verifier API, 1-digit / 6-digit raw ratio.
* `dump.sh`, `rel.py`, `idiff.py`, `showidx.py`, `opcodes.py` — function-relative
  disassembly and instruction-level diff.
* `orig.dis(.rel)`, `base.dis(.rel)`, `vdist.dis(.rel)`, `vtop.dis(.rel)`,
  `vn2.dis(.rel)`, `vbm.dis(.rel)`, `nodup.dis(.rel)` — side-by-side dumps.
* `radar.ll` — `clang++ -Oz -S -emit-llvm` of the whole TU (the `otherLock`
  CFG is at labels 251–268).
* `final_measure.py` / `final_measure.json`, `micro.py`, `perm.py`, `perm2.py`,
  `perm3.py`, `perm_exact.py`, `grid.py` — the sweeps above.
* `/dev/shm/wf102/mini/t*.cpp` — isolated reproductions of the `otherLock`
  chain (they produce the original's `cbnz` shape at every weight).
