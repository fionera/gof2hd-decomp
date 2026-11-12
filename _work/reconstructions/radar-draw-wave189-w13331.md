# Radar::draw wave 189 (w13331) — independent full-body / register-allocation pass: alienOrbit spill isolated, no size-exact raw win

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left at
the base. This pass commits only this note. Worktree `wt/w13331`, branch `wave189-w13331`,
base `master` `a80445390`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13308.md` (the campaign-in-`sl` family), `radar-draw-wave188-w13210.md`
(the 108-cell single/pair/triple cross product and the greedy spill-weight trace) and
`radar-draw-wave189-w13319.md` (the independent register/branch pass). This note is
self-contained: every number below was measured in this worktree on the 6,309 base.

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
| planet-loop counter `i` | `sl` |
| planet-loop `stations` | spilled `sp+0x84` |
| `.rel.plt` | byte-identical |
| `.text` differing bytes (whole `.so`) | 74,006 |
| `.ARM.extab` differing bytes | 639 (size/base exact at `0x1a3b78`, 156,312 B) |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. Where the base actually diverges (this pass's new map)

The base is **already byte-identical to the original through `12e2ff`** except for one
branch offset and a handful of stack-slot immediates:

* `12e000` block: 9 differing bytes (prologue immediates: `mode` slot, pc-relative
  literal offsets, the `12e074` cold-branch offset).
* `12e100` block: **1** differing byte (the `12e1b8` branch offset).
* `12e200` block: **0** differing bytes.

So the matrix/vector temporaries and the waypoint block are already exact; there is no
win there. The divergence starts at `12e300` and is dominated by the planet block
(`12e400`–`12e7a8`) and the two cold loops (`12e7aa`–`12ec24`).

The base is **2 bytes shorter** than the original at `12e400` because it lacks the
original's planet-loop `stations` cold copy:

```
orig  12e3c6 mov  sl, r0            ; stations -> sl
      12e3c8 mov.w fp, #0           ; i = 0
      12e3cc movs r0, #0
      12e3ce str.w sl, [sp, #0x74]  ; stations cold copy   (4 B, high reg)
      12e3d2 str  r0, [sp, #0x98]   ; planetHit = 0
base  12e3c6 str  r0, [sp, #0x84]   ; stations -> stack
      12e3c8 mov.w sl, #0           ; i = 0
      12e3cc movs r0, #0
      12e3ce str  r0, [sp, #0x90]   ; planetHit = 0
```

Everything after `12e3d0` is therefore shifted by 2 bytes and uses `sl` for `i` instead
of `fp`. That is the single largest structural residual in the base.

## 2. The campaign-in-`sl` family: the real blocker is the `alienOrbit` spill

The prior waves established that `campaign` must move to `sl` (and the shared zero to
`fp`) to reproduce the original's tail. This pass re-derived the family from the MIR and
isolated the exact instruction that makes every size-exact member raw-worse than the
base.

`bsor` = `bool otherLock` (direct OR) + `int marked = 0` + `bossSeen = bossSeen ||
enemy->shipGroup == 10`. It is the only size-exact campaign-in-`sl` body (raw 7,080).
In the `VirtRegRewriter` dump (`allpass_bsor.txt`, function `_ZN5Radar4draw...`,
`%stack.88`) the alienOrbit value is spilled immediately after the `goingToPlanet()`
call:

```
# bsor, 12e190
cmp.w r8, #0
mov   r1, r8          ; <-- dead copy inserted by the spiller
str.w r8, [sp, #0x90] ; <-- alienOrbit spill (4 B)
beq.n 12e1ae
# ... reloaded much later at 12e39c and 12e966
```

The original has no spill (`12e190 cmp.w r8,#0; 12e194 beq.n`), so the bsor tail is
shifted **+6 bytes** from `12e194` onward. `r8` is not used between `12e196` and
`12e39c`, so the spill is a pure greedy-allocator eviction, not a real conflict.

The `mov r1,r8` is dead (r1 is clobbered at `12e1d0`); it is the split-copy artifact of
the spilled interval.

### 2.1 Removing the spill: `volatile int autoPilotTarget`

The spill is driven by the register pressure of the rare `else` branch of
`if (alienOrbit)` (the `getAutoPilotTarget()`/`getPlanetTargets()` path). Marking
`autoPilotTarget` volatile forces it to the stack and removes the `alienOrbit` spill
entirely:

| body | raw | aligned | size | insn | `12e194` |
|---|---:|---:|---:|---:|---|
| base | **6,309** | 1,858 | 9,908 | 3,541 | `beq` (no spill) |
| `bsor` | 7,080 | 1,834 | 9,908 | 3,523 | `mov r1,r8; str r8,[sp,#0x90]` |
| `bsor` + `bool alienOrbit` | **7,074** | 1,836 | 9,908 | 3,522 | same spill |
| `bsor` + `volatile int autoPilotTarget` | **6,918** | 2,014 | 9,908 | 3,520 | **no spill** |
| `bsor` + `bool alienOrbit` + `volatile int autoPilotTarget` | 8,843 | 2,001 | 9,912 | 3,521 | no spill, +4 B |

`bsor` + `volatile int autoPilotTarget` is the best campaign-in-`sl` body found by this
pass (raw 6,918, size-exact). It is still 609 worse than the base because the volatile
edit re-colours the **waypoint block**: the `&this->lockLabel` temp moves from `r9` to
`sl`, which forces campaign to be saved/restored around the `update()` call:

```
orig  12e28a add.w r9, r4, #396   ; &lockLabel -> r9
bsorv 12e286 add.w sl, r4, #396   ; &lockLabel -> sl
bsorv 12e23c mov   r9, sl         ; save campaign  (+2 B, extra)
bsorv 12e2e2 b.n   12e302         ; (+2 B)
bsorv 12e2e8 mov   sl, r9         ; restore campaign (+2 B)
```

plus `beq.n` → `beq.w` at `12e3f8` (+2 B). Net +6 B of extra instructions, which is
exactly the shift the spill removal saved. The two effects cancel, and the waypoint
block (`12e200`, previously 0 diff) now differs by 195 bytes.

## 3. `stations` lifetime / address-taken shape (the assigned lever)

All of these are neutral or size-fatal; the base's `stations` stack slot is not moved by
any source-level spelling of the local:

| edit | raw | size | notes |
|---|---:|---:|---|
| base (`Array<int> *stations = ...getStations();`) | **6,309** | 9,908 | stations `sp+0x84` |
| `Array<int> *const stations` | 6,309 | 9,908 | byte-identical |
| `Array<int> &stations` + `stations.data()[i]` | 6,309 | 9,908 | byte-identical |
| `SolarSystem *planetSystem` local + `planetSystem->getStations()` | 6,309 | 9,908 | byte-identical |
| declare `stations` after `planetHit` | 6,309 | 9,908 | byte-identical |
| `uint32_t *stations` + `stations[i]` | 8,646 | **9,892** | `data()` load hoisted, −16 B |
| `int *stationData = stations->data()` + `stationData[i]` | 9,266 | **9,876** | −32 B |
| `stations` loaded before the `campaign >= 2` guard | 9,058 | **9,904** | −4 B |

`stations` only leaves `sp+0x84` when `campaign` also leaves `sp+0x90` (the bsor
family); the pointer's own spelling is fully folded by the compiler.

## 4. Other cells measured this pass (all reverted)

Every row is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`; scratch `/home/debian/GalaxyOnFire2/scratch/w13331/`).

### 4.1 Types / lifetimes on the base body

| edit | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,309** | 1,858 | 9,908 |
| `int marked` | 8,424 | 1,836 | **9,912** |
| `bool alienOrbit` | 6,317 | 1,830 | 9,908 |
| `int marked` + `bool alienOrbit` | 8,420 | 1,859 | **9,912** |
| `int planetHit` | 9,236 | 2,089 | **9,948** |
| `int turretMode` | 9,238 | 2,694 | **9,856** |
| `int otherLockFlag` | 9,442 | 1,944 | **9,896** |
| `int bossSeen` | 9,136 | 2,775 | **9,932** |
| `bool marked` declared before the loop | 6,309 | 1,858 | 9,908 |
| `int marked` declared before the loop | 8,424 | 1,836 | **9,912** |
| `volatile int autoPilotTarget` | 9,364 | 2,059 | **9,872** |
| swap `plasmaInRange`/`lockConfirmed` stores | 6,311 | 1,858 | 9,908 |
| move both zero stores after `SetColor` | 6,340 | 1,866 | 9,908 |
| `bool campaign = campaignVal >= 2` | 6,309 | 1,858 | 9,908 |
| `campaign` before `turretMode` (`camp_first`) | 8,562 | 1,848 | **9,900** |
| landmark `!Globals::status->inAlienOrbit()` (shorten live range) | 6,373 | 1,876 | 9,908 |
| planet guard `!Globals::status->inAlienOrbit()` | 9,187 | 2,749 | **9,868** |

### 4.2 `otherLock` / `marked` / `bossSeen` cross product on the bsor body

| edit | raw | size |
|---|---:|---:|
| `bsor` | 7,080 | 9,908 |
| `bsor` + `bool alienOrbit` | **7,074** | 9,908 |
| `bsor` + `unsigned char alienOrbit` | 7,080 | 9,908 |
| `bsor` + `int otherLock` (direct) | 9,206 | **9,892** |
| `bsor` + `bool marked` | 8,669 | **9,896** |
| `bsor` + `int bossSeen` | 9,208 | **9,888** |
| `bsor` + `int planetHit` | 8,926 | **9,928** |
| `bsor` + `int turretMode` | 8,206 | **9,904** |
| `bsor` + `volatile int autoPilotTarget` | **6,918** | 9,908 |

### 4.3 Landmark-loop guard / branch layout

Splitting the `if (i == 2 || expect(data()[i]==nullptr,0) || !isVisible()) continue;`
chain into separate `if`s (with or without the expect) is **byte-identical to the base**
(6,309 / 9,908) — clang re-fuses it into the same `ittt ne` predicated form. Removing the
expect is 6,338; flipping it to weight 1 is 9,248 / **9,904**. The original's branchy
guard (`cmp.w sl,#2; beq.w` then a separate `data()[i]==nullptr; beq.w`) is not
reachable from the current source shape.

## 5. Conclusion

The 6,309 base is a stronger local optimum than any size-exact campaign-in-`sl` body
found here. The two independent blockers are now both pinned to exact instructions:

1. the base is missing the original's `str.w sl,[sp,#0x74]` planet-loop `stations` cold
   copy, which shifts the planet block by −2 bytes; and
2. every body that restores the `mov sl,r0` + cold-copy structure either keeps the
   `alienOrbit` spill (`mov r1,r8; str.w r8,[sp,#0x90]` at `12e194`, +6 B) or removes it
   with a `volatile`/type edit that re-colours the waypoint block and re-adds the same
   +6 B as `mov r9,sl` / `b.n` / `mov sl,r9` / `beq.w`.

No size-exact, `.rel.plt`-identical raw win was found on the assigned cells; `src/` is
left at the base.

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6309 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13331/`: `Radar.base.cpp` (6,309 base),
`cand_bsor2.cpp` (7,080), `cand_bsor2_balien.cpp` (7,074), `cand_bsor2_volapt.cpp`
(6,918), `gen*.py` (candidate generators), `batch_run.sh` / `probe.sh` (build+measure+
disasm), `orig.dis` / `base_now.dis` / `bsor.dis` / `bsor2_volapt.dis`,
`allpass_base.txt` / `allpass_bsor.txt` (`-mllvm -print-after-all` dumps).
