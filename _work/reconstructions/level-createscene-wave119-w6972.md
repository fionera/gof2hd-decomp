# `Level::createScene` — wave-119 w6972: parts-loop store order solved, no score win

**Status:** dead-end sweep, 2026-11-06. Worktree `wt/w6972`, branch
`wave119-w6972` (base master `d512ec24e`). Target
`_ZN5Level11createSceneEv`, `src/game/world/Level.cpp`. No source change
landed; this note records the measurements and two new structural findings
so the next pass does not repeat them.

Continuation of `level-createscene-wave119-w6939.md` (the most complete
prior note: full measurement tables, MIR section, reproduction commands),
`…-w6929.md`, `…-w6934.md`, `…-w6903.md`, `…-w6820.md`.

## TL;DR

* Baseline and best: **92.7573 % / 2252 B**, 791/783 instructions, first
  diff at normalized instruction 478 (parts-loop store schedule).
* **New finding 1 (parts loop):** the original's three separate stores in
  the order *y, x, z* are produced by the **natural source order
  `rot.x = 0; rot.y = pi; rot.z = 0;`** (or `z, y, x`), not by the landed
  `y, z, x` order. `y` and `z` source-first orders make LLVM 7.0.2 merge
  `x`/`y` into a `strd`; `x`-first or `z`-first orders do not. Verified
  in isolation and in the function (see §1).
* **New finding 2 (ship loop):** an `unsigned short cap =
  (unsigned short) capacity;` used at the two `nextInt(capacity)` sites
  (especially the nested retry loop) **reproduces the original ship loop**
  — `r8 = &Globals::rnd`, value reloaded `ldr.w r0,[r8]`, `r5 = race`,
  `fp = type`, `r9 = i`, `sl = status/ship` — and, with the `x,y,z` parts
  order, **also reproduces the original parts loop exactly**. The
  resulting variant (`pac-xyz`) matches the original instruction stream
  from 478 to 547 and from 603 to 623 verbatim; its only remaining
  differences are a `capacity`/`spotUsed` **r4↔r6 register swap**, one
  extra `uxth`, and a cyclic permutation of the spill slots.
* `pac-xyz` scores **89.9235 % / 2256 B / 785 insns**, still below the
  landed 92.7573 %, so it is not landable as-is. But it is the closest
  structural match to the original found in any session and the most
  promising base for a future pass: fixing the r4↔r6 swap alone would
  lift it far above the landed score.
* ~60 new source variants measured. **None beat 92.7573 %.**

## 1. Parts loop: the store order is the source order

Original (instructions 480-487, 8 insns, 26 bytes):

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

Note the store order **y, x, z** and the fact that `x`/`y` are *not*
merged into a `strd` (they are adjacent: sp+56 / sp+60).

Isolated experiment (same NDK r18b clang 7.0.2 flags, `-Oz`, a plain
`struct Vector { float x, y, z; }` and an out-of-line `setRotation`):

| source order | emitted stores |
|---|---|
| `y; z; x` (landed) | `str z` + `strd x,y` (merged) |
| `y; x; z` | `str z` + `strd x,y` (merged) |
| `x; y; z` | `str y` ; `str x` ; `str z` — **3 stores, order y,x,z** |
| `z; y; x` | `str y` ; `str x` ; `str z` — **3 stores, order y,x,z** |

So the original source is almost certainly the natural ascending
`rot.x = 0.0f; rot.y = 3.1415927f; rot.z = 0.0f;` (or the descending
`z,y,x`), *not* the landed `y,z,x`.

In the real function the same rule holds, but only in the all-global
basin (see §2): `pac-xyz` reproduces the original 8-instruction parts
loop byte-for-byte (`ldr pi; add r1,sp,#56; str y; mov sl,#0; mov r0,r5;
str x; str z; blx`). In the landed (trick) basin the `x`-first order
collapses the whole allocation (first diff 28) — see §3.

## 2. The `unsigned short cap` variant (`pac-xyz`)

Source shape (all-global `Globals::rnd->` at every ship-loop site, plain
`g->setRotation(rot)`, natural `x,y,z` parts order):

```cpp
Vector rot;
rot.x = 0.0f;
rot.y = 3.1415927f;
rot.z = 0.0f;
g->setRotation(rot);
...
int capacity = kHangarCapacity[sceneRace];
int shipCount = Globals::rnd->nextInt(capacity + 1);
if (ownHangar) { ... }
unsigned char *spotUsed = new unsigned char[capacity];
for (int i = 0; i < capacity; i++) spotUsed[i] = 0;
unsigned short cap = (unsigned short) capacity;      // <-- the lever
const int *spots = sceneRace == 7 ? kHangarSpotsKhador[0] : kHangarSpotsDefault[0];
for (int i = 0; i < shipCount; i++) {
    int race = sceneRace;
    if (Globals::rnd->nextInt(100) <= 29) {          // A
        race = Globals::rnd->nextInt(4);             // B
        if (Globals::rnd->nextInt(100) < 30) race = 8;
    }
    ...
    int spot = Globals::rnd->nextInt(cap);           // E
    for (int tries = 100; tries != 0 && spotUsed[spot] != 0; tries--)
        spot = Globals::rnd->nextInt(cap);           // R (nested loop)
    ...
}
```

