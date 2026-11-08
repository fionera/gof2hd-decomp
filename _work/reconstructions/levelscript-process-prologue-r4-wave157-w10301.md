# LevelScript::process — wave 157 / w10301: prologue `&Globals::status` -> r4
# (new hypotheses; prologue not reachable size-exact, but a -1 raw c106 win landed)

Target `_ZN11LevelScript7processEi`, original `0x137d18`, **57,232 B (0xdf90)**,
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w10301`,
base master `08575efff` (wave 156).  Scratch `/dev/shm/w10301/`.

## Result (landed: c106 state-6 `Status *st` for the armor store)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,095            55,094            (-1)
process aligned insns    17,131            17,131            (0)
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## 0. The landed source change

In the mission-106 state-6 block, declare `Status *st = Globals::status;` and
use it for the armor-HP store only:

```cpp
Player *pp = player->player;
Status *st = Globals::status;
Globals::status->field_64 = pp->getHitpoints();
Globals::status->field_5c = pp->getShieldHP();
st->field_60 = pp->getArmorHP();       // <- local here
Globals::status->field_68 = pp->getGammaHP();
```

This is a pure codegen lever (the original reloads the status pointer for
every store).  It keeps the function size-exact at 57,232 B and the prologue
unchanged (`this`=fp, canary=sl, `&status`=r9), but drops raw by 1 byte.
`st` for field_60+field_68 (`cd`) measures the same; `st` for all four
stores is -8 B (57,224) and raw +119.

## 1. The exact prologue / callee-saved map (deliverable)

Read directly from objdump of both binaries (image base 0x10000):

```
ORIGINAL 0x137d18                          OURS (HEAD) 0x137d18
mov sl, r0        ; this -> sl             mov fp, r0        ; this -> fp
ldr r0,[pc]; add r0,pc                     ldr r0,[pc]; add r0,pc
mov r4, sl        ; r4 = this              mov r4, fp        ; r4 = this
mov r6, r1        ; delta -> r6            mov r6, r1        ; delta -> r6
ldr.w fp,[r0]     ; fp = &__stack_chk_guard ldr.w sl,[r0]    ; sl = &__stack_chk_guard
ldr.w r0,[fp]                              ldr.w r0,[sl]
str r0,[sp,#428]  ; canary                 str r0,[sp,#428]
ldr.w r0,[r4,#24]! ; r4 = this+24 = &m_pLevel   same
blx getMessages                            same
mov r5, r0        ; messages -> r5          same
ldr r0,[r4]                                same
blx getPlayer                              same
strd r4,r0,[sp,#80] ; &m_pLevel, player     same
ldr r0,[r4]                                same
blx getEnemies                             same
mov r8, r0        ; enemies -> r8           same
ldr r0,[pc]; add r0,pc                     same
ldr r4,[r0,#0]    ; r4 = &Globals::status   ldr.w r9,[r0]    ; r9 = &Globals::status
ldr r0,[r4,#0]    ; r0 = Globals::status    ldr.w r4,[r9]    ; r4 = Globals::status
blx getMission                             mov r0,r4; blx getMission
cmp r0,#0                                  same
str.w sl,[sp,#88] ; this                    str.w fp,[sp,#88] ; this
strd r4,r8,[sp,#92] ; &status, enemies      strd r9,r8,[sp,#92] ; &status, enemies
mov r0,r8                                  same
strd r5,r6,[sp,#72] ; messages, delta       same
beq.w 0x137e84                             beq.w 0x137e8a
ldr r0,[r4,#0]    ; reload status ptr      mov r0,r4         ; reuse cached value
blx getMission                             blx getMission
```

| value | original | ours (HEAD) |
|---|---|---|
| `this` | `sl` (r10) | `fp` (r11) |
| `&__stack_chk_guard` | `fp` (r11) | `sl` (r10) |
| `&Globals::status` | **`r4`** (spilled to `[sp,#92]`) | **`r9`** (spilled to `[sp,#92]`) |
| `Globals::status` value | reloaded from `[r4]` each use | cached in `r4` once (the `status` local CSEs the two dispatch loads) |
| `messages` / `delta` / `enemies` | r5 / r6 / r8 | r5 / r6 / r8 (match) |
| `&m_pLevel` | `[sp,#80]` | `[sp,#80]` (match) |
| `player` | `[sp,#84]` | `[sp,#84]` (match) |
| `this` spill slot | `[sp,#88]`, reloaded **328x** | `[sp,#88]`, reloaded **157x** |
| `&Globals::status` slot | `[sp,#92]`, reloaded **49x** | `[sp,#92]`, reloaded **25x** |

Frame 0x1b0 in both.  So the residual is a pure permutation:
`this`<->canary and `r4`<->`r9`, plus the original's second dispatch reloads
the status pointer (`ldr r0,[r4,#0]`) while ours reuses the cached value
(`mov r0,r4`).

## 2. New hypotheses measured this wave (all built + linked)

`local` = how `Globals::status` is spelled; `alias` = `Level *self = m_pLevel`
form.  Size must stay 57,232.

| # | variant | size | raw | aligned | prologue (this/canary/status) |
|---|---|---:|---:|---:|---|
| 0 | HEAD | 57232 | 55095 | 17131 | fp / sl / r9 |
| 1 | no local (all `Globals::status`) | 57232 | 55096 | 17139 | fp / sl / r9 |
| 2 | `Status *&status` ref, all uses | 57232 | 55096 | 17139 | fp / sl / r9 |
| 3 | `Status **statusPtr=&Globals::status`, all uses | 57232 | 55096 | 17139 | fp / sl / r9 |
| 4 | local only for first dispatch (w10201) | 57232 | 55096 | 17139 | fp / sl / r9 |
| 5 | `Level *self` value alias, whole fn | 57048 | 55146 | 19085 | **sl / fp** / r9 |
| 6 | `Level *&self` ref alias, whole fn | 57248 | 55189 | 17163 | fp / sl / r9 |
| 7 | `Status *status` local, all uses | 57124 | 55218 | 18269 | r8 / fp / r4 |
| 8 | #7 + `Level *self` value alias | 56796 | 54911 | 22162 | fp / r9 / sl |
| 9 | #7 + `Level *&self` ref alias | 57140 | 55292 | 18451 | r8 / — / r4 |
| 10 | `Level *self` accessors only, no local | 57232 | 55330 | 17153 | fp / sl / r9 |
| 11 | `Level *self` accessors only, partial local | 57232 | 55332 | 17180 | fp / sl / r9 |
| 12 | `Level *self` value alias after decls, partial local | 57040 | 55299 | 18615 | r9 / fp / r4 |
| 13 | `expect(mission==106,1)` | 57228 | 55083 | 18266 | (function start shifts -0xd0) |
| 14 | `expect(mission==106,0)` | 57244 | 55406 | 18656 | — |
| 15 | `expect(m_nState==6,0)` | 57244 | 55273 | 17148 | — |
| 16 | `expect(m_nState==6,1)` | 57232 | 55095 | 17131 | fp / sl / r9 (no-op) |
| 17 | `Status *st` for 4 field stores in c106 s6 | 57224 | 55214 | 17127 | — |
| 18 | `Status *st` for all c106 s6 status uses | 57172 | 55096 | 17849 | — |
| 19 | **landed: `Status *st` for c106 s6 field_60 only** | **57232** | **55094** | **17131** | fp / sl / r9 |
| 20 | `Status *st` for c106 s6 field_60+field_68 | 57232 | 55094 | 17131 | fp / sl / r9 |
| 21 | `Status *st` for all four c106 s6 stores | 57224 | 55214 | 17127 | — |

Full 24-permutation sweep of the four prologue declarations with the
all-local `Status *status` (messages/player/enemies/status) was also run:
sizes 56,940-57,124, **none** size-exact; best raw 55,006 (order
messages,player,status,enemies) but aligned 18,402.

## 3. Findings

1. **`&Globals::status` never lands in r4 in any size-exact shape.**  The
   compiler's greedy allocator gives r4 to `&m_pLevel` (1 def + 4 uses in the
   entry block, spill weight 5) and only then considers `&Globals::status`
   (weight ~3.x), so it takes r9.  The original's r4 assignment therefore
   comes from a whole-function allocation where `&m_pLevel` is spilled
   immediately (it is: `[sp,#80]`, reloaded 97x) and `this` wins `sl` over
   the canary.
2. **The `Level *self` value alias is the only lever found that flips
   `this`->sl / canary->fp** (variant 5).  It does so by removing the
   `&m_pLevel` live range, but `self` itself then takes r4, so
   `&Globals::status` still goes to r9, and the alias costs -184 B.
3. **The all-local `Status *status` is the only lever that puts the status
   value in r4** (variant 7), but it re-colours `this` to r8 and `messages`
   to sl, and costs -108 B.
4. **`__builtin_expect` on the mission-106 guard is the only expect that
   improves raw** (variant 13, -12 raw) but it moves the function start by
   -0xd0 and wrecks alignment (+1,135), and is not size-exact (-4 B).
5. The original's `this` is spilled/reloaded 328x vs our 157x, and
   `&Globals::status` 49x vs our 25x.  Matching the prologue therefore
   requires the *whole-function* spill pattern, not a local source shape.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10301/fdiff.py _ZN11LevelScript7processEi        # 55,095 / 57,232
python3 /dev/shm/w10301/align_diff.py _ZN11LevelScript7processEi  # 17,131
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

Scratch harnesses: `/dev/shm/w10301/{harness,gen,variant,perm,sweep_perm,sig,exp106,micro,micro2}.py`.

## 5. Open questions

1. A source shape that makes `&m_pLevel`'s entry-block live range dead
   *before* `&Globals::status` is defined, without introducing a `self`
   value that itself takes r4, would let `&status` reuse r4.  The alias
   forms tried all give r4 to the alias.
2. `Level *&self = m_pLevel` keeps the member address but still leaves
   `&status` in r9 and costs +16 B.
3. A +60 B faithful cold companion would make the all-c106-s6 `Status *st`
   variant (variant 18, -60 B) size-exact, but its aligned is 17,849.
