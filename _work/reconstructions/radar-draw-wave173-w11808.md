# Radar::draw wave 173 (w11808) — campaign/`sl` register-allocation attack

**Status:** complete, 2026-11-11. Worktree `wt/w11808`, branch `wave173-w11808`,
base `master` `3fafa5b03`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave171-w11707.md` (full re-sweep) and
`radar-draw-wave173-w11809.md` (planet-lock OR order + frame expect).

## 0. Baseline (verified)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,054 |
| instruction-index alignment | 1,582 |
| instruction count | 3,540 ours / 3,532 orig |
| `.rel.plt` | byte-identical (0) |
| `.ARM.extab` differing bytes | 711 |
| `.text` differing bytes | 81,503 |

## 1. Landed (one commit, two sites)

| # | site | change | raw | aligned | size |
|---|---|---|---:|---:|---:|
| 1 | planet-lock guard (line 642) | `missionBlocksPlanets \|\| planetLockTimer < 1` -> `planetLockTimer < 1 \|\| missionBlocksPlanets` | 7,054 -> **6,864** | 1,582 -> **1,574** | 9,908 |
| 2 | planet-lock frame test (line 646) | `__builtin_expect(field_0x18 != 0, 1)` on the first `&&` operand | 6,864 -> **6,849** | 1,574 -> **1,577** | 9,908 |

Both size-exact; `.rel.plt` stays byte-identical; `.ARM.extab` unchanged at 711;
whole-`.so` `.text` differing bytes 81,503 -> **81,298** (-205).  `check` green
(byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN /
drift 0 / parity 5,370).  This is the same pair landed by sibling w11809; the
campaign-allocation residual below was **not** cracked, so the OR/frame wins are
the measured deliverable.

## 2. Campaign residual: exact weight/interval accounting

The residual is the classic one: the original keeps `campaign` in `sl` (r10,
`12e0f8: mov sl,r0`, `12e38a: cmp.w sl,#2`), `missionBlocksPlanets` in `fp`
(r11, `12e07a: mov.w fp,#0`, `12e0d8: mov.w fp,#0`, spilled at `12e3ba`), and
reuses `sl` for `stations` after the campaign check.  Ours spills `campaign` to
`sp+144` (`12e0f8: str r0,[sp,#144]`), puts the shared zero in `sl`, and spills
the mission-block value too.

To get exact numbers I built Android LLVM 7.0.2 `llc` (assertions, from
`android.googlesource.com/toolchain/llvm @ 1d739ffb`) and patched
`lib/CodeGen/CalcSpillWeights.cpp` to dump per-instruction spill weights
(`SPILLDBG=1`).  **`llc -relocation-model=pic` reproduces clang's `Radar::draw`
codegen essentially byte-identically** (only diff: `12e1e2` orig `mov r6,r0`
vs llc `cmp r0,#0`); `-relocation-model=static` does not (frame 0x120 vs 0x128,
different RA).  Recipe:

```sh
clang++ <Radar.cpp flags from compile_commands.json, -g0, -S -emit-llvm> -o radar_nog.ll
llc -mtriple=thumbv7-none-linux-android16 -O2 -relocation-model=pic \
    -disable-postra-machine-licm -filter-print-funcs=_ZN5Radar4drawEP6PlayerP3Hudi \
    -print-machineinstrs radar_nog.ll -o /dev/null > mi_pic.txt
llc ... -debug-only=regalloc ... > regalloc_pic.log
SPILLDBG=1 llc ... > sw.log
```

Weight formula (`CalcSpillWeights.h`): `weight = totalWeight / (size + 25*8)`,
where `25*SlotIndex::InstrDist = 200`.

### 2.1 The two competing intervals

| value | interval | size | totalWeight | weight |
|---|---|---:|---:|---:|
| `campaign` = `%418` | `[2112r,8064B)[8240B,17216B)[18464B,19296B)[19472B,24528r)` | 20,816 | 1.2435 | **5.919979e-05** |
| zero/`missionBlocksPlanets` = `%2480` | `[240r,704B)[864r,928B)[1072r,1136B)[1280r,1344B)[1488r,1552B)[1648r,1680r)[1680r,1728B)[1728B,8064B)[8240B,17216B)[18464B,19296B)[19472r,35680r)[36672B,36704r)` | 33,184 | 2.2412 | **6.674888e-05** |

`campaign` contributions: `COPY $r0` in bb.1 (freq 0.625) w=0.625; `cmp #2`
w=0.6185473.  `%2480` contributions (all in `sw.log`): the `MOVi 0` in bb.1
w=0.625, `STRBi12 536` w=0.625, `STRBi12 304` w=0.625, plus `MOVi 0`
w=0.15625/0.058594/0.048828/0.024414x2 and two `cmp #0` w=0.0024162.

`%2480` is the PHI-eliminated `missionBlocksPlanets` vreg **coalesced with the
`plasmaInRange = 0` / `lockConfirmed = 0` store zero** (`%386 = MOVi 0` in bb.1
feeds `%384 = COPY %386` -> phi).  `isRematerializable()` returns false because
the interval has a PHI def, so the usual 0.5 remat discount is *not* applied.
That is why one shared zero carries 1.875 of the 2.241 total weight.

### 2.2 What must happen to flip the allocation

