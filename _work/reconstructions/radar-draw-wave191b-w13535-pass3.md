# Radar::draw wave 191b (w13535, pass 3) — campaign-in-`sl` family re-measured on the 6,015 base: no size-exact raw win

**Status:** analysis complete, 2026-11-12. **No new `src/` change**; `src/` is left at the
landed w13535 base (HEAD `a8d78a9e4`, raw 6,015). This pass commits only this note.
Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191b-w13535.md` (the landed `isDying`/`isDead` swap),
`-w13535-pass2.md` (the address-shift profile), `-w13520.md` (gas/planet chains),
`-w13510.md` (station-blip / 24000 / tractor cells) and `-w13319.md` (the original
`campaign`-in-`sl` register map and the `bsor` body).

## 0. Baseline (verified in this worktree)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,015** |
| instruction-index alignment | 1,941 |
| instruction count | 3,541 ours / 3,532 orig |
| `.rel.plt` | byte-identical |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` confirmed at the start of this pass (session log: `verify: byte 4507 linked
4507`, `data: exact 843 of 843`, `imports: 207`, `RATCHET PASS`).

## 1. Where the 6,015 raw bytes actually are

Per-256-byte bucket of the same-address byte diff (fresh build, `src/` at base). Buckets at
~240+ are fully-shifted windows; buckets at <10 are the delta-0 windows of the pass-2 map.

| bucket | diff bytes | | bucket | diff bytes |
|---|---:|---|---|---:|
| `12e000` | 9 | | `12f600` | 175 |
| `12e100` | 1 | | `12f700` | 1 |
| `12e300` | 69 | | `12f800` | 4 |
| `12e400` | 249 | | `12f900` | 1 |
| `12e500` | 245 | | `12fa00` | 3 |
| `12e600` | 234 | | `12fb00` | 1 |
| `12e700` | 240 | | `12fc00` | 158 |
| `12e800` | 233 | | `12fd00` | 130 |
| `12e900` | 244 | | `12fe00` | 133 |
| `12ea00` | 243 | | `12ff00` | 195 |
| `12eb00` | 246 | | `130000` | 130 |
| `12ec00` | 249 | | `130100` | 2 |
| `12ed00` | 237 | | `130200` | 106 |
| `12ee00` | 248 | | `130300` | 242 |
| `12ef00` | 180 | | `130400` | 227 |
| `12f000` | 143 | | `130500` | 221 |
| `12f100` | 220 | | `130600` | 119 |
| `12f200` | 245 | | | |
| `12f300` | 242 | | | |
| `12f400` | 164 | | | |
| `12f500` | 226 | | | |

The only genuine (non-shift) differences left in the delta-0 windows are single bytes:
`12f794` (literal-pool code address), `12f814` / `12f850` (`gasHit` slot `sp+0x60` vs
`sp+0x5c`), `12f85e` (`mode` slot `sp+0x64` vs `sp+0x68`), `12f8ae` / `12f91e` (branch
targets), and four mid-instruction bytes in the enemy loop. All are stack-slot / layout
artefacts, not source differences.

## 2. The `bsor` family, re-measured on the 6,015 base

`bsor` is the only body known to reproduce the original's planet-loop allocation
(`campaign` in `sl`, `fp` loop counter, `sl` `stations`, `marked` at `sp+0x84`).  Components:

- **A** = replace `bool otherLockFlag; ... int otherLock = otherLockFlag;` with a single
  `bool otherLock;` assigned in the same if/else.
- **B** = `bool marked = false;` -> `int marked = 0;`
- **C** = `bossSeen = enemy->shipGroup == 10;` -> `bossSeen = bossSeen || enemy->shipGroup == 10;`

| combo | raw | aligned | size | insn | campaign | 12e194 |
|---|---:|---:|---:|---:|---|---|
| base | **6,015** | 1,941 | 9,908 | 3541 | spill `sp+0x90` | clean |
| A | 9,247 | 2,396 | 9,860 | 3511 | spill | clean |
| B | 8,465 | 2,001 | 9,912 | 3540 | spill | clean |
| C | 9,257 | 2,402 | 9,972 | 3549 | spill | clean |
| AB | 9,263 | 2,388 | 9,864 | 3505 | spill | clean |
| AC | 8,815 | 2,042 | 9,896 | 3522 | **`sl`** | alienOrbit spill |
| BC | 9,174 | 2,430 | 9,896 | 3523 | spill | clean |
| ABC (`bsor`) | 6,786 | 2,037 | **9,908** | 3523 | **`sl`** | alienOrbit spill |
| ABC + `bool alienOrbit` | **6,781** | 2,008 | **9,908** | 3522 | **`sl`** | alienOrbit spill |

