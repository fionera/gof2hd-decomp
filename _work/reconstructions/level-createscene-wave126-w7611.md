# `Level::createScene` — wave-126 w7611: the residual is a four-value register
# cycle rooted at `ownHangar`, not a missing statement

**Status:** dead-end sweep, 2026-11-07. Worktree `wt/w7611`, base
`bf365a964`. Target `_ZN5Level11createSceneEv` (`src/game/world/Level.cpp`,
`createScene` only). **No source change landed** — no shape raises the
score. Baseline and best remains the wave-124 landed source:
**93.554563 % / 2252 B**, 783 original / 784 ours normalized instructions,
first structural diff at instruction 9 (`str r0,[sp,#36]` vs `[sp,#40]`,
the `&__stack_chk_guard` spill slot). Full gate re-measured green:
**byte_exact 4506 / linked_exact 4506**, avg 99.94 %, data 843/843,
imports 207, lint CLEAN, drift 0, symbol parity 5370/5370,
`.ARM.exidx` byte-identical, `.ARM.extab` size/base exact.

Follow-up to `level-createscene-wave125-w7517.md` (which followed
`…-wave124-w7418.md`, `…-wave123-w7311.md`, `…-wave99-w4911.md`,
`…-wave98-w4814.md`).

## TL;DR

The wave-125 note described the residual as "capacity/spotUsed/ownHangar
transposition + `uxth` + fused `ldrd` + one extra `b.n`". This pass
resolves *why* the transposition exists and shows it is a closed cycle
that no source spelling in the current basin can break:

* The original's `ownHangar` is **`r4`**; ours is **`r5`**.
* Because `ownHangar` is `r5`, `r4` is free when `capacity` is allocated,
  so `capacity` takes `r4` (original: `r6`).
* Because `capacity` owns `r4`, `spotUsed` takes `r6` (original: `r4`).
* Because `capacity` owns `r4`, the `cap` truncation reuses `r4`
  (`uxth r4,r4`), which is the extra instruction.

So the whole ship-loop residue is one pivot: **`ownHangar` must be `r4`**.
The `cap` split (wave-124) is what moves `&Globals::status` to `sl` and
`&Globals::rnd` to `r8` (the two big wins), but it also pushes
`ownHangar` from `r4` to `r5`.

The no-`cap` shape gets `ownHangar` right (`r4`) but puts `status` in
`r6` (not `sl`) and `capacity` in `r8` (not `r6`) — the 2228 B basin.
The two requirements (`status → sl` and `ownHangar → r4`) are mutually
exclusive in every shape tried so far.

## 1. Exact residual (base, from `verify --show`)

```
  str  r0, [sp, #36]        |   str  r0, [sp, #40]        ; &__stack_chk_guard slot
  ...
  ldr.w r6, [r1, r0, lsl #2]|   ldr.w r4, [r1, r0, lsl #2]; capacity
  mov  r4, r0               |   mov  r6, r0               ; spotUsed = new[]
  lsrs r4, r0, #5           |   lsrs r5, r0, #5           ; ownHangar
  ...                       |   uxth r4, r4               ; extra cap truncation
  ldr.w r0, [sl, #4]        |   ldrd r0, fp, [sl, #4]     ; player(+geometry)
  ...                       |   b.n 0x7a0                 ; extra switch branch
```

Target map (original, verified against the raw disassembly at `0xb42ec`):

| value | original | wave-124 landed |
|---|---|---|
| `spotUsed` | r4 | r6 |
| `race` | r5 | r5 |
| `capacity` | r6 | r4 |
| `&Globals::rnd` | r8 | r8 |
| `i` | r9 | r9 |
| `status` | sl (spilled `[sp,#40]` in loop) | sl (spilled `[sp,#44]`) |
| `type` | fp | fp |
| `ownHangar` | r4 (then `[sp,#24]`) | r5 (then `[sp,#24]`) |
| `ship` | sl | sl |
| `spots` / `hangarPart` | `[sp,#44]` | `[sp,#32]` |

### 1.1 The chain, in original instruction order

Original preheader (`0xb4900..0xb4944`):

```
b4900  ldr.w r0,[sl]          ; status
b4904  blx   getStation
b4908  blx   getIndex
b490c  cmp   r0,#108
b490e  bne   b4922
b4910  ldr.w r0,[sl]          ; status
b4914  ldr.w r0,[r0,#276]     ; field_114
b4918  subs  r0,#3
b491a  clz   r0,r0
b491e  lsrs  r4,r0,#5         ; ownHangar -> r4
b4922  movs  r4,#0
b4924  ldr   r0,[pc,#632]     ; &Globals::rnd GOT
b4926  ldr   r1,[pc,#636]     ; kHangarCapacity GOT
b4928  add   r0,pc
b492a  add   r1,pc
b492c  ldr.w r8,[r0]          ; &Globals::rnd -> r8
b4930  ldr   r0,[sp,#52]      ; sceneRace
b4932  ldr.w r6,[r1,r0,lsl#2]; capacity -> r6   <-- r4 is live (ownHangar)
b4936  ldr.w r0,[r8]
b493a  adds  r1,r6,#1
b493c  blx   nextInt
b4940  cmp   r4,#0            ; ownHangar
b4942  str   r0,[sp,#28]      ; shipCount
b4944  str   r4,[sp,#24]      ; spill ownHangar -> frees r4
...
b4964  mov   r0,r6
b4966  blx   _Znaj
b496a  mov   r4,r0            ; spotUsed -> r4 (r4 now free)
```

