# w4714: `Level::createMission` (90.721649 %, 17172 B) — wave-97: the direct-init's
# raid-block collapse is a *block-placement* difference, not a raid-block source problem

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w4714`, branch `wave97-w4714`,
base `9b58a39d5`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address − 0x10000).
**No source change is committed**: every variant below is neutral or a regression.
The deliverable is this note.

## Baseline (unchanged, gate green before and after)

```text
_ZN5Level13createMissionEv   fuzzy 90.721649485 %   size 17172 B   @0xaffd8
                             orig 6055 insns / ours 6070 insns
gate: byte 4500 / linked 4500, size 4524/4524, stubs 0, missing 0, wrong_type 0,
      extra 0, data 843/843, imports 207, .text 0x1337c8, .rodata 0x41b67,
      .ARM.exidx 0x6f70, lint CLEAN, drift 0, RATCHET PASS
```

`cmake --build cmake-build-match --target check -j8` passes before and after this
pass. `report.json` is not modified/committed.

## Result in one line

The `alienGuns` `Vector` direct-initialisation **is** the original's shape (13
`Vector::operator=` PLT calls and exactly 6055 instructions, both matching the
original), but it is still not landable: it re-lays-out the `if (raid)` block
from "out of line, reached by `b.n`" to "inline, fall-through", which costs
~150 matched instructions. **New evidence in this pass:** that re-layout is a
*final block-placement* difference (the emitted MBB order is identical for the
first 321 of 392 blocks and diverges exactly at the `if (pirateFortress)` block,
`Level.cpp:1972`), and the raid block's own source is **IR-neutral** in the
direct-init context — four different spellings of `raidType` / `hard` / the
stream-out test all compile to byte-identical 17144 B / 6055 insns. So the raid
block cannot be repaired from its own source; the lever is whatever changes the
MBB order after the `alienGuns` loop.

## 1. The direct-init `D` is proven, and its 6055-instruction count is exact

```cpp
            Vector turretDir = {t[3], t[4], t[5]};
            v = MatrixRotateVector(rotation, turretDir);
            gun->geometry->setRotation(v);
            Vector turretPos = {t[0], t[1], t[2]};
            v = MatrixRotateVector(rotation, turretPos);
```

```text
  variant   size    ours insns   raw score
  baseline  17172   6070         90.721649   (11 PLT + 3 GOT `operator=` calls)
  D         17144   6055         88.406276   (13 PLT `operator=` calls == original)
```

`D` reproduces the original's call count *and* the original's instruction count
exactly; the 28-byte size gap is branch-relaxation / literal-pool width, not a
missing statement (17172/6055 = 2.837 B/insn vs 17144/6055 = 2.831 B/insn).

## 2. New: the raid-block damage is a block-placement difference

Wave-95 §1 claimed "block placement is identical between baseline and D". That
is **not** true of the final layout. Compiling both with the exact `-Oz` recipe
and comparing the emitted `@ %bb.N` order inside `_ZN5Level13createMissionEv`:

```text
  baseline: 392 emitted MBBs, D: 392 emitted MBBs
  identical for emission index 0..320
  first divergence at index 321:
      baseline[321..323] = [496, 499, 503]
      D       [321..325] = [495, 497, 500, 502, 503]
