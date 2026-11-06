# `Level::createScene` — wave-119 w6903: no source win over 92.8000 %

**Status:** dead-end sweep, 2026-11-06. Worktree `wt/w6903`, branch
`wave119-w6903` (base master `50d2df436`). Target
`_ZN5Level11createSceneEv`, `src/game/world/Level.cpp`. No source change
landed; this note records the measurements so the next pass does not repeat
them.

Continuation of `level-createscene-wave119-w6820.md` (the most complete
prior note: full measurement tables, MIR section, reproduction commands),
`…-w6735.md` and `…-w6709.md` (which landed the `-g` Vector-address trick,
the y/z/x store order and the ship-loop `rnd` mask, 90.7243 -> 92.7573 %).
The wave-119 landed shape is still the unique maximum found.

## TL;DR

* Baseline and best: **92.8000 % / 2252 B**, 791/783 instructions, first
  diff at normalized instruction 478 (parts-loop store schedule).
* The function is instruction-exact up to instruction 477. The only two
  differing regions remain (a) the parts-loop store schedule (478-483) and
  (b) the ship-loop register allocation (548 onward).
* ~55 new source variants were measured this session (all-global `rnd`,
  value/reference/volatile/pointer-to-pointer `rnd` locals and every
  declaration position, loop-form rewrites, branch-order permutations,
  parts-loop store/address spellings, and per-file `-mllvm` register
  allocator knobs). **None beat 92.8000 %**; the closest non-landed shape
  is the `decl_in_loop` / `assign_in_loop` family at 91.5000 % (2252 B),
  which fixes nothing but moves the mission-2 branch allocation.
* Gate re-measured green after restoring the landed source:
  **byte_exact 4505/4524, linked_exact 4505/4524, stub_zero_size 0,
  missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
  lint CLEAN, drift 0, RATCHET PASS.** 27 sections, file size
  2,187,328 B.

## 1. The two remaining diff regions

### 1.1 Parts loop (instructions 478-483)

Original (`g->setRotation(rot)` — one-instruction address, three separate
stores, zero materialised after the y store):

```
ldr   r0, [pc, #808]      ; pi
add   r1, sp, #56         ; &rot
str   r0, [sp, #60]       ; rot.y = pi
mov.w sl, #0              ; zero
mov   r0, r5              ; g
str.w sl, [sp, #56]       ; rot.x = 0
str.w sl, [sp, #64]       ; rot.z = 0
blx   setRotation
```

Landed (`- (size_t) g` trick — two-instruction address, `strd` merge):

```
ldr   r0, [pc, #808]      ; pi
mov.w sl, #0              ; zero
str.w sl, [sp, #64]       ; rot.z = 0
strd  sl, r0, [sp, #56]   ; rot.x = 0, rot.y = pi (merged)
add   r0, sp, #56
subs  r1, r0, r5
mov   r0, r5
blx   setRotation
```

Both regions are 26 bytes. The `-g` trick is the only lever found that
forces the original's Basin-B prologue/parts-loop allocation; without it
the allocator moves the zero to `r4`, swaps Canvas/table-base (`r8`/`r9`)
and spills the parts-loop counter `k` (plain `setRotation(rot)` = 84.8793 %,
2248 B).

### 1.2 Ship loop (instructions 548 onward)

Original (all eight sites use `Globals::rnd->`; the GOT-derived address of
`Globals::rnd` is hoisted into **r8** and the pointer value is reloaded with
`ldr.w r0,[r8]` at every site):

```
ldr   r0, [pc, #632]      ; GOT
ldr   r1, [pc, #636]
add   r0, pc
add   r1, pc
ldr.w r8, [r0]            ; r8 = &Globals::rnd
ldr   r0, [sp, #52]       ; sceneRace
ldr.w r6, [r1, r0, lsl#2] ; capacity -> r6
ldr.w r0, [r8]            ; Globals::rnd
adds  r1, r6, #1
blx   nextInt
```

Landed (local `AERandom *rnd = Globals::rnd;` used at B, C and the retry
site; the value is cached in r5 and the address is spilled):

```
ldr   r0, [pc, #632]
ldr   r1, [pc, #636]
add   r0, pc
add   r1, pc
ldr   r2, [r0, #0]        ; r2 = &Globals::rnd
ldr   r0, [sp, #52]
str   r2, [sp, #44]       ; spill &Globals::rnd
ldr.w fp, [r1, r0, lsl#2] ; capacity -> fp
ldr   r0, [r2, #0]        ; Globals::rnd
add.w r1, fp, #1
blx   nextInt
```

Register map in the ship loop:

