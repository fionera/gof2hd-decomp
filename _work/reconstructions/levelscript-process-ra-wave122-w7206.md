# LevelScript::process — wave 122 / w7206 (register-allocation / local-layout analysis)

Status: **no source win**, note-only. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Base master `29cd275f6`.

## Baseline / gate

```
raw ratio (difflib)      60.200000000
orig / ours insns        20169 / 20038
orig / ours size         57232 / 57232        (size-exact)
byte_exact               4506
linked_exact             4506
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN
drift                    0
RATCHET                  PASS
```

The metric is `difflib.SequenceMatcher(None, onorm, unorm,
autojunk=False).ratio()*100` over `tools/verify/verify.py`'s reconciled
instruction streams of the linked `.so`. The verifier report rounds this
function to 60.2%.

## Current source (the only lines that matter for the prologue)

`src/game/world/LevelScript.cpp:610-613`:

```cpp
    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
    PlayerEgo *player = m_pLevel->getPlayer();
    Array<RadioMessage *> *messages = m_pLevel->getMessages();
```

The original's prologue calls the same three accessors in the opposite
order: **messages, player, enemies**. This is the first divergence and the
source-level lever for the whole register-allocation residual.

## Original prologue vs ours

Original (`0x137d18`):

```asm
137d26  mov   sl, r0                 ; this
137d2a  mov   r4, sl
137d2c  mov   r6, r1                 ; delta
137d3a  ldr   r0, [r4, #24]!         ; r4 = &m_pLevel, r0 = m_pLevel
137d3e  blx   Level::getMessages
137d42  mov   r5, r0                 ; messages -> r5
137d44  ldr   r0, [r4]
137d46  blx   Level::getPlayer
137d4a  strd  r4, r0, [sp, #80]      ; &m_pLevel, player
137d4e  ldr   r0, [r4]
137d50  blx   Level::getEnemies
137d54  mov   r8, r0                 ; enemies -> r8
137d56  ldr   r0, [pc]               ; &Globals::status
137d58  add   r0, pc
137d5a  ldr   r4, [r0]               ; status -> r4
...
137d64  str.w sl, [sp, #88]          ; this spill
137d68  strd  r4, r8, [sp, #92]      ; status, enemies
137d6e  strd  r5, r6, [sp, #72]      ; messages, delta
```

Ours (baseline):

```asm
137d26  mov   sl, r0                 ; this
137d2a  mov   r4, sl
137d2c  mov   r6, r1                 ; delta
137d3a  ldr   r0, [r4, #24]!         ; r4 = &m_pLevel, r0 = m_pLevel
137d3e  blx   Level::getEnemies
137d42  mov   r8, r0                 ; enemies -> r8
137d44  ldr   r0, [r4]
137d46  blx   Level::getPlayer
137d4a  strd  r4, r0, [sp, #80]      ; &m_pLevel, player
137d4e  ldr   r0, [r4]
137d50  blx   Level::getMessages
137d54  mov   r4, r0                 ; messages -> r4
137d56  ldr   r0, [pc]               ; &Globals::status
137d58  add   r0, pc
137d5a  ldr   r9, [r0]               ; status -> r9
...
137d68  str.w sl, [sp, #88]          ; this spill
137d6c  strd  r4, r6, [sp, #72]      ; messages, delta
137d72  strd  r9, r8, [sp, #92]      ; status, enemies
```

Register map:

| value | original | baseline ours |
|---|---|---|
| `messages` | `r5` -> `[sp,#72]` | `r4` -> `[sp,#72]` |
| `status` | `r4` -> `[sp,#92]` | `r9` -> `[sp,#92]` |
| `enemies` | `r8` -> `[sp,#96]` | `r8` -> `[sp,#96]` |
| `delta` | `r6` -> `[sp,#76]` | `r6` -> `[sp,#76]` |
| `this` | `sl` -> `[sp,#88]` | `sl` -> `[sp,#88]` |
| `player` | `r0` -> `[sp,#84]` | `r0` -> `[sp,#84]` |
| `&m_pLevel` | `r4` -> `[sp,#80]` | `r4` -> `[sp,#80]` |

