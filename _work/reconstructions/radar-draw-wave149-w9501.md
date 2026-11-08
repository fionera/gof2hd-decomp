# Radar::draw wave 149 (w9501) — the String-labelCopy 9,908 shape and the 12 B extab/rdata blocker

**Status:** analysis note, no source change landed (gate stays green at HEAD `fece77a64`).
Worktree `wt/w9501`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave148-w9401.md`, `radar-draw-wave147-w9402.md`,
`radar-draw-wave147-w9309.md`.

Baseline (HEAD, unchanged): original 9908 B; ours 9908 B; raw same-address diff
**8,712**; instruction-index alignment **1,657**. `.ARM.extab` 156,312 (size exact),
`.rodata` 0x1c9e10 (byte-identical), `.text` 86,225 differing bytes.

## 1. The String-labelCopy shape, re-measured on HEAD

`Radar.final.cpp` = HEAD + `String labelCopy2 = this->lockLabel;` in the landmark
`else` arm (the w9401 shape, with the same bool→int widenings):

| variant | usize | raw | align | .ARM.extab | .rodata |
|---|---:|---:|---:|---:|---:|
| HEAD | 9908 | 8712 | 1657 | 156312 | 0x1c9e10 |
| **`String labelCopy2 = this->lockLabel;`** | **9908** | **8549** | **2103** | **156324 (+12)** | **0x1c9e20 (+16)** |
| `String labelCopy2; labelCopy2 = this->lockLabel;` | 9908 | **8429** | **1998** | 156324 | 0x1c9e20 |
| `String labelCopy2(this->lockLabel);` | 9908 | 8549 | 2103 | 156324 | 0x1c9e20 |
| `const String labelCopy2 = this->lockLabel;` | 9908 | 8549 | 2103 | 156324 | 0x1c9e20 |

The default+assign form is strictly better than the w9401 shape on both metrics
(raw 8429 vs 8549, aligned 1998 vs 2103) but carries the same +12 extab.

## 2. References do NOT reproduce the allocation

`String &`, `const String &`, `String *`, `const String *` all give
**9884 / 9095 / 2001 / 156312** — i.e. exactly the no-temporary base. The copy
constructor is what forces `&this->lockLabel` into a register; a reference is
folded away. (This is a new falsification: the lever is the copy ctor's side
effect on the allocator, not the address-taking.)

Scoping variants (all measured):

| variant | usize | raw | align | extab |
|---|---:|---:|---:|---:|
| `{ String c = lockLabel; (void)c; }` then draw member | 9936 | 9456 | 2005 | **156312** |
| copy at top of `if (i==0)` arm, draw member | 9952 | 9515 | 2270 | 156336 |
| copy before `if (i<=1)`, draw member | 9968 | 9497 | 2310 | 156344 |
| copy in outer scope, draw copy | 9916 | 9056 | 2104 | 156324 |

The only extab-neutral form (`{...}` before the DrawString) is +28 B, so it is not
a 9908 shape.

## 3. `.ARM.extab` LSDA anatomy (new, parsed)

Record for Radar in the original: **132 B, 19 call-site entries**; ours (String
shape): **144 B, 21 entries**. Both have a 20-byte header (personality prel31 +
`c9 53 97 02 f0 80 00 85 b0 b0 b0 ab ff ff 01 <len>`), then the call-site table.
Each entry is a 4-tuple of ULEB128s `(start, len, landing_pad, 0)`.

Original table 112 B / 19 entries; ours 122 B / 21 entries (+10 B) plus 2 B
record padding to 4-byte alignment = +12 B section growth.

Original (11 LP≠0, 8 LP=0):
```
(0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
(2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
(2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
(5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)
```
Ours (12 LP≠0, 9 LP=0):
```
(0,568,0) (568,8,9808) (576,1704,0) (2280,10,9848) (2290,26,0)
(2316,20,9842) (2336,58,0) (2394,10,9844) (2410,16,9850) (2426,16,9832)
(2442,10,9822) (2466,20,9812) (2486,78,0) (2564,10,9810) (2574,1426,0)
(4000,10,9868) (4010,1722,0) (5732,8,9862) (5740,1068,0) (6808,8,9806)
(6816,3092,0)
```
The String copy adds exactly one LP≠0 region and one LP=0 gap (the copy ctor /
`String::operator=` site at `12e938` and the `DrawString` site at `12e964`), so the
record cannot be shrunk by re-scoping without also changing the code shape.

## 4. The 12 B arithmetic (definitive)

`.text` ends at `0x19cc08`; `.ARM.exidx` is fixed 0x6f70; `.ARM.extab` is
156312 (ours) / 156324 (String shape); `.rodata` is 16-aligned.

* String shape: `0x19cc08 + 0x6f70 + 0x262a4 = 0x1c9e1c` → **0x1c9e20** (+16).
* `.text` −12: `0x19cbfc + 0x6f70 + 0x262a4 = 0x1c9e10` → **0x1c9e10** (exact).
* `.text` −12..−28 all land on 0x1c9e10; −32 lands on 0x1c9e00.

So a **net `.text` shrink of 12 B** (or any value in 12..28) restores `.rodata`.

## 5. Why a middle-of-.text shrink does not work (new measurement)

Every function after the shrink point shifts by −12. Its own literal pool moves
with it (fine) and `.rodata` is fixed (fine), but its `bl/blx` offsets to the
**fixed PLT** (0x5e7ec) and to functions before the shrink change by 12, so its
raw bytes differ. The verifier compares each function at its own symbol address,
so this is a byte_exact loss per shifted function.

Confirmed by the w9401 measurements: `HangarWindow::render` −16 (address
0x1490d8, after Radar) gives byte_exact **2367** even though `.rodata` is still
0x1c9e10; MTW −8 + Level −8 gives **2625**. ~1,900–2,100 functions break.

**Consequence:** the only shift-free `.text` shrink is at the very tail of
`.text`. The last exported function is `__cxa_demangle` (0x194030, 0x238 B); after
it come only local libc++abi demangler helpers and libgcc. The last code is
`__aeabi_idiv0` (0x19ae8c, **12 B**, local) followed by a 7,536-byte data blob to
0x19cc08 (byte-identical in both binaries). Our source's last function is
`ArrayReleaseClasses<ParticleSystemSprite*>` at 0x183d74, so no source-level
−12 is reachable at the tail (libgcc is linked last).

## 6. Open questions / next levers

1. **Make Radar's LSDA 19 entries again while keeping the copy-ctor codegen.**
   The extra LP≠0 region is the copy ctor / `operator=` site; if a source shape
   makes that site share the `DrawString` landing pad (9842) the table loses one
   entry. Every re-scoping tried so far changes the code size instead.
2. **Shrink `.ARM.extab` by 12 elsewhere.** Baseline LSDA lengths match the
   original for all functions, so this requires deliberately removing two
   call-site entries from another function (e.g. collapsing a `String` temp's
   cleanup into an adjacent identical range) with zero `.text` change.
3. **Tail shrink.** A 12 B reduction of `__aeabi_idiv0` (libgcc) or of the
   trailing data blob would be shift-free; neither is reachable from `src/`.
4. The default+assign form (8429/1998) is the best raw/aligned String shape; if
   the 12 B is ever found, land that form rather than the w9401 8549/2103 one.

## 7. Gate (unchanged, note-only)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```
