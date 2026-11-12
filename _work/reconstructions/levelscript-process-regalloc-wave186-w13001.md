# LevelScript::process — wave 186 / w13001: deep register-allocation pass

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13001`, branch `wave186-w13001`, base master
`2d02567f6` (wave-185 harvest 9, raw **52,075**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13001`.

This pass dumps both prologues, maps every callee-saved register and every
spilled slot to its owning source variable, then sweeps the top-level
declaration order/lifetimes (`messages`, `player`, `enemies`, `status`), the
`Status *` form, `Level *` / `Player *` aliases, block-scope locals, the
existing `__builtin_expect` cells, and all adjacent statement pairs that
mention `Globals::status` / `m_pLevel` / `player` / `enemies` / `m_pCamera`.
**One size-exact, `.rel.plt`-identical raw win landed**: the two
`m_pCamera->setPosition(...)` calls in the case-105 / state-6 exit block are
emitted in the opposite source order (player-relative position first, then
`m_dirVec`).  The callee-saved allocation itself is not reachable from source
without a size change.

```
                         base (HEAD)      this pass
process raw diffbytes    52,075           52,034     (-41)
process aligned insns    20,766           20,772     (+6)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          74,402           74,361     (-41)
.ARM.exidx               0                0          (byte-identical)
.ARM.extab               647              647        (size/base exact 156,312 B @ 0x1a3b78)
.rodata                  0                0          (size/base exact 269,159 B @ 0x1c9e10)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.data / .bss / .comment  0 / 0 / 0        0 / 0 / 0
.note.gnu.build-id       20               20
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
137d26  4682        mov  sl, r0              ; this            -> sl (r10)
137d28  48de        ldr  r0, [pc,#888]       ; &__stack_chk_guard GOT slot
137d2a  4654        mov  r4, sl              ; r4 = this
137d2c  460e        mov  r6, r1              ; delta           -> r6
...
```

Ours (base):

```
137d18  b5f0        push {r4,r5,r6,r7,lr}
137d1a  af03        add  r7, sp, #12
137d1c  e92d 0f80   stmdb sp!, {r7,r8,r9,sl,fp}
137d20  ed2d 8b0c   vpush {d8-d13}
137d24  b0ec        sub  sp, #432
137d26  4683        mov  fp, r0              ; this            -> fp (r11)
137d28  48de        ldr  r0, [pc,#888]       ; &__stack_chk_guard GOT slot
137d2a  4682        mov  sl, r0              ; canary          -> sl (r10)
137d2c  460e        mov  r6, r1              ; delta           -> r6
...
```

Register / slot map (both builds, verified from the disassembly):

| value | original | ours (base) |
|---|---|---|
| `this` (`LevelScript *`) | `sl` (r10), used as `[sl,#…]` ~335× | `fp` (r11), used as `[fp,#…]` ~706× |
| stack canary | `fp` (r11) | `sl` (r10) |
| `&Globals::status` (GOT slot addr) | `r4` (`ldr r4, [pc,#…]`) | `r9` (`ldr.w r9, [pc,#…]`) |
| cached `Status *` value | **none** — reloads `ldr r0,[r4]` at each use (295×) | `r4`, cached once (`ldr.w r0,[r9]`) |
| `delta` | `r6` | `r6` |
| `player` / `enemies` / `messages` | `r5` / `r7` / `r8`-family | same family |
| `m_pLevel` | reloaded from `this` | reloaded from `this` |

Consequence: our prologue is 6 bytes longer (`ldr.w r9` + `ldr.w r0,[r9]` vs
`ldr r4` + `ldr r0,[r4]`, plus 2 bytes in the `m_nState == 1` arm), so the
mission-0 block starts at `0x137e8a` instead of `0x137e84` and the whole body
is shifted.  The original deliberately does **not** keep a `Status *` value in
a callee-saved register: every `Globals::status->` use re-loads `[r4]`.  In our
build the C++ local `Status *status = Globals::status;` is what creates the
cached `r4`.  Removing that local is size-neutral here (52,076, see §3) but
does not by itself recover the original `this=sl` split, because the greedy
allocator's `gprnopc` (`this`) vs `gpr` (`&status`, canary) class split drives
the tie-break.  `Level *` / `Status *` locals are `gprnopc`, so adding them
perturbs the split.

## 2. What was swept

All cells rebuilt with `link_so` (`-j2`, `ulimit -v 12000000`) and measured
with `_work/reconstructions/measure_w11901.py`; size must stay 57,232 and
`.rel.plt` must stay `289b2a06…`.

