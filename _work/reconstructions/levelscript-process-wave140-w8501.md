# LevelScript::process — wave 140 / w8501 (faithful case-94 block + the missing
# `m_bCinematicBreak = 0` store + duplicate-resetCamera removal)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base: wave-139 head `d4857f174`.
Scratch helpers in `/dev/shm/w8501/` (not tracked): `harness.py` (named-edit
apply/build/measure), `search2.py` / `search3.py` (subset sweeps), `secdiff.py`,
`fdiff.py` (raw same-address diff), `census.py` (call census).

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,262            54,140            (-122)
whole .text diffbytes    84,651            84,529            (-122)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab diffbytes     1,395             1,395             (size 156,312 exact)
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

The function stays exactly 57,232 B, so every later function keeps its original
address and `byte_exact`/`linked_exact` stay 4507. `.rel.plt` stays byte-identical.

## The landed set (all verified against objdump of the original)

### 1. Case-94 dock block — the full faithful receiver/store set (0x140280..0x14029a)

Original:

```
140280  ldr.w r0, [r5, #212]     ; m_pRadar
140284  strd  r8, r8, [r0, #4]   ; lockedEnemy = dockNavPtr = nullptr
140288  ldr   r0, [r5, #24]      ; m_pLevel
14028a  blx   r4                 ; Level::getPlayer
14028c  blx   PlayerEgo::resetGunDelay
140290  ldr   r0, [r5, #24]      ; m_pLevel
140292  blx   r4                 ; Level::getPlayer
140294  ldr.w r2, [r5, #212]     ; m_pRadar
140298  movs  r1, #0
14029a  blx   PlayerEgo::dockToDockingPoint
```

The source had the cached `player` local for `resetGunDelay` and
`dockToDockingPoint`, and only wrote `m_pRadar->lockedEnemy`.  Three faithful
fixes:

* `m_pRadar->dockNavPtr = nullptr;` — the original's `strd r8,r8,[r0,#4]` zeroes
  **both** `lockedEnemy@4` and `dockNavPtr@8`.
* `m_pLevel->getPlayer()->resetGunDelay();` — fresh `Level::getPlayer`, not the
  cached local.
* `m_pLevel->getPlayer()->dockToDockingPoint(nullptr, m_pRadar);` — fresh
  `Level::getPlayer`, not the cached local.

The compiler caches the `getPlayer` GOT slot in `r4` and emits the same three
`blx r4` calls as the original.

### 2. Case-154 state-7 — remove the duplicate `resetCamera(m_pLevel)` (0x1408c6)

The source called `resetCamera(m_pLevel)` twice back-to-back (source lines
3726-3727).  The original has exactly one call.  Removing the duplicate is a
faithfulness fix and `-12 B` in isolation.

### 3. Case-154 state-12 — cached `player` for `setDirection` (0x144dc2)

Original `ldr r4,[r4,#8]` with `r4 = player` (no `getPlayer` call):

```cpp
player->geometry->setDirection(-dir, Vector{0.0f, 1.0f, 0.0f});
```

The source spelled it `m_pLevel->getPlayer()->geometry->setDirection(...)`.
`-8 B` in isolation.

### 4. Case-154 state-9 — the missing `m_bCinematicBreak = 0` store (0x1449f6)

Original:

```
1449f2  ldrb.w r0, [sl, #170]   ; field_0xaa
1449f6  strb.w r4, [sl, #17]    ; m_bCinematicBreak = 0   <-- missing in source
1449fa  cbz    r0, 144a04
1449fc  ldr    r0, [sp, #84]
1449fe  movs   r1, #1
144a00  blx    PlayerEgo::setAutoTurret
144a04  strd   r4, r4, [r9]      ; m_nScriptTimerA = 0 (8-byte long long)
144a08  ldr    r0, [sl, #28]
144a0c  adds   r0, #1
144a0e  str    r0, [sl, #28]
```

This is the **+4 B faithful companion** the wave-139 w8403 note was looking for:
a single missing `strb` store.  The source had no `m_bCinematicBreak` assignment
in this block at all.

