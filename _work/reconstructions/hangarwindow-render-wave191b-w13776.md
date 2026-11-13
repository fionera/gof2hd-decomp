# HangarWindow::render — wave 191b (w13776): ship/upgrade order + loop-shape probes, no net win

Worktree `wt/w13776`, branch `wave191b-w13776`, base master `42b002f39`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result: no net-positive win landed; `src/` untouched

Baseline re-measured on the pristine tree (full `link_so` build, `-j2`):

| metric | value |
|---|---:|
| raw same-address diff | **1,013** |
| instruction-index alignment | **479** |
| function size | 9,700 / 9,700 |
| instructions | 3,731 / 3,734 |
| `.text` section diff | **74,582** |
| `.ARM.extab` section diff | **561** |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |
| `.note.gnu.build-id` | 20 |
| total differing bytes (`.text`+extab+20) | **75,163** |

The task prompt's "current section deltas" (`.text` 74,984, extab 564) are stale:
they predate the w13767 free-credits landing.  The fresh numbers above are what
the w13767 commit message reports (`.text` 74,582, extab 561).

LSDA (unchanged from base):
`row 39 O (2508,150,9664) U (2508,172,9664)`; `row 40 O (2696,140,9562)
U (2688,148,9550)`.  Row 39 is already 172 in ours, so the +638 extab penalty
(118 < 128) is not currently active.

## Raw-diff buckets (base, bytes per 0x100 of function address)

`149600:4 149700:55 149800:4 149b00:161 149c00:1 14a800:4 14a900:80 14aa00:7
14ab00:25 14ac00:149 14ad00:217 14ae00:50 14af00:1 14b200:186 14b300:15
14b400:2 14b600:52` = 1,013.

The free-credits loop (`14ac00..14ae00`, 416) and `buttons[17]`
(`14b200`, 186) still dominate; both are the established `this` ↔
`&Globals::layout` RAGreedy queue tie (`ldrd sl,r9` vs `ldrd r9,sl` at
`0x1496da`), which w13740 showed is post-RA and not source-orderable.

## New probes this session (full builds, all size 9,700 unless noted)

`total = .text + extab + 20`.

| variant | raw | aligned | `.text` | extab | total |
|---|---:|---:|---:|---:|---:|
| **base (pristine)** | **1,013** | **479** | **74,582** | **561** | **75,163** |
| buy-list `String description` before `price` (`str`) | 1,015 | **468** | 74,584 | 561 | 75,165 |
| `str` with comma decl (`String description(""), price("");`) | 1,015 | **468** | 74,584 | 561 | 75,165 |
| `str` + swapped case assignments | 1,027 | 502 | 74,596 | 561 | 75,177 |
| free-credits `setVisible→y→setPosition→draw` (`fc_draw_after`) | 1,047 | 475 | 74,616 | 561 | 75,197 |
| ship body `P,V,D` with `upgTab` before `P` (`ship_PVD_pre`) | 1,164 | 462 | 74,733 | 570 | 75,323 |
| ship body `P,D,V` with `upgTab` before `P` (`ship_PDV_pre`) | 1,162 | 472 | 74,731 | 570 | 75,321 |
| ship body `V,P,D` with `upgTab` before `V` (`ship_VPD_pre`) | 1,164 | 474 | 74,733 | 571 | 75,324 |
| ship body `V,D,P` with `upgTab` before `V` (`ship_VDP_pre`) | 1,159 | 484 | 74,728 | 571 | 75,319 |
| ship body `D,V,P` with `upgTab` before `D` (`ship_DVP_pre`) | 1,161 | 484 | 74,730 | 571 | 75,321 |
| ship body `D,P,V` with `upgTab` before `D` (`ship_DPV_pre`) | 1,160 | 472 | 74,729 | 571 | 75,320 |
| upgrade block `buttons[2]->setVisible` before `buttons[10]->setPosition` (`upg_swap`) | 1,160 | **460** | 74,729 | 565 | 75,314 |
| `unsigned int count` + `__builtin_expect(count <= 4,1)` | 1,013 | 479 | 74,582 | 561 | 75,163 |
| `unsigned int count` + `__builtin_expect(count < 5,1)` | 1,013 | 479 | 74,582 | 561 | 75,163 |
| `count < 5` with `__builtin_expect` | 1,013 | 479 | 74,582 | 561 | 75,163 |
| `count < 5` plain | 8,659 | 1,490 | size **9,664** | 1,741 | — |
| `count != 5` with `__builtin_expect` | 7,311 | 1,245 | size **9,664** | 1,528 | — |
| `int count = 0;` at declaration | 1,013 | 479 | 74,582 | 561 | 75,163 |
| free-credits `volatile` removed (`int textYv`) | 8,712 | 1,387 | size **9,740** | 1,678 | — |
| free-credits `volatile int &textYv` (old shape) | 2,760 | 514 | size **9,692** | 706 | — |
| ship guard `__builtin_expect(tab == 0, 0)` | 7,419 | 1,788 | size **9,664** | 1,511 | — |
| ship guard `__builtin_expect(tab == 0, 1)` | 7,494 | 847 | size **9,720** | 1,536 | — |

