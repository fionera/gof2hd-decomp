# LevelScript::process — wave 169 / w11503: mission-0 state machine

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11503`, branch `wave169-w11503`, base master
`00c403d5a`.  Scratch `/dev/shm/w11503/`.

## Baseline at hand-off (after w11403 + w11503 case-7)

```
process raw diffbytes    53,970   (w11403 hand-off)
case-7 timer/translate   53,936   (commit 985d4338f)
aligned                  17,818
orig / ours size         57232 / 57232   (size-exact)
byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## Landed: case-11 timer add, state-16 block (commit 02d220e41)

Three faithful fixes that together are size-exact and raw-positive:

1. **case 11** (`0x13d2d4`): the original starts the body with
   `m_nScriptTimerA += delta;` (`0x13d2d4..0x13d300`, the `ldrd [sl,#144]` /
   `__aeabi_l2f` / `strd [sl,#144]` sequence) and does **not** call
   `Globals::layout->enableFillScreen(true)` at all.  HEAD had the
   `enableFillScreen` call and no timer add.
2. **state 16** (`0x13f902`, the switch `default:` handler): the original is
   `enableFillScreen(true); nextCampaignMission(true); switch_to_target_setting
   = 1; initStreamOutPosition = 1; SetCurrentApplicationModule(2); return 0;`
   with **no** `m_nScriptTimerA += delta`.  HEAD had the timer add, no
   `enableFillScreen`, and `initStreamOutPosition = 0`.

Measured (full links, `tools/fn_diff.py`):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (case-7 landed) | 53,936 | 17,818 | 57,232 |
| + case-11 timer add / drop fill-screen | 54,747 | 17,821 | 57,240 |
| + state-16 fix | 54,436 | 17,815 | 57,224 |
| **all three** | **53,893** | **17,820** | **57,232** |

`check` green: byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 /
imports 207, lint CLEAN, RATCHET PASS.  `.rel.plt` untouched.

The raw metric is layout-dominated: the individual fixes are all raw-worse,
but the combination shifts the mission-0 block layout into a better alignment
with the original.  This is the same effect as the case-7 win.

## Mission-0 dispatch / block order (verified against the original)

Original dispatch at `0x1389c8`:

```
cmp r1,#4 ; beq 0x138d2a          (state 4)
cmp r1,#3 ; bne 0x138fd8          (state 3 body at 0x1389d4)
0x138fd8 cmp r1,#5 ; bne 0x139b1a (state 5 body at 0x138fde)
0x139b1a subs r0,r1,#6 ; cmp r0,#9 ; bhi default ; tbh
```

* Source order is therefore **4, 3, 5** (ours is 4, 5, 3).
* The `tbh` covers states 6..15 only; state 16 is the switch `default:`
  (`0x13f902 cmp r1,#16; bne 0x137f7c`).  Case 15's not-over path falls into it.
* Original `tbh` entry addresses: `s6=0x139b70 s10=0x13cf76 s7=0x13d022
  s8=0x13d13c s9=0x13d1de s11=0x13d2d4 s12=0x13d3b8 s13=0x13d440
  s15=0x13d4c6 s14=0x13d4f4` — physical order `6,10,7,8,9,11,12,13,15,14`.
* Ours physical order: `9,15,6,7,8,10,11,12,13,14` (`s9=0x13ce36 s15=0x13cecc
  s6=0x13cef8 s7=0x13d024 s8=0x13d198 s10=0x13d24e s11=0x13d2fe s12=0x13d3e6
  s13=0x13d474 s14=0x13d4fa`).  This is clang MachineBlockPlacement, not
  source order — reordering the `case` labels is a codegen no-op.
* Shared `m_nState = <reg>` tail: original `0x13d4ec str.w r0,[sl,#28];
  b.w 0x137f7c`, reached by states 4, 5, 12, 13, 15 (and 14/16 via their own
  tails).  Ours already hoists the same tail at `0x13d46c`; the address
  difference is block placement, not missing code.

## Remaining per-case call-sequence diff: state 8 only

