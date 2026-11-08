# LevelScript::process — wave 145 / w9004 (the genuine +16 B: the second
# `PlayerEgo::getSpeed()` read in the state-5 brake block)

Status: **code change landed, 2026-11-08**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base: wave-144 head `eb13ada9b`.
Scratch helpers in `/dev/shm/w9004/` (not tracked): `fdiff.py` (raw same-address
byte diff), `calldiff.py` (call-target multiset), `callseq.py`, `sites.py`
(per-call-site context), `obj_src.s` (object disassembly with source lines).

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,909            54,883            (-26)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    85,994            85,968            (-26)
.ARM.extab diffbytes     1,370             1,370             (size 156,312 exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0                 0   (byte-identical, 29,392 B)
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## The missing statement: state-5 `setSpeed` re-reads `getSpeed()`

The wave-144 handoff left the **faithful set** (case-80 metric store removed +
case-91 state-3 `m_nScriptTimerA = 0` only) at 57216 B, i.e. -16 B, and asked
for a genuine +16 B statement.  The call census found it immediately:
`PlayerEgo::getSpeed` was **4 original / 3 ours**.

The original state-5 brake block (`0x138ff0..0x1390b8`) reads the speed **twice**:

```
138ff0 add  r0, sp, #368        ; &pos
138ff2 mov  r1, r5
138ff4 blx  PlayerEgo::getPosition
138ff8 mov  r0, r5
138ffa blx  PlayerEgo::getSpeed ; read #1: the == 2.0f cruise-speed test
138ffe vmov.f32 s0, #0          ; 2.0
139002 vmov s2, r0
139006 vcmp.f32 s2, s0
13900a vmrs APSR_nzcv, fpscr
13900e bne.n 13907a
...                             ; 157/158/161 sound stings
139096 mov  r0, r5
139098 blx  PlayerEgo::getSpeed ; read #2: the brake multiplier
13909c vmov s2, r0
1390a0 vldr s0, [pc]            ; 0.98f
1390a4 mov  r0, r5
1390a6 vmul.f32 s0, s2, s0
1390aa vmov r1, s0
1390ae blx  PlayerEgo::setSpeed
```

Our source cached the first read in `int speedBits` and reused it for the
`setSpeed` multiplier (one call).  The faithful source re-reads it:

```cpp
// src/game/world/LevelScript.cpp, case-5 brake block (line ~719)
 Vector zero161{0.0f, 0.0f, 0.0f};
 Globals::sound->updateEvent3DAttributes(161, &pos, &zero161, false);
-int speedBits2...                       // (before: reused `speedBits`)
-player->setSpeed(reinterpret_cast<float &>(speedBits) * 0.98f);
+int speedBits2 = player->getSpeed();
+player->setSpeed(reinterpret_cast<float &>(speedBits2) * 0.98f);
```

`getSpeed()` is an external PLT call, so clang cannot CSE the two reads and
emits the second `mov r0,r5; blx getSpeed; vmov s2,r0; vldr s0; vmul; blx
setSpeed` exactly as the original (`0x138e40..0x138e58` in ours, register
shift only).  The call census is now **4/4** for `PlayerEgo::getSpeed`.

This is a genuine, witnessed statement — not a metric-only companion.  It is
the +16 B the wave-144 handoff was looking for, so the case-91 `dockNavPtr`
metric store from wave 144 can be (and is) dropped: the landed source is the
faithful set **plus** this fix, with no non-witnessed store anywhere.

## Landed source diff (2 hunks)

```diff
@@ state-5 brake block @@
-            player->setSpeed(reinterpret_cast<float &>(speedBits) * 0.98f);
+            int speedBits2 = player->getSpeed();
+            player->setSpeed(reinterpret_cast<float &>(speedBits2) * 0.98f);
@@ case-91 state-3 tail @@
                         m_nScriptTimerA = 0;
-                        m_pRadar->dockNavPtr = nullptr;   // wave-144 metric-only
                         m_nState++;
```

## Measured variants (all built + linked, `fdiff.py`)

| variant | size | raw |
|---|---:|---:|
| HEAD (wave 144: case-80 store removed, case-91 `timer; dock`) | 57232 | 54,909 |
| faithful set (case-91 `timer` only, no dock store) | 57216 | 54,912 |
| faithful set + `m_nScriptTimerB = 0` (metric-only) | 57232 | 54,906 |
| **faithful set + second `getSpeed()` (landed)** | **57232** | **54,883** |

The landed variant is both the lowest raw diff of any 57232 B variant measured
this wave **and** fully faithful: every store and call it contains is present
in the original block.  The call multiset is exact except for the known
pre-existing deltas (`setStanding`/`setAutoTurret` PLT anchors,
`TransformGetTransform` branch sharing, and the `setPosition(Vector)` vs
`setPosition(fff)` spelling differences).

## Why the other wave-144 leads were dead ends

* The case-91 state-3 tail (`0x13fed0..0x13fede`) is already instruction-for-
  instruction identical in the faithful set (`strd r1,r1,[r,#144]; adds; str
  [r,#28]; b.w`), so the +16 B was not there.  The original's 8-vs-3 `strd`
  timer-reset census is a tail-merging artefact of the cold-block layout, not
  a missing statement.
* The case-154 block (`0x13a7e0..0x13a920`) matches ours instruction-for-
  instruction including the `(*enemies)[1]->field_0x70 = 1` store.
* `forceUpdate`/timer reorder at case-16 state-2 (+12) and `dockToDockingPoint`
  receiver (-8) were already measured and rejected in wave 144.

## Open questions

* Remaining call-multiset deltas: `TargetFollowCamera::setPosition(Vector)`
  59/57 and `setPosition(fff)` 8/10 (two sites where the original uses the
  Vector overload and we use fff, or vice versa).  Locating those two sites
  may be the next genuine source win; they were not touched this wave because
  the faithful +16 B was found first and the size must stay 57232.
* `TransformGetTransform` 37/36 (case-157 branch sharing) is still open.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9004/fdiff.py _ZN11LevelScript7processEi   # 54,883
```
