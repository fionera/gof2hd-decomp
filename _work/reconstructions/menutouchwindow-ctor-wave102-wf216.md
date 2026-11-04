# MenuTouchWindow::MenuTouchWindow C1/C2 — wave-102 local-optimum audit (wf216)

Status: research note, 2026-11-04. Worker `wf216`, branch `wave102-wf216`, base
`53d9ee4d5`. The worker's context ended before it committed its note; this file
preserves its measured findings. No source change landed.

## Result

`MenuTouchWindow::MenuTouchWindow(int)` (C1/C2 alias) is **95.1%** (raw
`0.9509183980728696`), **9600 B**, 3322 instructions vs the original's 3320.
The body is a strong local optimum: statement-order permutations, 216
declaration/order variants, `__builtin_assume` forms, type changes, loop-condition
forms and scheduler variants all landed at 95.1% or below.

## Exact remaining difference

* Register assignment: ours `font=fp, lang=r6, button=r5, bw=r8, spacing=r4,
  i=r9, special=sl`; original `font=r5, lang=r8, button=r6, bw=fp, spacing=r4,
  i=r9, special=sl`.
* `mov r9, r8` placement: ours `0x12342c`, original `0x123400` (0x2c apart).
* Loop backedge: ours `bne.w`, original `bcc.w`.
* Stack slots: ours canary `[sp,#56]`, `langList` `[sp,#60]`; original canary
  `[sp,#60]`, other local `[sp,#56]`.
* 125 diff blocks; the body is structurally the same bottom-tested `for` loop.

## Searches performed

* 216 statement/declaration permutations; only the base and its `SFLPB`/`FLSPB`/
  `LSFPB`/`LFSPB` equivalents reach 95.1%. Any order with `bw` before `spacing`
  drops to 76.1% (`9640 B`).
* `__builtin_assume` variants (same-orientation selects) reach 95.1% but do not
  improve the score; `phi_uint` is the most faithful of these.
* Type changes (`int`/`unsigned`/`float`), loop-condition forms, scheduler and
  register-allocation hints: no improvement.

## Conclusion

The remaining 4.9% is a greedy-register-allocator permutation, not a source
semantic difference. The next lever is a source shape that moves `font` from
`fp` to `r5` and `lang` from `r6` to `r8` without disturbing the 9600-byte body.
