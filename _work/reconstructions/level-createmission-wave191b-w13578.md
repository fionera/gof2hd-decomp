# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13578

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13578`, base master `5b422acf1`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13560.md` (raid-block volatiles,
2628→2626), `...w13567.md` (tail `this`/`rnd` register split) and
`...w13545.md` (case-1 hoist).

```text
                                     size            raw    aligned  insn
base 5b422acf1                       17172/17172     2626    845     6206/6197
this pass (no new win)               17172/17172     2626    845     6206/6197
```

No size-exact raw win was found; the tree is left at `5b422acf1`
(`git status --porcelain` empty, `check` RATCHET PASS).

## 1. What the residual actually is

The whole function is byte-identical up to `0xaffd8+0x15xx` except branch
targets; the residual is three independent problems:

| region | raw bytes | cause |
|---|---:|---|
| `0xb158c..0xb1608` (case-1 hoist) | ~341 | pre-RA spiller hoists `ldr r5,[sp,#116]` (`enemyRace`) out of the case-1 attacker loop; the assembler then drops a `nop` and the whole `0xb1608..0xb178e` region shifts `-2 B`, so every branch target into it differs |
| `0xb1930..0xb1966` (case-2 loot) | ~83 | `this->enemies` reloaded per statement; store order `CSGR` vs binary `GCSR` (w13567 §7) |
| `0xb322e..0xb42ec` (alienGuns/pirateFortress/raid tail) | ~2204 | `this` in `r8` (binary `r4`) and the `&Globals::rnd` spill value kept in `r4` across both loops |

## 2. NEW: the dead `&Globals::rnd` reloads are pre-RA spiller hoists

Ours loads `&Globals::rnd` from `[sp,#100]` at `0xb3234`, `0xb3398` and
`0xb34e6` (three *dead* loads; `r4` is overwritten before each is read) and
uses the surviving `r4` at `0xb378c`.  The binary reloads `[sp,#100]` once,
inside the raid block at `0xb3788`.

w13545 §2 attributed this to **post-RA MachineLICM**.  That is wrong for
this function: compiling `Level.cpp` with
`-mllvm -disable-postra-machine-licm` produces a **byte-identical object**
(md5 equal to the normal compile), and `-mllvm -disable-postra-machine-sink`
does too.  The reload is already present in the pre-RA MIR dump
(`-mllvm -print-machineinstrs`, last `NoPHIs, TracksLiveness` dump):
`%5513 = t2LDRi12 %stack.76` sits at `bb.254` (the block after the case-2
loop) while its only use is `bb.260` (the raid block) — i.e. the greedy
spiller's `SpillPlacement` hoisted the reload out of the alienGuns and
pirateFortress loops.  `-mllvm -disable-spill-hoist` is also neutral
(identical object), so this is `SpillPlacement`, not the
`InlineSpiller::hoistSpill` optimisation.

Consequence: the "kill the dead load" lever cannot be reached with a
post-RA flag; it needs the source to make the `&Globals::rnd` value not be
live across the two loops (or not be spilled at all).

## 3. The alienGuns block is 2 instructions *shorter* than the binary

The binary's `Vector{t[k][3],t[k][4],t[k][5]}` / `Vector{t[k][0],t[k][1],t[k][2]}`
construction (`0xb32d4..0xb32ee`) loads `t[k][1..5]` with five separate
`ldr`/`str` pairs.  Ours forms `ldrd`/`strd` pairs (`0xb32e2..0xb32ec`) and
is **2 instructions / ~4 bytes shorter**.  That is exactly the size that
the `(bigAlienWave + (i < 0) + (i < 0))` crutch adds back; the plain
ternary is `17,168 B`.  So the crutch compensates for the `ldrd`/`strd`
formation, not for a tail problem.

Trying to force the binary's element-wise shape from the source
(`Vector tv; tv.x = ...; tv.y = ...; tv.z = ...;`, one reused temp, field
assignment order `z,y,x`, `const float *row = t[k]`) always changes the
size (17,164–17,196) and raw (3,900–4,400); none is a win.

## 4. Measurements (all size 17,172 unless noted; raw/aligned)

