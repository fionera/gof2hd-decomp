# `Status::missionCompleted` — wave 190 (w13423): branch-target / cold-block lens

Worktree `wt/w13423`, branch `wave190-w13423`, base master `3cb8d404a`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13423`.

**Status: research note, no `src/` change landed.**  Baseline re-confirmed
on the committed source: raw **451**, aligned **205**, size
**1,432/1,432**.  `check` RATCHET PASS (byte 4507, linked 4507, data
843/843, imports 207).  Tree clean.

## 0. New evidence vs w13413

Two things this pass established that the wave-190 pool-alignment note
(w13413) did not:

1. The `0ac1d0..0ac1d6` 6-byte region is **not** a scheduler or register
   allocation difference — it is a **BranchFolder tail-merge** difference.
   The `this` reload `ldr r6,[sp,#16]` is pulled out of the case-6 tail
   into a block shared with case 10, and the post-RA scheduler then emits
   `mov r8,r4; lsls; ldr r6` instead of the original's
   `ldr r6; mov r8,r4; lsls`.  Proven with `-mllvm -print-after-all` and
   with `-mllvm -tail-merge-threshold` (below).
2. The first constant island is 8 B (rank + `Globals::status`) **because
   both literals are 16-bit `tLDRpci` loads whose ±1020 B range cannot
   reach the end pool**, not because of a generic "first CP user" rule.
   The original has no rank literal, so its `Globals::status` literal is
   the only one and lands at `0xac3f8` (912 B from `0xac068`).

## 1. Per-type diff map (current baseline)

Using the two TBH tables (rank table in `.rodata` at `0x20b977`, TBH at
`0xabfe8`; second TBH at `0xac052`):

| type | orig | ours | shift | diff | osz | usz |
|---|---|---|---:|---:|---:|---:|
| 158 (case 6) | `ac180` | `ac180` | 0 | **6** | 92 | 92 |
| 166 (case 10) | `ac242` | `ac242` | 0 | **41** | 100 | 94 |
| 184 (case 12) | `ac2b4` | `ac2ae` | -6 | **39** | 58 | 60 |
| 189 (case 13) | `ac2ee` | `ac2ea` | -4 | 39 | 60 | 68 |
| 156,157,160,163,164,165,168 | — | — | +4 | 5,5,6,2,1,2,10 | — | — |
| 167/174 | `ac036` | `ac036` | 0 | 1 | 42 | 42 |
| 170 | `ac2a6` | `ac2a0` | -6 | 3 | 14 | 14 |
| 150..155, 162, 169, 171, 172 | — | — | 0 | 0 | — | — |

The 70-byte `+4` block shift is the case-10 (-6) + case-12 (+2) +
case-13-island (+8) sum, exactly as w13413 documented.

## 2. The `0ac1d0` 6-byte order diff is a tail merge

Pre-BranchFolder MIR (`-mllvm -print-after-all`, "After
Prologue/Epilogue Insertion") shows case 6 already as one block:

```
bb.62 (%ir-block.257):
  $r0 = t2ANDrr $r9, $r6
  $r0 = t2LSLri $r0, 31, def $cpsr
  $r8 = COPY $r4
  $r6 = t2LDRi12 $sp, 16
  t2Bcc %bb.110, 1, $cpsr
  t2B %bb.109
```

After BranchFolder the tail is split into a shared block:

```
bb.56:  and; lsls; mov r8,r4
bb.57:  ldr r6,[sp,#16]; bcc break; b return   ; predecessors bb.56, bb.66
```

`bb.66` is case 10's `cmp r0,#0`.  The post-RA scheduler then orders the
merged block as `mov r8,r4; lsls; ldr r6`, which is the committed
`0ac1d0` bytes `a046c007049e` instead of the original `049ea046c007`.

Compile-only proof (`-mllvm -tail-merge-threshold=N`, object symbol size
from `nm -S`):

| N | `missionCompleted` object size | second insn after `and` |
|---|---:|---|
| 1, 2, 3 | `0x664`, `0x664`, `0x618` | `ldr r6` (matches original) |
| 4 .. 150 (default) | `0x5f4` .. `0x58c` | `mov r8,r4` |

So the merge is skipped for N<=3 and the order matches, but the function
grows to 1,560 B (linked) because many *other* tail merges in the same
function are also skipped.  A `-mllvm -tail-merge-threshold/-size` sweep
(88 cells, `N=4..150`, `size=1..10`) found no cell with the `ldr` order
and the default 1,420 B object size; the smallest `ldr`-order cell was
`threshold=30,size=4` at `0x5b0` (linked 1,456 B).  Not size-exact.

Source shapes that do **not** break the merge (all byte-identical to the
committed 451): `hasGood && hasSpecial` as a `bool both` local, `int`
sum `== 2`, `switch`, `== true`, `&`, nested `if`, `goto`, `else break`,
`if (!...) break; return m;`, `return m != 0 ? m : 0`, `m != 0` guards,
`__builtin_expect(...,0/1)`, and the same set applied to case 10's
`hasEquipment` tail (`bool` local, `!` + break, `else`, `while`,
`do/while(0)`, combined `||` with `isInList`).  The IR canonicaliser
produces the same CFG in every case.

## 3. First-island 8 B: the 16-bit `tLDRpci` range

The rank load at `0xabfda` is `49c9` = 16-bit `ldr r1,[pc,#804]`
(`tLDRpci`, imm8=201, scale 4 → max 1020 B).  The end pool is at
`0xac4c4`, i.e. `0xac4c4 - 0xabfdc = 1256 B` away → **out of range**, so
the rank literal is forced into an island.  The case-208
`Globals::status` load at `0xac068` (`48a6`, 16-bit, max 1020) is
`1112 B` from the end pool → also out of range.  Both are placed at the
first water where both are in range, `0xac300` (806 B / 664 B), making
the 8 B island.

The original has no rank literal, so its only early literal is
`Globals::status` at `0xac068`, which lands at `0xac3f8` (912 B) — the
4 B pool w13413 identified.  This is why the pool cannot simply be
"moved later": the rank literal's 1020 B reach caps any island at
`0xac3d2`, and the `Globals` literal shares it.

Moving the table into `Status.cpp` as a `static const` array does not
help (compile-only test: still `ldr r1,.LCPI; add r1,pc; ldrb`, because
`.rodata` is ~1.4 MB from `.text`); putting it in `.text` also keeps the
literal-pool form (the distance is not known at compile time).

## 4. Case-12 and case-10

* Case 12 (`CBA` = station, orbit, campaign) emits 3 `ldr r0,[r4]`
  `Globals::status` reloads and is 60 B; the original's `C,I,S` order
  emits 2 and is 58 B.  `ABC`/`ACB`/`BAC` measure 466 raw, `BCA` 455,
  `CAB` 486 (size 1,428) — all worse than 451, because the -2 body shifts
  the following bodies and the island gains a 2 B alignment `nop`.
* Case 10's body is 94 B vs the original's 100 B.  The original re-queries
  `getProductionGoodIndex()` for `hasEquipment` (6 B) and uses a shared
  `cbnz` tail at `0xac458` (2 B shorter than our `cmp; b.n`); our re-query
  variant is 1,440 B because the tail is shared with case 6 instead of
  case 38.  All 6 permutations of `index`/`cargo`/`amount` were already
  swept by w13396.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13423
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=451 aligned=205 size=1432/1432
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13423/` (`regions.py`,
`bodies2.py`, `trial.py`, `batch*.py`, `combo.py`, `all.log`).
