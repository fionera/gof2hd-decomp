# MGame::OnTouchEnd — wave 142 (w8711): campaign-chain block layout after the 96/127 merge

Status: analysis-only, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w8711`,
base `1d10d0694` (wave 141). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w8711`. **No source change landed** (base restored; full `check`
green: byte 4507, linked 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN,
RATCHET PASS). Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

Baseline re-measured this wave: **8080/8080, raw same-address diff 4464**.

## 1. Where the remaining diff lives

Per-0x100 window same-address byte diff (base vs original):

```
0x17a144..0x17ad44   0..25 per window   (first half essentially exact)
0x17ad44            167
0x17ae44            248   (dockToStream block, +8 shifted)
0x17af44            246
0x17b044..0x17bf44  ~230-250 per window (campaign chain)
0x17bf44             59
```

So ~4,385 of the 4,464 diff bytes are in `0x17ad00..0x17c0d4`.  The first half
(`0x17a144..0x17ad00`, the pause/choice-window region) is within 79 bytes of exact.

The chain is **not** a uniform shift.  Best per-window shift search (step 2 B):

```
window      base  best shift  best
0x17ad44     167     +8        149
0x17ae44     248     +8        161
0x17af44     246    -16        231
0x17b044     244    +16        235
0x17b144     242    -24        234
0x17b244     234      0        234
0x17b344     248     -4        233
0x17b444     243    -22        239
0x17b544     247    -28        238
0x17b644     244     -8        238
0x17b744     242    -22        233
0x17b844     245     -6        234
0x17b944     249    +30        227
0x17ba44     245    -28        224
0x17bb44     250    +30        214
0x17bc44     237    +22        140
0x17bd44     228    -32        152
0x17be44     246    +26        114
```

The chain windows stay near 230-250 (≈ the window size) under every single shift,
i.e. the chain blocks are **reordered**, not translated.  Matching them needs the
original's block order, not a size tweak.

## 2. The +8 shift at the hud cluster boundary (largest single run)

`0x17ae13..0x17aed9` (198 B, the `dockToStream` "jump cancelled in stream" block)
is byte-identical content shifted **+8**.  The +8 is the sum of three hud-cluster
layout differences (all documented in waves 134-137):

1. `0x200` body owns its own `blx initHudMenu` (`+4`) instead of branching to the
   `0x400` body's shared `blx`;
2. the `cameraMode == 0` test is `beq.w` instead of `beq.n` (`+2`) because
   `maneuverTouchEnd` is laid out 0x4c further away;
3. the `freeCamTouchEnd` tail is `b.w` instead of `b.n` (`+2`) because the
   epilogue is far.

The survivor flip (`0x400` owning the shared `blx`) is the wave-134/135/136/137
dead end: `PredBB` is the join test's IR block-list predecessor and every
`goto`/nesting/permutation shape is canonicalised back (or costs the dispatch
order).  Confirmed again here.

## 3. Cold-block interleaving is the real chain difference

The original interleaves the cold blocks into the chain at different points than
the base:

| block | original | base |
|---|---|---|
| `weaponSlot = 0` (`movs r6,#0; b secondary`) | `0x17b1e4` | `0x17b0b6` |
| `weaponSlot = 1` | `0x17b51c` | `0x17b3da` |
| `weaponSlot = 2` | `0x17b704` | `0x17b68a` |
| `weaponSlot = 3` / secondary menu | `0x17b9ca` / `0x17b9cc` | `0x17b908` / `0x17b90a` |
| engine-sound block | `0x17b1e8` | `0x17b40c` |
| quick-menu (`isMining`) block | `0x17b910` | `0x17b7ba` |
| `getType()==3/5/11` cargo block | `0x17b374` | `0x17b0bc` |
| `==22` empty-body target (post-chain) | `0x17b182` | `0x17b20a` |

In the original the cargo block sits **after** the `==15/22/43` bodies; in the
base it sits immediately after the `getType()==12` block, before the chain.  That
single move accounts for the +0x88 chain-start shift (the first `==15` test is at
`0x17b106` orig / `0x17b18e` base).  The two source forms are semantically
identical (`if (getType()==12) ... else if (getType()==3||5||11) ...`); only
MachineBlockPlacement's cold-block placement differs.

## 4. Variants measured this wave (all reverted; base restored)

