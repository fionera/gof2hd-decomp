# PaintCanvas::SetTexture
elf 0x78212 body 8
Sig: undefined __stdcall SetTexture(uint param_1, uint param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::SetTexture(unsigned int, unsigned int) */

void AbyssEngine::PaintCanvas::SetTexture(uint param_1,uint param_2)

{
  (*(code *)(DAT_001ab1a4 + 0x1ab1a8))(*(undefined4 *)(param_1 + 0x34));
  return;
}

```

## target disasm
```
  00088212: ldr r0,[r0,#0x34]
  00088214: b.w 0x001ab198
  001ab198: bx pc
```
