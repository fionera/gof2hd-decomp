# `Gun::calcCharacterCollision` wave 191b (w13766): 463/57 landed; the E1
# slot flip and the faithful C-D-E tail are measured, no further win

Worker: w13766. Branch `wave191b-w13766`, base master `b429d4f9c`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13766`.
Predecessor notes: `gun-calccollision-wave191b-w13752.md` (455 plateau,
StackColoring merge swap), `gun-calccollision-wave191b-w13736.md` (RA priority).

## 0. Result

**No further `src/` change landed this session.** The faithful C-D-E candidate
recorded by w13752 is already committed as pass 1 (`6c9cd5c23`), and this pass
confirms it is the best aligned shape measured so far:

| build | raw | aligned | size | insns | `.text` | `.ARM.extab` | `.ARM.exidx` |
|---|---:|---:|---:|---:|---:|---:|---:|
| HEAD (`6c9cd5c23`, faithful C-D-E) | **463** | **57** | 1984/1984 | 660/660 | 0x7c0 | 0x10 | 0x8 |
| previous committed body (E-C-D tail) | 455 | 93 | 1984/1984 | 660/660 | 0x7c0 | 0x10 | 0x8 |

Whole-`.so` section sizes are unchanged by this note-only pass:
`.text=0x69440`, `.ARM.extab=0x1a3b78`, `.ARM.exidx=0x19cc08`.
`check` was not re-run because `src/` is untouched; the last full gate remains
byte_exact 4508/4524, linked_exact 4508/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

## 1. E1: the StackColoring slot assignment is reachable from source

`E1` replaces the two-line playerPosition/delta computation with

```cpp
Vector delta = enemyPosition - ((Level *) this->level)->getPlayer()->getPosition();
int distance = (int) AbyssEngine::AEMath::VectorLength(delta);
```

and measures **467 raw / 67 aligned / 1984 / 660** (`scratch/e1b.so`). It is the
only shape found that reproduces the original's physical slot assignment:

| slot | original | HEAD (463/57) | E1 (467/67) |
|---|---|---|---|
| `sp+44` (fi#11, align 4) | `playerPosition`, mine temp, `MatrixSetTranslation` sret | `delta`, mine temp, MSTR sret | `playerPosition`, mine temp, MSTR sret |
| `sp+104` (fi#10, align 8) | `delta`, `view`, k-loop `getPosition` temp | `playerPosition`, `view`, k-loop temp | `delta`, `view`, k-loop temp |

Direct witnesses (function-relative `add rX, sp, #N`):

```
orig 0x142  add r4, sp, #44      ; getPosition sret  = playerPosition @ sp+44
orig 0x14c  add r5, sp, #104     ; operator- sret    = delta          @ sp+104
orig 0x438  add r5, sp, #104     ; view memcpy dst   = view           @ sp+104
orig 0x448  add r0, sp, #44      ; MatrixSetTranslation sret          @ sp+44
orig 0x4b4  add r4, sp, #104     ; k-loop getPosition sret            @ sp+104

HEAD 0x142  add r4, sp, #104     ; playerPosition @ sp+104
HEAD 0x14c  add r5, sp, #44      ; delta          @ sp+44
E1   0x142  add r5, sp, #44      ; playerPosition @ sp+44
E1   0x14c  add r4, sp, #104     ; delta          @ sp+104
```

`d2.py HEAD e1b.so` reports exactly **12 differing instructions**, all in
`0x13c..0x158`:

```
ORIG 013c mov r6, r4           HEAD 013c mov r6, r5        E1 013c mov r6, r5
ORIG 0142 add r4, sp, #44      HEAD 0142 add r4, sp, #104   E1 0142 add r5, sp, #44
ORIG 014c add r5, sp, #104     HEAD 014c add r5, sp, #44    E1 014c add r4, sp, #104
ORIG 0150 mov r2, r4           HEAD 0150 mov r2, r4         E1 0150 mov r2, r5
ORIG 0158 mov r0, r5           HEAD 0158 mov r0, r5         E1 0158 mov r0, r4
```

E1 therefore gets the **slot** assignment right but swaps the **register roles**:
the hoisted long-range `sp+104` address takes r4 in E1, while the original keeps
`i*3` in r4 and only borrows r4 for the short-lived `sp+44` playerPosition
address (saving/restoring `i*3` through r6 at `0x13c`/`0x1ca`).

