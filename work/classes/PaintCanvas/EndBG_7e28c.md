# PaintCanvas::EndBG
elf 0x7e28c body 14
Sig: undefined __thiscall EndBG(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::EndBG() */

void __thiscall AbyssEngine::PaintCanvas::EndBG(PaintCanvas *this)

{
  **(PaintCanvas **)(DAT_0008e29c + 0x8e296) = this[0x1f0];
  return;
}

```

## target disasm
```
  0008e28c: ldr r1,[0x0008e29c]
  0008e28e: ldrb.w r0,[r0,#0x1f0]
  0008e292: add r1,pc
  0008e294: ldr r1,[r1,#0x0]
  0008e296: strb r0,[r1,#0x0]
  0008e298: bx lr
```
