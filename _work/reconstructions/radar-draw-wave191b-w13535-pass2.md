# Radar::draw wave 191b (w13535, pass 2) — shift-profile lens: no size-exact raw win

**Status:** analysis complete, 2026-11-12. **No new `src/` change**; `src/` is left at the
landed w13535 base (HEAD `16c311d56`, raw 6,015). This pass commits only this note.
Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191b-w13535.md` (the landed `isDying`/`isDead` swap),
`-w13520.md` (gas/planet chains, music mask), `-w13510.md` (station-blip / 24000 /
tractor cells) and `-w13507.md` (operand/statement sweeps).

## 0. Baseline (verified in this worktree)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,015** |
| instruction-index alignment | 1,941 |
| instruction count | 3,541 ours / 3,532 orig |
| `.rel.plt` | byte-identical |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

## 1. The decisive new measurement: the address-shift profile

`raw` is a *same-address* byte diff, so it is dominated by address shifts, not by
instruction mismatches.  A `difflib` sequence alignment of the two instruction streams
(scratch `shift.py`) gives the exact `ours_addr - orig_addr` delta for every matched
block.  This is the map a future pass should work from:

| orig range | delta (B) | note |
|---|---:|---|
| `12e050`–`12e3d0` | **0** | prologue + landmark/station blip; only the `fp`/`sl` zero and `campaign` spill bytes differ |
| `12e3d4`–`12e4ac` | −4 … −12 | `stations` cold-copy `str.w sl,[sp,#116]` missing in ours |
| `12e4ae`–`12e6c6` | −46 … −110 | planet-loop campaign-mission block (orig true-block inline at `12e474`, ours out-of-line at `12e5ca`) |
| `12e6c6`–`12eb5a` | −110 … −8 | planet-loop tail |
| `12eb5a`–`12ec10` | −8 | |
| `12ec10`–`12ef24` | −56 | enemy precompute |
| `12ef24`–`12efcc` | −6 … 0 | |
| `12efcc`–`12f088` | +18 … +12 | gas spark block |
| `12f088`–`12f11e` | **0** | |
| `12f11e`–`12f194` | +36 … +28 | gas lock |
| `12f194`–`12f21a` | 0 … −2 | |
| `12f21a`–`12f26c` | −50 | gas lock tail |
| `12f26c`–`12f352` | +122 | gas lock frame math |
| `12f352`–`12f3a6` | −34 | |
| `12f3a6`–`12f456` | −4 … 0 | |
| `12f456`–`12f488` | **0** | |
| `12f488`–`12f618` | +12 | spark loop |
| `12f618`–`12f6b2` | +6 … +16 | |
| `12f6b2`–`12f6d4` | −112 | gas 24000 chain |
| `12f6d4`–`12f6ee` | 0 … +10 | `calcDistance` arg setup |
| `12f6ee`–`12f816` | **0** | |
| `12f816`–`12fc54` | **0** | enemy loop (1,100 B, fully aligned) |
| `12fc54`–`12fc84` | +4 | wreck x/y chain |
| `12fc84`–`12fcb8` | −22 … −12 | |
| `12fcb8`–`12fcd2` | +6 | |
| `12fcd2`–`12fd74` | −10 … −4 | asteroid distance loop |
| `12fd74`–`12fd8c` | −6 | |
| `12fd8c`–`12fe38` | **0** | asteroid lock (166 B aligned) |
| `12fe38`–`12fe6c` | +2 … 0 | |
| `12fe6c`–`12feac` | −12 | `lockedEnemy` block |
| `12feac`–`12fec4` | **0** | |
| `12fec4`–`12ff30` | +2 … 0 | enemy-lock limit |
| `12ff30`–`12ff42` | +4 | |
| `12ff42`–`12ffca` | **0** | |
| `12ffca`–`13004a` | −126 | `hasScanner` block (out-of-line in both, different distance) |
| `13004a`–`130086` | −160 … −128 | |
| `130086`–`130290` | **0** | music block (≈520 B aligned) |
| `130290`–`1302be` | +20 … +4 | music mask |
| `1302be`–`130326` | **0** | |
| `130326`–`1304fe` | −68 … −154 | music tail |
| `1304fe`–`130592` | −70 | |
| `130592`–`13062e` | −182 … −224 | |
| `13062e`–`1306ae` | **0** | |
| `1306ae`–`130700` | −6 … 0 | EH cleanup |

So roughly **4,300 of the 9,908 bytes are already at delta 0**; the 6,015 raw diff is
essentially the sum of the shifted windows above.  The single largest lever remains the
`campaign`-in-`sl` allocation (which is what makes the `12e3d4`–`12ef24` window shift at
all); every local cell that changes the shift makes `raw` worse even when it makes the
instruction stream match better (§2).

## 2. Cells measured this pass (all reverted; base 6,015 / 1,941 / 9,908)

### 2.1 Gas-cloud `isDead`/`isDying` order (real source difference, not landable)

`Radar.cpp:991` is `if (cloud->isDying() || cloud->isDead())`.  The original is
**isDead-first**:

```
orig 12f5ee mov r0,sl ; 12f5f0 blx isDead  ; 12f5f4 cmp ; 12f5f6 bne
     12f5fa mov r0,sl ; 12f5fc blx isDying ; 12f600 cmp ; 12f602 bne
ours 12f5fa mov r0,sl ; 12f5fc blx isDying ; 12f600 cmp ; 12f602 bne
     12f606 mov r0,sl ; 12f608 blx isDead  ; 12f60c cmp ; 12f60e bne
```

Swapping to `isDead() || isDying()` is size-exact but **6,016** (+1): the block sits at
delta +12, so the two calls land on the wrong addresses anyway.  (The `enemies:` guard at
`:874`, the `wreck` at `:693`, the `candidateWreck` at `:1205` and the `lockedEnemy` at
`:675` are already in the original order.)

### 2.2 Asteroid lock-timer placement (real source difference, size-fatal or worse)

The original writes `asteroidLockTimer = 0` in the `best > -1` block (`12fd88`
`itt ne / movne r1,#0 / strne r1,[r4,#412]`) and `asteroidLockTimer += mode` at the top of
the `else` branch (`12fe40 add r0,r2`).  Ours does `+= mode` in the `best` block and
nothing in the `else`.  Fixing the semantics:

| cell | raw | size |
|---|---:|---:|
| base | **6,015** | 9,908 |
| `best = 0` + `else += mode` after `lockedAsteroid=nullptr`, `hasScanner` hoisted (`astA`) | 6,190 | 9,908 |
| same, `hasScanner` at its use (`astE`) | 6,897 | **9,904** |
| same, keep `lockedAsteroid=nullptr` + `hasScanner` at use (`astF`) | 6,288 | 9,908 |
| `hasScanner` moved to its use only (`hs_move`) | 6,131 | 9,908 |

The `best` block shrinks 14→8 B, which re-shifts the whole asteroid tail; no placement of
the `+= mode` recovers it.

### 2.3 Gas spark box `+` form (structure matches, raw worse)

`Radar.cpp:967` is `left > screenX - size*2` / `top > screenY - size*2`; the original
computes `left + size*2 > screenX` (`12f4de add.w r2,r2,r0,lsl#1`).  Rewriting to the
`+` form reproduces the original instruction sequence but re-rolls the gas-loop
allocation:

| cell | raw | aligned |
|---|---:|---:|
| base (`-` form) | **6,015** | 1,941 |
| `screenX < left + size*2` / `screenY < top + size*2` (`spark_lt`) | 6,019 | 1,937 |
| same + `int top` (`spark_itop_lt`) | 6,031 | **1,916** (best aligned found) |
| `int top`, `-` form (`spark_itop`) | 6,031 | 1,919 |
| `left + size*2 > screenX` (`spark_plus`) | 8,284 | 2,144 / size 9,904 |
| x half only (`spark_ltx`) | 6,016 | 1,937 |
| y half only (`spark_lty`) | 6,018 | 1,941 |
| `top` declared before `left` (`spark_tf`) | 6,015 | 1,941 |

Note the **gas-cloud box** (`Radar.cpp:1017`, `[r4,#292]`) is already byte-identical and
already uses the `+` form (`12f7d4 add.w r2,r2,r0,lsl#1`); only the spark box differs.

### 2.4 Campaign-mission `__builtin_expect` placement (all neutral)

| cell | raw |
|---|---:|
| base `expect(whole cond, 0)` | **6,015** |
| `expect(..., 1)` | 9,303 / **9,964** |
| no expect | 9,283 / **9,956** |
| expect on `isVisible && range` | 6,015 |
| expect on range only | 6,015 |
| expect on `!isEmpty()` only | 9,143 / **9,940** |
| expect on `isVisible` only | 9,210 / **9,936** |
| hoisted `bool campaignMark` + `expect(campaignMark,0)` | 6,015 |
| `expect(...) != 0` | 6,015 |

### 2.5 Enemy-lock limit ternary (neutral)

`int lockLimit; if (navWreck || dockNavPtr->field_0x24) ... else ...` and
`int lockLimit = cond ? ... : ...` are both exactly 6,015 (identical codegen).
`expect(cond, 0)` on the ternary is 6,656 / **9,912**.

### 2.6 `campaign` spellings (neutral, re-confirmed)

`const int campaign`, `campaignOk = campaign >= 2`, `campaign2 = campaign`: all 6,015
(identical codegen).

## 3. Why the local wins cannot be landed

Every real source difference above is *inside* a window whose address delta is non-zero
(§1), and every one of them is size-neutral or size-changing.  A size-neutral swap inside a
shifted window still misses the target addresses; a size-changing fix moves the whole tail
and costs far more than it saves.  The only lever that would collapse the 6,015 is the
`campaign`-in-`sl` / `stations`-in-`sl` colouring (plus the 4-byte `str.w sl,[sp,#116]`
cold copy and the 4-byte `str r0,[sp,#132]` spill it implies).  All spellings tried across
waves 162–191b leave the allocator's choice unchanged; the only `campaign`-in-`sl` body
known (`bsor`, w13319) is 771 B raw-worse.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6015 aligned= 1941 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS (src unchanged)
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13535/`: `try.py` (1 s compile+link+fn_diff
harness), `v60.json`–`v75.json` (this pass's cell definitions), `o_orig.txt` /
`o_ours.txt` (current disassembly), `shift.py` (the delta map of §1).
