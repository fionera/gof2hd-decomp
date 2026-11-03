# `Gun::calcCharacterCollision` pass 3: targetDir spill vs. the volatile-enemy anchor (w3909)

Worker: w3909. Branch: `wave89-w3909`, based on master `36c65db7`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base `0x10000`). Scratch:
`/dev/shm/w3909/`. Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump
`0x152f90`, size `1984 B`.

## 0. Result

| build | gate byte/linked | target pct | target size |
|---|---:|---:|---:|
| before (HEAD `36c65db7`) | **4498 / 4498** | **82.7%** | 1984 B |
| after (this pass) | **4498 / 4498** | **82.7%** | 1984 B |

No code change. The tree is left exactly as HEAD. This pass proves from the
Ghidra decompilation that the current source shape is correct (a local
`Vector *` to `targetDir`, enemy reused by the scatter loop, no `itemIndex`
local, no volatile enemy slot) and records why the two wave-88 anchors still
win: every faithful variant that removes the volatile enemy slot loses
`this = r9` and drops to 56.9-64.1%.

## 1. Reproducibility gotcha found this pass (important)

A fresh `cmake --preset default` in this worktree downloaded an NDK r18b zip
whose `libc++abi.a` was built with `/buildbot/...` source paths instead of the
original's `/Volumes/Android/buildbot/...`. That single 16-byte `.rodata`
string is enough to shift `.rodata` by 16 bytes and drop the whole-so gate from
`byte_exact 4498` to `4344` (linked stays 4498). The main repo's cached NDK has
the original paths:

```
$ strings .cache/ndk/.../libs/armeabi-v7a/libc++abi.a | grep abort_message.cpp
/buildbot/src/android/ndk-release-r18/...            # freshly downloaded
/Volumes/Android/buildbot/src/android/ndk-release-r18/...  # main repo / baseline
```

Copying `gof2-decomp/.cache/ndk/android-ndk-r18b` into this worktree's
gitignored `.cache` restores `byte_exact 4498`. Both NDKs have the same
`clang` binary (md5 `6d6638a686883135594d8912f45229ca`), so this is only the
prebuilt libc++abi archive, not a compiler difference.

## 2. Ghidra ground truth (image base 0x10000, function @ `0x162f90`)

`DecompAt2.java` on a copy of `_work/bins/..._ghidra.rep` (the live project
uses base 0x10000, so the function is `0x162f90`, not `0x152f90`). The
decompiler emits the exact source-level shape:

```c
bVar6 = iVar18 == 0xb;                       /* mine */
this_00 = (Vector *)(this + 0xd8);           /* &this->targetDir, local pointer */
*(undefined4 *)this = 0;                     /* lastHitKIPlayer = 0 */
for (uVar19 = 0; uVar19 < *puVar9; uVar19 = uVar19 + 1) {
  this_01 = *(Player **)(puVar9[1] + uVar19 * 4);   /* enemy */
  ...
  for (uVar23 = 0; uVar23 < *(uint *)(this + 8); uVar23 = uVar23 + 1) {
    ...
    if (iVar18 == 0x19) {                    /* scatter */
      ...
      for (uVar21 = 0; uVar21 < **(uint **)(this + 0xb4); uVar21 = uVar21 + 1) {
        this_01 = *(Player **)((*(uint **)(this + 0xb4))[1] + uVar21 * 4);
        Player::getPosition();
        AbyssEngine::AEMath::Vector::operator=(this_00, (Vector *)aAStack_d0);
        AbyssEngine::AEMath::Vector::operator-=(this_00, (Vector *)&local_78);
        fVar24 = (float)AbyssEngine::AEMath::VectorLength(this_00);
        ...
      }
    }
  }
}
```

Three facts that matter:

1. **`this_00` is a real local `Vector *` to `this->targetDir`.** The scatter
   loop calls `Vector::operator=`, `operator-=` and `VectorLength` with
   `this_00` as the receiver, not with `this` (`r9`). The shipped code reloads
   it from `[sp,#24]` at `15319c`, `153262`, `1533f6`.
2. **`this_01` (enemy) is reused as the scatter-loop `other`.** The original
   does not keep a separate `other`; it overwrites `r8` in the scatter loop.
   Our source already does this (`enemy = (*this->enemies)[k]`).
3. **There is no `itemIndex` local.** Both normal damage paths load
   `*(int *)(this + 0x58)` from memory (`1532e0`, `153304`), and the egoFlag
   path also reloads `*(int *)(this + 0x60)` (`1532d4` `vldr s0,[r9,#96]`).
   The current `const int itemIndex = this->itemIndex;` is a size anchor.

The shipped prologue and the targetDir store:

```
152f9c  sub  sp, #208
152f9e  mov  r9, r0                 ; this = r9
...
152ffa  movs r2, #0
153000  mov  r6, r9
153006  str.w r2, [r6], #216        ; lastHitKIPlayer = 0; r6 = this+0xd8
153010  str  r6, [sp, #24]          ; spill &targetDir
```

The shipped scatter-loop receiver:

```
15343e  movs r5, #0
153440  b.n  1534fc
153442  ldr  r0, [r0, #4]
153446  ldr.w r8, [r0, r5, lsl #2]  ; enemy/other = enemies[k]
15344a  mov  r0, r4
15344c  mov  r1, r8
15344e  blx  Player::getPosition()
153452  mov  r0, r6                 ; r6 = &targetDir (loaded from [sp,#24])
153454  mov  r1, r4
153456  blx  Vector::operator=
15345a  mov  r0, r6
15345c  mov  r1, sl                 ; sl = &position
15345e  blx  Vector::operator-=
153462  mov  r0, r6
153464  blx  VectorLength
```

