# w5915: `Level::createMission` (92.094405 %, 17172 B) — wave-111:
# spill-slot / bool-type / statement-order sweep; no size-preserving improvement

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w5915`, branch `wave111-w5915`,
base `8eea060cc`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; the disassembly addresses below are the linked `.so`
addresses, function start `0xaffd8`, end `0xb431c`).
**No `src/` change is committed**: every variant measured below is neutral or a
regression. The deliverable is this note. The worktree is clean.

## Baseline (unchanged, gate green before and after)

```text
_ZN5Level13createMissionEv   fuzzy 92.094405 %   size 17172 B (size-exact)
                             orig 6055 insns / ours 6063 insns
                             [sp,#84] 17 / 2,  [sp,#88] 2 / 17
gate: byte 4500/4524, linked 4500/4524, size 4524/4524, stubs 0, missing 0,
      wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
      RATCHET PASS
ELF shape unchanged by this pass: 27 sections exact, file size 2,187,328 B,
.ARM.extab 0x26294 (the pre-existing 4-byte-short delta vs 0x26298).
```

Every row below is a fresh `cmake --build cmake-build-match --target link_so`
followed by `score.py` (the official verifier's disassembly + reconcile +
`difflib.SequenceMatcher(autojunk=False)` normalisation, unrounded).  All
variants are applied on top of the committed baseline
(`/dev/shm/w5915/Level.base.cpp`, a byte copy of HEAD `Level.cpp`).

The residual is 317 non-equal diff regions / 475 vs 483 instructions of 6055.
132 of those regions differ in mnemonic shape (not just register names); the
rest are pure register/slot renames.  The call sequence is identical except
for the position of the `PlayerTurret::setScaling` call in the alienGuns loop.
The `hp` computation at `0xb35e0`-`0xb3620` is byte-identical, confirming the
source shape there.

## 1. The `raid`/`Globals::status` spill-slot swap (18 regions, ~0.3 pp)

The only frame-layout difference in the whole function is two adjacent 4-byte
spill slots:

```text
                     original   ours
raid   (bool)        sp+84      sp+88
status (Status*)     sp+88      sp+84
```

The prologue store at `0xb0024` (`str.w fp,[sp,#N]`, fp = `Globals::status`)
and the `raid` stores at `0xb04c0`/`0xb04d8` are otherwise byte-identical.
The swap is a pure spill-slot-creation-order artefact of `RegAllocGreedy`
(the two slots are otherwise the same size/alignment).  It produces ~18
`[sp,#84]`<->`[sp,#88]` diff regions.

Moving the `bool raid;` declaration (`after security` = baseline, `after
pirates`, `after raidRoll`, `after introSystem`, `after missionPtr=3`,
`security` before `raid`) does not change the slot order and is neutral or
worse.  Several *larger* source changes do flip the slots, but every one of
them costs far more than the ~0.3 pp the swap is worth:

| variant | pct | size | `[sp,#84]`/`[sp,#88]` |
|---|---:|---:|---|
| **baseline** | **92.094405** | 17172 | 2 / 17 |
| `bool raid;` after `pirates` | 92.094405 | 17172 | 2 / 17 |
| `bool raid;` after `raidRoll` | 91.252682 | 17172 | 2 / 17 |
| `bool raid = false;` after `mission` check | 91.252682 | 17172 | 2 / 17 |
| `bool raid = false;` before `introSystem` | 91.252682 | 17172 | 2 / 17 |
| `security` before `raid` | 92.094405 | 17172 | 2 / 17 |
| `bool raid = cmp; if (intro&&!hc) raid=false;` | 83.546392 | 17228 | 16 / 2 |
| `if (!hc && intro) raid=false; else raid=cmp;` | 74.221525 | 17172 | 13 / 2 |
| `else if (i > n \|\| !hasWanted)` reorder | 74.393264 | 17180 | 17 / 5 |
| `ArraySetLength` sum reorder (reverse) | 76.115832 | 17176 | 4 / 4 |
| `unsigned blueprint` moved after `wanted` | 76.035235 | 17408 | 4 / 2 |
| `int introSystem` | 78.952585 | 17164 | 4 / 2 |
| `int siegedStation` | 72.267523 | 17344 | 6 / 25 |
| all-bool Ghidra guns shape | 73.360284 | 17340 | 4 / 2 |

Conclusion: the swap is a symptom of the remaining ~475-instruction code
difference, not an independently fixable source choice.  Fixing it requires
first matching the surrounding code.

## 2. `bool` vs `int` for `alienWave` / `bigAlienWave` / `pirateFortress`

Ghidra shows `bVar7 = alienWave`, `bVar4 = bigAlienWave`, `bVar3 =
pirateFortress` as bools, and the original merges both `i == first && flag`
predicates into `eor`/`orrs` at `0xb2e04` and `0xb2f2a`:

```text
subs.w r0, fp, r3      ; i - first
mov    r1, r0
it ne
movne  r1, #1
ldr    r2, [sp,#60]    ; alienWave (0/1)
eor.w  r2, r2, #1
orrs   r1, r2
bne.w  ...
```

A `bool alienWave` *does* reproduce that merged predicate, but it re-lays the
whole alienGuns loop and loses ~3.8 pp.  A separate `bool alienFlag =
alienWave != 0` is optimised away (neutral).  Bitwise `&` forms are neutral
only for the alien predicate and much worse for the fortress predicate.

| variant | pct | size |
|---|---:|---:|
| **baseline (`int` for all three)** | **92.094405** | 17172 |
| `bool alienWave` (tuned loop) | 88.259577 | 17156 |
| `bool alienWave` + faithful loop order | 88.395510 | 17164 |
| `bool alienWave` + `(int)alienWave*(7+bigAlienWave)` | 87.941857 | 17148 |
| `bool bigAlienWave` + `int alienWave` | 89.204358 | 17164 |
| `bool pirateFortress` | 63.110964 | 17296 |
| `bool alienWave` + `bool pirateFortress` | 62.146986 | 17292 |
| all three `bool` | 71.893052 | 17316 |
| `char`/`unsigned char`/`signed char`/`short` `alienWave` | 89.424585 | 17156 |
| `bool alienWave` with `true`/`false` | 88.259577 | 17156 |
| `bool firstAlien = (i==first) & (alienWave!=0)` | 92.094405 | 17172 |
| `bool firstFort = (i==first) & (pirateFortress!=0)` | 86.197788 | 17188 |
| Ghidra guns shape (`g=7; if(big) g=8; if(!aw) g=0;`) with `int` | 91.797326 | 17172 |
| same with all `bool` | 73.360284 | 17340 |
| same with `bool alienWave` only | 63.077939 | 17304 |

The `int` form is a tuned local optimum; the faithful `bool` form scores
lower (see the task note: a faithful source shape can score lower than a
tuned one).

## 3. AlienGuns table select: still a CMOV, not a branch

Original `0xb32c6` selects the table with a real in-loop branch:

```text
ldr   r0, [sp,#64]     ; bigAlienWave
cbz   r0, .Lout
ldr   r1, [pc,#732]    ; kAlienStationTurrets
add   r1, pc
b     .Lsel
.Lout:
ldr   r1, [pc,#728]    ; kAlienOutpostTurrets
add   r1, pc
.Lsel:
add.w r0, r1, r9       ; row = t + k
```

Ours emits `cmp.w r1, r2, asr #31; it ne; movne`.  Confirmed again this pass:
`switch` (case 0/default), `if/else`, and `?:` with the `(bigAlienWave + (i<0))`
anchor all produce byte-identical code (92.094405 / 17172).  Without the
anchor the select is LICM-hoisted (17140 B, 88.186246).  The branchy form is
not reachable from clang 7.0.2 C++ source (see wave-110 note for the
`FoldTwoEntryPHINode` / `LowerSELECT_CC` analysis).

## 4. AlienGuns-loop statement order

The original statement order is `setPosition`, `setScaling(6.0f)`,
`cargo = nullptr`, `shipGroup = localRace` (`0xb3364`-`0xb337e`).  The tuned
baseline issues `setScaling` before the table select and `shipGroup` before
`cargo`, and scores higher than the faithful order:

| variant | pct | size |
|---|---:|---:|
| **baseline (setScaling early, shipGroup then cargo)** | **92.094405** | 17172 |
| setScaling after setPosition, shipGroup then cargo | 92.028388 | 17172 |
| setScaling after setPosition, cargo then shipGroup | 92.011883 | 17172 |
| setScaling after cargo/shipGroup (last) | 90.823568 | 17164 |

## 5. Case 3/5 loop bound: original reloads `size()` each iteration

Original `0xb41c8` reloads `this->enemies` and its count every iteration and
compares with `bcc` (unsigned); ours hoists the count to `sp+120` and uses
`bne`.  Source variants do not change this (the hoist is a machine-level
LICM decision):

| variant | pct | size |
|---|---:|---:|
| **baseline `for (unsigned i...)`** | **92.094405** | 17172 |
| `(unsigned) size()` cast | 92.094405 | 17172 |
| `this->enemies->count` | 92.094405 | 17172 |
| `int i; i < (int) size()` | 92.077901 | 17172 |
| `i < size()` with `++i` | 92.094405 | 17172 |
| `size() > i` | 92.061396 | 17172 |
| `i != size()` | 88.111542 | 17180 |
| `unsigned n = size(); i < n` | 86.209172 | 17196 |
| `Array<KIPlayer*> *es = this->enemies; i < es->size()` | 86.209172 | 17196 |

## 6. Convoy switch block order (`0xb13d0`-`0xb1400`)

The `if (k==2) ... else if (k==1) ... else if (k==0) ... else` chain compiles
in the original as `cmp #2; beq; cmp #1; beq; cbnz; case0` (block order
0,1,2,default) but in ours as `cmp #1; beq; cmp #2; bne; ...` (block order
2,0,1,default).  This is a block-layout choice; reordering the source `if`
chain changes the semantics of the offsets and scored worse in every tested
form (the wave-110 note records the same for the convoy if-chain).

## 7. Other neutral / worse variants measured this pass

| variant | pct | size |
|---|---:|---:|
| reorder `bigAlienWave`/`alienWave`/`pirateFortress` decls | 89.824214 | 17172 |
| reorder `pirateFortress` first | 89.506682 | 17180 |
| `int alienGuns = 0; if (alienWave) ...` | 92.094405 | 17172 |
| reorder `alienGuns`/`fortressGuns` decls | 92.094405 | 17172 |
| `alienGuns = alienWave ? (7+bigAlienWave) : 0` | 90.127126 | 17164 |
| `alienGuns = alienWave * (bigAlienWave+7)` | 88.714604 | 17156 |
| wingman loop `j=0; j<n; j++` (index `j+1`) | 88.712740 | 17156 |
| wingman loop `j != n+1` | 91.945866 | 17172 |
| wingman `setHitpoints` before `setMaxHitpoints` | 92.061396 | 17172 |
| `if (i==0 && hasWanted)` | 80.445728 | 17164 |
| `else if (i > n \|\| !hasWanted)` | 74.393264 | 17180 |
| friend loop `i != friendCount` | 86.555592 | 17240 |
| friend loop `unsigned` | 92.061396 | 17172 |
| jumper loop `unsigned` | 92.077901 | 17172 |
| station loop `unsigned` | 92.077901 | 17172 |
| `bool alienFlag = alienWave != 0` (both flags) | 85.091989 | 17176 |
| `bool alienFlag` alone / `bool fortFlag` alone | 92.094405 | 17172 |
| `ArraySetLength` sum reorder (friend first) | 92.094405 | 17172 |
| `int slot` sum reorder | 92.094405 | 17172 |
| `miningPlantIndex` sum reorder | 92.094405 | 17172 |
| hp formula term reorder (gameWon first) | 91.615778 | 17172 |
| hp formula term reorder (hitpoints, gameWon, level) | 91.698300 | 17172 |
| `wingmanHp = hp >> 1` | 89.285125 | 17164 |
| `Player *wantedPlayer` local | 90.499381 | 17164 |
| `PlayerFixedObject *wantedShip` local | 92.094405 | 17172 |
| reorder `field_164`/`field_168` stores | 91.978874 | 17172 |
| reorder `bigShipsCount`/`jumperCount` decls | 92.094405 | 17172 |
| `Status *st = Globals::status;` after alien-orbit block | 58.507659 | 17232 |
| swap `stationIndex`/`raidRoll` decls | 92.011883 | 17172 |
| swap `raidRoll`/`freelance` decls | 92.011883 | 17172 |
| `freelance` first | 92.028388 | 17172 |
| `introSystem` direct initialiser | 92.094405 | 17172 |
| `introSystem` ternary | 92.094405 | 17172 |
| `introSystem` condition reorder | 83.898095 | 17220 |
| `int hasWanted` | 87.694087 | 17148 |
| `int nameLeader` | 88.672552 | 17180 |
| `int alienRace` | 92.094405 | 17172 |

## Conclusion

The source shape is essentially right: the call sequence matches, the `hp`
computation is byte-identical, and the only frame-layout difference is the
`raid`/`status` slot swap.  The residual 475/483 instructions are dominated by
`RegAllocGreedy` register assignment and block layout, which are chaotic in a
6 k-instruction function: every semantic-preserving perturbation either leaves
the code identical (neutral) or moves several percent in the wrong direction.
No variant beat the committed baseline's 92.094405 % / 17172 B.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w5915/score.py _ZN5Level13createMissionEv diff
```

Scratch tooling (not committed): `/dev/shm/w5915/score.py` (official verifier
normalisation, unrounded), `sweep.py`/`sweep2.py` (apply replacement on top of
`Level.base.cpp`, `link_so`, print score + `[sp,#84]`/`[sp,#88]` counts),
`slots.py`, `group.py` (diff regions grouped by source line / address).
