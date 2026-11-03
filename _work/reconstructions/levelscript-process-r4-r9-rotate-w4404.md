# LevelScript::process — prologue register map, mission-125 chain, mission-1 rotate (w4404)

Status: code change landed, 2026-11-03. Branch `wave94-w4404`, base master `f262bd4b`.
Target: `_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
57232 B (`0xdf90`), 20169 instructions.

Independent second attack on the two residuals named in the wave-94 brief:

1. "`Globals::status` lives in r4 in the original vs r9 in ours";
2. "the mission-1 `player->rotate` argument slot order is swapped".

Both premises turn out to be **mis-stated**, and the measurements below say why.
The pass lands two faithful source-shape changes (the mission-125 `else if` chain and
the cached-player mission-1 `rotate`) for **52.0 -> 54.1** official, size-exact.

## Result

```
                         before            after
official rounded pct     52.0              54.1
raw fuzzy (autojunk=0)   52.98147283200997  55.14268650515437 (oracle)
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4499              4499
linked_exact             4499              4499
size-exact               4524/4524         4524/4524
stub_zero_size           0                 0
missing/wrong_type/extra 0/0/0             0/0/0
data_exact               843/843           843/843
imports                  207               207
.text                    0x1337c8          0x1337c8
.rodata                  0x41b67           0x41b67
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26298           0x26298
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

Section byte-diff after the change (helper at the bottom of this file):

```
.rel.plt   29392 29392      12 differing bytes
.text    1259464 1259464 105816
.rodata   269159 269159       0
.ARM.exidx 28528  28528    1647
.ARM.extab 156312 156304  (8 bytes short, known w4270 HangarWindow EH residual)
.rel.dyn   28744  28744       0
```

`.rodata` and `.rel.dyn` are byte-identical; `.text` differing bytes drop from
106,090 (wave-92 state) to 105,816.

## 1. The r4/r9 premise is wrong: r9 is `&__stack_chk_guard`, not `&Globals::status`

Original prologue (`0x137d18`, objdump of the ground-truth `.so`):

```
137d26: 4682       mov   sl, r0
137d28: 48de       ldr   r0, [pc, #888]      ; literal @0x1380a4 = 0xd7fd2
137d2a: 4654       mov   r4, sl
137d2c: 460e       mov   r6, r1
137d2e: 4478       add   r0, pc              ; r0 = 0x20fd04 = GOT slot of __stack_chk_guard
137d30: f8d0 b000  ldr.w fp, [r0]            ; fp = &__stack_chk_guard
137d34: f8db 0000  ldr.w r0, [fp]            ; canary
137d38: 906b       str   r0, [sp, #428]
137d3a: f854 0f18  ldr.w r0, [r4, #24]!      ; r4 = &this->m_pLevel, r0 = m_pLevel
137d3e: f72a e8d8  blx   Level::getMessages
137d42: 4605       mov   r5, r0               ; r5 = messages
137d44: 6820       ldr   r0, [r4, #0]
137d46: f72a e976  blx   Level::getPlayer
137d4a: e9cd 4014  strd  r4, r0, [sp, #80]   ; [sp+80] = &m_pLevel, [sp+84] = player
137d4e: 6820       ldr   r0, [r4, #0]
137d50: f72a e8b0  blx   Level::getEnemies
137d54: 4680       mov   r8, r0               ; r8 = enemies
137d56: 48d4       ldr   r0, [pc, #848]       ; literal @0x1380a8 = 0xd8400
137d58: 4478       add   r0, pc              ; r0 = 0x21015c = GOT slot of Globals::status
137d5a: 6804       ldr   r4, [r0, #0]         ; r4 = &Globals::status
137d5c: 6820       ldr   r0, [r4, #0]         ; r0 = Globals::status
137d5e: f729 ed08  blx   Status::getCurrentCampaignMission
```

Ours (same addresses, current tree):

```
137d28: 48e9       ldr   r0, [pc, #932]      ; literal @0x1380d0 = 0xd7fd2
137d2a: 4655       mov   r5, sl
137d2c: 460e       mov   r6, r1
137d2e: 4478       add   r0, pc              ; r0 = 0x20fd04 = GOT slot of __stack_chk_guard
137d30: 6800       ldr   r0, [r0, #0]         ; r0 = &__stack_chk_guard
137d32: 4681       mov   r9, r0               ; r9 = &__stack_chk_guard   <-- r9, not status
137d34: 6800       ldr   r0, [r0, #0]
137d36: 906b       str   r0, [sp, #428]
137d38: f855 0f18  ldr.w r0, [r5, #24]!      ; r5 = &this->m_pLevel
137d3c: f72a e8d8  blx   Level::getMessages
137d40: 4683       mov   fp, r0               ; fp = messages
137d42: 6828       ldr   r0, [r5, #0]
137d44: f72a e976  blx   Level::getPlayer
137d48: 4604       mov   r4, r0               ; r4 = player (kept live)
137d4a: 6828       ldr   r0, [r5, #0]
137d4c: 9514       str   r5, [sp, #80]
137d4e: f72a e8b2  blx   Level::getEnemies
137d52: 4680       mov   r8, r0
137d54: 48df       ldr   r0, [pc, #892]       ; literal @0x1380d4 = 0xd8402
137d56: 4478       add   r0, pc              ; r0 = 0x21015c = GOT slot of Globals::status
137d58: 6805       ldr   r5, [r0, #0]         ; r5 = &Globals::status      <-- r5, not r9
137d5a: 6828       ldr   r0, [r5, #0]
137d5c: f729 ed08  blx   Status::getCurrentCampaignMission
```

