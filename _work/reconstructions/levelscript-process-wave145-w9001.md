# LevelScript::process — wave 145 / w9001 (`setStanding` body-in-case-70
# rebased onto wave 144: the w8903 +4 shape is +120 here; a body-at-case-70-start
# shape with hoisted `enemies->data_`/`count` lands at 57,228/57,236)

Status: **analysis only, no code change landed** (source restored to the wave-144
HEAD `eb13ada9b`). Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, size-exact. Owned TU
`src/game/world/LevelScript.cpp`. Scratch helpers in `/dev/shm/w9001/`
(not tracked): `fdiff.py`, `reldiff.py`, `harness.py`/`harness2.py` (build +
measure), `variants.py`/`gen.py`/`bodyvar*.py` (the ~70 variants measured).

## Result

```
                         wave-144 HEAD     this pass
process raw diffbytes    54,909            54,909   (source pristine)
orig / ours size         57232 / 57232     57232 / 57232
.rel.plt                 0 diff            0 diff
byte_exact               4507              4507
size-exact               4524/4524         4524/4524
RATCHET                  PASS              PASS
```

## 1. The w8903 "+4" shape is confirmed — but only on the wave-143 base

Reproducing w8903 exactly on the **wave-143 base** `f5093474f` (body after
case-70 state-0, guard `mission==50||51`, anchor removed, case-80 `dockNavPtr`
store removed, no `break`):

| metric | value |
|---|---:|
| size | **57236** (+4) |
| raw same-address diff | 55,274 |
| `getStanding` / `setStanding` | `0x139aba` / **`0x139ac2`** |
| `.rel.plt` | **0 diff** |

This matches the w8903 note byte for byte. **But on the wave-144 HEAD the same
shape is 57,352 B (+120)**, not +4. The wave-144 changes (w8902 `break`+state-1
-first, w8904 case-91 nested test, ccabf20e7 case-91 `m_nScriptTimerA = 0`, and
the two `setPosition(Vector)` → `setPosition(x,y,z)` rewrites) are size-neutral
while the body sits after case 80, but **add ~116 B when the body is moved into
case 70**. Bisected on the wave-143 base + w8903 shape (57236):

| wave-144 change added | size | delta |
|---|---:|---:|
| case-91 nested test (w8904) | 57272 | +36 |
| `setPosition(m_tempVec)` → `(x,y,z)` (2 sites) | 57340 | +104 |
| port `setName`/`setActive` reorder | 57320 | +84 |
| restore `speedBits = player->getSpeed();` | 57244 | +8 |

So the w8903 shape cannot be landed on wave-144 without a **-120** companion,
not a -4. (The +120 is real code growth: removing one 16-bit store from the
body moves the size by 8, i.e. there is no branch-relaxation cliff to exploit.)

## 2. With `break` present, "after case-70 state-0" is cold-placed

The w8902 `break;` (`0x139abe b.w 0x144e36`) makes the block after the state-0
`if` reachable only via the pre-switch `goto`, so clang puts the body in the
cold section (`setStanding` `0x138a2e`, `.rel.plt` 16 diff). Dropping the
`break` restores the hot placement (`0x139aa0`, `.rel.plt` 0) but the shape is
still 57,352. The faithful `break` must stay (w8902).

## 3. Best wave-144 shapes found (all `.rel.plt` byte-identical)

The only way found to shrink the body-in-case-70 shape on wave-144 is to hoist
the `Array` payload into locals. `enemies->data_` is a high register (`r8`) in
this function, so every `(*enemies)[i]` costs a `ldr.w`; hoisting it to a low
register removes ~180 B.

```cpp
// body placed at the START of case 70 (before the state-0 if), guard mission==50
if (Globals::status->getCurrentCampaignMission() == 50) {
wormholeExitBody:
    if (messages->count >= 3 && ((RadioMessage *) ((*messages)[2]))->isOver() && m_nState == 0) {
        KIPlayer **en = enemies->data_;
        for (unsigned int i = 0; i < enemies->count; ++i) {
            if (!en[i]->isWingMan()) {
                en[i]->player->setAlwaysFriend(false);
                en[i]->player->setAlwaysEnemy(true);
                en[i]->player->turnEnemy();
            }
        }
        ((Standing *) (intptr_t) Globals::status->getStanding())->setStanding(0, 100);
        m_nState = 1;
    }
}
```

