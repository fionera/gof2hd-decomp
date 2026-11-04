# Residual taxonomy of the 24 non-exact functions — wave 111 / w5920

Status: **research note only, no `src/` change**. Worktree `wt/w5920`, branch
`wave111-w5920`, base = current master (`8eea060cc`, the wave-110 harvest).
Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra − 0x10000).

This note answers the wave-111 question: **classify every remaining diff byte
by cause**, then give a prioritized attack plan (one highest-leverage source
change per function) and flag shared causes.

The 24 non-exact symbols are **20 unique bodies** (four C1/C2 alias pairs:
`Layout`, `Globals`, `MenuTouchWindow`, `ModStation`).  All 20 are
**size-exact**; the entire `.text` residual is 95,999 bytes, and the
per-function byte counts below sum to exactly 95,999.

---

## 0. TL;DR

| cause | bytes | share of 95,999 |
|---|---:|---:|
| link-address / literal-pool slot (`raw-only`, masked by the fuzzy metric) | 41,842 | **43.6 %** |
| block placement (MachineBlockPlacement / block order) | 24,306 | **25.3 %** |
| immediate / operand difference | 14,602 | **15.2 %** |
| register allocation | 10,716 | **11.2 %** |
| structural source shape (different instruction) | 2,304 | 2.4 % |
| inserted instruction (ours only) | 1,144 | 1.2 % |
| instruction scheduling (in-block reorder) | 1,085 | 1.1 % |

* **Only 2.4 % of the differing `.text` bytes are a structurally different
  instruction** (plus 1.2 % ours-only inserts). The rest is ordering, register
  choice, operand encoding, or a link-address pool word that the fuzzy metric
  already masks.
* The single largest *fixable* cause is **block placement** (25.3 %), almost
  all of it in `LevelScript::process` (20,895 B) and `MGame::OnTouchEnd`
  (1,482 B).
* The largest *unfixable-by-source* cause is **`raw-only`** (43.6 %): literal
  pool words holding absolute/GOT addresses that differ between any two links.
  It is invisible to the fuzzy score; only pool *slot order* is source-shaped.
* **LSDA/.ARM.extab is a single shared cause**: only four functions have a
  record-size delta (`LevelScript +4`, `Hud +4`, `createShip −8`,
  `Generator −4`; net **−4** = the section shortfall).  Everything else in
  `.ARM.extab`/`.ARM.exidx` differs only because those four records shift the
  section by 4 bytes (plus one content-only delta in the byte-exact
  `PaintCanvas::DrawStringColor` LSDA, see §7).
* **Two cross-function shared causes are real source levers**:
  (a) `Globals::Canvas` is in `r5` in our build but `fp` in the original —
  shared by `HangarWindow::render` and `Hud::draw`;
  (b) `this`/`status`/loop-counter register assignment is the same greedy-RA
  decision in `LevelScript::process`, `PlayerEgo::calcCollision`,
  `MenuTouchWindow` and `SpaceLounge`.

---

## 1. Exact commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w5920
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Baseline (verified this pass):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint:   CLEAN
drift:  0 field(s) across 0 class(es)
```

Per-function side-by-side disassembly:

```sh
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump "$OBJ" \
  --show _ZN11LevelScript7processEi
