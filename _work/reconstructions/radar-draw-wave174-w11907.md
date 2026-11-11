# Radar::draw wave 174 (w11907) — call-count premise disproved; campaign/`sl` allocation is size-fatal

**Status:** no source change landed, 2026-11-11. Worktree `wt/w11907`, branch
`wave174-w11907`, base `master` `be9dd429b`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave173-w11808.md` (weight accounting + variant table),
`radar-draw-wave173-w11809.md` (landed OR swap + frame expect), and
`radar-draw-wave173-w11807.md` (image-chain re-sweep).

## 0. Baseline (rebuilt on this base)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 6,849 |
| instruction-index alignment | 1,577 |
| instructions | 3,541 ours / 3,532 orig |
| whole-`.so` `.text` differing bytes | 80,897 |
| `.rel.plt` | byte-identical (0) |
| gate | RATCHET PASS (byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370) |

## 1. The task premise is false: both functions have 12 campaign calls

The wave-174 brief said "the campaign-116/120/125 region has 12
`getCurrentCampaignMission()` calls vs the original's 8".  This is wrong.
`objdump -d` on the two function bodies gives **12 `blx getCurrentCampaignMission`
in each**, at the same semantic sites:

| # | original | ours (base) | context |
|---|---:|---:|---|
| 1 | 12e0f4 | 12e0f4 | prologue `int campaign = ...` |
| 2 | 12e368 | 12e368 | landmark-station guard (`> 93`) |
| 3 | 12e460 | 12e462 | campaign-mission visible guard (`< 148`) |
| 4 | 12e46c | 12e470 | same guard (`>= 152`) |
| 5 | 12e49a | 12e4a0 | `!= 120` target-station image |
| 6 | 12e624 | 12e5ee | `== 116` status-bit block |
| 7 | 12e65a | 12e64c | `== 120 && station == 93` |
| 8 | 12e676 | 12e67a | `== 125 && isFreighterMissionStation` |
| 9 | 1302c0 | 1302c0 | enemy-lock sound/`< 106 ? 2241 : 2242` |
| 10 | 1302fa | 1302fa | enemy-lock tail |
| 11 | 13059c | 13059c | cargo/`hudEvent` tail |
| 12 | 13064e | 13064e | music/sound tail |

So there is no call-count residual to fix.  The only residual is the
register allocation of `campaign`.

## 2. The allocation residual, exactly

Original:
```
12e0f8  mov  sl, r0        ; campaign -> r10
12e38a  cmp.w sl, #2       ; campaign >= 2
12e3ba  str.w fp, [sp,#112]; missionBlocksPlanets -> r11, spilled
12e3c6  mov  sl, r0        ; stations -> r10 (campaign dead)
12e3ce  str.w sl, [sp,#116]
12e3c8  mov.w fp, #0       ; planet-loop index i -> r11
```
Ours (base):
```
12e0f8  str  r0, [sp,#144] ; campaign spilled
12e38a  str  r0, [sp,#148]
12e38c  ldr  r0, [sp,#144]
12e38e  cmp  r0, #2
12e3ba  str.w sl, [sp,#116]; zero/missionBlocksPlanets -> r10, spilled
12e3c8  mov.w sl, #0       ; planet-loop index i -> r10
```

The `fn_diff` categorisation of the base is **purely allocation/layout**: the
top buckets are `extra-ours:ldr` 75 / `extra-orig:ldr` 64, `operand:b.n` 56,
`operand:ldr` 56, `extra-ours:movs` 49, `extra-orig:ldr.w` 44,
`operand:cbz` 43, `extra-orig:movs` 41, `width:ldr.w/ldr` 21,
`width:ldr/ldr.w` 11.  Every `opcode:` bucket in the aligned diff is a
*shifted* block (e.g. `mov sl,r0` vs `str r0,[sp,#144]` at the campaign
store, or the literal-pool words at `12e79a`), not a different operation.
The mnemonic multiset confirms the same picture: ours has 18 more 16-bit
`ldr` and 28 fewer 32-bit `ldr.w` (525/243 vs 507/271), plus 11 more `mov`
and 5 more `mov.w` — i.e. the same operations encoded narrow because the
values sit in low registers.  The source is operation-equivalent to the
original; only register choice, encoding width, stack slots and block
placement differ.

## 3. Why the greedy allocator picks the zero over `campaign`

The wave-173 note established the weights: `campaign` `%415` = 5.919979e-05
(size 20,816), shared zero/`missionBlocksPlanets` `%2481` = 6.538662e-05
(size 33,894).  Re-reading the greedy trace (`RA_TRACE=1`, patched LLVM 7.0.2)
shows the decisive moment is the **second** selection of `%2481` (stage 1):

```
select %2481 size=33894 w=6.538662e-05
try %2481 cand=$r10 intf=1 i0=%415(w=5.919979e-05)
try %2481 cand=$r11 intf=1 i0=%121(w=9.092625e-05)
assign %2481 -> $r10 (tryEvict)      # evicts %415, not %121
...
select %415 size=20816 w=5.919979e-05
try %415 cand=$r10 intf=1 i0=%2481(w=6.538662e-05)
try %415 cand=$r11 intf=1 i0=%121(w=9.092625e-05)
FINAL %415 -> $noreg                 # spilled
```

`%121` is the shared constant `1` (`t2MOVi 1`, size 32,536, weight
9.092625e-05).  Because it sits in `r11` with a higher weight than
`campaign`, `%2481` evicts `campaign` from `r10` instead of `%121` from
`r11`.  Flipping this needs `%121`'s weight below `campaign`'s (or `%121`
out of `r11`) — not the `bb.1` frequency lever the wave-173 note proposed.

## 4. Forcing the allocation alone is -92 B

Using the patched `llc` (`FORCEVREG` hook) to force `%415` (raw vreg
`2147484063`) into `$r10` (`ARM::R10 = 76`; note the ARM physreg enum is
`r0..r7 = 0..7`, then `fpsid=8, itstate=9, lr=10, pc=11, sp=12, ...`,
`R10=76`):

```sh
NOEVICTASSERT=1 FORCEVREG=2147484063:76 FORCEANYFN=1 FORCEEVICT=1 \
  llc -mtriple=thumbv7-none-linux-android16 -O2 -relocation-model=pic \
      -disable-postra-machine-licm radar_base.ll -filetype=obj -o x.o
# -> mov sl,r0 / cmp.w sl,#2  (campaign in sl)
# -> _ZN5Radar4drawEP6PlayerP3Hudi size 0x2658 = 9,816 B  (-92 vs base)
```

So the allocation change alone is **92 bytes short** of the original.  The
original is 9,908 B *with* campaign in `sl`; the base's spill code is what
makes up the difference.  This is the core coupling: fixing the allocation
without adding ~92 B of other code breaks the size-exact gate.

## 5. Every campaign-in-`sl` source shape is size-short and raw-worse

All measured this session (full `link_so` + `fn_diff`, `-j2`, `ulimit -v
12000000`), base = 6,849 / 1,577 / 9,908 / spill:

| shape | raw | aligned | size | campaign |
|---|---:|---:|---:|---|
| base | **6,849** | **1,577** | **9,908** | spill |
| stores after mission block | 9,520 | 1,659 | 9,868 | **sl** |
| `int turretMode` | 9,256 | 2,007 | 9,836 | **sl** |
| `bool notTurretMode` | 9,542 | 1,659 | 9,852 | **sl** |
| `bool otherLock` | 9,228 | 2,008 | 9,848 | **sl** |
| `expect(whole campaign cond, 1)` | 9,573 | 2,405 | 9,912 | **sl** |
| `expect(!alienOrbit, 0)` | 9,332 | 2,995 | 9,884 | **sl** |
| `expect(last three operands, 1)` | — | — | 9,940 | **sl** |

The important result is not just that they are size-short: **they are all
raw-worse** (9,200–9,600 vs 6,849).  Fixing `campaign` alone makes the rest
of the allocation cascade diverge further from the original.  The base's
spilled-campaign allocation is the best raw shape known.

## 6. Other measured cells (all reverted; size must stay 9,908)

| shape | size | campaign |
|---|---:|---|
| `int otherLockFlag` | 9,876 | spill |
| `int otherLock` (direct PHI) | 9,876 | spill |
| `int planetHit` | 9,940 | spill |
| `int marked` | 9,912 | spill |
| `int landmarkHit` | 9,932 | spill |
| `int bossSeen` | 9,896 | spill |
| `int enemyHit` | 9,928 | spill |
| `bool wreck` (from `int`) | 9,900 | spill |
| `int alwaysEnemySeen` | 9,952 | spill |
| `int gasHit` | 9,904 | spill |
| `enabled != 1` | 9,916 | spill |
| `enabled < 1` / `enabled == 0` | 9,908 | spill |
| `expect(planetTargets != nullptr, 1)` | 9,908 | spill |
| `campaign > 1` / `2 <= campaign` | 9,908 | spill |
| `landmark != nullptr` | 9,908 | spill |

Exhaustive pair/triple search over 18 type/shape changes (969 combinations)
found **zero** combination with size 9,908 and campaign in `sl`.

## 7. `__builtin_expect` on the entry `enabled` check does not help

`if (!this->enabled) return;` is `icmp eq i8 %35, 0`; LLVM's
`calcZeroHeuristics` (`ZH_TAKEN_WEIGHT=20`, `ZH_NONTAKEN_WEIGHT=12`) gives the
body `bb.1` a frequency of **62.5 %**.  Patching the IR branch weights
directly (`!prof !{!"branch_weights", i32 R, i32 B}`) and re-running `llc`
shows campaign still ends in `$noreg` at every ratio down to `f = 0.4`
(sizes 9,908–9,952); at `f = 0.5` campaign's own COPY weight falls with the
zero's, so the ordering does not flip.  The wave-173 note's "drop bb.1 to
0.5" estimate ignored that `campaign`'s `COPY $r0` is also in `bb.1`.

## 8. Conclusion

* The call count is already correct (12 = 12); the brief's "8" is wrong.
* The source is instruction-equivalent to the original; the residual is the
  greedy RA cascade triggered by `campaign`/zero weight ordering.
* Every campaign-in-`sl` shape is size-short (9,816–9,912) **and** raw-worse
  (9,200–9,600).  The base (6,849 / 1,577 / 9,908) is the best known shape.
* `src/` is left at the base.  Gate after the pass: byte 4,507 / linked
  4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 /
  parity 5,370 / `.text` 80,897 / `.rel.plt` 0.

## 9. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w11907/Radar.cpp.base src/game/weapons/Radar.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /home/debian/GalaxyOnFire2/scratch/w11907/
#   measure.sh, compile_one.py, harness.py, opsumm.py, opdiff.py,
#   multiset.py, regiondiff.py, findbytes.py, catdiff.py,
#   sweep_enabled.py, sweep_types.py, sweep_bools.py, sweep_tm.py,
#   sweep_ntm.py, sweep_marked.py, sweep_combo.py, search_combo.py,
#   sweep_campexpect.py, sweep_expect_combo.py, sweep_expect2.py,
#   sweep_raw.py, irweight.py, radar_base.ll, base.so, var_*.so
```
