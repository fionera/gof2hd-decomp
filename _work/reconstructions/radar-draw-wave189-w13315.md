# Radar::draw wave 189 (w13315) — full-body re-derivation: the campaign-in-`sl` body is
# reachable at 9,908 B, but every size-exact member is raw-worse

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left
at the base. This pass commits only this note. Worktree `wt/w13315`, branch
`wave189-w13315`, base `master` `b1455e159`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13308.md` (branch/register pass: the campaign-in-`sl`
family), `radar-draw-wave188-w13209.md` (the 108-cell planet-lock cross product) and
`radar-draw-wave187-w13110.md` (the `%122`/`%2481` greedy-allocator eviction trace). This
note is self-contained: every number below was measured in this worktree on the 6,309
base.

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
| `.text` diff (whole `.so`) | 74,032 |
| `.ARM.extab` diff | 639 (size/base exact at `0x1a3b78`, 156,312 B) |
| `.rel.plt` / `.rodata` / `.data` / `.bss` | 0 / 0 / 0 / 0 |
| `.note.gnu.build-id` | 20 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. The original's planet block, re-derived from the disassembly

The original's planet block is laid out **inline** and the whole tail after it is
byte-stable because the register assignment matches:

```
12e3c6: mov  sl,r0            ; sl = stations            (campaign's register, dead at 12e38a)
12e3c8: mov.w fp,#0           ; fp = i                   (the shared zero's register, dead)
12e3cc: movs r0,#0
12e3ce: str.w sl,[sp,#0x74]   ; cold copy of stations
12e3d2: str  r0,[sp,#0x98]    ; planetHit = 0
12e3d4: ... loop head, cmp fp,size
12e3fa: ldr.w r0,[sl,#4]      ; stations->data()   <- hot loop keeps stations in sl
12e3fe: ldr.w r8,[r0,fp,lsl#2]; stationId
12e40c: mov  r9,r0            ; currentStation
```

The two long-lived callee-saved values are `campaign` (`sl`, `12e0f8 mov sl,r0`,
`12e38a cmp.w sl,#2`) and the shared zero (`fp`, `12e07a mov.w fp,#0`, reused at
`12e3ba str.w fp,[sp,#0x70]` and re-materialised at `12e3c8`). When `campaign` dies at
`12e38a`, `sl` is reused for `stations`; when the zero dies at `12e3ba`, `fp` is reused
for the loop counter. That is the whole tail's register skeleton.

Original stack-slot map (planet block):

| slot | original |
|---|---|
| `0x64` | `mode` |
| `0x68` | `getStatusValue` temp (shared with the enemies-block `r9` save) |
| `0x6c` | `landmarkHit` |
| `0x70` | `&targetPos` (shared with a zero init) |
| `0x74` | `stations` cold copy |
| `0x80` | `textOffset` |
| `0x84` | `marked` (shared with `&targetPos` in the landmark block) |
| `0x90` | `otherLock` |
| `0x94` | `turretMode` |
| `0x98` | `planetHit` |

The base instead has `mode 0x64, temp 0x68, temp 0x6c, landmarkHit 0x70, ??? 0x74,
stations 0x84, campaign 0x90, otherLock 0x94, turretMode 0x98, planetHit 0x90` — a
rotation. `campaign`'s spill to `0x90` is the seed of the whole rotation.

## 2. The best size-exact campaign-in-`sl` body found: raw 7,074

Construct (on the base), **size-exact 9,908**, `campaign` in `sl`, zero in `fp`:

1. replace the `otherLockFlag` if/else with the plain OR chain
   `bool otherLock = lockedAsteroid != nullptr || lockedGasCloud != nullptr ||
   field_0x1c != nullptr || field_0x14 != 0;`
   (the `bool` type is what breaks the `%1212 -> %122` coalescing of wave-187);
2. `bossSeen = bossSeen || enemy->shipGroup == 10;` inside the rare `shipGroup==10`
   guard (gives the original `cmp; it eq; moveq` materialisation);
3. `bool marked` -> `int marked = 0;` (the original stores 4 bytes to `0x84`);
4. `int alienOrbit` -> `bool alienOrbit` (raw 7,080 -> 7,074).

| cell | raw | aligned | size | insn | frame | camp | zero |
|---|---:|---:|---:|---:|---:|---|---|
| base | **6,309** | 1,858 | 9,908 | 3,541 | 0x128 | spill `0x90` | `sl` |
| `bool otherLock` | 9,247 | 2,659 | **9,856** | 3,510 | 0x130 | `sl` | `fp` |
| + `bsor` | 8,819 | 1,870 | **9,896** | 3,522 | 0x128 | `sl` | `fp` |
| + `int marked` (7080 body) | 7,080 | 1,834 | **9,908** | 3,523 | 0x128 | `sl` | `fp` |
| + `bool alienOrbit` (7074 body) | **7,074** | 1,836 | **9,908** | 3,522 | 0x128 | `sl` | `fp` |
| 7074 + `egoPos`/`targetPos` swap | 7,079 | 1,825 | **9,908** | 3,523 | 0x128 | `sl` | `fp` |
| 7074 + `int turretMode` | 8,362 | 1,917 | **9,904** | 3,523 | 0x128 | `sl` | `fp` |

`bsor` = `bossSeen = bossSeen || enemy->shipGroup == 10;`.

The 7,074 body's planet loop is **exactly the original's skeleton**:
`12e3ca mov sl,r0` (stations), `12e3cc mov.w fp,#0` (i), `12e400 ldr.w r0,[sl,#4]`
(hot loop keeps `stations` in `sl`), `12e3d2 str.w sl,[sp,#0x68]` (cold copy at the
wrong slot). So the task's "stations spills instead of staying in sl" is not the blocker
in this body — the hot loop does keep `stations` in `sl`.

## 3. Why the 7,074 body still loses: a 4-byte address shift before the loop

The 7,074 body's planet loop starts at `12e3ca`, the original's at `12e3c6` — a **+4
byte shift**. The shift is caused by `alienOrbit` being spilled across the landmark
block:

```
7074  12e194: mov  r1,r8
      12e196: str.w r8,[sp,#0x90]   ; alienOrbit spill  (r8 needed in the landmark block)
      ...
      12e39c: ldr  r2,[sp,#0x90]    ; reload for the `orrs r0,r2` alienOrbit test
orig  12e39e: orrs.w r0,r0,r8       ; alienOrbit stays in r8
```

`r8` is needed inside the out-of-line landmark block because that block uses `r9` for
`&this->egoPos` (`12e8ec mov r0,r9`; `&targetPos` is at `[sp+0x70]` in both bodies) and
then `r9` again for the `originX/originY` offset (`7074 12e91c mov.w r0,#560;
12e92e ldr.w r8,[r4,r0]`) and for `labelStrings`/`gameText` temporaries
(`12e9a4 ldr.w r9,[sp,#0x64]`, `12ea12 ldr.w r9,[r1]`). The original instead keeps
`r9` free for the offset (`12e924 mov.w r9,#560; 12e942 ldr.w r5,[r4,r9]`) and uses
`fp` for `Globals::gameText` (`12e936 ldr.w fp,[r0]`), so `alienOrbit` survives in
`r8` and the planet loop lands at the original address.

Because `raw` is a same-address byte diff, the +4 shift mis-aligns the whole tail: the
7,074 body's diff is 538 bytes before `12e3c6` (vs 30 for the base) and 6,542 bytes
after it (vs 6,279 for the base). Net +765.

The same shift explains every other campaign-in-`sl` body: `bool otherLock` alone grows
the frame to `0x130` (`sub sp,#304`), so the prologue itself stops matching.

## 4. Coalescing-break levers re-measured (all size-exact unless noted)

The wave-187 result is confirmed: the `%1212 -> %122` merge (shared zero into
`otherLock`) is the root, and both `bool otherLock` and a non-`bool` `turretMode`
break it and move `campaign` into `sl`. New cells measured this pass:

| edit (on the base) | raw | size | camp | zero |
|---|---:|---:|---|---|
| `int turretMode` / `char` / `unsigned` | 9,238 | **9,856** | `sl` | `fp` |
| `int turretMode` + `bsor` + `int marked` | 8,368 | **9,904** | `sl` | `fp` |
| `int turretMode` + `bool otherLock` + `bsor` + `int marked` | 8,368 | **9,904** | `sl` | `fp` |
| `bool otherLock` + `bsor` + `int marked` + `int turretMode` | 8,368 | **9,904** | `sl` | `fp` |
| `bool alienOrbit` (base otherwise) | 6,317 | 9,908 | spill | `sl` |
| `unsigned campaign` / `uint32_t` | 6,310 | 9,908 | spill | `sl` |
| `long campaign` / `char`/`unsigned alienOrbit` | 6,309 | 9,908 | spill | `sl` |
| `short`/`unsigned short`/`unsigned char`/`signed char campaign` | 8,380-8,441 | **9,912-9,916** | spill | `sl` |

## 5. Structural reorderings (all reverted)

Every cell below is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`; scratch `/home/debian/GalaxyOnFire2/scratch/w13315/`).

### 5.1 Planet-loop declaration order

| edit | raw | size |
|---|---:|---:|
| base (`stationId, currentStation, warpGate, textOffset, campaignMission, freelance, marked`) | **6,309** | 9,908 |
| `marked` declared then assigned (`bool marked; marked = false;`) | 6,309 | 9,908 |
| `stationId` hoisted to the top of the loop body | 6,309 | 9,908 |
| `currentStation` before `stationId` | 8,980 | **9,900** |
| `campaignMission/freelance` before `stationId` | 9,701 | **10,160** |
| `textOffset`/warp-gate block after `campaignMission/freelance/marked` | 9,736 | **10,232** |
| `int *sd = stations->data();` alias, `sd[i]` everywhere | 9,266 | **9,876** |

### 5.2 Early declaration order (turretMode / alienOrbit / campaign)

| order | raw | size | camp |
|---|---:|---:|---|
| base `T A C` | **6,309** | 9,908 | spill |
| `T C A` | 6,316 | 9,908 | spill |
| `A T C` | 6,329 | 9,908 | spill |
| `A C T` / `C A T` | 6,334 | 9,908 | spill |
| `C T A` | 8,562 | **9,900** | spill |

### 5.3 `marked` / `planetHit` / `landmarkHit` / `enemyHit` types

81-cell cross product (`bool`/`int`/`char` each) on the base: the all-`bool` base is the
unique best at 6,309. The next best is `int marked + char planetHit + char landmarkHit +
char enemyHit` at 8,272 / **9,988**. `int marked` alone is 8,424 / **9,912**.

### 5.4 Landmark block

| edit | raw | size |
|---|---:|---:|
| base | **6,309** | 9,908 |
| `egoPos`/`targetPos` assignment swap | 6,318 | 9,908 |
| `lmFont` before `offset` | 6,309 | 9,908 |
| `rowHeight` moved inside `if (i <= 1)` | 6,310 | 9,908 |
| `KIPlayer *lm = landmarks->data()[i]` alias | 9,298 | **9,784** |
| `offset` as `if/else` or `(i & 0x7fffffff) == 0` (base and 7074 bodies) | identical | identical |

## 6. New this pass: the `volatile autoPilotTarget` + String-temp body (raw 6,721)

The 7,074 body's early `alienOrbit` spill is caused by the `onPlanetCourse` false branch:
the allocator needs a scratch register for the `targets` pointer at `12e700`, and because
`fp` is the live shared zero it uses `r8` (alienOrbit), spilling alienOrbit at `12e196`.
The original instead keeps `autoPilotTarget` on the stack (`str r0,[sp,#0x98]` at
`12e6f8`) and uses a 4-register shuffle (`fp->r9`, `r8->fp`, `sl->r8`) at
`12e706-12e70c`, so `r8` is never clobbered and alienOrbit is not spilled.

Forcing `autoPilotTarget` to a stack slot with `volatile int autoPilotTarget = ...`
removes the early alienOrbit spill exactly (`12e190 cmp.w r8,#0; 12e194 beq.n` matches
the original). It also makes the route block's `&this->lockLabel` assignment use `sl`
instead of `r9`, adding a `mov r9,sl` / `mov sl,r9` pair. Using a local
`String dist = calcDistance(...); this->lockLabel = dist;` instead of assigning
`calcDistance` directly to `this->lockLabel` cancels that size growth and lands a
**size-exact 9,908 B** body:

| edit (on the 7,074 body) | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| `volatile int autoPilotTarget` | 8,843 | 2,001 | **9,912** | 3,521 |
| + `String dist = calcDistance(...); this->lockLabel = dist;` | **6,721** | 2,018 | **9,908** | 3,519 |

This body is the first size-exact member found with the original's **pre-loop and planet-
loop skeleton**: `12e0f8 mov sl,r0` (campaign), `12e0d8 mov.w fp,#0` (zero), no early
alienOrbit spill, and `12e3c6 mov sl,r0` / `12e3c8 mov.w fp,#0` / `12e3d4 ldr.w
r0,[r4,#0x140]` at the original addresses. It is still raw-worse than the base because
the planet loop body is ~2x the original's length (the `continue` branch is `beq.w` at
`12e3f8`, target `12e5d0`, distance 472 B, vs the original `beq.n` target `12e4ec`,
distance 244 B), so the tail is still misaligned. The extra length is the `sl` status-
pointer CSE in the campaign condition (`ldr.w sl,[sp,#0x9c]` ... `ldr.w sl,[sp,#0x68]`),
which the original avoids by reloading `Globals::status` from `[sp,#0x9c]` into `r0` on
each use. Attempts to prevent that CSE (operand/parenthesis/cast/reorder variants,
`const`/`unsigned`/`volatile` `stations`, swapping `campaignMission`/`freelance`, moving
`freelance` later) either leave the body byte-identical or are size-fatal.

## 7. Conclusion

The `campaign`-in-`sl` skeleton is now reproducible at 9,908 B (the 7,074 body), and its
planet loop is instruction-for-instruction the original's — but it pays a 4-byte address
shift because `alienOrbit` is evicted from `r8` by the out-of-line landmark block's
`r9` usage. The `volatile autoPilotTarget` + String-temp body (section 6) removes that
shift and matches the original's pre-loop and planet-loop skeleton at the original
addresses, but its planet loop body is still ~2x the original's length (the `sl`
status-pointer CSE), so the tail remains misaligned. The base's 3,541-instruction stream
remains a stronger local optimum for the same-address byte metric than any size-exact
campaign-in-`sl` body found so far. No size-exact, `.rel.plt`-identical raw win exists in
the assigned cells on this base. The best campaign-in-`sl` body found is 6,721
(section 6), still 412 raw bytes worse than the base.

## 8. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6309 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13315/`: `Radar.base.cpp` (6,309 base),
`Radar.bool_bsor_marked.cpp` (7,080 body), `try.py` (build+measure+frame/campaign/zero
probe), `bytediff.py`/`byteagg.py` (same-address byte-run reports), `v20.py`..`v32.py`
(the sweeps above), `orig_radar.dis`/`ours_base.dis`/`bool_bsor_marked.dis`.

## Harvest on wave-189 master (`eacfcc9ec`)

Note-only: raw stays 6,309 / 1,858 / 9,908.  On current master `.text` is
73,999 and total 74,658.  The new size-exact 6,721 body (`volatile int
autoPilotTarget` + `String dist = calcDistance(...); this->lockLabel = dist;`)
is recorded here for the next Radar pass; its pre-loop/planet-loop skeleton
matches the original but the planet loop body is still ~2x long due to the
`sl` status-pointer CSE.
