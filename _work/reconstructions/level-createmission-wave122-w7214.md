# w7214: `Level::createMission` (92.326733 -> 92.326733 %, 17172 B) — wave-122
# case/convoy/turret block sweep + slot/CFG re-check; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7214`, branch `wave122-w7214`,
based on current master (`29cd275f6`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6…bed9483`, image base 0x10000; function `0xaffd8`-`0xb42ec`,
17172 B).  Continues `level-createmission-wave119-w6962.md`,
`…-w6970.md` and `…-wave121-w7112.md`.  `src/game/world/Level.cpp` is
unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The fuzzy ratio is `2*M/(6055+6065)` with `M = 5595`; one extra matching
instruction is `+0.0165 pp` (92.3267 -> 92.3432).

## 0. New evidence: the call sequence is identical

`calls2.py` extracts every `bl`/`blx` in address order from both binaries and
diffs them.  Both sides issue **810 calls**; the sequences are identical except
for exactly two things:

```text
U +0x03332  PlayerTurret::setScaling       (moved from O +0x03392)
O +0x038bc/38c4/38f2/38fa/3928/3930  blx r5  (ours: blx r4)
```

So the source is semantically correct everywhere — there is no missing or
extra statement, no wrong callee, no wrong constant.  The whole residual is
instruction scheduling, register allocation, stack-slot colouring and
MachineBlockPlacement.

Per-0x400-region instruction-count deltas (ours - original):

```text
0x00000 +0   0x00400 +0   0x00800 +0   0x00c00 +0   0x01000 +0
0x01400 +0   0x01800 +2   0x01c00 +0   0x02000 +0   0x02400 +0
0x02800 +0   0x02c00 -1   0x03000 +1   0x03400 +3   0x03800 +2
0x03c00 -1   0x04000 +4    total +10
```

## 1. The `sp+84`/`sp+88` slot is the stack protector, not `&Globals::rnd`

The assignment text calls the second slot `&Globals::rnd`; it is actually the
`__stack_chk_guard` address.  Prologue at `+0x10`:

```text
ldr r0,[pc,#896]   ; GOT slot for __stack_chk_guard (R_ARM_GLOB_DAT @ 0x20fd04)
ldr r1,[pc,#900]   ; GOT slot for Globals::status
add r0,pc
add r1,pc
ldr.w fp,[r0]      ; fp = &__stack_chk_guard
ldr r5,[r1,#0]     ; r5 = Globals::status
ldr.w r1,[fp]      ; guard value
str r1,[sp,#396]   ; save canary
```

`fp` is spilled at `+0x4c` (`str.w fp,[sp,#88]` orig / `[sp,#84]` ours) and
reloaded in the shared epilogue at `+0x2f2`; the `raid` alloca/spill is the
other slot.  The wave-90 note measured the mechanism exactly: `StackSlotColoring`
gives the **lowest** frame index (which PEI lays at the **highest** `sp`
offset) to the highest-weight interval.  Ours:

```text
raid   weight 1.197958  -> fi#83 -> sp+88
guard  weight 1.193760  -> fi#84 -> sp+84
```

The original is the mirror image, so in the original `guard > raid`, i.e. the
epilogue block frequency must exceed `1.197958 - 0.625000 = 0.572958`; ours is
`0.568760`.  The gap is 0.0042 (0.7 %).  This is a global CFG/branch-probability
effect, not a local source-order effect (wave-90 §"What was tried").

This session re-confirmed it with two variants that change the CFG in the two
places the wave-90 note identified as the likely upstream cause — **neither
flips the slot**:

| variant | guard slot | % | size |
|---|---|---:|---:|
| baseline | sp+84 | 92.326733 | 17172 |
| case-183 `goto` chain (test order 2,1,0) | sp+84 | 92.178218 | 17172 |
| branchless `!((i!=X) \| !alienWave)` | sp+84 | 83.283828 | 17176 |

## 2. New block evidence: case-2 convoy and case-3/5 loop reloads

`abstract.py` (register/slot-free diff) isolates the remaining *source-shape*
deltas.  Two are worth recording because they are loop-invariant-code-motion
differences, not just register names:

**case-2 convoy loop (`+0x1964`).**  Original reloads `this->enemies` for each
of `setSpeed`, `setRotationSpeed`, `player`; ours caches it in `r4` via the
`Array<KIPlayer *> *enemies = this->enemies;` local.  Original also stores
`cargo` (`+0x50`) before `crateCaptured` (`+0x4c`); ours stores them in the
opposite order (LLVM's scheduler reorders two non-overlapping stores to the
same base).  Removing the cache is 8 B larger and scores worse:

| variant | % | size |
|---|---:|---:|
| baseline (cached `enemies`) | 92.326733 | 17172 |
| no cache / `this->enemies` per statement | 90.009075 | 17180 |
| swap `cargo`/`crateCaptured` source order | 92.326733 | 17172 |

**case-2 second loop (`+0x1630`).**  Original reloads `localRace` from
`sp+116` inside the loop (`ldr r5,[sp,#116]`); ours hoists it to the loop
preheader (`+0x15b2`).  `localRace` is loop-invariant, so LICM hoists it; no
source spelling prevents this (`int race = localRace;` inside the body,
`unsigned j`, `(int) size()` all canonicalise back to the same IR and score
92.326733).

## 3. Variants measured this session

All are against the pristine baseline (92.326733 % / 17172 B) unless noted.
`tryall.py` applies replacements to the whole file (not first-occurrence only)
and rebuilds/relinks/scores each.

### 3a. `raid` slot / declaration

| variant | % | size |
|---|---:|---:|
| `bool raid = false;` / ternary init / braces | 92.326733 | 17172 |
| `char` / `unsigned char` / `signed char` / `unsigned` | 92.161716 | 17172 |
| `volatile bool raid` | 82.499587 | 17196 |
| `bool raid[1]` (+ all uses `raid[0]`) | 92.326733 | 17172 |

### 3b. `case4Tail` latch

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile unsigned n`** | **92.326733** | 17172 | 6065 |
| plain `unsigned n` (register-cached) | 92.178218 | 17172 | 6065 |
| `for (i = 0, n = size(); …)` | 92.178218 | 17172 | 6065 |
| faithful `i < this->enemies->size()` (unsigned / cast / int / while) | 91.341312 | 17164 | 6060 |

The faithful re-read is 8 B short: the original keeps `fp = 1` (the `hostile`
argument) and `sl = Globals::globals`, spilling `Globals::rnd` at `sp+120`
and reloading it each iteration; ours keeps `Globals::rnd` in `sl` and
materialises the constant `1` per iteration.

### 3c. Branchless `&&` at `+0x2e04` / `+0x2f52`

| variant | % | size |
|---|---:|---:|
| **baseline short-circuit** | **92.326733** | 17172 |
| `!((i != X) \| !alienWave)` (LICM-hoisted `!alienWave`) | 83.283828 | 17176 |
| `!((i != X) \| !alienWave)` + faithful tail | 83.263219 | 17184 |
| `!((i != X) \| !alienWave)` + plain `n` | 83.135314 | 17176 |
| `(i == X) & alienWave` | 84.155887 | 17204 |
| `(i == X) & (alienWave != 0)` / `& !!alienWave` | 63.045811 | 17312 |
| `alienWave && i == X` (operand swap) | 92.326733 | 17172 |
| `(i == X) * alienWave` | 92.095710 | 17172 |
| `*(volatile int *)&alienWave` read | 70.707738 | 17124 |
| `!((i != X) \| !*(volatile int *)&alienWave)` | 70.560053 | 17132 |

The original's inline `eor.w r2,r2,#1` is only reachable if `!alienWave` is
*not* loop-invariant; every spelling that reaches the branchless path makes
LICM hoist the negation into the preheader (+4 B) and collapses the layout.

### 3d. Alien-turret loop ordering

| variant | % | size |
|---|---:|---:|
| **baseline: `setScaling` before `setRotation`, shipGroup first, fresh load** | **92.326733** | 17172 |
| `setScaling` after `setPosition`, shipGroup first, fresh load | 92.277228 | 17172 |
| `setScaling` after `setPosition`, cargo first, fresh load | 92.260726 | 17172 |
| `setScaling` last (after cargo/shipGroup) | 92.244224 | 17172 |
| `setScaling` after position, cached `gun` | 92.209936 | 17172 |
| plain `bigAlienWave ? A : B` / `!= 0` / `+ (i & 0)` | 88.413577 | 17140 |
| `(bigAlienWave + (i >> 31))` anchor | 92.326733 | 17172 |

### 3e. `case 183` offset chain

| variant | % | size |
|---|---:|---:|
| **baseline if-chain 2,1,0** | **92.326733** | 17172 |
| `goto` chain (test order 2,1,0, body order 0,1,2,default in source) | 92.178218 | 17172 |
| ascending if-chain 0,1,2 | 92.178218 | 17172 |

The `goto` form does produce the original's test order `2,1,0` and body order
`k0,k2,k1,default` (MBP still puts k2 before k1), but the literal-pool order
changes and costs more than it recovers.

### 3f. Misc re-checks (neutral unless noted)

* `nameLeader` `goto case4Else` / `if (1) goto` after the name body: 92.326733.
* `case 4` `if (nextInt(2) == 0) goto else` / cached `Route *r`: 92.326733.
* `case 4` `goto` from the false branch / guard form: 91.162637 / 17168.
* `int &bigShipsCount = this->field_168` references: 92.326733;
  direct `this->field_168` member access: 86.383892 / 17156.
* inlining `stationRace`: 92.326733; `int alienRace`: 92.326733;
  inverted `alienRace` sense: 90.774055 / 17164.
* dropping `int guns = alienGuns + fortressGuns;`: 88.855894 / 17200.
* `case 2` loop `int race = localRace` / `unsigned j` / `(int) size()`:
  92.326733.

## 4. Conclusion

HEAD remains a strict local optimum at **92.326733 %** (17172 B, 6055/6065
insns).  ~55 variants were measured this session on top of the ~700 in the
preceding wave-119/121 notes; none beat the baseline.  The call sequence is
identical to the original, so the residual is entirely backend decisions:

1. `raid`/`__stack_chk_guard` `sp+84`/`sp+88` swap (17 sites, ~+0.28 pp) —
   needs the shared-epilogue frequency to rise from 0.568760 to > 0.572958;
   neither the case-183 chain nor the branchless `&&` moves it.
2. case-183 k-chain body order `0,1,2,default` vs ours `2,0,1`/`0,2,1` —
   MachineBlockPlacement, not source.
3. branchless `&&` — LICM hoists `!alienWave`; the inline `eor` is unreachable.
4. `case4Tail` `fp = 1` vs `fp = &Globals::rnd` — RA, and the faithful re-read
   is 8 B short.
5. case-2 convoy store order / `localRace` reload and case-3/5 `localRace`
   reload — scheduler/LICM, not source.

## 5. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=… -DGOF2_FMOD_LIBDIR=…
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7214/score.py            # 6-decimal ratio, size, insns
python3 /dev/shm/w7214/dump2.py _ZN5Level13createMissionEv LO HI
python3 /dev/shm/w7214/abstract.py         # register/slot-free structural diff
python3 /dev/shm/w7214/regions.py          # unmatched-original census per 0x400
python3 /dev/shm/w7214/cnt.py              # per-region instruction counts
python3 /dev/shm/w7214/calls2.py           # call-sequence diff (810 calls)
python3 /dev/shm/w7214/tryall.py vN.py     # apply-all/build/score/restore
python3 /dev/shm/w7214/slot.py             # guard spill slot probe
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w7214` (`Level.base.cpp` = pristine HEAD copy;
`v1.py`…`v18.py` = variant batches for this session).