```

The taxonomy was produced by importing `tools/verify/verify.py` itself
(`disassemble` + `reconcile`), so the instruction streams are byte-for-byte
the ones the official scorer uses.  Scratch harness:
`/dev/shm/w5920/{regions,tax2,tax3,finaltax}.py`.

---

## 2. Structural state (wave 110, unchanged)

```text
sections            27 (exact; original 27)
file size           2,187,328 B (exact)
.text               0x1337c8 @ 0x69440 (size/base exact)
.ARM.exidx          0x6f70   @ 0x19cc08 (size/base exact)
.ARM.extab          0x26298 orig / 0x26294 ours @ 0x1a3b78 (4 B short)
.rodata             0x41b67 orig / 0x41c27 ours @ 0x1c9e10 (192 B rank table appended)
differing bytes     .text 95,999 · .ARM.exidx 1,683 · .ARM.extab 128,946 · build-id 20
```

No section/extab byte changed during this pass (no `src/` change).

---

## 3. Taxonomy definition (how each byte was assigned)

For each function:

1. Disassemble both bodies with the verifier's exact pipeline
   (`verify.disassemble` + `verify.reconcile`).  All 20 bodies are the same
   size, so function-relative byte offsets are comparable.
2. Compute the LCS (`difflib.SequenceMatcher(..., autojunk=False)`) of the
   normalized instruction streams.  LCS-matched instructions are the
   in-order common stream.
3. On the unmatched instructions, pair in this order:
   * **exact-text reorder** — same normalized text; distance ≤ 20
     instructions → `scheduling`, > 20 → `block-placement`;
   * **same shape after canonicalising register names** → `register-alloc`;
   * **same mnemonic** → `imm/operand`;
   * remaining orig-only → `structural`; remaining ours-only → `insert`.
4. Assign each differing byte to the category of the instruction that
   contains it (orig side preferred; ours side for inserts; `raw-only` when
   both sides are LCS-matched).  The category byte counts sum exactly to the
   function's raw diff-byte count.

Caveats (stated honestly):

* `imm/operand` is a *residual pairing bucket*.  In heavily reordered
  functions (`LevelScript`, `Layout`, `Radar`) a block move can make a
  same-mnemonic/different-operand pair that is really a block-placement
  artefact.  Treat `imm/operand` as "same opcode, different operand" rather
  than "different source constant".
* `raw-only` is not a source-shape bug: it is a literal-pool word or a
  PC-relative address that differs between two independent links.  The fuzzy
  metric masks it (`pool:addr` / `#A`); it only matters for `byte_exact`.

---

## 4. Per-function taxonomy

`ratio` is the unrounded verifier ratio (`difflib` on the reconciled stream).
`extab` is the LSDA record-size delta (ours − orig) from the `.ARM.exidx`
table.  `RA` = register allocation, `block` = block placement, `sched` =
in-block scheduling, `imm/op` = immediate/operand, `struct` = structural,
`ins` = ours-only insert.

| function | size | diff B | ratio | raw-only | RA | block | sched | imm/op | struct | ins | extab |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `Layout::Layout` C1/C2 | 7072 | 5862 | 48.34 | 1004 | 1648 | 860 | 103 | 1440 | 602 | 205 | 0 |
| `LevelScript::process` | 57232 | 54637 | 56.09 | 18251 | 5729 | 20895 | 198 | 8004 | 1018 | 542 | **+4** |
| `Radar::draw` | 9908 | 7610 | 74.03 | 4096 | 703 | 521 | 137 | 1797 | 211 | 145 | 0 |
| `Level::createShip` | 2764 | 1203 | 74.38 | 357 | 409 | 51 | 59 | 217 | 80 | 30 | **−8** |
| `MGame::OnTouchEnd` | 8080 | 3977 | 78.92 | 1572 | 31 | 1482 | 104 | 770 | 14 | 4 | 0 |
| `Globals::Globals` C1/C2 | 612 | 405 | 80.53 | 241 | 75 | 20 | 47 | 9 | 8 | 5 | 0 |
| `Generator::createMission` | 1996 | 930 | 82.62 | 522 | 62 | 47 | 53 | 200 | 31 | 15 | **−4** |
| `Status::missionCompleted` | 1432 | 1024 | 86.97 | 817 | 0 | 10 | 0 | 171 | 16 | 10 | 0 |
| `PlayerEgo::calcCollision` | 968 | 602 | 87.64 | 426 | 9 | 12 | 0 | 105 | 30 | 20 | 0 |
| `Level::createScene` | 2252 | 454 | 88.31 | 173 | 120 | 15 | 8 | 89 | 41 | 8 | 0 |
| `HangarWindow::render` | 9700 | 4842 | 88.71 | 3346 | 551 | 132 | 121 | 616 | 46 | 30 | 0 |
| `Gun::calcCharacterCollision` | 1984 | 668 | 90.00 | 424 | 67 | 20 | 14 | 116 | 14 | 13 | 0 |
| `Level::createMission` | 17172 | 5881 | 92.09 | 4124 | 847 | 209 | 89 | 467 | 87 | 58 | 0 |
| `RocketGun::update` | 1908 | 746 | 93.78 | 612 | 64 | 7 | 13 | 35 | 9 | 6 | 0 |
| `Hud::draw` | 9332 | 2432 | 94.57 | 1790 | 174 | 17 | 58 | 302 | 57 | 34 | **+4** |
| `SpaceLounge::drawLounge` | 1948 | 178 | 95.54 | 97 | 31 | 0 | 13 | 20 | 11 | 6 | 0 |
| `MenuTouchWindow` C1/C2 | 9600 | 4436 | 95.69 | 3929 | 176 | 8 | 45 | 242 | 25 | 11 | 0 |
| `MenuTouchWindow::OnTouchBegin` | 1896 | 29 | 98.09 | 10 | 19 | 0 | 0 | 0 | 0 | 0 | 0 |
| `ModStation` C1/C2 | 1008 | 42 | 98.63 | 17 | 0 | 0 | 23 | 2 | 0 | 0 | 0 |
| `MGame::OnUpdate` | 11440 | 41 | 99.94 | 34 | 1 | 0 | 0 | 0 | 4 | 2 | 0 |
| **total (unique)** | | **95,999** | | **41,842** | **10,716** | **24,306** | **1,085** | **14,602** | **2,304** | **1,144** | **−4** |

