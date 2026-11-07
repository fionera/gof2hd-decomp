# LevelScript::process — wave 135 / w8101 (case-125 else-if + expect, -16 B companion: .extab blocker)

Status: **analysis only, no source change landed**, 2026-11-07. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**. Owned TU: `src/game/world/LevelScript.cpp`. Base master
`2e51d765d` (wave-135 docs, includes the case-125 state switch `8770f4f9b`).

## Result

```
                         baseline (HEAD)   best candidate
official verify pct      58.9              58.9
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        54316             54331   (+15)
whole .text diffbytes    84960             89737   (+4777)
.ARM.extab size          156312            156316  (+4 -> byte_exact 4506 -> 4044)
.ARM.exidx diffbytes     0                 833
.rel.plt                 exact             exact
byte_exact               4506              4044    (RATCHET FAIL)
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
```

The hypothesis is real and reproducible, but it is **not landable**: the faithful
`else if` grows the function's LSDA call-site table by 2 bytes (padded to 4),
`.ARM.extab` 156312 -> 156316, which shifts `.rodata`/`.data`/`.bss` and drops
`byte_exact` 4506 -> 4044.  No -16 B companion found in the time box also
restores the LSDA encoding.

## Key new finding: the switch landing already moved the cold chain

The handoff premise ("the `else if` + `__builtin_expect` shape moves the cold
chain") was measured on the **pre-switch** base (`bbffef482`).  On current HEAD
the wave-135 switch (`8770f4f9b`) already places the case-125 state-1 body
(`GameText::getText(3212)`) at **`0x13b680`** (function-relative `0x3968`), which
is essentially the original's `0x13b486` (`0x376e`, ours 506 B past).  The
`else if` chain *reverts* the switch and moves the same block **backward** to
`0x13ac3e` (`0x2f26`, 2120 B short).  So on HEAD the else-if shape is a
cold-chain regression, not an improvement.

| build | `getText(3212)` addr | rel to `0x137d18` | vs orig `0x376e` |
|---|---:|---:|---:|
| original | `0x13b486` | `0x376e` | — |
| HEAD (switch) | `0x13b680` | `0x3968` | +0x1fa |
| `else if` chain | `0x13ac3e` | `0x2f26` | -0x848 |

## The +16 B shape and the -16 B companion

The +16 B shape on HEAD is the `else if` chain with `__builtin_expect` on the
**condition** (not on the state test):

```cpp
if (m_nState == 0 && __builtin_expect(((*messages)[1])->isTriggered(), 1)) { ... }
else if (m_nState == 1) { ... }
if (m_nState == 2) { ... }
else if (m_nState == 3) { ... }
else if (m_nState >= 4 && m_nState <= 6 && ...) { ... }
```

Measured **57248 B (+16)**, `diffbytes 55091`, `.extab 156316`, `.rel.plt`
differs at 2833..2836 (`ParticleSystemManager::reset`/`startSmokeEmission` vs
`setVisible`/`deleteHackingGame`).

The -16 B companion found is the **case-24 state-0 loop-2 cache** (w7766's
`case 24 loop 2`, `-8` on the old base, but **-16** on this shape):

```cpp
for (unsigned int j = 0; j < enemies->count; ++j)          // was m_pLevel->getEnemies()->count
    (*enemies)[j]->player->setEnemies(nullptr);            // was (*m_pLevel->getEnemies())[j]
```

With the anchor restored to the original first-reference order
(`switchPlanetForIntro`, `switchSkyboxForIntro`, `ParticleSystemManager::reset`,
`startSmokeEmission`, `setVisible`; i.e. drop `PlayEngineSound`/`StopEngineSound`,
add `reset` + `startSmokeEmission`) the candidate is exactly **57232 B** with
`.rel.plt` byte-identical:

| variant | size | diffbytes | .extab | .rel.plt |
|---|---:|---:|---|---|
| HEAD (switch) | 57232 | 54316 | 156312 | exact |
| `else if` chain (no expect) | 57248 | 55090 | 156316 | differs |
| `+ __builtin_expect(cond,1)` | 57248 | 55091 | 156316 | differs |
| `+ __builtin_expect(state==0,1)` | 57240 | 54920 | 156316 | differs |
| nested `if (expect(state)) { if (cond) }` | 57236 | 55237 | 156316 | differs |
| `+` anchor trim (drop sound pair) | 57232 | 54384 | 156316 | differs |
| `+` anchor trim `+ startSmokeEmission` | 57232 | 54387 | 156316 | differs |
| `+` anchor trim `+ reset` | 57240 | 54909 | 156316 | differs |
| `+` full anchor (trim+sound swap+reset+smoke) | 57248 | 55090 | 156316 | exact |
| **full anchor `+` case-24 loop-2 cache** | **57232** | **54331** | **156316** | **exact** |

The candidate's `.ARM.extab` is still 156316, so the ratchet fails:
`byte_exact 4506 -> 4044` (the `.extab` shift moves `.rodata`/`.data`/`.bss`;
`.ARM.exidx` diffbytes 0 -> 833, `.data` 0 -> 72, `.bss` 0 -> 3).

## Why the .extab grows (new decode)

Parsing the `.ARM.exidx` entry for `0x137d18` and the LSDA at `.ARM.extab`:

```
baseline: process LSDA @0x1c2af8, len 0x15c (348 B), call-site table len 0x2c6
else-if : process LSDA @0x1c2b18, len 0x160 (352 B), call-site table len 0x2c8
```

The `else if` adds 2 bytes to the call-site table (one field's uleb128 widens),
padded to +4.  Removing calls does **not** shrink it: `NOANCHOR` (-88 B),
`ANCHOR1` (-24 B), case-4 `getStarSystem` hoist, case-135 `getEnemies` cache and
a state-1 `getRoute` hoist all leave `.extab` at 156316.  The growth is intrinsic
to the else-if CFG re-roll, not to any particular call site.

## Other companions measured (all on the else-if shape unless noted)

| edit | size | diffbytes | .extab | .rel.plt |
|---|---:|---:|---|---|
| case-24 loop-2 cache alone (on HEAD) | 57224 | 55104 | 156312 | exact |
| case-4 `getStarSystem` hoist alone (on HEAD) | 57224 | 55116 | 156312 | exact |
| case-135 `(*enemies)[i]->setVisible` alone (on HEAD) | 57224 | 55098 | 156312 | exact |
| `routePoints` int cast alone (on HEAD) | 57232 | 54335 | 156312 | exact |
| `+` case-4 hoist | 57240 | 54900 | 156316 | exact |
| `+` case-135 cache | 57240 | 54900 | 156316 | exact |
| `+` case-4 `+` case-135 | 57240 | 54904 | 156316 | exact |
| `+` case-4 `+` loop-2 | 57224 | 55126 | 156316 | exact |
| `+` case-135 `+` loop-2 | 57224 | 55128 | 156316 | exact |
| `+` case-16 landmarks cache (8 uses) | 57192 | 55189 | 156316 | exact |
| `+` each of the 9 `resetGunDelay` receiver fixes | 57240/57248 | 54894..55099 | 156316 | n/a |

`switch + __builtin_expect` (keep the switch, only add the hint) is
**size-neutral 57232, `.extab` exact, `.rel.plt` exact, diffbytes 54317 (+1),
pct 58.9 unchanged** — but it needs a `// lint: arm_builtin` waiver and does not
move the cold chain (same `0x13b680`), so it is not a win.

## Self-contained candidate source diff

Applied to HEAD `src/game/world/LevelScript.cpp` (case-24 loop-2 cache, case-125
else-if chain with expect, and the case-3 anchor re-ordered to the original's
first-reference sequence):

```diff
@@ case 24 state 0 (0x13e97e) @@
-                    for (unsigned int j = 0; j < m_pLevel->getEnemies()->count; ++j) {
-                        (*m_pLevel->getEnemies())[j]->player->setEnemies(nullptr);
+                    for (unsigned int j = 0; j < enemies->count; ++j) {
+                        (*enemies)[j]->player->setEnemies(nullptr);
@@ case 125 @@
-                if (m_nState == 0 && ((RadioMessage *) ((*messages)[1]))->isTriggered()) {
+                if (m_nState == 0 && __builtin_expect(((RadioMessage *) ((*messages)[1]))->isTriggered(), 1)) {
                     ...
                 }
-                switch (m_nState) {
-                case 1: {
+                else if (m_nState == 1) {
                     ...
-                    break;
                 }
-                case 2: {
+                if (m_nState == 2) {
                     ...
-                    break;
                 }
-                case 3: {
+                else if (m_nState == 3) {
                     ...
                     if (delta < 0) {
                         m_pLevel->getStarSystem()->switchPlanetForIntro();
                         m_pLevel->switchSkyboxForIntro();
-                        player->PlayEngineSound();
-                        player->StopEngineSound();
+                        m_pLevel->particleRenderBoolPtr->reset();
+                        player->startSmokeEmission();
                         ((PlayerStation *) (*m_pLevel->getLandmarks())[0])->setVisible(false);
                     }
                     ...
-                    break;
                 }
-                default: {
-                    if (m_nState >= 4 && m_nState <= 6 && player->isDockedToDockingPoint() && player->hackingWon()
+                else if (m_nState >= 4 && m_nState <= 6 && player->isDockedToDockingPoint() && player->hackingWon()
                            && player->getHackingGameDockIndex() >= 0) {
                     ...
-                    break;
-                }
                 }
                 break;
```

## Conclusion / open questions

The case-125 `else if` is semantically faithful (the original skips state 1 after
the state-0 body: `0x139ffa b.w 0x13b518`) but on HEAD it (a) moves the cold
chain *away* from the original and (b) grows `.ARM.extab` by 4, which is fatal to
`byte_exact`.  The wave-135 switch already captured the block-placement win.

Next wave: find a companion that narrows the LSDA call-site encoding back to
`0x2c6` (i.e. a layout change that moves the widened uleb128 field below its
7-bit boundary) *and* keeps 57232 B; or accept the switch shape and look for a
different diffbytes lever.  The `.extab` decode script is `/dev/shm/w8101/lsda*.py`.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS at HEAD
```

Scratch helpers in `/dev/shm/w8101/` (not tracked): `measure.py` (size /
diffbytes / section sizes), `relplt.py` (`.rel.plt` symbol list), `mk125.py` /
`mkhybrid.py` / `mkcomp.py` (source-shape generators), `lsda2.py` (LSDA decode),
`candidate.diff`.
