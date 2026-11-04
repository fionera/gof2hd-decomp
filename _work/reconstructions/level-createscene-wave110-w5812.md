# `Level::createScene` — wave-110 w5812: the exact 93.3 % target is
# **sink flag + `rnd` mask 35/37** (not 19/21); MachineSinking cannot fire
# because the parts-loop preheader has a single successor

**Status:** research note, 2026-11-04. Worktree `wt/w5812`, branch
`wave110-w5812`, base `390f58052`. **No source change landed** — the shipped
source is still the best flag-free shape found (88.3 % / 2252 B). This commit
contains only this note and leaves `src/` untouched.

Follow-up to `level-createscene-wave109-w5713.md` (and the chain back through
`…-wave108-w5615.md`, `…-wave107-w5514.md`, `…-wave106-w5420.md`,
`…-wave105-w5308.md`, `…-wave103-wf312.md`, `…-wave102-wf212.md`,
`…-wave101-wf111.md`, `…-wave100-w5012.md`, `…-wave99-w4911.md`,
`…-wave98-w4814.md`, `…-wave97-w4712.md`, `…-wave95-w4514.md`,
`…-wave94-w4414.md`, `…-wave92-w4249.md`, `…-wave92-w4213.md`,
`…-wave91-w4113.md`, `level-createscene-phi-use-w4013.md`,
`level-createscene-licm-hoist-w3833.md`, `Level-createScene-flag-free.md`,
`level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`, first diff at
  instruction 460 (`0xb4830`, the parts-loop preheader
  `add.w r9/r8, r0, r2, lsl #4`). Unchanged from waves 88–109.
* Gate re-measured before and after this pass, green:
  **byte_exact 4500/4524, linked_exact 4500/4524, stub_zero_size 0,
  missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
  lint CLEAN ✓, drift 0, RATCHET PASS.**
* **Correction to the wave-109 note.** The exact 93.3 % target is
  `-mllvm -sink-insts-to-avoid-spills` **plus `rnd` mask 35 or 37**, not
  19/21. A full 128-mask sweep re-run this pass gives:
  * no flag: max **88.3 %** at mask 41 (= shipped source, sites 0/3/5 use the
    local `rnd`); next best 88.2 (m097), 88.1 (m049);
  * sink flag: max **93.3 % / 2252 B / 783/791** at masks **35** and **37**;
    92.9 (m050/m052), 92.6 (m051/m053/m054), 92.5 (m058/m060),
    92.4 (m043/m045/m046); mask 41 = 89.3 / 2256; mask 0 = 75.9;
    mask 19/21 = **68.6** (the wave-109 note's "19/21 = 85.0/93.3" is
    wrong: without the flag m019/m021 measure 75.1 / 2228).
  * mask 35 = sites 0 (`nextInt(100)<=29`) and 1 (`race=nextInt(4)`) and 5
    (the `tries`-loop `spot=nextInt(capacity)`) use the local `rnd`;
    sites 2/3/4/6 use `Globals::rnd`. mask 37 = sites 0/2/5 local,
    3/4/6 `Globals::rnd`. Both differ from the shipped mask 41 only in that
    **site 3 (`pick = nextInt(3)`) must use `Globals::rnd`** plus one of
    site 1 / site 2 must use the local `rnd`. Changing site 3 alone
    (mask 33) is *not* enough: sink+33 = 87.2 %.
* **New this pass:** why `MachineSinking` cannot reproduce `SinkIntoLoop`
  from source. `MachineSinking::ProcessBlock` bails on any block with
  `succ_size() <= 1` (`MachineSink.cpp:342`). In the parts loop the hoisted
  defs all live in the preheader `bb.8`, whose only successor is the loop
  header `bb.9` (`successors: %bb.9(0x80000000)` in the MIR). The blocks that
  *do* have two successors (`bb.12` mesh check, `bb.13` `new AEGeometry`,
  `bb.14` `Vector`+`setRotation`, `bb.15` `if (!plainHangar)`) never contain
  the defs, because `MachineLICM` (which runs **before** `MachineSinking`,
  `TargetPassConfig.cpp:946,949`) has already moved them up to `bb.8`.
  `SinkIntoLoop` is the only pass that moves them back down, and it is
  `cl::opt`-gated (`MachineLICM.cpp:367`, default `false` in LLVM 7.0.2).
* ~80 further source shapes measured this pass (see §4). None beats 88.3 %
  at 2252 B. The `off_ge0m1` loop-variant address (wave 108) is still the
  only shape that reproduces the original parts-loop register signature, and
  it costs +8 B (86.2 %); combined with mask 35 it is 87.0 % / 2256 B.

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5812
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
```

Scratch (`/dev/shm/w5812`, not committed):

* `cc2.sh <src.cpp> <out.o> [flags]` — clang++ 7.0.2 with the exact ninja
  flags, compiles an arbitrary copy of `Level.cpp` (~1.5 s);
* `link2.sh <obj> <out.so>` — the exact gold link + `strip --strip-all` +
  `tools/reorder_shstrtab.py`, with the `Level.cpp.obj` and output paths
  rewritten, so several variants link in parallel;
* `score2.py <out.so>` — official single-function score (imports
  `tools/verify/verify.py` + `elf.py`);
* `mkvar2.py <spec> <out.cpp>` + `tryvar.sh` / `trysink.sh` — body-scoped
  replacement harness (`old\n>>>\nnew` chunks separated by `\n===\n`);
  `trysink.sh` adds `-mllvm -sink-insts-to-avoid-spills`;
* `gensweep.py` / `runsweep.sh` / `runsweepsk.sh` — the 128-mask `rnd`
  sweep, without / with the sink flag (logs `sweep_nosink.log`,
  `sweep_sink.log`, `sweep_sink2.log`);
* `orig.asm`, `ours.asm`, `diff2.py` — objdump of `0xb42ec..0xb4bb8` and a
  difflib opcode-level diff.

LLVM 7.0.2 sources used: `/dev/shm/wf213/llvm702src/lib/CodeGen/`
`{MachineLICM,MachineSink,TargetPassConfig}.cpp` and
`lib/Target/ARM/ARMInstrThumb2.td`.

## 2. The 93.3 % target, measured

| variant | pct | size | insns | note |
|---|---|---|---|---|
| shipped base (mask 41) | **88.3** | 2252 | 783/791 | first diff insn 460 |
| sink flag only (mask 41) | 89.3 | 2256 | 783/790 | parts loop exact |
| sink + mask 35 | **93.3** | **2252** | 783/791 | parts loop + ship loop |
| sink + mask 37 | **93.3** | **2252** | 783/791 | same score |
| sink + mask 50 / 52 | 92.9 | 2256 | 783/790 | |
| sink + mask 51 / 53 / 54 | 92.6 | 2256 | 783/790 | |
| sink + mask 43 / 45 / 46 | 92.4 | 2256 | 783/790 | |
| sink + mask 33 (site 3 only) | 87.2 | 2260 | 783/792 | not enough |
| sink + mask 0 (no local `rnd`) | 75.9 | 2232 | 783/792 | |
| mask 35, no flag | 85.0 | 2252 | 783/791 | |
| mask 0, no flag | 75.0 | 2228 | 783/787 | whole-function RA shifts |
| `off_ge0m1`, no flag | 86.2 | 2260 | 783/793 | wave-108 result |
| `off_ge0m1` + mask 35, no flag | 87.0 | 2256 | 783/794 | |

`sink + mask 35` matches the original parts loop **and** the ship loop; only
the ship-loop `rnd` reload shape and the literal-pool words still differ.
`diff2.py orig.asm vs sink+35` shows no diff at all between instruction 349
and 561 (the whole parts loop is byte-identical).

## 3. Why `MachineSinking` cannot do it from source

`SinkIntoLoop` (`MachineLICM.cpp:748`) is the pass that moves the hoisted
defs back into the loop. It exists only behind
`SinkInstsToAvoidSpills` (`MachineLICM.cpp:367`), default `false` in
LLVM 7.0.2. The only other pass that can move a def down is `MachineSinking`,
and it has two hard gates:

1. `ProcessBlock` returns immediately for `MBB.succ_size() <= 1`
   (`MachineSink.cpp:342`).
2. `FindSuccToSinkTo` needs every use of the def to be dominated by the
   chosen successor (`MachineSink.cpp:624`), and `isProfitableToSinkTo`
   additionally rejects a successor that post-dominates the def block
   (`MachineSink.cpp:536`).

In the base MIR (after `Early Machine Loop Invariant Code Motion`, dump
`/dev/shm/w5812/mir_all.txt:1124633`) the parts-loop preheader is

```
bb.8 (%ir-block.43):
; predecessors: %bb.7
  successors: %bb.9(0x80000000); %bb.9(200.00%)
  ...
  %528:tgpr    = tLDRLIT_ga_pcrel @_ZZN5Level11createSceneEvE13kHangarMeshes
  %529:gprnopc = t2ADDrs %528:tgpr, %3:rgpr, 34
  %530:rgpr    = t2MOVi 192
  %539:tgpr    = tLDRpci %const.1
  %541:gprnopc = t2ADDri %stack.1, 0
  %542:rgpr    = t2MOVi 304
  %545:rgpr    = t2MOVi -1
bb.9 (%ir-block.101):
; predecessors: %bb.8, %bb.24
  successors: %bb.12(0x7c000000), %bb.10(0x04000000)
```

`bb.8` has exactly one successor, so `MachineSinking` never looks at it.
The two-successor blocks of the loop body (`bb.12` mesh check, `bb.13`
`new AEGeometry`, `bb.14` `Vector` + `setRotation` + `new PlayerStatic`,
`bb.15` `if (!plainHangar)`) contain no hoisted def: `MachineLICM` runs
*before* `MachineSinking` (`TargetPassConfig.cpp:946` then `:949`) and has
already lifted `%530/%539/%541/%542/%545` into `bb.8`.

Making the preheader branch (e.g. by turning `plainHangar` into an `if`)
does not help either: `MachineLoop::getLoopPreheader()` requires the
predecessor to have exactly one successor, so `getCurPreheader()`
(`MachineLICM.cpp:1491`) falls through to `SplitCriticalEdge`, which creates
a *new* single-successor block for the hoisted defs. That block is again
invisible to `MachineSinking`, and the split costs an extra branch.

`IsProfitableToHoist` cannot be defeated from source either: `t2ADDri`
(`T2I_bin_ii12rs.ri`, `ARMInstrThumb2.td:769`) and `t2MOVi`
(`ARMInstrThumb2.td:1886`) both carry `isReMaterializable = 1`, and
`MachineLICM.cpp:1206` returns `true` unconditionally for any trivially
rematerializable instruction before the register-pressure heuristic runs.
`t2ADDri12` (the `ri12` member) is *not* rematerializable, but
`rewriteT2FrameIndex` (`Thumb2InstrInfo.cpp:478`) normalises any frame offset
that is a valid `t2_so_imm` back to `t2ADDri`; the `Vector` temp sits at
`sp+56`, so it can never become `t2ADDri12` without moving it past 255
(which needs a bigger frame — see the `pad[256]` measurement below).

## 4. Measured this pass (official score; base = 88.3 / 2252)

### 4.1 Full 128-mask `rnd` sweep

Sites in source order (bit *i* set = site *i* uses the local `rnd`):

| bit | source |
|---|---|
| 0 | `if (rnd->nextInt(100) <= 29)` |
| 1 | `race = Globals::rnd->nextInt(4)` |
| 2 | `if (Globals::rnd->nextInt(100) < 30)` |
| 3 | `int pick = rnd->nextInt(3)` |
| 4 | `int spot = Globals::rnd->nextInt(capacity)` |
| 5 | `spot = rnd->nextInt(capacity)` (tries loop) |
| 6 | `(float) Globals::rnd->nextInt(300) / 100.0f` |

Shipped source = mask 41 (bits 0/3/5).

No flag: max **88.3 %** (m041 = shipped). m097 88.2, m049 88.1, m056 87.5,
m057 87.4, m034/m036 85.5, m035/m037 85.0, m019/m021 **75.1 / 2228**,
m000 75.0 / 2228.

With `-mllvm -sink-insts-to-avoid-spills`: max **93.3 % / 2252 / 783/791**
at m035 and m037 (see §2 for the rest of the top of the table).

### 4.2 `Vector` construction / scope / type (all 88.3 unless noted)

`Vector rot(0,pi,0); g->setRotation(rot);` 88.3 · `Vector rot; rot.y=pi;`
88.3 · field-by-field `rot.x/rot.y/rot.z` 88.3 · `const Vector rot{...}`
88.3 · `Vector rot = Vector(0,pi,0)` 88.3 · `Vector(0, 3.1415927f, 0)` with
integer zeros 88.3 · `3.14159274f` 88.3 · `float halfTurn = 3.1415927f;`
88.3 · store order z,x,y 88.2 · `rot` declared before the `mesh` load 84.0 ·
`rot` declared before `new AEGeometry` 85.4 · `new PlayerStatic` before
`setRotation` 85.2 · `off_ge0m1` 86.2 / 2260.

### 4.3 Loop / CFG / statement-order permutations

`do { … } while (++k < 4)` 85.1 / 2256 · `k <= 3` 88.3 · `4 > k` 88.3 ·
`int k; for (k = 0; …)` 88.1 · braced `continue` 88.3 · `mesh == -1` 88.2 ·
`k != 4` 87.9 · `table = spots` initialiser + switch 88.3 ·
`if/else` chain instead of `switch` 74.3 / 2260 · `spots` via `if` 86.5 ·
`const int capacity` 88.3 · `char *spotUsed` 88.3 · `childCount` local 88.3 ·
`(unsigned short)` casts on the mesh ids 88.3 · `ArrayAdd(hangarPart, …)`
compile error (template deduction) · `plainHangar == false` 86.1 ·
`sceneRace >= 4` 88.3 · `plainHangar = sceneRace != 0 && != 2 && != 3`
80.5 · `capacity` before `ownHangar` 79.8 · `spots` before `rnd` 88.3 ·
`AERandom *rnd` hoisted before `shipCount` 63.6 / 2224 · no local `rnd`
75.0 / 2228 · `setPosition` with the y-term split as
`(float) table[..+1] + (float) kShipHangarHeights[type]` 84.1 ·
`nextInt(300) / 100.0f` without the `(float)` cast 88.3 ·
`nextInt(300) * 0.01f` 88.1 · `int base = table[spot*3+1]` hoisted 88.3 ·
loop-variant address via `(mesh >> 31)` 86.2 / 2260 (same code as
`off_ge0m1`) · via `(mesh < 0)` 88.3 (the compiler folds it to 0 after the
`continue`) · `Vector rots[2] = {…, …}; setRotation(rots[k - k])`
77.6 / 2288.

### 4.4 Frame-size probe

A `volatile char pad[256]; pad[0] = 0;` at the top of `createScene` grows the
frame and drops the score to **72.8 % / 2196 B** (the `Vector` temp moves and
every `[sp,#N]` operand shifts), so the "push the frame offset past
`t2_so_imm` so the address becomes the non-rematerializable `t2ADDri12`"
route is not viable at 2252 B. A non-volatile `char pad[256]` is optimised
away and is byte-identical to the base.

## 5. Proven vs hypothesis

**Proven (measured on this exact base):**

1. No flag-free source shape among the ~70 tried this pass and the ~2500
   from waves 88–109 beats **88.3 % / 2252 B**; the full 128-mask `rnd`
   sweep without the flag peaks at the shipped mask 41.
2. The exact 93.3 % / 2252 B target is `sink-insts-to-avoid-spills` **plus
   mask 35 or 37**, i.e. `pick = nextInt(3)` must use `Globals::rnd` and one
   of `race = nextInt(4)` / `nextInt(100) < 30` must use the local `rnd`.
   The wave-109 note's "mask 19/21" is a transcription error (those measure
   75.1 / 2228 without the flag and 68.6 with it).
3. `MachineSinking` cannot move the hoisted defs back into the loop from
   source: the preheader `bb.8` has a single successor, and every
   two-successor block of the loop body is downstream of the `MachineLICM`
   hoist.
4. `IsProfitableToHoist` returns `true` unconditionally for `t2ADDri` /
   `t2MOVi` because both are `isReMaterializable`, before the register
   pressure heuristic; `t2ADDri12` would not be hoisted but the `sp+56`
   frame offset is always normalised back to `t2ADDri`.
5. Making the preheader branch just makes `getCurPreheader()` split the
   critical edge into a new single-successor block; it does not expose the
   defs to `MachineSinking`.

**Hypothesis (unproven):**

* The original `createScene` was compiled by a build in which
  `SinkInstsToAvoidSpills` was enabled for this function (LLVM 8 made it the
  default). The rest of the `Level.cpp` TU is tuned to the non-sink build
  (whole-TU flag: `byte_exact 4500 → 2357`), so the flag cannot be landed
  per-TU either.

## 6. Structural state (unchanged by this pass)

* `createScene` is 2252 B in both, 783/791 instructions.
* No source file touched, so no ELF section or `.ARM.extab` delta was
  introduced: 28 section headers (the extra 192-byte `.jcr`
  `Status::missionCompleted` rank table), file size +44, `.ARM.extab`
  `0x26294` (4 B short), `.rodata` / `.dynsym` / `.dynstr` / `.hash` /
  `.dynamic` / `.rel.dyn` size-exact.
* The only remaining `createScene` diffs are the parts-loop / ship-loop
  register allocation and 8 literal-pool words whose global addresses are
  shifted by +2/+4/+6 bytes by the `.data`/`.bss` layout of other TUs.

## 7. Bottom line

`Level::createScene` remains **88.3 % / 2252 B** (783/791, first diff 460).
The path to 93.3 % is fully characterised (sink flag + `rnd` mask 35/37) but
no flag-free source shape that reproduces `SinkIntoLoop` has been found in
~2580 variants over waves 88–110. Worktree clean, gate green
(byte 4500 / linked 4500 / lint CLEAN / drift 0 / RATCHET PASS).
