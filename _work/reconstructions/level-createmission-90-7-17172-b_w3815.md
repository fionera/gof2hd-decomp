# w3815: `Level::createMission` (90.7 %, 17172 B) — stack-slot-coloring residual

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w3815`, branch `wave88-w3815`,
base `41804126`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address − 0x10000).
No source change is committed: every variant below is neutral or a regression.

## Baseline (unchanged, gate green)

```text
_ZN5Level13createMissionEv  fuzzy 90.7 %  size 17172 B  @0xaffd8
normalized instruction stream: 6055 (orig) vs 6070 (ours)
  mask [sp,#N] stack offsets            -> 91.8 %
  mask stack offsets + loc_XXXX targets -> 93.9 %
gate: byte 4498 / linked 4498, .text 0x1337c8, .rodata 0x41b67,
      lint CLEAN, drift 0, RATCHET PASS
```

`cmake --build cmake-build-match --target check -j8` passes before and after
this pass. `report.json` is not committed.

## Method

`verify.py --show` side-by-side, raw `arm-linux-androideabi-objdump` for
address-anchored diffs, `llvm-dwarfdump` on our `Level.cpp.obj` for the
variable→slot map, and `-print-machineinstrs` (`/dev/shm/w3815/pm.txt`) for
the pre-RA frame objects and the post-RA spill-slot numbering. The original
`.so` has no debug sections, so the original's slots were read from the
disassembly (every `str`/`ldr [sp,#N]` that defines/uses a variable).

## Confirmed root cause: `StackSlotColoring` weight order, not source order

The dominant residual is **spill-slot colouring**, and it is a *global*
function of reference counts and block frequencies — not of declaration
order. Mechanism (LLVM 7 `lib/CodeGen/StackSlotColoring.cpp`):

1. `InitializeSlots()` sorts the spill-slot live intervals by
   `li.weight` **descending** (`IntervalSorter`).
2. `ScanForSpillSlotRefs()` accumulates
   `weight += LiveIntervals::getSpillWeight(false, true, MBFI, MI)` for every
   non-debug reference to the slot — i.e. roughly *reference count × block
   frequency*.
3. `ColorSlot()` walks that sorted list and gives each interval the first
   still-unused frame index, so the **highest-weight slot gets the lowest
   frame index**.
4. PEI lays the surviving slots out so that offset decreases with frame
   index: `fi#74 -> sp+124`, `fi#75 -> sp+120`, …, `fi#101 -> sp+16`
   (`offset = 124 - 4*(fi-74)`).

So the highest-weight slot lands at the **highest** `sp` offset. This is why
moving a declaration or changing a type does nothing: the colour depends on
how many times the value is loaded/stored and in which blocks, which is a
downstream effect of the codegen shape.

### The `&Globals::rnd` / `raid` swap (≈16 sites)

| slot | original | ours |
|---|---|---|
| `&Globals::rnd` (entry GOT address) | `sp+88` (fi#83) | `sp+84` (fi#84) |
| `raid` (bool) | `sp+84` (fi#84) | `sp+88` (fi#83) |

Evidence: original `b0028`-region `str.w fp,[sp,#88]` (fp = `&Globals::rnd`)
and `b0eac`-region `str r0,[sp,#84]` (raid); ours is the mirror image.
`llvm-dwarfdump --debug-loc` on our object confirms `raid -> [sp,#88]`, and
the first `str.w fp` in our object is `[sp,#84]`.

Because the two slots are adjacent, the swap shows up as ~16 repeated
`[sp,#88]` vs `[sp,#84]` diffs plus the related `[sp,#40/44/48]` renames.
It is worth ≈1.1 pp of fuzzy (the 90.7 → 91.8 step when offsets are masked).

### The `bigAlienWave` / `alienWave` / `pirateFortress` rotation

Original (read off `b0ee2`/`b0ee6`/`b0ef2`):

```asm
str r0,[sp,#64]   ; bigAlienWave = campaign > 103
str r0,[sp,#60]   ; alienWave = 1
str r1,[sp,#56]   ; pirateFortress = 0
```

Ours:

```asm
str r0,[sp,#56]   ; bigAlienWave
str r0,[sp,#64]   ; alienWave
str r1,[sp,#60]   ; pirateFortress
```

i.e. original `{pirateFortress=56, alienWave=60, bigAlienWave=64}` vs ours
`{bigAlienWave=56, pirateFortress=60, alienWave=64}` — the same weight-order
effect. All six declaration permutations and several assignment permutations
were tried (see table); the baseline order is the best.

## Secondary real diffs (instruction shape, not just slots)

1. **`0xb0eac`** — `if (*bigShipsCount > 0 && localRace == 0)`
   * orig: `cmp r0,#0; ble.n`
   * ours: `cmp r0,#1; blt.n`
   The IR is already `icmp sgt i32 %x, 0` (verified in `level.ll` line 6856),
   yet the ARM backend selects the `sge 1` form. Rewriting the source as
   `>= 1`, `> 0`, `!= 0`, `!(x <= 0)` or swapping the `&&` operands does not
   produce `cmp #0; ble` (swapping operands regresses to 74.0 %).

2. **`0xb2e04`** — `if (i == friendCount+field_164 && alienWave)`
   * orig: `subs.w r0,fp,r3 / mov r1,r0 / it ne / movne r1,#1 /
     ldr r2,[sp,#60] / eor r2,#1 / orrs r1,r2 / bne`
   * ours: `ldr r0,[sp,#64] / cmp #0 / beq / cmp fp,r3 / bne / ldr r0,[sp,#56] / cmp #0 / beq`
   The original *materialises* `i == X` as a 0/1 value and ORs the negations;
   ours short-circuits with `alienWave` first. `bool first`, `int first`,
   `int diff`, `(bool)` casts, a ternary, `!(a||b)` and a nested-if rewrite
   all leave the shape unchanged (nested-if regresses to 84.6 % / 17316 B).

3. **`0xb415c` tail (`case4Tail`)** —
   orig hoists `hostile=1` into `fp` and emits `strd r0,fp,[sp]`; ours keeps
   `&Globals::rnd` in `fp` and materialises `1`/`0` with `movs`+`str` per
   iteration (3 extra instructions). The original reloads `&Globals::rnd`
   from `[sp,#120]` inside the loop; ours uses `fp`.

4. **`0xb32ba` (alienGuns loop)** — ours emits a default-constructed
   `Vector turretPos` zero-init (`strd r0,r0,[sp,#212]; str r0,[sp,#220]`,
   3 extra instructions) and rotates `{t[0],t[1],t[2]}` before
   `{t[3],t[4],t[5]}`; the original has no zero-init and rotates the
   direction first. Removing the default ctor regresses to 88.4 % / 17144 B.

5. **`0xb41f0`–`0xb42d0`** — EH landing pads: the original has one extra
   cleanup variant (`mov r9,r0; mov r0,r6; b.n`) and more `b.n loc_42ee`
   filler; ours has a slightly different branch tree and a `nop` before the
   literal pool. The `.ARM.extab` record is the same shape (148 call sites),
   so this is block order, not a different `try`/`catch` structure.

6. Literal-pool ordering differs for the case-183 diamond offsets
   (`25000.0f`/`-36000.0f`/`-45000.0f` at `0x15e8` vs `0x15fc`) and the
   final `pic:got:_ZN7Globals3rndE` word at `0x4310`.

## Experiments run this pass (all from the same baseline)

| variant | fuzzy | size |
|---|---:|---:|
| **baseline** | **90.7** | **17172** |
| `bool alienWave` / `bool bigAlienWave` | 89.2 | 17156 |
| `bool pirateFortress` | 89.9 | 17176 |
| `bool alien+big` | 89.2 | 17156 |
| `bool alien+pirate` | 67.1 | 17180 |
| `bool big+pirate` | 89.9 | 17176 |
| `bool all three` | 79.8 | 17184 |
| `bool firstStation = i == X` (both conditions) | 90.7 | 17172 |
| `int first = i == X` / `int first != 0` | 90.7 | 17172 |
| `int diff = i - X` | 90.7 | 17172 |
| `i - X == 0` | 90.7 | 17172 |
| `!(i != X || !alienWave)` | 90.7 | 17172 |
| `(i == X) & (alienWave != 0)` | 87.5 | 17188 |
| ternary `i == X ? alienWave : false` | 90.7 | 17172 |
| `(bool)(i == X) && (bool)alienWave` | 90.7 | 17172 |
| nested `if (i == X) { if (alienWave) … }` rewrite | 84.6 | 17316 |
| `localRace == 0 && *bigShipsCount > 0` | 74.0 | 17160 |
| `*bigShipsCount >= 1` / `> 0` / `!= 0` | 90.7 / 90.7 / 89.2 | 17172 / 17172 / 17164 |
| `unsigned stationIndex/raidRoll/pirates/extraFriend/localRace/raidRace` | 90.7 | 17172 |
| `unsigned security` | 90.6 | 17172 |
| `int convoy` / `int siegedStation` / `int hasWanted` / `int nameLeader` | 87.0 / 70.5 / 88.6 / 88.5 | — |
| `int resetStation` / `int introSystem` | 90.7 / 80.2 | 17172 / 17160 |
| direct `this->field_168/164` instead of `*bigShipsCount/*jumperCount` | 85.0 | 17156 |
| explicit `AERandom *rnd` for the whole `isEmpty` block | 56.9 | 17188 |
| `AERandom *rnd` before/inside the `case4Tail` loop | 90.6 / 90.7 | 17168 / 17172 |
| `Globals *g` removed / `Waypoint *wp` first / `int model` local | 90.7 / 90.5 / 90.7 | 17168 / 17168 / 17172 |
| `bool hostile = true` hoisted in the tail loop | 90.7 | 17172 |
| `Vector turretPos` without default ctor (alienGuns) | 88.4 | 17144 |
| `turretPos` assigned before `turretDir` | 90.6 | 17172 |
| explicit `turretDir`/`turretPos` in the fortressGuns loop | 90.5 | 17172 |
| `if (alienGuns)` / `if (pirateFortress != 0)` / ternary forms | 90.7 | 17172 |

## Conclusion / next step

The remaining gap is not a missing mission-case body or a wrong constant:
`.text` and `.rodata` are size-exact, the jump table has the same 15 entries,
and the EH record has the same 148 call sites. It is **register allocation /
spill-slot colouring / block order**, i.e. a global property of the whole
`createMission` IR.

The highest-value next experiment is to change the *reference counts* of
`&Globals::rnd` and `raid` so the colour order flips (e.g. make the
`case4Tail` loop reload `&Globals::rnd` from a stack slot instead of keeping
it in `fp`, which is exactly what the original does at `0xb415c`). Any such
change must keep `size == 17172` and `.text == 0x1337c8`, otherwise the
size-exact gate count drops.
