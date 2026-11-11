# LevelScript::process — wave 176 / w12003: branch-layout / state-machine pass
# (landed: two redundant `__builtin_expect` guards removed; raw 53,593 -> 53,581)

Target `_ZN11LevelScript7processEi`, original `0x137d18`, **57,232 B (0xdf90)**,
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w12003`,
branch `wave176-w12003`, base master `b21088985` (wave 175).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12003/`.

## Result (landed)

```
                         before (base)     after (landed)
process raw diffbytes    53,593            53,581            (-12)
process aligned insns    18,476            18,477            (+1)
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
.rel.plt                 3675/3675 ident   3675/3675 ident  (byte-identical)
.text section diff       80,166            80,154            (-12)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

### The landed source change

Two `__builtin_expect` wrappers are dropped (semantics unchanged; the guards
are redundant and the branch layout is closer to the original without them):

```diff
@@ case 5 (mission-106 state machine) @@
-                if (__builtin_expect(!Globals::layout->isFading(), 1)) { // lint: arm_builtin
+                if (!Globals::layout->isFading()) {
@@ mission-95/99/109/119/126/133/160/161 chain @@
-        if (__builtin_expect(m_nState == 2 && m_nScriptTimerA >= 2000, 1)) { // lint: arm_builtin
+        if (m_nState == 2 && m_nScriptTimerA >= 2000) {
```

Both edits are in the same TU, keep the function at 57,232 B, keep `.rel.plt`
byte-identical, and are the only two single-expect removals (out of 64) that
combine additively for a raw win.  See the sweep table below.

## 1. Cluster map — prologue (0x137d18..0x137d84)

The prologue is a pure register permutation plus one extra instruction.  Ours
is **6 bytes longer** before the first mission dispatch; that 6-byte shift is
the single largest local divergence and it propagates into the first literal
pool (orig `0x1380a4`, ours `0x1380c0`; +0x1c).

```
ORIGINAL 0x137d18                          OURS 0x137d18
mov sl, r0        ; this -> sl             mov fp, r0        ; this -> fp
ldr r0,[pc]; add r0,pc                     ldr r0,[pc]; add r0,pc
mov r4, sl        ; r4 = this              mov r4, fp        ; r4 = this
mov r6, r1        ; delta -> r6            mov r6, r1
ldr.w fp,[r0]     ; fp = &__stack_chk_guard ldr.w sl,[r0]    ; sl = &__stack_chk_guard
ldr.w r0,[fp]                              ldr.w r0,[sl]
str r0,[sp,#428]  ; canary                 str r0,[sp,#428]
ldr.w r0,[r4,#24]! ; r4 = &m_pLevel        same
blx getMessages                            same
mov r5, r0        ; messages -> r5          same
ldr r0,[r4]                                same
blx getPlayer                              same
strd r4,r0,[sp,#80] ; &m_pLevel, player     same
ldr r0,[r4]                                same
blx getEnemies                             same
mov r8, r0        ; enemies -> r8           same
ldr r0,[pc]; add r0,pc                     same
ldr r4,[r0,#0]    ; r4 = &Globals::status  ldr.w r9,[r0]    ; r9 = &Globals::status
ldr r0,[r4,#0]    ; r0 = Globals::status   ldr.w r4,[r9]    ; r4 = Globals::status
blx getMission                             mov r0,r4; blx getMission
cmp r0,#0                                  same
str.w sl,[sp,#88] ; this                    str.w fp,[sp,#88]
strd r4,r8,[sp,#92] ; &status, enemies      strd r9,r8,[sp,#92]
mov r0,r8                                  same
strd r5,r6,[sp,#72] ; messages, delta       same
beq.w 0x137e84                             beq.w 0x137e8a
ldr r0,[r4,#0]    ; reload status ptr      mov r0,r4        ; reuse cached value
blx getMission                             blx getMission
```

| value | original | ours |
|---|---|---|
| `this` | `sl` (r10) | `fp` (r11) |
| `&__stack_chk_guard` | `fp` (r11) | `sl` (r10) |
| `&Globals::status` | **`r4`** (16-bit `ldr`) | **`r9`** (32-bit `ldr.w`) |
| `Globals::status` value | reloaded from `[r4]` each use | cached in `r4` once |
| `messages` / `delta` / `enemies` | r5 / r6 / r8 | r5 / r6 / r8 (match) |
| `&m_pLevel` / `player` | `[sp,#80]` / `[sp,#84]` | `[sp,#80]` / `[sp,#84]` (match) |
| `this` / `&status` spill | `[sp,#88]` / `[sp,#92]` | `[sp,#88]` / `[sp,#92]` (match) |

The +6 B is exactly the three-instruction `ldr.w r9,[r0]; ldr.w r4,[r9];
mov r0,r4` (10 B) vs the original's `ldr r4,[r0]; ldr r0,[r4]` (4 B) plus the
second reload (2 B).  `r9` is a high register, so both loads need the 32-bit
encoding; the original's `r4` fits the 16-bit encoding.  This is a whole-function
greedy-allocator outcome, not a local source shape (waves 157/158/159 tried the
`Status*`/`Level*` alias and declaration-order levers; none is size-exact with
`&status` in r4).  See §5.

## 2. Cluster map — mission dispatch order (second chain)

`getCurrentCampaignMission()` compare sites, in binary order (from objdump):

```
ORIGINAL                                  OURS
0x137d62  #0                              0x137d68  #0
0x137d7c  #1                              0x137d82  #1
0x137fec  #2  (tail: mission>=2 gate)     0x137fec  #2
0x138132  #5                              0x13812c  #81
0x1381b8  #81                             0x138254  #5
0x1382e2  #26                             0x1382d4  #78
0x138332  #78                             0x1384cc  #26
0x1383ea  #50                             0x138558  #89
0x13857a  #89                             0x138754  #50
0x13877c  #51                             0x138764  #51
0x138c80  #106                            0x138ca6  #106
0x138e4a  #95 #99 #109 #119 #126 #133     0x138ebc  #95 #99 #109 #119 #126 #133
          #160 #161                                #160 #161
```

The source has **two** chains: the mission chain
`0, 1, 81, 78, 89, 106, 95..161, else` and the pre-switch specials chain
`5, 26, 50, 51` attached to `if (m_bStartSequence != 0)`.  The original
interleaves them (`5, 81, 26, 78, 50, 89, 51, 106`); ours interleaves them
differently (`81, 5, 78, 26, 89, 50, 51, 106`).  Rewriting the source in the
original's linear order was measured (wave 94 and this wave): it changes the
function size (57,016..57,160 B) and is not a raw win, so the interleave is a
block-placement artefact, not a source-order difference.

## 3. Cluster map — mission-0 state dispatch is a merged jump table

The mission-0 state machine is the largest structural divergence:

```
ORIGINAL 0x1389c8                          OURS 0x1389b0
cmp r1,#4 ; beq.w 138d2a  (state 4)        cmp r1,#5 ; beq.w 138d88  (state 5)
cmp r1,#3 ; bne.w 138fd8  (state 3 fall)   cmp r1,#4 ; bne.w 139048  (state 4 fall)
... state 3 body at 0x1389d4 ...           ... state 4 body at 0x1389bc ...
0x138fd8: cmp r1,#5 ; bne.w 139b1a         (state 3 body at 0x139048)
0x139b1a: subs r0,r1,#6 ; cmp r0,#9        (state 5 body at 0x138d88)
          bhi.w 13f906 ; tbh [pc,r0]       0x139048: subs r0,r1,#3 ; cmp r0,#12
          (states 6..15 jump table)                  bhi.w 13d598 ; tbh [pc,r0]
                                                     (states 3..15 jump table)
```

The original keeps states 3/4/5 as a compare chain and uses the `tbh` jump
table only for states 6..15 (`m_nState - 6`, 10 entries).  Ours merges the
`else if (m_nState == 3/4/5)` arms into the switch, producing a single
`m_nState - 3`, 13-entry jump table.  This is the single biggest source of
`extra-orig`/`extra-ours` instruction mismatches in the function.

Measured attempts to break the merge (all size-inexact or worse raw):

| variant | size | raw | rel.plt |
|---|---:|---:|---|
| base (merged table) | 57232 | 53593 | ident |
| `else if (m_nState >= 6) { switch }` | 57276 | 55448 | DIFF |
| `switch` moved out of the `else` | 57268 | 55446 | DIFF |
| `default: break;` in the switch | 57232 | 53593 | ident (no-op) |
| source arm order 3,4,5 | 57248 | 55238 | DIFF |
| source arm order 4,3,5 | 57232 | 54339 | DIFF 5 |
| source arm order 3,5,4 | 57232 | 53965 | DIFF 5 |

The original's arm order is 4, 3, 5 (compare order) with body order 3, 4, 5;
writing that order (`4,3,5`) is size-exact but raw 54,339 (worse) and rotates
`.rel.plt` by 5 entries.

## 4. Cluster map — raw diff distribution

The raw diff is essentially uniform: every 4 KiB window is ~95 % different.
There is no single hot block; the 6-byte prologue shift plus the jump-table
merge and the `player`-reload scheduling differences re-colour the whole
instruction stream.

```
region              diff/4096
00137d18-00138d18   3460
00138d18-00139d18   3963
00139d18-0013ad18   3873
0013ad18-0013bd18   3892
0013bd18-0013cd18   3957
0013cd18-0013dd18   3757
0013dd18-0013ed18   3989
0013ed18-0013fd18   3960
0013fd18-00140d18   3951
00140d18-00141d18   3939
00141d18-00142d18   3267
00142d18-00143d18   3930
00143d18-00144d18   3871
00144d18-00145ca8   3772
total               53581
```

A global ±2..40-byte shift of ours against the original never beats shift 0
(measured: best non-zero shift is +8 at 54,919), confirming the residual is
per-block codegen, not one global misalignment.

## 5. `__builtin_expect` leave-one-out sweep (64 guards)

Base raw 53,593.  Each row removes exactly one guard.  `size` is
ours/orig.  Full data in `scratch/w12003/expect_results.txt`.

| n | raw | size | rel.plt | guard (source line) |
|---:|---:|---|---|---|
| 8 | **53,586** | 57232/57232 | ident | `m_nState == 2 && m_nScriptTimerA >= 2000` (1444) |
| 3 | **53,588** | 57232/57232 | ident | `!Globals::layout->isFading()` (1342) |
| 19 | **53,588** | 57232/57232 | ident | `player->isDockedToDockingPoint()` (2840) |
| 53 | 53,591 | 57232/57232 | ident | `i < 22` (4909) |
| 0,12,16,34,48 | 53,593 | 57232/57232 | ident | no-op removals |
| 24 | 53,581 | 57228/57232 | DIFF 224 | `m_nState == 8` (size-inexact) |
| 9 | 53,660 | 57232/57232 | ident | `wingman->isWingMan()` |
| 7 | 54,768 | 57232/57232 | ident | `messages[0]->isOver()` |

Additive combinations measured this wave (all size-exact + rel.plt-identical
unless noted):

| combo | raw | aligned |
|---|---:|---:|
| {8} | 53,586 | 18,477 |
| {8,3} **(landed)** | **53,581** | 18,477 |
| {8,3,11} | 53,585 | 18,388 |
| {8,3,15} | 53,582 | 18,474 |
| {8,3,45} | 53,583 | 18,467 |
| {8,3,50} | 53,582 | 18,479 |
| {8,3,55} | 53,583 | 18,471 |
| {8,3,30} | 53,584 | 18,477 |
| {8,3,11,39} | 53,618 | 18,379 |
| {8,3,19} | 54,073 | 18,473 (size 57224, DIFF 949) |
| {8,3,53} | 53,623 | 18,479 (size 57236, DIFF 1004) |
| {8,3,7} | 54,763 | 18,477 |
| {8,3,9} | 53,648 | 18,469 |

`{8,3}` is the best raw combination.  `{8,3,11}` is the best **aligned**
combination (-89) at +4 raw; it is documented but not landed because the
deliverable metric is raw.

## 6. Other measured variants (not landed)

| variant | size | raw | rel.plt |
|---|---:|---:|---|
| base | 57232 | 53593 | ident |
| drop `Status *status` local, use `Globals::status` twice | 57232 | 53594 | ident |
| `Status *const status` | 57232 | 53593 | ident |
| `Status &status = *Globals::status` | 57232 | 53593 | ident |
| second dispatch via `Globals::status` only | 57232 | 53594 | ident |
| `Level *level = m_pLevel` alias | 57232 | 53640 | ident |
| declare `enemies` before `messages`/`player` | 57232 | 53607 | ident |
| `player` rotate moved before `setPosition` | 57232 | 53594 | ident |
| `player` rotate moved before the `>25000` gate | 57236 | 55369 | DIFF |
| `player` rotate via `m_pLevel->getPlayer()` | 57228 | 54813 | DIFF |
| mission-78 `case 11` before `case 12` | 57040 | 55421 | DIFF |
| mission chain source order 5,81,26,78,50,89,51 | 57016 | 55380 | DIFF |

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12003
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# raw= 53581 aligned=18477 size=57232/57232
cmake --build cmake-build-match --target check -j2    # RATCHET PASS
cmake --build cmake-build-match --target lint  -j2    # CLEAN
cmake --build cmake-build-match --target drift -j2    # 0
```

Scratch harnesses (all under `/home/debian/GalaxyOnFire2/scratch/w12003/`):
`measure.sh` (link + fn_diff + rel.plt compare), `combo2.py` (remove a set of
`__builtin_expect` guards by index), `expect_results.txt` (the 64-row sweep),
`missionmap.py` (the campaign-mission compare-site map), `regiondiff.py`
(normalised instruction diff of an address range), `firstdiff.py`,
`shiftcheck.py`, `regioncount.py`, `v_*.py` (the variant generators).

## 8. Open questions

1. **Break the state-3/4/5 jump-table merge.**  The original keeps a compare
   chain for states 3/4/5 and a 10-entry `tbh` for 6..15.  Every source shape
   tried either keeps the merge or changes the size.  A shape that makes the
   three `else if` arms opaque to switch lowering (e.g. a non-constant guard)
   without changing the body would be the largest remaining win.
2. **Prologue `&Globals::status` -> r4.**  Needs the whole-function spill
   pattern (original reloads `this` 328x and `&status` 49x; ours 157x / 25x).
   All local `Status*`/`Level*` alias shapes are size-inexact (waves
   157/158/159).
3. **`player` reload scheduling in `case 158`.**  Ours hoists
   `ldr.w r8,[sp,#84]` to just after `KIPlayer::isVisible` (0x13895e); the
   original reloads it at the `rotate` call site (0x14387e).  The extra 4-byte
   load is inside the `m_nScriptTimerA > 25000` branch.
4. **Wingmen loop rotation in the shared tail.**  Original is a bottom-tested
   loop (`bcc.n` back to the body); ours is top-tested with an extra
   `adds/b.n`.  `expect(isWingMan(),0)` removal makes raw worse (53,660).
