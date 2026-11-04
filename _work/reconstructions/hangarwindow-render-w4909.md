# `HangarWindow::render` — pass 13 (w4909): lazy-`layout` proof, `right > 0`

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave99-w4909`, base master `44225192c`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra image base
0x10000; ELF addresses below are the real ones).

This pass re-audits the lazy-GOT / 376-byte-frame tie left open by
`hangarwindow-render-w4811.md`, and **lands one faithful source fix**: the `right` guard
in the `isItem` branch is `if (right > 0)`, not `if (right)`.  Raw match
`0.8082059755 -> 0.8084813438` with an unchanged 9700-byte body and an unchanged gate.

## TL;DR

- **The original is the fully lazy shape.**  It contains exactly **two** GOT loads of
  `_ZN7Globals6layoutE` (GOT slot `0x210224`): one at `0x149132` (immediately before
  `Layout::drawBG`) and one at `0x14a85a` (immediately before `Layout::drawFooter`).
  Every other `Globals::layout->...` use in the body reuses the value loaded at one of
  those two points (spilled to `[sp,#132]` and reloaded).  A function-scope
  `Layout *layout = Globals::layout;` local would produce a single load, so it is ruled
  out.  The 80.82 % baseline's eager local is a reconstruction artefact whose `bb.0`
  load is kept alive only by the `-g` `DBG_VALUE` (w4709).
- The original's `if (right)` guard is really `if (right > 0)`:
  `cmp.w r9, #1 ; blt.n 0x149d26` at `0x149ce4`.  The reconstruction had
  `if (right)` (`cmp.w r9, #0 ; beq.n`), which mismatched.  Changing it to
  `if (right > 0)` matches the original's comparison and is the landed change.
- The lazy shape still does **not** reproduce the original's register roles
  (`this -> fp`, `&layout -> sl`, `&appManager -> r8`, frame 376): it gives
  `this -> sl`, `layout -> fp`, `&appManager -> r8`, frame 384.  The role swap is not
  explained by the raw live-range sizes (see §3); a physical-register hint
  (`VRM->hasKnownPreference`) must be involved, and no plain source shape tried this
  pass reproduces it.
- Caching the `appManager` *value* (`AM3`) is the only lazy shape that gives
  `this -> fp` + frame 376, but it is unfaithful (the original keeps
  `&Globals::appManager` in `r8`, spilled at `[sp,#64]` at `0x1491b8` and reloaded at
  `0x14a78e`), and its best size-correct variant only scores 74.95 %.

## Gate (before -> after)

```
verify: byte 4500 -> 4500  linked 4500 -> 4500  stubs 0  missing 0  extra 0  wrong_type 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
sections: .text 0x1337c8, .rodata 0x41b67, .ARM.exidx 0x6f70, .ARM.extab 0x26294 (-4)
render: 9700/9700 B, raw 0.8082059755 -> 0.8084813438, insns orig/ours 3626/3637, frame 376
```

Build (canonical):

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
```

## 1. The landed change

`src/game/ui/HangarWindow.cpp`, inside the `isItem` branch:

```cpp
// before
if (right) {
// after
if (right > 0) {
```

The original's guard at `0x149ce4` is

```
149ce4: f1b9 0f01   cmp.w  r9, #1
149ce8: db1d        blt.n  149d26        ; skip the buttons[9] block
```

i.e. `if (right > 0)` (`right >= 1`).  The neighbouring `if (left > 0)` guard
(`0x149ca8: cmp r0, #1 ; blt.n`) already matches.  The change is size-neutral
(9700 B) and moves exactly one instruction from mismatch to match
(`cmp.w r9,#0 ; beq.n` -> `cmp.w r9,#1 ; blt.n`); the raw ratio rises by
`1/3626 = 0.000275`.

## 2. New: the original has exactly two `Globals::layout` GOT loads

Decoding the Thumb `ldr rX,[pc,#N] ; add rX,pc` pairs in the original `render` and
matching the resulting GOT slot against `.rel.dyn`:

| site | literal | GOT slot | symbol |
|---|---|---|---|
| `0x149132` | `0x000c70ec` | `0x210224` | `_ZN7Globals6layoutE` |
| `0x14a85a` | `0x000c59c4` | `0x210224` | `_ZN7Globals6layoutE` |

Both are followed by `ldr.w sl,[r0]` (load the `Layout *` value).  The first feeds
`Layout::drawBG` (`0x14913e`), the second feeds `Layout::drawFooter` (`0x14a866`).
All other uses of `Globals::layout` in the body (`drawBox`, `drawHeader`,
`formatCredits`, the `freeCredits` block, ...) reuse the value: it is spilled once at
`0x1491b0` (`str.w sl,[sp,#132]`) and reloaded from `[sp,#132]` (e.g. `0x149248`,
`0x1492a8`, `0x14965a`, `0x14ab8c`).  This is exactly the fully lazy source shape
(`Globals::layout->...` at every use), not a function-scope local.

The same scan on the current eager build finds the layout GOT load in the prologue
(`bb.0`), which is the extra 4-instruction prologue that keeps the baseline at 80.82 %.

## 3. Register-allocation priority (LLVM 7 `RAGreedy`)

`RAGreedy::enqueue` (`RegAllocGreedy.cpp:680`) gives a global range

```
Prio = (1u<<31) | (1u<<29) | Size
if (VRM->hasKnownPreference(Reg)) Prio |= (1u<<30);
```

and tie-breaks on the lower vreg number.  `AllocationOrder` puts hints first, then
`RegisterClassInfo::getOrder`, which appends the CSR aliases in the `GPR` raw-order
(`ARMRegisterInfo.td`: `AltOrders = [(add LR, GPR)]` = `LR,R0..R12,SP,LR,PC`), i.e.
**LR, R4, R5, R6, R7, R8, R9, R10, R11** — *not* the `CSR_AAPCS` order
`R11,R10,R9,R8,...` quoted in w4709.  (The earlier handoff's "R11 allocated first"
claim is therefore wrong; the callee-saved pool is filled low-to-high.)

Measured pre-RA live spans (machine-instr dump byte offsets, `-print-machineinstrs=greedy`):

| shape | `this` | `&appManager` | `&layout` | result |
|---|---:|---:|---:|---|
| lazy (ours) | 104880 (16..104896) | 76928 (256..77184) | 73456 (768..74224) | this `sl`, layout `fp`, appManager `r8`, frame 384 |
| eager (ours) | 104848 | 76848 | 110712 | this `fp`, layout `sl`, appManager `r8`, frame 376 |

The lazy ordering is inconsistent with a pure `Size` priority (`this` is the longest
range but lands in `sl`, while the shorter `&layout` range takes `fp`), so a
physical-register hint (`hasKnownPreference`) must be steering at least one of the
three ranges.  The eager shape happens to reproduce the original's three roles and
frame 376, but only because the `bb.0` `DBG_VALUE` keeps the eager load alive.

## 4. Variant table (this pass; all measured with the official verifier)

Baseline = eager `Layout *layout = Globals::layout;` at the top, raw `0.8082060`.

| variant | raw | size | frame | `this` |
|---|---:|---:|---:|---|
| **`if (right > 0)` (landed)** | **0.8084813** | 9700 | 376 | fp |
| `if (right != 0)` | 0.8082060 | 9700 | 376 | fp |
| `if (left)` | 0.8079306 | 9700 | 376 | fp |
| `if (left >= 1)` | 0.8082060 | 9700 | 376 | fp |
| `if (barHeight)` (both sites) | 0.8068291 | 9700 | 376 | fp |
| `if (... || barPos)` | 0.8062784 | 9700 | 376 | fp |
| lazy (no local) | 0.6173077 | 9724 | 384 | sl |
| lazy + faithful `getWidth`/`getHeight` | 0.5201979 | 9716 | 384 | sl |
| eager + faithful `getWidth`/`getHeight` | 0.6225038 | 9748 | 376 | r8 |
| local inside `if (viewMode==0)` | 0.4665191 | 9592 | 384 | sl |
| local after `if (viewMode==1)` (before `drawFooter`) | 0.5937027 | 9732 | 376 | sl |
| `AM3` lazy (cache `appManager` value) | 0.7329569 | 9688 | 376 | fp |
| `AM3` lazy + 4 `description=description` | 0.7495183 | 9700 | 376 | fp |
| lazy + `h` + `t` + `AM3` | 0.6954489 | 9668 | 376 | fp |
| `hintLayout` -> `layout` (reuse eager local) | 0.6594372 | 9720 | 376 | r9 |
| eager local used at `drawHeader` only | 0.5873278 | 9696 | 384 | fp |
| eager local used at `drawBG`+`drawHeader` | 0.5880733 | 9704 | 384 | fp |
| eager local used at `drawBG`+`drawFooter` | 0.6023134 | 9692 | 384 | fp |
| `AM3` eager | 0.7335354 | 9668 | 376 | fp |

`pad3` initialiser re-sweep (base initialiser is `this->field_0x114`, best):
`+y` 0.7898651/9696, `*2` 0.7906401/9704, `scrollOffset` 0.8065538/9700,
`y` 0.7898651/9696, `left` 0.7901405/9696.

`description = description` filler re-sweep: `price=price` ×3 0.7938868,
2×desc+1×price 0.7996696, `description=price` ×3 0.7822614 (`9696` B),
desc/price interleaved 0.7996696.  The current 3× `description = description`
remains the best (`0.8082060` before the landed `right > 0` fix).

## 5. What remains

- The original's lazy shape needs `this -> fp` and `layout -> sl` simultaneously with
  frame 376.  Every plain source variant tried gives either (`this -> fp`, frame 384)
  or (`this -> sl`, frame 384), except `AM3` (unfaithful) and the eager local
  (DBG_VALUE-driven `bb.0` load).  The missing ingredient is a register hint, not a
  source-level local: the original has only two `Globals::layout` GOT loads.
- `.ARM.extab` is still `0x26294` (-4); the missing LSDA row is the final `rightText`
  `operator+` destination materialisation (w4811), tied to the same regalloc gap.
- The `description = description` ×3 filler is still not faithful (the original has 18
  `String::operator=` calls in `render`, the reconstruction 21); it remains
  load-bearing for the 9700-byte body.

## 6. Reproduction

```sh
python3 /dev/shm/w4909/raw.py     # 0.8084813438 size 9700 insns 3626/3637
python3 /dev/shm/w4909/roles.py   # frame 376 this fp
```

Scratch scripts used this pass live in `/dev/shm/w4909` (`raw.py`, `roles.py`,
`pro.py`, `try.py`, `orig.dis`, `ours.dis`, `ours_lazy.dis`, `mi_g.log`, `mi_e.log`).
