# `Level::createScene` — wave-122 w7215: `&Globals::rnd` live-range census, no source win

**Status:** dead-end sweep, 2026-11-06. Worktree `wt/w7215`. Target
`_ZN5Level11createSceneEv` (`src/game/world/Level.cpp`). No source change
landed. This note corrects two claims in
`level-createscene-wave121-w7114.md`, records the exact greedy-RA
eviction cascades (from `llc -debug-only=regalloc`) for the three
reachable basins, and lists the new variants measured.

## TL;DR

* Baseline and best remains the **landed** source: **92.8 % / 2252 B**
  (`byte_exact 4506`, `linked_exact 4506`, lint CLEAN, drift 0).
* **Correction to wave-121:** the landed source is the **trick** parts
  form (`Vector *rp = (Vector *) ((char *) &rot - (size_t) g);` with
  `rot.y; rot.z; rot.x;`), *not* the natural form. `grep -n "size_t) g"`
  confirms it at line 6625.
* **The `uxth` in the `pac` variant is load-bearing and costs 4 bytes.**
  `pac` is 2256 B, so it cannot be landed even if the `capacity`/
  `spotUsed` swap were fixed (its `uxth r4,r4` is exactly the extra
  4 bytes). Every "free split" alternative (`int cap = capacity;`,
  `cap + 0`, `capacity * 1`, ternary, recompute, …) collapses back to
  the 2228 B / 75 % basin.
* The three reachable basins and their greedy-RA eviction cascades are
  now fully characterised (§3). In every basin `capacity` is **first**
  assigned `r6` (matching the original) and then evicted by a
  higher-weight loop value; the original simply never evicts it.
* ~120 new variants measured. **None beat 92.8 % at 2252 B.**

## 1. Which variants keep `byte_exact` at 4506

`byte_exact` in `tools/verify/verify.py` is `ob == ub` where `ob` is the
original bytes at the original address and `ub` is our bytes at *our*
address. A size change only breaks it when the following function's
PC-relative encodings move, so not every size change is fatal:

| variant | pct | size | byte | linked |
|---|---|---|---|---|
| **landed (trick parts, local `rnd`, no cap)** | **92.8** | **2252** | **4506** | **4506** |
| `pac-xyz` (all-global, natural parts, `unsigned short cap`) | 89.8 | 2256 | 2360 | 4506 |
| `pac-yzx` (all-global, `y,z,x` parts, `unsigned short cap`) | 89.8 | 2252 | 4506 | 4506 |
| plain `g->setRotation(rot)` on the landed base (`x,y,z`) | 85.0 | 2248 | 4506 | 4506 |
| all-global + natural parts + no cap (`AGN`) | 75.0 | 2228 | 2360 | 4506 |
| all-global + natural parts + angle temp (`AT`) | 80.0 | 2224 | 3087 | 4506 |

So the only landable improvements are shapes that keep the size at
2252 (or 2248) **and** raise the score. The landed 92.8 % is the best
of those.

## 2. Register maps (target vs the three basins)

Target (from the original disassembly, `orig_cs_full.txt`):

| value | original |
|---|---|
| `spotUsed` | r4 |
| `race` | r5 |
| `capacity` | r6 |
| `&Globals::rnd` | r8 |
| `i` | r9 |
| `status` / `ship` | sl |
| `type` | fp |
| `ownHangar` | r4 (then `[sp,#24]`) |
| `this` | r6 early, `[sp,#48]` later |

| variant | capacity | &rnd | spotUsed | race | type | status | parts |
|---|---|---|---|---|---|---|---|
| **landed** | fp | `[sp,#44]` (value r5) | r4 | r6 | r8 | sl | trick |
| `pac` | r4 | **r8** | r6 | **r5** | **fp** | **sl** | **natural** |
| `AGN` | r8 | r9 | r4 | r5 | fp | `[sp,#44]` | natural |

`pac` matches the original on `&rnd`, `race`, `type`, `status`, `i` and
the parts loop; only `capacity`/`spotUsed` are transposed (plus the
`uxth`).

## 3. Greedy-RA eviction cascades (`llc -debug-only=regalloc`)

`llc` v7.0.0 (assertions build) with
`-mtriple=thumbv7-none-linux-androideabi -mcpu=cortex-a8 -mattr=+neon
-float-abi=soft -O2 -disable-fp-elim -debug-only=regalloc` on the
`clang -Oz -S -emit-llvm` IR reproduces the real `-Oz` allocation
(verified: `pac` `%27`→r4, `%32`→r6, `%557`→r8, `%562`→r11; landed
`%27`→r11, `%3`→r10, `%553`→r2/spill). The vreg numbers match the
`-mllvm -print-machineinstrs` MIR exactly.

### 3.1 landed (trick + local `rnd`)

```
%27 capacity  [6768,11904)            -> r6
%46 sceneRace [9936,11776) w=0.0279   evicts %27 from r6   (sceneRace is hotter)
%27            -> r11 (fp)
%3  status    [928,11920)             -> r10 (sl)          (matches original)
%34 rnd value [7792,11904)            -> r5               (local pointer, value kept)
%553 &rnd     -> r2 then spilled to [sp,#44]
```

