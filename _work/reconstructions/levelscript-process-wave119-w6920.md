# LevelScript::process — wave 119 / w6920 (faithful-prologue compensation search)

Status: **analysis note, no source change landed, 2026-11-06**. Branch
`wave119-w6920`, base master `8b63afe8a` (wave-119 harvest + Radar::draw).
Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57232 B (0xdf90)**, 20169 normalised instructions.

The task was to land the *faithful* prologue order
(`getMessages`, `getPlayer`, `getEnemies`) together with the faithful
case-135 `delete cargo; cargo = nullptr;` order at exactly 57232 B by finding
real compensating reductions.  The exact-size target is reachable, and the best
measured variant at 57232 B is **59.099726 %** (raw difflib), with `.rel.plt`
and every section size identical to the current baseline.  It is still below
the tuned master (**59.489705 %**), because the faithful prologue changes the
register allocation (see §4); the task brief explicitly allows a faithful shape
to score lower than a tuned one.

## 1. Result

```
                         master (8b63afe8a)   best exact variant (I2)
raw ratio (difflib)      59.489705            59.099726
orig / ours insns        20169 / 20043        20169 / 20041
orig / ours size         57232 / 57232        57232 / 57232   (size-exact)
byte_exact               4505                 4505
linked_exact             4505                 4505
size-exact               4524/4524            4524/4524
data_exact               843/843              843/843
imports                  207                  207
lint                     CLEAN                CLEAN
drift                    0                    0
RATCHET                  PASS                 PASS
.text size               1259464 (exact)      1259464 (exact)
.ARM.extab size          156304 (orig 156312) 156304 (orig 156312)
.rodata size             269351 (orig 269159) 269351 (orig 269159)
.rel.plt                 baseline 3-cycle     identical to baseline
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked
`.so` (helper `/dev/shm/w6920/buildscore.py`, same definition as the
wave-118/119 notes).  `verify.py` rounds both values to **59.5 %**; the raw
ratio is the discriminating number.

**No source change was committed**: the variant is not byte-exact, so the gate
counts stay 4505/4505 and the note is the deliverable.  The exact diff is in
§5 so the coordinator can apply it.

## 2. Exact commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w6920
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
```

Fast per-variant loop (`/dev/shm/w6920/buildscore.py`): compile only
`LevelScript.cpp` with the exact `compile_commands.json` flags, gold-link a
variant `.so` from the checked-in object list (same link command as
`build.ninja`, object/output paths substituted), GNU-strip +
`tools/reorder_shstrtab.py`, then score the target's normalised stream.  About
5–7 s per variant; the sweeps below ran 6 in parallel.

## 3. What each change costs on the current base

| variant | edits | size | raw % |
|---|---|---:|---:|
| master | tuned prologue, case-135 old order, full anchor | 57232 | **59.489705** |
| A | faithful prologue only | 57304 (+72) | 58.726603 |
| B | case-135 `delete` order only | 57248 (+16) | 59.203819 |
| C | A + B | 57312 (+80) | 58.740075 |
| D2 | C + anchor trim (`switchSkyboxForIntro`, `particleEmitBoolPtr->reset`) | 57304 (+72) | 58.757934 |
| E | C + whole anchor block removed | 57232 (0) | 58.754670 |
| H | D2 + `wingmen = enemies` (l.1483) + `player->geometry` (l.3838) | 57232 (0) | 59.087744 |
| I2 | C + `wingmen = enemies` (l.1483) + `player->geometry` (l.3838) + `(*enemies)[0]` (l.2036) + `player->setCollide` (l.1469) | 57232 (0) | **59.099726** |

Notes:

* A matches the original prologue **instruction-for-instruction** (same
  `getMessages`/`getPlayer`/`getEnemies` sequence, same
  `strd r4,r0,[sp,#80]`) except that `Globals::status` lands in **r9**
  instead of the original's **r4** (`strd r9,r8,[sp,#92]` vs
  `strd r4,r8,[sp,#92]`), and the `delta`/`messages` spill pair is
  emitted as `strd r6,r5,[sp,#72]` instead of `strd r5,r6,[sp,#72]`.
* C is +80, not +88: the prologue's +72 and the case-135 +16 overlap by 8 B
  (both change the same landing-pad/block layout).
