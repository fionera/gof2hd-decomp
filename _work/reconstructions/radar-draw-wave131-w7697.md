# Radar::draw wave 131 (w7697) — negated 24000-unit bounds test (`bgt`/`bmi`)

**Status:** structural source win, 2026-11-07. Worktree `wt/w7697`, based on
`master` (`7c6933c2d`). Target `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), TU `src/game/weapons/Radar.cpp`.

Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Raw fuzzy score is **unchanged** (75.3548, n=3492/3483) and the whole-`.text`
differing-byte count is **unchanged** (85,379), because the six branch
instructions are at layout-shifted addresses and differ in their target bytes
either way. This pass is a *semantic/structural* fidelity fix, not a score win.

Gate before and after: `RATCHET PASS`, byte **4506**, linked **4506**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave129-w7673.md`, `radar-draw-body-wave122-w7210.md`,
`radar-draw-body-wave121-w7106.md`, `radar-draw-three-shape-9908-w4105.md`.

## 1. The change: negate the six 24000-unit comparisons

The enemy on-screen bounds test was written as `<= 24000.0f` / `>= -24000.0f`.
Clang canonicalises `!(x <= 24000)` to `x > 24000 || unordered`, which it emits
as an unsigned-hi / less-than pair (`bhi` / `blt`). The original emits the
ordered forms `bgt` / `bmi`:

```
12ee36  vsub.f32 s0, s2, s0        ; egoPos.x - targetPos.x
12ee3a  vcmpe.f32 s0, s18          ; s18 = +24000.0f
12ee3e  vmrs APSR_nzcv, fpscr
12ee42  bgt.n 12ee9e               ; x >  24000 -> reject
12ee44  vcmpe.f32 s0, s16          ; s16 = -24000.0f
12ee48  vmrs APSR_nzcv, fpscr
12ee4c  bmi.n 12ee9e               ; x < -24000 -> reject
```

Writing the source as the negation of the two strict comparisons reproduces
the original's ordered `bgt`/`bmi` exactly:

```diff
-                if (this->egoPos.x - this->targetPos.x <= 24000.0f && this->egoPos.x - this->targetPos.x >= -24000.0f &&
-                    this->egoPos.y - this->targetPos.y <= 24000.0f && this->egoPos.y - this->targetPos.y >= -24000.0f &&
-                    this->egoPos.z - this->targetPos.z <= 24000.0f && this->egoPos.z - this->targetPos.z >= -24000.0f &&
+                if (!(this->egoPos.x - this->targetPos.x > 24000.0f) && !(this->egoPos.x - this->targetPos.x < -24000.0f) &&
+                    !(this->egoPos.y - this->targetPos.y > 24000.0f) && !(this->egoPos.y - this->targetPos.y < -24000.0f) &&
+                    !(this->egoPos.z - this->targetPos.z > 24000.0f) && !(this->egoPos.z - this->targetPos.z < -24000.0f) &&
                     enemy->getType() != 16917) {
```

After the change ours emits `bgt`/`bmi` at `12ee5e`/`12ee68`, `12ee7e`/`12ee88`,
`12ee9e`/`12eea8` (the `0x1c` shift is the known earlier-block delta). The other
two 24000-unit tests in the function (lines ~892–897 and ~998–1000) were
already written in the `>`/`<` form and already emit `bgt`/`bmi`.

## 2. New measurements this wave (not in earlier notes)

1. **The enemy `inSight` test is distance-first in the original, but
   distance-first loses score.** Original `0x12ef4c..0x12efc0` loads
   `crosshairPos.x/y` into `s26`/`s24`, computes
   `VectorLength(egoPos - targetPos)`, compares `< 60000.0f`, then does the four
   `field_0x124` bounds tests with `vcvt` after the call. This is the
   `field_0x73` crosshair test, *not* the 24000 test. Rewriting the baseline
   (bounds first, distance last) into that distance-first order gives
   9896 B / 68.8 % — worse than baseline 9908 B / 75.35 %. This contradicts
   `radar-draw-body-wave122-w7210.md` §4.1's implication that the faithful
   shape is also the better-scoring one.
