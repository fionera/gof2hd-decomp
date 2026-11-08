# Small tail wave 154 (w9917): Gun / MGame::OnUpdate — new hypotheses,
# no source win (note only)

Status: **note only, no `src/` change.** Worktree `wt/w9917`, base master
`7f7f5cf19` (wave 153). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave153-w9917.md`, `small-tail-wave152-w9817.md`,
`small-tail-wave151-w9717.md`.

Gate before/after: byte_exact **4507 -> 4507**, linked_exact **4507 -> 4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
RATCHET PASS, avg 99.93.  Both targets stay size-exact.

## 0. Residuals (raw same-address bytes / aligned instructions)

| target | size | raw before | raw after | aligned before | aligned after |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` | 1984 B | 463 | 463 | 57 | 57 |
| `MGame::OnUpdate` | 11440 B | 318 | 318 | 19 | 19 |

Mangled names: `_ZN3Gun22calcCharacterCollisionEv`, `_ZN5MGame8OnUpdateEv`.

## 1. Gun::calcCharacterCollision — StackColoring source confirmed; the
##    inline-`getPosition` shape fixes the *stack layout* but swaps r4/r5

Fetched the real LLVM 7.0.2 `StackColoring.cpp` (release/7.x; the wave-153
scratch copy was a 404).  Confirmed the algorithm: `std::stable_sort` of the
frame indices by `getObjectSize` **descending** (stable => ties keep frame
index order), then the `I/J` greedy merge
(`StackColoring.cpp:1225-1277`).  Both 60-byte objects (`fi#10` = `Matrix
view`, `fi#11` = the dead `MatrixSetTranslation` sret temp) are tied, so the
sort order is `[view, sret]` and cannot be flipped by a same-size local: the
sret temp is created at the call site, i.e. after every entry-block alloca,
so it always has the higher frame index.  A `Matrix translated;` declared
before `view` is optimised away (no alloca, measured identical to HEAD).

The wave-153 "playerPosition must overlap view" conclusion is still the only
way to get the original assignment, but there is a second, independent lever:

**NEW result:** inlining the scatter-block `getPosition()` call

```cpp
int distance = (int) AbyssEngine::AEMath::VectorLength(
        enemyPosition - ((Level *) this->level)->getPlayer()->getPosition());
```

(no named `playerPosition` local) gives the **original stack layout**:
`getPosition` sret temp at `sp+44`, the `enemyPosition - playerPosition` temp
at `sp+104`, `view` at `sp+104` — i.e. the wave-153 "delta at sp+104 /
playerPosition at sp+44" target.  Measured **1984 B, raw 467, aligned 67**
(HEAD 463/57).  The remaining difference is a pure **r4<->r5 swap**: the
original keeps `i*3` in r4 and `sp+104` in r5, this shape keeps `i*3` in r5
and `sp+104` in r4 (`add.w r4/r5, fp, fp, lsl #1` at 0x15306c; `add
r4/r5, sp, #104` at 0x1530dc).  Every later `add rN, sp, #104` / `mov rN, r5`
follows from that one choice.  So the stack colouring is solved; the residual
is the i-loop register allocation.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **463** | **57** | 1984 |
| inline `getPosition`, direct subtraction | 467 | 67 | 1984 |
| inline `getPosition` + named `Vector delta` | 467 | 67 | 1984 |
| `Matrix translated;` before `view` (unused) | 463 | 57 | 1984 |
| `Matrix translated = camera; view = translated` | 464 | 57 | 1984 |
| `view` uninitialised then assigned | 1817 | 243 | 2056 |
| `view = MatrixSetTranslation(view, ...)` | 1087 | 101 | 1992 |
| `Matrix translated = MatrixSetTranslation(...)` | 463 | 57 | 1984 |
| `view` at function scope | 1863 | 327 | 2016 |

Remaining untested idea: force `i*3` into r4 in the inline shape (e.g. a
live `unsigned i3 = i * 3;` used for the `positions`/`velocities` indexing)
so the allocator gives `sp+104` r5; that should reproduce the original
`add.w r4, fp, fp, lsl #1` / `mov r2, r5` pair.

## 2. MGame::OnUpdate — the `mov r0,r5` is a *value* live across the
##    createRadioMessage call; removing it always loses the r9 anchor

The IR (`-S -emit-llvm`) shows the real mechanism.  `Globals::status` is
loaded fresh at every use (the compiler does **not** CSE the value across
calls); what is kept in r9 is `&Globals::status` (the GOT-resolved address of
the pointer global), which is invariant.  In HEAD:

```
%1218 = load %class.Status*, %class.Status** @_ZN7Globals6statusE  ; condition field_0x100
...
call void @llvm.dbg.value(metadata %class.Status* %1218, ...)      ; Status *st = %1218
%1241 = call i64 @_ZN6Status14getPlayingTimeEv(%class.Status* %1218)
%1242 = load %class.Status*, %class.Status** @_ZN7Globals6statusE  ; store base (fresh)
```

`st` is the condition's `field_0x100` load, hoisted into r5 and live across
`createRadioMessage`; the store's base is a fresh load from r9.  That extra
live value is exactly what forces `&Globals::status` into r9 (without it the
whole chain uses r5 and the function is 48 B shorter, 11392).  Removing the
`mov` therefore requires keeping the r9 allocation without keeping a second
value alive.

New variants measured this wave (all 11440 B unless noted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (value anchor at pt store) | **318** | **19** | 11440 |
| `st` ccm+store via st (block scope) | 37 | 109 | 11440 |
| `st` ccm via st, direct store | 914 | 109 | 11436 |
| `st` decl after radioIndex | 922 | 127 | 11444 |
| `st` decl after radioBase | 908 | 125 | 11444 |
| chain-scope `st`, all chain uses | 5151 | 815 | 11384 |
| chain-scope `Status &st = *Globals::status`, all uses | 5377 | 1131 | 11428 |
| chain-scope `Status **stp = &Globals::status`, `(*stp)` | 5148 | 597 | 11392 |
| nested `else { Status *st; if (122) ... }`, st in 122 only | 4561 | 63 | 11432 |
| nested `else`, st in next block only | 4568 | 374 | 11432 |
| plain direct (no anchor) | 5148 | 597 | 11392 |

Note the `st` ccm+store variant is only **raw 37** — the whole function is
byte-identical to the original except for the two `mov r0,r5` sites and the
branch-target index shifts they cause (aligned 109).  That is the closest
raw shape found so far and may be a better base for a future pass than HEAD
if the `mov`s can be removed.

Remaining untested idea: a value that is live from the mission-122 condition
to the store but is *not* `Globals::status` (e.g. a `long long`/`int` temp
used only after the call), which would keep r9 allocated while leaving the
store a fresh `ldr r0,[r9]`.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9917
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9917/fdiff.py <mangled>
python3 /dev/shm/w9917/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w9917/`: `fdiff.py`, `align_diff.py`,
`adiff2.py`, `cc.sh`, `build_measure.sh`, `mgdump.sh`, `sweep_mg3..14.py`,
`sweep_gun6..9.py`, `Gun.base.cpp`, `MGame.base.cpp`, `gun_all.txt`,
`StackColoring.cpp` (real LLVM 7.0.2 source).

## 4. Files changed

* `_work/reconstructions/small-tail-wave154-w9917.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS, avg 99.93.
