# `Gun::calcCharacterCollision` pass 16: the wave-103 oracle confirms the shipped body is
# `F P H L K` with no `dmg` local, but the tuned tree body (`F H K L P` + `dmg`) still
# scores higher (90.0 vs 84.1/64.1) — no code change landed (wf313, wave 103)

Worker: wf313. Branch: `wave103-wf313`, base master `1d2b4398e`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Oracle (new in wave 103):
`/home/debian/GalaxyOnFire2/_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/Gun.c`
(`Gun::calcCharacterCollision` at `@0x0017e154`, 1968 B; KiritoJPK NDK-r14/clang-3.8 `-O2`).
Scratch: `/dev/shm/wf313*`.

## 0. Result

**No code change landed.** The tree body (`F H K L P` tail, `int dmg = this->damage;`) remains
the best measured shape: **90.0 %**, 1984/1984 B, 584/650 identical normalized instructions
(official verifier). Gate after: byte 4500 / linked 4500, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`.

| build | gate byte/linked | target pct | target size |
|---|---:|---:|---:|
| before (HEAD `1d2b4398e`) | **4500 / 4500** | **90.0 %** | **1984 B** |
| after this pass (note only) | **4500 / 4500** | **90.0 %** | **1984 B** |

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`, `H` = `setHitVector`,
`L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

## 1. What the oracle proves about the real source

The oracle decompilation is a different compiler and a slightly different source revision
(localization/DeviceInfo differ), but its control flow is unambiguous and agrees with the
shipped disassembly on every source-shape question that matters here:

1. **No `int dmg` local.** The oracle assigns `iVar9` in each damage arm and calls
   `Player::damage(this_02, iVar9, …)` once after the merge:
   ```
   if ((owner == 0) || (owner->enemyFlagsLo != 0)) {
       if (egoFlag && level->getPlayer() && level->getPlayer()->isDockedToDockingPoint())
           dmg = (int)(damage * 3.0f * 0.25f);
       else
           dmg = damage;
   } else if (egoFlag) {
       dmg = (int)(damage * 0.2f);
   } else {
       dmg = damage;
   }
   Player::damage(enemy, dmg, friendGun, itemIndex);
   ```
   The shipped binary reloads `this->damage` at all three sites (`0x1532b8`, `0x1532d4`,
   `0x153308`) and tail-merges the two plain arms at `0x153304`, exactly like the oracle.
2. **Tail order is `F P H L K`** (`0x153316` field_54, `0x15331c` playerGun, `0x15332a`
   setHitVector, `0x153340` lifetimes, `0x15334a` lastHitKIPlayer) — the oracle has the same
   order (`this_02[0x54]=1; if (this[0xf0]) …; setHitVector; lifetimes; lastHitKIPlayer`).
3. **Scatter loop is a plain `for (k = 0; k < enemies->size(); k++)`** — the oracle is
   top-tested, not a `do { … } while (++k < size)`.
4. **`explodeOnContact = bomb || mine`**, `explosive = bomb || rocketlike`, and the scatter
   radius branch (`mine` → `*5`; `scatter` → distance-based `*0.5/*2/*1.5/*3`) match our tree
   body. The `empDamage != kNoAttribute`, `empDamage > 0`, `itemIndex == 179`, medal 41/44,
   `emitManual(..., -1.0f)` and `impact` blocks all match.
5. The oracle's `[0x88]`/`[0x3c]` KIPlayer offsets are the *other* source revision; the shipped
   binary and our `Gun.cpp` use `[0x8c]`/`[0x40]` (`ldrb.w r1,[r0,#140]` / `[r0,#64]`).

So the faithful source is the oracle shape, **not** the tree body. The tree body is a tuned
shape: it caches `dmg`, emits three `enemy->damage(...)` calls, and orders the tail `F H K L P`.

## 2. Measured variants (official verifier; all `link_so` + `verify.py`)

All rows are `_ZN3Gun22calcCharacterCollisionEv`; `osize` is always 1984.

| variant | pct | usize | note |
|---|---:|---:|---|
| **tree body (`F H K L P`, `int dmg`)** | **90.0** | **1984** | current best |
| oracle faithful: no `dmg`, `F P H L K`, `for` | 64.1 | 1920 | true source shape |
| oracle damage shape (per-branch `dmg`), tree tail | 84.1 | 1980 | 4 B short |
| no `dmg`, tree tail | 84.0 | 1996 | 12 B long |
| `F P H L K` + `int dmg` | 64.8 | 1912 | low-pressure map |
| `F P H L K` + `dmg` + `do-while` + `expect(distance,0)` | 86.4 | 1984 | wf213 shape, high-pressure map |
| `F P H L K` + no `dmg` + `do-while` + `expect(distance,0)` | 78.9 | 2000 | |
| `F P H L K` + per-branch `dmg` + `do-while` + `expect(distance,0)` | 78.7 | 1980 | |
| tree + `do-while` (no expect) | 64.4 | 1916 | |
| tree + `expect(distance,0)` | 69.3 | 1944 | |
| tree + inline `isRocketLike` expansion | 41.1 | 1892 | |
| tree + oracle `bomb`/`explosive` if/else | 50.3 | 1884 | |
| tree + `expect(impact != null, 1)` | 75.9 | 1964 | |
| tree + `expect(scatter, 0)` | 55.0 | 1940 | |
| tree + `expect(geometries != null, 1)` | 60.3 | 1908 | |
| tree + `expect(isAsteroid(), 0)` | 59.0 | 1964 | |
| tree + mine anchor value `0` | 69.2 | 2000 | anchor value `1` is required |
| tree + `explodeOnContact` before `explosive` | 69.6 | 1952 | |
| tree + `td` declared before `explosive` | 67.1 | 1968 | td stays in a register |
| tree + `td` at function top | 66.2 | 1964 | wf112/wf213 agree |
| tree + `bool` `explosive`/`explodeOnContact` | 90.0 | 1984 | codegen-identical |
| tree + `char` `explosive`/`explodeOnContact` | 90.0 | 1984 | codegen-identical |
| tree + `int explosive = 0;` | 90.0 | 1984 | initializer folded |
| tree + `Vector *const td` | 90.0 | 1984 | codegen-identical |

The tree body's residual is the same one documented in wf112/w4912/wf213: the shipped
allocation is the high-pressure `this=r9`, `enemy=r8`, `i=fp`, `i*3=r4`, `td=r6`,
`explosive=[sp,#20]`, `td=[sp,#24]` map, while the tree body has `explosive=[sp,#24]`,
`td=[sp,#20]` (the two spill slots swapped) and `fp=1` instead of `fp=i`. The tail order
(`F H K L P` vs `F P H L K`) and the `dmg` cache account for the rest. The faithful
`F P H L K` order compiles to the low-pressure map (`this=r4`, `enemy=r6`, `i=r8`) and loses
~12 instructions; the tree order keeps the high-pressure map but emits the wrong tail order.

## 3. Conclusion

The wave-103 oracle confirms the shipped source shape (no `dmg`, `F P H L K`, `for` scatter
loop) but does not give a better byte match under clang 7.0.2 `-Oz`: the faithful shape scores
64.1 % / 1920 B, and the closest faithful-damage shape scores 84.1 % / 1980 B. The tree body
(`F H K L P` + `dmg`) remains the best measured shape at **90.0 % / 1984 B**. The residual is
the known PIC greedy-region-split lottery (`$r11` total 110.41 > 105.03 isolates the scatter
block and reproduces the shipped allocation; the shipped `F P H L K` order gives 98.31 <
105.03 and folds it), plus the resulting `explosive`/`td` spill-slot swap. No source shape
tried in this pass (or in passes 1–15) beats 90.0 % at 1984 B.

## 4. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984 byte=False linked=False
```

## 5. Files changed

`_work/reconstructions/gun-calccharactercollision-wf313.md` only (note).
Gate: byte 4500 / linked 4500, size-exact 4524/4524, data 843/843, imports 207,
`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`,
lint CLEAN, drift 0.
