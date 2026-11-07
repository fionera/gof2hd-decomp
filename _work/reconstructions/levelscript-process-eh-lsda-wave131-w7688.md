# LevelScript::process — wave 131 / w7688 (EH/LSDA record decode + scope-placement probes)

Status: **research note, no landed source change**, 2026-11-07. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 normalised instructions. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `7c6933c2d`.

## Result

```
                         before (master)   after (no change)
raw ratio (difflib)      59.879643905      59.879643905
official verify pct      59.9              59.9
orig / ours insns        20169 / 20045     20169 / 20045
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
.text differing bytes    85379             85379
.ARM.extab differing     1435              1435
.ARM.exidx               0                 0
.rel.plt                 0                 0
.rodata/.data/.bss       0                 0
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The task prompt's "60.1%" is the pre-wave-129 number; the current master is
**59.9** (docs/IDENTICALITY.md wave-129 update already records 59.89 raw).

## 1. The LevelScript::process `.ARM.extab` record

Decoded with the committed `_work/reconstructions/exidx_table.py` decoder
(`.ARM.exidx` entry `0x1a12a0`, extab record `0x1c2af8`):

| field | original | ours |
|---|---:|---:|
| record length | **348 B** | **348 B** (size-exact) |
| LSDA offset | `0x1c2b08` | `0x1c2b08` |
| `lpStart` encoding | `0xff` (omit) | `0xff` |
| `ttype` encoding | `0xff` (cleanup only) | `0xff` |
| call-site encoding | `0x01` (ULEB) | `0x01` |
| call-site rows | **46** | **46** |
| rows with a landing pad | **24** | **24** |
| raw record bytes differing | — | **291 / 348** |

So the record is already the right *size* and the right *shape*; the 291
differing bytes are the ULEB row starts / lengths / landing-pad offsets, all of
which are code-layout derived.

## 2. Cleanup-scope comparison (the new evidence)

Every one of the 24 landing pads was resolved through the Thumb disassembly to
its destructor/delete action. The **action multiset is identical**:

* 4 × `String` single-temporary cleanups (`sp+112`, `sp+124`, `sp+220`,
  `sp+232`) — lengths all **8 B** in both.
* 1 × 4-`String` cascade (`sp+136`, `sp+268`, `sp+368`, `sp+280`) — the
  case-125 `setName(*getText(3212) + String(" ") + String(i-7))` expression.
  Both construct in the same order (`String(" ")` -> `operator+` ->
  `String(i-7)` -> `operator+`) and destroy in the same order
  (`sp+136` -> `sp+268` -> `sp+368` -> `sp+280`). **Total cascade span is 42 B
  in both.**
* 16 × `new`-temporary delete cleanups. Their *length multiset* is identical
  (4×6, 8×8, 1×12, 1×10, 1×4, 1×16). The `Route`/`Explosion`/`AEGeometry`
  cleanups all cover exactly the constructor call, as in the original.

The only two differences are:

1. **4-String cascade range split.** Original `(14, 8, 12, 8)`, ours
   `(10, 10, 14, 8)`; both sum to 42. This is pure instruction encoding:
   the original keeps the first `operator+` result in `sl`
   (`add.w sl,sp,#368; mov r1,r9; mov r2,r6; mov r0,sl; blx operator+` = 14 B),
   ours uses `r0` directly (`add r0,sp,#368; mov r1,r5; mov r2,fp; blx` = 10 B).
   Same calls, same objects, same scope — a register-colouring delta.
2. **One delete trampoline register.** The original has a `delete fp`
   trampoline (case-157 `new AEGeometry(0x37a7)` marker, row start 49682);
   ours deletes the same object through `r6` and has one extra shared
   `delete r5` trampoline instead. Again pure register colouring.

Conclusion: **the EH cleanup scopes are already faithful.** There is no
missing/extra destructor scope to recover from source-level scope placement;
the residual `.ARM.extab` delta is the same MachineBlockPlacement/register
permutation that drives the `.text` delta.