---

## 5. Aggregate reading

* **`raw-only` 43.6 %** — the fuzzy metric already ignores these bytes.  For
  `HangarWindow`, `MenuTouchWindow`, `Status`, `RocketGun` and
  `createMission`, more than half of the raw diff is this category.  The only
  source-shaped part is the *slot order* of the literal pool (LDRLIT emission
  follows source expression order); the values themselves are link addresses.
* **`block-placement` 25.3 %** — dominated by two functions:
  `LevelScript::process` (20,895 B, 10,752 instructions) and
  `MGame::OnTouchEnd` (1,482 B, 654 instructions).  This is the single
  largest *fixable* cause in the fleet.
* **`register-alloc` 11.2 %** — spread over all 20, largest in `Layout`
  (1,648 B), `LevelScript` (5,729 B), `createMission` (847 B), `Radar`
  (703 B), `Hangar` (551 B), `createShip` (409 B).  It is a greedy-RA
  decision, not a source semantic.
* **`imm/operand` 15.2 %** — partly real (different pool offsets, stack slots,
  `movw` constants) and partly a residual pairing artefact of block moves.
* **`structural` 2.4 %** — the genuinely different instructions.  The largest
  are `Layout` (602 B, SLP/`stmia` grouping), `LevelScript` (1,018 B, state
  machine), `Radar` (211 B, call/`cbnz` shape), `createMission` (87 B,
  convoy/`cbz`), `createShip` (80 B, `strd`/`vmov` spawn shape).

---

## 6. Prioritized attack plan (one highest-leverage change per function)

Priority is by expected *fuzzy* gain per unit of source risk, not by current
score.  "Structural" means it mainly improves `byte_exact`/section exactness.

### P0 — largest fixable residuals

**1. `LevelScript::process` (56.09 %, 57,232 B) — block placement.**
The 925-instruction mission-0 state-machine block at original
`0x13cf64..0x13dd52` has no aligned counterpart; the case-91/case-125 cold
blocks are the reason for the +4 LSDA rows.  Highest-leverage single change:
reproduce the original's **separate-`if` mission-0 state machine** (not the
`else if` chain) so the state-0 body is inline and rows 13–19 fall below
offset 16384.  The known faithful shape is 57,208 B / 55.43 % (size −24), so
it must be paired with a size-exact completion before landing.  This is the
single biggest lever in the fleet (20,895 B of block placement).

**2. `MGame::OnTouchEnd` (78.92 %, 8,080 B) — block placement.**
The whole 1,482 B block residual is the `hudTouchFlags == 0` campaign-chain
fall-through.  Highest-leverage change: reorder the campaign-chain
`if`/`else` so the original's cold block is the fall-through.  Waves 107/109
found no size-preserving source shape; the next attempt should start from the
original block order at `0x17a...` rather than statement-level swaps.

**3. `Layout::Layout` (48.34 %, 7,072 B) — SLP grouping / RA.**
The original's outer `if (Globals::iPadHD)` is a **duplicated HD/non-HD
block** (42 SLP groups vs our 50).  Highest-leverage change: reconstruct that
nested structure, not more field-order hill-climbing.  Secondary:
`field_0x1e4` is a `float` `vstr` in the original but `int` in our header
(requires a `Hud.cpp` read-site change).

