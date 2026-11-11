# LevelScript::process — wave 175 / w11903: mission-0 state-0 block + first literal pool

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**).
Worktree `wt/w11903`, branch `wave175-w11903`, base master `5f599a137`.
Follow-up to `levelscript-process-state0-wave173-w11803.md` and
`levelscript-process-state0-wave174-w11903.md`.

## Result (landed)

The first literal pool now sits at the original address **`0x1380a4`** (was
`0x1380c0`, +28 B), the function is **size-exact 57,232 B**, and `.rel.plt` is
**byte-identical** again.  The pool's 108 bytes differ by only **3 bytes**
(was 92/108 in the base; the wave-174 `start_e1` candidate had 6/108).

```
process raw diffbytes    55,033      (base 53,673; w11803 candidate ~55.9k)
process aligned insns    20,732      (base 18,226)
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20724
first literal pool       0x1380a4    (original 0x1380a4)
pool bytes differing     3 / 108
whole .text diffbytes    81,799      (base 80,439; w11803 candidate 82,884)
.rel.plt                 29392 B, byte-identical (0)
gate                     byte 4507 / linked 4507 / size 4524 / data 843 /
                         imports 207 / lint CLEAN / drift 0 / parity 5370
```

The raw metric is **higher than the base** (53,673 -> 55,033) because the base
has the pool at the wrong address: every post-pool byte is compared at the
wrong offset.  The win is against the w11803 pool-correct candidate
(raw ~55.9k, `.text` 82,884, size 57,200, `.rel.plt` rotated); the landed shape
is size-exact, keeps the pool at `0x1380a4`, and restores `.rel.plt`.

## The six source edits

Base `src/game/world/LevelScript.cpp` at `5f599a137`:

1. `Status *status` -> `Status *volatile status`.
   Forces `status` to be reloaded from `[sp,#92]` instead of living in `r9` for
   the whole function.  This frees `r9` in the mission-0 block and is what lets
   the compiler hoist the enemies data pointer there (edit 3).

2. `PlayerEgo *player` -> `PlayerEgo *volatile player`.
   Same effect for the player pointer; it changes the callee-saved register
   assignment so the post-pool body comes back to 57,232 B.

3. Mission-0 state-0 3-ship loop: hoist `enemies->data_` and index it, instead
   of `(*enemies)[i]`:

   ```cpp
   KIPlayer **shipData = enemies->data_;
   for (int i = 0; i < 3; ++i) {
       KIPlayer *e = shipData[i];
       e->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
       e->geometry->setVisible(true);
   }
   ```

   The original `0x137f24..0x137f66` loads `enemies->data_` once and indexes it
   twice (`ldr.w r0,[r9,#4]` / `ldr.w r0,[r0,r5,lsl#2]` twice); this source
   shape reproduces that (`ldr r4,[r4,#4]` before the loop, `ldr.w
   r0,[r4,r5,lsl#2]` twice in the body) and adds the missing +4 B to the
   mission-0 block.  With the pool at `0x1380a4` this makes the whole pre-pool
   region 908 B, exactly the original size.

4. The mission chain after the tail is a separate `if`, not an `else if` of
   `m_bStartSequence` (oracle-confirmed; see the wave-173 note):

   ```cpp
   } else if (Globals::status->getCurrentCampaignMission() == 5) {
   ->
   if (Globals::status->getCurrentCampaignMission() == 5) {
   ```

   This makes the second wingman loop test block fall through to the mission-5
   check, so `arm-cp-islands` places the first constant pool after the loop
   entry (`bb.30`) instead of after the loop test (`bb.34`) -- the original
   `0x1380a4` layout with the second wingman loop at `0x138110`.

5. `if (m_bStartSequence != 0)` -> `if (__builtin_expect(m_bStartSequence != 0, 1))`.
   Restores the +8 B that edit 4 removes from the function (the branch-weight
   change keeps the hot tail layout and the exact 57,232 B size).

6. Wormhole-body condition: `m_nState == 0` -> `__builtin_expect(m_nState == 0, 0)`.
   Marks the shared mission-50/51 exit-wormhole body cold.  This is what keeps
   `.rel.plt` byte-identical: it moves the body (and its `Standing::setStanding`
   call) after the state machine's `PlayerEgo::setRocketControl` /
   `killLiberator` / `hackingWon` / `getHackingGameDockIndex` references, so the
   PLT stub order matches the original (otherwise `setStanding` is allocated 4
   slots early and `BombGun::update`'s `blx` changes by 12 B, dropping
   `byte_exact` 4507 -> 4506).

## Measurements

Base (`5f599a137`): raw **53,673**, aligned **18,226**, size 57,232, pool
`0x1380c0`, `.rel.plt` 0, `.text` 80,439.

| variant | raw | size | pool | .rel.plt | .text |
|---|---:|---:|---|---:|---:|
| base | 53,673 | 57,232 | `0x1380c0` | 0 | 80,439 |
| `sepif` only | 55,394 | 57,216 | `0x1380a0` | 9 | 361,484 |
| `sepif`+`start_e1` | 55,365 | 57,224 | `0x1380a0` | 9 | 358,041 |
| `data_` hoist (edit 3) | 55,172 | 57,224 | `0x1380a4` | 9 | 357,848 |
| + `status volatile` | 54,764 | 57,232 | `0x1380a4` | 9 | 81,532 |
| + `player volatile` + `m_nState` expect 0 | **55,033** | **57,232** | **`0x1380a4`** | **0** | **81,799** |

The `m_nState == 0` expect is the key `.rel.plt` fix.  Expects on the other
wormhole-body terms (`isOver()`, `messages->count >= 3`, the whole condition)
also fix `.rel.plt` but cost 40-52 B of size; the `m_nState` term is the one
that lands size-exact with `player volatile`.

## Remaining structural blockers

The post-pool block order still differs from the original:

```
original:  5 @0x138132, intro-81 @0x1381b8, 26 @0x1382e2, 50 @0x1383ea, 51 @0x13877c
ours:      5 @0x138132, 26 @0x1381b2, intro-81 @0x138202, 50 @0x138338, 51 @0x138348
```

The original places the intro mission-81 block between the tail mission-5
block and the tail mission-26 check; ours places the tail mission-26 check
first.  That is why raw is still 55,033 rather than below the base 53,673:
the first ~1.5 KB after the pool are laid out in a different order.  Fixing
that order is the next pass (it needs the mission-5 else branch to jump over
the intro mission-81 block, as in the original `bne.w 0x1382dc`).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11903
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
cmake --build cmake-build-match --target check -j2
```