| value | original | landed |
|---|---|---|
| `spotUsed` | r4 | r4 |
| `race` | r5 | r6 |
| `capacity` | r6 | fp |
| `&Globals::rnd` | **r8** | spilled `[sp,#44]` |
| `i` | r9 | r9 |
| `ship` / `status` | sl | sl |
| `type` | fp | r8 |
| `Globals::rnd` value | reloaded `[r8]` at every site | cached in r5 |
| `this` | spilled `[sp,#48]` | spilled `[sp,#48]` |

The landed build already matches the original's `this`-spilled /
`status`-in-`sl` assignment (and the whole prologue, mission-2 and
mission-4 branches are instruction-exact). The only mismatch is the extra
loop-live `rnd` value.

The central basin conflict (re-confirmed this session):

* **Local-value basin (landed, 92.8000 %):** `Globals::status` in `sl`,
  Canvas in `r8`, table base in `r9`, parts-loop zero in `sl`; ship loop
  caches `rnd` in r5 and spills the address.
* **All-global basin (75.4000 %, 2232 B):** removing the local `rnd`
  entirely gives the original's `ldr rX,[addr]` ship-loop shape, but the
  whole function collapses to a different allocation: `Globals::status` in
  `r4` (spilled), `this` kept in `sl`, `&Globals::rnd` in `r9`, `capacity`
  in `r8`, `i` spilled across the retry loop, and the parts-loop
  preheader `r8`/`r9` swap. First diff at instruction 6 (the `this` spill
  store is scheduled before the first `beq` instead of after the second
  `cmp`).

No source shape found this session bridges the two: the local value must
exist (and be live across the ship loop) to pin the prologue/parts-loop
basin, but its existence is exactly what pushes the address out of r8.

## 2. Measurements (all 2252 B unless noted)

Scored with the official single-function verifier (unrounded difflib
ratio). `pct` is the `--show` structural ratio; `fd` is the first differing
normalized instruction index.

### 2.1 `rnd` local form and declaration position

| variant | pct | size | fd |
|---|---|---|---|
| **landed (`AERandom *rnd` after the fill loop, B/C/retry local)** | **92.8000** | 2252 | 478 |
| all-global (no local `rnd`) | 75.4000 | 2232 | 6 |
| `AERandom *&rnd = Globals::rnd` (reference) | 75.4000 | 2232 | 6 |
| `AERandom **rndp = &Globals::rnd` (`(*rndp)->`) before shipCount | 75.4000 | 2232 | 6 |
| `AERandom *volatile rnd` (landed position, all 8 sites) | 77.6000 | 2240 | 8 |
| `AERandom *const volatile rnd` | 77.6000 | 2240 | 8 |
| all 8 sites local, decl before `shipCount` | 74.3000 | 2240 | 6 |
| decl after `spots` (immediately before the loop) | 92.8000 | 2252 | 478 |
| decl at loop top (inside the body) | 91.5000 | 2252 | 133 |
| `rnd = Globals::rnd;` assigned at loop top | 91.5000 | 2252 | 133 |
| decl before `ownHangar` | 71.3000 | 2220 | 6 |
| decl before `spotUsed` | 76.6000 | 2228 | 6 |
| decl before `plainHangar`, used at `shipCount` only | 74.2000 | 2244 | 8 |
| decl before `shipIndex`, used at `shipCount` only | 74.9000 | 2240 | 8 |
| decl before `capacity`, used at `shipCount` only | 75.4000 | 2232 | 6 |
| decl before `plainHangar`, used at `shipCount`+B/C/retry | 70.0000 | 2220 | 6 |
| `if (rnd == nullptr) continue;` at loop top | 75.0000 | 2224 | 5 |

The `decl_in_loop` / `assign_in_loop` family (91.5000 %) does **not**
change the ship loop at all; it only swaps r9/sl in the mission-2 branch
(`ldr.w r9,[fp]` vs `ldr.w sl,[fp]` at instruction 133).

### 2.2 Ship-loop statement-order / declaration probes (all neutral)

`int i = 0;` hoisted before the `spots` ternary and `for (; i < shipCount;
i++)`; `race`/`type`/`spot` declared outside the loop and assigned inside;
`const int capacity`; `const int shipCount` (build error, assignment in the
`ownHangar` arm); `int ownHangar = 0`; `for (...; ++i)` in the fill loop;
`while (i < shipCount) { …; i++; }`; `for (int i = 0; i < shipCount;) { …;
i++; }` — all **92.8000 %** / 2252 B / fd 478 (byte-identical code).
`do { … } while (i < shipCount)` = 88.5000 % (2248 B, fd 28).
`const int *spots = kHangarSpotsKhador[0]; if (sceneRace != 7) spots =
kHangarSpotsDefault[0];` (two statements instead of the ternary) = 86.8000 %
(2248 B, fd 9).
Hoisting `Array<KIPlayer*> *enemies = this->enemies;` before the ship loop
= 75.4000 % (2252 B, fd 4).

### 2.3 Branch order (all-global `rnd`)

