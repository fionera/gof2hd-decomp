# `Status::missionCompleted` — wave 188 (w13218): ordinary-C++ rank-table forms
# measured; the fixed-address table is still the only gate-clean win

Worktree `wt/w13218`, branch `wave188-w13218`, base master `ce16f45f1`
(wave 187).  Target `_ZN6Status16missionCompletedEbbx`
(`src/game/mission/Status.cpp`, 1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13218`.

**Status: landed.**  Raw **721 -> 449** (aligned 327 -> 160), size
1,432/1,432, `.rel.plt` byte-identical, `.ARM.extab` size/base exact
(156,312 B @ 0x1a3b78, 639 differing bytes = base), byte_exact
4507/4524, linked_exact 4507/4524, data 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370.  `.text` differing bytes 74,223 ->
**73,951**; total differing bytes 74,882 -> **74,804**.  The landed
form is the wave-185 rank table (separate last-linked TU, fixed
`0x20b977` address); the ordinary-C++ forms were built and measured
this wave and are all worse (see §2).

## 1. What the rank table does

The original lays the 23 switch bodies out in
`167/174, 150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172` order — two sorted runs, the
signature of two merged switches.  clang 7's `MachineBlockPlacement`
follows the jump table's value-sorted successor list, so a plain
`switch (m->getType())` emits them ascending (raw 721).  Switching on a
192-byte type -> rank table makes the ascending rank order equal the
original body order; the function stays 1,432 B and `.rel.plt` stays
byte-identical.  The table is the wave-178 implementation verbatim
(`tools/mission_case_table.cpp`, ranks 200..214 for types 0/8/11/13/14
and 0..39 for 150..189), linked after every archive so it is the last
`.rodata` input (lands at `0x20b977`, the first byte past the original
269,159-byte `.rodata` image; the gap to `.fini_array` at `0x20cba0`
absorbs it, file size stays 2,187,328 B).

## 2. Ordinary-C++ forms measured this wave (all size 1,432)

| form | raw | .rel.plt | byte_exact | .rodata | total diff |
|---|---:|---|---:|---|---:|
| base (plain type switch) | 721 | identical | 4507 | exact | 74,882 |
| **landed: last-linked TU + fixed `0x20b977`** | **449** | **identical** | **4507** | **269,351 (+192)** | **74,804** |
| `static const unsigned char kMissionCase[192]` in `Status.cpp` | 475 | identical | **4426** | 269,351 (+192) | 74,828 |
| `extern const` hidden table in last-linked TU, real symbol reference | 475 | **51 entries re-indexed** | 4507 | 269,351 (+192) | 80,541 |
| same, rank 38 for type 172 (table + `case 38:`) | 975 | identical | 4507 | 269,351 | 75,328 |

### 2a. `static const` table inside `Status.cpp` — raw 475, but 81 byte_exact lost

This is the form the task suggested.  It compiles to the same dispatch
shape (`ldr r0,[pc,#imm]; add r0,pc; ldrb r0,[r0,r1]`, 2 B longer than
the fixed-address `ldr`), so the body block shifts +2 and raw is 475,
not 449.  The table is emitted in the *middle* of `.rodata` (Status.cpp
is the 112th of 140 TUs), so every `.rodata` byte after it moves +192.
81 otherwise byte-exact functions carry literal-pool words holding
absolute `.rodata` addresses; their pool words change and the ratchet's
`byte_exact` drops **4507 -> 4426** (`linked_exact` stays 4507 — they
are structurally identical, `pct=100.0 linked=True`).  The ratchet is a
hard `byte_exact` gate, so this form cannot land.

### 2b. Real hidden symbol, last-linked TU — raw 475, but `.dynsym` reorders

`extern const unsigned char kMissionCase[192]` defined in the last-linked
TU under `#pragma GCC visibility push(hidden)` and referenced from
`Status.cpp` (same pragma) removes the fixed address.  It still costs
the +2 PIC `add r0,pc` (raw 475).  More importantly the new *undefined*
symbol in `Status.cpp.o` changes gold's symbol-table insertion order:
the `.dynsym` **set** is unchanged (5,577 entries, same names) but its
**order** permutes, so `.dynsym`/`.dynstr`/`.hash`/`.gnu.version`/
`.rel.dyn` and 51 `.rel.plt` `r_info` symbol indices change
(`r_offset` and target symbol are identical).  Total differing bytes
grow 74,882 -> 80,541, i.e. a net **loss** of 5,659 B for a 246-byte
raw win.  Adding 1..4 extra referenced hidden symbols does not change
the 102-byte `.rel.plt` delta, and the symbol name does not matter
(tried `kMissionCase`, `aa`, `zz`, `QQQQ`, ...), so it is the reference
itself, not a bucket collision.  Not landed: `.rel.plt` must stay
byte-identical.

### 2c. Rank-38 variant

The wave-186 note's "rank 38 for type 172" was measured against the
*static* table form.  With the fixed-address table, changing the table
row to 38 **and** the switch label to `case 38:` gives raw **975**
(worse); leaving the label at `case 39:` while the table says 38 gives
raw 449 but sends type 172 to `default` (a semantic bug).  The landed
table therefore keeps 172 -> 39.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13218
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
# -> raw=449 aligned=160 size=1432/1432 insn=529/527
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `Status_hard39.cpp` / `mct_rank39.cpp` (landed),
`Status_static38.cpp` (2a), `Status_hidden38.cpp` / `ArrayInst_hidden.cpp`
(2b), `Status_hard38.cpp` / `Status_hard38b.cpp` (2c), `tryname.py`,
`trydummy.py`, `mv.sh`.

## 4. Files changed

* `src/game/mission/Status.cpp` — rank-table body.
* `tools/mission_case_table.cpp` — the 192-byte rank table (new).
* `CMakeLists.txt` — `mission_table` OBJECT library linked after libgcc.
* `docs/IDENTICALITY.md` — wave-188 state.
* this note.
