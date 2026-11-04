# `Status::missionCompleted` — wave 103: the order is the whole gap; no landable
# source-only fix (wf303)

Status: research note (no source change landed). Worktree `wt/wf303`, branch
`wave103-wf303`, base master `1d2b4398e`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 `r328903`, LLVM `1d739ffb…`, `-Oz`, image base 0x10000; real VMA =
Ghidra − 0x10000). Target: `_ZN6Status16missionCompletedEbbx`
(`Status::missionCompleted(bool, bool, long long)`), original `0xabf38`,
size **1432 (0x598)**, committed fuzzy **60.4 %**.

This pass used the new wave-103 third-party Ghidra oracle
(`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/Status.c`,
clang-3.8 `-O2` mod rebuild) as a body-shape oracle, and a locally built
assertions LLVM 7.0.0 `llc` to quantify the layout gap. **No source change
improves the fuzzy score without breaking the gate** (byte_exact 4500,
size-exact 4524, `.text`/`.rodata`/file size exact).

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

Target row: `pct 60.4`, `osize 1432`, `usize 1432`, `byte False`,
`linked False`, `ninsn 523/527`. `.text 0x1337c8`, `.rodata 0x41b67`,
lint `CLEAN`, drift 0.

## 1. The oracle is a body-shape oracle only (its TBH is also value order)

The wave-103 oracle binary (KiritoJPK NDK-r14/clang-3.8 `-O2` rebuild,
`/dev/shm/wf303/oracle_libgof2hdaa.so`, 2,469,656 B) has
`missionCompleted` at `0xa924c`, 1388 B. Its TBH bodies are **ascending
value order**, like ours — it is not an ordering oracle. Its decompiled C
(`game/Status.c` lines 3062–3450) does confirm the body shapes:

* case 162: `if (ingredients->size() == 0) return m;` then
  `while (hasCargo(ing[i], qty[i])) { j++; if (size <= j) return m; }`
* case 163: `if (field_90->size() == 0) return m;` then
  `while ((*field_90)[j] < 0) { j++; if (size <= j) return m; }`
* case 166: no null check on `ship`
* case 168: `status->field_178 >= statusValue`, then `field_178 = 0`
* case 184: `statusValue == 0 && (currentCampaignMission != 0x5c ||
  inAlienOrbit() || station->getIndex() != 0x71)` — the original calls
  `Status::inAlienOrbit()` (`0xac2d2`), matching our source
* case 8: `currentCampaignMission == 0x8f && !atStation`

Applying the oracle 162/163 shapes to the committed source was measured at
**59.6 % / 1444 B** (worse); 156/160 at **60.4 % / 1432 B** (neutral). The
body code is already essentially correct; the gap is layout.

## 2. Quantifying the layout gap: right order = 78.1 %

The committed body order is produced by `MachineBlockPlacement::
selectBestCandidateBlock` walking `BlockWorkList`, which
`markBlockSuccessors` fills in `MBB->successors()` order. For the high jump
table that is `buildJumpTable`'s `Table` order = **ascending case value**.
The original's bodies are in **source order**.

A local assertions LLVM 7.0.0 `llc` (`/dev/shm/wf303/llvm-build`) reproduces
the committed build **exactly** (60.4 % / 1432 B) on the project IR, so it
is a faithful stand-in for the NDK clang for this function. Patching
`selectBestCandidateBlock` to `std::stable_sort` the worklist by
`MBB->getNumber()` (i.e. function/source order) and recompiling the same IR
gives:

| build | pct | size | body order |
|---|---:|---:|---|
| unpatched `llc` (== NDK clang) | 60.4 | 1432 | value order |
| worklist sorted by MBB number | **78.1** | **1436** | **exact original order** |

Decoded TBH order of the sorted build:
`167/174, 150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172, default` — the original's.
The remaining 78.1 → 100 gap is body-shape/regalloc/tail-merge drift
(the sorted build is 4 B larger, 1436 not 1432). **The layout alone is
worth ~18 fuzzy points.**

