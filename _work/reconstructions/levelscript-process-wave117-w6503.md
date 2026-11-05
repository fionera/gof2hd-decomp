# LevelScript::process — wave 117 / w6503 (case-154 state-1 statement reorder)

Status: **code change landed, 2026-11-05**. Branch `wave117-w6503`, base master
`c22678d64` (wave-116 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.193215         58.196110      (+0.002895)
orig / ours insns        20169 / 20035     20169 / 20033
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4501              4501
linked_exact             4501              4501
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.text                    0x1337c8          0x1337c8
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26294           0x26294   (pre-existing 4 B short)
```

Raw number = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6503/score.py`).  All 27 section headers, the exact file size and
the pre-existing `.ARM.extab` shortfall are unchanged.

**Size-exactness of this function is mandatory**: a ±4/8 B change to `process` shifts
every later `.text` symbol and drops `byte_exact` 4501 -> 3688.  The landed change is
therefore size-neutral.

## What was applied (one statement reorder, case-154 state 1)

`src/game/world/LevelScript.cpp`, the `case 154:` `m_nState == 1` arm
(the `m_nScriptTimerA >= 1501` epilogue, source ~4510):

```c++
// before (master)                          // after (landed)
m_tempVec = player->getPosition();          m_tempVec = player->getPosition();
m_pCamera->setTarget(player->geometry);     m_pCamera->setFixed(false);
m_tempVec += Vector{-350,400,-1500};        m_nState = 2;
m_pCamera->setPosition(m_tempVec);          m_pLevel->lodManager->forceUpdate(delta, false);
m_pCamera->setFixed(false);                 m_nScriptTimerA = 0;
m_nState = 2;                               m_tempVec += Vector{-350.0f, 400.0f, -1500.0f};
m_pLevel->lodManager->forceUpdate(...);     m_pCamera->setPosition(m_tempVec);
m_nScriptTimerA = 0;                        m_pCamera->setTarget(player->geometry);
```

The three `m_tempVec` statements keep their relative order (`= getPosition()` ->
`+= offset` -> `setPosition(m_tempVec)`), so the camera ends up at the same
(offset) position as in the original; only the interleaving with the independent
`setFixed` / `m_nState` / `forceUpdate` / `m_nScriptTimerA` / `setTarget` setters
changes.  This is a *tuned* reorder (semantically equivalent for independent
setters), not a provably faithful one: the original's emitted order is
`setTarget, getPosition, +=, setPosition, setFixed, m_nState=2, forceUpdate,
timerA=0` (see below).

## Ground truth: the original's order

The block is at original `0x1444be..0x144520` (case 154, `m_nState == 1`,
`m_nScriptTimerA >= 1501` arm; reached from `0x13e966`/`0x14444c`):

```
0x1444be  ldr r1,[r5,#8]        ; player->geometry
0x1444c0  ldr r0,[r8,#20]       ; this->m_pCamera
0x1444c4  blx TargetFollowCamera::setTarget
0x1444c8  add r4,sp,#368
0x1444ca  mov r1,r5              ; player
0x1444ce  blx PlayerEgo::getPosition
0x1444d2  add.w r5,r8,#40        ; &this->m_tempVec
0x1444da  blx Vector::operator=
0x1444de  ldr r2,[pc]            ; 400.0f   @0x1448b0
0x1444e0  ldr r0,[pc]            ; -350.0f  @0x1448b4
0x1444e2  ldr r1,[pc]            ; -1500.0f @0x1448b8
0x1444e4  strd r0,r2,[sp,#368]   ; Vector{-350,400,-1500}
0x1444ea  str r1,[sp,#376]
0x1444ee  blx Vector::operator+=
0x1444f2  ldr.w r0,[r8,#20]
0x1444f6  mov r1,r5
0x1444f8  blx TargetFollowCamera::setPosition
0x1444fc  ldr.w r0,[r8,#20]
0x144500  movs r1,#0
0x144504  blx TargetFollowCamera::setFixed
0x144508  ldrd r0,r1,[r8,#24]
0x14450e  adds r1,#1
0x144510  str.w r1,[r8,#28]      ; m_nState = 2
0x144514  ldr r1,[sp,#76]        ; delta
0x144518  blx LODManager::forceUpdate
0x14451c  strd r4,r4,[r6]        ; m_nScriptTimerA = 0
0x144520  b.w 0x144e36           ; shared tail
```

