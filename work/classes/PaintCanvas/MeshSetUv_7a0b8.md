# PaintCanvas::MeshSetUv
elf 0x7a0b8 body 64
Sig: undefined __stdcall MeshSetUv(uint param_1, ushort param_2, float param_3, float param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshSetUv(unsigned int, unsigned short, float, float) */

float AbyssEngine::PaintCanvas::MeshSetUv(uint param_1,ushort param_2,float param_3,float param_4)

{
  int iVar1;
  undefined4 *puVar2;
  char *pcVar3;
  uint in_r2;
  undefined4 in_r3;
  float in_stack_00000000;
  
  if ((uint)param_2 < *(uint *)(param_1 + 0x24)) {
    iVar1 = *(int *)(*(int *)(param_1 + 0x28) + (uint)param_2 * 4);
    if (*(ushort *)(iVar1 + 2) <= in_r2) {
      return param_3;
    }
    puVar2 = (undefined4 *)(*(int *)(iVar1 + 8) + in_r2 * 8);
    pcVar3 = *(char **)(DAT_0008a0f8 + 0x8a0da);
    *puVar2 = in_r3;
    if (*pcVar3 != '\0') {
      in_stack_00000000 = 1.0 - in_stack_00000000;
    }
    puVar2[1] = in_stack_00000000;
    param_3 = in_stack_00000000;
  }
  return param_3;
}

```

## target disasm
```
  0008a0b8: ldr.w r12,[r0,#0x24]
  0008a0bc: cmp r12,r1
  0008a0be: bls 0x0008a0f6
  0008a0c0: ldr r0,[r0,#0x28]
  0008a0c2: ldr.w r0,[r0,r1,lsl #0x2]
  0008a0c6: ldrh r1,[r0,#0x2]
  0008a0c8: cmp r1,r2
  0008a0ca: it ls
  0008a0cc: bx.ls lr
  0008a0ce: ldr r1,[0x0008a0f8]
  0008a0d0: vmov s2,r3
  0008a0d4: ldr r0,[r0,#0x8]
  0008a0d6: add r1,pc
  0008a0d8: vldr.32 s0,[sp]
  0008a0dc: add.w r0,r0,r2, lsl #0x3
  0008a0e0: ldr r1,[r1,#0x0]
  0008a0e2: vstr.32 s2,[r0]
  0008a0e6: ldrb r1,[r1,#0x0]
  0008a0e8: cbz r1,0x0008a0f2
  0008a0ea: vmov.f32 s2,0x3f800000
  0008a0ee: vsub.f32 s0,s2,s0
  0008a0f2: vstr.32 s0,[r0,#0x4]
  0008a0f6: bx lr
```
