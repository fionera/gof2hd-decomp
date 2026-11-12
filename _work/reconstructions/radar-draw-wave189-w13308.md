# Radar::draw wave 189 (w13308) — branch/register pass: campaign-in-`sl` bodies found, none is a raw win

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left
at the base. This pass commits only this note. Worktree `wt/w13308`, branch
`wave189-w13308`, base `master` `247466fde` (wave-188 notes commit). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave188-w13209.md` (the 108-cell planet-lock cross product on this
same 6,309 body), `radar-draw-wave187-w13110.md` (the `%122`/`%2481` greedy-allocator
eviction trace: `otherLock` evicts the shared zero from `$r11` and `campaign` spills),
`radar-draw-wave187-w13111.md` (expect/alias re-sweep) and `radar-draw-wave186-w13010.md`
(the greedy trace). This note is self-contained: every number below was measured in this
worktree on the 6,309 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,309** |
| instruction-index alignment | 1,858 |
| instruction count | 3,541 ours / 3,532 orig |
| frame | `sub sp,#296` (`0x128`) |
| `campaign` | spilled `sp+0x90` |
| shared zero | `sl` (`12e07a mov.w sl,#0`) |
| `.rel.plt` | byte-identical |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

The original has `campaign` in `sl` (`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and the
shared zero in `fp` (`12e07a mov.w fp,#0`). Ours swaps them. The whole tail after
`12e38a` differs (~240 of every 256 bytes), so flipping the allocation is the only lever
that can move `raw` by thousands, not tens.

## 1. The campaign-in-`sl` family is reachable — but every size-exact member is raw-worse

This pass concentrated on source constructs that break the `%1212 -> %122` coalescing
(which is what lets `otherLock` evict the zero) while keeping the function at 9,908 B.
The family exists. Its best members:

| cell | raw | size | insn | frame | camp | zero |
|---|---:|---:|---:|---:|---|---|
| base | **6,309** | 9,908 | 3,541 | 296 | spill `0x90` | `sl` |
| `bool otherLock` | 9,247 | **9,856** | 3,510 | 304 | `sl` | `fp` |
| `volatile int otherLock` | 9,501 | **9,868** | 3,528 | 296 | `sl` | `r2` |
| `volatile char/short otherLock` | 9,407/9,408 | **9,892** | 3,531 | 296 | `sl` | `r2` |
| `otherLockFlag` as `char/short/int/uint8_t/long` | 9,442 | **9,896** | 3,521 | 296 | `sl` | `r2` |
| `int otherLock; if(...) otherLock = ...;` (no flag) | 9,442 | **9,896** | 3,521 | 296 | `sl` | `r2` |
| `bool otherLock` + `bossSeen = bossSeen \|\| x==10` | 8,819 | **9,896** | 3,522 | 296 | `sl` | `fp` |
| `bool otherLock` + `if (x==10) bossSeen = true` | 9,177 | **9,884** | 3,518 | 296 | `sl` | `fp` |
| `bool otherLock` + bsor + `int marked = 0` | **7,080** | **9,908** | 3,523 | 296 | `sl` | `fp` |
| `camp_first` (campaign before turretMode/alienOrbit) | 8,562 | **9,900** | 3,541 | 296 | spill `0x90` | `fp` |
| `camp_first` + `volatile int otherLock` | 9,290 | **9,888** | 3,533 | 296 | `sl` | `fp` |
| `camp_first` + `volatile int otherLock` + dock expect + bsif | 9,026 | **9,908** | 3,528 | 296 | `sl` | `fp` |
| `int bossSeen` + `if (x==10) bossSeen = 1` + sound-plain | 9,161 | **9,908** | 3,524 | 304 | spill `0x9c` | `fp` |

`bsor` = `bossSeen = bossSeen || enemy->shipGroup == 10;` (the `cmp; it eq; moveq`
materialisation the original uses). `camp_first` = moving
`int campaign = Globals::status->getCurrentCampaignMission();` above the `turretMode` /
`alienOrbit` initialisers.

The closest size-exact campaign-in-`sl` body is **`bool otherLock` + `bossSeen =
bossSeen || x==10` + `int marked = 0` at raw 7,080** — 771 bytes worse than the base.
It is not landed.

## 2. Why the campaign-in-`sl` bodies still lose: the stack-slot layout shifts

`bsor_marked` (the 7,080 body) has the original's frame (`0x128`), `campaign` in `sl`,
the shared zero in `fp`, and a prologue that matches the original instruction for
instruction **except the `mode` argument slot**:

```
orig  12e05e: str r3, [sp, #100]   ; 0x64   <- mode
bsm   12e05e: str r3, [sp, #108]   ; 0x6c   <- mode (shifted +8)
```

The extra 8 bytes are the planet-loop `stations` array: the original keeps it in `sl`
(`12e3c6 mov sl,r0`, then `12e3fa ldr.w r0,[sl,#4]` inside the loop, spilling only a
cold-block copy to `0x74`), while every campaign-in-`sl` body found here **spills
`stations` to `0x68` and reloads it in the hot loop** (`12e3d2 str.w sl,[sp,#104]`,
`12e486 ldr.w sl,[sp,#104]`, ...). That spill pushes `mode` from `0x64` to `0x6c` and
shifts every later slot by 8, so even with the right `campaign`/zero registers the
same-address byte diff stays above the base.

`bool otherLock` alone grows the frame to `0x130`; only when it is paired with a
`bossSeen` conditional form does the frame come back to `0x128` (the `bossSeen` shape
change removes the extra spill).

## 3. Other cells measured this pass (all reverted)

Every measurement is a full incremental `link_so -j2` + `tools/fn_diff.py` with a
disassembly probe of the `getCurrentCampaignMission` destination register, the frame
size and the `strb.w` zero register (`ulimit -v 12000000`; scratch
`/home/debian/GalaxyOnFire2/scratch/w13308/`).

### 3.1 `missionBlocksPlanets`

| edit | raw | size | camp | zero |
|---|---:|---:|---|---|
| `bool missionBlocksPlanets = false;` | 6,446 | **9,912** | spill `0x90` | `sl` |
| `unsigned char` / `unsigned int` / `short` / `long` | 6,309 | 9,908 | spill `0x90` | `sl` |
| `long long` | 9,116 | **9,924** | spill `0x90` | `sl` |

### 3.2 `otherLock` / `otherLockFlag` types

All of `char`, `unsigned char`, `short`, `unsigned short`, `long`, `unsigned`,
`int8_t`, `uint8_t`, `int16_t`, `uint16_t`, `int32_t` on `otherLock` are byte-identical
to the base (6,309 / 9,908). `volatile bool otherLock` is 9,435 / **9,844** with
`campaign` in `fp` and the zero in `sl` (the reverse of the original).

### 3.3 `__builtin_expect` on the `enemies:` guard (line 675)

| edit | raw | size | frame | camp | zero |
|---|---:|---:|---:|---|---|
| base | 6,309 | 9,908 | 296 | spill `0x90` | `sl` |
| `expect(lockedEnemy != nullptr && (isDying\|\|isDead), 0)` | 8,821 | **9,932** | 304 | spill `0x9c` | `sl` |
| `expect(..., 1)` | 8,715 | **9,904** | 296 | `fp` | `sl` |

The weight-1 form flips `campaign` to `fp` (not `sl`) and is 4 B short. Combining it
with `bool otherLock` (9,980) or with sound-plain (9,900) does not recover `sl`.

### 3.4 `__builtin_expect` on the planet guard (line 535) and the otherLockFlag guard

| edit | raw | size | camp |
|---|---:|---:|---|
| `expect(campaign >= 2 && ..., 0)` | 9,644 | **9,964** | `sl` |
| `expect(campaign >= 2 && ..., 1)` | 9,536 | **9,912** | `sl` |
| `expect(otherLockFlag condition, 0)` | 8,137 | **9,912** | spill `0x90` |
| `expect(otherLockFlag condition, 1)` | 6,636 | **9,916** | spill `0x90` |

### 3.5 `bossSeen`

| edit | raw | size | frame |
|---|---:|---:|---:|
| `int bossSeen = 0;` + `bossSeen = x==10;` | 9,136 | **9,932** | 296 |
| `int bossSeen = 0;` + `if (x==10) bossSeen = 1;` | 9,189 | **9,912** | 304 |
| `unsigned char bossSeen` + assign | 9,138 | **9,932** | 296 |
| `bossSeen = __builtin_expect(x==10, 0/1);` | 6,309 | 9,908 | 296 |

The `__builtin_expect` on the assignment is byte-identical; the `it eq; moveq` shape
only comes from the conditional-assignment forms, and every one of those changes the
function size.

### 3.6 `campaign` lifetime

Moving the `campaign` declaration down to the planet block (just before the line-533
`otherLock` recompute, or just before the line-535 guard) gives 9,216 / **9,904** with
`campaign` spilled to `sp+0x5d` — the shortened live range loses the callee-saved
register entirely.

### 3.7 Loop-variable types

`for (int i ...)` vs `for (uint32_t i ...)` on the planet loop and the enemies loop is
byte-identical (6,309 / 9,908). The gas-cloud and asteroid loops are 6,310; the
landmark loop is byte-identical.

### 3.8 `int marked = 0;` on the base

9,912 (4 B over) / raw 8,424. It is only size-neutral (`+12` B) on top of the
`bool otherLock` + `bsor` body, which is where the 7,080 cell comes from.

## 4. Conclusion

The `campaign`-in-`sl` residual is now reproducible in several size-exact bodies
(the best is 7,080 raw), but none beats the 6,309 base. Two independent blockers remain
after the register flip:

1. the planet-loop `stations` array is spilled in every campaign-in-`sl` body, shifting
   the whole stack frame by 8 (`mode` at `0x6c` vs the original's `0x64`); and
2. the `bossSeen` `it eq; moveq` materialisation only survives at 9,908 in bodies that
   also change the frame to `0x130` or move `campaign` to `fp`.

The base's 3,541-instruction stream is a stronger local optimum for the same-address
byte metric than any of the 9,908 B campaign-in-`sl` bodies found. No size-exact,
`.rel.plt`-identical raw win exists in the assigned branch/register cells on this base.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6309 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13308/`: `Radar.base.cpp` (6,309 base),
`harness.py` (build+measure), `probe.py` (build+measure+frame/campaign-register/zero-
register probe), `batch.py`, `cells1.py`..`cells22.py` (the cells above),
`last_probe.json`, `orig.dis`/`ours.dis`, `bsormarked.dis`, `bsordock.dis`,
`bsif2.dis`, `bool2.dis`, `flagchar.dis`.
