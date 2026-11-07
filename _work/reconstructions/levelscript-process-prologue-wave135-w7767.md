# LevelScript::process — wave 135 / w7767: the prologue hypothesis is falsified

Status: **research note, no landed source change**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7767`. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Owned TU: `src/game/world/LevelScript.cpp`.
Base master `bbffef482` (wave-134 head). Scratch helpers in `/dev/shm/w7767/`
(not tracked): `score.py`, `regcheck.py`, `prologue.py`, `batch.py`,
`variant.sh`, `LevelScript.base.cpp`, plus ~150 variant `.cpp` files.

## Result

```
                         before (master)   after (no code change)
raw difflib ratio        58.166538         58.166538
official verify pct      58.2              58.2
orig / ours insns        20169 / 20050     20169 / 20050
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The wave-134 note's open question was: find a source shape that keeps the
original prologue accessor order `messages, player, enemies`, the
`strd r4,r0,[sp,#80]` pairing and `status` in `r4`, without the +180 B
downstream growth; the note guessed the missing piece was `player`'s stack
slot (original sp+84, reordered variants sp+72).

**That guess is wrong.** The baseline already has `player` at **sp+84** and
already emits the `strd r4,r0,[sp,#80]` pairing. The +184 B is entirely the
`status` register: the original keeps the `Status *` value in **r4** (16-bit
encodable), ours in **r9** (32-bit `ldr.w`/`str.w`), and the source has 105
`Globals::status->` uses. `player`'s slot is not the lever.

## 1. The baseline prologue already matches the stack layout

Original (`0x137d18..0x137d72`):

```
137d3a  ldr.w r0,[r4,#24]!        ; r4 = &this->m_pLevel
137d3e  blx   Level::getMessages  ; r5 = messages
137d44  ldr   r0,[r4,#0]
137d46  blx   Level::getPlayer
137d4a  strd  r4,r0,[sp,#80]      ; &m_pLevel @80, player @84
137d4e  ldr   r0,[r4,#0]
137d50  blx   Level::getEnemies   ; r8 = enemies
137d56  ldr   r0,[pc,#848]
137d58  add   r0,pc
137d5a  ldr   r4,[r0,#0]          ; r4 = Globals::status (Status*)
137d5e  ldr   r0,[r4,#0]
137d60  blx   Status::getCurrentCampaignMission
137d64  str.w sl,[sp,#88]         ; this @88
137d68  strd  r4,r8,[sp,#92]      ; status @92, enemies @96
137d6c  mov   r0,r8
137d6e  strd  r5,r6,[sp,#72]      ; messages @72, delta @76
```

Baseline (`e,p,m` init order):

```
137d3a  ldr.w r0,[r4,#24]!
137d3e  blx   Level::getEnemies   ; r8 = enemies   <-- order differs
137d44  ldr   r0,[r4,#0]
137d46  blx   Level::getPlayer
137d4a  strd  r4,r0,[sp,#80]      ; &m_pLevel @80, player @84  (MATCH)
137d4e  ldr   r0,[r4,#0]
137d50  blx   Level::getMessages  ; r4 = messages  <-- r4, not r5
137d56  ldr   r0,[pc,#884]
137d58  add   r0,pc
137d5a  ldr.w r9,[r0]             ; r9 = Globals::status       <-- r9, not r4
137d5e  ldr.w r0,[r9]
137d60  blx   Status::getCurrentCampaignMission
137d64  str.w sl,[sp,#88]
137d68  strd  r4,r6,[sp,#72]      ; messages @72, delta @76   (MATCH)
137d6c  mov   r0,r8
137d6e  strd  r9,r8,[sp,#92]      ; status @92, enemies @96
```

**Every stack slot is identical** — messages 72, delta 76, `&m_pLevel` 80,
player 84, this 88, status 92, enemies 96. The only differences are the two
registers: `messages` r4 vs r5 and `status` r9 vs r4.

## 2. `messages, player, enemies` + the line-947 `player` use

The faithful order alone (`F`) spills `player` at **sp+72** and `&m_pLevel` at
sp+84 (two separate `str`s), because the cached `player` local is not used
until much later. Adding the line-947 fix
`player->rotate(0.0f, angle, angle)` (the original loads `[sp,#84]` at
`0x137da6`) extends `player`'s live range across `getPlayer`, so the compiler
re-forms the pair. Variant `I` = `F` + all three `getPlayer` fixes:

```
137d3a  ldr.w r0,[r4,#24]!
137d3e  blx   Level::getMessages  ; r5 = messages   (MATCH)
137d44  ldr   r0,[r4,#0]
137d46  blx   Level::getPlayer
137d4a  strd  r4,r0,[sp,#80]      ; &m_pLevel @80, player @84  (MATCH)
137d4e  ldr   r0,[r4,#0]
137d50  blx   Level::getEnemies   ; r8 = enemies   (MATCH)
137d56  ldr   r0,[pc,#884]
137d58  add   r0,pc
137d5a  ldr.w r9,[r0]             ; r9 = status    <-- only remaining diff
137d5e  ldr.w r0,[r9]
137d60  blx   Status::getCurrentCampaignMission
137d64  str.w sl,[sp,#88]
137d68  strd  r9,r8,[sp,#92]
137d6c  mov   r0,r8
137d6e  strd  r6,r5,[sp,#72]      ; delta @72, messages @76 (pair swapped)
```

So `I` reproduces **4 of the 6** prologue differences (accessor order, messages
r5, player sp+84, `&m_pLevel` sp+80). It is 2 instructions from the original:
`status` r9 vs r4 and the messages/delta pair order. But it is **57416 B
(+184)**, and the body re-colours.

Variant table (all built with the real project flags and linked):

| variant | size | insns | raw % | note |
|---|---:|---:|---:|---|
| baseline `e,p,m` | **57232** | 20050 | **58.1665** | master |
| `F` = `m,p,e` | 57432 | 20054 | 57.1514 | player sp+72 |
| `G` = `m,p,e` + line-947 fix | 57416 | 20047 | 57.6388 | player sp+84, status r9 |
| `I` = `m,p,e` + all 3 getPlayer fixes | 57408 | 20047 | 57.6089 | player sp+84, status r9 |
| `I` + status local (prologue only) | 57408 | 20048 | 57.6075 | no change |
| `I` + `Status *&status` used everywhere | 57408 | 20047 | 57.6089 | no change |
| `I` + `PlayerEgo *const player` | 57408 | 20047 | 57.6089 | no change |
| `I` + status local used everywhere | 57068 | 19902 | 40.63 | full re-colour |
| `I` + `Level *level` used everywhere | 57152 | 20032 | 51.36 | full re-colour |
| `I` + status local first+last use | 57452 | 20049 | 47.13 | status r4, but re-coloured |

## 3. The +184 B is exactly the `status` register

The body has **105** `Globals::status->` uses. In `I` the `Status *` value is
in `r9`, so every access is `ldr.w r0,[r9]` / `str.w` (4 B); in the original it
is in `r4`, so every access is `ldr r0,[r4]` (2 B). `105 × 2 ≈ 210 B`, which
matches the +184 after scheduling. The `messages`/`delta` pair swap is a
2-byte-neutral stack-slot colouring difference, not a size difference.

MIR evidence (`-mllvm -print-after-all`, `Virtual Register Rewriter` dump of
`I`): the `Status *` value is allocated to `$r9`; `$r4` is free from the
`strd` spill at `0x137d4a` until the `Globals::layout` scratch at `0x137e56`.
The original's allocator instead gives the `Status *` value `r4` and uses `r9`
for the scratch. This is a **global greedy-allocator tie-break**, not a local
source-shape property.

## 4. Why the allocator differs: the body is incomplete

| metric | original | baseline | `I` |
|---|---:|---:|---:|
| `[sp,#84]` (player) reloads | **163** | 92 | 92 |
| `[sp,#92]` (status) reloads | **49** | 23 | 23 |
| `[sl,#24]` (m_pLevel) reloads | **40** | 91 | 91 |

The original's body reloads `player` 163 times and `status` 49 times; ours 92
and 23. The original's body is 119 instructions longer and 16-bit-heavier
(13044/7786 vs ours ~12800/7900). The allocator's choice of `r4` for `status`
is a consequence of that denser body, not of declaration placement.

## 5. Declaration placement and cached `PlayerEgo *` are no-ops

All of the following compile to the same 57408 B / 57.61 % as `I`:

* all 6 declaration permutations of `{messages, player, enemies}` with the
  `m,p,e` initialiser order;