Result: **89.9235 % / 2256 B / 785 insns / first diff 9**.

What matches the original exactly:
* the whole prologue (0-8 except one spill slot),
* the parts loop (478-487) — 3 separate stores, y/x/z order,
* the entire ship-loop head and race roll (603-623): `ldr.w r0,[r8]`,
  `movs r1,#100`, `blx nextInt`, `ldr r5,[sp,#52]`, `cmp r0,#29`, ...,
  `mov fp,r0` (type),
* the `switch (sceneRace)` bit-test preheader (`uxth r0,r1; movw r1,#267;
  lsr.w`).

What still differs:
1. **`capacity` and `spotUsed` are swapped between r4 and r6.**
   Original: `r6 = capacity`, `r4 = spotUsed`. `pac-xyz`: `r4 = capacity`,
   `r6 = spotUsed`. This is the single largest source of diffs (every
   `cmp r1,r6` / `mov r1,r6` / `strb r0,[r4,r1]` in the loop body).
2. **One extra `uxth r4, r4`** in the ship-loop preheader (the
   `(unsigned short)` truncation). It is exactly compensated by the
   parts loop being 4 bytes shorter than... nothing — the parts loop is
   *correct*, so the function is 4 bytes longer than the original
   (2256 vs 2252) and every literal-pool offset after it is +4.
3. **Spill slots are cyclically permuted**: guard-ptr `[sp,#40]` vs
   `[sp,#36]`, status `[sp,#44]` vs `[sp,#40]`, `spots` `[sp,#28]` vs
   `[sp,#44]`, shipCount `[sp,#32]` vs `[sp,#28]`, and one slot at
   `[sp,#36]` vs `[sp,#32]`.
4. `ownHangar` lands in **r5** instead of r4 (`lsrs r5,r0,#5` vs
   `lsrs r4,r0,#5`).

### 2.1 Why `unsigned short` works (MIR analysis)

