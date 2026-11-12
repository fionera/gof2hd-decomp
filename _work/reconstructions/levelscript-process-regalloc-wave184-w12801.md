# LevelScript::process — wave 184 / w12801: deep register-allocation pass

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12801`, branch `wave184-w12801`, base master
`4c06c2662` (wave-183 harvest 6, raw **52,281**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12801`.

This pass dumps both prologues, maps every callee-saved register and every
spilled slot to its owning source variable, and then sweeps the top-level
declaration order/lifetimes (`messages`, `player`, `enemies`, `status`), the
`Status *` form, and block-scope `Status *` / `Level *` / `Player *` aliases.
It also re-runs the existing-`__builtin_expect` flip/strip sweep on the
current body.  **One size-exact, `.rel.plt`-identical raw win landed** (the
case-142 expect strip); the prologue allocation itself is not reachable from
source without a size change.

```
                         base (HEAD)      this pass
process raw diffbytes    52,281           52,280     (-1)
process aligned insns    20,700           20,701     (+1)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,725  20,830 / 20,725
.text diffbytes          74,842           74,841     (-1)
.ARM.exidx               0                0          (byte-identical)
.ARM.extab               646              646        (size/base exact 156,312 B @ 0x1a3b78)
.rodata                  0                0          (size/base exact 269,159 B @ 0x1c9e10)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.data / .bss / .comment  0 / 0 / 0        0 / 0 / 0
.note.gnu.build-id       20               20
total differing bytes    75,508           75,507
file size                2,187,328        2,187,328      (exact)
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
137d26  4682        mov  sl, r0              ; this          -> sl (r10)
137d28  48de        ldr  r0, [pc,#888]       ; &__stack_chk_guard GOT slot
137d2a  4654        mov  r4, sl              ; r4 = this
137d2c  460e        mov  r6, r1              ; delta         -> r6
137d2e  4478        add  r0, pc
137d30  f8d0 b000   ldr.w fp, [r0]           ; canary base   -> fp (r11)
137d34  f8db 0000   ldr.w r0, [fp]
137d38  906b        str  r0, [sp,#428]       ; canary value  -> [sp,#428]
137d3a  f854 0f18   ldr.w r0, [r4,#24]!      ; &m_pLevel     -> r4, [sp,#80]
137d3e  f72a e8d8   blx  Level::getMessages  ; messages      -> r5, [sp,#72]
137d42  4605        mov  r5, r0
137d44  6820        ldr  r0, [r4,#0]
137d46  f72a e976   blx  Level::getPlayer    ; player        -> [sp,#84]
137d4a  e9cd 4014   strd r4, r0, [sp,#80]
137d4e  6820        ldr  r0, [r4,#0]
137d50  f72a e8b0   blx  Level::getEnemies   ; enemies       -> r8, [sp,#96]
137d54  4680        mov  r8, r0
137d56  48d4        ldr  r0, [pc,#848]
137d58  4478        add  r0, pc
137d5a  6804        ldr  r4, [r0,#0]         ; &Globals::status -> r4, [sp,#92]
137d5c  6820        ldr  r0, [r4,#0]         ; Status* reloaded per use
137d5e  f729 ed08   blx  Status::getCurrentCampaignMission
```

Ours (base, unchanged in this pass):

```
137d18  b5f0        push {r4,r5,r6,r7,lr}
137d1a  af03        add  r7, sp, #12
137d1c  e92d 0f80   stmdb sp!, {r7,r8,r9,sl,fp}
137d20  ed2d 8b0c   vpush {d8-d13}
137d24  b0ec        sub  sp, #432
137d26  4683        mov  fp, r0              ; this          -> fp (r11)
137d28  48e5        ldr  r0, [pc,#916]
137d2a  465c        mov  r4, fp              ; r4 = this
137d2c  460e        mov  r6, r1              ; delta         -> r6
137d2e  4478        add  r0, pc
137d30  f8d0 a000   ldr.w sl, [r0]           ; canary base   -> sl (r10)
137d34  f8da 0000   ldr.w r0, [sl]
137d38  906b        str  r0, [sp,#428]
137d3a  f854 0f18   ldr.w r0, [r4,#24]!      ; &m_pLevel     -> r4, [sp,#80]
137d3e  f72a e8d8   blx  Level::getMessages  ; messages      -> r5, [sp,#72]
137d42  4605        mov  r5, r0
137d44  6820        ldr  r0, [r4,#0]
137d46  f72a e976   blx  Level::getPlayer    ; player        -> [sp,#84]
137d4a  e9cd 4014   strd r4, r0, [sp,#80]
137d4e  6820        ldr  r0, [r4,#0]
137d50  f72a e8b0   blx  Level::getEnemies   ; enemies       -> r8, [sp,#96]
137d54  4680        mov  r8, r0
137d56  48db        ldr  r0, [pc,#876]
137d58  4478        add  r0, pc
137d5a  f8d0 9000   ldr.w r9, [r0]           ; &Globals::status -> r9, [sp,#92]
137d5e  f8d9 4000   ldr.w r4, [r9]           ; Status* cached in r4
137d62  4620        mov  r0, r4
137d64  f729 ed04   blx  Status::getCurrentCampaignMission
```

