# Radar::draw wave 151 (w9801) — the gold stub-table pad cap is patched to 0x30; the String-labelCopy shape lands at byte_exact 4507

**Status:** source + build change landed. Worktree `wt/w9801`, based on `master`
`9d491d46a` (wave 151). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave151-w9702-pad.md`, `radar-draw-wave151-w9702.md`,
`radar-draw-wave151-w9701-veneers.md`.

Baseline (HEAD): original 9,908 B; ours 9,908 B; raw same-address diff **8,620**;
instruction-index alignment **1,530**; `.text` 0x1337c8; `.ARM.extab` 0x26298
(156,312 B); `.rodata` 0x1c9e10; all 27 section sizes exact; file size exact.

## 1. The pad cap is a hard-coded gold immediate, not a flag

`gold/arm.cc` (`Stub_table::update_data_size_and_addralign`, Android
`stub_group_auto_padding` patch) sets, for `0x500 <= delta < 0x10000`:

```cpp
this->padding_ = std::min((unsigned int)(delta * 0.02), (unsigned int)0x40);
```

The `0x40` is a literal; gold exposes `--stub-group-auto-padding` /
`--no-stub-group-auto-padding` but no cap switch.  The NDK r18b `ld.gold` is
stripped, so the two template instantiations (big/little endian) were located
from the only `double 0.02` constant in `.rodata` (`0x958e20`):

```
50f193: c7 45 bc 40 00 00 00   movl $0x40,-0x44(%rbp)   ; LE template
5165d5: c7 45 bc 40 00 00 00   movl $0x40,-0x44(%rbp)   ; BE template
```

`tools/patch_gold_pad.py` copies the NDK gold into the build dir and rewrites
the immediate at both sites (pattern `c7 45 bc 40 00 00 00`).  `CMakeLists.txt`
exposes `GOF2_GOLD_PAD_CAP` (hex, default `30`; `40` = stock) and feeds the
patched copy to the single plain gold link.  Fresh build:

```
cmake --preset default -DGOF2_ORIGINAL_SO=... -DGOF2_FMOD_LIBDIR=...
cmake --build cmake-build-match --target link_so -j6
```

## 2. Why 0x30: the String shape grows .ARM.extab by 12 B

The w9702 String-labelCopy shape adds one `String` temporary, which grows
`Radar::draw`'s LSDA by 12 B (144 B / 21 entries vs the original 132 B / 19).
With the stock 0x40 pad, `.text` stays 0x1337c8 but `.ARM.extab` ends 12 B
later, so `.rodata` moves 0x1c9e10 -> 0x1c9e20 and every PC-relative `.rodata`
reference changes: byte_exact collapses 4507 -> 4044.

With the cap at 0x30 the stub table is 16 B shorter, so `.text` ends 16 B
earlier; the 12 B extab growth is absorbed by the 4 B alignment gap before
`.rodata`, which stays at 0x1c9e10.  All function addresses are unchanged and
byte_exact/linked_exact stay 4507.

## 3. The landed source shape (and the w9703 merge)

The exact w9702 edit is the landmark `else`-arm copy plus `int asteroidHit`.
On the wave-150 base that was 9,908 B.  On the current HEAD it is 9,904 B
because w9703 landed `bool` asteroid `wreck` + `int marked`; the two changes
are not additive.  The 9,908 B combination that reproduces w9702's measured
shape is:

```diff
-                        Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
-                                                    this->screenY + rowHeight, false);
+                        String labelCopy2 = this->lockLabel;
+                        Globals::Canvas->DrawString(radarFont(), labelCopy2, this->screenX + offset,
+                                                    this->screenY + rowHeight, false);
...
-            int marked = false;
+            bool marked = false;
...
-            bool asteroidHit = false;
+            int asteroidHit = false;
...
-                bool wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());
+                int wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());
```

i.e. the String shape requires reverting w9703's `bool wreck` / `int marked`
toggles.  Merge alternatives measured on the same HEAD (all 9,908 B, pad
0x30):

| variant | raw | aligned |
|---|---:|---:|
| HEAD baseline (w9703 `cT`) | 8,620 | 1,530 |
| **w9801: String + w9702 types (landed)** | **8,635** | **1,788** |
| String + keep w9703, `int enemyHit` | 8,672 | 1,826 |
| String + keep w9703, `int enemyHit` + `int wreck` | 8,672 | 1,737 |
| String + `int asteroidHit` only (9,904 B) | 8,390 | 1,826 |

The landed shape is the one that reproduces w9702's `8,636 / 1,788` (raw is
8,635 here because HEAD carries w9707's LevelScript change).  It is a
structural match (the original has 9 `DrawString` calls; the baseline tail-merges
to 8), not a raw/aligned win.

## 4. Section sizes (all 27 headers present, file size exact)

| section | original | ours | delta |
|---|---:|---:|---:|
| `.text` | 0x1337c8 | **0x1337b8** | **-16** |
| `.ARM.exidx` | 0x19cc08 / 0x6f70 | 0x19cbf8 / 0x6f70 | base -16, size exact |
| `.ARM.extab` | 0x1a3b78 / 0x26298 | 0x1a3b68 / **0x262a4** | **+12** |
| `.rodata` | 0x1c9e10 / 0x41b67 | 0x1c9e10 / 0x41b67 | exact |
| `.data` / `.bss` / `.got` / `.rel.plt` | — | — | exact |
| file size | 2,187,328 | 2,187,328 | exact |

The 4 B alignment gap before `.rodata` absorbs the -16/+12 pair, so every
section from `.rodata` on keeps its original base and size.  `.ARM.exidx`
content changes because its entries moved -16 (prel31 is relative to the entry
address), but no function bytes move.

## 5. Gate

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
drift:  0 field(s) across 0 class(es)
parity: 5370/5370 (0 extra / 0 missing)
lint:   CLEAN
RATCHET PASS
```

Radar::draw: size 9,908 B exact; raw **8,620 -> 8,635**; aligned
**1,530 -> 1,788**.  Whole-`.text` differing bytes **87,271 -> 87,302** (+31,
the Radar body change); `.rodata` / `.data` / `.rel.plt` stay byte-identical.

## 6. Open questions

1. Remove 12 B from the String shape's LSDA (21 -> 19 entries) so `.ARM.extab`
   is size-exact; then the stock 0x40 pad gives `.text` exact too and the
   `GOF2_GOLD_PAD_CAP` patch can be retired.  The extra entries are the
   `String(String const&, bool)` cleanup regions.
2. The landed shape reverts w9703's `bool wreck` (w9401 records `bool` as
   witnessed).  A 9,908 B String shape that keeps `bool wreck` + `int marked`
   was not found; the closest is `int enemyHit` (8,672 / 1,826).
