# LevelScript::process — wave 143 / w8801 (extra-call census; m4bc is a
# -244 raw lead at +24 B; landed the -3 size-neutral move4b)

Status: **code change landed (METRIC-ONLY)**, 2026-11-08. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, size-exact. Owned TU: `src/game/world/LevelScript.cpp`.
Base: wave-142 head `bf083ab41` (byte_exact 4507, raw 54,691).
Scratch helpers in `/dev/shm/w8801/` (not tracked): `fdiff.py`, `census.py`,
`count2.py`, `callseq.py`, `sig.py`, `sig2.py`, `objmap.py`, `harness.py`.

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,691            54,688            (-3)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The landed edit is a 3-line reorder in the case-16 state-1 teardown
(source line ~1591): `setSpeed(0.0f); removeAllGuns(); setComputerControlled(true);`
instead of `setComputerControlled(true); setSpeed(0.0f); removeAllGuns();`.
It is **METRIC-ONLY**: the original at `0x13aa58..13aa66` is
`setComputerControlled, setSpeed, removeAllGuns`, i.e. the pre-change order.
The coordinator may prefer the faithful source; the raw win is only 3 bytes.

## The extra-call census (re-measured on wave-142 HEAD)

The prompt's "call census exact" holds for the four named symbols
(`getEnemies 34=34`, `getPlayer 23=23`, `setStation 6=6`,
`nextCampaignMission 10=10`). The remaining multiset deltas are:

| symbol | orig | ours |
|---|---:|---:|
| `TargetFollowCamera::setTarget(AEGeometry*)` | 81 | **82** |
| `AEGeometry::setPosition(Vector const&)` | 23 | **24** |
| `PlayerEgo::getPosition()` | 41 | **42** |
| `TargetFollowCamera::translate(fff)` | 57 | **58** |
| `PaintCanvas::TransformGetTransform` | 37 | **36** (ours *missing* one) |
| `Status::getStanding` / `Standing::setStanding` | 1 / 1 | 2 / 2 (PLT anchors) |
| `PlayerEgo::setAutoTurret` / `autoTurretIsEnabled` | 2 / 1 | 3 / 2 (PLT anchors) |

So the four "one too many" calls are the **camera/ship** group
`setTarget + PlayerEgo::getPosition + AEGeometry::setPosition + translate`,
not the `TargetFollowCamera::setPosition` group (that one is 59/59 exact).
The `TransformGetTransform` deficit is a clang tail-merge: the original at
`0x141164` and `0x141184` has two separate `TransformGetTransform` calls
(`[r2,#12]` and `[r2,#20]`), while ours shares one call at `0x1411fa`
(`b.n` from the first arm). That is codegen, not a missing source statement.

## Measured variants (all built + linked on wave-142 HEAD)

| edit | size | raw diff | note |
|---|---:|---:|---|
| HEAD | 57232 | 54,691 | |
| **move4b (landed)** | **57232** | **54,688** | −3, unfaithful call order |
| state-4 re-fetch revert (`rev`) | 57232 | 54,678 | −13, but `getEnemies` 33 |
| faithful reorder of `m_nScriptTimerA += delta` in the particle block (source 2159-2165) | 57220 | 55,041 | −12 B, raw +350 |
| remove `m_pCamera->setPosition(player->getPosition())` (state-1) | 57208 | 55,044 | |
| remove `m_pCamera->setTarget(player->geometry)` (state-1) | 57224 | 55,172 | |
| remove `m_pCamera->translate(1000,700,1000)` (state-1) | 57252 | 54,895 | |
| remove whole state-1 camera block (3 calls) | 57168 | 55,321 | |
| remove case-144 `setStation` (`m144`) | 57208 | 54,771 | −24 B, unfaithful |
| revert case-157 tail store reorder | 57240 | 54,772 | |
| **m4bc** (state-4 teardown move, w8704) | **57256** | **54,447** | **−244 raw, +24 B** |
| m4bc + `rev` | 57256 | ~54,434 | |
| m4bc + `m144` | 57208 | 54,771 | |
| m4bc + faithful reorder2160 | 57220 | 55,041 | |

## The big lead: `m4bc` is −244 raw at +24 B

Moving the four leading state-5 calls (`setFreeLookMode`,
`enableFirstPersonCam`, `hideShipForFirstPersonCameraView`, `removeAllGuns`)
into state 4 after `resetCamera`, and using the cached `player` for
`setCollide`, is a **faithful** reshape (it matches the original's block
order) and drops the raw same-address diff from 54,691 to **54,447 (−244)**.
It costs exactly **+24 B** (57256). The `m144` `setStation` removal is also
−24 B but is unfaithful (the original has the call), and the combination
`m4bc + m144` lands at 57208 (not 57232) and *raises* raw to 54,771 — the
two edits do not compose additively because of padding/alignment.

**Next wave:** find a genuine −24 B companion that composes with `m4bc`
(possibly a second faithful reorder like the particle-block one, which is
−12 B, plus another −12 B), then `m4bc + companion` should land at 57232
with raw ≈ 54,447. The state-4 re-fetch revert is size-neutral but breaks
the `getEnemies` census (33) and only gives −13.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8801/fdiff.py _ZN11LevelScript7processEi   # 54,688
python3 /dev/shm/w8801/count2.py                             # census table
```