The stack slots themselves are already correct. The residual is the
`messages`/`status` register swap: original `messages=r5, status=r4`;
ours `messages=r4, status=r9`.

## Exact spill/reload evidence

Counts are `ldr`/`ldrd` loads from each stack slot in the linked function
(the one prologue `str` is excluded):

| slot | value | original | baseline ours | messages-first variant |
|---|---|---:|---:|---:|
| `[sp,#72]` | `messages` | 48 | 94 | 183 |
| `[sp,#76]` | `delta` | 160 | 180 | 41 |
| `[sp,#80]` | `&m_pLevel` | 72 | 73 | 80 |
| `[sp,#84]` | `player` | 163 | 165 | 165 |
| `[sp,#88]` | `this` | **327** | **195** | **195** |
| `[sp,#92]` | `status` | 45 | 24 | 23 |
| `[sp,#96]` | `enemies` | 167 | 178 | 180 |

The original has **982** stack-slot loads; ours has **909** (+73). The
original body is 131 instructions longer (20169 vs 20038), so roughly half
of the instruction-count residual is the extra spill/reload traffic. The
rest is block placement.

The user-facing “328 vs 195” reload count is the `[sp,#88]` reference count:
original 327 loads + 1 prologue store = 328; ours 195 loads + 1 store = 196.
The load-only numbers are 327 vs 195.

## Which live ranges force the original's `this` spill

`this` is the implicit `this` pointer, not a declared local. Its DWARF
location list in our build shows the shape the original also has:

```
this:  r0   [0x00,0x12)
       r10  [0x22,0x54)      ; sl
       sp+88 from 0x54 onward
delta: r1   [0x00,0x22)
       r6   [0x22,0x5a)
       sp+76 from 0x5a onward
enemies: r8  [0x2c,0x5a)
       sp+96 from 0x5a onward
messages: r4 [0x3e,0x58)
       sp+72 from 0x58 onward
```

The original keeps five long-lived values in the callee-saved set:

* `messages` in `r5`
* `status` in `r4`
* `enemies` in `r8`
* `delta` in `r6`
* `this` in `sl`

`player` and `&m_pLevel` are stack-only. That leaves `sl` as the only
callee-saved register that can be borrowed. The original borrows it
repeatedly for:

* stack-address materialisation: `add.w sl, sp, #...` at `0x13810e`,
  `0x1394ce`, `0x139de8`, `0x13a054`, `0x13a888`, `0x13acea`, ...
* loop counters: `mov.w sl, #0` at `0x13922c`/`0x1392f8`,
  `add.w sl, sl, #1` at `0x1392de`
* indirect/vtable call targets: `ldr.w sl, [r0]; blx sl` at
  `0x138b2e`/`0x138b3c`, `0x13a224`/`0x13a22a`, ...

Every such borrow forces a reload of `this` from `[sp,#88]`. Because
`messages`/`status`/`enemies`/`delta` are live for essentially the whole
function, the allocator cannot dedicate another callee-saved register to
`this`; it is stack-resident and reloaded on demand.

In our baseline, `messages` takes `r4`, so `status` falls to `r9`. That
leaves `r5` as an extra scratch register. The allocator uses `r5` instead
of `sl` in many of the same places, so `this` is reloaded only 195 times.
The original's `messages=r5, status=r4` assignment consumes the extra
scratch register, so `sl` is borrowed more and `this` is reloaded 327
times. This is the root cause of the 328-vs-195 asymmetry.

## Source-level cause

