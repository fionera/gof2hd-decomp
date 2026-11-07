# MGame::OnTouchEnd — wave 130 (w7675): mining-counter form, exact campaign/hudMenu content, cold-block order

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7675`,
base `003a9623a`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). No source change
landed (every size-8080 variant that improved `.text` regressed `.rel.plt` and/or `.ARM.extab`;
the exact-content reconstruction is 12 B short). Scratch `/dev/shm/w7675*` (not committed).

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.
Baseline (verified): fuzzy **81.9 %**, 8080/8080 B, `.text` **85,441**, `.ARM.extab` **1,427**,
`.rel.plt` **0**, `.ARM.exidx` 0, `.rodata`/`.data`/`.bss` exact.

## New findings (all measured against the original binary)

### 1. The base mining loop is already faithful; only the guard predicate differs

Contrary to the wave-129 summary, the *current* tree's mining loop is the original's
decrementing form:

```
orig 17a58c mov.w r9,#0          base 17a58c mov.w r9,#0
orig 17a5b6 sub.w r9,r9,#1       base 17a5b6 sub.w r9,r9,#1     ; amount--
orig 17a5da rsb r1,r9,#0         base 17a5da rsb r1,r9,#0       ; String(-amount)
orig 17a5ca cmp.w r9,#0          base 17a5ca cmp.w r9,#-1
orig 17a5ce beq.n 0x50e          base 17a5ce bgt.n 0x50e
```

The only mismatch is the guard predicate: original `amount != 0` (`cmp #0; beq`), base
`count > 0` (`amount < 0`, canonicalised by the frontend to `cmp #-1; bgt`). The source is
`int count = -amount; if (count > 0)`, and clang's frontend emits
`%count = sub 0, %amount; icmp slt %amount, 0` (base.ll block 266). The original's IR must be
`icmp ne %amount, 0`.

### 2. Every `amount != 0` spelling canonicalises the counter and flips the tail

`if (amount != 0) { String(-amount) }` (M1) and all its spellings (`!!amount`,
`(unsigned)amount`, `amount != 0U`, `count != 0`, `int count = -amount; if (amount != 0)`,
`amount < 0`) emit `add.w r8,r8,#1` + `cmp.w r8,#0; beq` and shrink the body to **8064 B**.
`amount < 0` is 8084 B. The canonicalisation is not a frontend transform (the IR is already
decrementing); it happens in the ARM backend, and it is triggered by the `icmp ne` predicate
(`icmp slt` keeps the decrementing `sub r9,r9,#1`).

M1 reproduces the original's **cold-block order** exactly: `0x400 body -> shared blx ->
test -> isAutoPilot -> return -> stack_chk_fail -> cold hudTouchFlags==0 body`, and puts the
0x400 case (not the 0x200 case) as the shared-`blx` owner. It is 16 B short and swaps
`this`/`amount` between `sl`/`r9`.

### 3. The exact campaign content is 12 B short of 8080

Reconstructed from the original disassembly (`0x17adc8..0x17ae06`, `0x17af5a..0x17af7c`):

```
bool failed = Globals::status->getMission()->hasFailed();            // bare, cmp #0; beq
bool won = getMission()->hasWon() || getCampaignMission()->hasWon(); // both bare
if (won) {
    bool keepObjectives = getCampaignMission()->hasWon()
        ? !getMission()->isCampaignMission() : false;                // NO campaign temp
    Mission *mission = getMission()->hasWon() ? getMission()
        : getCampaignMission();                                      // third getCampaignMission
    ...
}
```
i.e. the original calls `getCampaignMission()` **three** times (won, keepObjectives,
mission fallback); our wave-125 `campaign` temp reuses one call and saves 4 B. The
`menuWindow->OnTouchEnd(x,y,touchId) == true` is also bare in the original
(`cmp r0,#0; beq` @ `0x17aecc`; `MenuTouchWindow::OnTouchEnd` returns `int`, so `== true`
emits `cmp #1; bne`).

Measured sizes (all `base` = 8080):

| variant | size | pct | `.text` | `.rel.plt` |
|---|---:|---:|---:|---:|
| base | 8080 | 81.9 | 85,441 | 0 |
| failed bare | 8088 | 73.9 | 210,105* | 6 |
| won bare | 8064 | 77.0 | 164,611* | 0 |
| no campaign temp | 8084 | 80.9 | 210,089* | 0 |
| failed+won+no-temp | 8072 | 74.2 | 163,279* | 6 |
| failed+won+no-temp+menu | 8068 | 73.6 | 163,204* | 6 |
| **separate `if (hudTouchFlags==0)` + failed bare** | **8080** | **77.6** | **85,373** | **6** |

