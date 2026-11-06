# LevelScript::process — wave 121 / w7104 register-allocation root cause

Status: **note-only, no source change**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
instructions. Source: `src/game/world/LevelScript.cpp`.

## Result

```
                         before (master)   after (this note)
official verify pct      60.0              60.0
raw difflib ratio        59.959208039      59.959208039
orig / ours insns        20169 / 20035     20169 / 20035
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

`raw difflib ratio` is
`difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams. The official
`verify.py` rounds this to `60.0`.

## Ground truth: prologue and register map

Original prologue (`0x137d18..0x137d72`):

```
137d26  mov   sl, r0                 ; this
137d3a  ldr   r0, [r4, #24]!         ; r4 = &this->m_pLevel
137d3e  blx   Level::getMessages
137d42  mov   r5, r0                 ; messages = r5
137d44  ldr   r0, [r4, #0]
137d46  blx   Level::getPlayer
137d4a  strd  r4, r0, [sp, #80]      ; [80]=&m_pLevel, [84]=player
137d4e  ldr   r0, [r4, #0]
137d50  blx   Level::getEnemies
137d54  mov   r8, r0                 ; enemies = r8
137d56  ldr   r0, [pc, #848]         ; Globals::status
137d5a  ldr   r4, [r0, #0]           ; status = r4
137d5c  ldr   r0, [r4, #0]
137d5e  blx   Status::getCurrentCampaignMission
137d64  str.w sl, [sp, #88]          ; spill this
137d68  strd  r4, r8, [sp, #92]      ; [92]=status, [96]=enemies
137d6c  mov   r0, r8
137d6e  strd  r5, r6, [sp, #72]      ; [72]=messages, [76]=delta
```

Current build prologue:

```
137d26  mov   sl, r0                 ; this
137d3a  ldr   r0, [r4, #24]!
137d3e  blx   Level::getEnemies
137d42  mov   r8, r0                 ; enemies = r8
137d44  ldr   r0, [r4, #0]
137d46  blx   Level::getPlayer
137d4a  strd  r4, r0, [sp, #80]
137d4e  ldr   r0, [r4, #0]
137d50  blx   Level::getMessages
137d54  mov   r4, r0                 ; messages = r4
137d56  ldr   r0, [pc, #884]         ; Globals::status
137d5a  ldr.w r9, [r0]               ; status = r9
137d5e  ldr.w r0, [r9]
137d62  blx   Status::getCurrentCampaignMission
137d68  str.w sl, [sp, #88]          ; spill this
137d6c  strd  r4, r6, [sp, #72]      ; [72]=messages, [76]=delta
137d72  strd  r9, r8, [sp, #92]      ; [92]=status, [96]=enemies
```

The **stack slots are already identical**:

| slot | original | current |
|---|---|---|
| `sp+72` | `messages` | `messages` |
| `sp+76` | `delta` | `delta` |
| `sp+80` | `&this->m_pLevel` | `&this->m_pLevel` |
| `sp+84` | `player` | `player` |
| `sp+88` | `this` | `this` |
| `sp+92` | `status` | `status` |
| `sp+96` | `enemies` | `enemies` |

The difference is purely **register assignment**:

| value | original | current |
|---|---|---|
| `this` | `sl` until first scratch use, then `[sp+88]` | `sl` for most of the function |
| `messages` | `r5` | `r4` |
| `enemies` | `r8` | `r8` |
| `status` | `r4` | `r9` |
| `delta` | `r6` | `r6` |
| stack guard | `fp` | `fp` |
| EH exception object | mostly `sl` | mostly `r9` |

## Evidence that the original spills `this` much harder

Counts from the linked `.so` (NDK `objdump`, exact function range):

