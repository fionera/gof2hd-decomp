# PaintCanvas::GetImage2DWidth
elf 0x78d9c body 24
Sig: undefined __stdcall GetImage2DWidth(uint param_1)

## decompile
```c

/* AbyssEngine::PaintCanvas::GetImage2DWidth(unsigned int) */

undefined2 AbyssEngine::PaintCanvas::GetImage2DWidth(uint param_1)

{
  uint in_r1;
  
  if (in_r1 < *(uint *)(param_1 + 0x14c)) {
    return *(undefined2 *)(*(int *)(*(int *)(param_1 + 0x150) + in_r1 * 4) + 0x10);
  }
  return 0;
}

```

## target disasm
```
  00088d9c: ldr.w r2,[r0,#0x14c]
  00088da0: cmp r2,r1
  00088da2: itt ls
  00088da4: mov.ls r0,#0x0
  00088da6: bx.ls lr
  00088da8: ldr.w r0,[r0,#0x150]
  00088dac: ldr.w r0,[r0,r1,lsl #0x2]
  00088db0: ldrh r0,[r0,#0x10]
  00088db2: bx lr
```
