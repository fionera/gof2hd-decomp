# Small tail B wave 152 (w9818): MenuTouchWindow / ModStation / SpaceLounge —
# new hypotheses on the three residuals (analysis only, no source change)

Status: **analysis only, no landed `src/` change**. Worktree `wt/w9818`, base
`9d491d46a` (wave 151). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-b-wave151-w9718.md`, `small-tail-b-wave150-w9618.md`,
`small-tail-wave149-w9516.md`, `spacelounge-drawlounge-wave130-w7682.md`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507 ->
4507`, `linked_exact 4507 -> 4507`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  Body sizes unchanged: `OnTouchBegin`
1896 B, `ModStation` 1008 B, `drawLounge` 1948 B.

Metrics: raw same-address diff bytes (`/dev/shm/w9818/fdiff.py`) and
instruction-index alignment (`_work/reconstructions/align_diff.py`).

| fn | raw before | raw after | aligned before | aligned after |
|---|---:|---:|---:|---:|
| `MenuTouchWindow::OnTouchBegin` (`0x129bd4`) | 29 | 29 | 15 | 15 |
| `ModStation::ModStation` C1 (`0xd52d0`) | 43 | 43 | 12 | 12 |
| `SpaceLounge::drawLounge` (`0x170d38`) | 175 | 175 | 41 | 41 |

## 1. MenuTouchWindow: two independent Greedy-RA ties (no new source shape)

Re-confirmed the two clusters of wave 150/151:

* `0x129fd2`/`0x129fd8` (case 11, fourth hit test): original
  `ldr.w ip,[sl,#340]` + `add r3,ip`; ours `ldr.w r4,[sl,#340]` + `add r3,r4`.
  `r4` holds `[sp,#16]` (the layout pointer) on entry to the cluster in both
  builds and is dead after `0x129fd2`; the allocator simply picks a different
  scratch register.  A pure tie, not source-reachable.
* `0x12a1e0..0x12a24c` (case 15 scrollbar test): original reloads
  `Globals::layout` after the first `GetImage2DWidth` call for
  `field_0x20_top` (`ldr r2,[sp,#16]; ldr r2,[r2,#0]; ldr r6,[r2,#32]`),
  ours keeps the pre-call layout value in `r6` and uses it for the top load.
  Every direct-C spelling also moves `&Globals::layout` into `fp` instead of
  `Globals::Canvas` and/or drops the `[sp,#16]` spill (body 1892 B).

New spelling tried this wave (reverted): hoisting a local
`int opt84 = *(int *) &Globals::options[84];` / `opt88` for the case-11
hit tests.  This is *not* what the original does: the body shrinks to 1884 B
(the repeated `*(int *) &Globals::options[NN]` is load-bearing) and both
metrics explode:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **29** | **15** | 1896 |
| `opt84` + `opt88` locals | 195 | 124 | 1884 |
| `opt84` local only | 612 | 88 | 1884 |
| `opt88` local only | 627 | 119 | 1884 |

## 2. ModStation: the pool order is caused by the volatile yaw load

New evidence.  The MIR of the whole TU contains a *second* function
(`ModStation.cpp` line 791, `(Globals::iPad ? gHangarCamYawIPad :
gHangarCamYaw)[race]`, **non-volatile**) that already emits the original
order and the original select shape:

```
%445 = tLDRLIT gHangarCamYawIPad
%446 = tLDRLIT gHangarCamYaw
%450 = t2MOVCCr %445, %446, 0        ; true operand first, no COPY
```

The C1 block (both loads `volatile`) instead emits

```
%117 = tLDRLIT gHangarCamYaw
%118 = tLDRLIT gHangarCamYawIPad
%121 = t2MOVCCr %118, %117, 0
```

i.e. the `volatile` on the **yaw** load is what flips the select operand
order (and therefore the literal-pool order and the `mov r0,r3`/`cmp`
ordering).  Dropping `volatile` from the yaw load alone
(`int yawBits = *(const int *) &yawTable[race];`) fixes exactly those two
artefacts:

| shape | raw | aligned | size | pool order |
|---|---:|---:|---:|---|
| HEAD (both volatile) | **43** | **12** | 1008 | Pitch,Yaw,YawIPad |
| pitch volatile, yaw plain | 44 | 18 | 1008 | **Pitch,YawIPad,Yaw** |
| yaw volatile, pitch plain | 666 | 60 | 1008 | — |
| both plain | 667 | 59 | 1008 | — |
| `volatile` pointer vars | 667 | 59 | 1008 | — |

`pitch volatile, yaw plain` gets the pool order and `mov r0,r3`-before-`cmp`
right, but the scheduler then sinks the `Yaw` LDRLIT from `0xd543a` to
`0xd5448`, moves `mov r4,r2` early (`0xd5456` vs `0xd546e`) and reorders
`str r8`/`movs r0,#2`, for a net 44/18 (worse than 43/12).  Keeping the
`yawTable` pointer spilled (size 1008) requires the volatile yaw load, so the
two artefacts cannot both be bought from source in clang 7.0.2 here.
All condition/declaration spellings re-tested (`pitch` first, `yaw` first,
`if/else`, `init-then-if`, `two_vars`, `direct_pitch`) stay at 43/12.

## 3. SpaceLounge: the right arm is reachable, the pad/x colouring is not

The wave-151 `pad + pad` association is still the best (175/41).  New
spellings:

| shape | raw | aligned | notes |
|---|---:|---:|---|
| HEAD (`-width - pad - pad`) | **175** | **41** | 4-insn right arm |
| `- (width + pad + pad)` | 176 | 42 | same 4-insn arm |
| `int pad2 = pad + pad` in top only | 175 | 41 | identical |
| `pad2` in top + box | 175 | 41 | identical |
| `pad2` in top + box + right arm `- (width + pad2)` | 445 | 54 | **correct 2-insn arm** |
| `pad * 2` in top only | 175 | 41 | identical |
| `pad * 2` in top + right arm | 445 | 54 | correct arm |
| local `Layout *lay` for the pad load | 175 | 41 | identical |

The `pad2`/`pad*2` right arm produces the original
`itt gt; addgt.w r1,r6,sl; subgt.w r9,r0,r1`, but the allocator then keeps
`pad` in `fp` and `x` in `r9` (original: `pad` in `r9`, `x` in `r5`) and
materialises `mov r5,fp` in *both* arms for the later `x - pad` /
`top - pad` uses (the 16-bit `subs` needs a low register).  That is the
wave-130 `agent > pad > x` weight-window tie; no source spelling found this
wave flips it while keeping the 2-insn arm.

## Conclusion

No source win landed this wave.  The three residuals are now pinned to:
(1) two independent Greedy-RA ties in `OnTouchBegin`; (2) the `volatile`
yaw load in `ModStation` forcing the select operand order (pool order and
`mov`/`cmp` order are the same artefact); (3) the `pad`/`x` register-colouring
tie in `drawLounge`, which is independent of the right-arm association.
