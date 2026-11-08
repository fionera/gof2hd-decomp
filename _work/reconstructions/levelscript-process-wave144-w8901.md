# LevelScript::process — wave 144 / w8901 (m4bc does NOT reproduce on the
# wave-143 base; the +24 B variant is swap45+D_cached, and it perturbs .rel.plt)

Status: **code change landed (METRIC-ONLY, -3 raw)**, 2026-11-08. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, size-exact. Owned TU: `src/game/world/LevelScript.cpp`.
Base: wave-143 head `f5093474f` (raw 54,619, byte_exact 4507).
Scratch helpers in `/dev/shm/w8901/` (not tracked): `harness.py`, `fdiff.py`,
`fdiff2.py`, `runs.py`, `shiftmap.py`, `shiftsum.py`, `callseq2.py`,
`reldiff.py`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,619            54,616            (-3)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The landed edit is the **removal of a dead `speedBits = player->getSpeed();`
re-read** in the mission-0 state-5 block (source line ~720).  It is
**METRIC-ONLY**: the original has four `getSpeed` calls and the re-read is one
of them (census after removal is 4 orig / 3 ours), so the source statement is
faithful even though it is a dead store.  The raw win is only 3 bytes.

## The headline: m4bc is NOT -244 raw on the wave-143 base

The wave-143 w8801 note measured `m4bc` (move the four leading state-5 calls
`setFreeLookMode` / `enableFirstPersonCam` /
`hideShipForFirstPersonCameraView` / `removeAllGuns` into state 4 after
`resetCamera`, cached `player` for `setCollide`) at **57256 B / raw 54,447** on
the wave-142 head.  On the wave-143 head (`f5093474f`) the *same* edit is
**size-neutral and raw-neutral**: 57232 B, raw **54,619** (identical to HEAD).
The wave-143 w8801/w8802/w8803/w8804 changes to the mission>=2 switch changed
the raw metric enough that the wave-142 numbers no longer hold.

Measured variants on the wave-143 base (all built + measured):

| edit | size | raw diff |
|---|---:|---:|
| HEAD | 57232 | 54,619 |
| **m4bc** (four calls after resetCamera, removeAllGuns early, cached setCollide) | 57232 | **54,619** |
| m4bc with removeAllGuns in the original position (after setComputerControlled) | 57232 | 54,620 |
| m4bc with fresh `m_pLevel->getPlayer()` for setCollide | 57240 | 54,978 |
| move only removeAllGuns | 57232 | 54,622 |
| move only setFreeLookMode+hideShip+removeAllGuns | 57232 | 54,619 |
| move only 3 calls (no removeAllGuns) | 57224 | 55,134 |
| swap45 (state-4 branch before state-5 branch) | 57264 | 54,859 |
| **D_cached + swap45** (faithful state-4 order, branch swap) | **57256** | **54,585** |

So the only +24 B shape is `D_cached + swap45` (57256, raw 54,585), not
54,447.  That is the closest reproduction of the w8801 "m4bc +24 B" claim.

## Why m4bc cannot be landed: .rel.plt

`m4bc` moves the four calls from the state-5 block (laid out at `0x138d40`,
before the state-5 `getSpeed` at `0x138d5c`) into the state-4 block (laid out
at `0x139080`, after `getSpeed`).  The `.rel.plt` first-reference order is
therefore perturbed:

```
original / HEAD .rel.plt idx 2807..2811:
    setFreeLookMode, enableFirstPersonCam, hideShip, removeAllGuns, getSpeed
m4bc (D_cached) .rel.plt idx 2807..2811:
    getSpeed, setFreeLookMode, enableFirstPersonCam, hideShip, removeAllGuns
```

`byte_exact` drops **4507 -> 4503** (the usual four MGame PLT-order
casualties), exactly as w8801 reported.  `swap45` restores the four-call /
getSpeed order but moves a different pair
(`PlayerStation::setVisible` / `PlayerEgo::deleteHackingGame`, idx 2831..2836),
so `D_cached + swap45` still gives `byte_exact` **4504**.

`reldiff.py` (in `/dev/shm/w8901/`) decodes the `.rel.plt` symbol order with
`arm-linux-androideabi-readelf -r`; use it to check any future companion.

## No -24 B companion found that composes to 57232 with byte_exact 4507

Every -24 B companion tried either fails to compose or breaks the census /
`.rel.plt`:

| companion on `D_cached+swap45` (57256) | size | raw | note |
|---|---:|---:|---|
| `m144` setStation removal | 57232 | 54,647 | unfaithful (census setStation 6->5) |
| particle-block `m_nScriptTimerA += delta` reorder (timer_after_if) | 57252 | 54,638 | only -4, not -12, on this base |
| timer_after_pos | 57244 | 55,008 | |
| c91 companion revert (w8804) | 57248 | 55,115 | |
| timer + m144 | 57228 | 54,608 | size -4, breaks size-exact |
| timer + c91 + m144 | 57220 | 54,982 | |

The `m144` row is the only exact 57232 composition, but it is **unfaithful**
and raises raw to 54,647.  `D_cached+swap45+timer+m144` is 57228 / raw 54,608
(the best raw at near-57232) but is 4 B short of size-exact; a genuine +4 B
faithful statement was not found (every added store/assignment measured +8).

## Other observations

* The prologue accessor order is `enemies, player, messages` (source order);
  the original is `messages, player, enemies`.  Reordering the three
  declarations to `messages, player, enemies` makes the first ~30 prologue
  instructions byte-identical (only `status` stays in `r9` instead of `r4`),
  but costs **+76 B** (57308) and raw rises to 54,831.  A `Level *level =
  m_pLevel;` local alone is **-76 B** (57156, raw 55,188).  Neither is
  size-neutral.
* The function is 94% byte-different; per-window best-shift analysis
  (`shiftsum.py`) shows 13,777 bytes of the 54,283 diff in
  `0x137d18..0x145aa0` are pure block-placement shift.  The mission-26 block
  is placed at `0x1384e8` in ours but `0x1382ea` in the original, and the
  mission-95/99 chain A sits between state 4 and state 5 in the original but
  before state 4 in ours.
* `case 70` `break;` is a real source fix but is **+8 B** on this base
  (57240, raw 55,159), so it cannot be landed without a -8 companion.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8901/fdiff.py _ZN11LevelScript7processEi   # 54,616
python3 /dev/shm/w8901/reldiff.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

## Open questions

* Find a `.rel.plt`-preserving way to place state 4 before state 5 without
  moving `PlayerStation::setVisible` / `PlayerEgo::deleteHackingGame`
  (`D_cached+swap45` is 57256 / 54,585 but byte_exact 4504).
* Find a genuine +4 B faithful statement to land
  `D_cached+swap45+timer+m144` at 57232 / raw 54,608.
* The mission-26 / chain-A block placement is the biggest shift lever
  (1,340 bytes in `0x138500..0x138c00`); a source reorder that reproduces the
  original's mission-26-before-state-3 order would be worth far more than any
  of the above.
