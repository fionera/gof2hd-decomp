# LevelScript::process — wave 155 / w10102: cheap faithful mission-0 loop
# (`(*enemies)[i]` + live `enemies` bound) at 57,232 B, raw -204 / aligned +4

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10102`, base master
`f4e0ea40b` (wave 154, w10007 landed).  Scratch in `/dev/shm/w10102/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,327            55,123            (-204)
process aligned insns    17,176            17,180            (+4)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## 1. The landed source change

```cpp
for (int i = 0; i < 3 && enemies->data_ != nullptr; ++i) {
    KIPlayer *e = (*enemies)[i];
    e->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
    e->geometry->setVisible(true);
}
```

The key is that `enemies` stays **live across the loop** (the `data_` null
bound re-reads it every iteration), which is what the original's loop does
(`ldr.w r9,[sp,#96]` then `ldr.w r0,[r9,#4]`).  But the per-element pointer is
loaded once into `e`, so the body is one `ldr.w r4,[r0,r5,lsl#2]` + two
`ldr r0,[r4,#8]` instead of the original's two full
`data_[i]->geometry` chains.  That is the cheap shape.

## 2. Why the w10004 faithful loop was expensive

The w10004 faithful loop (`(*enemies)[i]` for **both** calls) is the only
shape that puts `&Globals::status` in **r4** (matching the original), but it
re-colours the whole function: `this`->r9 (orig sl), canary->sl (orig fp),
`player`->fp (orig spilled), frame 0x1a8 (orig 0x1b0).  Measured on this base:

| loop shape | size | raw | aligned | status |
|---|---:|---:|---:|---|
| HEAD `enemyData` local (double load) | 57232 | 55,327 | 17,176 | r9 |
| faithful `(*enemies)[i]` both (double load) | 57352 | 55,462 | **21,884** | **r4** |
| merged `KIPlayer *e = (*enemies)[i]` (single load) | 57224 | 55,187 | 17,195 | r9 |
| hoist `data_` outside loop, `enemyData[i]` both | 57232 | 55,329 | 17,191 | r9 |
| **landed: merged + live `data_` bound** | **57232** | **55,123** | **17,180** | r9 |

The `(*enemies)[i]` *anywhere* in the loop body (even one of the two calls)
triggers the +4,7xx re-colour; only a single element load avoids it.  The
`data_` bound is the smallest source change that keeps `enemies` live (and
therefore the loop's `enemies`-in-a-register shape) without the second
element chain.

## 3. Prologue register map (deliverable)

Landed (cheap) build vs original:

| value | original | landed |
|---|---|---|
| `this` | `sl` | **`fp`** |
| `&__stack_chk_guard` | `fp` | **`sl`** |
| `&m_pLevel` (temp) | `r4` | `r4` |
| `messages` | `r5` | `r5` |
| `delta` | `r6` | `r6` |
| `enemies` | `r8` | `r8` |
| `player` | spilled from `r0` to `[sp+84]` | **spilled from `r0` to `[sp+80]`** |
| `&Globals::status` | **`r4`** | **`r9`** (vtable in `r4`) |
| frame size | `0x1b0` (432) | **`0x1b0`** (432, match) |
| canary slot | `[sp+428]` | `[sp+428]` (match) |
| stack slots | msg/delta `[sp+72]`, &level `[sp+80]`, player `[sp+84]`, this `[sp+88]`, status `[sp+92]`, enemies `[sp+96]` | same layout, player `[sp+80]`, status `[sp+92]`, enemies `[sp+96]` |

So the landed shape recovers the **original frame size and stack layout**
(0x1b0, `player` spilled from r0) but leaves `this`/canary swapped
(`fp`/`sl` vs `sl`/`fp`) and `status` in `r9` rather than `r4`.  The
faithful double-load loop is the only shape that puts `status` in `r4`, at
the cost of the +4,708 aligned re-colour.

## 4. Sweep (all built + linked on the wave-154 base)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| faithful double `(*enemies)[i]` | 57352 | 55,462 | 21,884 |
| faithful, no status local | 57344 | 55,091 | 21,843 |
| merged local (status local) | 57224 | 55,187 | 17,195 |
| merged local, no status local | 57224 | 55,188 | 17,191 |
| merged `enemies->data_[i]` | 57224 | 55,187 | 17,195 |
| merged `const` / `&` | 57224 | 55,187 | 17,195 |
| hoist `data_`, `enemyData[i]` both | 57232 | 55,329 | 17,191 |
| hoist `data_`, local `e` | 57328 | 55,455 | 21,253 |
| pointer-increment `for (p = data_; ...)` | 57364 | 55,363 | 21,607 |
| **merged + `data_` bound** | **57232** | **55,123** | **17,180** |
| merged + `m_bStartSequenceOver = 0;` | 57340 | 55,522 | 17,385 |
| faithful + `__builtin_expect(state0)` | 57480 | 55,407 | 23,011 |

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10102/fdiff.py _ZN11LevelScript7processEi        # 55,123 / 57,232
python3 /dev/shm/w10102/align_diff.py _ZN11LevelScript7processEi  # 17,180
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## 6. Open questions

1. The landed loop is not the original's exact `(*enemies)[i]` double load;
   it is the cheapest shape that keeps `enemies` live.  A shape that keeps the
   double load **and** the original `this`=sl / canary=fp allocation would
   remove the +4,708 aligned outright; the allocator lever is not yet found.
2. `status` r9 vs r4 remains the correlated delta.  `player` now spills
   correctly (frame 0x1b0), so the remaining swap is `this`/canary only.
3. The `data_ != nullptr` bound is a plausible defensive guard, but the
   original emits `cmp r5,#3; bne` (not `cmp #2; bls`); a bound that keeps
   `enemies` live without changing the comparison is still open.
