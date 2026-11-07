# Radar::draw wave 139 (w8410) — branch-probability lever on the status-reload sink (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w8410`, based on `master`
(`bf123bd47`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline raw same-address diff **7724 B** (primary metric; official fuzzy 75.4).
Gate at start and end: `RATCHET PASS`, byte **4507**, linked **4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

Read with `radar-draw-wave138-w8310.md`, `radar-draw-wave137-w8212.md`,
`radar-draw-wave136-w8110.md`, `radar-draw-wave135-w7774.md`.

This wave attacked the wave-138 open question: *can the status GOT reload be
moved out of the guard else block (making it 6 B) while keeping `int otherLock`
in `r9` and the function at 9908 B?* The wave-138 answer was "not by a
size-neutral source edit". This wave found a **new lever the earlier waves did
not use**: the `__builtin_expect` weight on the *first* guard term. It is the
only source knob that moves the reload to `bb.37` while `r9` stays occupied —
but every spelling that does it changes the branch probabilities enough to
re-roll the route section, so the raw diff is worse.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w8410
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~0.8 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch in `/dev/shm/w8410` (not committed): `Radar.base.cpp` (pristine),
`fdiff.py` (raw same-address diff), `gparse.py` (guard disasm), `gdiff.py`
(guard-region raw diff), `b1..b16.py` (variant drivers), `drawmir_base.txt`
(`-print-after-all -filter-print-funcs=...`, 1.0 M lines), `SpillPlacement.cpp`
/ `InlineSpiller.cpp` (LLVM 7 sources fetched for the reload-placement model).

## 2. The new lever: the first-term `expect` flips the branch weights

Baseline guard weights are `bb.28 -> bb.30(37.5%) / bb.29(62.5%)` (the else
path is hot). Adding `__builtin_expect(this->lockedAsteroid == nullptr, 1)`
flips them to `bb.30(99.95%) / bb.29(0.05%)` and moves the status GOT reload
from `bb.29`/`bb.30` into `bb.37`:

```
la_exp1 (first-term expect 1):
12e30a cmp   r0, #0
12e30c bne.w 1305de        ; else (cold)
12e310 ldr   r0, [r4, #56]
12e312 ldr   r5, [sp, #156] ; status GOT -> r5, in the check-B predecessor
12e314 cmp   r0, #0
12e316 itt   eq
12e318 ldreq r0, [r4, #28]
12e31a cmpeq r0, #0
12e31c beq.n 12e338        ; all null -> then
12e31e mov.w r9, #1        ; hot else: otherLock = 1   (4 B, r9!)
12e322 b.n   12e346        ; -> join                  (2 B)  == 6 B else block
...
12e338 ldr.w r9, [r4, #20] ; then: otherLock = field_0x14
12e346 ldr.w r0, [r4, #312] ; join: landmarks
12e34a cbz   r0, 12e386
12e34c ldr   r0, [r5, #0]   ; *status
```

This is the mission's exact target shape: **6 B else block, `otherLock` in
`r9`, status load sunk into both predecessors** (the check-B block at
`0x12e312` and the cold else at `0x1305e2`). It is **9900 B** (8 B short) and
the raw diff is **8473** (baseline 7724).

## 3. Variant matrix (all single edits on the pristine baseline)

| variant | source | usize | raw diff | guard diff | preroute diff |
|---|---|---:|---:|---:|---:|
| **baseline** | outer `expect(...,0)` | **9908** | **7724** | 123/132 | 10/684 |
| `exp1` | outer `expect(...,1)` | 9872 | 8611 | — | — |
| `la_exp1` | first term `expect(...,1)`, no outer expect | 9900 | 8473 | 130/132 | 353/684 |
| `la_e_o0` | first term `expect(...,1)` + outer `expect(...,0)` | 9912 | 8683 | **117/132** | 43/684 |
| `la_e_o1` | first term `expect(...,1)` + outer `expect(...,1)` | 9872 | 8611 | — | — |
| `la1_gc0` | first term exp1 + gas-cloud exp0 | 9916 | 8793 | — | — |
| `la1_gc1` | first term exp1 + gas-cloud exp1 | 9900 | 8473 | — | — |
| `e1_sb` | outer exp1 + `Status *status` before guard | 9872 | 8627 | — | — |
| `e1_sa` | outer exp1 + `Status *status` after guard | 9872 | 8611 | — | — |
| `e1_gsfirst` | outer exp1 + `Globals::status != nullptr` first | 9868 | 8069 | — | — |
| `e1_gsnull` | outer exp1 + `Globals::status != nullptr` second | 9884 | 8369 | — | — |
| `gs_o1` | `Globals::status != nullptr` first + outer exp1 | 9868 | 8069 | — | — |
| `status_both` | `Status *status` loaded in both guard branches | 9908 | 7987 | — | — |
| `sb_o0_i0` | `Status *status` before guard, outer exp0, gas exp0 | 9896 | 8838 | — | — |
| `sb_o1_i1` | `Status *status` before guard, outer exp1, gas exp1 | 9872 | 8627 | — | — |

`la_e_o0` is the only variant that improves the *guard region* (117 vs 123 raw
bytes) while keeping `r9`; it is 4 B too big and its route section regresses
(43 vs 10 raw bytes), so the whole-function diff is worse.

## 4. Why the branch weights are the real knob

`SpillPlacement` (LLVM 7) works on edge bundles weighted by
`MachineBlockFrequencyInfo`; a block with uses gets a `PrefReg` bias scaled by
its frequency, and transparent blocks are links. The status GOT value is used
in `bb.37/39/41/43` (the four station checks). With the baseline weights the
else path is hot (62.5%), so the `bb.29`/`bb.30` bundle wins the Hopfield
network and the reload is hoisted there. With the first-term `expect(1)` the
else path is cold (0.05%), the `bb.35` link loses, and the reload stays in
`bb.37`.

The cost is that the same weights drive `MachineBlockPlacement` for the whole
function: the route section's `DrawString` call (`0x12e1da..0x12e224`) is
re-laid out (preroute raw diff 10 -> 353 bytes). The branch weights and the
route-section register assignment are one fixed point.

A control confirms the reload placement is a *global* spill/hoist fixed point:
compiling the pristine source with `-mllvm -disable-spill-hoist` changes the
guard to the inverse layout (`cmp; bne.w` chain, then hot, else cold) at
9872 B — i.e. the hoist pass itself is part of what keeps the baseline's
`bb.29/bb.30` reload.

## 5. Root cause / open questions

The mission's target (6 B else block = `mov.w r9,#1` + `b.n`, status reload at
the join, 9908 B) is **reachable as a shape** (`la_exp1`) but not at 9908 B and
not without re-rolling the route section. The remaining coupling is:

1. `SpillPlacement` reload block (`bb.29/30` vs `bb.37`) is controlled by the
   guard's branch weights, not by the source order of the status load.
2. The same weights set `MachineBlockPlacement` for the route section, so any
   weight change that moves the reload also costs ~330 raw bytes there.
3. `la_e_o0` shows the guard region itself can be improved to 117/132 with
   `r9` intact; the blocker is the 4 B size overshoot and the 33-byte route
   regression.

Candidate next probes:

* find a *second* weight knob (e.g. an `expect` on the landmark check or on
  the route `if`) that restores the baseline route layout while keeping the
  first-term `expect(1)` reload placement;
* shave 4 B from `la_e_o0` without touching the guard (it is 9912 B) — the
  guard region is already better than the baseline;
* check whether the original's route section was compiled with a different
  block-frequency hint (the original's `12e1da..12e224` matches the baseline
  byte-for-byte at 10/684, so the baseline weights are the original's).

## 6. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,476** differing bytes (size/base
exact), `.ARM.extab` **1,396** (size exact 156,312), `.rel.plt` 0
(byte-identical), `.note.gnu.build-id` 20. `.rodata`, `.data`, `.bss`, `.got`,
`.ARM.exidx`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are byte-identical.