### 3.2 pac (all-global + natural + `unsigned short cap`)

```
%557 &Globals::rnd [6880,11952)       -> r6
%32 spotUsed       [7456,11984) w=.0285 evicts %557 from r6
%557               -> r8               (matches original)
%35 cap            [7776,11952)       -> r4  (evicts %123=this from r4)
%27 capacity       [6752,7776)+[8032,8128) -> r4
%46 sceneRace      -> r5               (matches original)
%3  status         -> r10 (sl)         (matches original)
%562 type          -> r11 (fp)         (matches original)
```

The `t2UXTH` splits `capacity` into `%27` (short, pre-loop) and `%35`
(loop-carried `cap`). That split is what moves `&Globals::rnd` from
r9 into r8 — but it costs the extra `uxth r4,r4` (4 bytes).

### 3.3 AGN (all-global + natural + no cap)

```
%555 &Globals::rnd -> r8
%27  capacity      -> r6               (matches original!)
%32  spotUsed      -> r4               (matches original!)
%492 ship->geometry [11376,11584) w=.0257 evicts %27 from r6
%27                -> r8 (evicts %555)
%555               -> r9
```

`AGN` is *exactly* the original for the first three assignments; the
only thing that breaks it is the `ship->geometry` load (the
`setRotation(0f, nextInt(300)/100f, 0f)` argument) evicting `capacity`
from r6 because `capacity`'s normalised weight (0.0245) is below
`geometry`'s (0.0257). `weight = UseDefFreq / (size + 25*InstrDist)`,
so the long `capacity` interval is penalised.

`AT` (`float angle = ...; ship->geometry->setRotation(0f, angle, 0f);`)
lengthens the geometry interval enough to stop that eviction
(`&rnd` stays r8, `spotUsed` r4), but then a loop PHI `%561`
(w=0.4139) evicts `capacity` from r6 instead, and `status` is pushed
out of `sl`.

## 4. New variants measured (all no-win)

Each was compiled with the repo's exact NDK r18b `-Oz` flags and the
register map read from the object disassembly.

* `pac` with every `cap` type: `unsigned short`/`short`/`unsigned
  char`/`signed char` → `r4/r6/r8` basin (2256 B); `int`/`unsigned`/
  `long`/`unsigned int` → `r8/r4/r9` basin (2228 B). The 16-bit type is
  what forces the `uxth`.
* "free split" attempts (all → 2228 B, `r8/r4/r9`): `int cap =
  capacity;`, `(int) capacity`, `capacity + 0`, `capacity * 1`,
  `capacity - 0`, `capacity | 0`, `ownHangar ? capacity : capacity`,
  `int cap = kHangarCapacity[sceneRace];`, `cap` declared before
  `if (ownHangar)`, `cap` used only for the array size.
* `pac-yzx` (2252 B, `r4/r6/r8`) perturbations — all identical:
  `ownHangar` as `int`/`unsigned char`/`&&` expression, `const int
  capacity`, `char *spotUsed`, `unsigned shipCount`.
* landed-base perturbations — all identical (`fp/r4/r6/r8`, 2252 B):
  `unsigned capacity`/`shipCount`/`race`/`type`/`spot`/`tries`/`i`,
  `bool *spotUsed`, `char *spotUsed`, `(unsigned) capacity` cast,
  `spots` moved before `spotUsed`, `AERandom *const rnd`.
* `rnd` local placement on the landed base: before `capacity` 2236 B
  (`r8/r5/r4/r9`), before `if (ownHangar)` 2236 B, before `spotUsed`
  2228 B, before the zero loop 2224 B, before `spots` 2252 B (same
  basin), before the ship loop 2252 B (same basin).
* `AERandom *volatile rnd` 2240 B, `capacity=r8 race=r5 type=fp` —
  closest on `race`/`type` but `capacity` still wrong and the size is
  2240.

## 5. Remaining root cause

`capacity` is allocated `r6` first in **every** basin (it is the
earliest long-lived interval), but is then evicted by whichever
loop-local value happens to have a higher normalised spill weight:
`sceneRace` (landed), `ship->geometry` (`AGN`), or the `spot` PHI
(`AT`). The original keeps it because in its MIR the evicting value is
either shorter or already has a cheaper register. The `pac` `uxth`
sidesteps the problem by shortening `capacity`'s interval, but adds the
4 bytes that make the function 2256 B.

A landable fix therefore needs a **no-instruction live-range split of
`capacity`** (so `capacity` is short for the `new`/zero-loop part and a
long loop-carried copy exists) that the coalescer does not fold away,
while keeping the natural `x,y,z` parts loop. Every source-level split
tried so far is either coalesced (→ 2228 B basin) or emits an
instruction (→ 2256 B).

## 6. Reproduction

```
OBJDUMP=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --show _ZN5Level11createSceneEv ...
cmake --build cmake-build-match --target check -j6
```
