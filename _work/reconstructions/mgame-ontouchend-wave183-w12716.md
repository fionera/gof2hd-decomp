# MGame::OnTouchEnd — wave 183 (w12716): hoist the instant-action test

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w12716`, branch
`wave183-w12716`, base master `9555474d8`. Scratch `/home/debian/GalaxyOnFire2/scratch/w12716`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 3,034 | **3,030** |
| aligned (instruction) diff | 1,445 | 1,446 |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 75,015 | **75,011** |
| `.rel.plt` diff | 0 | 0 |
| `.ARM.extab` diff | 646 | 646 |

Gate after the change (`check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 section headers, file size 2,187,328 exact.

## The change

In the `won` mission dispatch the source tested the campaign branch first and the
instant-action branch second:

```cpp
if (mission->isCampaignMission()) {
    Globals::status->nextCampaignMission(true);
} else if (!mission->isInstantActionMission()) {
    ...freelance...
} else {
    ...instant action...
}
```

The original binary evaluates `Mission::isInstantActionMission()` first (call at
`0x17af7e`, before the `isCampaignMission` call at `0x17afa8`), then still branches
to the campaign test.  Hoisting the instant-action predicate into a local bool keeps
the body/block order but moves the call ahead of the campaign test:

```cpp
bool instantAction = mission->isInstantActionMission();
if (mission->isCampaignMission()) {
    Globals::status->nextCampaignMission(true);
} else if (!instantAction) {
    ...freelance...
} else {
    ...instant action...
}
```

This removes the `mov r4, r0` / `cmp r4, #0` pair at the head of the instant-action
block relative to the base schedule and aligns the two predicate calls with the
original order: raw 3,034 -> 3,030 (`.text` 75,015 -> 75,011).  The body block order
is unchanged; a full body reorder (`if (instant) ... else if (campaign) ... else ...`)
was measured at raw 4,455 because it changes MBP's global block layout, and is not used.

## Rejected / measured, not landed

- Body reorder to `if (isInstantActionMission()) {instant} else if (isCampaignMission()) {campaign} else {freelance}`:
  raw 4,455 (global MBP layout shift).
- `__builtin_expect` on `isCampaignMission` (0/1) or `!isInstantActionMission` (0/1):
  raw 4,248-4,726, several variants size-fatal (8,076/8,084/8,088).
- `__builtin_expect` on `isDockingToAsteroid` / `isDockingToStream` in the
  `hudTouchFlags & 0x40` shared tail: raw 3,386-3,717.
- `count >= 0` -> `count != 0` in the ore-refine loop emits the original's `beq`
  at `0x17a5ce` but grows the function to 8,084 B (register allocation), size-fatal.
- Expression re-spellings of the ore report line (`(A+B)+C`, named locals, `String`
  temporaries): raw 3,072-7,521; the base association is already best.  The 22-byte
  `add r8, sp, #80` PostRA-scheduler hoist at `0x17a5e8` is unchanged.
- Chain-unit permutation sweep from the base order (identity re-verified at 3,034;
  original emitted order `15,22,43,42,65,81,74,95,96,100,110,120,126,134,144,155,161,162`
  measured at 3,757): no improvement over the wave-180 order.

## Residual map (unchanged)

The first differing byte is still the 22-byte String-ctor schedule at `0x17a5e8`
(`add r8, sp, #80` hoisted one slot early).  Everything from `0x17a5fe` to `0x17af82`
is byte-identical.  The rest is the campaign-chain block order/placement residual
(`0x17af82` onward) and the `dockToAsteroid` tail-merge direction at `0x17bf60`
(original merges into the `isDockingToAsteroid` site; ours into the
`isDockingToStream` site).
