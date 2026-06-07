# PaintCanvas::SpriteSystemSetPosition
elf 0x7a9d6 body 62
Sig: undefined __stdcall SpriteSystemSetPosition(uint param_1, ushort param_2, float param_3, float param_4, float param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemSetPosition(unsigned int, unsigned short, float, float,
   float) */

float AbyssEngine::PaintCanvas::SpriteSystemSetPosition
                (uint param_1,ushort param_2,float param_3,float param_4,float param_5)

{
  ushort *puVar1;
  undefined4 *puVar2;
  uint in_r2;
  undefined4 in_r3;
  undefined4 in_stack_00000000;
  float in_stack_00000004;
  
  if (((uint)param_2 < *(uint *)(param_1 + 0x180)) &&
     (puVar1 = *(ushort **)(*(int *)(param_1 + 0x184) + (uint)param_2 * 4), puVar1 != (ushort *)0x0)
     ) {
    if (*puVar1 <= in_r2) {
      return param_3;
    }
    puVar2 = (undefined4 *)(*(int *)(puVar1 + 2) + in_r2 * 0xc);
    *puVar2 = in_r3;
    puVar2[1] = in_stack_00000000;
    puVar2[2] = in_stack_00000004;
    param_3 = in_stack_00000004;
  }
  return param_3;
}

```

## target disasm
```
  0008a9d6: ldr.w r12,[r0,#0x180]
  0008a9da: cmp r12,r1
  0008a9dc: bls 0x0008aa12
  0008a9de: ldr.w r0,[r0,#0x184]
  0008a9e2: ldr.w r0,[r0,r1,lsl #0x2]
  0008a9e6: cbz r0,0x0008aa12
  0008a9e8: ldrh r1,[r0,#0x0]
  0008a9ea: cmp r1,r2
  0008a9ec: it ls
  0008a9ee: bx.ls lr
  0008a9f0: ldr r0,[r0,#0x4]
  0008a9f2: add.w r1,r2,r2, lsl #0x1
  0008a9f6: vldr.32 s0,[sp,#0x4]
  0008a9fa: vmov s4,r3
  0008a9fe: add.w r0,r0,r1, lsl #0x2
  0008aa02: vldr.32 s2,[sp]
  0008aa06: vstr.32 s4,[r0]
  0008aa0a: vstr.32 s2,[r0,#0x4]
  0008aa0e: vstr.32 s0,[r0,#0x8]
  0008aa12: bx lr
```
