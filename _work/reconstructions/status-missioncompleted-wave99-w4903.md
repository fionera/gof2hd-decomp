# `Status::missionCompleted` — case-166 allocation is already exact; the residual
# is the MBP body order (w4903, wave 99)

Status: **research note, no source change landed** (gate unchanged). Worktree
`wt/w4903`, branch `wave99-w4903`, base master `44225192c`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 r328903, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 (0x598)**, currently
**60.4 % / 1432 B** (`report.json`: `pct 60.4`, `osize 1432`, `usize 1432`).

## 0. Gate (start and end of this pass)

```
cmake --build cmake-build-match --target check -j8
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

No source change was landed: every variant tried below is either the same
60.4 or worse, and the 1428-B variants break `size-exact`.

## 1. What the committed source already gets right

The committed `case 166` (git HEAD) is:

```cpp
if (atStation && station->getIndex() == m->getTargetStation()) {
    int index = m->getProductionGoodIndex();
    int amount = m->getProductionGoodAmount();
    Ship *s = ship;
    if (s == 0) return m;
    if (Item::isInList(index, amount, s->getCargo())) {
        return m;
    }
    if (Globals::status->ship->hasEquipment(m->getProductionGoodIndex(), 1)) {
        return m;
    }
}
break;
```

Its real build (`cmake-build-match/libgof2hdaa.so`, `0xac2fe`) is:

```
ac318  mov   r0, sl
ac31a  blx   Mission::getProductionGoodIndex
ac31e  mov   r9, r0                 ; index -> r9   (target)
ac320  mov   r0, sl
ac322  blx   Mission::getProductionGoodAmount
ac326  mov   r5, r0                 ; amount -> r5  (target)
ac328  ldr.w r0, [r6, #400]         ; this in r6    (target)
ac32c  cmp   r0, #0                 ; <- null check (extra)
ac32e  beq.w ac49c                  ; <- null check (extra)
ac332  blx   Ship::getCargo
ac336  mov   r2, r0
ac338  mov   r0, r9
ac33a  mov   r1, r5
ac33c  blx   Item::isInList
```

The original (`0xac260`) is the same stream **without** the two
`cmp/beq.w` instructions:

```
ac260  mov   r0, sl
ac262  blx   Mission::getProductionGoodIndex
ac266  mov   r9, r0
ac268  mov   r0, sl
ac26a  blx   Mission::getProductionGoodAmount
ac26e  mov   r5, r0
ac270  ldr.w r0, [r6, #400]
ac274  blx   Ship::getCargo
ac278  mov   r2, r0
ac27a  mov   r0, r9
ac27c  mov   r1, r5
ac27e  blx   Item::isInList
```

So **the case-166 register allocation is already identical to the original**
(`index -> r9`, `amount -> r5`, `this -> r6`, ship load `r0`/`[r6,#400]`).
The only case-166 delta is the two-instruction ship null check, which is the
size anchor that keeps the direct-`int` switch at 1432 B (the faithful plain
form is 1428 B and shifts `.text`).

## 2. Why the plain form does not reproduce the allocation

`plain` (`if (Item::isInList(index, amount, ship->getCargo()))`, no check) is
1428 B and allocates the *other* way:

```
ac31e  mov   r5, r0                 ; index -> r5
ac326  mov   r1, r6                 ; this saved to r1
ac328  mov   r6, r0                 ; amount -> r6
ac32a  ldr.w r0, [r1, #400]         ; ship via r1
...
ac3e6  ldr   r6, [sp, #16]          ; this reloaded
```

The null check splits the case body into two MBBs. The greedy allocator then
treats `index`/`amount` as global intervals and the `amount` interval evicts
`index` from `r5` to `r9` (the same cascade as the original). Without the
split, `amount` takes the free `r6` and `this` is spilled instead. This is the
mechanism documented in `status-missioncompleted-wave97-w4703.md` and
`status-missioncompleted-wave96-w4601.md`; nothing new.

## 3. Per-body match: everything else is branch-width / layout

Aligning each TBH body by content (ignoring addresses) gives:

| case | orig addr | ours addr | orig len | ours len | match |
|---|---:|---:|---:|---:|---:|
| 150–155 | ac110.. | ac110.. | 2/2/20/2/8/6 | same | **100 %** |
| 156 | ac32a | ac180 | 14 | 14 | 92.9 % (`bge.w` vs `bge.n`) |
| 157 | ac354 | ac1ac | 20 | 20 | **100 %** |
| 158 | ac180 | ac1e0 | 33 | 33 | **100 %** |
| 160 | ac388 | ac23c | 16 | 16 | 87.5 % (`blt.w/beq.w` vs `blt.n/beq.n`) |
| 162 | ac1dc | ac26e | 35 | 35 | **100 %** |
| 163 | ac3b6 | ac2d4 | 11 | 11 | 90.9 % (`bcs.w` vs `bcs.n`) |
| 164/165 | ac3d2/ac3d6 | .. | 2/3 | 2/3 | **100 %** |
| 166 | ac242 | ac2fe | 35 | 37 | 88.9 % (null check + branch widths) |
| 167 | ac036 | ac036 | 75 | 75 | 94.7 % (jump-table entries) |
| 168 | ac3de | ac364 | 13 | 10 | 78.3 % (pool placement) |
| 169 | ac3fc | ac37c | 2 | 2 | **100 %** |
| 170 | ac2a6 | ac380 | 5 | 5 | 80 % (`bne.w` vs `bne.n`) |
| 171 | ac400 | ac38c | 10 | 10 | **100 %** |
| 172 | ac41a | ac3a6 | 24 | 25 | 93.9 % (`cbnz` vs `cmp+bne.n`) |
| 184 | ac2b4 | ac3f4 | 20 | 20 | 75 % (`bne.w/beq.w` vs `.n`) |
| 189 | ac2ee | ac426 | 22 | 51 | 54.8 % (boundary artifact) |

Every non-100 % body differs only in **16- vs 32-bit branch width** (or in
literal-pool placement), i.e. in the distance to the shared
loop-continue/return blocks. No body has a source-shape difference other than
the case-166 null check.

## 4. The layout residual (compiler-side)

The original lays the 22 high-group bodies out in **source order**:

```
167/174 150 151 152 153 154 155 158 162 166 170 184 189
        156 157 160 163 164 165 168 169 171 172
```

The stock NDK clang 7.0.2 lays them out in **ascending case value**:

```
150 151 152 153 154 155 156 157 158 160 162 163 164 165 166
    168 169 170 171 172 184 189
```

`SelectionDAGBuilder::buildJumpTable` adds the switch successors in *jump
table* order (ascending case value; "use table order for determinism"), and
`MachineBlockPlacement::selectBestCandidateBlock` picks the first
max-frequency block from that worklist. The MBB numbers are already in
source order (`36=150 38=151 39=152 44=153 45=154 46=155 47=158 57=162
62=166 66=170 67=184 71=189 77=156 80=157 85=160 88=163 91=164 92=165
93=168 96=169 97=171 98=172`), so sorting the worklist by MBB number
(`MBP_SORT_SBCB`) reproduces the original layout exactly — but that is a
compiler patch, not a source shape.

Reordering the source cases does **not** change the high-group layout
(measured: the worklist is always ascending case value). It only changes the
low-group/prologue codegen, and the committed source already reproduces the
original low-group layout byte-for-byte (`case 8 @ ac060`, `case 11 @ ac0c4`,
`case 13 @ ac0ce`, `case 14 @ ac0dc`, high group starting at `ac110`).

The 8 jump-table entries for 150–155, 167 and 174 already compare equal; the
other 32 differ only because their target bodies are at different addresses.

## 5. Case-166 shape sweep (real build, direct `int`)

All variants keep the rest of the function identical. `len`/`M` are the
normalized instruction-sequence lengths and LCS match count used by
`tools/verify/verify.py` (`ratio = 2M/(len_o+len_u)`).

| variant | size | len | M | raw ratio | pct |
|---|---:|---:|---:|---:|---:|
| **committed** (`Ship *s; if (s==0) return m;`) | **1432** | 527 | 317 | 60.380952 | **60.4** |
| `if (ship == 0) return m;` (no local) | 1432 | 527 | 317 | 60.380952 | 60.4 |
| `if (ship == 0) return 0;` | 1432 | 527 | 317 | 60.380952 | 60.4 |
| `if (!ship) return m;` | 1432 | 527 | 317 | 60.380952 | 60.4 |
| `Array<Item*> *cargo = ship->getCargo(); if (cargo==0) return m;` | 1432 | 527 | 317 | 60.380952 | 60.4 |
| `if (missions != 0) { ... }` | 1432 | 527 | 317 | 60.380952 | 60.4 |
| `if (ship != 0 && missions != 0 && isInList(...))` | 1432 | 527 | 317 | 60.380952 | 60.4 |
| `if (ship != 0 && station != 0 && isInList(...))` | 1432 | 527 | 317 | 60.380952 | 60.4 |
| `if (ship != 0 && isInList(...))` | 1428 | 526 | 317 | 60.438513 | 60.4 |
| `if (ship == 0) break;` | 1428 | 526 | 317 | 60.438513 | 60.4 |
| `if (ship != 0 && m != 0 && isInList(...))` | 1428 | 526 | 317 | 60.438513 | 60.4 |
| plain `ship->getCargo()` (no check) | 1428 | 526 | 312 | 59.485224 | 59.5 |
| `if (Globals::status->ship == 0) return m;` | 1436 | 528 | 314 | 59.752617 | 59.8 |
| `if (Globals::status == 0) return m;` | 1432 | 527 | 314 | 59.809524 | 59.8 |
| `if (station == 0) return m;` | 1436 | 528 | 317 | 60.323501 | 60.3 |
| `if (field_0xf0 \|\| isInList(...))` | 1436 | 527 | 317 | 60.323501 | 60.3 |
| `Ship *s` before `index` | 1436 | 529 | 274 | 52.091255 | 52.1 |
| `if (s==0) return m;` before `index` | 1436 | 529 | 314 | 59.695817 | 59.7 |
| `if (s==0) return m;` after `isInList` | 1440 | 530 | 313 | 59.449193 | 59.4 |
| `if (m == 0) return m;` | 1428 | 526 | 312 | 59.485224 | 59.5 |
| `do{}while(0)` / `for(k<1)` / `if(1)` / `switch(0)` wrappers | 1428 | 526 | 312 | 59.485224 | 59.5 |
| `volatile Ship *s` / `volatile int index` / `volatile cargo` | 1460–1468 | 549–552 | 265–267 | 49.3–49.7 | 49.x |

The sweep is saturated at **M = 317**: no case-166 shape reaches M = 318
(which would give `2·318/(523+527) = 60.51 %` → 60.5). The 1428-B shapes
(`M = 317`, raw 60.4385) round to 60.4 and break `size-exact`.

Also tried, all 60.380952 / 1432 B unless noted: `if (ship == 0) return m;`
placed after `getProductionGoodAmount`, `if (ship == 0) { return m; } else
{...}`, `if (!(s != 0))`, cached `bool ok = s != 0`, `cargo` ternary,
`return (*missions)[i]`, `return *(&m)`, `bool found`, `if (!isInList) break;
return m;`, `else`/`goto`/`while (m != 0)` wrappers, and explicit
`case 159/161/173/175..188: break;` holes (the compiler merges them with the
default). Low-group source permutations: base 60.38, `8,0,13,11,14` 60.00,
`8,11,0,13,14` 60.00, `0,11,8,13,14` 60.38, all others ≤ 60.00. High-group
source permutations (including the original order and its reverse) leave the
high-group layout ascending and only perturb the prologue/low group; the
committed order is the best measured.

## 6. Conclusion

* The case-166 register allocation is **already exact** in the committed
  build; the only case-166 delta is the two-instruction null-check size
  anchor.
* The remaining ~40 % fuzzy delta is dominated by the compiler-side
  `MachineBlockPlacement` body order (ascending case value vs the original's
  source order). That changes every intra-function branch target and every
  branch width, so it cannot be recovered by a source-shape change with the
  stock NDK clang.
* No strict improvement was found: the committed shape is the best measured
  variant (raw 60.380952 → 60.4; every 1432-B alternative is ≤ 60.38 and
  every 1428-B alternative breaks `size-exact`). The gate is unchanged.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4903
cmake --build cmake-build-match --target check -j8   # RATCHET PASS
cmake --build cmake-build-match --target verify -j8  # report.json: pct 60.4

# exact-LLVM oracle (scratch, /dev/shm/w4903)
./mkll.sh src/game/mission/Status.cpp s.ll            # clang++ -g0 -S -emit-llvm
./mkobj.sh s.ll s.o                                   # llc (unpatched)
MBP_SORT_SBCB=1 ./mkobj.sh s.ll sorted.o              # original body order, 1436 B
python3 cases.py                                      # 40-entry TBH layout
python3 perbody.py                                    # per-body match table
python3 ratio.py                                      # len/M/raw ratio
```

Scratch (not committed): `/dev/shm/w4903/` — `base.cpp` (committed
`Status.cpp`), `mkll.sh`/`mkobj.sh` (exact LLVM 7.0.0 oracle), `cc.sh`,
`cases.py`/`lowcases.py`/`casesizes.py` (TBH decoding), `perbody.py`
(per-body match), `ratio.py`/`score_raw.py`/`score_one.py` (verify.py
scoring), `gen*.py` (variant sweeps), `tryorder.sh`/`reorder2.py` (source
order permutations), `orig.dis`/`ours.dis`/`norm.txt` (disassembly).
