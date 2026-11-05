# LevelScript::process — wave 119 / w6757 (statement-order hill-climb, angle H)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6757`, base master
`4efc90c64`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

Metric = repo's own `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the linked
`.so` (helper `/dev/shm/w6757/score.py`).  Baseline on this tree reads
**58.48878276874099** (matches the wave-119 w6704 coordinator-merge note).

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.488783         58.498122      (+0.009339)
orig / ours insns        20169 / 20037     20169 / 20034
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
.ARM.extab (ours/orig)   0x2628c / 0x26298 0x2628c / 0x26298  (pre-existing 8 B short)
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67
```

All three landed changes are size-neutral (57232 B) and are reorders of
independent statements.  Semantics are unchanged.

## Landed changes

| # | source region | change | note |
|---:|---|---|---|
| 1 | case 4, `timer<=7000 && timer+delta>7000` (~1323) | `startFade(true,255,delta+1000)` moved before `m_nState = 5` | tuned |
| 2 | case 70 state 2 conclude (~2498) | `m_pHud->visible=1` moved before `setComputerControlled(false)`/`setTarget` | tuned |
| 3 | case 73 state 0 (~3258) | `setPosition(...)`, `sound->stop(...)` moved before `setTarget(...)` | tuned |

Change 1 is a **tuned** variant: the original case-4 block at `0x13...` emits
`startFade` and then `ldrd r0,r1,[r4]` (`m_nScriptTimerA += delta`) with **no**
`m_nState = 5` store at all (the store lives in the dropped `>= 8000` arm, see
the wave-118 w6603 note).  Changes 2/3 are also tuned: the original emits
`setLookAtCam, setTarget, setComputerControlled, hud, radar` (change 2) and
`setTarget, setPosition, stop, play` (change 3), i.e. the pre-change source order
was faithful; the reorders win only on register allocation / block placement.

Running score after each change: `58.488783 -> 58.493757 -> 58.496667 ->
58.498122`.

## Method: parallel statement-order sweeps

`/dev/shm/w6757/sweep2.py` compiles one variant `LevelScript.cpp` with the exact
`compile_commands.json` flags, gold-links it against the checked-in objects
(replacing only the `LevelScript.cpp.obj` input), and scores the resulting `.so`
with `verify.py`'s own disassembler.  40 workers, ~3 s/variant on an idle box.
Sweeps run from the current base and then hill-climb on each improved base:

* adjacent simple-statement swap (1882 candidates) — 1 win (+0.004974),
* swap statements 2 apart (1456) — 1 win (+0.002910), 1 near-miss,
* rotate three adjacent statements (1456) — 1 win (+0.001455),
* re-run adjacent from the improved base (1882) — **no** further wins.

Only size-57232 variants are landable.

Other measured dead ends (all size-exact unless noted):

| variant | % | note |
|---|---:|---|
| prologue declarations reordered to M,P,E | 57.470062 | **57300 B** (not landable) |
| M,P,E + the three landed wins | 57.473611 | **57308 B** |
| prologue P,E,M | 58.487328 | size-exact, subsumed |
| prologue M,E,P / E,M,P / P,M,E | 57.45-57.47 | **57300 B** |
| mission-0 state-4 arm moved before state-5 arm | 56.370464 | **57248 B** |
| mission-0 switch `default: break;` added | 58.498122 | identical binary |
| mission-0 `switch ((unsigned)m_nState)` | 58.498122 | identical binary |
| mission-0 state 16 moved into the switch as `case 16` | 54.775644 | **57244 B** |
| pre-switch specials: swap mission-26 / mission-5 arms | 58.418526 | size-exact |
| pre-switch specials: move mission-50/51 arms first | 57.152807 | size-exact |
| case-73 state-2: `setVisible(false)` before `m_tempVec=...` | 58.495212 | size-exact, weaker than change 2 |

## Structural findings (recorded, not landed)

**Prologue order is M,P,E, not E,P,M.**  The original prologue
(`0x137d18`, raw objdump) is unambiguous:

```
137d3a  ldr.w r0,[r4,#24]!      ; r4 = &m_pLevel
137d3e  blx   61ef0 <Level::getMessages()>
137d42  mov   r5,r0             ; messages -> r5
137d44  ldr   r0,[r4,#0]
137d46  blx   62034 <Level::getPlayer()>
137d4a  strd  r4,r0,[sp,#80]    ; &m_pLevel, player
137d4e  ldr   r0,[r4,#0]
137d50  blx   61eb4 <Level::getEnemies()>
137d54  mov   r8,r0             ; enemies -> r8
```

The current source declares them `enemies, player, messages` (E,P,M), which is
why our prologue emits E,P,M.  Reordering the declarations to M,P,E is the
faithful shape, but every M-first permutation compiles to **57300 B** (+68) and
scores ~57.47, so it cannot be landed without also fixing whatever body shape
adds the 68 bytes.  Note the original's **stack layout** still matches the
current E,P,M build (`messages` at `sp+72`, `delta` at `sp+76`,
`&m_pLevel`/`player` at `sp+80/84`, `status`/`enemies` at `sp+92/96`); the M,P,E
build puts `delta` at `sp+72` and `messages` at `sp+76`.  So the faithful source
needs M,P,E *call order* **and** the current slot assignment, which clang only
produces for the E,P,M declaration order — a genuine open reconstruction
problem, not a one-line fix.

**Mission-0 `switch` still merges states 4/5.**  Original table (tbh at
`0x13...`) covers states 6..15 (`subs #6; cmp #9`, 10 entries); ours covers
4..15 (`subs #4; cmp #11`, 12 entries).  Adding `default: break;`, casting the
switch operand, moving state 16 into the switch, or reordering the state-4/5
arms all either leave the binary identical or change the body size.  The
original's state-4 body is also placed after the mission-106 block (cold-block
placement), so table position and entry count both differ.

**tbh inventory.**  Both sides have 15 `tbh` sites, but the entry-count
multisets differ: orig `{12:1, 5:2, 15:1, 10:5, 4:3, 7:1, 6:2}`, ours
`{12:2, 5:2, 15:1, 10:4, 4:2, 7:2, 6:2}`.  Besides the mission-0 merge this is
one switch that is 7 entries in ours but 4 in the original; the remaining ~41.5 %
sequence loss is dominated by MachineBlockPlacement order and global register
allocation, not by missing statements.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target lint  -j6      # CLEAN
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 /dev/shm/w6757/score.py cmake-build-match/libgof2hdaa.so
```

## Coordinator merge note (wave 119 harvest)

w6757's base predates w6728's `startFade`-before-`m_nState` reorder, so its first
hunk is already on master. Re-applying all three hunks on the merged master grows
the body to 57240 B (byte_exact 4505 -> 2740). Measured individually on the merged
base:

| variant | size | ratio_raw |
|---|---:|---:|
| merged base (w6728) | 57232 | 58.496667 |
| H2 (setTarget/computerControlled/hud) | 57236 | 58.332504 |
| H3 (setTarget after sound->stop) | **57232** | **58.498122** |

Only H3 is landed; H2 is rejected (it changes the body size).