`runs2.py` raw-byte decomposition: HEAD 463 in **44 runs**; E1 467 in **48
runs**. Both are identical outside `0x13c..0x158`; E1 pays six single-byte diffs
there (`0x143, 0x146, 0x14d, 0x150, 0x152, 0x158`) against HEAD's two
(`0x142, 0x14c`), i.e. **+4 raw / +10 aligned**. Shared residual runs:
`df ea fe 13c 1ca 1de 1e2 1ea 1f0 1fa 204 208(26) 223(158) 2c2(42) 438(4)
43e(40) 494 4a8 4b0(19) 4c4 4c6 4c8(2) 4cc 4ce 4d0(2) 4d4(13) 4e2 4e4(9)
4ef(16) 500(3) 504 506 508 50a 50c(26) 527(14) 536 538(39) 562(22) 67a 68e
6aa`.

## 2. MIR / RA mechanism (re-confirmed)

- After Local Stack Slot Allocation, HEAD has `%stack.10` shared by
  `playerPosition` and `view` (both `DBG_VALUE %stack.10`), and `%stack.11`
  shared by `delta` and the `MatrixSetTranslation` sret. E1 swaps this:
  `%stack.10` = `delta`/`view`, `%stack.11` = `playerPosition`/MSTR.
- MachineLICM hoists the last `t2ADDri %stack.10` (`%308` pre-LICM) from the
  view block to the i-loop preheader (`bb.5`); MCSE then folds the earlier
  `%stack.10` ADDris onto it. The hoisted value is live from the scatter branch
  (`%497`, 3604B) to the view block (`%498`, 15008B) — the ~14608-slot range.
- Greedy RA (`RAG.cpp` 725–742): `Prio = (1u<<29) + Size` for global ranges,
  ties by lower vreg. The ~14608 range beats `%99` (`i*3`, ~12832), so r4 goes
  to the hoisted stack address and `i*3` falls to r5. That cascades into the
  7 `i*3` bytes, the 226-byte i-loop phase shift, the view-block
  rematerialisation and the k-loop rotation.
- StackColoring runs **after** Greedy RA (`allf.err`: Greedy Register Allocator
  at 154593, Stack Slot Coloring at 157714), so the physical merge is not the
  RA cause; the cause is the LSSA-shared `%stack.N` plus LICM/MCSE.

## 3. Original structural witnesses (re-confirmed)

- **k-loop is header-first**: `movs r5,#0; b 0x34fc` at `0x4b0`, body at
  `0x4b2`, latch `adds r5,#1` at `0x4fa`, header `0x34fc..0x3504`, exit
  `b 0x3506` -> `0x25a`. Ours is body-first/rotated (`bcs.w` guard at `0x4b8`).
- **view block keeps the address in r5**: `add r5,sp,#104` once at `0x438` +
  `mov r0/r1/r2,r5`; ours rematerialises `add r0/r1/r4,sp,#104` three times
  (`0x43a/0x44c/0x45a`).
- **i reload**: original uses `ldr.w fp,[sp,#40]` at `0x208`/`0x26c`; ours uses
  `add r4,sp,#104` at `0x208`/`0x2d0` and compensates with an extra
  `ldr.w fp,[sp,#40]` at `0x2e8`.
- **k-loop body order is C-D-E**: original computes `diff = magnitude -
  distance`, clamps `falloff` (C), calls `damage` (D), then conditionally calls
  `damageEmp` (E) — `0x4ec..0x566`. This is exactly the committed body; the old
  455/93 body's E-C-D order is not faithful.
- Residual structural diffs HEAD vs ORIG: **46 folded / 57 aligned**.

## 4. Cells measured this pass (all `link_so -j2` + `tools/fn_diff.py`)

Size-exact 1984/660 and `.text=0x7c0`, `.ARM.extab=0x10`, `.ARM.exidx=0x8`
unless stated; none beats 463/57.

### E1-based shapes

| variant | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| **HEAD (committed)** | **463** | **57** | 1984 | 660 |
| E1 | 467 | 67 | 1984 | 660 |
| E1const / E1split / E1ctor | 467 | 67 | 1984 | 660 |
| E1viewctor / E1viewscope / E1viewref / E1viewptr | 467 | 67 | 1984 | 660 |
| E1paren / E1lvl / E1pp | 467 | 67 | 1984 | 660 |
| E1refval (`Vector playerPosition` + `Vector delta`) | 463 | 57 | 1984 | 660 |
| E1refval2 (`Vector delta; delta = ...`) | 1643 | 283 | 1964 | 661 |
| E1assign (`Vector delta; delta = ...`) | 1644 | 282 | 1964 | 661 |
| E1_decl_pevr | 512 | 90 | 1984 | 660 |
| E1_decl_evpr | 550 | 113 | 1984 | 660 |
| E1_decl_epvr | 550 | 110 | 1984 | 660 |
| E1_decl_vpre | 482 | 90 | 1984 | 660 |
| E1_radiusfirst | 1789 | 209 | 1968 | 656 |
| E1_viewrev | 476 | 80 | 1984 | 660 |
| E1_viewtrans_first | 467 | 67 | 1984 | 660 |
| E1hoist (`Vector delta;` at i-loop top) | 1709 | 246 | 1964 | 661 |
| E1hoistdummy | 1709 | 246 | 1964 | 661 |
| E1viewhoist (`Matrix view;` at i-loop top) | 1876 | 383 | 2024 | 682 |
| E1bothhoist | 1907 | 418 | 2048 | 692 |
| E1sub / E1sub2 (`delta -= ...`) | 1563 | 170 | 1992 | 662 |
| E1float (`float distance`) | 1596 | 318 | 1964 | 660 |
| E1revsub (`-playerPosition + enemyPosition`) | 1778 | 271 | 1956 | 658 |

