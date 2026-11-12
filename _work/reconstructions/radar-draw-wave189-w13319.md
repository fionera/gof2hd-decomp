# Radar::draw wave 189 (w13319) — independent register-allocation / branch-layout pass: no size-exact raw win

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left at
the base. This pass commits only this note. Worktree `wt/w13319`, branch `wave189-w13319`,
base `master` `beef26e16`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13308.md` (the campaign-in-`sl` family on the same 6,309
base), `radar-draw-wave188-w13209.md` (108-cell planet-lock cross product),
`radar-draw-wave187-w13110.md` (the `%122`/`%2481` greedy-allocator eviction trace) and
`radar-draw-wave187-w13111.md` (expect/alias re-sweep). This note is self-contained: every
number below was measured in this worktree on the 6,309 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,309** |
| instruction-index alignment | 1,858 |
| instruction count | 3,541 ours / 3,532 orig |
| frame | `sub sp,#296` (`0x128`) |
| `mode` argument slot | `sp+0x64` (matches original) |
| `campaign` | spilled `sp+0x90` |
| shared zero | `sl` (`12e07a mov.w sl,#0`) |
| planet-loop counter | `sl` |
| planet-loop `stations` | spilled `sp+0x84` |
| `.text` differing bytes (whole `.so`) | 74,032 |
| `.ARM.extab` differing bytes | 639 (size/base exact at `0x1a3b78`, 156,312 B) |
| `.rel.plt` | byte-identical |
| build-id | 20 |
| total differing bytes | 74,691 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. Register map: original vs the 6,309 base

Both bodies are structurally the *same instruction stream* modulo register names and stack
slots; the raw metric is dominated by the `sl`/`fp` swap.

| role | original | base |
|---|---|---|
| `this` | `r4` | `r4` |
| `Globals::status` | `r5` | `r5` |
| campaign mission / freelance | `r6` | `r6` |
| `stationId` | `r8` | `r8` |
| `currentStation` | `r9` | `r9` |
| shared zero (early) | **`fp`** (`12e07a mov.w fp,#0`) | **`sl`** (`12e07a mov.w sl,#0`) |
| `campaign` | **`sl`** (`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) | **spilled `sp+0x90`** |
| planet-loop counter `i` | **`fp`** (`12e3c8 mov.w fp,#0`, `12e3da cmp fp,r1`) | **`sl`** (`12e3c8 mov.w sl,#0`, `12e3d6 cmp sl,r1`) |
| planet-loop `stations` | **`sl`** (`12e3c6 mov sl,r0`, `12e3fa ldr.w r0,[sl,#4]`) | **spilled `sp+0x84`** |
| `marked` | spilled `sp+0x84` | **`fp`** |
| `mode` | `sp+0x64` | `sp+0x64` |

The original's planet loop reloads `stations` from `sl` on every use (`12e4be`, `12e62c`,
`12e648`, `12e668`, `12e682`, `12e6ac`) and keeps only a cold copy at `sp+0x74`; the base
reloads it from `sp+0x84`. The original also keeps `stationId` in `r8` across the whole
iteration and uses `sl` only as a short-lived temp for the campaign target comparison
(`12e474 ldr [sl,#4]; 12e478 ldr.w sl,[r0,fp,lsl#2]; 12e48c cmp sl,r0; 12e48e ldr.w
sl,[sp,#0x74]`).

## 2. MIR / stack-slot evidence

`-mllvm -print-after-all` dumps (scratch `allpass.txt`, `allpass_bsor.txt`, ~1.95M lines,
must be redirected to disk) locate the Greedy RA dump at ~line 1,567,140 (base) and the PEI
stack-slot map at ~line 1,620,668.

Base PEI map: `stack.44 = 0x64` (`mode`), `stack.43 = 0x68`, `stack.48 = 0x54` (`hud`).
The `bool otherLock` + `int marked` + `bossSeen ||` body (`bsor`) PEI map:
`stack.40 = 0x74` (cold `stations` copy), `stack.42 = 0x6c` (`mode`), `stack.43 = 0x68`
(hot `stations`), `stack.44 = 0x64` (`stationId`), `stack.48 = 0x54` (`hud`). The extra
`stationId` spill at `0x64` is what pushes `mode` to `0x6c` in every campaign-in-`sl` body.

`-mllvm -print-after=greedy` / `-print-after=stack-slot-coloring` do **not** work in clang
7.0.2; only `-print-after-all` does.

## 3. The campaign-in-`sl` family (re-measured)

`bsor` = `bool otherLock` + `int marked = 0` + `bossSeen = bossSeen || enemy->shipGroup == 10`.

| cell | raw | size | notes |
|---|---:|---:|---|
| base | **6,309** | 9,908 | mode `0x64`, campaign spill `0x90`, zero `sl` |
| `bool otherLock` | 9,247 | **9,856** | campaign `sl`, zero `fp`, frame `0x130` |
| `int marked` | 8,424 | **9,912** | mode `0x64`, but 4 B over |
| `bossSeen \|\|` | 9,208 | **9,976** | |
| `bool otherLock` + `int marked` | 9,229 | **9,860** | |
| `bool otherLock` + `bossSeen \|\|` | 8,819 | **9,896** | campaign `sl`, zero `fp` |
| `int marked` + `bossSeen \|\|` | 9,206 | **9,892** | |
| `bool otherLock` + `int marked` + `bossSeen \|\|` (bsor) | **7,080** | **9,908** | only size-exact campaign-in-`sl` body; mode `0x6c` |
| `bool otherLock` + `bool marked` + `bossSeen \|\|` | 8,819 | **9,896** | |
| `int otherLock` + `int marked` + `if (x==10) bossSeen=true` | 9,184 | **9,892** | |

