# `Status::missionCompleted` — exact LLVM oracle, MBB-order worklist confirmed,
# and the last 4 bytes localised to case 166 (w4601, wave 96)

Status: research note, **no source change landed** (gate unchanged). Worktree
`wt/w4601`, branch `wave96-w4601`, base master `3bcaeb3a`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 `r328903`/`llvm-svn.328903`, `-Oz`, image base 0x10000; real VMA =
Ghidra − 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 (0x598)**, currently
**57.7 % / 1432 B** with the committed `switch ((short) m->getType())`.

This pass adds three things the earlier notes did not have:

1. A **local build of the exact Android LLVM (`llvm-svn.328903`)**, proven to
   reproduce the NDK clang object **byte-for-byte** for this function. All
   experiments below run in ~0.1 s instead of a full tree rebuild.
2. A **decisive re-measurement** of the three candidate layouts with that
   oracle. `MBP_SORT_SBCB` (sort the placement worklist by MBB number)
   reproduces the original body order **exactly**; the
   `getFirstUnplacedBlock` fallback does **not** (it changes the whole
   function, not just the high group).
3. The **last 4-byte delta localised to one two-instruction register-allocation
   difference in case 166** (`mov r1,r6; mov r6,r0`). With the sort patch the
   function is 1436 B and differs from the original in exactly two body-shape
   spots (case 158 tail scheduling, case 166 regalloc) plus the jump-table
   data.

## 0. Baseline (measured in this worktree)

```sh
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 1. The exact-LLVM oracle (new)

```sh
git clone --depth 1 --branch llvm-svn.328903 \
    https://android.googlesource.com/toolchain/llvm /dev/shm/w4601/llvm328903
# (GCC 14 needs `#include <limits>` added to lib/Demangle/ItaniumDemangle.cpp)
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD=ARM \
      -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_INCLUDE_TESTS=OFF ../llvm328903
ninja llc -j32
```

`CMakeLists.txt` of that checkout says LLVM 7.0.0svn, and the `.comment`
string hash `1d739ffb` matches the NDK r18b object.

Compile the project flags to IR (`-g0 -S -emit-llvm`, same `cc.sh` flags as
the tree), then:

```sh
llc -mtriple=thumbv7-none-linux-androideabi -mattr=+neon -float-abi=soft \
    -function-sections -filetype=obj -relocation-model=pic status_g0.ll -o t.o
