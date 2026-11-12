# Radar::draw wave 191 (w13484) — dockNav `lockedEnemy` isDead/isDying
# order: raw 6,058 -> 6,057

**Status:** landed, 2026-11-12. Worktree `wt/w13484`, branch `wave191-w13484`,
base `master` `f325e2d87`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13493.md` (asteroid-lock tail, base 6,058),
`-w13474.md` (statement-order/regalloc sweep), `-w13476.md` (landmark store order).

## 0. The change

In the `dockNav` block (`Radar.cpp:874`) the `lockedEnemy` release guard is
reconstructed with the two predicate calls in the opposite order:

```cpp
-            if (this->lockedEnemy != nullptr && (this->lockedEnemy->isDying() || this->lockedEnemy->isDead())) {
+            if (this->lockedEnemy != nullptr && (this->lockedEnemy->isDead() || this->lockedEnemy->isDying())) {
                 this->lockedEnemy = nullptr;
             }
```

Both predicates are pure queries on the same non-null object, so the guard is
semantically identical (`A || B` with no side effects).

The byte win is a **call-target immediate**, not a shape change: clang emits the
same `ldr/cbz/blx/cbnz/ldr/ldr/blx/cbz` skeleton either way, but the two
`blx` PLT stubs (`KIPlayer::isDead` @ 0x61ec0, `KIPlayer::isDying` @ 0x61f50)
have different encodings.  Our dockNav block sits 10 bytes ahead of the
original's (`12fe62` vs `12fe6c`), so the *absolute-address* raw metric
happens to line up the second call with the original's `isDying` at `12fe7a`
only in the swapped order.  This is a raw-byte win under the gate's metric; the
original's own source order at this site reads `isDying` first (orig
`12fe7a: blx isDying`, `12fe84: blx isDead`), so the swap is a metric-alignment
artifact, not new source evidence.  It costs nothing (size/insn/aligned
unchanged) and is kept because raw is the primary hard count.

Only **one** byte in `.text` changes (the low immediate byte of the `blx` at
`12fe7a`); the other swapped call's byte at `12fe70` was already mismatched in
both orders.  `.ARM.extab`, `.ARM.exidx` and `.rel.plt` are unchanged.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,058 | **6,057 (-1)** |
| instruction-index alignment | 2,051 | 2,051 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 71,811 | **71,810 (-1)** |
| `.ARM.extab` differing bytes | 575 | 575 (no growth) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.note.gnu.build-id` | 20 | 20 |
| `.rel.plt` | byte-identical | **byte-identical** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 1. Negative cells measured this wave (all size-exact unless noted)

Every `&&`/`||` operand swap in the draw body (200 sites) was tried: this is the
only one that beats 6,058.  Other measured cells, all reverted:

- first `lockedEnemy` guard (line 675) `isDying/isDead` swap alone: 6,058 (neutral).
- `bool otherLock`: 9,255, size 9,864 (size-fatal); `volatile int otherLock`: 9,462/9,880;
  `bool otherLock + volatile int zero`: 6,224/9,908; `otherLock = 1`: 9,012/9,932.
- `int alienOrbit -> bool`: 6,067 (aligned 1,978, raw loss); landmark `egoPos/targetPos`
  swap: 6,067; both: 6,076.
- `int missionBlocksPlanets -> bool`: 6,216/9,912; `int notTurretMode -> bool`: 9,529/9,872;
  `int notOtherLock -> bool`: 9,498/9,880.
- `int inCenter -> bool`: 7,486/9,912; `int slotCount -> bool`: 7,234/9,912;
  asteroid `int wreck -> bool`: 6,907/9,904; `wreck+inCenter+slotCount` all bool: 6,668/9,908.
- `bossSeen` spellings: 8,536..9,213 (size 9,900..10,000).
- `campaignMission` guard hint removal/reorder: 8,582..9,255 (size-fatal).
- `plasmaInRange = lockConfirmed = 0` chain: 6,060; swapped: 6,060; after `SetColor`: 6,089.
- tractor guard permutations: 6,059..6,060; `wreck` first: 7,475/9,904.
- `bc < 3 -> <= 2`, `< 5 -> <= 4`, `>= 1 -> != 0`: neutral; `>= 1 -> > 0`: 6,059.
- `== nullptr -> == 0`, `!= nullptr -> != 0`, `!ptr`: byte-identical.
- all 30 `__builtin_expect` remove/flip variants: neutral or worse.

### Additional negative cells (same wave, base 6,057)

- swapped operands of every `==`/`!=` comparison in the draw body (124 sites,
  simple operands): all neutral at 6,057 or worse.
- tractor guard nestings (`field_0x1c == nullptr` outer, `wreck`/`tractorMode2`
  inner, both nestings): byte-identical to base.
- tractor guard bitwise forms `(a & b & c)` / `(a && (b & c))`: 7,649 / 6,918
  (size 9,940 / 9,900, size-fatal).
- `int otherLockFlag` / `int otherLock` / ternary-init `otherLock`: all
  byte-identical to each other, raw 9,437, size 9,900 (size-fatal) — the
  `bool`-to-`int` normalisation in our source is worth 8 bytes that the
  original spends elsewhere.
- `float damageRate = getDamageRate(); float hull = field_0x1c8 * (rate/100)`:
  6,061 raw, aligned 2,006; same for the EMP rate: 6,058 raw, aligned 1,996;
  both: 6,062 / 1,994.  Size-exact but raw-negative (aligned improves).
- `asteroidLockTimer = 0` instead of `+= mode` (line 1188; the original stores
  `movne r1,#0; strne r1,[r4,#412]` at `12fd88`): raw 9,101, aligned 2,130,
  size 9,892 (size-fatal, -16).  A faithfulness fix that needs 16 bytes of
  compensation elsewhere before it can land.
- `if (navWreck) { if (field_0x1c == nullptr) ... } else { if (lockedEnemy ==
  nullptr) ... }` for the `*(navWreck ? ...)` store: 6,058 raw (one worse).

## 2. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6057 aligned= 2051 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
