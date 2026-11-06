# LevelScript::process — wave 125 / w7506 (state-3/4/5 fidelity fixes)

Status: **code change landed, 2026-11-06**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Base master `02513de33` (wave-124 final). Owned TU:
`src/game/world/LevelScript.cpp`.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.250192743      60.265114775     (+0.014922032)
official verify pct      60.3              60.3             (same, rounded)
orig / ours insns        20169 / 20040     20169 / 20040
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
process .text diffbytes  54619             54615            (-4)
whole .text diffbytes    85975             85971            (-4)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
.ARM.exidx diffbytes     0                 0                (byte-identical)
.ARM.extab diffbytes     0                 0                (size/base exact)
.rel.plt diffbytes       0                 0                (byte-identical)
.rodata/.data/.bss       0                 0
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the
linked `.so` (helper `/dev/shm/w7506/score.py`).

## Landed changes (mission-0 state 3/4/5 blocks, first 140 lines)

All four edits are local statement reorderings / constant corrections in the
mission-0 intro state machine.  They add/remove no call, symbol or literal and
keep the body exactly 57232 B; `.rel.plt` stays byte-identical.

1. **State 3 teardown: `m_pRadar->enabled = 1` before the `getPlayer()` /
   `setCollide(true)` pair.**  Original `0x138a2c..0x138a3e`:

   ```
   138a2c  ldr.w r0, [sl, #212]    ; m_pRadar
   138a30  strb.w r5, [r0, #72]    ; enabled = 1
   138a34  ldr.w r0, [sl, #24]     ; m_pLevel
   138a38  blx   Level::getPlayer()
   138a3c  movs  r1, #1
   138a3e  blx   PlayerEgo::setCollide(bool)
   ```

   The source had `m_pLevel->getPlayer()->setCollide(true);` first, then
   `m_pRadar->enabled = 1;`.  Swapped to the binary order.

2. **State 3 teardown: `m_bStartSequence = 0` before `field_0x12 = 1`.**
   Original `0x138a5a..0x138a5e`:

   ```
   138a5a  strb.w r4, [sl, #32]    ; m_bStartSequence (this+0x20) = 0
   138a5e  strb.w r5, [sl, #18]    ; field_0x12 (this+0x12) = 1
   ```

   The source had the two stores in the opposite order; clang followed the
   source order.  Swapped.

3. **State 4: `play(143)`, not `play(161)`.**  Original `0x138d4c`:

   ```
   138d4c  movs r1, #143           ; 0x8f
   138d50  movs r2, #0
   138d52  movs r3, #0
   138d54  blx  FModSound::play(int, Vector*, Vector*, float)
   ```

   The source comment already said "stop event 143's radio sting and replay
   it", but the code played 161.  Fixed to 143.

4. **State 5: `play(161)`, not `play(143)`.**  Original `0x13906e`:

   ```
   13906e  movs r1, #161           ; 0xa1
   139070  movs r2, #0
   139072  movs r3, #0
   139074  str  r6, [sp, #0]
   139076  ldr  r6, [sp, #76]
   139078  blx  r9                 ; FModSound::play(161,...)
   ```

   The source had 143 here (the two constants were swapped between states 4
   and 5).  Fixed to 161.

## What was tried but not landed

The original's state 4 block also contains four calls that our source still
has in state 5 (between `resetCamera` and `m_bCinematicBreak`):

```
138d70  blx PlayerEgo::setFreeLookMode(bool)
138d7a  blx TargetFollowCamera::enableFirstPersonCam(bool)
138d82  blx PlayerEgo::hideShipForFirstPersonCameraView(bool)
138dac  blx Player::removeAllGuns()
```

Moving them to state 4 (and removing them from state 5) is size-exact
(57232 B) and scores **60.270088786** (state-5 `play(161)` included), but it
moves `PlayerEgo::getSpeed` earlier in the TU's first-reference order.  The
`.rel.plt` order changes from

```
... setFreeLookMode, enableFirstPersonCam, hideShip, removeAllGuns, getSpeed ...
```

to

```
... getSpeed, setFreeLookMode, enableFirstPersonCam, hideShip, removeAllGuns ...
```

and `byte_exact` drops **4506 -> 4502** (four `MGame` functions lose raw-byte
exactness).  The four-call move is therefore blocked on a `.rel.plt` anchor;
it is left for a later pass.  The landed variant keeps the four calls in state
5 and only fixes the two `play` constants and the two state-3 store orders.

Other measured variants (all reverted):

| variant | raw % | size |
|---|---:|---:|
| baseline | 60.250192743 | 57232 |
| **landed (state-3 order + play 143/161)** | **60.265114775** | **57232** |
| + move four calls to state 4 (state-5 play 161) | 60.270088786 | 57232 (`.rel.plt` differs) |
| + move four calls to state 4 (state-5 play 143) | 60.275062797 | 57232 (`.rel.plt` differs) |
| state-4 `setCollide` via `m_pLevel->getPlayer()` | 59.093282933 | 57240 |
| mission-1 `player->rotate` instead of `getPlayer()` | 57.467031600 | 57328 |
| faithful prologue `messages,player,enemies` | 58.772017116 | 57416 |
| state blocks order `345` / `435` / `453` / `534` / `543` | 57.41 / 57.30 / 59.22 / 54.95 / 54.95 | 57264 / 57248 / 57240 / 57228 / 57228 |

## Remaining root cause

Unchanged: (1) prologue register map (`messages` r4 vs original r5,
`&Globals::status` r9 vs original r4, `this` in sl vs original `[sp,#88]`);
(2) MachineBlockPlacement sub-block permutation inside the big cases.  The
state-3/4/5 fixes above are local content/order corrections; they do not move
the prologue allocation.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields
cmake --build cmake-build-match --target lint           # CLEAN
python3 /dev/shm/w7506/score.py                         # 60.265114775
```

Scratch helpers (`score.py`, `firstdiff.py`, `calldiff.py`, variant `.py`
files) live in `/dev/shm/w7506/` and are not part of the tree.
