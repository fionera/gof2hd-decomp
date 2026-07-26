# Sparks::explode(int, int, int) deferred reconstruction

Target: `_ZN6Sparks7explodeEiii`

- Original: `0x15e48c`, 264 bytes.
- Current: 264 bytes, 97.8% structural match.
- TU: `src/engine/render/Sparks.cpp`.
- AAPCS inputs: `this=r0`, `x=r1`, `y=r2`, `z=r3`.
- Verified fields: `spriteSystem +0x04`, `active +0x10`, `kind +0x14`,
  and `count +0x18`.
- The random bound is `0x190` (400), and the common epilogue starts at
  function offset `+0xea`.

The sole executable mismatch is argument-copy scheduling:

```text
original                         current
ldrb r0, [r0, #0x10]             ldrb r0, [r0, #0x10]
mov r6, r3                        cmp r0, #0
mov fp, r1                        bne <epilogue>
cmp r0, #0                        ldr r0, [r5, #0x14]
bne <epilogue>                   mov r6, r3
ldr r0, [r5, #0x14]              mov fp, r1
```

Both streams reach the `kind` comparison at the same offset, and the rest of
the executable body and 264-byte size match. Literal-pool differences are
relocations.

Attempts exhausted:

1. Removed the `x0/y0/z0` aliases and used the formal parameters directly.
   No byte changed.
2. Replaced the early return with a positive `active == 0` guarded body.
   No byte changed.
3. A fresh retry and final escalation found no further safe local form:
   trivial aliases, direct parameters, and branch inversion all collapse to
   equivalent SSA and remain legal MachineSink targets.

Forcing early copies would require observable work on the active-return path
or an address escape, volatile barrier, assembly, or compiler attribute.
Those choices either change semantics or violate campaign rules. The leading
hypothesis is an unrecovered non-scalar lifetime/control-flow shape in the
original source, or translation-unit compiler-state drift. Revisit only with
new source-provenance evidence or a distinct legal lifetime construct.
