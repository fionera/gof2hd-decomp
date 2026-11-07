# Radar::draw pass (wave 129, w7673) — landmark else-if block-order fix (75.268817 -> 75.326165 %, 9908 B)

**Status:** source win, 2026-11-07. Worktree `wt/w7673`, based on current
`master` (`003a9623a`). Target `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), TU `src/game/weapons/Radar.cpp`.

Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.268817 %** (official display 75.3). After this pass = **75.326165 %**
(display 75.3). Gate at start and end: `RATCHET PASS`, byte **4506**,
linked **4506**, data 843/843, imports 207, lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave128-w7650.md`, `radar-draw-wave127-w7628.md`,
`radar-draw-wave126-w7606.md`, `radar-draw-planet-loop-wave125-w7512.md`.

## 1. The win: the landmark `i==1` / `i==3` else-if source order

The landmark loop's screen-edge fallback is an `else if` chain:

```cpp
} else if ((i & 0x7fffffff) == 1) {
    Globals::Canvas->DrawImage2D(this->image_0xbc, ...);
} else if ((i & 0x7fffffff) == 3) {
    Globals::Canvas->DrawImage2D(this->image_0xb8, ...);
}
```

The baseline source order (`==1` then `==3`) compiles to **checks in reverse
source order** (`cmp #3` first, then `cmp #1`) with the `==1` block first in
layout and the `==3` block falling through to the shared `DrawImage2D`:

```
12ea00  bic  r0, sl, #0x80000000
12ea04  cmp  r0, #3
12ea06  beq.n 12ea14
12ea08  cmp  r0, #1
12ea0a  bne.w 12eb44          ; increment
12ea0e  ldr  r1, [r4, #188]   ; image_0xbc  (==1 block)
12ea12  b.n  12ea18
12ea14  ldr  r1, [r4, #184]   ; image_0xb8  (==3 block)
12ea18  ... DrawImage2D ...
```

The original checks `==1` first, then `==3`, but lays the `==3` block out
first and lets the `==1` block fall through:

```
12e9ea  bic  r0, sl, #0x80000000
12e9ee  cmp  r0, #1
12e9f0  beq.n 12e9fc
12e9f2  cmp  r0, #3
12e9f4  bne.n 12ea18          ; increment
12e9f6  ldr  r1, [r4, #184]   ; image_0xb8  (==3 block)
12e9fa  b.n  12ea00
12e9fc  ldr  r1, [r4, #188]   ; image_0xbc  (==1 block)
12ea00  ... DrawImage2D ...
```

Swapping the two source arms (`==3` first, `==1` second) makes clang emit
exactly the original's check order and block order. This is a genuine
source-order reconstruction, not a codegen hack: the original source ordered
the `==3` arm before the `==1` arm.

Diff:

```diff
-            } else if ((i & 0x7fffffff) == 1) {
-                Globals::Canvas->DrawImage2D(this->image_0xbc, ...);
             } else if ((i & 0x7fffffff) == 3) {
                 Globals::Canvas->DrawImage2D(this->image_0xb8, ...);
+            } else if ((i & 0x7fffffff) == 1) {
+                Globals::Canvas->DrawImage2D(this->image_0xbc, ...);
             }
```

The function stays at 9908 B and the full gate stays green. The whole-`.text`
differing-byte count is unchanged (85,441): the fix moves matching
instructions into place without changing the total number of mismatched bytes
elsewhere, but it removes a real structural delta and raises the fuzzy score.

## 2. Loop nesting / block order / literal-pool sweep (new measurements)

The assignment's hypothesis was tested directly. The two loops are both
**unrotated in the IR** (planet header `%718`, latch `%952`, `br label %718`;
landmark header `%320`, latch `%581`, `br label %320`). The difference is
purely `MachineBlockPlacement`:

* planet loop: header `bb.117` stays at the top (`bb.116` preheader,
  `bb.117` header, `bb.118` body, ..., `bb.168` latch, `bb.169` exit);
* landmark loop: header `bb.36` is moved to the bottom (`bb.35` setup,
  `bb.39` body, ..., `bb.87` latch, `bb.36` header, `bb.37` exit).

