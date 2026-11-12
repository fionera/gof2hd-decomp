# Radar::draw wave 186 (w13010) — campaign `sl` vs `sp+0x90` register-allocation pass (llc-trace driven)

**Status:** analysis complete, 2026-11-12; **no size-exact raw win found**, `src/`
left at the base. Worktree `wt/w13010`, branch `wave186-w13010`, base `master`
`2d02567f6`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave185-w12911.md` (expect/alias re-sweep on the 6,311
body), `radar-draw-wave183-w12710.md` (campaign `sl` analysis, declaration
sweep) and `radar-draw-wave173-w11808.md` (first weight accounting + the
assertions-`llc` recipe). This note is self-contained: every number below was
measured in this worktree on the 6,311 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,311** |
| instruction-index alignment | 1,903 |
| instruction count | 3,544 ours / 3,532 orig |
| `.text` differing bytes | 74,402 |
| `.ARM.extab` differing bytes | 647 (size/base exact at `0x1a3b78`, 156,312 B) |
| `.rel.plt` / `.rodata` / `.data` / `.bss` | 0 / 0 / 0 / 0 |
| `.note.gnu.build-id` differing bytes | 20 |
| total differing bytes | 75,069 |
| file size | 2,187,328 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this
pass; only this note.

## 1. Prologue map (original vs ours)

The first `0x140` bytes are byte-identical except for the register names and
the spill-slot rotation:

```
                        ORIGINAL                              OURS