So the original's source order is `setTarget, getPosition, +=, setPosition,
setFixed, m_nState=2, forceUpdate, timerA=0`.  The landed order differs only in the
placement of the independent setters; the `m_tempVec` chain and the final camera
position are preserved.

## Measurements (all on the wave-116 base unless noted)

### Case-154 state-1 epilogue: exhaustive 280-permutation search

All permutations of the eight statements that keep the data dependency
(`m_tempVec = getPosition()` < `m_tempVec += offset` < `setPosition(m_tempVec)`)
were built and scored (helper `/dev/shm/w6503/permblock3.py`).  Only size-57232
variants are listed; the best faithful-*ish* one was landed.

| order (abridged) | % | size |
|---|---:|---:|
| master (baseline) | 58.193215 | 57232 |
| **landed (setFixed/state/forceUpdate/timerA first, then `+=`, setPosition, setTarget)** | **58.196110** | **57232** |
| setFixed/state/forceUpdate first, then `= getPosition`, timerA, `+=`, setPosition, setTarget | 58.194662 | 57232 |
| setTarget, getPosition, setFixed, state, forceUpdate, timerA, `+=`, setPosition | 58.164814 | 57232 |
| setFixed, state, forceUpdate, setTarget, getPosition, timerA, `+=`, setPosition | 58.163367 | 57232 |
| faithful `setTarget` before `getPosition`, rest as master | 57.975773 | **57228** (not landable) |

The faithful `setTarget`-before-`getPosition` swap is -4 B (57228), which shifts every
later symbol and drops `byte_exact` to 3688, so it cannot be landed without a
compensating +4 B elsewhere.

### Other single-statement moves found by the random sweep (all tuned, not landed)

A random single-statement move sweep over the whole body (287 statement blocks,
2046 statements; helper `/dev/shm/w6503/movestmt.py`) found two further size-exact
improvements, both **semantically wrong** and therefore not landed:

| move | % | size | why not landed |
|---|---:|---:|---|
| `m_nState = 13;` moved after `return m_bCinematicBreak != 0;` (case 9, ~line 1207) | 58.196742 | 57232 | store becomes dead; original emits `movs r0,#13; str r0,[sl,#28]` at `0x13b8b2..0x13b8b8` before the return |
| `m_tempVec += Vector{-350,400,-1500};` moved after `m_nScriptTimerA = 0;` (case 154, ~line 4512) | 58.196110 | 57232 | `setPosition` then receives the un-offset vector |
| both together | 58.199637 | 57232 | both of the above |

### Dead ends (no size-exact win)

* Case-158 adjacent-statement sweep (lines 5147-5292): nothing above baseline.
* Case-16 adjacent-statement sweep (lines 1641-1770): nothing above baseline.
* Case-80 adjacent-statement sweep (lines 2569-2776): nothing above baseline.
* Case-131 adjacent-statement sweep (lines 4037-4447): nothing above baseline.
* Random 3-statement rotations (200 samples, seed 7) over the whole body: best
  58.183265 (worse).
* Random single-statement moves (250 samples, seed 11): found the case-154 move above.
* Random single-statement moves (400 samples, seed 23): found the case-9 dead store above.

## Structural findings this pass (recorded, not landed)

* **The case-92 state machine is already a 7-entry `tbh` in our build** (ours
  `0x13a152`, original `0x13a024`), despite the source being a linear `else if`
  chain.  Converting the source to an explicit `switch (m_nState)` (cases 1..7 with
  the message check inside case 7, then `if (>=9 && <=10) / else if (==11) /
  else if (==8)`) measures 57.722627 @ **57272 B** (+40) - a dead end.  The handoff
  note that called this "case 125" was a mislabel: the mission-125 entry is
  `0x139f9c` (route creation); the 7-entry table at `0x13a024` belongs to mission 92
  (`cmp r0,#92; beq 0x139ffe` at `0x13882c`).
* **The original's top-level mission compare chain order is**
  `0, 1, 5, 81, 26, 78, 50, 89, 106, 95-group, 51, [switch guard]`
  (original compare sites `0x137d62, 0x137d7c, 0x138132, 0x1381b8, 0x1382e2,
  0x138332, 0x1383ea, 0x13857a, 0x138c80, 0x138e4a, 0x13877c`).  Our source order is
  `0, 1, 81, 78, 89, 106, 95-group, 26, 5, 50, 51`; swapping 26/5 is size-neutral
  but scores 57.968707 (wave-116 measurement, re-confirmed shape).
* **All 15 original `tbh` tables are PC-relative.**  Our build also has 15, one of
  which (`0x13a6f4`, case 154's 10-entry `switch (m_nState - 1)`) uses the
  register-based form `tbh [r1, r0, lsl #1]` after `addw r1, pc, #8`.  The original's
  case-154 non-zero-state dispatch is a PC-relative 10-entry table at `0x13e8f0`
  (`subs r0,#1; cmp r0,#9; bhi tail; tbh [pc,r0,lsl #1]`), i.e. the same case set.
* The mission-78 `m_nState >= 5 && <= 8` camera-drift epilogue
  (`0x13c88c..0x13c8ba`, `translate((float)-delta * 0.8f, 0, 0)`) is still missing
  from our source.  Adding it after the mission-78 switch gives 51.470076 @ 57200
  (-32); before the switch gives 57.913714 @ 57280 (+48).  Neither is size-exact.
* The remaining ~42 % sequence loss is still global register allocation / stack-slot
  assignment.  Example: in case-92 state 1 the original keeps `Vector ref` at
  `sp+280` and the `ref + offset` result at `sp+368`; ours uses `sp+368` for `ref`
  and `sp+280` for the result.  In the prologue the original's accessor order is
  `messages, player, enemies` and its stack layout is `72=messages, 76=delta,
  80=&m_pLevel, 84=player, 88=this, 92=status, 96=enemies`; the baseline `enemies,
  player, messages` declaration reproduces that stack layout exactly but emits the
  accessors in the wrong order.  Declaring `messages, player, enemies` reproduces
  the accessor order but swaps the `messages`/`delta` slots (`72=delta, 76=messages`)
  and costs +80 B (57312, 57.203316).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

**Build gotcha:** the build is timestamp-driven.  After any `git checkout`/restore of
`src/game/world/LevelScript.cpp` you must `touch` it before `link_so`, otherwise the
score is computed on a stale `.so` (58.19 vs 58.18 oscillations).
