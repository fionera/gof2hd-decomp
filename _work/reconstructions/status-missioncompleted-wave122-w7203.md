# `Status::missionCompleted` — wave 122 (w7203): branch-probability hints
# recover 11.6 points without the rank table

Status: **source change landed**.  Worktree `wt/w7203`, base master
`29cd275f6` (wave-121 docs).  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b
clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | before (HEAD `29cd275f6`) | after (this pass) |
|---|---:|---:|
| function score | **59.4 %** | **71.0 %** |
| function size | 1432 B | 1432 B |
| `.text` size/base | 0x1337c8 @ 0x69440 | 0x1337c8 @ 0x69440 |
| `.rodata` | byte-identical | byte-identical |
| gate | byte 4506 / linked 4506 | **byte 4506 / linked 4506** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

## 1. What changed

Four `__builtin_expect(..., 1)` hints on the `if` conditions of case bodies
166, 184, 157 and 163 (each with the project's `// lint: arm_builtin`
waiver).  No control-flow, no data, no header, no table change:

```cpp
case 166:
    if (__builtin_expect(atStation && station->getIndex() == m->getTargetStation(), 1)) { // lint: arm_builtin
...
case 184:
    if (__builtin_expect(m->getStatusValue() == 0, 1)) { // lint: arm_builtin
...
case 157: {
    ...
    if (__builtin_expect((*equipment)[j] != 0 && (*equipment)[j]->getType() == m->getStatusValue(), 1)) { // lint: arm_builtin
...
case 163: {
    ...
    if (__builtin_expect((*field_90)[j] >= 0, 1)) { // lint: arm_builtin
```

The hints change the *branch probabilities* of those four in-body branches,
which changes the block layout / branch direction / branch width of the
bodies.  They do **not** change the switch dispatch, the TBH, or the case
body order (which is still clang's value order).

## 2. Why this helps even though the body order is still wrong

`report.json` row before: `pct 59.4, osize 1432, usize 1432`.
After: `pct 71.0, osize 1432, usize 1432`.  The function size is unchanged,
so every following `.text` symbol stays at its original address and the gate
is unchanged (byte 4506 / linked 4506, `.rodata` still byte-identical).

The score is `difflib.SequenceMatcher` over the normalized instruction
stream.  The prologue, loop header, high/low dispatch and the low group
(`8, 11, 13, 14`) already match byte-for-byte; the residual is the A/B body
interleaving.  The four hints move the in-body branch targets and widen/narrow
branches so that more of the A/B body instructions land in the LCS.  The
structural-diff count drops 368 -> 333.

The minimal set is exactly `{166, 184, 157, 163}` (a 2^7 subset sweep of the
seven candidate conditions found 71.0 only for this set and supersets; the
other three — 156, 160, 164 — are neutral).  Adding any *other* condition
from the remaining 25 bodies either leaves 71.0 or regresses (e.g. 167 ->
28.4, 8a -> 49.7, 150..155 -> 67.9, 152 -> 33.3, 158a -> 36.4, 162b -> 45.5,
170 -> 59.1, 168 -> 58.3, 169 -> 50.3, 171 -> 46.2, 172a -> 46.2, all at
1432-1480 B).

## 3. Body-order sweep (the original A-then-B order is still not reachable)

All variants below were built with the project's exact flags and scored with
the official single-symbol verifier.  The original body order is
`167/174, 8, 11, 13, 14, A(150 151 152 153 154 155 158 162 166 170 184 189),
B(156 157 160 163 164 165 168 169 171 172), default`; our plain switch emits
value order (`... 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172
184 189, default`).

| variant | usize | pct | body order |
|---|---:|---:|---|
| **plain switch + 4 hints (landed)** | **1432** | **71.0** | value |
| plain switch, no hints (before) | 1432 | 59.4 | value |
| plain switch + all 7 hints | 1432 | 71.0 | value |
| two sequential switches, cached `int` | 1508-1512 | 51.0-52.6 | **original** |
| two sequential switches + hints | 1560 | 42.8 | original |
| two switches, every 2-way group split (16 combos) | 1508-1752 | 20.5-51.0 | original/partial |
| duplicate A labels (A freq 2/40) | 1376 | 45.8 | A-then-B-ish |
| duplicate A labels + hints | 1376 | 43.2 | A-then-B-ish |
| full-A fallthrough | 1388 | 49.7 | **original** |
| full-A fallthrough + hints | 1388 | 45.2 | original |
| if-chain over all 29 bodies | 1448 | -- | scattered |
| `switch (__builtin_expect(m->getType(), N))` | 625-643 | -- | default before A |
| nested `default:` inner switch | 1408 | -- | B merged into default |
| switch A then `if`+switch B | 1536 | -- | B merged |
| `do{}while(0)`, `if(1)`, nested `for`, `default: break;` first, cached `int` | 1432 | -- | value |

The A-then-B order is reproducible (two switches / duplicate labels /
fallthrough) but every shape that reproduces it also changes the function
size (1376-1512 B) or the register allocation (`m` moves out of `sl`,
`this` out of `r6`), and the size/register shift costs more in the fuzzy
score than the order gains.  The plain switch keeps the original
prologue/dispatch/register allocation and is the best scoring shape.

## 4. Remaining root cause

`SelectionDAGBuilder::buildJumpTable` adds the jump-table successors in
`Table` (ascending value) order, so `MachineBlockPlacement`'s worklist is
value-ordered; the original's A-then-B layout is the
`getFirstUnplacedBlock` (MBB/source order) fallback, which is only reached
when the worklist is empty.  With this compiler the worklist has 23 entries
at the high TBH, so the fallback is never taken from a plain switch.  The
only source-level levers are unequal switch-edge frequencies (duplicate
labels / `!prof`) or a fallthrough chain, and both change the body code or
the size.  See `status-missioncompleted-wave119-w6943.md` and
`status-missioncompleted-wave121-w7113.md`.

## 5. Reproduction

```sh
cmake --build cmake-build-match --target check -j6    # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6    # 0 fields
cmake --build cmake-build-match --target lint  -j6    # CLEAN
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN6Status16missionCompletedEbbx
```

Scratch (not committed): `/dev/shm/w7203/` — `cc.sh` (compile one variant),
`order.py` (TBH body-order decoder), `onescore.py` (single-symbol verifier),
`cand.sh` (copy variant -> link_so -> score), `expsearch.py`/`combo.py`
(hint sweeps), `split.py` (two-switch group splits), `localtest.sh`
(NDK clang -> local LLVM 7.0.0 `llc` -> order).
