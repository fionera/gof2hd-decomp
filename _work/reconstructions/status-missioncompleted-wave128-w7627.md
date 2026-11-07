# `Status::missionCompleted` — wave 128 (w7627): per-body reconstruction audit;
# case 162/163 bodies are instruction-identical once the branch polarity is
# written the original way, but the PGO body order keeps the fuzzy score at 71.1

Status: **note-only** (no source change landed; baseline kept at 71.1 %).
Worktree `wt/w7627`, base master `4bbe88776` (wave-127 snapshot, byte 4506 /
linked 4506).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`,
image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `4bbe88776`) |
|---|---:|
| function score | **71.1 %** (523 vs 524 normalised insns) |
| function size | 1432 B |
| gate byte_exact | 4506 |
| gate linked_exact | 4506 |
| data_exact | 843/843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |
| `.text` differing bytes | 85,559 |

## 0. Summary

This pass audited the function **body by body** (TBH-target delimited) instead
of only the whole-function fuzzy score, and re-ran exhaustive `__builtin_expect`
sweeps.  Three concrete results:

1. **Every body is instruction-identical to the original except for branch
   width and two branch polarities.**  The differences are *distance-to-shared-
   tail* effects of the wrong body order, not missing source.  The only real
   source-shape mismatches are case 162 (`beq.w` vs the original's `bne.n`
   loop branch) and case 163 (`bgt.w` vs the original's `ble.n`).  Both can be
   made byte-identical in source (see §3), but each costs 0.1 fuzzy because
   the 1432-B layout shifts.
2. **No single or pair of `__builtin_expect` hints beats 71.1 at 1432 B.**
   A full greedy sweep of all 48 `if`/`for` conditions (both values), all
   single-hint removals/inversions of the 5 landed hints, and 90 random hint
   pairs all top out at 71.1.
3. **`switch (__builtin_expect(type, K))` is a dead end** for this switch:
   `LowerExpectIntrinsic::handleSwitchExpect` emits `getNumCases()+1` weights
   but `calcMetadataWeights` requires `getNumSuccessors()+1`, and this switch
   has 29 case values but only 27 distinct successors (167/174 and 0/11 share
   bodies), so the metadata is silently ignored.  Every K tried (0, 8, 150,
   151, 152, 156..175, 184, 189) drops the score to 18–31 %.

The residual is therefore still exactly the wave-126 conclusion: the original
lays the high case bodies out in **source order** (A run then B run), our
identical clang in **ascending case-value order**, which needs switch-edge
frequencies (`!prof` / PGO) that C++11 cannot emit.

## 1. Baseline evidence (measured this pass)

```sh
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
# compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506
# data: 843/843   imports: 207   RATCHET PASS
```

Report row: `{"name":"_ZN6Status16missionCompletedEbbx","pct":71.1,
"osize":1432,"usize":1432}`; normalised instruction counts 523 (orig) vs 524
(ours).

## 2. Per-body audit (TBH-target delimited, immediates normalised)

Decoding both TBHs (`0xabfe2`, 40 entries, values 150..189; `0xac04e`,
7 entries, values 8..14) and slicing each function at its body starts gives
the following.  "identical" means the instruction stream is the same after
normalising branch-target addresses.

| case | orig addr | ours addr | verdict |
|---|---:|---:|---|
| 167/174 | `ac036` | `ac036` | **identical** |
| 8 | `ac060` | `ac060` | **identical** (pool delta only) |
| 0/11 | `ac0c4` | `ac0c4` | **identical** |
| 13 | `ac0ce` | `ac0ce` | **identical** |
| 14 | `ac0dc` | `ac0dc` | **identical** |
| 150 | `ac110` | `ac110` | **identical** |
| 151 | `ac116` | `ac116` | **identical** |
| 152 | `ac11c` | `ac11c` | **identical** |
| 153 | `ac150` | `ac150` | **identical** |
| 154 | `ac156` | `ac156` | **identical** |
| 155 | `ac16e` | `ac16e` | **identical** |
| 156 | `ac32a` | `ac180` | `bge.n` vs `bge.w` only |
| 157 | `ac354` | `ac1ac` | loop rotated in orig (cond at bottom) |
| 158 | `ac180` | `ac1e0` | orig restores `this` (`ldr r6,[sp,#16]`) + own tail |
| 160 | `ac388` | `ac236` | `blt.n`/`beq.n` vs `blt.w`/`beq.w` |
| 162 | `ac1dc` | `ac268` | **branch polarity** (`bne.n` loop vs `beq.w` latch) |
| 163 | `ac3b6` | `ac2d0` | **branch polarity** (`ble.n` loop vs `bgt.w` latch) |
| 164 | `ac3d2` | `ac2ec` | **identical** |
| 165 | `ac3d6` | `ac2f0` | **identical** |
| 166 | `ac242` | `ac2fc` | `this` spilled (r9/r5 vs r5/r6) + shared tail |
| 168 | `ac3de` | `ac368` | **identical** (orig has the `ac3f6` nop/pool) |
| 169 | `ac3fc` | `ac380` | **identical** |
| 170 | `ac2a6` | `ac384` | `bne.w` vs `bne.n` only |
| 171 | `ac400` | `ac390` | `cbz` vs `cmp+beq` (cbz range: 88 B vs 198 B) |
| 172 | `ac41a` | `ac3ac` | `cbz`/`cbnz` vs `cmp+beq` (range) |
| 184 | `ac2b4` | `ac3f4` | `cmp+bne.w` vs `cbnz` (range) |
| 189 | `ac2ee` | `ac424` | `beq.w` vs `beq.n` only |
| latch | `ac45a` | `ac45a` | **identical** |
| return tails | `ac49a..ac4c2` | same | **identical** |

The `cbz`/`cbnz` vs `cmp+beq` differences are purely `cbz`'s forward-only
±126 B range: the original's bodies sit closer to the shared tails than ours.
So the *only* source-shape mismatches are case 162 and case 163.

## 3. The two source-shape mismatches (each fixable, each −0.1 fuzzy)

### case 162

Original `ac21c..ac240`:

```
ac21c ldr r0,[r5,#0] ; cmp r4,r0 ; bcs.w ac49e
ac224 ... blx Ship::hasCargo
ac23a adds r4,#1
ac23c cmp r0,#0
ac23e bne.n ac21c        ; hasCargo -> loop
ac240 b.n  ac45a         ; else -> latch
```

The landed wave-126 form

```cpp
if (__builtin_expect(!ship->hasCargo(...), 1)) { complete = false; break; }
```

emits `beq.w ac45a; b.n loop` (opposite polarity).  Writing the loop the way
the original reads

```cpp
for (unsigned j = 0; j < ingredients->size(); j++) {
    if (ship->hasCargo((*ingredients)[j], (*quantities)[j])) {
        continue;
    }
    complete = false;
    break;
}
```

emits exactly `cmp r0,#0; bne.n loop; b.n latch` — the case-162 body becomes
**instruction-identical** to the original.  Measured: 1432 B, **71.0 %**
(524 vs 525).  The 0.1 drop is a global-alignment artifact: the changed body
shifts one later instruction.

### case 163

Original `ac3b6..ac3d0`:

```
... cmp.w r3,#4294967295 ; ble.n ac3be (loop) ; b.n ac45a (latch)
```

The landed `__builtin_expect((*field_90)[j] >= 0, 1)` emits
`cmp.w #-1; bgt.w latch; b.n loop`.  Dropping the hint entirely
(`if ((*field_90)[j] >= 0)`) emits `cmp.w #-1; ble.n loop; b.n latch` —
instruction-identical.  Measured: 1432 B, **71.0 %** (524 vs 525).

Applying *both* fixes together shrinks the function to **1428 B** (the two
saved branch words compound), which is fatal for the link layout; a full
re-hint sweep on top of both only recovers 1432 B / 71.0 %.

### `.text` differing bytes vs fuzzy score

The two metrics disagree on the wave-126 hint.  Measured with a raw
`cmp` of the two `.text` sections (`0x69440`, `0x1337c8`):

| variant | fuzzy | `.text` differing bytes |
|---|---:|---:|
| baseline (wave-126 hint) | **71.1** | 85,520 |
| case-162 `continue` form | 71.0 | **85,513** |
| case-162 hint removed (plain `if (!hasCargo)`) | 71.0 | **85,513** |
| case-163 hint removed | 71.0 | 85,519 |

So the wave-126 hint *raises* the fuzzy score by 0.1 but *adds 7 differing
`.text` bytes*.  The `continue` form is the faithful reconstruction (its
case-162 body is instruction-identical) and is 7 bytes closer to the original,
but it does not move the fuzzy score in the right direction.  It was left out
of the landed source so the wave-126 71.1 score is not regressed; it is the
first thing to land if the project ever prioritises `.text` differing bytes
over the fuzzy score for this function.

## 4. Exhaustive hint sweeps (all at 1432 B unless noted)

* All 48 `if`/`for` conditions, `__builtin_expect(cond, 1)` and `(cond, 0)`,
  one at a time on top of the wave-126 baseline: **max 71.1**.
* All 5 landed hints: remove / invert to 0: **max 71.1** (h2 `==0` → 0 is
  code-identical to 1).
* All subsets of removing {h0,h3,h4}: **max 71.1**.
* 90 random hint pairs: **max 71.1**.
* `switch (__builtin_expect(type, K))` for K ∈ {0,8,150,151,152,156..175,
  184,189}: 18.1–31.3 % (and 1360–1456 B).  The switch has 29 case values but
  27 successors, so `handleSwitchExpect`'s `getNumCases()+1` weight vector is
  rejected by `calcMetadataWeights`' `getNumSuccessors()+1` check.

## 5. Root cause (unchanged from wave 126)

`SelectionDAGBuilder::buildJumpTable` adds jump-table successors in ascending
case-value order and `MachineBlockPlacement::selectBestCandidateBlock` drains
the worklist by `MBFI` block frequency, first-on-tie.  With uniform weights
every high body has the same frequency, so the bodies lay out in value order;
the original's A-then-B order needs `A > B` switch-edge weights, and the low
group first needs `low > A`.  That is a PGO/`!prof` profile, not expressible
from C++11 (`__builtin_expect` sets exactly one weight to 2000).  The
`MBP_SORT_SBCB`/`MBP_FORCE_FALLBACK` debug hooks reproduce the order at
1436/1440 B (76.0 / 63.2 %), but neither is a source construct and neither is
1432 B.

## 6. Reproduction

```sh
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6    # RATCHET PASS, byte 4506

# single-function score (fast, ~0.1 s)
python3 /dev/shm/w7627/score.py
# pct=71.1 osize=1432 usize=1432 oinsn=523 uinsn=524

# per-body audit
python3 /dev/shm/w7627/perbody.py

# decode the original TBHs
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
```

Scratch (not committed): `/dev/shm/w7627/` — `orig.dis`/`ours.dis`,
`score.py` (single-symbol verify score), `perbody.py` (TBH-delimited body
diff), `diffblocks.py` (difflib opcodes), `harness.py` (edit/build/score),
`sweep.py`/`greedy3.py`/`greedy4.py`/`pairsearch.py` (hint sweeps),
`status.ll` + `status_llc.o` (custom-LLVM MBP study).
