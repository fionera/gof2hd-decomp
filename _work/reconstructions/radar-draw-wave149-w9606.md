# Radar::draw wave 149 (w9606) — strd-tail cells and the `int planetHit` prologue-rotation lever

**Status:** source change landed, 2026-11-08. Worktree `wt/w9606`, based on `master`
(`7eed2966e`, wave 149). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave148-w9401.md`, `radar-draw-wave147-w9402.md`,
`radar-draw-wave147-w9309.md`, `radar-draw-wave149-w9501.md`.

Baseline (HEAD, w9402 shape): original 9908 B; ours 9908 B; raw same-address diff
**8,712**; instruction-index alignment **1,657**; `.text` **86,449** differing bytes;
`.rodata`/`.data`/`.bss`/`.ARM.exidx` byte-identical; `.ARM.extab` 1,309 (size exact);
`.rel.plt` 4; build-id 20.

## 1. Reproducing w9502's cell exactly

The w9502 worktree left no committed note; its reported cell (raw **8,811**, aligned
**2,040**, size **9,908 B**) is reproduced exactly by

> `A` `expect(this->enemies != nullptr, 1)` + `B` `int enemy wreck` + `C` first
> `expect(lockedEnemy…)` + `D` second `expect(lockedEnemy…)` + `O` `int planetHit` +
> `K` `int turretMode` + `r` `bool asteroidHit`

`ABCDOKr` = **9908 / 8811 / 2040**, `mov.w r9,#68` x3, `strd r6,r9`/`strd r5,r9`
x4. So w9502 recovered the *strd pairing* with **r9 = 68**, not the original's
**sl = 68** (the mission wording says `strd r6,sl`; the register is actually r9 in
every 9,908 B shape found). The original's `sl`=68 is a different allocation.

## 2. Every requested cell (all built on the w9402 base)

| cell | shape | usize | raw | align | sl68 | r9_68 | strd r9 | camp reg |
|---|---|---:|---:|---:|---:|---:|---:|---|
| baseline | w9402 | 9908 | 8712 | 1657 | 0 | 0 | 0 | stack |
| **`int wreck` alone** | B | **9924** | **8579** | **1650** | 0 | 0 | 0 | stack |
| **`expect` outer guard only** | A | **9804** | **9139** | **1922** | 0 | 4 | 2 | sl |
| `expect` + `int wreck` | AB | 9808 | 9162 | 1634 | 0 | 7 | 6 | stack |
| **`int wreck` + `bool asteroidHit` (LANDED)** | Br | **9908** | **8691** | **1493** | 0 | 0 | 0 | stack |
| + `int alienOrbit` | BrW | 9908 | 8688 | 1491 | 0 | 0 | 0 | stack |
| w9502 cell | ABCDOKr | 9908 | 8811 | 2040 | 0 | 3 | 4 | fp |
| w9502 cell + `int alienOrbit` | ABCDOKrW | 9908 | 8811 | 2039 | 0 | 3 | 4 | fp |
| `expect`+`int wreck`+`int planetHit`+`int turretMode`+`int enemyHit` | ABrOKE | 9908 | 8727 | 2137 | 0 | 3 | 4 | fp |
| `expect`+all-int-except… | ABCDEM | 9908 | 9006 | 1910 | 0 | 7 | 6 | r9 |
| + `int bossSeen` | ABCDMrH | 9908 | 9279 | 1905 | 0 | 7 | 6 | r9 |

`int wreck` alone is **not landable** (+16 B shifts every following function). `expect`
on the outer guard alone is **not landable** (−104 B). Every 9,908 B shape that carries
the strd tails (`r9_68 > 0`) has aligned ≥ 1,905 and raw ≥ 8,727, i.e. it regresses at
least one metric vs the 8,712 / 1,657 baseline. The strd tails cost ~250–400 aligned
instructions because 68 is hoisted into **r9**, not the original's **sl**.

## 3. New lever: `int planetHit` flips the prologue rotation

The original's prologue uses `fp` as the zero constant and `sl` as `campaign`
(`12e07a mov.w fp,#0`, `12e0f8 mov sl,r0`). Every baseline variant spills `campaign`
to `[sp,#144]` and uses `sl` as the zero/loop register.

`O` (`bool planetHit` → `int planetHit`, the planet-targets loop flag) flips this:
`campaign` moves out of the stack into a callee-saved register. With `A` on top
(`AO`, `ABO`, `ADO`, `ABDO`) it lands in **sl** (`mov sl,r0`), exactly the original's
home for campaign. That is the first source-level reproduction of the original
prologue rotation. `AO` is 9,824 B and `ABO` 9,828 B, so the rotation is real but the
body is still ~80 B short of 9,908. Adding `K` (`int turretMode`) flips campaign back
to `fp` and 68 to `r9`.

`ABO` keeps `mov.w r9,#68` (r9 free), so even with `campaign` in `sl` the allocator
still picks r9 for the height. The original keeps `&this->lockLabel` in r9 across the
`getPosition` calls (`12ecc8 add.w r9,r4,#396`, `12ee08 mov sl,r9`, `12eea0 mov r9,sl`),
which is what denies r9 to the constant. An explicit `String *lockLabelPtr =
&this->lockLabel` local reproduces the r9 occupation but grows the function by
80–200 B (`ABOL` 10,088), so it is not a 9,908 shape.

## 4. Landed change

```diff
-            bool wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
+            int wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
...
-            int asteroidHit = false;
+            bool asteroidHit = false;
```

`int enemy wreck` (unwitnessed) buys +16 B; `bool asteroidHit` (witnessed by w9206)
gives it back −16 B, keeping 9,908. Net effect:

* raw same-address **8,712 → 8,691** (−21)
* instruction-index alignment **1,657 → 1,493** (−164)
* `.text` differing bytes **86,449 → 86,428** (−21)
* `.ARM.extab` 1,309 (size exact 156,312), `.rodata`/`.data`/`.bss`/`.ARM.exidx`
  byte-identical, `.rel.plt` 4, build-id 20, all 27 section sizes exact.

This shape does **not** carry the strd tails; it is the best 9,908 B metric pair found
in the sweep. The strd-tail shapes remain blocked on the original's `sl = 68`
allocation.

## 5. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 6. Open questions

1. `O` (`int planetHit`) is the prologue-rotation lever: `AO`/`ABO` put campaign in
   `sl`. Find the missing ~80 B of spill body that keeps `AO`/`ABO` at 9,908 without
   `K` (which flips campaign to `fp`).
2. Occupy r9 with the original's transient `&this->lockLabel` without the +80 B of an
   explicit pointer local. That should move 68 from r9 to sl and drop the aligned
   metric below 1,657.
3. `ABCDOKr` is the exact w9502 cell; any future merge must re-measure it against
   `Br` (they are mutually exclusive source edits).
