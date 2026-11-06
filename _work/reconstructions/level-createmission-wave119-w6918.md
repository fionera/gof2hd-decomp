# w6918: `Level::createMission` (92.293729 -> 92.326733 %, 17172 B) — wave-119

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6918`, branch `wave119-w6918`,
based on master `be3034dc1`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(function `_ZN5Level13createMissionEv`, image base 0x10000, `0xaffd8`-`0xb42ec`,
17172 B). Continues `level-createmission-wave119-w6834.md` (and the notes before it).

## Result

| | before | after |
|---|---:|---:|
| exact fuzzy score | **92.293729 %** | **92.326733 %** |
| size | 17172 B | 17172 B |
| instructions | 6055 / 6065 | 6055 / 6065 |
| gate `byte_exact` | 4505 | 4505 |
| gate `linked_exact` | 4505 | 4505 |

`cmake --build cmake-build-match --target check -j6`: **RATCHET PASS**, byte 4505,
linked 4505, data 843/843, imports 207, lint CLEAN. `drift`: 0 fields.
No section, `.ARM.extab`, `.rodata`, or file-size change (27 sections, 2,187,328 B).

## Kept source change

The `bigShipsCount` / `localRace` decision was written as the positive conjunction:

```cpp
if (*bigShipsCount > 0 && localRace == 0) {
    ... alien wave ...
} else if (localRace == 1 && *bigShipsCount >= 1 && Globals::rnd->nextInt(100) <= 29) {
    ... pirate fortress ...
}
```

The original emits `cmp #0; ble` for the first test.  The positive spelling
emits `cmp #1; blt` (the compiler folds `> 0` into `>= 1`).  Rewriting the
same decision with the negated first test restores the original `cmp #0; ble`:

```cpp
if (*bigShipsCount <= 0 || localRace != 0) {
    if (localRace == 1 && *bigShipsCount >= 1 && Globals::rnd->nextInt(100) <= 29)
        pirateFortress = Globals::status->getCurrentCampaignMission() > 140;
} else {
    if (Globals::rnd->nextInt(100) <= 29) {
        if (Globals::rnd->nextInt(100) <= 29)
            bigAlienWave = Globals::status->getCurrentCampaignMission() > 103;
        alienWave = 1;
    }
}
```

`if (!(*bigShipsCount > 0) || localRace != 0)` and `if (*bigShipsCount < 1 || localRace != 0)`
are equivalent and score identically.  Swapping the operands (`localRace != 0 || ...`)
is much worse (73.885771 %) because it moves the cold `localRace` test first.

## Variant measurements

All measurements are exact-score %, 17172 B unless noted.

### `bigShipsCount` condition

| variant | % |
|---|---:|
| **`*bigShipsCount <= 0 \|\| localRace != 0` (kept)** | **92.326733** |
| `!(*bigShipsCount > 0) \|\| localRace != 0` | 92.326733 |
| `*bigShipsCount < 1 \|\| localRace != 0` | 92.326733 |
| `*bigShipsCount <= 0 \|\| localRace` | 92.326733 |
| `localRace != 0 \|\| *bigShipsCount <= 0` | 73.885771 (17180 B) |
| `localRace \|\| *bigShipsCount <= 0` | 73.885771 (17180 B) |
| `*bigShipsCount == 0 \|\| *bigShipsCount < 0 \|\| localRace != 0` | 83.403483 (17192 B) |

### Convoy `k`-chain (case 183, function-relative `0x1434`)

The original is 68 B (`0x1434`-`0x1478`); ours is 70 B (`0x1434`-`0x147a`).
The two streams re-sync at `0x1a14` and `0x2ada`.  Original block layout is
`[k=0][k=1][k=2][default][shared store]` with tests `cmp #2; beq; cmp #1; beq; cbnz`.
Source-order permutations and every `switch` case/default position were measured;
the closest layouts are:

| variant | % |
|---|---:|
| **current descending `if/else if` (kept, x3 base)** | **92.326733** |
| `switch`, default first (any case text order) | 92.326733 |
| `switch` cases `2,0,1`/`2,1,0`, default any position | 92.326733 |
| `if` nested descending | 92.326733 |
| negated `k != 2 / k != 1 / k != 0` chain | 92.326733 |
| `bool` temporaries for the three tests | 92.326733 |
| `switch`, default last (cases `0,1,2` or `0,2,1`) | 92.178218 |
| ascending `if` chain (`0,1,2`) | 92.178218 |
| independent `if` chain | 92.178218 |
| 4-case `switch` (case 3 = default body) | 92.251836 (TBB) |
| `if (k >= 2)` / `if (k < 2)` split | 89.734280 / 89.701271 (17164 B) |
| `Vector offset` local instead of `float offset[3]` | 89.950495 (17176 B) |

The ascending/switch-default-last form does reproduce the original test sequence
(`cmp #2; beq; cmp #1; beq; cbnz`), but `MachineBlockPlacement` lays the bodies out
`[0][2][1]` and puts the shared store before `default`, so it still needs the extra
`b.n` and does not match the original `[0][1][2][default]` layout.

### Turret table select (alienGuns loop, original `0xb32c6`)

Original:

```text
ldr  r0, [sp,#64]      ; bigAlienWave
cbz  r0, use_outpost
ldr  r1, [pc,#732]     ; kAlienStationTurrets
add  r1, pc
b    join
use_outpost:
ldr  r1, [pc,#728]     ; kAlienOutpostTurrets
add  r1, pc
join:
add.w r0, r1, r9
```

Ours uses a conditional move (`cmp.w r1, r9, asr #31; it ne; movne r6, r0`).
The `(i < 0)` anchor in the current source is required: without it the compiler
hoists the select out of the loop and the function shrinks to 17140 B (88.413577 %).
With the anchor, `if/else`, `switch`, `?:`, `&&`, `&`, `|`, `^` and `-` spellings
all stay at 92.326733 % or lower; the branch-vs-cmov choice is not reachable from
the source at this register pressure.

### Raid / stationPirates loop guards (original `0xb2e04`, `0xb2f32`)

Original merges the `if (raid)` / `if (stationPirates)` guard into the loop
condition as a branchless

```text
subs  r0, fp, r3
it    ne
movne r1, #1
ldr   r2, [sp,#60]
eor.w r2, r2, #1
orrs  r1, r2
bne   after
```

i.e. `(i != bound) | (flag ^ 1)`.  Ours emits two branches
(`if (flag == 0) goto after; if (i != bound) goto after;`).  Local-bound rewrites
and branchless `&&` spellings were swept in w6834/w6814 and scored 88.7-91.1 %;
left as a residual.

### Other residuals

* Stack-slot colouring differs at the first spill: original saves `fp` at
  `[sp,#88]`, ours at `[sp,#84]` (and the `[sp,#84]`/`[sp,#88]` pair is swapped
  for the corresponding locals).  Prologue and frame size are identical
  (`sub sp,#400`), so this is register-allocator stack-slot colouring, not a
  source-shape difference.
* Convoy store order: original stores `cargo = 0` (`0x50`) then
  `crateCaptured() = 0` (`0x4c`); ours emits the reverse.  Swapping the two
  source statements is neutral (92.326733 %); a direct `hasCargo = 0` store
  changes the store width and scores 84.7 %.
* Literal-pool offsets after the convoy block are shifted by the 2-byte size
  difference and account for most of the remaining `ldr [pc,#N]` diffs.

## Gate

```text
byte_exact 4505 / 4524
linked_exact 4505 / 4524
size_exact 4524 / 4524
data_exact 843 / 843
imports 207
lint CLEAN
drift 0
RATCHET PASS
```