## 3. Register allocation (original vs HEAD)

| value | original | HEAD (with anchors) |
|---|---|---|
| `this` | `r9` | `r9` |
| `enemy` outer | `r8` | `r6` |
| `enemy` inner | `r8` (same value) | `r8` via `[sp,#208]` reload |
| `mine` | `r5` | `r5` |
| `i` | `fp` (also spilled `[sp,#40]`) | `r1` (spilled `[sp,#40]`) |
| `i*3` | `r4` | `r4` |
| `&enemy->radius` | `sl` | `r6` |
| `&this->targetDir` | spilled `[sp,#24]` | `sl` |
| `&position` (`sp+192`) | cached in `sl` | rematerialised `add rX,sp,#192` |
| frame | 208 B | 216 B |

The volatile `enemySlot` anchor is what makes `this = r9`, `mine = r5`,
`i*3 = r4` (the shipped assignment). Removing it moves `this` to `r4` in every
faithful variant tried. With the anchor, the allocator gives `r8` to `td` (or
`sl`), and the outer enemy lands in `r6`.

## 4. Experiments (official verifier unless marked "fast")

The fast scorer is `difflib.SequenceMatcher(autojunk=False)` over
objdump-normalised instructions; it tracks the official pct within ~1 point
(base: fast 82.8, official 82.7).

| variant | official pct | size | notes |
|---|---:|---:|---|
| **HEAD (volatile enemy slot + hitFlag + itemIndex local)** | **82.7** | 1984 | baseline |
| `Vector *td = &targetDir` after the store, keep slot+itemIndex | 74.8 | 1980 | td stays in `sl` |
| `Vector *td`, drop itemIndex local, keep slot | 74.6 | 1972 | td in `r8`, enemy `r6` |
| `Vector *td`, drop itemIndex, drop slot | 56.9 | 1896 | `this=r4` |
| `Vector &td`, drop itemIndex, keep slot | 74.6 | 1972 | identical codegen |
| `Vector *td[1] = {&targetDir}` (forces stack) | 74.6 | 1972 | array optimised away |
| `Vector *td`, `Vector **tdp = &td` | 74.5 | 1980 | td still `sl` |
| `Vector *volatile td` | 55.8 | 1900 | too much volatile |
| no slot, `enemy` declared outside outer loop, td, no itemIndex | 64.1 | 1924 | `this=r4` |
| no slot, td, itemIndex, enemy outside | 62.8 | 1920 | `this=r4`, enemy `r8`, td `sl` |
| no slot, td, itemIndex, enemy outside, function-scope `radius` | 62.8 | 1920 | same |
| no slot, td, itemIndex, enemy outside, `while` inner loop | 62.7 | 1924 | fast |
| no slot, td, itemIndex, enemy outside, guarded `do/while` inner loop | 56.6 | 1924 | fast |
| HEAD + guarded `do/while` inner loop | 65.7 | — | fast |
| HEAD + `while` inner loop | 62.7 | — | fast |
| HEAD + `volatile` final-else damage only | 82.6 | 1984 | **fixes** the damage block (`vldr s0,[r9,#96]` + `ldr r1,[r9,#96]` reload) but shifts block layout |
| HEAD + `volatile` final-else damage + drop hitFlag | 78.9 | 1980 | fast |
| HEAD + `volatile` final-else damage + td local | 74.4 | — | fast |
| HEAD + `enemySlot` reads in outer checks | 79.6 | — | fast |
| `volatile const int itemIndex` | 63.0 | — | fast |
| `Array<Player*> *enemies` local | 52.4 | — | fast |
| `Player **enemyData = enemies->data()` + inner reload | 57.7 | — | fast |
| inner `enemy = (*enemies)[e]` reload (no slot) | 68.0 | 1988 | `this=sl` |
| `enemySlot` declared outside the outer loop | 71.7 | — | fast |
| `tdSlot` volatile instead of enemySlot | 58.7 | 1904 | `this=r4` |

Type/const/alias sweeps (`int` vs `const int` weaponType, `bool` vs `const
bool` mine/scatter, `bool` explosive/explodeOnContact, `int` itemIndex,
`enemies->count` vs `size()`, `data()[e]` vs `(*enemies)[e]`, `size()` for
positions, `const` radius/ki/outer/r) all produced **identical codegen**
(fast 82.8, official 82.7). The compiler is insensitive to these.

## 5. Why the base still wins

The shipped allocation needs seven callee-saved values live in the inner loop:
`this (r9)`, `enemy (r8)`, `mine (r5)`, `i (fp)`, `i*3 (r4)`,
`&radius/&position (sl)`, plus `targetDir` which is spilled. HEAD's volatile
slot reproduces the first four but pins `enemy` to `[sp,#208]`; that frees `r8`
for `td`, so `td` is not spilled and `&position` is rematerialised. Removing
the slot frees `r8` for `enemy` but lets `this` fall to `r4` and `td` take
`sl`.

The one variant that fixes a real block without losing the anchor is the
`volatile` final-else damage read: it produces the shipped
`vldr s0,[r9,#96]` in the egoFlag path and the `ldr r1,[r9,#96]` reload in
the normal path (`1532d0`/`153304`), but the block-layout shift costs 0.1
point overall (82.6). It is not a strict improvement.

To beat 82.7, the remaining work is to force `td` to spill **while keeping
`this=r9`**: i.e. a source shape that keeps `r8` occupied by `enemy` across
the inner loop without a stack slot. Every natural pointer/local/loop form
tried so far either moves `this` to `r4` or keeps `td` in a register.

## 6. Files changed

Note only. `src/game/weapons/Gun.cpp` is byte-identical to HEAD.