The original's first accessor call is `getMessages`, so `messages` is the
first call result and is coloured to `r5`. `getPlayer` follows, then
`getEnemies` (`r8`), then the first `Globals::status` use (`r4`). Our
source initialises `enemies` first, so `messages` is the third call result
and is coloured to `r4`; the first `Globals::status` use then has to take
`r9`. The declaration order is therefore the source-level cause of the
`messages`/`status` swap.

The stack-slot pairing is an independent allocator decision: baseline
`enemies, player, messages` produces `strd r4, r6, [sp,#72]` (messages
lower, delta higher), matching the original's `strd r5, r6, [sp,#72]`.
Making messages first produces `strd r6, r5, [sp,#72]` (delta lower,
messages higher), which is why the naive reorder loses despite fixing the
call order.

## Variants measured (all reverted)

| variant | raw % | size orig/ours | ours insns | notes |
|---|---:|---:|---:|---|
| baseline `enemies, player, messages` | **60.200000** | 57232/57232 | 20038 | current |
| `messages, player, enemies` | 58.800000 | 57232/57416 | 20025 | original call order; messages r5, status still r9; slots 72/76 swap |
| `messages, enemies, player` | 58.800000 | 57232/57416 | 20026 | same |
| `enemies, messages, player` | 59.000000 | 57232/57308 | 20004 | messages r5; slots 72/76 swap |
| `player, enemies, messages` | 59.300000 | 57232/57308 | 20046 | messages r4 |
| `player, messages, enemies` | 58.800000 | 57232/57416 | 20026 | messages r5 |
| messages-first + `Status *status = Globals::status` all uses | 32.400000 | 57232/55636 | 19202 | collapses status reloads |
| messages-first + local status first 2 conditions | 48.700000 | 57232/57256 | 19936 | local status raises priority but changes CFG |
| messages-first + `Status *volatile status` | 55.100000 | 57232/57100 | 19963 | forces reloads, but status in `fp` |
| messages-first + `Level **levelSlot = &m_pLevel` | 58.800000 | 57232/57416 | 20025 | status still r9 |
| wave-89 `-mllvm -regalloc=basic` diagnostic | 54.98 | 57232/57232 | — | gives `r4` prologue, confirms greedy-allocator tie-break (prior note) |

The baseline is a strong local optimum: every prologue reorder that fixes
the call order loses 1.2–1.4 raw points because it also swaps the
messages/delta stack slots and perturbs the block layout.

## Concrete patch proposal

The minimal faithful source change is the three-line reorder:

```cpp
    Array<RadioMessage *> *messages = m_pLevel->getMessages();
    PlayerEgo *player = m_pLevel->getPlayer();
    Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
```

This reproduces the original's call order and `messages -> r5`. It is a
fidelity patch, not a score win: in our build it still leaves
`status -> r9`, swaps `[sp,#72]`/`[sp,#76]`, grows the body to 57416 B, and
scores 58.8%. The remaining `r4`/`r9` tie-break is the greedy allocator's
choice, not something the three declarations alone control.

If the next pass wants the full original spill pattern, it should combine
the reorder with a source-level change that raises `status`'s priority
above `r9` while keeping the direct `Globals::status` expression (a local
`Status *status` collapses the reload pattern and loses 13.8 points). The
least invasive candidate is a narrow-scope local around the first
`if` chain:

```cpp
    Status *status = Globals::status;
    if (status->getCurrentCampaignMission() == 0) { ... }
    else if (status->getCurrentCampaignMission() == 1) { ... }
    // then use Globals::status directly again
```

Measured at 48.7% (57256 B, 19936 insns) for the first two conditions, so
it is not landable by itself. The full fix needs the original's block
layout as well; the prologue is only the first divergence.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields
cmake --build cmake-build-match --target lint           # CLEAN
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```

Scratch helpers used for the raw ratio and the slot counts lived in
`/dev/shm/w7206/` (`score.py`, `test_counts.py`, `test_status.py`,
`test_level_alias.py`); they are not part of the tree. No source change was
committed; the worktree is clean.
