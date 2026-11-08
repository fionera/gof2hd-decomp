# Small tail B wave 153 (w9918): MenuTouchWindow / ModStation / SpaceLounge

Status: **one landed `src/` change** (`MenuTouchWindow::OnTouchBegin`, metric-only).
Worktree `wt/w9918`, base `4a6104a43` (wave 152). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-b-wave152-w9818.md`, `small-tail-b-wave151-w9718.md`,
`small-tail-wave149-w9516.md`, `modstation-extab-lp-wave149-w9520.md`.

Gate before/after (full `check`): **RATCHET PASS**, `byte_exact 4507 -> 4507`,
`linked_exact 4507 -> 4507`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  Body sizes unchanged:
`OnTouchBegin` 1896 B, `ModStation` 1008 B, `drawLounge` 1948 B.

Metrics: raw same-address diff bytes (`/dev/shm/w9918/fdiff.py`) and
instruction-index alignment (`/dev/shm/w9918/align_diff.py`, a path-adjusted
copy of `_work/reconstructions/align_diff.py` because the committed tool
hard-codes the main worktree's build).

| fn | raw before | raw after | aligned before | aligned after |
|---|---:|---:|---:|---:|
| `MenuTouchWindow::OnTouchBegin` (`0x129bd4`) | 29 | **28** | 15 | 16 |
| `ModStation::ModStation` C1 (`0xd52d0`) | 43 | 43 | 12 | 12 |
| `SpaceLounge::drawLounge` (`0x170d38`) | 175 | 175 | 41 | 41 |

## 1. MenuTouchWindow: LANDED — B via the reference, C direct

New shape (not in w149/w151/w152): keep the inner `Layout &layout =
*Globals::layout;` but move the *reference* use from C (`field_0x20_top`) to
B (`field_0xc_leftMargin`), and make C a direct `Globals::layout->field_0x20_top`:

```
if (x > Globals::w - Globals::layout->buttonInsetX - Canvas->GetImage2DWidth(...) &&
    y < Globals::layout->field_0x20_top + (layout.field_0xc_leftMargin + Canvas->GetImage2DHeight(...)))
```

This **kills the `%508` cached-layout use for C**: the original reloads
`Globals::layout` after the first `GetImage2DWidth` call and uses it for both
`field_0xc_leftMargin` and `field_0x20_top`; the landed shape now emits that
reload (`ldr r2,[sp,#16]; ldr r2,[r2,#0]; ldr r5,[r2,#12]; ldr r6,[r2,#32]`).
It keeps the 1896 B frame and `fp = &Globals::Canvas` colouring (the reference
is still live across the call for B), unlike the all-direct shape (1892 B,
`fp = &Globals::layout`).

Trade-off: B is now loaded from the cached reference (`ldr r4,[r6,#12]`)
instead of from the reload, so the instruction-index alignment gains one
`imm:ldr` mismatch (15 -> 16) while raw bytes drop 29 -> 28.  Head kept the
opposite trade (B matching, C cached).  The case-11 `ip`/`r4` scratch tie at
`0x129fd2` is independent and unchanged.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (A,B direct; C ref; D direct) | 29 | **15** | 1896 |
| **A,B ref; C direct; D direct (LANDED)** | **28** | 16 | 1896 |
| A direct; B ref; C direct; D direct | 28 | 16 | 1896 |
| A ref; B,C,D direct | 358 | 76 | 1892 |
| all four direct | 358 | 76 | 1892 |
| all direct + `(void)layout;` / `sizeof` / `&layout` | 358 | 76 | 1892 |
| B ref + D ref, C direct | 287 | 74 | 1892 |
| outer `Layout *layout` for A/B, C/D direct | 1762 | 818 | 1912 |

## 2. ModStation: no shape keeps both the pool order and the mov/cmp order

Re-confirmed HEAD 43/12/1008.  New spellings tried this wave (all reverted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (both volatile) | **43** | **12** | 1008 |
| pitch load before the yaw-table select | 690 | 52 | 1000 |
| yaw-table select after the pitch load | 689 | 61 | 1000 |
| duplicate select (`yawTable2`) before the volatile yaw load | 690 | 52 | 1000 |
| yaw plain (`yawTable[race]`) | 44 | 18 | 1008 |
| yaw plain + `__builtin_expect` / if-expect / volatile `iPad` / switch | 44 | 18 | 1008 |
| `volatile` yaw-table pointer | 278 | 149 | 1008 |
| `volatile` yaw-table pointer + volatile read | 696 | 152 | 1016 |

The `volatile` on the yaw *element* load is still the only lever that flips
the select operand order (and hence the literal-pool order `YawIPad, Yaw` and
`mov r0,r3` before `cmp`).  Dropping it gives the original pool order but the
scheduler sinks the `Yaw` LDRLIT (`0xd543a -> 0xd5448`) and moves `mov r4,r2`
early (`0xd546e -> 0xd5456`); every new spelling that tries to pin the select
before the pitch load shrinks the body to 1000 B.  No source shape found that
keeps both.

## 3. SpaceLounge: the pad/x colouring tie is independent of the right arm

Re-confirmed HEAD 175/41/1948.  New spellings tried this wave (all reverted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (`pad + pad` clamp) | **175** | **41** | 1948 |
| `int boxX/boxY/boxW` locals before `drawBox` | 175 | 41 | 1948 |
| `int x = ...` directly (no `centerX`) | 175 | 41 | 1948 |
| `SetColor` after `top` | 175 | 41 | 1948 |
| local `Layout *lay` for the pad load | 175 | 41 | 1948 |
| `top = ... (pad * 2)` | 175 | 41 | 1948 |
| `pad` loaded after `centerX` | 254 | 72 | 1948 |
| `pad2` in top + clamp (2-insn arm) | 445 | 54 | 1948 |
| `pad2` in top + box | 1835 | 249 | 1964 |
| `pad2` in top + box + clamp | 1812 | 237 | 1960 |

The original has `pad` in `r9` and `x` (centerX) in `r5`; ours keeps `pad` in
`fp` and `x` in `r9`.  The `pad2`/`pad * 2` right arm reproduces the original
2-instruction clamp (`itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1`) but leaves the
same `pad -> fp` / `x -> r9` colouring and adds `mov r5,fp` in both arms
(445/54).  The colouring tie is independent of the arm association; no source
shape tested this wave moves it.

## 4. Files changed

* `src/game/ui/MenuTouchWindow.cpp` — case-15 C (`field_0x20_top`) now direct,
  B (`field_0xc_leftMargin`) via the reference.  Size 1896 unchanged; raw
  29 -> 28, aligned 15 -> 16.
* `_work/reconstructions/small-tail-b-wave153-w9918.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.
