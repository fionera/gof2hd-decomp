# Radar::draw wave 143 (w8906) — the String census is already identical; `int enemyHit` is the witnessed type

**Status:** source change landed, 2026-11-08. Worktree `wt/w8906`, based on `master`
(`f5093474f`, wave 143). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8613 B** (wave-143
merge-fix state). After this pass **8471 B** (-142). `.text` differing bytes
**85,851 -> 85,709** (-142). `.ARM.extab` 1,499 (unchanged). `.rel.plt` **0**
(byte-identical, 29,392 B). All 27 section sizes exact; `.rodata`/`.data`/`.bss`/
`.ARM.exidx` byte-identical. Gate: `RATCHET PASS`, byte **4507**, linked **4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

Read with `radar-draw-wave143-w8807.md`, `radar-draw-wave143-w8808.md`,
`radar-draw-wave141-w8610.md`.

## 1. Mission hypothesis: the "15 vs 11 String" census is stale

The w8807 note claimed our build has 15 `String::~String` vs the original's 11
(plus 8 vs 6 `operator=`, 4 vs 2 `String(char*,bool)`, 4 vs 2 `String(int)`,
1 vs 0 `SubString`, 1 vs 0 `operator+`). **Re-measured against the current
`master` binary, the census is identical:**

| call | original | ours (master) |
|---|---:|---:|
| `String::~String` (direct PLT) | 11 | 11 |
| `String::operator=` | 6 | 6 |
| `String::operator+` | 2 | 2 |
| `String::String(char*, bool)` | 1 | 1 |
| `String::String(int)` | 1 | 1 |
| `String::String(const String&, bool)` | 1 | 1 |
| `String::SubString` | 0 | 0 |
| `PaintCanvas::DrawString` | **9** | **8** |

The 11 destructors are 6 `lockLabel = calcDistance(...)` temporaries + 5
exception-cleanup calls; the five String temporaries in the landmark
`i == 0 && !alienOrbit` block (`String(": ")`, `operator+`, `String(tecLevel)`,
`String(String,bool)`, `operator+`) are destroyed through an indirect
GOT-loaded dtor thunk in **both** binaries (12eaba..12eaca orig, 12ea84..12ea94
ours), so they do not show up as `StringD1Ev` PLT calls. The w8807 counts were
taken on a different (pre-merge) build.

**The only real String delta is `DrawString` 9 vs 8.** Our clang tail-merges the
two `DrawString(radarFont(), this->lockLabel, ...)` calls of the landmark
`i <= 1` block into one shared call site (ours `12eafc`, reached by the else
branch's `b.n`); the original keeps two separate `blx` sites (12e9e0 else,
12eb36 `i == 0`). The original's `i == 0` block is placed out of line after the
loop increment (12ea2a); ours is inline at 12e9f4.

`__builtin_expect(i == 0 && !alienOrbit, 0)` does move the block cold, but it
gives **9900 B** (8 short) and raw **9095** — worse than the 9908-B baseline.
So the merge is not the lever.

## 2. Where the "60 B the bool-flag build is short" actually is

Measured on the wave-142 base (`13a941fca`, frame 296, raw 8106):

| variant | usize | frame |
|---|---:|---:|
| wave-142 base | 9908 | 296 |
| + `bool otherLockFlag` guard only (`v_g`) | **9848** | **296** |
| + `lockLabel` only (`v_l`) | 9908 | 296 |
| + campaign `expect(...,0)` only (`v_c`) | 9960 | 304 |
| - recompute only (`v_n`) | 9860 | 296 |
| faithful combo (`v_glcn`: guard+label+campaign+norecomp) | 9892 | 304 |

So the 60 B is the **whole bool-guard build** (`v_g` = 9848), and it is *not*
concentrated in the enemy/asteroid String handling. It is register-allocation
width: the original keeps `inSight` in `r6`, so every 0/1 materialisation is a
16-bit `movs r6,#imm`; ours keeps it in `fp`, so the same five sites become
32-bit `mov.w fp,#imm`:

