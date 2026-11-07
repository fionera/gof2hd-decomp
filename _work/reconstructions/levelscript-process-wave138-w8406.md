# LevelScript::process — wave 138 / w8406: mission-0 data-pointer hoist + dead state-9 store

Status: **code change landed**, 2026-11-07. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `bf123bd47` (wave-138 head).
Scratch helpers in `/dev/shm/w8406/` (not tracked): `fdiff.py`, `try.sh`,
`batch*.py`, `LevelScript.base.cpp`.

## Result

```
                         before (master)   after (landed)
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process raw diff bytes   55072             54910           (-162)
whole .text diff bytes   85476             85314           (-162)
.ARM.extab size          156312            156312          (size/base exact; diff 1396)
.ARM.exidx               exact             exact
.rodata/.data/.rel.plt   exact             exact
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## 1. NEW: mission-0 state-0 visibility loop — hoist `enemies->data_`

w8306/w8204 established that the original's state-0 loop (0x137f34) reloads
`(*enemies)[i]` for both calls and keeps `enemies` in `r9`, whereas our source
caches the element in a local and spills `enemies`.  The full double-index
spelling costs +60 B because it re-colours the prologue.

This pass tried a *different* local placement: hoist the array's data pointer
once, then index it twice:

```cpp
KIPlayer **enemyData = enemies->data_;
for (int i = 0; i < 3; ++i) {
    enemyData[i]->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
    enemyData[i]->geometry->setVisible(true);
}
```

This is +8 B (the loop keeps `enemyData` in `r4` and reloads `[r4,r5,lsl#2]`
twice; the prologue is unchanged: `r4=messages`, `r9=status`).  It lowers the
raw diff by 35 B on its own (55072 -> 55037) — the closest to the original's
`r4`/`r9` map any size-neutral-ish spelling has reached.

Measured spellings (all built + scored on this head):

| spelling | size | diff |
|---|---:|---:|
| baseline (cached `KIPlayer *enemy`) | 57232 | 55072 |
| `(*enemies)[i]` twice (full DI) | 57292 | 55432 |
| `enemies->data_[i]` twice (no local) | 57292 | 55432 |
| **`KIPlayer **enemyData = enemies->data_;`** | **57240** | **55037** |
| `enemyData[i]` cached enemy (`KIPlayer *enemy`) | 57200 | 55230 |
| `AEGeometry *g = enemyData[i]->geometry` | 57168 | 55263 |
| `data_` for the three `setPosition` calls too | 57220 | 55133 |

## 2. NEW: dead `m_nState = 9` in mission-0 state 7

The original state-7 block (tbh entry 0x13d022; body 0x13d13c) ends:

```
13d134  blx   FModSound::play(141)
13d138  movs  r0, #8
13d13a  b.n   13d4ba        ; shared tail: str.w r0,[sl,#28]; strd 0 -> timer
```

There is **no** `m_nState = 9` store.  Our source wrote `m_nState = 9;` before
`play(141)` and `m_nState = 8;` after it; at `-Oz` clang does **not** eliminate
the dead store across the call (it cannot prove the callee does not read
`this->m_nState`), so it emitted an extra `movs r0,#9; str.w r0,[sl,#28]`
(-8 B when removed).

Removing it alone lands at 57224 B (diff 55258); combined with the `data_`
hoist it restores exactly 57232 B and the diff drops to 54910.

## 3. Other mission-0 body-order checks (no size win)

* `move4b` (the four `setFreeLookMode` / `enableFirstPersonCam` /
  `hideShipForFirstPersonCameraView` / `removeAllGuns` calls belong in state 4,
  not state 5) is now **size-exact on this head** (57232 B) but the raw diff is
  +5 (55077).  It is a correctness fix, kept for a later pass where it can be
  paired with a size-negative companion.
* `Globals::sound->stop(player->field_0x1c)` in state 4 is wrong: the original
  (0x138d40-0x138d44) does `ldr r0,[sound]; ldr r1,[r0,#0]`, i.e.
  `Globals::sound->stop(Globals::sound->currentMusicEvent)`.  Size-neutral; the
  raw diff is +4 in isolation (55076).
* state-5 speed order: the original does `getSpeed(); setSpeed(getSpeed()*0.98f)`,
  our source had `setSpeed(speedBits*0.98f); speedBits = getSpeed();`.  Swapping
  the two statements is size-neutral and matches the original instruction for
  instruction; raw diff +14 in isolation.
* `m_nState = m_nState + 1` -> `m_nState = 8` (case 89 state 7) is -4 B but
  the original really does store the immediate; not combined here.
* `m_nState = 2` before `m_nState = 10` (line ~2690) is already CSE'd by clang
  (byte-identical when removed).

## Why the pair lands

`data_` hoist: +8 B, -35 diff.  Dead state-9 store: -8 B, and its removal also
moves the state-7 block's `m_nState` store into the shared tail exactly as the
original does.  The two together are size-exact and the raw diff falls by 162 B.
The prologue is untouched (`r4=messages`, `r6=delta`, `r8=enemies`, `r9=status`,
`sl=this`, `fp=guard`), so `.rel.plt`/PLT stubs are unchanged and the gate is
green.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8406/fdiff.py _ZN11LevelScript7processEi   # 57232/57232, diff 54910
cmake --build cmake-build-match --target check -j6            # RATCHET PASS
```
