# Radar::draw wave 151 (w9702) — the 0x40 tail pad is Android gold's `stub_group_auto_padding`; pad=0x30 + String shape is byte_exact 4507

**Status:** analysis note, no source change landed (gate green at HEAD `3aac48a93`).
Worktree `wt/w9702`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi`.

Read with `radar-draw-wave151-w9702.md`, `radar-draw-wave149-w9605-tail-veneers.md`,
`radar-draw-wave149-w9501.md`.

Baseline (HEAD): original 9,908 B; ours 9,908 B; raw **8,739**; aligned **1,483**;
`.text` 0x1337c8; `.ARM.extab` 0x26298 (156,312); `.rodata` 0x1c9e10; byte_exact 4507.

## 1. The 0x40 pad is not layout padding — it is Android gold's smart padding

The NDK r18b gold is **not** vanilla binutils 2.27: it carries an Android patch
`stub_group_auto_padding` in `gold/arm.cc` (`Stub_table::padding_`).  Vanilla
2.27 links the same objects to `.text` 0x133788 with crtend exactly at the end of
the veneer table (no pad).  The Android gold adds `padding_` to the stub table's
data size:

```
if (size > prev_data_size_) {
  delta = size - prev_data_size_;
  if (delta <= padding_) padding_ -= delta;
  else if (delta >= 0x50000) padding_ = 0x250;
  else if (delta >= 0x30000) padding_ = 0x150;
  else if (delta >= 0x10000) padding_ = 0x100;
  else if (delta >= 0x500)   padding_ = min(delta*0.02, 0x40);
}
else if (size < prev_data_size_) padding_ += delta;
```

`Arm_input_section::set_final_data_size` then uses
`align(original_size, stub_addralign) + stub_data_size` where
`stub_data_size = prev_data_size_ + padding_`.

Measured with a rebuilt vanilla 2.27 gold + this patch (prints instrumented):
the **first** relaxation pass already has `size = 7472` (467 veneers), `prev = 0`,
so `padding_ = min(7472*0.02, 0x40) = 0x40`.  The stub area is therefore
`7472 + 64 = 7536 = 0x1D70`, and the owner data_size is `0xc + 0x1D70 = 0x1D7C`.
This reproduces the Android gold exactly (`.text` 0x1337c8, crtend 0x19cc08).

**Consequence:** the pad is a function of the *first-pass* veneer count, not of
the final one.  Removing a veneer reduces `size` by 16 and the pad stays 0x40
(`min(7456*0.02,0x40)=0x40`), so `.text` shrinks by exactly 16 — the w9605
"veneer lever" is real.  But it cannot make the pad 0x30: that needs
`delta <= 0x960` (2,400 B ≈ 150 veneers).

## 2. The pad can be forced to 0x30 — and then the String shape is free

Rebuilding the same gold with the `0x40` cap changed to `0x30` and linking the
String shape gives:

| | .text | .ARM.exidx | .ARM.extab | .rodata | byte_exact |
|---|---:|---:|---:|---:|---:|
| HEAD baseline | 0x1337c8 | 0x19cc08 | 0x26298 | 0x1c9e10 | 4507 |
| String shape (pad 0x40) | 0x1337c8 | 0x19cc08 | 0x262a4 | 0x1c9e20 | 4044 |
| **String shape (pad 0x30)** | **0x1337b8** | **0x19cbf8** | **0x262a4** | **0x1c9e10** | **4507** |

So with a real 16 B shift-free tail shrink the String shape keeps `byte_exact`
and `linked_exact` at **4507** (only `Radar::draw` is non-exact; every other
function keeps its address and its `.rodata` base).  The section sizes are
`.text` −16 and `.ARM.extab` +12, so this is not a section-size-exact landing.

Exact String-shape edit (landmark `else` arm, ~line 476, plus the asteroid loop):

```diff
-                        Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
-                                                    this->screenY + rowHeight, false);
+                        String labelCopy2 = this->lockLabel;
+                        Globals::Canvas->DrawString(radarFont(), labelCopy2, this->screenX + offset,
+                                                    this->screenY + rowHeight, false);
...
-            bool asteroidHit = false;
+            int asteroidHit = false;
```

Measured: usize 9,908; raw **8,636**; aligned **1,788**; extab 156,324.

## 3. Why no source-level pad=0x30 lever was found

The first-pass stub table is 7472 B (467 veneers), fixed by the number of
distinct Thumb→ARM `b.w` tail-call targets.  The only ways to change `delta`
are (a) remove a veneer (−16, but costs `byte_exact`), or (b) reduce the count
to ~150, which is not a local source change.  Vanilla-gold pass trace:

```
STUBTABLE update size=7472 reloc=7472 prev=0 padding=64
STUBTABLE set_final cur=7536
AIS owner orig=12 addralign=4 stubdata=7536
STUBTABLE update size=7472 reloc=7472 prev=7472 padding=64
```

## 4. Veneer-removal cost (recomputed against the real non-exact set)

For veneer index k, cost = number of **byte-exact** functions that reference
veneer k..end (the callers of k need a new target; callers of k+1..end shift).
Minimum over all 467 veneers is **2**, at the last veneer
(`_ZNSt9exceptionD2Ev`, 0x19cbb8, callers `_ZNSt8bad_castD1/D2Ev` and
`_ZNSt10bad_typeidD1/D2Ev`, all 4-byte `b.w`).  The alias rows are separate in
`report.json`, so the real row cost may be 4, not 2.  Next cheapest: idx 465
(4), idx 464 (7).  So a veneer-removal lever is not byte_exact-neutral.

## 5. Open questions

1. A source-level way to make the first relaxation pass emit 462–464 veneers and
   the final pass 467 (then `padding_` shrinks by 48–64 and `.text` shrinks by
   48–64, shift-free).  Not found.
2. Or a 12 B `.ARM.extab` reduction elsewhere with zero code change (String
   shape's Radar LSDA is 144 B / 21 entries vs the original 132 B / 19).
3. Or accept `.text` −16 / `.ARM.extab` +12 with pad=0x30 (byte_exact 4507).

## 6. Gate (unchanged, note-only)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```