## 3. Scope-placement probes (all reverted)

Every variant was compiled with the real project flags, linked, and scored on
the official reconciled stream. Baseline: size 57232, raw **59.879644**.

| variant | size | raw % | note |
|---|---:|---:|---|
| baseline | 57232 | **59.879644** | |
| prologue accessors reordered `messages, player, enemies` | 57416 | 58.527971 | reproduces the original accessor order but +184 B; collapses byte_exact (see wave 130) |
| case-125 `setName` named temp `String name = ...; setName(name)` | 57296 | 59.139037 | +64 B |
| case-125 full named `s1/s2/s3` block | **57232** | 59.867701 | size-exact; cascade split becomes `(18,10,14,8)` |
| case-125 partial `s1/s2` + inline `String(i-7)` | 57244 | 59.207679 | +12 B |
| case-125 named `s3` only | 57464 | 54.792133 | +232 B, layout re-roll |
| case-125 named `s1` only (`String(" ")`) | 57468 | 54.939747 | +236 B, layout re-roll |
| case-91 state-0 nested `if` (polarity flip) | 57216 | 57.402940 | -16 B |
| case-40 state-4 `m_nState = 5` (faithful, replaces `speed = 0`) | 57224 | 59.094753 | -8 B; **byte_exact 4506 -> 3693** |
| case-40 state-1 `translate(0,0,(float)(-2*delta))` | 57256 | 59.259075 | +24 B |
| case-40 state-4 + state-1 translate | 57252 | 59.198846 | +20 B |
| case-157 marker via `m_pGeometry6` (no local) | 57240 | 59.175453 | +8 B |

The size-sensitivity is the hard constraint: **any** non-zero size delta makes
later symbols shift and the ratchet's `byte_exact` collapses (a -8 B process
alone gives 4506 -> 3693). Only a 57232 B variant can land. No 57232 B
scope-placement variant improved the raw ratio; the best size-exact one
(full named `s1/s2/s3`) is 0.012 raw below baseline.

## 4. Why the `.rel.plt` cold-block angle does not help here

`.rel.plt` is byte-identical (3674/3674 names, 0 differing bytes). The four
`String` destructors in the cascade are called **indirectly** through the
`R_ARM_GLOB_DAT` slot for `_ZN11AbyssEngine6StringD1Ev` (`.rel.dyn`, also
byte-identical), not through `.rel.plt`; the landing-pad blocks at
`0x145bf0..0x145c6a` are in the same order and at the same offsets for the
four `String` cleanups (`57132/57142/57152/57162`). Moving them would change
`.rel.plt` only if a moved block became the first reference to a PLT symbol,
and no such block exists in this function's cold tail.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
python3 /dev/shm/w5304/lsda.py cmake-build-match/libgof2hdaa.so   # row table
```

Scratch helpers (`score.py`, `run.sh`, `diffblocks.py`, `scope_table.txt`, the
experiment patches `e1..e12.txt`) live in `/dev/shm/w7688/` and are not part
of the tree.

## Open questions for the next wave

* The only size-exact structural lever left is the prologue register map
  (`messages` r4 vs r5, `status` r9 vs r4). The faithful accessor order costs
  +184 B; a shape that gets `messages` into r5 while keeping the current call
  order (e.g. by keeping `&m_pLevel` live across `getMessages`) would recover
  it without the size delta.
* The original tail-merges six `m_nState = N; break` stores into one shared
  `ldr r1,[sp,#88]; str r0,[r1,#28]; b post_switch` block at `0x1438d4`
  (states 1, 4, 5, 7, ?, 9). Ours has a shared setter too, at `0x142e7a`, but
  reached from a different set of transitions (1, 2, 4, ?, ?, ?). Aligning
  which transitions tail-merge is a block-placement problem, not a scope one.
