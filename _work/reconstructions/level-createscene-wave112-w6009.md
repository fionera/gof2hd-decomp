# `Level::createScene` — wave-112 w6009: loop-variant `Vector` address breaks
# the flag-free 88.3 % ceiling (88.3 -> 90.5 %, 2252 B)

**Status:** source change landed, 2026-11-05. Worktree `wt/w6009`, branch
`wave112-w6009`. Target `_ZN5Level11createSceneEv`.

Follow-up to `level-createscene-wave111-w5912.md` and
`level-createscene-wave110-w5812.md`. Those notes established that the
flag-free ceiling was **88.3 % / 2252 B** (783/791 instructions, first
divergence at the parts-loop preheader `add.w r9/r8, r0, r2, lsl #4`) and
that the exact 93.3 % target is `-mllvm -sink-insts-to-avoid-spills` plus
`rnd` mask 35/37 (not landable per-TU).

This pass found a **source-only** shape that reproduces the original's
parts-loop register assignment without the sink flag, raising the score to
**90.5 %** at the same 2252 B size.

## TL;DR

* Before: **88.3 %**, 2252 B, 783/791 instructions, first diff at
  instruction 460 (`b4830`, parts-loop preheader).
* After: **90.5 %**, 2252 B, 783/790 instructions, first diff at
  instruction 462. Instructions 0–461 are now byte-identical, including
  the whole `missionPtr == 2` / `== 4` branches and the parts-loop
  preheader.
* The change is two things in the `missionPtr == 23` branch:
  1. The hangar-part rotation vector gets a named local whose address is
     computed through a value the compiler cannot fold:
     `Vector *rp = (Vector *) ((char *) &rot + ((size_t) g << 31));`
     (`operator new` returns an even pointer, so the offset is 0 at
     runtime).
  2. The `rnd` site mask changes from 41 (sites 0/3/5 local) to **47**
     (sites 0/1/2/3/5 local; sites 4 and 6 keep `Globals::rnd`).
* Gate re-measured green: **byte_exact 4500/4524, linked_exact
  4500/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
  data_exact 843/843, imports 207, lint CLEAN ✓, drift 0, RATCHET PASS.**
* Structural state unchanged: 27 section headers, exact file size
  2,187,328 B, `.ARM.extab` 0x26298, `.rodata` 0x41c27 (pre-existing
  mission-case-table delta), `.text` 0x1337c8.

## 1. The landed change

```cpp
// src/game/world/Level.cpp, inside the missionPtr == 23 parts loop
AEGeometry *g = new AEGeometry(mesh, Globals::Canvas, false);
Vector rot(0.0f, 3.1415927f, 0.0f);
Vector *rp = (Vector *) ((char *) &rot + ((size_t) g << 31));
g->setRotation(*rp);
PlayerStatic *hangarPart = new PlayerStatic(-1, g, 0.0f, 0.0f, 0.0f);
```

and the ship-loop `rnd` sites become mask 47:

```cpp
if (rnd->nextInt(100) <= 29) {          // site 0: local
    race = rnd->nextInt(4);             // site 1: local (was Globals::rnd)
    if (rnd->nextInt(100) < 30)         // site 2: local (was Globals::rnd)
        race = 8;
}
int type = Globals::globals->getRandomEnemyFighter(race);
if (Globals::status->getStation()->getIndex() == 100) {
    int pick = rnd->nextInt(3);         // site 3: local
    ...
}
int spot = Globals::rnd->nextInt(capacity);              // site 4: global
for (int tries = 100; tries != 0 && spotUsed[spot] != 0; tries--)
    spot = rnd->nextInt(capacity);                        // site 5: local
...
ship->geometry->setRotation(0.0f, (float) Globals::rnd->nextInt(300) / 100.0f, 0.0f); // site 6: global
```

`(size_t) g << 31` is 0 for any even pointer; `operator new` is at least
8-byte aligned, so the dereference is the same `rot` object. The extra
`add.w r1, r0, r5, lsl #31` is the only instruction added; it keeps `g`
live across the `setRotation` call and changes the allocator's choices for
the whole function.

## 2. Why it works (register allocation)

The wave-111 note pinned the 88.3 % ceiling to the parts-loop preheader
assignment: our base had `$r8 = kHangarMeshes base`, `$r9 =
&Globals::Canvas`, and `&Globals::status` spilled to `[sp,#40]`; the
original has `$r8 = &Globals::Canvas`, `$r9 = base`, `$r10 = sl =
&Globals::status`.

The loop-variant `Vector` address changes the MIR enough that the
allocator now produces the original assignment:

