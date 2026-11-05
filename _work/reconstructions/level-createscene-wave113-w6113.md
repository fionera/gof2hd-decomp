# `Level::createScene` — wave-113 w6113: full 128-mask re-sweep on the
# wave-112 tricked source; 90.5277 % / 2252 B is the flag-free maximum

**Status:** research note, 2026-11-05. Worktree `wt/w6113`, branch
`wave113-w6113`. Target `_ZN5Level11createSceneEv`.
**No source change landed** — the shipped wave-112 source is still the best
flag-free shape found. This commit contains only this note; `src/` is
untouched.

Follow-up to `level-createscene-wave112-w6009.md` (and the chain back through
waves 88–111). Wave 112 landed the loop-variant `Vector` address trick and
raised the flag-free score from 88.3 % to **90.5277 %**
(`Vector *rp = (Vector *) ((char *) &rot + ((size_t) g << 31));`). This pass
re-ran the full `rnd`-site sweep on that source, swept the trick expression
itself, and tested the `rnd` declaration shapes that the original Thumb code
implies. Nothing beats 90.5277 %.

## TL;DR

* Target: **90.5277 %**, orig/ours both **2252 B**, 783/790 instructions,
  `byte=False`, `linked=False`. First diff at instruction 462 (the
  parts-loop preheader `b.n` target: `loc_610` vs `loc_614`, a pure +4 shift
  from the trick's extra `add.w`).
* Full **128-mask** `rnd`-site sweep on the tricked source: **m47 is the
  unique maximum at 90.5277 %**; next best m110 90.4005, then
  m124/m122/m118/m109/m107/m103 89.7648, m61/m59/m55 89.6376.
  Single-site masks are 66–69 %. (Wave 111's no-trick sweep max was m41
  88.3 %; wave 112's landed mask is 47.)
* Trick-expression sweep (mask 47): the shift amount is irrelevant
  (`g<<31`, `g<<30`, `g<<29`, `g<<28`, `g>>31`, `g>>30` all produce the
  identical 90.5277 % / 2252 B / 783/790). `Globals::Canvas<<31` 87.29,
  `this<<31` 87.82, `mesh<<31` 89.27, `sceneRace<<31` 84.32, `k<<31` 88.44,
  `this->enemies<<31` 77.89.
* `rnd` declaration shape is irrelevant at mask 47: pointer / `const` /
  `auto` / parenthesised / C-cast / split declaration / reference-to-object
  (`AERandom &rnd = *Globals::rnd` + `rnd.nextInt`) all give the identical
  90.5277 %.
* **The original reloads `Globals::rnd` at every one of the 7 ship-loop
  sites** (`ldr.w r0,[r8]` with `r8 = &Globals::rnd`, GOT slot `0x2101f8`,
  verified from the literal pool). That is the codegen of a source that
  writes `Globals::rnd->nextInt(...)` directly, with no local pointer. But
  the all-global source (mask 0) loses the parts-loop register assignment
  (`r8` = mesh row instead of `r9`) and scores only **75.4765 % / 2232 B**;
  `AERandom *&rnd = Globals::rnd` and `AERandom **rnd` compile to the same
  mask-0 code (75.4765 %, 2232 B).
* The trick's +1 instruction is unavoidable: clang 7.0.2 always materialises
  `&rot` into a GPR first (`add r1, sp, #N`) and then emits
  `add.w r1, r1, r0, lsl #31`; it never folds to `add.w r1, sp, r0, lsl #31`
  (verified with a 6-function micro-TU). Array indexing by `(size_t)g<<31`
  is folded away by the compiler (element size 12 makes the offset overflow
  to 0), so it does not perturb the allocator.
* Gate green before and after this pass: **byte_exact 4500/4524,
  linked_exact 4500/4524, stub_zero_size 0, missing 0, wrong_type 0,
  extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
  RATCHET PASS.**
* Structural state unchanged: 27 section headers, file size 2,187,328 B,
  `.text` `0x1337c8`, `.rodata` `0x41c27`, `.ARM.exidx` `0x6f70`,
  `.ARM.extab` `0x26298` (size- and base-exact).

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6113
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
```

Scratch harness (`/dev/shm/w6113`, not committed): `fast.sh` compiles one
copy of `Level.cpp` with the exact NDK r18b clang 7.0.2 `-Oz` flags, runs
the saved gold link + `strip --strip-all` + `reorder_shstrtab.py`, and scores
with the official single-function verifier (`exact.py`); `genmask.py`
generates the 128 `rnd`-site variants; `gentrick.py` generates the
trick-expression variants; `normdiff.py` prints the opcode-level diff.

## 2. Full 128-mask sweep on the tricked source (no flag)

`mask` bit *i* = site *i* uses the local `AERandom *rnd`; clear = the site
writes `Globals::rnd->nextInt(...)` directly. Sites in source order:
0 `nextInt(100)<=29`, 1 `race=nextInt(4)`, 2 `nextInt(100)<30`,
3 `pick=nextInt(3)`, 4 `spot=nextInt(capacity)`, 5 retry
`spot=nextInt(capacity)`, 6 rotation `nextInt(300)`.

| mask | pct | size | insns |
|---|---|---|---|
| **47** (0,1,2,3,5 local) | **90.5277** | **2252** | **783/790** |
| 110 (1,2,3,5,6 local) | 90.4005 | 2252 | 783/790 |
| 124 / 122 / 118 / 109 / 107 / 103 | 89.7648 | 2252 | 783/790 |
| 61 / 59 / 55 | 89.6376 | 2252 | 783/790 |
| 125 | 88.3662 | 2252 | 783/790 |
| 52 / 50 / 37 | 88.0000 | 2256 | 783/792 |
| 1 / 2 / 4 / 8 / 16 / 64 (single site) | 66.5–68.9 | 2236–2240 | 783/795–800 |
| 0 (all global) | 75.4765 | 2232 | 783/791 |

The wave-112 landed source is exactly m47. m47 is the unique maximum.

## 3. Trick-expression sweep (mask 47)

| expression | pct | size |
|---|---|---|
| `(size_t) g << 31` (shipped) | **90.5277** | 2252 |
| `(size_t) g << 30` / `<< 29` / `<< 28` | 90.5277 | 2252 |
| `(size_t) g >> 31` / `>> 30` | 90.5277 | 2252 |
| `(size_t) mesh << 31` | 89.2698 | 2252 |
| `(size_t) k << 31` | 88.4371 | 2252 |
| `(size_t) this << 31` | 87.8173 | 2252 |
| `(size_t) Globals::Canvas << 31` | 87.2935 | 2256 |
| `((size_t) g << 31) | ((size_t) g << 30)` | 87.3651 | 2256 |
| `((size_t) g << 31) + ((size_t) g >> 31)` | 87.3651 | 2256 |
| `(size_t) plainHangar << 31` | 84.4642 | 2260 |
| `(size_t) sceneRace << 31` | 84.3175 | 2252 |
| `(size_t) this->enemies << 31` | 77.8907 | 2240 |

The allocator only cares that the extra `add.w` uses `g` (r5); the shift
amount and direction are irrelevant.

## 4. `rnd` declaration shape (mask 47)

All identical (90.5277 %, 2252 B, 783/790):

* `AERandom *rnd = Globals::rnd;` (shipped)
* `AERandom *const rnd = Globals::rnd;`
* `auto *rnd = Globals::rnd;`
* `AERandom *rnd(Globals::rnd);`
* `AERandom *rnd = (AERandom *) Globals::rnd;`
* `AERandom *rnd; rnd = Globals::rnd;`
* `AERandom &rnd = *Globals::rnd;` + `rnd.nextInt(...)`

By contrast, `AERandom *&rnd = Globals::rnd;` and
`AERandom **rnd = &Globals::rnd;` both compile to the mask-0 code
(75.4765 %, 2232 B) — the compiler treats them as direct global accesses.

## 5. The original reloads `Globals::rnd` at every site

Original ship-loop `nextInt` sites (function `0xb42ec`, `.text`):

```
b4936: ldr.w r0,[r8]   ; shipCount = nextInt(capacity+1)
b49ae: ldr.w r0,[r8]   ; site 0
b49be: ldr.w r0,[r8]   ; site 1
b49ca: ldr.w r0,[r8]   ; site 2
b49f6: ldr.w r0,[r8]   ; site 3
b4a46: ldr.w r0,[r8]   ; site 4
b4a56: ldr.w r0,[r8]   ; site 5
b4ad8: ldr.w r0,[r8]   ; site 6
```

`r8` is loaded once at `b492c` (`ldr.w r8,[r0]` from the GOT slot at
`0x2101f8`, which is `_ZN7Globals3rndE`), so `r8 = &Globals::rnd` and every
site reloads the pointer value. That is the natural codegen of
`Globals::rnd->nextInt(...)` at every site — no local pointer.

But the all-global source (mask 0) does **not** reproduce the original
register allocation: its parts-loop preheader computes the mesh row into
`r8` (original: `r9`) and `&Globals::Canvas` into `r9` (original: `r8`),
and the whole function's RA shifts from the first basic block
(`ldr r4,[r0,#0]` instead of `ldr.w sl,[r0]`, an extra
`str r6,[sp,#48]`, etc.). The local `rnd` pointer (and the trick) is what
pins the parts-loop RA; the price is that the compiler caches the `rnd`
value in `r5` and spills `&Globals::rnd` to `[sp,#44]`, which is exactly the
remaining ship-loop diff.

## 6. Sink-flag reference points (measured on this base)

| variant | pct | size | insns |
|---|---|---|---|
| no trick + `-mllvm -sink-insts-to-avoid-spills` + mask 35 | **93.2656** | 2252 | 783/791 |
| no trick + sink + mask 37 | 93.2656 | 2252 | 783/791 |
| no trick + sink + mask 47 | 88.8041 | 2248 | 783/789 |
| no trick + sink + mask 0 | 75.9365 | 2232 | 783/792 |
| trick + sink + mask 35 | 88.0000 | 2256 | 783/792 |
| trick + sink + mask 47 | 90.5277 | 2252 | 783/790 |
| trick + no flag + mask 35 | 88.0000 | 2256 | 783/792 |
| no trick + no flag + mask 35 | 84.9524 | 2248 | 783/792 |
| no trick + no flag (mask 47) | 85.3147 | 2244 | 783/789 |

The sink flag is a no-op on the tricked source (the trick already forces the
`&rot` def into the loop body). With the no-trick source it moves the
`t2ADDri %stack.1, 0` back into the loop body and flips the allocator to the
original parts-loop **and** ship-loop assignment (93.27 %).

## 7. Other variants measured this pass (all 2252 B unless noted)

Neutral (90.5277 %, identical code): `Vector rot = {0, pi, 0}`,
`rot.x=0; rot.y=pi; rot.z=0`, `const Vector rot`, shared `int i` for the
`spotUsed` fill and ship loops, `const int *heights = kShipHangarHeights`,
`const int *capacityTable = kHangarCapacity`, `long`/`unsigned` `type`,
`Vector` trick + child-mesh trick (CSE'd).

Worse: `rot.y=pi; rot.x=0; rot.z=0` 90.0191; `rot.y=pi` only 90.1462;
`short type` 84.3175 (2264 B); Vector trick + `PlayerStatic` 3rd-arg trick
86.2571; Vector trick + `addChild` arg trick 85.1523; preheader row-pointer
trick 85.8958 (2260 B); preheader `sceneRace` trick 73.3968;
`PlayerStatic` g-arg trick 84.7328 (2244 B); trick moved to the end of the
loop body (`ArrayAdd` arg) 85.1145 (2244 B); trick on the child mesh
85.3147 (2244 B); ship-loop `spot` trick 69.4990 (2244 B); ship-loop table
index trick 68.9480 (2248 B).

## 8. Why the +1 instruction cannot be removed

The original parts-loop vector setup is:

```
add   r1, sp, #56
str   r0, [sp, #60]     ; y = pi
mov.w sl, #0
mov   r0, r5            ; g
str.w sl, [sp, #56]     ; x = 0
str.w sl, [sp, #64]     ; z = 0
blx   setRotation
```

The tricked source adds `add r0, sp, #56` + `add.w r1, r0, r5, lsl #31`
(and reorders `mov.w sl,#0` before them). A micro-TU compiled with the same
flags shows clang 7.0.2 never folds the stack address into the shifted
register add (`add.w r1, sp, r0, lsl #31` is encodable but not selected);
it always materialises `&rot` in a GPR first. So the trick always costs
exactly one 4-byte instruction, and that instruction shifts every branch
target after it by +4 (the 90.5 % ceiling).

## 9. Bottom line

`Level::createScene` remains **90.5277 % / 2252 B** (783/790, first diff at
instruction 462). The flag-free maximum over the full 128-mask sweep, the
trick-expression sweep and the `rnd`-declaration sweep is the shipped
wave-112 source. The 93.27 % target remains `-mllvm
-sink-insts-to-avoid-spills` + no trick + `rnd` mask 35/37, which is not
landable per-TU. Worktree clean, gate green (byte 4500 / linked 4500 /
lint CLEAN / drift 0 / RATCHET PASS), sections unchanged.
