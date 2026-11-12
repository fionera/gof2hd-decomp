# MGame::OnTouchEnd — wave 190 (w13378): finishMission block + 96/127 split lenses

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13378`, branch
`wave190-w13378`, base master `1904beaf7`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13378`.

Prior notes: `mgame-ontouchend-wave189-w13318.md` (full-body/RA, `!keepObjectives`
720-sweep on the pre-w13357 base), `-w13323.md` (96/127 split analysis, body order),
`-w13344.md` (split compensation search), `-w13357.md` (escort-block raw win that set
the current 2,913 base).

## Result

No size-exact raw win was found. **`src/` is unchanged (base).** This note records the
measurements so the next pass does not repeat them, and one structural finding about
the finishMission block.

| metric | base |
|---|---:|
| raw same-address diff bytes | 2,913 |
| aligned (instruction) diff | 1,443 |
| function size | 8,080 / 8,080 |
| insn count | 2,775 / 2,771 |
| `.text` differing bytes | 73,505 |
| `.ARM.extab` size / diff | 156,312 / 610 |
| `.rel.plt` diff | 0 (byte-identical) |
| build-id diff | 20 |
| total differing bytes (text+extab+build-id) | 74,135 |
| file size | 2,187,328 exact |

Base is a strong local minimum: the finishMission 720-permutation sweep, the full
adjacent-transposition sweep, the per-block getter reorder sweep, and the full
single-block chain-order sweep (18 blocks x 18 positions) all fail to beat raw 2,913.

## 1. `!keepObjectives` finish-mission block — original order is raw-worse

The original's finishMission `!keepObjectives` block is at `0x17b26c` and emits
**A B C F D E** (`removeObjectives`, `setMission`, `setRoute`, `goingToWaypoint` guard +
`setAutoPilot`, `removeRoute`, `setPlayerRoute`), i.e. the *same* order as the
failed-mission copy at `0x17b89c` (which our build already matches byte-for-byte):

```
17b26c  ldr  r0,[fp,#120]
17b270  blx  Level::removeObjectives
17b274  ldr  r0,[pc,#340] ; add r0,pc ; ldr r0,[r0] ; ldr r1,[r0] ; ldr r0,[r8]
17b280  blx  Status::setMission
17b284  ldr  r0,[fp,#88] ; movs r1,#0
17b28a  blx  PlayerEgo::setRoute
17b28e  ldr  r0,[fp,#88]
17b292  blx  PlayerEgo::goingToWaypoint
17b296  cbz  r0, 17b2a2
17b298  ldr  r0,[sp,#24] ; movs r1,#0 ; ldr r0,[r0]
17b29e  blx  PlayerEgo::setAutoPilot
17b2a2  ldr  r0,[fp,#88]
17b2a6  blx  PlayerEgo::removeRoute
17b2aa  ldr  r0,[fp,#120] ; movs r1,#0
17b2b0  blx  Level::setPlayerRoute
```

Our finishMission block emits **A F B C D E** (guard hoisted before `setMission`) at
`0x17b3de`, 0x16e bytes *after* the original's block. Reordering the source to the
original A B C F D E is size-exact (8,080) and `.rel.plt`-identical but **raw
2,913 -> 2,922 (+9)** and `.text` 73,505 -> 73,514: the block lands at `0x17b3de`, where
the original has block 65's body, so a same-address comparison cannot reward the
content fix. The 0x16e shift is the chain-order difference (ours puts 74 and 162 before
42; the original puts 15,22,43,42 first), not this block.

Full 720-permutation sweep of the six statements (guard position free) on the current
base: best raw is **2,913** for `A F B C D E` (landed), tied with `A F B D C E`,
`A F B D E C`, `A F B E D C`. All keep size 8,080. The original `A B C F D E` is 2,922.

## 2. 96/127 split — still +8 body, still raw-hostile

The split is structurally correct (the original's 127 test at `0x17bc24` is
`mov r0,r5; blx isCampaignMission; cbz; ldr r0,[r8]; blx
getCurrentCampaignMission; cmp r0,#127; beq.w 17b722`, jumping into the shared 96
body) but duplicates the 8-byte `isCampaignMission` predicate. On the current base the
split after 126 is 8,088 B (+8), raw 3,874.