The instruction sequence is identical; only the physical registers differ.
The frame is **432 bytes in both**, and the canary is at `[sp,#428]` in both.

## 2. Callee-saved register map

| value | original | ours (base) | source variable |
|---|---|---|---|
| `this` | **sl (r10)** | **fp (r11)** | implicit `this` |
| canary base (`&__stack_chk_guard`) | **fp (r11)** | **sl (r10)** | compiler stack protector |
| `delta` | r6 | r6 | parameter `int delta` |
| `messages` | r5 | r5 | `Array<RadioMessage *> *messages` |
| `enemies` | r8 | r8 | `Array<KIPlayer *> *enemies` |
| `&m_pLevel` | r4 (pre-inc `ldr.w r0,[r4,#24]!`) | r4 (same) | `this->m_pLevel` address |
| `&Globals::status` | **r4** | **r9** | `Globals::status` |
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

The stack layout is byte-identical.  The source-variable ownership of every
slot is identical.  Only the register assignment differs.

## 4. Root cause: greedy-allocator tie-break, not declaration order

The residual is a whole-function live-interval permutation:

* `this` (`gprnopc`) vs the canary base (`gpr`) swap between `{sl,fp}`.
  The swap is length-neutral (both are `mov`/`ldr.w` forms).
* `&Globals::status` lands in r9 instead of r4.  The original uses the low
  register r4 (`ldr r4,[r0]` = 2 B, `ldr r0,[r4]` = 2 B); ours uses the high
  register r9 (`ldr.w r9,[r0]` = 4 B, `ldr.w r0,[r9]` = 4 B), a 4-byte
  prologue excess.  The second dispatch adds the other 2 bytes (original
  reloads `ldr r0,[r4]`; ours reuses the cached value with `mov r0,r4`).
  The mission-0 branch target is therefore `0x137e8a` instead of `0x137e84`.

`-mllvm -print-machineinstrs` (Virtual Register Rewriter dump) shows the
entry block:

```
%12028:rgpr    = COPY $r1        ; delta
%11349:gprnopc = COPY $r0        ; this
%13161:gpr     = t2LDRi12 ...    ; canary base
%12169:gpr     = COPY %11349     ; transient &m_pLevel copy (r4)
%12211:gpr     = COPY $r0        ; messages
%11615:gpr     = COPY $r0        ; enemies
%11589:gpr     = t2LDRi12 ...    ; &Globals::status
```

After the greedy allocator: `this->r11`, canary->r10, `&status->r9`,
`%12169->r4`, messages->r5, delta->r6, enemies->r8.  The transient
`&m_pLevel` copy takes r4 first, and the whole-function `&status` vreg is
placed in r9.  In the original the same transient copy takes r4 but the
allocator reuses r4 for `&status` after the copy dies at `0x137d4e`.

The original also reloads the `Globals::status` address from `[sp,#92]`
43 times in the body; ours reloads it 24 times (it stays in r9).  The
original's `Status*` value is reloaded `[r4]` on every use; ours caches it
in r4 for the two dispatch tests.  This is a body-structure / register
pressure difference, not a declaration-order effect.

## 5. Sweeps tried (all size-exact unless noted)

### 5.1 Top-level declaration order (24 permutations of messages/player/enemies/status)

Base order `MPES` is still the unique best on the current body:

