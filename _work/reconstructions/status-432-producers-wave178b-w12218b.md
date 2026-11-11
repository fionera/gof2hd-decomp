# Status::missionCompleted — remove the hardcoded rank table (wave 178, w12218b)

Status: **source change landed**.  Worktree `wt/w12218b`, branch
`wave178-w12218b`, base `1168cb87f` (wave 178).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12218b`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN6Status16missionCompletedEbbx` (1,432 B).  The wave-178 base
(commit `1168cb87f`) read the mission-type -> case-rank table through the
hardcoded absolute address `0x20b977` and linked the table from a separate
`tools/mission_case_table.cpp` OBJECT library as the last `.rodata` input.
That is a manual address / manually linked data shortcut.

## 0. Bottom line

The table is removed.  `missionCompleted` is again a plain
`switch (m->getType())` with the case bodies in the original hot-then-cold
order (the wave-177 shape).  The hardcoded address, the extra TU, the
`.rodata` +192 B growth and the 26-of-27 section-header delta are gone.

| state | raw | aligned | size | `.text` diff | `.rodata` size | headers |
|---|---:|---:|---:|---:|---:|---:|
| wave-178 base (hardcoded rank table) | 449 | 160 | 1432 | 78,979 | 269,351 (+192) | 26/27 |
| **this pass (plain switch)** | **721** | **327** | **1432** | **79,251** | **269,159 (exact)** | **27/27** |

The plain switch costs **+272 `.text` bytes** (raw 449 -> 721) because clang
7's `MachineBlockPlacement` follows the jump table's ascending successor list,
so the case bodies come out ascending by mission type instead of the
original's hot-then-cold order.  The rank table was a codegen device that
forced the successor order to the body order.  The trade is deliberate: the
project goal is human-style C++ with no manual addresses, no manual jump
tables and no manually linked data, and the structural win is an exact-size
`.rodata` and all 27 original section headers.

## 1. What changed

* `src/game/mission/Status.cpp`: `missionCompleted` is the wave-177 plain
  `switch (m->getType())` body again (cases 167/174, 8, 0/11, 13, 14, then
  150, 151, 152, 153, 154, 155, 158, 162, 166, 170, 184, 189, 156, 157,
  160, 163, 164, 165, 168, 169, 171, 172).  The `#define MISSION_CASE`
  pointer is gone.
* `tools/mission_case_table.cpp`: deleted.
* `CMakeLists.txt`: the `mission_table` OBJECT library and its link/dep
  entries are gone.  The link is back to the plain `tools/source_order.txt`
  object list with no extra `.rodata` input.

## 2. Gate and structural result

```sh
cd /home/debian/GalaxyOnFire2/wt/w12218b
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
# -> raw=721 aligned=327 size=1432/1432 insn=529/527
cmake --build cmake-build-match --target check -j2
# verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
cmake --build cmake-build-match --target diff -j2   # SYMBOL PARITY 5370/5370
cmake --build cmake-build-match --target lint -j2   # CLEAN
cmake --build cmake-build-match --target drift -j2  # 0 fields
```

Section state (original vs ours):

| section | original | ours | delta |
|---|---:|---:|---:|
| `.text` | 0x1337c8 | 0x1337c8 | size/base exact; 79,251 differing bytes (was 78,979) |
| `.rodata` | 0x41b67 | 0x41b67 | **size/base exact; content byte-identical** |
| `.ARM.exidx` | 0x6f70 | 0x6f70 | byte-identical |
| `.ARM.extab` | 0x26298 | 0x26298 | size/base exact; 731 differing bytes (unchanged) |
| `.rel.plt` | 0x72d0 | 0x72d0 | **byte-identical** |
| `.data` / `.comment` | | | **byte-identical** |
| file size | 2,187,328 | 2,187,328 | exact |

All **27/27** original section headers are now exact.  The only differing
section bytes are `.text` (79,251), `.ARM.extab` (731) and
`.note.gnu.build-id` (20).

### The `.data (2)` / `.comment (4)` report

`/tmp/secdiff.py` prints `.data ... diff 2` and `.comment ... diff 4` for the
wave-178 base.  Those are artifacts of that script: it indexes a section's
file offset with `q[4]` (the LMA) instead of `q[5]` (the file offset).  For
`.data` the LMA is 0x1000 above the file offset, and for `.comment` the file
offset is 0x21596c while the LMA is 0.  Reading the real file offsets shows
`.data` and `.comment` are byte-identical in both the base and this pass.
The only real base deltas were `.text` 78,979, `.ARM.extab` 731,
`.rodata` size +192 and `.note.gnu.build-id` 20.

## 3. Real-table variants tried (and why they lost)

All were measured with a single-TU compile + the real gold link
(`scratch/w12218b/harness.py`).  The table contents are the wave-178 rank
permutation (see §5).