The MIR dump (`-mllvm -print-after-all`) shows the landmark header's edge
weights are `bb.39 (96.88 %) / bb.37 (3.12 %)`, i.e. the standard
`calcLoopBranchHeuristics` weights, and `MachineBlockPlacement::rotateLoop`
rotates the chain so the header becomes the bottom block. The planet loop has
the same edge weights but is not rotated, so the difference is in the chain /
exit selection, not in the source `for` spelling. Every source-level loop
spelling tried this wave left the layout unchanged:

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | 75.268817 |
| `for (int i = 0; i < (int) size; ++i)` | 9908 | 75.268817 (tie) |
| `for (int i = 0; i != (int) size; ++i)` | 9908 | 75.268817 (tie) |
| `for (unsigned i = 0; i < size; ++i)` | 9908 | 75.268817 (tie) |
| `if`-wrapped body (no `continue`) | 9908 | 75.268817 (tie) |
| `if (!(...))`-wrapped body | 9908 | 75.268817 (tie) |
| `switch (i & 0x7fffffff)` for the two arms | 9908 | 75.268817 (tie) |
| hoisted `int masked = i & 0x7fffffff` | 9908 | 75.268817 (tie) |
| `for (...; __builtin_expect(i < size, 1); ...)` | 10064 | 37.187500 |

Literal pools (measured, unchanged by this pass): the pool **words are
identical and in the same order** in both binaries. First pool (5 words):
`__stack_chk_guard`, `Globals::Canvas`, `Globals::status`,
`PaintCanvas::DrawImage2D`, `Globals::font`; second pool (3 words):
`StarSystem::orbitPlanetIndex`, `PlayerEgo::crosshairPos`, `Globals::font`.
Only their positions differ (first `12e340` orig / `12e310` ours; second
`12e79c` orig / `12e7f4` ours), which is the known consequence of the first
`otherLock` block and the landmark header rotation.

## 3. Other variants measured this wave

All rows are single edits on the pristine baseline unless noted; `rev` is the
landed change.

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | 75.268817 |
| **landmark arms swapped (`rev`, landed)** | 9908 | **75.326165** |
| `rev` + `i<=1` then/else swapped | 9908 | 74.580645 |
| `i<=1` then/else swapped alone | 9908 | 74.523297 |
| `rev` + first-`otherLock` outer `expect(...,1)` | 9872 | 73.451200 |
| `rev` + first-`otherLock` inner `expect(...,0)` | 9908 | 72.096358 |
| `rev` + first-`otherLock` no expect | 9908 | 74.150538 |
| `rev` + positive-OR `otherLock`, plain | 9908 | 74.150538 |
| `rev` + positive-OR `otherLock`, outer `expect(...,1)` | 9908 | 75.297491 |
| `rev` + positive-OR `otherLock`, outer `expect(...,0)` | 9872 | 73.451200 |
| `rev` + `!landmarkHit && notOtherLock && i != 3 && notTurretMode` | 9912 | 73.634409 |
| `rev` + `notOtherLock && !landmarkHit && i != 3 && notTurretMode` | 9908 | 74.465950 |
| `rev` + `!landmarkHit && notOtherLock && notTurretMode && i != 3` | 9924 | 71.522799 |
| `rev` + `i != 3 && !landmarkHit && notTurretMode && notOtherLock` | 9900 | 72.930713 |
| `rev` + `!landmarkHit && notTurretMode && notOtherLock && i != 3` | 9900 | 72.930713 |
| `rev` + `notTurretMode && !landmarkHit && notOtherLock && i != 3` | 9904 | 72.531573 |
| `rev` + `i != 3 && notTurretMode && !landmarkHit && notOtherLock` | 9920 | 72.479564 |
| `rev` + `notOtherLock && i != 3 && !landmarkHit && notTurretMode` | 9904 | 73.188926 |

The baseline order of the four-term `!landmarkHit && i != 3 &&
notTurretMode && notOtherLock` guard is already the best of the eight
permutations; the original's evaluation order (`!landmarkHit && notOtherLock`
first, then `i != 3 && notTurretMode`) is a register-pressure/rematerialisation
side effect, not a source-order difference.

## 4. Root cause still open

The remaining residual is unchanged: the first `otherLock` block is a `cbnz`
chain in the original but `cbz`+`itt` in ours (moving the first literal pool
from `12e340` to `12e310`); the planet campaign `isTarget` rematerialises
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

Section deltas vs the original: `.text` 85,441 differing bytes (size/base
exact), `.ARM.extab` 1,427, `.note.gnu.build-id` 20. `.rodata`, `.data`,
`.bss`, `.got`, `.ARM.exidx`, `.rel.plt`, `.rel.dyn`, `.dynsym`, `.dynstr`,
`.hash` are byte-identical.

## 6. Artifacts

Scratch in `/dev/shm/w7673` (not committed): `Radar.base.cpp` (pristine
source), `score.py` (single-symbol scorer mirroring `tools/verify/verify.py`),
`drv.py`/`batch1.py`–`batch6.py` (variant drivers), `orig.dis`/`ours.dis`
(full function disassembly), `show.txt`, `print.log` (MIR `-print-after-all`
dump). The analysis is self-contained in this note.
