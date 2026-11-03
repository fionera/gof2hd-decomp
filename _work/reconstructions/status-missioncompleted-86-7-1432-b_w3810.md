# `Status::missionCompleted` — rank mechanism / direct-TBH audit (w3810)

Status: research note (no source change landed). Worktree `wt/w3810`, branch
`wave88-w3810`, based on master `41804126`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
`r328903`, LLVM `1d739ffb…`, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).

Topic: the current tree is at **86.7 % / 1432 B** with the `kMissionCase[192]`
type→rank table. Previous notes identify the rank-load literal-pool word and the
40-entry TBH table as the blockers. This pass re-derived the original dispatch
from Ghidra and the ELF, re-tested the rank mapping, and checked whether the
original really uses a direct 40-entry TBH with source-order bodies.

**Answer: yes.** The original is a single `switch (m->getType())` with a direct
40-entry TBH (types 150..189) and a 7-entry TBH (types 8..14), no rank load, and
its case bodies are laid out in source order. The current rank table is a
codegen-shaping device, not original data. The best source-order remap that keeps
the 1432-byte size scores **86.3 %**, slightly below the current 86.7 %; the
current rank mapping is a local optimum under the fixed rank set. No code change
landed.

## 1. Baseline (this worktree)

```sh
cmake --preset default   # GOF2_ORIGINAL_SO/GOF2_FMOD_LIBDIR point at /home/debian/GalaxyOnFire2/_work
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 86.7`,
`osize 1432`, `usize 1432`. `.text 0x1337c8`, `.rodata`/`.data`/`.data.rel.ro`
byte-identical, lint `CLEAN ✓`, drift 0.

## 2. Original dispatch (binary evidence)

`Status::missionCompleted` is at `0xabf38`, size 1432. It calls
`Mission::getType()` exactly once and dispatches with two in-text TBH tables:

```
abfd6: blx  Mission::getType()
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]      @ 40 halfwords at 0xabfe6 (types 150..189)
...
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]      @ 7 halfwords at 0xac052 (types 8..14)
```

There is **no `sxth`, no `ldrb`, and no rank-table load**. Decoding the two
tables (target = table_base + 2*entry) gives the physical body order:

```
167/174 8 11 13 14 150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172  default
```

This is exactly the order of the `case` labels in the current source (the w3570
source order), with `case 0` sharing the `case 11` body.

The key observation: the peeled case is `167/174` (two case values share one
body, so `peelDominantCaseCluster` moves it first). The remaining high bodies are
in **source order**, which is *not* ascending type order. `sortAndRangeify` sorts
the case clusters by value and `buildJumpTable` adds successors in table order,
so a direct switch on the type lays the bodies out in value order (57.7 % /
1428 B, or 1432 B with the `(short)` cast). The original's source-order layout
therefore means its case values were already sorted in source order: the
`Mission::id` values in the original are **ranks assigned in source order**, not
the raw mission types. `getType()` returns that rank, so the switch is direct
and needs no lookup table.

## 3. Current rank table vs the original

The current source maps the mission type to a rank with a 192-byte
`kMissionCase[192]` table and switches on the rank:

```
ldr  r1, [pc, #844]     @ literal-pool word -> kMissionCase
add  r1, pc
ldrb r0, [r1, r0]
sub.w r1, r0, #150
cmp  r1, #32            @ 33-entry table (ranks 150..182)
...
```

So the current function has three extra dispatch instructions (8 B) and one
extra literal-pool word (4 B), and its high table is 33 entries (66 B) instead of
the original's 40 entries (80 B). The body code is otherwise the same set of
blocks; the current rank mapping places all but two of them in the original
order (block 170 is hoisted before block 150, and block 152 before block 151).

## 4. Rank remap experiment

The rank values are a permutation of a 33-value subset of 150..182 (24 used
values plus holes). Reassigning the ranks monotonically in source order (using the
same rank set) makes the physical body order exactly the original's:

```
150/151 8 11 13 14 152 154 155 156 157 159 160 161 162 163 165 166 167
168 169 171 172 173 174 176 177 182  default
```

Measured (full `check` build, same 1432 B, `.text 0x1337c8`, gate PASS):

| tree | rank mapping | body order | pct | gate |
|---|---|---|---:|---|
| current | w3810 base | near source order (2 swaps) | **86.7** | PASS |
| rank3 | monotonic source order | exact source order | 86.3 | PASS |

The exact source-order remap is slightly *worse* on the fuzzy metric because the
table entries are a different permutation of the original's offsets and the
rank load's register pressure changes a few body encodings (e.g. an extra
`cmp r0,#0` in the `hasEquipment` body). A local search over all rank swaps and
all unused-rank replacements (fast object-level score, validated against the
full verifier for the current/rank3 cases) found no mapping that beats 86.7 %
while keeping the 1432-byte size.

## 5. Why a 40-entry table cannot be combined with the rank load

The original's 40-entry table covers 150..189. A rank table that also spans
150..189 (mapping the 24 high type entries onto the original's present type
values) reproduces the body order and the 40-entry table shape, but the function
grows to **1448 B** (rank load 8 B + 40-entry table +14 B over the 33-entry
table), which shifts `.text` and fails the ratchet (`byte_exact 4498 -> 2326`).
The only way to keep the original's 40-entry table and 1432 B is to remove the
rank load, i.e. switch directly on `getType()` and have the `id` values already
be ranks.

## 6. Conclusion

**Proven:**

1. The original `Status::missionCompleted` is a direct `switch (m->getType())`
   with two TBH tables (40 + 7 entries), no `sxth`, no rank load, and 1432 B.
2. Its case bodies are laid out in source order; the peeled `167/174` body is
   first, then the low bodies, then the high bodies in source order.
3. The current `kMissionCase` rank table is a codegen device that forces the
   source-order layout; it adds a 3-instruction rank load and an extra
   literal-pool word, and compresses the high table to 33 entries.
4. A monotonic rank remap gives the exact source-order layout at the same
   1432 B, but scores 86.3 % vs the current 86.7 %; the current mapping is a
   local optimum under the fixed rank set.
5. A 40-entry rank table reproduces the original table shape but grows the
   function to 1448 B and fails the gate.

**Hypothesis (most important remaining root cause):**

* The original's `Mission::id` stores **ranks assigned in source order**, not the
  raw mission types; `getType()` returns that rank. Our reconstruction stores
  types in `id`, so a direct switch cannot reproduce the source-order bodies and
  we need the rank table. Recovering the original `id` semantics (or moving the
  type→rank mapping into the mission-creation code) would remove the rank load
  and let a direct 40-entry TBH match the original. That is a cross-TU change
  (`Generator::createMission` / `Level::createMission` / all type comparisons),
  not a `missionCompleted`-local one.
