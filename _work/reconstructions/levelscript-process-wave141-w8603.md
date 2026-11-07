# LevelScript::process — wave 141 / w8603 (3rd missing `getEnemies` loop condition)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base: wave-140 head `e18e0c2a4`.
Scratch helpers in `/dev/shm/w8603/` (not tracked): `fdiff.py` (worktree-pathed raw
same-address diff), `harness.py` (named-edit apply/build/measure), `callmulti.py`
(call-multiset diff), `percase.py`, `secdiff.py`.

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,138            54,138            (neutral)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab diffbytes     1,411             1,411             (size 156,312 exact)
.text diffbytes          86,472            86,472
.rel.plt                 0                 0   (byte-identical, 29,392 B)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## The call-multiset census (the new lever)

Comparing the ordered call multiset of the whole function (orig vs ours) is the
cheapest way to find genuinely missing statements, independent of block
placement. Baseline differences:

| symbol | orig | ours |
|---|---:|---:|
| `Level::getEnemies` | 34 | **31** |
| `Level::getPlayer` | 23 | 22 |
| `AEGeometry::setPosition(Vector)` | 23 | 24 |
| `ApplicationManager::SetCurrentApplicationModule` | 9 | 10 |
| `TargetFollowCamera::setTarget` | 81 | 82 |
| `PlayerEgo::getPosition` | 41 | 42 |
| `PlayerEgo::setAutoTurret` | 2 | 3 |
| `PlayerEgo::autoTurretIsEnabled` | 1 | 2 |
| `Status::getStanding` / `Standing::setStanding` | 1 / 1 | 2 / 2 |

The three **missing** `getEnemies` calls are exactly the three loops whose
condition in the source is `i < enemies->count` (the cached local) while the
original re-fetches `m_pLevel->getEnemies()->count` every iteration:

* **case 24** (source line 1797), original loop `0x13ea60`:
  `ldr r0,[r6]; blx getEnemies; ldr r1,[r1]; cmp r4,r1; bcc`.
* **case 16 state 3** (source line 1709), original loop `0x13f2e0`:
  same `getEnemies`-in-condition shape.
* **case 16 state 4** (source line 1727), original loop `0x14059a`:
  same shape.

## The landed change

Only the **state-4** loop (source line 1727) is landed, because it is the one
that is exactly **size-neutral** (57232 B) with no companion:

```cpp
// before
for (unsigned int i = 0; i < enemies->count; i++) {
    KIPlayer *ship = (*m_pLevel->getEnemies())[i];
    if (ship != nullptr && ship->shipGroup == 9) {
        ship->setVisible(false);
        ship->setDead();
    }
}
// after
for (unsigned int i = 0; i < m_pLevel->getEnemies()->count; i++) {
    ...
}
```

The generated condition now matches the original instruction-for-instruction
(`blx getEnemies; mov r1,r0; ldr r0,[r6]; ldr r1,[r1]; cmp r4,r1; bcc`), and the
`getEnemies` call multiset goes 31 -> 32 (still 2 short: case 24 and case-16
state 3). Raw same-address diff is **unchanged at 54,138** because the added
call replaces an equivalent `ldrd`/`ldr` setup (same byte count) and the loop
sits at the same address.

## Measured size/raw of the three condition fixes (each on HEAD)

| edit | size | raw diff |
|---|---:|---:|
| HEAD | 57232 | 54,138 |
| `e16b` (case-16 state-4, landed) | **57232** | **54,138** |
| `e24` (case 24) | 57244 (+12) | 54,384 |
| `e16a` (case-16 state-3) | 57240 (+8) | 54,889 |
| all three | 57256 (+24) | 55,177 |

The +12/+8 variants are faithful but shift every following block, so the raw
metric rises; they need a **-12 / -8 companion located after the edited loop**
to stay aligned. No such companion was found this pass.

## New leads recorded (not landed)

1. **`if (delta == 0x7d) getStanding()->setStanding(0,100)` anchor (source line
   2011).** Removing it is exactly size-neutral and lowers raw diff by **180**
   (54,138 -> 53,958) and removes the extra `getStanding`/`setStanding` pair
   (multiset then matches orig 1/1). But it moves `Standing::setStanding` in
   `.rel.plt` from slot 4593 to 4606 (26 bytes differ), which shifts `.plt`
   stubs and drops `byte_exact` 4507 -> 4504. **Not landable** until the real
   `setStanding` call (source line 1540, ours `0x13bd14`, original `0x139b06`)
   is block-placed earlier.
2. **Extra `SetCurrentApplicationModule(2)` in the cold mission-complete
   duplicate.** Original cold block `0x145bd0` stores `switch_to_target_setting`
   and `initStreamOutPosition` then `b.w 0x13d6b6` (where the single
   `SetCurrentApplicationModule(2)` lives). Ours inlines the call in the cold
   block too (`0x145be2`), giving 10 calls vs the original's 9. Removing the
   duplicate is ~8-12 B and would help raw, but the block is compiler-outlined
   from the case-157 `m_nScriptTimerA >= 6500` tail — needs a source reshape.
3. **The 351-instruction case-125 state-1 body** (`0x13b43e..0x13b7ea`) is still
   outlined in ours; w8506's `s1first_cases_r2` shape (raw 54,058) remains the
   best raw win but grows the process LSDA table 0x146 -> 0x148 (limit 0x147),
   which grows `.ARM.extab` and drops `byte_exact`. Shaving 2 table bytes is the
   unblocker.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8603/harness.py e16b                  # 57232, raw 54138
python3 /dev/shm/w8603/callmulti.py                     # getEnemies 34 vs 32
```
