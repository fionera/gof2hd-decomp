# w10713 (wave 161): `Level::createMission` — case-183 default-store
# permutation 2,0,1; raw 5,461 -> 5,454, aligned 781 -> 786, size 17,172 B

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10713`, based on master
`01c7f92d6` (wave 160).  Continues
`level-createmission-wave160-w10613.md`, `wave160-merge-tests.md`,
`level-createmission-wave159-w10509.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B).  Ghidra
MCP unavailable; NDK `objdump` is the authority.  Raw same-address diff:
`/dev/shm/w10713/fdiff.py`; instruction-index alignment:
`/dev/shm/w10713/align_diff.py` (worktree-local copies with `OURS` pointed at
this worktree's `cmake-build-match/libgof2hdaa.so`).

## Baseline (pristine HEAD 01c7f92d6, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5461   align 781
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 85,518   .ARM.extab 1,258   .rel.plt 0 (byte-identical)
```

## 1. Landed change (`src/game/world/Level.cpp`, case 183 `switch (k)` default)

The case-183 freighter loop stores a `float offset[3]` per `k`.  After the
wave-160 hoist (`w10612`) the six store permutations of the `k==3` default
block re-measure differently from w10613's table.  Exhaustive sweep, all
size-exact at 17,172 B unless noted:

| default store order | raw | align | size |
|---|---:|---:|---:|
| `1,2,0` (HEAD, landed w10613) | 5,461 | 781 | 17172 |
| **`2,0,1` (landed this wave)** | **5,454** | **786** | **17172** |
| `2,1,0` | 5,457 | 783 | 17172 |
| `0,2,1` | 5,460 | 786 | 17172 |
| `1,0,2` | — | — | 17188 (fatal) |
| `0,1,2` | — | — | 17164 (fatal) |

`2,0,1` also makes the case-0/1/2 literal pool match the original exactly
(`-3000, 20000, -50000, -7000, 10000, -30000, 25000, -36000`); only the
default triple's pool order still differs (`-45000,12000,6000` vs original
`6000,12000,-45000`).  The raw metric is the ground truth for byte-identical,
so this is a real -7 B `.text` win; the aligned metric rises +5 because the
greedy alignment penalises the moved block (same trade the coordinator
accepted for w10612's +77).

## 2. case-183 case1/case2 body swap (measured, not landable)

Original body layout: case0 `0xb1420`, **case1 `0xb142c`**, **case2
`0xb1438`**, default `0xb1444`.  Ours: case0, **case2**, **case1**, default.
Reordering the `switch` cases in source (0,2,1) does not change codegen
(raw 5,461 / align 781, byte-identical), confirming this is a sample-PGO
block-placement artefact, not a source-order lever.

## 3. `0xb2e04` / `0xb2f2a` branchless if-conversions (measured, not landable)

The original guards

```
if (i == start && alienWave) ... else if (... && pirateFortress) ...
```

are branchless in the original (`subs`/`it ne`/`movne`/`eor`/`orrs`), ours
emit short-circuit branches.  Tested `(a) & b` forms (sizes 17,184 / 17,192 /
17,188 — all fatal) and swapping the `&&` operand order (byte-identical
5,461 / 781).  No size-exact source form reproduces the if-conversion at
`-Oz`; it is a branch-weight/PGO signature.

## 4. 4-byte stack-slot permutation (root cause found, no source lever)

`-mllvm -print-machineinstrs` (`/dev/shm/w10713/pmi.txt`) and llc
`-debug-only=stack-slot-coloring` (`ssc_debug.txt`) show
`StackSlotColoring` sorts spill intervals by weight and assigns the lowest
free frame index.  Ours: `__stack_chk_guard` (fi#83, w=7.33) -> `sp+84`,
`raid` (fi#82, w=0.40) -> `sp+88`.  Original has guard -> `sp+88`,
raid -> `sp+84`.  The two intervals never overlap, so no slot sharing; a fix
needs the weight/interval-set to change, which no local-declaration reorder
found this wave.  Non-wins measured (all size 17,172 unless noted):
`int raid`/`unsigned char raid` (17,170), `bool raid` before `int security`
(17,170), `__builtin_expect(isEmpty(),0)` (17,232) / `(...,1)` (17,356),
`__builtin_expect` on the raid guard (17,200), dropping the `volatile` cast
on `Status *status` (identical), dropping the `(i<0)` turret anchor (17,156).

## 5. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5461 -> 5454   (-7)
                           align  781 -> 786    (+5)
.text size             0x1337c8 -> 0x1337c8 (exact)
.text diff             85,518 -> 85,511  (-7)
.ARM.extab size        0x26298 -> 0x26298 (exact)
.ARM.extab diff         1,258 -> 1,258
.rel.plt                    0 -> 0   (byte-identical)
.rodata/.data/.bss/.ARM.exidx 0 -> 0
.note.gnu.build-id         19 -> 19
file size             2,187,328 -> 2,187,328 (exact)
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
```

`check` is green.

## 6. Open questions

1. The case-183 case1/case2 body swap and the `0xb2e04` if-conversions are
   sample-PGO artefacts; reproducing them needs profile data or a clang flag
   (not allowed).
2. The 4-byte stack-slot permutation is the largest remaining non-PGO
   residual.  Root cause is `StackSlotColoring` weight order between the
   `__stack_chk_guard` spill and the `raid` spill; a source shape that changes
   either interval's weight would remove most `operand:ldr` mismatches.
3. The default triple's literal-pool order (`-45000,12000,6000` vs original
   `6000,12000,-45000`) still differs; the store order is pinned by codegen.