## Findings

1. **The buy-list `String` declaration order is source-faithful but net +2 B.**
   The original constructs the `sp+312` temporary first (`0x14a93a`,
   `mov r0,r6` with `r6 = sp+312`), then `sp+300` (`0x14a944`, `r5`).  Declaring
   `description` before `price` reproduces that order and drops the aligned
   metric 479 → 468, but the stack-slot swap moves one register elsewhere and
   costs +2 raw / +2 `.text` (total 75,165 vs 75,163).  It is the only cheap
   aligned-only win; not landed because raw is the primary gate metric and the
   net byte count grows.
2. **The ship body's original `setPosition→setVisible→draw` order is a raw
   regression.**  Every permutation of `{P,V,D}` (with `upgTab` declared before
   or after the sequence) is raw ≥ 1,159 vs the base's `V,D,P`-with-`upgTab`-in-
   the-middle at 1,013.  The base shape is a deliberate codegen anchor.
3. **The upgrade block's original `setVisible(buttons[2])` before
   `setPosition(buttons[10])` order** (`upg_swap`) is the best aligned shape
   found (460) but costs +147 raw, concentrated in `149b00`/`149c00`.
4. **The loop back-edge is structurally different but size-locked.**  Original
   `cmp.w r9,#4; bls.w 14a93a`; ours `cmp.w r9,#5; bne.w 14a92e`.  Any plain
   `count <= 4`/`count < 5` form drops the function to 9,664 B (the
   `__builtin_expect` anchor is load-bearing), and unsigned/`<5`/`!=5` spellings
   under the hint all canonicalise back to the base code.
5. **The free-credits `volatile int textYv` is load-bearing.**  Removing it
   explodes the function to 9,740 B / raw 8,712; the old `volatile int &textYv`
   reference form gives 9,692 B / raw 2,760.  The w13767 value-copy form is the
   unique size-exact shape.
6. **Harness caveat.**  The scratch `harness.run` captures only `tail -1` of the
   build, so a compile error (last line `ninja: build stopped`) is silently
   reported as a stale-`.so` measurement.  Verify any surprising "identical to
   base" result with a direct `link_so` before trusting it.

## Conclusion

The base remains the unique size-exact **net** minimum.  Every source shape that
reproduces the original's ship-body, upgrade-block, free-credits or
`buttons[17]` call order either costs raw bytes (the register-allocation tie
moves elsewhere) or changes the function size / `.ARM.extab`.  No change landed.

Gate re-run on the pristine tree: `RATCHET PASS` — byte_exact 4508/4524,
linked_exact 4508/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.
