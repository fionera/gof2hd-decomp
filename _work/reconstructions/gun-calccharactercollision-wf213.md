# `Gun::calcCharacterCollision` pass 15: a `do { … } while (++k < size)` scatter loop
# reaches the shipped `F P H L K` + high-pressure map at 1984 B, but HEAD's tuned
# `F H K L P` body still scores higher (90.0 vs 86.4) (wf213, wave 102)

Worker: wf213. Branch: `wave102-wf213`, base master `53d9ee4d5`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 instructions.
Scratch: `/dev/shm/wf213/`.

## 0. Result

**No code change landed.** The tree body (`F H K L P` tail, `int dmg`) is still the best
measured shape: **90.0 %**, 1984/1984 B, 585/650 normalized instructions (official verifier).

| build | gate byte/linked | target pct (official) | target size |
|---|---:|---:|---:|
| before (HEAD `53d9ee4d5`) | **4500 / 4500** | **90.0 %** | **1984 B** |
| after this pass (note only) | **4500 / 4500** | **90.0 %** | **1984 B** |

Full gate after: size-exact 4524/4524, data 843/843, imports 207, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, lint `CLEAN ✓`, drift 0, `RATCHET PASS`.

This pass contributes one new source shape and a complete trigger table:

1. **A `do { … } while (++k < this->enemies->size())` scatter loop** on top of the faithful
   `F P H L K` + `dmg` + `__builtin_expect(distance < this->magnitude, 0)` body reaches the
   shipped high-pressure map (`this=r9`, `enemy=r8`, `fp=i`, `i*3=r4`) **and** the shipped
   size **1984 B**, at **86.4 %** — better than every previously reported `F P H L K` body
   (w5013's best was `expect(mine,0)` at 85.5 / 1984) but still below HEAD's 90.0.
2. The `do-while` rotates the scatter loop exactly like the shipped binary
   (`movs r5,#0; b.n <cond>; <body>; adds r5,#1; <cond>: cmp; bcc <body>`), which the
   `for`-loop `expect(distance,0)` body does not (it is top-tested).
3. `expect(mine,0)` + `expect(distance,0)` together on the `F P H L K` body give the
   **exact** shipped size/instruction count/map (`1984 B / 650 insns / this=r9 / enemy=r8 /
   fp=i`) but only **82.3 %** — the `i*3` value lands in `r6` instead of `r4`, so the map is
   not the shipped one after all.

## 1. The shipped facts (re-verified from the binary)

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`, `H` = `setHitVector`,
`L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

Shipped instruction order is **`F P H L K`** (`0x153316` … `0x153350`), and the shipped
allocation is the tree's high-pressure `F H K L P` map:

| value | shipped |
|---|---|
| `this` | r9 |
| `enemy` | r8 |
| `i` | fp (spilled to `[sp,#40]` around the radius computation) |
| `i*3` | r4 |
| `td` | r6 (`[sp,#24]`) |
| `explosive` | `[sp,#16]` |
| `explodeOnContact` | `[sp,#20]` |
| `scatter` | `[sp,#32]` |
| `mine` | `[sp,#36]` |

The shipped body has **no `int dmg = this->damage` local**: it reloads `this->damage` at
`0x1532b8` (`vldr s0,[r9,#96]`, the `*3.0f*0.25f` site), `0x1532d4` (`vldr s0,[r9,#96]`, the
`*0.2f` site) and `0x153308` (`ldr.w r1,[r9,#96]`, the plain `damage` site). The two plain
`else` arms are tail-merged into one block at `0x153304`.

## 2. New: the `do-while` scatter loop

The shipped scatter loop is a rotated `for`:

```
15343e: movs r5,#0
153440: b.n  1534fc          ; -> condition
153442: <body>               ; getPosition / *td = / *td -= / VectorLength / if(...)
1534fa: adds r5,#1
1534fc: ldr.w r0,[r9,#180]   ; enemies
153500: ldr   r1,[r0,#0]     ; size
153502: cmp   r5,r1
153504: bcc   153442
```

Every `for` spelling of this loop in our PIC build is either top-tested or, with
`expect(distance,0)`, top-tested with an out-of-line `if` body. Writing the loop as a
`do { … } while (++k < this->enemies->size())` rotates it the same way as the shipped binary
and, combined with `expect(distance,0)`, keeps the high-pressure map:

```c++
unsigned k = 0;
do {
    enemy = (*this->enemies)[k];
    *td = enemy->getPosition();
    *td -= position;
    int distance = (int) AbyssEngine::AEMath::VectorLength(*td);
    if (__builtin_expect(distance < this->magnitude, 0)) {
        ...
    }
} while (++k < this->enemies->size());
```

Measured (official verifier, full `link_so`):

| body | pct | osize | usize | map |
|---|---:|---:|---:|---|
| HEAD (`F H K L P`, `dmg`) | **90.0** | 1984 | 1984 | high, `fp=1` |
| `F P H L K` + `dmg` + `expect(distance,0)` + `do-while` | **86.4** | 1984 | 1984 | high, `fp=i`, `this=r9`, `enemy=r8` |
| `F P H L K` + `dmg` + `expect(mine,0)` (w5013 best) | 85.5 | 1984 | 1984 | high, `enemy=r6` |
| `F P H L K` + `dmg` + `expect(distance,0)` | 84.8 | 1984 | 1980 | high, `enemy=r8` |
| `F P H L K` + `dmg` + `expect(mine,0)` + `expect(distance,0)` | 82.3 | 1984 | 1984 | high, `enemy=r8`, `i*3=r6` |
| `F P H L K` + `dmg` + `expect(distance,0)` + `expect(explosive,1)` | 73.2 | 1984 | 1984 | high, `enemy=r8` |
| `F P H L K` + `dmg` + `expect(explosive,1)` | 78.9 | 1984 | 1988 | high, `enemy=r8` |
| `F P H L K` + `dmg` (faithful order) | 64.8 | 1984 | 1912 | low |
| `F P H L K` no `dmg` (faithful) | 64.1 | 1984 | 1920 | low |

The `do-while` body is the only `F P H L K` shape found that is simultaneously
**size-exact (1984)**, **`F P H L K`**, **high-pressure map with `this=r9`/`enemy=r8`/`fp=i`**,
and **scatter-loop-rotated**. It still loses to HEAD because its `if` body is placed
out of line (the `expect(distance,0)` flips the `distance < magnitude` branch weight), which
the shipped binary does not do (shipped `if` body is the fall-through after `ble`).

## 3. Trigger table (all on the `F P H L K` + `dmg` body)

Single `__builtin_expect` site, value 0/1, screened with the object-level `inspect.py`
(size / tail order / `this` / `enemy`), then confirmed with the official verifier where noted:

| site | val | size | map |
|---|---:|---:|---|
| `mine` (radius `*= 5`) | 0 | **1984** | high, `this=r9`, `enemy=r6`, `td=r8` (clean swap) |
| `distance < magnitude` | 0 | 1980 | high, `this=r9`, `enemy=r8`, scatter top-tested |
| `explosive` (emitManual) | 1 | 1988 | high, `this=r9`, `enemy=r8`, scatter rotated |
| `!isActive()` | 0 | 1984 | high, `this=r9`, `enemy=r8` |
| `getHitpoints() < 1` | 0 | 1984 | high, `this=r9`, `enemy=r8` |
| `scatter` (radius block) | 1 | 1976 | high, `enemy=r6` |
| `owner == null \|\| enemyFlagsLo` | 0 | 1980 | high, `enemy=r6` |
| everything else | — | ≤ 1992 | low or wrong map |

`expect(mine,0)` + `expect(distance,0)` is the only pair that gives 1984/650/`enemy=r8`, but
its `i*3` is `r6` (shipped `r4`), so it scores 82.3.

## 4. Conclusion

The shipped body is `F P H L K` with no `dmg` local and the tree's high-pressure allocation.
The region split that produces that allocation is reachable from source (`expect(distance,0)`,
`expect(mine,0)`, and the new `do-while`), but every reachable spelling either changes the
scatter-loop layout (out-of-line `if` body, top-tested loop) or the `enemy`/`td`/`i*3`
registers. HEAD's tuned `F H K L P` body remains the best fuzzy match at **90.0 % / 1984 B**
because it keeps `enemy=r8`, `i*3=r4`, `td=r6` and the rotated scatter loop; only the tail
order (`F H K L P` vs `F P H L K`) and the `dmg` cache differ.

## 5. Reproduction

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

Scratch harness (`/dev/shm/wf213/`):

| file | purpose |
|---|---|
| `body_head.txt` | HEAD body (`F H K L P`, `dmg`) |
| `body_fphlk_nodmg.txt` | faithful `F P H L K`, no `dmg` |
| `body_fphlk_dmg_dist0.txt` | `F P H L K` + `dmg` + `expect(distance,0)` (84.8 / 1980) |
| `body_dowhile.txt` | `F P H L K` + `dmg` + `expect(distance,0)` + do-while (86.4 / 1984) |
| `body_mine0.txt` | `F P H L K` + `dmg` + `expect(mine,0)` (85.5 / 1984) |
| `body_m0_dist0.txt` | `expect(mine,0)` + `expect(distance,0)` (82.3 / 1984) |
| `body_d0_expl1.txt` | `expect(distance,0)` + `expect(explosive,1)` (73.2 / 1984) |
| `trybody.py` / `inspect.py` | 0.2 s object-level size/tail/map screener |
| `link_score.sh <body>` | splice, `link_so`, official verifier row, `git checkout` |
| `build_body.sh <body> <so>` | splice, `link_so`, copy the `.so`, `git checkout` |
| `normdiff.py <so>` | official-normalized instruction diff against the shipped `.so` |
| `batch*.py`, `search_tail.py` | the knob sweeps in §3 |

## 6. Files changed

None under `src/` (note only). Gate: byte 4500 / linked 4500, size-exact 4524/4524,
data 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
lint CLEAN, drift 0.
