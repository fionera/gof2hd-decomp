# Layout::isFading
elf 0xd514c body 6
Sig: undefined __thiscall isFading(Layout * this)

## decompile
```c

/* Layout::isFading() */

Layout __thiscall Layout::isFading(Layout *this)

{
  return this[0x400];
}

```

## target disasm
```
  000e514c: ldrb.w r0,[r0,#0x400]
  000e5150: bx lr
```
