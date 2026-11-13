# Radar::draw wave 191b (w13606) — IR-stability probe, allocation-order derivation, no win

**Status:** complete, 2026-11-13. Worktree `wt/w13606`, branch `wave191b-w13606`,
base `master` `855fc0820`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13593.md` (definitive post-RA map),
`-w13574.md` (eviction weights), `-w13562.md` (queue order).
**No source change landed**: `src/` is untouched, every measured cell is
raw-neutral (6,013) or worse and size-exact unless noted.

## 0. Baseline verified on this worktree

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,965 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes (whole section) | 71,199 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` | 0 / 0 |
| diff runs inside `Radar::draw` | 402 |

Reproduce:
```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
```

## 1. New evidence: the pre-RA IR is *provably* stable across spellings

This pass built a 6-second MIR probe (`scratch/mircheck.py`) that compiles one
source variant with `-mllvm -print-machineinstrs` and extracts, for
`Radar::draw`, (a) the vreg used by the two `t2STRBi12` zero stores, (b) the
vreg of the `t2SUBri ..., 172` `missionBlocksPlanets` def, and (c) the
`After Greedy Register Allocator` line at slot `2128B` (the `campaign` def).

Baseline: all three are `%2487`, and `2128B` is
`t2STRi12 $r0, %stack.45` — i.e. `campaign` is spilled at its definition.

The decisive measurement: **`unsigned campaign` changes exactly one line of the
whole 9,017-line pre-RA MIR** (`After Machine Instruction Scheduler`):

```
 B 24464B  t2Bcc %bb.212, 11, killed $cpsr   ; blt  (signed)
 U 24464B  t2Bcc %bb.212,  3, killed $cpsr   ; bcc  (unsigned)
```

Everything else — every vreg number, every live range, every operand — is
byte-identical, so the register allocator produces the *same* assignment and
the raw delta is exactly the one changed condition-code nibble (6,013 -> 6,014).
This is the general rule for this function: **source spellings that do not
change a live-range size or a block frequency cannot move the RA at all.**

`onScreen`-first is the one structural variant that renumbers the IR: it
creates two fewer vregs before the planet loop, so every later vreg number
shifts by 2 (`%2487 -> %2485`, `%2494 -> %2492`, ...). 1,991 MIR lines differ
but only by the renumbering; sizes and hints are unchanged, the RA is
identical, and `campaign` is still spilled to `%stack.45`.

## 2. New evidence: the ARM allocation order explains the expected outcome

`RegisterClassInfo::compute` (`lib/CodeGen/RegisterClassInfo.cpp`) puts the
volatile registers first and then the callee-saved ones **in the target's CSR
order**.  The prologue of this function is the split-push form
(`push {r4,r5,r6,r7,lr}` + `stmdb sp!,{r7,r8,r9,sl,fp}`), so the CSR list is
`CSR_AAPCS_SplitPush` (`lib/Target/ARM/ARMCallingConv.td:258`):

```
(add LR, R7, R6, R5, R4, R11, R10, R9, R8, D15..D8)
```

With `r7` reserved as the frame pointer and `lr` not allocatable, the
callee-saved allocation order is **r6, r5, r4, r11, r10, r9, r8** — i.e.
**`r11` (fp) is preferred over `r10` (sl)**.

That makes the original's colouring the *expected* one, not the surprising one:

* `campaign` (`%422`, hinted, size 22,320) is dequeued before `%2487`
  (non-hinted, size 36,400). It takes the first free callee-saved register.
* `%2487` (zero + `missionBlocksPlanets`) is dequeued later; if `r11` is free
  it takes `r11` — that is the original (`mov.w fp,#0` … `subs.w fp,r0,#172`,
  spilled to `sp+0x70` at `0x12e3ba`, reloaded into `r5` at `0x12ffae`).

So our build is the anomaly: at `%2487`'s dequeue **`r11` must already be
occupied**, forcing the eviction of `campaign` from `r10`. The evicting range
is a short/local range that is later split away (r11 is pure scratch in the
final code — first `$r11` def in the VRR dump is `4840B`,
`$r11 = COPY killed $r10`). The lever is therefore *the local-range population
of bb.1*, not `campaign` itself.