12e07a  mov.w  fp, #0                 (zero/missionBlocksPlanets)  mov.w  sl, #0
12e086  strb.w fp, [r4,#536]          lockConfirmed = 0            strb.w sl, [r4,#536]
12e08c  strb.w fp, [r4,#304]          plasmaInRange = 0            strb.w sl, [r4,#304]
12e0d8  mov.w  fp, #0                 missionBlocksPlanets = 0     mov.w  sl, #0
12e0e8  str    r0, [sp,#148] (0x94)   turretMode                   str    r0, [sp,#152] (0x98)
12e0f8  mov    sl, r0                 campaign                     str    r0, [sp,#144] (0x90)
12e38a  cmp.w  sl, #2                 campaign >= 2                ldr    r0,[sp,#144]
12e38e  str    r0, [sp,#144] (0x90)   otherLock                    cmp    r0, #2
                                                                   str    r0,[sp,#148] (0x94)
12e3ba  str.w  fp, [sp,#112] (0x70)   spill missionBlocksPlanets   str.w  sl, [sp,#116] (0x74)
12e3c6  mov    sl, r0                 stations -> sl               str    r0, [sp,#132] (0x84)
12e3c8  mov.w  fp, #0                 planet loop index -> fp      mov.w  sl, #0
```

So the residual is exactly one register swap: the original keeps `campaign` in
`sl` (r10) and the shared zero/missionBlocksPlanets in `fp` (r11); ours keeps
the zero in `sl`, spills `campaign` to `sp+0x90`, and every later stack slot is
rotated by one 4-byte slot (`otherLock` 0x90->0x94, `turretMode` 0x94->0x98,
missionBlocksPlanets spill 0x70->0x74, `stations` `sl`->`sp+0x84`, loop index
`fp`->`sl`).

Everything before 12e0f8 is structurally identical (only branch targets differ,
which follow from the block layout), so a correct flip of the allocation would
collapse most of the 6,311 raw bytes.

## 2. The actual greedy trace (assertions `llc`)

Built Android LLVM 7.0.2 `llc` from
`android.googlesource.com/toolchain/llvm @ 1d739ffb0366421d383e04ff80ec2ee591315116`
(the exact commit the NDK r18b clang is based on), `-DLLVM_ENABLE_ASSERTIONS=ON
-DLLVM_TARGETS_TO_BUILD=ARM`, and confirmed it reproduces the NDK clang codegen
byte-for-byte for this function (prologue identical; only the same 3,544 insns).

Recipe (see `scratch/w13010/`):

```sh
# 1. .ll from the current Radar.cpp (drop the codegen -mllvm flags, add -g0)
clang++ <project flags> -g0 -S -emit-llvm -o radar.ll src/game/weapons/Radar.cpp
# 2. codegen + regalloc trace
llc -mtriple=thumbv7-none-linux-android16 -O2 -relocation-model=pic \
    -disable-postra-machine-licm -mattr=+cheap-predicable-cpsr \
    -debug-only=regalloc -o /dev/null radar.ll > regalloc_base.log
```

Trace for `Radar::draw` (vreg numbers match the NDK `-print-machineinstrs`
dump; `campaign` = `%417`, zero = `%2481`):

* `campaign %417` [2128r,8080B)[8256B,17248B)[18496B,19328B)[19504B,24448r),
  weight 5.95e-05, has a physical copy hint `$r0` -> **enqueued first** (the
  `1<<30` `hasKnownPreference` boost), `assigning %417 to $r11` (fp).
* `%411` (a long `%r0`-hinted vreg, weight 1.84e-04) evicts `%417` from `$r11`.
* `%417` -> `$r6`, evicted again -> `$r10` (sl).
* The zero `%2481` [240r..36608r), weight 7.63e-05, **no** copy hint (all phi
  copies were coalesced away), so it is dequeued in the non-boosted group.
  It first takes `$r11` (evicting `%38`, weight 5.47e-05).
* `%122` (`otherLock`, interval [23952r,57232r), weight **9.07e-05**) evicts
  `%2481` from `$r11` (Cascade 27).
* `%2481` re-dequeues, evicts `%417` from `$r10` (Cascade 27) and takes `sl`.
* `%417` re-dequeues, no callee-saved register is free, and is spilled
  (`[%417 -> fi#45]`; the four spill reloads in bb.121/123/125/126 are the
  `ldr r0,[sp,#144]` before the `cmp #2`).

So the whole residual reduces to **`%122` (`otherLock`) evicting the zero from
`fp`**. If the zero stayed in `fp`, `campaign` would stay in `sl` and the
original assignment would be reproduced. `%122`'s weight (9.07e-05) is only
~19% above the zero's (7.63e-05), and the zero's weight is dominated by the
three bb.1 contributions (`MOVi 0` + the two `STRBi12`, freq 0.625 each =
1.875 of its 2.6 total weight).

## 3. What was tried this session (all reverted; size must stay 9,908)

Every variant was built with `link_so -j2` under `ulimit -v 12000000` and
measured with `tools/fn_diff.py`; the campaign register was read from the
instruction immediately after the first `getCurrentCampaignMission` `blx`.
`camp=sl`/`fp`/`spill` is the post-RA assignment.

### 3.1 `otherLock` / `otherLockFlag` / `not*` cells

| edit | raw | size | camp |
|---|---:|---:|---|
| `bool otherLock` | 9,241 | **9,856** | sl |
| `bool notTurretMode` | 9,532 | **9,868** | sl |
| `bool notOtherLock` | 9,511 | **9,876** | sl |
| `bool notTurretMode` + `bool notOtherLock` | 9,511 | 9,876 | sl |
| `bool otherLock` + both nots | 9,050 | 9,876 | sl |
| `int otherLockFlag` (and the 3-assign form) | 9,448 | **9,896** | sl |
| `int turretMode` | 9,232 | **9,856** | sl |
| `landmarkHit` int | 8,720 | **9,940** | spill |
| `otherLock = __builtin_expect(OR, 0)` | 8,818 | 9,908 | **fp** |
| `otherLock = __builtin_expect(OR, 1)` | 6,311 | 9,908 | spill |
| `otherLockFlag = __builtin_expect(field_0x14 != 0, 0/1)` | 6,311 | 9,908 | spill |
| `otherLock` unsigned / long / int32 / `(flag != 0)` / `=0` then assign | 6,311 | 9,908 | spill |
| `notOtherLock = otherLock == 0` / `1 - otherLock` | 6,311 | 9,908 | spill |
| `notTurretMode = turretMode == 0` / `1 - turretMode` | 6,311 | 9,908 | spill |

Per-operand `__builtin_expect` on the 4-term `otherLock` OR and the 3-term
`otherLockFlag` AND (8 + 6 cells): best neutral 6,311 (`or3` both weights);
the rest 6,345-8,829 and/or size-fatal.

### 3.2 `missionBlocksPlanets` / zero cells

| edit | raw | size | camp |
|---|---:|---:|---|
| `unsigned` / `char` / `unsigned char` / `0u` / `0L` / `0x0` / `(char)0` / `!true` / `1-1` / `mode*0` / `(int)0u` | 6,311 | 9,908 | spill |
| `!(getType() == 172)` / `172 != getType()` / `(getType()-172) != 0` | 6,311 | 9,908 | spill |
| declaration after `alwaysEnemy*` / before `Mission *mission` / `Mission` after it | 6,311 | 9,908 | spill |
| `__builtin_expect(getType() != 172, 1)` | 6,311 | 9,908 | spill |
| `__builtin_expect(getType() != 11, 0)` on the mission `if` | 9,534 | **9,920** | spill |

### 3.3 Campaign cells

| edit | raw | size | camp |
|---|---:|---:|---|
| `int32_t` / `signed` / `const` / `register` / `(int)` / `+` / `(long)` cast | 6,311 | 9,908 | spill |
| `campaign > 1` | 6,311 | 9,908 | spill |
| `campaign >= 2u` | 6,312 | 9,908 | spill |
| comma-declared with `alienOrbit` | 6,311 | 9,908 | spill |
| `campaign` after `turretMode` | 6,336 | 9,908 | spill |
| swap `alienOrbit`/`campaign` declarations | 6,318 | 9,908 | spill |
| `volatile int campaign` | 9,230 | **9,888** | spill |
| `__builtin_expect(campaign >= 2, 0)` / `(campaign > 1, 0)` | 9,129 | **9,960** | spill |
| `__builtin_expect(campaign >= 2, 1)` / `(campaign > 1, 1)` | 8,771 | **9,920** | spill |

### 3.4 Enabled-check and combinations

| edit | raw | size | camp |
|---|---:|---:|---|
| `__builtin_expect(!enabled, 0)` | 6,348 | 9,908 | spill |
| `__builtin_expect(!enabled, 1)` | 10,192 | **10,448** | sl |
| `otherLock exp0` + `enabled exp0` | 9,385 | **9,996** | fp |
| `otherLock exp0` + `otherLockFlag exp0` | 8,818 | 9,908 | fp |
| `otherLock exp0` + `campaign>=2 exp1` | 8,748 | **9,920** | spill |

### 3.5 Why the weight lever does not compose

`__builtin_expect(!enabled, 0)` raises the zero's weight from 7.63e-05 to
**1.22e-04** (above `%122`'s 9.07e-05), so it is no longer evicted from `fp`
by `%122` — but the greedy allocator then chooses `RS_Split` for the zero
instead of a register (`Best split candidate ... may not cause bad eviction
chain`), and the size changes. The base's zero takes `RS_Assign` and waits for
the second round; the weight bump changes that decision. There is no
weight in between that both beats `%122` and keeps `RS_Assign`, on this body.

## 4. Conclusion

The campaign residual is a chaotic eviction dance: `campaign` is boosted by its
`$r0` copy hint and is assigned first, but every callee-saved register it
reaches is stolen by a higher-weight vreg, and the final thief is the shared
zero `%2481` after `%122` (`otherLock`) pushes the zero off `fp`. Every source
shape tried that moves `campaign` into `sl` either changes the function size
(9,856-9,996) or the raw diff (8,700-9,500); the only size-exact
campaign-in-`fp` shape (`otherLock = __builtin_expect(OR, 0)`) is raw 8,818
because it changes the `otherLock` OR branch layout. No size-exact raw win is
available from the register-allocation lever alone on this base.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6311 aligned= 1903 size=9908/9908 insn=3544/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13010/`:
`Radar.base.cpp` (6,311 base), `harness2.py` (build+measure+campaign_reg),
`batch11.py`-`batch17.py`, `sweep_big.py`, `sweep_or.py`, `sweep_camp.py`,
`sweep_combo.py`, `sweep_ol.py`, `sweep_misc.py` (the cells above),
`hints.py` / `qv.py` (interval size/weight/copy-hint extraction from a
`-print-machineinstrs` dump), `mnem.py` / `sbs.py` / `heat.py` (diff aids),
`mi_base.txt` / `mi_c3.txt` (33 MB `-print-machineinstrs` dumps), and
`llvm702/` (the assertions-enabled Android LLVM 7.0.2 `llc` build;
`llvm702/build/bin/llc`). `regalloc_base.log` is the full greedy trace.