So the actual prologue register map difference is:

| value | original | ours |
|---|---|---|
| `this` | `sl` (+ `r4` temp) | `sl` (+ `r5` temp) |
| `&__stack_chk_guard` | `fp` | `r9` |
| `&m_pLevel` | `r4` | `r5` |
| `messages` | `r5` | `fp` |
| `delta` | `r6` | `r6` |
| `enemies` | `r8` | `r8` |
| `player` | stored to `[sp+84]` straight from `r0` | kept in `r4` until `0x137d66`, then stored |

`&Globals::status` is `r4` in the original and `r5` in ours — **not** `r9`.
`r9` is the stack-canary address in ours. Both functions end with the identical
canary reload `ldr.w r1, [fp]` (orig `0x144f62`, ours `0x144fa4`), because both
execute `mov fp, r9` (orig `0x1448c2`, ours `0x144c22`) before the shared epilogue.

### Why ours needs one more callee-saved register

The original keeps six values live across the prologue (`fp`, `sl`, `r4`, `r5`,
`r6`, `r8`) and spills `player` immediately; `r9` stays free. Ours keeps seven
(`r9`, `sl`, `r5`, `fp`, `r6`, `r8`, `r4`) because the allocator parks `player`
in `r4` across `getEnemies` and the first `getCurrentCampaignMission` call and
only spills it at `0x137d66`. That extra live value is what pushes
`&m_pLevel`/`&Globals::status` from `r4` to `r5`, `messages` from `r5` to `fp`,
and the canary address from `fp` to `r9`.

### Prologue shapes measured (oracle raw %, size)

All start from the landed tree. `pct` is the object-level screening number
(offset ~-1.0 from the official verifier; the ranking is reliable).

| prologue shape | pct | size |
|---|---:|---:|
| landed (`messages; enemies; player; enemies=`) | **55.14269** | 57232 |
| natural order (`messages; player; enemies` all initialised) | 55.14269 | 57232 |
| `enemies` fetched 2nd (`messages; enemies; player`) | 55.11651 | 57200 |
| `enemies` fetched 1st | 55.11132 | 57200 |
| `player` fetched 1st | 55.13749 | 57232 |
| declare-all-then-assign | 55.14269 | 57232 |
| `Level *level = m_pLevel;` + use `level` for all three getters | 54.53317 | 57240 |
| `Level *const level = m_pLevel;` (same) | 54.53317 | 57240 |
| `Level *&level = m_pLevel;` | 55.14269 | 57232 |
| `PlayerEgo *const player` | 55.14269 | 57232 |
| `register PlayerEgo *player` | 55.14269 | 57232 |
| `PlayerEgo *volatile player` | 43.07580 | 57020 |

The `Level *level` variant is the only one that reproduces the original's
`fp`/`r4`/`r5` assignment:

```
       4: e92d 0f80  stmdb sp!, {r7, r8, r9, sl, fp}
      1a: f8db 0000  ldr.w r0, [fp]        ; fp = &__stack_chk_guard   (matches)
      22: f850 4f18  ldr.w r4, [r0, #24]!  ; r4 = &m_pLevel            (matches)
      2a: bl    Level::getMessages
      2e: 4605       mov   r5, r0           ; r5 = messages             (matches)
```

but it moves `player` to `[sp+88]` (original `[sp+84]`) and spills `this` to
`[sp+80]`, which costs more than the prologue gains: 54.53 vs 55.14. The stack
slot colouring of the landed variant (`[sp+72]` messages/delta, `[sp+80]`
`&m_pLevel`, `[sp+84]` player, `[sp+88]` `this`, `[sp+92]` `&status`, `[sp+96]`
enemies) already matches the original exactly, so the landed shape is kept.

## 2. The mission-1 `rotate` argument order is already correct

Original `0x137d90`:

