# w8214 (wave-137): `Level::createMission` — faithful tail-loop argument order (fuzzy 92.10 -> 92.34 %)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8214`, based on master `655e3bcc5`.
Continues `level-createmission-wave136-w8112.md`, `…-wave135-w7776.md`,
`…-wave134-w7755.md`, `…-wave133-w7733.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; `_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17172 B).
Manual Thumb disassembly (objdump) plus a fresh Ghidra decompile via
`rea decompile … --procedure _ZN5Level13createMissionEv`
(`GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC`). Build/flags unchanged
(NDK r18b clang 7.0.2, `-Oz`).

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   official fuzzy 92.1033 % (report 92.2)   size 17172/17172 B
                             orig 6055 / ours 6064 insns
.text diff_bytes 84,709 (whole section)
.ARM.extab 1,395   .ARM.exidx/.rodata/.data/.bss exact
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## New hypothesis: the tail loop's argument evaluation order

The previous waves rejected the faithful one-liner tail because the **positional**
`raw fn differing bytes` metric rose 5761 -> 5763 (the `strd r0,fp,[sp]` becomes
`str r0,[sp]; movs r0,#1; str r0,[sp,#4]`, a 2-byte shift).  But that metric is
positional: a 2-byte insertion near the end of the loop shifts the rest of the
symbol and inflates the count.  The **official** verifier metric is a
`difflib` ratio over the normalized instruction stream, which is not confused by
that shift.  Re-scoring the rejected variant with the official metric shows it is
a genuine win.

### Ghidra decompile of the original tail loop

```c
for (uVar50 = 0; uVar50 < **(uint **)(this + 0xf8); uVar50 = uVar50 + 1) {
    iVar11 = Globals::getRandomEnemyFighter(Globals::globals, 8);
    pAVar8 = Globals::rnd;
    pRVar21 = *(Route **)(this + 0x108);
    iVar14 = Route::length(pRVar21);
    iVar14 = AbyssEngine::AERandom::nextInt(pAVar8, iVar14);
    pWVar29 = (Waypoint *)Route::getWaypoint(pRVar21, iVar14);
    uVar12 = createShip(this, 8, 0, iVar11, pWVar29, true, false);
    *(undefined4 *)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar50 * 4) = uVar12;
    KIPlayer::setToSleep(*(KIPlayer **)(*(int *)(*(int *)(this + 0xf8) + 4) + uVar50 * 4));
}
```

The original evaluates `getRandomEnemyFighter(8)` **first**, then the waypoint
index, then `createShip`.  The checked-in `int idx` temp evaluated the waypoint
index first, which also swapped the tail register map (`this` r9 / `i` r8
instead of the original `this` r8 / `i` r9) and reloaded `this->playerRoute`
twice.

### Original / checked-in tail disassembly

Original (`b416e`):

```text
b416e: ldr.w r0,[sl]              ; Globals::globals
b4172: movs r1,#8
b4174: blx   getRandomEnemyFighter
b4178: mov   r5,r0                ; type
b417a: ldr   r0,[sp,#120]         ; &Globals::rnd
b417c: ldr.w r4,[r8,#264]         ; this->playerRoute
b4180: ldr   r6,[r0]              ; rnd
b4182: mov   r0,r4
b4184: blx   Route::length
b4188: mov   r1,r0
b418a: mov   r0,r6
b418c: blx   AERandom::nextInt
b4190: mov   r1,r0
b4192: mov   r0,r4
b4194: blx   Route::getWaypoint
b4198: strd  r0,fp,[sp]           ; wp, true
b419c: movs  r0,#0
b419e: str   r0,[sp,#8]           ; false
b41a0: mov   r0,r8                ; this
b41a2: movs  r1,#8
b41a4: movs  r2,#0
b41a6: mov   r3,r5                ; type
b41a8: blx   createShip
b41ac: ldr.w r1,[r8,#248]         ; this->enemies
b41b0: ldr   r1,[r1,#4]
b41b2: str.w r0,[r1,r9,lsl #2]    ; enemies->data[i] = ship
b41b6: ldr.w r0,[r8,#248]
b41ba: ldr   r0,[r0,#4]
b41bc: ldr.w r0,[r0,r9,lsl #2]
b41c0: blx   setToSleep
b41c4: add.w r9,r9,#1
b41c8: ldr.w r0,[r8,#248]
b41cc: ldr   r0,[r0,#0]           ; re-read enemies->size()
b41ce: cmp   r9,r0
b41d0: bcc.n b416e
```

The checked-in `int idx` form instead emitted `Route::length`/`nextInt` first,
then `getRandomEnemyFighter`, and reloaded `this->playerRoute` for
`getWaypoint`; `this`/`i` were swapped (r9/r8).

### Landed source

