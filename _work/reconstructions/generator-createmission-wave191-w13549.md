# Generator::createMission — wave 191 / w13549: 3-byte residual, no win landed

Status: **no code change landed** (HEAD shape retained).  Worktree
`wt/w13549`, branch `wave191b-w13549`, base master `6e55ef9d6`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` (1996 B,
744 insn) is still **raw 3 / aligned 3 / size 1996/1996**:

| offset | original | ours |
|---|---|---|
| `0x9735a` (`bhi.n`) | target `0x97388` (race<4 && type==13 test) | target `0x973fa` (folded away) |
| `0x9770e` | `str r1,[sp,#32]` (rem) | `str r1,[sp,#44]` |
| `0x97718` | `ldr r0,[sp,#32]` | `ldr r0,[sp,#44]` |

Both are independent; fixing either alone is a raw-1/raw-2 win.  Nothing
below beat the HEAD shape at 1996 B.

## 1. The `rem` slot is a *spill* in the original, not an alloca

New evidence this wave (machine-IR dumps via
`clang++ -mllvm -print-machineinstrs -mllvm -filter-print-funcs=<mangled>`,
pass `# After Merge disjoint stack slots` / `# After Greedy Register Allocator`):

* Before StackColoring the frame has `fi#0:4, fi#1:12, fi#2:4, fi#3:12,
  fi#4:12, fi#5:12`.  Only **fi#1 (agentName String), fi#2 (volatile
  `rem`), fi#5 (getName String)** carry `LIFETIME_START/END`; fi#3 and
  fi#4 have **no** markers, so `StackColoring` skips them (their
  `LiveInterval` is empty → `SortedSlots = -1`).
* Merge order is by size: fi#1 (spans 973fa..978aa) overlaps both fi#5
  and fi#2, so fi#5 and fi#2 are then compared and are disjoint →
  **fi#2 is always merged into fi#5** (sp+44).  A `volatile int rem`
  therefore can never land at sp+32; every volatile spelling tried
  (`rem`, `remCopy`, `rem[1]`, struct, `const volatile`, assign-later,
  `alignas(4)`, …) reproduces raw 3.
* In the original, sp+32 is a genuine **spill slot** shared by `status`
  (97228), `rem` (9770e/97718) and `mission` (9775e/9776c) — exactly the
  machine-level `StackSlotColoring` pattern, which only colours spill
  slots (allocas are already fixed by `StackColoring`).  So the original
  source must use a **plain `int rem`** that greedy RA decides to spill.

### Why our plain-`int rem` does not spill

With plain `int rem` the whole function is byte-identical to the original
**except for an +8 frame shift**: greedy keeps `rem` in `r9` and spills
`agent` instead (frame 104, `rem` in `r9`, `ldr r0,[sp,#36]` at 9776a).
`RAGreedy::shouldEvict` evicts `agent` because `rem.weight >
agent.weight` (rem is a 2-use, ~4-slot local interval; agent spans
97306..9776a).  raw 97 / size 1996.

### New lead: the RA *can* be flipped, but only by changing block weight

`if (__builtin_expect((type | 4) == 12, 1))` (i.e. make the bonus `else`
cold) flips greedy into spilling `rem` to **sp+32 with frame 96** —
the target state — but it reallocates registers globally (`mov r6,r1`
instead of `mov r9,r1`, r5/r6 swap) and the function shrinks to
**1992 B**, raw 1826.  Weight 0 gives 2004 B / raw 267.  So the target
RA state is reachable; the missing piece is a source form that changes
the bonus block's branch weight *without* perturbing the global
allocation or the size.

Measured this wave (all with HEAD bonus block unless noted):

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (`volatile int rem`) | **3** | 1996 | rem merged into String sp+44 |
| plain `int rem` | 97 | 1996 | frame 104, rem in r9, agent spilled |
| plain + `if (agent==nullptr){bonus=0;}` | 571 | 2004 | **rem at sp+32, frame 96** |
| plain + `expect((type|4)==12,1)` | 1826 | **1992** | **rem at sp+32, frame 96** |
| plain + `expect((type|4)==12,0)` | 267 | 2004 | frame 104 |
| HEAD + `expect((type|4)==12,1)` | 1826 | 1992 | rem still sp+44 |
| HEAD + `expect((type|4)==12,0)` | 1124 | 1996 | rem still sp+44 |
| `alignas(8) volatile int rem` | 85 | 1996 | frame 104, rem sp+48 |
| `volatile int bonus` (top) + plain rem | 78 | 1996 | frame 104 |
| `volatile int rawBonus` | build fail (`volatile` decl) | — | — |

## 2. The `bhi` fold

Unchanged from wave 190: the first `JumpThreading` pass retargets the
`type<15` false edge (block 119 → 134) straight to the merge block 178
because LVI proves `type u>= 15 ⇒ type != 13`.

New measurement: `if (race < 4 && (type & 0xffffU) == 13)` **fixes the
branch** (target 0x97388) at **size 1996**, but inserts a 2-byte `uxth`
at 0x97390 which shifts the rest of the function (raw 366, 15 diff runs).
The handoff's 1964-B figure was for a different spelling.  Removing the
`__builtin_expect` from the whole `&&` collapses the layout to 1984 B
(the `getRoutes()==nullptr` cold block moves inline), so the
`__builtin_expect(race < 4 && type == 13, 1)` wrapper is load-bearing for
the 1996-B layout *and* is what lets JT see the `and`.

Tried and identical to HEAD (raw 3 / 1996): expect weight 2, negated
expect, `(race<4)&(type==13)`, `13==type`, `type==13u`, `(uint32_t)race<4`,
`int type` instead of `uint32_t type`, `unsigned type`,
`expect(mask,1)`.  `expect(guard,1)` → 1960 B; `expect(guard,0)` → 2028 B;
`expect(mask,0)` → 1984 B; `race<4 && type==13` without expect → 1984 B.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
# raw=3 aligned=3 size=1996/1996 insn=744/744
```

Scratch harness (not in the repo): `/home/debian/GalaxyOnFire2/scratch/w13549/`
— `Generator.base.cpp` (pristine HEAD), `score.sh`, `batch.py` (build+score),
`probe.py`/`probe2.py`/`probe3.py`, `diffs.py` (byte-run diff with
orig/ours instruction), `mir.sh` (machine-IR dump), `b1..b18.py` sweeps.

## 4. Suggested next angles

* Find a source form that makes the `(type | 4) == 12` `else` cold
  (so greedy spills `rem` to sp+32) **without** the global
  register reallocation that `__builtin_expect` triggers.  Candidates:
  reordering the bonus block relative to the `costs` block, or a
  different but equivalent `(type|4)==12` spelling whose fall-through
  weight differs.
* Defeat LVI on the `type<15` false edge without adding an instruction:
  every mask/trunc form that works costs a `uxth`/`and` (size 1964 or a
  2-byte shift cascade).
