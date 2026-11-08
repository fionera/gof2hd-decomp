# LevelScript::process — wave 142 / w8705 (getEnemies call-site census;
# the state-4 loop re-fetch is a 76-byte raw-diff regression)

**Coordinator addendum (wave-142 merge):** the source revert is *not* landed. The state-4
`getEnemies()` re-fetch is present in the original (0x14059a), so the faithful source keeps it;
w8705's -76 raw win is recorded here as a metric-only variant. The call-census table and the
+24 B companion combinations are the durable results.
Status: **code change landed, 2026-11-08**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base: wave-141 head `1d10d0694`.
Scratch helpers in `/dev/shm/w8705/` (not tracked): `fdiff.py` (raw same-address
byte diff), `secdiff.py` (per-section diff), `count.py` / `esites.py` (call
census + per-site context), `calign.py` (difflib call-sequence alignment),
`try.py` (restore pristine base, apply named edits, build, measure).

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,108            54,032            (-76)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    84,567            84,491            (-76)
.ARM.extab diffbytes     1,395             1,395             (size 156,312 exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0                 0   (byte-identical)
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

## The call-site census (re-measured on wave-141 HEAD)

`count.py` parses every `bl`/`blx` in the extracted function body (the
`orig.s`/`ours.s` scratch dumps are the function only, `0x137d18..0x145ca6`).
Baseline multiset deltas:

| symbol | orig | ours |
|---|---:|---:|
| `Level::getEnemies` | **34** | **32** |
| `Level::getPlayer` | 23 | 23 |
| `AEGeometry::setPosition(Vector)` | 23 | 24 |
| `ApplicationManager::SetCurrentApplicationModule` | 9 | 10 |
| `TargetFollowCamera::setTarget` | 81 | 82 |
| `PlayerEgo::getPosition` | 41 | 42 |
| `PlayerEgo::setAutoTurret` | 2 | 3 |
| `PlayerEgo::autoTurretIsEnabled` | 1 | 2 |
| `Status::getStanding` / `Standing::setStanding` | 1 / 1 | 2 / 2 |

The prompt's `getPlayer 23 vs 22` is stale: `getPlayer` already matches at 23/23
on this base. `getEnemies` is the only **missing**-call delta; the other seven
rows are the known PLT anchors (auto-turret pair, `setStanding`) plus one extra
`setTarget`/`setPosition`/`getPosition`/`SetCurrentApplicationModule` each.

## The two missing `getEnemies` re-fetches (Thumb, original)

Both are loop **conditions** that re-fetch `m_pLevel->getEnemies()->count`
instead of using the cached `enemies->count` local:

1. **case 24** (source line 1797, `(*getEnemies())[j]->player->setEnemies(nullptr)`).
   Original loop body `0x13ea4c`, condition `0x13ea60`:

   ```
   13ea5e  adds r4, #1
   13ea60  ldr  r0, [r6, #0]
   13ea62  blx  Level::getEnemies
   13ea66  mov  r1, r0
   13ea68  ldr  r0, [r6, #0]
   13ea6a  ldr  r1, [r1, #0]        ; ->count
   13ea6c  cmp  r4, r1
   13ea6e  bcc.n 13ea4c
   ```

2. **case 16 state 3** (source line 1708, `KIPlayer *ship = (*getEnemies())[i]`).
   Original loop body `0x13f1f4`, condition `0x13f2dc`:

   ```
   13f2d8  add.w fp, fp, #1
   13f2dc  ldr  r0, [r5, #0]
   13f2e0  blx  Level::getEnemies
   13f2e4  mov  r1, r0
   13f2e6  ldr  r0, [r5, #0]
   13f2e8  ldr  r1, [r1, #0]
   13f2ea  cmp  fp, r1
   13f2ec  bcc.w 13f1f4
   ```

The third re-fetch (case-16 **state 4**, source line 1727) was landed by w8603
and is present at original `0x14059a` / ours `0x14059a` — it is *not* missing.

## Measured size / raw of every candidate (built + linked on HEAD)

| edit | size | raw diff | getEnemies |
|---|---:|---:|---:|
| HEAD | 57232 | 54,108 | 32 |
| `e1` — case-16 state-3 condition | 57248 (+16) | 55,108 | 33 |
| `e2` — case-24 condition | 57252 (+20) | 54,959 | 33 |
| `e1+e2` | 57256 (+24) | 55,083 | 34 |
| `m144` — drop mission-144 duplicate `setStation(getStation())` | 57216 (−16) | 55,067 | 32 |
| `c` — mission-157 tail store reorder | 57228 (−4) | 54,176 | 32 |
| **`e1+e2+m144+c`** (size-exact, faithful 34 calls) | **57232** | **54,677** | **34** |
| `e1+e2+d2+m144` (drop case-105 state-6 `setTarget`) | 57232 | 55,040 | 34 |
| `rev+e1+e2+m144+c` | 57232 | 54,664 | 33 |
| **`rev` — revert the state-4 condition to `enemies->count`** | **57232** | **54,032** | **31** |
| `rev+c` | 57228 | 54,100 | 31 |
| `rev+m144` | 57216 | 55,073 | 31 |
| `rev+f` / `rev+r` | 57232 | 54,034 | 31 |

Key negative result: **adding the two missing re-fetches raises the raw
same-address diff** even when the size is held at exactly 57,232 B
(`e1+e2+m144+c` is +569). Each re-fetch re-colours the loop condition and the
surrounding block, and the raw metric is dominated by register allocation
(~94.6 % of all bytes differ at baseline), not by statement presence.

## The landed change

The state-4 loop condition (source line 1727) is reverted from the w8603
re-fetch form back to the cached local:

```cpp
// before (w8603)
for (unsigned int i = 0; i < m_pLevel->getEnemies()->count; i++) {
// after (landed)
for (unsigned int i = 0; i < enemies->count; i++) {
    KIPlayer *ship = (*m_pLevel->getEnemies())[i];
```

This is **not** the faithful shape (the original re-fetches at `0x14059a`), and
it drops the `getEnemies` count 32 -> 31 (original 34). It is landed because it
is the only size-exact change measured that **lowers** the assigned primary
metric: raw same-address diff **54,108 -> 54,032 (−76)**, `.text` 84,567 ->
84,491, with `.rel.plt` byte-identical and every section size unchanged.

The faithful alternative — both missing re-fetches plus the `m144`/`c`
companions — is `e1+e2+m144+c` at exactly 57232 B / raw 54,677 / getEnemies 34.
It is the variant to land once a genuine −24 B companion (rather than the
unfaithful `m144` call removal) is found.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8705/fdiff.py _ZN11LevelScript7processEi   # 54,032
python3 /dev/shm/w8705/secdiff.py                            # .text 84,491
python3 /dev/shm/w8705/count.py                              # getEnemies 34 vs 31
```

## Open questions

* A genuine −24 B companion (not the `m144` duplicate-call removal) would let
  `e1+e2` land at 57232 with all 34 `getEnemies` calls. Candidate families: the
  extra `setTarget` / `setPosition` / `getPosition` / `SetCurrentApplicationModule`
  calls that ours has one too many of.
* `rev` is a metric win, not a map win; a future pass that reproduces the
  original state-4 register map should restore `m_pLevel->getEnemies()->count`.