**4. `Radar::draw` (74.03 %, 9,908 B) — structural call shape + block.**
The baseline has a duplicate post-loop recompute (352 calls vs 342).
Highest-leverage change: replace it with the faithful planet tail
(`nodup` + 6-call asteroid predicate + unmerged landmark else-branch
`DrawString`); the tail must be the original's ~0x5e B inline planet tail,
not the 0x52 B duplicate.

**5. `Level::createShip` (74.38 %, 2,764 B) — LSDA row + spawn shape.**
Highest-leverage *structural* change: add the one +4 LSDA row
(`q < 128` and no throwing call in `[q,next)`) to move the record 328 → 332;
this is the only safe way to make `.ARM.extab` exact (+4 net).  For the fuzzy
score the residual is RA (409 B) + `imm/op` (217 B), i.e. the tuned spawn
shape is already the local optimum.

### P1 — medium residuals

**6. `Generator::createMission` (82.62 %, 1,996 B) — arm block layout.**
Highest-leverage change: swap the pick/type arms to the original order
(the wave-110 note's remaining 17.4 % is the pick/type arm block layout).
Also its −4 LSDA record is one of the four extab deltas.

**7. `Status::missionCompleted` (86.97 %, 1,432 B) — table-free switch.**
Highest-leverage change: find a table-free `switch (m->getType())` whose
block order matches the original's `167/174, 150..155,158,162,166,170,184,
156,185,157,160,163,164,165,168,169,171,172`.  The current 192-byte rank
table is a workaround; the best table-free exact-order shape is 1,472 B /
76.4 %, so this is a structural trade, not a score win.

**8. `PlayerEgo::calcCollision` (87.64 %, 968 B) — `i`/`fp` pair.**
Highest-leverage change: make the allocator keep `i` in `fp` and `sl` as the
scratch (the landed two-anchor body is the unique 87.64 % point).  The
faithful `le/le` + real-guard shape is 85.67 % at 984 B; no source shape
tested reaches the original pair.

**9. `Level::createScene` (88.31 %, 2,252 B) — rnd mask / sink.**
Highest-leverage change: `pick = nextInt(3)` must use `Globals::rnd` and one
of `race = nextInt(4)` / `nextInt(100) < 30` must use the local `rnd`
(mask 35/37).  With the whole-TU `-sink-insts-to-avoid-spills` flag this is
93.3 %, but the flag alone collapses `byte_exact` 4500 → 2357, so it must be
a source-only shape.

**10. `HangarWindow::render` (88.71 %, 9,700 B) — `Canvas` in `fp`.**
Highest-leverage change: move the `Globals::Canvas` GOT pointer from `r5` to
`fp` (same shape as `Hud::draw`, see §7).  Secondary: the docking `String`
`operator+` schedule and the `field_0x279` `r8` cache.

**11. `Gun::calcCharacterCollision` (90.00 %, 1,984 B) — shared `1` constant.**
Highest-leverage change: shorten the constant-1 live range (tail
`F P H L K`) so `i3 = r4` and the join reload is in `fp`.  The two decisions
are driven by the same greedy priority order; every tested tail is ≤ 88 %.

**12. `Level::createMission` (92.09 %, 17,172 B) — alienGuns select.**
Highest-leverage change: reproduce the original's real in-loop branch for the
alienGuns table (`bigAlienWave` selects `kAlienStationTurrets` vs
`kAlienOutpostTurrets`), not the CMOV.  The `(bigAlienWave + (i < 0))` anchor
keeps it in-loop; the convoy if-chain is the second region.

**13. `RocketGun::update` (93.78 %, 1,908 B) — r4/r5 split.**
Highest-leverage change: alter the `elapsed` use pattern so greedy splits the
live range and keeps `elapsed` in `r4` (the original's `add.w r8, sp, #116`
is a consequence of that).  Statement reordering has been exhausted.

**14. `Hud::draw` (94.57 %, 9,332 B) — `Canvas` in `fp` + `String` schedule.**
Highest-leverage change: the same `Canvas`-pointer move as `HangarWindow`;
secondary is moving the docking `String` `operator+` after the
`progress`/`barOffset` float math.  Its +4 LSDA record is one of the four
extab deltas.