## 3. Cells measured this pass (all reverted; base 6,013 / 1,965 / 9,908)

### 3.1 Early-block spelling (all raw 6,013, size 9,908)

`SetColor(-1)` / `~0u` / `0xFFFFFFFFu`; `Mission *const mission`;
`Mission *volatile mission` (9,210 / 9,900); `int`/`unsigned`/`int8_t`/
`uint8_t`/`long` `missionBlocksPlanets`; `mbp = 0` / `0u` / `(int)false` /
`!true` / `1-1`; `mbp = (type != 172) ? 1 : 0` / `? true : false` /
`172 != type` / `(type == 172) ? 0 : 1` / `(int)(type != 172)` / `!(type == 172)`;
zero stores `= 0L` / `(uint8_t)0` / `1-1` / `0*0` / `0&0` / `0|0` / `0^0` / `0/1`
/ `~0u ^ 0xff`; declaration permutations of
`(mission, missionBlocksPlanets, alwaysEnemySeen, alwaysEnemy)`;
`bool`->`int` for `alwaysEnemySeen` (9,191 / 9,952) and `alwaysEnemy`
(8,599 / 9,920).

### 3.2 `campaign` spelling (all size 9,908)

`long long` / `int32_t` / `intptr_t` / `size_t` / `+1-1` / `|0` / `&-1` / `*1`
/ `(int)` / `static_cast<int>` / `+0u` / `^0` / `<<0` — all raw 6,013.
`unsigned` / `unsigned int` — raw 6,014. `short` / `char` / `uint8_t` —
8,446–8,491 / 9,912. `& 0xffff` 8,505 / 9,912; `% 100000` 9,035 / 9,920;
min-clamp 9,324 / 9,928. `volatile int` 9,208 / 9,892.
Placement: between turret/alien 6,020; after `planetTargets` 6,090;
before turret 8,559 / 9,900.

### 3.3 Getter / local temporaries (all raw 6,013, size 9,908)

`enemies`/`landmarks`/`route`/`planetTargets` through a named local; `cameraX/Y/Z`
through locals; `Matrix m = ...; this->transform = m` (9,147 / 9,908);
`Matrix inv = ...` (6,943 / 9,896); `StarSystem *ss = level->getStarSystem()`.

### 3.4 Planet-loop preamble (size 9,908)

| cell | raw | aligned | runs |
|---|---:|---:|---:|
| base (stationId, currentStation, onScreen) | **6,013** | 1,965 | 402 |
| `onScreen` first (matches binary) | 6,014 | **1,961** | **401** |
| `onScreen` first, no braces / `==0` / `!!` | 6,014 | 1,961 | 401 |
| `onScreen` between stationId and currentStation | 6,014 | 1,961 | 401 |
| `onScreen` after warpGate / after textOffset | 6,023 | 2,044 | – |
| `textOffset` before `onScreen` | 6,013 | 1,965 | 402 |
| `onScreen` first + `campaign` unsigned | 6,015 | 1,961 | – |
| `onScreen` first + chained zero stores | 6,016 | 1,961 | – |
| `onScreen` first + `mission` const / `mbp 0u` / `long` | 6,014 | 1,961 | – |
| `onScreen` first + `alienOrbit` bool | 6,023 | 2,009 | – |

`onScreen`-first is the **correct** evaluation order (the binary does
`update(); if(!onScreen) continue; stationId; currentStation; warpGate;
textOffset`), and it is the best *aligned* size-exact form (1,961 vs 1,965) with
one fewer diff run (401 vs 402), but it costs **+1 raw byte** and is neutral in
`.ARM.extab` (577 -> 577). Measured whole-section deltas:

| variant | `.text` diff | `.ARM.extab` diff |
|---|---:|---:|
| base | 71,199 | 577 |
| `onScreen`-first | 71,200 | 577 |

So it is a net −1 on the primary metric; left out, consistent with w13593.

