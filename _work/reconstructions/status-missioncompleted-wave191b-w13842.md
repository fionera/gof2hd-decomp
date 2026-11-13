# `Status::missionCompleted` — wave 191b (w13842): dead `hasEquipment` compare, case 158

Worktree `wt/w13842`, branch `wave191b-w13842`, base master `c968df2c0`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13842`.  Continues w13726 (landed
`case 158` body) and w13742 (residual sweep, no win).

**Status: landed.**  Raw **90 -> 83** (-7), aligned **32 -> 30** (-2),
size **1,432/1,432** exact, **527/527** instructions.  `.text`
differing bytes **76,006 -> 75,999** (-7); `.ARM.extab` **552**
unchanged (size/base exact); `.ARM.exidx` **0**; `.rodata` **0**
(content and sh_size exact); `.rel.plt` **byte-identical**; build-id
**20** unchanged.  `check` (ratchet) PASS: byte_exact 4508,
linked_exact 4508, size 4524/4524, data 843/843, imports 207,
stub/missing/extra 0, lint CLEAN, drift 0, parity 5370/5370.

## 1. What changed

The wave-191b `case 158` body (w13726) is kept verbatim except for the
second `hasEquipment` test, which gains a dead compare:

```cpp
                    if (Item::isInList(m->getProductionGoodIndex(), amount, cargo)) {
                        return m;
                    }
                    // Codegen device: hasEquipment() returns bool, so `< 0` is
                    // always false.  The dead comparison makes clang emit the
                    // original's unconditional branch to the shared
                    // `if (hasEquipment()) return m` test block instead of a
                    // local `cmp; b`, which is how the original reaches the
                    // same test from case 10.
                    if (Globals::status->ship->hasEquipment(m->getProductionGoodIndex(), 1) < 0) {
                        return m;
                    }
```

`hasEquipment` returns `bool`, so `< 0` is always false: same values,
same calls, same control flow.  It emits a
`-Wtautological-constant-compare` warning, which is acceptable (same
device as w13511's `case 10`).

## 2. Why this is a win

The original's tail after the `hasEquipment` call is an **unconditional**
`b.n ac458`, i.e. it jumps straight to the shared `cbnz r0, ac49e`
(`if (hasEquipment()) return m`) test block rather than doing a local
`cmp r0,#0; b`.  The bool result can never be negative, so clang folds
the dead `< 0` branch away and emits exactly that unconditional tail.
That removes the local test from the case-158 body and lets the branch
target / shared test placement match the original.  It is the same
mechanism that closed the `case 10` tail in w13511.

## 3. Residual after this change (83 raw bytes)

Function-relative byte-diff regions (`tools/fn_diff.py` raw = 83):

| region | bytes | note |
|---|---:|---|
| `0x0ba..0x0fe` | 24 | 40-entry TBH table permutation (not source-expressible; see w3633) |
| `0x32c..0x36d` | 58 | `case 158` prefix regalloc + tail reload |
| `0x58c` | 1 | `Globals::status` GOT literal (`0x163ece` vs `0x163ed0`, other-TU layout) |

The body diff is now only:

| region | orig | ours |
|---|---|---|
| prefix `ac260..ac284` | `index->r9`, `amount->r5`, `cargo->r2` | `amount->r5`, `cargo->r6`, `index->r0` (call order amount,cargo,index) |
| hasEquipment block `ac288..ac2a0` | — | **byte-identical** (2 B earlier) |
| body tail | `b.n ac458` | `ldr r6,[sp,#16]; b.n ac45a` (cargo overwrote `this` in r6) |

The `hasEquipment` block itself is byte-for-byte the original's; the
remaining body cost is the `amount,cargo,index` call order forced by
LLVM 7 `RAGreedy` (see w13726/w13742: with `index` defined first it wins
`r5`, spills `this`, and the function grows to 1,436 B).  No source
spelling of the faithful `index,amount,cargo` order is size-exact in the
full TU.

## 4. Measured variants (full `link_so`, `case 158` only)

All size 1,432 unless noted.  `lt0` = the dead `< 0` compare; `pos` =
positive `if (hasEquipment(...))`.

| variant | body shape | raw | aligned | size |
|---|---|---:|---:|---:|
| **landed `lt0_nohoist`** | `amount`,`cargo` locals; index inline; `lt0` | **83** | **30** | 1432 |
| `lt0_st_after` | `Status *st` after `isInList`; `lt0` | 83 | 30 | 1432 |
| `a_c_i_lt0` | `amount`,`cargo`,`index` locals; `lt0` | 83 | 30 | 1432 |
| `lt0_st_after_amount` | `Status *st` between amount and cargo; `lt0` | 79 | 42 | 1432 |
| `lt0_gs_mid` | `Ship *gs = Globals::status->ship`; `lt0` | 81 | 41 | 1432 |
| `c_a_i_lt0` | `cargo`,`amount`,`index` locals; `lt0` | 87 | 33 | 1432 |
| `t1_lt0` | `Status *st` first; `lt0` | **72** | 36 | 1432 |
| `t1` | `Status *st` first; `pos` | 79 | 38 | 1432 |
| `t1_gt0`/`t1_ne0`/`t1_eqtrue` | as `t1` with `> 0` / `!= 0` / `== true` | 79 | 38 | 1432 |
| `lt0_neg` | `!hasEquipment(...)`; | 682 | 147 | 1436 |
| `i_a_c_lt0` | `index`,`amount`,`cargo` locals; `lt0` | 676 | 143 | 1436 |
| `i_c_a_lt0` | `index`,`cargo`,`amount` locals; `lt0` | 378 | 41 | 1432 |
| `a_i_c_lt0` | `amount`,`index`,`cargo` locals; `lt0` | 680 | 142 | 1436 |
| `all_inline_lt0` | all args inline; `lt0` | 676 | 143 | 1436 |
| **`A_landed` (HEAD)** | `amount`,`cargo` locals; index inline; `pos` | **90** | **32** | 1432 |

`t1_lt0` is the raw optimum (72) but pays +6 aligned by hoisting
`Globals::status` to the top of the block (4 instructions moved from the
`hasEquipment` block to the prefix).  The landed `lt0_nohoist` is the
strict Pareto improvement over HEAD (raw -7 **and** aligned -2) and
keeps the faithful source shape, so it is the preferred landing; the
72/36 variant is recorded here for a future aligned-vs-raw decision.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13842
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so --json _ZN6Status16missionCompletedEbbx
# raw 83  aligned 30  size 1432/1432  insn 527/527
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```
