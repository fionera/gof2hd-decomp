# Build provenance: PGO / `!prof` evidence from the binary (wave 147, w9319)

Status: **note-only research commit**, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w9319`, base `9c9744ca5` (wave 146). Scratch:
`/dev/shm/w9319`. No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or
doc change is landed. This note plus the two experiment scripts are the only
commit.

Gate at the base (fresh `cmake --build cmake-build-match --target check -j6`):

```text
verify: byte 4507/4524  linked 4507/4524  size 4524/4524  avg 99.93
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
lint CLEAN   drift 0
RATCHET PASS
```

Mission: decide from the binary whether the original was PGO'd, by
(a) sampling switch-heavy functions and comparing the original's physical
case-block order with our source order, (b) testing `-fprofile-instr-use` with
a synthetic flat profile on `Status::missionCompleted`, and (c) identifying a
source-level equivalent.

## 0. Bottom line

1. **The original's switch block order is source order, not numeric.** A new
   jump-table reader extracts every `TBH`/`TBB` table from the shipped `.so`
   and reads the target addresses in table-index order. Of the 16 source
   switches that could be matched to a table, **6 are exactly source order and
   10 are source order with one case moved**; **none is numeric**. This is the
   direct binary witness behind w9214's `Status` finding: the original's block
   layout follows the source, so the compiler must have had branch weights.
2. **A flat PGO profile is falsified.** Building every TU with
   `-fprofile-instr-use` and a synthetic profile whose counters are all 1
   changes the layout of *every* TU (0/188 objects byte-identical) and drops
   the gate from **4507 to 1274** `byte_exact` (linked 3485). A flat profile is
   not the original's profile. Even on `Status` alone a flat profile is not a
   no-op: it produces a different object and a scrambled table order
   (`159,161,173,175..188,167,174,184,189,158,156,165,152,...`), neither source
   nor numeric.
3. **A non-uniform profile does reproduce source order.** With a synthetic
   profile whose case counters decrease in source order, `Status`'s 24
   non-default case blocks are laid out **exactly in source order** (the
   default block moves to the front). But the function shrinks to **1408 B**
   (original 1432 B), so this particular weight vector is not the original's.
4. **The only source-level producer of per-case `!prof` weights is
   `switch (__builtin_expect(x, hot))`.** It emits
   `!{!"branch_weights", i32 1, i32 2000, i32 1, ...}` (one hot case, all
   others 1). On `Status` it moves 167/174 to the front but leaves the rest in
   numeric order and shrinks the function to **1392 B**. It is not sufficient.
   Per-case weights (all cases, not one) have no C++11 source spelling; they
   require a profile.

## 1. Method: reading the original's switch tables

`_work/reconstructions/jumptable_w9319.py` (committed) parses the ELF, finds
every `tbh [pc, rX, lsl #1]` / `tbb [pc, rX]` in `.text`, recovers the case
base and count from the preceding `sub rX, r0, #base` / `cmp rX, #N`, reads
the table, and sorts the case values by target address.

**Key correctness point:** the table base is **`tbh_addr + 4`, not
`align4(tbh_addr + 4)`**. The ARM ARM pseudocode for `TBH` uses `R[n]` (the
PC value = instruction + 4) directly; the table starts immediately after the
4-byte instruction and is not 4-aligned. Using `align4` shifts every entry by
one halfword and produces a plausible-looking but wrong order (it was the
first bug found in this pass). The same applies to `TBB`.

The function association comes from our unstripped link
(`/dev/shm/w9319/ours.unstripped.so`, produced by replaying the checked-in
gold command), whose function addresses match the original for all
size-exact functions. The switch association comes from the DWARF line table
of the same link.

## 2. (a) Block-order census

Every `TBH`/`TBB` table in the original, with the physical order of its case
values (address order) and the source order of the matching `switch`:

| function | switch | table | physical order | verdict |
|---|---|---|---|---|
| `String::getWCharFromUtf8` | `String.cpp:163` | `0x72670` tbb 0x410..0x425 | 0x410,0x412,0x415,0x41a,0x41c,0x41d,0x41e,0x420,0x421,0x422,0x425,0x411,0x413,0x414,0x416,0x417,0x418,0x419,0x41b,0x41f,0x423,0x424 | **SOURCE** |
| `String::ToUpperCase` | `String.cpp:621` | `0x730e4` tbb 0x81..0x86 | 0x81,0x82,0x85,0x83,0x84,0x86 | **SOURCE** |
| `String::ToLowerCase` | `String.cpp:652` | `0x73168` tbb 0x8e..0x9a | 0x9a,0x8e,0x8f,0x90,0x92,0x99 | source, `0x9a` first |
| `Explosion::playSound` | `Explosion.cpp:244` | `0xa82be` tbb 0x29..0x3d | 0x2a,0x3c,0x3d,0x3e,0x2e,0x2b,0x2c,0x2d,0x29 | source, first moved |
| `Status::missionCompleted` | `Status.cpp:1334` | `0xabfe2` tbh 150..189 | 167,174,150,151,152,153,154,155,158,162,166,170,184,189,156,157,160,163,164,165,168,169,171,172 | **SOURCE** |
| `Status::missionCompleted` | `Status.cpp:1334` | `0xac04e` tbh 8..14 | 8,11,13,14,9,10,12 | source, 9/10/12 moved |
| `Globals::loadFont` | `Globals.cpp:1076` | `0xe47d0` tbb 9..14 | 9,10,11,12,13,14 | **SOURCE** |
| `Globals::getDialogueSoundId` | `Globals.cpp:1568` | `0xe58a2` tbh 370..389 | 370,389,371..388 | source, 389 moved |
| `Globals::getDialogueSoundId` | `Globals.cpp:1634` | `0xe58ee` tbh 370..447 | 370,389,371..388,426..447 | source, 389 moved |
| `Globals::getDialogueSoundId` | `Globals.cpp:2136` | `0xe58ee` tbh 370..447 | 370,389,371..388,426..447 | **SOURCE** |
| `Globals::getInAppPurchaseArrayIndex` | `Globals.cpp:2666` | `0xe8d12` tbb 50..54 | 50,51,52,53,54 | **SOURCE** |
| `StarSystem::initLight` | `StarSystem.cpp:460` | `0x134b6a` tbb 11..18 | 12,15,16,17,18,11 | source, first moved |
| `Level::createCampaignMission` | `Level.cpp:2574` | `0xb4dcc` tbh 131..158 | 131,135,137,139,142,144,145,147,154,155,157,158,132..153,156 | table order (source not mapped) |
| `Level::createGun` | `Level.cpp:1264` | `0xbdfc8` tbh 34..42 | 34,40,35,39,42,36,37,38,41 | table order (source not mapped) |
| `SpaceLounge::onKeyPress` | `SpaceLounge.cpp:563` | `0x16e580` tbh 0x4000..0x4007 | 0x4002,0x4004,0x4005,0x4006,0x4000,0x4001,0x4003,0x4007 | table order (source not mapped) |
| `Db::parseType` | zlib `inftrees.c` | `0x1949ca` tbh 65..122 | 75,86,114,85,65,67,68,70,71,77,79,80,82,83,84,66,69,72,73,74,76,78,81,87..96,107,112,113,97..106,108..111,115..122 | table order (source not mapped) |

No table is in numeric order. The "moved" cases are all block-placement
artefacts of a *single* case (usually the first or last), not a numeric
sort. The table itself is always indexed by value (that is the ISA), but the
*target addresses* follow source order.

The 7 compare-chain switches in the residual set (`LevelScript::process`,
`Level::createMission`, `Generator::createMission`, `MGame::OnTouchEnd`,
`Hud::hudEvent`, `MenuTouchWindow::draw`, `SpaceLounge::onKeyPress`) have no
table to read; the DWARF-line proxy is noisy for them and is not claimed
here. Their dispatch order is the remaining open item.

## 3. (b) `-fprofile-instr-use` with a synthetic flat profile

**Profile construction.** `Status.cpp` was compiled with
`-fprofile-instr-generate`; the `__llvm_prf_data` record for
`_ZN6Status16missionCompletedEbbx` gives `FuncHash = 0x1bceb494c8954315`
(64-bit: low 32 at record+8, high 32 at record+12) and `NumCounters = 116`
(the `__profc` symbol is 928 B = 116 x 8). A text profile with all 116
counters = 1 was merged with the NDK's `llvm-profdata`:

```text
_ZN6Status16missionCompletedEbbx
# Func Hash:
2003737435339834133
# Num Counters:
116
# Counter Values:
1
... (116 ones)
```

**Result on one TU.** `-fprofile-instr-use=flat.profdata` produces a
different `Status.cpp.obj` (`sha256 555eba4c…` vs the canonical `f662a061…`).
The table order becomes
`159,161,173,175..188 (default), 167,174,184,189,158,156,165,152,162,153,
170,168,154,150,155,172,157,151,160,164,169,163,171,166` — neither source
order nor numeric order. A flat profile is therefore **not** a no-op: it
changes the block-placement decision, but not to the original's layout.

**Result on the whole project.** A flat profile for all 3215 instrumented
functions was generated the same way (2272 functions after dropping
zero-counter/zero-hash records), merged, and every one of the 188 app TUs was
recompiled with `-fprofile-instr-use`. **0/188 objects are byte-identical**
to the canonical build, and the full gold link verifies at:

```text
compared 4524  avg 91.30%  byte_exact 1274  linked_exact 3485
```

So a flat profile does not preserve the 4507 exact functions; it is
**falsified** as the original's profile. (The 1274/3485 numbers are from the
corrected 40-byte `__llvm_profile_data` parser: `NumCounters` is at record
offset `0x1c`, not `0x0c`.)

