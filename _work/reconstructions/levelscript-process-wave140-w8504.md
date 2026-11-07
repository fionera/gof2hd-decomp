# LevelScript::process — wave 140 / w8504 (case-125 state-7 and case-90 state-12 receivers)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `d4857f174` (wave-139 docs).
Scratch helpers in `/dev/shm/w8504/` (not tracked): `fdiff.py` (raw same-address diff),
`sdiff.py` (address-carrying normalised instruction diff built on `tools/verify/verify.py`),
`diff.py` (mnemonic/operand diff), `harness.py` (named-edit measurement harness).

## Result

```
                         before (master)   after (landed)
process raw diffbytes    54,262            54,115            (-147)
whole .text diffbytes    84,651            84,504            (-147)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab diffbytes     1,395             1,395             (size 156,312 exact)
.ARM.exidx               exact             exact
.rodata / .data / .bss   exact             exact
.rel.plt                 byte-identical    byte-identical (29,392 B)
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
address and `byte_exact`/`linked_exact` are unchanged.  `.rel.plt` stays byte-identical.

## The landed set (both verified against objdump of the original)

These are the two remaining w8403 receiver fixes that were not in the wave-139 head
(the third, `m_pRadar->dockNavPtr = nullptr`, was already present at source line 3061).

### 1. Case-125 state-7 `resetGunDelay` receiver (source line ~3062)

Original `0x140288..0x14028c`:

```
140288  ldr  r0, [r5, #24]       ; m_pLevel
14028a  blx  r4                  ; Level::getPlayer   (r4 caches the GOT slot)
14028c  blx  PlayerEgo::resetGunDelay
```

Our source used the cached `player->resetGunDelay()`.  Changed to
`m_pLevel->getPlayer()->resetGunDelay()` (**+8 B** in isolation).

### 2. Case-90 state-12 `setDirection` receiver (source line ~3855)

Original `0x144dc2..0x144dca`:

```
144dc2  ldr  r4, [r4, #8]        ; player->geometry   (r4 = cached player)
144dc4  add  r5, sp, #280
144dc6  mov  r0, r5
144dc8  mov  r1, r4
144dca  blx  AEGeometry::getDirection
```

Our source called `m_pLevel->getPlayer()->geometry->setDirection(...)`, emitting a
redundant `Level::getPlayer`.  Changed to `player->geometry->setDirection(...)`
(**-8 B** in isolation).

Net size delta is exactly 0; the measured raw diff drops 147 bytes.

## New hypotheses tested (measured on this base; do not re-run)

All measurements keep the rgd94+sd12 set above unless stated.

| variant | size | raw diff |
|---|---:|---:|
| baseline (wave-139 head) | 57232 | 54,262 |
| **rgd94 + sd12 (landed)** | **57232** | **54,115** |
| local declaration order `messages, player, enemies` (the original's prologue call order) | 57328 | 55,254 |
| declaration order `enemies, messages, player` | 57332 | 55,244 |
| declaration order `player, enemies, messages` | 57236 | 54,536 |
| declaration order `player, messages, enemies` | 57328 | 55,266 |
| declaration order `messages, enemies, player` | 57328 | 55,266 |
| `m_pLevel->getPlayer()->dockToDockingPoint(...)` (original calls getPlayer 3x via cached GOT slot) | 57248 | 54,957 |
| case-157 state-7: move `m_nScriptTimerA += delta` after the `setPosition(dir*10000)` (original order) | 57220 | 55,034 |

The declaration-order sweep is the important negative result: the original's
**first prologue call is `Level::getMessages`**, then `getPlayer`, then `getEnemies`
(`0x137d3e/0x137d46/0x137d50`), but reordering our three locals to that order *raises*
the function to 57,328 B and worsens the raw diff.  The current `enemies, player,
messages` order is the only permutation that stays size-exact; clang's scheduler, not
the declaration order, decides the emitted call order at `-Oz`.

The `dockToDockingPoint` receiver is faithful (`0x140290` calls `getPlayer` a third
time) but costs +8 B and worsens the raw diff to 54,957; it needs a different -8 B
companion to land.

## Instruction-count analysis

The original has **19,342** real instructions (excluding 827 literal-pool words);
ours has **19,205** — a 137-instruction deficit.  The deficit is almost entirely
`ldr`: original 5,660 vs ours 5,525 (**-135**); every other mnemonic is within a
handful.  This is *not* missing statements in a few cases: the `ldr` surplus is
spread evenly across the whole function (1,556 `ldr` in orig-only/replace regions vs
1,421 in ours-only/replace regions), i.e. it is the register-allocation/encoding-width
difference, not missing source.  The call multiset is nearly identical (orig 2,356
direct `blx` vs ours 2,363); the only net-missing direct calls are the three
`getEnemies()` loop-condition re-fetches already documented in w8407.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8504/fdiff.py _ZN11LevelScript7processEi   # 54,115
```

## Open questions

* The `dockToDockingPoint` receiver (`0x140290`, +8 B) is faithful but needs a -8 B
  companion.  The original caches `Level::getPlayer` in r4 and calls it three times
  (setAutoPilot, resetGunDelay, dockToDockingPoint); our compiler only caches it for
  two direct calls and reuses the `player` local for the third.
* The three loop-condition `getEnemies()` re-fetches (w8407) are still not in the
  source; adding one costs +8..+16 B and currently worsens the raw diff.
* `.ARM.extab` remains 1,395 differing bytes at an exact 156,312 B size.
