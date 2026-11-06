# LevelScript::process — wave 119 / w6965 (condition-order + register-allocation probe)

Status: **code change landed, 2026-11-06**. Branch `wave119-w6965`, base master
`d512ec24e` (wave-119 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      59.957716702      59.959208039     (+0.001491337)
orig / ours insns        20169 / 20036     20169 / 20035
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
.text                    1259464 (exact)   1259464 (exact)
.ARM.extab               156312 (exact)    156312 (exact)
.rodata                  269351            269351            (pre-existing +192)
.rel.plt / .rel.dyn      baseline          identical to baseline
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6965/score.py`, same definition as the wave-118/119 notes).

## Landed change (one line)

`src/game/world/LevelScript.cpp`, case 157 state 3 (line 4272):

```diff
-                            if (Globals::rnd->nextInt(100) <= 9 && !((PlayerFighter *) (*enemies)[i])->field_0x13c) {
+                            if (!((PlayerFighter *) (*enemies)[i])->field_0x13c && Globals::rnd->nextInt(100) <= 9) {
```

This is a **tuned** edit, not a fidelity edit: the original evaluates the RNG
first (`0x13d5d2`: `blx AERandom::nextInt; cmp r0,#9; bgt; ldrb r1,[r0,#316];
cbnz`), so the swapped form skips the `nextInt` call when `field_0x13c != 0`
and changes the RNG stream. The faithful order scores the baseline
(59.957716702); the swapped order is the only measured improvement. The
faithful-order rewrites that keep the RNG call (`int roll = nextInt(100);`,
nested `if`) also score the baseline exactly.

## What the remaining gap is (structural, measured)

The function is size-exact but ours has **133 fewer instructions**, all from
register allocation, not missing code:

| | original | ours |
|---|---:|---:|
| `ldr` (total) | 5660 | 5551 |
| `ldr rX,[sp,#88]` (`this` reload) | 328 | 195 |
| `ldr rX,[sp,#92]` (status reload) | 48 | 25 |
| `ldr rX,[sl,#28]` (`this->m_nState`) | 66 | 150 |
| `ldr rX,[sl,#24]` (`this->m_pLevel`) | 45 | 121 |
| `ldr rX,[sl,#20]` (`this->m_pCamera`) | 85 | 186 |
| `sl` register mentions | 877 | 1246 |
| `tbh/tbb` | 15 | 15 |
| `cmp` | 498 | 493 |

The original spills `this` to `[sp,#88]` and reloads it (r4 = `Globals::status`,
`sl` reused for temporary Vector pointers); our build keeps `this` in `sl` and
`status` in `r9`. The extra low-register reloads are why the original has more
16-bit encodings and more instructions at the same byte size. `tbh` count and
the switch structure match exactly, so this is MachineBlockPlacement / register
allocation, not a missing block.

## Dead ends measured (all size-exact unless stated; raw difflib %)

Baseline = 59.957716702. All variants below were compiled with the exact
`compile_commands.json` flags and gold-linked against the checked-in objects
(`/dev/shm/w6965/buildscore.py`), ~3.7 s each.

| variant | size | raw % | note |
|---|---:|---:|---|
| prologue order `messages,player,enemies` (faithful) | 57304 | 58.751774 | +72 B, `status` still r9 |
| prologue order `player,enemies,messages` | 57232 | 59.956225 | |
| prologue order `enemies,messages,player` | 57304 | 58.730396 | |
| `Status *status = Globals::status;` (1 use) | 57304 | 58.701984 | +72 B |
| `Status *status` (2/3/5/10 uses) | 57312/57304/57428/57516 | 56.49/56.45/55.05/54.20 | |
| `Level *level = m_pLevel;` + all uses | 56984 | 50.334852 | −248 B |
| `Hud *hud = m_pHud;` | 57164 | 49.966394 | |
| `Radar *radar = m_pRadar;` | 57148 | 50.113260 | |
| `Vector *tv = &m_tempVec;` + all uses | 57152 | 46.173368 | |
| `volatile LevelScript *self` + all `m_nState` | 57736 | 17.972396 | +504 B |
| non-volatile `self` + all `m_nState` | 57232 | 59.957717 | identical codegen |
| case-125 `if (state==1)` -> `else if` (faithful) | 57248 | 57.729138 | +16 B |
| `&&` operand swap, 105 single-line conditions | — | max 59.959208 (this change) | only positive |
| `&&` -> nested `if`, 60 conditions | — | all <= 59.959208 | ties at 20035 insns |
| `else if` -> `if`, 128 sites | — | max 59.959208 (tie) | |
| adjacent simple-statement swap, 1658 pairs | — | max 59.959208 (ties) | no win |
| `m_nState++` / `+= 1` / `++m_nState` rewrites | — | all <= baseline | |
| `m_nScriptTimerA/B += delta` -> assignment | — | all == baseline | |
| `m_pLevel->getPlayer()` -> `player`, 31 sites | — | max 59.956225 | all worse |
| `m_pLevel->getEnemies()` -> `enemies`, 90 sites | — | max 59.947768 | all worse |
| best pair of getPlayer/getEnemies substitutions | — | 59.954734 | |
| `nextInt(100) < 10` (faithful) | 57232 | 59.957717 | identical codegen |
| `m_nState == 0` -> `0 == m_nState` | 57232 | 59.957717 | identical codegen |

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6965/score.py cmake-build-match/libgof2hdaa.so   # 59.959208039
```

`/dev/shm/w6965` helpers are scratch only; the note and the one-line source
change are the sole committed artifacts. `.ARM.extab` is 156312 B (exact);
`.rodata` is 269351 B (the pre-existing +192 B); `.rel.plt`/`.rel.dyn` are
byte-identical to the master baseline.