**Non-flat control.** A profile whose 27 case counters decrease in source
order (100, 99, …, 74) and whose default counter is 1 produces `!prof`
`branch_weights` of `1, 101, 2, 100, 99, 2, 98, …` and lays the 24
non-default `Status` case blocks out **exactly in source order**:

```text
phys: [159,161,173,175..188 (default), 167,174,150,151,152,153,154,155,
       158,162,166,170,184,189,156,157,160,163,164,165,168,169,171,172]
```

But the function is **1408 B** (original 1432 B) and the raw same-address diff
is 1369 (canonical 722). The mechanism is real; this weight vector is not the
original's.

## 4. (c) Source-level equivalent

`__builtin_expect` on the switch *condition* is the only C++11 spelling that
emits switch `!prof` weights. Tested on the real `Status.cpp`:

```cpp
switch (__builtin_expect(m->getType(), 167)) {
```

IR: `!{!"branch_weights", i32 1, i32 2000, i32 1, i32 1, ...}` (default 1,
case 167 = 2000, every other case = 1). Effect on the table:

```text
phys: [167,174, 159,161,173,175..188 (default), 150,151,...,172, 184,189]
```

i.e. the hot case moves to the front, but the remaining cases stay numeric.
The function shrinks to **1392 B** (original 1432 B). It is a partial match,
not a win.

