# Radar::draw wave 147 (w9308) — landmark DrawString un-merge at 9908 via a hoisted rowHeight

**Status:** source change landed, 2026-11-08. Worktree `wt/w9308`, based on `master`
(`9c9744ca5`, wave 146). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8,733 B** (wave-146
master). After this pass **8,672 B** (-61). Aligned instruction diff **2,712 -> 2,726**
(+14; the un-merge adds the 9th call and re-orders the landmark tail, so the greedy
alignment scores slightly worse even though the call census now matches). `.text`
differing bytes **85,928 -> 85,867** (-61). `.ARM.extab` size exact (156,312), differing
bytes **1,393 -> 1,395** (+2). `.ARM.exidx`/`.rodata`/`.data`/`.bss` byte-identical;
`.rel.plt` 4 (unchanged). All 27 section sizes exact; file size exact. Gate:
`RATCHET PASS`, byte **4507**, linked **4507**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

Read with `radar-draw-wave146-w9206.md`, `radar-draw-wave145-w9010.md`,
`radar-draw-wave145-w9009.md`.

## 1. Mission and answer

The mission asked for the source shape that emits the original's **9** landmark-loop
`DrawString` calls at exactly 9908 B (ours tail-merged the `i == 0` and `else` lockLabel
draws to 8 calls), then to switch `enemyHit` to `bool` and measure.

**The shape is a hoisted `int rowHeight = Globals::layout->field_0x4;` placed *after* the
first landmark `DrawString` (the `labelStrings[i]` draw), used as the row step in both
lockLabel draws.** It is semantically identical to the two inline
`Globals::layout->field_0x4` uses, but the extra local changes the register allocation
of the landmark tail enough that clang 7.0.2 no longer tail-merges the two
`blx DrawString` sites. DrawString census becomes **9 == 9** and the function lands at
exactly **9908 B** with `.ARM.extab` size exact.

`bool enemyHit` is the witnessed type (the original widens it with `and r1,#1` at
`12f3c2` before storing `enemyHit`), and `bool asteroid wreck` is also witnessed: the
original's asteroid tractor test at `12fcc4` is `eors.w r0, sl, #1` (bool negation),
not the `cmp/movne/orrs` sequence an `int` produces. The enemy `wreck` stays `int`
(word store `12ed82 str r0,[sp,#152]`).

## 2. Landed source

```cpp
int offset = i == 0 ? this->originX : this->originY;
Globals::Canvas->DrawString(radarFont(), *this->labelStrings->data()[i],
                            this->screenX + offset, this->screenY, false);
int rowHeight = Globals::layout->field_0x4;      // <-- un-merges the two lockLabel draws
if (i <= 1) {
    if (i == 0 && !alienOrbit) {
        ... first DrawString (": " + tecLevel) ...
        this->lockLabel = this->calcDistance(...);
        DrawString(radarFont(), this->lockLabel, this->screenX + this->originX,
                   this->screenY + rowHeight * 2, false);
    } else {
        ...
        this->lockLabel = this->calcDistance(...);
        DrawString(radarFont(), this->lockLabel, this->screenX + offset,
                   this->screenY + rowHeight, false);
    }
}

// enemy loop
bool enemyHit = false;                            // witnessed bool
// asteroid loop
bool wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());  // witnessed bool
```

`bool alwaysEnemy` and `bool asteroidHit` are unchanged (witnessed in wave 146).

## 3. Variant matrix

All variants below carry `bool enemyHit` + `bool asteroid wreck` (the faithful set;
9880 B with no un-merge). `raw` is the same-address byte diff, `al` the aligned
instruction diff, `extab` the `.ARM.extab` size (original 156,312).

| variant | usize | raw | al | extab | calls |
|---|---:|---:|---:|---:|---:|
| baseline wave-146 master (`int enemyHit`, no un-merge) | 9908 | 8733 | 2712 | exact | 8 |
| faithful types, no un-merge | 9880 | 8812 | 2682 | exact | 8 |
| `String labelCopy` in i==0 arm | 9912 | 8671 | 2691 | **+12** | 9 |
| `String labelCopy` in else arm | 9900 | 8673 | 2681 | **+12** | 9 |
| hoist rowHeight after `int offset` | 9892 | 7921 | 2920 | exact | 9 |
| **hoist rowHeight after first DrawString (landed)** | **9908** | **8672** | **2726** | **exact** | **9** |
| hoist rowHeight after first DrawString, `const` | 9908 | 8672 | 2726 | exact | 9 |
| hoist labelY after `int offset` | 9920 | 8689 | 2731 | exact | 9 |
| hoist labelY + rowHeight | 9904 | 8442 | 2840 | exact | 9 |
| `lockLabel = lockLabel` self-assign in i==0 | 9828 | 9134 | — | exact | 9 |
| `lockLabel = lockLabel` self-assign in else | 9832 | 9140 | — | exact | 9 |

The `String labelCopy` variants look best at the *function* level (raw 8671-8673,
aligned 2681-2691) but are rejected: the extra `String(String const&, bool)` cleanup
grows `.ARM.extab` by 12 B, which shifts `.ARM.exidx`/`.data`/`.bss` and raises the
whole-`.text` diff to **90,394** (vs 85,928 baseline). They are metric-only.

The `rowHeight` hoist is the only shape that is size-exact, extab-size-exact, emits
9 calls, and lowers the whole-`.text` diff (-61).

## 4. Remaining deltas / open questions

1. The hoist is not literally in the original: the original reloads
   `Globals::layout->field_0x4` from the layout pointer in each arm
   (`12e9cc ldr r2,[r2,#4]`, `12eb1c ldr r2,[r2,#4]`). Our hoisted local lives in a
   stack slot and is reloaded (`ldr r2,[sp,#104]`), so the codegen is close but the
   source shape is a workaround, not the original. The original's un-merge is a pure
   register-allocation artefact (`mov r9,r6` after the else draw), which no tested
   source shape reproduces without changing the call census.
2. The aligned metric regresses +14 because the un-merged tail re-orders the landmark
   block. Raw and `.text` both improve; this is the trade the mission asked for.
3. `.ARM.extab` differing bytes +2 (size exact) — the hoisted local slightly changes
   the LSDA cleanup range. No size regression.
4. The extra `FModSound::play` (ours 8, original 7) and the outlined-tail
   `strd r5,sl,[sp]` vs `str r6,[sp,#0]; movs r6,#68; str r6,[sp,#4]` deltas from
   wave 146 remain.

## 5. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,867** differing bytes (size/base exact
1,259,464), `.ARM.extab` **1,395** (size exact 156,312), `.rel.plt` 4
(setVisible/deleteHackingGame swap), `.note.gnu.build-id` 20. `.rodata`, `.data`,
`.bss`, `.got`, `.ARM.exidx`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are
byte-identical. All 27 section sizes exact.