| metric | original | current |
|---|---:|---:|
| `[sp, #88]` references (store + loads) | **328** | **195** |
| `ldr.w sl, [sp, #88]` | 10 | 20 |
| `ldrd sl, r9, [sp, #88]` | 0 | 1 |
| `ldr r0, [sp, #88]` | 103 | 60 |
| `ldr r4, [sp, #88]` | 51 | 30 |
| `ldr r5, [sp, #88]` | 46 | 26 |
| `ldr r6, [sp, #88]` | 41 | 22 |
| `ldr r1, [sp, #88]` | 31 | 14 |
| `ldr r2, [sp, #88]` | 13 | 2 |
| `ldr.w r8, [sp, #88]` | 10 | 5 |
| `ldr.w r9, [sp, #88]` | 8 | 8 |
| `ldr.w fp, [sp, #88]` | 5 | 2 |
| `ldr r3, [sp, #88]` | 6 | 4 |
| `mov sl, r0` (EH exception) | 16 | 3 |
| `mov r9, r0` (EH exception) | 8 | 20 |
| `[sp, #92]` status spill refs | 48 | 25 |

The original's extra `this` reloads are not concentrated at one site. They are
spread over the whole function and correlate with `sl` being reused as a
general scratch register:

* `0x137ea6` `mov sl, fp` -- `sl` becomes the stack-guard pointer during the
  mission-0 setup; `this` is restored at `0x137f78`.
* `0x1387ee` `ldr.w sl, [sp, #76]` -- `sl` is loaded with `delta` (a dead
  reload before `0x138878` reloads `this`).
* `0x138b2e` `ldr.w sl, [r0]` -- `sl` holds a vtable function pointer used by
  the three calls at `0x138b3c`, `0x138b72`, `0x138b9e`.
* `0x138bbc` `add.w sl, sp, #400` and `0x138bdc` `ldmia.w sl, {r8, r9, sl}`
  -- `sl` is a matrix/vector temporary.
* `0x13922c`/`0x1392de` and `0x13b534`/`0x13b6ac` -- `sl` is a loop counter.
* `0x1394ce`, `0x1394f4`, `0x13a054`, `0x13a888`, `0x13acea`, `0x13c52e`,
  `0x13da8c`, ... -- `sl` is a `Vector` temporary pointer.

At each of those live ranges the original must reload `this` from `[sp+88]`
before touching a member. The current build instead keeps `this` in `sl` and
uses `r9` for `Globals::status`, so the same scratch values are allocated to
`r5`/`fp`/`sl` and the allocator can keep `sl=this` across more blocks.

The EH landing-pad split is a direct symptom of the same allocation:

* original: 16 `mov sl, r0` (mostly the 14 late cleanup pads) and 8
  `mov r9, r0`;
* current: 3 `mov sl, r0` and 20 `mov r9, r0` (the 14 late cleanup pads all
  use `r9`).

The late cleanup pads are the same 14 `String`/`delete` cleanups in both
builds; only the chosen exception register differs (`sl` vs `r9`).

## Source-level experiments (all size-exact unless noted)

The original's accessor order is `messages, player, enemies`; the current
source order is `enemies, player, messages` (lines 611-613). The faithful
prologue is:

```diff
-    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
-    PlayerEgo *player = m_pLevel->getPlayer();
-    Array<RadioMessage *> *messages = m_pLevel->getMessages();
+    Array<RadioMessage *> *messages = m_pLevel->getMessages();
+    PlayerEgo *player = m_pLevel->getPlayer();
+    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
```

Measured variants (raw difflib ratio, not the rounded official pct):

| variant | size | raw ratio | prologue result |
|---|---:|---:|---|
| current (enemies, player, messages) | 57232 | **59.959208039** | messages=r4, status=r9 |
| faithful prologue (messages, player, enemies) | 57304 | 58.753236407 | messages=r5, enemies=r8, **status still r9** |
| messages, enemies, player | 57304 | ~58.7 | messages=r5, enemies=r8, status=r9 |
| player, messages, enemies | 57304 | ~58.7 | messages=r5, enemies=r8, status=r9 |
| base + hoist `msg2` to top of mission-1 branch | 57232 | 59.874639339 | messages=r4, status=r9 |
| messages-first + hoist `msg2` | 57304 | ~58.7 | messages=r5, status=r9 |
| explicit `Status *status = Globals::status` (all uses) | 55724 | 25.535696151 | status=r9, code shape changes |
| explicit status local, first two dispatch tests only | 57304 | ~58.7 | status pointer=r9, vtable=r4 |