### 5. Case-154 state-9 — drop the spurious `m_nScriptTimerB = 0` (0x144a04)

`m_nScriptTimerA` and `m_nScriptTimerB` are both `long long` (8 bytes) at
offsets 144 and 152.  The original's `strd r4,r4,[r9]` zeroes only the 8-byte
`m_nScriptTimerA`; it never touches `[sl,#152]`.  The source additionally wrote
`m_nScriptTimerB = 0`, which emitted a spurious `strd r4,r4,[sl,#152]`.
Removing it is `-8 B` in isolation.  Together with (4) it is exactly the w8405
`cine9` pair (`-4 B` net).

## Size accounting (measured individually on HEAD)

| edit | Δ size |
|---|---:|
| `docknav` (`m_pRadar->dockNavPtr = nullptr`) | +8 |
| `rgd94` (fresh `getPlayer` for `resetGunDelay`) | +8 |
| `dock94` (fresh `getPlayer` for `dockToDockingPoint`) | +8 |
| `sd12` (cached `player` for `setDirection`) | -8 |
| `cine9a` (missing `m_bCinematicBreak = 0`) | +4 |
| `rmtimer9` (drop spurious `m_nScriptTimerB = 0`) | -8 |
| `dupreset` (drop duplicate `resetCamera`) | -12 |
| **net measured** | **0** (57232 B) |

## Why the wave-139 w8403 set measured 57,228 B, not 57,232 B

On the wave-139 head the w8403 set alone (`docknav + rgd94 + sd12`) is already
exactly **57,232 B** (raw 54,118).  The w8403 note's 57,228 B figure was measured
on a base that also carried the w8405 `cine9` pair (the timer-B removal plus the
`m_bCinematicBreak` store).  The two are independent: this landing folds the full
case-94 receiver set, the missing store, the duplicate-call removal and the
spurious timer store into one size-exact set.  The `+4 B` missing store is
`cine9a`; it is paired with `dupreset` (-12) and `rgd94`/`dock94` (+16) so the
whole set still lands at exactly 57,232 B.

## Subset sweep (all built + linked + measured on HEAD)

| variant | size | raw diff |
|---|---:|---:|
| HEAD | 57232 | 54,262 |
| `dock94,sd12` | 57232 | 54,114 |
| `rgd94,sd12` | 57232 | 54,115 |
| `docknav,sd12` | 57232 | 54,117 |
| `docknav,rgd94,sd12` (w8403 set) | 57232 | 54,118 |
| `docknav,dock94,sd12` | 57232 | 54,123 |
| **`docknav,cine9a,rgd94,dock94,sd12,dupreset,rmtimer9` (landed)** | **57232** | **54,140** |
| `docknav,cine9a,sd12,dupreset,setcollide` | 57232 | 54,194 |
| `docknav,cine9a,dupreset` | 57232 | 54,468 |

The landed set is the lowest-raw **exact-57232** combination that contains the
`+4 B` missing store (`cine9a`) *and* the full faithful case-94 receiver block
(`docknav + rgd94 + dock94 + sd12`).  It is 122 B below HEAD.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8501/harness.py \
  docknav,cine9a,rgd94,dock94,sd12,dupreset,rmtimer9   # 57232, raw 54140
python3 /dev/shm/w8501/secdiff.py                       # .text 84529, extab 1395
```

## Open questions

* The three case-16/case-24 `getEnemies()` loop **conditions** still use the
  cached `enemies->count`; the original re-fetches there too.  Adding a
  condition re-fetch costs +8..+16 B and currently worsens the raw diff.
* `.ARM.extab` remains 1,395 differing bytes at an exact 156,312 B size; the
  remaining `.text` delta is dominated by 16/32-bit encoding width and
  `MachineBlockPlacement`, not missing statements.
* The case-157 tail store order (w8404) and the w8408 case-125 two-chain are
  still reverted on this base; both are size-exact but raw-positive.
