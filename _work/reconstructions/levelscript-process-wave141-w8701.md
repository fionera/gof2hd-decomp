# LevelScript::process — wave 141 / w8701: the `setStanding` PLT anchor on current master

Status: **note-only** (no source change landed; no gate-green variant found).
Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, size-exact. Owned TU: `src/game/world/LevelScript.cpp`.
Base: wave-141 head `1d10d0694`. Scratch helpers in `/dev/shm/w8701/`
(not tracked): `fdiff.py`, `harness.py`, `secdiff.py`, `relplt.py`, `align.py`,
`try2.py`, `moveanchor*.py`, `sweepcases.py`.

## Result

```
                         HEAD (measured)
process raw diffbytes    54,108
orig / ours size         57232 / 57232     (size-exact)
whole .text diffbytes    84,567
.rel.plt                 0  (byte-identical, 29,392 B)
.plt                     0
.ARM.extab               1,395 (size 156,312 exact)
```

The mission's premise (w8603 lead 1: "removing the anchor is size-neutral and
lowers raw by 180") **does not reproduce on current master**.  It reproduces
exactly on the pre-w8601 base `e18e0c2a4` (plus the w8603 `getEnemies` edit):
there the removal is `57232 B`, raw `54,138 -> 53,957`.  On HEAD it is `-24 B`
and raw `54,108 -> 55,118`, because the w8601 `s4gp` and `case40` edits re-roll
MachineBlockPlacement and change the anchor-removal size budget.

## Measured on HEAD

| variant | size | raw | `.rel.plt` |
|---|---:|---:|---:|
| HEAD | 57232 | 54,108 | 0 |
| remove anchor (`X`) | **57208** | 55,118 | 26 |
| `X` + revert `s4gp` (`B_rev`) | 57200 | 54,709 | 26 |
| `X` + revert `case40` (`D_rev`) | 57224 | 54,760 | 26 |
| `X` + revert **both** `s4gp`+`case40` | **57232** | **53,970** | 26 |
| move anchor to case-29 state-1 body (`case29s1`) | **57232** | **54,005** | 26 |
| move anchor to case-40 body start (`case40start`) | 57232 | 54,050 | 10 |
| move anchor to case-67 body start | 57232 | 54,145 | 0 |
| move anchor to case-29 `case 29: {` head | 57224 | 55,077 | 0 |
| move anchor to case-14 body start | 57232 | 54,170 | 26 |
| move anchor to case-16/21/42 body start | 57224 | ~55,070 | 17 |
| move anchor to case-24 body start | 57212 | 55,091 | 10 |
| move anchor to case-56 body start | 57224 | 55,061 | 4 |
| move anchor to case-64 body start | 57224 | 55,113 | 16 |
| move anchor to the function prologue | 57312 | 55,376 | 23 |
| move anchor to the mission-50/51 body gate | 57232 | 54,396 | 16 |
| drop anchor + both missing `getEnemies` conditions (`e24`+`e16a`) | 57240 | 54,816 | 26 |

### Why `s4gp`/`case40` matter

`e18e0c2a4` + w8603 `A` + anchor removal = **57232 B / 53,957**.  Adding the
w8601 `s4gp` (`m_pLevel->getPlayer()->setCollide(false)`) alone makes the same
removal `+8 B` (57240); adding `case40` (nested state-0 test) alone makes it
`-32 B` (57200); together they give `-24 B` (57208).  Both are genuine
faithfulness fixes verified against the original (`0x138d86` fresh `getPlayer`
for `setCollide`; nested `m_nState == 0` / `isTriggered` test), so reverting
them to buy the anchor removal is a faithfulness regression.  `B_rev+D_rev`
without the removal is raw `54,134` (26 worse than HEAD), so the net of
"revert both + remove anchor" is `54,108 -> 53,970` (`-138`) but `.rel.plt`
is still 26 bytes off and the gate fails.

## The `.rel.plt` constraint (unchanged)

`Standing::setStanding` must be first referenced before
`TargetFollowCamera::setRotationAroundTarget` (both in the window at
`.rel.plt` index 2816).  The original's first reference is the real
exit-wormhole-body call at `0x139b06`; ours is the dead anchor at `0x139b68`
(case 41), and the real call sits at `0x13be00`/`0x13be14`, far later.

Removing the anchor rotates 13 symbols (26 differing `.rel.plt` bytes) and
drops `byte_exact 4507 -> 4504` (measured with `check`).  `.plt` itself stays
byte-identical; the loss is the `.rel.plt` order.