1. **24 declaration permutations** of `messages/player/enemies/status`:
   base `MPES` = 52,075 is best.  `EPMS` 52,079, `MEPS/PMES` 52,087,
   `PEMS/EMPS` 52,089, `EPSM/ESPM` 52,095/52,099, `PSEM` 52,102,
   `SEPM` 52,114, `SPEM` 52,115.  Many orders are size-changing
   (57,288–57,312) and therefore gate-fatal.
2. **`status` local forms**: no local 52,076; `Status *&status` 52,076;
   `Status **status` 52,076; `Status *const` 52,075 (neutral);
   local-first-only 52,076; local-second-only 52,075 (neutral, identical
   prologue); two locals 52,075; assign-in-cond 52,076;
   cast-load / ref-load / addr-local 52,075.
3. **All 100 body `Globals::status->` rewritten to the local** = 55,268,
   size 57,204 (prologue becomes `mov r8,r0; mov r5,r8` — much worse).
   Body-local with both dispatch tests via `Globals::status` = 55,439,
   size 57,332 (worse).
4. **Block-scope `Status *st16`** in the mission-0 state-16 arm: 52,087 (+12).
5. **`Level *level = m_pLevel` aliases**: all three getters = 55,146,
   size 57,240; messages only = 52,075 (neutral); enemies only = 55,450,
   size 57,296.  `this->m_pLevel` = neutral.  `(void)messages` / `(void)lv` /
   `int d` dummy locals = 52,087 (+12).
6. **Mission-1 FAGS reorder** (original emitted order: forceUpdate, rotate,
   getPosition, setToSleep) = 52,079 (+4); alone with no-local = 52,080 (+5).
7. **Whole-condition `__builtin_expect` sweep** (all 1,100 add/flip cells,
   `levelscript-process-expect-sweep-w11901.py`): no cell with
   raw < 52,075 and size 57,232 and rel.plt-identical.  345 cells are neutral
   52,075.
8. **Adjacent statement-pair swap sweep** (`sweep_adj2.py`, 1,242 measured
   pairs of the 1,319 adjacent pairs involving `Globals::status` /
   `m_pLevel` / `player` / `enemies` / `m_pCamera`): **one win** — the two
   `m_pCamera->setPosition(...)` calls in the case-105 / state-6 exit block
   (source pair `m_pCamera->setPosition(m_dirVec);` followed by
   `m_pCamera->setPosition(player->getPosition() - player->GetDirVector() * 2400.0f);`).
   Swapping them gives **52,034** (−41).  Next best 52,085.
9. **`-mllvm` flag probes** (`flagtest2.py`, exact gold link from
   `linkcmd.txt`): `-split-spill-mode=size` 55,450 size 57,324 (rel.plt DIFF);
   `=speed` neutral 52,075; `-enable-deferred-spilling` 55,270 size 57,084;
   `-optimize-regalloc=false` 124,987 size 126,336; `-stress-regalloc=10`
   55,549 size 57,264; `=9` 55,774 size 56,024 rel.plt DIFF;
   `-regalloc-csr-first-time-cost=0/2/3/5/10` all neutral 52,075;
   `-join-liveintervals=false` 56,729 size 58,440;
   `-condsider-local-interval-cost` 55,435 size 57,272;
   `-early-live-intervals` crashes clang.
   (Note: `ninja` alone does not relink after a manual object overwrite — use
   the captured gold command in `linkcmd.txt`.)

## 3. Landed edit

`src/game/world/LevelScript.cpp`, inside `case 105`, `m_nState == 6`,
`m_nScriptTimerA >= 4001`:

```cpp
                        m_pCamera->setPosition(player->getPosition() - player->GetDirVector() * 2400.0f);
                        m_pCamera->setPosition(m_dirVec);
```

(was `setPosition(m_dirVec)` first).  This matches the original's emitted
order and moves the two `Player *`-dependent address computations before the
`m_dirVec` store, shortening the live ranges around `player` in the exit
block.  Size stays exactly 57,232, `.rel.plt` stays byte-identical, `.text`
drops 74,402 → 74,361.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13001
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52034 aligned=20772 size= 57232/57232 insn=20726/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52034 size=57232/57232 text=74361 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`harness.py`, `flagtest2.py`, `sweep_adj2.py`,
`cells.tsv`, `linkcmd.txt`, `mi_base.txt`, `t*.py`) live in
`/home/debian/GalaxyOnFire2/scratch/w13001/`; they are scratch, not tracked.
