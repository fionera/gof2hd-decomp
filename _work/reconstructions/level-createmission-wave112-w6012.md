# w6012: `Level::createMission` (92.094405 %, 17172 B) — wave-112:
# `orrs` merged-predicate mechanism / frame-layout map; no landable gain

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6012`, branch `wave112-w6012`,
base `5d98bd391`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; addresses below are linked `.so` addresses, function
`0xaffd8`–`0xb431c`).
**No `src/` change is committed**: every variant measured below is neutral or a
regression. The deliverable is this note. The worktree is clean.

## Baseline (unchanged, gate green before and after)

```text
_ZN5Level13createMissionEv   fuzzy 92.094405 %   size 17172 B (size-exact)
                             orig 6055 insns / ours 6063 insns
                             317 raw diff regions
                             shape-normalised score 94.949662 % (255 regions)
gate: byte 4500/4524, linked 4500/4524, size 4524/4524, stubs 0, missing 0,
      wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
      RATCHET PASS
ELF shape unchanged by this pass: 27 sections exact, file size 2,187,328 B,
.ARM.extab 0x26298 (now exact), .rodata 0x41b67/0x41c27 (ours +0xC0, the
Status rank table tail).
```

`shape` above is the same verifier disassembly with every register name
(`r0`–`r12`, `sp`, `lr`, `pc`, `s0`–`s31`, `d0`–`d31`) collapsed to `R` before
`difflib.SequenceMatcher(autojunk=False)`.  The 2.86 pp gap between 92.09 and
94.95 is therefore **pure register-name assignment**; the remaining 5.05 pp is
instruction shape / literal-pool placement.

## 1. The `orrs` merged predicates: exact LLVM-7.0.2 rule (new)

The original contains three *real* `orrs` merged predicates:

```text
b2ab0  wanted block      (hasWanted && i == 0)          -- ours already matches
b2e14  alienWave block   (i == first && alienWave)      -- ours is 2 branches
b2f36  pirateFortress    (i == first && pirateFortress) -- ours is 2 branches
```

(`orrs` at `b0a7a` and `b35be` are literal-pool words, not code.)  Ours has
exactly one (`0x2af8`, the wanted block).

The original shape at `b2e04`:

```text
subs.w r0, fp, r3        ; d = i - first
mov    r1, r0
it     ne
movne  r1, #1            ; r1 = (d != 0)
ldr    r2, [sp, #60]     ; alienWave (0/1)
eor.w  r2, r2, #1        ; !alienWave
orrs   r1, r2
bne.w  ...
```

Ours is the short-circuit form (`ldr/cmp/beq/cmp/bne`, 5 insns).

### The rule

`SelectionDAGBuilder::visitBr` only tries the merged path when the branch
condition is a `BinaryOperator` (`and`/`or`) with one use.  It then calls
`FindMergedConditions`, collects two `CaseBlock`s, and calls
`ShouldEmitAsBranches` (`/tmp/SelectionDAGBuilder.cpp` lines 1900–1963):

```cpp
if (Cases[0].CC == ISD::SETEQ && Cases[0].TrueBB == Cases[1].ThisBB)
  return false;                       // -> materialise the and/or -> orrs/ands
if (Cases[0].CC == ISD::SETNE && Cases[0].FalseBB == Cases[1].ThisBB)
  return false;
```

`EmitBranchForMergedCondition` gives a **non-`CmpInst` leaf** the synthetic
`CaseBlock(SETEQ, Cond, true, ...)`; a real `icmp` keeps its own predicate.
For an `and` tree, leaf 0 has `TrueBB = TmpBB` and leaf 1 has
`ThisBB = TmpBB`, so:

* **leaf 0 is `icmp eq …`** (or any non-`CmpInst` i1)  -> `SETEQ` and
  `TrueBB == ThisBB` -> `ShouldEmitAsBranches` returns **false** -> the whole
  `and` is materialised and (because the true block is the next block) inverted
  by `visitSwitchCase`, giving `not(and a,b)` = `or(!a,!b)` = **`orrs`**.
* **leaf 0 is `icmp ne …`** -> `SETNE` and `FalseBB != ThisBB` -> returns
  **true** -> two short-circuit branches (what we emit).

This is why the **wanted** block already matches: its IR is
`%1166 = and i1 %449, %1165` where `%449` is a `phi i1` (the `hasWanted` bool)
and `%1165 = icmp eq i32 %i, 0`.  Leaf 0 is the phi -> synthetic `SETEQ` ->
merged form.

The station-loop IR is different:

```text
%1123 = icmp ne i32 %1115, 0          ; alienWave != 0   (leaf 0, SETNE)
%1410 = icmp eq i32 %1402, %1403      ; i == first       (leaf 1, SETEQ)
%1411 = and i1 %1123, %1410
br i1 %1411, label %1412, label %1528
```

Leaf 0 is `icmp ne` -> two branches.  **The operand order is the whole
problem.**  `InstCombine` canonicalises the commutative `and` by ascending
`getComplexity`: `icmp ne (phi, 0)` has complexity 2, `icmp eq (phi, add)` has
complexity 5, so the `SETNE` operand is always first.  Both source orders
(`i == first && alienWave` and `alienWave && i == first`) and every spelling
tried below canonicalise identically.

### Ways to get leaf 0 to be `SETEQ`

1. Make `alienWave` a real `bool` (phi i1) -> leaf 0 becomes the phi ->
   merged form.  Works, but costs 3.8 pp elsewhere (see §3).
2. Make `i == first` the lower-complexity operand (not reachable from source:
   `first` is `friendCount + field_164`, an `add`).
3. Make leaf 0 a non-`CmpInst` i1 that is *not* an `icmp` (a phi, `and`, `or`,
   `select`, `trunc`).  `bool alienHere = false; … alienHere = true;` built in
   parallel with the int `alienWave` does produce the `orrs`, but GVN/regalloc
   then costs 8 pp (§3).

## 2. Per-basic-block diff map (baseline)

First divergence map (instruction index in the verifier stream):

| index | what | note |
|---:|---|---|
| 28 | `str.w fp,[sp,#88]` vs `[sp,#84]` | `Globals::status` spill slot swap |
| 257–260 | `.short` jump-table offsets | mission `switch` case bodies shifted |
| 451–991 | `str/ldr [sp,#84]` vs `[sp,#88]` | 18 regions, `raid`/`status` slot swap |
| 635/777 | `[sp,#48]` vs `[sp,#44]`, `[sp,#44]` vs `[sp,#40]` | `pirateEscorts` slot shifted 4 B |
| 1368 | `cmp r0,#0; ble` vs `cmp r0,#1; blt` | `*bigShipsCount > 0` negation form |
| 1864–1895 | convoy `if (k==2/1/0)` chain | original tests 2,1,0; ours 0,1,2 |
| 4182–4190 | `alienWave` merged predicate | §1 |
| 4275–4284 | `pirateFortress` merged predicate | §1 |
| 4590–4609 | alienGuns table select + `setScaling` | CMOV vs in-loop `cbz`; tuned order |
| 4649–4653 | `setScaling` in the original's later slot | known tuned deviation |
| 4781–4787 | pirateFortress loop bound | pure regalloc |
| 5185–5203 | raid-loop bound (`guns` in `r9` vs `sp+112`) | pure regalloc |
| 5456–5461 | alienGuns `base+v` float adds | register pressure |
| 5889–5896 | `enemies->size()` LICM hoist | original reloads each iteration |

The first 450 instructions are byte-identical apart from the `raid`/`status`
spill swap; the first 1300 apart from that swap, the `pirateEscorts` 4-byte
slot shift and branch/jump-table displacements.  The call sequence is identical
(679 calls each, `PlayerTurret::setScaling` only moved).

Frame-slot usage counts (orig / ours):

```text
sp+40  4/6   sp+44  6/5   sp+48  6/8   sp+84 17/2   sp+88  2/17
sp+92 13/7   sp+96 15/15  sp+100 28/39 sp+104 18/20 sp+108 36/35
sp+112 30/30 sp+116 37/44 sp+120 28/31 sp+124 57/56
```

So the frame layout is not just the `raid`/`status` pair: at least
`pirateEscorts` (orig `sp+48`, ours `sp+44`) and several reused slots in
`sp+92`–`sp+120` are assigned differently.  This is `RegAllocGreedy` /
`VirtRegMap::assignVirt2StackSlot` creation order, not source order.

## 3. Variants measured this pass

Every row is a fresh `cmake --build cmake-build-match --target link_so -j6`
followed by the official verifier normalisation (unrounded).  All are applied
on top of `/dev/shm/w6012/Level.base.cpp` (byte copy of HEAD `Level.cpp`).

### 3.1 `bool` for the wave flags (the faithful Ghidra shape)

Ghidra declares `bool bVar3` (pirateFortress), `bool bVar4` (bigAlienWave),
`bool bVar7` (alienWave).

| variant | pct | size |
|---|---:|---:|
| **baseline (`int` for all three)** | **92.094405** | 17172 |
| `bool alienWave` | 88.259577 | 17156 |
| `bool alienWave` + parallel `bool alienHere` phi | 83.949802 | 17168 |
| `int raid` (control for the slot swap) | 91.929361 | 17172 |
| `unsigned char raid` / `char raid` | 91.929361 | 17172 |

`bool alienWave` does produce the `orrs` (U4184), but adds a spill of `i`
(`str.w fp,[sp,#116]` / `ldr r1,[sp,#116]` around `createStaticObject`) and
re-lays the alienGuns loop, for a net −3.8 pp.  The parallel-phi variant also
produces the `orrs` but is worse still.

### 3.2 Condition-spelling sweep for `(i == first && alienWave)`

All of these are **neutral** (92.094405 / 17172, `orrs` count still 1):

```text
alienWave && i == first            (operand order swapped)
(i == first) && (alienWave != 0)
(i == first) && !!alienWave
(i == first) && (bool) alienWave
(i == first) && (alienWave + 0)
(i == first) && (alienWave - 1 + 1)
(i == first) && (alienWave ? 1 : 0)
```

Worse:

| variant | pct |
|---|---:|
| `(alienWave & 1)` | 91.846839 |
| `alienWave == 1` | 91.846839 |
| `(unsigned char) alienWave` | 91.846839 |
| `(short) alienWave` | 91.846839 |

None of the casts flips the `InstCombine` operand order (checked: `orrs` count
stays 1).

### 3.3 `raid` declaration / initialisation (slot-swap attempt)

All neutral on the slots (`[sp,#84]` 2 / `[sp,#88]` 17) and on the score:

```text
bool raid = false;  (same place)
bool raid = (intro && !hc) ? false : roll < ...;
if (!(intro && !hc)) raid = ...; else raid = false;
```

`int raid = 0;` is 91.929361.  The `raid`/`status` slot swap is not reachable
from the `raid` declaration.

### 3.4 Convoy `if (k == 2 / 1 / 0)` chain (`0xb1408`)

The original tests `cmp #2; beq; cmp #1; beq; cbnz` with bodies laid out
`case0, case1, case2, default`; ours tests `cmp #0; beq; cmp #1; beq; cmp #2;
bne` with bodies `case2, case0, case1, default`.

| variant | pct |
|---|---:|
| **baseline if-chain 2,1,0** | **92.094405** |
| nested `if (k==2) else { if (k==1) else { if (k==0) else … } }` | 92.094405 |
| `switch (k)` cases 2,1,0, default last | 92.094405 |
| if-chain ascending 0,1,2 | 91.945866 |
| `switch (k)` cases 0,1,2, default last | 91.945866 |

The 2,1,0 forms are already identical codegen; the ascending forms are worse.
The original's descending test order is not reproducible from clang 7.0.2
source for this chain.

### 3.5 `*bigShipsCount > 0` negation form (`0xb07f8`)

Original `cmp r0,#0; ble` (i.e. `x <= 0`), ours `cmp r0,#1; blt` (`x < 1`).
Both are the negation of `x > 0`; clang canonicalises `> 0`, `>= 1` and `0 < x`
identically in our build.  All neutral:

```text
*bigShipsCount >= 1
0 < *bigShipsCount
*bigShipsCount > 0   (baseline)
```

Reordering the `&&` to `localRace == 0 && …` is a large regression
(74.037985 %, 17164 B) because it makes leaf 0 `SETEQ` and merges the two
branches into a single `ands`; `localRace == 0 && 0 < *bigShipsCount` is the
same 74.037985.

### 3.6 Other neutral / worse variants

| variant | pct | size |
|---|---:|---:|
| `const int guns` | 92.094405 | 17172 |
| `if (alienGuns)` instead of `!= 0` | 92.094405 | 17172 |
| `const int first`, `const int escortsEnd` | 92.094405 | 17172 |
| `int blueprint` / `int stationPirates` | 92.094405 | 17172 |
| `unsigned stationRace` | 92.094405 | 17172 |
| `int alienRace` | 92.094405 | 17172 |
| `if (raid != false)` | 92.094405 | 17172 |
| `int raidType = 0;` before `if (raid)` | 92.094405 | 17172 |
| `int first = friendCount + field_164;` (loop cond unchanged) | 92.094405 | 17172 |
| `int pirateEscorts = stationPirates ? (int)(…) : 0` | 86.075113 | 17180 |
| `int first = …` used in the loop condition/body | 76.338771 | 17272 |
| `int d = i - first;` + `d == 0 && alienWave` | 76.338771 | 17272 |
| alienGuns loop without `k++` in the for-header | 88.481546 | 17160 |

## 4. Conclusion

The residual is dominated by `RegAllocGreedy` register/slot assignment in a
6 k-instruction function, and by two shape clusters that clang 7.0.2 cannot
reproduce from plain source with the current IR shape:

* the `orrs` merged `alienWave`/`pirateFortress` predicates (§1) — the only
  source-reachable form is `bool` flags, which costs 3.8 pp elsewhere;
* the in-loop branchy `kAlienStationTurrets` select (`LowerSELECT_CC` always
  returns `getCMOV`, see wave-110) and the descending convoy chain (§3.4).

No variant beat the committed baseline's 92.094405 % / 17172 B.  The next
worker should treat §1 as the actionable lever: any source shape that makes
leaf 0 of the station-loop `and` an `SETEQ`/non-`CmpInst` **without** changing
`alienWave`'s type (and therefore without the 3.8 pp bool regression) would fix
both merged predicates for free.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w6012/score.py _ZN5Level13createMissionEv [diff|map]
```

Scratch tooling (not committed): `/dev/shm/w6012/score.py` (official verifier
normalisation, unrounded), `shapescore.py` (register-collapsed score),
`regmap.py` (aligned register-rename histogram), `slotlines.py` (spill-slot ->
source-line map), `try.py`/`try2.py`/`batch*.py` (apply replacement, link,
score), `cc.sh`/`fn.sh` (single-TU compile + `orrs`/`ands` count),
`dumpidx.py`/`idx2line.py` (instruction index <-> source line).
