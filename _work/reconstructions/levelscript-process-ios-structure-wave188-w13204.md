# LevelScript::process — iOS-structure pass 6 (wave 188, w13204)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13204`, branch
`wave188-w13204`, base master `ce16f45f1` (wave-187 harvest, raw **51,930**,
aligned 20,928, `.text` 74,223).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13204`.

Continuation of wave-183 w12704, wave-184 w12804, wave-185 w12904, wave-186
w13004 and wave-187 w13104 (iOS 1.1.4 structural oracle).  This pass re-diffs
the iOS Mach-O implementation and the original source-layout dumps against the
current 51,930 body, re-tests every remaining structural divergence, and runs a
fresh distance-1 (adjacent-statement) sweep over the current body.  **No
size-exact, `.rel.plt`-identical raw win was found; `src/` is left at the base.**

## Baseline (verified on this worktree)

```
process raw diffbytes    51,930
process aligned insns    20,928
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726
.text diffbytes          74,223
.rel.plt                 md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
byte_exact               4507
linked_exact             4507
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN
drift                    0
symbol parity            5370/5370
RATCHET                  PASS
```

## iOS 1.1.4 oracle re-check

`_work/bins/ios_1.1.4_HD` is the stripped armv7 Mach-O.  `LevelScript::process`
is the Thumb function at `0x134098` (93,948 B from `LC_FUNCTION_STARTS`).
Re-confirmed from the source-layout dump
(`source file list dump/gof2hd_1.1.19_ios.cpp`, `Classes/Game/LevelScript.cpp`):
the TU has exactly the same non-`process` members as the Android dynsym
(`setAutoPilotToProgrammedStation`, ctor/dtor, `resetCamera`, `lookBehind`,
`render3D`, `skipSequence`, `canSkipCutsceneNow`, `skipCutscene`,
`resetStartSequenceOver`, `startSequenceOver`, `startSequence`, `getEvent`,
`setEvent`, plus `ArrayAdd<Player*>`).  **No per-state helpers exist in either
build.**

The complete `tbh` census is unchanged from w13004: 15 in both, and the only
structural divergences are mission-0's switch range (original `6..15`, ours
`3..15` because clang 7 merges our `else if (m_nState == 3)` into the switch)
and mission-145 (original splits states 0/1 from the `2..5` `tbh`; ours uses
one `switch` 0..5).  No enum value differs and no state is split/collapsed in
mission 0/1/145.

## New structural evidence from the iOS/Android comparison

### Mission-0 state 0: `m_nState = 1` placement

Both the iOS binary (`0x134188`) and the Android original (`0x137ea4
str r0,[r4,#0x1c]`) set `m_nState = 1` **before** the player/camera
`setPosition` calls.  The wave-186 note's claim that Android sets it after is
incorrect.  Our source sets it after.  Moving it before is faithful but raw
**51,930 -> 51,934 (+4)** on the current body, so it is not landed.

### Mission-1 intro order

Both binaries emit `forceUpdate(delta,true)` first, then the angle computation
and `rotate(0,angle,angle)`, then `enemies[0]->getPosition()`, then
`setToSleep()`.  Our source order is `rotate`, `setToSleep`, `getPosition`,
`forceUpdate`.  Re-testing the full faithful order on the current body gives
raw **51,934 (+4)**; `getPosition` before `setToSleep` alone is **51,931
(+1)**; `forceUpdate` before `rotate` alone is **51,933 (+3)**.  All 24
permutations of the four actions were measured; the current order
(`rotate`, `setToSleep`, `getPosition`, `forceUpdate`) is the unique best at
**51,930**.

### Mission-145 state 0 order

Both binaries emit `new Explosion(0)`, store `m_pExplosion`, `addFireStreaks()`,
`setScaling(12.0f)`, `m_nState = 1`.  Our source has `setScaling` before
`addFireStreaks`.  Swapping them is **51,931 (+1)**.  All 24 permutations of
the three statements after `new Explosion(0)` were measured; the base order is
optimal (51,930; `0132` and `0312` tie at 51,930 but do not beat it).

### Mission-145 state 1 order

Both binaries emit the teardown in this order: `setTurretMode(false)`, then
inside `isInRocketControl()`: `setRocketControl(nullptr,nullptr)`,
`killLiberator()`, `setRumblePercentage(0.0f,0)`; then `resetCamera(m_pLevel)`,
`setFreeLookMode(false)`, `enableFirstPersonCam(false)`,
`hideShipForFirstPersonCameraView(false)`, `stopShooting(0)`,
`player->player->setVulnerable(false)`, `setVisible(false)`, `setFreeze(true)`,
`m_bCinematicBreak = 1`, `m_pRadar->enabled = 0`, `m_pHud->visible = 0`,
`setLookAtCam(true)`, `setTarget(enemies[0]->geometry)`, the `m_tempVec`
getPosition/+=18000/+=3000 sequence, `setPosition(m_tempVec)`,
`enemies[0]->setVisible(true)`, `setActive(true)`, `m_nScriptTimerA = 0`,
`m_nState++`.

Our source differs in several of these positions.  The full faithful order is
raw **51,938 (+8)**.  Individual faithful moves are all worse:
`setRumblePercentage` after `killLiberator` **+1**; `resetCamera` before
`setFreeLookMode` **+2**; `enableFirstPersonCam` before
`hideShipForFirstPersonCameraView` **+2**; `setVulnerable` before
`setVisible` **+4**; `setLookAtCam` after radar/hud **+3**.

### Mission-145 dispatch split

The original's dispatch is `cmp #1; beq state1; cbz r4,state0; b switch` and
then a `subs #2; cmp #3; tbh` for states 2..5.  Our single `switch (m_nState)`
0..5 compiles to `cmp #5; tbh`.  Several source forms were re-measured on the
current body:

| variant | raw | size | note |
|---|---:|---:|---|
| **base: single `switch` 0..5** | **51,930** | **57,232** | |
| `if (s<=1) { switch 0..1 } else { switch 2..5 }` | 54,525 | 57,236 (+4) | two switches |
| `if (s==0) {...} if (s==1) {...} else switch` | 51,930 | 57,232 | clang merges back |
| nested `default: switch (s)` | 51,930 | 57,232 | clang merges back |
| `goto` into the state-1 `if` | 51,930 | 57,232 | clang merges back |

The faithful two-switch form is a net loss (+4 B size, +2,595 raw), so it is
not landed.

### Mission-0 state-3 / state-16 layout

Moving the `m_nState == 3` block after the mission-0 switch (to force the
`6..15` switch range) gives raw **54,192**, size **57,240 (+8)**.  Moving the
state-16 handler into the switch's `default:` gives raw **55,254**, size
**57,244 (+12)**.  Both are worse than the current `if (m_nState == 16)` after
the switch.

## Fresh distance-1 sweep (current 51,930 body)

The wave-187 w13104 d1 sweep ran on the 51,968 body and the body has since
moved to 51,930 (w13101/w13102/w13103/w13105/w13107).  A fresh d1 sweep was
run over the whole `process` body (lines 605..5370):

* 1,692 adjacent single-line statement pairs (same indentation, no control
  keywords, no braces) were compiled with the exact NDK r18b `-Oz` command,
  gold-linked and measured with `tools/fn_diff.py`.
* **0 candidates beat 51,930.**  The current body is a distance-1 local
  optimum.

## Conclusion

* The iOS 1.1.4 re-diff confirms the wave-183..187 conclusions: no per-state
  helpers, no enum differences, no state split/collapse outside mission 0's
  state-3 merge and mission 145's 0/1-vs-2..5 dispatch split.
* Every remaining structural divergence re-measured on the current body is a
  raw regression; the single-switch mission-145 form and the current statement
  orders are the codegen local optimum.
* No size-exact, `.rel.plt`-identical raw win was found.  `src/` is at the base.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13204
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51930 aligned= 20928 size= 57232/57232 insn= 20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`sweep1.py`, `combo.py`, `perm.py`, `perm1.py`,
`linkcmd.txt`, `orig_process.dis`, `ios_process.dis`, `ours_process.dis`) live
in `/home/debian/GalaxyOnFire2/scratch/w13204/`; they are scratch, not tracked.
