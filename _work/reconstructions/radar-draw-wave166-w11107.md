# Radar::draw wave 166 (w11107) — sound-ladder sub-guard expects

**Status:** one source change landed, 2026-11-10. Worktree `wt/w11107`, branch
`wave166-w11107`, based on `master` (`85c1f88df`). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), size 9,908 B.

Read with `radar-draw-wave164-w11008.md` (the prior landing: `field_0x1bc`
sound ladder + faithful tail, 8,124 -> 7,549 raw).

## 0. Entry state (master 85c1f88df)

raw **7,549**, aligned **1,446**, size **9,908/9,908**, `.text` 83,147
differing, `.ARM.extab` 659, `.rel.plt` **0**. `check` green (byte 4507,
linked 4507, size 4524, data 843, imports 207, lint CLEAN, parity 5370).

## 1. Landed: flip the sub-guard expects in the `field_0x1bc` sound ladder

The wave-164 note had `__builtin_expect(this->field_0x1bc < 3, 1)` and
`__builtin_expect(this->field_0x1bc < 5, 1)`. The original's branch layout
wants the *cold* arm to be the `else` (out of line) and the `>= 5` arm to
fall through, so the expects are `0`. Additionally the two `alwaysEnemySeen`
tests in the `< 5` and `>= 5` arms are hot and want `expect(..., 1)`.

```cpp
} else if (__builtin_expect(this->field_0x1bc < 3, 0)) {  // lint: arm_builtin
    ...
} else if (__builtin_expect(this->field_0x1bc < 5, 0)) {  // lint: arm_builtin
    if (alwaysEnemySeen) { ... }
    else if (__builtin_expect(bossSeen, 1)) { ... }       // lint: arm_builtin
    ...
} else {
    if (__builtin_expect(alwaysEnemySeen, 1)) { ... }     // lint: arm_builtin
    ...
}
```

Measured (all size 9,908 unless noted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| entry base | 7,549 | 1,446 | 9,908 |
| `<3` expect 0 only | 7,548 | 1,449 | 9,908 |
| `<5` expect 0 only | 7,538 | 1,449 | 9,904 |
| both `<3`,`<5` = 0 | 7,538 | 1,449 | 9,904 |
| both 0 + `bossSeen,1` in `<5` arm | 7,510 | 1,424 | 9,904 |
| **both 0 + `bossSeen,1` + `alwaysEnemySeen,1` in `>=5` arm (LANDED)** | **7,490** | **1,431** | **9,908** |
| all 8 combinations of the three ladder expects | 7,538-8,196 | 1,429-1,449 | 9,904/9,908 |

Exit: raw **7,490**, aligned **1,431**, size **9,908**, `.text` **83,088**,
`.ARM.extab` **658**, `.rel.plt` **0**. `check` = RATCHET PASS (byte 4507,
linked 4507, size 4524, data 843, imports 207, lint CLEAN, parity 5370).

## 2. Rejected variants (entry shape, all measured)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `expect(current == 143, 0/1)` | 7,549 / 8,222 | 1,446 / 1,498 | 9,908 / 9,884 |
| `alwaysEnemySeen` as `int` | 8,455 | 1,410 | 9,904 |
| `alwaysEnemy` as `int` | 8,631 | 1,467 | 9,916 |
| `notOtherLock`/`notTurretMode` as `bool` | 8,946 | 1,795 | 9,860 |
| reorder `campaign` before `alienOrbit` | 7,556 | 1,451 | 9,908 |
| `alwaysEnemySeen != 0` / `== 1` | 7,549 | 1,446 | 9,908 |
| `expect(cur != 145, 0/1)` in ambient block | 8,181 | 1,439 | 9,900 |
| `expect(alwaysEnemySeen,1)` in `<3` arm | 7,510 (no change) | 1,424 | 9,904 |
| `expect(bossSeen,1)` in `<3` arm | 7,510 (no change) | 1,424 | 9,904 |
| `expect(bossSeen,1)` in `>=5` arm only | 7,510 (no change) | 1,424 | 9,904 |
| reorder `!landmarkHit && notOtherLock && i != 3 && notTurretMode` | 8,447 | 1,419 | 9,912 |
| nested `if (!landmarkHit && notOtherLock) { if (i != 3 && notTurretMode) }` | 9,423 | 1,557 | 9,884 |

## 3. Residual 4 B / cleanup pads

Still the known wave-164 residual: ours has 7 destructor landing-pad sites
vs the original's 6, and `_Unwind_Resume` is at `0x1306e8` vs `0x1306ea`
(literal pool 4 B larger to compensate).  The extra site is a
`StringD1Ev` on `sp+220` at `0x1306de`.  It comes from the landmark
`DrawString` `String` temporary
(`*getText(133) + String(": ") + String(String(...), false)`); every
reshaping tried in wave 164 was much worse (raw 8,562-9,444), so it is
left alone.
