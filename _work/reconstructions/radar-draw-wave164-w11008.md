# Radar::draw wave 164 (w11008) — `expect` on the `field_0x1bc` sound ladder

**Status:** source change landed, 2026-11-08. Worktree `wt/w11008`, based on
`master` (`b3d3a1f68`, wave 163). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave163-w10908.md` and `wave163-merge-tests.md`.

## 0. The wave-163 claim DOES reproduce

`wave163-merge-tests.md` says w10908 "claimed 7,584 but measured 8,124" and
was reverted.  Re-applying `git show aab4acfad -- src/game/weapons/Radar.cpp`
on `b3d3a1f68` and rebuilding measures **raw 7,584 / aligned 1,420 /
9,908 B** exactly.  The merge note is wrong (stale/incorrect measurement,
not a real non-reproduction).  The wave-163 commit is a genuine -540 raw
win; this pass starts from it.

## 1. Sweep: the `field_0x1bc` sound ladder

The original's sound selection is a three-arm ladder:

```
if (field_0x1bc >= 1) { ... }        // hot
else { ... ambient ... }             // cold
...
if (field_0x1bc < 3) ... else if (field_0x1bc < 5) ... else ...
```

Adding `__builtin_expect(..., 1)` to the two sub-guards (`< 3` and `< 5`)
keeps their bodies as fall-through and pushes the `else` arms out of line.
Measured on top of the wave-163 shape (all 9,908 B exact unless noted):

| variant | raw | aligned | `.text` |
|---|---:|---:|---:|
| wave-163 shape (`expect(>=1,1)` only) | 7,584 | 1,420 | 84,112 |
| + `expect(< 3, 1)` only | 7,586 | 1,451 | 84,114 |
| + `expect(< 5, 1)` only | 7,598 | 1,441 | 84,126 |
| **+ both `< 3` and `< 5`** | **7,578** | **1,428** | **84,106** |
| both, no `expect(>=1,1)` | 8,277 | 1,296 | 387,875 (size 9,904) |
| both, `int alwaysEnemySeen`/`int alwaysEnemy` | 8,409 | 1,451 | 392,207 (size 9,892) |
| both, `int alwaysEnemySeen` only | 8,467 | 1,416 | 388,073 (size 9,900) |

The two sub-guard expects are a small but real additional win (-6 raw,
-6 `.text`).  Dropping `expect(>=1,1)` recovers the aligned metric
(1,296 < 1,320) but costs raw (8,277 > 8,124), so it is not a win.

## 2. The faithful 4 B tail

With the three expects the tail is the original's **faithful** tail:

```cpp
    this->field_0x1bc = 0;
}
```

and the wave-163 16 B filler companion (`plasmaInRange = 0;`,
`field_0x28 = 0;`, a dead `getCurrentCampaignMission()`) is no longer
needed: the function is exactly 9,908 B.  The remaining 4 B code deficit
is in the exception-cleanup landing pads at the end of the function
(`blx _Unwind_Resume` is at `0x1306e8` vs the original `0x1306ea`; the
literal pool is 4 B larger to compensate).  It comes from the `String`
temporaries in the landmark DrawString expression
(`*getText(133) + String(": ") + String(String(...), false)`); every
source reshaping tried there (parenthesised, explicit copy, reversed
order, no inner `String`) was much worse (raw 8,562-8,658), so it was
left alone.

## 3. Other levers swept (no win at 9,908 B)

* `expect` on `current == 143` (0), `alwaysEnemySeen` (0), `bossSeen` (0),
  `inAlienOrbit` (0), `current != 151` (1): identical or worse.
* `int` variants of every `bool` local (`missionBlocksPlanets`,
  `turretMode`, `landmarkHit`, `planetHit`, `marked`, `bossSeen`,
  `enemyHit`, `gasHit`, `asteroidHit`, ...): all worse.
* Planet guard (`!missionBlocksPlanets && planetLockTimer >= 1`) branchless
  form: `planetLockTimer >= 1 && !missionBlocksPlanets` and
  `!(missionBlocksPlanets || planetLockTimer < 1)` are byte-identical to
  the base; the compiler still folds the `missionBlocksPlanets` test.
* Moving `bool alwaysEnemy` back inside the scanner block: raw 8,507,
  size 9,912.  Dropping the duplicate `stationLockTimer` store: raw 7,579,
  size 9,896.  Both are needed.

## 4. Landed change

```diff
-    int alwaysEnemySeen = false;
+    bool alwaysEnemySeen = false;
+    bool alwaysEnemy = false;
...
                 if (this->lockedStation == nullptr) {
                     this->field_0x28 = 0;
+                    this->stationLockTimer = 0;
+                } else {
+                    this->stationLockTimer = 0;
                 }
