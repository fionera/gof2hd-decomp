# LevelScript::process — wave 136 / w8102 (the genuinely missing case-40 state-1 translate)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `2e51d765d` (wave-135 head).
Scratch helpers in `/dev/shm/w8102/` (not tracked): `symtrans.py` (symbolic
translate-argument tracer), `score.py` (process-only fuzzy + section diff),
`secdiff.py`, `rem.py`/`rem2.py` (size-compensator sweeps), `diffproc.py`.

## Result

```
                         before (master)   after (landed)
official verify pct      58.8              58.9
raw difflib ratio        58.8516           58.8806
process .text diffbytes  54,359 (wave135)  54,327   (-32)
whole .text diffbytes    84,960            84,971   (+11)
orig / ours insns        20169 / 20044     20169 / 20049
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
.rel.plt                 identical         **4 bytes differ**
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## The missing statement (not at 0x13a4b8)

The handoff's `translate(-7000,500,17000)` at `0x13a4b8` is a **misread** (already
documented by wave 135): `0x13a4b8` is `translate(-10000,5000,25000)` (case 157
state 1) and `-7000,500,17000` is `0x13a5fc` (case 40 state 0).  Both are present
in the source (lines 4772 and 1925) and in the emitted code.

The real missing translate was found by symbolic-tracing every
`TargetFollowCamera::translate(f,f,f)` call in both binaries and diffing the
argument expressions (register/float dataflow, `symtrans.py`).  The original has
**59** translate calls, ours had **58**; the unmatched one is:

```
0x13e6da  (case 40, state 1)
  movs r4,#0 ; sub.w r0,r4,sl,lsl #1 ; movs r1,#0 ; movs r2,#0
  vmov s0,r0 ; vcvt.f32.s32 s0,s0 ; ldr r0,[sp,#88] ; ldr r0,[r0,#20]
  vmov r3,s0 ; blx TargetFollowCamera::translate
```

i.e. `m_pCamera->translate(0.0f, 0.0f, (float)(-2 * delta));` at the **top of
case-40 state 1**, *before* the `messages[4]->isOver()` test.  Our source had the
state-1 body starting directly with the `isOver` check.  This is the statement
the handoff was looking for (the address in the handoff was wrong).

## Size-neutral companions

Adding the statement costs **+36 B**.  The case-125 state-3 first-reference
anchor block (source line ~3976, guarded by `delta < 0`, never taken) contains
five duplicated calls that the original emits only once elsewhere.  Removing two
of them and the `player->resetGunDelay()` size pad restores exactly 57232 B:

| edit | size delta |
|---|---:|
| case-40 state-1 `translate(0,0,-2*delta)` | +36 |
| remove anchor `m_pLevel->switchSkyboxForIntro()` (real call at line 824) | -8 |
| remove anchor `((PlayerStation*)landmarks[0])->setVisible(false)` (real call at line 2151) | -24 |
| remove `player->resetGunDelay()` at the `setFixed(false)` body (line 4696) | -4 |
| **net** | **0** |

The two anchor removals are semantically clean: the original has exactly one
`switchSkyboxForIntro` and one `PlayerStation::setVisible`, and both real calls
remain.  The `resetGunDelay` removal is the size pad: the original calls it on a
*fresh* `Level::getPlayer()` there (`0x14497e`), so the faithful form would be
`m_pLevel->getPlayer()->resetGunDelay()` (+8 B); the pad uses the cached `player`
local instead (wave-132's -4 B compensator).  Removing it drops the emitted
`resetGunDelay` count from 10 to 9 (the original has 10).

## `.rel.plt` trade-off

Removing the anchor `PlayerStation::setVisible` moves that symbol's first
reference from the dead `delta < 0` block to the real line-2151 block, so
`.rel.plt` differs by **4 bytes** (one entry moves; size is unchanged at 29,392 B).
Keeping the anchor restores `.rel.plt` exactly but costs 24 B, which the
remaining compensators cannot absorb at 57232 B.  The ratchet is green either
way; the fuzzy score is 58.9 in both cases.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w8102/symtrans.py                      # translate-call multiset diff
python3 /dev/shm/w8102/score.py                         # 58.9, 57232 B, relplt 4
```

## Open questions

* The original has **10** `resetGunDelay` calls; ours now emits **9**.  The
  missing one is the `0x144982` call on a fresh `Level::getPlayer()` in the
  `setFixed(false)` tail.  Restoring it costs +8 B; a different -8 B duplicate
  (e.g. the case-125 anchor `switchPlanetForIntro`) would keep the size exact.
* `.rel.plt` is 4 bytes off because the `PlayerStation::setVisible` anchor was
  removed.  A size-neutral way to keep an early reference to that symbol would
  restore `.rel.plt` byte-identical.
* The state-4/5 mission-0 teardown fix from w7764 is still semantically right
  but was not landed here (it re-rolls block placement and costs +97 .text
  bytes on this base).
