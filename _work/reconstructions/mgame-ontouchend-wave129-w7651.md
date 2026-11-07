# MGame::OnTouchEnd — wave 129 (w7651): cold-block order via LLVM CTOB instrumentation + mining-guard layout flip

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7651`,
base `fee528cbe`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). No source change
landed (all measured variants either changed the 8080 B body size or lowered the score).
Scratch `/dev/shm/w7651*` (not committed).

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.
Current state: **81.9 %**, 8080/8080 B, `.text` 85,475 differing bytes, `.rel.plt` 0,
`.ARM.exidx` 0, `.ARM.extab` 1,433.

## New tooling: instrumented LLVM 7 `llc`

`/home/debian/llvm7build/llvm-7.0.0.src/lib/CodeGen/BranchFolding.cpp` already carries
`MGame::OnTouchEnd`-gated debug prints (added by an earlier wave but never used):

```
CTOB: chosen=<n> tails: <...>          // CreateCommonTailOnlyBlock survivor
TMB: IBB=<n> PredBB=<n> pots: <n>(h<hash>) ...
```

`/home/debian/llvm7build/build/bin/llc` (LLVM 7.0.0, assertions) is built from it. The NDK
clang's `-emit-llvm -O? -g0` IR can be fed to this `llc` and reproduces the same MIR block
numbering / merge decisions as the in-tree clang 7.0.2 codegen:

```
clang++ <NDK r18b flags> -g0 -emit-llvm -S -o MGame.ll src/game/menu/MGame.cpp
llc -mtriple=armv7-none-linux-androideabi16 -mcpu=cortex-a8 -float-abi=soft -O2 \
    -filetype=asm MGame.ll -o MGame.s 2> llc.err
```

This is a durable way to inspect the *exact* `BranchFolder`/`MachineBlockPlacement` decisions
without guessing from the final disassembly.

## What the instrumentation shows (current source)

The initHudMenu bodies are MIR blocks **420** (`initHudMenu(1)`, 0x200 case) and **426**
(`initHudMenu(2)`, 0x400 case); both branch to the test block **463**. The `TMB` line for the
test is

```
TMB: IBB=463 PredBB=462 pots: 426(h298562) 428(h53826) 430(h298562) 447(h6528) 448(h53826)
```

`CreateCommonTailOnlyBlock` is *not* called for this pair (no `CTOB` line mentions 426/430); the
survivor is chosen in the `commonTailIndex` loop (`SameTails[i].tailIsWholeBlock()`), i.e. by
the **tail-is-whole-block** rule, not by the runtime-estimate tie-break described in wave 128.
The current build lets block 420 (0x200) own the shared `blx initHudMenu`; the original lets the
0x400 body own it. The first `CTOB` line in the log (`chosen=180 tails: 236 207 180`) is an
unrelated `SetCurrentApplicationModule` merge.

## Key new result: the mining guard flips the whole tail layout

The original mining counter is `amount != 0` / `String(-amount)` (decrementing `amount`,
`sub.w r9,r9,#1` + `rsb r1,r9,#0` + `cmp.w r9,#0; beq`). The current tree spells it
`int count = -amount; if (count > 0)` (`add.w r8,r8,#1` + `mov r1,r8` + `cmp.w r8,#0; beq`).

Replacing the mining guard with the faithful `if (amount != 0) { ... String(-amount) ... }`
(**variant M1**) does **not** change the score directly, but it **flips the entire tail layout
to the original's**:

| | original | current | M1 (`amount != 0`) |
|---|---|---|---|
| return block (canary) | `0x17b004` (offset 0xec0) | `0x17b992` (0x184e) | `0x17af84` (0xe40) |
| order around test | 0x400 body → shared `blx` → test → isAutoPilot → return → cold | 0x200 body owns `blx`; test after 0x800 body; return at end | **0x400 body → shared `blx` → test → isAutoPilot → return → cold** |
| body size | 8080 | 8080 | **8064** (−16) |
| fuzzy | — | 81.9 | 74.3 |