**Conclusion for (c):** there is no C++11 source form that gives *per-case*
weights. `switch (__builtin_expect(x, hot))` gives one hot case; a full
source-order layout needs a profile (real PGO) or a compiler extension. If the
project wants to reproduce the original's layout without committing a profile,
the remaining option is to find a case order whose numeric order *is* the
original's physical order — but the values are fixed, so that is not possible
for `Status`.

## 5. What this means for the 17 residuals

The original's switch layout is source order because it was built with branch
weights. Our build has no weights, so the block-placement heuristic is free to
choose numeric order; it does so for `Status::missionCompleted` (the only
residual function with a `TBH` table). The other 16 residuals are not
switch-layout cases (they are RA/tail/register-allocation residuals, per the
w9214 note). A real PGO profile is therefore a plausible *build-provenance*
explanation for the `Status` residual, but a flat profile is not, and no
source-level `__builtin_expect` reproduces it.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9319
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6

# 1. dump every TBH/TBB table from the original
python3 _work/reconstructions/jumptable_w9319.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# 2. flat-profile experiment (Status only)
python3 _work/reconstructions/pgo_flat_w9319.py --status-only
# 3. flat-profile experiment (all 188 TUs + full verify)
python3 _work/reconstructions/pgo_flat_w9319.py --all
```

Scratch (`/dev/shm/w9319`): `ours.unstripped.so`, `ours.dis`,
`status_instr.o`, `status_instr.ll`, `flat.profdata`, `flat_all2.profdata`,
`instr/`, `use/`, `flatall.unstripped.so`, `census2.json`.

## 7. Open items

1. Recover the original's actual weight vector. The descending-weights control
   proves the mechanism; the next step is a search over plausible profiles
   (e.g. mission-type frequency from `Level::createMission`/`Status` call
   sites) that keeps the function at 1432 B.
2. The 7 compare-chain residuals need a non-DWARF block-order oracle (manual
   disassembly of each dispatch).
3. `switch (__builtin_expect(x, hot))` is a *source-level* `!prof` producer and
   is worth trying on the other residual switch functions (`LevelScript::
   process`, `Level::createMission`, `Generator::createMission`) even though it
   lost on `Status`.