| order | raw | aligned | size |
|---|---:|---:|---:|
| **MPES (base)** | **52,281** | 20,700 | 57,232 |
| EPMS | 52,285 | 20,702 | 57,232 |
| MEPS | 52,293 | 20,701 | 57,232 |
| PMES | 52,293 | 20,704 | 57,232 |
| PEMS | 52,295 | 20,706 | 57,232 |
| EMPS | 52,295 | 20,706 | 57,232 |
| EPSM | 52,301 | 20,717 | 57,232 |
| PESM | 52,305 | 20,717 | 57,232 |
| ESPM | 52,305 | 20,718 | 57,232 |
| PSEM | 52,308 | 20,717 | 57,232 |
| SEPM | 52,320 | 20,719 | 57,232 |
| SPEM | 52,321 | 20,718 | 57,232 |

The other 12 orders are size-changing (57,244–57,312) and lose.  Every
size-exact order keeps `&Globals::status` in r9 (or r5); none reaches r4.

### 5.2 `status` local form / lifetime

| form | raw | size | prologue |
|---|---:|---:|---|
| `Status *status = Globals::status;` (base) | 52,281 | 57,232 | this=fp, canary=sl, &status=r9, value=r4 |
| no local (`Globals::status->` twice) | 52,282 | 57,232 | this=fp, canary=sl, &status=r9, value reloaded |
| all 97 body `Globals::status->` through the local | 55,469 | 57,228 (-4) | this=sl, canary=fp, &status=r9 |
| body-local, dispatch via `Globals::status` | 55,378 | 57,196 (-36) | re-laid frame |

Removing the cached value does not move `&status` to r4: r4 simply stays
free and `&status` still gets r9.

### 5.3 Block-scope `Level *` / `Status *` / `Player *` aliases

For every `{}` block in `process` with >=2 uses of `m_pLevel->`,
`Globals::status->`, or `player->player->`, a block-scope alias was inserted
and the uses rewritten.  No size-exact raw win:

* `Level *lvl = m_pLevel;` — 100+ candidate blocks; best size-exact was
  52,282 (line 3900 block), all others >= 52,289.
* `Status *st = Globals::status;` — best size-exact was 52,281 (neutral,
  line 2950 block), all others >= 52,291.
* `Player *pp = player->player;` — best size-exact was 52,297 (+16).

### 5.4 Long-lived aliases / parameter copies

`PlayerEgo *pl = player;`, `Array<KIPlayer *> *en2 = enemies;`,
`Array<RadioMessage *> *msgs = messages;`, `int deltaV = delta;`, and
`LevelScript *lsp = this;` all coalesce away and are bit-identical to the
base.  `Player *plr = player->player;` used everywhere is size-changing
(57,296, raw 55,260).  A function-scope `Level *lvl = m_pLevel;` used
everywhere is 57,076 (-156), raw 55,069.

### 5.5 Existing-`__builtin_expect` flip/strip sweep

`levelscript-process-expects-w11901.py` was re-run on the current body
(136 cells).  Exactly one size-exact raw winner was found:

| line | variant | raw | size | text |
|---:|---|---:|---:|---:|
| 4284 | strip | **52,280** | 57,232 | 74,841 |

## 6. Landed win (code)

Case 142 (`m_nState < 2` gas-cloud cargo collection, original `0x144e36`
region): strip the `__builtin_expect(..., 1)` around the
`VectorLength(e0A->getPosition() - e0A->getRoute()->getWaypoint(0)->getPosition()) < 3000.0f`
condition.  The plain condition is the better branch-weight/register shape
on the current body.

```cpp
// before
if (__builtin_expect((AbyssEngine::AEMath::VectorLength( // lint: arm_builtin
            e0A->getPosition() - e0A->getRoute()->getWaypoint(0)->getPosition())
    < 3000.0f), 1)) {
// after
if ((AbyssEngine::AEMath::VectorLength(
            e0A->getPosition() - e0A->getRoute()->getWaypoint(0)->getPosition())
    < 3000.0f)) {
```

Raw **52,281 -> 52,280** (-1), `.text` **74,842 -> 74,841**, size-exact
(57,232), `.rel.plt` byte-identical (`289b2a06…`), `.ARM.extab` unchanged
(646), all 27 section headers exact, file size exact.  The aligned count
moves 20,700 -> 20,701 (the raw/text win is the gate metric).

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12801
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52280 aligned=20701 size= 57232/57232 insn=20725/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52280 size=57232/57232 text=74841 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`sweep_decl.py`, `sweep_locals.py`, `sweep_alias_blocks.py`,
`sweep_alias_status.py`, `sweep_alias_pp.py`, `probe_*.py`, and the
`-print-machineinstrs` dumps) live in
`/home/debian/GalaxyOnFire2/scratch/w12801/`; they are scratch, not tracked.
