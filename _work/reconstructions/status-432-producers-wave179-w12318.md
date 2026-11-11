# Status::missionCompleted + small tails — wave 179 (w12318): the branch-weight plateau is post-MBP-canonicalised

Status: **analysis only, no `src/` change landed** (no size-exact raw win
found).  Worktree `wt/w12318`, branch `wave179-w12318`, base master
`64014ce6f` (wave 178 merge).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12318`.

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS**, `byte_exact 4507`, `linked_exact 4507`, size 4524/4524,
`data_exact 843/843`, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical.

Target `_ZN6Status16missionCompletedEbbx` (1,432 B) plus the five small
tails.  Baselines re-confirmed exactly with `tools/fn_diff.py`:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN6Status16missionCompletedEbbx` | 721 | 327 | 1432 |
| `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1948 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 40 | 21 | 1996 |
| `_ZN10ModStationC1Ev` / `C2Ev` | 39 | 11 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 15 | 111 | 11440 |

Read first: `status-432-producers-wave178b-w12218b.md`,
`status-switch-nested2-wave162-w10815.md`,
`status-switch-nested-wave161-w10715.md`,
`status-plateau-source-wave156-w10213.md`,
`small-tail-wave178-w12220.md`.

## 0. Bottom line

1. **The if/else-chain fold does produce the target weight vector, but the
   final code is byte-identical to the base.**  A chain in the original body
   order with `__builtin_expect(cond, 0)` on every arm folds into an IR
   `switch` whose `!prof` weights decrease monotonically in chain order
   (the wave-179 handoff's "big discovery"), and
   `MachineBlockPlacement` *does* lay the case blocks out in that order.
   However, the `-Oz` size passes that run **after** MBP (`Thumb2
   instruction size reduce`, constant-island/branch-shortening) reorder the
   blocks back to numeric (ascending) order, so the linked `.so` is
   `cmp`-identical to the plain-switch base.  Every chain/nested variant
   below is byte-identical to the base.
2. **The plateau (raw 558 / aligned 47 / 1432 B) is still profile-only.**
   The wave-156/162 conclusion stands: the `4/4/4/3/2` vector is the only
   size-exact producer of the original hot-then-cold order, and no
   source-level `__builtin_expect` shape produces a flat 3-level vector.
3. **The five small tails remain at their wave-178 baselines.**  Re-swept
   the ternary/order/local shapes; all are greedy-register-allocation ties
   that the source does not move without a size change.

## 1. Status::missionCompleted — the chain-fold experiment

### 1a. The fold vector is real

`gen_chain.py` (scratch) compiles a toy `int f(int x)` whose arms are the
original body order `167/174,150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172` and every arm is
`if (__builtin_expect(x == c, 0))`.  `SimplifyCFG` folds the chain into a
`SwitchInst` with `!prof` weights that decrease monotonically in chain order
(the first two arms share a huge weight, the rest step down by ~600), so
sorting by weight reproduces the chain order exactly.  The same fold happens
inside the real function: `v_A0.cpp` (outer switch unchanged, `default:`
replaced by the 22-arm chain) emits

```
switch i32 %51, label %351 [ i32 167, i32 174, i32 8, i32 0, i32 11,
  i32 13, i32 14, i32 150, i32 151, ... i32 165 ], !prof !9330
!9330 = !{!"branch_weights", i32 -1492370373, i32 -1467039516, x7,
         i32 1413251, i32 1412543, i32 1411840, ...}
```

i.e. the outer 7 cases tie at the top, then the 150..165 cases decrease in
chain order.  A second `SwitchInst` holds 168/169/171/172.

### 1b. MBP lays it out in chain order, then the size passes undo it

`-mllvm -print-machineinstrs` on `v_A0.cpp` (scratch `a0.mir`) shows the
switch block's successors with the graded probabilities, and the dump
`# After Branch Probability Basic Block Placement` places the case blocks in
the chain order (167/174 first, then 150,151,152,153,154,155,158,162,166,
170,184,189,156,...).  But the *last* dump of the same file (after
`Thumb2 instruction size reduce` / constant-island / branch-shortening) has
the blocks back in numeric order `bb.0,bb.1,...,bb.10,bb.12,bb.13,...`, and
the linked `.so` is byte-identical to the plain-switch base:

```sh
./compile_file.sh v_A0.cpp      # raw=721 aligned=327 size=1432
cmp base.so a0.so               # identical
```

This is why the wave-177/178 sweeps saw a flat raw landscape: the source
shape only changes the *first* MBP layout, and the size passes canonicalise
it.  The wave-161/162 nested shapes that measured 1384/1392 B do not
reproduce that size from the current wave-178 case bodies: every nested
shape measured this wave is 1432 B and `cmp`-identical to the base.  (The
wave-162 note's exact 1384-B source shape was not recovered; a faithful
re-emission of its "outer `expect(mtype,167)`, default -> inner G3" shape
from both the current and the wave-162 `Status.cpp` measures 721/1432.)