The same order is produced by the w4003 trick of forcing
`selectBestCandidateBlock` to return `nullptr` (MBP `getFirstUnplacedBlock`
fallback walks MBB order). Both are compiler patches; neither is reachable
from source.

## 3. Why the source cannot reach it (measured variants)

Every variant below was compiled with the project's exact flags and linked
with the real gold command (`/dev/shm/wf303/fast2.sh`), then scored with the
official verifier (`tools/verify/verify.py --show`).

| variant | pct | size | body order |
|---|---:|---:|---|
| committed (direct `int` switch, source order, 166 anchor) | **60.4** | 1432 | value order |
| plain `int`, no 166 anchor | 60.2 | 1428 | value order |
| all cases ascending in source | 54.5 | 1380 | value order |
| sequential two-switch, 2× `getType()` | 45.6 | 1460 | 2nd switch collapses |
| sequential two-switch, local `int type` | 52.8 | 1512 | **exact original order** |
| 2-switch local + oracle 162/163 | 51.4 | 1524 | exact |
| nested `default: switch` | 39.1 | 1408 | value order |
| `__builtin_expect(type, 167)` nested | 20.1 | 1408 | peeled |
| `__builtin_expect(type, 150/155)` | 18.1 | 1424 | peeled |
| all bodies `continue` | 19.7 | 1472 | value order |
| `static const unsigned char kMissionCase[192]` rank table, `.rodata` | **70.6** | **1388** | exact |
| same rank table, function-local `static` | 70.6 | 1388 | exact |
| same rank table, `#pragma clang section rodata=".text"` | 70.6 | 1388 | exact |

The **two-switch** form gets the exact original order but pays 76–80 B of
extra body code: the stored `type` stays live across the first switch in a
callee-saved register, pushing `i`/`atStation` down and spilling `this`
(see `status-missioncompleted-mbp-fallback-w4003.md` §4). It scores
*lower* than the wrong-order single switch.

The **rank table** (`switch (kMissionCase[m->getType()])`, case labels =
ranks 0..27) makes the value order equal the source order and gets the
exact original body order, but the table load costs code and the function
comes out **1388 B**, i.e. not size-exact.

## 4. Why the rank table is not landable (byte_exact collapse)

Putting the 192-byte table anywhere in `.rodata` or `.text` shifts later
literal-pool references and collapses `byte_exact`:

| placement | pct | usize | byte_exact |
|---|---:|---:|---:|
| 192-B table in `.rodata` (middle of `Status.cpp`) | 70.6 | 1388 | **2326** |
| 192-B table as a function-local `static` | 70.6 | 1388 | **2326** |
| 192-B table in `.text` (`#pragma clang section`) | 70.6 | 1388 | **2294** |
| committed (no table) | 60.4 | 1432 | 4500 |

Measured with the official `verify.py` after a real link. A table in a
separate section (the wave-102 `.missioncase`) kept `byte_exact` 4500 but
adds a 28th section, which wave 102 removed and wave 103 must not
reintroduce. The table also breaks `size-exact` (1388 ≠ 1432), the file
size, and `.text`/`.rodata` sizes. **No table placement is landable.**

## 5. New observation on the MBP tie-break (not yet exploitable)

Instrumenting `selectBestCandidateBlock` with the worklist size and
`IsEHPad` shows the difference between the two big switches in this TU:

* `missionCompleted` (switch inside the `for` loop): worklist `ehpad=0`,
  so the tie-break keeps the **first** max in successor (value) order.
* `nextCampaignMission` (switch outside any loop, byte-exact, 8492 B): the
  body worklist is `ehpad=1`, so the tie-break keeps the **last** max, which
  reverses the worklist and yields source order.

So the original layout of `missionCompleted` would be reproduced if its
body worklist were treated the same way as `nextCampaignMission`'s, or if
the jump-table successor list were source-ordered. Neither is reachable
from a plain single-switch source: the successor list is
`buildJumpTable`'s `Table` (value-sorted, `SelectionDAGBuilder.cpp`
"Add successors. Note: use table order for determinism."), and the `ehpad`
flag comes from the block, not the source.