Starting from the pass-2 tree (raw 2,626 / aligned 845):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,626** | **845** | 17,172 |
| raid `Status *volatile &status` (3 uses) | 2,626 | 845 | 17,172 |
| `status` volatile ref first, before `rnd` | 2,626 | 845 | 17,172 |
| `bool hard` after `int guns` | 2,626 | 845 | 17,172 |
| `raidType, guns, hard` order | 2,626 | 845 | 17,172 |
| `globals` volatile ref moved after `raidType` | 2,627 | 844 | 17,172 |
| `AERandom *&rnd` (non-volatile ref) | 2,627 | 845 | 17,172 |
| `AERandom *volatile *rndAddr = &Globals::rnd` in raid, `(*rndAddr)->` | 2,626 | 845 | 17,172 |
| `AERandom **volatile rndAddr` in raid | 4,145 | 1,294 | 17,184 |
| `AERandom *volatile rnd = Globals::rnd` in raid | 4,233 | 1,276 | 17,168 |
| `AERandom *rndArr[1] = {Globals::rnd}` in raid | 4,204 | 841 | 17,152 |
| `(*(AERandom *volatile *)((char *)&Globals::rnd))` | 2,626 | 845 | 17,172 |
| tail-scope `rnd` volatile ref from alienGuns / from raid | 2,626 | 858 | 17,172 |
| tail-scope `status` volatile ref from alienGuns | 2,626 | 845 | 17,172 |
| tail-scope `globals` volatile ref from alienGuns | 2,626 | 845 | 17,172 |
| tail-scope `gameText` volatile ref from alienGuns | 2,638 | 847 | 17,172 |
| case4Tail `Globals *volatile &globals` | 2,732 | 838 | 17,172 |
| case4Tail `Status *volatile &status` | 2,626 | 845 | 17,172 |
| case4Tail `Mission *mission` local | 2,626 | 845 | 17,172 |
| `alienGuns`/`pirateFortress`/`raid` `__builtin_expect` (all 10 forms) | 4,344–16,730 | — | 17,180–17,324 |
| `volatile int enemyRace` | 16,402 | 2,309 | 17,232 |
| case-1 `enemyRace += (i == 0x7fffffff)` / `(i >> 31)` | 2,626 | 845 | 17,172 |
| case-1 `enemyRace += (i & 0x40000000) != 0` | 4,913 | **750** | 17,172 |
| `int guns` hoisted before `if (raid)` (inner removed) | 3,560 | 916 | 17,168 |
| `int guns` moved after the `raidRace == 8` test | 4,184 | 849 | 17,164 |

Declaration-order sweep: all 120 permutations of
`{rnd ref, globals ref, raidType, hard, guns}` measured; best is 2,626
(20+ permutations tie), none is 2,625.  Adjacent-statement swap sweeps over
`1500–2535` (every simple adjacent pair, ~450 builds) found **no** pair with
size 17,172 and raw < 2,626.  Best aligned-only results: swap `1969/1970`
(the two `MatrixRotateVector` calls) = 2,626 / **830** / 17,172 (2 extra
insns); swap `2021/2022` = 2,627 / **815**.

Per-TU `-mllvm` flag sweep on `Level.cpp` (object md5 compared): neutral
for `-disable-postra-machine-licm`, `-disable-postra-machine-sink`,
`-disable-spill-hoist`, `-sink-insts-to-avoid-spills`,
`-condsider-local-interval-cost`, `-regalloc-csr-first-time-cost=0/100`,
`-enable-deferred-spilling`, `-disable-spill-fusing`,
`-optimize-regalloc=false`, `-misched-postra`, `-disable-post-ra`,
`-disable-licm-promotion`, `-disable-early-taildup`,
`-disable-early-ifcvt`, `-enable-gvn-hoist`, `-enable-gvn-sink`,
`-disable-cgp`.  Re-enabling `-enable-earlycse-memssa` is also neutral now
(the checked-in `=false` override no longer moves a byte at this base).

## 5. Next steps

1. The `ldrd`/`strd` formation in the alienGuns block is the only known
   size surplus in the tail.  If a source shape is found that makes clang 7
   emit the binary's five separate `ldr`/`str` pairs *and* keeps the plain
   `bigAlienWave ? ... : ...` ternary, the crutch can be dropped and the
   ~4 raw bytes of the ternary region recovered (the crutch itself is
   raw-neutral, so this alone is small).
2. The ~2.2 KB tail needs the `&Globals::rnd` spill value to stop being
   live across the alienGuns/pirateFortress loops.  Since the reload is
   placed by `SpillPlacement` (not MachineLICM), the source must make the
   value either unspilled or redefined after the loops.  `-disable-spill-hoist`
   and `-disable-postra-machine-licm` are both no-ops here.
3. The case-1 hoist (`ldr r5,[sp,#116]` at `0xb158c`) is also a
   `SpillPlacement` hoist; a loop-body store to the same spill slot would
   suppress it, but every source form tried either folds away or changes
   the loop body enough to lose more raw than the 341 bytes it recovers.

## 6. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13578
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Expected: `raw=2626 aligned=845 size=17172/17172`, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13578/`
(`tryrun.py`, `hunks.py`, `sweep_decl2.py`, `sweep_swap.py`, `batch*.py`,
`flagtry.sh`, `mir_all.log`, `mir_final.txt`, `Level.cpp.base`).
