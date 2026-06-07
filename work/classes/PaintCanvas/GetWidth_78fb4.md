# PaintCanvas::GetWidth
elf 0x78fb4 body 8
Sig: undefined __stdcall GetWidth(void)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetWidth() */

void AbyssEngine::PaintCanvas::GetWidth(void)

{
  int in_r0;
  
  (*(code *)(DAT_001ab1d4 + 0x1ab1d8))(*(undefined4 *)(in_r0 + 0x34));
  return;
}

```

## target disasm
```
  00088fb4: ldr r0,[r0,#0x34]
  00088fb6: b.w 0x001ab1c8
  001ab1c8: bx pc
```