The pivot is `b4932`: at the moment `capacity` is allocated, `r4` is
**occupied by `ownHangar`**, so the allocator picks `r6`. `ownHangar` is
then spilled at `b4944`, freeing `r4` for `spotUsed` at `b496a`.

In the landed source `ownHangar` is `r5`, so `r4` is free at `b4932`:

```
b4932  ldr.w r4,[r1,r0,lsl#2] ; capacity -> r4
...
b496a  mov   r6,r0            ; spotUsed -> r6
b497c  uxth  r4,r4            ; cap reuses r4
```

## 2. Why the two requirements are mutually exclusive

The `cap` split is load-bearing for the *early* allocation: it shortens
`capacity`'s live range and raises pressure enough that
`&Globals::status` stays in `sl` and `&Globals::rnd` in `r8` (wave-124).
But the same pressure pushes `ownHangar` from `r4` to `r5`. Removing the
split restores `ownHangar → r4` but drops `status` to `r6` and
`capacity` to `r8` (the 2228 B basin).

Greedy RA processes global intervals in decreasing `Size`. `status`
(spans almost the whole function) and `&Globals::rnd` are enqueued before
`capacity`; `capacity` is enqueued before `ownHangar`'s short first
segment. The `cap` split adds one more long global (`cap`) and shifts the
queue just enough to give `status` `sl`, but it also changes which of
`r4`/`r5` is free when `capacity` is placed.

## 3. Variants measured this pass (all ≤ 93.554563 %)

Every row was compiled with the repo's exact NDK r18b `-Oz` flags and
scored with the unrounded `difflib` ratio (not the 1-decimal report).
Variants marked "identical" produce byte-identical code to the landed
source.

### 3.1 `cap` form (all identical, 93.554563 % / 2252 B / 784 insns)

* `short cap = (short) capacity;` (`sxth`/`uxth` same allocation)
* `const unsigned short cap`
* `unsigned short const cap = static_cast<unsigned short>(capacity)`
* `const int capacity` / `const int capacity` + `const unsigned short cap`
* `cap` declared after `spots`, after the zero loop, after `new[]`,
  before `ownHangar`, before `capacity` (recomputed from the table)
* `ownHangar` as `int`, `unsigned char`, `== true`, `&&` single
  expression, ternary
* `plainHangar` as `int`
* `spotUsed` as `bool*`
* zero-loop index renamed `z`
* `hangarPart` typed `KIPlayer*` (no cast at `ArrayAdd`)
* parts-loop row pointer hoisted to `const int *kHangarRow`
* `new AEGeometry(kHangarMeshes[sceneRace][k], …)` (mesh re-indexed)
* switch: `table = spots` initialiser with no `default`, `default`
  first, cases reversed (`8,3,1,0`), `table = spots` + `default: break`

### 3.2 Forms that move the basin (worse)

| variant | pct | size | insns |
|---|---|---|---|
| zero loop bounded by `cap` (`i < cap`) | 93.426930 | 2252 | 784 |
| `unsigned int capacity` | 93.426930 | 2252 | 784 |
| `geo` after `setToSleep` (cap kept) | 89.9 | 2256 | 785 |
| no `cap` (capacity in loop) + `geo` hoist | 80.3 | 2228 | 786 |
| no `cap` + `geo` after `setToSleep` | 75.0 | 2228 | 787 |
| `int cap = capacity` / `& 0xffff` / `+0` / `*1` / `<<16>>16` | 78.8–80.3 | 2228 | 786 |
| `int cap = capacity % 65536` | 78.8 | 2240 | 790 |
| `new[]`/zero loop use `cap` | 79.2–79.8 | 2228 | 786 |

## 4. Remaining root cause

The residual is entirely Greedy-RA / MachineBlockPlacement ordering:

1. `ownHangar` `r4` (original) vs `r5` (ours) — the pivot.
2. `capacity` `r6` vs `r4`, `spotUsed` `r4` vs `r6` — the consequence.
3. The 4-slot frame rotation
   `{spots, parts-status, &chk_guard, kShipHangarHeights}`.
4. The `uxth` from the `cap` split.
5. The fused `ldrd` (from the `geo` hoist) and the switch-default `b.n`.

All constants, calls, and call order already match the binary. Closing
the function needs a shape in which `ownHangar` is `r4` **and**
`&Globals::status` is `sl` simultaneously — i.e. the split of
`capacity`'s live range must raise pressure without occupying `r4` at
`b4932`. No source spelling found across waves 91–126 does this.

## 5. Reproduction

Scratch is `/dev/shm/w7611`:

* `Level.cpp.base` — pristine `src/game/world/Level.cpp`;
* `bs.sh` — `link_so` + single-function score;
* `fast.py` — imports `tools/verify/verify.py`, disassembles only
  `_ZN5Level11createSceneEv`, prints the **unrounded** ratio, sizes and
  instruction counts (`fast.py diff` gives the aligned stream);
* `sig.py` — extracts the `capacity/spotUsed/&rnd/i/status/ownHangar/ship`
  register map from the preheader;
* `run.py <json>` — apply `[name, [[old,new],…]]` to the pristine base,
  build, score;
* `v1.json … v10.json` — the variant sets of §3.

```sh
OBJDUMP=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump $OBJDUMP --show _ZN5Level11createSceneEv
cmake --build cmake-build-match --target check -j8   # byte 4506 linked 4506
```