* declare-then-assign (`PlayerEgo *player; ... player = ...;`);
* `PlayerEgo *player = 0; player = m_pLevel->getPlayer();`;
* `PlayerEgo *const player`;
* `Status *&status = Globals::status;` (compiles identically);
* `this->m_pLevel->` prefix on the accessor calls.

The only things that move the prologue are the **call order** and a **body use
of the cached `player` local** (line 947).

## 6. The `Level *level` lever (new, but not size-neutral)

A `Level *level = m_pLevel;` local used for the three accessor calls **and one
body use** flips `messages` to `r5` (matching the original) and raises the
fuzzy score, but costs +92 B:

| variant | size | insns | raw % |
|---|---:|---:|---:|
| `C` = `level` for 3 calls only (`e,p,m`) | **57232** | 20051 | 58.0806 |
| `C3` = `C` + `level->lodManager->forceUpdate(delta,true)` (mission 1) | 57324 | 20028 | **58.3228** |
| `C3` + remove `delta<0` anchor (-80) | 57244 | 20000 | 58.3335 |
| `C3` + remove `delta<0` + autoTurret anchor (-96) | 57228 | 19994 | **58.3572** |
| `C3` + remove `delta<0` + auto + standing (-112) | 57212 | 19988 | 58.3460 |
| `C3` + remove `delta<0` + auto + standing, add missing `translate(-10000,5000,25000)` at `0x13a4b8` | 57252 | 20001 | 58.3321 |

`C3`'s `messages` is `r5` (matches) but `status` is still `r9`, so the +92 is
the `messages`-in-r5 body re-colour, not the target +184.

The known anchors are exactly **-80** (`delta<0` block), **-16** (auto-turret)
and **-16** (standing), so `C3 - 80 = 57244` and `C3 - 96 = 57228`; neither is
57232. Removing the anchors changes `.rel.plt` and drops `byte_exact`
(measured: `AE` = baseline + missing translate + no auto/stand anchors is
57232 B but `byte_exact 4503`; `X3` = `C3` - auto + reduced anchor - rgd is
57232 B but `byte_exact 4505`). The anchors are load-bearing for `.rel.plt`.

**Any `.text` size change is fatal even at 57232 B** if it moves `.rel.plt`:
a 4-byte size change shifts `.rodata`/`.data` and drops `byte_exact` from 4506
to 4327 (measured on `Q4` = 57228 B). This is why the function must stay at
exactly 57232 B *and* keep the anchor first-reference order.

## 7. Conclusion

The wave-134 "player stack slot" hypothesis is falsified:

1. The baseline already has `player` at sp+84 and the `strd r4,r0,[sp,#80]`
   pairing; every stack slot matches the original.
2. The only remaining prologue differences are `messages` r4→r5 and `status`
   r9→r4.
3. `m,p,e` + line-947 achieves `messages` r5 and `player` sp+84, but `status`
   stays r9; the +184 B is entirely the `status` register (105 uses × 2 B).
4. `status` r4 cannot be forced from source without re-colouring the whole
   body (status local → 40.6 %); it is a global allocator consequence of the
   incomplete (119-instruction-short) body.
5. The `Level *level` body-use lever raises the fuzzy score to 58.32–58.36 %
   but costs +92 B, and the available anchors cannot hit 57232 B without
   breaking `.rel.plt`/`byte_exact`.

No size-exact, PLT-preserving source change improved the function this wave.

## Open questions for the next wave

* Complete the ~119 missing body instructions; the original's denser
  `player`/`status` reload pattern should then move `status` to `r4` for free
  and make the faithful `m,p,e` order size-neutral.
* The missing `m_pCamera->translate(-10000.0f, 5000.0f, 25000.0f)` after the
  `setPosition((*enemies)[10]->getPosition() + Vector{6000,-200,1000})` at
  source line ~2295 (original `0x13a4b8`, pool `0x13a638/63c/640`) is a real
  semantic gap worth +32 B; it needs a -32 B companion that preserves
  `.rel.plt`.
* `Level *level` used in the body is the only lever found that flips
  `messages` to `r5`; it needs a faithful -92 B companion.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7767/score.py                          # 58.166538, 57232/57232
python3 /dev/shm/w7767/regcheck.py <variant.cpp>         # status register / slots
python3 /dev/shm/w7767/prologue.py <variant.cpp>         # prologue disasm
```
