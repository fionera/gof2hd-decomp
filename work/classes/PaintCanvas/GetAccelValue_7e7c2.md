# PaintCanvas::GetAccelValue
elf 0x7e7c2 body 8
Sig: undefined __thiscall GetAccelValue(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetAccelValue() */

void __thiscall AbyssEngine::PaintCanvas::GetAccelValue(PaintCanvas *this)

{
  (*(code *)(DAT_001ab434 + 0x1ab438))(*(undefined4 *)(this + 0x34));
  return;
}

```

## target disasm
```
  0008e7c2: ldr r0,[r0,#0x34]
  0008e7c4: b.w 0x001ab428
  001ab428: bx pc
```
