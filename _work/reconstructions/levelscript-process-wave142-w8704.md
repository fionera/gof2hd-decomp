# LevelScript::process — wave 142 / w8704: state-4/5 translate swap + state-4 teardown move

Status: **code change landed, 2026-11-08**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `1d10d0694` (wave-141 docs head).
Scratch helpers in `/dev/shm/w8704/` (not tracked): `fdiff.py`, `dis.py`, `regions.py`,
`shiftmap.py`, `matchruns.py`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,108            54,108            (0)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
RATCHET                  PASS              PASS
```

The landed change is a **genuine semantic fix**, not a metric win: the two
`m_pCamera->translate` calls in the mission-0 state machine were swapped
between state 4 and state 5.

```
orig state 4 (0x138de6..0x138df0):
    ldr r1,[pc] ; 25000.0f
    ldr r2,[pc] ; -200.0f
    ldr r3,[pc] ; -1000.0f
    blx TargetFollowCamera::translate
orig state 5 (0x1390be..0x1390f2, inside messages[13]->isOver()):
    vmov s0,r6 ; vldr d17(0.3) ; negs r0,r6 ; vldr s2(0.2)
    ... (float)((double)delta*0.3), 0.0f, (float)-delta*0.2f
    blx TargetFollowCamera::translate
```

Our source had the constant translate in state 5 and the delta translate in
state 4. Swapping them is size-neutral (the +30 B constant→delta in state 5 is
exactly offset by the −30 B delta→constant in state 4) and leaves raw
same-address diff unchanged at 54,108.

## NEW experiment: state-4 teardown move (`move4b_cached`) — -19 raw, but .rel.plt

Moving the four leading state-5 calls (`setFreeLookMode`, `enableFirstPersonCam`,
`hideShipForFirstPersonCameraView`, `removeAllGuns`) into state 4 after
`resetCamera`, and using the cached `player` for `setCollide`, is size-neutral
and lowers raw diff:

| variant | size | raw diff | byte_exact | note |
|---|---:|---:|---:|---|
| baseline | 57232 | 54108 | 4507 | |
| **move4b_cached (m4bc)** | **57232** | **54089** | **4503** | .rel.plt perturbed |
| trswap (landed) | 57232 | 54108 | 4507 | |
| trswap + m4bc | 57224 (−8) | 55100 | — | |
| trswap + m4bc + swap45 | 57240 (+8) | 55139 | — | |
| m4bc + swap45 | 57240 (+8) | 55129 | — | |

`m4bc` is **not landable**: it drops `byte_exact` 4507 → 4503 because it
perturbs `.rel.plt`.  The `-s -j .rel.plt` diff shows `PlayerEgo::getSpeed`
moving from relocation index 2812 to 2808 (ahead of the four calls).  The
original's first references are ordered `setFreeLookMode, enableFirstPersonCam,
hideShipForFirstPersonCameraView, removeAllGuns, getSpeed`; after `m4bc` the
state-5 block (laid out first) reaches `getSpeed` before state-4's four calls.
The four affected functions are the usual MGame PLT-order casualties.

`swap45` (state-4 branch before state-5, matching the original's block order)
does restore the four-call/getSpeed order but moves a different pair:
`PlayerStation::setVisible` / `PlayerEgo::deleteHackingGame` shift from
relocations 2836-2837 to 2832-2833.  So `m4bc+swap45` still perturbs `.rel.plt`.

## NEW observation: the function is 94.5 % byte-different

`regions.py` reports 1,711 diff runs / 54,108 bytes.  `shiftmap.py` (best local
shift per 256 B window) shows most of the 0x138e18..0x145700 middle is not a
simple shift — it is a global register-map difference.  The only large matching
runs at the same address are `0x1389d3..0x138a63` (state 3), `0x1457d6..0x1459c6`
and `0x14580b..0x145869` (function tail).  The prologue register map is the
biggest lever: the original has `r4=status, r5=messages`; ours has
`r9=status, r4=messages`.  Reordering the accessors to `messages, player,
enemies` reproduces `r5=messages` but leaves status in `r9` (clang's allocator
choice) and costs +88 B.

## Open leads

1. `m4bc` is a real −19 raw win blocked only by `.rel.plt`.  A size-neutral
   `.rel.plt`-preserving companion (or a way to make `getSpeed`'s first
   reference follow the four calls without `swap45`) would land it.
2. The `messages/status` prologue is still a 4-byte lever (`ldr r4,[r0]` +
   `ldr r0,[r4]` vs `ldr.w r9,[r0]` + `ldr.w r0,[r9]`); it needs a −88 B
   companion in the shared tail.
3. The state-4/5 branch layout (`swap45`) is the faithful order but costs +8 B.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8704/fdiff.py _ZN11LevelScript7processEi   # 54108
```
