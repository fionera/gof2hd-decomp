# LevelScript::process — wave 136 / w8104: the m,p,e prologue hypothesis, re-tested on the current base

Status: **research note, no landed source change**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w8104`. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Owned TU: `src/game/world/LevelScript.cpp`.
Base master `2e51d765d` (wave-135 docs head; the function itself last changed
in `8770f4f9b`, wave-135 case-125 switch). Scratch helpers in `/dev/shm/w8104/`
(not tracked): `score.py`, `prologue.py`, `diffblocks.py`, `diffsizes.py`,
`diffbytes.py`, `variant.sh`, `sweep.py`, `sweep2.py`, `LevelScript.base.cpp`,
plus ~80 variant `.cpp` files.

## Result

```
                         before (master)   after (no code change)
official verify pct      58.8              58.8
orig / ours insns        20169 / 20047     20169 / 20047
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        54316             54316
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
```

## 1. The wave-135 premise is stale: the baseline `m,p,e` reorder already has `player` at sp+84

The mission said "reordered variants put `player` at sp+72". On the current
base that is **false**. Declaring the three accessor locals in the original
order `messages, player, enemies` (`F`) already produces the full original
prologue through `getEnemies`:

```
ORIGINAL (0x137d18)                         F = m,p,e (57392 B)
137d3a ldr.w r0,[r4,#24]!                   ldr.w r0,[r4,#24]!
137d3e blx   Level::getMessages  ; r5        blx   Level::getMessages  ; r5   MATCH
137d44 ldr   r0,[r4,#0]                      ldr   r0,[r4,#0]
137d46 blx   Level::getPlayer                blx   Level::getPlayer
137d4a strd  r4,r0,[sp,#80]  ; &m_pLevel@80, player@84
                                             strd  r4,r0,[sp,#80]              MATCH
137d4e ldr   r0,[r4,#0]                      ldr   r0,[r4,#0]
137d50 blx   Level::getEnemies ; r8          blx   Level::getEnemies ; r8      MATCH
137d56 ldr   r0,[pc,#848]                    ldr   r0,[pc,#888]
137d5a ldr   r4,[r0,#0]    ; status -> r4    ldr.w r9,[r0]      ; status -> r9 DIFF
137d5e ldr   r0,[r4,#0]                      ldr.w r0,[r9]
137d64 str.w sl,[sp,#88]                     str.w sl,[sp,#88]
137d68 strd  r4,r8,[sp,#92]                  strd  r9,r8,[sp,#92]
137d6c mov   r0,r8                           mov   r0,r8
137d6e strd  r5,r6,[sp,#72]                  strd  r6,r5,[sp,#72]   ; pair swapped
```

So `F` reproduces **five of the six** prologue differences (accessor order,
messages r5, `&m_pLevel` sp+80, player sp+84, `strd` pairing). Only `status`
(r9 vs r4) and the `messages`/`delta` slot order remain. `F` is **57392 B
(+160)**, not the +180 the wave-135 note recorded on its older base.

## 2. The +160 is a body re-colouring, not the prologue

Byte-size diff between the original and `F`, accumulated over the difflib
diff blocks (`diffsizes.py`): the first ~220 instructions (prologue + the
whole mission-1 block) accumulate only **+6 B**. The +160 appears later as
the body's 16/32-bit mix shifts:

| metric | original | baseline `e,p,m` | `F` = `m,p,e` |
|---|---:|---:|---:|
| 16-bit insns | 13044 | 12798 | 12682 |
| 32-bit insns | 7786 | 7909 | 8007 |
| total bytes | 57232 | 57232 | 57392 |

`F` has 141 fewer instructions but 160 more bytes: the m,p,e reorder frees
`r4` as the body scratch and pushes more values into 32-bit encodings.

## 3. `status` in r4 cannot be forced from source

MIR evidence (`-mllvm -print-after-all`, Virtual Register Rewriter dump of
`F`, `/dev/shm/w8104/mir.F.txt`):

```
%12286:gpr  (&m_pLevel) -> r4      live 112B..320B (killed by getEnemies)
%12331:gpr  (messages)  -> r5
%12137:rgpr (delta)     -> r6
%11744:gpr  (enemies)   -> r8
%11441     (this)       -> r10 (sl)
%11718:gpr  (Status*)   -> r9      loaded 432B, spilled to stack.678 at 624B
```

`r4` is **free from 320B to 1136B** — it does not interfere with the status
live range — yet the greedy allocator picks `r9`. The original picks `r4`.
This is a global greedy-allocator tie-break (spill-weight order), not a
local source-shape property. The status value is short-lived (2 uses before
the spill), so its weight is low and the allocator does not give it the
"first" callee-saved register.

Tested source shapes (all built with the real project flags and linked):

| variant | size | pct | note |
|---|---:|---:|---|
| baseline `e,p,m` | **57232** | **58.8** | master |
| `F` = `m,p,e` | 57392 | 58.7 | player sp+84, status r9 |
| `F` + line-947 `player->rotate` (`G`) | 57412 | 56.2 | re-colours (this→r5) |
| `F` + line-1786 fix | 57384 | 58.6 | |
| `F` + line-3247 fix | 57384 | 58.6 | |
| `F` + both | 57376 | 58.6 | |
| `F` + all 3 getPlayer fixes | 57396 | 56.2 | |
| `F` + cached `PlayerEgo *const` | 57392 | 58.7 | identical to `F` |
| `F` declare-then-assign / `=nullptr` | 57392 | 58.7 | identical to `F` |
| `F` + `Status *status` local (all uses) | 57216 | 50.3 | full re-colour |
| `F` + `Status *status` local (dispatch only) | 57432 | 46.1 | full re-colour |
| `F` + `Level *level` local | 57392 | 58.6 | |
| `emp` (enemies,messages,player) | 57308 | **58.9** | best pct, not size-exact |
| `emp` + line-1786 | 57300 | 58.9 | |
| `pem` (player,enemies,messages) | 57292 | 58.5 | |
| `C_epm` = `e,p,m` + `Level *level` for the 3 calls | **57232** | 58.7 | size-exact, worse |

**Declaration placement and a cached `PlayerEgo *` local are no-ops**: all
six permutations of `{messages, player, enemies}` with the `m,p,e`
initialiser order, `PlayerEgo *player; ... player = ...;`,
`PlayerEgo *player = nullptr; ...`, and `PlayerEgo *const player` compile to
the identical 57392 B / 58.7 % as `F`. The only levers that move the
prologue are the **call order** (which `F` already has) and a **body use of
the cached `player` local** (line 947), which re-colours the whole function.

## 4. Size must be exactly 57232 — a measured constraint

A +76 B variant (`emp`) drops `byte_exact` from **4506 to 2741** when fully
verified: growing `process` shifts `resetCamera` (the next symbol at
`0x145ca8`) and every later function, so the raw bytes of thousands of
PC-relative functions change. Any candidate must therefore stay at exactly
57232 B.

## 5. Size-exact semantic fixes all make the function worse

The known real semantic gaps (the three extra `getPlayer` calls and the
missing `m_pCamera->translate(-10000, 5000, 25000)` after the mission-1
`setPosition` at `0x13a4b8`) were applied in every combination that lands on
57232 B. Every one **increases** the process diffbytes above the baseline's
54316:

| variant | size | pct | diffbytes |
|---|---:|---:|---:|
| baseline `e,p,m` | 57232 | 58.8 | **54316** |
| + line-1786 fix | 57224 | 58.6 | 55132 |
| + line-3247 fix | 57224 | 58.6 | 55109 |
| + line-690 `player->setCollide` | 57224 | 58.6 | 55174 |
| + line-1470 `player->player->setVulnerable` | 57224 | 58.6 | 55209 |
| + line-2811 `player->setAutoPilot` | 57224 | 58.6 | 55057 |
| + line-947 `player->rotate` | 57336 | 57.6 | 55442 |
| + missing `translate(-10000,5000,25000)` | 57256 | 58.6 | 55012 |
| `translate` + 1786 + 3247 + 690 (size-exact) | 57232 | 58.7 | 54411 |
| `translate` + 690 + 1470 + 2811 (size-exact) | 57232 | 58.7 | 54484 |

The baseline is a sharp local optimum in diffbytes. The body is still
missing large blocks (the biggest single gap is **1092 B** at original
instruction 14322, plus 750/676/668/642/566 B blocks), so adding correct
code misaligns the block layout faster than it fixes it.

## 6. Conclusion

The wave-134/135 "player stack slot" hypothesis is **falsified on the
current base**:

1. The `m,p,e` reorder already produces `player` at sp+84 and the
   `strd r4,r0,[sp,#80]` pairing; `player`'s slot is not the missing piece.
2. The remaining prologue delta is `status` r9 vs r4 (a global greedy
   allocation tie-break) and the `messages`/`delta` slot order.
3. `status` r4 cannot be forced from source: declaration placement, a cached
   `PlayerEgo *` local, a cached `Status *` local, and a `Level *` local all
   either do nothing or re-colour the whole body.
4. The +160 B is body re-colouring, not the prologue (first 220 instructions
   are +6 B).
5. No size-exact variant beats 58.8 %; the best size-exact alternative is
   58.7 %. `emp` reaches 58.9 % but is +76 B and breaks `byte_exact`.

The real lever remains completing the ~1000+ bytes of missing body code; the
denser `player`/`status` reload pattern should then move `status` to `r4`
for free and make the `m,p,e` order size-neutral.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8104/score.py            # 58.8, 57232/57232
python3 /dev/shm/w8104/diffbytes.py        # 54316
python3 /dev/shm/w8104/prologue.py _ZN11LevelScript7processEi 34
python3 /dev/shm/w8104/diffsizes.py        # per-diff-block byte deltas
```
