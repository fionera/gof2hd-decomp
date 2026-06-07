# PaintCanvas::MeshSetPoint
elf 0x7a02a body 58
Sig: undefined __stdcall MeshSetPoint(uint param_1, ushort param_2, float param_3, float param_4, float param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::MeshSetPoint(unsigned int, unsigned short, float, float, float) */

float AbyssEngine::PaintCanvas::MeshSetPoint
                (uint param_1,ushort param_2,float param_3,float param_4,float param_5)

{
  int iVar1;
  undefined4 *puVar2;
  uint in_r2;
  undefined4 in_r3;
  undefined4 in_stack_00000000;
  float in_stack_00000004;
  
  if ((uint)param_2 < *(uint *)(param_1 + 0x24)) {
    iVar1 = *(int *)(*(int *)(param_1 + 0x28) + (uint)param_2 * 4);
    if (*(ushort *)(iVar1 + 2) <= in_r2) {
      return param_3;
    }
    puVar2 = (undefined4 *)(*(int *)(iVar1 + 4) + in_r2 * 0xc);
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
  0008a02a: ldr.w r12,[r0,#0x24]
  0008a02e: cmp r12,r1
  0008a030: bls 0x0008a062
  0008a032: ldr r0,[r0,#0x28]
  0008a034: ldr.w r0,[r0,r1,lsl #0x2]
  0008a038: ldrh r1,[r0,#0x2]
  0008a03a: cmp r1,r2
  0008a03c: it ls
  0008a03e: bx.ls lr
  0008a040: ldr r0,[r0,#0x4]
  0008a042: add.w r1,r2,r2, lsl #0x1
  0008a046: vldr.32 s0,[sp,#0x4]
  0008a04a: vmov s4,r3
  0008a04e: add.w r0,r0,r1, lsl #0x2
  0008a052: vldr.32 s2,[sp]
  0008a056: vstr.32 s4,[r0]
  0008a05a: vstr.32 s2,[r0,#0x4]
  0008a05e: vstr.32 s0,[r0,#0x8]
  0008a062: bx lr
```