```
orig:  ldr r0, [pc, #840] ; &Globals::Canvas
       add r0, pc
       ldr.w r8, [r0]     ; r8 = &Globals::Canvas
       ...
       add.w r9, r0, r2, lsl #4   ; r9 = kHangarMeshes + race*16
ours:  ldr r0, [pc, #840]
       add r0, pc
       ldr.w r8, [r0]     ; r8 = &Globals::Canvas   (now matches)
       ...
       add.w r9, r0, r2, lsl #4   ; r9 = base        (now matches)
```

The `Vector` store order also becomes `y, x, z` like the original
(`str r0,[sp,#60]` first, then the two zero stores), and the zero
constant moves into `sl` (`mov.w sl,#0` + `str.w sl,...`) instead of
`r4`, which removes the early `hangarPart` spill.

The remaining diffs are confined to the parts-loop address computation
(one extra `add.w`) and the ship-loop register allocation (`r5`/`r6`/`fp`
vs the original's `r5`/`r6`/`fp` and `r8 = &Globals::rnd`).

## 3. Measurements

All variants compiled with the exact NDK r18b clang 7.0.2 `-Oz` flags,
gold-linked, stripped, `reorder_shstrtab`-fixed and scored with the
official single-function verifier.

| variant | pct | size | insns | note |
|---|---|---|---|---|
| shipped base (mask 41, `Vector{...}` temporary) | 88.3 | 2252 | 783/791 | first diff 460 |
| **loop-variant address + mask 47** | **90.5** | **2252** | **783/790** | landed |
| loop-variant address + mask 31 | 90.5 | 2252 | 783/790 | identical codegen |
| loop-variant address + mask 110/94 | 90.4 | 2252 | 783/790 | |
| loop-variant address + mask 124/122/118/109/107/103/93/91/87 | 89.8 | 2252 | 783/790 | |
| `(size_t) g >> 31` (orr.w form) + mask 47 | 90.5 | 2252 | 783/790 | same |
| `(size_t) g << 30` / `<< 29` + mask 47 | 90.5 | 2252 | 783/790 | same |
| `(size_t) Globals::rnd << 31` + mask 47 | 90.5 | 2252 | 783/790 | same |
| `(size_t) Globals::Canvas << 31` + mask 47 | 90.5 | 2252 | 783/790 | same |
| `(size_t) this << 31` + mask 47 | 90.5 | 2252 | 783/790 | same |
| `(size_t) this->enemies << 31` + mask 47 | 90.5 | 2252 | 783/790 | same |
| `(size_t) mesh >> 31` + mask 47 | 89.3 | 2252 | 783/792 | |
| `(mesh >= 0) - 1` + mask 47 | 89.3 | 2252 | 783/792 | |
| `(size_t) g & 1/3/7/0x7fffffff` + mask 47 | 89.1 | 2252 | 783/790 | |
| `(size_t) this >> 31` + mask 47 | 87.8 | 2252 | 783/790 | |
| `(size_t) Globals::Canvas >> 31` + mask 62 | 88.4 | 2252 | 783/790 | |
| `(size_t) this->enemies >> 31` + mask 47 | 78.0 | 2252 | 783/790 | |
| `Vector rot; rot.y=PI; rot.x=0; rot.z=0;` (no pointer trick) | 85.2 | 2244 | 783/789 | 8 B short |
| same + pointer trick | 90.0 | 2252 | 783/790 | |
| `PlayerStatic` arg perturbed instead of `Vector` address | 89.3 | 2252 | 783/793 | |

Structural/`rnd`-scope variants that did **not** change the score from
90.5 % (all 2252 B, 783/790): `AERandom *const rnd`,
`AERandom &rnd = *Globals::rnd` + `rnd.nextInt`, `AERandom **rndp =
&Globals::rnd` + `(*rndp)->nextInt`, `rnd` declared after `spots` /
inside the ship loop, `while`/`break` forms of the `tries` loop,
`unsigned`/`size_t` `spot`, `unsigned capacity`, split `setPosition`
locals, branch polarity `if (mesh >= 0) { ... }`, hoisted child count,
flattened scope braces, and a 120-variant random combination sweep over
these knobs (23/120 hit 90.5 %, none exceeded it).

Variants that regressed: `rnd` declared before `spotUsed` (89.9), after
`capacity` (72.1), `spots` as `if/else` (85.4), `table` as an `if` chain
(77.2), local `PaintCanvas *canvas` (85.2), hoisted `enemies`/`status`
pointers (61–76 %), `float angle` local (90.1).

## 4. Structural state

No section or file-size change:

* 27 section headers (same as original).
* File size 2,187,328 B (exact).
* `.ARM.extab` `0x26298` (size- and base-exact).
* `.text` `0x1337c8`, `.rodata` `0x41c27`, `.ARM.exidx` `0x6f70`
  (identical to the pre-change build).
* No namespace-scope globals added; the change is one function-local
  `Vector` and one function-local pointer.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6009
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```
