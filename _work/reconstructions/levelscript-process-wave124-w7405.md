# LevelScript::process — wave 124 / w7405 (register-allocation / `this`-spill probes)

Status: **no source win**, note-only. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Base master `6f8ee2320` (wave-123 final). Source:
`src/game/world/LevelScript.cpp` (unchanged).

## Baseline / gate

```
raw ratio (difflib)      60.256665340
official verify pct      60.3
orig / ours insns        20169 / 20039
orig / ours size         57232 / 57232        (size-exact)
byte_exact               4506
linked_exact             4506
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN ✓
drift                    0
RATCHET                  PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked
`.so` (helper `/dev/shm/w7405/score.py`).

## What this pass established (all measured, not guessed)

### 1. The faithful accessor order still loses, and the loss is the spill-slot swap

`messages, player, enemies` (the original's call order) reproduces the first
~30 prologue instructions almost exactly:

```
orig                            faithful mpe
ldr r0,[r4,#24]!                ldr r0,[r4,#24]!
bl  getMessages                 bl  getMessages
mov r5,r0                       mov r5,r0          <- messages=r5  ✔
ldr r0,[r4,#0]                  ldr r0,[r4,#0]
bl  getPlayer                   bl  getPlayer
strd r4,r0,[sp,#80]             strd r4,r0,[sp,#80]
ldr r0,[r4,#0]                  ldr r0,[r4,#0]
bl  getEnemies                  bl  getEnemies
mov r8,r0                       mov r8,r0          <- enemies=r8  ✔
ldr r0,[pc]                     ldr r0,[pc]
add r0,pc                       add r0,pc
ldr r4,[r0,#0]                  ldr.w r9,[r0]      <- status=r4 vs r9  ✘
ldr r0,[r4,#0]                  ldr.w r0,[r9]
bl  getCurrentCampaignMission   bl  getCurrentCampaignMission
cmp r0,#0                       cmp r0,#0
str.w sl,[sp,#88]               str.w sl,[sp,#88]
strd r4,r8,[sp,#92]             strd r9,r8,[sp,#92]
mov r0,r8                       mov r0,r8
strd r5,r6,[sp,#72]             strd r6,r5,[sp,#72]  <- slot 72/76 swap ✘
beq.w ...                       beq.w ...
```

Result: **58.783431**, 57416 B, 20026 insns. The two remaining diffs are
`status=r9` (not `r4`) and the `[sp,#72]`/`[sp,#76]` pairing.

### 2. The slot swap is a greedy-allocator spill-slot-weight decision

`-mllvm -print-machineinstrs` (NDK clang 7.0.2) shows the spill-slot indices
assigned by `SpillSlotAllocator` (higher index -> lower final frame offset):

| value | baseline idx -> offset | faithful mpe idx -> offset |
|---|---|---|
| `messages` | `%stack.682` -> 72 | `%stack.680` -> 76 |
| `delta` | `%stack.680` -> 76 | `%stack.681` -> 72 |
| `this` | `%stack.676` -> 88 | `%stack.677` -> 88 |
| `status` | `%stack.677` -> 92 | `%stack.676` -> 92 |
| `enemies` | `%stack.678` -> 96 | `%stack.675` -> 96 |

The final offsets are not a source-order decision: in both builds the same
`strd` store the pair, but the slot *indices* flip. `SpillSlotAllocator`
orders slots by live-interval weight, and the weight of the split
`messages`/`delta` intervals changes when `messages` moves from `r4` to `r5`.
The baseline (`enemies, player, messages`) is the only order that keeps
`messages` at the lower slot index (72) while still being size-exact.

### 3. `this` is not fixed by the faithful prologue either

Even with `messages=r5`, the faithful build still reloads `this` from
`[sp,#88]` **196x** (baseline 195x), not the original's **328x**. The
original's extra reloads come from `sl` being borrowed as a general scratch
register throughout the body; in our build `r4` (free after the prologue)
takes that role instead. Fixing `status=r4` is necessary before `sl` can be
released as scratch, and `status` cannot be moved to `r4` from source (see
§4).

## Variants measured (all reverted; 57232 B unless noted)

| variant | raw % | ours size | note |
|---|---:|---:|---|
| **baseline** | **60.256665** | 57232 | current master |
| faithful `m,p,e` | 58.783431 | 57416 | `messages=r5` ✔, `status=r9`, slots swap |
| `m,e,p` / `p,m,e` | 58.767041 | 57416 | same |
| `p,e,m` | 59.290759 | 57300 | `messages=r4` |
| `e,p,m` (baseline) | 60.256665 | 57232 | identical to master |
| `m,p,e` with `Level *level = m_pLevel` | 58.752115 | 57416 | prologue becomes `mov r0,sl; ldr r4,[r0,#24]!` |
| `m,p,e` with `const Level *` / `Level *const` | 58.752115 | 57416 | same |
| `m,p,e` with `Level &level = *m_pLevel` | 58.752115 | 57416 | same |
| baseline + `Level *level` used for accessors | 59.206207 | 57300 | |
| `m,p,e` + `Status *status` first condition | 58.303702 | 57424 | |
| `m,p,e` + `Status *status` first two conditions | 58.383889 | 57440 | |
| baseline + `Status *status` first condition | 58.733673 | 57416 | |
| `Status &status` / `Status *const` first condition | 58.303702 | 57424 | |
| `m,p,e` + `Status *status` (all uses) | 49.245278 | 57120 | removes repeated GOT loads |
| `m,p,e` + `Level *level` (all uses) | 40.873441 | 57104 | |
| branch-scoped `Level *level` / `Status *status` (mission 0/1) | 60.256665 | 57232 | optimized away (byte-identical) |
| `const` on each of the three locals | 60.256665 | 57232 | byte-identical |
| `int dt = delta` / `const int &dt` / `int *dtp=&delta` | 58.783431 (mpe) | 57416 | delta interval unchanged |
| `volatile int dt = delta` | 46.765012 | 57668 | forces reloads, delta -> `r6`, all slots move |
| loop `(*enemies)[i]->geometry` twice (faithful re-eval) | 56.579635 | 57324 | delta moves to `fp`, slot order lost |
| loop with `Array<KIPlayer*> *e = enemies` | 60.256665 | 57232 | byte-identical |
| loop `geom_local` / `visible_first` | 60.147234 / 60.246717 | 57232 | slightly worse |
| adjacent-statement swaps, first 140 lines (34 candidates) | <= 60.256665 | 57232 | no improvement |

## Why the source forms fail

1. **`status=r4` cannot be forced.** In the faithful build `r4` is dead at the
   status load (`&m_pLevel` is killed by the third `ldr r0,[r4,#0]` before
   `getEnemies`), yet the greedy allocator still colours `status` to `r9`.
   Declaring `Status *status = Globals::status` (any scope, pointer/ref/const)
   either gets folded to the same `r9` code or changes the whole body
   (GOT-load CSE) and loses 11+ points.
2. **The slot swap is a weight tie-break, not declaration order.** Reordering
   the three declarations, forward-declaring them, or aliasing `delta`
   (`int dt`, `const int &`, `int *`) produces byte-identical code in the
   baseline order and the same swapped slots in the faithful order.
3. **`this` spill is downstream of `status`.** With `status=r9`, `r4` is the
   free scratch register, so `sl` stays `this`. The original's `sl`-scratch
   pattern (and 328 reloads) only appears once `r4=status` and `r5=messages`
   are both occupied.
4. **The loop shape is faithful only with a global reallocation.** Writing
   `(*enemies)[i]->geometry` twice (the original's reload pattern) makes the
   compiler hoist `enemies` and drop the `enemy` temporary, but it moves
   `delta` to `fp` and changes every later slot; the body grows to 57324 B and
   scores 56.58. The current `KIPlayer *enemy` local is a size-exact local
   optimum.

## Remaining root cause (unchanged)

The dominant residual is still MachineBlockPlacement sub-block permutation
inside the case bodies plus the `messages`/`status`/`this` register map. The
prologue is only the first divergence; the body's block layout drives the
allocator tie-breaks, and every source change that fixes the prologue
perturbs the body by more than it gains. 18 functions remain non-exact; this
one is still 60.3%.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields
cmake --build cmake-build-match --target lint           # CLEAN ✓
python3 /dev/shm/w7405/score.py                         # 60.256665340
```

Scratch helpers (`score.py`, `try.py`, `summary.py`, `opmap.py`, `ctx.py`,
`hill.py`, `LevelScript.base.cpp`) live in `/dev/shm/w7405/` and are not part
of the tree. No source change was committed; the worktree is clean.
