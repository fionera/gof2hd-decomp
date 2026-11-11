# w11814 (wave 173): `Level::createMission` — PEI spill-slot rotation sweep;
# landed a 4-byte raw win (5,425 -> 5,421 / 940 / 17,172 B)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11814`, branch
`wave173-w11814`, base master `3fafa5b03` (wave 172).  Continues
`level-createmission-wave172-w11712.md` (scope/lifetime + convoy-switch
sweeps) and `level-createmission-wave172-w11711.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline and landed result

```text
                                     size            raw    aligned  slots SP/EA/FG
HEAD (base 3fafa5b03)                17172/17172     5425   936      44/40/48
landed (this pass)                   17172/17172     5421   940      44/40/48
```

`.text` differing bytes **81,503 -> 81,499** (-4).  Gate after the change:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact
**4524/4524**, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, symbol parity **5370/5370**, `.rel.plt` byte-identical.
`RATCHET PASS`.

Landed source shape (two independent, semantics-preserving reorders inside
the `if (pirateFortress)` turret loop):

```cpp
for (int i = this->field_168 + this->field_164 + this->friendCount;
     i < this->field_168 + this->field_164 + this->friendCount + fortressGuns; i++) {
    ...
    gun->cargo = nullptr;
    gun->shipGroup = 1;
```

Both are pure operand-order permutations (`+` is commutative on `int`, and
the two stores touch different fields), so this is a codegen-shape win only.
The aligned metric moves 936 -> 940; aligned is not a gate metric and the
function is still non-exact either way, so this is a raw-only win.

## Corrected slot map (the wave-172 note's map was wrong)

The three long-lived locals are the 4-byte spill slots `fi#92/93/94`
(`[sp,#48/44/40]`; disassembly offset = `496 - |PEI offset|`):

```text
              original   ours    pre-colour FI   colour
stationPirates  sp+48     sp+44   stack.79        fi#93
extraAliens     sp+44     sp+40   stack.81        fi#94
fortressGuns    sp+40     sp+48   stack.88        fi#92
```

`blueprint` is *not* part of the cycle: it is `fi#95` (`sp+36`) in both
(3 refs at the same addresses).  `sp84`/`sp88` are also exact in both:
`raid` = `[sp,#84]` (17 refs, identical addresses/registers) and
`[sp,#88]` (2 refs, identical).

So the delta is a 3-cycle among `{stationPirates, extraAliens, fortressGuns}`,
not the 4-way map in the wave-172 note.

## Mechanism: StackSlotColoring colour order, not declaration order

Dumped the machine pipeline for `createMission` with
`-mllvm -print-machineinstrs -mllvm -filter-print-funcs=_ZN5Level13createMissionEv`
(58 MB, scratch only):

* Before coloring there are **75** 4-byte spill slots `fi#73..fi#147`
  (the `Virtual Register Rewriter` dump).
* After `Stack Slot Coloring` there are **28** colours `fi#73..fi#100`.
* `StackSlotColoring` sorts the spill intervals by `li.weight`
  (`IntervalSorter`: `LHS->weight > RHS->weight`), then `ColorSlot` either
  reuses the lowest used non-overlapping colour or takes the next unused
  colour in ascending FI order.  PEI then lays the surviving objects out in
  FI order, so a lower colour = a higher `[sp,#N]`.

Matching post-colour weights to the pre-colour weights recovers the
colour map exactly (each colour's weight equals one pre-colour interval's
weight, i.e. no sharing on these colours):

```text
colour 92 <- stack.88 (fortressGuns)   colour 93 <- stack.79 (stationPirates)
colour 94 <- stack.81 (extraAliens)
```

A straightforward CFG-propagation of the printed successor probabilities
gives the pre-colour weights

```text
stationPirates 0.3994   extraAliens 0.3528   fortressGuns 0.2636
```

i.e. `SP > EA > FG`, which is *the original's* colour order
(SP->92, EA->93, FG->94).  Clang 7.0.2's actual colour order is
`FG->92, SP->93, EA->94`, so the weights SSC really uses differ from the
printed-CFG propagation (stale/prescaled MBFI is the likely cause).  The
rotation therefore cannot be predicted from the printed CFG; it has to be
searched empirically.

One concrete structural consequence: the original references the
`fortressGuns` slot 4 times, ours 5 times.  The extra ref is a preheader
load at `0xb33fa`; the original loads `fortressGuns` only in the loop
header (`0xb34d2`).  That is block-placement fallout, not a source ref.

## Variant sweep (all size-exact unless noted)

| variant | size | raw | aligned | slots SP/EA/FG |
|---|---:|---:|---:|---|
| HEAD | 17172 | **5425** | 936 | 44/40/48 |
| cargo/store swap only | 17172 | 5423 | 939 | 44/40/48 |
| pf bound reversed (`field_168+field_164+friendCount+fortressGuns`) | 17172 | 5424 | 937 | 44/40/48 |
| start `friendCount+field_168+field_164` + cargo | 17172 | 5422 | 939 | 44/40/48 |
| **start `field_168+field_164+friendCount` + FG-first bound + cargo (landed)** | **17172** | **5421** | **940** | 44/40/48 |
| outer switch: move `case 4` before `case 6` | 17172 | 5425 | 936 | 44/40/48 |
| inner switch source order `(0,2,1,default)` | 17172 | 5425 | 936 | 44/40/48 |
| inner switch source order `(2,0,1,default)` | 17172 | 5426 | 931 | 44/40/48 |
| inner switch source order `(1,*,*,*)` | 17172 | 5431 | 938 | 44/40/48 |
| default store order `(0,1,2)` | 17164 | 11334 | 1007 | 44/40/48 |
| default store order `(0,2,1)` | 17172 | 5431 | 936 | 44/40/48 |
| default store order `(1,0,2)` | 17188 | 11326 | 975 | 44/40/48 |
| default store order `(1,2,0)` | 17172 | 5432 | 931 | 44/40/48 |
| default store order `(2,1,0)` | 17172 | 5428 | 933 | 44/40/48 |
| `__builtin_expect(extraAliens >= 1, 1)` | 17168 | 5282 | 913 | 44/40/48 |
| `__builtin_expect(extraAliens > 0, 1)` | 17168 | 5282 | 913 | 44/40/48 |
| `__builtin_expect(extraAliens, 1)` | 17176 | 5314 | 863 | 44/40/48 |
| `__builtin_expect(raid, 1)` | 17192 | 14875 | 1729 | 40/44/48 |
| `__builtin_expect(stationPirates, 1)` | 17184 | 13490 | 1952 | 44/40/48 |
| `__builtin_expect(pirateFortress, 1)` | 17128 | 12931 | 2921 | 48/40/44 |
| `__builtin_expect(blueprint, 1)` | 17172 | 5595 | 934 | 40/44/48 |
| `__builtin_expect(convoy, 1)` | 17204 | 15877 | 1103 | 44/40/48 |
| `__builtin_expect(alienGuns != 0, 1)` | 17180 | 5816 | 866 | 44/40/48 |
| `__builtin_expect(nameLeader, 1)` | 17180 | 13100 | 1040 | 44/40/48 |
| `__builtin_expect(siegedStation, 1)` | 17368 | 16677 | 5935 | 40/48/44 |
| `bp` expect + landed start/cargo | 17172 | 5591 | 938 | 40/44/48 |
| `bp` + `pf` expects | 17136 | 12996 | 2951 | 44/40/48 |
| `bp` + `ea` expects | 17152 | 12772 | 996 | 40/44/48 |
| `bp` + `raid` expects | 17184 | 14870 | 3342 | 40/44/48 |
| `bp` + `ag` expects | 17180 | 5992 | 864 | 40/44/48 |

Notes on the table:

* Only `__builtin_expect(blueprint, 1)` rotates a slot *size-exactly*
  (SP 44->40, EA 40->44) and it costs +170 raw.  `pirateFortress` rotates
  all three but changes the size (17128); `raid` rotates all three at
  17192.  `siegedStation` rotates two at 17368.
* The `extraAliens` expect gives the best raw of any shape (5,282), but the
  function loses 4 B (17,168).  All forms of the condition
  (`>=1`, `>0`, `!=0`, `(bool)`) give the same 17,168; the truthy form
  `__builtin_expect(extraAliens, 1)` gives 17,176.  The 4-byte deficit
  comes from a register-reload difference after `ArraySetLength`
  (`0xb158c`), not from the extraAliens block itself (which is byte-identical
  at `0xb1150..0xb1180`).  Recovering it would need a real 4-byte code
  shape, not padding.
* The 2-byte phase shift documented in the wave-172 note is confirmed: the
  first insertion/deletion is the convoy inner-switch default body
  (`0xb1444..0xb1450`).  Ours factors `str [sp,#376]` into the shared tail,
  the original factors `str [sp,#380]`; the source order that would fix it
  (`1,0,2`) makes LICM hoist the constants into `s16..s22` (`vmov.f32` x4,
  +16 B).  The landed `2,0,1` default order is the only size-exact shape.

## Conclusion

The 3-cycle is a `StackSlotColoring` colour-order artifact.  The original's
colour order matches the weights one computes from the printed CFG, but
clang 7.0.2's actual SSC weights are not those weights, and every source
knob that flips the order either changes the function size or costs more
raw than it saves.  No size-exact shape rotates all three slots.  The
landed operand-order pair is the only measured size-exact raw win
(5,425 -> 5,421, `.text` 81,503 -> 81,499); everything else in this sweep
was reverted.

Scratch artefacts (not tracked): `pm_base.txt` (58 MB machine-pipeline
dump), `weights3.py`/`weights4.py`/`weights5.py` (colour/weight matching),
`swbatch.py`/`swbatch2.py`/`defbatch.py`/`expbatch.py`/`eabatch.py`/
`combo.py` (variant sweeps).
