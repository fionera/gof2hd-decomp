# MGame::OnTouchEnd — wave 191b (w13540): faithful original chain reconstruction (note only, no raw win)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13540`, branch
`wave191b-w13540`, base master `23a6ebf8b`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13540`.

Prior notes: `mgame-ontouchend-wave191-w13532.md` (100/126 getter order, chain order
re-sweep), `-w13514.md` (case-0 tail permutations), `-w13497.md` (74/162/15 chain move,
126/162 store rotations), `-w13496.md` (126 store rotation), `-w13421.md` (block-100
getter order), `-w13405.md` (original chain order).

## Result

Base unchanged. `check` stays green: byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, data_exact **843/843**, imports **207**,
lint **CLEAN**, drift **0**, parity **5370/5370**. Current `MGame::OnTouchEnd`
raw same-address diff **2,882**, aligned **1,458**, size **8,080/8,080**,
`.text` diff **71,487**, `.ARM.extab` diff **586**, `.rel.plt` identical.

## What was tested this pass (all reverted; no size-exact raw win)

### 1. Full faithful original chain reconstruction (the main experiment)

Decoded the original's emitted campaign chain with a GOT/relocation-aware
disassembler (`scratch/w13540/decode2.py`). The original's *emitted* block order
is exactly

```
15, 22, 43, 42, 65, 81, 74, 95, 96, 100, 110, 120, 126, 134, 144, 155, 161, 162
```

and every condition is `mission->isCampaignMission() && getCurrentCampaignMission()==N`
(isCampaignMission first), with `(96 || 127)` for the 96 arm.  Rewriting the
source chain to that order and the bodies to the original store order reproduces
the original emitted order **exactly** (verified block-by-block), but:

| metric | base | faithful order+bodies |
|---|---:|---:|
| size | 8,080 | **8,084 (+4, fatal)** |
| raw | 2,882 | 4,261 |
| `.text` diff | 71,487 | 197,187 |
| `.ARM.extab` diff | 586 | 1,292 |
| `.rel.plt` diff | 0 | 0 |

The +4 is a **single extra literal-pool word** at the end of the function
(orig pool ends `0x17c0d2`, ours `0x17c0d6`); the code itself is the same shape.
Faithful order with the *current* bodies is 8,076 (-4, fatal) / raw 3,945.
Because block sizes still differ, raw explodes even with the emitted order
matching; matching emitted order alone is not sufficient.

### 2. Per-block faithful body order on the current base (18 builds)

Replacing one campaign block's body with the original store order, current chain
order kept:

| block | raw | size |
|---|---:|---:|
| 15, 43, 120, 134, 144, 155 | 2,882 (tie) | 8,080 |
| 96 | 2,883 (+1) | 8,080 |
| 162 | 2,883 (+1) | 8,080 |
| 126 | 2,885 (+3) | 8,080 |
| 161 | 2,884 (+2) | 8,080 |
| 81 | 2,887 (+5) | 8,080 |
| 65 | 2,894 (+12) | 8,080 |
| 110 | 2,908 (+26) | 8,080 |
| 95 | 2,922 (+40) | 8,080 |
| 100 | 3,620 | **8,048 (-32, fatal)** |

Combinations 96+162 (2,884), 96+126 / 162+126 (2,886), 96+162+126 (2,887) —
no win.

### 3. Chain order re-sweep on the current base (153 pair swaps)

Only `15<->43` ties at 2,882 (size 8,080); every other pair is worse. Confirms
w13532: the current source order is a strict local minimum on the current bodies.

### 4. Per-block single-statement moves (block 162, 10 statements = 90 moves)

All ties at 2,882 or worse; no win. (w13532/w13497 already swept the
switch/initStream/active/module/departStation singles.)

### 5. Non-adjacent statement swaps (blocks 96/162/126, all pairs)

No win; only ties.

### 6. `case 0:` / `case 1:` cinematic-steer original store order

- `case 0:` original `switch=1, active=0, module(2), initStream=0` -> raw **2,885** (+3), size 8,080.
- `case 1:` original `switch=0, active=0, module(5)` -> size **8,076 (-4, fatal)**, raw 3,912.

### 7. Mining loop condition (semantically faithful form)

`count >= 0` (base) is semantically wrong (always true) but is the byte minimum.
`count > 0` / `amount < 0` are semantically correct, size 8,080, but raw **2,884**
(the original emits `cmp r9,#0; beq`; clang emits `cmp r9,#-1; bgt`).
`amount != 0` / `count != 0` / `if (amount)` flip the induction variable to a
positive counter: size **8,088 (+8, fatal)**, raw 6,637.

### 8. Mining String expression schedule

`paren` / `paren2` 8,072 (fatal), `line` 8,092, `copy` 8,016, named `t` temp
8,080 but raw 2,920. The single expression remains the minimum (22 B schedule
diff is a register-allocation choice: ours hoists `add.w r8,sp,#80` above the
first `operator+`).

### 9. Back-to-station tail faithful order

`switch=0, HP, Shield, Armor, Gamma, active=0, module(5)` -> raw **2,892** (+10),
size 8,080.

## Conclusion

The residual is dominated by campaign-chain block placement: +0xf00..+0x1dc0
accounts for 2,738 of the 2,882 differing bytes. The current source order/bodies
are a genuine local minimum under pair swaps, single-block moves, per-block
statement moves, non-adjacent swaps and the faithful original body orders.
A future pass should only retry the faithful reconstruction after eliminating
the extra literal-pool word (size 8,084 -> 8,080) and re-checking raw.

## Reproduction

Scratch (`/home/debian/GalaxyOnFire2/scratch/w13540/`): `measure.py` (raw/aligned/
size + section diffs), `decode2.py` (GOT-aware block decoder), `rebuild.py`
(faithful order+bodies), `bodyswap2.py` / `movetest.py` / `chainsweep.py`
(sweeps), `strtest.py` (String forms). All builds
`cmake --build cmake-build-match --target link_so -j2` under `ulimit -v 12000000`.
