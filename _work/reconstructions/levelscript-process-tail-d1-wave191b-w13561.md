# LevelScript::process — wave 191b / w13561: tail d1/d2/d3/d4 statement sweeps

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13561`, branch
`wave191b-w13561`, base master `4a865a92e` (raw **51,540**, `.text` 71,240,
`.ARM.extab` 576, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

## Lens

The three handoff notes (`levelscript-process-d5-wave191-w13516`,
`cold-blocks-wave191b-w13517`, `cold-blocks-wave191b-w13524`) cover the tail's
cross-control-boundary moves, d5/d6 rotations and distance-3/4 transpositions
on older bases.  This pass re-runs the *plain* distance-1/2/3/4 transposition
sweeps on the merged wave-191b base, restricted to the tail half (source lines
2600-5200: cases 80/91/92/102/105/125/131/135/139/142/144/145/154/157/158).

Harness: `scratch/w13561/{fast,sweep,lines}.py` compiles one candidate with the
exact NDK r18b `-Oz` command from `compile_commands.json` and gold-links it with
the checked-in object list (LevelScript object swapped in place), then measures
same-address diff bytes/size/`.rel.plt`.  Four candidates run concurrently.

## Landed changes

### 1. case-157 state-9 flash block (source ~5065)

```cpp
                     if (messages[13]->isOver() && m_pExplosion == nullptr) {
+                        m_nFlashCountdown = 300;
                         m_pExplosion = new Explosion(0);
-                        m_nFlashCountdown = 300;
                         m_nScriptTimerA = 0;
```

`m_nFlashCountdown` is a plain member store; `new Explosion(0)` reads nothing
from `this`.  Semantics preserving.

```
raw        51,540 -> 51,539  (-1)
size       57,232 / 57,232   (exact)
.text      71,240 -> 71,239  (-1)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS
```

### 2. case-92 teardown camera pair (source ~3086)

```cpp
-                        m_pCamera->setTarget(player->geometry);
-                        m_pCamera->setLookAtCam(false);
+                        m_pCamera->setLookAtCam(false);
+                        m_pCamera->setTarget(player->geometry);
```

Both are independent camera setters (target pointer vs. look-at flag); no
statement between them reads what the other writes.  (The original emits
`setTarget` before `setLookAtCam` at this site, so this is a codegen-coincidence
win of the kind the wave-190/191 notes accept: the gate is the same-address
byte diff.)

```
raw        51,539 -> 51,538  (-1)
size       57,232 / 57,232   (exact)
.text      71,239 -> 71,238  (-1)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS
```

## d2/d3/d4 sweeps

On the resulting 51,538 body the distance-2/3/4 transposition sweeps were run
over the same tail range (828 / 666 / 590 candidates).  All measured wins are
listed; the producer/consumer ones are rejected.

| kind | lines | raw | safe? |
|---|---|---:|---|
| d2 | 3235/3237 | 51,537 | no — swaps `m_dirVec = getRightVector()` past `m_tempVec += m_dirVec*1200`, second add reads UP |
| d2 | 4964/4966 | 51,537 | no — `chaser->setPosition(m_tempVec+…)` then assigns `m_tempVec`, first uses stale vector |
| d3 | 3025/3028 | 51,537 | no — swaps getDirection past the `m_tempVec += m_dirVec*1.7` add |
| d3 | 4614/4617 | 51,537 | no — `setPosition(m_tempVec)` before `m_tempVec` is rebuilt |
| d4 | 3460/3464 | 51,537 | no — swaps getDirection past the two `m_tempVec += m_dirVec*step` adds |
| d4 | 4026/4030 | 51,537 | **landed** |

### 3. case-125 teardown flag group (source ~4026)

```cpp
                         player->hideShipForFirstPersonCameraView(false);
                         m_bCinematicBreak = 1;
+                        m_pRadar->enabled = 0;
                         m_pHud->visible = 0;
                         player->setFreeLookMode(false);
                         m_pCamera->enableFirstPersonCam(false);
+                        player->stopShooting(0);
-                        m_pRadar->enabled = 0;
```

`stopShooting(0)` early-returns only on turret-active/dead; none of the
intervening stores touch those.  Independent flag stores otherwise.

```
raw        51,538 -> 51,537  (-1)
size       57,232 / 57,232   (exact)
.text      71,238 -> 71,237  (-1)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS
```

## Cross-control-boundary and independent-window rotation sweeps

* Cross-control-boundary moves (simple statement ↔ adjacent `if`/`for` block,
  both directions, 217 candidates on the 51,537 body): no size-exact win.
  (A parser bug that inserted the statement *inside* the block produced six
  spurious -1/-2 cells; each makes the statement conditional, so all were
  rejected.)
* Independent-window rotations (window size 3-6 inside a straight-line run,
  only windows whose statements have pairwise disjoint identifier sets — 823
  candidates): one win, landed below.

### 4. case-154 camera/flag group (source ~4543)

```cpp
                     m_tempVec = (*m_pLevel->getEnemies())[2]->geometry->getPosition();
+                    ((PlayerFighter *) (*enemies)[0])->setAIDisabled(true);
                     m_tempVec += Vector{300.0f, 300.0f, 5800.0f};
                     m_bCinematicBreak = 1;
-                    ((PlayerFighter *) (*enemies)[0])->setAIDisabled(true);
```

The three statements touch disjoint state (`m_tempVec`, `m_bCinematicBreak`,
enemy[0] AI flag); the `m_tempVec` producer (line above) stays before the add.

```
raw        51,537 -> 51,536  (-1)
size       57,232 / 57,232   (exact)
.text      71,237 -> 71,236  (-1)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS
```

## Whole-condition `__builtin_expect` sweep (tail lines 2600-5200)

Re-ran the w11901 whole-condition expect sweep on the 51,536 body (726 cells,
both hint values).  Two size-exact / `.rel.plt`-identical -1 cells.

### 5. case-92 state-4 timer guard (source ~3068)

```cpp
-                    if (m_nScriptTimerA > 2000) {
+                    if (__builtin_expect(m_nScriptTimerA > 2000, 1)) { // lint: arm_builtin
```

Branch-hint only; semantics unchanged.

```
raw        51,536 -> 51,535  (-1)
size       57,232 / 57,232   (exact)
.text      71,236 -> 71,235  (-1)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS
```

### 6. case-157 state-10 explosion window guard (source ~5118)

```cpp
-                    if (m_nScriptTimerB > 2000 && m_nScriptTimerB < 9000 && m_nScriptTimerB + delta >= 9000) {
+                    if (__builtin_expect(m_nScriptTimerB > 2000 && m_nScriptTimerB < 9000 && m_nScriptTimerB + delta >= 9000, 1)) { // lint: arm_builtin
```

Branch-hint only; semantics unchanged.

```
raw        51,535 -> 51,534  (-1)
size       57,232 / 57,232   (exact)
.text      71,235 -> 71,234  (-1)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS
```

## Final state

```
                         base (4a865a92e)   this pass
process raw diffbytes    51,540             51,534     (-6)
orig / ours size         57232 / 57232      57232 / 57232  (exact)
.text diffbytes          71,240             71,234     (-6)
.ARM.extab               unchanged (size/base exact)
.ARM.exidx               byte-identical
.rodata                  485               485
.rel.plt                 md5 289b2a06…      md5 289b2a06…  (byte-identical)
byte_exact               4507               4507
linked_exact             4507               4507
size-exact               4524/4524          4524/4524
data_exact               843/843            843/843
imports                  207                207
lint                     CLEAN ✓            CLEAN ✓
drift                    0                  0
symbol parity            5370/5370          5370/5370
RATCHET                  PASS               PASS
```

### Sweeps that produced no size-exact win on this base

* d2/d3/d4 transpositions (re-run after the expect landings): only the
  producer/consumer cells listed above.
* d5/d6 transpositions: none.
* multi-line statement rotations (statement parser, windows 3-6, strict
  disjoint-identifier filter, 856 cells): none.
* multi-line adjacent swaps (strict disjoint filter, 480 cells): none.
* multi-line rotations with type-name-insensitive keys (1492 cells): none.
* correct cross-control-boundary moves (217 cells): none.

Scratch harness (`fast.py`, `sweep.py`, `ctrlsweep.py`, `rotsweep*.py`,
`stmts.py`, `linemap.py`, `patience.py`) lives in
`/home/debian/GalaxyOnFire2/scratch/w13561/`.
