# Small tail B wave 151 (w9718): MenuTouchWindow / ModStation / SpaceLounge —
# one landed SpaceLounge association (raw 176 -> 175, aligned 42 -> 41)

Status: **one landed `src/` change** (`SpaceLounge::drawLounge`, metric-only).
Worktree `wt/w9718`, base `db9658a51` (wave 150). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-b-wave150-w9618.md`, `small-tail-wave149-w9516.md`,
`modstation-extab-lp-wave149-w9520.md`.

Gate before/after (full `check`): **RATCHET PASS**, `byte_exact 4507 -> 4507`,
`linked_exact 4507 -> 4507`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  Body sizes unchanged:
`OnTouchBegin` 1896 B, `ModStation` 1008 B, `drawLounge` 1948 B.
`.text` size unchanged (the landed change is intra-function and size-neutral).

Metrics: raw same-address diff bytes (`/dev/shm/w9718/fdiff.py`) and
instruction-index alignment (`_work/reconstructions/align_diff.py`).

| fn | raw before | raw after | aligned before | aligned after |
|---|---:|---:|---:|---:|
| `MenuTouchWindow::OnTouchBegin` (`0x129bd4`) | 29 | 29 | 15 | 15 |
| `ModStation::ModStation` C1 (`0xd52d0`) | 43 | 43 | 12 | 12 |
| `SpaceLounge::drawLounge` (`0x170d38`) | 176 | **175** | 42 | **41** |

## 1. MenuTouchWindow: the `%29` live-across-call tie is not source-reachable

Re-confirmed the two clusters from w9618: case-8 `ldr.w ip,[sl,#340]` vs
`r4`, and case-15 pad/x colouring (`Globals::layout` value kept in `r6`
across the first `GetImage2DWidth` call instead of being reloaded).

The MIR decode of the current HEAD (A,B,D direct; C via `Layout &`) is:

```
bb.38: %29 = LDR [&Globals::layout]      ; used for A (CSE with the ref binding)
bb.40: %319 = LDR [&Globals::layout]     ; reload after the call
       %320 = LDR [%29, 32]              ; C top  <- uses the OLD value
       %323 = LDR [%319, 12]             ; B leftMargin
```

The original uses `%319` for both B and C, i.e. C is a direct
`Globals::layout->field_0x20_top` access (the same reload as B).  But making
C direct removes the reference local's only live-across-call use, so clang
drops the `[sp,#16]` spill and the body shrinks to 1892 B with a completely
different colouring (`&Globals::Canvas` moves out of `fp`).

New spellings tried this wave (all reverted; 1896 B unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| **HEAD** | **29** | **15** | 1896 |
| all four via `layout.` reference | 308 | 76 | 1888 |
| A ref, B,C,D direct | 358 | 76 | 1892 |
| A,B,C direct, D ref | 314 | 74 | 1888 |
| C direct, D ref | 314 | 74 | 1888 |
| C direct (all direct) | 358 | 76 | 1892 |
| `(top + leftMargin) + height` grouping | 45 | 23 | 1896 |
| `leftMargin + top + height` | 45 | 23 | 1896 |
| `leftMargin + (top + height)` | 29 | 15 | 1896 |
| `height + top + leftMargin` | 116 | 64 | 1896 |

**Conclusion.** The original's simultaneous `%29`-dead-at-call and
`&Globals::layout`-spilled-on-stack state is a Greedy-RA remat tie.  Every
source shape that makes C a direct reload also lets clang eliminate the
reference stack slot (or keeps the value live in a callee-saved register),
so the 1896 B frame and the `fp = Globals::Canvas` colouring cannot be
preserved.  The case-8 `ip`/`r4` tie is independent and unchanged.

## 2. ModStation: the CMOV false-arm COPY is an isel/scheduler order, not source

Re-confirmed 43 raw / 12 aligned / 1008 B.  The three artefacts are
unchanged: literal-pool order (`Pitch, YawIPad, Yaw` vs `Pitch, Yaw,
YawIPad`), `mov r0,r3` before vs after `cmp r1,#0`, and the `str r8` /
`mov r4,r2` sink after vs before the yaw load.

The MIR (`-mllvm -print-machineinstrs`, after two-address pass) is:

```
%115 = LDRLIT Pitch
%117 = LDRLIT Yaw
%165 = LDRLIT YawIPad
%119 = LDRB Globals::iPad
%236 = LDRs [%115 + race*4]       ; pitch
t2CMPri %119, 0
%121 = COPY %165                  ; false arm
%121 = t2MOVCCr %121, %117, 0     ; true arm = Yaw
%122 = LDRs [%121 + race*4]       ; yaw
```

The machine scheduler leaves this order untouched; the original needs the
`COPY` emitted before the `CMP`.  New spellings tried this wave (all
reverted; 1008 B unless noted):

| shape | raw | aligned |
|---|---:|---:|
| HEAD | **43** | **12** |
| `iPad == 0 ? Yaw : YawIPad` | 43 | 12 |
| `!iPad ? Yaw : YawIPad` | 43 | 12 |
| `iPad != 0 ? YawIPad : Yaw` | 43 | 12 |
| `int ipad = Globals::iPad; ipad ? ...` | 43 | 12 |
| `int ipad = ...; ipad != 0 ? ...` | 43 | 12 |
| `bool ipad = iPad != 0; ipad ? ...` | 43 | 12 |
| `yawTable = YawIPad; if (!iPad) yawTable = Yaw;` | 43 | 12 |
| `yawTable = Yaw; if (iPad) yawTable = YawIPad;` | 43 | 12 |
| `__builtin_expect(iPad, 0/1) ? ...` | 43 | 12 |
| `(__builtin_expect(iPad,0) != 0) ? ...` | 43 | 12 |
| `__builtin_expect(iPad != 0, 1) ? ...` | 43 | 12 |
| `*(volatile unsigned char *)&iPad ? ...` | 43 | 12 |
| `const float *const` / `const float*` / `__restrict` | 43 | 12 |
| `(iPad) ? (YawIPad) : (Yaw)` | 43 | 12 |
| `(iPad != 0) ? ...` / `(iPad > 0) ? ...` / `(iPad >= 1) ? ...` | 43 | 12 |
| value select `&(iPad ? YawIPad : Yaw)[race]` | 690 | 52 (1000 B) |
| `yawBits` before `pitchBits` | 744 | 40 (1000 B) |

All pointer-select spellings canonicalise to the same IR/DAG and the same
two-address MIR.  The `COPY`-before-`CMP` order is the reversed topological
emission tie described in w9520; no source form tested (now >40 for this
block) moves it while keeping `it eq`, the roles and 1008 B.

## 3. SpaceLounge: LANDED — `+ (-width - pad - pad)` association

The 42-instruction residual is the pad/x colouring plus the right-edge
clamp arm.  HEAD (`pad + pad`) emits the 4-instruction arm
`itttt gt; movgt r1,#0; subgt.w r1,r1,fp,lsl#1; subgt r1,r1,r6;
addgt.w r9,r1,r0`.  The faithful `pad * 2` form emits the original's
2-instruction arm (`itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1`) but
re-colours pad to `fp` and adds two `mov r5,fp`, shifting the tail
(445 raw / 54 aligned).

**New hypothesis:** change only the *association* of the clamp expression to
`w - inset + (-width - pad - pad)`.  This keeps the HEAD colouring and the
4-instruction arm, but makes the preceding condition sum as
`(pad + x) + width` — the original's `add.w r1,r9,r5; add r1,r6` order
instead of HEAD's `add.w r1,r6,fp; add r1,r9` (`(width + pad) + x`).
Result: **raw 176 -> 175, aligned 42 -> 41, size 1948 unchanged**.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD `-(width + pad + pad)` | 176 | 42 | 1948 |
| **`+ (-width - pad - pad)` (LANDED)** | **175** | **41** | 1948 |
| `+ (-pad - pad - width)` | 175 | 41 | 1948 |
| `+ (-pad - width - pad)` | 175 | 41 | 1948 |
| `+ (-width - pad * 2)` / `pad2` variable | 445 | 54 | 1948 |
| `+ (-width - (pad + pad))` | 445 | 54 | 1948 |
| `+ (-(width + pad + pad))` | 176 | 42 | 1948 |
| `- width - (pad + pad)` | 445 | 54 | 1948 |
| `- width - pad - pad` | 464 | 64 | 1944 |
| `- (pad + pad + width)` | 445 | 54 | 1948 |
| `- 2 * pad` / `- (pad << 1)` | 445 | 54 | 1948 |
| `w - (inset + width + pad + pad)` | 473 | 77 | 1952 |

The remaining 41 aligned instructions are the pad/x colouring
(original `pad` in `r9`, `x` in `r5`; ours `pad` in `fp`, `x` in `r9`) and
the resulting 4-instruction arm.  The w130 `agent > pad > x` weight window
is still the only known lever for that swap; no source shape tested this
wave (including swapped clamp arms, reordered condition operands, `x`
before `pad`, `pad2` in top only) moves it.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9718
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9718/fdiff.py <mangled>
python3 /dev/shm/w9718/align_diff.py <mangled>
```

MIR dump used for the ModStation decode (not committed):

```sh
clang++ ... -mllvm -print-machineinstrs -c src/game/menu/ModStation.cpp
# "After Instruction Selection" / "After Two-Address instruction pass",
# bb.12/bb.15 near-key block
```

Scratch (not committed), `/dev/shm/w9718/`: `fdiff.py`, `align_diff.py`,
`sd.py`, `structdiff.py`, `measure.sh`, `patch.py`, `ms_try.py`,
`sl_try.py`, `mtw_try.py`, `base/` (pristine sources).

## 5. Files changed

* `src/game/menu/SpaceLounge.cpp` — drawLounge right-edge clamp association
  `- (width + pad + pad)` -> `+ (-width - pad - pad)`; comment updated.
  Size 1948 unchanged; raw 176 -> 175, aligned 42 -> 41.
* `_work/reconstructions/small-tail-b-wave151-w9718.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.