The faithful prologue gets the first two registers right (`messages=r5`,
`enemies=r8`) but **not** `status=r4`. It also flips the spill-slot order:
`messages` is spilled to `[sp+76]` at the mission-1 use (`0x137e24` loads
`[sp+76]` instead of using `r5`), so `[72]=delta` and `[76]=messages`. That is
the source of the +72 B and the lower score. The current order is the only
size-exact one.

Hoisting `RadioMessage *msg2` to the top of the mission-1 branch was tried to
keep `messages` live in `r5`; it does keep the current 57232 B size on the base
order but scores slightly worse (59.874639339) and still leaves `status=r9`.

An explicit `Status *status = Globals::status` local was tried because the
original keeps `status` in `r4` and reloads it from `[sp+92]` only 47 times.
Replacing all `Globals::status` uses with the local shrinks the function to
55724 B and destroys the match (25.535696151): it removes the repeated
`Globals::status` GOT loads that the original still performs. A partial local
(first two dispatch tests only) keeps 57304 B and still puts the status pointer
in `r9` (with the vtable in `r4`). The original therefore does **not** appear to
have an explicit `Status *status` local in this function.

## Root cause

1. The source-level trigger is the prologue accessor order. The original calls
   `getMessages`, `getPlayer`, `getEnemies`; the current source calls
   `getEnemies`, `getPlayer`, `getMessages`. That alone explains why
   `messages` is `r5` in the original and `r4` here.
2. The faithful order is necessary but not sufficient. The remaining
   difference is whole-function live-range pressure: the original's `sl` is a
   general scratch register (delta, vtable pointer, matrix/vector temporaries,
   loop counters, EH exception object), so `this` is spilled to `[sp+88]` and
   reloaded 328 times. The current build keeps `this` in `sl` and uses `r9`
   for `status`, so `sl` stays live as `this` and the scratch values move to
   `r5`/`fp`. That is why the current build has only 195 `[sp+88]` references.
3. The exact `sl` live ranges that force the original spill are the ones listed
   in the evidence section: `0x1387ee`, `0x138b2e`, `0x138bbc`, `0x13922c`,
   `0x1394ce`, `0x13b534`, and the later `Vector` temporary sites. They are not
   a single source declaration; they are the original's local
   `Vector`/`String`/loop scopes. Matching them requires reconstructing the
   original block-local declarations, not just reordering the three prologue
   locals.

## Concrete patch proposal

The next landable experiment should be the faithful prologue plus a live-range
anchor for `messages`, then a re-check of the mission-1 spill:

```diff
-    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
-    PlayerEgo *player = m_pLevel->getPlayer();
-    Array<RadioMessage *> *messages = m_pLevel->getMessages();
+    Array<RadioMessage *> *messages = m_pLevel->getMessages();
+    PlayerEgo *player = m_pLevel->getPlayer();
+    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
...
     } else if (Globals::status->getCurrentCampaignMission() == 1) {
+        RadioMessage *msg2 = (RadioMessage *) ((*messages)[2]);
         m_pLevel->lodManager->forceUpdate(delta, true);
...
-        RadioMessage *msg2 = (RadioMessage *) ((*messages)[2]);
         if (msg2->isOver() && m_nState == 0) {
```

This is the faithful prologue and keeps `messages` live across the mission-1
branch. It currently measures **57304 B / 58.7** (not a win) and still leaves
`status=r9`, so it should be treated as a stepping stone, not a landed pass.
The follow-up must make `sl` the preferred scratch again in the same blocks as
the original (the `Vector` temporaries around `0x1394ce`/`0x13a054` are the
best first target); only then will `status` move to `r4` and the 328/195
`this`-reload gap close.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```
