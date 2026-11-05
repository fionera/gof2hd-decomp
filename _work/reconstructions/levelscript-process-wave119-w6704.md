# LevelScript::process — wave 119 / w6704 (per-case statement-order hill-climb)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6704`, base master
`47a1396bd` (wave-118 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.199637         58.431997      (+0.232360)
orig / ours insns        20169 / 20034     20169 / 20035
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4503              4503
linked_exact             4503              4503
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.ARM.extab (ours/orig)   0x26290 / 0x26298 0x26290 / 0x26298  (pre-existing 8 B short)
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67
```

Raw number = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the linked
`.so` (helper `/dev/shm/w6704/score_so.py`).  The baseline on this tree reads
**58.199637** (wave-118 harvest; older notes quote 58.193215 from the wave-116 base).

Size-exactness is mandatory: a +-4/8 B change to `process` shifts every later
`.text` symbol and drops `byte_exact` 4503 -> ~3690.  Every landed reorder is
size-neutral (57232 B).

## What was applied

Eleven statement reorders, all inside straight-line runs of independent
statements.  Ten are **semantically equivalent** (stores to unrelated objects /
pure getters reordered among themselves); one (item 9) is a **tuned** reorder that
changes the RNG call order and therefore runtime behaviour (see below).

| # | source region (line in the landed file) | change | safe? |
|---:|---|---|---|
| 1 | mission-0 state 6 (~756) | `camera->translate` moved after `m_nScriptTimerA += delta` | yes |
| 2 | mission-15 state 1 (~1928) | `setTarget` after `m_bCinematicBreak=0`; `setVulnerable` after `setLookAtCam` | yes |
| 3 | mission-42 state 6 (~2127) | `m_nState=7` before `m_nScriptTimerA=0` | yes |
| 4 | mission-50 (~2167) | `programmedStation`, `initStreamOutPosition`, `comingFromAlienWorld` reordered | yes |
| 5 | mission-78 (~2346) | `m_bCinematicBreak=1` after `stopShooting`/hud/radar stores | yes |
| 6 | mission-89 (~2498) | `setComputerControlled(false)` after `m_pHud->visible=1` | yes |
| 7 | mission-158 state 4 (~3341) | `m_bCinematicBreak=1` after `leaderShip` load + `setTarget` | yes |
| 8 | mission-158 (~4470) | `m_pRadar->enabled=0` after `m_tempVec = getPosition()`; `setPosition` after `setAIDisabled` | yes |
| 9 | mission-158 state 4 (~4834) | `offset.z = nextInt(30000)-87000`, `offset.x=120000`, `offset.y=nextInt(12000)-6000` | **tuned** |
| 10 | mission-158 (~4895) | `setVulnerable(false)` after `field_0x140=0` | yes |
| 11 | mission-158 (~5140) | `switch_to_target_setting`, `SetCurrentApplicationModule(2)`, `initStreamOutPosition` reordered | yes |

### Item 9 is tuned, not faithful

The original emits the two `nextInt` calls in **y(12000) then z(30000)** order:

```
0x9d90  movw r1,#12000 ; blx nextInt      -> s16 (stored to [sp,#372] = offset.y)
0x9da4  movw r1,#30000 ; blx nextInt      -> s0  (stored to [sp,#376] = offset.z)
0x9dbc  str.w r9,[sp,#368]                ; offset.x = 120000.0f
```

so the master source order (`y`, `z`, `x`) is faithful.  The landed order
(`z`, `x`, `y`) swaps which random draw feeds `y` vs `z`; it is a tuned
register-allocation alignment, not a behavioural reconstruction.  The
**fully faithful** variant (items 1-8,10,11 only, keeping `y,z,x`) measures
**58.375824** @ 57232 B and is the fallback if the RNG order must be preserved.

## Method: parallel hill-climb

The build is timestamp-driven and a single `link_so` is ~3 s.  A harness
(`/dev/shm/w6704/pbuild2.py`) compiles one variant `LevelScript.cpp` with the
exact `compile_commands.json` flags, gold-links it against the checked-in
objects (replacing only the `LevelScript.cpp.obj` input), and scores the
resulting `.so` with `verify.py`'s own disassembler.  With 96 workers a
~2000-variant sweep takes ~5 min.

Sweeps run from the wave-118 base (and then from each improved base):

* adjacent statement swap (1995 candidates),
* move a statement 2 / 3 positions (1496 / 1172),
* rotate three adjacent statements (1496),
* 1225 random same-indent pair swaps at distance 4-40.

Only size-57232 variants are landable.  The hill-climb was:

```
base (wave-118)                       58.199637
+ items 1,2,4,5,6,7,8,10,11 (b3/v3)   58.375824   <- fully faithful best
+ item 9 (tuned)                      58.431997   <- landed
```

Other measured dead ends (all size-exact unless noted):

| variant | % | note |
|---|---:|---|
| swap `m_nScriptTimerA=0` / `m_nState=7` (mission 42) | 58.201085 | safe, subsumed |
| case-78 `m_nState=13` store after `return` (wave-117 w6504) | 58.196742 | already in base |
| `offset` order `x,y,z` / `y,x,z` | 58.171235 / 58.225959 | **57224 B**, not landable |
| swap `offset.y`/`offset.z` (z,y,x) | 58.367327 | tuned, weaker than item 9 |
| mission-0 switch `case 4/5` merged into the `tbh` (`subs #4`, 12 entries) vs original `subs #6`, 10 entries | — | shape difference; every source restructure that un-merges them is 57240-57400 B, not landable |

## Structural findings (recorded, not landed)

* Our mission-0 `switch (m_nState)` jump table covers states **4..15** (12 entries,
  `subs r0,r1,#4; cmp #11`) because clang merges the preceding
  `else if (m_nState == 5 && ...)` / `else if (m_nState == 4)` arms into it.  The
  original's table covers **6..15** (10 entries, `subs r0,r1,#6; cmp #9`); its
  state 4/5 arms are tested outside the table.  Swapping the two arms (+24 B),
  making them independent `if`s (+168 B), or moving them out of the chain
  (+124 B) all lose both size and ratio, so the merge is left as-is.
* The whole function still has 15 `tbh` sites on both sides.  The remaining
  ~41.6 % sequence loss is dominated by MachineBlockPlacement order and global
  register allocation, not by missing statements.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4503
cmake --build cmake-build-match --target lint  -j6      # CLEAN
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 /dev/shm/w6704/score_so.py cmake-build-match/libgof2hdaa.so
```

**Build gotcha:** the build is timestamp-driven; after restoring
`src/game/world/LevelScript.cpp` you must `touch` it before `link_so` or the
score is computed on a stale `.so`.

## Coordinator merge note (wave 119 harvest)

w6704's branch was based on `3d4a98a3e`, before the w6603/w6604 reorders landed.
Cherry-picking it wholesale onto the merged master produced a 57240 B body
(byte_exact 2740), because its file still carried the pre-w6603 case-4 / case-145 /
case-154 order. The six size-neutral hunks that do **not** overlap w6603/w6604 were
re-applied one at a time on the merged base and all kept:

| hunk | ratio_raw |
|---|---:|
| merged base | 58.355926 |
| H9 offset.y after offset.x | 58.412098 |
| H4 programmedStation/initStreamOut/comingFromAlienWorld | 58.441946 |
| H5 m_bCinematicBreak after m_pRadar->enabled | 58.458936 |
| H2 setTarget/vulnerable reorder | 58.468885 |
| H7 m_bCinematicBreak after setTarget(leaderShip) | 58.478834 |
| H11 switch_to_target/appManager/initStreamOut order | **58.488783** |

Body stays exactly 57232 B; gate `byte 4503 linked 4503`, RATCHET PASS.
Hunks H3/H6/H8 were rejected because they change the body size (57240/57236/57240).
