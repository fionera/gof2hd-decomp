# LevelScript::process — wave 138 / w8305 (case-125 state machine from Thumb; freeze reorder landed)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `96b069792` (wave-137 docs).

## Result

```
                         before (master)   after (landed)
official verify pct      58.9              57.4
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        54327             55145
whole .text diffbytes    85535             85529
.ARM.extab size          156312            156312   (size/base exact; 1395 diff)
.ARM.exidx               exact             exact
.rodata/.data/.rel.plt   exact             exact
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The fuzzy ratio drops 58.9 -> 57.4 because the freeze reorder re-rolls the 57 KB block
layout; the hard gate is unchanged and the function stays exactly 57,232 B with
`.ARM.extab` size- and base-exact.

## New hypothesis tested — the exact case-125 state machine

The original case-125 dispatch is **two if/else-if chains**, not one chain and not a
`switch` (Thumb, original base `0x137d18`):

```
139f9c  state 0 test: cmp r1,#0 ; bne 13b43e
139fba  state 0 body (inline)  ...  b.w 13b518   ; skip the state-1 test
13b43e  state 1 test: cmp r1,#1 ; bne 13b518
13b444  state 1 body  ...  falls through to 13b518
13b518  state 2 test: cmp r0,#2 ; bne 13dcb6
13b522  state 2 body  ...  b.w 144a0c   ; shared m_nState++ tail -> 144e36
13dcb6  state 3 test: cmp r0,#3 ; bne 13de44
13dcbc  state 3 body  ...  b.w 144e36
13de44  default range: subs r0,#4 ; cmp r0,#2 ; bhi 144e36
13de4c  state 4..6 body  ...  b.w 144e36
```

That is exactly:

```cpp
if (m_nState == 0 && msg1->isTriggered()) { state0; }
else if (m_nState == 1) { state1; }
if (m_nState == 2) { state2; }
else if (m_nState == 3) { state3; }
else if (m_nState >= 4 && m_nState <= 6 && ...) { state4..6; }
```

The state-0 body jumps to the state-2 test because state 0 and state 1 are mutually
exclusive; state 1 falls through to state 2; state 2 is an `if` paired with the state-3
`else if`, so its body leaves the chain.  The shared `m_nState++` at `0x144a0c` is the
compiler's merged increment tail.

## Why the faithful shape does not land (LSDA scope)

The process LSDA is at `.ARM.extab 0x1c2af8` (personality `0x5eb60`), header at `+0x10`
(`ff ff 01`, 46 call-site entries).  The extab entry is
`4 (personality) + 12 (compact prefix) + 5 (header) + table + pad`, 4-byte aligned, so
**the call-site table must be <= 0x147 bytes** to keep the 0x15c-byte entry (and therefore
`.ARM.extab` at 156,312 B).  The master switch shape is `0x146`; all faithful case-125
shapes overflow:

| variant | process size | LSDA table | extab |
|---|---:|---:|---:|
| master switch + base loop | 57232 | 0x146 | 156312 |
| two-chain (state 0/1 then 2/3/4..6) | 57216 | 0x148 | 156316 |
| two-chain + freeze reorder | 57216 | 0x148 | 156316 |
| single else-if chain | 57368 | 0x14d | 156320 |
| nested state-0 (`if (state==0) { if (msg) ... }`) | 57208 | 0x14b | 156316 |
| switch 0..3 + default | 57344 | 0x14b | 156316 |

The two-chain compiler layout outlines the state-0 body to `0x13c17e` (rel `0x4466`),
pushing the `new Route` call site past `0x4000` (3-byte uleb start).  The nested form keeps
the Route call inline at `0x139d86` (rel `0x206e`) but moves the state-1 `String`
temporaries to `0x485e` (rel `0x4b46`), adding six 3-byte starts.  The original keeps state
0 inline at rel `0x22a2` and state 1/2 at rel `0x3726`/`0x3800`, all below `0x4000`.

## Landed change

Two source edits, both faithful/equivalent to the Thumb:

1. **Freeze reorder.**  The original teardown calls `setVisible(false)` at `0x13b738`
   and `setFreeze(true)` at `0x13b740`; the source had `setFreeze(true)` before the
   hud/radar/camera stores.  Moving it after `setVisible(false)` matches the original.
   On the switch shape this is -8 B (57232 -> 57224).
2. **State-3 advance.**  The original writes `m_nState = 4` at `0x13dd22`; the source
   now uses the semantically identical `m_nState++`.  This restores exactly 8 B, landing
   at 57232 B, and leaves the LSDA table at `0x146`.

The state-2 ambush loop keeps the base declaration order.  The original order is
`awake, pirate, playerPos, sideZ, offsetY, offsetZ, sideX` (`0x13b578..0x13b5c6`), but
reordering the declarations to that order is -16 B on the else-if shape and **+168 B** on
the switch shape (57400 B), so it cannot be landed without a -168 B companion.

## Open questions

- The original's state-0 body is inline and state 1/2 follow it below `0x4000`; our
  compiler swaps the two cold blocks (state 0 outlined when state 1 is inline, and vice
  versa).  Matching the original LSDA offsets needs a global block-order change, not a
  case-125 statement reorder.
- The faithful loop reorder needs a -168 B companion on the switch shape.
- The state-2 body has no throwing call sites, so it only contributes to the big no-pad
  range; its statement order cannot shrink the call-site table.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
python3 tools/verify/verify.py --build-dir /dev/shm/w8305/verify_var \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
```