### 1c. Shapes measured this wave (all `cmp`-identical to the base)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| plain switch (base) | 721 | 327 | 1432 |
| outer switch + 22-arm chain, every arm `expect(cond,0)` | 721 | 327 | 1432 |
| same chain, no expect | 721 | 327 | 1432 |
| same chain, every arm `expect(cond,1)` | 721 | 327 | 1432 |
| same chain, first 12 arms 1 / last 10 arms 0 | 721 | 327 | 1432 |
| pure 29-arm chain, all `expect(cond,0)` | 721 | 327 | 1432 |
| pure 29-arm chain, outer 1 / rest 0 | 721 | 327 | 1432 |
| pure 29-arm chain, no expect | 721 | 327 | 1432 |
| pure 29-arm chain, A 1 / B 0 | 721 | 327 | 1432 |
| 3-level nested switch, one-hot expect at each level | 721 | 327 | 1432 |
| outer switch `expect(mtype,167)` + nested G3 default | 721 | 327 | 1432 |
| outer `expect(167)`, inner `expect(150)`, inner `expect(156)` | 721 | 327 | 1432 |
| `switch (__builtin_expect(mtype, K))` on the whole switch, K=150/167/156 | 721 | 327 | 1432 |
| `switch (m->getType())` (no `mtype` local) | 721 | 327 | 1432 |
| source `case 162`/`case 158` swapped | 721 | 327 | 1432 |

Every one of these linked to the same 2,187,328-byte file; `cmp base.so
<variant>.so` reported no difference.  The `!prof` vector is present in the
IR (`grep branch_weights`) but does not survive to the final layout.

## 2. Small tails — re-swept, no size-exact raw win

### 2a. `MGame::OnUpdate` — 15/111/11440 (unchanged)

The wave-178 `Status *volatile st` anchor is in the base.  The 15 raw bytes
are four runs:

```
0x17e2bf..0x17e2c1  ldrd r2,r3  vs ldrd r3,r5   (m122 3600000 compare)
0x17e2c2..0x17e2c3  sbcs r1,r3  vs sbcs r1,r5
0x17e2c9..0x17e2cb  mov r2,#0   vs mov r3,#0
0x17f18c..0x17f195  ldr.w r0,[r9] vs str r2,[sp,#120] / ldr r0,[sp,#120]
```

New shapes tried (all 15/111/11440): `this->level->createRadioMessage(...)`
direct (no `Level *level` local); `Level *level` declared before `st`;
`Status *st` non-volatile; `st` used only for the volatile store and
`radioBase` via `Globals::status`; `(void) st`; the 3600000 comparison
written as `3600000 < pt - field`, `field + 3600000 < pt`, `>= 3600001`,
`3600000LL`.  The compiler canonicalises all of them.  The 5-byte `ldrd`
tie is caused by the volatile spill keeping the `&Globals::status` GOT base
live; removing the volatile local restores the wave-163 16-byte radio-block
diff instead (raw 16).

### 2b. `ModStation::ModStation` C1/C2 — 39/11/1008 (unchanged)

The residual is the `gHangarCamYaw`/`gHangarCamYawIPad` materialisation
(original `ldr r3,[pc]; ldr r4,[pc]`, ours `ldr r4,[pc]; ldr r3,[pc]`) plus
the `add rX,pc` interleave.  Re-swept `iPad ? YawIPad : Yaw`,
`!iPad ? Yaw : YawIPad`, `iPad == 0 ? ...`, `iPad != 0 ? ...`, `if/else`,
`&Yaw[0]`, `(const float *)` casts, parenthesised, and the two named arm
pointers.  All give 39/11/1008.  Pure greedy-RA tie.

### 2c. `MenuTouchWindow::OnTouchBegin` — 28/16/1896 (unchanged)

Two ties: `0x129fd2 ldr.w ip,[sl,#340]` vs `ldr.w r4,[sl,#340]` (case-8
second `if`, `layout->field_0x20_top` reload of `Globals::layout`) and the
case-15 scrollbar-hit register permutation.  Re-swept using
`Globals::layout` directly in the second `if`, a fresh `Layout *layout2`,
reordered `&&` operands, and `y - top - margin < width`.  All 28/16/1896.

### 2d. `Generator::createMission` — 40/21/1996 (unchanged)

The residual is the bonus block: `rem` slot `sp+32` (orig) vs `sp+44`
(ours), and `it ne; subne r5,r6,r0` (orig) vs `cbz; subs` (ours).  The
wave-178 ternary/`volatile int rem`/`bv` shapes were re-confirmed; no
size-exact improvement.

### 2e. `SpaceLounge::drawLounge` — 175/41/1948 (unchanged)

The `pad` register tie (`r9` vs `fp`) and the grouped-clamp shape were
re-confirmed; no size-exact improvement.

## 3. Conclusion

The 721-byte `Status::missionCompleted` residual is a genuine
post-`MachineBlockPlacement` plateau: the branch weights *are* attached and
*do* reorder the first MBP layout, but the `-Oz` size passes that run after
MBP canonicalise the block order back to ascending.  The original's
hot-then-cold order needs the flat `4/4/4/3/2` profile vector (wave-156
plateau), which no source-level `__builtin_expect` construct produces.
The five small tails are unchanged greedy-RA ties.

No `src/` change landed; the worktree is clean and the gate is green at the
wave-178 counts (`byte_exact 4507`, `linked_exact 4507`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12318
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12318/`: `Status_base.cpp`,
`parse_status.py`, `mkvar*.py` (variant generators), `compile_file.sh`
(single-TU compile + real gold link + `fn_diff`), `base.so`/`a0.so`/`h.so`/
`q1.so` (byte-identical variants), `a0.mir`/`q1.mir`/`h.mir`
(`-print-machineinstrs` dumps), `gen_chain.py`, `MGame.cpp.base`,
`ModStation.cpp.base`, `MTW.cpp.base`, `compile_mgame.sh`,
`compile_mod.sh`, `compile_mtw.sh`, `bytediff.py` (per-run byte diff).

## 5. Files changed

* `_work/reconstructions/status-432-producers-wave179-w12318.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change (no gate
  number moved).