For `campaign` to outrank `%2480`:
`campaign_totalWeight > 2.2412 * 21016/33384 = 1.411` (currently 1.2435), or
`%2480_totalWeight < 1.2435 * 33384/21016 = 1.975` (currently 2.2412).
The three bb.1 contributions (1.875) are the only realistic lever: dropping
bb.1's frequency from 0.625 to 0.5 would take `%2480` to 1.866 -> 5.59e-05 <
`campaign`.

Greedy trace (`regalloc_pic.log`): `%418` r11 -> evicted by `%127`/`%412` -> r6
-> evicted -> r10 -> evicted by `%2480` (Cascade 27) -> second round -> spilled.
`%2480` r11 -> evicted by `%124` (Cascade 27) -> r10 -> evicted by `%2498`
(Cascade 29) -> second round -> spilled.  So **both** values end up spilled in
ours, whereas the original keeps both in callee-saved regs.

## 3. Variants measured (all reverted; size must stay 9,908)

| shape | raw | aligned | size | campaign reg |
|---|---:|---:|---:|---|
| OR swap (landed) | 6,864 | 1,574 | 9,908 | spill |
| OR swap + frame expect (landed) | **6,849** | 1,577 | 9,908 | spill |
| `missionBlocksPlanets \| (timer < 1)` | 6,864 | 1,574 | 9,908 | spill |
| `mbp != false` / `!!mbp` / `(mbp = ...)` / `if (...) mbp=true` | 7,054 | 1,582 | 9,908 | spill |
| `bool mbp = {}` / `(false)` / `= 0` / `= !true` / `= true && false` | 7,054 | 1,582 | 9,908 | spill |
| nested mission `if` | 7,054 | 1,582 | 9,908 | spill |
| `mbp` single `&&` expression | 7,055 | 1,579 | 9,908 | spill |
| `campaign` unsigned / `(unsigned)campaign >= 2u` | 7,055 | 1,582 | 9,908 | spill |
| `campaign` `int32_t` / `long` | 7,054 | 1,582 | 9,908 | spill |
| `campaign` `short` | 8,451 | 1,607 | 9,912 | spill |
| `campaign` declared top, assigned in place | 7,054 | 1,582 | 9,908 | spill |
| `campaign` before `inAlienOrbit()` | 7,061 | 1,587 | 9,908 | spill |
| `campaign` before `isInTurretMode()` | 7,877 | 1,579 | 9,900 | spill |
| `campaign` before mission block | 7,929 | 1,587 | 9,900 | spill |
| `campaign` after `getEnemies()` | 9,212 | 1,642 | 9,904 | spill |
| `mbp` int/uint/char/uchar/int8/int32 | 7,077 | 1,584 | 9,904 | spill |
| `lockConfirmed = plasmaInRange` (store-to-load forwarded) | 7,054 | 1,582 | 9,908 | spill |
| `plasmaInRange = lockConfirmed = 0` chained | 7,056 | 1,582 | 9,908 | spill |
| stores reversed | 7,056 | 1,582 | 9,908 | spill |
| `SetColor` before stores | 7,085 | 1,590 | 9,908 | spill |
| stores after `SetColor` | 7,085 | 1,590 | 9,908 | spill |
| stores after `getMission()` | 7,096 | 1,590 | 9,908 | spill |
| `bool zero = false; plasma=zero; lock=zero;` | 7,054 | 1,582 | 9,908 | spill |
| `mbp` declared before stores | 7,054 | 1,582 | 9,908 | spill |
| ternary zero `(field_0x14 == 0) ? 0 : 0` | 7,054 | 1,582 | 9,908 | spill |
| `expect(!enabled, 0)` / `expect(enabled,1)==0` / `!expect(!enabled,1)` | 6,884 | 1,584 | 9,908 | spill |
| `expect(enabled, 1)` | 7,090 | 1,589 | 9,908 | spill |
| `expect(!enabled, 1)` (return-path fall-through) | 9,921 | 3,595 | 10,308 | spill |
| **stores after mission block** | 9,537 | 1,663 | **9,872** | **sl** |
| **mission block before stores** | 9,578 | 1,675 | **9,872** | **sl** |
| **one store moved after mission block** | 9,597 | 1,653 | **9,872** | **sl** |
| line 557 uses `campaign` instead of a fresh call | 8,919 | 2,777 | 9,924 | **sl** |
| line 426 uses `campaign` instead of a fresh call | 9,242 | 1,825 | 9,932 | r9 |

### 3.1 Key finding: campaign-in-`sl` and size 9,908 never co-occur

Every shape that puts `campaign` in `sl` also *shrinks* the function: the
"stores after mission" family gives 9,868-9,872 B and `insn=3513/3532` (19 fewer
instructions than the original).  The original is 9,908 B with `campaign` in
`sl`, so it must contain ~40 B / ~19 instructions of code elsewhere that our
source is missing.  Fixing the campaign allocation in isolation therefore
overshoots the size and would break the size-exact gate.  The campaign residual
is coupled to a separate missing-code residual (the function also has 12
`getCurrentCampaignMission()` calls vs the original's 8, so the campaign-116/120/
125 region is the likely place).

## 4. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: scratch/w11808/{campreg.py,batch.py,v2.py..v15.py,regalloc_pic.log,sw2.log}
```
