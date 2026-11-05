# `Status::missionCompleted` — wave 119 (w6796): rank-table TBH assignment is
# already optimal; low-address / duplicate-label plain-switch shapes are dead ends

Status: **research note (no source change landed)**.  Worktree `wt/w6796`,
branch `wave119-w6796`, base master `075f49c72`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 r328903, LLVM `1d739ffb…`, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.  Baseline row:
**92.5 % / 1432 B** (byte False, linked False).  Gate after this pass:
`RATCHET PASS`, `byte_exact 4505`, `linked_exact 4505`, imports 207,
`lint CLEAN`, `drift 0`, 27 sections, exact file size 2,187,328 B.

This note closes the two remaining "remove the rank table" avenues suggested by
the wave-119 handoff:

1. the 40-slot TBH rank assignment is already optimal (a 400 000-sample
   hill-climb over every valid assignment with min rank 0 / max rank 39 finds
   nothing better than the committed one), and
2. both plain-switch shapes that make clang 7 emit the original A++B body
   order — a low-address table (no literal-pool word) and the duplicate-label
   frequency trick — lose more in body size/address alignment than they gain.

No 1432 B variant beat 92.5 %.

## 0. Baseline

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 92.5`,
`osize 1432`, `usize 1432`, `byte False`, `linked False`.

## 1. Corrected original body layout (TBH base is tbh+4, offsets are halfwords)

The original big TBH is at `0xabfe2`; base `0xabfe6`; 40 halfword entries
(`target = 0xabfe6 + 2*entry`).  Decoding it gives the body layout by address:

```
0xac036  167/174 (shared)
0xac046  big-switch default = small dispatch
0xac052  small TBH table (7 entries, base 0xac052, base type 8)
0xac060  8
0xac0be  type-0 check (`cmp r0,#0`)
0xac0c4  11/0 (shared)
0xac0ce  13
0xac0dc  14
0xac110  150  0xac116 151  0xac11c 152  0xac150 153  0xac156 154  0xac16e 155
0xac172  shared counter tail (150/151/153/155)
0xac180  158  0xac1dc 162  0xac242 166  0xac2a6 170  0xac2b4 184  0xac2ee 189
0xac32a  156  0xac354 157  0xac388 160  0xac3b6 163  0xac3d2 164  0xac3d6 165
0xac3de  168  0xac3fc 169  0xac400 171  0xac41a 172
0xac45a  loop latch / default
```

So the big bodies are two ascending runs
`A = {150,151,152,153,154,155,158,162,166,170,184,189}` then
`B = {156,157,160,163,164,165,168,169,171,172}`, with the small bodies
`8,11,13,14` before A and `167/174` hoisted to the front by its 2× case
frequency.  The small TBH is `sub.w r1,r0,#8; cmp r1,#6; tbh` with the
type-0 check `cmp r0,#0` in its default.

The committed rank table reproduces this layout exactly but (a) its dispatch
is `ldr r1,[pc,#804]; ldrb r0,[r0,r1]; cmp r0,#39; bhi; tbh` (no `sub #150`),
(b) the small dispatch is `sub #208` and the type-0 check is `cmp r0,#200`,
and (c) its 192 B table leaves `.rodata` exactly +192 B and its table-address
literal-pool word lands at `0xac300`, inside body 189.

Per-body size deltas of the committed rank variant vs the original (all other
bodies are size-exact):

| body | original | committed | delta |
|---|---:|---:|---:|
| 166 | 100 | 96 | −4 |
| 189 | 60 | 68 | +8 |
| 168 | 30 | 26 | −4 |

`166 −4` shifts 170/184/189 to −4; `189 +8` then shifts 156..168 to +4;
`168 −4` brings 169/171/172/default back to exact.

## 2. The 40-slot rank assignment is optimal

The TBH slot `k` of the committed variant points to the body with rank `k`,
while original slot `k` points to type `150+k`.  Since the body layout is the
ascending rank order (with 167/174 hoisted), the assignment is constrained:
the 22 non-shared bodies must get strictly increasing ranks in A++B order,
167/174 must keep two ranks (to stay 2× and hoisted), and the rank range must
stay `0..39` (a case at 39 is needed to keep the 40-entry table and hence the
1432 B size).

I scored the TBH halfword sequence with `difflib.SequenceMatcher` (the same
metric `verify.score` uses) and hill-climbed all valid assignments
(400 000 moves, min rank 0, max rank 39, 167/174 two ranks).  The committed
assignment is a fixed point:

```
ranks (A++B): 150→0 151→1 152→2 153→3 154→4 155→5 158→6 162→9 166→10
              170→11 184→12 189→13 156→14 157→15 160→16 163→17 164→18
              165→19 168→20 169→22 171→23 172→39;  167/174→21,25
TBH ratio 0.725 (18 positional matches); no tested assignment exceeded it.
```

Compressing the A ranks to 0..11 and moving B to 12..21 raises the positional
match count (up to 24) but *lowers* the sequence ratio to 0.700, because the
default runs no longer align.  So 92.5 % is a hard ceiling for the rank-table
approach.

