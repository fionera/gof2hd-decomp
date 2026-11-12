# LevelScript::process — wave 188 / w13201: register-allocation pass (prologue map + same-receiver adjacent sweep)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13201`, branch `wave188-w13201`, base master
`ce16f45f1` (wave-187 harvest 5, raw **51,930**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13201`.

**Wave-189 rebase (master 235d3aa67, 2026-11-12).** The same five reorders
apply cleanly on top of the wave-189 body (which already contains the three
comparison-operand swaps). Measured on current master: raw **51,914 -> 51,900
(-14)**, aligned **20,936 -> 20,963**, size 57,232/57,232, `.rel.plt`
byte-identical, `.ARM.extab` content unchanged 639. `.text` differing bytes
**74,039 -> 74,032 (-7)**; total differing bytes **74,698 -> 74,691**. The
numbers below are the original wave-188 measurement (51,930 -> 51,921); the
rebase is the landed form.

This pass dumps both prologues, maps every callee-saved register and every
spilled slot to its owning source variable, re-probes the top-level
declaration order / `Status *` lifetime / `Level *` aliases, and then runs a
**new adjacent-pair sweep restricted to pairs of calls on the same receiver**
(the wave-186/187 d1 sweeps deliberately excluded same-receiver call pairs
because their data-flow model treats them as conflicting).  **Nine size-exact,
`.rel.plt`-identical statement reorders compose to raw 51,930 -> 51,921 (-9)**.

```
                         base (HEAD)      this pass
process raw diffbytes    51,930           51,921     (-9)
process aligned insns    20,928           20,932     (+4)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          74,223           74,214     (-9)
.ARM.extab diffbytes     639              639        (size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx               0                0
.rodata                  0                0          (size/base exact 269,159 B @ 0x1c9e10)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.data / .bss / .comment  0 / 0 / 0        0 / 0 / 0
.note.gnu.build-id       20               20
file size                2,187,328        2,187,328  (exact)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
stub_zero_size           0                0
missing / wrong_type     0 / 0            0 / 0
extra                    0                0
RATCHET                  PASS             PASS
```

## 1. Prologue dumps (image base 0x10000)

Original:

```
137d18  b5f0        push {r4,r5,r6,r7,lr}
137d1a  af03        add  r7, sp, #12
137d1c  e92d 0f80   stmdb sp!, {r7,r8,r9,sl,fp}
137d20  ed2d 8b0c   vpush {d8-d13}
137d24  b0ec        sub  sp, #432
137d26  4682        mov  sl, r0              ; this            -> sl (r10)
137d28  48de        ldr  r0, [pc,#888]       ; &__stack_chk_guard GOT slot
137d2a  4654        mov  r4, sl              ; r4 = this
137d2c  460e        mov  r6, r1              ; delta           -> r6
137d2e  4478        add  r0, pc
137d30  f8d0 b000   ldr.w fp, [r0]           ; canary base     -> fp (r11)
137d34  f8db 0000   ldr.w r0, [fp]
137d38  906b        str  r0, [sp,#428]       ; canary value    -> [sp,#428]
137d3a  f854 0f18   ldr.w r0, [r4,#24]!      ; &m_pLevel       -> r4, [sp,#80]
137d3e  f72a e8d8   blx  Level::getMessages  ; messages        -> r5, [sp,#72]
137d42  4605        mov  r5, r0
137d44  6820        ldr  r0, [r4,#0]
137d46  f72a e976   blx  Level::getPlayer    ; player          -> [sp,#84]
137d4a  e9cd 4014   strd r4, r0, [sp,#80]
137d4e  6820        ldr  r0, [r4,#0]
137d50  f72a e8b0   blx  Level::getEnemies   ; enemies         -> r8, [sp,#96]
137d54  4680        mov  r8, r0
137d56  48d4        ldr  r0, [pc,#848]
137d58  4478        add  r0, pc
137d5a  6804        ldr  r4, [r0,#0]         ; &Globals::status -> r4, [sp,#92]
137d5c  6820        ldr  r0, [r4,#0]         ; Status* value, NOT cached
137d5e  f729 ed08   blx  Status::getCurrentCampaignMission
```

Ours (base):

```
137d18  b5f0        push {r4,r5,r6,r7,lr}
137d1a  af03        add  r7, sp, #12
137d1c  e92d 0f80   stmdb sp!, {r7,r8,r9,sl,fp}
137d20  ed2d 8b0c   vpush {d8-d13}
137d24  b0ec        sub  sp, #432
137d26  4683        mov  fp, r0              ; this            -> fp (r11)
137d28  48e5        ldr  r0, [pc,#916]
137d2a  465c        mov  r4, fp
137d2c  460e        mov  r6, r1
137d2e  4478        add  r0, pc
137d30  f8d0 a000   ldr.w sl, [r0]           ; canary base     -> sl (r10)
137d34  f8da 0000   ldr.w r0, [sl]
137d38  906b        str  r0, [sp,#428]
137d3a  f854 0f18   ldr.w r0, [r4,#24]!      ; &m_pLevel       -> r4, [sp,#80]
137d3e  f72a e8d8   blx  Level::getMessages  ; messages        -> r5, [sp,#72]
137d42  4605        mov  r5, r0
137d44  6820        ldr  r0, [r4,#0]
137d46  f72a e976   blx  Level::getPlayer    ; player          -> [sp,#84]
137d4a  e9cd 4014   strd r4, r0, [sp,#80]
137d4e  6820        ldr  r0, [r4,#0]
137d50  f72a e8b0   blx  Level::getEnemies   ; enemies         -> r8, [sp,#96]
137d54  4680        mov  r8, r0
137d56  48db        ldr  r0, [pc,#876]
137d58  4478        add  r0, pc
137d5a  f8d0 9000   ldr.w r9, [r0]           ; &Globals::status -> r9, [sp,#92]
137d5e  f8d9 4000   ldr.w r4, [r9]           ; Status* value cached in r4
137d62  4620        mov  r0, r4
137d64  f729 ed04   blx  Status::getCurrentCampaignMission
```

## 2. Callee-saved register map

| value | original | ours (base) | source variable |
|---|---|---|---|
| `this` | **sl (r10)** | **fp (r11)** | implicit `this` |
| canary base (`&__stack_chk_guard`) | **fp (r11)** | **sl (r10)** | compiler stack protector |
| `delta` | r6 | r6 | parameter `int delta` |
| `messages` | r5 | r5 | `Array<RadioMessage *> *messages` |
| `enemies` | r8 | r8 | `Array<KIPlayer *> *enemies` |
| `&m_pLevel` | r4 (pre-inc `ldr.w r0,[r4,#24]!`) | r4 (same) | `this->m_pLevel` address |
| `&Globals::status` | **r4**, then spilled to `[sp,#92]` and reloaded | **r9**, kept live | `Globals::status` |
| cached `Status *` | — (reloads `[r4]`/`[sp,#92]` per use) | **r4** | `Status *status` local (used twice) |

## 3. Spilled slots (identical in both)

| slot | value | source variable |
|---|---|---|
| `[sp,#72]` | `messages` | `Array<RadioMessage *> *messages` |
| `[sp,#76]` | `delta` | parameter |
| `[sp,#80]` | `&m_pLevel` | `this->m_pLevel` address |
| `[sp,#84]` | `player` | `PlayerEgo *player` |
| `[sp,#88]` | `this` | implicit `this` |
| `[sp,#92]` | `&Globals::status` | `Globals::status` |
| `[sp,#96]` | `enemies` | `Array<KIPlayer *> *enemies` |
| `[sp,#428]` | canary value | stack protector |

Frame 0x1b0 in both; stack layout is byte-identical.

## 4. Why the prologue map is not reachable from source (re-confirmed)

The original does **not** keep `&Globals::status` in a callee-saved register:
it uses r4 only for the first dispatch, spills it to `[sp,#92]`, and every
later `Globals::status->` use is `ldr r0,[sp,#92]; ldr r0,[r0]` (two 2-byte
instructions).  Ours keeps `&Globals::status` in r9 and emits
`ldr.w r0,[r9]` (one 4-byte instruction) — same byte count per use, but a
different encoding.  The prologue is 6 bytes longer because of
`ldr.w r9,[r0]; ldr.w r4,[r9]; mov r0,r4` (10 B) vs
`ldr r4,[r0]; ldr r0,[r4]` (4 B), which shifts the mission-0 block from
`0x137e84` to `0x137e8a`.

Re-probed on the current 51,930 body (all size-exact unless noted):

| variant | raw | size | prologue |
|---|---:|---:|---|
| base `Status *status = Globals::status;` | **51,930** | 57,232 | this=fp, canary=sl, &status=r9, value=r4 |
| no local (both dispatches `Globals::status->`) | 51,931 | 57,232 | this=fp, canary=sl, &status=r9, value r0 |
| `Status *&status` | 51,931 | 57,232 | same as no local |
| `Status *const status` | 51,930 | 57,232 | same as base |
| `Status **status = &Globals::status` | 51,931 | 57,232 | same as no local |
| `Status *volatile status` | 55,107 | 57,240 | re-laid frame |
| local only at first dispatch | 51,931 | 57,232 | same as base |
| local only at second dispatch | 51,930 | 57,232 | same as base |
| `LevelScript *self = this;` for the getters | 51,930 | 57,232 | CSE'd, identical |
| `Level *level = m_pLevel;` first 1 use | 51,930 | 57,232 | identical |
| `Level *level = m_pLevel;` first 2..257 uses | 55,053..55,616 | 57,152..57,392 | this=**sl** or r6, but size-changing |

24 top-level declaration permutations: base `MPES` is still best; `EPMS`
51,934, `MEPS`/`PMES` 51,942, all size-exact ones keep `this=fp`.
`Level *level = m_pLevel` is the only lever that flips `this` to sl, and it
always changes the frame (57,152..57,392), so it cannot be landed alone.
Adding a null check (`if ((intptr_t)this == 0) return false;`) also flips
`this` to sl but costs +204 B.  The prologue residual is therefore a
whole-function live-interval permutation, not a local declaration-order
effect.

## 5. New sweep: same-receiver adjacent call pairs (the win)

The wave-186/187 adjacent-pair sweeps used a data-flow model that treats two
calls on the same receiver as conflicting, so pairs such as
`player->setFreeze(...)` / `player->setVisible(...)` were never measured.
This pass enumerated all 1,765 adjacent single-line statement pairs in
`process`, selected the **354** pairs where both lines call the same receiver
(`m_pCamera`, `player`, `(*enemies)`, `Globals::sound`, `Globals::layout`,
`m_pLevel`, `Globals::status`, …), swapped each, and measured the exact
NDK r18b `-Oz` build with a fast ELF reader.  Four pairs lower raw by 1 B;
two of them compose.

Landed (source lines from the base):

| # | line | before | after |
|---:|---:|---|---|
| 1 | 3344 | `player->setFreeze(false);` then `player->setVisible(true);` | visible first |
| 2 | 5302 | `revive(); turnEnemy(); setPosition(...); setActive(true);` | `revive(); setPosition(...); setActive(true); turnEnemy();` |

Both are pure reorders of independent calls:
* `PlayerEgo::setFreeze` and `PlayerEgo::setVisible` write different flags on
  the same `PlayerEgo`; neither reads the other.
* `KIPlayer::revive` is `PlayerFighter::revive` for these ships, which resets
  the ship and re-applies `turnEnemy()` iff the player was already turned
  enemy (it samples `turnedEnemy()` before `reset()`), so moving
  `turnEnemy()` after `revive()` is idempotent; `setPosition`/`setActive`
  do not touch the player relation.  The resulting order also matches the
  original's emitted `turnEnemy`-last order at `0x145856`.

Combination: raw **51,930 -> 51,928 (-2)**, size stays exactly 57,232,
`.rel.plt` stays `289b2a06…`, `.text` 74,223 -> 74,221.

## 5b. Pass 2: `Globals::status` / `m_pLevel` adjacent pairs

A second adjacent-pair sweep restricted to pairs where either line mentions
`Globals::status` or `m_pLevel` (363 candidates) found three further
composing size-exact winners.  All three move a
`m_pLevel->lodManager->forceUpdate(...)` call across an independent
`m_nState` / camera-mode statement:

| # | base line | before | after | raw |
|---:|---:|---|---|---:|
| 3 | 1240 | `forceUpdate(30,false);` then `m_nState = 12;` | `m_nState = 12;` first | -4 |
| 4 | 4633 | `m_pCamera->setFixed(false);` then `forceUpdate(delta,false);` | `forceUpdate` first | -2 |
| 5 | 4679 | `camera->setPosition(tv);` then `forceUpdate(delta,false);` | `forceUpdate` first | -1 |

`forceUpdate` touches only the LOD manager; `m_nState`, `setFixed` and
`setPosition` touch disjoint objects/fields, so every move is
semantics-preserving.  Composed with the two pass-1 swaps the total is
**51,930 -> 51,921 (-9)**, size 57,232 exact, `.rel.plt`
`289b2a06…`, `.text` 74,223 -> 74,214.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13201
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51921 aligned=20932 size= 57232/57232 insn=20725/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51921 size=57232/57232 text=74214 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`fastraw.py`, `sweep_adj.py`, `sweep.py`, `try.py`,
`v_*.py`, `mi.txt`, `orig_body.txt`, `base_body.txt`) live in
`/home/debian/GalaxyOnFire2/scratch/w13201/`; they are scratch, not tracked.
