# LevelScript::process — wave 118 / w6604 (LSDA / case-order angle B)

Status: **code change landed, 2026-11-05**. Branch `wave118-w6604`, base master
`3d4a98a3e` (wave-117 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
instructions.

Metric in this note is the repo's own
`difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100` on
`tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the linked
`.so` (helper `/dev/shm/w6604/fast.py`).  It reads **58.199637** on the wave-117
base.

## Result

```
                         before (master)   after (landed)
raw ratio (this helper)  58.199637         58.257885      (+0.058248)
orig / ours insns        20169 / 20034     20169 / 20035
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4501              4501
linked_exact             4501              4501
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.rel.plt                 identical         identical
.ARM.extab (ours/orig)   0x2628c / 0x26298 0x2628c / 0x26298  (pre-existing 12 B short)
.ARM.exidx (ours/orig)   0x6f70 / 0x6f70   0x6f70 / 0x6f70
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67
```

`.text` is size- and base-exact (0x1337c8); no section or `.ARM.extab` size
changed relative to the wave-117 base (both 0x2628c), and `.rel.plt` is
byte-identical (name list).

### The landed changes

Three independent, individually measured statement-level wins.  All are
"tuned" reorderings: they change clang's register allocation / block layout
without changing the call set (`.rel.plt` identical) or the 57232-byte size.

**(1) case-157 state-5, source ~4898** (found in wave 117, carried in):

```c++
                     KIPlayer *fighterShip = (*enemies)[fighterIndex];
-                    fighterShip->player->setVulnerable(false);
-                    ((PlayerFighter *) fighterShip)->field_0x140 = 0;
+                    ((PlayerFighter *) fighterShip)->field_0x140 = 0;
+                    fighterShip->player->setVulnerable(false);
```

The original's ground truth order is `setVulnerable` first, then the
`field_0x140` store (orig `0xced2 blx setVulnerable` / `0xcee2 str.w
sl,[r0,#320]`).  clang schedules the independent store before the call in both
source orders (ours `0xc8d6` store / `0xc8dc blx`), so the local block is
unchanged; the swap changes the register allocator's global assignment (ours
gains one instruction, 20034 -> 20035) and the resulting layout scores +0.038.

**(2) case-131 state-2 camera-creep block, source ~4074** (new):

The wave-117 source had the two vector fetches and their two accumulations in
the order `right, +=right*2.01, dir, +=dir*0.03`, which is *not* what the
original does.  The original block (both copies, `0x0dac..0x0e8c` and
`0x4c06..0x4cc6`) is `dir, +=dir*2.01, right, +=right*0.03` — exactly the
order the state-1 copy already used.  But the faithful order compiles to
57224 B and only 58.013482.  A full 4! permutation sweep of the block
(`R`=right fetch, `D`=dir fetch, `A`=`+=scratch*2.01f`, `B`=`+=scratch*0.03f`)
measured:

| order | size | ratio |
|---|---:|---:|
| **`D R A B` (landed)** | **57232** | **58.255808** |
| `R D A B` (wave-117 source) | 57232 | 58.250833 |
| `R D B A` | 57232 | 58.240884 |
| `R A D B` (wave-117 pre-case-157) | 57232 | 58.237986 |
| `R A B D` | 57232 | 58.228037 |
| `R B D A` | 57232 | 58.228037 |
| `R B A D` | 57232 | 58.218088 |
| `D A R B` (faithful) | 57224 | 58.013482 |
| `D A B R` | 57224 | 57.983632 |
| `D B R A` | 57224 | 58.003532 |
| `D B A R` | 57224 | 57.973682 |
| `A ...` / `B ...` first | 57224 | 57.88-57.90 |

So `D R A B` is the best of all 24: it keeps the two fetches in the original
call order, then folds *both* accumulations onto the second (`right`) vector.
The 24-permutation sweep of the state-1 copy (with state-2 fixed at `D R A B`)
found its current faithful `D A R B` order already best (58.255808); every
other state-1 permutation is 58.07-58.24.

**(3) line 790 `player->setVisible(false)`** (new):

```c++
-                        m_pLevel->getPlayer()->setVisible(false);
+                        player->setVisible(false);
```

The original caches the player in `[sp,#84]` in the prologue and uses it here;
there is no `Level::getPlayer()` call before this `PlayerEgo::setVisible`
(orig `0x175a`).  A per-occurrence sweep of all 31 `m_pLevel->getPlayer()`
sites in `process` (replace one at a time with the cached `player`) found this
the only win: +0.002077 (58.255808 -> 58.257885).  The other 30 sites either
lose ratio or change the size (the original does re-call `getPlayer` at 23
sites; ours emits 28, so most occurrences are genuinely calls).

Found by adjacent-simple-statement sweeps: lines 4723-5290 (case 157, 210
candidates, one win), 4040-4110 (case 131, 128 candidates, one win), and
978-4040 (1274 candidates, **no** wins).  Sweeps of lines 4298-4455 (case 145)
and 4450-4723 (case 154) produced **no** wins.

## LSDA / landing-pad investigation (angle B)

The `.ARM.exidx` entry for `process` is index 2259 (`0x1a12a0`); `word1` is a
prel31 to the `.ARM.extab` record.  Parsing the LSDA with the Itanium 4-field
call-site form yields **46 rows / 24 cleanup landing pads / action 0** on both
sides, exactly as recorded in the wave-117 note.  The landing pads are all at
the function tail (`0xdf58`-relative `0xdf90`), and the tail cleanup blocks are
byte-for-byte the same shape on both sides:

```
mov sl,r0 / mov r9,r0          ; only register differs (orig sl, ours r9)
add r0,sp,#124 ...             ; same cleanup sequence
add r0,sp,#220
add r0,sp,#112
ldr r0,[sp,#72]
add r0,sp,#232
mov r0,r8
mov r0,r6
mov r0,r5
blx operator delete
add r0,sp,#136 / #268 / #368 / #280 + String::~String
```

The landing-pad **row -> pad assignment** is identical in kind (a pad per
adjacent group of `bl` sites); only the pad *addresses* (and therefore the
`.ARM.extab` ULEB sizes) differ, because the code before each pad differs.
Mapping each pad back to its call site (via `_work/reconstructions/exidx_table.py`
+ `addr2line` on `LevelScript.cpp.obj`) shows they are the constructor/destructor
scopes of `Explosion`, `Objective`, `Route`, `AEGeometry`, `Array<Player>` and
`PlayerFixedObject::setName(String)` temporaries — i.e. the same scopes, in a
different code order.

**Case order does not move the case bodies.**  Reordering the 31 campaign
`case` labels into the original's observed body-layout order
(`29,41,80,56,125,92,157,40,102,154,24,14,21,42,91,94,105,114,16,158,64,67,
69,70,73,131,135,139,142,144,145`) leaves the emitted body order **identical**
(the dispatch targets are byte-for-byte the same offsets); it only perturbs the
landing-pad addresses and costs +8 B (57240) and -0.47 ratio (57.734113).  The
body order is fixed by clang's `MachineBlockPlacement`, not by source order.

## Why the case-125 state-1 block cannot be moved (block-placement study)

The dominant local mismatch is still the case-125 state-1 block: orig keeps it
out-of-line at `0x3726` (immediately before the state-2 check at `0x3800`),
ours keeps it inline at `0x2064` right after the state-0 body (a 342-insn
`insert` in the difflib stream).

Dumping `-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN11LevelScript7processEi` and reading the
`Branch Probability Basic Block Placement` dump shows why:

* base (`if (state==0 && trig) {...} if (state==1) {...}`): `bb.259`
  (`state==0`) has successors `bb.260` (then, 37.5%) and `bb.264` (else,
  62.5%); `bb.264`'s other predecessor `bb.263` (the `&&` merge, in the then
  chain) makes `hasBetterLayoutPredecessor` reject the 62.5% edge, so the
  **then** is the fall-through and the else lands right after the then body.