| variant | size | raw diff | notes |
|---|---:|---:|---|
| base | 8080 | **4464** | reference |
| `S` split 96/127 (faithful, 23 calls) | 8080 | 5816 | +0 B on this base, but re-lays the chain |
| `B` inline `supernovaChallenge` | 8068 | 7025 | hoists `ldrb [fp,#484]; cmp` **before** the call; -12 B |
| `C` `hasWon()==true` -> `hasWon()` | 8068 | 6023 | matches orig `cmp #0; beq`, but -12 B re-lays |
| `BC` | 8056 | 7014 | |
| `E` `__builtin_expect(failed,0)` | 8064 | 5115 | exact join block, but breaks `0x17ab00..0x17ad00` |
| `F` swap 0x200/0x400 cases | 8080 | 4466 | survivor 0x400, dispatch order wrong |
| `G` `__builtin_expect(flags==0,0)` | 8076 | 6058 | |
| `won1` `__builtin_expect(won,1)` | 8088 | 4710 | |
| `type12` `__builtin_expect(getType()==12,0)` | 8084 | 4688 | |
| `camp1` `__builtin_expect(==15,0)` | 8080 | 4464 | **byte-identical** (hint ignored) |
| `camp0` `__builtin_expect(isCampaignMission(),0)` | 8080 | 4464 | **byte-identical** |
| `swap96` `(cm==127 || cm==96)` | 8080 | 4464 | **byte-identical** |
| `ret22` `goto backToStation` in `==22` | 8080 | 4464 | **byte-identical** |
| `inv` `if (!failed) {won} else {failed}` | 8080 | 4464 | **byte-identical** (canonicalised) |
| `type3` `__builtin_expect(getType()==3||5||11,0)` | 8100 | 4808 | |
| `SB` split + inline supernova | 8068 | 6988 | |
| `SBC` split + inline + hasWon | 8088 | 6943 | |
| `CE` hasWon + expect(failed) | 8068 | 5276 | |
| `BE` inline + expect(failed) | 8052 | 6993 | |

The base is a strong local optimum for the same-address metric: every faithful
content fix (`S`, `B`, `C`) is more correct in the instruction multiset but
re-lays the chain and raises the raw diff.

## 5. The `supernovaChallenge` -8 lever

`bool supernovaChallenge = this->field_0x1e4;` inlined as `if (!this->field_0x1e4)`
saves 12 B (not 8) because clang hoists `ldrb r1,[fp,#484]; cmp r1,#0` **before**
the `choiceWindow->OnTouchEnd` call (the original loads `r4` before the call but
tests after).  It is a size lever only; the site becomes less faithful.  No +8/+12
companion that improves the raw diff was found on this base:
`S` is +0, `C` is -12, `E` is -16, `type3` is +20.  A companion would have to be
a *new* faithful +8 (none identified).

## 6. Open questions / next leads

1. **Cold-block placement.** The chain is reordered, not shifted.  The cargo block
   (`getType()==3||5||11`) is the single biggest misplacement (moves the chain
   start by 0x88).  A source shape that makes MachineBlockPlacement defer it past
   the `==15/22/43` bodies (e.g. a `__builtin_expect` on the `getType()==12`
   test, a helper call, or a `goto` chain that survives SimplifyCFG) is the most
   promising unexplored lever.  `__builtin_expect(getType()==12,0)` alone gave
   8084/4688 (wrong direction); try `__builtin_expect(...,1)` or a `switch`.
2. **Join-test polarity.** The original's `cbz flags_body` (fall through to the
   epilogue) is still not reproduced without `__builtin_expect(failed,0)`, which
   re-lays `0x17ab00..0x17ad00`.  A shape that flips only the join-test
   fall-through (not the whole function) would fix the `0x17ae44` window (+8).
3. **`hasWon()==true`.** `C` matches the original's `cmp #0; beq` but is -12 B;
   pair it with a genuine +12 faithful change (not yet found).

## 7. Reproduction

```
cp /dev/shm/w8711/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8711/fdiff.py _ZN5MGame10OnTouchEndEiiPv  # 4464 / 8080 / 8080
python3 /dev/shm/w8711/shifts.py                            # per-window shift table
python3 /dev/shm/w8711/patch.py {B,C,BC,E,F,G}
python3 /dev/shm/w8711/patch2.py {won1,type12,camp1,failed0won1,f12}
python3 /dev/shm/w8711/patch3.py {CE,BE,BCE}
python3 /dev/shm/w8711/patch6.py {swap96,camp0,type3}
```

Scratch: `/dev/shm/w8711/{fdiff.py,diffruns.py,seq.py,shifts.py,orig.dis,ours.dis,
patch*.py,MGame.cpp.base}`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