### P2 — near-exact (≤ 2 % residual)

**15. `SpaceLounge::drawLounge` (95.54 %, 1,948 B) — split `x` at the join.**
Highest-leverage change: make `x`'s pre-join piece outrank `%46` while
`%46/%47/%45` avoid `r9`, so `pad → r9` and `x → r9` after the join.  Not
reachable from any C++ shape tested in waves 106–110.

**16. `MenuTouchWindow` C1/C2 (95.69 %, 9,600 B) — `cmp #10; bcc`.**
Highest-leverage change: a source shape that emits `cmp #10; bcc` without
clang using the `[0,9]` range fact for the `special` select.  `i < 10` and
every strong-range spelling change the select to an address select and shift
the frame by 8 bytes (9,548 B / 72.06 %).

**17. `MenuTouchWindow::OnTouchBegin` (98.09 %, 1,896 B) — `r6` value cache.**
Highest-leverage change: the all-direct case-15 shape (right pre-RA IR) but
greedy keeps `&Globals::layout` in `fp`.  The `ip`/`r4` case-3 difference is
a pure tie-break (`r4` is dead in both binaries).

**18. `ModStation` C1/C2 (98.63 %, 1,008 B) — pool order.**
Highest-leverage change: the wave-106 `__atomic_signal_fence` placement after
the `yawTable` select already fixed the `{Pitch, YawIPad, Yaw}` pool order.
The remaining 42 B is one `ldr [rX,#636]` vs `[rX,#628]` operand and the
resulting pool-offset shift.

**19. `MGame::OnUpdate` (99.94 %, 11,440 B) — m23 `Status*` cache.**
Highest-leverage change: the no-`st` shape (right pre-RA IR) but greedy
assigns the compact split piece `$r5`, not `$r9`.  Every `st` placement that
keeps `r9` costs a 2-byte `mov`; the baseline is the unique byte-neutral
placement.

**20. `Globals::Globals` C1/C2 (80.53 %, 612 B) — pool order.**
Highest-leverage change: variant B (`recordHandler` last in the early pointer
block, `status` in the late block) reproduces the original's 31-word pool
order exactly but costs 1.7 points (78.8 %).  Keep the baseline unless the
goal is pool order rather than fuzzy score.

---

## 7. Shared causes (cross-function leverage)

### 7.1 `Globals::Canvas` in `r5` vs `fp` — `HangarWindow::render` + `Hud::draw`

Both functions load `&Globals::Canvas` and keep it in a callee-saved register
across long live ranges.  Our build picks `r5`; the original picks `fp`.
This is the same greedy-RA decision in both functions.  A single source-shape
change that lengthens/shortens the `Canvas` live range (e.g. obtaining the
pointer at a different point, or using a local `PaintCanvas *canvas` with a
different first use) is the one lever most likely to move both functions at
once.  It is not proven that one spelling fixes both; the live-range
neighbourhoods differ.

### 7.2 `this` / `status` / loop-counter register file — four functions

The original keeps `this` in `sl` and reloads from `[sp,#88]` in
`LevelScript`; `status` is `r9` in ours and `r4` in the original.  The same
kind of split (`i` in `fp` vs `sl`) is the entire residual of
`PlayerEgo::calcCollision` and `MenuTouchWindow`, and the `pad`/`x` split in
`SpaceLounge`.  These are all greedy-RA tie-breaks driven by live-range
weight, not by source semantics.  They are *shared in mechanism* but not in
a single edit.

### 7.3 LSDA / `.ARM.extab` — four functions, one section delta

Only four functions have a record-size delta:

| function | orig record | ours | delta |
|---|---:|---:|---:|
| `LevelScript::process` | 348 | 352 | **+4** |
| `Hud::draw` | 476 | 480 | **+4** |
| `Level::createShip` | 336 | 328 | **−8** |
| `Generator::createMission` | 172 | 168 | **−4** |
| **net** | | | **−4** |

