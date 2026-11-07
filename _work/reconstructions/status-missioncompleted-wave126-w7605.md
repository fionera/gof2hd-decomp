# `Status::missionCompleted` — wave 126 (w7605): case-162 branch hint,
# 71.0 -> 71.1 %; the residual is the high-TBH body order (edge weights)

Status: **source change landed** (one `__builtin_expect` on the case-162
`hasCargo` loop condition).  Worktree `wt/w7605`, base master `bf365a964`
(wave-125 final snapshot, byte 4506 / linked 4506).
Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`,
image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | before | after |
|---|---:|---:|
| function score | 71.0 % | **71.1 %** |
| function size | 1432 B | **1432 B** |
| gate byte_exact | 4506 | **4506** |
| gate linked_exact | 4506 | **4506** |
| data_exact | 843/843 | **843/843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |
| `.text` differing bytes | 85,636 | **85,559** |
| `.ARM.extab` differing bytes | 1,436 | **1,438** (size exact) |
| `.rodata` differing bytes | 0 | **0** |

## 0. Summary

1. The original's high jump table is a plain 40-entry `tbh` over
   `getType()-150`; the case bodies are laid out in **source order**
   (`167/174, 8, 11, 13, 14, A(150..189 source run), B(156..172 source
   run), latch`), while our identical compiler lays them out in **ascending
   case-value order**.  This is the dominant residual.
2. The compiler is byte-identical to the original (`.comment`:
   `Android (4751641 based on r328903) clang 7.0.2`, LLVM `1d739ffb`), and
   another non-ascending switch in the same binary
   (`Globals::getDialogueSoundId`, cases 389,370,371,...) *is* byte-exact
   with value-order bodies.  So the original compiler uses value order; the
   `missionCompleted` A-then-B order therefore comes from **unequal switch
   edge frequencies** (PGO / `!prof`), not from a different compiler.
3. `MachineBlockPlacement` proves it: with the stock NDK clang the high
   jump-table successors are all 4.10 % except `167/174` (8.19 %), and
   `selectBestSuccessor` drains them in successor (value) order.
4. One new source win: a `__builtin_expect(...,1)` on the case-162
   `hasCargo` loop condition (71.0 -> 71.1 %, size unchanged).  A full
   greedy re-sweep of every `if` in the function (both expect values) found
   no other 1432-B improvement.

## 1. Original jump-table / literal-pool structure (ground truth)

Decoded from the original `.so` (image base 0x10000; real VMA = Ghidra -
0x10000).  Dispatch at `0xabfd4`:

```
abfd4  mov r0, sl
abfd6  blx Mission::getType
abfda  sub.w r1, r0, #150
abfde  cmp   r1, #39
abfe0  bhi.n ac046            ; -> low range check
abfe2  tbh   [pc, r1, lsl #1] ; high table, 40 entries at abfe6
```

High table (`base = abfe6`, target = base + 2*entry), values 150..189:

```
150 ac110  151 ac116  152 ac11c  153 ac150  154 ac156  155 ac16e
156 ac32a  157 ac354  158 ac180  159 ac45a  160 ac388  161 ac45a
162 ac1dc  163 ac3b6  164 ac3d2  165 ac3d6  166 ac242  167 ac036
168 ac3de  169 ac3fc  170 ac2a6  171 ac400  172 ac41a  173 ac45a
174 ac036  175..183 ac45a  184 ac2b4  185..188 ac45a  189 ac2ee
```

Low table at `0xac04e` (7 entries, values 8..14):

```
8 ac060  9 ac45a  10 ac45a  11 ac0c4  12 ac45a  13 ac0ce  14 ac0dc
```

Sorted by body address, the high bodies are exactly the source order used in
`src/game/mission/Status.cpp`:

```
167/174 ac036
150 151 152 153 154 155 158 162 166 170 184 189   (A run, ac110..ac2ee)
156 157 160 163 164 165 168 169 171 172           (B run, ac32a..ac41a)
latch ac45a
```

Our build emits the same dispatch / TBH size / TBH base and the same low
group, but the high bodies in ascending value order:

```
150 151 152 153 154 155 156 157 158 160 162 163 164 165 166
    168 169 170 171 172 184 189
```

The literal pools are identical in content modulo the PC-relative deltas
(`0x001640ee`, `0x00163ece`, `0x00163d78`, `0x00163e96` in the original;
the same four words re-based in ours).  The shared tails
(`ret0 ac49a`, `retm ac49e`, `setWon ac4a8`, `setWon+field_178 ac4b2`) are
byte-identical, so **return shapes are not a residual**.

## 2. Why the body order is a weight effect (compiler evidence)

The project's custom LLVM 7.0.0 tree (`/home/debian/llvm7build`) was used
to dump `MachineBlockPlacement` decisions for the current source.  The switch
is MBB `%bb.104`; its successors are added by
`SelectionDAGBuilder::buildJumpTable` in `Table` (ascending value) order and
all carry the same `JTProbs` except `167/174` (two case values -> 2x):

```
Candidate: %bb.36 4.10%   (150)     Candidate: %bb.80 4.10%  (157)
Candidate: %bb.38 4.10%   (151)     Candidate: %bb.47 4.10%  (158)
Candidate: %bb.39 4.10%   (152)     Candidate: %bb.85 4.10%  (160)
Candidate: %bb.44 4.10%   (153)     Candidate: %bb.57 4.10%  (162)
Candidate: %bb.45 4.10%   (154)     Candidate: %bb.88 4.10%  (163)
Candidate: %bb.46 4.10%   (155)     ...
Candidate: %bb.77 4.10%   (156)     Candidate: %bb.16 8.19%  (167/174)
```

`selectBestSuccessor` picks `%bb.16` (8.19 %), then the remaining bodies tie
at 4.10 % and are drained in worklist (= value) order.  Unequal
`!prof branch_weights` with A > B reproduce the original order exactly
(wave 123/124/125: `MBP_SORT_SBCB=1` or `default 1, A 200, B 199,
167/174 200, low 201`), but plain C++11 cannot emit multi-case switch
weights (`__builtin_expect` sets exactly one weight to 2000; the
if-chain fold emits uninitialised `SmallVector` garbage).

## 3. Per-body comparison (normalised instructions)

Aligning each TBH body by content (branch widths and pool offsets ignored):

| body | orig | ours | note |
|---|---:|---:|---|
| 167/174 | 17 | 17 | **identical** |
| 8 | 35 | 35 | pool delta only |
| 11,13,14 | 3/4/16 | same | branch width only |
| 150,151,152,153,154,155 | 2/2/20/2/8/6 | same | **identical** |
| 158 | 33 | 31 | `ldr r6,[sp,#16]`/`beq` placement |
| 162 | 35 | 35 | branch width (see §4) |
| 166 | 35 | 39 | regalloc + duplicated `hasEquipment` tail |
| 170 | 5 | 5 | branch width |
| 184 | 20 | 19 | `cmp+bne` vs `cbnz` |
| 189 | 22 | 21 | trailing `b` |
| 156 | 14 | 14 | branch width |
| 157 | 20 | 19 | loop rotation |
| 160 | 16 | 16 | branch width |
| 163 | 11 | 11 | `ble` vs `bgt` |
| 164,165 | 2/3 | 2/3 | branch width |
| 168 | 13 | 10 | extra words are the literal pool |
| 169 | 2 | 2 | branch width |
| 171 | 10 | 11 | `cbz` vs `cmp+beq` |
| 172 | 24 | 29 | `cbz`/`cbnz` range (body-order consequence) |
| tails | 1/4/4/7 | same | **identical** |

Every non-identical body except 166/184/163 is a 16- vs 32-bit branch
encoding or a literal-pool placement, i.e. a distance-to-shared-tail effect
of the body order.  Body 166's allocation is the wave-97/98 cascade
(`this` spilled because the case-166 `__builtin_expect` splits the block);
it is also the size anchor that keeps the function at 1432 B.

## 4. The landed change: case-162 `hasCargo` hint

`case 162` walks the blueprint ingredient/quantity lists:

```cpp
for (unsigned j = 0; j < ingredients->size(); j++) {
    if (!ship->hasCargo((*ingredients)[j], (*quantities)[j])) {
        complete = false;
        break;
    }
}
```

Wrapping the condition in `__builtin_expect(..., 1)` (same-line
`// lint: arm_builtin` waiver) makes the "cargo present" path the likely
fallthrough and matches the original's loop branch layout more closely.
Measured with the official single-symbol verifier:

| variant | usize | pct |
|---|---:|---:|
| baseline (wave 125) | 1432 | 71.0 |
| **+ case-162 `hasCargo` expect(1)** | **1432** | **71.1** |

A full greedy re-sweep of all 45 `if` conditions in the function (both
`__builtin_expect(...,1)` and `(...,0)`, one at a time, on top of the new
hint) found no other 1432-B variant above 71.1.  `switch
(__builtin_expect(m->getType(), K))` for K in {0,8,14,150,167,189} is
1384-1428 B and 16-27 % (K=14 is the only 1432-B one, scrambled).

## 5. Why no larger source win exists

* Reordering the source cases does not move the high-group layout: the
  successor worklist is always ascending case value (measured; waves
  119-125).
* The only plain-C++ ways to make the A bodies hotter are extra case labels
  (hole-fill: exact order but 1376 B and a different TBH content), a rank
  table (wave 121: 92.45 % but +192 B `.rodata`, now banned as manually
  linked data), or a second switch (1440+ B).
* `!prof` weights are not expressible from C++11: `__builtin_expect` sets
  one weight to 2000; the if-chain fold's weights are uninitialised
  `SmallVector` contents.

The residual is therefore the original's PGO-like switch-edge profile, not
a source shape.  The 71.1 % result keeps the original dispatch, TBH, low
group, register allocation and all shared tails.

## 6. Reproduction

```sh
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8    # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j8    # 0 fields

# decode the original TBH (base 0xabfe6 / 0xac052)
python3 - <<'PY'
import struct, sys
sys.path.insert(0, "tools/verify")
from elf import Elf32
e = Elf32("/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so")
for tbh, n, first in ((0xabfe2, 40, 150), (0xac04e, 7, 8)):
    base = tbh + 4
    b = e.bytes_at(base, 2*n)
    print([(first+i, hex(base + 2*struct.unpack_from("<H", b, 2*i)[0]))
           for i in range(n)])
PY

# single-function score
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN6Status16missionCompletedEbbx
```

Scratch (not committed): `/dev/shm/w7605/` — `orig.dis`/`ours.dis`
(disassembly), `decode.py` (TBH decoder), `perbody.py` (per-body LCS),
`score.py` (single-function verify score), `vary.py` (edit/build/score
harness), `genchain.py` (switch -> if-chain generator), `dbg.txt`
(`-debug-only=block-placement` dump from the custom LLVM).