* D2's anchor trim saves only **8 B** on this base (the w6828 note measured
  16 B on its base); the anchor's four remaining calls cost 0 B individually
  except `setVisible` (16 B), and the `if (delta < 0)` wrapper itself is worth
  ~32 B.
* E is the only exact-size variant that removes the artificial anchor
  completely.  It changes `.rel.plt` by one swap
  (`PlayerStation::setVisible` ↔ `PlayerEgo::deleteHackingGame`), because the
  anchor was exactly the workaround for that first-reference rotation.
* I2 keeps the full anchor, so `.rel.plt` is identical to the master baseline,
  and reaches the same 57232 B by removing 80 B of duplicate local lookups
  instead.

## 4. Why the faithful prologue costs 72 B (proven vs hypothesis)

**Proven** (from the linked disassembly at `0x137d18`):

* The prologue call sequence and stores are identical to the original except
  the two register-allocation choices above.
* The +72 B is not extra source code: A has *fewer* instructions than master
  (20007 vs 20043) but 36 more 32-bit encodings.  `Globals::status` is hoisted
  into **r9** (a high register), so every `status->…` access becomes a 4-byte
  `ldr.w r0,[r9]` instead of the original's 2-byte `ldr r0,[r4,#0]`; the
  `messages` spill is also reloaded from `[sp,#76]` (4-byte encoding) where
  master keeps it in r4 (`ldr r0,[r4,#4]`, 2 bytes).
* The original does **not** keep status in r4 for the whole function: it
  stores it to `[sp,#92]` and later reloads it into r6/r4 (e.g. `0x13803c`
  `ldr r6,[sp,#92]`, `0x1381b2` `ldr r0,[r4,#0]` after a reload).  Master's
  r9 form is a different (tuned) allocation, not a different source.

**Hypothesis** (not proven): the original source probably used the
`Globals::status` global directly and clang chose to spill it; the faithful
prologue's different live ranges make clang keep it in r9 instead.  A local
`Status *status = Globals::status;` was tried (all uses and first-two uses) and
made the score much worse (45.44 % / 58.71 %), so the local is not the answer.
The remaining 0.39-point gap to master is this allocation difference.

## 5. Best exact variant (I2) — exact diff

```diff
--- a/src/game/world/LevelScript.cpp
+++ b/src/game/world/LevelScript.cpp
@@ -608,9 +608,9 @@
-    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
-    PlayerEgo *player = m_pLevel->getPlayer();
     Array<RadioMessage *> *messages = m_pLevel->getMessages();
+    PlayerEgo *player = m_pLevel->getPlayer();
+    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
@@ -1466,7 +1466,7 @@
-            m_pLevel->getPlayer()->setCollide(true);
+            player->setCollide(true);
@@ -1480,7 +1480,7 @@
-                Array<KIPlayer *> *wingmen = m_pLevel->getEnemies();
+                Array<KIPlayer *> *wingmen = enemies;
@@ -2033,7 +2033,7 @@
-                    KIPlayer *freighter = (*m_pLevel->getEnemies())[0];
+                    KIPlayer *freighter = (*enemies)[0];
@@ -3835,7 +3835,7 @@
-                        m_pLevel->getPlayer()->geometry->setDirection(-dir, Vector{0.0f, 1.0f, 0.0f});
+                        player->geometry->setDirection(-dir, Vector{0.0f, 1.0f, 0.0f});
@@ -4155,8 +4155,8 @@
-                        (*enemies)[i]->cargo = nullptr;
                         delete (*enemies)[i]->cargo;
+                        (*enemies)[i]->cargo = nullptr;
```

The first hunk is the faithful prologue; the last is the faithful case-135
delete order.  The middle four are the compensating reductions:

| edit | bytes | call-count effect |
|---|---:|---|
| `wingmen = enemies` (l.1483) | −60 | `getEnemies` 36 → 35 |
| `(*enemies)[0]` (l.2036) | −8 | `getEnemies` 35 → 34 (matches original) |
| `player->geometry` (l.3838) | −12 | `getPlayer` 28 → 27 |
| `player->setCollide` (l.1469) | 0 | `getPlayer` 27 → 26 |