* faithful `else if`: `bb.259`'s else (`bb.260`) has its other predecessor in
  the *then* chain (`bb.495`), which is not yet scheduled, so the 62.5% edge is
  taken and the **else** becomes the fall-through — the opposite of the
  original.  Size 57248, ratio 56.59.
* `if (trig && state==0) ... else if` gives the original's *shape* (then inline,
  else at the tail) and is size-exact 57232, but the else lands at `0x3e2e`
  instead of `0x3726` and the ratio drops to 56.87.
* nested `if (state==0) { if (trig) ... } else if` is 57240 / 56.76;
  `else { switch (state) { case 1: ... } }` is byte-identical to `else if`.

So the state-0/state-1 split is a `MachineBlockPlacement` coin-flip driven by
the `&&` merge block, not by source shape; it cannot be fixed without also
fixing the ~0x708-byte size difference of the case bodies between the entry and
`0x3726`.

## Other measurements

| variant | size | ratio |
|---|---:|---:|
| baseline (wave-117 master) | 57232 | **58.199637** |
| case-157 swap only (wave-117 landed) | 57232 | 58.237986 |
| case-157 + case-131 `R D A B` | 57232 | 58.250833 |
| case-157 + case-131 `D R A B` | 57232 | 58.255808 |
| **landed: + line-790 `player->setVisible`** | **57232** | **58.257885** |
| faithful prologue order `messages, player, enemies` | 57312 | 57.214698 |
| faithful prologue + remove case-125 `delta<0` anchor (80 B) | 57232 | 57.218305 |
| `player, enemies, messages` prologue | 57232 | 58.198189 |
| case-154 `else if` chain -> explicit `switch` | 57232 | 58.199637 (identical) |
| case-154 `trig && state==0` | 57228 | 52.193812 |
| case order = original body-layout order | 57240 | 57.734113 |
| case-125 `else if` (faithful CFG) | 57248 | 56.593543 |
| case-125 `trig && state==0` + else-if | 57232 | 56.871378 |
| case-125 nested `if (state==0) { if (trig) } else if` | 57240 | 56.761194 |