| variant | raw | aligned | size | `.rodata` | notes |
|---|---:|---:|---:|---|---|
| **plain switch (chosen)** | **721** | **327** | **1432** | exact | 27/27 headers; `.dynsym` exact |
| `static const unsigned char MISSION_CASE[192]` in `Status.cpp` | 1,202 | 183 | 1,436 | +192, content shifted | 32,876 `.rodata` bytes shift; `.dynsym` 219, `.data.rel.ro` 118, `.data` 11 |
| same + case-189 declaration swap | 609 | 196 | 1,432 | +192, content shifted | still shifts 32,876 `.rodata` bytes and `.dynsym`/`.data.rel.ro`/`.data` |
| `extern` symbol in the last-linked table TU | 1,202 | 183 | 1,436 | +192, content exact | cross-TU reference reorders `.dynsym`/`.dynstr`/`.hash`/`.rel.plt` |
| same + case-189 declaration swap | 609 | 200 | 1,432 | +192, content exact | `.dynsym` 746, `.dynstr` 4,417, `.hash` 372, `.rel.plt` 102 |

Key findings:

* A real static table in `Status.cpp` is addressed with
  `ldr r1,[pc,#imm]; add r1,pc; ldrb` (8 B) instead of the hardcoded
  pointer's `ldr r1,[pc,#imm]; ldrb` (6 B).  The extra 2 B plus literal-pool
  alignment makes the function 1,436 B, and the table's mid-`.rodata`
  position shifts every later `.rodata` byte (32,876 differing bytes) and
  the exported-object addresses in `.dynsym`.
* A cross-TU symbol reference to the last-linked table TU keeps the
  `.rodata` content exact, but gold adds the referenced symbol to its hash
  table and reorders `.dynsym`/`.dynstr`/`.hash`/`.rel.plt`.  The wave-178
  note already called this out as the reason the base used the fixed address.
* Moving the table TU before the vendored archives (a normal `source_order`
  position) shifts the archives' `.rodata` and costs ~48 KB more `.text`
  differing bytes (127,238 vs 79,139).  The table must be the last `.rodata`
  input to avoid that.
* The plain switch is the only variant that keeps `.rodata`, `.dynsym`,
  `.dynstr`, `.hash`, `.rel.plt`, `.data` and `.comment` all exact.

## 4. Residual and what was tried

The 721 raw bytes are the wave-177 residual: the dispatch head is the
original `sub.w r1,r0,#150; cmp r1,#39; tbh` (4 B) and the 40-entry TBH
entries are in type order, but the case bodies are laid out ascending by
type instead of the original hot-then-cold order.  The wave-177 note already
records the local-order sweep; the rank table was the only way found to
force the body order.

Also tried this pass (all no better than the chosen plain switch):

* Splitting the inner 150..189 switch into a hot switch and a cold switch:
  raw 1,302, size 1,424.
* A hot switch with the cold group in its `default:` (nested switch):
  raw 722, size 1,432; the compiler merged the cold cases back into the
  outer TBH, so the body order stayed ascending.
* `__builtin_expect(mtype, <hot value>)` on the inner switch: 1,312..1,353
  raw and size 1,348..1,424 (the hint suppresses the jump table).
* `__builtin_expect(cond, 1)` on the hot case conditions: raw 1,351,
  size 1,476.

## 5. Rank table (preserved for reference)

The wave-178 table is `mission type -> case rank`; ascending rank is the
original physical body order.  It is not used by the landed source, but is
kept here for any future attempt to express the permutation in ordinary C++
without a fixed address:

```
200, 250, 250, 250, 250, 250, 250, 250, 208, 250, 250, 211, 250, 213, 214, 250,
250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250,
250, 250, 250, 250, 250, 250,   0,   1,   2,   3,   4,   5,  14,  15,   6, 250,
 16, 250,   9,  17,  18,  19,  10,  21,  20,  22,  11,  23,  39, 250,  25, 250,
250, 250, 250, 250, 250, 250, 250, 250,  12, 250, 250, 250, 250,  13, 250, 250,
```

`250` means default.  Rank order 0..23,25,39 maps to types
150,151,152,153,154,155,158,162,166,170,184,189,156,157,160,163,164,165,
168,167,169,171,174,172; the second group maps type 0 -> 200, 8 -> 208,
11 -> 211, 13 -> 213, 14 -> 214.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12218b
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12218b/`: `harness.py`
(single-TU compile + real gold link + `fn_diff`), `plain177.cpp` (the chosen
body), `static.cpp`, `extern.cpp`, `extern_swap.cpp`, `v189A.cpp` (the
real-table variants), `table_ptr.cpp`, `blocks2.py` (rank-block size
comparison).

## 7. Files changed

* `src/game/mission/Status.cpp` — plain `switch (m->getType())`, wave-177
  body order.
* `tools/mission_case_table.cpp` — deleted.
* `CMakeLists.txt` — drop the `mission_table` OBJECT library and link/dep
  entries.
* `docs/IDENTICALITY.md`, `docs/VALIDATION.md` — wave-178b state and the
  raw 449 -> 721 / `.rodata` exact trade-off.
* `_work/reconstructions/status-432-producers-wave178b-w12218b.md` — this
  note.