Pre-RA MIR (`-mllvm -print-machineinstrs`, "After Machine Instruction
Scheduler") for `pac-xyz`:

```
%589:rgpr = t2LSRri %417:rgpr, 5          ; ownHangar      (bb.30)
%27:rgpr  = t2LDRs %420:tgpr, %3:rgpr, 2  ; capacity       (bb.31)
%35:rgpr  = t2UXTH %27:rgpr, 0            ; cap            (preheader)
```

`%35` is live across the **nested retry loop**, which gives it enough
spill weight to be kept in a callee-saved register; the `t2UXTH` **splits
`capacity`'s live range** into a short pre-loop part and a long
loop-carried part. That split is what changes the global allocation from
the `74.7 %` all-global basin to the `89.9 %` basin. A plain
`int cap = capacity;` is coalesced away and does nothing
(`ag-cap-plain` = 74.7 %).

Consequence: the `uxth` is *load-bearing*. Removing it (e.g. by making
`capacity` itself `unsigned short`, which would move the truncation to
the `kHangarCapacity` load) reverts the basin.

## 3. Measurements (all 2252 B unless noted)

Scored with the official single-function verifier (unrounded difflib
ratio). `fd` = first differing normalized instruction index.
`fd 4/6/28` = basin change; `fd 477/478` = parts-loop store schedule.

### 3.1 Parts-loop store order

| variant | pct | size | insns | fd |
|---|---|---|---|---|
| **landed: trick `- (size_t) g`, y,z,x** | **92.7573** | 2252 | 791/783 | 478 |
| trick, y,x,z | 92.6302 | 2252 | 791/783 | 477 |
| trick, x,y,z | 88.3100 | 2252 | 791/783 | 28 |
| trick, z,y,x | 88.1830 | 2252 | 791/783 | 28 |
| plain `g->setRotation(rot)`, x,y,z | 84.9524 | 2248 | 792/783 | 28 |
| plain, z,y,x | 84.9524 | 2248 | 792/783 | 28 |
| plain, y,x,z | 84.8793 | 2248 | 791/783 | 28 |
| plain, y,z,x | 84.8793 | 2248 | 791/783 | 28 |

(The 4-byte size difference between the trick and plain forms is exactly
the `subs r1, r0, r5` address computation.)

### 3.2 All-global `Globals::rnd` ship loop

| variant | pct | size | insns | fd |
|---|---|---|---|---|
| all-global + plain parts, y,z,x (no cap) | 74.7449 | 2224 | 785/783 | 6 |
| all-global + plain parts, x,y,z (no cap) | 75.0318 | 2228 | 787/783 | 6 |
| all-global + plain parts, z,y,x (no cap) | 75.0318 | 2228 | 787/783 | 6 |
| all-global + plain parts, y,x,z (no cap) | 74.7449 | 2224 | 785/783 | 6 |
| **all-global + plain parts, x,y,z + `unsigned short cap`** | **89.9235** | 2256 | 785/783 | 9 |
| all-global + plain parts, z,y,x + cap | 89.7959 | 2256 | 785/783 | 9 |
| all-global + plain parts, y,z,x + cap | 89.7829 | 2252 | 783/783 | 9 |
| all-global + plain parts, y,x,z + cap | 89.6552 | 2252 | 783/783 | 9 |
| trick (y,z,x) + all-global + cap | 89.4133 | 2256 | 785/783 | 9 |
| trick (x,y,z) + all-global + cap | 87.7707 | 2260 | 787/783 | 9 |

### 3.3 `pac-xyz` perturbations — all identical at 89.9235 / 2256 / fd 9

`bool ownHangar` → `int` / `unsigned char` / single `&&` expression;
`int capacity` → `unsigned` / `const int`; `int race` declared outside
the loop; `unsigned spot`; `unsigned type`; `KIPlayer *const ship`;
`char *spotUsed`; `const int *table = spots;`; `unsigned shipCount`;
`unsigned tries`; `KIPlayer *ship = nullptr; ship = ...`;
`unsigned char *spotUsed = nullptr; spotUsed = new ...;`;
`unsigned short cap = capacity;` (implicit cast); `unsigned char cap`;
`(void) capacity;` after the loop; `while` form of the retry loop;
`if (ownHangar != 0)`; `if/else` form of the `shipCount` ternary;
`cap` used only at the retry site (`cap-retry`);
`spotUsed`/`cap` declared before `capacity`.

Other perturbations:

| variant | pct | size | fd |
|---|---|---|---|
| `pac-xyz` + zeroing loop moved after `spots` | 88.5350 | 2260 | 9 |
| `pac-xyz` + `capacity` declared before `ownHangar` | 87.2543 | 2276 | 9 |
| `pac-xyz` + `int c2 = capacity;` used at the first `nextInt` | 75.0797 | 2228 | 6 |
| `pac-xyz` + `cap` used only at the first `nextInt` | 74.7616 | 2236 | 6 |
| `pac-xyz` + `unsigned i` in the zeroing loop | 89.7959 | 2256 | 9 |

`cap` as `int` / `unsigned` / `long` / `const int&` / `int&` = 74.7-75.4 %
(no split, no basin change).

## 4. The remaining lever: the `capacity`/`spotUsed` r4↔r6 swap

Original ship loop register map:

| value | original | `pac-xyz` |
|---|---|---|
| `spotUsed` | **r4** | r6 |
| `race` | r5 | r5 |
| `capacity` | **r6** | r4 |
| `&Globals::rnd` | r8 | r8 |
| `i` | r9 | r9 |
| `status` / `ship` | sl | sl |
| `type` | fp | fp |
| `ownHangar` | r4 | r5 |
| `this` | `[sp,#48]` | `[sp,#48]` |

`pac-xyz` differs by exactly one transposition. In the original,
`ownHangar` takes r4 and `capacity` skips r5 (which `race` later occupies)
to take r6; `spotUsed` then reuses r4 after `ownHangar` is spilled. In
`pac-xyz`, `capacity` takes r4 first, `ownHangar` takes r5, and
`spotUsed` is pushed to r6. The greedy allocator's processing order for
these three intervals is the whole difference, and none of the ~60
source perturbations tried moves it.

Untried ideas for the next pass:
* force `ownHangar` into r4 by giving it an earlier, longer live range
  (e.g. a witnessed original local in the mission-2/4 branches that is
  live across a loop);
* find a *free* live-range split of `capacity` (the `uxth` is the only
  extra instruction; if the split can be made without an instruction the
  variant would be 2252 B and every literal-pool offset would line up);
* reorder the local declarations so the spill-slot cycle
  (36/40/44/28/32 → 40/44/28/32/36) closes.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6972
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
```

Scratch harness (not committed): `/dev/shm/w6972/` — `Level.base.cpp`
(landed source), `score.py` (official verifier import, unrounded ratio +
size + first-diff index), `build.sh` (relink + score, ~5 s),
`mkpac.py` (constructs the `pac`/`pac-xyz` source), `ndiff.py`
(per-instruction diff dump), `t28.py` … `t42.py` (this session's
variant batches), `mir2.txt` (full `-print-machineinstrs` for
`pac-xyz`), `show_pac2.txt` (landed-vs-`pac` side-by-side).

## 6. Gate

Re-measured green after restoring the landed source:
**byte_exact 4505/4524, linked_exact 4505/4524, stub_zero_size 0,
missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.** 27 sections, file size 2,187,328 B.
No source change landed.
