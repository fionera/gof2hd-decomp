# Radar::draw wave 137 (w8212) — "sink the status load into both predecessors" hypothesis (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w8212`, based on `master`
(`655e3bcc5`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline official fuzzy **75.4** (`tools/verify/verify.py --show`, exact
`75.4000`). Gate at start and end:
`RATCHET PASS`, byte **4507**, linked **4507**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0.

Read with `radar-draw-wave136-w8110.md`, `radar-draw-wave135-w7774.md`,
`radar-draw-wave134-w7753.md`.

This wave tested the wave-136 open question directly: *can a source shape sink
the `Globals::status` reload into both predecessors of the guard join while
keeping `otherLock` in `r9` and the function at 9908 B?* The answer is **yes,
the baseline already is that shape**, and every spelling that changes it either
ties at 75.4000 or loses. The new evidence is a machine-level trace of *where*
the reload is placed and *why* it cannot be moved by source.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w8212
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~1 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch in `/dev/shm/w8212` (not committed): `Radar.base.cpp` (pristine
source), `score.py` (single-symbol scorer importing `tools/verify/verify.py`,
reproduces 75.4000 exactly), `addrblocks.py` (difflib map),
`gen*.py` (variant generators), `mir.txt` (`-mllvm -print-after-all`,
1.97 M lines), `mirguard2.py` (per-pass guard extractor), `orig.dis`/`ours.dis`.

## 2. NEW: the reload is a Greedy-RA spill placement, not a source decision

`clang -mllvm -print-after-all` on the pristine baseline shows the exact point
where the status reload is duplicated. At the **Greedy Register Allocator**
dump the guard join is `bb.35` (4 predecessors: `bb.29`, `bb.30`, `bb.33`,
`bb.34`). The spilled `Globals::status` pointer is `%2609` / `%stack.31`, and
the reload is inserted in **all four predecessors**:

```
bb.29:  %3037 = MOVi 1
        %2609 = t2LDRi12 %stack.31, 0      ; status reload
        B bb.35
bb.30:  %570 = t2LDRi12 %this, 56
        CMP %570, 0
        %2609 = t2LDRi12 %stack.31, 0      ; status reload
        Bcc bb.32, eq
bb.33:  %3037 = MOVi 1
        %2609 = t2LDRi12 %stack.31, 0      ; status reload
        ...
        B bb.35
bb.34:  %3037 = t2LDRi12 %this, 20         ; otherLock = field_0x14 != 0
        CMP; MOVCCi
        %2609 = t2LDRi12 %stack.31, 0      ; status reload
bb.35:  %579 = t2LDRi12 %this, 312         ; landmarks
        CMP; Bcc bb.37, ne
bb.37:  %584 = t2LDRi12 %2609, 0           ; *status
```

So the source-level `load @Globals::status` *is* in the landmark-true block
(`bb.37`, matching the original's `ldr r6,[sp,#156]` at `0x12e31a`), but the
**spill reload of the GOT pointer** is hoisted to the guard join's
predecessors because the spilled value is live-in to `bb.35`. That is exactly
the "sunk into both predecessors" shape wave 136 asked for — and it is why the
else block is 10 B (`mov.w r9,#1` + `ldr r5,[sp,#156]` + `b.n`) instead of the
original's 2 B.

Two controls pin the mechanism:

* `bool otherLock` (which reproduces the original's guard byte-for-byte:
  `cbnz` chain, `movs r5,#1`, `ldr r6,[sp,#156]` after the join, `eor`
  `notOtherLock`) makes the same Greedy dump place the reload in `bb.37`
  instead of `bb.35`'s predecessors. The reload placement is therefore a
  function of the register pressure at the join, i.e. of the `otherLock`
  register — the coupling wave 136 described.
* `Status *status = Globals::status;` assigned *in both branches* of the
  otherLock if (`bool_both`, `int_both_after`) is **not** kept in the
  branches: clang's IR-level sinking merges it back to the join
  (`ldr r0,[sp,#156]; ldr r6,[r0]` after `cbz`), so the source cannot force the
  duplication.

## 3. Variant matrix (all single edits on the pristine baseline)

### 3.1 Status-load placement (the wave-136 lever)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | `Globals::status` in the landmark condition | **9908** | **75.4000** |
| `status_both_branches` | `Status *status` assigned in both otherLock branches, used in landmark guard | 9908 | 75.1000 |
| `status_both_branches_pre` | same, plus `Status *status = Globals::status;` before the if | 9908 | 75.1000 |
| `status_after_decl` | `Status *status = Globals::status;` after the guard, used in landmark guard | 9908 | 75.1000 |
| `int_both_after` | `int otherLock` + status assigned before the assignment in both branches | 9904 | 74.0000 |
| `int_else_only` | `int otherLock` + status local in both branches | 9908 | 75.1000 |
| `status_before_guard` | `Status *status` before the guard (wave-135 shape) | 9896 | 73.4000 |
| `sb_guard_boolnol` | `status_before_guard` + `bool notOtherLock` (restores 9908 B) | 9908 | 73.0000 |
| `si_before` | hoist `int stationIndex = status->getStation()->getIndex()` before landmark check | 9984 | 39.8000 |
| `si_before2` | `Status *status` local before landmark check | 9908 | 75.1000 |

`status_before_guard` is the only shape that reproduces the original's
`cbnz` chain (else block `mov.w r9,#1`, 4 B, falls through), but it costs
12 B and 2.0 points because the status pointer and its vtable are hoisted onto
the hot path and each of the four `getStation`/`getCurrentCampaignMission`
calls needs an extra `mov r0,r6`. `sb_guard_boolnol` restores the 9908 B size
but scores 73.0.

### 3.2 Guard spellings / block-placement probes

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | negated `&&`, `expect(...,0)` | **9908** | **75.4000** |
| `pos_expect` | positive `\|\|`, `expect(...,1)` | 9908 | 75.3000 |
| `pos_noexp` | positive `\|\|`, no expect | 9908 | 74.2000 |
| `pos_exp_each` | positive `\|\|`, expect on each operand | 9920 | 60.8000 |
| `landmark_local` | `bool landmarksOk = this->landmarks != nullptr;` | 9908 | 75.4000 (tie) |
| `nested` | landmark guard as nested `if`s | 9908 | 75.4000 (tie) |
| `lm_expect1` | `expect(landmarks != nullptr, 1)` | 9812 | 67.5000 |
| `lm_expect0` | `expect(landmarks != nullptr, 0)` | 9932 | 57.9000 |
| `lm_outer_exp1` | `expect(<whole landmark cond>, 1)` | 9872 | 67.7000 |
| `st_exp0` | `expect(<station checks>, 0)` | 9888 | 56.7000 |
| `status_first` | station checks before the landmark null check | 9916 | 71.0000 |
| `status_first_ne` | same, `!=` spelling | 9916 | 71.0000 |

### 3.3 `bool otherLock` (re-confirmed, reproduces the original guard)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | `int otherLock` | **9908** | **75.4000** |
| `bool_ol` | `bool otherLock` | 9812 | 69.1000 |
| `bool_both` | `bool otherLock` + status local in both branches | 9812 | 68.9000 |
| `bool_both_pre` | same, status local declared before | 9812 | 68.9000 |
| `bool_else_only` | `bool otherLock` + status local only in the else branch | 9812 | 68.9000 |
| `bool_nol_bool` | `bool otherLock` + `bool notOtherLock`/`notTurretMode` | 9860 | 70.0000 |
| `bool_intconv` | `bool otherLock` + `int otherLockInt = otherLock` used in two later tests | 9808 | 57.8000 |

`bool_ol` is the important control: its guard region is the original's
(`cbnz` chain, `movs r5,#1`, `ldr r6,[sp,#156]` after the join, `eor`
`notOtherLock`), but the whole function is 96 B short (9812 B) and the
call sequence has 5 extra calls (397 vs 392). The 96 B is distributed, as
wave 131 found; it is not recoverable by a single source edit here.

## 4. Root cause (confirmed with the MIR trace)

The residual is still the coupled register-allocation / stack-coloring fixed
point, now with the exact mechanism:

1. The spilled `Globals::status` GOT pointer is live-in to the guard join
   `bb.35`, so Greedy RA inserts its reload in **all four** predecessors.
   With `int otherLock` (r9) the reload takes r5, so the else block is 10 B and
   `MachineBlockPlacement` puts it first (`cbz a, check`) and if-converts the
   `b && c` pair (`itt eq`). The original's else block is 2 B
   (`movs r5,#1`) because `bool otherLock` (r5) changes the register pressure
   enough that Greedy RA reloads in `bb.37` instead.
2. `stations` in `sl`+spill `[sp,#116]` (original) vs `r6`+`sl` backup
   (baseline) — unchanged from waves 134/135.
3. `turretMode`/`planetHit` slots `[sp,#148]`/`[sp,#152]` swapped — unchanged.
4. the invented post-loop `onPlanetCourse` recompute (72 B) — unchanged.
5. the enemy `inSight` block and the ship `inSight` `s24`/`s26` live range —
   unchanged.

Every source shape tried this wave either ties at 75.4000 or loses. The
baseline remains a strong local optimum, and the only shape that reproduces
the original guard (`bool otherLock`) is a 96 B loss elsewhere.

## 5. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **84,709** differing bytes (size/base
exact), `.ARM.extab` **1,395** (size exact 156,312), `.rel.plt` **4** (size
exact 29,392), `.note.gnu.build-id` 20. `.rodata`, `.data`, `.bss`, `.got`,
`.ARM.exidx`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are byte-identical.
