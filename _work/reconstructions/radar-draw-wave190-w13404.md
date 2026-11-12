# Radar::draw wave 190 (w13404) — spark `inSight` box: `int top` + `left + size*2` /
# `top + size*2` bounds; aligned 1,841 -> 1,816 (size-exact, `.rel.plt` identical)

**Status:** landed, 2026-11-12. Worktree `wt/w13404`, branch `wave190-w13404`, base
`master` `dcb6b809c`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13398.md` (24000-box upper-bound-first, landed),
`radar-draw-wave190-w13381.md` (negative-result record: 134 adjacent swaps and the
five-operand permutations exhausted) and `radar-draw-wave190-w13370.md`.

## 0. The change

The gas-cloud spark `inSight` box test (source ~line 965) kept `top` as a `float` and
tested both upper bounds as `left > screenX - size*2` / `(int) top > screenY - size*2`.
The original (`12f4a8..12f4f8`) instead materialises **both** `left` and `top` as ints
up front (`vldr s2/s4` + two `vsub` + two `vcvt.s32`) and tests the upper bounds as
`screenX < left + size*2` (`add.w r2,r2,r0,lsl #1`) and `screenY < top + size*2`
(`add r0,r1`). Source edit:

```cpp
-                        float top = PlayerEgo::crosshairPos.y - (float) size;
-                        inSight = this->screenX > left && left > this->screenX - size * 2 && this->screenY > (int) top &&
-                                  (int) top > this->screenY - size * 2;
+                        int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
+                        inSight = this->screenX > left && this->screenX < left + size * 2 && this->screenY > top &&
+                                  this->screenY < top + size * 2;
```

