# `Gun::calcCharacterCollision` pass 7: single `damage` call site, a real `dmg` local, and the five-statement order (w4229)

Worker: w4229. Branch: `wave92-w4229`, base master `b379d00d`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base `0x10000`). Scratch:
`/dev/shm/w4229/`. Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump
`0x152f90`, size `1984 B`.

## 0. Result

| build | gate byte/linked | target pct (official) | target size |
|---|---:|---:|---:|
| before (HEAD `b379d00d`) | **4498 / 4498** | **82.7%** | 1984 B |
| after (this pass) | **4498 / 4498** | **90.0%** | 1984 B |

Full gate after: size-exact 4524/4524, data 843/843, imports 207,
`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26290`
(-8), lint `CLEAN ✓`, drift 0, `RATCHET PASS`. The target is still not
byte-exact (`byte: false`), so the hard counts are unchanged; the improvement is
the per-function parity 82.7 -> 90.0 at the *same* 1984 B size.

## 1. The size constraint is the whole game

`byte_exact` compares raw bytes, so a size change in this function shifts every
later function in `.text` and collapses the hard count. Measured directly: the
84.4% / 1992 B variant (see below) gives

```
verify: byte 3800  linked 4498   (byte_exact 4498 -> 3800)
RATCHET FAIL
```

so a candidate is only landable if it is **exactly 1984 B**. This is why the
wave-90/91 candidates (84-85% at 1976/1980/1992/1996 B) were all left as notes.
The winning change had to keep the size at 1984 *and* raise parity.

## 2. The original has one `Player::damage` call site, not four

The tree's source has four `enemy->damage(...)` statements (two "plain"
`this->damage` calls, two computed calls). The shipped binary has **one**
`Player::damage` call:

```
1532e0  ldr.w r3, [r9, #88]      ; computed path: friendGun
1532e4  ldrb.w r2, [r9, #249]    ; itemIndex
1532e8  vcvt.s32.f32 s0, s0
1532ec  vmov  r1, s0             ; r1 = computed damage
1532f0  b     153310
...
153304  ldr.w r3, [r9, #88]      ; plain path: friendGun
153308  ldr.w r1, [r9, #96]      ; r1 = this->damage
15330c  ldrb.w r2, [r9, #249]    ; itemIndex
153310  mov   r0, r8             ; enemy
153312  blx   Player::damage
```

Both predecessors materialise `r1` and fall/branch into the single call block.
That is the signature of a **single call with a phi/local**, not four call sites
that happened to be tail-merged. Reconstructing it with a real local

```cpp
int dmg = this->damage;
...
    enemy->damage(dmg, this->friendGun, this->itemIndex);   // plain paths
...
    enemy->damage((int) ((float) this->damage * 3.0f * 0.25f), ...);  // computed
```

placed immediately before the `if (this->owner == nullptr || ...)` block is the
first shape that reproduces the single call site *and* keeps the function at
1984 B. (Moving the declaration anywhere else — function top, after the null
check, outside the outer loop, after `td` — drops the match to 62-63%.)

## 3. The five-statement order is the RA lever

The five statements after `Player::damage` are semantically independent enough
that their source order changes the greedy allocator's result. Faithful order
(`f54, playerGun, hitvec, lifetimes, lastKIP`) is *not* the order that
reproduces the shipped allocation. The winning order is

```
f54 -> hitvec -> lastKIP -> lifetimes -> playerGun
```

i.e. `reinterpret_cast<uint8_t*>(&enemy->field_54)[0] = 1;`,
`enemy->setHitVector(...)`, `this->lastHitKIPlayer = ...`,
`this->lifetimes[i] = -1000000;`, then `if (this->playerGun) ...`.

With that order the clang 7 greedy allocator produces the shipped signature:

| property | shipped | HEAD (4 calls + anchors) | this pass |
|---|---|---|---|
| `this` | r9 | r9 | **r9** |
| `i*3` | r4 | r4 | **r4** |
| `enemy` | r8 | r6 | **r8** |
| frame | 208 | 216 | **208** |
| `td` | spilled | (no `td` local) | **spilled** |
| size | 1984 | 1984 | **1984** |

This also removes the three codegen anchors HEAD needed (`const int itemIndex`,
`Player *volatile enemySlot`, the `volatile uint8_t hitFlag` read). The body is
now anchor-free except for the pre-existing `__builtin_expect` in the mine
branch (removing that costs 89.8 -> 84.4, so it stays).

## 4. Variant table (object-level oracle, `autojunk=False`; official in parentheses)

`4call` = four `enemy->damage` statements; `local` = `int dmg = this->damage;`
before the damage block; `phi` = `int dmg;` assigned in every branch.