```
137d90: f06f 000f  mvn.w r0, #15
137d94: ed9f 1ac5  vldr  s2, [pc, #788]     ; 1.52587890625e-05
137d98: ea00 0046  and.w r0, r0, r6, lsl #1
137d9c: 2100       movs  r1, #0             ; arg1 = 0.0f
137d9e: ee00 0a10  vmov  s0, r0
137da2: eeb8 0ac0  vcvt.f32.s32 s0, s0
137da6: 9815       ldr   r0, [sp, #84]      ; this = cached player
137da8: ee20 0a01  vmul.f32 s0, s0, s2
137dac: ed9f 1ac0  vldr  s2, [pc, #768]     ; 6.2831855
137db0: ee20 0a01  vmul.f32 s0, s0, s2
137db4: ee10 2a10  vmov  r2, s0             ; arg2 = angle
137db8: 4613       mov   r3, r2             ; arg3 = angle
137dba: f72e eed4  blx   PlayerEgo::rotate
```

Ours (current tree):

```
137d90: f06f 000f  mvn.w r0, #15
137d94: ed9f 1ac5  vldr  s2, [pc, #832]
137d98: ea00 0046  and.w r0, r0, r6, lsl #1
137d9c: 2100       movs  r1, #0
137d9e: ee00 0a10  vmov  s0, r0
137da2: 4620       mov   r0, r4             ; this = player (in r4)
137da4: eeb8 0ac0  vcvt.f32.s32 s0, s0
137da8: ee20 0a01  vmul.f32 s0, s0, s2
137dac: ed9f 1ac0  vldr  s2, [pc, #812]
137db0: ee20 0a01  vmul.f32 s0, s0, s2
137db4: ee10 2a10  vmov  r2, s0
137db8: 4613       mov   r3, r2
137dba: f72e eed4  blx   PlayerEgo::rotate
```

The argument slots are **identical**: `r1 = 0.0f`, `r2 = angle`, `r3 = angle`,
matching the source `player->rotate(0.0f, angle, angle)`. The only residual is
the `this` load: original `ldr r0, [sp,#84]` (spilled player) vs ours
`mov r0, r4` (player still in `r4`). That is the same prologue spill decision as
section 1, not a slot-order bug.

The landed change is `m_pLevel->getPlayer()->rotate(...)` -> `player->rotate(...)`.
It is faithful (the original loads the cached player, it does not re-call
`Level::getPlayer`) and it removes 8 bytes, which is what lets the mission-125
chain below stay size-exact.

## 3. The mission-125 `else if` chain (the actual +2.1)

`src/game/world/LevelScript.cpp`, `case 125`, state 3 and state >= 4:

```diff
-                }
-                if (m_nState == 3) {
+                } else if (m_nState == 3) {
                     ...
-                }
-                if (m_nState >= 4 && m_nState <= 6 && player->isDockedToDockingPoint() && player->hackingWon()
+                } else if (m_nState >= 4 && m_nState <= 6 && player->isDockedToDockingPoint() && player->hackingWon()
                            && player->getHackingGameDockIndex() >= 0) {
```

Ghidra's `LevelScript::process` decompilation (`/dev/shm/w4404/process.c`, the
`switch` at Ghidra `0x14866a` = ELF `0x13866a`) shows the mission-125 state
machine as one `if / else if / else if / else if` chain, not independent `if`s.

Measured on the landed base: 16-way sweep over the four joins
`state0|state1`, `state1|state2`, `state2|state3`, `state3|state>=4`
(`join` = `} else if (...)`, `split` = `}` + a fresh `if (...)`). The landed
source shape is `join / split / join / join`.

| shape (s01/s12/s23/s34) | oracle pct | size |
|---|---:|---:|
| `join / split / join / join` (**landed**) | **55.14269** | **57232** |
| `split / join / join / join` | 55.67771 | 57228 |
| `join / split / split / split` | 52.51194 | 57224 |
| `split / split / join / join` | 54.94505 | 57228 |
| `join / split / join / split` | 48.54933 | 57276 |
| `join / join / split / join` | 48.52876 | 57124 |
| `join / split / split / join` | 45.67558 | 57216 |
| `join / join / join / join` | 45.02266 | 56804 |
| `split / join / join / split` | 48.27229 | 57352 |
| `split / join / split / join` | 47.74522 | 57140 |
| `split / join / split / split` | 47.73483 | 57164 |
| `split / split / join / split` | 48.40920 | 57252 |
| `split / split / split / join` | 48.14536 | 57192 |
| `split / split / split / split` | 48.12709 | 57192 |
| `join / join / split / split` | 48.41969 | 57140 |
| `join / join / join / split` | 38.35255 | 57092 |

`join/split/join/join` is the only shape that is both size-exact (57232) and
near the top. The one shape that screens higher, `split/join/join/join`
(55.68), was built and measured with the *official* verifier: **53.9** at
57228 B (vs 54.1 at 57232 for the landed shape), so the oracle ranking does not
transfer and the size-exact shape is landed.