2. **Asteroid dead-check**: `if (isDead() || isDying()) { if (!hasCargo) continue; }`
   (nested) is size- and score-identical to the baseline
   (75.3548) and matches the original's call order better than the
   `dead && !hasCargo` form (72.8 %, 9928 B).
3. **`otherLock`**: the original's `cbnz` chain (`lockedAsteroid`, then
   `lockedGasCloud`, then `field_0x1c`) is not reproducible from source with
   clang 7. Every spelling tried (nested ifs, else-if, positive OR,
   init-to-1, with/without `__builtin_expect`, inner expect 1/0, outer expect
   0/1, whole-chain expect) gives either `cbz`+`itt eq` (75.3548, 9908 B) or a
   `cbnz` chain that costs 56 B (9852 B, 72.96 %). Baseline is a local optimum.
   Baseline `otherLock` = `r9`, `status` = `r5`; original `otherLock` = `r5`,
   `status` = `r6`.
4. **Landmark loop rotation**: `for(;;){if(i>=size)break;}` is byte-identical
   to baseline. `while(true)` collapses to 9656 B / 66.6 %. `for (uint32_t i…)`,
   `i != size`, `size > i`, `!(i >= size)` all tie at 75.3548.
   `__builtin_expect(i<size,0)` → 59.2 %; `expect(...,1)` → 37.2 % (10064 B).
   The loop header stays bottom-placed (`bcc.w` back-edge) vs the original's
   top-tested header at `0x12e7d4`.
5. **Duplicate autopilot recompute** (lines ~615–625) is real in the baseline:
   ours has `getAutoPilotTarget` 2×, `getStations` 3×, `getStationEnumIndex` 2×,
   `getSystem` 7×, `getPlayer` 31×; original has 1/2/1/5/30. Removing it gives
   9856 B / 70.8 %; reordering it to the initial-block order gives
   9916 B / 73.6 %.
6. **`X1`**: the `else if (field_0x11e && field_0x11f) { x=screenX; y=screenY+11; }`
   arm is redundant with the following `else if (field_0x11f)` arm; removing it
   is byte-identical (75.3548). Not landed.
7. **`E`** (i==0 landmark arm assigns `this->lockLabel` instead of local
   `String label`) → 9904 B / 71.85 %. **`F`** (swap `field_0x18` /
   `planetLockTimer` stores) → 9904 B / 65.68 %.

## 3. Variant table (single edits on baseline)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | 75.3548 |
| **negated 24000 bounds (`rev`, landed)** | 9908 | 75.3548 (tie) |
| distance-first `inSight` | 9896 | 68.8 |
| asteroid dead-check nested (`D2`) | 9908 | 75.3548 (tie) |
| asteroid `dead && !hasCargo` (`D1`) | 9928 | 72.8 |
| `otherLock` `cbnz` chain | 9852 | 72.96 |
| whole-chain `expect(...,1)` (`Q1`) | 9872 | 73.4 |
| `Q3` (first two terms expect 1) | 9900 | 74.1 |
| landmark `while(true)` | 9656 | 66.6 |
| remove duplicate autopilot recompute | 9856 | 70.8 |
| reorder autopilot recompute | 9916 | 73.6 |
| remove redundant `field_0x11e && field_0x11f` arm (`X1`) | 9908 | 75.3548 (tie) |
| `E` (i==0 arm writes `this->lockLabel`) | 9904 | 71.85 |
| `F` (swap `field_0x18`/`planetLockTimer` stores) | 9904 | 65.68 |

## 4. Root cause still open

Unchanged from wave 129: the first `otherLock` block is a `cbnz` chain in the
original but `cbz`+`itt` in ours (moving the first literal pool from `12e340`
to `12e310`); the planet campaign `isTarget` rematerialises
`stations->data()[i]` in the original; `marked` is canonicalised on reload;
and the landmark loop header is bottom-placed in ours. A real byte-exact win
still needs a source shape that simultaneously keeps the first `otherLock`
contiguous, keeps the single campaign reload with `stations` in `sl`+spill,
and un-rotates the landmark loop.

## 5. Gate / section deltas (after the landed change)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` 85,379 differing bytes (size/base
exact), `.ARM.extab` 1,435, `.note.gnu.build-id` 20. `.rodata`, `.data`,
`.bss`, `.got`, `.ARM.exidx`, `.rel.plt`, `.rel.dyn`, `.dynsym`, `.dynstr`,
`.hash` are byte-identical.

