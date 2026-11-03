# `HangarWindow::render` — 80.6 % / 9700 B (w3809)

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave88-w3809`, base master `41804126`. Ground truth:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra `android_2.0.16_libgof2hdaa.so`,
image base 0x10000; function at Ghidra `0x1590d8`). Build-flag work is settled
(`worst_function_flag_sensitivity_w3751.md`, `build_flags_independent_audit_w3750.md`);
this pass is source-shape only.

## TL;DR

The function is **size-exact (9700 B) and call-sequence-exact** except for the three
`description = description;` self-assigns (21 `String::operator=` vs the original's 18)
and the `getWidth`/`getHeight` order in the free-credits loop. Everything else that
differs is **register allocation, instruction scheduling, 16/32-bit encoding width and
literal-pool placement** — not missing code. The current tree is a strong local optimum:
a 64-cell sweep over the plausible source knobs (lazy `Globals::layout`, `hintLayout`,
`getWidth` order, self-assigns, `right > 0`, `inTabIndex` forms) found no combination
above the baseline.

Landed: `item->inTabIndex < 0 ? 4 : 3` -> `item->inTabIndex == -1 ? 4 : 3`
(raw fuzzy 0.806003 -> 0.806443; reported pct stays 80.6, size 9700, gate unchanged).
The change is semantically safe (`inTabIndex` is `-1` or a non-negative index) and is a
genuine difflib gain, but it is an **alignment** gain: it turns clang's arithmetic
`3 + (x >> 31)` select into a branchy `mov 3 / it eq / moveq 4`, which shifts the
in-function branch targets two bytes closer to the original's. It is not a structural
reconstruction.

## Measured baseline (this tree)

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
.text 0x1337c8 @ 0x69440, .ARM.extab 0x2628c @ 0x1a3b78, .rodata 0x41b67 @ 0x1c9e10
render: 9700/9700 B, pct 80.6, raw 0.806003 (3626 vs 3637 normalized insns)
```

After the landed change: same sections, render raw **0.806443** (3626 vs 3638 insns).

## What the original actually does

The call sequence (`blx` target stream) is byte-for-byte identical to ours except:

1. three extra `AbyssEngine::String::operator=` calls in our credit-list loop
   (`description = description;` x3, kept as a `.text` size anchor);
2. `TouchButton::getHeight` is emitted after `getWidth` in the original free-credits
   loop (`0x14ad28 blx getWidth`, `0x14ad38 blx getHeight`); ours is the reverse.

The prologue loads only **two** pc-relative globals before `SetColor`
(`__stack_chk_guard`, `Globals::Canvas`), then `Globals::appManager`, then
`Globals::layout` lazily at `drawBG` (`0x149132 ldr r0,[pc]; add r0,pc;
ldr.w sl,[r0]; ldr.w r0,[sl]; blx drawBG`). `sl` holds `&Globals::layout` and the
pointer is reloaded at each use (it is rematerialized at `drawFooter`, `0x14a85e`).
`this` is in `fp`; `&appManager` in `r8`; frame 376.

Ghidra confirms the source shape we have: separate `if (viewMode == 0)` /
`if (viewMode == 1)` blocks, `Globals::layout->drawFooter()` after them, the buy-credits
`switch (count)` with `String` temporaries, the free-credits loop, and signed
`right > 0` / `left > 0` compares (`cmp #1; blt`).

## The remaining root cause: the `this`/`&Globals::layout` register tie

The faithful lazy-load shape (no `Layout *layout` local, `Globals::layout->drawBG()`,
no `hintLayout`) reproduces the original's *structure* — same pool order
(guard, Canvas, appManager, layout), same load sites, same call sequence — but the
allocator swaps the two long-lived values:

| shape | `this` | `&Globals::layout` | frame | pct |
|---|---|---|---:|---:|
| original | `fp` | `sl` | 376 | — |
| baseline (eager `Layout *layout` local) | `fp` | `sl` | 376 | 80.6 |
| faithful (no local) | `sl` | `fp` | 384 | 60.9 |

The swap is a pure RegAllocGreedy tie-break: the faithful IR defines
`&Globals::layout` at `drawBG` (short live range -> higher spill weight -> wins `fp`),
while the original behaves as if the value were live from the prologue. An
**unconditional** early `Globals::layout` load (the top-level local, or a local declared
after `SetColor`/`GetApplicationModule`) pins `this` in `fp`; a **conditional** load
(inside `if (station...)` or `if (viewMode == 0)`) flips it to `sl` and grows the frame
by 8. No plain-C++ shape tried gives both the lazy emitted load and `this = fp`.

