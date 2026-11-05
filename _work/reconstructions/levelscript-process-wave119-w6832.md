# LevelScript::process — wave 119 / w6832 (statement-reorder hill-climb, round 5)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6832`, base master
`fdb68f8ee`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57232 B (0xdf90)**.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      59.474784         59.489705      (+0.014921)
orig / ours entries      20169 / 20043     20169 / 20043
orig / ours size         57232 / 57232     57232 / 57232  (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6832/score.py`, same definition as the wave-118/119 notes).
`verify`'s one-decimal report rounds both values to **59.5 %**; the raw ratio is the
discriminating number.

## Landed change (one size-neutral statement reorder)

`src/game/world/LevelScript.cpp`, the mission-125/154 "chaser wakes up" arm
(base lines 4893–4900).  Source order below is the final file order.

```cpp
m_pLevel->lodManager->forceUpdate(delta, false);
KIPlayer *fighterShip = (*enemies)[fighterIndex];
((PlayerFighter *) fighterShip)->field_0x140 = 0;
m_nState = 6;
fighterShip->player->setVulnerable(false);
chaser->setVisible(true);
m_nScriptTimerA = 0;
chaser->setActive(true);
```

The eight statements are independent (except that `fighterShip` must be declared
before its two uses), so the reorder is semantics-preserving.  It changes no call,
symbol, literal or section: `.rel.plt`/`.rel.dyn` stay byte-identical and `.text`
stays 57232 B.

## Method

1. Built and confirmed the master baseline (`RATCHET PASS`, byte 4505,
   raw 59.474784).
2. Exhaustive same-indent statement-swap sweep over the whole body
   (`/dev/shm/w6832/sweep_swap2.py`): 1999 candidates at statement distance 4–6.
   Exactly five candidates won, all in the same eight-statement block above
   (swap `setVisible`/`setActive` with `forceUpdate`/`m_nScriptTimerA`/`m_nState`):
   all scored 59.479757.
3. Hill-climb inside that eight-statement block (adjacent swaps + move-one-item
   to every position, then 1200 random valid permutations of the 8! space with
   the `fighterShip`-before-use constraint).  Best found:
   `forceUpdate, fighterShip, field_0x140, m_nState=6, setVulnerable,
   setVisible, m_nScriptTimerA, setActive` at **59.489705**.

## Dead ends measured (recorded so they are not retried)

| variant | ratio | size | note |
|---|---:|---:|---|
| master (epm decl order) | **59.474784** | 57232 | landed base |
| pem decl order | 59.473305 | 57232 | |
| prologue messages,player,enemies (matches original disasm) | 58.726603 | 57304 | prologue matches; +72 B (more 32-bit encodings) |
| same + `Status *status` local | 58.243781 | 57312 | status still lands in r9, not the original r4 |
| swap mission 5/26 blocks | 59.395205 | 57232 | |
| mission 5 first as `if` | 57.521089 | 57472 | |
| `Status *status` local, all uses | 45.426449 | 57352 | |
| `Status *status` local, first 2 uses | 58.705229 | 57304 | |
| `player->rotate` in mission 1 (l.947) | 55.537034 | 57372 | |
| `player->resetGunDelay` (l.3268) | 59.236968 | 57220 | |
| `player->geometry->getPosition` (l.1783) | 59.179309 | 57224 | |
| `player->geometry->getPosition` (l.3787) | 59.155910 | 57224 | |
| remove anchor block l.3974–3981 | 59.171333 | 57152 | |
| case-91 reorder (setActive+setName in state 0) | 59.145614 | 57240 | |
| tail `else if` → independent `if` (matches original CFG) | 57.511200 | 57456 | +224 B |
| same + mission 5 before 26 | 57.521089 | 57472 | |
| mission-51 condition `messages && mission==51` | 59.474784 | 57232 | clang canonicalises |
| faithful setVulnerable-before-field_0x140 (l.4896/4897) | 59.118152 | 57228 | -4 B |
| faithful + best block order | 59.434995 | 57232 | |
| landed block order + faithful swap | 59.118152 | 57228 | |

## Structural findings vs the original (for a future full-layout pass)

1. **Prologue**: original calls `getMessages`, `getPlayer`, `getEnemies`; master
   calls `getEnemies`, `getPlayer`, `getMessages`.  Original keeps messages in
   r5, player on the stack, enemies in r8 and `Globals::status` in **r4**; ours
   keeps messages in r4 and status in r9.  Every size-exact permutation tried is
   worse because the allocator then emits 74 more 32-bit encodings (+72 B).
2. **Top-level CFG**: original is a single `if / else if` chain
   `0, 1, 81, 78, 89, 106, {95,99,109,119,126,133,160,161}` (0x137d62–0x138ea4),
   then the shared tail `if (m_bStartSequence)` at 0x137f7c, then an **independent**
   `if (mission == 5) … else if (26) … else if (50) … else if (51)` chain at
   0x13812c, then the per-mission switch at 0x13879e.  Master instead has the
   105/144/131/158 intro blocks in the top chain and folds the 5/26/50/51 chain
   into the tail's `else if`.  Converting the tail to independent `if`s is
   semantically right but costs +224 B (not size-exact).
3. Mission 91: original order is `isTriggered`, `KIPlayer::setActive`,
   `getDockingTarget`, `getText(3211)`, String ctor, `setName`, String dtor, then
   `isDockedToDockingPoint`; original sets `m_nState = 2` at the start of the
   state-0 arm.
4. Call-count deltas (ours − original): `getPlayer` +5, `getEnemies` +2,
   `getLandmarks` +2, `getStarSystem` +2, `ParticleSystemManager::reset` +2, and
   the six-call first-reference anchor block at l.3974–3981 appears twice in ours
   but once in the original.  The anchor is what keeps `.rel.plt` rotation and
   the 57232-byte size exact; removing it drops 80 B and the ratio.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6832/score.py                         # 59.489705
```

`/dev/shm` helpers are scratch only; the landed source is the sole copy of the
change.  `.rel.plt`/`.rel.dyn` remain byte-identical, `.ARM.extab` is unchanged
(0x26290) and all 27 section sizes are unchanged.