### Faithful-tail sweep (HEAD base, `int diff` before/inside the guard)

`C` = falloff clamp, `D` = `damage`, `E` = `damageEmp`. The original order is
C-D-E (see §3).

| condition | diff pos | order | raw | aligned | size |
|---|---|---|---:|---:|---:|
| `magnitude > distance` | inside | **C-D-E** | **463** | **57** | 1984 |
| `magnitude > distance` | either | C-E-D | 460 | 89 | 1984 |
| `magnitude > distance` | either | C-E-D (sunk) | 462 | 78 | 1984 |
| `magnitude > distance` | either | D-C-E | 466 | 80 | 1984 |
| `magnitude > distance` | either | E-D-C / D-E-C | 1788/1796 | 167/157 | 1940 |
| `diff > 0` | before | C-E-D | 457 | 84 | 1984 |
| `diff > 0` | before | C-D-E | 458 | 63 | 1984 |
| `diff > 0` | before | E-C-D (old 455 body) | 455 | 93 | 1984 |
| `diff > 0` | before | D-C-E | 463 | 83 | 1984 |
| `diff > 0` | before | E-D-C / D-E-C | 1794/1803 | 171/160 | 1940 |

The E-C-D old body is still the raw minimum (455), but it is not the original
order; the faithful C-D-E body is the aligned minimum (57).

### View-block placement

| variant | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| HEAD | **463** | **57** | 1984 | 660 |
| view decl before the two `TransformGetTransform` calls (VM1) | 1162 | 120 | 1988 | 661 |
| view decl between the two `TransformGetTransform` calls (VM2) | 510 | 77 | 1984 | 660 |
| `Matrix view; view = ...` before the calls (VM3) | 1899 | 271 | 2064 | 689 |

## 5. Conclusion

E1 proves the StackColoring/LSSA slot swap is reachable from source, but it
costs +4 raw / +10 aligned because it also flips the greedy-RA order: the
hoisted `sp+104` address takes r4 instead of `i*3`. The real prize is a shape
that (a) keeps E1's slot assignment and (b) prevents the hoisted `sp+104`
address from out-ranking `i*3` in RA. The original does not hold `sp+104` in
one callee-saved register across the function: it recomputes
`add r5,sp,#104` inside the view block (`0x438`) and uses r4 for the k-loop
temp (`0x4b4`). That suggests the original's LSSA did **not** share one
`%stack.N` between the scatter-branch delta and the view alloca (so LICM/MCSE
did not create the long-range value), while StackColoring still merged the
physical slots after RA. A future pass should attack that LSSA sharing directly
(e.g. by finding the original declaration/scope shape for `delta`/`view`), not
the RA order.

The committed faithful C-D-E body (463 / **57** / 1984 / 660, `.text` 0x7c0,
`.ARM.extab` 0x10, `.ARM.exidx` 0x8) remains the best aligned candidate and is
kept.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13766
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Gun22calcCharacterCollisionEv
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -h cmake-build-match/CMakeFiles/gof2.dir/src/game/weapons/Gun.cpp.obj | \
    grep -E '\.text|\.ARM.extab|\.ARM.exidx'
```

Scratch helpers: `ex.py` (string-replacement rebuild + measure), `runs2.py`
(per-byte raw runs vs ORIG), `d2.py` (instruction diff between two `.so`s),
`dis2.py` (`sym`/`dis`/`show`), `sbs3.py` (side-by-side vs ORIG).
Captured: `head_sbs.txt`/`head_fold.txt` (TOTAL DIFF 46), `e1_new_sbs.txt`,
`e1_fold.txt`, `ORIG.dis`, `OURS.dis`, `orig.text`, `ours.text`, `head.text`,
`e1.text`, MIR dumps `pre_Merge.mir`, `pre_Local.mir`, `pre_licm.mir`,
`post_licm.mir`, `li.mir`, `gra.mir`, `allf.err`, `e1all.err`, `e1b.so`.
