# LevelScript::process — wave 183 / w12702: deep branch-layout pass (tbh map + fall-through structure)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12702`, branch `wave183-w12702`, base master
`9555474d8` (wave-182 harvest 5, raw **52,412**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12702`.

Follow-up to the wave-182 w12603 branch-layout note.  This pass builds the
complete `tbh` dispatch map of both binaries, compares every switch range and
its default target, aligns the direct-call stream to locate block-order
divergences, and reconstructs the original fall-through structure from the
Thumb disassembly.  **No size-exact, `.rel.plt`-identical raw win was found**;
`src/` is left at the base (raw 52,412).  The measurements below are the
durable result.

## Result

```
                         base (HEAD)      landed
process raw diffbytes    52,412           52,412          (no code change)
process aligned insns    22,050           22,050
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,728
.rel.plt                 md5 289b2a06…    289b2a06…       (byte-identical)
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

## 1. Complete `tbh` dispatch map (branch-layout ground truth)

The function contains **15 `tbh` tables** in both binaries.  Decoding the
halfword table at `tbh+4` (`target = tbh+4 + 2*entry`) and reading the
`subs/cmp` range guard immediately before each `tbh` gives the switch range:

| # | original `tbh` | range (guard) | entries | ours `tbh` | range (guard) | entries |
|---|---------------:|---------------|--------:|-----------:|---------------|--------:|
| 1 | `0x138384` | `subs #1; cmp #11` | 12 (1..12) | `0x138328` | `subs #1; cmp #11` | 12 (1..12) |
| 2 | `0x13866a` | `subs #1; cmp #4`  | 5 (1..5)   | `0x138648` | `subs #1; cmp #4`  | 5 (1..5) |
| 3 | `0x138a8a` | campaign prong A   | 16         | `0x138a8a` | campaign prong A   | 16 |
| 4 | `0x138c3e` | campaign prong B   | 16         | `0x138c3e` | campaign prong B   | 16 |
| 5 | `0x13983e` | `cmp #3`           | 4 (0..3)   | `0x1392d0` | `cmp #5`           | 6 (0..5) |
| 6 | `0x139b22` | `subs #6; cmp #9`  | **10 (6..15)** | `0x139050` | `subs #3; cmp #12` | **13 (3..15)** |
| 7 | `0x13a024` | `subs #1; cmp #6`  | 7 (1..7)   | `0x13a1c8` | `subs #1; cmp #6`  | 7 (1..7) |
| 8 | `0x13acb6` | `cmp #3`           | 4 (0..3)   | `0x13977a` | `cmp #3`           | 4 (0..3) |
| 9 | `0x13aeec` | `subs #1; cmp #9`  | 10 (1..10) | `0x13af18` | `subs #1; cmp #9`  | 10 (1..10) |
| 10 | `0x13cad4` | `subs #2; cmp #3` | 4 (2..5)   | `0x13a802` | `subs #1; cmp #5`  | 6 (1..6) |
| 11 | `0x13cdf0` | `subs #2; cmp #5` | 6 (2..7)   | `0x13ccce` | `subs #2; cmp #5`  | 6 (2..7) |
| 12 | `0x13e638` | `subs #1; cmp #9` | 10 (1..10) | `0x13ec40` | `subs #1; cmp #9`  | 10 (1..10) |
| 13 | `0x13e81e` | `subs #1; cmp #5` | 6 (1..6)   | `0x13a888` | `subs #1; cmp #9`  | 10 (1..10) |
| 14 | `0x13e8f0` | `subs #1; cmp #9` | 10 (1..10) | `0x13acbe` | `cmp #3`           | 4 (0..3) |
| 15 | `0x143d28` | `subs #8; cmp #4` | 5 (8..12)  | `0x143be0` | `subs #8; cmp #4`  | 5 (8..12) |

Only **one** table has a different range: the mission-0 state machine
(`switch (m_nState)` at source line 756, cases 6..15).  The original lowers it
as `subs r0,r1,#6; cmp r0,#9; bhi 13f906; tbh` — **10 entries (6..15)** with
the state-16 handler as the default target at `0x13f902`.  Ours lowers it as
`subs r0,r1,#3; cmp r0,#12; bhi 13d598; tbh` — **13 entries (3..15)** with
entries 3/4/5 dead, because clang's SimplifyCFG **merged the preceding
`else if (m_nState == 3)` arm into the switch as case 3**.  The state-16
handler is therefore the inline fall-through at `0x13d598` instead of the
out-of-line default at `0x13f902`.  This is the root cause of the entire
mission-0 block-order divergence.

The other 14 tables match in range (the address/order differences in rows
5/8/10/13/14 are the same tables placed in a different order, not different
switches).

## 2. Branch-target differences grouped by state block

Direct-call anchoring (`bl`/`blx <sym>` only, 2,356 direct calls in the
original, 1,628 matched by name) gives the address delta `ours - orig` at each
matched call.  The delta is not a uniform shift; it changes sign inside the
mission-0 region:

```
orig call   ours call   delta   symbol
0x138a06    0x1389c2     -68    RadioMessage::isOver
0x138a4c    0x138a4c       0    TargetFollowCamera::setLookAtCam
0x138c0a    0x138c0c      +2    TargetFollowCamera::setLocal
0x138c7c    0x138ca2     +38    Status::getCurrentCampaignMission
0x138d30    0x138d8e     +94    RadioMessage::isOver
0x138d44    0x138e12    +206    FModSound::stop
0x138df0    0x138e9c    +172    TargetFollowCamera::translate
0x138fe4    0x1390a2    +190    RadioMessage::isOver
0x1391f0    0x13916c    -132    Status::getCampaignMission
```

So the mission-0 bodies are laid out in a different order:

| block | original addr | ours addr | note |
|-------|--------------:|----------:|------|
| state 4 body | `0x138d2a` | `0x1389bc` | ours falls through from the dispatch; original branches to it |
| state 5 body | `0x138fde` | `0x138d88` | |
| state 3 body | `0x1389d4` | `0x13906e` (tbh entry 0) | original falls through from the dispatch; ours merged into the switch |
| shared tail | `0x13d8d0` | `0x13d598`/`0x13d4ec` | ours' case bodies fall through to the state-16 handler |

Original mission-0 dispatch (`0x1389c8`): `cmp r1,#4; beq 138d2a` →
`cmp r1,#3; bne 138fd8` → **state-3 body inline** → `138fd8: cmp r1,#5;
bne 139b1a` → state-5 body → `139b16: ldr r1,[sl,#28]; subs #6; cmp #9;
bhi 13f906; tbh`.  Test order **4, 3, 5**, fall-through body **state 3**.

Ours (`0x1389b0`): `cmp r1,#5; beq 138d88` → `cmp r1,#4; bne 139048` →
**state-4 body inline** → `139044: ldr r1,[fp,#28]; subs #3; cmp #12;
bhi 13d598; tbh`.  Test order **5, 4**, fall-through body **state 4**, state 3
merged into the switch.

The campaign-switch dispatch itself matches: both do the two range prongs
`sub r1,r0,#131; cmp #14; bls 138a8a` and `sub r1,r0,#64; cmp #9; bls 138c3e`
before the linear compare chain `14,16,21,24,29,40,41,42,56,80,91,92,94,102,
105,114,125,154,157,158` (ours is a uniform 6-byte earlier because of the
prologue).

### Prologue register permutation (unchanged, still the dominant shift)

Original: `this -> sl`, `&__stack_chk_guard -> fp`, `&Globals::status -> r4`
(16-bit `ldr`), status value reloaded from `[r4]`.
Ours: `this -> fp`, `&guard -> sl`, `&status -> r9` (32-bit `ldr.w`), status
value cached in `r4`, plus an extra `mov r0,r4` (2 B) before the first
`getCurrentCampaignMission()`.  This is the global +6/+2 B shift that keeps
the raw metric at ~52.4 KB; it is not a branch-layout issue and was already
the subject of the wave-157..166 prologue passes.

## 3. Rejected experiments (all full compile + gold link, size and `.rel.plt` checked)

Every row is a semantics-preserving source edit measured against the 52,412
base.  `size` is ours/orig; `rel` is `ident` when the `.rel.plt` md5 equals
the original's `289b2a069f8504d1e8a3919e43915e04`.

### Mission-0 arm permutations (all 6 orders of the state-4/5/3 `else if` arms)

| source order | raw | size | rel.plt |
|---|---:|---:|---|
| **4,5,3 (base)** | **52,412** | 57232/57232 | ident |
| 4,3,5 | 53,693 | 57232/57232 | DIFF (`a8d49c…`) |
| 5,4,3 | 55,159 | 57228/57232 | DIFF |
| 5,3,4 | 55,159 | 57228/57232 | DIFF |
| 3,4,5 | 55,171 | 57256/57232 | DIFF |
| 3,5,4 | 53,611 | 57232/57232 | DIFF |

`4,3,5` is the only order that reproduces the original mission-0 `tbh` range
**6..15** (verified: table at `0x13b1da`, `subs #6; cmp #9`), i.e. it fixes the
merge, but it changes the PLT first-reference order (`.rel.plt` DIFF) and the
raw rises to 53,693.  The base order `4,5,3` is the only one with the
original's `.rel.plt`, so the original source order is `4,5,3` and clang
reordered the tests/bodies in the original build for a reason not expressible
from this source shape.

### Switch / if-chain shape variants (mission-0 switch)

| variant | raw | size | rel.plt |
|---|---:|---:|---|
| `default:` case holding the state-16 handler | 55,216 | 57256/57232 | DIFF |
| switch cases rewritten as `else if (m_nState == N)` chain | 55,216 | 57256/57232 | DIFF |
| state-16 `else if` moved before the switch | 55,216 | 57256/57232 | DIFF |
| `else if (m_nState >= 6 && m_nState <= 15)` guard | 55,378 | 57244/57232 | DIFF |
| `else if (m_nState >= 6)` / `> 5` guard | 55,223 | 57252/57232 | DIFF |
| state-16 check wrapped in `__builtin_expect(...,0)` | 55,238 | 57244/57232 | ident |
| state-16 check wrapped in `__builtin_expect(...,1)` | 53,800 | 57236/57232 | DIFF |
| state-3 condition `>=3 && <=3` / `<4 && >2` | 54,475 | 57256/57232 | ident |
| state-3 condition `- 3 == 0` / `3u` / `3 == x` | 52,412 | 57232/57232 | ident (canonicalised) |

Reordering the ten `case 6..15` blocks in source order does **not** change
codegen at all (clang sorts switch cases by value); the original's body order
`6,10,7,8,9,11,12,13,15,14` cannot be driven from source case order.

### `&&` short-circuit / guard order

| variant | raw | size | rel.plt |
|---|---:|---:|---|
| mission-51 body `count>=3 && isOver && state==0` (matches original guard order) | 52,415 | 57232/57232 | ident |
| `isOver && state==0 && count>=3` | 54,761 | 57224/57232 | DIFF |
| `count>=3 && state==0 && isOver` | 54,764 | 57224/57232 | DIFF |
| `state==0 && isOver && count>=3` | 54,764 | 57224/57232 | DIFF |
| `state==0 && count>=3 && isOver` | 54,763 | 57224/57232 | DIFF |
| mission-51 `__builtin_expect` moved onto one operand / removed | 52,714 | 57232/57232 | ident |
| mission-51 `messages != nullptr` first | 53,612 | 57232/57232 | ident |
| mission-50 arm moved before the mission-89 arm | 55,723 | 57564/57232 | DIFF |

The original's guard at `0x13876c` really is `count>=3` → `isOver` →
`state==0`, but writing that order in source gives 52,415 (+3), not a win; the
`cmp r0,#3; bcc` vs `cbz` lowering differs.

### Plain `if/else` inversions (14 candidates)

Swapping the then/else bodies and negating the condition:
- 8 of 14 produce **identical** code (clang canonicalises the branch);
- line 3082 (`docked`) → 52,413 (+1);
- line 2926 → 54,332 (size +16);
- 4 candidates fail to parse (multi-line `if` headers) and were not measured.

### Adjacent pure `else if` arm swaps (62 candidates)

Swapping adjacent arms whose conditions contain no calls/`->`:
- 49 produce identical code;
- the only size-exact rel.plt-ident movers are +1/+2/+3 raw (e.g. line 1436
  → 52,815, line 4318 → 53,248); none improves on 52,412.

### `if (A && B)` → nested `if (A) { if (B) }` (44 candidates)

All size-exact candidates are ≥ 52,412; the closest are identical
(52,412) and the rest are +50 or worse.  No win.

### Single `__builtin_expect` flips (61 cells)

Flipping the trailing `0`/`1` of every `__builtin_expect` in `process` one at a
time: the best is 52,412 (line 2881, identical); every other flip is ≥ 52,413.
The current expect set is locally optimal.

## 4. Conclusion

The residual is a **block-placement** difference, not a missing branch or a
wrong `if/else` shape.  The single structural divergence is clang's
SimplifyCFG merging the mission-0 `else if (m_nState == 3)` arm into the
following `switch (m_nState)`, which extends the `tbh` from 6..15 to 3..15 and
moves the state-16 default handler inline.  The only source order that
prevents the merge (`4,3,5`) changes the PLT first-reference order and is
therefore gate-fatal (`.rel.plt` must stay byte-identical), and it raises raw
to 53,693.  No size-exact, `.rel.plt`-identical raw win was found; `src/` is
left at the base.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12702
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=52412 aligned=22050 size=57232/57232
cmake --build cmake-build-match --target check -- -j2
# -> RATCHET PASS
```

Scratch harnesses (`harness.py`, `tbh2.py`, `tbhd.py`, `deltas2.py`,
`side.py`, `invsweep3.py`, `swapchain.py`, `expsweep.py`, `andsweep.py`,
`exp5.py`..`exp16.py`) live in `/home/debian/GalaxyOnFire2/scratch/w12702`
(scratch, not tracked).
