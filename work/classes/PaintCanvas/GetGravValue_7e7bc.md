# PaintCanvas::GetGravValue
elf 0x7e7bc body 8
Sig: undefined __thiscall GetGravValue(PaintCanvas * this)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetGravValue() */

void __thiscall AbyssEngine::PaintCanvas::GetGravValue(PaintCanvas *this)

{
  (*(code *)(DAT_001ab424 + 0x1ab428))(*(undefined4 *)(this + 0x34));
  return;
}

```

## target disasm
```
  0008e7bc: ldr r0,[r0,#0x34]
  0008e7be: b.w 0x001ab418
  001ab418: bx pc
```