All 20 source placements of the 127 block were re-measured on the current base (18
chain blocks + the four `if` forms). The only size-exact placement is **before 120**
(raw 3,574 / aligned 1,456 / 8,080); the others are 8,072/8,084/8,088/8,092/8,104/8,112/
8,116. Adding the original finishMission order (size-neutral) keeps 8,080 but raw
~3,583. The split's ~+450 raw floor is the chain body/tail layout, exactly as
w13349 concluded; it cannot be landed as a raw win until the chain bodies are fixed.

`count != 0` (the original's `cmp.w r9,#0; beq.n`) still flips the global `sl`/`r9`
allocation and grows the function (+8 base / +8 split). `count > 0` is size-exact and
semantically correct but raw 2,915 (+2), `.text` 73,507 (+2), `.ARM.extab` unchanged
(610). `amount <= 0` is byte-identical code to `count >= 0` but grows `.ARM.extab`
610 -> 612 (w13357).

## 3. `won` block restructure to the original shape — +4 B

The original checks `instantAction` *first* (`0x17af82 cbz r0, 17afa6`), then
`isCampaignMission`, and does `setFreelanceMission` before `showMissionRewardMessage`.
Reconstructing that shape:

```cpp
if (instantAction) { switch_to_target_setting = 2; active = 0; module(1); return; }
if (mission->isCampaignMission()) nextCampaignMission(true);
else { setFreelanceMission(empty); showMissionRewardMessage(...); }
```

is 8,084 B (+4), raw 4,706 (or 4,710 with `showMissionRewardMessage` first). Not
size-exact; the +4 is the extra branch. (`else if` spelling is identical.)

## 4. Negative sweeps on the current base

- **Adjacent transpositions** (all same-indentation simple-statement pairs in
  `OnTouchEnd`, 256 candidates): no candidate beats raw 2,913; ~40 are neutral.
- **Per-block getter reorder** (each of blocks 15/22/43/65/74/81/95/96 to the
  original HP,Shield,Armor,Gamma order): neutral (2,913) or +1 (2,914). Block 100's
  full original order is -20 B (8,060) and is not usable alone.
- **Single-block chain-order move** (18 blocks x 18 positions, proper `if`/`else if`
  reassembly): base order is optimal at 2,913; the best alternative moves are neutral
  (162<->43, 43->15/162) and everything else is >= 2,916. `74` and `162` were included
  this time (the earlier w13318 sweep treated them as fixed/merged).
- **`String("t ")` schedule** (named temp inside/outside the if, split `+=`, `report =
  report + ...`): named-in-if 2,951; the others are size-fatal (8,016-8,096) and raw
  6,400+. The 22-byte `add.w r8,sp,#80` hoist at `+0x4a4` is caused by the `count`
  branch, not the string-expression shape (confirmed again).

## Residual map (unchanged)

First differing byte is still `count >= 0` emitting `bgt.n` where the original emits
`beq.n` at `+0x48b`. Then the 22-byte String-ctor schedule at `+0x4a4` and its 4-byte
literal-pool consequence. The dominant residual is the campaign-chain block
order/placement from `+0xe3e` (0x17b044..0x17be44 holds ~2,900 of the 2,913 diff bytes;
the whole pre-chain region 0x17a144..0x17b044 has only ~99). The tail-merge direction
at `0x17bf60` still differs (original merges the isDockingToStream path into the
isDockingToAsteroid `dockToAsteroid`+`hudEvent(6)` site; ours is the reverse), but both
directions are 8 bytes total, so it is placement-only.

## Reproduction

Scratch scripts (`/home/debian/GalaxyOnFire2/scratch/w13378/`): `fn_diff.py` (raw /
aligned / size), `sec_diff.py` (section diffs), `sweep_keep.py` (720 finishMission
permutations), `sweep_adj.py` (adjacent transpositions), `sweep_getters.py`
(per-block getter order), `sweep_chainmove3.py` (single-block chain moves, 18 blocks),
`sweep_127pos.py` (127 placement), `try_string.py` (String schedule). All builds used
`ninja -j2` (`ulimit -v 12000000`).

## 5. Second session (same worker): body-reconstruction lens

All measurements on the same base (raw 2,913 / 8,080 / rel.plt 0). Nothing landed;
`src/` unchanged. New scratch: `patch_recon.py`, `patch_escort2.py`,
`patch_won3.py`, `patch_won4.py`, `patch_mining2.py`, `patch_keep2.py`,
`patch_decl.py`, `patch_dock.py`, `sweep_recon1.py`, `sweep_fin_move.py`,
`sweep_b96_move.py`.

### 5.1 Full per-block body reconstruction (original emitted statement order)

Extracted the exact original statement order of every chain block from the
disassembly and rewrote the source to match (getter order HP,Shield,Armor,Gamma;
`switch_to_target_setting` first; `new FileRead` after the getters; escort block
`field_0x1e0` + `makeItem` + `setEquipment` before the `Mission` ctor; back-to-station
tail `sw=0` first, `active=0` last). Ten independent replacements, each measured
alone on the base order:

| block | raw | size |
|---|---:|---:|
| 65 | 2,916 | 8,080 |
| 96/127 | 2,914 | 8,080 |
| 100 | 3,649 | **8,048** (size-fatal) |
| 95 | 2,953 | 8,080 |
| 110 | 2,939 | 8,080 |
| 81 | 2,918 | 8,080 |
| 126 | 2,915 | 8,080 |
| 161 | 2,915 | 8,080 |
| 162 | 2,917 | 8,080 |
| back-to-station tail | 2,923 | 8,080 |

All ten together in base order: 8,080 / raw 3,014. In the original chain order:
8,084 / raw 4,281. So even with byte-faithful bodies the original order is +4 and
raw-hostile — the residual is compiler **block placement**, not statement order.

Block 100's original body is **-32 B** (8,048), not the -20 B reported earlier
(the earlier number was getter order only; the full original order also moves
`module(5)` after `departStation` and `initStreamOut`/`sw` before it).

### 5.2 Escort block (post-finishMission) — original order is raw-worse

Original order: `field_0x1e0 = level->killCountB;` → `makeItem(10)` →
`ship->setEquipment` → `new Mission(...)` → `setMission` → `setFreelanceMission`
→ `active=0` → `module(2)`. Rewriting to that order: 8,080 / raw 2,922 (base 2,913),
`.ARM.extab` 612. With the full body reconstruction + original chain order: 8,084 /
raw 4,271.

### 5.3 `won` block: the +4 is a pre-won layout shift, not the block itself

The `keepObjectives` computation (`0x17add2..0x17ae06`) is **byte-identical** in the
base. The base's won prologue is also byte-identical through `0x17af7e`
(`isInstantActionMission`). The only source difference is the order of the
`instantAction` and `isCampaignMission` tests. Every original-shape spelling
(direct call, `bool instantAction`, `else if`, `switch_to_target_setting` before
`active`) measures 8,084 / raw 4,706-4,710 with `.rel.plt` diff 8 (rotation of
`skipCutscene`/`removeObjectives`/`goingToWaypoint`/`removeRoute`). The `shift.py`
trace shows the +2/+4/+8 is introduced at `0x17adb0..0x17adc6` (the `failed == 1`
region), i.e. the compiler re-places cold blocks; the won block itself is
size-neutral. Not landable.

Other won/condition variants, all byte-identical to base (raw 2,913):
`hasWon() == true` → `hasWon()`, `||` → `if (!won)`, `?:` → `&&` for
`keepObjectives`, `keepObjectives = false; if (...) ...`, and declaration
permutations that keep keepObjectives→mission→instantAction order. Moving
`mission`/`instantAction` before `keepObjectives` is worse (3,302 / 8,084).

### 5.4 Mining `count` condition — `count != 0` is still +8

Re-confirmed on the base: `count != 0`, `if (count)`, `amount != 0`, `if (amount)`,
`String(-amount)` all give 8,088 / raw 6,592 (register-alloc flip `sl`/`r9`).
`count > 0` / `count >= 1` stay 8,080 / raw 2,915.

### 5.5 Chain-order + body searches

- `!keepObjectives` A B C F D E **plus** every single-block chain move (324 builds):
  best 2,922 (order `74,15,43,162,42,22,65,120,96,100,95,110,81,134,144,126,155,161`).
- block-96 body reorder **plus** every single-block chain move (324 builds):
  best 2,914 (same order). No combination beats 2,913.
- Swapping the `isDockingToAsteroid` / `isDockingToStream` branches (to flip the
  `0x17bf60` tail-merge direction): 8,080 but raw 3,245 — much worse.

### 5.6 Conclusion

The base remains a strong local minimum at raw 2,913. The remaining ~2,595 diff
bytes in the chain region are dominated by clang **block placement** (cold
`failed == 1` / `menuTouchOpen` / engine-sound blocks interleaved between chain
blocks), which is not reachable by statement reordering alone. Matching it would
require reproducing the original CFG/profile hints, not just the block bodies.