`*` = body-size change shifts every later `.text` symbol, so the raw diff is dominated by the
shift, not the function.

The exact-content reconstruction is **8068 B (-12)**. The missing 12 B is not the mining
guard (same size) and not `failed`/`won`/`menu` (same instruction sizes); it must be an
unidentified extra instruction/call in the campaign chain or a branch-encoding difference.

### 4. `failed` bare is the lever that permutes `.rel.plt`

`failed` bare alone (8088) flips the first-reference order of `Hud::resetAnalogStick` and
`MenuTouchWindow::inCinematicMode` inside OnTouchEnd:

```
orig: inCinematicMode @0x17b816, setCinematicMode @0x17b820, resetAnalogStick @0x17b8ec
base: inCinematicMode @0x17b704, setCinematicMode @0x17b70e, resetAnalogStick @0x17b81a  (OK)
failed: resetAnalogStick @0x17b66a, inCinematicMode @0x17b75e, ...                     (flipped)
```

So `.rel.plt` becomes a 3-entry permutation (6 differing bytes). The original has the
`failed` fix *and* `.rel.plt` exact, so its block placement must keep the `cutsceneActive`
tail (`resetAnalogStick`) after the `menuTouchOpen` block. Our `failed` fix moves the tail
before it.

### 5. The best size-8080 `.text` improvement found

`} else if (this->hudTouchFlags == 0) {` -> `}\n if (this->hudTouchFlags == 0) {`
(separate `if`, matching the original CFG where the test at `0x17afe2` is reached from the
chain end) **plus** `failed` bare:

* size 8080/8080, fuzzy 77.6 %
* `.text` **85,373** (-68 B, the best measured this wave)
* `.ARM.extab` 1,432 (+5 B), `.rel.plt` 6 (+6 B)
* full `check` green: `byte 4506 linked 4506`, data 843/843, imports 207, lint CLEAN, drift 0

It was not landed because the task requires `.rel.plt` exact and `.ARM.extab` is a monitored
section. It is the strongest candidate for a follow-up that re-places the `cutsceneActive`
tail after `menuTouchOpen` (which would restore `.rel.plt`).

## Variants measured and rejected (all reverted)

| variant | size | pct | note |
|---|---:|---:|---|
| base | 8080 | 81.9 | `.text` 85,441, `.rel.plt` 0 |
| `amount != 0` / `!!amount` / `(unsigned)` / `!=0U` / `count!=0` / `amount<0` | 8064/8084 | 74.3/72.0 | backend canonicalises to `add r8,r8,#1` |
| `amount != 0 && count > 0` | 8080 | 81.9 | frontend folds to `amount < 0` (base) |
| `amount != 0 && amount < 0` | 8080 | 81.9 | same |
| `amount & 0x7fffffff` | 7948 | 48.6 | |
| `hasFailed() != 0` / `!= false` / `> 0` | 8088 | 73.9 | same as bare |
| `hasFailed() == 1` / `unsigned char == 1` | 8080 | 81.9 | same as base |
| `campaign` temp removed | 8084 | 80.9 | |
| `won` second `hasWon()` bare | 8064 | 77.0 | |
| `menuWindow->OnTouchEnd(...)` bare | 8080 | 80.5 | `.text` 85,460 |
| `cutsceneActive != 0` / `hudMenuOpen != 0` / `== true` | 8080 | 81.9 | canonicalised |
| `cutsceneActive == true` | 8076 | 75.8 | |
| separate `if (hudTouchFlags==0)` | 8076 | 79.1 | `.rel.plt` 0 |
| separate `if` + `campaign` temp | 8080 | 79.3 | `.text` 85,775 |
| separate `if` + failed bare | 8080 | 77.6 | `.text` 85,373, `.rel.plt` 6 |
| separate `if` + failed + menu bare | 8076 | 75.7 | `.rel.plt` 0 |
| separate `if` + failed + menu + temp | 8080 | 76.9 | `.text` 85,477, `.rel.plt` 0 |

## Next attack

1. Find the missing 12 B of the exact-content reconstruction (8068 -> 8080). The campaign
   chain / `finishMission` region is the only place that can hold it; compare the original
   0x17b0b6..0x17b8e2 against the 8068 B variant block-by-block.
2. Re-place the `cutsceneActive` tail (`frameTime=0; resetAnalogStick; releaseAllKeys;
   return;`) after the `menuTouchOpen` block so the `failed` fix keeps `.rel.plt` exact;
   then land the separate-`if` + `failed` shape (`.text` 85,373).
3. Keep using `/home/debian/llvm7build/build/bin/llc -print-before=branch-folder` for block
   numbering; `-print-after=block-placement` aborts on this function in `printAsOperand`
   (stale `%ir.*` use-list), so read the final order from the disassembly.