## 7. Additional wave-161 experiments (all measured, none landed)

Raw 5,454 / aligned 786 / 17,172 B unless noted.

| candidate | raw | size | why not |
|---|---:|---:|---|
| `blueprint` computed before `stationPirates` | 16,620 | 17,380 | size fatal |
| `int extraAliens` declared before `stationPirates` | 5,454 | 17,172 | identical codegen |
| `bool raid;` moved before `int security` | 5,454 | 17,172 | identical |
| `unsigned extraAliens` | 5,457 | 17,172 | +3 raw |
| `stationPirates`/`pirateEscorts` after `blueprint` | 16,618 | 17,348 | size fatal |
| `int raiders` (from `float`) | 15,539 | 17,204 | size fatal |
| drop `float raiders` intermediate | 15,539 | 17,204 | size fatal |
| `bool raid = false;` | 5,454 | 17,172 | identical |
| `int raid;` | 5,681 | 17,172 | +227 raw |
| `float raiders;` declared early | 5,454 | 17,172 | identical |
| `!((i != x) \|\| !alienWave)` (0xb2e04) | 5,454 | 17,172 | identical to `&&` |
| `(i == x) & alienWave` | 12,415 | 17,184 | size fatal |
| nested `if (i==x) { if (alienWave) ... }` | 15,116 | 17,152 | size fatal |
| `bool atFirst = (i==x)` then `&&` | 5,454 | 17,172 | identical |
| `alienWave && i==x` (operand swap) | 5,454 | 17,172 | identical |
| `__builtin_expect(alienWave,1)` | 11,829 | 17,180 | size fatal |
| `__builtin_expect(i==x,1) && alienWave` | 5,454 | 17,172 | identical |
| `__builtin_expect` both operands | 14,454 | 17,148 | size fatal |
| `__builtin_expect(alienWave,0)` | 15,935 | 17,136 | size fatal |
| case-2 convoy: swap `cargo`/`crateCaptured` stores | 5,455 | 17,172 | +1 raw |
| case-2 convoy: drop `enemies` local | 10,677 | 17,180 | size fatal |
| case-2 convoy: drop local + crate-first | 10,678 | 17,180 | size fatal |

The `0xb2e04` if-conversion is a branch-weight artefact: our MBP weights are
`alienWave` 75/25 and `i==x` 66.67/33.33 (pmi.txt bb.465/bb.466), so clang
emits two branches; the original's profile made it if-convert to
`subs/it ne/movne/eor/orrs`.  No size-exact source form reproduces it.

## 8. Stack-slot frame-object order (root cause detail)

The two swapped slots are `%stack.82` (raid, sp+88) and `%stack.83`
(`&__stack_chk_guard`, sp+84) in ours.  The frame objects fi#73..fi#100 are
laid out at sp+124 down to sp+16 (higher fi# = lower sp), so the original
must have the guard-addr slot at a *lower* fi# than raid.  The guard-addr
spill is emitted at b0024 (before the raid store at b04c0) yet gets the
higher fi#, i.e. the frame-object order is not code order.  No local
declaration/type reorder tested changes this ordering.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10713/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10713/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w10713/{fdiff.py,align_diff.py,cc.sh,probe.py,try*.py,
pmi.txt,ssc_debug.txt,Level7.ll,base.cpp}`.
