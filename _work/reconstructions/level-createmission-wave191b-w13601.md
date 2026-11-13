# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13601

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13601`, base master `9ef7c2265`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13560.md` (raid-block volatiles,
2628→2626), `...w13567.md` (tail `this`/`rnd` register split) and
`...w13578.md` (full declaration/statement sweep).

```text
                                     size            raw    aligned  insn
base 9ef7c2265                       17172/17172     2626    845     6206/6197
this pass (no new win)               17172/17172     2626    845     6206/6197
```

No size-exact raw win was found; the tree is left at `9ef7c2265`
(`git status --porcelain` empty).

## 1. NEW: the case-1 hoist is a greedy-RA SpillPlacement decision, and the
##    loop body is byte-identical modulo the 2-byte load

The residual region `0xb1608..0xb178e` is **not** a structural difference.
Byte-compare of the loop body:

```text
orig  b1608: 9d1d        ldr   r5, [sp, #116]   ; enemyRace
      b160a: f8d8 0000   ldr.w r0, [r8]         ; globals
      ...                                        (identical from here)
ours  b1608: f8d8 0000   ldr.w r0, [r8]
      ...
```

The preheader (orig `0xb1588`, ours `0xb1588`) is the same except that ours
has the extra 2-byte `ldr r5,[sp,#116]` and therefore no `nop` padding:

```text
orig  0xb1588..0xb159a   ldr/movs/mov.w/movs/add/ldr.w  (16 B)
      0xb1598: e054      b.n 0xb1644
      0xb159a: bf00      nop
      0xb159c: constant pool (108 B, identical in both)
      0xb1608: loop body
ours  0xb1588..0xb159a   same + ldr r5,[sp,#116]  (18 B)
      0xb159a: e052      b.n 0xb1642
      0xb159c: constant pool (identical, same address)
      0xb1608: loop body (2 B shorter)
```

So the constant pool is at the **same address** in both; only the loop body
and everything after it shift by −2.  Fixing the reload placement is a pure
`raw −341` win with **no size change** (the preheader loses 2 B and the body
gains 2 B; the `nop` reappears).

MIR evidence (`-mllvm -print-machineinstrs`, `# After Greedy Register
Allocator`, function dump at line 769299 of `mir.txt`): the reload is already
in the preheader block there (`%4968:gpr = t2LDRi12 %stack.75`), i.e. the
decision is made by `SpillPlacement` inside greedy RA, not by MachineLICM.
The loop body block has a 96.88 % back-edge probability (31/32), so the
allocator keeps the spilled `enemyRace` in callee-saved `r5` across the loop.
The original chose "spill" (reload in the body) instead.

## 2. NEW: `%698` (`&Globals::rnd`) is one CSE'd GOT load live across the
##    whole function

Pre-RA MIR (`# After Two-Address instruction pass`, line 717665):

```text
%697:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals3rndE
%698:gpr  = t2LDRi12 killed %697:tgpr, 0  :: (load 4 from got)
```

`%698` is defined once, just before the `enemyRace` computation (source line
2146) and is used by every `Globals::rnd` access in the function (45 mentions
in the pre-RA dump).  It is therefore live across the whole function and the
greedy allocator keeps it in `r4` across the alienGuns/pirateFortress/raid
tail.  The original keeps it in memory and reloads `[sp,#100]` only at the
raid block (`0xb3788`), so `this` gets `r4` there (ours: `this` in `r8`,
`gun` in `r4`).  Both residuals (case-1 hoist and the tail) are the same
`SpillPlacement` "keep vs spill" decision, i.e. the original's IR must differ
in a way that lowers the spill weight of these two values in those blocks.

## 3. Measurements (all size 17,172 unless noted; raw/aligned)

