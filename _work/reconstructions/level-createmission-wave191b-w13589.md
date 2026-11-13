# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13589

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13589`, base master `b060a2c04`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13560.md` (raid `rnd`/`globals`
volatile refs, 2628 -> 2626), `...w13567.md` (tail `[sp,#100]` live-range
diagnosis) and `...w13545.md` (case-1 hoist).

```text
                                     size            raw    aligned  insn
base b060a2c04                       17172/17172     2626    845     6206/6197
this pass (no new win)               17172/17172     2626    845     6206/6197
```

No size-exact raw win was found; the tree is left at `b060a2c04`
(`git status --porcelain` empty).

## 1. The residual is two *independent* mechanisms

| region | diff bytes | mechanism |
|---|---:|---|
| `0xaffd8..0xb1500` | 2 | branch-target indices (clean) |
| `0xb1500..0xb178e` (case-1 `enemyRace` reload) | 339 | **post-RA MachineLICM hoist** |
| `0xb178e..0xb1930` | 4 | branch-target indices (shift fallout) |
| `0xb1930..0xb1980` (case-2 loot) | 59 | source order (CSGR is best, w13545 §6) |
| `0xb1980..0xb322e` | ~24 | stack-slot offsets / branch indices |
| `0xb322e..0xb42ec` (alienGuns/fortress/raid tail) | 2,204 | **greedy-RA live-range** |

The two big pieces are *different* passes, which is why no single source edit
moves both.

## 2. Case-1 hoist: it is post-RA MachineLICM (confirmed)

A fresh `-mllvm -print-after-all` dump of the base (`scratch/w13589/mir_base.txt`,
886 MB; the createMission dumps are at lines 6816618..7311757) shows the pass
sequence

```
Greedy RA -> VirtRegRewriter -> StackSlotColoring -> MachineCopyPropagation
  -> Machine Loop Invariant Code Motion  (line 7235773)
  -> PostRA MachineSink -> ShrinkWrapping -> PEI
```

and the hoist happens exactly in **Machine Loop Invariant Code Motion**:

```text
bb.371 (%ir-block.4017):            ; loop preheader
  ...
  renamable $r5 = t2LDRi12 %stack.75, 0   ; <-- added by LICM
bb.372: ... t2CMPrr $r6, $r0 ...
bb.376 (%ir-block.4035):            ; loop body
  renamable $r0 = t2LDRi12 $r8, 0   ; Globals::globals
  $r1 = COPY renamable $r5          ; enemyRace
  tBL getRandomEnemyFighter
  ...
  $r1 = COPY killed renamable $r5   ; enemyRace
```

`%stack.75` is frame index `fi#75`, a **spill slot** (reused for
`stationRace`, `enemyRace`, the case-183 loop index `k`, ...). The load is a
16-bit `ldr r5,[sp,#116]` (2 bytes). Hoisting it into the preheader makes the
body 2 bytes shorter and removes the 2-byte literal-pool `nop` the original
keeps at `0xb159a`, so the whole `0xb1608..0xb178e` region shifts by -2. The
shift is compensated by the case-2 prologue, so the function size stays exact.

### Exact LLVM-7 condition (from `llvm/lib/CodeGen/MachineLICM.cpp`)

`ProcessMI` makes the load a candidate because
`TII->isLoadFromStackSlot(MI,FI) && MFI->isSpillSlotObjectIndex(FI)`.
`HoistRegionPostRA` then hoists it unless one of:

1. `StoredFIs.count(FI)` — some instruction **in the loop** stores to `fi#75`;
2. `PhysRegClobbers.test(Def)` — `r5` is defined **more than once** in the loop;
3. `TermRegs.test(Def)` — the preheader terminator reads/clobbers `r5`;
4. the load's FI is **not** a spill-slot object (i.e. a front-end stack object;
   then `isSpillSlotObjectIndex` is false and the `!HasNonInvariantUse` branch
   is dead because an FI operand sets `HasNonInvariantUse = true`).

The load has no register operands, so the final `Safe` loop always passes.

## 3. Case-1 hoist experiments (all size 17,172 unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 2,626 | 845 | 17,172 |
| `volatile int enemyRace` (direct) | 16,402 | 2,309 | **17,232** |
| `volatile int enemyRace` + `int race = enemyRace;` in loop | 16,351 | 2,359 | **17,224** |
| `int *enemyRacep = &enemyRace;` (SROA promotes) | 2,626 | 845 | 17,172 |
| `int *volatile enemyRacep = &enemyRace;` | 16,596 | 2,364 | **17,248** |
| `int er = enemyRace;` copy, split uses | 2,626 | 845 | 17,172 |
| `int er = enemyRace; int er2 = er;` | 2,626 | 845 | 17,172 |
| `(void) enemyRace;` / `int idx = i;` | 2,626 | 845 | 17,172 |
| `i != attackerCount` | 2,626 | 847 | 17,172 |
| `i += 1` / `++i` / `unsigned i` | 2,626 | 845/847 | 17,172 |
| `(int) enemyRace` / `enemyRace + 0` / `enemyRace \| 0` | 2,626 | 845 | 17,172 |
| `int race = enemyRace;` *inside* loop (w13545) | 9,371 | 1,606 | **17,164** |
| `do`/`while` (w13545) | — | — | **17,180** |

