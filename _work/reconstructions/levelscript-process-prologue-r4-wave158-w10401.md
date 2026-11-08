# LevelScript::process — wave 158 / w10401: prologue `&Globals::status` -> r4
# (r4 lever confirmed: all-local `Status*` puts the status value in r4, but the
#  +108 B size companion is not reachable with the shapes measured this wave)

Target `_ZN11LevelScript7processEi`, original `0x137d18`, **57,232 B (0xdf90)**,
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w10401`,
base master `9e4795206` (wave 157).  Scratch `/dev/shm/w10401/`.

## Result (no source change landed; note only)

Baseline is unchanged and `check` is green.  The r4 hypothesis was tested
exhaustively; the all-local `Status*` shape does put the status pointer in
`r4`, but every shape that also reaches size 57,232 either re-colours the
whole prologue (losing the r4 win) or is 4+ bytes off.  Details below.

```
                         before (HEAD)     after (note only)
process raw diffbytes    55,094            55,094
process aligned insns    17,129            17,129
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## 1. Exact prologue / callee-saved map (deliverable)

Read from objdump of both binaries (image base 0x10000).  The original and
our HEAD are instruction-for-instruction identical in the prologue; only the
physical register numbers differ.

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
ldr r0,[r4,#0]    ; r0 = Globals::status    ldr.w r4,[r9]    ; r4 = Globals::status value
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

Frame 0x1b0 in both.  So the residual is a pure permutation:
`this`<->canary and `r4`<->`r9`, plus the original's second dispatch reloads
the status pointer (`ldr r0,[r4,#0]`) while ours reuses the cached value
(`mov r0,r4`).

## 2. The r4 lever, measured

`local` = how `Globals::status` is spelled; `alias` = `Level *self = m_pLevel`
form.  Size must stay 57,232.  `raw` = same-address diff bytes; `aligned` =
instruction-index alignment (`_work/reconstructions/align_diff.py`).

| # | variant | size | raw | aligned | this / canary / status |
|---|---|---:|---:|---:|---|
| 0 | HEAD | 57232 | 55094 | 17129 | fp / sl / r9 |
| 1 | no local (all `Globals::status`) | 57232 | 55095 | 17138 | fp / sl / r9 |
| 2 | `Status *&status` ref, all uses | 57232 | 55095 | 17138 | fp / sl / r9 |
| 3 | `Status **statusp = &Globals::status` | 57232 | 55095 | 17138 | fp / sl / r9 |
| 4 | **all-local `Status *status`, all uses** | **57124** | **55218** | **18269** | **r8 / fp / r4(value)** |
| 5 | `Level *self` value alias, whole fn | 57040 | 55282 | 18610 | r9 / fp / sl |
| 6 | `Level *&self` ref alias, whole fn | 57248 | 55187 | 17138 | fp / sl / r9 |
| 7 | all-local + `Level *self` value alias | 57036 | 55258 | 22590 | r8 / stack / fp |
| 8 | all-local + `Level *&self` ref alias | 57140 | 55292 | 18451 | r8 / fp / r4(value) |
| 9 | status decl moved first (dispatch only) | 57284 | 55248 | **16956** | r6 / fp / r9 |
| 10 | status decl first + all-local | 57024 | 55309 | 18420 | r8 / stack / fp |
| 11 | status decl first + `Level *&self` | 57300 | 55383 | 17008 | r6 / fp / r9 |
| 12 | `Status *st` for all c106-s6 uses (v18) | 57180 | **55082** | 17836 | fp / sl / r9 |
| 13 | all-local + `expect(mission==0,1)` | 57228 | 55500 | 22796 | (start shifts) |
| 14 | all-local + `expect(mission==0,0)` | 57200 | 55557 | 23189 | — |
| 15 | all-local + `expect(mission==1,1)` | 57228 | 55500 | 22796 | — |
| 16 | all-local + `expect(mission==106,1)` | 57228 | 55500 | 22801 | — |
| 17 | status decl first + `expect(mission==0,1)` | 57428 | 55637 | 22014 | — |
| 18 | v18 + status decl first | 57216 | 55403 | 17877 | r6 / fp / r9 |
| 19 | v18 + `expect(mission==0,0)` | 57488 | 55397 | 22130 | — |
| 20 | all-local, revert last 1 status use | 57248 | 55018 | 18895 | r9 / stack / r6 |
| 21 | all-local, revert last 96/103 uses | **57232** | 55260 | 18544 | r9 / fp / — |

## 3. Findings

1. **The all-local `Status *status` shape is the only lever that puts the
   status pointer in `r4`** (variant 4: `ldr.w r4,[r0]` -> value in r4,
   `this`=r8).  It is **-108 B** (57,124) and its aligned metric is 18,269
   (worse than HEAD) because `this` moves from fp to r8 and `messages` from
   r5 to sl.
2. **No size-exact shape found with a genuine +108 B companion.**  The
   `__builtin_expect` companions change the block layout too much
   (`expect(mission==0,1)` is +104 B -> 57,228, aligned 22,796; the other
   guards are +76/+136/+244).  The `Level *self` alias changes the size by
   -184/-436, not +76/+184 as hoped.
3. **`v18` (`Status *st` for all c106-s6 status uses) is the best raw lever
   measured: 55,082 (-12)** but -52 B (57,180), so it cannot land without a
   +52 B genuine companion.  `v18 + status-decl-first` is 57,216 (-16 B).
4. **Moving the `status` declaration to the top of the function** (variant
   9) is the best aligned shape measured: **16,956 (-173)** and raw 55,248.
   But it is +52 B (57,284); a +52 B shrink is not available from the
   measured levers.  Note: any non-size-exact function shifts every later
   `.text` symbol, so `byte_exact` collapses 4507 -> 2741 (measured).
5. The original's `&Globals::status` in r4 is a whole-function allocation
   outcome: the original spills/reloads `this` 328x and `&status` 49x, ours
   157x / 25x.  A local source permutation cannot reproduce it because the
   allocator reserves r4 for the `&m_pLevel` temporary (weight 5) in every
   size-exact shape.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10401/fdiff.py _ZN11LevelScript7processEi        # 55,094 / 57,232
python3 /dev/shm/w10401/align_diff.py _ZN11LevelScript7processEi  # 17,129
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

Scratch harnesses: `/dev/shm/w10401/{harness,sweep,sweep2,sweep3,grid,rev2,exp,mf,final}.py`.

## 5. Open questions

1. A genuine +108 B cold companion that keeps the all-local r4 allocation
   (this=r8, value=r4) would land variant 4 at 57,232.  Candidate: the
   `extra-orig` gaps at `0x13a596..0x13a6a6` (108 insns) and
   `0x13c48e..0x13c50a` (40 insns) — but adding them requires removing an
   equal amount elsewhere to stay size-exact.
2. `v18` at 57,180 needs a +52 B genuine companion; `v18 + status-decl-first`
   is 57,216 and needs +16 B.  A `m_nState = N` -> `m_nState++` style edit is
   ~8 B but did not move the size in the tested contexts.
3. The `status`-decl-first shape (aligned 16,956) is the most promising
   structural lever if a -52 B size companion can be found.

## 6. Late lead: `status`-decl-first + partial v18 (raw 54,836, aligned 17,002)

Combining the status-decl-first shape (variant 9) with a partial `Status *st`
in the c106-s6 block (N of the 7 `Globals::status->` uses replaced by `st->`)
gives the best raw/aligned numbers measured this wave, but none is size-exact:

| N/7 | size | raw | aligned |
|---:|---:|---:|---:|
| 0 | 57284 | 55248 | 16956 |
| 1 | 57292 | 55412 | 16997 |
| 2 | 57276 | 54861 | 17007 |
| **3** | **57276** | **54836** | **17002** |
| 4 | 57276 | 54836 | 17002 |
| 5 | 57224 | 55410 | 17874 |
| 6 | 57216 | 55412 | 17878 |
| 7 | 57216 | 55403 | 17877 |

`N=3` is a **-258 raw / -127 aligned** improvement at +44 B (57,276).  A
genuine -44 B companion would land it and be the best LevelScript shape so
far measured; `N=5` is -8 B (57,224) and needs +8 B.

Additional `status`-decl-first + partial-v18 companions measured: caching the
mission-89 state-5 block's 3 `Globals::status` uses in a `Status *st` local
saves 60 B (57,276 -> 57,216, raw 55,339, aligned 17,886); `mf+3/7` alone is
57,276.  Neither is size-exact.
