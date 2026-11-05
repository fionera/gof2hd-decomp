# `Status::missionCompleted` — wave 117: TBH-target LCS re-tune of the rank table (w6513)

Status: **landed**.  `_ZN6Status16missionCompletedEbbx` **91.881566 % → 92.454632 %**
(fuzzy ratio, official verifier), **1432 B** unchanged.  Gate: `RATCHET PASS`,
`byte_exact 4501`, `linked_exact 4501`, imports 207, `lint CLEAN`, `drift 0`,
27 sections, exact file size 2,187,328 B, `.text 0x1337c8`, `.rodata 0x41c27`,
`.ARM.extab 0x26294` (4 B short, unchanged).  Worktree `wt/w6513`, branch
`wave117-w6513`.

## 0. What the rank table can and cannot do

`Status.cpp` switches on `MISSION_CASE[m->getType()]`, where `MISSION_CASE` is the
192-byte `unsigned char` table in `tools/mission_case_table.cpp` at the fixed
address `0x20b977` (mission type → case-label rank).  The emitted code (dispatch,
TBH contents, body layout) depends only on the **case labels written in the
source**; the table only chooses which type reaches which label.  The table bytes
themselves are not compared by the verifier.

clang 7's `MachineBlockPlacement` lays the case bodies out in the jump table's
successor order (`buildJumpTable`'s `Table` = ascending case value), after
`selectBestSuccessor` peels the one body with two case values (167/174, 8.19 %
vs the tied 4.10 %).  So the body layout is the **label order**.  The previous
wave-116 table put the labels in the original body order `L`, which reproduces
the original body addresses exactly; that is worth far more than the table
contents (identity labels, i.e. a perfect table, score only 85.3 %).

Given the layout must stay `L`, the label assignment is a monotone matching:
labels must increase along `L[1..]` (the shared 167/174 body is peeled first by
frequency, so its two labels are free).  The emitted TBH slot `s` points at the
body whose label is `s`; the original's slot `s` points at the body of type
`150+s`.  A perfect table would need `label(t) = t`, which forces value-order
layout, so the table can only be *approximated*.

## 1. The change

The verifier scores with `difflib` (LCS) over the normalised instruction stream,
so the right objective for the 40 TBH halfwords is the **LCS against the
original's table**, not the number of positionally-equal slots.  A hill-climb /
random search over monotone label assignments (constraint: the range must stay
`0..39` so the TBH is still 40 entries and the dispatch still needs no `sub`)
found a unique optimum with LCS **29/40** (wave-116: 26/40):

```
body   150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165 168 169 171 172
label    0   1   2   3   4   5   6   9  10  11  12  13  14  15  16  17  18  19  20  22  23  39
shared 167/174 -> labels 21 and 25
low group 0/11/8/13/14 -> 200/211/208/213/214 (unchanged)
default -> 250
```

Emitted TBH targets (function-relative):

```
1d8 1de 1e4 218 21e 236 248 522 522 2a4 30a 36a 378 3b2 3f6 420 454 482
49e 4a2 4aa  fe 4c4 4c8 522  fe 522 522 522 522 522 522 522 522 522 522
522 522 522 4e2
```

The first six slots (`150..155`) now match the original exactly; the shared body
sits in slots 21/25 (original: 17/24) and the default runs stay at 23, 26..38.
Body addresses are unchanged from wave 116 (the layout is still `L`).

## 2. Dead ends measured this wave (all reverted)

| variant | pct | usize |
|---|---:|---:|
| wave-116 baseline (labels 0..22,34,39; shared 0/24) | 91.881566 | 1432 |
| **this pass (monotone LCS-29 label set)** | **92.454632** | **1432** |
| per-slot-optimal label set (24 positional matches, LCS 23) | not landed | 1432 |
| shared labels 20/24, bodies 0..19,21,39 (LCS 24) | not landed | 1432 |
| body 166: re-query `getProductionGoodIndex()` (natural source) | 75.619048 | 1440 |
| body 166: re-query + drop the body-168 `(unsigned short)` | 75.619048 | 1440 |
| body 166: `amount`/`index` swapped | 89.971347 | 1432 |
| body 166: hoist `ship->getCargo()` into a local | 72.206304 | 1432 |
| body 168: drop the `(unsigned short)` size device | 74.689589 | 1432 |
| plain `switch (m->getType())`, L source order | 58.396947 | 1424 |
| plain `switch (m->getType())`, L source order + body-166 re-query | 59.372027 | 1432 |
| labels shifted to 150..189 (dispatch gains `sub #150`) | 91.3085 | 1432 |

The plain switch is the faithful source shape (the w4003 note proves the body
layout is source order when MBP takes the `getFirstUnplacedBlock` fallback), but
the stock compiler never takes that fallback here: the worklist is non-empty and
`selectBestCandidateBlock` walks it in jump-table (value) order.  With a plain
switch the bodies come out in value order and the function loses ~34 fuzzy
points even though the TBH is then value-ordered and the dispatch is exact.

Shifting the labels to 150..189 makes the dispatch `ldr/ldrb; sub #150; cmp #39`
(4 B longer than the original's `sub #150; cmp #39`), which shifts every body by
+4 and costs more than the dispatch instructions gain.

## 3. Remaining deltas (unchanged from wave 116 except the table)

- Dispatch: ours `ldr/ldrb/cmp #39` (rank indirection) vs the original's
  `sub #150/cmp #39` — 3 vs 2 instructions, same 6 B.
- Low group: `sub #208/cmp #6` vs `sub #8/cmp #6`; outer default `cmp #200`
  vs `cmp #0`.  The low-group labels cannot become 8..14 without colliding with
  the high group's 0..39 range.
- Body 166 is 4 B short (index reused instead of re-queried); body 168 carries
  one extra `uxth`; body 189 is 8 B long (the `MISSION_CASE` + GOT literal pool
  lands inside it).  These three cancel so the loop latch stays at `0x522`.
  Re-querying costs +8 B and shifts every later body, which is why the natural
  source scores far lower.
- Body 166's register allocation: ours keeps `index` in r5 and moves `this`
  (r6) aside for `amount`; the original keeps `index` in r9, `amount` in r5 and
  `this` in r6.  Not reachable from the tested source shapes.

## 4. Reproduction

```sh
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4501
cmake --build cmake-build-match --target drift -j6     # 0 fields
cmake --build cmake-build-match --target lint  -j6     # CLEAN
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN6Status16missionCompletedEbbx
```

Scratch: `/dev/shm/w6513/` (`search_table.py`, `diffnorm.py`, `dumpobj.py`,
`measure.sh`, `layout.py`, the LLVM 7 trace build).

## 5. Files

- `src/game/mission/Status.cpp` — case labels renumbered (shared body 21/25,
  high group 0..20/22/23/39); comments updated.
- `tools/mission_case_table.cpp` — rank permutation above (192-byte table,
  same size and address).