So the layout hypothesis is confirmed: making the mining guard faithful is the lever that
pulls the 0x400 body in front of the test and outlines the cold `hudTouchFlags == 0` block
after the shared return. The score drops only because (a) the body shrinks by 16 B (shifting
every later literal pool) and (b) the register allocator swaps `sl`/`r9` (`this` moves from
`sl` to `r9`; `amount` from `r9` to `sl`). The cold-block *content* is already instruction-
identical to the original in both builds; only placement differs.

### M1 still differs in one respect: won-body prologue placement

M1 reproduces the original's **tail** order exactly (0x400 case → shared `blx` → test →
isAutoPilot → return → stack_chk_fail → cold `hudTouchFlags == 0` body), but the original
*also* interleaves the won-body prologue (`keepObjectives` `mov.w sl,#0`, mission selection,
`isInstantActionMission` dispatch) **before** the 0x400 case (orig blocks 62–64 @
`0x17af5a..0x17afd8`), whereas M1 places it **after** the cold blocks (M1 blocks 69–70 @
`0x17aff6..`). The current 81.9 % build interleaves the won body correctly (block 53 @
`0x17add0`) but has the tail wrong. The winning shape must get **both**: won-body prologue
interleaved with the hudMenu chain *and* the 0x400 body owning the shared `blx`.

## Variants measured this pass (all reverted; size must stay 8080)

| variant | size | pct | note |
|---|---:|---:|---|
| **current (base)** | **8080** | **81.9** | `.text` 85,475 |
| M1 mining `amount != 0` | 8064 | 74.3 | tail layout matches original; −16 B |
| M1 + `hasFailed()` bare | 8084 | 68.8 | |
| M1 + `hasWon()` bare | 8064 | 74.3 | |
| M1 + `hasFailed()` + `hasWon()` bare | 8072 | 69.2 | |
| `hasFailed()` bare alone | 8088 | 73.9 | `cmp #0` matches original, +8 B |
| `hasFailed() != 0` / `unsigned char` / `!!` / `(bool)` | 8088 | 73.9 | all identical |
| separate `if (hudTouchFlags == 0)` | 8076 | 79.1 | return still at end |
| separate `if` + `hasFailed()` bare | 8080 | 77.6 | size-exact but worse |
| `wingmanCommand` as separate `if` | 8076 | 79.1 | |
| both weapon/wingman + hud as separate `if`s | 8076 | 79.1 | |
| 0x400 case before 0x800 | 8076 | 79.0 | test order changes |
| `__builtin_expect(hudTouchFlags != 0, 1)` | 8080 | 81.9 | no-op |
| `__builtin_expect(hudTouchFlags == 0, 0)` | 8076 | 78.2 | |
| `__builtin_expect(count > 0, 1)` | 7944 | 48.5 | |
| `__builtin_expect(count > 0, 0)` | 8064 | 70.3 | |
| `count != 0` | 8064 | 74.3 | |
| `unsigned int amount` + `(int)` cast | 8064 | 74.3 | still canonicalises |
| `volatile int amount` | 8064 | 71.8 | |
| `amount * -1` / `-1 * amount` / `0 - amount` / `~amount + 1` | 8064 | 74.3 | all canonicalise |
| `campaign` temp removed (wave-124 form) | 8084 | 80.9 | |
| `hasWon()` bare in `won` | 8064 | 74.3 | |

The mining guard canonicalisation (`amount--` + `-amount` → `count++` + `count`) survives every
spelling tried, including `volatile` and unsigned. The original is the *non-canonicalised*
decrementing form; matching it costs 16 B in this tree.

## Next attack

Find a **size-neutral** way to keep the mining guard non-canonicalised (or otherwise flip the
tail layout) at exactly 8080 B. Candidates:
* a 16 B-faithful anchor elsewhere in the same TU that survives the register-allocation cascade;
* a source form of the mining loop that keeps `amount--`/`String(-amount)` in registers without
  the `InstCombine` count rewrite (e.g. an opaque read of `amount` that does not add code);
* re-run the instrumented `llc` on each candidate and require `TMB: IBB=463` to pick block 426
  (0x400) while the body stays 8080 B.

Gate at commit: `byte 4506 linked 4506`, `avg 99.94`, data 843/843, imports 207, lint CLEAN,
drift 0.
