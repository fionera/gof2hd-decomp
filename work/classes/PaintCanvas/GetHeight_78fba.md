# PaintCanvas::GetHeight
elf 0x78fba body 8
Sig: undefined __stdcall GetHeight(void)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetHeight() */

void AbyssEngine::PaintCanvas::GetHeight(void)

{
  int in_r0;
  
  (*(code *)(DAT_001ab1e4 + 0x1ab1e8))(*(undefined4 *)(in_r0 + 0x34));
  return;
}

```

## target disasm
```
  00088fba: ldr r0,[r0,#0x34]
  00088fbc: b.w 0x001ab1d8
  001ab1d8: bx pc
```