Starting from the base tree (2,626 / 845):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,626** | **845** | 17,172 |
| `if (alienWave)` instead of `if (alienGuns != 0)` | 2,626 | 845 | 17,172 |
| `if (alienGuns)` / `> 0` / `!= 0 && true` / `!!alienGuns` | 2,626 | 845 | 17,172 |
| raid `Status *volatile &status` (4 positions, all uses) | 2,626 | 845 | 17,172 |
| raid `status` ref for `hardCoreMode`+`inPirateLootOrbit` only | 2,626 | 845 | 17,172 |
| `status` volatile ref only around the loot `if` | 2,626 | 845 | 17,172 |
| `rnd` direct in the `raidRace==8` if-condition (ref kept) | 2,626 | 845 | 17,172 |
| `rnd` direct in the loot body (ref kept) | 2,627 | 845 | 17,172 |
| no `rnd` ref (globals ref kept) | 2,627 | 845 | 17,172 |
| no `globals` ref | 2,733 | 837 | 17,172 |
| no refs at all | 2,734 | 837 | 17,172 |
| `AERandom *volatile const &rnd` | 2,626 | 845 | 17,172 |
| `Globals *volatile const &globals` | 2,626 | 845 | 17,172 |
| refs before `if (raid)` (function scope) | 2,626 | 845 | 17,172 |
| `rnd` ref at function scope, `globals` ref inside | 2,626 | 845 | 17,172 |
| `int guns` first in the raid prologue | 2,626 | 845 | 17,172 |
| decls after `int guns` | 2,626 | 845 | 17,172 |
| `rnd` ref after `hard` / after `guns` | 2,626 | 845 | 17,172 |
| `globals` ref after `raidType` | 2,627 | 844 | 17,172 |
| `bool hard` before `raidType` | 4,201 | 848 | 17,160 |
| raid `int guns` hoisted before `if (raid)` (inner removed) | 3,560 | 916 | 17,168 |
| same + 3-term crutch | 3,866 | 918 | 17,168 |
| same + 1-term crutch | 4,066 | 897 | 17,164 |
| case-1 `const int enemyRace` / `unsigned` / `long` | 2,626 | 845 | 17,172 |
| case-1 `if/else` instead of ternary | 2,626 | 845 | 17,172 |
| case-1 `enemyRace = 8; if (rnd>=75) ...` | 2,644 | 850 | 17,172 |
| case-1 reversed ternary / reversed if-else | 2,644 | 850 | 17,172 |
| case-1 `nextInt(100) > 74` / `<= 74` | 2,628 / 2,626 | 847 / 845 | 17,172 |
| case-1 `do {} while` | 10,750 | 1,045 | 17,180 |
| case-1 `while` / manual `i++` / reversed condition | 2,626 | 845 | 17,172 |
| case-1 `unsigned attackerCount` | 2,627 | 845 | 17,172 |
| case-1 `int race = *(&enemyRace)` in body | 2,626 | 845 | 17,172 |
| case-1 `const int *racep = &enemyRace` before loop | 2,626 | 845 | 17,172 |
| case-1 `int race = *(volatile int *)&enemyRace` in body | 16,406 | 2,369 | **17,232** (insn 6197/6197!) |
| case-1 loop bound `+ (enemyRace < 0)` etc. (5 forms) | 15,428–15,525 | 1,161–1,163 | 17,188–17,196 |
| case-1 `Objective(7, attackerCount + (enemyRace>>31))` | 16,091 | 1,338 | 17,204 |
| case-2 loot `GCSR` no local | 2,631 | 841 | 17,172 |
| case-2 loot `GCSR` with `KIPlayer *ship` local | build fail (`crateCaptured` not in `KIPlayer`) | — | — |
| alienGuns plain ternary + elemwise `Vector a/b` | 4,035 | 904 | 17,176 |
| alienGuns crutch + elemwise `Vector a/b` | 3,980 | 858 | 17,168 |
| alienGuns crutch + elemwise first only | 2,953 | 850 | 17,172 |
| alienGuns crutch + named `Vector a/b` | 4,144 | 885 | 17,184 |

`-mllvm -regalloc=basic`, `-regalloc=pbqp`, `-stress-regalloc=9/10/11` all
measure 2,626 / 845 / 17,172 (the flag does not move this function).

## 4. Next steps

1. The two residuals are one problem: greedy-RA `SpillPlacement` keeps
   `enemyRace` (`%stack.75`) and `&Globals::rnd` (`%698`) in callee-saved
   registers across loops where the original spills them.  A source change
   that lowers the spill weight of either value in those blocks (without
   changing instruction count) is the lever.  All declaration/scope/volatile
   spellings tried so far are neutral.
2. The case-1 fix is worth `raw −341` at zero size cost if the reload can be
   moved into the loop body.  The `(volatile int *)&enemyRace` read does move
   it and restores the original instruction count (6197/6197) but costs
   +60 B elsewhere, so it needs a 60-byte compensation elsewhere — not yet
   found.
3. The `(bigAlienWave + (i < 0) + (i < 0))` crutch is still required for
   size 17,172; it compensates the `ldrd`/`strd` formation in the alienGuns
   `Vector{...}` construction (plain ternary = 17,168).

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13601
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2626 aligned=845 size=17172/17172 insn=6206/6197`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13601/`
(`batch47.py` … `batch58.py`, `flagsweep.sh`, `sbs2.py`, `grep.py`,
`normdiff.py`, `regions.py`, `mir.txt`).