### 3.5 Per-TU `-mllvm` flag probes (Radar.cpp only, size must stay 9,908)

| flag | raw | aligned | size |
|---|---:|---:|---:|
| (none) | **6,013** | **1,965** | **9,908** |
| `-enable-local-reassign` | 6,013 | 1,965 | 9,908 |
| `-condsider-local-interval-cost` | 6,013 | 1,965 | 9,908 |
| `-regalloc-csr-first-time-cost=1` / `=2` | 6,013 | 1,965 | 9,908 |
| `-lcr-max-depth=8` / `-lcr-max-interf=16` | 6,013 | 1,965 | 9,908 |
| `-exhaustive-register-search` | 6,013 | 1,965 | 9,908 |
| `-join-globalcopies=0` / `=1`, `-join-splitedges=0` / `=1` | 6,013 | 1,965 | 9,908 |
| `-enable-earlycse-memssa=false` | 6,013 | 1,965 | 9,908 |
| `-tail-dup-placement-threshold=2` / `=8` | 6,013 | 1,965 | 9,908 |
| `-align-all-nofallthru-blocks=1` | 6,013 | 1,965 | 9,908 |
| `-disable-licm-promotion` | 6,013 | 1,965 | 9,908 |
| `-terminal-rule=1` | 6,015 | 1,969 | 9,908 |
| `-disable-postra-machine-sink` | 6,169 | 1,977 | 9,908 |
| `-misched-postra` | 6,058 | 1,985 | 9,908 |
| `+prof-unpr` | 6,847 | 2,063 | 9,912 |
| `-enable-deferred-spilling` | 8,664 | 2,477 | 9,916 |
| `-split-spill-mode=default` | 9,060 | 3,111 | 9,964 |
| `-split-spill-mode=size` | 9,415 | 2,250 | 9,892 |
| `-join-liveintervals=false` | 10,156 | 4,624 | 10,452 |
| `-likely-branch-weight=1..1000` (+`-unlikely=1`) | 8,466–9,286 | 2,005–2,147 | 9,912–9,960 |

`-enable-deferred-spilling` is the only flag that keeps the frame within 8 bytes
of the original and improves alignment, but it is size-fatal (+8). No flag
combination tried (`deferred` + `split-spill-mode=size`/`default` +
`join-globalcopies`/`terminal-rule`/`misched-postra`) returns to 9,908.

## 4. Conclusion

The residual is one greedy-RA eviction cascade rooted in the *local*-range
population of bb.1, not in `campaign`'s own weight. `campaign` is only 1 def +
1 use (live 2,128..24,448) and every source spelling that keeps the live-range
sizes fixed produces a byte-identical pre-RA MIR, hence an identical
allocation. The original's colouring (`campaign`->sl, zero/`missionBlocksPlanets`
->fp) is exactly what the ARM split-push allocation order (r6, r5, r4, r11,
r10, r9, r8) predicts when `r11` is free at `%2487`'s dequeue; in our build a
local range holds `r11` at that moment.

Every size-exact candidate measured (this pass plus w13593's ~200) is raw
6,013 or worse. The only structural mismatch left in the instruction stream —
the planet-loop preamble order — is fixed by `onScreen`-first at a cost of +1
raw byte (`.ARM.extab` neutral, aligned −4, diff runs −1). `src/` is therefore
untouched.

## 5. Scratch

`/home/debian/GalaxyOnFire2/scratch/w13606/`:
`mircheck.py` (6 s MIR probe: zero-store vreg, mbp vreg, `2128B` RA line),
`mirdiff.py` (pre-RA MIR diff of two probe dumps), `cell2.py`/`cell.py`
(0.9 s compile+link+strip+`fn_diff` cell), `batchA.py`–`batchE.py` (the ~120
cells above), `sweepflags.py` (per-TU `-mllvm` sweep via a CMakeLists edit that
restores itself), `secdiff.py`, `structdiff.py`, `bytediff.py`,
`mir_base.txt` / `mir_os.txt` / `mir_uint.txt`.
