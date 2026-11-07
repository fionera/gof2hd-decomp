# LevelScript::process — wave 140 / w8602 (w8506 case-125 shapes re-measured on the
# post-w8501/w8505 base: the 54,058 win does not transfer; master stays best)

Status: **note only, source reverted to master**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `e18e0c2a4` (wave-140 docs head).
Scratch helpers in `/dev/shm/w8602/` (not tracked): `fdiff.py` (raw same-address diff),
`lsdatab.py` (ARM EHABI LSDA parser), `widths.py` (per-entry uleb-width diff),
`relplt.py` (`.rel.plt` row diff), `gen2.py`/`gen4.py`/`gen5.py`/`gen7.py`/`gen8.py`
(case-125 shape generators), `gen91.py` (case-91 nested form), `sweep.py`/`sweep5.py`,
`micro.py`/`micro2.py` (statement-order probes).

## Baseline (current master, unchanged and gate-green)

```
process raw diffbytes    54,138       (57232 / 57232, size-exact)
LSDA call-site table     0x146        (46 rows; original 0x144)
.ARM.extab               0x26298      (size exact)
.rel.plt                 byte-identical (0 differing rows)
byte_exact / linked      4507 / 4507
```

## The w8506 result reproduced exactly — on the OLD base

`d4857f174` (the w8506 base) + `s1first_cases` + the R2 receiver fix
(`player->geometry->setDirection(-dir, ...)` at the case-90 block) reproduces the
w8506 note bit-for-bit:

```
old base + s1first + R2:  size 57232, raw 54,058, table 0x148, .rel.plt 6 rows
```

So the generator is faithful to w8506.  The current base already contains R2 (landed
by w8501, `197f40791`) *and* the w8501 case-125/90 receiver fixes and the w8505
mission-0 data-pointer move.  Those changes are why the same source shape no longer
wins.

## The key new result: on the current base every w8506 shape is WORSE

All rows below are the current base, `size` = process bytes, `raw` = same-address
diff bytes, `tab` = LSDA call-site table length (landable iff `<= 0x147`),
`rel` = `.rel.plt` differing rows.

| variant | size | raw | tab | rel |
|---|---:|---:|---:|---:|
| **master (`if(0&&msg)` + `switch(1,2,3,default)`)** | **57232** | **54,138** | **0x146** | **0** |
| `s1first` (`if(1) else if(0&&msg)` + `switch(2,3,default)`) | 57240 | 54,879 | 0x148 | 6 |
| `s1first` + case-91 nested | 57228 | 55,129 | **0x146** | 6 |
| `twochain` (faithful w8408 two-chain) | 57224 | 55,077 | 0x146 | 2 |
| `twochain_elseif` (one if/else-if chain) | 57248 | 54,850 | 0x148 | 6 |
| `twochain_elseif` + case-91 nested | 57236 | 54,791 | **0x146** | 6 |
| `s0pre_if1_sw234` (`if(0&&msg)`, `if(1)`, `switch(2,3,default)`) | 57224 | 55,077 | 0x146 | 2 |
| `s0pre_if1_sw234` + case-91 nested | 57228 | 55,137 | **0x144** | 2 |
| `nested_cases` (`if(0){if(msg)} else if(1)` + switch) | 57240 | 54,943 | 0x14b | 6 |
| case-91 nested alone (master case 125) | 57244 | 55,166 | **0x144** | 0 |
| `switch` order `2,1,3,default` | 57232 | **54,089** | 0x14b | 0 |
| `switch` order `2,3,1,default` | 57232 | 54,089 | 0x14b | 0 |
| `switch` order `3,1,2,default` | 57232 | 54,212 | 0x14b | 0 |
| `switch` with `case 0` (any order) | 57372 | 55,338 | 0x14b | 0 |

The `switch` case order only changes codegen when **case 2 is first**; every other
permutation is byte-identical to master (`sw:1,2,default,3`, `sw:1,default,2,3` both
give 54,138 / 0x146).

## Why `s1first` loses here (the interaction)

The w8506 win was `-204` on a base whose case-91 state-0 body was already inline.
On the current base the case-91 body sits at `0x495e` (outlined, past the `0x4000`
uleb boundary), while the case-125 state-1 String loop sits at `0x39f6`.  The
`s1first` re-roll moves the state-1 body *before* the state-2 body and pushes the
String loop to `0x490a`/`0x4d84`, so:

- the process grows `+8` (57240) — the extra `b.w`/pool padding the w8506 note saw
  in the opposite direction;
- entries 16/18/19 of the call-site table cross `0x4000` (`+3`), entry 12/14 are
  structurally `+1` each, entry 15 is `-1` -> table `0x148` (needs `0x147`).

Applying the faithful case-91 nested form (which restores the original's inline
`setName(getText(3211))` and gives the **exact 0x144 table**) brings `s1first` back
to `0x146`, but at `55,129` raw — i.e. the table is fixed but the block-layout win
is gone.

## The only landable sub-54,138 shape is not faithful

Reordering the two camera calls in the case-125 state-3 body

```cpp
m_pCamera->setLookAtCam(false);
m_pCamera->setTarget(player->geometry);
```

to `setTarget` then `setLookAtCam` gives **raw 54,128 (-10)**, size 57232, table
0x146, `.rel.plt` byte-identical.  But the original's Thumb at `0x13ddc8/0x13ddd2`
is `setLookAtCam` then `setTarget`, so the swap is semantically wrong (both mutate
camera state).  Left unlanded.

Statement-order micro-probes that are codegen-neutral or worse:
`m_nState++` vs `m_nState = m_nState + 1` (identical), `2*delta` vs `delta*2`
(identical), `setPlayerRoute`/`setRoute` swap (+1), `setTurretMode`/`resetCamera`
swap (+16 size), `m_pHud`/`m_pRadar` store swaps (identical), state-2 loop
declaration order `awake,pirate,playerPos,sideZ,offsetY,offsetZ,sideX` (+132 size).

## Conclusion / open question

The w8506 hypothesis ("case-125 if/else shape buys 204 raw bytes") is base-specific:
it was measured before w8501's receiver fixes and w8505's mission-0 move.  On the
current base the switch lowering is already the best block layout, and every
if/else shape either grows the function or pushes the call-site table past `0x147`.
The remaining lever is not case 125 but the **early block order**: the first
divergence from the original is at `0x6b2` (the mission-type dispatch), and the
original keeps the case-91 state-0 body inline at `0x2f82` between the case-125
state-0 and state-1 bodies, while ours outlines it to `0x495e`.  Matching that
placement (not the case-125 chain shape) is what would drop the table to `0x144`
*and* move the raw diff.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8602/fdiff.py _ZN11LevelScript7processEi
python3 /dev/shm/w8602/lsdatab.py cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi | head -2
python3 /dev/shm/w8602/relplt.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# shape sweep:  python3 /dev/shm/w8602/sweep.py   (switch/if permutations)
#               python3 /dev/shm/w8602/sweep5.py  (named shapes)
```
