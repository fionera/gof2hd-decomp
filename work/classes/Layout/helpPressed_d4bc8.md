# Layout::helpPressed
elf 0xd4bc8 body 6
Sig: undefined __thiscall helpPressed(Layout * this)

## decompile
```c

/* Layout::helpPressed() */

Layout __thiscall Layout::helpPressed(Layout *this)

{
  return this[0x3c0];
}

```

## target disasm
```
  000e4bc8: ldrb.w r0,[r0,#0x3c0]
  000e4bcc: bx lr
```
