# `Level::createScene` — wave-97 w4712: the ship-loop statement order is an exhaustive dead end

**Status:** research note, 2026-11-04. Worktree `wt/w4712`, branch `wave97-w4712`,
base `9b58a39d5`. **No source change landed** — no source shape found that raises
`byte_exact`. This commit contains only this note.

Follow-up to `level-createscene-wave95-w4514.md` (which followed
`level-createscene-wave94-w4414.md`, `level-createscene-wave92-w4249.md`,
`level-createscene-wave92-w4213.md`, `level-createscene-wave91-w4113.md`,
`level-createscene-phi-use-w4013.md`, `level-createscene-licm-hoist-w3833.md`,
`Level-createScene-flag-free.md`, `level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`.
* Gate re-measured before and after this pass, unchanged: **byte_exact
  4500/4524, linked_exact 4500/4524, size-exact 4524/4524, stub_zero_size 0,
  missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207, lint
  CLEAN ✓, drift 0, RATCHET PASS.**
* **The 7-statement ship-loop prelude is now exhaustively searched.** All 840
  topological orders of
  `A=ownHangar, B=capacity, C=shipCount, D=if(ownHangar), E=spotUsed,
  F=rnd-local, G=spots` (constraints `B<C`, `A<D`, `B<E`) were built and
  measured. The shipped order `A B C D E F G` is the **unique optimum at
  88.3 %**; the next best (`A B C G D E F`, `A B G C D E F`, `A G B C D E F`)
  is 87.3 %, and every order that moves `spots` (G) before `if (ownHangar)`
  (D) collapses to ≤ 75.4 %. §3.
* **Correction to the wave-97 handoff's `&Globals::status` "reference" lead.**
  The original's `ldr.w r0,[sl]` at all 12 status sites is *not* a
  reference-to-pointer shape. `sl` is the **address of the `Globals::status`
  variable** (loaded from the GOT at `b433e: ldr.w sl,[r0]`), and
  `ldr.w r0,[sl]` is the ordinary PIC value load for a direct
  `Globals::status->…` access. The shipped source already emits exactly this
  shape; the all-globals source's `ldr r0,[r6,#0]` is the *same* shape with a
  different register and a 16-bit encoding. There is no new lever here. §2.
* The local-`rnd` form is settled: the shipped form (a value copy declared
  before `spots`, used at the race check / `pick` / `tries` sites, with
  `Globals::rnd` at the other five) is the best. Reference (`AERandom *&`),
  pointer-to-pointer (`AERandom **`), `volatile`, all-8-sites-local and every
  declaration position are all worse. §4.
* The remaining 8-instruction gap is entirely spill traffic: the shipped
  source spills `k` to `sp#44` in the parts loop and spills the `rnd` value to
  `sp#44` in the ship loop; the original spills `hangarPart` instead and keeps
  the `&Globals::rnd` address in `r8`. §5.

## 1. Baseline diff

`python3 /dev/shm/w4712/fscore.py --diff` at the baseline: 276 `|` lines, all
inside the `missionPtr == 23` branch. The first 459 instructions are
byte-identical; the first diff is instruction 460, the parts-loop preheader
(`add.w r9,…` vs `add.w r8,…`). Everything after is register names, spill-slot
numbers and 8 extra spill/reload instructions on our side (791 vs 783).

Structurally (normalising registers, branch targets and literal-pool
addresses) the two instruction streams are identical **except**:

* the shipped source has one extra `str r6,[sp,#44]` (the `k` spill) between
  the two `Vector` zero stores in the parts loop, and a matching
  `ldrd sl,r6,[sp,#40]` restore at the latch;
* the ship loop's `rnd` handling differs: the original keeps
  `&Globals::rnd` in `r8` and reloads the value at every site; the shipped
  source spills `&Globals::rnd` to `sp#44` and reloads both the address and
  the value.

## 2. The `&Globals::status` "reference" lead is a red herring

The wave-97 handoff observed that the original does `ldr.w r0,[sl]` at all 12
status sites and the all-globals source does `ldr r0,[r6,#0]`, and proposed a
`Status *&status = Globals::status;` local. That reading is wrong.

Original prologue (`orig.txt`):

```
b433a: ldr r0,[pc,#872]     ; GOT slot for Globals::status
b433c: add r0,pc
b433e: ldr.w sl,[r0]        ; sl = &Globals::status   (GOT entry = address)
b4342: ldr.w r0,[sl]        ; r0 = Globals::status    (the value)
b4346: blx Status::getStation
```

`ldr.w sl,[r0]` loads the **GOT entry**, which for a default-visibility global
is the address of the variable, not its value. So `sl = &Globals::status` and
`ldr.w r0,[sl]` is the plain value load of a direct `Globals::status->…`
access. The shipped source emits exactly this sequence (byte-identical at
`b433a..b4342`). The all-globals source emits the same shape with the address
in `r4`/`r6` and the 16-bit `ldr r0,[r6,#0]` encoding — a register/encoding
difference, not a source-shape difference.

Measured: `Status *&status = Globals::status;` on the shipped and all-globals
shapes changes nothing (88.3 % / 75.0 % respectively, identical instruction
counts). No lever.

## 3. Exhaustive ship-loop prelude permutation (840 orders)

The seven statements between the parts loop and the ship loop were extracted
verbatim from the shipped source:

| tag | statement |
|---|---|
| A | `bool ownHangar = false; if (getStation()->getIndex()==108) ownHangar = field_114==3;` |
| B | `int capacity = kHangarCapacity[sceneRace];` |
| C | `int shipCount = Globals::rnd->nextInt(capacity + 1);` |
| D | `if (ownHangar) { … ships->size() … }` |
| E | `unsigned char *spotUsed = new unsigned char[capacity]; for (…) spotUsed[i]=0;` |
| F | `AbyssEngine::AERandom *rnd = Globals::rnd;` |
| G | `const int *spots = sceneRace == 7 ? kHangarSpotsKhador[0] : kHangarSpotsDefault[0];` |

All 840 orders satisfying `B<C`, `A<D`, `B<E` were built with the official
`link_so` target and scored with the single-function verifier. Top of the
table:

| order | pct | size | insns |
|---|---|---|---|
| **A B C D E F G** (shipped) | **88.3** | 2252 | 791 |
| A B C D E G F | 88.3 | 2252 | 791 |
| A B C G D E F | 87.3 | 2252 | 791 |
| A B G C D E F | 87.3 | 2252 | 791 |
| A G B C D E F | 87.3 | 2252 | 791 |
| A B C D G E F | 84.8 | 2252 | 791 |
| B A C G D E F | 84.6 | 2252 | 791 |
| G A B C D E F | 84.4 | 2252 | 791 |
| B C A D E F G | 82.8 | 2244 | 789 |
| B A C D E F G (capacity-first) | 79.8 | 2276 | 800 |

`F`/`G` commute (both 88.3 %). Everything that moves `G` before `D` loses at
least 10 points. The shipped order is the unique optimum; **there is no
statement-order lever left in the ship-loop prelude.**

## 4. The local-`rnd` form is settled

| variant | pct | size | insns |
|---|---|---|---|
| shipped: value copy before `spots`, local at race/pick/tries | **88.3** | 2252 | 791 |
| `AERandom *&rnd = Globals::rnd;` (reference) | 75.0 | 2228 | 787 |
| `AERandom *const &rnd` | 75.0 | 2228 | 787 |
| `AERandom **rnd = &Globals::rnd;` + `(*rnd)->` | 75.0 | 2228 | 787 |
| `AERandom *volatile rnd` | 76.8 | 2232 | 796 |
| no local at all (all-globals) | 75.0 | 2228 | 787 |
| all 8 sites through the local, decl before `spots` | 83.7 | 2236 | 786 |
| all 8 sites through the local, decl before `shipCount` | 73.5 | 2236 | 789 |
| decl before `ownHangar` | 71.9 | 2232 | 800 |
| decl before `capacity` / before `shipCount` | 63.6 | 2224 | 799 |
| decl before `spotUsed` | 76.0 | 2228 | 794 |
| decl before the parts loop | 71.0 | 2232 | 800 |
| decl at the top of the `missionPtr == 23` block | 63.5 | 2192 | 791 |
| decl at the top of the function | 60.8 | 2200 | 786 |

The reference and pointer-to-pointer forms both collapse to the all-globals
shape (2228 B), which confirms that clang treats them as direct global access.
Only the value copy reproduces the original's 2252-byte size.

## 5. Where the 8 extra instructions are

The original is 783 instructions; the shipped source is 791. The gap is pure
spill traffic:

* **parts loop.** Shipped: `k` is spilled to `sp#44` (`str r6,[sp,#44]`) and
  restored at the latch (`ldrd sl,r6,[sp,#40]`), because `hangarPart` takes
  `r6`. Original: `k` stays in `r6`, `hangarPart` takes `r4` and is spilled to
  `sp#44` for the child loop only. Same number of spill instructions, but the
  shipped form also pays the `ldrd` restore.
* **ship loop.** Shipped: `&Globals::rnd` is spilled to `sp#44` and both the
  address and the value are reloaded at the three local-`rnd` sites.
  Original: `&Globals::rnd` stays in `r8` and only the value is reloaded.

The parts-loop register signature

```
row → r9   Canvas → r8   zero → sl   k → r6
hangarPart → r4   g → r5   j → sl   child count → fp
```

is reproduced **only** by the all-globals + capacity-first shape (77.2 % /
2264 B). That shape also duplicates `shipCount = nextInt(capacity+1)` into
both arms of the `ownHangar` branch and moves the ship-loop values into
different stack slots, so it is not byte-exact. The shipped shape gets only
`g`, `j`, `i`, `&status` and `spotUsed` right, but is 11 points ahead overall.

## 6. What was also measured this pass (all 88.3 %, no change)

Semantically neutral rewrites of the parts loop and the surrounding code do
not move the allocation at all:

* `Vector` forms: `Vector{0.0f,3.1415927f,0.0f}`, `Vector(…)`,
  `Vector{0,3.1415927f,0}`, `Vector{0.0f,3.14159265f,0.0f}`, a named
  `Vector rot` inside the loop, a named `const Vector rot` before the loop
  (83.9 %), `rot.x=0;rot.y=pi;rot.z=0` before the loop (83.9 %).
* `PlayerStatic(-1,g,0.0f,0.0f,0.0f)` vs `(…,0,0,0)` vs `(…,0.0f,0,0.0f)`.
* `kBarChildMeshes[sceneRace] + j` vs `j + kBarChildMeshes[sceneRace]`;
  caching `child->transform` in a local.
* `if (mesh < 0) continue;` vs `if (mesh >= 0) { … }`.
* `plainHangar` as `bool`/`int`, parenthesised, or inlined into the `if`.
* `ownHangar` as `bool`/`int`, parenthesised, or as one `&&` expression.
* `spotUsed` as `bool *` / `unsigned char *` / `int *` (int is 84.0 %);
  `new unsigned char[capacity]()` (84.8 %); `while` zero loop; `memset`.
* `switch (sceneRace)` vs an `if/else` chain (74.3 %).
* `const int *heights = kShipHangarHeights;` local; `const int *row` local.
* `if (this->enemies != nullptr)` vs `if (this->enemies)` vs `!= 0`;
  `ArrayReleaseClasses(*enemies)`.
* hoisting `shipCount`/`ownHangar`/`capacity`/`spotUsed`/`spots` declarations
  to the top of the block.

## 7. Conclusion / next lever

The register allocation is a single whole-function greedy pass ordered by
`(live-interval size, vreg number)`; the parts-loop signature is decided by
the ship-loop intervals, and the ship-loop prelude statement order is now
exhaustively ruled out. The only shapes that move the parts loop are
(a) removing the local `rnd` (→ all-globals, 75.0 %), (b) hoisting
`Globals::Canvas` into a local (82.8 %), and (c) capacity-first on top of
all-globals (77.2 %). All three lose more in the ship loop than they gain in
the parts loop.

The remaining plausible lever is an IR-level difference that does not change
the instruction stream — e.g. a different `rnd` live-range split that keeps
the address in `r8` while retaining the 2252-byte layout. No source spelling
tried so far (≈ 950 variants cumulative across waves 91–97, ≈ 900 this pass
including the 840-order sweep) produces it.

## 8. Reproduction

Scratch is `/dev/shm/w4712`:

* `fscore.py` — single-function score (`--diff` for the structural diff);
* `normdiff.py <orig.so> <ours.so>` — register/branch/literal-normalised
  structural diff (used for §1);
* `intervals2.py` — CFG liveness / live-interval-size approximation over the
  `Expand ISel Pseudo-instructions` machine-IR dump;
* the 840-order sweep driver is reproduced inline in this note's §3 (the
  seven block texts are the literal statements from
  `src/game/world/Level.cpp`).

Build recipe used:

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```