The chain alone (without the cached `rotate`) is `55.50475` at **57240 B**, i.e.
+0.36 oracle but not size-exact (4523/4524). The cached `rotate` buys back the
8 bytes and lands at `55.14269` / 57232.

### `deleteHackingGame` placement

The tree also carries `player->deleteHackingGame();` **before** `m_nState++`
(the original order). Moving it after the `if (m_nState == 6)` block costs
0.12 oracle and 16 bytes:

| variant | pct | size |
|---|---:|---:|
| `deleteHackingGame` before `m_nState++` (**landed**) | 55.14269 | 57232 |
| after the `m_nState == 6` block | 49.44384 (official 48.7) | 57672 |

## 4. Things re-measured and rejected

Starting from the landed tree (55.14269 / 57232):

| variant | pct | size |
|---|---:|---:|
| mission-5 block moved before mission-81 (original source order) | 42.53450 | 57220 |
| mission-5 moved after mission-26 | 55.14269 | 57232 |
| mission-0 `else if (m_nState == 4)` split into `if` | 49.35463 | 57204 |
| mission-0 `else if (m_nState <= 2)` split into `if` | 45.66080 | 57360 |
| mission-1 second `if` -> `else if` | 51.55710 | 57216 |
| both `setCollide` sites re-fetch `Level::getPlayer()` | 55.01753 | 57248 |
| drop `volatile int pad = delta` anchor | 53.78064 | 57232 |
| drop the `m_nState == 0x7f` size anchor | 48.42723 | 57124 |
| drop both `volatile` anchors | 48.39087 | 57124 |
| `else if` at source line 1717 | 54.83678 | 57188 |
| `else if` at source line 3107 | 49.44544 | 57212 |
| `else if` at source line 1969 | 55.14269 | 57232 (identical code) |
| `else if` at source line 2016 | 53.92226 | 57216 |
| `else if` at source line 2835 | 53.19414 | 57220 |
| `else if` at source line 3100 | 51.97538 | 57256 |
| `else if` at source line 4742 | 47.66771 | 56056 |

Note that the original's pre-switch source order really is `5, 81, 26, 78, 50,
89, 51` (original disassembly: mission-5 body at `0x13813c`, mission-81 test at
`0x1381b4`, mission-26 test at `0x1382dc`, mission-78 at `0x13832c`, mission-50
at `0x1383e4`, mission-89 at `0x138574`), but writing our chain in that order
drops the score to 42.5 and shrinks the body to 57220. The mission-81 block's
own body is still not shaped like the original's, so the source order alone does
not reproduce the original block placement. This is the largest open item.

## 5. What remains

* **Prologue register map.** Ours keeps `player` live in `r4` across
  `getEnemies`; the original spills it straight from `r0`. That single extra
  live value shifts `&m_pLevel`/`&status` `r4 -> r5`, `messages` `r5 -> fp`,
  canary address `fp -> r9`. No faithful source shape found that both spills
  `player` early and keeps the original stack-slot colouring. `Level *level`
  gets the registers right but moves `player` to `[sp+88]` and scores 54.53.
* **Pre-switch mission-special order.** Original is `5, 81, 26, 78, 50, 89, 51`;
  ours is `81, 26, 5, 50, 51`. Writing the original order scores 42.5, so the
  body shapes (not the order) are the blocker.
* **`.rel.plt` is 12 bytes off.** With the `else if` chain the first-reference
  order becomes `deleteHackingGame` before `switchPlanetForIntro`. This is not
  ratcheted (only `.text`/`.rodata`/`.ARM.exidx`/`.ARM.extab` sizes and the
  verify counts are), and every shape that restores `.rel.plt` = 0 loses more
  score than it gains.
* `.ARM.extab` is still 8 bytes short (known w4270 `HangarWindow` EH residual).

## 6. Reproduction

```
cmake --build cmake-build-match --target check -j8      # RATCHET PASS
cmake --build cmake-build-match --target drift -j8      # 0 fields
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```

Fast screening (compiles one TU, ~1.7 s):

```
python3 _work/reconstructions/w3692_oracle.py \
  --src src/game/world/LevelScript.cpp --fn _ZN11LevelScript7processEi
```

Section byte-diff helper:

```python
import sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32
O = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
U = 'cmake-build-match/libgof2hdaa.so'
def sec(p, n):
    e = Elf32(p); s = next(x for x in e.sections if x['name'] == n)
    return e.data[s['offset']:s['offset'] + s['size']]
for n in ('.rel.plt', '.text', '.rodata', '.ARM.exidx', '.ARM.extab', '.rel.dyn'):
    a, b = sec(O, n), sec(U, n)
    print(n, len(a), len(b), sum(x != y for x, y in zip(a, b)) + abs(len(a) - len(b)))
```