| order | pct | size |
|---|---|---|
| 23,4,2 (source) | 75.4000 | 2232 |
| 23,2,4 | 75.4000 | 2232 |
| 2,23,4 | 66.4000 | 2236 |
| 4,23,2 | 74.4000 | 2232 |

### 2.4 Parts-loop store / address spellings

| variant | pct | fd |
|---|---|---|
| **landed y,z,x + `- (size_t) g`** | **92.8000** | 478 |
| y,x,z (or `rot.z = rot.x = 0`) | 92.6000 | 477 |
| `Vector rot; rot.y = pi;` only (default ctor zeroes) | 87.9000 | 28 |
| `Vector rot = Vector(); rot.y = pi; rot.x = 0; rot.z = 0;` | 92.6000 | 477 |
| `Vector *rp = …-g; rp->y/z/x = …` (stores through `rp`) | 85.1000 | 28 |
| `rp` declared before the stores | 92.8000 | 478 |
| `size_t off = (size_t)&rot - (size_t)g; rp = (Vector*)((size_t)&rot - off)` | 72.9000 | 3 (2160 B) |
| `- (size_t) rnd` (all-global ship loop, `rnd` declared before parts loop) | 74.1000 | 8 (2244 B) |
| `- (size_t) this` (all-global ship loop) | 75.1000 | 6 |
| `- (size_t) sceneRace` (all-global ship loop) | 74.8000 | 6 |
| plain `g->setRotation(rot)` (all-global ship loop) | 74.7000 | 6 (2224 B) |

### 2.5 Per-file `-mllvm` register-allocator knobs

Temporarily added as `set_source_files_properties(Level.cpp COMPILE_OPTIONS
"-mllvm;<flag>")`, scored, then reverted (`CMakeLists.txt` is unchanged in
the landed tree):

| flag | pct | size |
|---|---|---|
| `-split-spill-mode=size` | 92.6000 | 2252 |
| `-split-spill-mode=default` | 71.3000 | 2248 |
| `-condsider-local-interval-cost=true` | 92.8000 | 2252 |
| `-regalloc-csr-first-time-cost=10` | 92.8000 | 2252 |
| `-exhaustive-register-search` | 92.8000 | 2252 |
| `-regalloc=fast` | 17.7000 | 2692 |

(`-split-spill-mode=loc` / `=weight` are rejected by this clang 7.0.2
build. `-sink-insts-to-avoid-spills` was already measured neutral in
w6820.)

## 3. Why the local value cannot simply be removed

The ship loop needs `Globals::rnd->` at all eight sites so that the
GOT-derived address `&Globals::rnd` is hoisted into r8 and the pointer
value is reloaded at each site. That is the all-global source. But the
all-global source loses the *prologue* basin: `Globals::status` moves from
`sl` to `r4` (spilled), `this` moves from `[sp,#48]` into `sl`, and the
parts-loop preheader swaps `r8`/`r9`. The landed local-value source pins
the prologue basin but the extra loop-live `rnd` value is exactly one
register too many, so the allocator spills `&Globals::rnd` instead of
keeping it in r8 and caches the pointer value in r5.

The register budget in both builds is the same seven callee-saved
registers (`r4,r5,r6,r8,r9,sl,fp`; `r7` is the frame pointer), so this is
a pure assignment/priority difference, not a pressure difference. The
original assignment is a cyclic permutation of the landed one:
`{capacity,race,type,&rnd} = {r6,r5,fp,r8}` vs landed
`{fp,r6,r8,spill}`.

## 4. Conclusion

The landed wave-119 shape is a genuine local maximum for clang 7.0.2 at
`-Oz`. The `-g` trick is the only lever found that forces the original's
Basin-B prologue/parts-loop allocation, and the local `rnd` value is the
only lever found that keeps it. The original's ship loop needs
`&Globals::rnd` in r8, which the compiler only produces in the all-global
basin where `Globals::status` moves to r4 and `this` takes sl.

The next pass should look for the source feature that pins the prologue
basin **independently of the `rnd` local** — a witnessed original local
that keeps `Globals::status` in `sl` and `this` spilled without adding a
ship-loop live value. Candidates not yet tried: a `spots`-based dependency,
or a `capacity` read through a `const int *capTable` local combined with a
different parts-loop address form (the `capTable` form alone is neutral).
The all-global build is the semantically correct source; the only missing
piece is that early-allocation pin.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6903
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (not committed): `/dev/shm/w6903/` — `Level.base.cpp`
(the landed source), `score.py` (official verifier import, unrounded ratio
+ size + first-diff index), `vtry.py` … `vtry14.py` (this session's
variant batches), `difflist.py` (per-instruction diff dump),
`orig.asm` / `ours_new.asm` / `ag2.asm` / `plain.asm` (resolved
disassembly), `mir_base.txt` (full `-print-machineinstrs` dump).