| variant | pct | size | frame | this | postinc | sp24 | i3 | enemy |
|---|---:|---:|---:|---|---|---|---|---|
| HEAD (4call + 3 anchors) | 82.4 (82.7) | 1984 | 216 | r9 | - | r2 | r4 | r6 |
| faithful (4call, no anchors) | 64.3 | 1920 | 208 | r4 | r9 | r2 | r5 | r6 |
| 4call + `f54 hitvec playerGun lifetimes lastKIP` | 85.1 | 1992 | 208 | r9 | r6 | r2 | r4 | r8 |
| 4call + `f54 hitvec playerGun lastKIP lifetimes` | 84.8 | 1992 | 208 | r9 | r6 | r2 | r4 | r8 |
| phi + `playerGun hitvec f54 lifetimes lastKIP` | 85.5 | 1976 | 208 | r9 | r6 | r2 | r4 | r8 |
| local + `f54 hitvec playerGun lifetimes lastKIP` | 85.0 | 1976 | 208 | r9 | r6 | r2 | r4 | r8 |
| local + `f54 hitvec lastKIP playerGun lifetimes` | 84.5 | 1976 | 208 | r9 | r6 | r2 | r4 | r8 |
| **local + `f54 hitvec lastKIP lifetimes playerGun`** | **89.8 (90.0)** | **1984** | **208** | **r9** | **r6** | **r2** | **r4** | **r8** |
| local + `f54 lastKIP hitvec lifetimes playerGun` | 89.5 | 1984 | 208 | r9 | r6 | r2 | r4 | r8 |
| local + `lastKIP f54 hitvec lifetimes playerGun` | 89.5 | 1984 | 208 | r9 | r6 | r2 | r4 | r8 |
| local + `f54 hitvec lifetimes lastKIP playerGun` | 75.6 | 1964 | 208 | r9 | r8 | r2 | r4 | r6 |

Only three of the 120 `local` permutations land on 1984 B, and all three put
`playerGun` last; the best is 89.8/1984. `local` with the faithful order is
64.5/1912 (this=r4), so the order, not the local, is what moves the allocation.

Other levers tried on top of the winner (all 89.8/1984 or worse):
`bool`/`const` on `explosive`/`explodeOnContact`/`dmg`/`position`/`velocity`/
`enemyPosition`/`outer`; `td` as `const`, parenthesised, or `Vector&`; moving
`td` before the `bomb` block or before `explosive`; `if (x != 0)` forms;
`playerGun`/`level` locals; removing the nested `impact != nullptr`; removing
`__builtin_expect` (84.4); adding back `itemIndex` (59.5), `hitFlag` (84.8), or
`volatile enemySlot` (74.3); `vorder` permutations of the four inner-loop
locals (best is the current `position, velocity, enemyPosition, radius`).

## 5. What still differs (linked disassembly, ratio 94.65)

The remaining ~10% is three cosmetic/allocator residuals, none of which change
the size:

1. **Stack-slot colouring swap.** Shipped: `[sp,#20]=explosive`,
   `[sp,#24]=td`, `[sp,#28]=e`. Ours: `[sp,#20]=td`, `[sp,#24]=explosive`,
   `[sp,#28]=e`. Both values are spilled; only the frame-index order differs
   (td is spilled before explosive in ours). Six instructions differ by
   `#20`/`#24`. Moving the `td`/`explosive` declarations and assignments in the
   source did not flip it.
2. **`dmg` held in `sl`.** Ours loads `this->damage` once into `sl` and uses
   `mov r1, sl` in the plain block; the shipped plain block reloads
   `ldr r1, [r9,#96]`. The `dmg` local is what buys the single call site and the
   1984 B size, so this is the price of the shape.
3. **Statement scheduling.** Ours emits the five statements in source order
   (`... lifetimes, playerGun`); the shipped binary has the `playerGun` store
   before `setHitVector`. The allocator outcome (this=r9, enemy=r8, i3=r4) is
   tied to `playerGun` being last in the source, so this residual is the
   remaining tension between "source order" and "RA outcome".

Also noted but not chased: `movs r5,#1` vs `movs r0,#1` for the impact flag and
the `ldr r2,[pc,...]` pool offsets (both pure scheduling/pool placement).

## 6. Conclusion

The pass proves that the 82.7% plateau was **not** an allocator dead end: the
function is a single `damage` call with a real `dmg` local, and the five
post-damage statements must be ordered `f54, hitvec, lastKIP, lifetimes,
playerGun` for clang 7 to reproduce `this=r9 / enemy=r8 / i*3=r4 / frame 208`
and spill `td`. That shape is 1984 B, so it lands with the hard gate intact
(byte 4498, linked 4498) and lifts the target from 82.7% to **90.0%**.

Remaining work: the `explosive`/`td` stack-slot swap, the `dmg`-in-`sl` reload,
and the `playerGun` scheduling order. None of them is reachable by a pure
source reorder that also preserves the 1984 B size in the variants tried.