bsor matches the original's planet-loop allocation (`fp` counter, `sl` `stations`, `marked`
at `sp+0x84`) but is 771 B raw-worse: its `bool otherLock` introduces an extra
`mov r1,r8; str r8,[sp,#0x90]` (alienOrbit spill) at `12e194`, shifting the whole tail by
6 B, and its `stationId` spill at `0x64` shifts `mode` to `0x6c`.

## 4. Cells measured this pass (all reverted)

Every row is a full incremental `link_so -j2` + `tools/fn_diff.py` with a
`str r3,[sp,#N]` probe of the `mode` slot (`ulimit -v 12000000`; scratch
`/home/debian/GalaxyOnFire2/scratch/w13319/`).

### 4.1 `stationId` lifetime / aliasing (the assigned lever)

| edit | raw | size | mode |
|---|---:|---:|---|
| base | **6,309** | 9,908 | `0x64` |
| remove `int stationId`, use `stations->data()[i]` at every use | 9,366 | **9,984** | `0x6c` |
| same on the bsor body | 9,046 | **9,928** | `0x6c` |
| `stationId` declared after `currentStation`/`warpGate` | 8,980 | **9,900** | `0x6c` |
| `textOffset` declared first | 6,309 | 9,908 | `0x64` |
| `(*stations)[i]` instead of `stations->data()[i]` | 6,309 | 9,908 | `0x64` |
| `const int stationId` / `const int currentStation` / `const int warpGate` | 6,309 | 9,908 | `0x64` |
| `stations` as `const` / `__restrict` / `auto` | 6,309 | 9,908 | `0x64` |
| `stationData` alias for `stations->data()` | 9,869 | **10,220** | — |

### 4.2 `campaign` type / lifetime

| edit | raw | size | mode |
|---|---:|---:|---|
| base | **6,309** | 9,908 | `0x64` |
| `unsigned int` / `uint32_t` / `long` / `int32_t` | 6,310 / 6,310 / 6,309 / 6,309 | 9,908 | `0x64` |
| `short` / `unsigned char` | 8,441 / 8,380 | **9,912** | `0x64` / `0x68` |
| `volatile int` / `const volatile int` | 9,227 | **9,888** | `0x68` |
| remove the local, call `getCurrentCampaignMission()` at the guard | 9,216 | **9,904** | `0x64` |

### 4.3 `__builtin_expect` / branch layout on the campaign block

| edit | raw | size | mode |
|---|---:|---:|---|
| base (`expect(...,0)`) | **6,309** | 9,908 | `0x64` |
| `expect(...,1)` | 9,283 | **9,956** | `0x6c` |
| plain `if` (no expect) | 9,268 | **9,972** | `0x64` |
| plain `if` + bsor knobs | 7,095 | **9,908** | `0x64` |
| `expect(...,1)` + bsor knobs | 9,342 | **9,964** | — |

The plain `if` does put the campaign body inline (matching the original's `12e474`
placement) but re-colours the whole loop (`fp`=`stations`, `sl`=`i`, `r5`=`stationId`,
`r8`=`status`) and grows the function by 64 B.

### 4.4 Prologue zero materialisation

`this->plasmaInRange = 0; this->lockConfirmed = 0;` rewritten as `= false`, or through a
`char`/`bool`/`int`/`unsigned char` zero local, is byte-identical to the base (6,309 /
9,908 / zero `sl`) — the zero is CSE'd and the `sl` tie-break is unchanged.

### 4.5 Other single-knob cells (base body)

All size-exact and raw-worse: `alwaysEnemySeen`/`alwaysEnemy` as `int` (8,997 / 8,582),
`bossSeen` as `int` (9,136), `notOtherLock`/`notTurretMode` as `bool` (9,513 / 9,547),
`otherLockFlag` as `int`/`uint8_t` (9,442), `planetHit`/`landmarkHit`/`enemyHit` as `int`
(9,236 / 8,710 / 8,616), `bool+mark` (9,229), `bool+bsor` (8,819), `bool+mark+bsor`
(7,080). `marked` as `char`/`uchar`/`short`/`ushort`/`long`/`uint`/`int8..uint16` on the
bsor body is byte-identical to 7,080; `bool`/`float`/`double`/`long long` are size-fatal.

## 5. Conclusion

The 6,309 base is a strong local optimum: its prologue (`mode` at `sp+0x64`, frame
`0x128`) matches the original instruction for instruction, and the residual is the global
`sl`/`fp` colouring (`campaign`/zero/`i`/`stations`/`marked`). Every source construct that
flips `campaign` into `sl` either changes the function size or introduces an extra early
spill (`alienOrbit` at `12e194`, `stationId` at `0x64`) that shifts the entire tail. No
size-exact, `.rel.plt`-identical raw win was found in the assigned register-allocation /
branch-layout cells; `src/` is left at the base.

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6309 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13319/`: `Radar.base.cpp` (6,309 base),
`Radar.bsor.cpp` (7,080 body), `Radar.markoutside.cpp`, `allpass.txt` / `allpass_bsor.txt`
(`-print-after-all` dumps), `orig.dis`/`ours.dis`/`bsor.dis`, `heat.py` (same-address equal
run map), `align.py`/`structdiff.py` (instruction alignment / structural blocks),
`try.py`..`try25.py`, `sweep.py`, `sweep2.py`, `sweep3.py`.
