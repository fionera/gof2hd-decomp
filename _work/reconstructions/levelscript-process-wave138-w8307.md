# LevelScript::process — wave 138 / w8307 (call-census: the six missing getEnemies + the
# missing nextCampaignMission / resetGunDelay; every size-neutral landing is PLT-coupled)

Status: **analysis only, no source change landed**, 2026-11-07. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**. Owned TU: `src/game/world/LevelScript.cpp`. Base master
`96b069792` (wave-137 head). Scratch helpers in `/dev/shm/w8307/` (not tracked):
`regions2.py` (instruction difflib), `spans2.py` (per-call-span diff, pools excluded),
`multiset.py` (normalised instruction multiset), `apply.py` (named-edit harness),
`score.py`, `diffbytes.py`.

## Result

```
                         baseline (HEAD)   best exact-size candidate
official verify pct      57.4              57.4
orig / ours insns        20169 / 20032     20169 / 20030
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        55151             55217
byte_exact               4507              4506   (MGame::gameOverCheck flips)
```

**The handoff's 58.9 % is stale**: on the wave-137 head (`96b069792`) the official
difflib score of `process` is **57.4 %**, not 58.9 %.  All numbers below are
re-measured on this head.

## The body is not ~120 instructions short — it is encoding width

`spans2.py` aligns the 2,896/2,894 `blx` sites with difflib and compares the number
of *non-pool* instructions between each pair of matched calls.  The largest
original-vs-ours span excess in the whole 57 KB function is **+10 instructions**
(and that one is a cold block of another case placed between the two calls).  The
remaining excesses are +7, +7, +6, +6, +5, +5, +4, +4, +4, +3 …  There is **no
missing statement block**.  The 137-instruction total deficit is the 16/32-bit
encoding-width difference already documented by wave 137 / w8205
(ours keeps `this` in `sl`, the original spills it to `[sp,#88]`).

## What the call census does find (all verified against objdump)

`Level::getEnemies` is called **34×** in the original and only **28×** in ours.
The six missing calls are exactly three loops that our source wrote against the
cached `enemies` local instead of re-fetching `m_pLevel->getEnemies()`:

| source | loop | original |
|---|---|---|
| case 16 state 3 (line ~1712) | wormhole exit ships | `0x13f1f4` (element) + `0x13f2e0` (count) |
| case 16 state 4 (line ~1729) | hide/dead the wormhole ships | `0x140578` (element) + `0x14059c` (count) |
| case 24 state 0 (line ~1798) | clear every pilot's enemy list | `0x13ea4c` (element) + `0x13ea62` (count) |

Two more genuinely missing statements, both already identified by earlier waves but
never landed:

* **`Globals::status->nextCampaignMission(true);`** in case 144 state 3
  (`0x13d674`), between `m_nState = 4` and `setStation(status->getStation())`.
  The original has 10 calls, ours 9.  This is the call w8202/w8205 found misplaced.
* **`m_pLevel->getPlayer()->resetGunDelay();`** in the case-154 state-9 teardown
  (`0x144982`), between `m_pCamera->setFixed(false)` and `setLookAtCam(false)`.
  The original has 10 calls, ours 9.  This is w8201's open lead.

The census also confirms two *artificial* PLT-order anchors in the source that the
original does not have:

* `player->setAutoTurret(player->autoTurretIsEnabled());` (line 1813, case 24 state 1)
* `if (delta == 0x7d) status->getStanding()->setStanding(0,100);` (line 2012)

They are the reason ours emits `autoTurretIsEnabled` 2× (orig 1×),
`setAutoTurret` 3× (orig 2×), `getStanding`/`setStanding` 2× each (orig 1× each).

## Measured size deltas (each edit applied to the pristine head)

| edit | Δ size | pct | process diffbytes |
|---|---:|---:|---:|
| `turret` (remove auto-turret anchor) | −16 | 57.4 | 55175 |
| `anchor` (remove standing anchor) | −24 | 57.4 | 55227 |
| `loops` (three getEnemies loops) | +8 | 56.6 | 54936 |
| `ncm` (nextCampaignMission) | +16 | 57.4 | 54989 |
| `rgd` (resetGunDelay) | +12 | 57.4 | 54867 |
| `nstate` (drop redundant `m_nState++` at case 144) | −8 | 57.4 | — |

All three positive edits *lower* the raw byte diff (the faithful form is closer),
but the difflib pct is dominated by block placement: the `loops` edit alone drops
57.4 → 56.6, while `ncm`/`rgd` keep 57.4.

## Why no combination can be landed at 57232 B with the gate green

The function must stay **exactly 57232 B**: any other size shifts every later
function and `byte_exact` collapses (e.g. `anchor` alone → 57208 gives
`byte_exact 4507 → 3694`).

The only exact-size combination found is **`turret + ncm`** (57232 B, pct 57.4,
diffbytes 55217).  It fails the ratchet: `byte_exact 4507 → 4506` because removing
the auto-turret anchor moves the first reference to `PlayerEgo::setAutoTurret` /
`autoTurretIsEnabled` later in the TU, which re-orders `.rel.plt` and moves
`PlayerEgo::isInWormhole`'s PLT slot (`0x66c84 → 0x66c6c`); `MGame::gameOverCheck`
then has a different `blx` displacement and is no longer byte-exact.  The anchor
exists precisely to pin that PLT order.

Exact-size combinations tested (all pct 57.4 unless noted):

| combination | size | diffbytes | gate |
|---|---:|---:|---|
| **baseline** | **57232** | **55151** | green |
| `turret ncm` | 57232 | 55217 | byte 4506 (PLT) |
| `anchor loops ncm` | 57232 | 55277 | byte 3694? (anchor PLT) |
| `turret ncm nstate` | 57216 | 55187 | size |
| `turret rgd` | 57228 | 55203 | size |
| `anchor ncm rgd` | 57236 | 54936 | size |
| `ncm rgd` | 57260 | 54784 | size |

`ncm + rgd` has by far the best diffbytes (54784, −367) but is +28 B; the only
−28 companions are the two PLT anchors, and removing either re-orders `.rel.plt`.

## Next lead

Land `ncm + rgd` (+28 B) with a **PLT-safe −28 B** companion.  The candidates are
the two spurious `Level::getPlayer()` calls in ours (orig 23 / ours 25).  The
census locates the extra pair at ours `0x1402c4`/`0x1402e4` (a `setAutoPilot;
resetGunDelay; getPlayer` + `dockToDockingPoint; setDockingState; ...` sequence
with no original counterpart) and at ours `0x142d10` / `0x144db4`.  Removing a
`getPlayer()` that is not the first reference to that symbol is PLT-safe and worth
≈8 B each.  Two of them plus `nstate` (−8) would close the +28 without touching
either anchor.  This was not completed in the wave-138 time box.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8307/spans2.py                        # per-call-span diff (pools excluded)
python3 /dev/shm/w8307/multiset.py                      # register-allocation census
python3 /dev/shm/w8307/apply.py turret ncm              # exact-size candidate (byte 4506)
python3 /dev/shm/w8307/score.py                         # 57.4 %, 57232 B
```