-                this->stationLockTimer = 0;
...
-        int alwaysEnemy = false;
...
-    if (this->field_0x1bc >= 1) {
+    if (__builtin_expect(this->field_0x1bc >= 1, 1)) {  // lint: arm_builtin
...
-            } else if (this->field_0x1bc < 3) {
+            } else if (__builtin_expect(this->field_0x1bc < 3, 1)) {  // lint: arm_builtin
...
-            } else if (this->field_0x1bc < 5) {
+            } else if (__builtin_expect(this->field_0x1bc < 5, 1)) {  // lint: arm_builtin
...
     this->field_0x1bc = 0;
-    this->plasmaInRange = 0;
-    Globals::status->getCurrentCampaignMission();
-    Globals::status->getCurrentCampaignMission();
 }
```

## 5. Gate

| metric | before | after |
|---|---:|---:|
| size | 9,908 | **9,908** |
| raw same-address diff | 8,124 | **7,578** |
| instruction-index alignment | 1,320 | **1,428** |
| `.text` differing bytes | 84,652 | **84,106** |
| `.ARM.extab` differing bytes | 725 | **720** |
| `.rel.plt` | byte-identical | **byte-identical** |
| byte_exact / linked_exact | 4507/4507 | **4507/4507** |
| size-exact | 4524/4524 | **4524/4524** |
| data | 843/843 | **843/843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |
| parity | 5370/5370 | **5370/5370** |

## 6. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j8
```

## 7. Additional sweeps (same pass, all rejected at 9,908 B)

Each variant was built and measured against the landed shape (raw 7,578 /
aligned 1,428).  None beat it.

### Campaign-mission guard (`campaignMission != nullptr && ... (cur < 148 || cur >= 152)`)

The original places the campaign *body* inline (fall-through at `0x12e474`,
skip out of line at `0x12e4a8`); our `__builtin_expect(..., 0)` puts the body
out of line at `0x12e5ca` and the skip inline.  Flipping the layout was
swept exhaustively:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `expect(cond, 0)` (landed) | **7,578** | **1,428** | 9,908 |
| no expect | 8,696 | 1,503 | 9,964 |
| `expect(cond, 1)` | 9,250 | 1,654 | 9,964 |
| `expect(campaignMission != nullptr, 0) && ...` | 9,313 | 2,719 | 9,952 |
| `expect(isVisible(), 0) && ...` | 9,195 | 2,701 | 9,944 |
| `!(cur >= 148 && cur < 152)` + `expect(...,0)` | 8,952 | 1,469 | 9,940 |
| `!(cur >= 148 && cur < 152)` no expect | 8,840 | 1,496 | 9,960 |
| nested `if (mission-ok) { if (148/152) ... }` | 8,696 | 1,503 | 9,964 |
| nested, expect on outer | 9,195 | 2,701 | 9,944 |
| nested, expect on inner | 8,843 | 1,502 | 9,944 |

So the single whole-condition `expect(..., 0)` is load-bearing: it is the only
shape that reaches 9,908 B.  The inline-body layout costs +56 B of long
branches (the no-expect build has 3,522 insns but 9,964 B, i.e. 10 fewer
instructions and 56 more bytes than the original).

### Sound ladder (`field_0x1bc`)

Swapping the arms to `if (__builtin_expect(field_0x1bc < 1, 0)) { cold } else
{ hot }` is byte-identical in raw (7,578) but one instruction worse in the
alignment metric (1,429), so the landed `>= 1, 1` form is kept.  The
no-expect swapped form is 8,277 raw / 1,297 aligned at 9,904 B (4 B short —
not allowed).

### Tail `String` expression

The 4 B deficit is in the exception-cleanup landing pads at the end of the
function: ours has 7 destructor sites (`sp+232, 196, 184, 220, 208, 232, 220`)
vs the original's 6 (`sp+232, 196, 184, 220, 208, 232`).  The LSDA call-site
tables are structurally identical (19 regions, same starts/lengths), so the
exception regions match; only the pad layout differs.  Reshaping the
`DrawString` expression does not help:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| landed `*getText(133) + String(": ") + String(String(tecLevel), false)` | **7,578** | **1,428** | 9,908 |
| parenthesised `(*getText(133) + String(": ")) + ...` | 7,578 | 1,428 | 9,908 |
| `String(": ", false)` / `String(String(x))` (default arg) | 7,578 | 1,428 | 9,908 |
| `String(*getText(133), false) + ...` (explicit copy) | 8,616 | 1,540 | 9,936 |
| `String(String(tecLevel))` (drop inner copy) | 8,658 | 2,659 | 9,884 |
| named `String value = ...; DrawString(..., value, ...)` | 9,444 | 1,521 | 9,872 |
| named value + `operator+=` chain | 8,730 | 1,701 | 9,876 |

The three byte-identical variants differ only in source spelling; the default
argument and the explicit `false` compile to the same `movs r2, #0`.
