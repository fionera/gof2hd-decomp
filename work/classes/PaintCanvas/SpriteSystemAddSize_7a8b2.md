# PaintCanvas::SpriteSystemAddSize
elf 0x7a8b2 body 52
Sig: undefined __stdcall SpriteSystemAddSize(uint param_1, ushort param_2, short param_3)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemAddSize(unsigned int, unsigned short, short) */

void AbyssEngine::PaintCanvas::SpriteSystemAddSize(uint param_1,ushort param_2,short param_3)

{
  short *psVar1;
  ushort *puVar2;
  uint uVar3;
  short in_r3;
  
  uVar3 = (uint)param_3;
  if (((uint)param_2 < *(uint *)(param_1 + 0x180)) &&
     (puVar2 = *(ushort **)(*(int *)(param_1 + 0x184) + (uint)param_2 * 4), puVar2 != (ushort *)0x0)
     ) {
    if (*puVar2 <= uVar3) {
      return;
    }
    psVar1 = *(short **)(puVar2 + 4);
    if ((char)puVar2[6] != '\0') {
      *psVar1 = *psVar1 + in_r3;
      return;
    }
    psVar1[uVar3] = psVar1[uVar3] + in_r3;
  }
  return;
}

```

## target disasm
```
  0008a8b2: ldr.w r12,[r0,#0x180]
  0008a8b6: cmp r12,r1
  0008a8b8: bls 0x0008a8e4
  0008a8ba: ldr.w r0,[r0,#0x184]
  0008a8be: ldr.w r1,[r0,r1,lsl #0x2]
  0008a8c2: cbz r1,0x0008a8e4
  0008a8c4: ldrh r0,[r1,#0x0]
  0008a8c6: cmp r0,r2
  0008a8c8: it ls
  0008a8ca: bx.ls lr
  0008a8cc: ldr r0,[r1,#0x8]
  0008a8ce: ldrb r1,[r1,#0xc]
  0008a8d0: cbz r1,0x0008a8da
  0008a8d2: ldrh r1,[r0,#0x0]
  0008a8d4: add r1,r3
  0008a8d6: strh r1,[r0,#0x0]
  0008a8d8: bx lr
  0008a8da: ldrh.w r1,[r0,r2,lsl #0x1]
  0008a8de: add r1,r3
  0008a8e0: strh.w r1,[r0,r2,lsl #0x1]
  0008a8e4: bx lr
```