`case29s1` is the best raw variant found: it moves the anchor into the case-29
state-1 body, where clang **merges** it into the real exit-wormhole call
(size-neutral, `57232 B`) and gives raw **54,005** (`-103`).  But the merged
call stays at `0x13be00`, so `.rel.plt` is still 26 bytes off.  Every position
that restores `.rel.plt` (before `0x139b70`) either shifts the function size
(case-29 head: `-8`) or is placed after `setRotationAroundTarget`.

## Why this is blocked

The only size-neutral, `.rel.plt`-clean, raw-improving placement would need the
real exit-wormhole body (and its `setStanding` call) to be block-placed at the
original `0x139ac2..0x139b12`, i.e. **before** the case-41 block.  On HEAD the
body is at `0x13bdba` and case 41 at `0x139b60` — the two blocks are inverted
relative to the original.  That is a MachineBlockPlacement consequence of the
whole-function CFG, not a local spelling; `case29s1` proves the merge is
size-neutral but does not move the block.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8701/harness.py     # {'size': 57232, 'raw': 54108, 'textdiff': 84567, 'relplt': 0}
```

`case29s1` edit: delete the case-41 anchor, and insert after `m_nState = 1;`
in the case-29 state-1 body:

```cpp
if (delta == 0x7d)
    ((Standing *) (intptr_t) Globals::status->getStanding())->setStanding(0, 100);
```

## Open questions

* Find a size-neutral `.rel.plt`-clean position for the anchor that also keeps
  the `case29s1` merge (raw 54,005).  A `-8`/`+8` companion pair around the
  case-29 head placement is the obvious next probe.
* Or move the exit-wormhole body before the case-41 block by matching the
  original's CFG; that would let the anchor be removed outright.

## Full case-start sweep (anchor moved to the head of each switch case)

All measured on HEAD (anchor deleted from case 41, re-inserted at the head of
the listed case).  `size` is the process symbol size; only rows with
`size == 57232` are size-exact.

| case | size | raw | `.rel.plt` |
|---|---:|---:|---:|
| 0 | 57228 | 55,053 | 19 |
| 1 | 57228 | 55,081 | 16 |
| 2 | 57236 | 54,578 | 17 |
| 3 | 57232 | 54,150 | 17 |
| 4 | 57236 | 54,574 | 17 |
| 5 | 57236 | 54,584 | 17 |
| 6/8 | 57224 | 55,081 | 20 |
| 7 | 57224 | 55,098 | 20 |
| 9 | 57224 | 55,095 | 20 |
| 10 | 57232 | 54,168 | 20 |
| 12 | 57232 | 54,143 | 20 |
| 11 | 57224 | 55,083 | 20 |
| 14 | 57264 | 54,969 | 15 |
| 16 | 57224 | 55,076 | 17 |
| 21 | 57224 | 55,066 | 17 |
| 24 | 57212 | 55,091 | 10 |
| 29 | 57224 | 55,077 | **0** |
| 40 | **57232** | **54,050** | 10 |
| 41 | 57240 | 54,973 | 0 |
| 42 | 57224 | 55,066 | 17 |
| 56 | 57224 | 55,061 | 4 |
| 64 | 57224 | 55,113 | 16 |
| 67 | **57232** | **54,145** | **0** |
| 69 | 57232 | 54,169 | 0 |
| 70 | 57232 | 54,179 | 0 |
| 73 | 57232 | 54,169 | 0 |
| 80 | 57224 | 55,068 | 4 |
| 91 | 57224 | 55,062 | 17 |
| 92 | 57232 | 54,154 | 6 |
| 94 | 57232 | 54,164 | 17 |
| 102 | 57224 | 55,067 | 10 |
| 105 | 57224 | 55,059 | 17 |
| 114 | 57224 | 55,055 | 17 |
| 125 | 57224 | 55,058 | 6 |
| 135 | 57232 | 54,179 | 9 |
| 131 | 57232 | 54,184 | 16 |
| 139 | 57224 | 55,278 | 26 |
| 142 | 57232 | 54,181 | 6 |
| 144 | 57232 | 54,137 | 6 |
| 145 | 57224 | 55,106 | 9 |
| 154 | 57216 | 55,046 | 10 |
| 157 | 57224 | 55,054 | 8 |
| 158 | 57224 | 55,133 | 16 |

No case head gives `size == 57232`, `.rel.plt == 0` **and** `raw < 54,108`.
The two `.rel.plt`-clean size-exact placements (cases 67/69/70/73) are all
raw-positive (`54,145..54,179`).  The only raw-negative size-exact placement
is case 40 (`54,050`, `-58`), but it leaves `setStanding` after
`AEGeometry::isVisible` (first referenced at `0x13a14a` in case 125) and so
`.rel.plt` is 10 bytes off.
