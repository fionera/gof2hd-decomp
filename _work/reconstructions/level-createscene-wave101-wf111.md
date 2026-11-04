# `Level::createScene` — wave-101 wf111: 2-D `rnd`-local sweep (declaration position ×
# use-site mask) is an exhaustive dead end; P0/`0x29` is the global optimum

**Status:** research note, 2026-11-04. Worktree `wt/wf111`, branch `wave101-wf111`,
base `3856949ab`. **No source change landed** — no source shape found that raises
`byte_exact`. This commit contains only this note.

Follow-up to `level-createscene-wave100-w5012.md` (which followed
`…-wave99-w4911.md`, `…-wave98-w4814.md`, `…-wave97-w4712.md`,
`…-wave95-w4514.md`, `…-wave94-w4414.md`, `…-wave92-w4249.md`,
`…-wave92-w4213.md`, `…-wave91-w4113.md`, `level-createscene-phi-use-w4013.md`,
`level-createscene-licm-hoist-w3833.md`, `Level-createScene-flag-free.md`,
`level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`.
* Gate re-measured before and after this pass, unchanged and green:
  **byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN ✓, drift 0, RATCHET PASS.**
* **New: the `rnd`-local declaration position × use-site mask space is now
  exhaustively swept (5 positions × 128 masks = 640 full `link_so` builds).**
  The shipped shape (`rnd` declared before `spots`, sites `{top, pick, tries}`
  = mask `0x29`) is the **unique global optimum at 88.3 %**. Every other
  declaration position is strictly worse: before `spotUsed` max 87.2,
  before `ownHangar` max 75.0, before the parts loop max 75.0, inside the
  loop max 87.9. §3.
* The corrected 7-bit mask sweep at the shipped declaration position
  reproduces the wave-98/99/100 result exactly (max `0x29` = 88.3, next
  `0x61` = 88.2, `0x31` = 88.1). §2.
* The local-inside-loop forms are all worse: shipped sites 86.8, all sites
  83.3, all-globals sites 75.0. §4.
* Ship-loop body statement re-orderings (spot before createShip, spotUsed
  after the switch, friend/sleep before setPosition, rotation before
  setPosition, exhaust before rotation, ArrayAdd before rotation, sleep
  before friend) are all ≤ 88.1. §5.
* `Vector` construction forms (parens, named local in/out, default+assign,
  copy-ctor, int zeros, double pi) are all 88.3. `static Vector` collapses
  to 71.4. §6.
* **`capacity`-first with the local `rnd` gives the *shipped* parts loop,
  not the original's** (row `r8`, Canvas `r9`): the local `rnd` value cache
  masks the wave-100 `B<A` lever. So the original's parts loop can only be
  reached from the all-globals shape, which in turn breaks the early
  allocation. §7.
* The scoped `-mllvm -sink-insts-to-avoid-spills` flag on `Level.cpp`
  measures **89.3 % / 2256 B** on this base (not the 93.3 % of pass 92 and
  not size-exact), so it is not a candidate under the size-exact gate. §8.

## 1. Baseline diff and register map

`tools/verify/verify.py --show _ZN5Level11createSceneEv` at the baseline:
291 `|` lines. The first 459 instructions are byte-identical; the first diff
is instruction 460, the parts-loop preheader. The instruction stream is the
same on both sides apart from the `rnd` handling (ours caches the value in
`r5`, spills `&Globals::rnd` to `sp#44`; the original keeps `&Globals::rnd`
in `r8` and reloads the value at every site). Register maps (unchanged from
wave-98/99/100):

| value | original | shipped |
|---|---|---|
| parts `row` | `r9` | `r8` |
| parts `Globals::Canvas` addr | `r8` | `r9` |
| parts zero | `sl` | `r4` |
| parts `k` | `r6` | spilled `sp#44` |
| parts `hangarPart` | `r4` (spilled) | `r6` |
| parts `j` / child count | `sl` / `fp` | `sl` / `r8` |
| ship `&Globals::rnd` | `r8` | spilled `sp#44` |
| ship `rnd` value | never cached | `r5` |
| ship `capacity` | `r6` | `fp` |
| ship `race` | `r5` | `r6` |
| ship `type` | `fp` | `r8` |
| ship `i` | `r9` | `r9` |
| ship `&Globals::status` | `sl` | `sl` |
| ship `spotUsed` | `r4` | `r4` |

## 2. Corrected 7-bit mask sweep at the shipped declaration position

The wave-100 handoff's mask numbering is the 7-bit set
`0 top, 1 nextInt(4), 2 inner, 3 pick, 4 int spot, 5 tries, 6 rotation`
(the `shipCount` site is not in this set). The shipped source is `0x29`
(`top, pick, tries`). The sweep was re-run with a corrected site-5 pattern
(the bare `spot = Globals::rnd->nextInt(capacity);` is a substring of the
`int spot = …` line, which corrupted a naive sweep). Results (full table in
`/dev/shm/wf111/sweep_P0.json`):

| mask | sites | pct | size | insns |
|---|---|---|---|---|
| **0x29** | top, pick, tries (shipped) | **88.3** | 2252 | 791 |
| 0x61 | top, tries, rotation | 88.2 | 2252 | 791 |
| 0x31 | top, int-spot, tries | 88.1 | 2252 | 791 |
| 0x68 | pick, tries, rotation | 87.7 | 2252 | 791 |
| 0x69 | top, pick, tries, rotation | 87.7 | 2252 | 791 |
| 0x70 | int-spot, tries, rotation | 87.7 | 2252 | 791 |
| 0x38 | pick, int-spot, tries | 87.5 | 2252 | 791 |
| 0x39 | top, pick, int-spot, tries | 87.4 | 2252 | 791 |
| 0x78 | pick, int-spot, tries, rotation | 86.9 | 2252 | 791 |
| 0x22 | nextInt(4), tries | 85.5 | 2252 | 794 |

This exactly reproduces the wave-98/99/100 maximum.

## 3. New: 2-D declaration-position × mask sweep (5 × 128 = 640 builds)

Declaration positions (`DECL = AbyssEngine::AERandom *rnd = Globals::rnd;`):

| tag | position |
|---|---|
| P0 | before `spots` (shipped) |
| P1 | before `spotUsed` (after `shipCount`'s `if`) |
| P2 | before `shipCount` |
| P3 | before `ownHangar` |
| P4 | before the parts loop (`plainHangar`) |
| P5 | inside the ship loop, before `int race = sceneRace;` |

Best mask per position (full tables in `/dev/shm/wf111/sweep_P{0,1,3,4,5}.json`):

| position | best mask | pct | size | insns |
|---|---|---|---|---|
| **P0** | **0x29** | **88.3** | 2252 | 791 |
| P1 | 0x69 | 87.2 | 2252 | 791 |
| P3 | 0x00 (all-globals) | 75.0 | 2228 | 787 |
| P4 | 0x00 (all-globals) | 75.0 | 2228 | 787 |
| P5 | 0x68 / 0x69 | 87.9 | 2252 | 791 |

(P2 is the wave-99 "before shipCount" position; the handoff already measured
its best at 63.6 %, so it was not re-swept.)

**Conclusion:** the shipped declaration position is the unique optimum. The
`rnd`-local placement space is exhausted.

## 4. Local-inside-loop forms (all worse)

| variant | pct | size | insns |
|---|---|---|---|
| `rnd` inside the loop, shipped sites (`top,pick,tries`) | 86.8 | 2252 | 791 |
| `rnd` inside the loop, all sites | 83.3 | 2244 | 790 |
| `rnd` inside the loop, all-globals sites | 75.0 | 2228 | 787 |

Declaring the local inside the loop lets the compiler reload the value per
iteration; it collapses to the all-globals IR (or a worse variant).

## 5. Ship-loop body statement re-orderings (all ≤ 88.1)

| variant | pct | size | insns |
|---|---|---|---|
| shipped (base) | **88.3** | 2252 | 791 |
| `spotUsed[spot]=1` after the switch | 87.9 | 2252 | 791 |
| `setExhaustVisible` before `setRotation` | 87.9 | 2252 | 791 |
| `ArrayAdd` before `setRotation` | 87.8 | 2252 | 791 |
| `setToSleep` before `setAlwaysFriend` | 88.1 | 2252 | 791 |
| `spot` before `createShip` | 61.5 | 2252 | 803 |
| friend/sleep before `setPosition` | 63.3 | 2280 | 797 |
| `setRotation` before `setPosition` | 59.8 | 2272 | 801 |

The shipped statement order is the unique local optimum.

## 6. `Vector` construction forms (all 88.3 except `static`)

| variant | pct | size | insns |
|---|---|---|---|
| `Vector{0.0f, pi, 0.0f}` (base) | **88.3** | 2252 | 791 |
| `Vector(0.0f, pi, 0.0f)` (parens) | 88.3 | 2252 | 791 |
| named `Vector rot(...)` inside the loop | 88.3 | 2252 | 791 |
| `const Vector rot(...)` inside the loop | 88.3 | 2252 | 791 |
| `Vector rot; rot.y = pi;` | 88.3 | 2252 | 791 |
| copy-ctor `Vector rot(Vector(...))` | 88.3 | 2252 | 791 |
| `Vector{0, pi, 0}` (int zeros) | 88.3 | 2252 | 791 |
| `(float) 3.141592653589793` | 88.3 | 2252 | 791 |
| `static Vector rot; rot.y = pi;` | 71.4 | 2232 | 777 |

## 7. `capacity`-first with the local `rnd` gives the *shipped* parts loop

Wave-100 proved that on the **all-globals** shape, every prelude order with
`capacity` (B) before `ownHangar` (A) reproduces the original's parts loop
(`row r9, Canvas r8, zero sl, k r6, hangarPart r4`). On the **shipped
(local-`rnd`) shape**, swapping A/B to `B A C D E F G` measures
**79.8 % / 2276 B / 800 insns** and its parts loop is the **shipped** one
(`row r8, Canvas r9, zero r4, k spilled, hangarPart r6`), not the original's.
The local `rnd` value cache therefore masks the wave-100 lever. This is the
concrete reason the two known shapes cannot be combined: the all-globals
shape has the right instruction stream but a rotated allocation; the
local-`rnd` shape has the right early allocation but the wrong ship loop.

## 8. Scoped sink-flag measurement on this base

Adding `-mllvm -sink-insts-to-avoid-spills` to `Level.cpp`'s compile options
(per-file `set_source_files_properties`, reverted) gives
**89.3 % / 2256 B / 790 insns** — better fuzzy score but a **size
regression** (2252 → 2256), so it is not a candidate under the
`size-exact 4524/4524` gate and is not the flag-free goal. All-globals +
flag is 75.9 % / 2232 B.

## 9. What this pass proves / what remains

**Proved:**

1. The `rnd`-local declaration-position × use-site-mask space is exhausted;
   P0/`0x29` is the unique global optimum at 88.3 %.
2. The corrected 7-bit mask sweep reproduces the prior maximum exactly.
3. The local-inside-loop, ship-loop body re-order and `Vector`-form spaces
   are all dead ends.
4. `capacity`-first only helps the all-globals shape; the local `rnd` masks
   it (shipped parts loop at 79.8 %).
5. The sink flag is not a size-exact fix (2256 B).

**Remaining lever (unchanged):** a source shape that keeps the shipped
prologue/early allocation (`&Globals::status` in `sl`, `this` in `r6`) while
making the ship loop use `&Globals::rnd` in `r8` with the value reloaded at
every site (no `r5` value cache), and giving `capacity` `r6`, `race` `r5`,
`type` `fp`. Equivalently: the all-globals IR with the shipped shape's
global-interval queue. No source spelling tried so far (≈1700 variants
cumulative across waves 91–101) produces it.

## 10. Reproduction

Scratch is `/dev/shm/wf111`:

* `Level.cpp.base` — pristine `src/game/world/Level.cpp`;
* `fscore.py` — official single-function score (imports `tools/verify`);
* `v.py` — apply `[name, [[old,new],…]]` replacements, build `link_so`, score;
* `sweep2d.py <P0|P1|P3|P4|P5> <out.json>` — the 2-D sweep (§3);
* `sig.py <disasm>` — parts-loop register-signature extractor;
* `orig.txt` / `ours.txt` / `allg.txt` / `capfirst.txt` — disassemblies;
* `sweep_P*.json`, `sweep_P*.log` — raw sweep results.

Build recipe:

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level11createSceneEv
```
