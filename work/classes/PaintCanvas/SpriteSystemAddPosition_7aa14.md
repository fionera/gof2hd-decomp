# PaintCanvas::SpriteSystemAddPosition
elf 0x7aa14 body 86
Sig: undefined __stdcall SpriteSystemAddPosition(uint param_1, ushort param_2, float param_3, float param_4, float param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemAddPosition(unsigned int, unsigned short, float, float,
   float) */

float AbyssEngine::PaintCanvas::SpriteSystemAddPosition
                (uint param_1,ushort param_2,float param_3,float param_4,float param_5)

{
  ushort *puVar1;
  float *pfVar2;
  uint in_r2;
  float in_r3;
  float in_stack_00000000;
  float in_stack_00000004;
  
  if (((uint)param_2 < *(uint *)(param_1 + 0x180)) &&
     (puVar1 = *(ushort **)(*(int *)(param_1 + 0x184) + (uint)param_2 * 4), puVar1 != (ushort *)0x0)
     ) {
    if (*puVar1 <= in_r2) {
      return param_3;
    }
    pfVar2 = (float *)(*(int *)(puVar1 + 2) + in_r2 * 0xc);
    param_3 = pfVar2[2] + in_stack_00000004;
    *pfVar2 = *pfVar2 + in_r3;
    pfVar2[1] = pfVar2[1] + in_stack_00000000;
    pfVar2[2] = param_3;
  }
  return param_3;
}

```

## target disasm
```
  0008aa14: ldr.w r12,[r0,#0x180]
  0008aa18: cmp r12,r1
  0008aa1a: bls 0x0008aa68
  0008aa1c: ldr.w r0,[r0,#0x184]
  0008aa20: ldr.w r0,[r0,r1,lsl #0x2]
  0008aa24: cbz r0,0x0008aa68
  0008aa26: ldrh r1,[r0,#0x0]
  0008aa28: cmp r1,r2
  0008aa2a: it ls
  0008aa2c: bx.ls lr
  0008aa2e: ldr r0,[r0,#0x4]
  0008aa30: add.w r1,r2,r2, lsl #0x1
  0008aa34: vldr.32 s0,[sp,#0x4]
  0008aa38: vmov s4,r3
  0008aa3c: add.w r0,r0,r1, lsl #0x2
  0008aa40: vldr.32 s2,[sp]
  0008aa44: vldr.32 s6,[r0]
  0008aa48: vldr.32 s8,[r0,#0x4]
  0008aa4c: vldr.32 s10,[r0,#0x8]
  0008aa50: vadd.f32 s4,s6,s4
  0008aa54: vadd.f32 s2,s8,s2
  0008aa58: vadd.f32 s0,s10,s0
  0008aa5c: vstr.32 s4,[r0]
  0008aa60: vstr.32 s2,[r0,#0x4]
  0008aa64: vstr.32 s0,[r0,#0x8]
  0008aa68: bx lr
```
