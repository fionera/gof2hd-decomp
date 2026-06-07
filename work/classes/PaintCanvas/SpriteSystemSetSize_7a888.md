# PaintCanvas::SpriteSystemSetSize
elf 0x7a888 body 42
Sig: undefined __stdcall SpriteSystemSetSize(uint param_1, ushort param_2, short param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemSetSize(unsigned int, unsigned short, short) */

void AbyssEngine::PaintCanvas::SpriteSystemSetSize(uint param_1,ushort param_2,short param_3)

{
  ushort *puVar1;
  undefined2 in_r3;
  
  if (((uint)param_2 < *(uint *)(param_1 + 0x180)) &&
     (puVar1 = *(ushort **)(*(int *)(param_1 + 0x184) + (uint)param_2 * 4), puVar1 != (ushort *)0x0)
     ) {
    if ((uint)*puVar1 <= (uint)(int)param_3) {
      return;
    }
    if ((char)puVar1[6] != '\0') {
      **(undefined2 **)(puVar1 + 4) = in_r3;
      return;
    }
    (*(undefined2 **)(puVar1 + 4))[param_3] = in_r3;
  }
  return;
}

```

## target disasm
```
  0008a888: ldr.w r12,[r0,#0x180]
  0008a88c: cmp r12,r1
  0008a88e: bls 0x0008a8b0
  0008a890: ldr.w r0,[r0,#0x184]
  0008a894: ldr.w r0,[r0,r1,lsl #0x2]
  0008a898: cbz r0,0x0008a8b0
  0008a89a: ldrh r1,[r0,#0x0]
  0008a89c: cmp r1,r2
  0008a89e: it ls
  0008a8a0: bx.ls lr
  0008a8a2: ldr r1,[r0,#0x8]
  0008a8a4: ldrb r0,[r0,#0xc]
  0008a8a6: cbz r0,0x0008a8ac
  0008a8a8: strh r3,[r1,#0x0]
  0008a8aa: bx lr
  0008a8ac: strh.w r3,[r1,r2,lsl #0x1]
  0008a8b0: bx lr
```
