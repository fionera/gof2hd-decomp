# Radar::draw wave 190 (w13404) — music-tail first branch: original CAB order
# (`inAlienOrbit` -> `isAttackedByAliens` -> `campaign == 16`); aligned 1,816 -> 1,811

**Status:** landed, 2026-11-12. Worktree `wt/w13404`, branch `wave190-w13404`. Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`, 9,908 B.

Read with `radar-draw-wave190-w13404.md` (spark `inSight` box, landed) and
`radar-draw-wave190-w13398.md` / `-w13381.md` / `-w13370.md`.

## 0. The change

The music tail's first branch (source ~line 1276) evaluated the three predicates in the
order `isAttackedByAliens` -> `getCurrentCampaignMission() == 16` -> `inAlienOrbit`.
The original binary (`1302ac..1302c6`) evaluates them **`inAlienOrbit` ->
`isAttackedByAliens` -> `campaign == 16`**:

```
1302ac  blx Status::inAlienOrbit
1302b0  cbnz r0, 1302c8
1302b4  blx Station::isAttackedByAliens
1302bc  cbnz r0, 1302c8
1302c0  blx Status::getCurrentCampaignMission
1302c4  cmp r0, #16
1302c6  bne.n 130348
```

Source edit (`a || b || c` reordered to the original's short-circuit order):

```cpp
-            if (Globals::status->getStation()->isAttackedByAliens() ||
-                Globals::status->getCurrentCampaignMission() == 16 || Globals::status->inAlienOrbit()) {
+            if (Globals::status->inAlienOrbit() || Globals::status->getStation()->isAttackedByAliens() ||
+                Globals::status->getCurrentCampaignMission() == 16) {
```

Semantically identical (three pure getters; the `||` short-circuit order does not change
the result).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 6,307 | 6,322 |
| instruction-index alignment | 1,816 | **1,811** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.rel.plt` | byte-identical | **byte-identical** (diff 0) |
| `.ARM.extab` | 603 | 603 (no growth) |
| `.ARM.exidx` | byte-identical | byte-identical |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370. The function is already
non-byte-exact, so `byte_exact` is unchanged.

The +15 raw is a shift artefact: the reordered tail emits in the original's order but the
whole tail still sits at a different address (the enemy/asteroid cold block is ~+126 B in
ours), so same-address bytes cannot line up. The shift-invariant aligned metric — the
progress measure for this function — improves by 5 instructions.

## 1. Negative results measured this pass (reverted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| second branch `inAlienOrbit \|\| isAttackedByAliens` (`c2`) | 6,307 | 1,816 | 9,908 |
| both branches reordered (`c1c2`) | 6,322 | 1,811 | 9,908 |

`c2` is neutral; `c1c2` equals `c1`. The second branch already matches the original's
order after the `c2` swap, so it contributes nothing on top of `c1`.

## 2. Second landed change — mask check before the `alwaysEnemySeen` expect

The original evaluates the three-way guard at `130280..1302a2` in the order
`(current-136) > 15` -> `((1 << (current-136)) & 0xe071) == 0` -> `(current != 151 &&
alwaysEnemySeen)` (the mask check precedes the `alwaysEnemySeen` expect). The source had
the expect second. Swapping the two operands:

```cpp
-        if ((uint32_t) (current - 136) > 15 ||
-            __builtin_expect((current != 151 && alwaysEnemySeen), 0) ||  // lint: arm_builtin
-            ((1 << (current - 136)) & 0xe071) == 0) {
+        if ((uint32_t) (current - 136) > 15 ||
+            ((1 << (current - 136)) & 0xe071) == 0 ||
+            __builtin_expect((current != 151 && alwaysEnemySeen), 0)) {  // lint: arm_builtin
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 6,322 | 6,330 |
| instruction-index alignment | 1,811 | **1,809** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.rel.plt` | byte-identical | **byte-identical** (diff 0) |
| `.ARM.extab` | 603 | 603 (no growth) |

`RATCHET PASS` (byte_exact 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 /
lint CLEAN / drift 0 / parity 5,370). Raw music-region diff 940 -> 963; the aligned metric
(shift-invariant) improves by 2.

The original's branchy form of the `&&` (`cmp r1,#151; beq; ldr; eors.w; bne`) is
reproduced by `(current != 151 && __builtin_expect(alwaysEnemySeen, 0))` (aligned 1,801)
but that spelling is 4 B short (9,904/9,908) because the whole music tail is ~6 B shorter
than the original's layout; it is held for a later pass that also recovers the tail
block-layout bytes.

## 3. Third landed change — drop the `__builtin_expect(field_0x1bc >= 1, 1)` hint

The `if (__builtin_expect(this->field_0x1bc >= 1, 1))` hint made clang treat the whole
music body as hot and place the `else` arm (`field_0x54 = 0; campaign != 145; ...`) out of
line at `1304e2` behind a `blt.w`. The original places that else arm **inline at `1302f0`**
behind a short `blt.n` (`13027e db37`). Removing the hint:

```cpp
-    if (__builtin_expect(this->field_0x1bc >= 1, 1)) {  // lint: arm_builtin
+    if (this->field_0x1bc >= 1) {
```

makes clang emit the else arm at `1302dc` behind a short `blt.n` (`13027a db2f`), matching
the original's branch shape.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 6,330 | 7,262 |
| instruction-index alignment | 1,809 | **1,765** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,543 / 3,532 | 3,537 / 3,532 |
| `.text` differing bytes (whole `.so`) | 73,038 | 73,970 |
| `.rel.plt` | byte-identical | **byte-identical** (diff 0) |
| `.ARM.extab` | 603 | 603 (no growth) |

`RATCHET PASS` (byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 / imports 207 /
lint CLEAN / drift 0 / parity 5,370). The +932 raw is a pure shift artefact: the whole
music tail now sits at a different offset (the enemy/asteroid cold block is still ~+126 B
in ours), so same-address bytes cannot line up; the shift-invariant aligned metric improves
by 44 instructions. Instruction count 3,543 -> 3,537 (closer to the original's 3,532).

## 4. Fourth landed change — second music branch in original order

The second music branch (`field_0x1bc < 1` else arm) evaluated `isAttackedByAliens` before
`inAlienOrbit`. The original (`130396..1303ae`) evaluates **`inAlienOrbit` first**:

```
130396 ldr r0,[r8]
13039a blx Status::inAlienOrbit
13039e cbnz r0, 1303b2
1303a0 ldr r0,[r8]
1303a4 blx Status::getStation
1303a8 blx Station::isAttackedByAliens
1303ac cmp r0,#0
1303ae beq.w 130598
```

```cpp
-                if (Globals::status->getStation()->isAttackedByAliens() || (Globals::status->inAlienOrbit())) {  // lint: arm_builtin
+                if (Globals::status->inAlienOrbit() || Globals::status->getStation()->isAttackedByAliens()) {
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 7,262 | 7,267 |
| instruction-index alignment | 1,765 | **1,763** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.rel.plt` | byte-identical | **byte-identical** (diff 0) |
| `.ARM.extab` | 603 | 603 (no growth) |

`RATCHET PASS` (byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 / imports 207 /
lint CLEAN / drift 0 / parity 5,370). Raw `.text` diff 73,970 -> 73,975.

## 5. Held variant (not landed): condition without the `alwaysEnemySeen` expect

`if (A || ((1 << (current - 136)) & 0xe071) == 0 || (current != 151 && alwaysEnemySeen))`
(no `__builtin_expect`) reproduces the original's branchy `&&` at `130298..1302a2`
(`cmp r1,#151; beq; ldr; eors.w; bne`) and reaches aligned **1,723**, but the function is
**9,912 B (+4)**: the two `field_0x1bc < 3` / `< 5` bodies fall out of line behind `ble.w`
instead of the original's inline fallthrough, and the `eor.w` gets an extra `lsls
r2,r2,#31`. Held until a size-exact compensation (most likely the `campaign` register
rotation, which is still +4 B in the early region) is found.