Original state 8 `>= 4001` arm (`0x13d194..0x13d1da`):

```
m_nState = 9;
m_nScriptTimerA = 0;
setRumblePercentage(0,0);
TransformGetTransform(...)->SetAnimationState(0);
TransformGetTransform(...)->SetAnimationState(3);
```

Ours additionally calls `FModSound::stop(currentMusicEvent)` and
`m_pGeometry5->setPosition(player->getPosition())`, and is missing the
`m_nState = 9; m_nScriptTimerA = 0;` pair.  Fixing it is **-32 B** (57,200),
so it needs a +32 B companion before it can land.  Raw 54,847, aligned 17,806
(16 aligned better, but raw much worse).

## Measured experiments that did NOT land (all on the landed base)

| change | raw | aligned | size |
|---|---:|---:|---:|
| landed (case-11 + state-16) | **53,893** | 17,820 | 57,232 |
| case-6 translate before timer add | 53,897 | 17,822 | 57,232 |
| state-16 `initStreamOutPosition` first / after SetModule / stores swapped | 53,894 / 53,893 / 53,893 | 17,820 | 57,232 |
| state-16 `initStreamOutPosition = 0` | 54,425 | 17,816 | 57,224 |
| state-16 no `initStreamOutPosition` store | 54,634 | 17,814 | 57,216 |
| case-12 `setPosition(m_tempVec)` vector overload | 54,471 | 17,815 | 57,224 |
| state-8 faithful arm | 54,847 | 17,806 | 57,200 |
| prologue decl order `player,messages,enemies,status` | 53,905 | 17,819 | 57,232 |
| prologue decl order `messages,enemies,player,status` | 53,905 | 17,816 | 57,232 |
| prologue decl order `status,messages,player,enemies` | 55,511 | 17,644 | 57,312 |
| 4,3,5 pre-switch reorder | 55,184 | 18,495 | 57,248 |
| `__builtin_expect` sweep (all mission-0 cells, e1) | 54,777..55,577 | — | 57,168..57,352 |

The `__builtin_expect` sweep on the landed base: `msg6`, `isPlaying(142)`,
`messages[7]`, `messages[8]`, `messages[13]` are exact codegen no-ops
(54,840/17,804/57,200 — but note that is *not* the landed layout); everything
else is worse.

## Case-body statement orders verified faithful

* state 4: stop(currentMusicEvent), play(143), setTurretMode(false),
  resetCamera, setFreeLookMode(false), enableFirstPersonCam(false),
  hideShipForFirstPersonCameraView(false), m_bCinematicBreak=1,
  getPlayer/setCollide(false), radar.enabled=0, setComputerControlled(true),
  removeAllGuns, setLookAtCam(true), setTarget, setRotation(0,pi/2,0),
  getPosition, setPosition, translate(25000,-200,-1000), m_nState=5.
* state 5: matches 1:1 including the `speed == 2.0f` arm and the
  `speed * 0.98f` brake; `stop(player->field_0x1c)` (w11403).
* state 9: m_nState=10 first, then switchPlanet/switchSkybox/setPosition(0,0,0)/
  both particle resets/setDirection/rotate(pi/4)/setPosition(player pos)/
  camera setPosition(5000,500,-10000)/timer=0.  Matches.
* state 12: Update, spin=delta/3000, rotate, then on messages[17] isOver:
  tempVec=getPosition, tempVec+={-2000,-2000,-5000}, camera setPosition
  (Vector overload), timer=0, m_nState=13.
* state 13: timer+=delta, spin=delta/4000, rotate, at >=6001: tempVec=
  getPosition, +={700,0,-1700}, setPosition(Vector), m_nState=14, timer=0.
* state 14: timer+=delta, spin=delta/5000, rotate, camera translate(0,0,-2*delta),
  at >=12001: tempVec=getPosition, +={-3000,3500,-6700}, setPosition(Vector),
  m_nState=15, timer=0.
* state 15: messages[22] isOver -> startFade(true,255,5000), m_nState=16.