```
orig 12ef4e movs r6,#0 ; strb r6,[r8,#115]      ours 12ef38 mov.w fp,#0 ; strb fp,[r8,#115]
orig 12ef96 movs r6,#0                          ours 12ef82 mov.w fp,#0
orig 12efb4 movs r6,#0                          ours 12efa2 mov.w fp,#0
orig 12efbe movs r6,#0                          ours 12efb0 mov.w fp,#0
orig 12efba movlt r6,#1                         ours 12efaa movlt.w fp,#1
```

That is 10 B per loop, plus the frame (+8 B: the campaign `expect` pushes the
working stack area from 116..156 to 124..164, two extra 4-byte slots). The
original's frame is 296; every faithful wave-143 shape has 304.

## 3. Landed change: `int enemyHit` (witnessed type)

```cpp
bool bossSeen = false;
int enemyHit = false;   // was bool
```

The original loads `enemyHit` as a **word**, not a byte:
`12f058 ldr r1,[sp,#152]; mvns r1,r1; tst r1,r0` (gas-cloud guard) and
`12fb30 ldr r1,[sp,#144]; orrs r0,r1` (asteroid-loop guard). A `bool` would
normally be a byte load. Spelling it `int` is therefore faithful and drops raw
**8613 -> 8471** at the same 9908 B.

## 4. Variant matrix (all on the wave-143 merge-fix base, size 9908 unless noted)

| variant | usize | raw |
|---|---:|---:|
| baseline (all-int padding) | 9908 | 8613 |
| **`int enemyHit` (landed)** | **9908** | **8471** |
| `bool slotCount` | 9908 | 8504 |
| `bool landmarkHit` + `bool enemy wreck` + `int asteroidHit` + `int asteroid wreck` | 9908 | 8579 |
| `bool notOtherLock` | 9908 | 9025 |
| `int dead` (enemy) / `int inSight` / `int gasHit` / `int hasDockNav` / `int candidateWreck` / `int navWreck` / `int dead` (asteroid) | 9908 | 8613 (no change) |
| `int enemyHit` + any of the above | 9908 | 8471 (masked) |
| `bool enemyHit` + `bool landmarkHit` + `bool enemy wreck` + `bool slotCount` | 9908 | 8645 |
| wave-142 base (invented recompute, int guard, local label) | 9908 | 8106 |

The wave-142 base still has the lowest raw of any 9908-B shape, but it carries
the invented post-loop recompute and the `int otherLock` guard that w8807/w8808
were landed to fix. The landed `int enemyHit` keeps every witnessed wave-143
change and is a strict improvement on both raw and faithfulness.

## 5. Open questions

* The `DrawString` tail merge (9 vs 8) is still unfixed. Preventing it without
  the 8 B size loss needs a faithful source shape that makes the `i == 0`
  block cold *and* keeps 9908 B; `__builtin_expect(...,0)` alone gives 9900.
* The frame is 304 vs the original 296. The campaign `expect(...,0)` is the
  sole cause (removing it: 9896 B, frame 296, raw 9126). A source-order or
  inner-`expect` shape that gets the cold placement without the extra two
  4-byte slots would likely collapse a large part of the remaining 8471.
* `inSight` is in `fp` (ours) vs `r6` (original). The 16-vs-32-bit `mov`
  materialisation accounts for ~10 B per enemy/asteroid loop; matching the
  original's register pressure is the remaining structural lead.

## 6. Gate

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,709** differing bytes (size/base
exact 1,259,464), `.ARM.extab` **1,499** (size exact 156,312), `.rel.plt` **0**
(byte-identical, 29,392 B), `.note.gnu.build-id` 20. `.rodata`, `.data`,
`.bss`, `.got`, `.ARM.exidx`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are
byte-identical. All 27 section sizes exact.