After I2 the `getEnemies` call count is **34 = original**.  `getPlayer` is 26
vs the original's 23 (three extras remain at l.790, l.1783, l.3787); the
anchor still supplies the duplicate `getLandmarks`/`getStarSystem`/
`ParticleSystemManager::reset` calls (17/17/4 vs original 15/15/2).

The l.1469 edit is a *tuning* edit, not a fidelity edit: the original does call
`getPlayer` there (`0x13801c`).  It is size-neutral and worth +0.002 raw; it
can be dropped (I = 59.097692 %) if strict call-site fidelity is preferred.

## 6. Dead ends measured (recorded so they are not retried)

All sizes are for `LevelScript::process`; score is the raw difflib ratio.

| variant | size | raw % | note |
|---|---:|---:|---|
| faithful prologue, no compensation | 57304 | 58.726603 | prologue matches, +72 B |
| case-135 only | 57248 | 59.203819 | +16 B |
| prologue + case-135 | 57312 | 58.740075 | +80 B |
| + anchor trim (2 calls) | 57304 | 58.757934 | −8 B on this base |
| + full anchor removal | 57232 | 58.754670 | `.rel.plt` swap |
| C + `wingmen = enemies` only | 57252 | 59.004327 | −60 B, 20 B short |
| D2 + `wingmen = enemies` + `player->geometry` (H) | 57232 | 59.087744 | best without l.2036/l.1469 |
| C + `wingmen = enemies` + `(*enemies)[0]` + `player->geometry` + `player->setCollide` (I2) | 57232 | **59.099726** | best measured |
| `Status *status` local, all uses | 57352 | 45.441404 | allocation collapses |
| `Status *status` local, first two uses | 57304 | 58.705229 | still r9 |
| `stars` local removed (l.1301) | 57312 | 58.715185 | worse |
| `wingmen` local removed entirely (l.1450) | 57360 | 43.847129 | allocation collapses |
| `ge:1483` + `gp:790` + `gp:3787` | 57228 | 58.979780 | −4 B, interactions |
| `ge:1483` + `gp:1783` + `gp:3268` | 57228 | 58.998209 | −4 B |
| `ge:1483` + `gp:3047` + `gp:3787` (on C) | 57236 | 58.898727 | −4 B |
| `ge:1483` + `gp:3838` + `ge:4537` | 57240 | 58.990301 | +8 B |
| `ge:1483` + `gp:3838` + `ge:4582` | 57240 | 58.990301 | +8 B |
| `ge:1483` + `gp:3838` + `ge:2424` | 57240 | 58.980353 | +8 B |
| `ge:1483` + `gp:3838` + `ge:4467` | 57240 | 58.990301 | +8 B |

A greedy hill-climb over all `getPlayer`/`getEnemies` single edits from I2
found no exact-size improvement (best single edit `gp:3995` = 59.098257 %);
I2 is a local optimum in that neighbourhood.

## 7. Section / `.rel.plt` status

* `.text` is **1259464 B = original** for I2 and E (the function is exactly
  57232 B).  A/C/D2 are +96 B in `.text` and `.ARM.exidx` because the function
  is 57304/57312 B.
* `.ARM.extab` stays **156304 B** (original 156312, 8 B short) for I2/E — the
  same pre-existing delta as master; `Generator::createMission`'s LSDA is the
  cause (see `generator-createmission-wave119-w6780.md`).
* `.rodata` stays **269351 B** (original 269159) for I2/E — the same
  pre-existing delta as master.
* `.rel.plt`: the current master already differs from the original in a
  pre-existing 3-cycle (`MenuTouchWindow::inCinematicMode`,
  `MGame::setCinematicMode`, `Hud::resetAnalogStick`), introduced by the
  wave-119 MenuTouchWindow/MGame passes, not by LevelScript.  A/B/C/D2/I2 keep
  the baseline `.rel.plt` **byte-for-byte**.  E additionally swaps
  `PlayerStation::setVisible` ↔ `PlayerEgo::deleteHackingGame`, because the
  anchor was the workaround for that first-reference order.

## 8. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6920/buildscore.py /dev/shm/w6920/I2.cpp /dev/shm/w6920/vI2
# 59.099726%  size=57232  orig_insns=20169 ours_insns=20041
```

`/dev/shm/w6920` helpers are scratch only; the note is the sole committed
artifact.  The master source is unchanged, so the worktree stays clean.
