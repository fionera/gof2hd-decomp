# Level::createMission — wave 192 w13987 (entry block, +0x000..+0x2d4)

Target: `_ZN5Level13createMissionEv` (`0xaffd8`, 17,172 B, size-exact).
Base master `49e4c89ca`. Baseline and current measured state (unchanged, no source edit):
raw **4,265** / aligned **386** / size **17,172** / oinsn 6,197 vs uinsn 6,198.

## Result: entry block is already byte-identical

```
cmp -n $((0x2d4)) -i $((0xaffd8)):$((0xaffd8)) \
    android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so   # identical
```

The prologue and everything up to (and including) the `tbh` instruction at `0xb02a8`
match instruction-for-instruction. There is therefore no entry-block source change
that can move the metric; the first raw difference is downstream, at **+0x2d6**.

## First divergence: `tbh` table entries for cases 1 and 3

The jump table starts at `0xb02ac` (right after `e8df f011` = `tbh [r1, r0, lsl #1]`).
Only two of its 14 halfword entries differ:

| table entry | original | ours | delta |
|---|---|---|---|
| case 1 (`+0x2d6`) | `0x140a` | `0x130a` | -0x100 |
| case 3 (`+0x2da`) | `0x8a0b` | `0x890b` | -0x100 |

Every other table entry is identical. The two case bodies are each 0x100 bytes
shorter in ours, i.e. the residual is entirely downstream case-body layout
(owned by w13979's largest-aligned-cluster work), not the entry block.

## Entry-block shape (for reference)

- `push {r4,r5,r6,r7,lr}; add r7,sp,#12; stmdb {r7,r8,r9,sl,fp}; vpush {d8-d14}; sub sp,#400`
- `fp = Globals::globals` (`ldr.w fp,[r0]`), `r5 = Globals::status` (`ldr r5,[r1,#0]`)
- stack guard `[sp,#396]`; `Status::getMission()` -> r9, null early-return at `0xb0194`
- `this->player->geometry->getPosition()` -> `[sp,#260]`; `Status::inAlienOrbit()`
- slots: `[sp,#124]=this`, `[sp,#108]=status`, `[sp,#88]=globals`
- alien-orbit branch: count via `rnd->nextInt(2)` (campaign 68/33 -> 2),
  `new Array<KIPlayer*>`, `createShip(9,0,getRandomEnemyFighter(9),nullptr,true,false)`
- empty-mission branch: `rnd->reset()`, `missionPtr=0`, `createScene()`, `missionPtr=3`,
  intro-system check (`getSystem()->getIndex()==15 && getCurrentCampaignMission()<16`),
  station index -> `[sp,#104]`, `rnd->nextInt(100)` raid roll, freelance mission type
  0/11/15, difficulty/10*5
- non-empty branch at `0xb0262`: `getSystem()->getRace()`, `rnd->nextInt(100)`,
  `<75 ? 8 : getStanding()->getEnemyRace(race)` -> `[sp,#116]`, `mission->getType()`,
  then the `tbh` at `0xb02a8` (`cmp r1,#14; bhi 0xb10c6`).

## Register-allocation divergence (downstream, not entry)

| | original | ours |
|---|---|---|
| `Globals::globals` | `ldr.w fp,[r0]` -> **fp** | `ldr r6,[r0]` -> **r6** |
| `Globals::status` | `ldr r5,[r1,#0]` -> **r5** | `ldr.w r8,[r1]` -> **r8** |

Original reloads `this` from `[sp,#124]` at block boundaries; ours keeps it in r8.
Original slots: `[sp,#84]`=raid, `[sp,#104]`=stationIndex, `[sp,#108]`=status,
`[sp,#116]`=introSystem, `[sp,#124]`=this, `[sp,#88]`=globals.
Ours: `[sp,#104]`=status, `[sp,#108]`=security, raid in r8; literal pool 16 B later
(`0xb036c` -> `0xb037c`). That cascade is the w13979 cluster, not this task.

## Tried and reverted in this pass (do not repeat)

1. Removing fortress `Vector p` / `Vector q` -> 16,648 / 2,000 / 17,308
2. Braced `{Vector p...}` / `{Vector q...}` scopes -> same
3. w13849 `extraAliens start` reorder -> neutral
4. w13773 variants: `int guns` after `if` -> 17,164; `Vector p` removed -> 17,312;
   plain ternary (7 spellings) -> 17,156
5. `int er = *(volatile int *) &enemyRace;` -> raw 16,473 (known dead end)

No measured win available from the entry block; tree left clean, nothing committed.

## CORRECTION (w13999, 2026-11-14 05:45 UTC) — the table values above were byte-swapped

The `tbh` table starts at `0xb02ac` and the target is `0xb02ac + 2*value`, so the
halfwords must be read little-endian from `0xb02ac + 2*index`. Re-dumped:

| idx | offset | original | ours | original target | ours target | delta |
|---|---|---|---|---:|---:|---:|
| 1 | `0xb02ae` | `0x0a14` | `0x0a13` | `0xb16d4` | `0xb16d2` | **2 bytes** |
| 3 | `0xb02b2` | `0x0b8a` | `0x0b89` | `0xb19c0` | `0xb19be` | **2 bytes** |

Every other entry is identical. The earlier "`0x140a` vs `0x130a`" / "`0x8a0b` vs
`0x890b`" figures were byte-swapped reads of these same halfwords, and the
"0x100 bytes shorter" conclusion was **wrong**: each affected body is only
**2 bytes** short, and the 2-byte shift is what cascades into the 4,265 raw
diff. The two bodies to fix are the block ending at `0xb16d4` (idx 1) and the
block ending at `0xb19c0` (idx 3). w13999 verified idx 2/4 (shared body
`0xb02d0`-`0xb10c4`, 3,572/3,572 bytes) and `0xb10c6`-`0xb158c`
(1,222/1,222 bytes) are identical.