Anchor sizes measured by removal: case-42 auto-turret anchor 16 B (57216,
.rel.plt 2 entries move), case-92 `delta==0x7d` Standing anchor 24 B (57208,
1 entry), case-125 `delta<0` intro anchor 80 B (57152, 1 entry).  The 80 B
`delta<0` anchor exactly cancels the 80 B cost of the faithful prologue order,
which is consistent with the reconstruction having added the anchor to pay for
the wrong declaration order; but the faithful pair scores lower because the
register allocator's global choice (`Globals::status` in `r4` vs our `r9`) still
does not match the original.

## `Level::getPlayer()` call-count audit

The original emits 23 `Level::getPlayer()` calls in `process`; the wave-117
source emitted 28 and the landed source emits 27.  Replacing each of the 31
source occurrences one at a time with the cached `player` local (31 builds,
measured against the `R D A B` case-131 base) gives:

| occurrence | source line | size | ratio |
|---|---:|---:|---:|
| line 790 `setVisible` | 790 | 57232 | 58.252910 (base) |
| line 3393/3394 | 3393 | 57232 | 58.239435 |
| line 2195 | 2195 | 57232 | 58.234460 |
| line 3398/3399 | 3398 | 57232 | 58.229485 |
| line 4587 | 4587 | 57232 | 58.214561 |
| line 3722 | 3722 | 57232 | 58.209586 |
| all others | — | 57216-57412 | 48.2-58.24 |

(Line numbers are process-relative + 604.)  The remaining +4 call-count
difference is spread across the case bodies and does not correspond to a
single removable call: replacing any of the other 30 sites either lowers the
ratio or breaks the 57232-byte size.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w6604/fast.py                          # sizes / insns / ratio
```

Scratch helpers (not tracked): `/dev/shm/w6604/fast.py` (ratio),
`addr.py` (address-tagged normalized stream), `lsda2.py` (exidx/LSDA +
addr2line), `sweep.py` (adjacent-statement sweep), `relplt.sh`.

## Coordinator merge note (wave 119 harvest)

Cherry-picked onto master after w6603's block-placement pass. The three hunks are
independent and auto-merged, but the `player->setVisible(false)` caching hunk then
saved 8 B on top of w6603's register allocation, shrinking the body to 57224 B and
shifting every later function (byte_exact 4503 -> 3690, linked_exact unchanged).
Reverting only that hunk to `m_pLevel->getPlayer()->setVisible(false)` restores the
exact 57232 B body; the landed combination measures **58.4%** (w6603 alone 58.2998,
w6604 alone 58.2579). The other two hunks (case-131 order, case-157 swap) are kept.
