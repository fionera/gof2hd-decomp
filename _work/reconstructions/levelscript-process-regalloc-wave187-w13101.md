# LevelScript::process — wave 187 / w13101: prologue register map + adjacent-pair sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13101`, branch `wave187-w13101`, base master
`8b5d4281c` (wave-186 harvest 6, raw **51,968**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13101`.

This pass dumps both prologues, maps every callee-saved register and every
spilled slot to its owning source variable, re-probes the top-level
declaration order / `Status *` lifetime, and then runs a fresh **full
adjacent-statement-pair sweep** (1,892 candidates) on the current body.
**One size-exact, `.rel.plt`-identical raw win landed**: ten independent
adjacent swaps compose to raw **51,968 -> 51,949 (-19)**.

```
                         base (HEAD)      this pass
process raw diffbytes    51,968           51,949     (-19)
process aligned insns    20,856           20,910     (+54)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,727  20,830 / 20,727
.text diffbytes          74,274           74,255     (-19)
.ARM.extab diffbytes     647              647        (size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx diffbytes     0                0
.rodata                  0                0          (size/base exact 269,159 B @ 0x1c9e10)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.data / .bss / .comment  0 / 0 / 0        0 / 0 / 0
.note.gnu.build-id       20               20
total differing bytes    74,941           74,922     (-19)
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
137d5a  6804        ldr  r4, [r0,#0]         ; &Globals::status -> r4 (reuses &m_pLevel's r4)
137d5c  6820        ldr  r0, [r4,#0]         ; Status* value   -> r0 (reloaded per use)
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
137d28  48e5        ldr  r0, [pc,#916]       ; &__stack_chk_guard GOT slot
137d2a  465c        mov  r4, fp              ; r4 = this
137d2c  460e        mov  r6, r1              ; delta           -> r6
137d2e  4478        add  r0, pc
137d30  f8d0 a000   ldr.w sl, [r0]           ; canary base     -> sl (r10)
137d34  f8da 0000   ldr.w r0, [sl]
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
137d56  48db        ldr  r0, [pc,#876]
137d58  4478        add  r0, pc
137d5a  f8d0 9000   ldr.w r9, [r0]           ; &Globals::status -> r9
137d5e  f8d9 4000   ldr.w r4, [r9]           ; Status* value   -> r4 (cached local)
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
| `&Globals::status` | **r4** (reuses r4 after `&m_pLevel` dies) | **r9** | `Globals::status` |
| cached `Status *` | — (reloads `[r4]` per use) | **r4** | `Status *status` local |
| frame pointer | r7 | r7 | compiler |

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

The frame is 0x1b0 in both and the stack layout is byte-identical.

## 4. Why the prologue map is not reachable from source

The residual is a whole-function live-interval permutation.  In the original
the transient `&m_pLevel` copy occupies r4 only across the three prologue
getters, then `&Globals::status` reuses r4; the `Status*` value is reloaded
`ldr r0,[r4]` at every use.  In ours the `Status *status = Globals::status;`
local creates an extra whole-function interval, so the greedy allocator
assigns `&status` to r9 and caches the `Status*` value in r4.

Re-probed on the current body (all size-exact unless noted):

| variant | raw | size | prologue |
|---|---:|---:|---|
| base `Status *status = Globals::status;` | **51,968** | 57,232 | this=fp, canary=sl, &status=r9, value=r4 |
| no local (both dispatches `Globals::status->`) | 51,969 | 57,232 | this=fp, canary=sl, &status=r9, value r0 |
| `Status *&status` | 51,969 | 57,232 | same as no local |
| `Status *const status` | 51,968 | 57,232 | same as base |
| `status` declared first (before getters) | 55,360 | 57,320 | re-laid frame |
| `status` declared second | 55,361 | 57,320 | re-laid frame |
| `status` declared third | 55,212 | 57,288 | re-laid frame |

Removing the cached value does not move `&status` to r4: the seven
whole-function intervals (`this`, canary, `&m_pLevel`, `messages`, `delta`,
`enemies`, `&status`) still fill `r4,r5,r6,r8,r9,r10,r11`; r4 is simply
reused by the transient `&m_pLevel` copy.  The original has the same seven
values but reuses r4 for `&status` because its `Status*` value is transient.
Any source change that removes the cached `Status*` also changes the body
(`ldr.w r0,[r9]` -> `ldr r0,[r4]` at ~169 status uses), so matching the
prologue map is a body-size change, not a 6-byte prologue edit.

## 5. Adjacent-statement-pair sweep (the win)

A full line-based sweep of the `process` body (1,892 adjacent simple-statement
pairs at equal indentation) was built and measured with `tools/fn_diff.py`.
Eighteen pairs are size-exact, `.rel.plt`-identical and raw-lowering.  Ten of
them are semantically independent (a declaration move, or two setters on
disjoint state) and compose cleanly; the other eight change evaluation order
(e.g. `m_pGeometry6 = intro;` vs `m_pGeometry6->setPosition(...)`,
`camPos.y += 800.0f;` vs `m_pCamera->setPosition(camPos);`) and were left out.

Landed (1-based base line numbers; swap moves the first statement below the
second):

| # | line | moved statement | across |
|---:|---:|---|---|
| 1 | 1368 | `PlayerFighter *fighter = (PlayerFighter *) (*enemies)[0];` | `m_nState = 2;` |
| 2 | 1371 | `(*enemies)[0]->awake();` | `(*enemies)[0]->setVisible(true);` |
| 3 | 1729 | `m_pCamera->setPosition(camPos);` | `int routePoints[3] = {...};` |
| 4 | 4575 | `e->setActive(false);` | `e->setVisible(false);` |
| 5 | 4579 | `m_pCamera->setTarget(player->geometry);` | `tv = player->getPosition();` |
| 6 | 4629 | `m_pCamera->setTarget(player->geometry);` | `tvA += Vector{-350.0f, 400.0f, -1500.0f};` |
| 7 | 4632 | `m_pCamera->setPosition(tvA);` | `m_pLevel->lodManager->forceUpdate(delta, false);` |
| 8 | 4675 | `player->setSpeed(0.02f);` | `tv += Vector{150.0f, 400.0f, -1500.0f};` |
| 9 | 5015 | `player->setSpeed(4.0f);` | `player->setPosition(*m_pCamera->getPosition() + Vector{5000.0f, 300.0f, -14000.0f});` |
| 10 | 5245 | `(*enemies)[0]->geometry->setPosition(m_pGeometry4->getPosition());` | `m_pCamera->setTarget((*enemies)[0]->geometry);` |

Each is a pure reorder of independent statements; the emitted instruction
stream changes only in scheduling/register pressure.  The combination is
worth **-19 raw bytes** (`51,968 -> 51,949`), size stays exactly 57,232 and
`.rel.plt` stays byte-identical.  The aligned instruction count moves
20,856 -> 20,910 (+54); raw is the per-function gate metric used by the fleet.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13101
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51949 aligned=20910 size= 57232/57232 insn=20727/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`sweep_adj.py`, `combine.py`, `each_safe.py`,
`measure.py`, `mi.txt`, `greedy.txt`, `vrrw_process.txt`) live in
`/home/debian/GalaxyOnFire2/scratch/w13101/`; they are scratch, not tracked.