The net −4 is exactly the `.ARM.extab` shortfall (`0x26294` vs `0x26298`).
Because `.ARM.extab` precedes `.data.rel.ro`/`.dynamic`/`.got`/`.rel.dyn`,
**any delta other than +4 shifts later sections and collapses `byte_exact`**
(wave-110 createShip note).  The safe target is therefore exactly **+4**:
`createShip` 328 → 332 is the cleanest single change (the wave-110 note gives
the row arithmetic: `q < 128` and no throwing call in `[q,next)`).

Everything else in `.ARM.extab` (128,946 differing bytes) and `.ARM.exidx`
(1,683 bytes) is a **shift artefact** of those four records.  Fixing the net
+4 would make `.ARM.extab` size-exact and remove the global exidx shift, even
though the four functions would still be non-exact in `.text`.

**New finding this pass:** the *first* differing `.ARM.extab` byte is at
`0x1a507e`, inside the record of the **byte-exact**
`AbyssEngine::PaintCanvas::DrawStringColor` (record `0x1a504c`, same size
60 B in both).  Its call-site table differs:

```text
orig:  0x78274 +78 lp=0 ; ... ; 0x78374 +8 lp=0x783a8 ; 0x7837c +88 lp=0
ours:  0x78274 +78 lp=0 ; ... ; 0x78374 +14 lp=0x783a8 ; 0x78382 +82 lp=0
```

i.e. the boundary between two adjacent call-site ranges moved by 6 bytes
even though the function bytes are identical.  This is a **content-only LSDA
delta in a byte-exact function**, independent of the four size deltas.  It
means `.ARM.extab` cannot become byte-identical by fixing only the four
non-exact functions; the `DrawStringColor` invoke-scope shape must also be
reconstructed (a source-level `try`/cleanup scope that does not change
codegen).

### 7.4 Literal-pool slot order — all functions with `raw-only` bytes

The `raw-only` bytes are pool words.  The *order* of pool slots follows the
backend's LDRLIT emission order, which follows the source expression order
(not the declaration order; see the ModStation wave-106 note).  This is a
shared mechanism but a per-function edit.  It is the main reason a
"faithful" source shape can score lower than a tuned one: moving a statement
to fix the pool order can change the RA/schedule.

---

## 8. Proven vs hypothesis

**Proven (measured from the built ELF and the verifier's own streams):**

* The 24 non-exact symbols are 20 unique bodies; all are size-exact.
* The per-function byte taxonomy in §4 sums to exactly the 95,999 differing
  `.text` bytes.
* The aggregate shares in §0 are exact for this build.
* `.ARM.extab` is 4 B short; the only four record-size deltas are
  `LevelScript +4`, `Hud +4`, `createShip −8`, `Generator −4` (net −4).
* The first `.ARM.extab` content difference is in the byte-exact
  `PaintCanvas::DrawStringColor` LSDA, not in any of the 24.
* `Globals::Canvas` is in `r5` (ours) vs `fp` (original) in both
  `HangarWindow::render` and `Hud::draw` (from the wave-109/110 notes and
  re-checked in the current disassembly).
* The fuzzy metric ignores `raw-only` pool words: 43.6 % of the raw `.text`
  diff does not affect the score.

**Hypothesis (not proven):**

* The `imm/operand` bucket overstates real constant differences; part of it
  is block-placement mispairing.  A block-aware aligner (not difflib LCS)
  would move some of the 15.2 % into `block-placement`.
* The `Canvas`-pointer move is the most promising *single* cross-function
  source lever, but no spelling has been measured that moves it to `fp`
  without changing the instruction count.
* The `DrawStringColor` LSDA delta is a source `try`/cleanup scope
  difference; a code-neutral scope reconstruction is possible in principle
  but has not been attempted.
* `LevelScript`'s 20,895 B block-placement residual is the largest single
  fixable cause, but the faithful separate-`if` shape is 24 B short; a
  size-exact completion has not been found.

---

## 9. What was *not* changed

No `src/` file was touched.  `tools/verify_baseline.json`, `report.json`,
`docs/` and the section layout are untouched.  The worktree is clean apart
from this note.

Scratch (not committed): `/dev/shm/w5920/`
(`regions.py`, `tax2.py`, `tax3.py`, `finaltax.py`, `tax.json`, `final.json`,
`analyze.py`, `byteclass.py`, `taxonomy.py`, `topregions.py`).