## 3. Low-address table: removes the pool word, but shifts bodies

If the table address is small enough for the 12-bit `ldrb` immediate
(e.g. `#define MISSION_CASE ((const unsigned char *) 0x80)`), clang emits
`ldrb.w r0,[r0,#128]` (4 B, same size as `ldr;ldrb`) and no table-address
literal-pool word.  Body 189 then shrinks to the original 60 B and body 168
absorbs the `0x001640ee` pool word (30 B).  But the function drops to
**1428 B** and the body addresses move: 150..166 exact, 170/184/189 at −4,
156..168 at −4, 169 at −8, 171/172/default at −4.  Measured **74.6 %**.
Adding the body-166 re-query (+12 in this shape) gives **1440 B / 75.8 %**,
not 1432.

The address is also not a real table address (0x80 is the ELF header), so this
is only a codegen probe, not a landable source shape.

## 4. Duplicate-label frequency trick: wrong small-body placement, bodies shrink

Adding a second case label (a default-slot type) to each A body gives it 2×
frequency, and adding one/two extra labels to 167/174 gives it 3×/4×.  With
`switch (m->getType())` (real types, so the dispatch is the exact
`sub #150; cmp #39; tbh`) the big bodies do come out A-then-B, but:

* the small bodies 11/13/14 move *after* A (the extra labels lower the big
  switch's default probability, so the small switch is no longer placed
  before A), and
* the bodies shrink: 166 is 92 B (not 100), 152 is 50 B (not 52), etc.

Measured **1368 B / 46.9 %** for both the 3× and 4× variants.  Not landable.

## 5. Other measured variants (this session)

All compile+link+verify-target only (`/dev/shm/w6796/measure.py`), same
project flags:

| variant | usize | pct | note |
|---|---:|---:|---|
| committed rank table | 1432 | **92.5** | baseline |
| local `const unsigned char *mc = (…)0x20b977` | 1432 | 92.5 | byte-identical to baseline |
| `Status.nocast` (drop `(unsigned short)`) | 1432 | 74.9 | loses `uxth`, bodies shift −2 |
| `Status.rq` (body-166 re-query) | 1440 | 75.8 | +8, all later bodies shift |
| low-address table (0x80) | 1428 | 74.6 | pool word gone; body 189 exact |
| low-address + re-query | 1440 | 75.8 | |
| compressed A ranks (0..11) / B (12..21) | 1400 | 68.4 | table shrinks to 23 entries |
| compressed A ranks + 172→39 | 1432 | 67.2 | loses 167/174 2× hoist (single rank) |
| duplicate-label 2× A + 4× 167/174 | 1368 | 46.9 | small bodies after A |
| duplicate-label 2× A + 3× 167/174 | 1368 | 46.9 | same |

Prior-wave measurements remain valid: plain real-type switch 1424 / 58.4;
`real_rq` 1432 / 59.4; `nr2` (rank == type) 1436 / 75.2; forced
`MBP_FORCE_FALLBACK` 1432 / 64.2–66.1 (not landable); two-switch cached
1504 / 47.1.

## 6. Conclusion — proven vs open

**Proven (built and measured):**

1. The original body layout is A++B with 167/174 hoisted; the committed rank
   table reproduces it exactly.
2. The committed 40-slot rank assignment is optimal for the TBH sequence
   ratio (0.725) under min=0/max=39 and the two-rank 167/174 constraint.
3. Removing the table-address pool word via a 12-bit `ldrb` immediate makes
   body 189 exact but costs 4 B of total size and shifts every body after 166;
   it scores 74.6 %.
4. The duplicate-label frequency trick reproduces A-before-B with the real
   type switch (exact dispatch) but misplaces the small bodies and shrinks
   the bodies; it scores 46.9 %.
5. No 1432 B variant beats 92.5 %.

**Open:** a source shape that makes `MachineBlockPlacement` take
`getFirstUnplacedBlock` (empty worklist) at the jump-table block while keeping
the real type in `r0`; that would delete the rank table, make `.rodata` exact
and give the exact `sub #150` dispatch.  The worklist is non-empty for any
plain switch because every case body has exactly one predecessor (the switch
block); the original must have had a CFG where each body had a second
unscheduled predecessor, or a compiler state we cannot reproduce from source.

## 7. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4505
```

Scratch (not committed): `/dev/shm/w6796/` — `measure.py` (link + verify
target only, restores the baseline `.so`), `cc.sh` (project-flag compile),
`bodyorder.py` (TBH decoder), `genrank.py` (rank-table/source generator),
`tbhopt.py` / `tbhsearch2.py` (TBH assignment search), `Status.mvaddr.cpp`,
`Status.lowaddr.cpp`, `Status.lowrq.cpp`, `Status.dupA.cpp`,
`Status.dupB.cpp`, `cand2_Status.cpp`, `cand3_Status.cpp`,
`cand2_table.cpp`, `cand3_table.cpp`.