```

`%bb.492` is the `alienGuns` loop body (the `Matrix` zero-init at
`Level.cpp:1961`) and `%bb.496` (baseline) / `%bb.495` (D) is the
`if (pirateFortress)` test (`.loc 47 1972`). The divergence is therefore the
**landing-pad run of the `alienGuns` loop**: `D` has one fewer EH pad there
(wave-95 measured 688 vs 689 pads), and every block after that run — including
the raid block — shifts. The raid block itself is emitted at emission index
**339 in baseline** but **324 in D** — 15 slots earlier.

This is directly visible in the linked disassembly. Both the original and the
baseline reach the raid block with a forward `b.n`; `D` falls straight through
into it:

```text
  original  b34d8  add r3, r2
            b34da  cmp r4, r3
            b34dc  blt.n loc_3426
            b34de  ldr r4, [sp, #124]
            b34e0  ldr.w r8, [sp, #108]
            b34e4  ldr.w sl, [sp, #52]
            b34e8  b.n loc_377e          <-- raid block is out of line

  baseline  b34dc  ...
            b34e2  ldr r5, [sp, #124]
            b34e4  ldr r6, [sp, #52]
            b34e6  b.n loc_3780          <-- same shape as the original

  D         b34d0  ...
            b34d0  ldr r5, [sp, #124]
            b34d2  ldr r6, [sp, #52]
            b34d4  ldr r3, [sp, #48]
            b34d6  adds r4, r6, r3
            b34d8  ldr r3, [sp, #88]
            b34da  str r4, [sp, #112]
            b34dc  cmp r3, #0
            b34de  beq.w loc_395c        <-- raid block is inline
```

So the ~150-instruction raid-block loss in `D` is mostly *layout*, not a
different instruction selection: the same raid-block code sits at a different
address and the surrounding branches relax differently.

## 3. New: the raid block's own source is IR-neutral in the `D` context

Four independent source spellings of the raid prologue were applied **on top of
`D`** and measured. All four produced a **byte-identical** object:

| `D` + raid-prologue variant | size | insns | raw score |
|---|---:|---:|---:|
| `bool hard` (as-is) | 17144 | 6055 | 88.406276 |
| `int hard = …hardCoreMode();` | 17144 | 6055 | 88.406276 |
| swap the `raidType` / `hard` declaration order | 17144 | 6055 | 88.406276 |
| `Level::initStreamOutPosition && raidRace == 8` (condition order swapped) | 17144 | 6055 | 88.406276 |
| `20 + (hard ? 20 : 0) + getLevel()` | 17144 | 6055 | 88.406276 |
| `int raidStart = guns + …; for (i = raidStart; i < raidStart + hostileCount; i++)` | 17148 | 6057 | 88.342140 |
| `int raidEnd = …; for (i = …; i < raidEnd; i++)` | 17140 | 6054 | 88.397060 |

The first five are byte-identical, so the raid-block *source* is not the lever —
consistent with §2: the block is emitted in a different position and its
registers/spills follow from that.

## 4. `alienGuns` source-shape sweep (all reverted)

Every shape below replaces the baseline block (which is
`Vector turretPos;` + `turretPos = Vector{t[0],t[1],t[2]};`). Object sizes are a
direct `Level.cpp` compile (NDK r18b clang 7.0.2, `-Oz`); score is the official
`verify.py` normalisation (`autojunk=False`).

| variant | size | ours insns | raw score |
|---|---:|---:|---:|
| **baseline** | **17172** | **6070** | **90.721649** |
| `D` direct-init after `setRotation` | 17144 | 6055 | 88.406276 |
| `turretPos;` first, then field-wise `x/y/z` | 17160 | 6061 | 88.395510 |
| same, `z/y/x` order | 17160 | 6061 | 88.395510 |
| `turretDir` first, then `turretPos;`, field-wise | 17160 | 6061 | 88.511060 |
| `turretPos` direct-init **before** `turretDir` | 17144 | 6056 | 88.547601 |
| `float *tp = turretPos; tp[0..2] = t[0..2];` | 17168 | 6063 | 88.364417 |
| `turretDir` inline in the call + `turretPos = Vector{…}` | 17172 | 6070 | 90.705155 |
| both vectors in their own `{}` scopes | 17152 | 6061 | 88.296467 |
| `turretPos;` hoisted above the `for` | 17168 | 6068 | 88.228986 |
| both vectors hoisted above the `for` | 17184 | 6073 | 88.242084 |
| `const float *t; if (bigAlienWave) t = …; else t = …;` | 17320 | 6077 | 61.259479 |
| `bigAlienWave ? kAlienStationTurrets[k] : kAlienOutpostTurrets[k]` | 17312 | 6074 | 61.274631 |
| `(bigAlienWave == 1 ? … : …)[k]` | 17172 | 6070 | 90.721649 (neutral) |

Nothing beats the baseline. Note that *every* shape that removes the third
`operator=` call collapses the raid block to ~88.4 %, so the baseline's extra
copy-assign is load-bearing for the block layout.

## 5. `convoy` switch (`Level.cpp:2185`): the if-chain order is canonicalised

The original lowers the `k == 2 / 1 / 0 / else` chain as
`cmp #2; beq case2; cmp #1; beq case1; cbnz r1, default` with the blocks laid
out `case0, case1, case2, default` (ascending). Ours lays them out
`case2, case1, case0, default` (source order). Every source reordering was
measured:

| variant | size | insns | raw score |
|---|---:|---:|---:|
| **baseline (source order 2,1,0)** | **17172** | **6070** | **90.721649** |
| source order `0,1,2` | 17172 | 6070 | 90.573196 |
| source order `0,2,1` | 17172 | 6070 | 90.573196 |
| source order `1,0,2` | 17172 | 6070 | 90.573196 |
| source order `1,2,0` | 17172 | 6070 | 90.573196 |
| source order `2,0,1` | 17172 | 6070 | 90.721649 (byte-identical to baseline) |
| `switch (k) { case 0/1/2 + default }` | 17172 | 6070 | 90.573196 |
| `switch (k) { case 0/1/2/3 }` (no `default`) | 17172 | 6069 | 90.646651 |

`clang` canonicalises the chain, so only `2,1,0` and `2,0,1` reproduce the
baseline; the original's ascending block order is not reachable from source
order. The `switch` forms land on the same `0,2,1` layout as source order
`0,1,2`.

## 6. `b2e04`: `bool alienWave` reproduces the merged test but costs 1.5 pp

The original's first station test is a *merged* predicate
(`b2e04 subs.w r0, fp, r3; mov r1, r0; it ne; movne r1, #1; ldr r2, [sp, #60];
eor.w r2, r2, #1; orrs r1, r2; bne.w loc_2f52`), i.e.
`(i != first) | !alienWave` — that is the `&&`-of-two-cheap-values form clang
emits when the second operand is known to be 0/1. Ours emits two separate
branches. Making `alienWave` a `bool` does materialise the merged form (the
`b2e04` cluster disappears from the diff), but it also sinks the three
zero-initialisations in the `bigAlienWave/alienWave/pirateFortress` prologue and
shifts the EH pads:

| variant | size | insns | raw score |
|---|---:|---:|---:|
| **baseline** | **17172** | **6070** | **90.721649** |
| `bool alienWave = false;` | 17156 | 6064 | 89.215282 |
| `bool alienWave` + `bool bigAlienWave` | 17156 | 6064 | 89.215282 |
| `bool alienWave` + `bool pirateFortress` | 17180 | 6077 | 67.111771 |
| all three `bool` | 17184 | 6077 | 79.821958 |
| `(i == first) & alienWave` | 17160 | 6067 | 74.476159 |
| `alienWave && i == first` | 17172 | 6070 | 90.721649 (neutral) |
| `bool atStation = i == first;` then `atStation && alienWave` | 17172 | 6070 | 90.721649 (neutral) |
| `(i == first) && (alienWave != 0)` | 17172 | 6070 | 90.721649 (neutral) |

So the merged form is reachable, but the `bool` costs more elsewhere than the
`b2e04` cluster is worth.

## 7. Method / commands

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
# exact Level.cpp compile (NDK r18b clang 7.0.2, -Oz), see /dev/shm/w4714/cc.sh
clang++ -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  -stdlib=libc++ ... -ffunction-sections -funwind-tables -c src/game/world/Level.cpp
# emitted MBB order (block placement)
clang++ ... -S -o Level.s src/game/world/Level.cpp     # then diff the `@ %bb.N` order
# official score (same normalisation as verify.py, autojunk=False)
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump <ndk objdump> \
  --show _ZN5Level13createMissionEv
```

Scratch (not committed): `/dev/shm/w4714/` (`cc.sh`, `measure.sh`, `score.py`,
`tryvar.py`, `tryvar2.py`, `addr_of.py`, `nstream.py`, `nrange.py`,
`gen_ag.py`, `genconv.py`, `variants/`, `Level.cpp.base`, `streams*.json`,
`base97.s`, `D97.s`).

## 8. What remains

1. **Land `alienGuns` direct-init together with the original's raid-block
   placement.** `D` is proven (13 PLT calls, 6055 insns) and only loses on
   layout. The missing piece is what makes `MachineBlockPlacement` keep the
   `if (raid)` block *out of line* (reached by `b.n`) as the original and the
   baseline do. The divergence begins in the `alienGuns` loop's landing-pad run
   (`%bb.492` = loop body, `%bb.496`/`%bb.495` = the `if (pirateFortress)` test),
   i.e. it is decided before the raid block is even considered — so the lever is
   the number/order of throwing invokes in the `alienGuns` loop, not the raid
   block.
2. **`turretDir`/`turretPos` slot sharing** (`sp+200` for both in the original).
   `D` puts `turretDir` at `sp+212`, `turretPos` at `sp+200`, sret temp at
   `sp+188`; the original has both vectors at `sp+200` and the temp at `sp+212`.
   No spelling tried so far merges them.
3. **`b3dd0` bound order** (`start + pirates + extraAliens`) is proven but −8 B
   (90.457732); it needs an 8-byte compensation elsewhere.
4. **`b2e04` merged predicate** is reachable via `bool alienWave` but the
   `bool` costs 1.5 pp in the prologue + EH pads.
5. **`convoy` switch** ascending block order is not reachable from source order.
6. **EH pads**: still the largest single cluster (~50 unmatched instructions at
   the tail, `b41ea`–`b42cc`); block order only, invoke-order driven.