## 6. Artifacts

Scratch in `/dev/shm/w7697` (not committed): `Radar.base.cpp` (pristine
baseline source, sha256 `6a97cabddbd9abc603e47c5ddc2e57af292e55732e71ecb6798b248d147102e4`),
`score.py` (single-symbol scorer mirroring `tools/verify/verify.py`),
`driver.py`/`v1.py`–`v28.py` (variant drivers), `orig.dis`/`ours.dis`
(full function disassembly), `radar_dump.txt` (Ghidra decompilation),
`all.log` (MIR `-print-after-all` dump), `textdiff.py` (whole-`.text`
differing-byte count). The analysis is self-contained in this note.

## 7. Addendum — `bool otherLock` reproduces the `cbnz` chain but loses 96 B

New this wave (all measured against the landed baseline, 9908 B / 75.3548):

The first `otherLock` block is the single biggest structural delta. The original
(`0x12e2fc..0x12e312`) is a `cbnz` chain and keeps `otherLock` in `r5`:

```
12e2fc  mov   r1, r4
12e2fe  ldr.w r0, [r1, #12]!   ; &lockedAsteroid, lockedAsteroid
12e302  str   r1, [sp, #124]
12e304  cbnz  r0, 12e312       ; lockedAsteroid != 0 -> otherLock = 1
12e306  ldr   r0, [r4, #56]    ; lockedGasCloud
12e308  cbnz  r0, 12e312
12e30a  ldr   r0, [r4, #28]    ; field_0x1c
12e30c  cmp   r0, #0
12e30e  beq.w 12ebc2           ; all null -> otherLock = (field_0x14 != 0)
12e312  movs  r5, #1
```

The baseline (`__builtin_expect(...,0)`, `int`) emits `cbz r0, <rest>` +
`mov.w r9,#1` and `itt eq` for the remaining two tests, with `otherLock` in
`r9`. Declaring the variable `bool` flips clang to the original's exact shape:

```
12e318  cbnz  r0, 12e326
12e31a  ldr   r0, [r4, #56]
12e31c  cbnz  r0, 12e326
12e31e  ldr   r0, [r4, #28]
12e320  cmp   r0, #0
12e322  beq.w 1305a4
12e326  movs  r5, #1          ; otherLock in r5, matching the original
```

and it also produces the original's materialised `notOtherLock` at the landmark
loop setup (`eor.w r0, r5, #1`, original `0x12e7c8`; baseline folds
`!otherLock` into `cmp r9,#0`). Ghidra independently types this variable
`bool` (`bVar46` in `radar_dump.txt`).

But every `bool` spelling lands at **9812 B / 69.5 %** (96 B short of the
original), and `bool`-then-`int` lands at **9852 B / 72.96 %** (56 B short —
the "cbnz chain costs 56 B" row from wave 129). So the `bool` shape is
almost certainly the original's, but it enables a further 56–96 B of
simplification somewhere else (likely the later `otherLock = ... || ...`
assignments and the `!otherLock`/`!turretMode` uses) that the original does
not have. The next wave should combine `bool otherLock` with a source shape
that restores those bytes — that is the most promising remaining lead.

Variant table for this addendum:

| variant | usize | raw % |
|---|---:|---:|
| **baseline (landed)** | 9908 | 75.3548 |
| `bool otherLock` (B) | 9812 | 69.0679 |
| `bool otherLock = true; if (all null) …` (L) | 9812 | 69.5001 |
| `bool otherLock = !(all null) \|\| field_0x14 != 0` (M) | 9812 | 69.5001 |
| `bool otherLockFlag; int otherLock = otherLockFlag` (K) | 9852 | 72.9625 |
| `bool notOtherLock = !otherLock` (G) | 9920 | 71.9633 |
| `bool notTurretMode = !turretMode` (H) | 9844 | 73.4359 |
| both `not*` bool (I) | 9920 | 71.9633 |
| `bool notOtherLock = otherLock == 0` (J) | 9920 | 71.9633 |
| `int` negated-OR form (P) | 9908 | 74.1792 |