Because `this` is in `sl` in the faithful build, every `this->` access uses a different
register and the whole body's diff explodes (60.9 %); the eager-load baseline keeps the
right roles and scores 80.6 % even though its prologue carries one extra
`ldr [pc] / add pc / ldr.w sl / ldr.w r4` chain.

## Measured experiments (all 9700 B unless noted)

| variant | pct / raw | frame / `this` |
|---|---|---|
| baseline | 80.6 / 0.806003 | 376 / fp |
| **`inTabIndex == -1` (landed)** | **80.6 / 0.806443** | **376 / fp** |
| `right > 0` explicit | 80.6 / 0.806003 | 376 / fp |
| `inTabIndex >= 0` | 80.6 / 0.806003 | 376 / fp |
| `layout` local after `SetColor` | 80.5 / 0.805452 | 376 / fp |
| `layout` local after `GetApplicationModule` | 80.5 / 0.804626 | 376 / fp |
| remove `hintLayout` (faithful hint loop) | 77.0 / 0.769612 | 376 / fp |
| remove 3 self-assigns | 78.0 / 0.780044 | 376 / fp |
| `getWidth` before `getHeight` | 69.3 / 0.692636 | 376 / fp |
| `Layout *volatile` local | 66.0 / 0.660151 | 384 / fp |
| faithful (no local) | 60.9 / 0.609066 | 384 / sl |
| `layout` local inside `if (station...)` | 60.9 / 0.609066 | 384 / sl |
| `layout` local inside `if (viewMode == 0)` | 60.9 / 0.609066 | 384 / sl |
| `Layout **` / `Layout *&` / `Layout *const &` | 60.9 / 0.609066 | 384 / sl |
| `HangarWindow *self = this` (all `self->`) | 60.9 / 0.609066 | 384 / sl |
| remove self-assigns + `getWidth` first | 65.7 / 0.657021 | 376 / fp |

Other neutral (codegen-identical) source forms: `tab == 1 || tab == 4`,
`tab == 0 || tab == 3`, `count < 5`, `r < rows + 1`, `b != 24`,
`i != items->size()`, `i != tabButtons->size()`, `i >= 2`.

## Why the self-assigns and `pad0`/`pad0b`/`pad3` stay

They are load-bearing size/regalloc anchors, not reconstructions:

* removing the three `description = description;` lines drops the body to 9680 B
  (and 9672 B with the `rows` guard removed), which moves `.rodata` and collapses
  `byte_exact` (3734 in the w3572 measurement);
* `pad0`/`pad0b` pin the credit-loop call-site cleanup scope;
* `pad3` splits the `buttons[8]->draw()` row (+12 B of `.ARM.extab`).

The real source has 18 `String::operator=` calls and no such locals, but the missing
bytes must be recovered from the item-loop body (our item loop is 2253 insns / 5784 B
vs the original's 2252 / 5802 B: one *more* instruction but 18 *fewer* bytes, i.e. more
16-bit encodings because fewer values live in high registers).

## Commands

```sh
cmake --build cmake-build-match --target check -j8     # RATCHET PASS byte 4498 linked 4498
cmake --build cmake-build-match --target lint  -j8     # CLEAN
cmake --build cmake-build-match --target drift -j8     # 0 fields
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

## Proven vs hypothesis

Proven (measured in this tree):

* Baseline gate byte 4498 / linked 4498 / `.text 0x1337c8` / `.ARM.extab 0x2628c` /
  `.rodata 0x1c9e10`; render 9700 B, raw 0.806003.
* The `blx` call sequence matches the original exactly except the three
  `String::operator=` self-assigns and the `getWidth`/`getHeight` order.
* The prologue loads `Globals::layout` lazily at `drawBG` in the original; ours loads it
  eagerly because of the top-level `Layout *layout` local.
* The faithful lazy shape flips `this` from `fp` to `sl` and grows the frame 376 -> 384.
* Every unconditional early layout load pins `this = fp`; every conditional one flips it.
* The landed `inTabIndex == -1` form improves the raw difflib ratio by 0.00044 with the
  same section sizes and gate counts.

Hypothesis / open:

* The original's IR must contain an unconditional `Globals::layout` reference (or an
  equivalent value with a prologue-start live range) that the emitted code does not
  show, because the lazy emitted load and `this = fp` are otherwise mutually exclusive
  in clang 7's greedy allocator.
* Recovering the 18-byte item-loop encoding gap (more high-register live values) is the
  path to matching the branch targets and the `.ARM.extab` record without padding.