Key negative: `volatile int enemyRace` **does** remove the hoist but the frame
stays `sub sp,#400` (496 B) — the front-end object is stack-coloured into the
same reused slot. The +52..60 is therefore extra load/store instructions, not
frame growth. So the original's `enemyRace` is either (a) a spill slot that is
*stored to inside the loop*, or (b) a front-end object reached with a cheaper
access pattern than `volatile`.

## 4. Tail: greedy-RA spill placement, *not* MachineLICM

The extra `ldr r4,[sp,#100]` at `0xb3234`, `0xb3398`, `0xb34e6` is already
present in the post-RA MachineCopyPropagation dump — the alienGuns region is
**byte-identical before and after** MachineLICM. So it is a greedy-RA
live-range/split decision, not a hoist.

`[sp,#100]` = `%stack.74` = `&Globals::rnd` (stored at `0xb01c2`; the stack
slot is reused for `wingRoute` and `globals` later). RA keeps `&Globals::rnd`
in `r4` from `0xb3234` to the raid-block use at `0xb378c`, reloading it after
each clobber. That steals `r4`, so `this` lands in `r8` (orig `r4`) and
`alienGuns` in `r6`/spill (orig `sl`); the whole `0xb322e..0xb42ec` tail is
register-renamed.

## 5. Tail experiments (all 2,626 / 845 / 17,172 unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `if (raid != 0)` / `if (raid == 1)` / `if (raid == true)` | 2,626 | 845 | 17,172 |
| `const int guns` / `fortressGuns + alienGuns` operand swap | 2,626 | 845 | 17,172 |
| `int hard` / `const bool hard` / `const int raidType` | 2,626 | 845 | 17,172 |
| `raidType` before `hard` | 4,201 | 848 | **17,160** |
| `if (alienGuns > 0)` / `if (pirateFortress != 0)` | 2,626 | 845 | 17,172 |
| `unsigned k` / `++i` in raid & pirates loops | 2,626 | 845 | 17,172 |
| `int stationPirates` (instead of `unsigned`) | 2,626 | 845 | 17,172 |
| `Status *volatile &st` in raid block | 2,626 | 845 | 17,172 |
| `Status *&st` (non-volatile) over the tail | 2,626 | 845 | 17,172 |
| `Status *volatile &st` over the tail | 2,633 | 848 | 17,172 |
| `GameText *volatile &gt` over the tail | 2,678 | 860 | 17,172 |
| `Globals *volatile &gl` over the tail | 2,658 | 860 | 17,172 |
| `AERandom *volatile &rd` over the tail | 15,724 | 1,302 | **17,124** |
| `volatile bool *optv = Globals::options;` in tail | 2,626 | 845 | 17,172 |
| `volatile bool initStream = Level::initStreamOutPosition;` | 3,767 | 1,085 | **17,208** |
| `raid`/`alienWave`/`bigAlienWave`/`pirateFortress` as `int` | 2,855–5,049 | — | 17,168–17,172 |
| `int guns` hoisted before `if (raid)` (shared) | 3,560 | 916 | **17,168** |
| `alien_gun_after` (load `gun` after `createStaticObject`) | 2,627 | **841** | 17,172 |
| `alien_gun_after` + any of {`base_const`, `status_tail_nv`, `globals_first`, `hard_int`, `raidtype_const`} | 2,627 | 841 | 17,172 |
| `fortress_gun_after` | 2,632 | 842 | 17,172 |
| `alien_base_first` (base before rotation) | 2,701 | 867 | 17,172 |
| `alien_rotation_outside` (rotation/v out of the if) | 16,674 | 2,675 | **17,444** |
| `alien_loop_unsigned` | 4,380 | 883 | **17,168** |
| `alien_base_const` / `alien_k_unsigned` / `alien_k_top` | 2,626 | 845 | 17,172 |
| plain ternary `bigAlienWave ? ...` (no crutch) | 4,380 | 883 | **17,168** |

All 2-combinations and 3-combinations of the neutral tail variants
(`gun_after`, `base_const`, `status_tail_nv`, `globals_first`, `hard_int`,
`raidtype_const`) measure 2,627/841, i.e. no interaction recovers the raw.

## 6. Scratch / reproduce

`/home/debian/GalaxyOnFire2/scratch/w13589/`:
`measure.sh` (build + `fn_diff` line), `Level.base.cpp` (pristine TU),
`taildiff.py` (region diff), `sweep25.py`..`sweep33.py` (this pass),
`mir_base.txt` (fresh `-print-after-all` of the base; createMission at
6816618..7311757), `mcp_before.txt` / `mlicm_after.txt` (the two dumps around
the hoist), `MachineLICM.cpp` (LLVM 7 source used for the exact conditions).

```sh
cd /home/debian/GalaxyOnFire2/wt/w13589
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

## 7. Next steps

1. **Case-1 hoist (worth ~339 raw bytes).** Block it without the +52 volatile
   cost. The most promising untried shapes: make `fi#75` store-bearing inside
   the loop (condition 1) — e.g. a second local that greedy-RA colours to the
   same slot and stores each iteration — or make `r5` defined twice in the loop
   (condition 2). The original's `ldr r5,[sp,#116]` is the 16-bit encoding, so
   its object is 4-byte aligned at offset <= 1020, consistent with either a
   spill slot or a stack-coloured front-end object.
2. **Tail (worth ~2,204 raw bytes).** Needs a greedy-RA colouring change so
   `&Globals::rnd` is *not* kept in `r4` across `0xb3234..0xb378c`. Every
   single-statement edit tried so far is neutral; the value's live range is
   created by RA's reload placement, not by the source order.