Semantically identical (integer truncation happens at the same point; the two
`a > b - c` -> `a + c > b` rewrites are exact for these ranges). With `int top` clang
emits the original's combined x/y prologue and the original's `r2 = crosshairPos`,
`r1 = screenX` register assignment; the spark block's instruction sequence now matches
`12f4a8..12f4f8` instruction-for-instruction except that the block still starts 12 B
later (see §1.1).

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,291 | 6,307 |
| instruction-index alignment | **1,841** | **1,816** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 72,999 | 73,015 |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.ARM.exidx` | byte-identical | byte-identical |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370. The function is already
non-byte-exact, so `byte_exact` is unchanged.

The +16 raw is a pure shift artefact, not a content regression: the spark block sits
12 B later in both base and this build (an extra literal-pool island at `12f488` in
ours; the original's pool is at `12f794`), so same-address bytes cannot line up
regardless of content. Raw diff inside `12f4a8..12f510` is 82 B in base and 98 B here,
while the aligned metric — which is shift-invariant — improves by 25 instructions.
Every other region of the function is byte-for-byte unchanged by this edit
(`12e050..12f4a8` and `12f510..130700` diff counts identical to base).

## 1. Where the remaining structure is lost (measured this session)

Per-0x100-region raw byte diff of the whole function (base): prologue/landmark 80,
planet 889, mid 2,810, spark 391, gas 432, enemy/asteroid 749, music 940.

The `shiftmap.py` (difflib opcodes on normalised instruction streams) delta profile
(orig addr - ours addr) is: 0 up to `12e3d0`, then 2, 4, 46, 50, 54, 52, 24, 110, 28,
32, 36, 38, 26, 28, 52, 36, 28, 48, 58, 66, 68, 8, 56, 62, 60, 58, 44, 46, 6, 0, 2, 0,
-18, -12, -50, -34, 0, -38, 2, 0, -2, 50, 48, -122, 34, 4, 0, 4, 0, -12, -14, -8, -6,
-4, 0, -2, 0, **+126/128 in `12ff22..130086`**, 0 in `1300xx`, then -8, 0, -8, 68, 104,
114, 168, 156, 154, 156 in the music tail. So ours is ~+126 B in the enemy/asteroid
cold-block region and ~-150 B in the music tail — roughly compensating.

### 1.1 The root cause of the early shift: `campaign` is spilled, the shared zero is in `sl`

The prologue is byte-identical to the original up to `12e0e0`. The first divergence is
`12e0f8`:

```
orig:  12e0d8  mov.w fp,#0        (shared zero)     12e0f8  mov sl,r0        (campaign)
ours:  12e0d8  mov.w sl,#0        (shared zero)     12e0f8  str r0,[sp,#144] (campaign)
```

`int campaign = Globals::status->getCurrentCampaignMission();` is used exactly once
(`campaign >= 2` at the planet-block guard), so its live range `12e0f8..12e38a` is long.
The original keeps it in `sl`; ours spills it to `[sp,#0x90]` and puts the long-lived
zero constant in `sl` instead. Consequences cascade: `stations` goes to `[sp,#0x84]`
instead of `sl` (`str.w sl,[sp,#0x74]` vs `str r0,[sp,#0x84]`, -4 B), the planet-loop
index `i` lands in `sl` instead of `fp`, `planetHit` is kept in `fp` instead of
`[sp,#0x98]`, and `12e38a` becomes `ldr r0,[sp,#0x90]; cmp r0,#2` instead of
`cmp.w sl,#2`. This single register choice accounts for most of the 46-68 B delta in
`12e4xx..12ebxx`.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,291** | 1,841 | 9,908 |
| `bool otherLock` (gets `sl`=campaign, `fp`=zero) | 9,228 | 2,714 | 9,860 |
| `bool otherLock` + `int otherLockFlag` | 9,179 | 2,569 | 9,888 |
| `bool otherLock` + `bool notOtherLock` | 9,055 | 2,580 | 9,880 |
| `bool otherLock` + `int landmarkHit` | 9,225 | 2,770 | 9,860 |
| `int otherLock; if(...) ... else ...` (no temp) | 9,415 | 2,016 | 9,900 |
| `int otherLock = (...)? ... : 1;` | 9,415 | 2,016 | 9,900 |
| `bool otherLock` ternary | 9,179 | 2,569 | 9,888 |

`bool otherLock` reproduces the original's register skeleton exactly but grows the
frame from 0x128 to 0x130 (Canvas address slot moves `0x8c` -> `0x94`, all String
temps shift +8) and comes out 48 B short. Every `bool`/`int`/ternary spelling of the
`otherLock` initialiser was measured; all land in the 9,0xx-9,4xx raw range.

### 1.2 Enemy/asteroid cold-block placement (`12ff22..130086`, ours +126 B)

The original places the asteroid block's `if (!candidateWreck)` body at `12ff4c`,
i.e. **between** the enemy block's `current` selection (`12ff22..12ff4a`) and its
blip-sprite body (`12ffca..13007e`); ours places it after the blip body at `130006`.
The two `current` arms themselves are identical (`cmp.w r8,#0 / beq / ldr / b`). This
is a MachineBlockPlacement tie-break, not a CFG difference.

### 1.3 Music tail (`1302xx..1306xx`, ours -150 B)

Original first-branch order is `stop` -> `inAlienOrbit` -> `isAttackedByAliens` ->
`campaign == 16` (source order CAB); second branch is `inAlienOrbit` ->
`isAttackedByAliens`. Also `if (field_0x1bc >= 1)`'s `else` arm is placed inline at
`1302f0` in the original but at `1304e2` in ours.

## 2. Negative results measured this pass (all reverted)

Full incremental `link_so -j2` + `tools/fn_diff.py` (`ulimit -v 12000000`, `-j2`;
scratch `/home/debian/GalaxyOnFire2/scratch/w13404/`).

### 2.1 Spark `inSight` box spellings

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `int top` + both `+ size*2` bounds (**landed**) | 6,307 | **1,816** | 9,908 |
| `int top` + x-add only | 6,307 | 1,819 | 9,908 |
| `int top` + mixed | 6,307 | 1,818 | 9,908 |
| `int top`, y-first ordering | 6,308 | 1,823 | 9,908 |
| `int top`, y as `screenY - size*2 < top` | 6,307 | 1,818 | 9,908 |
| `int top`, y as `screenY < size + size + top` | 6,307 | 1,816 | 9,908 |
| `float top`, both `+ size*2` bounds | 6,295 | 1,837 | 9,908 |
| `float top`, x-add only | 6,292 | 1,837 | 9,908 |
| `float top`, `size + size` instead of `size*2` | 6,295 | 1,837 | 9,908 |
| `float top`, `top + size*2 > screenY` | 8,264 | 1,954 | 9,904 |
| `int top`, `top + size*2 > screenY` | 8,264 | 1,935 | 9,904 |
| `int top`, `top + size + size > screenY` | 8,264 | 1,935 | 9,904 |
| `int top`, `screenY - size - size < top` | 9,761 | 3,517 | 10,160 |

The `float top` forms leave the original's combined `vldr s2/s4` prologue unemitted
(clang sinks the y computation past the first two comparisons); only `int top` produces
it. The `top + size*2 > screenY` spelling that would give the original's final `add`
instead of the `subs` re-associates the whole block and is size-fatal.

### 2.2 `campaign` register rotation

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `unsigned int campaign` | 6,292 | — | 9,908 |
| `register int campaign` | 6,291 | 1,841 | 9,908 |
| `__builtin_expect(campaign >= 2, 1)` | 8,786 | 2,121 | 9,916 |
| `__builtin_expect(campaign >= 2, 0)` | 9,127 | 3,811 | 9,964 |
| direct call at use site (remove local) | 9,213 | 1,996 | 9,904 |
| `campaign` declared after `otherLock` | 9,182 | 2,028 | 9,916 |
| declaration perm CAB (campaign first) | 8,559 | 1,831 | 9,900 |
| declaration perm ACB | 6,298 | 1,846 | 9,908 |
| declaration perm BAC | 6,311 | 1,849 | 9,908 |
| declaration perm BCA | 6,316 | 1,848 | 9,908 |
| declaration perm CBA | 6,316 | 1,854 | 9,908 |
| planet-guard operand reorders (4 spellings) | 8,616-9,465 | 1,994-3,041 | 9,872-9,924 |

### 2.3 `__builtin_expect` value flips (all 31 occurrences, one at a time)

Only `flip14` (occurrence 15, `landmark` `i == 2 || expect(data[i]==nullptr, 0)`) is
fully neutral (6,291 / 1,841 / 9,908); `flip16` is raw-neutral but aligned 1,936. Every
other flip is raw-worse (6,293 .. 9,568). Notably flipping
`expect(getCurrentCampaignMission() != 120, 1)` -> 0 gives 6,305 / 1,847, and flipping
`expect(field_0x18 != 0, 1)` -> 0 gives 6,387 / 1,822 but size 9,904 (4 B short).

### 2.4 Music-tail condition orders

`c2` (second branch `inAlienOrbit || isAttackedByAliens`) is neutral (6,291 / 1,841);
`c1` (first branch CAB) is 6,306 / 1,836; `c1c2` identical to `c1`. Removing the
`__builtin_expect(..., 0)` on the `campaignMission` guard is 9,274 / 2,056 / 9,972;
flipping it to 1 is 9,294 / 2,281 / 9,960.

### 2.5 Other

* `bool planetHit` -> `int`/`unsigned`/`char`/`volatile`/uninitialised: 8,485-9,176,
  only `char` and uninitialised are size-close (9,916 / 9,904), both raw-worse.
* `bool gasHit` / `int current` / `bool ambient` hoisted to function scope: neutral.
* asteroid `if (!candidateWreck)` inverted to `if (candidateWreck)`: 6,294 / 1,839.
* `if (!turretMode && !enemyHit)` -> `if (!enemyHit && !turretMode)`: neutral.
* `asteroidLockTimer > field_0x1b8 - 200` -> `field_0x1b8 - 200 < asteroidLockTimer`:
  6,293 / 1,841.
* `lockedEnemy->isDying() || isDead()` -> `isDead() || isDying()`: 6,292 / 1,841.
* `dockNavPtr->hasCargo` -> `!= 0`; `hasDockNav` -> `!= 0`; `auto hasScanner` ->
  `int hasScanner`: all neutral.
* `int elapsedInt` before `Sprite *blip`: 6,883 / 1,857 (raw much worse).
* `elapsed`/`totalInt` if-blocks swapped: 6,948 / 1,859 / 9,892 (size short).
* `__builtin_expect(dockNavPtr != current, 1)` -> 0: 8,516 / 2,611 / 9,916.
* `__builtin_expect(field_0x1a9, 1)` -> 0: 8,574 / 2,331 / 9,908.
* `int thr = ...; if (enemyLockTimer > thr)`: neutral.

### 2.6 EH/LSDA

`.ARM.extab` is size-exact at `0x1a3b78` (156,312 B) and its diff stays at 603 B for
every variant above. `Radar::draw`'s LSDA is at `0x1c2038` (exidx entry `0x1a10b8`).
The original has 19 call-site entries, ours 17, and the landing-pad offsets differ
because the code layout differs; there is no independent LSDA-scope fix that does not
first fix the code layout. `String` temp slots are identical (`sp+0x9c..0xf4` in both,
7 x 12 B), so the summary's "String temp 8 B higher" observation was an artefact of the
`bool otherLock` variant, not of the base.
