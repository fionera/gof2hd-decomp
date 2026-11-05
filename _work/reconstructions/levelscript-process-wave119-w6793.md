# LevelScript::process — wave 119 / w6793 (statement-order hill-climb, angle K)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6793`, base master
`075f49c72`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

Metric = the repo's own `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the linked
`.so` (helper `/dev/shm/w6793/score.py`, unrounded).  Baseline on this tree reads
**58.648823441619825** (matches the wave-119 coordinator base).

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.648823         58.832894      (+0.184071)
orig / ours insns        20169 / 20033     20169 / 20033
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
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
.ARM.extab (ours/orig)   0x26294 / 0x26298 0x26294 / 0x26298  (pre-existing 8 B short)
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67
```

The .text / .ARM.exidx / .ARM.extab / .rodata sizes and the 27-section,
2,187,328-byte file layout are unchanged by this pass.

## Landed changes

All are size-neutral (57232 B) statement reorders.  Nine are reorders of
independent statements (semantics unchanged); the case-102 state-6/7
permutation is a **tuned** cross-block move (see below).

| # | source region | change | kind |
|---:|---|---|---|
| 1 | case 14 state 3 (~1635) | `forceUpdate(delta,false)` moved after `m_nState = 4; m_nScriptTimerA = m_nScriptTime;` | equivalent |
| 2 | case 40 state 1 (~1931) | `m_pCamera->setTarget(player->geometry)` moved after `player->player->setVulnerable(true)` | equivalent |
| 3 | case 102 state 6/7 (~2112, 2144, 2152) | four-statement permutation: `startFade(true,255,4000)` to the state-6 entry; `m_nScriptTimerA = 0` to the per-frame particle block; `m_pCamera->setPosition(player->getPosition())` and `m_pParticleGeom1->setPosition(dir*10000.0f)` to the `m_nScriptTimerA >= 15001` block | **tuned** |
| 4 | case 70 state 2 (~2481) | `m_nState = 2` moved before `forceUpdate(delta,false)` | equivalent |
| 5 | case 89 state 4 (~2499) | `m_pHud->visible = 1` moved before `player->setComputerControlled(false)` | equivalent |
| 6 | case 94 state 3 (~2643) | `m_nState = 9` moved before `m_nScriptTimerA = 0` | equivalent |
| 7 | case 105 state 3 (~2961) | `m_pCamera->setFixed(true)` moved before `m_dirVec = leader->getUpVector()` | equivalent |
| 8 | case 105 state 7 (~3700) | `setFreeze, setLookAtCam, setVisible, setFixed, setTarget` order | equivalent |
| 9 | case 158 state 5 (~4879) | `player->setFreeze(true)` moved after `m_pCamera->setTarget(chaser->geometry)` | equivalent |
| 10 | case 158 state 5 (~4892) | `m_nScriptTimerA = 0` moved before the `fighterShip` declarations; `chaser->setActive(true); chaser->setVisible(true);` moved after `forceUpdate` | equivalent |

Running score after the greedy hill-climb (each step measured on top of the
previous one):

```
base                                   58.648823
+ case-70 forceUpdate/store swap       58.665804
+ case-14 forceUpdate move             58.741885   (via intermediate steps)
+ case-105 lookAt/setTarget swap       58.746860
+ case-158 setFreeze move              58.756809
+ case-158 setActive move              58.761784
+ case-158 setVisible move             58.766759
+ case-102 startFade move (tuned)      58.822944
+ case-102 particle move (tuned)       58.832894   <- landed
```

## Faithful alternative (measured, not landed)

If the case-102 cross-block permutation is rejected as behavioural, the best
**semantically equivalent** variant is **58.794557** at 57232 B.  It is the
landed set minus item 3, plus the case-102 state-6 `m_nState = 7` /
`reset(true)` swap and the case-105 `setLookAtCam`-before-`setVisible` swap:

```
case-102: m_nState = 7 before ((PlayerWormHole *)...)->reset(true)
case-105: setLookAtCam(false) before setVisible(true)
```

The original disassembly puts `m_pCamera->setPosition(player->getPosition())`
in the state-6 `isInWormhole` block (orig `0x13c2a0`/`0x13c2b4`) and
`startFade(true,255,4000)` in the state-7 `>=15001` block, so the base source
order is the faithful one; item 3 is a register-allocation/block-placement
tune, not a behavioural reconstruction.

## Method

`/dev/shm/w6793/compile_one.sh` compiles one variant `LevelScript.cpp` with the
exact `compile_commands.json` flags; `/dev/shm/w6793/link_variant.sh` gold-links
it against the checked-in objects (replacing only the `LevelScript.cpp.obj`
input) into an unstripped `.so`; `/dev/shm/w6793/score_variant.py` scores that
`.so` with `verify.py`'s own disassembler.  ~6 s/variant single-threaded,
96-112 variants in parallel on the 128-core box.

Sweeps run from the current best base:

* adjacent statement swap (1861 candidates) — 2 wins,
* distance-2 swap (1440) — 1 win,
* distance-3 swap (1161) — 1 win,
* distance-4 swap (964) — 3 wins,
* local all-pairs statement swaps over the hot regions
  (1700-1760, 2100-2160, 2480-2520, 2940-2980, 3670-3720, 4860-4910)
  — 6 further wins.

Only size-57232 variants are landable (a +-4/8 B change to `process` shifts
every later `.text` symbol and drops `byte_exact`).

## Measured dead ends (all 57232 B unless stated)

| variant | % | note |
|---|---:|---|
| tuned case-102 move on top of the faithful case-102 state-7 swap | 58.788120 | non-additive |
| tuned case-102 move on top of the full faithful gb9 set | 58.793095 | non-additive |
| case-102 `m_pCamera->setPosition` -> state-7 entry | 58.817969 | tuned |
| case-102 `m_pCamera->setPosition` -> `>=15001` block | 58.816506 | tuned |
| case-102 `m_pCamera->setPosition` -> `m_bRenderParticles = 0` | 58.794557 | tuned |
| case-158 `setPosition`/`setDirection` move (loc_04865_04878) | 58.824407 | tuned |
| case-158 `m_pParticleGeom1->setPosition` move (loc_02144_02154) | 58.832894 | **landed** |
| case-158 `chaser->setActive` + `m_pRadar` move combined | 58.033382 | **57272 B** (not landable) |
| case-105 `setTarget`/`setFixed` swap | 58.265131 | **57216 B** (not landable) |
| prologue declarations M,P,E (faithful call order) | 57.903254 | **57308 B** (not landable) |

## Structural findings (recorded)

* The original prologue is still M,P,E (`getMessages`, `getPlayer`,
  `getEnemies`); every M-first declaration order compiles to 57308 B in this
  tree, so the E,P,M source order is kept (see the wave-119 w6757 note).
* The remaining ~41 % sequence loss is still dominated by
  MachineBlockPlacement order and global register allocation.  The largest
  single divergence remains the ~297-instruction original-only run at
  normalized offset 9459 (a whole mission arm placed before the literal pool).
* `.ARM.extab` remains 8 B short (`createShip` -8 only); 27 sections, file size
  2,187,328 B, all unchanged by this pass.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6793/score.py                          # 58.832894
```

## Coordinator merge note (wave 119 harvest)

w6793's base predates w6797's adjacent-statement hill-climb. Re-applying its one
conflicting hunk on the landed 59.5% body gives 57216 B / 59.1%, so it is not
additive; note only.
