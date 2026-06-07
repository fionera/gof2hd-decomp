# PaintCanvas::SpriteSystemSetRGBA
elf 0x7a840 body 72
Sig: undefined __stdcall SpriteSystemSetRGBA(uint param_1, ushort param_2, float param_3, float param_4, float param_5, float param_6)

## decompile
```c

/* AbyssEngine::PaintCanvas::SpriteSystemSetRGBA(unsigned int, unsigned short, float, float, float,
   float) */

void AbyssEngine::PaintCanvas::SpriteSystemSetRGBA
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
    AbyssEngine::SpriteSystemSetRGBA
              ((ushort)in_r2,in_stack_00000008,param_4,in_stack_00000004,param_6,in_r3);
    return;
  }
  return;
}

```

## target disasm
```
  0008a840: ldr.w r12,[r0,#0x180]
  0008a844: cmp r12,r1
  0008a846: bls 0x0008a886
  0008a848: ldr.w r0,[r0,#0x184]
  0008a84c: ldr.w r0,[r0,r1,lsl #0x2]
  0008a850: cbz r0,0x0008a886
  0008a852: ldrh r1,[r0,#0x0]
  0008a854: cmp r1,r2
  0008a856: it ls
  0008a858: bx.ls lr
  0008a85a: push {r5,r6,r7,lr}
  0008a85c: add r7,sp,#0x8
  0008a85e: vldr.32 s2,[r7,#0xc]
  0008a862: mov r1,r3
  0008a864: vldr.32 s4,[r7,#0x8]
  0008a868: vmov lr,s2
  0008a86c: vldr.32 s0,[r7,#0x10]
  0008a870: vmov r12,s4
  0008a874: str r0,[sp,#0x4]
  0008a876: mov r0,r2
  0008a878: vstr.32 s0,[sp]
  0008a87c: mov r3,lr
  0008a87e: mov r2,r12
  0008a880: blx 0x0006fcdc
  0008a884: pop {r2,r3,r7,pc}
  0008a886: bx lr
```
