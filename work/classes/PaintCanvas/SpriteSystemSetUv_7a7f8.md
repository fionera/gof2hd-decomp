# PaintCanvas::SpriteSystemSetUv
elf 0x7a7f8 body 72
Sig: undefined __stdcall SpriteSystemSetUv(uint param_1, ushort param_2, float param_3, float param_4, float param_5, float param_6)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemSetUv(unsigned int, unsigned short, float, float, float,
   float) */

void AbyssEngine::PaintCanvas::SpriteSystemSetUv
               (uint param_1,ushort param_2,float param_3,float param_4,float param_5,float param_6)

{
  ushort *puVar1;
  uint in_r2;
  SpriteSystem *in_r3;
  float in_stack_00000004;
  float in_stack_00000008;
  
  if (((uint)param_2 < *(uint *)(param_1 + 0x180)) &&
     (puVar1 = *(ushort **)(*(int *)(param_1 + 0x184) + (uint)param_2 * 4), puVar1 != (ushort *)0x0)
     ) {
    if (*puVar1 <= in_r2) {
      return;
    }
    AbyssEngine::SpriteSystemSetUv
              ((ushort)in_r2,in_stack_00000008,param_4,in_stack_00000004,param_6,in_r3);
    return;
  }
  return;
}

```

## target disasm
```
  0008a7f8: ldr.w r12,[r0,#0x180]
  0008a7fc: cmp r12,r1
  0008a7fe: bls 0x0008a83e
  0008a800: ldr.w r0,[r0,#0x184]
  0008a804: ldr.w r0,[r0,r1,lsl #0x2]
  0008a808: cbz r0,0x0008a83e
  0008a80a: ldrh r1,[r0,#0x0]
  0008a80c: cmp r1,r2
  0008a80e: it ls
  0008a810: bx.ls lr
  0008a812: push {r5,r6,r7,lr}
  0008a814: add r7,sp,#0x8
  0008a816: vldr.32 s2,[r7,#0xc]
  0008a81a: mov r1,r3
  0008a81c: vldr.32 s4,[r7,#0x8]
  0008a820: vmov lr,s2
  0008a824: vldr.32 s0,[r7,#0x10]
  0008a828: vmov r12,s4
  0008a82c: str r0,[sp,#0x4]
  0008a82e: mov r0,r2
  0008a830: vstr.32 s0,[sp]
  0008a834: mov r3,lr
  0008a836: mov r2,r12
  0008a838: blx 0x0006fcd0
  0008a83c: pop {r2,r3,r7,pc}
  0008a83e: bx lr
```
