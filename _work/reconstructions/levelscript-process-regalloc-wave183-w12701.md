# LevelScript::process — wave 183 / w12701: callee-saved / spill register-allocation pass

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12701`, branch `wave183-w12701`, base master
`9555474d8` (wave-182 harvest 5, raw **52,412**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12701`.

This pass dumps both prologues, maps every callee-saved register and every
spilled slot to the source variable that owns it, and then sweeps the
top-level declaration order / lifetime / scope of the `messages`, `player`,
`enemies` and `status` locals (plus the block-scope `Status *st` and
`Level *self/lvl` locals) to try to make clang 7 reproduce the original
allocation.  **Result: no size-exact, `.rel.plt`-identical raw win.**  The
base body is already the best measured shape; `src/` is unchanged.

```
                         base (HEAD)      this pass
process raw diffbytes    52,412           52,412     (no change)
process aligned insns    22,050           22,050
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,728
.text diffbytes          75,015           75,015
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
RATCHET                  PASS             PASS
```

## 1. Prologue dumps

Original (`objdump -d`, image base 0x10000):

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
137d5c  6820        ldr  r0, [r4,#0]         ; reload *status per use
137d5e  f729 ed08   blx  Status::getCurrentCampaignMission
```

Ours (base):

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
137d5e  f8d9 4000   ldr.w r4, [r9]           ; Status* cached in r4 (source local)
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

## 4. Root cause: 4-byte prologue excess from `&Globals::status` in r9

The first `0x48` bytes differ by exactly **12 bytes**:

```
0x137d26  mov sl,r0            vs mov fp,r0             (register bit)
0x137d28  ldr r0,[pc,#888]     vs ldr r0,[pc,#916]      (literal offset)
0x137d2a  mov r4,sl            vs mov r4,fp             (register bit)
0x137d33  ldr.w fp,[r0]        vs ldr.w sl,[r0]         (register bit)
0x137d34  ldr.w r0,[fp]        vs ldr.w r0,[sl]         (register bit)
0x137d56  ldr r0,[pc,#848]     vs ldr r0,[pc,#876]      (literal offset)
0x137d5a  ldr r4,[r0]          vs ldr.w r9,[r0]         (2 vs 4 bytes)
0x137d5c  ldr r0,[r4]          vs ldr.w r0,[r9]         (2 vs 4 bytes)
```

The `{this, canary}` swap is **length-neutral** (both are `mov`/`ldr.w`
forms).  The `&Globals::status` load, however, is 4 bytes longer in ours:
the original uses the low register r4 (`ldr r4,[r0]` = 2 B, `ldr r0,[r4]`
= 2 B), ours uses the high register r9 (`ldr.w r9,[r0]` = 4 B,
`ldr.w r0,[r9]` = 4 B).  This 4-byte excess shifts the `beq.w` target of
the mission==0 block from `0x137e84` (original) to `0x137e8a` (ours); the
remaining 2 bytes come from the mission==1 branch body.  Everything after
the prologue is therefore 6 bytes out of alignment, which is why the raw
diff is dominated by address-shifted-but-equivalent instructions.

The original also does **not** cache the `Status *` value: every use is
`ldr r0,[r4]` (48 reloads of `[sp,#92]`).  The base caches it in r4
(25 reloads of `[sp,#92]`); removing the local makes clang reload it, but
still puts `&Globals::status` in r9.

## 5. Sweeps tried (all size-exact unless noted)

### 5.1 Top-level declaration order (24 permutations of messages/player/enemies/status)

Base order `M P E S` is best at **52,412**.  Every other order is worse or
size-changing:

| order | raw | size |
|---|---:|---:|
| **MPES (base)** | **52,412** | 57,232 |
| MPSE | 55,041 | 57,260 |
| MEPS | 52,424 | 57,232 |
| MESP | 55,267 | 57,244 |
| MSPE | 55,377 | 57,312 |
| MSEP | 55,336 | 57,312 |
| PMES | 52,424 | 57,232 |
| PMSE | 55,041 | 57,260 |
| PEMS | 52,426 | 57,232 |
| PESM | 52,436 | 57,232 |
| PSME | 55,377 | 57,312 |
| PSEM | 52,439 | 57,232 |
| EMPS | 52,426 | 57,232 |
| EMSP | 55,267 | 57,244 |
| EPMS | 52,416 | 57,232 |
| EPSM | 52,432 | 57,232 |
| ESMP | 55,369 | 57,328 |
| ESPM | 52,436 | 57,232 |
| SMPE | 55,376 | 57,312 |
| SMEP | 55,335 | 57,312 |
| SPME | 55,374 | 57,312 |
| SPEM | 52,452 | 57,232 |
| SEMP | 55,368 | 57,328 |
| SEPM | 52,451 | 57,232 |

### 5.2 `status` local form / lifetime

| form | raw | size | prologue |
|---|---:|---:|---|
| `Status *status = Globals::status;` (base) | 52,412 | 57,232 | this=fp, canary=sl, &status=r9, val=r4 |
| no local (`Globals::status->` directly) | 52,413 | 57,232 | this=fp, canary=sl, &status=r9, reload |
| `Status *&status = Globals::status;` | 52,413 | 57,232 | same as no-local |
| `Status &status = *Globals::status;` | 52,412 | 57,232 | &status=r9, val=r4 |
| `Status **status = &Globals::status;` | 52,413 | 57,232 | &status=r9, reload |
| `Status **status = &Globals::status;` + all uses `(*status)->` | 52,413 | 57,232 | &status=r9, reload |
| `Status *status = *&Globals::status;` | 52,412 | 57,232 | same as base |
| `Status *const status = …` | 52,412 | 57,232 | same as base |
| `Status *const volatile status = …` | 55,157 | 57,240 | reallocated |
| local for first condition only, global for second | 52,413 | 57,232 | &status=r9 |
| local for second condition only (`status2`) | 52,412 | 57,232 | &status=r9 |
| all 100 `Globals::status->` through the local | 54,986 | 57,228 (-4) | caches value |

No form moves `&Globals::status` into r4 or flips `{this,canary}`.

### 5.3 Scope merges / splits

* Four block-scope `Status *st = Globals::status;` (body lines 1345, 1408,
  1485, 2873) replaced by `Globals::status->` or the function-scope `status`:
  raw 54,370 (size 57,216) and 55,410 (size 57,316) — worse.
* 14 block-scope `Level *self/lvl = m_pLevel;` hoisted to one function-scope
  `Level *lvl2`: raw 55,475, size 57,412 — worse.
* Hoisting one `Level *self/lvl` at a time (14 variants) with a function-scope
  `lvl2`: raw 55,293–55,538, size 57,280–57,328 — all worse.
* Long-lived aliases `Level *`, `Radar *`, `Hud *`, `TargetFollowCamera *`
  (first+last use): raw 55,033–55,518, size 56,780–57,328 — all worse and
  size-changing.
* `Array<KIPlayer *> *en2 = enemies;` first+last use: neutral 52,412,
  size 57,232 (no allocation change).
* `Level *lvl2 = m_pLevel;` used only for the first `m_pLevel->` (messages):
  neutral 52,412, size 57,232 (coalesced, no allocation change).
* `Level *lvl2 = m_pLevel;` used for the last `m_pLevel->`: 55,332,
  size 57,312 (frame grows 432 -> 440).

### 5.4 Mission==1 statement permutation (24 orders of forceUpdate / rotate / getPosition / setToSleep)

The original order is **F A G S** (forceUpdate, rotate, getPosition,
setToSleep).  The base source order is **A S G F**.  All 24 permutations
were measured; the base order `ASGF` is the unique best at **52,412**.
`FAGS` (the original order) is 52,416; `AFSG`/`AGFS`/`AGSF`/`ASFG` are
52,413.  Matching the original semantic order therefore does **not** lower
the same-address raw metric because the prologue's 4-byte excess keeps the
mission==0 block 6 bytes out of alignment.

## 6. Conclusion

The original and our body use the same function-scope locals, the same
spill slots and the same slot-to-variable mapping.  The residual is a
**global greedy-allocator tie-break**:

* `this` (`gprnopc`) vs canary base (`gpr`) swap between `{sl,fp}` — length
  neutral, but changes 4 register-bit bytes.
* `&Globals::status` (`gpr`) lands in r9 instead of r4 — costs 4 extra
  prologue bytes and shifts the whole function by 6 bytes.

The tie-break is driven by the whole-function live-interval/interference
graph, not by the top-level declaration order or by the `status` local's
form: every size-exact declaration order and every `status` form measured
here keeps the same base map (`this=fp, canary=sl, &status=r9`), and every
source lever that moves `&Globals::status` out of r9 (long-lived aliases)
also grows the frame by 8 bytes and loses more raw than the 4-byte prologue
saving.  No size-exact, `.rel.plt`-identical raw win was found; `src/` is
left at the base.

The remaining lever is the body itself: if the mission==0 block and the
giant switch were byte-aligned to the original, the 6-byte prologue shift
would disappear.  That is the same body-layout work the wave-181/182
statement sweeps are doing, not a declaration-order change.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12701
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52412 aligned= 22050 size= 57232/57232 insn= 20728/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52412 size=57232/57232 text=75015 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Prologue dumps and the machine-instr dump used for the analysis live in
`/home/debian/GalaxyOnFire2/scratch/w12701/` (`orig_full.txt`,
`ours_full.txt`, `mi_base.txt`); they are scratch, not tracked.