## 6. Conclusion — proven vs open

**Proven (command-backed):**

1. The committed build's prologue, both dispatches, the low-case bodies and
   the first ~215 normalized instructions are already correct; the first
   structural difference is the high TBH's 40 halfword entries (body
   order), then the high bodies.
2. The original's high bodies are in source order; ours are in ascending
   value order (`buildJumpTable` `Table` order + MBP first-max tie-break).
3. Fixing only the order (patched `llc`: worklist sorted by MBB number)
   raises the fuzzy score 60.4 → **78.1** and reproduces the original body
   order exactly; it is 1436 B, not 1432.
4. The sequential two-switch form reproduces the order in real source but
   scores 52.8 % / 1512 B because of the extra `type` liveness.
5. The rank table reproduces the order and scores 70.6 % / 1388 B, but any
   placement collapses `byte_exact` (4500 → 2294–2326) and breaks
   size-exactness, `.text`/`.rodata` and file size.
6. The wave-103 oracle is ascending-order like ours; it confirms body
   shapes (162/163/166/168/184/8) but not the layout.

**Open:** a plain single-switch source shape that makes the jump-table
successor list source-ordered (or makes the body worklist behave like
`nextCampaignMission`'s `ehpad=1` list) while keeping the direct
`sub r1,r0,#150 / cmp #39 / tbh` dispatch, the 1432-byte size and no new
data. With the order fixed, the residual 78.1 → 100 is body-shape/tail-merge
drift, not dispatch.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf303
cmake --build cmake-build-match --target check -j8      # RATCHET PASS, byte 4500

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump $OD --show _ZN6Status16missionCompletedEbbx

# body order of the current build (decode both TBH tables)
python3 /dev/shm/wf303/bodies2.py "$PWD/cmake-build-match/libgof2hdaa.so"

# right-order experiment (local assertions LLVM 7.0.0)
#   patch selectBestCandidateBlock to stable_sort WorkList by MBB->getNumber()
#   ninja -C /dev/shm/wf303/llvm-build llc
#   clang -S -emit-llvm (project flags) -> sed 's/, variables: ![0-9]*//g'
#   llc -mtriple=thumbv7-none-linux-androideabi -mattr=+neon -float-abi=soft \
#       -filetype=obj -O2 -relocation-model=pic
#   replace CMakeFiles/gof2.dir/src/game/mission/Status.cpp.obj, run the gold link
```

Scratch (not committed): `/dev/shm/wf303/` — `Status.baseline.cpp`,
`fast2.sh` (compile+link+score one variant), `score1.py`, `bodies2.py`,
`analyze.py` (difflib matching blocks), `sbcb.sh` (instrumented MBP trace),
`llcpipeline.sh` (clang-IR → patched-`llc` → gold link), `S_rank*.cpp`,
`S_2sw_local.cpp`, `S_seq.cpp`, `llvm-7.0.0.src/`, `llvm-build/`.

## 8. Follow-up experiments (same worker, after the first note)

All measurements below use the project's exact NDK r18b compile command and the
real gold link (`/dev/shm/wf303/fast2.sh`), scored with the official verifier.
The committed source was restored and `check` re-run green after every row.

### 8.1 Rank table in the last TU (the 4th placement)

The rank table placed in `ArrayInstantiations.cpp` (the last TU, `extern const`
in `Status.cpp`) is the best-scoring variant measured so far, but still not
landable:

| variant | pct | usize | byte_exact | .text | .rodata |
|---|---:|---:|---:|---|---|
| rank table in last TU | **70.6** | **1388** | **1378** | 0x1337a8 | 0x41c27 |
| committed (no table) | 60.4 | 1432 | 4500 | 0x1337c8 | 0x41b67 |

The 44-byte shrink (1388 vs 1432) and the 192-byte `.rodata` growth shift
every later function's literal-pool reference, so `byte_exact` collapses
4500 -> 1378 even though the table is in the last TU.

### 8.2 Nested-switch source shapes (the oracle's shape) are worse

The wave-103 oracle's `Status.c` puts the low switch (8..14/0) in the
`default:` of the high switch. Reproducing that shape in C++ (outer = high,
inner = low; outer = low, inner = high; oracle ascending with explicit empty
cases; a 3-level A/B/low nesting) changes the *dispatch*, not just the layout:

| variant | pct | usize | dispatch |
|---|---:|---:|---|
| outer = high (current order), default = low | 27.1 | 1380 | only the 7-entry low `tbh`; high switch becomes comparisons |
| outer = low, default = high | 27.0 | 1424 | high switch lowered differently |
| outer = `{167,8,0,13,14}`, default = high | 26.9 | 1436 | high switch lowered differently |
| oracle shape (outer 150..189 ascending with explicit empty cases, default = low) | 43.5 | 1436 | 40-entry high `tbh` at `0xabfde`, 15-entry low `tbh` |
| 3-level A / B / low | 19.0 | 1396 | two `tbh` tables (21 + 17 entries) |

Only a *flat single switch* reproduces the original's `sub/cmp/bhi/tbh`
dispatch (40-entry high table + 7-entry low table + `cmp r0,#0`). Nesting is
not viable.

### 8.3 Explicit empty cases, `default:` position, `__builtin_expect`

| variant | pct | usize | body order |
|---|---:|---:|---|
| all 40 high values explicit (`case 159: ... break;` etc.) | 60.4 | 1432 | value order (unchanged) |
| `default: break;` at the start of the switch | 60.4 | 1432 | value order |
| `default: break;` at the end of the switch | 60.4 | 1432 | value order |
| `switch (__builtin_expect(m->getType(), 150))` | 22.8 | 1388 | dispatch changed |
| `switch (__builtin_expect(m->getType(), 167))` | 27.2 | 1392 | dispatch changed |
| `switch (__builtin_expect(m->getType(), 189))` | 24.3 | 1424 | dispatch changed |

### 8.4 Source order does not change the physical body order

Three different source orders were measured; the physical high-body order is
always ascending value order (`150,151,152,153,154,155,156,157,158,160,...`):

| source order | pct | usize | physical order |
|---|---:|---:|---|
| group A then group B (committed) | 60.4 | 1432 | value order |
| ascending 150..189 | 54.5 | 1380 | value order |
| reverse 172..150 | 51.0 | 1432 | value order |

Source order only changes the *size* (tail merging / shared tails), never the
layout. The original's group A / group B physical split
(`150,151,152,153,154,155,158,162,166,170,184,189, 156,157,160,163,164,165,168,169,171,172`)
is therefore not a source-order effect.

### 8.5 Why `nextCampaignMission` is byte-exact but `missionCompleted` is not

Instrumenting `selectBestCandidateBlock` (worklist + per-block MBFI frequency)
shows the difference between the two switches:

* `missionCompleted`'s high jump table: every case-body chain head has the
  **same** frequency `0.1864530226`, and the worklist is in `successors()`
  (value) order. The first-max tie-break therefore lays the bodies out in
  value order.
* `nextCampaignMission` (byte-exact, 8492 B): the case-body frequencies differ
  in the 8th decimal place (`0.006289307384` vs `0.006289314881`); the
  first-max tie-break then picks the MBB order, which is the source order.
  Its worklist is hash order (`fillWorkLists`), not value order.

So the original `missionCompleted` must have had *some* frequency difference
(or a different worklist order) that favoured the group A bodies. Nothing in
the current source produces one: all 24 high case bodies are direct successors
of the same `tbh` block with equal edge probability, so MBFI gives them
identical frequencies. This is the concrete remaining gap.

### 8.6 Updated conclusion

The rank table in the last TU is the best fuzzy score (70.6 %) but is not
landable (`byte_exact` 4500 -> 1378, size 1388). Nested switches, explicit
empty cases, `default:` position and `__builtin_expect` all either change the
dispatch or leave the value-order layout intact. The committed direct switch
(60.4 %, 1432 B, size-exact, 27 sections) remains the best landable source.
The residual is an MBP tie-break that needs a frequency difference the current
source cannot express.