The blocker is exactly the +6 B shift the w13319 note identified: `bsor` emits

```
12e190 cmp.w r8, #0
12e194 mov   r1, r8            ; +2
12e196 str.w r8, [sp, #144]    ; +4   <- alienOrbit spilled to sp+0x90
12e19a beq.n ...
```

where the original has only `cmp.w r8,#0; beq.n`.  `alienOrbit` is reloaded at `12e39c`
(`ldr r2,[sp,#144]`) and used as `orrs.w r0,r0,r2`; the original keeps it in `r8` until
`12e39e` and only then reuses `r8` for `stationId` (`12e3fe ldr.w r8,[r0,fp,lsl#2]`).  So the
spill is *not* semantically required — it is the greedy allocator choosing to free `r8` early
once `stationId` becomes the preferred `r8` occupant.  `const int alienOrbit`,
`bool alienOrbit` and `bool otherLock = false` do not remove it.

Because `bsor` is +6 from `12e198` through `12e3d0` (a delta-0 window in the base), it loses
~570 raw bytes there even though its later windows are *closer* than the base's
(`12e6ee`..`12ed52` sits at `-34` instead of the base's `-56`..`-110`).  Net: 6,781 vs 6,015.
A future pass would have to remove the `alienOrbit` spill **and** keep the `stationId`/`marked`
slot colouring (`mode` moves to `sp+0x6c`) to make this family competitive.

## 3. `marked` is type-sensitive; `otherLock`/`planetHit`/`landmarkHit` are not

| cell | raw | size |
|---|---:|---:|
| `bool marked = false;` (base) | **6,015** | 9,908 |
| `uint8_t marked = 0;` + `marked = 1;` | 8,465 | **9,912** |
| `int marked = 0;` | 8,465 | **9,912** |
| `char marked = 0;` | 8,465 | **9,912** |
| `int marked = 0;` (alone, keeps `otherLockFlag`) | 8,465 | **9,912** |

Any non-`bool` `marked` costs +4 B and re-rolls the whole planet tail.  `int marked` alone does
**not** move `campaign` into `sl` (still `str r0,[sp,#0x90]` at `12e0f8`), so `bool otherLock`
is genuinely required for the `sl` colouring.

## 4. Declaration-order / qualifier sweep (all exactly neutral, 6,015 / 9,908)

`stations` as `Array<int> * const`; `stationId`, `currentStation`, `warpGate`, `campaign` as
`const int`; `bool marked` moved before `campaignMission`; `freelance` moved before `marked`;
`bool planetHit` moved before `stations`; `bool gasHit` hoisted to function scope or to just
after the `otherLock` recomputation; `bool landmarkHit` / `bool enemyHit` moved to the top;
`for (int i ...)` / `for (unsigned i ...)`; `2 <= campaign`; `marked != 0`; `marked ? textOffset
+ 14 : textOffset`; `isTarget && stationId != currentStation`; `int isTarget`.  Only
`(unsigned) campaign >= 2u` differs at all (6,016).

## 5. Turret HUD bar block (`12f26c`, the +122 shift)

The original stores `barWidth = (int) hull` **before** the `DrawRegion2D` call
(`12f2a6 vstr s2,[r4,#388]`), while the source has it after.  Moving it before the call is
size-fatal because the original's argument stores collapse into one `stmia`:

```
orig  12f2a4 add r3, sp, #16
      12f2b8 stmia r3!, {r1, r2, r6}   ; 3 args in 4 B
ours  str r0,[sp,#20]; str r1,[sp,#24] ; 2 separate stores
```

| cell | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,015** | 1,941 | 9,908 |
| `barWidth` before the call (hull) | 8,317 | 1,888 | **9,912** |
| `int hullW = (int) hull; barWidth = hullW;` before, arg `hullW` | 8,317 | 1,888 | **9,912** |
| `int hullW = (int) hull;` before, `barWidth = hullW` after (arg `hullW`) | 6,015 | 1,941 | 9,908 |
| `barWidth` before + `(rate/100)*width` operand swap | 6,021 | 1,919 | 9,908 |
| `barWidth` before (hull) + `barWidth` after (emp) | 6,076 | 1,891 | 9,908 |
| `barWidth` after (emp) only | 8,284 | 1,976 | **9,904** |
| `hull = (rate/100) * width` operand swap | 6,020 | **1,884** | 9,908 |
| `float rate = ...; hull = width * rate` | 6,019 | 1,896 | 9,908 |

The operand-swap cells are the best *aligned* results found anywhere in this pass
(1,884 / 1,888 vs 1,941) but they are raw-negative, because the block lives inside the
+122-shifted window: better instruction matching cannot show up in a same-address diff.

## 6. Enemy-lock guard (`12fec4`, the `ittt eq` vs `bne` if-conversion)

| cell | raw | aligned | size |
|---|---:|---:|---:|
| base (`expect(cond && dockNavPtr != nullptr, 1)`) | **6,015** | 1,941 | 9,908 |
| `expect(timer > limit, 1) && dockNavPtr != nullptr` | 6,018 | 1,943 | 9,908 |
| no `__builtin_expect` | 6,730 | **1,877** | 9,908 |
| `navWreck ? 500 : (field_0x24 ? 500 : 0)` | 6,035 | 1,940 | 9,908 |
| `current` as if/else | 6,015 | 1,941 | 9,908 |
| `elapsed` as ternary | 6,015 | 1,941 | 9,908 |
| `totalInt` as ternary | 6,015 | 1,941 | 9,908 |
| both ternaries | 6,015 | 1,941 | 9,908 |

`expect_none` reproduces the original's `bne` + separate loads (aligned 1,877) but the block
placement change costs 715 raw.


## 7. Enemy tractor block (`12f15c`, real source-order difference, raw-negative)

The original evaluates the `tractor:` guard as
`tractorMode2 && field_0x1c == nullptr && wreck && isDead` (`12f15c` ldrb `[r4,#431]`,
`12f162` `[sp,#96]`->`field_0x1c`, `12f168` `wreck`, `12f170` `isDead`), while the source has
`tractorMode2 && wreck && isDead && field_0x1c == nullptr`.  Reordering the source to match:

| cell | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,015** | 1,941 | 9,908 |
| `tractorMode2 && field_0x1c == nullptr && wreck && isDead` | 6,021 | 1,946 | 9,908 |
| `... && field_0x1c == nullptr && isDead && wreck` | 8,310 | 2,005 | **9,904** |
| swap the two stores only (`dockNavPtr` before `field_0x1c`) | 6,015 | 1,962 | 9,908 |
| condition reorder + store swap | 6,021 | 1,946 | 9,908 |

The reorder reproduces the original's compare order exactly, but the block sits in the
`+36..+28` gas/enemy window, so raw still rises by 6.  The store order is *not* controllable:
clang always emits `field_0x1c` (`[r4,#28]`) before `dockNavPtr` (`[r4,#8]`) here, while the
original emits `[r4,#8]` first, regardless of source order.

## 8. Conclusion

Every remaining raw byte is either (a) inside a shifted window whose shift is set by the
`campaign`/`stations`/`fp`/`sl` colouring, or (b) a stack-slot number (`gasHit` `sp+0x60` vs
`sp+0x5c`, `mode` `sp+0x64` vs `sp+0x68`) with no source-level lever.  The `bsor` family is
the only route to the original colouring and is blocked by a single 6-byte `alienOrbit` spill
at `12e194`.  Two genuine source-order differences were confirmed but are raw-negative because
their blocks sit inside shifted windows: the gas-cloud `isDead`-first guard (+1, pass 2) and
the enemy-tractor guard order (+6, §7).  No size-exact, `.rel.plt`-identical, raw-positive cell
was found.

## 9. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6015 aligned= 1941 size=9908/9908 insn=3541/3532
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13535/`: `try.py` (1 s compile+link+fn_diff
harness), `w1.json`–`w6.json` (this pass's cell definitions), `bsor_sweep.py` /
`bsor2.py` (the §2 table), `o_orig.txt` / `o_ours.txt` (base disassembly), `shift.py`
(the pass-2 delta map).
