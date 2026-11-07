# LevelScript::process — wave 137 / w8203 (case-125 state-2 body statement order; -16 B companion found, .extab still the blocker)

Status: **analysis only, no source change landed**, 2026-11-07. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**. Owned TU: `src/game/world/LevelScript.cpp`. Base master
`655e3bcc5` (wave-136 docs). Scratch helpers in `/dev/shm/w8203/` (not tracked):
`gen.py`, `gen2.py` (variant generators), `measure.py`, `lsda.py`, `lsdacmp.py`,
`dumplsda.py`.

## Result

```
                         baseline (HEAD)   else-if + loop/freeze
official verify pct      58.9              56.6
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
orig / ours insns        -                 -
process diffbytes        54327             54316   (-11)
whole .text diffbytes    (baseline)        -
.ARM.extab size          156312            156316  (+4 -> byte_exact 4507 -> 4044)
.ARM.exidx               exact             exact
.rel.plt                 differs 4 B       differs 4 B
byte_exact               4507              4044    (RATCHET FAIL)
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
```

## New hypothesis tested

The handoff asked: does a different **statement order in the case-125 state-2
body** (not the dispatch keyword) remove the +16 B of the `else if` +
`__builtin_expect` shape without changing the LSDA?

**Answer: yes for the 16 B, no for the LSDA.** Two independent statement-order
reorders in the case-125 state-2 body each save exactly 8 B on the else-if
shape; together they take 57248 -> 57232 B. The `.ARM.extab` is unchanged by
the reorder (still 156316, the else-if shape's value), so the LSDA is still the
ratchet blocker. On the current base the else-if shape is also a *pct
regression* (56.5 vs 58.9), so it would not be a win even if the extab were
fixed.

## The two reorders (both are faithfulness fixes vs the original)

### (a) case-125 state-2 ambush loop — call order must match the original

The original's loop body (`0x13b578..0x13b6b4`) issues its calls in this order:

```
(*enemies)[i]->awake();
KIPlayer *pirate = (*enemies)[i];       // 0x13b58c, right after awake()
Vector playerPos = player->getPosition(); // 0x13b594
int sideZ = Globals::rnd->nextInt(100);   // 0x13b5a4
int offsetY = Globals::rnd->nextInt(1000);// 0x13b5b0
int offsetZ = Globals::rnd->nextInt(2000);// 0x13b5bc
int sideX = Globals::rnd->nextInt(100);   // 0x13b5c6
```

Our source currently emits `awake, sideZ, getPosition, sideX, pirate, offsetY,
offsetZ` (verified in the base build at `0x13c1ec..0x13c23a`). Reordering the
local declarations to the original sequence is a **-8 B** change on the else-if
shape (and a **+168 B** change on the switch shape — the block placement
interacts with the dispatch, see below).

### (b) case-125 state-2 teardown — `setFreeze` belongs after `setVisible`

The original's teardown (`0x13b6c0..0x13b7ee`) is:

```
stopShooting(0)
m_bCinematicBreak = 1
m_pHud->visible = 0
m_pRadar->enabled = 0
m_pCamera->setLookAtCam(true)
player->setVisible(false)
player->setFreeze(true)      // 0x13b738, AFTER setVisible
m_pCamera->setTarget(leader)
...
```

Our source has `setFreeze(true)` *before* the three stores / `setLookAtCam` /
`setVisible`. Moving it after `setVisible(false)` is a **-8 B** change on the
else-if shape (size-neutral on the switch shape).

### Combined source diff (applied to the w8101 else-if candidate)

```diff
@@ case 125 state 2 ambush loop @@
                         for (int i = 2; i < 6; i++) {
                             (*enemies)[i]->awake();
-                            int sideZ = Globals::rnd->nextInt(100);
-                            Vector playerPos = player->getPosition();
-                            int sideX = Globals::rnd->nextInt(100);
                             KIPlayer *pirate = (*enemies)[i];
+                            Vector playerPos = player->getPosition();
+                            int sideZ = Globals::rnd->nextInt(100);
                             int offsetY = Globals::rnd->nextInt(1000);
                             int offsetZ = Globals::rnd->nextInt(2000);
+                            int sideX = Globals::rnd->nextInt(100);
@@ case 125 state 2 teardown @@
                         player->stopShooting(0);
-                        player->setFreeze(true);
                         m_bCinematicBreak = 1;
                         m_pHud->visible = 0;
                         m_pRadar->enabled = 0;
                         m_pCamera->setLookAtCam(true);
                         player->setVisible(false);
+                        player->setFreeze(true);
```

## Full experiment table (all on current HEAD `655e3bcc5`)

| variant | size | diffbytes | .ARM.extab | pct |
|---|---:|---:|---|---:|
| **master (switch)** | **57232** | **54327** | **156312** | **58.9** |
| `elseif` (w8101 shape, no body reorder) | 57248 | 55018 | 156316 | 56.5 |
| `elseif` + loop reorder | 57240 | 55030 | 156316 | — |
| `elseif` + freeze reorder | 57248 | 55015 | 156316 | — |
| **`elseif` + loop + freeze** | **57232** | **54316** | **156316** | **56.6** |
| `elseif` + loop + freeze, no `__builtin_expect` | 57232 | 54303 | 156316 | 56.6 |
| `elseif` + full anchor + loop + freeze | 57264 | 55047 | 156316 | — |
| `elseif` + full anchor + case-24 + loop + freeze | 57408 | 55230 | 156312 | — |
| switch + loop | 57400 | 55429 | 156312 | — |
| switch + freeze | 57232 | 54416 | 156312 | — |
| switch + loop + freeze | 57400 | 55406 | 156312 | — |

The switch-shape numbers confirm the reorders are **dispatch-coupled**: the
loop reorder alone is +168 B on the switch shape but -8 B on the else-if shape,
so it cannot be landed on the switch shape without a large companion.

## Why the .extab still grows (LSDA decode)

The process LSDA is at `.ARM.extab` `0x1c2af8` (personality `0x5eb60`), next
entry at `0x1c2c54`, i.e. 0x15c bytes in both the original and the base. The
LSDA header is `LPStart=omit (0xff), TType=omit (0xff), call-site enc=uleb128
(0x01), length=uleb128`. Decoding the call-site table (46 entries, each
`uleb start, uleb length, uleb landing_pad, byte action`):

| build | call-site table length |
|---|---:|
| original | `0x144` |
| master (switch) | `0x146` |
| else-if (+ loop/freeze) | `0x148` |

The base's +2 bytes are absorbed by 2 trailing zero pad bytes at the end of its
0x15c-byte LSDA; the else-if's +4 bytes are not, so the *next* function's extab
entry moves `0x1c2c54 -> 0x1c2c56` and `.ARM.extab` grows 156312 -> 156316,
shifting `.rodata`/`.data`/`.bss` and dropping `byte_exact` 4507 -> 4044.

The +2 vs the base comes from four call-site fields whose uleb128 width changes
(base -> else-if): entry [12] landing pad 1 -> 3 bytes (+1), entry [15] landing
pad 3 -> 1 byte (-1), entry [16] start offset 2 -> 3 bytes (+1, `14794 ->
17470` crosses the 0x4000 7-bit/14-bit boundary), entry [17] start offset 2 -> 3
bytes (+1, `14802 -> 17478`). The offending call sites are the case-125 state-0
`new Route(routePoints, 3)` / `setPlayerRoute` / `setRoute` calls, which the
else-if block placement pushes from `~0x39ca` to `~0x443e` (function-relative).
Pulling them back below `0x4000` would shrink the call-site table back to
`0x146` and make `.ARM.extab` exact again — but that needs a ~0x43e-byte block
move, not a statement reorder.

## Conclusion / open questions

- The handoff hypothesis is **confirmed for size**: the case-125 state-2 body
  statement order (`awake/pirate/playerPos/sideZ/offsetY/offsetZ/sideX` and
  `setFreeze` after `setVisible`) removes the else-if's +16 B exactly
  (57248 -> 57232) and even improves raw diffbytes (55018 -> 54316).
- It is **not landable**: `.ARM.extab` stays 156316 (the reorder is LSDA-neutral
  relative to the else-if shape), which fails the ratchet.
- The else-if shape is also a **pct regression on the current base** (56.5-56.6
  vs 58.9). w8101's "58.9" was measured on the pre-wave-136 base; the wave-136
  case-102/145/154 and case-40 changes re-rolled the global layout, and the
  else-if cold-chain placement now scores worse than the switch.
- Next wave: if the else-if shape is to be pursued at all, the lever is the
  case-125 state-0 `Route` call-site offset crossing 0x4000 (a ~0x43e block
  move), not the 16 B. Otherwise keep the switch shape and look for a different
  diffbytes lever.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
python3 /dev/shm/w8203/gen2.py elseif loop freeze   # writes the candidate
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8203/measure.py                   # 57232 B, diffbytes 54316, extab 156316
python3 /dev/shm/w8203/lsdacmp.py base.so ei.so     # call-site table 0x146 vs 0x148
```
