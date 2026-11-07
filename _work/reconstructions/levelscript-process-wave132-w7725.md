# LevelScript::process — wave 132 / w7725 (prologue register-map hypothesis: declaration order, local caching, first-use order)

Status: **note only, no source change landed, 2026-11-07**. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 normalised instructions. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `144fc16b0` (wave-132 w7705 set).

## Result

```
                         before (master)   after (no code change)
raw ratio (difflib)      59.200000         59.200000
orig / ours insns        20169 / 20047     20169 / 20047
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
process .text diffbytes  54507             54507
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams (`/dev/shm/w7725/score.py`).
`process .text diffbytes` is the byte-diff count over the function's own 57232-byte
range (`/dev/shm/w7725/score.py`).

## The prologue difference, exactly

Original (`0x137d26..0x137d72`):

```
137d3a  ldr.w r0,[r4,#24]!        ; r4 = &this->m_pLevel, r0 = m_pLevel
137d3e  blx   Level::getMessages
137d42  mov   r5,r0               ; messages -> r5
137d44  ldr   r0,[r4,#0]
137d46  blx   Level::getPlayer
137d4a  strd  r4,r0,[sp,#80]      ; &m_pLevel, player
137d4e  ldr   r0,[r4,#0]
137d50  blx   Level::getEnemies
137d54  mov   r8,r0               ; enemies -> r8
137d56  ldr   r0,[pc,#848]        ; GOT slot of Globals::status
137d58  add   r0,pc
137d5a  ldr   r4,[r0,#0]          ; r4 = &Globals::status
137d5c  ldr   r0,[r4,#0]          ; r0 = Globals::status
137d5e  blx   Status::getCurrentCampaignMission
137d62  cmp   r0,#0
137d64  str.w sl,[sp,#88]         ; this
137d68  strd  r4,r8,[sp,#92]      ; &Globals::status, enemies
137d6c  mov   r0,r8
137d6e  strd  r5,r6,[sp,#72]      ; messages, delta
137d72  beq.w 137e84
```

Master (baseline):

```
137d3a  ldr.w r0,[r4,#24]!
137d3e  blx   Level::getEnemies   ; <-- enemies first
137d42  mov   r8,r0
137d44  ldr   r0,[r4,#0]
137d46  blx   Level::getPlayer
137d4a  strd  r4,r0,[sp,#80]
137d4e  ldr   r0,[r4,#0]
137d50  blx   Level::getMessages  ; <-- messages last
137d54  mov   r4,r0               ; messages -> r4 (r4 free after the spill)
137d56  ldr   r0,[pc,#884]
137d58  add   r0,pc
137d5a  ldr.w r9,[r0]             ; &Globals::status -> r9 (r4 taken)
137d5e  ldr.w r0,[r9]
137d62  blx   Status::getCurrentCampaignMission
137d66  cmp   r0,#0
137d68  str.w sl,[sp,#88]
137d6c  strd  r4,r6,[sp,#72]      ; messages, delta (spilled first)
137d70  mov   r0,r8
137d72  strd  r9,r8,[sp,#92]      ; &Globals::status, enemies
137d76  beq.w 137e8e
```

So the map is a direct consequence of the **initialisation order**:

* `messages` is called last, after `strd r4,r0,[sp,#80]` has killed the
  `&this->m_pLevel` pointer in r4, so the allocator hands `messages` r4.
* `enemies` is r8 in both (it is the first call, so r4 is still live).
* `&Globals::status` then cannot use r4 and goes to r9 (32-bit `ldr.w`).

In the original, `messages` is called **first**, while r4 still holds
`&this->m_pLevel`; it is therefore forced to r5, and the later
`&Globals::status` reuses the now-dead r4 (16-bit `ldr r4,[r0]`).

## Measured variants (all built and scored; source restored after each)

Baseline: `59.200000`, 57232 B, 20047 insns, bytediff 54507.

| variant | raw % | size | insns | bytediff |
|---|---:|---:|---:|---:|
| **baseline** `enemies,player,messages` | **59.200000** | **57232** | **20047** | **54507** |
| `messages,player,enemies` (faithful call order) | 58.400000 | 57408 | 20031 | 55520 |
| `messages,enemies,player` | 58.400000 | 57408 | 20032 | 55532 |
| `player,messages,enemies` | 58.400000 | 57408 | 20032 | 55532 |
| `player,enemies,messages` | 58.900000 | 57304 | 20055 | 55385 |
| `enemies,messages,player` | 58.600000 | 57320 | 20018 | 55437 |
| baseline + `Status *status = Globals::status;` (after the 3 locals) | 58.600000 | 57320 | 20018 | 55426 |
| baseline + `Status *status` before the 3 locals | 58.400000 | 57408 | 20032 | 55561 |
| baseline + `Status *&status = Globals::status;` | 59.200000 | 57232 | 20047 | 54507 (byte-identical) |
| baseline + `Status **statusSlot = &Globals::status;` | 59.200000 | 57232 | 20047 | 54507 (byte-identical) |
| messages-first + `Status *status` after | 58.400000 | 57408 | 20032 | 55519 |
| messages-first + `Status *&status` before | 58.400000 | 57408 | 20031 | 55520 |
| messages-first + `player->rotate` in mission-1 | 56.000000 | 57420 | 20049 | 55387 |
| baseline + `player->rotate` in mission-1 | 57.200000 | 57324 | 20028 | 55382 |

Declaration order alone is a no-op: `messages; enemies; player;` declarations with the
**same initialisation order** compile byte-identically to the plain declarations
(D1–D4 sweep, all 59.200000 / 57232 / 54507). Only the order of the three
initialiser expressions changes the prologue.

## Why the faithful order costs +176 B

The reordered build reproduces the original prologue almost exactly — `messages` r5,
`enemies` r8, the `strd &status,enemies` / `strd messages,delta` spill order — but it
does **not** reuse r4 for `&Globals::status` (still `ldr.w r9`), and the
`messages/delta` stack pair is swapped (`strd r6,r5,[sp,#72]` vs `strd r5,r6`).

The size cost is in the body, not the prologue. With `messages` first, the compiler no
longer folds the mission-1 body's `m_pLevel->getPlayer()->rotate(...)` into the spilled
`player` local; it emits a fresh `Level::getPlayer()` call:

```
original / baseline:  137da6  ldr   r0,[sp,#84]   ; player local
reordered:            137daa  ldr.w r0,[sl,#24]
                      137dba  blx   Level::getPlayer
```

That re-colouring then propagates through the whole 57 KB function: the reordered build
has fewer instructions (20031 vs 20047) but 176 more bytes because 105 more instructions
widen from 16-bit to 32-bit:

| build | 16-bit insns | 32-bit insns | size |
|---|---:|---:|---:|
| original | 13044 | 7786 | 57232 |
| baseline | 12808 | 7904 | 57232 |
| messages-first | 12682 | 8011 | 57408 |

The original is the most 16-bit-heavy of the three. Its compactness comes from the
body's low-register allocation, which our incomplete body (122 fewer instructions than
the original) does not reproduce.

## Local caching

`Status *status = Globals::status;` does not reproduce the original. The original caches
the **address** of the global (`r4 = &Globals::status`, then `ldr r0,[r4]` per call);
a `Status *` local caches the **value** and adds an extra `ldr.w r4,[r9]` + `mov r0,r4`.
A `Status *&` reference and a `Status **` slot both canonicalise back to the plain
`Globals::status->` form (byte-identical to baseline), so the allocator still chooses r9.

## Conclusion / next lead

The faithful source order is `messages, player, enemies` (the emitted call order is
authoritative); the master order `enemies, player, messages` is a reconstruction
artefact. The reorder fixes 2 of the 3 prologue registers (`messages` r5, `enemies` r8)
and the spill order, but it cannot be landed at 57232 B until the body is complete:
the +176 B is the mission-1 `getPlayer()` re-colouring plus the resulting 16→32-bit
widening, not the prologue itself. The remaining r4→r9 difference for
`&Globals::status` is a backend temporary whose allocation is not controllable from the
source in this build (all caching spellings tested leave it in r9).

Concrete next levers:
1. Complete the ~122 missing instructions of the body first; the faithful
   `messages,player,enemies` order should then be size-neutral (and is the right source).
2. Once the body is complete, re-check whether the mission-1 `m_pLevel->getPlayer()`
   folds back into the `player` local; that fold is what keeps the prologue compact.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7725/score.py                         # 59.200000, 54507
python3 /dev/shm/w7725/widths.py                        # 16/32-bit instruction split
```

Scratch helpers in `/dev/shm/w7725/` (not tracked): `base.cpp`, `score.py`,
`widths.py`, `diffstreams.py`, `perm.py`, `variants.py`, `variants2.py`,
`variants3.py`, `variants4.py`, `base.ll`, `reord.ll`.
