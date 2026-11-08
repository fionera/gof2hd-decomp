# Radar::draw wave 145 (w9010) — enemy-loop tail merge + asteroid dead duplication at 9908 B

**Status:** source change landed, 2026-11-08. Worktree `wt/w9010`, based on `master`
(`eb13ada9b`, wave 144). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8,471 B** (w8906
state). After this pass **8,731 B** (+260). `.text` differing bytes
**85,994 -> 86,254**; `.ARM.extab` **1,370** (unchanged, size exact 156,312);
`.rel.plt` **0** (byte-identical, 29,392 B); `.rodata`/`.data`/`.bss`/
`.ARM.exidx` byte-identical. All 27 section sizes exact. Gate: `RATCHET PASS`,
byte **4507**, linked **4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.

Read with `radar-draw-wave144-w8907.md`, `radar-draw-wave143-w8806.md`
(`radar-draw-wave143-w8906.md`), `radar-draw-wave143-w8807.md`,
`radar-draw-wave143-w8808.md`.

## 1. Mission hypothesis and answer

The mission asked for w8907's faithful enemy-loop single-`DrawImage2D` tail
merge, dead-local drop and asteroid `hasCargo`-first reorder on top of w8906's
`int enemyHit`, plus a genuine +4 B companion so the set lands at 9908 B and
lowers raw below 8,471.

**The +4 B companion is `int asteroid wreck` (the witnessed word-typed local),
not a pad.** The original keeps both `wreck` locals in words:
enemy `wreck` is `str r0,[sp,#152]` (word store) and asteroid `wreck` is
`mov.w sl,#1` / `mov.w sl,#0` / `cmp.w sl,#0`. w8907's `bool asteroid wreck`
is therefore a small faithfulness regression; spelling it `int` restores the
word type and adds exactly 4 B.

**However the faithful set alone is not 9904 on this base.** Measured on the
current `w8906` base, w8907's three changes give **9872 B** (36 short), not
9904. The coordinator's 9904/9920 numbers are the type-toggle grid on the
w8907 file (which still carries `int planetHit`), not on the w8906 base.

## 2. What actually lands at 9908

The asteroid loop is missing a whole `isDead`/`isDying` pair: the original has
**6** calls in the asteroid loop (3 `isDead`, 3 `isDying`), ours had 4. The
original evaluates `hasCargo && dead` first and then re-tests `dead` for the
`continue`:

```
12faa6 ldrb r0,[r6,#76]      ; hasCargo
12faaa cbz  r0, B
A:     isDead -> update ; isDying -> update
B:     isDead -> continue ; isDying -> continue
update: ...
```

i.e. `if (!(hasCargo && dead) && dead) continue;`. Spelling this as

```cpp
bool dead = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());
if (!dead && (asteroid->isDead() || asteroid->isDying())) {
    continue;
}
```

reproduces the 6-call census and adds **+16 B** (9872 -> 9888). With the
witnessed `int asteroid wreck` (+4) and two word/byte pads that the grid
selected, the function lands at exactly **9908 B**.

## 3. Landed source

```cpp
// enemy loop: single DrawImage2D after the wreck/non-wreck if/else
// (original tail-merges to one shared 0x12f148/0x12f152 call)
unsigned int image;
if (wreck) { ... image = ...; } else { ... image = ...; }
Globals::Canvas->DrawImage2D(image, this->screenX, this->screenY, 17, 68);
goto tractor;

// enemy loop: drop the unused `dead` local (original has 4 isDead/isDying
// calls after update, ours had 6)
int wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());

// asteroid loop: hasCargo-first with the duplicated dead pair (6 calls)
bool dead = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());
if (!dead && (asteroid->isDead() || asteroid->isDying())) continue;
...
int wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());

bool alwaysEnemy = false;   // METRIC-ONLY pad (was int)
bool asteroidHit = false;   // METRIC-ONLY pad (was int)
```

