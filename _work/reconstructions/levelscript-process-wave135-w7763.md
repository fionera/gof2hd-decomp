# LevelScript::process — wave 135 / w7763 (case-70 state-2-before-state-1 reorder; case-125 hypothesis)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `bbffef482` (wave-134 head).

## Result

```
                         before (master)   after (landed)
official verify pct      58.2              58.3
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        54359             54262   (-97)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
.rel.plt                 exact             exact
RATCHET                  PASS              PASS
```

`diffbytes` is the offset-wise byte comparison of the two 57232-byte function
bodies; `pct` is `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False)` on
`tools/verify/verify.py`'s reconciled streams.

## Landed change — case 70: state 2 is checked before state 1 (orig `0x13c04e`)

The original case-70 else path tests state 2 first:

```
1399a2  messages[0]->isTriggered() ; m_nState == 0 ?  -> state-0 body
139abe  b.w 144e36                       (state-0 body -> post-switch tail)
13c04e  cmp r1,#2 ; beq.w 13ec3c         (state 2 first -> far state-2 body)
13c054  cmp r1,#1 ; bne.w 144e36         (then state 1)
13c05a  state-1 body
```

Our source had `if (state == 0 && ...) {...} else if (state == 1) {...}` followed by
a separate `if (state == 2) {...}`.  Reordering the two blocks so the state-2 test
precedes the state-1 test reproduces the original check order and block placement:

```cpp
if (messages[0]->isTriggered() && m_nState == 0) { ... m_nState = 1; }
if (m_nState == 2) { ...state 2... }
else if (m_nState == 1) { ...state 1... }
```

This is **size-neutral** (57232 B) and **`.rel.plt`-exact**.  Behaviour is unchanged:
the state-1 body is a no-op immediately after state 0 because the `Route` built in
the state-0 body has `waypoint(0)->state == 0` (`Waypoint::Waypoint` sets
`this->state = 0` in `src/game/world/Waypoint.cpp`), and `getRoute() != 0` is true
only because state 0 just called `setRoute`.  The original's `b.w 144e36` after the
state-0 body skips the state-1 body; the reordered source falls through it, but the
fall-through does nothing.

The faithful *else-if* variant (`if (state==2) ... else if (state==1)` with the
state-0 `if` chained directly into it) was also measured: size-neutral but
`diffbytes 54375` (+16 vs master), so it was not used.

## Case-125 hypothesis (measured, NOT landed)

The w7747 lead: the faithful case-125 `else if` + `__builtin_expect` shape moves the
cold chain but is +16 B.  Reproduced exactly on this base:

| variant | size | pct | diffbytes | .rel.plt |
|---|---:|---:|---:|---|
| master | 57232 | 58.2 | 54359 | exact |
| case-125 `else if` + `__builtin_expect(state==0,1)` | 57248 (+16) | 56.4 | 55067 | **differs** |
| + anchor trim (drop `PlayEngineSound`/`StopEngineSound`) | 57232 (0) | 56.6 | **54309** | **differs** |
| + anchor: drop sound pair, add `startSmokeEmission` | 57240 (+8) | 56.4 | 54856 | exact |
| + anchor trim `{switchSkybox,startSmoke,setVisible}` | 57216 (-16) | 56.4 | 55135 | differs |

The `-16 B` companion is the first-reference anchor in case-125 state 3
(`if (delta < 0) { ... }`, seven calls).  Removing `PlayEngineSound` +
`StopEngineSound` is exactly `-16 B` and gives a landable 57232 B with
**diffbytes 54309 (-50 vs master)**, but it rotates `.rel.plt` entries 2835/2836:
the original order is `PlayerEgo::startSmokeEmission`, `PlayerStation::setVisible`,
`PlayerEgo::deleteHackingGame`; the variant emits `setVisible`, `deleteHackingGame`,
`startSmokeEmission`.  The case-125 reorder alone already breaks that order, so no
anchor trim restores it without also changing the size.

Restoring `startSmokeEmission` as an anchor call (before `setVisible`) does recover
`.rel.plt` exactly, but the function grows to 57240 B (+8) and no faithful `-8 B`
companion was found in the time box (`case-154` state-2 creep order is 0 B,
`case-70` else-if is 0 B).  The variant is therefore documented, not landed.

### Other measured faithful edits (all on this base)

| edit | size | diffbytes |
|---|---:|---:|
| case-154 state-2 creep order (`GetDirVector` before first use) | 0 | 54359 |
| case-125 state-2 `setFreeze(true)` after `setVisible(false)` | 0 | 54378 |
| case-70 faithful else-if (state 2 before state 1, chained) | 0 | 54375 |
| case-70 state-2-before-state-1 separate `if` (landed) | 0 | **54262** |

The case-154 state-2 creep order was reverted by the wave-134 merge reconciliation
(`aa9b7c395`); it is size- and diffbytes-neutral here, so it was left as-is.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

Scratch helpers in `/dev/shm/w7763/` (not tracked): `fast.py` (size/pct/diffbytes),
`try.py`, `batch.py`, `batch2.py`, `anc2.py` (anchor/case-125 sweeps), `orig.dis`,
`ours.dis`, `show.txt`, `opcodes.py`.
