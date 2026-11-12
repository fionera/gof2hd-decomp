# Radar::draw wave 189 (w13335) — planet-loop block placement is the real residual;
# the `sl` status CSE is a red herring. No size-exact raw win.

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left
at the base. This pass commits only this note. Worktree `wt/w13335`, branch
`wave189-w13335`, base `master` `8225c71a8`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13315.md` (the 6,721 body) and
`radar-draw-wave189-w13308.md` (campaign-in-`sl` family). This note is self-contained:
every number below was measured in this worktree on the 6,309 base or on the checked-in
6,721 body (`scratch/w13335/Radar.6721.cpp`, identical to the one recorded by w13315).

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
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. Where the base's 6,309 diff actually lives

Byte-diff heat (256-byte blocks, `scratch/w13335/heat.py`):

```
12e050    9   12e150    1   12e250    1   12e350  148   12e450  249   12e550  247
12e650  237   12e750  239   12e850  232   12e950  244   12ea50  247   12eb50  252
12ec50  246   12ed50  241   12ee50  246   12ef50  177   12f050  136   12f150  229
12f250  243   12f350  228   12f450  189   12f550  219   12f650  146   12f750   50
12f850    3   12f950    1   12fa50    6   12fb50    5   12fc50  228   12fd50  235
12fe50  140   12ff50  251   130050   51   130150    2   130250  184   130350  230
130450  245   130550  184   130650   96/180
```

The base is **byte-identical (delta 0) from `12e050` to `12e3cc`** and again from
`12f850` to `12fc54` (an instruction-shift trace, `scratch/w13335/x/shift.py`, shows
`delta=+0` over both runs). The residual is almost entirely *block-placement shifts* in
the planet loop and the tail, not wrong instructions.

## 2. The planet loop: what the original does and what we do

Original (`12e3c6`):

```
12e3c6  mov  sl,r0            ; sl = stations
12e3c8  mov.w fp,#0           ; fp = i
12e3cc  movs r0,#0
12e3ce  str.w sl,[sp,#0x74]   ; cold copy of stations
12e3d2  str r0,[sp,#0x98]     ; planetHit = 0
12e3d4  ldr.w r0,[r4,#0x140]  ; loop head
...
12e3f8  beq.n 12e4ec          ; continue
...
12e4ec  add.w fp,fp,#1        ; latch, placed BEFORE the crosshair body
12e4f0  b.n 12e3d4
12e4f2  ...crosshair body...
```

Base (`12e3c6`):

```
12e3c6  str r0,[sp,#0x84]     ; stations spilled (2 B) instead of mov+str.w (6 B)
12e3c8  mov.w sl,#0           ; i reuses the shared zero's register
12e3cc  movs r0,#0
12e3ce  str r0,[sp,#0x90]
12e3d0  ldr.w r0,[r4,#0x140]  ; loop head: -4 vs original
...
12e3f4  beq.w 12e5c4          ; continue: latch is AFTER the crosshair body
...
12e5c4  add.w sl,sl,#1        ; latch
12e5c8  b.n 12e3d0
12e5ca  ...campaign-true block...
```

Two independent facts:

1. **The base loop head is `-4`** because the base spills `stations` directly
   (`str r0,[sp,#0x84]`, 2 B) instead of keeping it in `sl` and storing a cold copy
   (`mov sl,r0; str.w sl,[sp,#0x74]`, 6 B). This is the campaign-in-`sl`/zero-in-`fp`
   register skeleton: with `campaign` in `sl` the shared zero moves to `fp`, and `i`
   coalesces with `fp` instead of `sl`, freeing `sl` for `stations`.
2. **The base latch is after the crosshair body** (`12e5c4`), the original's is before
   it (`12e4ec`). This is a `MachineBlockPlacement` decision: the original treats
   `if (!otherLock && !planetHit && !turretMode)` as unlikely (crosshair body is the
   *taken* branch, latch is fallthrough); the base treats it as likely.
   Consequences: every `continue` becomes `beq.w` (4 B) instead of `beq.n` (2 B), and
   the campaign range check becomes `blt.w`/`bge.w` (4 B) instead of `blt.n` (2 B).

The 6,721 body (w13315) fixes fact 1 (loop head lands at the original `12e3d4`) but not
fact 2 (its latch is `12e5d0`, still after the crosshair body). That is why it is
raw-worse than the base despite matching the pre-loop/planet-loop instruction sequence.

## 3. The `sl` status-pointer CSE is a red herring

w13315's note blames the "`sl` status-pointer CSE" for the ~2x planet-loop body. It is
not the cause. Both bodies spend the **same two instructions** on `Globals::status`:

```
base   12e45a  ldr.w fp,[sp,#0x9c]  ; 12e45e ldr.w r0,[fp]   (reused at 12e46c)
6721   12e45e  ldr.w sl,[sp,#0x9c]  ; 12e462 ldr.w r0,[sl]   (reused at 12e470)
orig   12e45c  ldr   r0,[sp,#0x9c]  ; 12e45e ldr   r0,[r0]   (reloaded at 12e468)
```

The original reloads the address from `[sp,#0x9c]` twice (2+2 instrs); ours loads it
once and reuses it (2 instrs). Instruction count is identical. The loop body is long
because the latch and the campaign-true block are placed out of line, not because of
the CSE.

## 4. New cells measured this pass (all on the 6,721 body unless noted)

`ol0` = `__builtin_expect(!otherLock && !planetHit && !turretMode, 0)` on the crosshair
guard. `ol1` = the same with `1`. `noexp` = remove the `__builtin_expect(...,0)` from
the campaign condition. `first0` = `__builtin_expect(campaignMission != nullptr, 0)`
only. `inner0/1` = expect on the inner `(cur < 148 || cur >= 152)` only.

| edit | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| 6,721 body (reference) | 6,721 | 2,018 | 9,908 | 3,519 |
| `ol0` | 9,260 | **2,983** | 9,916 | 3,538 |
| `ol1` | 7,519 | 2,039 | 9,916 | 3,526 |
| `noexp` | 9,139 | 2,003 | 9,896 | 3,515 |
| `noexp` + `ol0` | 9,305 | 2,960 | 9,900 | 3,527 |
| `noexp` + `ol1` | 9,469 | 2,029 | 9,868 | 3,512 |
| nested `if (visible) { int cur=...; if (range) }` | 9,052 | 2,007 | 9,940 | 3,516 |
| `first0` | 8,704 | 2,925 | 9,888 | 3,522 |
| `inner0` | 6,721 | 2,018 | 9,908 | 3,519 |
| `inner1` | 8,905 | 2,001 | 9,900 | 3,515 |
| `int alienOrbit` | 8,526 | 2,099 | 9,904 | 3,518 |
| `ol0` + `int marked` | 9,237 | 2,960 | 9,924 | 3,544 |
| `ol0` + `int alienOrbit` | 9,294 | 2,963 | 9,916 | 3,539 |
| `ol0` + `bool missionBlocksPlanets` | 9,293 | 2,986 | 9,920 | 3,541 |
| `ol0` + `int gasHit` | 8,451 | 2,982 | 9,912 | 3,536 |
| `ol0` + `int marked` + `int alienOrbit` | 9,219 | 2,976 | 9,924 | 3,545 |

On the **base**:

| edit | raw | aligned | size |
|---|---:|---:|---:|
| `noexp` | 9,268 | 1,982 | 9,972 |
| `exp1` | 9,283 | 2,206 | 9,956 |
| `ol0` | 9,215 | 2,666 | 9,948 |
| `ol1` | 9,170 | 2,578 | 9,872 |
| `noexp`+`ol1` | 9,114 | 2,570 | 9,876 |
| `exp1`+`ol1` | 9,317 | 2,642 | 9,928 |
| nested `if` + local `cur` | 8,763 | 1,958 | 10,000 |
| `first0` | 9,235 | 2,806 | 9,948 |
| `inner0` | **6,309** | **1,858** | **9,908** |
| `inner1` | 8,675 | 2,096 | 9,960 |

**The `ol0` result is the interesting one:** it moves the latch ahead of the crosshair
body exactly like the original (`12e4ca: add.w sl,sl,#1; b 12e3e6`, crosshair body at
`12e506`) and lifts `aligned` from 1,858 to **2,983** — the highest alignment seen for
this function. But it grows the function to 9,916 B and shifts the early
`Globals::Canvas->SetColor` block by `-2` (it hoists one `ldr`), so `raw` is 9,260.
Fixing the `-2` shift and the `+8` size would be the route to a real win; every
size-compensation cell tried here (`int marked`, `int gasHit`, `int alienOrbit`,
`bool missionBlocksPlanets`) either overshoots 9,908 or leaves the `-2` shift in place.

## 5. Single-type sweep on the base (all cells measured fresh; none beat 6,309)

| edit | raw | aligned | size |
|---|---:|---:|---:|
| `bool alienOrbit` | 6,317 | 1,830 | 9,908 |
| `int marked` | 8,424 | 1,836 | 9,912 |
| `int turretMode` | 9,238 | 2,694 | 9,856 |
| `int gasHit` | 7,956 | 1,863 | 9,904 |
| `int alwaysEnemySeen` | 8,997 | 2,761 | 9,920 |
| `int planetHit` | 9,236 | 2,089 | 9,948 |
| `int landmarkHit` | 8,710 | 1,882 | 9,936 |
| `int enemyHit` | 8,616 | 2,052 | 9,936 |
| `bool missionBlocksPlanets` | 6,446 | 1,862 | 9,912 |
| `bool notTurretMode` | 9,547 | 1,903 | 9,868 |
| `bool notOtherLock` | 9,513 | 1,932 | 9,876 |
| `bool inCenter` | 7,557 | 1,884 | 9,912 |
| `int asteroidHit` | 7,633 | 2,162 | 9,924 |
| `int alwaysEnemy` | 8,582 | 1,890 | 9,920 |
| `int bossSeen` | 9,136 | 2,775 | 9,932 |
| `bool otherLock` | 9,247 | 2,659 | 9,856 |

Loop-variable type (`int`/`unsigned`/`size_t`), `stations` `const`, and
`stationsCopy`/reference aliases are all byte-identical to the base (6,309). `stations
volatile` gives aligned 3,066 but size 10,240. `int *stationData = stations->data();`
first-use-only is 8,387/9,912; all-uses is 9,266/9,876. `n = size()` hoist is
7,011/9,908. `lockedEnemy` reorder (`isDead()` first) is 6,310/9,908; local
`KIPlayer *le` is 6,783/9,908. `wreck`/`continue` condition reorders are identical.

## 6. Conclusion

No size-exact raw win exists in the cells tried here. The base's 6,309 is a strong
local optimum because it is byte-identical over the first `0x37c` bytes and again over
`12f850`-`12fc54`; every campaign-in-`sl` body trades those early bytes for a loop
skeleton that is still misaligned by the latch placement.

The concrete next lever (not landed): make `ol0`-like latch placement (`aligned`
2,983) size-exact by removing the `-2` early shift and the `+8` growth. The `-2` shift
is the hoisted `ldr` in the `Globals::Canvas->SetColor` sequence at `12e090`; the `+8`
is the extra out-of-line crosshair spill code. Both are register-allocation effects of
the `!prof` branch weights, so a source formulation that produces the same latch
placement without perturbing the early schedule is the remaining avenue.

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6309 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13335/x/`: `try.py` (variant harness),
`heat2.py` (byte-diff heat), `shift.py` (instruction-shift trace), `dump.py`
(build+dump), `v_ol0.cpp` / `v_ol1.cpp` / `v_noexp.cpp` / `v_nest.cpp` /
`v_boolal.cpp` / `v_6721aint.cpp` (variants), `o*.txt` / `u*.txt` (side-by-side dumps).