| variant (wave-144 base) | size | raw | setStanding | plt |
|---|---:|---:|---:|---:|
| **`start_g50_all`** | **57236** | **55,204** | 0x139988 | **0** |
| `start_g50_all_n` (`unsigned int n = enemies->count;`) | 57228 | 55,401 | 0x139980 | 0 |
| `start_g50or51_all_n` | 57236 | 55,352 | 0x13998c | 0 |
| `start_g50or51_all` | 57244 | 55,388 | 0x139994 | 0 |
| `end_g50_all_n` (no break) | 57156 | 55,332 | 0x139a94 | 0 |
| `end_g50_all_n` (break) | 57164 | 55,484 | 0x138a20 | 16 |
| w8903 shape (no break, no hoist) | 57352 | 55,482 | 0x139aa0 | 0 |
| w8903 shape (break, no hoist) | 57360 | 55,524 | 0x138a2e | 16 |

`start_g50_all` is the best raw metric of every body-in-window shape measured
(55,204; the base anchor shape is 54,909). It is **+4**, not 0.

## 4. Why 57228/57236 and not 57232

The size moves in 8-byte quanta around the target: removing/adding one store,
one `getCurrentCampaignMission` call, or the `n` local moves it by 8. A genuine
single 32-bit Thumb instruction (4 B) is still needed. Tried and measured
(all stay at 57228 or 57236): `setStanding(0,1000)`, `m_nState = (int)1`,
`volatile int dummy`, `for (int i...)`, `i != n`, `i += 1`, `n` as `int`,
`KIPlayer **en = &(*enemies)[0]`, iterator loop `begin()/end()`, hoisting
`Player *p` for one/two/three calls, `messages != nullptr` folded into the
guard, `(m & ~1) == 50`, `(unsigned)(m-50) <= 1`, `m==50||m==51||m==52`.

Tail-sharing the body's `m_nState = 1` with the mission-5 tail at `0x1381aa`
(the original's `b.w 0x1381aa`) was tried with a labelled `missionFiveStateOne`
inside the mission-5 `if` (the `Vector *scratch` initializer must be split into
`Vector *scratch;` + assignment so the `goto` is legal). It moves the body out
of the window (`setStanding` `0x13a416`, `.rel.plt` 40) and is 57,088 B on the
wave-143 base, so it is not usable as-is.

## 5. Faithfulness notes

* Removing the `if (delta == 0x7d)` anchor is faithful: the original has
  exactly one `getStanding`/`setStanding` pair in `process` (ours has two).
* The `if (mission == 50 || mission == 51)` guard around the body is dead on
  the case-70 fall-through (case 70 only runs for mission 70) and exists only
  to keep the block hot; it is not proven.
* Hoisting `enemies->data_`/`count` into locals contradicts the original's
  witnessed asm (the original reloads `enemies->data_` before every call at
  `0x139ad4/0x139ae2/0x139af0`), so `start_g50_all` would be **METRIC-ONLY**
  as a size vehicle; the faithful body has no locals.

## Open questions

1. Find a single 32-bit-instruction companion that turns `start_g50_all`
   (57236) into 57232, or `start_g50_all_n` (57228) into 57232, with
   `.rel.plt` 0.
2. Or find a wave-144 shape whose natural size is 57232 without the
   `enemies->data_` hoist (the faithful body is +120 on wave-144).
3. The original body ends `b.w 0x1381aa` (shared `m_nState = 1` tail); ours
   ends `movs r0,#1; str.w r0,[fp,#28]` (6 B) in the hot variant. Making the
   tail a 4-byte `b.w` is worth -2 and is the most faithful remaining lever.