```cpp
(*this->enemies)[i] = this->createShip(
        8, 0, Globals::globals->getRandomEnemyFighter(8),
        this->playerRoute->getWaypoint(Globals::rnd->nextInt(this->playerRoute->length())), true, false);
(*this->enemies)[i]->setToSleep();
```

This restores the original call order and the original tail register map
(`this`=r8, `i`=r9, `Globals::globals` via `sl`).  The only remaining tail
difference is that `&Globals::rnd` lives in `fp` instead of `[sp,#120]`, so the
`true` argument is materialised as `movs r0,#1; str r0,[sp,#4]` instead of the
original `strd r0,fp,[sp]` (2 bytes).  That does not hurt the instruction-level
match; the whole tail now lines up instruction-for-instruction.

## Measurements (fresh `link_so` each)

Official metric is `tools/verify/verify.py`'s `difflib` ratio over the
normalized instruction stream (reproduced locally with the same
`disassemble`/`reconcile`/`score` calls; exact ratio shown unrounded).

| variant | size | insns (orig/ours) | ratio |
|---|---:|---:|---:|
| **baseline `int idx`** | **17172** | 6055/6064 | **92.1033** |
| **one-liner tail (landed)** | **17172** | 6055/6065 | **92.3432** |
| one-liner + `int type` temp | 17172 | 6055/6065 | 92.3432 |
| one-liner + cached `Route *route` | 17172 | 6055/6065 | 92.3432 |
| one-liner + `int i` loop counter | 17172 | 6055/6065 | 92.3432 |
| one-liner + `KIPlayer *ship` after store | 17172 | 6055/6065 | 92.3432 |
| one-liner + non-volatile `n` latch | 17172 | 6055/6065 | 92.1947 |
| one-liner + faithful `this->enemies->size()` | 17180 | 6055/6065 | — (size +8) |
| one-liner + pirate-fortress `cargo` before `shipGroup` | 17172 | 6055/6065 | 92.2937 |
| one-liner + pirate-fortress `setScaling` last | 17172 | 6055/6065 | 92.2772 |
| one-liner + case-183 `[8]` `shipGroup`/`field_0x74` swap | 17172 | 6055/6065 | 92.3432 |
| one-liner + case-183 `[9]` swap | 17172 | 6055/6065 | 92.3267 |
| one-liner + alien-turret faithful order | 17172 | 6055/6062 | 92.0 (rounded) |

The one-liner is the unique best shape.  The `int type` temp, cached route and
`int i` spellings coalesce to the identical stream.

## Alien blocks / mission-object store order (re-verified, no change)

The Ghidra decompile confirms the alien bodies and every mission-object store
are already in the original order:

* Big alien wave (`createStaticObject(...,18804,true)`): store into
  `enemies[i]`, `setSpacePoints(0)`, `setPosition(pos)`, `setMoving(false)`,
  `player->setHitpoints(9999999)`, `delete cargo; cargo = nullptr`,
  `field_0x70 = 0`.
* Small alien ship: `createShip(localRace,1,14,...)`, store, `setMoving(false)`,
  three RNG draws, `setPosition`.
* Pirate fortress (`19051`): store, `setSpacePoints`, `setPosition`,
  `setMoving(false)`, `setHitpoints(9999999)`, `setName(getText(1667))`,
  `field_0x70 = 0`, `delete cargo; cargo = nullptr`.
* Alien turret loop (`6772`): `createStaticObject`, store, `gun = enemies[i]`,
  `setMaxHitpoints(1000)`, table select, rotate/setRotation, rotate/setPosition,
  `setScaling`, `cargo = nullptr`, `k += 1`, `shipGroup = localRace`.
  The checked-in tuned order (`setScaling` before `setRotation`, `shipGroup`
  before `cargo`, fresh `(*enemies)[i]` receiver) still scores better than the
  faithful order (92.0 vs 92.34 overall), because the original's in-loop `cbz`
  table select is not reachable from source under clang 7.0.2 (the
  `(bigAlienWave + (i < 0))` anchor if-converts to `cmp.w r1,r9,asr #31;
  it ne; movne`).  Anchor sweeps this wave (`k < 0`, `(i>>31)`, nested ternary,
  `bigAlienWave && i>=0`, `&`, volatile copy) are all neutral or worse.

## Gate counts (before -> after)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      84,709 -> 84,711  (+2 positional bytes; fuzzy 92.1033 -> 92.3432)
.ARM.extab diff  1,395 -> 1,395
```

The +2 `.text` positional bytes are the `strd` -> `movs/str` split described
above; the instruction-level fuzzy ratio (the metric that reflects source
fidelity) rises by **+0.24 pp** and the function size is unchanged at 17,172 B.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8214/score.py     # exact official fuzzy ratio for the one function
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch: `/dev/shm/w8214/{Level.base.cpp,measure.sh,rawfn.py,score.py,variant3.py,
orig.asm,ours.asm,gh_decomp.c}`.
