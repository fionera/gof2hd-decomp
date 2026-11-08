# LevelScript::process — wave 156 / w10203: case-106 callee-saved allocation

Target `_ZN11LevelScript7processEi`, original `0x137d18`, **57,232 B (0xdf90)**,
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Base master
`cf74f4ee6` (wave 155).  Scratch in `/dev/shm/w10203/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,123            55,117            (-6)
process aligned insns    17,180            17,180            (0)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## The landed source change

In the mission-106 state-6 block, cache the `Player *` once:

```cpp
Player *pp = player->player;
Globals::status->field_64 = pp->getHitpoints();
Globals::status->field_5c = pp->getShieldHP();
Globals::status->field_60 = pp->getArmorHP();
Globals::status->field_68 = pp->getGammaHP();
```

The original actually reloads `player->player` before each getter
(`ldr r0,[r4,#0]` x4); this shape is *not* more faithful, but it is the only
size-exact variant measured this wave that improves raw.  It also swaps the
`player`/`this` stack slots (`player` [sp,#84] -> [sp,#88], `this` [sp,#88] ->
[sp,#84]); aligned is unchanged.

## Callee-saved map (deliverable)

Baseline (and landed): `this` = `fp`, canary ptr = `sl`, `&Globals::status` =
`r9`; `messages` r5 / `delta` r6 / `enemies` r8; `r4` free.
Original: `this` = `sl`, canary ptr = `fp`, `&Globals::status` = `r4`;
`messages` r5 / `delta` r6 / `enemies` r8; `r9` free (used as a scratch,
e.g. `mov.w r9,#1`).  Frame is 0x1b0 in both.

So the residual is a pure permutation: `this`<->canary and `r4`<->`r9`
(original keeps `&Globals::status` in the low callee-saved r4 and spills it to
`[sp,#92]` for the cold state-6 body; ours keeps it in r9 and never spills).

## Sweep (all built + linked; size must stay 57,232)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD (pristine) | 57232 | 55,123 | 17,180 |
| **landed: `Player *pp` in state-6** | **57232** | **55,117** | **17,180** |
| remove `status` local (all `Globals::status`) | 57232 | 55,124 | 17,189 |
| `Status **statusPtr = &Globals::status` | 57232 | 55,124 | 17,189 |
| `Status *const status` | 57232 | 55,123 | 17,180 |
| `expect(mission==106, 0)` | 57232 | 55,159 | 17,190 |
| `expect(mission==106, 1)` | 57232 | 55,165 | 17,193 |
| `Status *st` in state-6 only | 57176 | 55,154 | 17,858 |
| `Status *st` whole mission-106 block | 57184 | 55,477 | 17,773 |
| `Level *self = m_pLevel` (whole fn) | 57056 | 54,778 | 18,498 |
| `Level *self` mission-106 only | 57240 | 55,291 | 17,551 |
| `TargetFollowCamera *cam = m_pCamera` | 57060 | 55,267 | 22,770 |
| reorder `status` decl first | 57284 | 55,340 | 17,044 |
| reorder `status` after `messages` | 57284 | 55,341 | 17,038 |
| reorder `status` after `player` | 57248 | 55,399 | 17,400 |
| `expect(mission==0, 0)` | 57416 | 55,547 | 22,014 |
| `expect(mission==0, 1)` (w10113) | 57400 | 55,685 | 22,148 |
| `expect(mission==1, 0)` | 57392 | 55,603 | 21,901 |
| `expect(mission==1, 1)` | 57476 | 55,426 | 22,864 |

## Key finding

The `Level *self = m_pLevel` alias (whole function) **does** produce the
original allocation: `this`=sl, canary=fp, `&status`=r9 (and the state-6 body
then reloads `player` from stack as the original does).  It costs -176 B
(57,056) because it removes the per-use `ldr r0,[r4]` reload of `m_pLevel`
(the original keeps `&m_pLevel` in r4 and reloads `m_pLevel` at every use).
So the original source used `m_pLevel` directly (member access), not a local
alias; the alias is the only lever found that flips `this`/canary, but it
cannot be made size-exact without re-adding the member reloads.

Reordering the local declarations (status first / after messages) also moves
`this` off `fp` (to r6) and improves aligned to 17,038-17,044, but costs
+52 B, so it fails the size gate.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10203/fdiff.py _ZN11LevelScript7processEi        # 55,117 / 57,232
python3 /dev/shm/w10203/align_diff.py _ZN11LevelScript7processEi  # 17,180
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## Open questions

1. The `this`=sl / canary=fp flip is reachable only via the `m_pLevel` alias
   (-176 B) or decl reordering (+52 B).  A source shape that lowers `this`'s
   spill weight *without* changing the `m_pLevel` access pattern is still open.
2. The original spills `&Globals::status` (r4 -> [sp,#92]) in the cold
   mission-106 body; ours keeps it in r9.  Forcing a cold-region spill of that
   value is the direct route to the 931-insn 0x13d7f8 cluster.