```

**`-relocation-model=pic` is mandatory**: without it the object is 1408/1420 B
instead of 1432. With it, the `.text._ZN6Status16missionCompletedEbbx` section
of the committed source is **byte-identical** to the NDK clang object
(1432/1432 bytes, verified by `objcopy`-style section dump).

The local tree carries env-gated hooks in `lib/CodeGen/MachineBlockPlacement.cpp`
(all no-ops unless the env var is set):

| env var | effect |
|---|---|
| `MBP_SORT_SBCB` | `std::sort` the worklist by `getNumber()` in `selectBestCandidateBlock` |
| `MBP_FORCE_FALLBACK` | `selectBestCandidateBlock` always returns `nullptr` |
| `MBP_FALLBACK_FN` | same, but only for `_ZN6Status16missionCompletedEbbx` |
| `MBP_FALLBACK_HI` | `nullptr` only when the worklist head is one of the 22 high-group MBBs |
| `MBP_FALLBACK_BIG` | `nullptr` when the worklist has >20 entries (this function only) |
| `MBP_FUB_DUMP` | print each `getFirstUnplacedBlock` result for this function |
| `MBP_DUMP` | print the worklist at every `selectBestCandidateBlock` |
| `MBP_SKIP_JT_PUSH` | skip predecessors with >4 successors in `fillWorkLists` |

## 2. Measured layout of every candidate (exact oracle)

Body order decoded from the 40-entry TBH (case values 150..189):

| build | size | high-group body order |
|---|---:|---|
| original `.so` | 1432 | `167/174 150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165 168 169 171 172` |
| committed `(short)`, unpatched | 1432 | ascending `150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189` |
| plain `int`, unpatched | 1428 | ascending |
| committed `(short)`, `MBP_SORT_SBCB` | 1436 | **original order** |
| plain `int`, `MBP_SORT_SBCB` | 1436 | **original order** |
| `MBP_FORCE_FALLBACK` / `MBP_FALLBACK_FN` / `MBP_FALLBACK_HI` | 1444 | original order |

The worklist at the high jump table (from `MBP_DUMP`) is

```
%bb.36 %bb.38 %bb.39 %bb.44 %bb.45 %bb.46 %bb.77 %bb.80 %bb.47 %bb.85
%bb.57 %bb.88 %bb.91 %bb.92 %bb.62 %bb.93 %bb.96 %bb.66 %bb.97 %bb.98
%bb.67 %bb.71 %bb.21
```

with `36=150 38=151 39=152 44=153 45=154 46=155 77=156 80=157 47=158
85=160 57=162 88=163 91=164 92=165 62=166 93=168 96=169 66=170 97=171
98=172 67=184 71=189` — i.e. **ascending case value**, while the MBB numbers
(which follow the IR block order, which follows the source order) are already
in the **original** order. `MBP_SORT_SBCB` therefore reproduces the original
layout exactly, and the whole residual is "make the push order equal the MBB
order".

## 3. The fallback is *not* the mechanism (re-measured with the exact oracle)

`getFirstUnplacedBlock` walks `F` in MBB order, so it also yields the original
high-group order — but it changes the layout of the **whole function**, not
just the high group:

* `MBP_FALLBACK_FN` / `MBP_FALLBACK_HI` (fallback only for this function, or
  only for the 22 high-group MBBs) give the original body order at **1444 B**,
  but the first difference is at **0x4a**: the original has the loop test
  inline (`ldr.w r0,[r6,#408]; ldr r1,[r0]; cmp fp,r1; bcs.w`), the fallback
  build has `b.n` to a loop-test block placed elsewhere.
* `-disable-block-placement` changes the dispatch shape altogether (the high
  range check becomes a separate block at 0x4ca).

So the original is the *worklist* path (ascending push order, MBB-order
selection) — not the fallback, and not "no MBP".

## 4. The last 4 bytes: case 166 register allocation

With `MBP_SORT_SBCB` the plain-`int` build is 1436 B. Instruction-level diff
against the original (branch targets normalised, calls compared by symbol,
jump-table halfwords excluded) leaves exactly:

**(a) case 158 tail — size-neutral, scheduling only.**

```
orig  ac1cc  and.w r0, r9, r6        ours  294  and.w r0, r9, r6
      ac1d0  ldr   r6, [sp, #16]           298  mov   r8, r4
      ac1d2  mov   r8, r4                  29a  lsls  r0, r0, #31
      ac1d4  lsls  r0, r0, #31             29c  ldr   r6, [sp, #16]
      ac1d6  beq.w ...                     29e  beq.w ...
```

The original restores `this` (r6, spilled because r6 holds `hasGood` in the
loop) immediately after the last use of r6; ours restores it after `lsls`.
Same instruction multiset, different order — byte difference, no size
difference.

**(b) case 166 — this is the whole 4-byte delta.**

```
orig  ac260  mov   r0, sl                    ours  328  mov   r0, sl
      ac262  blx   getProductionGoodIndex          32a  bl    getProductionGoodIndex
      ac266  mov   r9, r0                         32e  mov   r5, r0
      ac268  mov   r0, sl                         330  mov   r0, sl
      ac26a  blx   getProductionGoodAmount        332  bl    getProductionGoodAmount
      ac26e  mov   r5, r0                         336  mov   r1, r6      <-- +1 insn
      ac270  ldr.w r0, [r6, #400]                 338  mov   r6, r0      <-- +1 insn
      ac274  blx   getCargo                       33a  ldr.w r0, [r1, #400]
      ac278  mov   r2, r0                         33e  bl    getCargo
      ac27a  mov   r0, r9                         342  mov   r2, r0
      ac27c  mov   r1, r5                         344  mov   r0, r5
      ac27e  blx   isInList                       346  mov   r1, r6
                                                  348  bl    isInList
```

The original keeps `this` in r6 and puts `index` in **r9**, `amount` in
**r5**. Ours puts `index` in **r5**, `amount` in **r6** (evicting `this`,
hence the two extra `mov`s = +4 B). Note the *same* pattern in case 8 is
allocated r9/r5 in **both** builds, so r9 is available and the allocator can
make that choice; case 166's choice is shape-driven.

**(c) consequence of (b):** ours must restore `this` before the shared
"return m" tail, so `if (hasEquipment(...)) return m;` becomes
`bl hasEquipment; cmp r0,#0; b.n <case-158 tail>` (4 B) instead of the
original's `bl hasEquipment; b.n ac458` (2 B) where `ac458` is the dedicated
`cbnz r0, ac49e` block. This is absorbed by alignment padding, so it does not
add to the size, but it is a byte difference.

**(d) non-diffs:** `bl inAlienOrbit` vs a local `bl` (relocation only), the
jump-table halfwords and the 3-word literal pool (both follow from the
layout).

### Case-166 source-shape sweep (all with `MBP_SORT_SBCB`)

| variant | size |
|---|---:|
| committed `Ship *s = ship;` | 1436 |
| `ship->getCargo()` inline | 1436 |
| `Item::isInList(m->getProductionGoodIndex(), m->getProductionGoodAmount(), ship->getCargo())` | 1436 |
| `Ship *s` before / after the two calls | 1436 |
| `amount` then `index` | 1436 |
| `const int` / `unsigned` / `long long` index+amount | 1436 |
| `Array<Item*> *cargo = ship->getCargo();` | 1436 |
| nested `{ }` scope around the isInList check | 1436 |
| `bool inList = ...; if (inList)` | 1436 |
| `hasEquipment(...) != 0` | 1436 |
| `Globals::status->ship->getCargo()` | 1436 |
| `int index2 = m->getProductionGoodIndex(); hasEquipment(index2, 1)` | **1432** (but it hoists the second `getProductionGoodIndex` *before* the `Globals::status` load, so it is a different instruction stream) |

### Case-158 source-shape sweep (all with `MBP_SORT_SBCB`)

| variant | size | effect |
|---|---:|---|
| committed | 1436 | — |
| declare `hasGood` before `hasSpecial` | 1436 | `movs r6,#0` moves; `and.w r0,r6,r9` operand swap |
| `if (hasSpecial && hasGood)` | 1436 | changes case 166 instead (hoists `ldr.w r6,[r6,#400]`) |
| `int` flags instead of `bool` | 1476 | worse |
| cache `equipment->size()` in a local | 1424 | changes the loop |

## 5. Source-order / type sweep (unpatched oracle, plain `int`)

All still ascending, all 1428 unless noted: explicit `default: break;`,
`default: continue;`, `do { switch } while (0)`, `switch ((unsigned) ...)`,
`switch (m->getType() & 0xffff)` (1436, same order). Reordering the `case`
labels is irrelevant (confirmed again): the jump table's successor list is
built by `SelectionDAGBuilder::buildJumpTable` from `Clusters` sorted
ascending by case value, so the push order is always ascending.

Two-switch forms (the only known order-correct source shapes):

| form | size | table |
|---|---:|---|
| two `switch (m->getType())` (re-call) | 1468 | 25-entry high table (wrong shape) |
| `int mtype = m->getType();` + two `switch (mtype)` | **1508** | 40-entry merged table + 17-entry TBB (original order) |
| group B in `default:` with cached `mtype` | 1428 | ascending |

The cached two-switch form is the w4103 result (1508 B, original order): the
group-B bodies get a second in-loop predecessor (the surviving second
dispatch block), so they are pushed after group A. It costs +76 B because
`mtype` stays live in r11 across the first switch and the second dispatch
block + TBB is emitted; the original has **no** second dispatch, so this is
not the original shape.

## 6. Conclusion — proven vs open

**Proven (this pass):**

1. The exact `llvm-svn.328903` `llc` reproduces the NDK clang object
   byte-for-byte for this function (with `-relocation-model=pic`), so it is a
   valid oracle.
2. `MBP_SORT_SBCB` (worklist sorted by MBB number) reproduces the original
   body order **exactly**; the worklist is otherwise ascending case value
   while the MBB numbers are already in the original (source) order.
3. The fallback (`getFirstUnplacedBlock`) also yields the original high-group
   order but changes the whole function's layout (first diff at 0x4a), so it
   is not the mechanism.
4. With the sort patch the function is **1436 B**; the entire 4-byte delta is
   the two extra `mov`s in case 166 (`this` evicted from r6 because `index`
   and `amount` are allocated r5/r6 instead of r9/r5). The case-158 tail is a
   size-neutral scheduling difference.
5. The two-switch form reproduces the order but is 1508 B and emits a second
   dispatch block the original does not have.

**Open:** a single-switch CFG in which `markBlockSuccessors` pushes the 22
high-group bodies in MBB order (equivalently: a CFG in which the high jump
table's successor list is in source order, or in which the group-B bodies
have a second unplaced in-loop predecessor that is placed after 189) while
keeping the plain-`int` register allocation and no extra dispatch block. Every
shape tried so far (≈140 variants across w3906/w4003/w4103/w4201/w4222/w4251/
w4272/w4405 and this pass) either stays ascending, changes the whole function,
or costs ≥36 B.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4601
cmake --build cmake-build-match --target check -j8      # RATCHET PASS

# exact oracle (scratch, /dev/shm/w4601)
./cc.sh src/game/mission/Status.cpp s.ll -g0 -S -emit-llvm
llvm328903/build/bin/llc -mtriple=thumbv7-none-linux-androideabi -mattr=+neon \
    -float-abi=soft -function-sections -filetype=obj -relocation-model=pic \
    s.ll -o s.o
MBP_SORT_SBCB=1 llvm328903/build/bin/llc ... s.ll -o sorted.o
python3 phys.py sorted.o        # size + 40-entry TBH body order
python3 cmp5.py sorted.o        # instruction diff vs the original
```

Scratch (not committed): `/dev/shm/w4601/` — `llvm328903/` (full LLVM tree
with `build/bin/llc` and the MBP hooks), `cc.sh`, `phys.py` (size + TBH
decode), `cmp5.py` (normalised instruction diff), `decorig.py`, `order.py`,
`chk.ll`/`ndk.o`/`llc.o` (oracle proof), `v166*.cpp`, `b166_*.cpp`,
`s158_*.cpp`, `c166_*.cpp`, `twosw*.cpp`, `T1/T2.cpp`.
