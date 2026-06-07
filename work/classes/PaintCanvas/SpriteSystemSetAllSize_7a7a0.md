# PaintCanvas::SpriteSystemSetAllSize
elf 0x7a7a0 body 30
Sig: undefined __stdcall SpriteSystemSetAllSize(uint param_1, short param_2)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemSetAllSize(unsigned int, short) */

void AbyssEngine::PaintCanvas::SpriteSystemSetAllSize(uint param_1,short param_2)

{
  if (*(uint *)(param_1 + 0x180) <= (uint)(int)param_2) {
    return;
  }
  if (*(int *)(*(int *)(param_1 + 0x184) + param_2 * 4) == 0) {
    return;
  }
  (*(code *)(DAT_001ab284 + 0x1ab288))();
  return;
}

```

## target disasm
```
  0008a7a0: ldr.w r3,[r0,#0x180]
  0008a7a4: cmp r3,r1
  0008a7a6: it ls
  0008a7a8: bx.ls lr
  0008a7aa: ldr.w r0,[r0,#0x184]
  0008a7ae: ldr.w r1,[r0,r1,lsl #0x2]
  0008a7b2: cbz r1,0x0008a7ba
  0008a7b4: mov r0,r2
  0008a7b6: b.w 0x001ab278
  0008a7ba: bx lr
  001ab278: bx pc
```
