# PaintCanvas::GetImage2DHeight
elf 0x78db4 body 24
Sig: undefined __stdcall GetImage2DHeight(uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetImage2DHeight(unsigned int) */

undefined2 AbyssEngine::PaintCanvas::GetImage2DHeight(uint param_1)

{
  uint in_r1;
  
  if (in_r1 < *(uint *)(param_1 + 0x14c)) {
    return *(undefined2 *)(*(int *)(*(int *)(param_1 + 0x150) + in_r1 * 4) + 0x12);
  }
  return 0;
}

```

## target disasm
```
  00088db4: ldr.w r2,[r0,#0x14c]
  00088db8: cmp r2,r1
  00088dba: itt ls
  00088dbc: mov.ls r0,#0x0
  00088dbe: bx.ls lr
  00088dc0: ldr.w r0,[r0,#0x150]
  00088dc4: ldr.w r0,[r0,r1,lsl #0x2]
  00088dc8: ldrh r0,[r0,#0x12]
  00088dca: bx lr
```
