# Radar::draw wave 151 (w9702) — the 9,908 String shape is copy-init + `int asteroidHit`; the -16 B tail shrink must be pad-based

**Status:** analysis note, no source change landed (gate green at HEAD `db9658a51`).
Worktree `wt/w9702`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave149-w9605-tail-veneers.md`,
`radar-draw-wave149-w9501.md`, `radar-draw-wave148-w9401.md`.

Baseline (HEAD, unchanged): original 9,908 B; ours 9,908 B; raw same-address diff
**8,739**; instruction-index alignment **1,483**; `.text` 86,863 differing bytes;
`.ARM.extab` 156,312 (size exact); `.rodata` 0x1c9e10; `.rel.plt` byte-identical.

## 1. The 9,908 String shape on the *current* HEAD is copy-init + `int asteroidHit`

w9605's "copy-init + `int wreck` (enemy loop)" was measured on the wave-149 base,
before w9606 landed `int wreck`. On wave-150 HEAD `int wreck` is already present,
so the +16 B companion is now **`int asteroidHit`** (w9606 had landed
`bool asteroidHit` as the -16 B compensation). Measured on HEAD:

| variant | usize | raw | align | .ARM.extab | .rodata |
|---|---:|---:|---:|---:|---:|
| HEAD (baseline) | 9908 | 8739 | 1483 | 156312 | 0x1c9e10 |
| copy-init only | 9892 | 8735 | 1631 | 156324 | 0x1c9e20 |
| **copy-init + `int asteroidHit`** | **9908** | **8636** | **1788** | **156324** | **0x1c9e20** |
| copy-init + `int enemyHit` | 9912 | 8676 | 1787 | 156324 | 0x1c9e20 |
| copy-init + `int planetHit` | 9928 | 8426 | 2199 | 156324 | 0x1c9e20 |
| assign + `int asteroidHit` | 9912 | — | — | 156324 | 0x1c9e20 |

The exact source edit for the 9,908 shape (ready to land once the tail shrink exists):

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

## 2. The 9,892 shape is NOT shift-free — the -16 B is absorbed before `__cxa_demangle`

The copy-only shape is 9,892 B but `.text` stays **0x1337c8** (unchanged) and the
stub table start stays 0x19ae98. The -16 B shrink is absorbed by the alignment gap
before `__cxa_demangle` (0x194030 is unchanged); every function between
`Radar::draw` and that gap shifts **-16** (e.g. `Radar::calcDistance`
0x130704 -> 0x1306f4, zlib/libzip/openssl all -16). Their `bl`/`b.w` to the fixed
PLT change by 16, so `byte_exact` collapses to **3,291** (measured). Manually
patching the pad -16 to put `.rodata` back at 0x1c9e10 leaves `byte_exact` at
**3,291** — the loss is the function shift, not `.rodata`. So the 9,892 shape is
unlandable; only the size-exact 9,908 shape keeps every function address.

## 3. Veneer-removal cost is not zero (new measurement)

Removing a veneer shifts every later veneer in the table by -16 (the table is in
first-call-site order; 34 inversions of 466). Every `b.w` to a shifted veneer
changes its displacement, so its caller's bytes change.

* Middle veneer: retarget `Item::isInList(Item*, ItemArray*)` to the 3-arg form
  (w9605's experiment) removes the veneer for 0x63948 -> `byte_exact 4507 -> 2488`,
  `linked_exact 4507 -> 4506`, `.text` +16 (measured).
* Last veneer (`_ZNSt9exceptionD2Ev`, table slot 466 at 0x19cbb8, callers
  `_ZNSt8bad_castD2Ev` / `_ZNSt10bad_typeidD2Ev`, both 4-byte `b.w`) is the only
  shift-free removal; it still costs **2 `byte_exact`** because those two callers'
  `b.w` target must change.
* Minimum cost over all 467 veneers is 2 (computed from the byte-exact caller set).

So the -16 B "veneer lever" cannot keep `byte_exact` at 4507 unless the two
4-byte libc++abi destructors are changed too (they are vendored, not `src/`).

## 4. The 0x40 pad is fixed; the -16 B tail shrink must come from the pad

The tail is `__aeabi_idiv0` (12 B) + 467 veneers (7,472 B) + a **fixed 0x40 zero
pad**. Adding one veneer (dummy tail call to `__android_log_print`) keeps the pad
at 0x40; removing one (Item retarget) keeps it at 0x40. Gold 2.27's
`Arm_input_section::set_final_data_size` gives the stub owner
`align(12, stub_addralign=4) + 467*16 = 0x1D3C`, ending at 0x19cbc8; the next
input section (`crtend_so.o` .text, size 0) is placed at 0x19cc08 anyway, i.e. the
0x40 gap is output-section layout padding, not the stub table. A source-level way
to make that pad 0x30 would be a shift-free -16 B lever that costs zero
`byte_exact`; it was not found in the time box.

## 5. Gate (unchanged, note-only)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 6. Open questions

1. Make the 0x40 output-section pad 0x30 (shift-free, zero `byte_exact` cost).
   Candidate: the pad is the layout gap before `crtend_so.o`; a zero-size `.text`
   input section with a matching alignment may move it.
2. Or make the two 4-byte libc++abi destructors (`bad_cast`/`bad_typeid`) not
   tail-call `std::exception::~exception`, so the last veneer can go for 0 cost.
3. w9701's -16 B lever was not committed at the time of this note; the merge
   order (String shape first vs lever first) could not be tested. The String
   shape alone is `byte_exact 4044` (`.rodata` 0x1c9e20); the lever must be
   applied on top of the 9,908 shape, not the 9,892 one.