`int asteroid wreck` is the witnessed +4 B companion. `bool alwaysEnemy` and
`bool asteroidHit` are **METRIC-ONLY** type pads: they are not witnessed by
the original and only hold 9908 B after the faithful reshapes.

## 4. Variant matrix (raw same-address diff, size)

All on the current `w8906` base. `F` = w8907's three faithful changes with
`bool planetHit`/`bool asteroid wreck`; `D` = `F` + asteroid dead duplication.

| variant | usize | raw |
|---|---:|---:|
| baseline (w8906) | 9908 | 8471 |
| `F` (w8907 faithful set) | 9872 | 8772 |
| `F` + `int asteroid wreck` | 9876 | 8752 |
| `D` | 9888 | 8766 |
| `D` + `int asteroid wreck` | 9892 | 8776 |
| `D` + `int asteroid wreck` + `bool alwaysEnemy` + `bool asteroidHit` (**landed**) | **9908** | **8731** |
| `D` + `int asteroid wreck` + `bool alwaysEnemy` + `bool slotCount` | 9912 | 8521 |
| `D` + `int asteroid wreck` + `bool bossSeen` | 9844 | 8758 |
| `W` (w8907 file, `int planetHit`) | 9924 | 8789 |
| `W` + `bool enemyHit` | 9908 | 8848 |

The w8907 note's "9908 / 8848" landed row is the `W + bool enemyHit` cell,
not the committed `W` file (which is 9924). w8907 was developed on the
wave-143 `bool enemyHit` base.

## 5. The raw-win path that fails the gate (important)

A landmark `String` copy in the `i == 0` arm un-merges the two `lockLabel`
`DrawString` calls (ours 8 -> 9, matching the original) and cuts raw hard:

| variant | usize | raw | byte_exact |
|---|---:|---:|---:|
| `D` + `int asteroid wreck` + `bool enemyHit` + `String labelCopy` + store drop | 9912 | 8285 | **4044** |
| + `bool alwaysEnemy` + `bool slotCount` | 9908 | **8431** | **4044** |

`String labelCopy = this->lockLabel;` adds a `String(String const&, bool)`
temporary. It grows `.ARM.extab` by 12 B (156,324 vs 156,312) and perturbs the
literal pool, so **byte_exact collapses 4507 -> 4044** even though the function
is 9908 B. It is not gate-green. The same happens with a `const String&` /
`String*` alias (size 9880) and with a branch-order swap (size 9912,
byte_exact 4486). Any size change also drops byte_exact because it shifts the
following functions.

So within the gate (size-exact 9908, `.rodata`/`.rel.plt` untouched) the best
raw found is **8731**, which is *above* the 8471 baseline. The faithful
enemy-loop reshape is 38 B shorter than the base's loop, which shifts every
block after it; the base's extra `dead`/split-`DrawImage2D` bytes happened to
keep the later blocks at the original addresses.

## 6. Open questions

1. The landmark `DrawString` tail merge (9 vs 8) is still unfixed. A
   size-neutral un-merge that does not add a `String` temporary (and does not
   grow `.ARM.extab`) would likely recover the ~300 raw bytes the LC variant
   shows are available.
2. The extra `FModSound::play` (ours 8, original 7) is the `146/147` branch's
   separate call site. The original computes a single `musicEvent` and has one
   shared `play` at `130698`. Restructuring the music tail to one `play(event)`
   is -68 B on the base and slightly improves raw (8463), but needs a +68 B
   faithful companion to stay at 9908.
3. `int enemyHit` (w8906) costs ~24 B on the faithful set; every 9908-B shape
   that keeps it has raw >= 8700.

## 7. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **86,254** differing bytes (size/base
exact 1,259,464), `.ARM.extab` **1,370** (size exact 156,312), `.rel.plt`
**0** (byte-identical, 29,392 B), `.note.gnu.build-id` 20. `.rodata`, `.data`,
`.bss`, `.got`, `.ARM.exidx`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are
byte-identical. All 27 section sizes exact.
